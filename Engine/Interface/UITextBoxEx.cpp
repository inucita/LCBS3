
#include "StdH.h"

#include "UITextBox.h"
#include "UITextBoxEx.h"

#include "UITextEx.h"

#include "UIManager.h"
#include "UITextureManager.h"

#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UICommand.h>

extern CFontData *_pfdDefaultFont;
extern INDEX g_iCountry;

enum eTAG_VAL_COL
{
	eTVC_STR = 0,
	eTVC_COL,
};


enum eTAG_VAL_CMD
{
	eTVCMD_STR = 0,
	eTVCMD_CMD,
	eTVCMD_INDEX,
	eTVCMD_COL_IDLE,
	eTVCMD_COL_ON,
	eTVCMD_COL_CLICK,
	eTVCMD_COL_DISABLE,
};

CUITextBoxEx::CUITextBoxEx()
	: m_eSplitType(SPLIT_SPACE)
	, m_pScroll(NULL)
	, m_nStartX(0)
	, m_nStartY(0)
	, m_nGapY(0)
	, m_colBase(DEF_UI_COLOR_WHITE)
	, m_pBtnCmd(NULL)
	, m_eControlType(eCT_TEXT)
	, m_nScrollThumbWidth(0)
	, m_nShowCount(0)
{
	m_colBtn[UBS_IDLE]		= DEF_UI_COLOR_WHITE;
	m_colBtn[UBS_ON]		= DEF_UI_COLOR_BLUE;
	m_colBtn[UBS_CLICK]		= DEF_UI_COLOR_BLUE;
	m_colBtn[UBS_DISABLE]	= DEF_UI_COLOR_WHITE;

	setType(eUI_CONTROL_TEXTBOX_EX);
	deleteCont();
}

CUITextBoxEx::~CUITextBoxEx()
{
	deleteCont();

	Destroy();
}

void CUITextBoxEx::AddText( std::string& str, DWORD dwCol /*= DEF_UI_COLOR_WHITE*/ )
{
	if (str.empty() == true)
		return;

	int nFind = str.find("\r\n");

	while (nFind != std::string::npos)
	{
		str.replace(nFind, 2, "");
		nFind = str.find("\r\n");
	}

	nFind = str.find("\n");

	while (nFind != std::string::npos)
	{
		str.replace(nFind, 1, "");
		nFind = str.find("\n");
	}

	nFind = str.find("\t");

	while (nFind != std::string::npos)
	{
		str.replace(nFind, 1, "");
		nFind = str.find("\t");
	}

	m_strOrigin += str;
	m_colBase = dwCol;
	m_eControlType = eCT_TEXT;
	split(str);
}

void CUITextBoxEx::AddBtn( std::string& str, Command* pCmd, 
	DWORD dwColIdle /*= DEF_UI_COLOR_WHITE*/, 
	DWORD dwColOver /*= DEF_UI_COLOR_BLUE*/, 
	DWORD dwColClick /*= DEF_UI_COLOR_BLUE*/, 
	DWORD dwColDisable /*= DEF_UI_COLOR_WHITE*/ )
{
	if (str.empty() == true)
		return;

	m_strOrigin += str;
	m_colBtn[UBS_IDLE] = dwColIdle;
	m_colBtn[UBS_ON] = dwColOver;
	m_colBtn[UBS_CLICK] = dwColClick;
	m_colBtn[UBS_DISABLE] = dwColDisable;
	m_pBtnCmd = pCmd;
	m_eControlType = eCT_BTN;
	split(str);
}

