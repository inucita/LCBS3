#include "stdh.h"

#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include "PetTrainingUI.h"
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Contents/function/PetFreeUI.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/function/PetItemMixUI.h>
#include <Engine/Interface/UIInventory.h>

#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Notice.h>
#include <Engine/Info/MyInfo.h>

enum eSelection
{
	PET_TRAINING,
	PET_CHANGE,
	PET_SKILLINIT,
	PET_TALK,
	PET_STUFF, // �� ��� ������ ��ȯ
	PET_UNIQUE_PRODUCT,  // ����ũ ������ ����
	PET_SEAL_CANCEL,	// �� ���� ����
	PET_EVOLUTION,		// �� ��ȭ
	PET_NPC_HELP,
};

enum eChangeSelection
{
	CHANGE_RIDE,						// Ż������...
	PETITEM_DESTRUCTION,				// �� ������ �Ҹ�
	DRAGON_WEAPON,						// �巡�� ����
	KNIGHT_ARMOR,						// ����Ʈ ���
};

enum
{
	eTAB_COMMAND = 0,
	eTAB_SKILL,
	eTAB_MAX
};

enum
{
	eDESIGN_NAME = 0,
	eDESIGN_ADD,
	eDESIGN_INFO,
	eDESIGN_CONTENTS,
	eDESIGN_SPACE
};

//--------------------------------------------------------------
// Select Item
class CmdPetTrainingSelectItem : public Command
{
public:
	CmdPetTrainingSelectItem() : pUI_(NULL), TabType_(-1) {}
	void setData(CPetTrainingUI* pUI, SBYTE tabType, int index)	
	{ 
		pUI_ = pUI;
		TabType_ = tabType;
		Index_ = index;
	}
	void execute() {
		if (pUI_)
			pUI_->SelectItem(Index_, TabType_);
	}
private:
	CPetTrainingUI* pUI_;
	SBYTE TabType_;
	int Index_;
};

CPetTrainingUI::CPetTrainingUI()
	: m_pMoveArea(NULL)
	, m_pstrAbilityPoint(NULL)
	, m_pDesign(NULL)
	, m_pMainTab(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
{
	m_fNpcX = 0.0f;
	m_fNpcZ = 0.0f;
	m_nNpcIndex = -1;
	int i;
	for (i = 0; i < eBTN_MAX; ++ i)
		m_pBtn[i] = NULL;
	for (i = 0; i < eLIST_MAX; ++ i)
		m_pList[i] = NULL;

	setInherit(false);
}

CPetTrainingUI::~CPetTrainingUI()
{
	SAFE_DELETE(m_pDesign);
	Destroy();
}

ENGINE_API void CPetTrainingUI::OpenPetTraining( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_PETTRAINING_REQ ) || IsVisible() )
		return;	

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	m_nNpcIndex = iMobIndex;

	CMobData* MD	= CMobData::getData(iMobIndex);	
	CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();	

	// Create skill learn message box
	pUIManager->CreateMessageBoxL( _S(2190, "�ֿϵ��� �Ʒ�" ), UI_PET_TRAINING, MSGLCMD_PETTRAINING_REQ );

	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, TRUE, _S(2191, "������ ���� ������ �����ʴϱ�?" ), -1, 0xA3A1A3FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, TRUE, _S(2192, "���� �Ʒý�Ű�� �ͽ��ϱ�?" ), -1, 0xA3A1A3FF );

	CTString strMessage;
	strMessage.PrintF( _S(2193, "�ֿϵ����� �Ʒý�Ų��." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_TRAINING );

	strMessage.PrintF( _S(2194, "Ż������ �����." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_CHANGE );		

	strMessage.PrintF( _S( 2467, "�� ��� ������ ��ȯ" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_STUFF );

	strMessage.PrintF( _S( 2468, "����ũ ������ ����" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_UNIQUE_PRODUCT );

	strMessage.PrintF( _S( 2444, "�� ��������" ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_SEAL_CANCEL );

	strMessage.PrintF( _S(2195, "��ų�� �ʱ�ȭ��Ų��." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_SKILLINIT );

	strMessage.PrintF(_S(4744, "�ֿϵ��� ��ȭ"));
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_EVOLUTION );

	if( bHasQuest )
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_PETTRAINING_REQ);				

	pUIManager->AddMessageBoxLString(MSGLCMD_PETTRAINING_REQ, FALSE, _S( 1748, "NPC �ȳ�" ), PET_NPC_HELP); //ttos : �ȳ��ý��� �߰���
	strMessage.PrintF( _S( 1220, "����Ѵ�." ) );		
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage );
}

void CPetTrainingUI::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PETTRAINING_NOTIFY:
		break;

	case MSGCMD_PET_CHANGE:				// Ż������ ��ȯ?
		if( bOK )
		{
			_pNetwork->SendPetChangeRide();
		}
		break;
	case MSGCMD_PET_DESTRUCTION:		// �� ������ �Ҹ�( �Ҹ�Ǹ鼭 ������������~~ ) eons
		if( bOK )
		{
			// TODO : ��Ʈ��ũ �޼����� ����.
			_pNetwork->SendPetDestruction();
		}
		CUIManager::getSingleton()->GetInventory()->Lock(FALSE, FALSE, LOCK_PET_TRAINING);
		break;
	}
}

