#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "HelperUI.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include "HelperManager.h"

#include <vector>
#include <algorithm>

#define MAX_HELPER_LEVEL				(5)
#define INFO_MAX						(4)
#define	HELPER_MAX_STUDENTS				(20)		// �ִ� ���� ��û��.

class CmdHelperMouseOver : public Command
{
public:
	CmdHelperMouseOver() : m_pWnd(NULL), m_nOver(-1)	{} ;
	void setData(CHelperUI* pWnd, int nOver)	{ m_pWnd = pWnd; m_nOver = nOver;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->ChangeOverColor(m_nOver);
	}
private:
	CHelperUI* m_pWnd;
	int m_nOver;
};

class CmdHelperMouseLeave : public Command
{
public:
	CmdHelperMouseLeave() : m_pWnd(NULL), m_nLeave(-1)	{} ;
	void setData(CHelperUI* pWnd, int nLeave)	{ m_pWnd = pWnd; m_nLeave = nLeave;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->ChangeLeaveColor(m_nLeave);
	}
private:
	CHelperUI* m_pWnd;
	int m_nLeave;
};

CHelperUI::CHelperUI()
	: m_pBtnRegistTeacher(NULL)
	, m_pListDisciple(NULL)
	, m_pListInfo(NULL)
	, m_nOldMouseOverIdx(-1)
{
	setInherit(false);
}

CHelperUI::~CHelperUI()
{
	Destroy();
}

void CHelperUI::initialize()
{
#ifndef		WORLD_EDITOR
	CUIBase* pBase = NULL;
	if (pBase = findUI("base_drag"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	m_pListDisciple = (CUIList*)findUI("list_disciple");
	m_pListInfo		= (CUIList*)findUI("list_info");

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::CloseUI, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::CloseUI, this));

	if (pBase = findUI("btn_amend"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::AmendReq, this));

	if (pBase = findUI("btn_abandon"))
		pBase->SetCommandFUp(boost::bind(&CHelperUI::FireMyStudent, this));

	m_pBtnRegistTeacher = (CUIButton*)findUI("btn_enrolment");
#endif // WORLD_EDITOR
}

void CHelperUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CHelperUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CHelperUI::OpenUI( )
{
	if (GetHide() == FALSE || IsVisible() == TRUE)
		return;

	RefreshStudentList(TRUE);

	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_HELPER, TRUE);
}

static LONG g_lChaIndex				= -1;		// ���� ��û�� ���õ� �κп��� ����.
static CTString g_strTeacherName	= CTString("");

void CHelperUI::CloseUI()
{
	g_lChaIndex				= -1;
	g_strTeacherName.Clear();

	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder( UI_HELPER, FALSE );
}

void CHelperUI::SetRegistredTeacher( BOOL bRegistered )
{
	if (m_pBtnRegistTeacher == NULL)
		return;

	CTString strTemp;

#ifndef		WORLD_EDITOR
	// �İ��� ����Ʈ�� ����� �ȵǾ� ���� ���
	if (bRegistered == FALSE)
	{
		m_pBtnRegistTeacher->SetCommandFUp(boost::bind(&CHelperUI::RegisterTeacher, this));
		strTemp = _S(1136, "�İ��� ���");
	}
	else
	{
		m_pBtnRegistTeacher->SetCommandFUp(boost::bind(&CHelperUI::CancelRegisterTeacher, this));
		strTemp = _S(1137, "�İ��� ��� ���");
	}
#endif		// WORLD_EDITOR

	m_pBtnRegistTeacher->SetText(strTemp);
}