void CUITextBoxEx::split(std::string str)
{
	std::string strSyntax = str;
	std::string strTagID;
	std::string strText;

	int nBoxWidth = GetWidth() - m_nStartX;

	// ��ũ���� ����Ǿ� �ִٸ� ��ũ�ѹ� width��ŭ ���ش�.
	// UIFactory���� SetScroll()���� split�� ���� ȣ�� �Ǳ� ������ 
	// ������ �� ���� �ϵ��� ����.
	nBoxWidth -= m_nScrollThumbWidth;

	if (nBoxWidth <= 0)
		return;	

	if (m_vecUICont.size() <= 0)
		AddUI();

	while (str.empty() == false)
	{
		strSyntax = parseSyntax(str, strTagID);
		str = str.substr(strSyntax.length(), str.length() - strSyntax.length());

		vecValcont val;

		if (strTagID.empty() == false)
			getValue(strSyntax, strTagID, val, strText);
		else
			strText = strSyntax;

		switch(m_eSplitType)
		{
		case SPLIT_NONE:
			splitNone(strText, val, strTagID, nBoxWidth);
			break;
		case SPLIT_SPACE:
			splitSpace(strText, val, strTagID, nBoxWidth);
			break;
		case SPLIT_ONELINE:
			splitOneLine(strText, val, strTagID);
			break;
		}
	}
}

bool CUITextBoxEx::calcSplit( std::string& str, int& nCurCnt, int& nSplitCnt)
{
	std::string strTmp;
	int len = str.size();
	int pos;
	bool bEnterChar = false;
	bool bDBSC = false;

	if (g_iCountry == KOREA || g_iCountry == THAILAND)
		bDBSC = true;

	pos = str.find("\\n", nCurCnt);

	if (pos != std::string::npos)
	{
		nSplitCnt = pos + 2;
		bEnterChar = true;
	}

	// �ѱ����� �˻�.
	if (bDBSC == true)
	{
		if ((nCurCnt + nSplitCnt) < len && IsDBCSLeadByte(str[nCurCnt + nSplitCnt]))
		{
			bool bLead = false;

			for (int i = nCurCnt; i <= (nCurCnt + nSplitCnt); ++i)
			{
				if (IsDBCSLeadByte(str[i]))
				{
					bLead = true;
					++i;

					if (i > (nCurCnt + nSplitCnt))
					{
						bLead = false;
					}
				}
				else
				{
					bLead = false;
				}
			}

			if (bLead == true)
				--nSplitCnt;
		}
	}

	if ((nCurCnt + nSplitCnt) > len)
		nSplitCnt = len - nCurCnt;

	return bEnterChar;
}

void CUITextBoxEx::splitNone( std::string& strAdd, vecValcont& val, std::string& strTagID, int nTextArea )
{
	int nCurPos = 0;
	int nEndPos;
	int nCurUIPos = 0;
	bool bEnterChar;

	std::string strTemp = strAdd;
	CUITextEx* pCurUI = NULL;

	while (1)
	{
		std::string strCurLine;

		nCurUIPos = m_vecUICont.size() - 1;
		pCurUI = (CUITextEx*)m_vecUICont[nCurUIPos];

		if (pCurUI == NULL)
			break;

		strCurLine = pCurUI->GetOriginString();

		if (checkWidth(strCurLine, strAdd, nTextArea, nEndPos) == false)
			AddUI();

		bEnterChar = calcSplit(strAdd, nCurPos, nEndPos);
		strTemp = strAdd.substr(nCurPos, nEndPos);

		// ���ڿ��� �ٹٲ��� ���� ���
		if (bEnterChar == true)
			strTemp.replace(strTemp.size() - 2, 0, "");

		strAdd = strAdd.substr(nEndPos, strAdd.length() - nEndPos);

		createControl(strTagID, pCurUI, val, strTemp);

		if (bEnterChar == true)
			AddUI();

		if (strAdd.size() <= 0)
			break;
	}
}

void CUITextBoxEx::splitSpace( std::string& strAdd, vecValcont& val, std::string& strTagID, int nTextArea )
{
	int nCurPos = 0;
	int nCurUIPos = 0;
	int nEndPos;
	int	lastSpace = 0;
	bool bEnterChar;

	std::string strTemp = strAdd;
	CUITextEx* pCurUI = NULL;

	while (1)
	{
		std::string strCurLine;

		nCurUIPos = m_vecUICont.size() - 1;
		pCurUI = (CUITextEx*)m_vecUICont[nCurUIPos];

		if (pCurUI == NULL)
			break;

		strCurLine = pCurUI->GetOriginString();

		if (checkWidth(strCurLine, strAdd, nTextArea, nEndPos) == false)
		{
			AddUI();

			if (checkSpace(strAdd, nTextArea, nEndPos) == false)
				continue;
		}

		bEnterChar = calcSplit(strAdd, nCurPos, nEndPos);
		strTemp = strAdd.substr(nCurPos, nEndPos);

		// ���ڿ��� �ٹٲ��� ���� ���
		if (bEnterChar == true)
			strTemp.replace(nEndPos - 2, 2, "");

		strAdd = strAdd.substr(nEndPos, strAdd.length() - nEndPos);
		createControl(strTagID, pCurUI, val, strTemp);

		if (bEnterChar == true)
			AddUI();

		if (strAdd.size() <= 0)
			break;
	}
}