void CPetTrainingUI::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	CTString strMessage;
	switch( nCommandCode )
	{
	case MSGLCMD_PETTRAINING_REQ:
		if( nResult == PET_TRAINING )				// �ֿϵ����� �Ʒý�Ų��.
		{
			openUI();
		}
		else if( nResult == PET_CHANGE )			// Ż������ �����.
		{
			pUIManager->CloseMessageBoxL( MSGLCMD_PETCHANGE_REQ );
			// Create skill learn message box
			pUIManager->CreateMessageBoxL( _S(2196,"�ֿϵ��� ����â"), UI_PET_TRAINING, MSGLCMD_PETCHANGE_REQ );	

			pUIManager->AddMessageBoxLString( MSGLCMD_PETCHANGE_REQ, TRUE, _S(2197,"�ֿϵ����� �̵��������� ������ų �� �ֽ��ϴ�."), -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_PETCHANGE_REQ, TRUE, _S(2198,"�̵������� �Ǹ� �ֿϵ��� �϶� ����� ����� ��� �ʱ�ȭ�Ǹ�, �̵� �������μ��� ���ο� ��� ���� ���� �Ǵ� ������ �����Ͻñ� �ٶ��ϴ�."), -1, 0xA3A1A3FF );		

			CTString strMessage;
			strMessage.PrintF( _S(2199, "Ż������ �Ʒ��ϱ�." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETCHANGE_REQ, FALSE, strMessage, CHANGE_RIDE );
		}
		else if( nResult == PET_SKILLINIT)
		{
			if (pInfo->GetMyApetInfo() != NULL &&
				pInfo->GetMyApetInfo()->m_nIdxServer > 0)
			{
				// P2 ���̶�� 
				pUIManager->GetChattingUI()->AddSysMessage(_S(6279, "��ų �ʱ�ȭ�� �Ұ����� ������ �ֿϵ����Դϴ�."), SYSMSG_ERROR);
				return;
			}

			pUIManager->CloseMessageBoxL( MSGLCMD_PETSKILLINIT_REQ );
			// Create skill learn message box
			pUIManager->CreateMessageBoxL( _S(2575,"�ֿϵ��� ��ų �ʱ�ȭ"), UI_PET_TRAINING, MSGLCMD_PETSKILLINIT_REQ );	

			pUIManager->AddMessageBoxLString( MSGLCMD_PETSKILLINIT_REQ, TRUE, _S(2576, "�ֿϵ��� ��ų �ʱ�ȭ�� �ֿϵ����� �����ߴ� ������� ��� ����Ʈ�� ȯ����Ű�� ���� ���մϴ�."), -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSKILLINIT_REQ, TRUE,CTString(""),0xA3A1A3FF);
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSKILLINIT_REQ, TRUE, _S(2577, "�ֿϵչ� ��ų�� �ʱ�ȭ��Ű�� ȯ������ ��� ����Ʈ�� �ֿϵ����� �� ���� ��Ű�ų� ������ �ֿϵ��� ��ȯ�� �� �� �ֽ��ϴ�. "), -1, 0xA3A1A3FF );		

			CTString strMessage;
			strMessage.PrintF( _S(2575, "�ֿϵ��� ��ų �ʱ�ȭ" ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSKILLINIT_REQ, FALSE, strMessage, PET_SKILLINIT );
			strMessage.PrintF( _S( 1220, "����Ѵ�." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSKILLINIT_REQ, FALSE, strMessage );

		}
		else if ( nResult == PET_EVOLUTION)
		{
			if( pUIManager->DoesMessageBoxExist( MSGCMD_PET_EVOLUTION ) ) return;

			pUIManager->CloseMessageBoxL( MSGLCMD_PETSKILLINIT_REQ );

			CUIMsgBox_Info MsgBoxInfo;
			strMessage.PrintF(_S(4727, "�� ��ȭ" ));
			MsgBoxInfo.SetMsgBoxInfo( strMessage, UMBS_YESNO | UMBS_BUTTONEX, UI_NONE, MSGCMD_PET_EVOLUTION );
			strMessage.PrintF(_S(4745,  "��ȭ�� �ϰ� �Ǹ� ����� �ٲ�� ������ 1������ �˴ϴ�. ������ ����ߴ� �� �̸��� ��ų�� �ʱ�ȭ�Ǿ� ����� �� �����ϴ�. ������ �����ؼ� ��ȭ�� �����ϼ���" ));
			MsgBoxInfo.SetBtnType( UBET_ITEM, CItemData::ITEM_ACCESSORY, CItemData::ACCESSORY_PET );
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );		
		}
		else if( nResult == PET_TALK )				// �̾߱� �Ѵ�.
		{
			// ����Ʈ â ����
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == PET_STUFF ) // �� ��� ������ ��ȯ eons
		{
			pUIManager->CloseMessageBoxL( MSGLCMD_PETSTUFFCHANGE_REQ );
			pUIManager->CreateMessageBoxL( _S( 2467, "�� ��� ������ ��ȯ" ), UI_PET_TRAINING, MSGLCMD_PETSTUFFCHANGE_REQ );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSTUFFCHANGE_REQ, TRUE,
				_S( 2470, "����Ʈ ������ ���� �Ҹ��Ű�� ����ũ ������ ���� ��Ḧ ���� �� �ֽ��ϴ�." ), -1, 0xA3A1A3FF );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSTUFFCHANGE_REQ, TRUE,
				_S( 2471, "���� ������ �������� ��� ��Ḧ ���� �� �ִ� Ȯ���� �����մϴ�." ), -1, 0xA3A1A3FF );

			CTString strMessage;
			strMessage.PrintF( _S( 2472, "���� �Ҹ� ��Ų��." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSTUFFCHANGE_REQ, FALSE, strMessage, PETITEM_DESTRUCTION );
			strMessage.PrintF( _S( 1220, "����Ѵ�." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETSTUFFCHANGE_REQ, FALSE, strMessage );
		}
		else if( nResult == PET_UNIQUE_PRODUCT )//060217 eons
		{
			if( _pNetwork->MyCharacterInfo.job == NIGHTSHADOW )
			{
				CTString	strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo(   _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_NULL);
				strMessage.PrintF( _S(5691, "����Ʈ������� �̿��� �� �����ϴ�." ) );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;
			}

			pUIManager->CloseMessageBoxL( MSGLCMD_PETUNIQUEPRODUCT_REQ );
			pUIManager->CreateMessageBoxL( _S( 2473, "������ ����" ), UI_PET_TRAINING, MSGLCMD_PETUNIQUEPRODUCT_REQ );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETUNIQUEPRODUCT_REQ, TRUE,
				_S( 2474, "������ ���� ����� ���������� �̿��Ͽ� ����ũ �������� �����Ͻ� �� �ֽ��ϴ�." ), -1, 0xA3A1A3FF );

			CTString strMessage;
			strMessage.PrintF( _S( 2460, "�巡�� ���� ����" ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETUNIQUEPRODUCT_REQ, FALSE, strMessage, DRAGON_WEAPON );
			strMessage.PrintF( _S( 2475, "����Ʈ �� ����" ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETUNIQUEPRODUCT_REQ, FALSE, strMessage, KNIGHT_ARMOR );
			strMessage.PrintF( _S( 1220, "����Ѵ�." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_PETUNIQUEPRODUCT_REQ, FALSE, strMessage );
		}
		else if( nResult == PET_SEAL_CANCEL )	// �� ���� ����
		{
			//[100119 sora] �� ���� ������ ������ ���� ���ε� ������ Ȯ���ϴ� �κ� �߰�
			if (_pNetwork->MyWearItem[WEAR_PET].IsEmptyItem() == FALSE && _pNetwork->MyWearItem[WEAR_PET].IsFlag( FLAG_ITEM_SEALED ))
			{
				// Close message box of skill learn
				pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

				// Create message box of skill learn
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(2188, "�ֿϵ���" ), UMBS_OK,
					UI_PET_TRAINING, MSGCMD_PETTRAINING_NOTIFY );
				MsgBoxInfo.AddString( _S(4765,"������ �����Ͽ��� ��Ȱ�� �����մϴ�.") );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;
			}
			pUIManager->GetPetFree()->openUI(m_fNpcX, m_fNpcZ, m_nNpcIndex);
		}
		else if( nResult == PET_NPC_HELP)										
		{
			pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
		}
		// [090527: selo] Ȯ���� ����Ʈ ����
		else if( ciQuestClassifier < nResult )	
		{
			// ������ ����Ʈ�� ���� ���� �Ǵ� ���� â�� ����.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}
		break;
	case MSGLCMD_PETCHANGE_REQ:						// �ֿϵ��� ����â.
		if( nResult == CHANGE_RIDE )				// Ż������ �Ʒ��ϱ�.
		{
			if( IsNotPetWear() )		// �� ���� Ȯ��
				return;
			// Create message box of skill learn
			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(2200, "Ż �� �Ʒ�" ), UMBS_OKCANCEL, UI_PET_TRAINING, MSGCMD_PET_CHANGE );
			strMessage.PrintF( _S(2201, "���� Ż �� �Ʒ��� �Ͻðڽ��ϱ�?" ) );
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSGLCMD_PETSKILLINIT_REQ://!!�� ��ų �ʱ�ȭ �۾� 
		{
			if(nResult == PET_SKILLINIT){
				if( IsNotPetWear() ) // �� ���� Ȯ��
					return;
				else 
					_pNetwork->SendPetSkillInit();		
			}

		}
		break;
	case MSGLCMD_PETSTUFFCHANGE_REQ: // �� ��� ������ ��ȯ
		if( nResult == PETITEM_DESTRUCTION )	// ���� �Ҹ� ��Ų��.
		{
			if( IsNotPetWear() ) // �� ���� Ȯ��
				return;

			CTString	strMessage;
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S(191,"Ȯ��"), UMBS_OKCANCEL, UI_PET_TRAINING, MSGCMD_PET_DESTRUCTION );
			strMessage.PrintF( _S( 2476, "���� ���������� ��ȯ �Ͻðڽ��ϱ�?") );
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		break;
	case MSGLCMD_PETUNIQUEPRODUCT_REQ:
		if( nResult == DRAGON_WEAPON )
		{
			pUIManager->GetPetItemMixUI()->openUI(DRAGON_WEAPON);
		}
		else if( nResult == KNIGHT_ARMOR )
		{
			pUIManager->GetPetItemMixUI()->openUI(KNIGHT_ARMOR);
		}
		break;
	}
}

void CPetTrainingUI::SendLearnSkill()
{
	// Close message box of skill learn
	CUIManager::getSingleton()->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );
	ObjInfo* pInfo = ObjInfo::getSingleton();

	SLONG	slIndex = 0;

	if (m_pMainTab == NULL)
		return;

	int nCurTabNo = m_pMainTab->getCurSelTab();

	if (nCurTabNo < 0 || nCurTabNo >= eTAB_MAX)
		return;

	if (m_pList[nCurTabNo] == NULL)
		return;

	int nSelectItemIndex = m_pList[nCurTabNo]->getCurSel();

	if(nSelectItemIndex < 0 || nSelectItemIndex >= m_pList[nCurTabNo]->getListItemCount())
		return;

	if (nCurTabNo == eTAB_COMMAND)
		slIndex = m_vecCommand[nSelectItemIndex].lSkillIndex;
	else
		slIndex = m_vecSkill[nSelectItemIndex].lSkillIndex;
	
	if( pInfo->GetMyPetInfo()->bIsActive)
		_pNetwork->LearnPetSkill( slIndex );
	else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_nIdxServer > 0 )
		_pNetwork->LearnWildPetSkill( slIndex );

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);
}

