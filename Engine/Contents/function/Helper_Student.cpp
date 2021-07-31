
#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "Helper_Student.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include "HelperManager.h"

CHelper_StudentUI::CHelper_StudentUI()
	: m_pList(NULL)
	, m_strTeacherName("")
	, m_slIndex(-1)
{
	setInherit(false);
}

CHelper_StudentUI::~CHelper_StudentUI()
{

}

void CHelper_StudentUI::initialize()
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

	m_pList = (CUIList*)findUI("list_");

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CHelper_StudentUI::CloseUI, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CHelper_StudentUI::CloseUI, this));

	if (pBase = findUI("btn_refresh"))
		pBase->SetCommandFUp(boost::bind(&CHelper_StudentUI::SendRefresh, this));

	if (pBase = findUI("btn_apply"))
		pBase->SetCommandFUp(boost::bind(&CHelper_StudentUI::BeMyTeacher, this));

	if (pBase = findUI("btn_abandon"))
		pBase->SetCommandFUp(boost::bind(&CHelper_StudentUI::FireMyTeacher, this));

#endif // WORLD_EDITOR
}

void CHelper_StudentUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CHelper_StudentUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CHelper_StudentUI::OpenUI()
{
	RefreshTeacherList();

	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_HELPER_STUDENT, TRUE);
}

void CHelper_StudentUI::CloseUI()
{
	m_pList->DeleteAllListItem();

	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_HELPER_STUDENT, FALSE);
}

void CHelper_StudentUI::BeMyTeacher( )
{
	if (m_pList == NULL)
		return;

	int	iSelMember = m_pList->getCurSel();

	if (iSelMember < 0)
		return;

	CHelperManager* hm = GAMEDATAMGR()->GetHelperManager();
	CHelperManager::sHelperMember* TempMember = hm->GetHelperMember(iSelMember);

	if (TempMember == NULL)
		return;

	m_slIndex		= TempMember->lIndex;
	m_strTeacherName= TempMember->strName;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );

	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	
	MsgBoxInfo.SetMsgBoxInfo( _S( 1150, "�İ��� ��û" ), UMBS_OKCANCEL, UI_HELPER_STUDENT, MSGCMD_HELPER_BEMYTEACHER );	
	strMessage.PrintF( _S( 1151, "[%s]���� �İ������� ��û�Ͻðڽ��ϱ�?" ), TempMember->strName );			
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelper_StudentUI::FireMyTeacher( )
{
	if (_pNetwork->MyCharacterInfo.lTeacherIndex < 0)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );

	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1152, "�߽��� ����" ), UMBS_OKCANCEL, UI_HELPER_STUDENT, MSGCMD_HELPER_FIRE_MYTEACHER );		
	strMessage.PrintF( _S( 1153, "[%s]���� �߽����� �׸� �νðڽ��ϱ�? �ѹ� �׸� �ΰԵǸ� �ٽô� �İ����� ������ �� �����ϴ�." ), _pNetwork->MyCharacterInfo.strTeacherName );	
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( "" );
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( _S( 5067, "����Ƚ�� : %dȸ" ), 2 - _pNetwork->MyCharacterInfo.iStudentGiveUpCnt );
	MsgBoxInfo.AddString( strMessage, 0x00ffffff );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelper_StudentUI::RefreshTeacherList()
{
	if (m_pList == NULL)
		return;

	m_pList->DeleteAllListItem();

	CHelperManager* hm = GameDataManager::getSingleton()->GetHelperManager();
	CHelperManager::_vec_teacher_info t_info = hm->GetTeacherInfoList();
	CHelperManager::_vec_member h_list = hm->GetHelperList();

	if (t_info.empty() == true || h_list.empty() == true)
		return;

	std::sort(h_list.begin(), h_list.end());
	std::sort(t_info.begin(), t_info.end());
	COLOR		crLevel = 0xAAAAAAFF;	

	CTString strMessage;

	CUIBase* pTmp = m_pList->GetListItemTemplate();
	CUIBase* pItem = NULL;
	CUIText* pText = NULL;
	CUIImage* pImg = NULL;
	CUIImageArray* pImgArr = NULL;

	for ( int i = 0; i < h_list.size(); ++i)
	{
		m_pList->AddListItem(pTmp->Clone());
		pItem = m_pList->GetListItem(i);

		if (pItem == NULL)
			continue;

		if (pImgArr = (CUIImageArray*)pItem->findUI("img_class"))
		{
			pImgArr->SetRenderIdx(h_list[i].iJob);
			pImgArr->setTooltip(CJobInfo::getSingleton()->GetName(h_list[i].iJob,  h_list[i].iJob2 ));
		}

		if (pText = (CUIText*)pItem->findUI("txt_name"))
			pText->SetText(h_list[i].strName);

		if (pText = (CUIText*)pItem->findUI("txt_time"))
		{
			strMessage.PrintF(_S( 5069, "%d�� ~ %d��"),
				t_info[i].sbStartPlayTime, t_info[i].sbEndPlayTime );
			pText->SetText(strMessage);
		}

		if (pText = (CUIText*)pItem->findUI("txt_positivity_cnt"))
		{
			strMessage.PrintF(_S(5070, "%d��"), t_info[i].nCntCompleteStudent);
			pText->SetText(strMessage);
		}

		if (pImg = (CUIImage*)pItem->findUI("img_info"))
		{
			int nCompletePercent = 0;
			
			if ((t_info[i].nCntCompleteStudent + t_info[i].nCntFailStudent) > 0)
				nCompletePercent = int(t_info[i].nCntCompleteStudent / float(t_info[i].nCntCompleteStudent + t_info[i].nCntFailStudent) * 100 + 0.5f);//�ݿø�

			// �� ��ġ, �缺���� �ʺ���, �缺 �Ϸ� �ο�, �缺 ���� �ο�, �缺 ������
			int nStrIdx[5] = {1662, 1663, 1664, 1665, 1666};
			COLOR colText[5] = {0xE0FF40FF, DEF_UI_COLOR_WHITE,
				DEF_UI_COLOR_WHITE, DEF_UI_COLOR_WHITE, DEF_UI_COLOR_WHITE};

			int nVal[5] = {t_info[i].nFame, t_info[i].nCntTeachingStudent, 
			t_info[i].nCntCompleteStudent, t_info[i].nCntFailStudent, nCompletePercent};

			for (int j = 0; j < 5; ++j)
			{
				strMessage.PrintF(_S(nStrIdx[j], ""), nVal[j]);
				pImg->addTooltip(strMessage, colText[j]);
			}
		}
	}

	m_pList->UpdateList();
	m_pList->UpdateScroll(h_list.size());
}

