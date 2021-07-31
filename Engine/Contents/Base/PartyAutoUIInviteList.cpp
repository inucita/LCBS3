#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "CommunityDefines.h"
#include <Engine/Interface/UIPartyAuto.h>
#include "PartyAutoUIInviteList.h"
#include <Engine/Help/Util_Help.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>

class CmdPAInviteBtnCallback : public Command
{
public:
	CmdPAInviteBtnCallback() : _ui(NULL), nCmd(0) {}
	void setData(CUIPartyAutoInviteList* pUI, int cmd_idx) {
		_ui = pUI;	nCmd = cmd_idx;
	}

	void execute() {
		if (_ui != NULL)
			_ui->SendPartyReq(nCmd);
	}

private:
	CUIPartyAutoInviteList*		_ui;
	int nCmd;
};


CUIPartyAutoInviteList::CUIPartyAutoInviteList()
	: m_pList(NULL)
	, m_pImgLevel(NULL)
	, m_nCurrentPageNum(0)
	, m_pBtnPrev(NULL)
	, m_pBtnNext(NULL)
	, m_pCbClass(NULL)
	, m_pChkLimit(NULL)
	, m_pCbType(NULL)
{
	setInherit(false);

	int		i;

	for (i = 0; i < TOTAL_JOB; ++i)
		m_pImgClass[i] = NULL;


	for (i = 0; i < PT_MAX; ++i)
		m_pImgType[i] = NULL;
}

void CUIPartyAutoInviteList::initialize()
{
	int		i;

	CUIText* pTxt = (CUIText*)findUI("txt_title");

	if (pTxt != NULL)
	{
		int l, t, r, b;
		l = pTxt->GetPosX();
		t = pTxt->GetPosY();
		r = l + pTxt->GetWidth();
		b = t + pTxt->GetHeight();
		setTitleRect(l, t, r, b);
	}

	m_pList = (CUIList*)findUI("list_invite");

	if (m_pList != NULL)
	{
		m_pList->SetSelectedForeRender(true);
		m_pList->DeleteAllListItem();
	}

	{
		// class ���
		const char* arr_class_name[] = {
			"img_titan",
			"img_knight",
			"img_healer",
			"img_mage",
			"img_rogue",
			"img_sorcerer",
			"img_nightshadow",
			"img_exrogue",
			"img_archmage",
		};

		for (i = 0; i < TOTAL_JOB; ++i)
			m_pImgClass[i] = (CUIImage*)findUI(arr_class_name[i]);
	}

	{
		const char* arr_paryy_type[] = {
			"img_type_peaceever",
			"img_type_survival",
			"img_type_attack"
		};

		for (i = 0; i < PT_MAX; ++i)
			m_pImgType[i] = (CUIImage*)findUI(arr_paryy_type[i]);
	}

	m_pBtnPrev = (CUIButton*)findUI("btn_prev");

	if (m_pBtnPrev != NULL)
	{
		CmdPAInviteBtnCallback* pCmd = new CmdPAInviteBtnCallback;
		pCmd->setData(this, -1);
		m_pBtnPrev->SetCommandUp(pCmd);
	}

	m_pBtnNext = (CUIButton*)findUI("btn_next");

	if (m_pBtnNext != NULL)
	{
		CmdPAInviteBtnCallback* pCmd = new CmdPAInviteBtnCallback;
		pCmd->setData(this, 1);
		m_pBtnNext->SetCommandUp(pCmd);
	}

	m_pImgLevel = (CUIImage*)findUI("img_leveldiff");

	m_pCbClass = (CUIComboBox*)findUI("cb_class");
	m_pChkLimit = (CUICheckButton*)findUI("chk_limitlevel");
	m_pCbType = (CUIComboBox*)findUI("cb_type");

	// ---------------------------------------------------------------------
#ifndef		WORLD_EDITOR
	CUIButton* pBtn = (CUIButton*)findUI("btn_close");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoInviteList::close, this));

	pBtn = (CUIButton*)findUI("btn_cancel");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoInviteList::close, this));

	pBtn = (CUIButton*)findUI("btn_search");

	if (pBtn != NULL)
	{
		CmdPAInviteBtnCallback* pCmd = new CmdPAInviteBtnCallback;
		pCmd->setData(this, 0);
		pBtn->SetCommandUp(pCmd);
	}

	pBtn = (CUIButton*)findUI("btn_invite");

	if (pBtn != NULL)
		pBtn->SetCommandFUp(boost::bind(&CUIPartyAutoInviteList::_invite, this));

#endif	// WORLD_EDITOR

	init_data();
}

void CUIPartyAutoInviteList::open()
{
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_PARTYAUTO_INVITE_LIST, TRUE);

	_pNetwork->SendPartyListReq(m_nCurrentPageNum, -1, 0, -1, FALSE);
}