void CPetTrainingUI::LearnSkill( int nPetIdx, SLONG slIndex, SBYTE sbLevel, BOOL bAutoLearn, bool bShowMsg )
{	
	CSkill		&rSkill = _pNetwork->GetSkillData( slIndex );

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Create message box of skill learn
	if (bShowMsg == true)
	{
		// Close message box of skill learn
		pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK, UI_PET_TRAINING, MSGCMD_PETTRAINING_NOTIFY );

		if( bAutoLearn ) 
			strMessage.PrintF( _S(2202, "���ο� �米����(%s)�� �����Ͽ����ϴ�." ), rSkill.GetName() );
		else 
			strMessage.PrintF( _S( 277, "%s ��ų�� �����Ͽ����ϴ�" ), rSkill.GetName() );

		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}

	MY_INFO()->SetPetSkill(nPetIdx, slIndex, sbLevel);

	if(rSkill.GetJob() == PET_JOB)
	{
		int nPetIdx = MY_PET_INFO()->lIndex;

		if (pUIManager->GetPetInfo()->IsLearnSkill(nPetIdx, slIndex) == FALSE)
			pUIManager->GetPetInfo()->AddSkill( nPetIdx, slIndex, sbLevel );
	}
	else if(rSkill.GetJob() == WILDPET_JOB)
	{
		if(pUIManager->GetWildPetInfoUI()->IsLearnSkill(slIndex) == FALSE)
			pUIManager->GetWildPetInfoUI()->AddSkill(slIndex,sbLevel);
	}

	if (m_pMainTab == NULL)
		return;

	int nCurTab = m_pMainTab->getCurSelTab();

	if (nCurTab == eTAB_COMMAND)
		updateCommand();
	else
		updateSkill();
}

void CPetTrainingUI::LearnSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch( ubError )
	{
	case MSG_EX_PET_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "������ �����Ͽ� ��ų�� ������ �� �����ϴ�." );
		break;
	case MSG_EX_PET_LEARN_ERROR_POINT:
		strMessage = _S( 279, "���õ��� �����Ͽ� ��ų�� ������ �� �����ϴ�." );
		break;
	case MSG_EX_PET_LEARN_ERROR_ITEM:
		strMessage = _S( 280, "�������� �������� �ʾ� ��ų�� ������ �� �����ϴ�." );
		break;
	case MSG_EX_PET_LEARN_ERROR_WEAR:
		strMessage = _S(2203, "�ֿϵ����� �������� �ʾ� ��ų�� ������ �� �����ϴ�." );
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK,
		UI_PET_TRAINING, MSGCMD_PETTRAINING_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CPetTrainingUI::PetChangeItemError( SBYTE sbResult )
{
	// Show result
	CTString	strMessageA;

	switch( sbResult )
	{
	case MSG_EX_PET_CHANGE_ITEM_ERROR_OK:
		strMessageA = _S( 2477, "�����մϴ�.\n\n����ũ ������ ��Ḧ ȹ���ϼ̽��ϴ�" );
		break;
	case MSG_EX_PET_CHANGE_ITEM_ERROR_NOITEM:
		strMessageA = _S( 2478, "������ ȹ�� ����" );
		break;
	case MSG_EX_PET_CHANGE_ITEM_ERROR_FAIL:
		strMessageA = _S( 2479, "������ ȹ�� ������ �ƴմϴ�" );
		break;
	}

	closeUI();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1519, "��ȯ" ), UMBS_OK, UI_PET_ITEM_MIX, MSG_EX_PET_MIX_ITEM );		

	MsgBoxInfo.AddString( strMessageA );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