// ----------------------------------------------------------------------------
// Name : TeacherAccept()
// Desc : �İ����� �Ǿ��ְڴ��� ���.
// ----------------------------------------------------------------------------
void CHelperUI::TeacherAccept( LONG lIndex, const CTString& strStudentName )
{
	g_lChaIndex			= lIndex;
	g_strTeacherName	= strStudentName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_STUDENT_ACCEPT );

	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1154, "�İ��� ���" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_STUDENT_ACCEPT );	
	strMessage.PrintF( _S( 1155,  "[%s] ���� �İ����� �Ǿ�޶�� ��Ź�� �մϴ�. �����Ͻðڽ��ϱ�?"  ),  strStudentName );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelperUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_NOTIFY:		// ���� �߻�
		{			
		}
		break;
	case MSGCMD_HELPER_REGISTER:			// ���� ��Ͽ� �߰��մϴ�.
		{
			if( !bOK )
				return;

			CUIMsgBox_Info MsgBoxInfo;
			CTString strTemp;
			MsgBoxInfo.SetMsgBoxInfo( _S( 5071, "�İ��� �ý���"), UMBS_OKCANCEL | UMBS_INPUTBOX | UMBS_SECOND_INPUTBOX, UI_HELPER, MSGCMD_HELPER_PLAYTIME, 250);	
			MsgBoxInfo.AddString( _S( 5073, "�ַ� �����ϴ� �ð��� �Է����ּ���.") );
			MsgBoxInfo.AddString( _S( 5074, "(�߽����� �İ��� ������ �־� ���� �˴ϴ�.)") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddString( _s(" ") );
			MsgBoxInfo.AddStringEx( _s("h ~ "), 5, 15 );
			MsgBoxInfo.AddStringEx( _s("h"), 5, 30 );
			MsgBoxInfo.SetInputBox( 5, 5, 2, 50 );
			MsgBoxInfo.SetSEInputBox( 5 , 20, 2, 50 );
			MsgBoxInfo.m_nInputPosY += 4;
			MsgBoxInfo.m_nSEInputPosY += 4;
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSGCMD_HELPER_CANCEL_REGISTER:		// ���� ��Ͽ��� ����
		{
			if( !bOK )
				return;
			// ������ �޼����� ����.
			_pNetwork->TeachTeacherCancelRegister();
		}
		break;	

	case MSGCMD_HELPER_STUDENT_ACCEPT:
		// �л��� �İ��� ��û�� ��û�� ���¿���...(���� ������ ��������...)
		// �㰡��? ��ҳ�?
		{
			if( !bOK )		// �л��� �ȹ޾� ����
			{
				_pNetwork->TeachTeacherReject( FALSE, g_lChaIndex, g_strTeacherName );
			}
			else			// �л� �޾�~!!!
			{
				_pNetwork->TeachTeacherAccept( _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name, 
					g_lChaIndex, g_strTeacherName );
			}			
		}
		break;
	case MSGCMD_HELPER_FIRE_MYSTUDENT:
		{	// ������ �л� ����
			if ( !bOK )
				return;
			int nPos = m_pListDisciple->getCurSel();

			if (nPos < 0)
				return;

			CHelperManager::sHelperMember* member = GameDataManager::getSingleton()->GetHelperManager()->GetHelperMember(nPos);
			int nIndex = member->lIndex;
			CTString strStudentName = member->strName;
			_pNetwork->TeachTeacherGiveUp(nIndex, strStudentName, _pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name);
		}
		break;
	}
}

void CHelperUI::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