void CUIPartyAutoInviteList::close()
{
	SetVisible(FALSE);
	Hide(TRUE);

	{
		m_nCurrentPageNum = 0;
		m_vectorPartyListData.clear();
	}

	CUIBase* pUI = CUIFocus::getSingleton()->getUI();

	if (pUI == m_pCbClass || pUI == m_pCbType)
		CUIFocus::getSingleton()->setUI(NULL);
}

void CUIPartyAutoInviteList::ReceiveMemberData(CNetworkMessage* istr)
{
    int		i;

    SLONG	slCnt;
    SLONG	slPageNum;

    (*istr) >> slPageNum;
    (*istr) >> slCnt;

    if (slPageNum < 0 || slCnt == 0)	// �߸��� ������ or ��� ����
        return;

	// ���� ������ ����
	if (m_pList != NULL)
		m_pList->DeleteAllListItem();

    m_vectorPartyListData.clear();
    m_nCurrentPageNum = slPageNum;

    for (i = 0; i < slCnt; i++)
    {
        SLONG		slIndex, slZone;
        SBYTE		sbLvdiff, sbPtType, sbJob;
        CTString	strName;

        PartyListData	TempData;

        (*istr) >> slIndex;	// ĳ���� �ĺ���(n)
        (*istr) >> strName;	// ĳ���� �̸�(str)
        (*istr) >> sbLvdiff;	// ���� ����(c)
        (*istr) >> slZone;	// ��ġ(n)
        (*istr) >> sbJob;		// ����(c)
        (*istr) >> sbPtType;	// ��Ƽ ����(c)

        TempData.nIndex = (int)slIndex;
        TempData.strSubject = strName;
        TempData.nLeveldiff = (int)sbLvdiff;
        TempData.nZone = (int)slZone;

        if (sbJob == 0)	TempData.dwJobflag = JF_TITAN;
        else if (sbJob == 1)	TempData.dwJobflag = JF_KNIGHT;
        else if (sbJob == 2)	TempData.dwJobflag = JF_HEALER;
        else if (sbJob == 3)	TempData.dwJobflag = JF_MAGE;
        else if (sbJob == 4)	TempData.dwJobflag = JF_ROGUE;
        else if (sbJob == 5)	TempData.dwJobflag = JF_SORCERER;
        else if (sbJob == 6)	TempData.dwJobflag = JF_NIGHTSHADOW;

#ifdef	CHAR_EX_ROGUE
        else if (sbJob == 7)	TempData.dwJobflag = JF_EX_ROGUE;
#endif	// CHAR_EX_ROGUE

#ifdef	CHAR_EX_MAGE
        else if (sbJob == 8)	TempData.dwJobflag = JF_EX_MAGE;
#endif	// CHAR_EX_MAGE

        TempData.nPartyType = (int)sbPtType;

        m_vectorPartyListData.push_back(TempData);
    }

    // update list
	if (m_pList != NULL)
	{
		int max = m_vectorPartyListData.size();

		CUIListItem* pItem;
		CTString	strTmp;
		CUIText*	pTxt;
		CUIImage*	pImg;

		for (i = 0; i < max; ++i)
		{
			pItem = (CUIListItem*)m_pList->GetListItemTemplate()->Clone();

			m_pList->AddListItem(pItem);

			int no = ((m_nCurrentPageNum * 10) + i + 1);

			pTxt = (CUIText*)pItem->findUI("txt_no");

			if (pTxt != NULL)
			{
				strTmp.PrintF("%d", no);
				pTxt->SetText(strTmp);
			}

			pTxt = (CUIText*)pItem->findUI("txt_name");

			if (pTxt != NULL)
			{
				pTxt->SetText(m_vectorPartyListData[i].strSubject);

				if (m_vectorPartyListData[i].nPartyNum >= 8)
				{
					pTxt->setFontColor(0xA7A9A5FF);
				}
				else
				{
					pTxt->setFontColor(DEF_UI_COLOR_WHITE);
				}
			}

			pTxt = (CUIText*)pItem->findUI("txt_pos");

			if (pTxt != NULL)
			{
				strTmp = CZoneInfo::getSingleton()->GetZoneName(m_vectorPartyListData[i].nZone);
				pTxt->SetText(strTmp);
			}

			if (m_vectorPartyListData[i].nLeveldiff == 0)
			{
				pImg = (CUIImage*)pItem->findUI("img_level");
				
				if (pImg != NULL)
				{
					if (m_pImgLevel != NULL)
						m_pImgLevel->Copy(pImg);
				}
			}

			pImg = (CUIImage*)pItem->findUI("img_class");

			if (pImg != NULL)
			{
				DWORD dwFlag = m_vectorPartyListData[i].dwJobflag;
				int job = 0;

				while (dwFlag >= 2)
				{
					dwFlag /= 2;

					++job;
				}

				if (m_pImgClass[job] != NULL)
					m_pImgClass[job]->Copy(pImg);
			}

			pImg = (CUIImage*)pItem->findUI("img_type");

			if (pImg != NULL)
			{
				int type = m_vectorPartyListData[i].nPartyType;
				if (m_pImgType[type] != NULL)
					m_pImgType[type]->Copy(pImg);
			}
		}

		m_pList->UpdateList();
	}
}