void CPetTrainingUI::EvolutionError(UBYTE errorcode)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Show result
	CTString	strMessageA;

	switch( errorcode )
	{
	case 0:
		strMessageA = _S(4746, "�� ��ȭ�� �����Ͽ����ϴ�." );
		break;
	case 1:
		strMessageA = _S(4747, "��ȭ�� �� ���� �����Դϴ�." );
		break;
	case 2:
		strMessageA = _S(4748, "������ �����Ͽ��� ��ȭ�� �����մϴ�." );
		break;
	case 3:
		strMessageA = _S(1921, "�κ��丮�� �����մϴ�." );
		break;
	case 4:
		strMessageA = _S(4750, "��ȭ ������ ���� �������� �ʽ��ϴ�." );
		break;
	case 6:
		strMessageA = _S(5442, "�� ��� ������ ���¿����� ��ȭ �� �� �����ϴ�.");
		break;
	case 5:
		{
			Notice* pNotice = GAMEDATAMGR()->GetNotice();

			if (pNotice != NULL)
				pNotice->AddToNoticeList(4020, Notice::NOTICE_EVENT);

			return;
		}		
	}

	closeUI();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(4751, "��ȭ" ), UMBS_OK, UI_PET_TRAINING, MSGCMD_PETTRAINING_NOTIFY );		

	MsgBoxInfo.AddString( strMessageA );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CPetTrainingUI::initialize()
{
#ifndef		WORLD_EDITOR

	// design
	m_pDesign = new CPetTrainingDesign;
	CUIManager::getSingleton()->LoadXML("PetTrainingDesign.xml", (CUIBase*)m_pDesign);

	int i;
	// Btn set
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel" };

	for (i = 0; i < eBTN_MAX; ++ i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CPetTrainingUI::closeUI, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CPetTrainingUI::closeUI, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CPetTrainingUI::SendLearnSkill, this));

	// List set
	std::string strListID[eLIST_MAX] = { "list_cmd", "list_skill", "list_contents" };

	for (i = 0; i < eLIST_MAX; ++ i)
		m_pList[i] = (CUIList*)findUI(strListID[i].c_str());

	// Move Area
	m_pMoveArea = (CUIImage*)findUI("move_area");

	// Text
	m_pstrAbilityPoint = (CUIText*)findUI("str_abilitypoint");

	// main tab
	m_pMainTab = (CUITab*)findUI("tab_main");

#endif		// WORLD_EDITOR
}

void CPetTrainingUI::openUI()
{
	if( IsNotPetWear() )		// �� ���� Ȯ��
		return;

	CUIManager* pUIMgr = UIMGR();

	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		// �̹� Lock �� â�� ���� ��� ���� ���Ѵ�.
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	Hide(FALSE);
	SetVisible(TRUE);

	_SetUI();
	updateUI();
	pUIMgr->RearrangeOrder( UI_PET_TRAINING, TRUE );
	pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_PET_TRAINING);
}

void CPetTrainingUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_PET_TRAINING, FALSE );
	pUIMgr->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );
	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_PET_TRAINING);
}

void CPetTrainingUI::updateUI()
{
	updateSkill();
	updateCommand();
	updateAbilityPoint();
}