void CHelperUI::RegisterTeacher( )					// �İ��� ���
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1164, "�İ��� ���" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_REGISTER );	
	strMessage.PrintF( _S( 1165, "�İ��� ��Ͽ� ����Ͻðڽ��ϱ�?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelperUI::CancelRegisterTeacher( )			// �İ��� ��� ���
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1166, "�İ��� ��� ���" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_CANCEL_REGISTER );	
	strMessage.PrintF( _S( 1167, "�İ��� ��Ͽ��� �����Ͻðڽ��ϱ�?"  ) );	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc : Edit Box 2���� ��,
// ----------------------------------------------------------------------------
void CHelperUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput, CTString &strSEInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_PLAYTIME:
		{
			if ( bOK )
			{
				// �İ��� ��û �޽��� ������ : Client -> Server
				CTString strMessage = CTString("");
				CUIMsgBox_Info MsgBoxInfo;
				int sbStartPlayTime;
				int sbEndPlayTime;

				if ( strInput.IsInteger() && strSEInput.IsInteger() )
				{
					sbStartPlayTime = atoi(strInput);
					sbEndPlayTime = atoi(strSEInput);

					if ( sbStartPlayTime > 24 || sbEndPlayTime > 24 )
					{
						strMessage = _S( 5080, "24 ������ ���ڸ� �Է����ּ���.");
					}
					else if ( sbStartPlayTime < 0 || sbEndPlayTime < 0 )
					{
						strMessage = _S( 4348, "0 �̻��� ���ڸ� �Է��� �ֽñ� �ٶ��ϴ�.");
					}
				}
				else
				{
					strMessage = _S(4351, "���ڸ� �Է��� �ֽñ� �ٶ��ϴ�.");
				}

				if (strMessage.Length() > 0)
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 5071, "�İ��� �ý���"), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( strMessage );
					CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
					return;
				}

				_pNetwork->TeachTeacherRegister( sbStartPlayTime, sbEndPlayTime );
			}
		}
	}
}

// ----------------------------------------------------------------------------
// Name : FireMyStudent()
// Desc : �İ����� �߽��� ����
// ----------------------------------------------------------------------------
void CHelperUI::FireMyStudent()
{
	int nPos = m_pListDisciple->getCurSel();
	if ( nPos >= 0 )
	{
		CHelperManager* hm = GameDataManager::getSingleton()->GetHelperManager();
		CHelperManager::sHelperMember* pData = hm->GetHelperMember(nPos);

		CTString	strStudentName = pData->strName;
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 5068, "�İ��� ����" ), UMBS_OKCANCEL, UI_HELPER, MSGCMD_HELPER_FIRE_MYSTUDENT );		
		strMessage.PrintF( _S( 5081, "[%s]���� �İ����� �׸� �νðڽ��ϱ�? �׸� �ΰԵǸ� ��ġ 5�� �Ұ� �˴ϴ�." ), strStudentName );	
		MsgBoxInfo.AddString( strMessage );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );	
	}
}

// ----------------------------------------------------------------------------
// Name : SetMyTeachInfo()
// Desc : �� ���� ����
// ----------------------------------------------------------------------------
void CHelperUI::SetMyTeachInfo( SLONG CurTeachCnt, SLONG CompleteTeachCnt, SLONG FailTeachCnt )
{
	if (m_pListInfo == NULL)
		return;

	if (m_pListInfo->GetItemCount() < INFO_MAX)
		return;

	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CTString strTmp;
	int i;
	// �缺���� �ο�, �缺 �Ϸ� �ο�, �缺 ���� �ο�, �缺 ������
	int nStr_idx[INFO_MAX] = {5077, 5078, 5082, 5079};
	// �缺 ������
	int nCompletePercent = 0;

	if (CompleteTeachCnt + FailTeachCnt > 0)
		nCompletePercent = int(CompleteTeachCnt / float(CompleteTeachCnt + FailTeachCnt) * 100 + 0.5f);

	int nCnt[INFO_MAX] = {CurTeachCnt, CompleteTeachCnt, FailTeachCnt, nCompletePercent};

	for (i = 0; i < INFO_MAX; ++i)
	{
		pItem = m_pListInfo->GetListItem(i);

		if (pItem == NULL)
			continue;

		if (pText = (CUIText*)pItem->findUI("txt_title"))
		{
			pText->SetText(_S(nStr_idx[i], " "));
		}

		if (pText = (CUIText*)pItem->findUI("txt_cnt"))
		{
			// �������� �ۼ�Ƽ���� �Է�.
			if (i == 3)
				strTmp.PrintF("%d%%", nCnt[i]);
			else
				strTmp.PrintF("%d", nCnt[i]);

			pText->SetText(strTmp);
		}

	}
}