void CUITextBoxEx::splitOneLine( std::string& strAdd, vecValcont& val, std::string& strTagID )
{
	int nCurUIPos = m_vecUICont.size() - 1;

	CUITextEx* pCurUI = (CUITextEx*)m_vecUICont[nCurUIPos];

	if (pCurUI == NULL)
		return;

	createControl(strTagID, pCurUI, val, strAdd);

	int nCurWidth = pCurUI->GetWidth();

	if (nCurWidth >= m_nWidth)
		m_nWidth = nCurWidth + (m_nStartX * 2);
}

std::string CUITextBoxEx::parseSyntax(std::string& src, std::string& tagID)
{
	int nStart = 0;
	int nEnd = 0;

	getTagID(src, tagID, nStart, nEnd);

	if (tagID.empty() == true || nStart > 0)
	{
		tagID = "";
		return src.substr(0, nStart);
	}
	else
	{
		return src.substr(nStart, nEnd);
	}
}

void CUITextBoxEx::getTagID( std::string& strSyntax, std::string& strID, int &start, int &end )
{
	int nEndTag = 0;

	start = strSyntax.find('<', start);

	if (start == std::string::npos)
	{
		start = strSyntax.length();
		strID = "";
		return;
	}

	// ���ۺ��� �����̽������� ���ڸ� �ױ� ���̵�� ���.
	end = strSyntax.find(' ', start);

	if (end == std::string::npos)
	{
		start = strSyntax.length();
		strID = "";
		return ;
	}

	strID = strSyntax.substr(start + 1, (end - start - 1));

	std::string strEndTag;
	char szBuff[128];

	sprintf(szBuff, "</%s>", (char*)strID.c_str());
	strEndTag = szBuff;

	end = strSyntax.find(strEndTag);

	// ���� �ױ� ã��.
	if (end == std::string::npos)
	{
		++start;
		getTagID(strSyntax, strID, start, end);
		return;
	}

	end += strEndTag.size();
}

bool CUITextBoxEx::getValue(std::string& strSyntax, std::string& strTagID, vecValcont& val, std::string& strText)
{
	if (strTagID.empty() == true)
		return false;

	char szBuff[128];
	std::string strTag;
	std::string strVal;
	
	strText = strSyntax;

	sprintf(szBuff, "<%s", (char*)strTagID.c_str());
	strTag = szBuff;

	int nValPos = strSyntax.find(strTag) + strTag.length();
	int nValEndPos = strSyntax.find('>');

	if (nValPos == std::string::npos || nValEndPos == std::string::npos)
		return false;

	sprintf(szBuff, "</%s>", (char*)strTagID.c_str());
	strTag = szBuff;

	int nCharPos = nValEndPos + 1;
	int nCharEndPos = strSyntax.find(strTag);

	if (nCharPos == std::string::npos || nCharEndPos == std::string::npos)
		return false;

	strVal = strSyntax.substr(nCharPos, (nCharEndPos - nCharPos));
	val.push_back(strVal);
	strText = strVal;

	strVal = "";
	int i = nValPos + 1;

	for (; i < nValEndPos + 1; ++i)
	{
		if (strSyntax[i] == ' ' || strSyntax[i] == '|' || strSyntax[i] == '>')
		{
			val.push_back(strVal);
			strVal = "";
			continue;
		}

		strVal += strSyntax[i];
	}

	return true;
}

void CUITextBoxEx::insertText( CUITextEx* pTextEx, vecValcont& val, std::string& str)
{
	COLOR col = m_colBase;

	if (val.size() > 0)
		col = strtoul(val[eTVC_COL].c_str(), NULL, 16);

	pTextEx->AddText(str.c_str(), col);
}