void CPetTrainingUI::updateSkill()
{
	if (GetHide() == TRUE)
		return;

	int		iRow;
	INDEX	ctPosSkill = 0;			// Possible
	INDEX	ctImposSkill = 0;		// Impossible	

	std::vector<sCollectSkill>		vectorPosSkill;
	std::vector<sCollectSkill>		vectorImposSkill;

	vectorPosSkill.resize(SLEARN_SLOT_ROW_TOTAL);
	vectorImposSkill.resize(SLEARN_SLOT_ROW_TOTAL);

	INDEX iCrrJob = PET_JOB;
	ObjInfo* pInfo = ObjInfo::getSingleton();

	int nPetLevel = 0;

	if(pInfo->GetMyPetInfo()->bIsActive)
	{
		iCrrJob = PET_JOB;
		nPetLevel = pInfo->GetMyPetInfo()->iLevel;
	}
	else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
	{
		iCrrJob = WILDPET_JOB;
		nPetLevel = pInfo->GetMyApetInfo()->m_nLevel;
	}

	int iSkill;
	for( iSkill = 1; iSkill <= _pNetwork->wo_iNumOfSkill; iSkill++ )
	{
		CSkill	&rSkill = _pNetwork->GetSkillData( iSkill );

		// If job is different
		if( rSkill.GetJob() != iCrrJob)
			continue;

		if( ( rSkill.GetJob2() < 0 ) )
			continue;
		// Type ���� 
		if(iCrrJob == PET_JOB)
		{
			if( ( rSkill.GetJob2() % 2 != pInfo->GetMyPetInfo()->iType%2 ) )
				continue;

			// Ÿ�� ���϶��� Job 2�̻��� ��ų�� ó�� 
			if( rSkill.GetJob2() < 2 &&  pInfo->GetMyPetInfo()->iAge == 2 )
				continue;
		}
		else if(iCrrJob == WILDPET_JOB)
		{
			if (pInfo->GetMyApetInfo() != NULL)
			{
				if(!((rSkill.GetJob2() ==pInfo->GetMyApetInfo()->m_nType)&&(rSkill.GetWildpetIndex() == pInfo->GetMyApetInfo()->m_nIndex)))
					continue;
			}

		}

		int		nSkillIndex = rSkill.GetIndex();

		SBYTE	sbSkillLevel = -1;

		if (iCrrJob == PET_JOB)
			sbSkillLevel = MY_INFO()->GetPetSkillLevel(pInfo->GetMyPetInfo()->lIndex, nSkillIndex);
		else if(iCrrJob == WILDPET_JOB)
			sbSkillLevel = MY_INFO()->GetPetSkillLevel(0, nSkillIndex);

		// If this skill is already max level
		if( sbSkillLevel >= rSkill.GetMaxLevel() )
			continue;

		if( sbSkillLevel < 1 )	sbSkillLevel = 0;

		ULONG	ulNeedCharLevel = rSkill.GetLearnLevel( sbSkillLevel );
		sbSkillLevel++;

		switch( rSkill.GetType() )
		{
		case CSkill::ST_PET_SKILL_PASSIVE:	// pet skill
		case CSkill::ST_PET_SKILL_ACTIVE:
			{
				int petLevel;
				if( iCrrJob == WILDPET_JOB )
					petLevel = pInfo->GetMyApetInfo()->m_nLevel;
				else
					petLevel = pInfo->GetMyPetInfo()->iLevel;

				if( ulNeedCharLevel <= petLevel )
					vectorPosSkill[ctPosSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);		
				else
					vectorImposSkill[ctImposSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
			}
			break;
		case CSkill::ST_MELEE:		// ������ ���� ���� �� ��ų�� ������� ����
			{
				if( ulNeedCharLevel <= pInfo->GetMyApetInfo()->m_nLevel )
					vectorPosSkill[ctPosSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);		
				else
					vectorImposSkill[ctImposSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
			}
			break;
		}
	}

	std::sort(vectorPosSkill.begin(), vectorPosSkill.end());
	std::sort(vectorImposSkill.begin(), vectorImposSkill.end());

	CUIListItem* pTemp = NULL;
	CUIListItem* pItem = NULL;
	
	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;
	CTString strTemp;

	// Add active skill buttons
	iSkill = 0;

	if (m_pList[eLIST_SKILL] != NULL)
	{
		m_pList[eLIST_SKILL]->DeleteAllListItem();

		pTemp = (CUIListItem*)m_pList[eLIST_SKILL]->GetListItemTemplate();

		if (pTemp == NULL)
			return;

		// Possible active skill
		for( iRow = 0; iRow < ctPosSkill; iRow++ )
		{
			m_pList[eLIST_SKILL]->AddListItem((CUIListItem*)pTemp->Clone());
			pItem =(CUIListItem*)m_pList[eLIST_SKILL]->GetListItem(iRow);

			if (pItem == NULL)
				continue;

			CmdPetTrainingSelectItem* pCmd = new CmdPetTrainingSelectItem;
			pCmd->setData(this, eTAB_SKILL, iSkill);
			pItem->SetCommand(pCmd);
			iSkill++;

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if (pIcon == NULL)
				continue;

			pIcon->setData(UBET_SKILL, vectorPosSkill[iRow].lSkillIndex);
			
			pText = (CUIText*)pItem->findUI("text_name");

			if (pText == NULL)
				continue;

			CSkill	&rSkill = _pNetwork->GetSkillData(vectorPosSkill[iRow].lSkillIndex);
			SBYTE	sbLevel = vectorPosSkill[iRow].sbSkillLevel;
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			strTemp.PrintF( "%s", rSkill.GetName() );
			pText->SetText(strTemp);
			pText->setFontColor(nPetLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF);

			pText = (CUIText*)pItem->findUI("text_levelSp");

			if (pText == NULL)
				continue;

			strTemp.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );

			pText->SetText(strTemp);
			pText->setFontColor(0xBDA99FFF);
		}

		// Impossible active skill
		for( iRow = 0; iRow < ctImposSkill; iRow++ )
		{
			m_pList[eLIST_SKILL]->AddListItem((CUIListItem*)pTemp->Clone());
			pItem = (CUIListItem*)m_pList[eLIST_SKILL]->GetListItem(iRow + iSkill);

			if (pItem == NULL)
				continue;

			CmdPetTrainingSelectItem* pCmd = new CmdPetTrainingSelectItem;
			pCmd->setData(this, eTAB_SKILL, iRow + iSkill);
			pItem->SetCommand(pCmd);

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if (pIcon == NULL)
				continue;

			pIcon->setData(UBET_SKILL, vectorImposSkill[iRow].lSkillIndex);
			
			pText = (CUIText*)pItem->findUI("text_name");

			if (pText == NULL)
				continue;

			CSkill	&rSkill = _pNetwork->GetSkillData(vectorImposSkill[iRow].lSkillIndex);
			SBYTE	sbLevel = vectorImposSkill[iRow].sbSkillLevel;
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			strTemp.PrintF( "%s", rSkill.GetName() );
			pText->SetText(strTemp);
			pText->setFontColor(nPetLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF);

			pText = (CUIText*)pItem->findUI("text_levelSp");

			if (pText == NULL)
				continue;

			strTemp.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );

			pText->SetText(strTemp);
			pText->setFontColor(0xBDA99FFF);
		}

		int nItemCount = m_pList[eLIST_SKILL]->getListItemCount();
		m_pList[eLIST_SKILL]->setCurSel(-1);
		CUIScrollBar* pScroll = m_pList[eLIST_SKILL]->GetScroll();
		if (pScroll != NULL)
			pScroll->SetScrollCurPos(0);

		m_pList[eLIST_SKILL]->UpdateScroll(nItemCount);
		m_pList[eLIST_SKILL]->UpdateList();
	}

	m_vecSkill.clear();

	for (iRow = 0; iRow < vectorPosSkill.size(); ++iRow)
	{
		if (vectorPosSkill[iRow].lSkillIndex > 0)
			m_vecSkill.push_back(vectorPosSkill[iRow]);
	}

	for (iRow = 0; iRow < vectorImposSkill.size(); ++iRow)
	{
		if (vectorImposSkill[iRow].lSkillIndex > 0)
			m_vecSkill.push_back(vectorImposSkill[iRow]);
	}

	if (m_pMainTab == NULL)
		return;

	int nCurTab = m_pMainTab->getCurSelTab();

	if (nCurTab == eTAB_SKILL)
	{
		if (m_pList[eLIST_SKILL] == NULL)
			return;

		int nSelectItemIndex = m_pList[eLIST_SKILL]->getCurSel();
		SelectItem(nSelectItemIndex, eTAB_SKILL);
	}
}