// ----------------------------------------------------------------------------
// Name : RefreshStudentList()
// Desc : ����� �����մϴ�.
// ----------------------------------------------------------------------------
void CHelperUI::RefreshStudentList( BOOL bInit )
{
	if( !bInit && GetHide() == TRUE )
		return;

	m_pListDisciple->DeleteAllListItem();
	CHelperManager* hm = GameDataManager::getSingleton()->GetHelperManager();
	CHelperManager::_vec_member list = hm->GetHelperList();
	std::sort(list.begin(), list.end());

	COLOR		crLevel = 0xAAAAAAFF;
	CTString strLevel;
	CUIBase* pTmp = m_pListDisciple->GetListItemTemplate();
	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CUIImageArray* pImgArr = NULL;

	CHelperManager::_vec_member::const_iterator member_end = list.end();
	CHelperManager::_vec_member::iterator it = list.begin();
	int i = 0;

	for(it = list.begin(); it != member_end; ++it)
	{		
		if((*it).iJob>=0 && (*it).iJob<TOTAL_JOB)
		{
			const int iJob		= (*it).iJob;
			const int iJob2		= (*it).iJob2;
			const int iLevel	= (*it).iLevel;
			strLevel.PrintF( "%d", iLevel );

			m_pListDisciple->AddListItem(pTmp->Clone());
			pItem = m_pListDisciple->GetListItem(i);
			
			if (pItem == NULL)
				continue;

			if (pImgArr = (CUIImageArray*)pItem->findUI("img_class"))
			{
				pImgArr->SetRenderIdx(iJob);
				pImgArr->setTooltip(CJobInfo::getSingleton()->GetName(iJob,  iJob2 ));
			}
			
			if (pText = (CUIText*)pItem->findUI("txt_name"))
			{
				pText->SetText((*it).strName);
				pText->setFontColor(0xffcc66FF);
			}

			if (pText = (CUIText*)pItem->findUI("txt_lv"))
			{
				pText->SetText(strLevel);
				pText->setFontColor(0xffcc66FF);
			}

			CmdHelperMouseOver* pCmdOver = new CmdHelperMouseOver();
			pCmdOver->setData(this, i);
			pItem->SetCommandEnter(pCmdOver);

			CmdHelperMouseLeave* pCmdLeave = new CmdHelperMouseLeave();
			pCmdLeave->setData(this, i);
			pItem->SetCommandLeave(pCmdLeave);

			++i;
		} 
		else 
			ASSERTALWAYS("JOB NUMBER must have this range [iJob>=0 && iJob<TOTAL_JOB]!!");
	}

	m_pListDisciple->UpdateList();
	m_pListDisciple->UpdateScroll(m_pListDisciple->getListItemCount());
}

void CHelperUI::AmendReq()
{
	_pNetwork->TeachGiftReq();
}

void CHelperUI::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_HELPER_REQ:
		{
			switch ( nResult ) 
			{
			case SEL_HELPER_TEACHER:		// �߽��� ����
				{
					OpenUI();
				}
				break;
			}
		}
		break;
	}
}

void CHelperUI::ChangeOverColor(int idx)
{
	if (idx < 0)
		return;

	CUIBase* pTemp = NULL;
	CUIText* pText = NULL;

	pTemp = m_pListDisciple->GetListItem(idx);
	if (pTemp != NULL)
	{
		if (pText = (CUIText*)pTemp->findUI("txt_name"))
			pText->setFontColor(0xF8E1B5FF);

		if (pText = (CUIText*)pTemp->findUI("txt_lv"))
			pText->setFontColor(0xF8E1B5FF);
	}
}

void CHelperUI::ChangeLeaveColor( int idx )
{
	if (idx < 0)
		return;

	CUIBase* pTemp = NULL;
	CUIText* pText = NULL;

	pTemp = m_pListDisciple->GetListItem(idx);
	if (pTemp != NULL)
	{
		if (pText = (CUIText*)pTemp->findUI("txt_name"))
			pText->setFontColor(0xffcc66FF);

		if (pText = (CUIText*)pTemp->findUI("txt_lv"))
			pText->setFontColor(0xffcc66FF);
	}
}