void CHelper_StudentUI::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGLCMD_HELPER_REQ:
		{
			switch ( nResult ) 
			{
			case SEL_HELPER_STUDENT:		// �İ��� ����
				{
					OpenUI();									
				}
				break;
			}
		}
		break;
	}
}

void CHelper_StudentUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_BEMYTEACHER:	// �İ��� ��û ��...
		{
			if( bOK == FALSE )
				return;

			CUIManager* pUIManager = CUIManager::getSingleton();

			pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER_REQ );
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;

			MsgBoxInfo.SetMsgBoxInfo( _S( 1162, "�İ��� ��û" ), UMBS_CANCEL, UI_HELPER_STUDENT, MSGCMD_HELPER_BEMYTEACHER_REQ );	
			strMessage.PrintF( _S( 1163, "%s �Բ� �İ����� �Ǿ�޶�� �޽����� ������, ������ ��ٸ��� ���Դϴ�."  ), m_strTeacherName );	
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			_pNetwork->TeachTeacherRequest( m_slIndex, m_strTeacherName );
		}
		break;

	case MSGCMD_HELPER_BEMYTEACHER_REQ:
		// �л��� �İ��� ��û�� ����� ���.
		{	
			_pNetwork->TeachTeacherReject( TRUE, m_slIndex, m_strTeacherName );
		}
		break;

	case MSGCMD_HELPER_FIRE_MYTEACHER:		// �л��� ������ ����ħ(���� �л��� ����...)
		{
			if( !bOK )
				return;

			// ������ �޼����� ����.
			_pNetwork->TeachTeacherGiveUp( _pNetwork->MyCharacterInfo.lTeacherIndex, _pNetwork->MyCharacterInfo.strTeacherName, 
				m_slIndex, m_strTeacherName );
		}
		break;
	}
}

void CHelper_StudentUI::SendRefresh()
{
	_pNetwork->TeachRefreshTeacherList();
}