void CPetTrainingUI::updateCommand()
{
	if (GetHide() == TRUE)
		return;

	int		iRow;
	INDEX	ctPosCommand = 0;		// Possible
	INDEX	ctImposCommand = 0;		// Impossible	

	std::vector<sCollectSkill>		vectorPosCommand;
	std::vector<sCollectSkill>		vectorImposCommand;

	vectorPosCommand.resize(SLEARN_SLOT_ROW_TOTAL);
	vectorImposCommand.resize(SLEARN_SLOT_ROW_TOTAL);

	INDEX iCrrJob = PET_JOB;
	ObjInfo* pInfo = ObjInfo::getSingleton();
	int nPetLevel = 0;

	if(pInfo->GetMyPetInfo()->bIsActive)
	{
		iCrrJob = PET_JOB;
		nPetLevel = pInfo->GetMyPetInfo()->iLevel;
	}
	else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
	{
		iCrrJob = WILDPET_JOB;
		nPetLevel = pInfo->GetMyApetInfo()->m_nLevel;
	}		

	int iSkill;
	for( iSkill = 1; iSkill <= _pNetwork->wo_iNumOfSkill; iSkill++ )
	{
		CSkill	&rSkill = _pNetwork->GetSkillData( iSkill );

		// If job is different
		if( rSkill.GetJob() != iCrrJob)
			continue;

		if( ( rSkill.GetJob2() < 0 ) )
			continue;
		// Type ���� 
		if(iCrrJob == PET_JOB)
		{
			if( ( rSkill.GetJob2() % 2 != pInfo->GetMyPetInfo()->iType%2 ) )
				continue;

			// Ÿ�� ���϶��� Job 2�̻��� ��ų�� ó�� 
			if( rSkill.GetJob2() < 2 &&  pInfo->GetMyPetInfo()->iAge == 2 )
				continue;
		}
		else if(iCrrJob == WILDPET_JOB)
		{
			if (pInfo->GetMyApetInfo() != NULL)
			{
				if(!((rSkill.GetJob2() ==pInfo->GetMyApetInfo()->m_nType)&&(rSkill.GetWildpetIndex() == pInfo->GetMyApetInfo()->m_nIndex)))
					continue;
			}

		}
		int		nSkillIndex = rSkill.GetIndex();
		SBYTE	sbSkillLevel = -1;

		if (iCrrJob == PET_JOB)
			sbSkillLevel = MY_INFO()->GetPetSkillLevel(pInfo->GetMyPetInfo()->lIndex, nSkillIndex);
		else if(iCrrJob == WILDPET_JOB)
			sbSkillLevel = MY_INFO()->GetPetSkillLevel(0, nSkillIndex);

		// If this skill is already max level
		if( sbSkillLevel >= rSkill.GetMaxLevel() )
			continue;

		if( sbSkillLevel < 1 )	sbSkillLevel = 0;

		ULONG	ulNeedCharLevel = rSkill.GetLearnLevel( sbSkillLevel );
		sbSkillLevel++;

		switch( rSkill.GetType() )
		{
		case CSkill::ST_PET_COMMAND:	// pet command
			{
				if( ulNeedCharLevel <= pInfo->GetMyPetInfo()->iLevel )
					vectorPosCommand[ctPosCommand++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);				
				else
					vectorImposCommand[ctImposCommand++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
			}
			break;
		}
	}

	std::sort(vectorPosCommand.begin(), vectorPosCommand.end());
	std::sort(vectorImposCommand.begin(), vectorImposCommand.end());

	CUIListItem* pTemp = NULL;
	CUIListItem* pItem = NULL;

	CUIIcon* pIcon = NULL;
	CUIText* pText = NULL;
	CTString strTemp;
	// Add active skill buttons
	iSkill = 0;

	if (m_pList[eLIST_COMMAND] != NULL)
	{
		m_pList[eLIST_COMMAND]->DeleteAllListItem();

		pTemp = m_pList[eLIST_COMMAND]->GetListItemTemplate();

		if (pTemp == NULL)
			return;

		// Possible active skill
		for( iRow = 0; iRow < ctPosCommand; iRow++ )
		{
			m_pList[eLIST_COMMAND]->AddListItem((CUIListItem*)pTemp->Clone());
			pItem = (CUIListItem*)m_pList[eLIST_COMMAND]->GetListItem(iRow);

			if (pItem == NULL)
				continue;

			CmdPetTrainingSelectItem* pCmd = new CmdPetTrainingSelectItem;
			pCmd->setData(this, eTAB_COMMAND, iSkill);
			pItem->SetCommand(pCmd);
			iSkill++;

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if (pIcon == NULL)
				continue;

			pIcon->setData(UBET_SKILL, vectorPosCommand[iRow].lSkillIndex);

			pText = (CUIText*)pItem->findUI("text_name");

			if (pText == NULL)
				continue;

			CSkill	&rSkill = _pNetwork->GetSkillData(vectorPosCommand[iRow].lSkillIndex);
			SBYTE	sbLevel = vectorPosCommand[iRow].sbSkillLevel;
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			strTemp.PrintF( "%s", rSkill.GetName() );
			pText->SetText(strTemp);
			pText->setFontColor(nPetLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF);

			pText = (CUIText*)pItem->findUI("text_levelSp");

			if (pText == NULL)
				continue;

			strTemp.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );

			pText->SetText(strTemp);
			pText->setFontColor(0xBDA99FFF);
		}

		// Impossible active skill
		for( iRow = 0; iRow < ctImposCommand; iRow++ )
		{
			m_pList[eLIST_COMMAND]->AddListItem((CUIListItem*)pTemp->Clone());
			pItem = (CUIListItem*)m_pList[eLIST_COMMAND]->GetListItem(iRow + iSkill);

			if (pItem == NULL)
				continue;

			CmdPetTrainingSelectItem* pCmd = new CmdPetTrainingSelectItem;
			pCmd->setData(this, eTAB_COMMAND, iRow + iSkill);
			pItem->SetCommand(pCmd);

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if (pIcon == NULL)
				continue;

			pIcon->setData(UBET_SKILL, vectorImposCommand[iRow].lSkillIndex);

			pText = (CUIText*)pItem->findUI("text_name");

			if (pText == NULL)
				continue;

			CSkill	&rSkill = _pNetwork->GetSkillData(vectorImposCommand[iRow].lSkillIndex);
			SBYTE	sbLevel = vectorImposCommand[iRow].sbSkillLevel;
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			strTemp.PrintF( "%s", rSkill.GetName() );
			pText->SetText(strTemp);
			pText->setFontColor(nPetLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF);

			pText = (CUIText*)pItem->findUI("text_levelSp");

			if (pText == NULL)
				continue;

			strTemp.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );

			pText->SetText(strTemp);
			pText->setFontColor(0xBDA99FFF);
		}

		int nItemCount = m_pList[eLIST_COMMAND]->getListItemCount();
		m_pList[eLIST_COMMAND]->setCurSel(-1);
		CUIScrollBar* pScroll = m_pList[eLIST_COMMAND]->GetScroll();
		if (pScroll != NULL)
			pScroll->SetScrollCurPos(0);

		m_pList[eLIST_COMMAND]->UpdateScroll(nItemCount);
		m_pList[eLIST_COMMAND]->UpdateList();
	}

	m_vecCommand.clear();

	for (iRow = 0; iRow < vectorPosCommand.size(); ++iRow)
	{
		if (vectorPosCommand[iRow].lSkillIndex > 0)
			m_vecCommand.push_back(vectorPosCommand[iRow]);
	}

	for (iRow = 0; iRow < vectorImposCommand.size(); ++iRow)
	{
		if (vectorImposCommand[iRow].lSkillIndex > 0)
			m_vecCommand.push_back(vectorImposCommand[iRow]);
	}

	if (m_pMainTab == NULL)
		return;

	int nCurTab = m_pMainTab->getCurSelTab();

	if (nCurTab == eTAB_COMMAND)
	{
		if (m_pList[eLIST_COMMAND] == NULL)
			return;

		int nSelectItemIndex = m_pList[eLIST_COMMAND]->getCurSel();
		SelectItem(nSelectItemIndex, eTAB_COMMAND);
	}
}

void CPetTrainingUI::updateAbilityPoint()
{
	if (GetHide() == TRUE)
		return;

	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo == NULL)
		return;

	if (pInfo->GetMyPetInfo() == NULL)
		return;

	CTString strAbility;
	if(pInfo->GetMyPetInfo()->lAbility >= 0)
		strAbility.PrintF( "%d", pInfo->GetMyPetInfo()->lAbility );
	else
		strAbility.PrintF("0");
	
	if (m_pstrAbilityPoint != NULL)
		m_pstrAbilityPoint->SetText(strAbility);
}

void CPetTrainingUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
//	������� �ʾ� �ּ�ó����
// 	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
// 	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
// 
// 	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
// 		closeUI();

	updateAbilityPoint();
}

WMSG_RESULT CPetTrainingUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();
	return WMSG_FAIL;
}