void CUITextBoxEx::insertBtn( CUITextEx* pTextEx, vecValcont& val, std::string& str )
{
	COLOR col[UBS_TOTAL] = { m_colBtn[UBS_IDLE], m_colBtn[UBS_ON], m_colBtn[UBS_CLICK], m_colBtn[UBS_DISABLE] };
	int i, nSize = val.size();

	for (i = eTVCMD_COL_IDLE; i < nSize; ++i)
	{
		col[i - eTVCMD_COL_IDLE] = strtoul(val[i].c_str(), NULL, 16);
	}
	
	int nCmd = -1;
	int nIndex = -1;
	if (nSize >= eTVCMD_INDEX)
	{
		nCmd = atoi(val[eTVCMD_CMD].c_str());
		nIndex = atoi(val[eTVCMD_INDEX].c_str());
	}

	switch((eCMD_ID)nCmd)
	{
	case eCMD_ITEMGET:
		m_pBtnCmd = new CmdItemGet(nIndex);
		break;
	case eCMD_GONPC:
		m_pBtnCmd = new CmdGoNpc(nIndex);
		break;
	}

	pTextEx->AddBtn(str.c_str(), m_pBtnCmd, col[UBS_IDLE], col[UBS_ON], col[UBS_CLICK], col[UBS_DISABLE]);
	m_pBtnCmd = NULL;
}

bool CUITextBoxEx::checkWidth( std::string& strPrev, std::string& strAdd, int nTextArea, int& nSplitPos )
{
	if (g_iCountry == RUSSIA)
	{
		std::string strNofixed;

		strNofixed = strPrev + strAdd;
		nSplitPos = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, (char*)strNofixed.c_str(), nTextArea);

		nSplitPos -= strPrev.size();

		if (strAdd.size() > nSplitPos)
			return false;

		return true;
	}

	int nFontWidth = _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();

	int nPrevTextWidth = nFontWidth * (strPrev.size());
	int nAddTextWidth = nFontWidth * (strAdd.size());
	int nTotalWidth = nPrevTextWidth + nAddTextWidth;
	nSplitPos = strAdd.size();
	
	if (nTotalWidth > nTextArea)
	{
		nSplitPos = (nTextArea - nPrevTextWidth) / nFontWidth;
		return false;
	}

	return true;
}

void CUITextBoxEx::deleteCont()
{
	vec_UICont_iter		iter = m_vecUICont.begin();
	vec_UICont_iter		eiter = m_vecUICont.end();

	for(; iter != eiter; ++iter )
	{
		deleteChild( (*iter) );
	}

	m_vecUICont.clear();
	m_strOrigin = "";
	m_colBase = DEF_UI_COLOR_WHITE;

	m_pBtnCmd = NULL;
	m_pCmd = NULL;

	if (m_pScroll != NULL)
		m_pScroll->SetScrollCurPos(0);
}

void CUITextBoxEx::erase()
{
	m_vecUICont.clear();
	m_VecChild.clear();

	m_strOrigin = "";
	m_colBase = DEF_UI_COLOR_WHITE;

	m_pBtnCmd = NULL;
	m_pCmd = NULL;
}

void CUITextBoxEx::AddUI(CUIBase* pBase /*= NULL*/)
{
	if (pBase != NULL)
	{
		m_vecUICont.push_back(pBase);
		addChild(pBase);
		return;
	}

	CUITextEx* pAddUI = new CUITextEx;
	m_vecUICont.push_back(pAddUI);
	addChild(pAddUI);

	int nCurPos = m_vecUICont.size() - 1;
	int nPosY = _pUIFontTexMgr->GetFontHeight();

	pAddUI->SetPos(m_nStartX, ((nCurPos * (nPosY + m_nGapY)) + m_nStartY));

	if (m_pScroll != NULL)
		m_pScroll->SetCurItemCount(m_vecUICont.size());
}

