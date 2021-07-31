
#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include "Helper_GuardianInfoUI.h"
#include <Engine/GameDataManager/GameDataManager.h>
#include "HelperManager.h"

#define DEF_LIST_MAX	(5)

CHelper_GuardianInfoUI::CHelper_GuardianInfoUI()
	: m_pListGuardInfo(NULL)
{
	setInherit(false);
}

CHelper_GuardianInfoUI::~CHelper_GuardianInfoUI()
{

}

void CHelper_GuardianInfoUI::initialize()
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

	m_pListGuardInfo = (CUIList*)findUI("list_");

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CHelper_GuardianInfoUI::CloseUI, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CHelper_GuardianInfoUI::CloseUI, this));

	// �߽��� ����.
	if (pBase = findUI("btn_fire"))
		pBase->SetCommandFUp(boost::bind(&CHelper_GuardianInfoUI::FireMyTeacher, this));

#endif // WORLD_EDITOR
}

void CHelper_GuardianInfoUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CHelper_GuardianInfoUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CHelper_GuardianInfoUI::OpenUI()
{
	setData();
	Hide(FALSE);
	UIMGR()->RearrangeOrder(UI_HELPER_GUARDIAN_INFO, TRUE);
}

void CHelper_GuardianInfoUI::CloseUI()
{
	Hide(TRUE);
	UIMGR()->RearrangeOrder(UI_HELPER_GUARDIAN_INFO, FALSE);
}

// �߽��� ����.
void CHelper_GuardianInfoUI::FireMyTeacher( )
{
	if (_pNetwork->MyCharacterInfo.lTeacherIndex < 0)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );

	// Create message box of guild destroy
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1152, "�߽��� ����" ), UMBS_OKCANCEL, UI_HELPER_GUARDIAN_INFO, MSGCMD_HELPER_FIRE_MYTEACHER );		
	strMessage.PrintF( _S( 1153, "[%s]���� �߽����� �׸� �νðڽ��ϱ�? �ѹ� �׸� �ΰԵǸ� �ٽô� �İ����� ������ �� �����ϴ�." ), _pNetwork->MyCharacterInfo.strTeacherName );	
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( "" );
	MsgBoxInfo.AddString( strMessage );
	strMessage.PrintF( _S( 5067, "����Ƚ�� : %dȸ" ), 2 - _pNetwork->MyCharacterInfo.iStudentGiveUpCnt );
	MsgBoxInfo.AddString( strMessage, 0x00ffffff );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CHelper_GuardianInfoUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{	
	switch( nCommandCode )
	{
	case MSGCMD_HELPER_FIRE_MYTEACHER:		// �л��� ������ ����ħ(���� �л��� ����...)
		{
			if( !bOK )
				return;

			// ������ �޼����� ����.
			_pNetwork->TeachTeacherGiveUp( _pNetwork->MyCharacterInfo.lTeacherIndex, _pNetwork->MyCharacterInfo.strTeacherName, 
				_pNetwork->MyCharacterInfo.index, _pNetwork->MyCharacterInfo.name );
		}
		break;
	}
}

void CHelper_GuardianInfoUI::MsgBoxLCommand( int nCommandCode, int nResult )
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

void CHelper_GuardianInfoUI::setData()
{
	if (m_pListGuardInfo == NULL)
		return;

	CHelperManager::_vec_teacher_info info = GAMEDATAMGR()->GetHelperManager()->GetTeacherInfoList();
	CHelperManager::sHelperMember* member = GAMEDATAMGR()->GetHelperManager()->GetHelperMember(0);

	// �̸�, Ŭ����, �缺���� �ο�, �缺 �Ϸ� �ο�, �缺 ������
	int nIdx[DEF_LIST_MAX]	= {71, 1160, 5077, 5078, 5079};

	CUIBase* pItem = NULL;
	CUIText* pText = NULL;

	if (info.empty() == true || member == NULL)
	{
		for (int i = 0; i < DEF_LIST_MAX; ++i)
		{
			pItem = m_pListGuardInfo->GetListItem(i);

			if (pItem == NULL)
				continue;

			if (pText = (CUIText*)pItem->findUI("txt_title"))
				pText->SetText(_S(nIdx[i], ""));

			if (pText = (CUIText*)pItem->findUI("txt_info"))
				pText->SetText(CTString(""));
		}

		return;
	}

	CTString strCntTeachingStu;
	CTString strCompleteStu;
	CTString strCompleteStuPercent;
	strCntTeachingStu.PrintF( _S( 5070, "%d��"), info[0].nCntTeachingStudent);
	strCompleteStu.PrintF( _S( 5070, "%d��"), info[0].nCntCompleteStudent);

	int percent = 0;
	int nTry = info[0].nCntCompleteStudent + info[0].nCntFailStudent;
	if (nTry > 0)
		percent = int(info[0].nCntCompleteStudent / float(nTry) * 100.f + 0.5f);//�ݿø�
	strCompleteStuPercent.PrintF("%d%%", percent);

	CTString strInfo[DEF_LIST_MAX] = {_pNetwork->MyCharacterInfo.strTeacherName,
									  CJobInfo::getSingleton()->GetName(member->iJob, member->iJob2),
									  strCntTeachingStu,
									  strCompleteStu,
								  	  strCompleteStuPercent};

	for (int i = 0; i < DEF_LIST_MAX; ++i)
	{
		pItem = m_pListGuardInfo->GetListItem(i);

		if (pItem == NULL)
			continue;

		if (pText = (CUIText*)pItem->findUI("txt_title"))
			pText->SetText(_S(nIdx[i], ""));

		if (pText = (CUIText*)pItem->findUI("txt_info"))
			pText->SetText(strInfo[i]);
	}
}