WMSG_RESULT CPetTrainingUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}
	
	CUIManager::getSingleton()->RearrangeOrder( UI_PET_TRAINING, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CPetTrainingUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

bool CPetTrainingUI::IsNotPetWear()
{
	bool bWildPet = false;
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if (pInfo->GetMyApetInfo() != NULL)
	{
		bWildPet  = pInfo->GetMyApetInfo()->m_nIdxServer > 0 ? true : false;
	}

	if( !(pInfo->GetMyPetInfo()->bIsActive || bWildPet))
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		// Close message box of skill learn
		pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

		// Create message box of skill learn
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S(2188, "�ֿϵ���" ), UMBS_OK,
			UI_PET_TRAINING, MSGCMD_PETTRAINING_NOTIFY );
		MsgBoxInfo.AddString( _S(2189,"�ֿϵ����� ���Կ� �����Ͽ��߸� �մϴ�.") );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		return true;
	}
	else
	{	return false;	}
}

void CPetTrainingUI::_SetUI()
{
	if (m_pList[eLIST_CONTENTS] != NULL)
		m_pList[eLIST_CONTENTS]->DeleteAllListItem();

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(FALSE);

	if (m_pstrAbilityPoint != NULL)
		m_pstrAbilityPoint->SetText(CTString("0"));	
}

void CPetTrainingUI::SelectItem( int nSelectIndex, int nTabType )
{
	if (m_pList[eLIST_CONTENTS] == NULL)
		return;

	m_pList[eLIST_CONTENTS]->DeleteAllListItem();

	if (nSelectIndex < 0)
		return;

	int nCheckCount = 0;
	
	if (nTabType == eTAB_COMMAND)
		nCheckCount = m_vecCommand.size();
	else
		nCheckCount = m_vecSkill.size();

	if (nSelectIndex >= nCheckCount)
		return;
	
	int nSkillIndex = 0;
	int nSkillLv = 0;

	if (nTabType == eTAB_COMMAND)
	{
		nSkillIndex = m_vecCommand[nSelectIndex].lSkillIndex;
		nSkillLv = m_vecCommand[nSelectIndex].sbSkillLevel;
	}
	else
	{
		nSkillIndex = m_vecSkill[nSelectIndex].lSkillIndex;
		nSkillLv = m_vecSkill[nSelectIndex].sbSkillLevel;
	}

	CTString	strTemp, strCount;	
	CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );

	strTemp.PrintF( "%s", rSkill.GetName() );
	_AddString(eDESIGN_NAME, strTemp);


	if( rSkill.GetFlag() & SF_SINGLEMODE )
	{
		strTemp.PrintF( _S( 499, "�۽��δ��� ���� ��ų" ) );
		_AddString(eDESIGN_ADD, strTemp);
	}

	_AddString(eDESIGN_SPACE);

	const char	*pDesc = rSkill.GetDescription();
	if( pDesc != NULL )
	{
		strTemp.PrintF( "%s\n\n", pDesc );
		_AddString(eDESIGN_CONTENTS, strTemp);
	}	

	_AddString(eDESIGN_SPACE);

	--nSkillLv;

	// Get learning condition
	int		nLearnSkillIndex[3];
	SBYTE	sbLearnSkillLevel[3];
	int		nLearnItemIndex[3];
	int		nLearnItemCount[3];
	BOOL	bLearnSkill = FALSE;
	BOOL	bLearnItem = FALSE;

	for( int i = 0; i < 3; i++ )
	{
		nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nSkillLv, i );
		sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nSkillLv, i );
		nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nSkillLv, i );
		nLearnItemCount[i] = rSkill.GetLearnItemCount( nSkillLv, i );

		if( nLearnSkillIndex[i] != -1 )
			bLearnSkill = TRUE;
		if( nLearnItemIndex[i] != -1 )
			bLearnItem = TRUE;
	}

	CTString strSp;
	//!!! �߰� �۾� �ʿ� 
	switch( rSkill.GetType() )
	{
	case CSkill::ST_MELEE:					// Active
	case CSkill::ST_RANGE:					// Active
	case CSkill::ST_MAGIC:					// Active
		{
			strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), rSkill.GetLearnLevel( nSkillLv ) );
			_AddString(eDESIGN_INFO, strTemp);

			strSp.PrintF("%d", rSkill.GetLearnSP( nSkillLv ));
			UIMGR()->InsertCommaToString(strSp);

			strTemp.PrintF( _S( 257, "�ʿ� SP : %s" ), strSp ); // ���õ�
			_AddString(eDESIGN_INFO, strTemp);

			const int iLearnStr = rSkill.GetLearnStr( nSkillLv );
			const int iLearnDex = rSkill.GetLearnDex( nSkillLv );
			const int iLearnInt = rSkill.GetLearnInt( nSkillLv );
			const int iLearnCon = rSkill.GetLearnCon( nSkillLv );

			if( iLearnStr > 0 ) 
			{
				strTemp.PrintF( _S( 1391, "�ʿ� �� : %d" ), iLearnStr );		// �ʿ� �� 
				_AddString(eDESIGN_INFO, strTemp);
			}

			if( iLearnDex > 0 ) 
			{
				strTemp.PrintF( _S( 1392, "�ʿ� ��ø : %d" ), iLearnDex );	// �ʿ� ��ø
				_AddString(eDESIGN_INFO, strTemp);
			}

			if( iLearnInt > 0 ) 
			{
				strTemp.PrintF( _S( 1393, "�ʿ� ���� : %d" ), iLearnInt );	// �ʿ� ����
				_AddString(eDESIGN_INFO, strTemp);
			}

			if( iLearnCon > 0 )
			{
				strTemp.PrintF( _S( 1394, "�ʿ� ü�� : %d" ), iLearnCon );	// �ʿ� ü��
				_AddString(eDESIGN_INFO, strTemp);
			}

			// Need skill
			if( bLearnSkill )
			{
				strTemp.PrintF(_S( 258, "�ʿ� ��ų" ));
				_AddString(eDESIGN_INFO, strTemp);
				for( int i = 0; i < 3; i++ )
				{
					if( nLearnSkillIndex[i] != -1 )
					{
						CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
						strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
						_AddString(eDESIGN_INFO, strTemp);
					}
				}
			}

			// Need item
			if( bLearnItem )
			{
				strTemp.PrintF(_S( 259, "�ʿ� ������" ));
				_AddString(eDESIGN_INFO, strTemp);
				for( int i = 0; i < 3; i++ )
				{
					if( nLearnItemIndex[i] != -1 )
					{
						strCount = UIMGR()->IntegerToCommaString(nLearnItemCount[i]);
						strTemp.PrintF( _S( 260, "  %s %s��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), strCount );
						_AddString(eDESIGN_INFO, strTemp);
					}
				}
			}

			int	nNeedMP = rSkill.GetNeedMP( nSkillLv );
			int	nNeedHP = rSkill.GetNeedHP( nSkillLv );
			if( nNeedHP == 0 )
			{
				if( nNeedMP != 0 )
				{
					strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
					_AddString(eDESIGN_INFO, strTemp);
				}
			}
			else
			{
				if( nNeedMP == 0 )
				{
					strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );		
					_AddString(eDESIGN_INFO, strTemp);
				}
				else
				{
					strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
					_AddString(eDESIGN_INFO, strTemp);
					strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );		
					_AddString(eDESIGN_INFO, strTemp);
				}
			}

			if( rSkill.GetPower( nSkillLv ) > 0 )
			{
				strTemp.PrintF( _S( 65, "���� : %d" ), rSkill.GetPower( nSkillLv ) );
				_AddString(eDESIGN_INFO, strTemp);
			}

			strTemp.PrintF( _S( 66, "��ȿ �Ÿ� : %.1f" ), rSkill.GetFireRange() );
			_AddString(eDESIGN_INFO, strTemp);
			strTemp.PrintF( _S( 261, "�ִ뽺ų ���� : %d" ), rSkill.GetMaxLevel() );
			_AddString(eDESIGN_INFO, strTemp);
		}
		break;

	case CSkill::ST_PASSIVE:
	case CSkill::ST_PET_SKILL_PASSIVE:				// Passive
		{
			strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), rSkill.GetLearnLevel( nSkillLv ) );
			_AddString(eDESIGN_INFO, strTemp);

			strSp.PrintF("%d", rSkill.GetLearnSP( nSkillLv ));
			UIMGR()->InsertCommaToString(strSp);

			strTemp.PrintF( _S( 257, "�ʿ� SP : %s" ), strSp ); // ���õ�
			_AddString(eDESIGN_INFO, strTemp);

			const int iLearnStr = rSkill.GetLearnStr( nSkillLv );
			const int iLearnDex = rSkill.GetLearnDex( nSkillLv );
			const int iLearnInt = rSkill.GetLearnInt( nSkillLv );
			const int iLearnCon = rSkill.GetLearnCon( nSkillLv );

			if( iLearnStr > 0 ) 
			{
				strTemp.PrintF( _S( 1391, "�ʿ� �� : %d" ), iLearnStr );		// �ʿ� ��
				_AddString(eDESIGN_INFO, strTemp);
			}

			if( iLearnDex > 0 ) 
			{
				strTemp.PrintF( _S( 1392, "�ʿ� ��ø : %d" ), iLearnDex );	// �ʿ� ��ø
				_AddString(eDESIGN_INFO, strTemp);
			}

			if( iLearnInt > 0 ) 
			{
				strTemp.PrintF( _S( 1393, "�ʿ� ���� : %d" ), iLearnInt );	// �ʿ� ����
				_AddString(eDESIGN_INFO, strTemp);
			}

			if( iLearnCon > 0 )
			{
				strTemp.PrintF( _S( 1394, "�ʿ� ü�� : %d" ), iLearnCon );	// �ʿ� ü��
				_AddString(eDESIGN_INFO, strTemp);
			}

			// Need skill
			if( bLearnSkill )
			{
				strTemp.PrintF(_S( 258, "�ʿ� ��ų" ));
				_AddString(eDESIGN_INFO, strTemp);

				for( int i = 0; i < 3; i++ )
				{
					if( nLearnSkillIndex[i] != -1 )
					{
						CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
						strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
						_AddString(eDESIGN_INFO, strTemp);
					}
				}
			}

			// Need item
			if( bLearnItem )
			{
				strTemp.PrintF(_S( 259, "�ʿ� ������" ));
				_AddString(eDESIGN_INFO, strTemp);

				for( int i = 0; i < 3; i++ )
				{
					if( nLearnItemIndex[i] != -1 )
					{
						strCount = UIMGR()->IntegerToCommaString(nLearnItemCount[i]);
						strTemp.PrintF( _S( 260, "  %s %s��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), strCount );
						_AddString(eDESIGN_INFO, strTemp);
					}
				}
			}

			strTemp.PrintF( _S( 261, "�ִ뽺ų ���� : %d" ), rSkill.GetMaxLevel() );
			_AddString(eDESIGN_INFO, strTemp);
		}
		break;
	}

	int nItemCount = m_pList[eLIST_CONTENTS]->getListItemCount();

	m_pList[eLIST_CONTENTS]->UpdateScroll(nItemCount);
	m_pList[eLIST_CONTENTS]->UpdateList();

	int nShowCnt = m_pList[eLIST_CONTENTS]->GetItemShowNum();

	CUIScrollBar* pScroll = m_pList[eLIST_CONTENTS]->GetScroll();

	if (pScroll != NULL)
	{
		pScroll->SetScrollCurPos(0);
		pScroll->SetItemsPerPage(nShowCnt);
	}
	
	int nPetLevel = 0;
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if(pInfo->GetMyPetInfo()->bIsActive)
		nPetLevel = pInfo->GetMyPetInfo()->iLevel;
	else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
		nPetLevel = pInfo->GetMyApetInfo()->m_nLevel;

	int	nNeedLevel = rSkill.GetLearnLevel(nSkillLv);

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(nPetLevel >= nNeedLevel ? TRUE : FALSE);
}