void CUIPartyAutoInviteList::ReceivePartyInviteMessage( int nErrorcode, CNetworkMessage *istr )
{
	CTString strMessage;

	close();

	switch( nErrorcode )
	{
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_OK:
		{
			SBYTE sbPartyType;
			SLONG slBoss, slDest;
			CTString strBossName, strDestName;

			(*istr) >> sbPartyType;
			(*istr) >> slBoss;
			(*istr) >> strBossName;
			(*istr) >> slDest;
			(*istr) >> strDestName;

			SLONG slMyIndex = _pNetwork->MyCharacterInfo.index; 

			CUIManager* pUIManager = CUIManager::getSingleton();

			if( slBoss == slMyIndex )
			{
				GAMEDATAMGR()->GetPartyInfo()->SetPartyRequested( TRUE );
				GAMEDATAMGR()->GetPartyInfo()->SetDestName( strDestName );
			}

			GAMEDATAMGR()->GetPartyInfo()->PartyInvite( sbPartyType, slBoss, strBossName );

			return;
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_NOT_FOUND:
		{
			strMessage = _S( 2702, "��� ��Ͽ� ���� �����Դϴ�" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_LEVEL:
		{
			strMessage = _S( 2703, "��Ƽ�� �����⿡�� ������ �������� �ʽ��ϴ�" );	
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_NOBOSS:
		{
			strMessage = _S( 2704, "��Ƽ�常�� �ʴ��� �� �ֽ��ϴ�" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_DIFF_TYPE:
		{
			strMessage = _S( 2705, "��Ƽ Ÿ���� ���� �ʽ��ϴ�" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_TARGET:
		{
			strMessage = _S( 396, "����� �̹� ��Ƽ ���Դϴ�" );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_FULL:
		{
			strMessage = _S( 2700, "��Ƽ �ο��� ���� ���ֽ��ϴ�." );
		}
		break;
	case MSG_EX_PARTY_MATCH_ERROR_INVITE_ALREADY_REQUEST:
		{
			strMessage = _S( 2708, "����� ��Ƽ ��û ���Դϴ�" );
		}
		break;
	}

	_pNetwork->ClientSystemMessage( strMessage );
}

void CUIPartyAutoInviteList::SendPartyReq(int nNum)
{
	int nSendPage;
	int nJob = 0;
	int nPtType = 0;
	int nLvLimit = 0;

	if (m_pCbClass != NULL)
		nJob = m_pCbClass->GetCurSel();

	if (m_pCbType != NULL)
	{
		nPtType = m_pCbType->GetCurSel();

		// �Լ��켱�� �������� �߰��� +1�� ��
		if (nPtType == 2)
			nPtType++;
	}

	if (m_pChkLimit != NULL)
		nLvLimit = m_pChkLimit->IsChecked();

	if (nNum != 0)
	{
		nSendPage = m_nCurrentPageNum + nNum;
	}
	else if (nNum == 0)
	{
		nSendPage = 0;
		m_vectorPartyListData.clear();

		if (m_pList != NULL)
			m_pList->DeleteAllListItem();
	}

	// -1 �������� �������� �ʴ´�.
	if (nSendPage < 0)
		return;

	_pNetwork->SendPartyListReq(nSendPage, nJob - 1, nLvLimit, nPtType - 1, FALSE);
}

// --------------------------------------------------------------------------

void CUIPartyAutoInviteList::init_data()
{
	int		i;

	CTString strAll = _S(506, "��ü");

	if (m_pCbClass != NULL)
	{
		m_pCbClass->AddString(strAll);

		for (i = 0; i < TOTAL_JOB; ++i)
			m_pCbClass->AddString(UTIL_HELP()->GetClassName((eJob)i));
	}

	if (m_pCbType != NULL)
	{
		m_pCbType->AddString(strAll);
		m_pCbType->AddString(_S(3354, "�Ϲ�"));
		m_pCbType->AddString(_S(2665, "������"));
	}
}

void CUIPartyAutoInviteList::_invite()
{
    if (m_pList != NULL)
    {
        int nSel = m_pList->getCurSel();

        if (nSel < 0 || nSel >= m_vectorPartyListData.size())
        {
			CTString strMessage = _S(2693, "��Ƽ�� �ʴ��� ����� �������� �ʾҽ��ϴ�");
			_pNetwork->ClientSystemMessage(strMessage);
		}
		else
		{
            if (m_vectorPartyListData.empty() == false)
                _pNetwork->SendPartyInviteReq(m_vectorPartyListData[nSel].nIndex);
        }
    }
}