CUIBase* CUITextBoxEx::Clone()
{
	CUITextBoxEx* pTextBoxEx = new CUITextBoxEx(*this);

	pTextBoxEx->erase();

	vec_uinode_iter		iter = m_VecChild.begin();
	vec_uinode_iter		eiter = m_VecChild.end();

	for(; iter != eiter; ++iter )
	{
		if ((*iter)->getType() == eUI_CONTROL_SCROLL)
		{
			CUIScrollBar* pScroll = (CUIScrollBar*)(*iter)->Clone();
			pTextBoxEx->SetScroll(pScroll);
			pTextBoxEx->addChild((CUIBase*)pScroll);
		}
		else
		{
			pTextBoxEx->AddUI((*iter)->Clone());
		}
	}

	return pTextBoxEx;
}

CUIBase* CUITextBoxEx::GetLineText( int nIdx )
{
	if (nIdx < 0 && nIdx >= m_vecUICont.size())
		return NULL;

	return m_vecUICont[nIdx];
}

bool CUITextBoxEx::checkSpace( std::string& strAdd, int nTextArea, int& nSplitCnt )
{
	int lastSpace = strAdd.rfind(0x20, nSplitCnt);
	int nNextEnd = 0;
	int nNextSpace = 0;

	// �߶� ���ڿ��� �����̽��� �ִ� ��츦 �˻�.
	if (lastSpace != std::string::npos)
	{
		std::string strCheck = strAdd.substr(lastSpace+1, strAdd.size() - lastSpace+1);

		if (checkWidth(std::string(""), strCheck, nTextArea, nNextEnd) == false)
		{
			nNextSpace = strCheck.rfind(0x20, nNextEnd);

			if (nNextSpace != std::string::npos)
				nSplitCnt = ++lastSpace;
		}
		else
		{
			nSplitCnt = ++lastSpace;
		}
	}
	else
	{
		if (checkWidth(std::string(""), strAdd, nTextArea, nNextEnd) == false)
		{
			nNextSpace = strAdd.rfind(0x20, nNextEnd);

			if (nNextSpace != std::string::npos)
				return false;
		}
	}

	return true;
}

void CUITextBoxEx::createControl( std::string& strTagID, CUITextEx* pTextEx, vecValcont& val, std::string& str )
{
	if (strcmpi(strTagID.c_str(), "col") == 0)
	{
		insertText(pTextEx, val, str);
	}
	else if (strcmpi(strTagID.c_str(), "cmd") == 0)
	{
		insertBtn(pTextEx, val, str);
	}
	else
	{
		val.clear();

		switch(m_eControlType)
		{
		case eCT_TEXT:
			insertText(pTextEx, val, str);
			break;
		case eCT_BTN:
			insertBtn(pTextEx, val, str);
			break;
		}
		
	}
}

void CUITextBoxEx::SetScroll( CUIScrollBar* pScroll )
{
	if (pScroll == NULL)
		return;

	m_pScroll = pScroll;

#ifndef		WORLD_EDITOR
	m_pScroll->SetCommandF(boost::bind(&CUITextBoxEx::UpdateBox, this));
#endif		// WORLD_EDITOR
	m_pScroll->SetCurItemCount(GetTextExCount());
}

void CUITextBoxEx::UpdateBox()
{
	if (m_pScroll == NULL)
		return;

	int nStartIdx = m_pScroll->GetScrollPos();
	int nMax = m_vecUICont.size();
	int nPosY = m_nGapY;
	int nMaxHeight = m_nHeight - m_nGapY - m_nStartY;
	int nShowCount = 0;

	CUIBase* pItem = NULL;

	for (int i = 0; i < nMax; ++i)
	{
		pItem = m_vecUICont[i];

		if (pItem == NULL)
			continue;

		if (nStartIdx > i)	// 
		{
			pItem->Hide(TRUE);
			continue;
		}
		else if (nPosY > nMaxHeight)
		{
			pItem->Hide(TRUE);
			continue;
		}

		pItem->Hide(FALSE);
		pItem->SetPosY(nPosY);

		nPosY += pItem->GetHeight() + m_nGapY;
		++nShowCount;
	}

	if (m_nShowCount != nShowCount)
	{
		m_pScroll->SetItemsPerPage(nShowCount);
		m_nShowCount = nShowCount;
	}
}