void CPetTrainingUI::_AddString(int nAddType, CTString strAdd)
{
	if (m_pList[eLIST_CONTENTS] == NULL)
		return;

	if (m_pDesign == NULL)
		return;

	CUIText* pText = NULL;
	
	switch(nAddType)
	{
	case eDESIGN_NAME:
		pText = (CUIText*)m_pDesign->CloneNameText();
		break;
	case eDESIGN_ADD:
		pText = (CUIText*)m_pDesign->CloneAddText();
		break;
	case eDESIGN_INFO:
		pText = (CUIText*)m_pDesign->CloneInfoText();
		break;
	case eDESIGN_CONTENTS:
		{
			CUITextBox* pTbBox = (CUITextBox*)m_pDesign->CloneDescText();
			if(pTbBox != NULL)
			{
				COLOR color = pTbBox->GetTextColor();
				pTbBox->SetText(strAdd, color, TRUE);
				m_pList[eLIST_CONTENTS]->AddListItem(pTbBox);
			}
		}
		return;
	case eDESIGN_SPACE:
		pText = (CUIText*)m_pDesign->CloneSpace();
		break;
	}

	if (pText != NULL)
	{
		pText->SetText(strAdd);
		m_pList[eLIST_CONTENTS]->AddListItem(pText);
	}
}

CPetTrainingDesign::CPetTrainingDesign()
	: m_ptbDesc(NULL)
	, m_pstrName(NULL)
	, m_pstrInfo(NULL)
	, m_pstrAdd(NULL)
	, m_pstrSpace(NULL)
{

}

void CPetTrainingDesign::initialize()
{
	m_ptbDesc = (CUITextBox*)findUI("tb_contents");
	m_pstrName = (CUIText*)findUI("text_name");
	m_pstrInfo = (CUIText*)findUI("text_info");
	m_pstrAdd = (CUIText*)findUI("text_Add");
	m_pstrSpace = (CUIText*)findUI("text_space");
}
