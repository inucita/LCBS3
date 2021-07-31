#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIPetTraining.h>
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


#define	PT_TAB_WIDTH				96
#define	PT_COMMAND_TAB_CX			60
#define	PT_SKILL_TAB_CX				156

#define	PT_TAB_SY					34


// Define text
#define	PT_NAME_CX					122
#define	PT_NAME_SY					58
#define	PT_NEED_RX					184
#define	PT_CURSP_SX					40 //�̱�ȯ ���� (05.01.03) : SP->���õ� ��� �ܾ� ���� �ϸ鼭 ��ġ ����(97->75)
#define	PT_CURSP_RX					199
#define	PT_CURSP_SY					343
#define	PT_DESC_CHAR_WIDTH			170

// Define text position
#define	PT_TITLE_TEXT_OFFSETX		25
#define	PT_TITLE_TEXT_OFFSETY		5


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

// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;
// ----------------------------------------------------------------------------
// Name : CUIPetTraining()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPetTraining::CUIPetTraining()
{
	m_nSelCommandID		= -1;
	m_nSelSkillID		= -1;	

	m_nCurrentTab		= PETTRAINING_TAB_COMMAND;

	m_nNpcIndex = -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPetTraining()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPetTraining::~CUIPetTraining()
{
	int		i;

	for (i = 0; i < SLEARN_SLOT_ROW_TOTAL ; ++i)
		SAFE_DELETE(m_pIconsCommand[i]);

	for (i = 0; i < SLEARN_SLOT_ROW_TOTAL ; ++i)
		SAFE_DELETE(m_pIconsSkill[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = PT_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	m_rcIcons.SetRect( 17, 55, 51, 200 );
	m_rcTab.SetRect( 12, 31, 204, 50 );

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SkillLearn.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	m_rtBackground.SetUV( 0, 0, 216, 400, fTexWidth, fTexHeight );
	m_rtSelOutline.SetUV( 218, 80, 250, 112, fTexWidth, fTexHeight );
	m_rtTabLine.SetUV( 12, 31, 13, 50, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Learn button
	m_btnLearn.Create( this, _S( 269, "����" ), 70, 372, 63, 21 );
	m_btnLearn.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnLearn.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnLearn.CopyUV( UBS_IDLE, UBS_ON );
	m_btnLearn.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "���" ), 141, 372, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Scroll bar of active skill icons
	m_sbCommandIcon.Create( this, 193, 51, 9, 153 );
	m_sbCommandIcon.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbCommandIcon.SetScrollPos( 0 );
	m_sbCommandIcon.SetScrollRange( SLEARN_SLOT_ROW_TOTAL );
	m_sbCommandIcon.SetCurItemCount( 0 );
	m_sbCommandIcon.SetItemsPerPage( SLEARN_SLOT_ROW );
	// Up button
	m_sbCommandIcon.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbCommandIcon.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbCommandIcon.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbCommandIcon.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbCommandIcon.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbCommandIcon.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbCommandIcon.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbCommandIcon.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbCommandIcon.SetWheelRect( -181, -1, 180, 154 );

	// Scroll bar of passive skill icons
	m_sbSkillIcon.Create( this, 193, 51, 9, 153 );
	m_sbSkillIcon.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbSkillIcon.SetScrollPos( 0 );
	m_sbSkillIcon.SetScrollRange( SLEARN_SLOT_ROW_TOTAL );
	m_sbSkillIcon.SetCurItemCount( 0 );
	m_sbSkillIcon.SetItemsPerPage( SLEARN_SLOT_ROW );
	// Up button
	m_sbSkillIcon.SetUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_sbSkillIcon.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbSkillIcon.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbSkillIcon.SetBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_sbSkillIcon.SetDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_sbSkillIcon.SetDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_sbSkillIcon.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbSkillIcon.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbSkillIcon.SetWheelRect( -181, -1, 180, 154 );

	// List box of skill description
	m_lbSkillDesc.Create( this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE );
	m_lbSkillDesc.CreateScroll( TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10 );
	// Up button
	m_lbSkillDesc.SetScrollUpUV( UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight );
	m_lbSkillDesc.SetScrollUpUV( UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight );
	m_lbSkillDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbSkillDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbSkillDesc.SetScrollBarTopUV( 219, 32, 228, 42, fTexWidth, fTexHeight );
	m_lbSkillDesc.SetScrollBarMiddleUV( 219, 43, 228, 45, fTexWidth, fTexHeight );
	m_lbSkillDesc.SetScrollBarBottomUV( 219, 46, 228, 56, fTexWidth, fTexHeight );
	// Down button
	m_lbSkillDesc.SetScrollDownUV( UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight );
	m_lbSkillDesc.SetScrollDownUV( UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight );
	m_lbSkillDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbSkillDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );

	int		iRow;
	// Active skill buttons
	for( iRow = 0; iRow < SLEARN_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_pIconsCommand[iRow] = new CUIIcon;
		m_pIconsCommand[iRow]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETTRAINING, UBET_SKILL);
	}

	// Passive skill buttons
	for( iRow = 0; iRow < SLEARN_SLOT_ROW_TOTAL ; iRow++ )
	{
		m_pIconsSkill[iRow] = new CUIIcon;
		m_pIconsSkill[iRow]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PETTRAINING, UBET_SKILL);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : InitPetTraining()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::InitPetTraining()
{
	if( !IsVisible() ) return;

	// Reset description
	m_lbSkillDesc.ResetAllStrings();
	
	int		iRow;
	// Reset buttons
	for( iRow = 0; iRow < SLEARN_SLOT_ROW_TOTAL; iRow++ )
	{
		m_pIconsCommand[iRow]->clearIconData();
		m_pIconsSkill[iRow]->clearIconData();
	}
	
	// Collect skills
	INDEX	ctPosCommand = 0, ctPosSkill = 0;			// Possible
	INDEX	ctImposCommand = 0, ctImposSkill = 0;		// Impossible	

	std::vector<sCollectSkill>		vectorPosCommand;
	std::vector<sCollectSkill>		vectorImposCommand;
	std::vector<sCollectSkill>		vectorPosSkill;
	std::vector<sCollectSkill>		vectorImposSkill;
	
	vectorPosCommand.resize(SLEARN_SLOT_ROW_TOTAL);
	vectorImposCommand.resize(SLEARN_SLOT_ROW_TOTAL);
	vectorPosSkill.resize(SLEARN_SLOT_ROW_TOTAL);
	vectorImposSkill.resize(SLEARN_SLOT_ROW_TOTAL);

	INDEX iCrrJob = PET_JOB;
	ObjInfo* pInfo = ObjInfo::getSingleton();

	if(pInfo->GetMyPetInfo()->bIsActive)
	{
		iCrrJob = PET_JOB;
	}
	else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
	{
		iCrrJob = WILDPET_JOB;
	}
	
	int iSkill;
	for( iSkill = 1; iSkill <= _pNetwork->wo_iNumOfSkill; iSkill++ )
	{
		CSkill	&rSkill = _pNetwork->GetSkillData( iSkill );
				
		// If job is different
		if( rSkill.GetJob() != iCrrJob)
			continue;
		
		if( ( rSkill.GetJob2() < 0 ) )
		{
			continue;
		}
		// Type ���� 
		if(iCrrJob == PET_JOB)
		{
			if( ( rSkill.GetJob2() % 2 != pInfo->GetMyPetInfo()->iType%2 ) )
			{
				continue;
			}
			
			// Ÿ�� ���϶��� Job 2�̻��� ��ų�� ó�� 
			if( rSkill.GetJob2() < 2 &&  pInfo->GetMyPetInfo()->iAge == 2 )
			{
				continue;
			}
		}else if(iCrrJob == WILDPET_JOB)
		{
			if (pInfo->GetMyApetInfo() != NULL)
			{
				if(!((rSkill.GetJob2() ==pInfo->GetMyApetInfo()->m_nType)&&(rSkill.GetWildpetIndex() == pInfo->GetMyApetInfo()->m_nIndex)))
				{
					continue;
				}
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
				{
					vectorPosCommand[ctPosCommand++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);				
				}
				else
				{
					vectorImposCommand[ctImposCommand++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
				}
			}
			break;
			
		case CSkill::ST_PET_SKILL_PASSIVE:	// pet skill
		case CSkill::ST_PET_SKILL_ACTIVE:
			{
				int petLevel;
				if( iCrrJob == WILDPET_JOB )
					petLevel = pInfo->GetMyApetInfo()->m_nLevel;
				else
					petLevel = pInfo->GetMyPetInfo()->iLevel;

				if( ulNeedCharLevel <= petLevel )
				{
					vectorPosSkill[ctPosSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);		
				}
				else
				{
					vectorImposSkill[ctImposSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
				}
			}
			break;
		case CSkill::ST_MELEE:		// ������ ���� ���� �� ��ų�� ������� ����
			{
				if( ulNeedCharLevel <= pInfo->GetMyApetInfo()->m_nLevel )
				{
					vectorPosSkill[ctPosSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);		
				}
				else
				{
					vectorImposSkill[ctImposSkill++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);					
				}

			}break;
		}
	}
	
	std::sort(vectorPosCommand.begin(), vectorPosCommand.end());
	std::sort(vectorImposCommand.begin(), vectorImposCommand.end());
	std::sort(vectorPosSkill.begin(), vectorPosSkill.end());
	std::sort(vectorImposSkill.begin(), vectorImposSkill.end());
	
	// Add active skill buttons
	iSkill = 0;
	// Possible active skill
	for( iRow = 0; iRow < ctPosCommand; iRow++ )
	{
		m_pIconsCommand[iSkill++]->setData(UBET_SKILL, vectorPosCommand[iRow].lSkillIndex);
	}
	
	// Impossible active skill
	for( iRow = 0; iRow < ctImposCommand; iRow++ )
	{
		m_pIconsCommand[iSkill++]->setData(UBET_SKILL, vectorImposCommand[iRow].lSkillIndex);
	}

	// Set active scroll bar
	m_sbCommandIcon.SetScrollPos( 0 );
	m_sbCommandIcon.SetCurItemCount( iSkill );
	
	// Add passive skill buttons
	iSkill = 0;
	// Possible passive skill
	for( iRow = 0; iRow < ctPosSkill; iRow++ )
	{
		m_pIconsSkill[iSkill++]->setData(UBET_SKILL, vectorPosSkill[iRow].lSkillIndex);
	}

	// Impossible passive skill
	for( iRow = 0; iRow < ctImposSkill; iRow++ )
	{
		m_pIconsSkill[iSkill++]->setData(UBET_SKILL, vectorImposSkill[iRow].lSkillIndex);
	}
	// Set passive scroll bar
	m_sbSkillIcon.SetScrollPos( 0 );
	m_sbSkillIcon.SetCurItemCount( iSkill );

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
}

// ----------------------------------------------------------------------------
// Name : IsNotPetWear()
// Desc : �� ���� ���θ� Ȯ���ϰ� �޼��� â�� ���� �ݴϴ�.
// ----------------------------------------------------------------------------
bool CUIPetTraining::IsNotPetWear()
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
			UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );
		MsgBoxInfo.AddString( _S(2189,"�ֿϵ����� ���Կ� �����Ͽ��߸� �մϴ�.") );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		return true;
	}
	else
	{	return false;	}
}

// ----------------------------------------------------------------------------
// Name : OpenPetTraining()
// Desc : nMasterType�� �Ϲ� ��ų�϶��� ����, Ư�� ��ų�϶��� Ư�� ��ųŸ���� �˴ϴ�.
// ----------------------------------------------------------------------------
void CUIPetTraining::OpenPetTraining( int iMobIndex, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
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
	pUIManager->CreateMessageBoxL( _S(2190, "�ֿϵ��� �Ʒ�" ), UI_PETTRAINING, MSGLCMD_PETTRAINING_REQ );
	
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, TRUE, _S(2191, "������ ���� ������ �����ʴϱ�?" ), -1, 0xA3A1A3FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, TRUE, _S(2192, "���� �Ʒý�Ű�� �ͽ��ϱ�?" ), -1, 0xA3A1A3FF );
	
	CTString strMessage;
	strMessage.PrintF( _S(2193, "�ֿϵ����� �Ʒý�Ų��." ) );
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_TRAINING );

//  060518 Ż�� �� ��
//	if(g_iCountry == JAPAN ||g_iCountry == KOREA || g_iCountry == TAIWAN  || g_iCountry == TAIWAN2 )
	{	
		strMessage.PrintF( _S(2194, "Ż������ �����." ) );
		pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage, PET_CHANGE );		
	}
	
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
	{
		// 2009. 05. 27 ������
		// �̾߱��Ѵ� ���� ó��
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_PETTRAINING_REQ);				
	}


	pUIManager->AddMessageBoxLString(MSGLCMD_PETTRAINING_REQ, FALSE, _S( 1748, "NPC �ȳ�" ), PET_NPC_HELP); //ttos : �ȳ��ý��� �߰���
	strMessage.PrintF( _S( 1220, "����Ѵ�." ) );		
	pUIManager->AddMessageBoxLString( MSGLCMD_PETTRAINING_REQ, FALSE, strMessage );
	
	m_nSelCommandID		= -1;
	m_nSelSkillID	= -1;
}

// ----------------------------------------------------------------------------
// Name : ClosePetTraining()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::ClosePetTraining()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );

	pUIManager->RearrangeOrder( UI_PETTRAINING, FALSE );

	pUIManager->GetInventory()->Lock(FALSE, FALSE, LOCK_PET_TRAINING);
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	//!!!
	//if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	//	ClosePetTraining();

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );


	// Tab
	pDrawPort->AddTexture( m_nPosX + m_rcTab.Left + PT_TAB_WIDTH, m_nPosY + m_rcTab.Top,
		m_nPosX + m_rcTab.Left + PT_TAB_WIDTH + 1, m_nPosY + m_rcTab.Bottom,
		m_rtTabLine.U0, m_rtTabLine.V0, m_rtTabLine.U1, m_rtTabLine.V1,
		0xFFFFFFFF );	

	// Close button
	m_btnClose.Render();

	// Learn button
	m_btnLearn.Render();

	// Cancel button
	m_btnCancel.Render();

	// Scroll bar of skill icon
	if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
		m_sbCommandIcon.Render();
	else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
		m_sbSkillIcon.Render();

	// List box of skill desc
	m_lbSkillDesc.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Skill buttons
	RenderSkillBtns();

	// Text in skill learn
	pDrawPort->PutTextEx( _S(2190, "�ֿϵ��� �Ʒ�" ), m_nPosX + PT_TITLE_TEXT_OFFSETX,
										m_nPosY + PT_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	
	pDrawPort->PutTextExCX( _S(2174, "Ŀ�ǵ�"), m_nPosX + PT_COMMAND_TAB_CX, 
		m_nPosY + PT_TAB_SY,
		m_nCurrentTab == PETTRAINING_TAB_COMMAND ? 0xFFCB00FF : 0x6B6B6BFF );
	pDrawPort->PutTextExCX( _S(91, "��ų" ), m_nPosX + PT_SKILL_TAB_CX,
		m_nPosY + PT_TAB_SY,
		m_nCurrentTab == PETTRAINING_TAB_SKILL ? 0xFFCB00FF : 0x6B6B6BFF );	

	// �̱�ȯ ����(05.01.03) : SP->���õ� �� ����
	pDrawPort->PutTextEx( _S(2181, "��� ����Ʈ" ) , m_nPosX + PT_CURSP_SX,			
										m_nPosY + PT_CURSP_SY );
	
	CTString strAbility;

	if(pInfo->GetMyPetInfo()->lAbility >= 0)
	{
		strAbility.PrintF( "%d", pInfo->GetMyPetInfo()->lAbility );
	}
	else{
		strAbility.PrintF("0");
	}
	pDrawPort->PutTextExRX( strAbility, m_nPosX + PT_CURSP_RX, m_nPosY + PT_CURSP_SY, 0xBDA99FFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : AddSkillDescString()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::AddSkillDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	int		iPos;
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbSkillDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddSkillDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddSkillDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddSkillDescString( strTemp, colDesc );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : GetSkillDesc()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::GetSkillDesc( int nIndex, int nLevel )
{
	m_lbSkillDesc.ResetAllStrings();

	// If skill is not exist
	if( nIndex == -1 )
		return;

	// Make description of skill
	CTString	strTemp;	

	CSkill		&rSkill = _pNetwork->GetSkillData( nIndex );
	
	strTemp.PrintF( "%s", rSkill.GetName() );
	AddSkillDescString( strTemp, 0xFFC672FF );

	if( rSkill.GetFlag() & SF_SINGLEMODE )
		AddSkillDescString( _S( 499, "�۽��δ��� ���� ��ų" ), 0xCACACAFF );

	AddSkillDescString( CTString( " " ) );

	const char	*pDesc = rSkill.GetDescription();
	if( pDesc != NULL )
	{
		strTemp.PrintF( "%s\n\n", pDesc );
		AddSkillDescString( strTemp, 0xC5C5C5FF );
	}
	
	--nLevel;
	
	// Get learning condition
	int		nLearnSkillIndex[3];
	SBYTE	sbLearnSkillLevel[3];
	int		nLearnItemIndex[3];
	int		nLearnItemCount[3];
	BOOL	bLearnSkill = FALSE;
	BOOL	bLearnItem = FALSE;
	
	for( int i = 0; i < 3; i++ )
	{
		nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
		sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
		nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
		nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );
		
		if( nLearnSkillIndex[i] != -1 )
			bLearnSkill = TRUE;
		if( nLearnItemIndex[i] != -1 )
			bLearnItem = TRUE;
	}
	
	//!!! �߰� �۾� �ʿ� 
	switch( rSkill.GetType() )
	{
	case CSkill::ST_MELEE:					// Active
	case CSkill::ST_RANGE:					// Active
	case CSkill::ST_MAGIC:					// Active
		{
			strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), rSkill.GetLearnLevel( nLevel ) );
			AddSkillDescString( strTemp, 0xBDA99FFF );
			strTemp.PrintF( _S( 257, "�ʿ� SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // ���õ�
			AddSkillDescString( strTemp, 0xBDA99FFF );				

			const int iLearnStr = rSkill.GetLearnStr( nLevel );
			const int iLearnDex = rSkill.GetLearnDex( nLevel );
			const int iLearnInt = rSkill.GetLearnInt( nLevel );
			const int iLearnCon = rSkill.GetLearnCon( nLevel );

			if( iLearnStr > 0 ) 
			{
				strTemp.PrintF( _S( 1391, "�ʿ� �� : %d" ), iLearnStr );		// �ʿ� �� 
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}

			if( iLearnDex > 0 ) 
			{
				strTemp.PrintF( _S( 1392, "�ʿ� ��ø : %d" ), iLearnDex );	// �ʿ� ��ø
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			
			if( iLearnInt > 0 ) 
			{
				strTemp.PrintF( _S( 1393, "�ʿ� ���� : %d" ), iLearnInt );	// �ʿ� ����
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}

			if( iLearnCon > 0 )
			{
				strTemp.PrintF( _S( 1394, "�ʿ� ü�� : %d" ), iLearnCon );	// �ʿ� ü��
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			
			// Need skill
			if( bLearnSkill )
			{
				AddSkillDescString( _S( 258, "�ʿ� ��ų" ), 0xBDA99FFF );
				for( int i = 0; i < 3; i++ )
				{
					if( nLearnSkillIndex[i] != -1 )
					{
						CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
						strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}
			}
			
			// Need item
			if( bLearnItem )
			{
				AddSkillDescString( _S( 259, "�ʿ� ������" ), 0xBDA99FFF );
				for( int i = 0; i < 3; i++ )
				{
					if( nLearnItemIndex[i] != -1 )
					{
						strTemp.PrintF( _S( 260, "  %s %d��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), nLearnItemCount[i] );
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}
			}

			int	nNeedMP = rSkill.GetNeedMP( nLevel );
			int	nNeedHP = rSkill.GetNeedHP( nLevel );
			if( nNeedHP == 0 )
			{
				if( nNeedMP != 0 )
				{
					strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
			}
			else
			{
				if( nNeedMP == 0 )
				{
					strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );		
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
				else
				{
					strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
					AddSkillDescString( strTemp, 0xBDA99FFF );
					strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );		
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
			}

			if( rSkill.GetPower( nLevel ) > 0 )
			{
				strTemp.PrintF( _S( 65, "���� : %d" ), rSkill.GetPower( nLevel ) );
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}

			strTemp.PrintF( _S( 66, "��ȿ �Ÿ� : %.1f" ), rSkill.GetFireRange() );
			AddSkillDescString( strTemp, 0xBDA99FFF );
			strTemp.PrintF( _S( 261, "�ִ뽺ų ���� : %d" ), rSkill.GetMaxLevel() );
			AddSkillDescString( strTemp, 0xBDA99FFF );
		}
		break;

	case CSkill::ST_PASSIVE:
	case CSkill::ST_PET_SKILL_PASSIVE:				// Passive
	{
		strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), rSkill.GetLearnLevel( nLevel ) );
		AddSkillDescString( strTemp, 0xBDA99FFF );
		strTemp.PrintF( _S( 257, "�ʿ� SP : %d" ), rSkill.GetLearnSP( nLevel ) ); // ���õ�
		AddSkillDescString( strTemp, 0xBDA99FFF );

		const int iLearnStr = rSkill.GetLearnStr( nLevel );
		const int iLearnDex = rSkill.GetLearnDex( nLevel );
		const int iLearnInt = rSkill.GetLearnInt( nLevel );
		const int iLearnCon = rSkill.GetLearnCon( nLevel );
		
		if( iLearnStr > 0 ) 
		{
			strTemp.PrintF( _S( 1391, "�ʿ� �� : %d" ), iLearnStr );		// �ʿ� ��
			AddSkillDescString( strTemp, 0xBDA99FFF );
		}
		
		if( iLearnDex > 0 ) 
		{
			strTemp.PrintF( _S( 1392, "�ʿ� ��ø : %d" ), iLearnDex );	// �ʿ� ��ø
			AddSkillDescString( strTemp, 0xBDA99FFF );
		}
		
		if( iLearnInt > 0 ) 
		{
			strTemp.PrintF( _S( 1393, "�ʿ� ���� : %d" ), iLearnInt );	// �ʿ� ����
			AddSkillDescString( strTemp, 0xBDA99FFF );
		}
		
		if( iLearnCon > 0 )
		{
			strTemp.PrintF( _S( 1394, "�ʿ� ü�� : %d" ), iLearnCon );	// �ʿ� ü��
			AddSkillDescString( strTemp, 0xBDA99FFF );
		}

		// Need skill
		if( bLearnSkill )
		{
			AddSkillDescString( _S( 258, "�ʿ� ��ų" ), 0xBDA99FFF );
			for( int i = 0; i < 3; i++ )
			{
				if( nLearnSkillIndex[i] != -1 )
				{
					CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );
					strTemp.PrintF( "  %s Lv.%d", rNeedSkill.GetName(), sbLearnSkillLevel[i] );
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
			}
		}

		// Need item
		if( bLearnItem )
		{
			AddSkillDescString( _S( 259, "�ʿ� ������" ), 0xBDA99FFF );
			for( int i = 0; i < 3; i++ )
			{
				if( nLearnItemIndex[i] != -1 )
				{
					strTemp.PrintF( _S( 260, "  %s %d��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), nLearnItemCount[i] );
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}
			}
		}

		strTemp.PrintF( _S( 261, "�ִ뽺ų ���� : %d" ), rSkill.GetMaxLevel() );
		AddSkillDescString( strTemp, 0xBDA99FFF );
	}
	break;
	}
}

// ----------------------------------------------------------------------------
// Name : RenderSkillBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::RenderSkillBtns()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	int	nX = SLEARN_SLOT_SX, nY = SLEARN_SLOT_SY;
	int	iRow, iRowS, iRowE;
	// Active skill tab
	if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
	{
		// Active skill button
		iRowS = m_sbCommandIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{      
			m_pIconsCommand[iRow]->SetPos( nX, nY );
			if( m_pIconsCommand[iRow]->IsEmpty() )		
				continue;
			
			m_pIconsCommand[iRow]->Render(pDrawPort);
		}
	}
	// Passive skill tab
	else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
	{
		// Passive skill button
		iRowS = m_sbSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			m_pIconsSkill[iRow]->SetPos( nX, nY );
			if( m_pIconsSkill[iRow]->IsEmpty() )		
				continue;
			
			m_pIconsSkill[iRow]->Render(pDrawPort);
		}
	}	

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );

	// Outline of selected button
	// Active skill tab
	if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
	{
		iRowS = m_sbCommandIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelCommandID >= 0 && iRowS <= m_nSelCommandID && m_nSelCommandID < iRowE )
		{
			// Set skill learn texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );

			m_pIconsCommand[m_nSelCommandID]->GetAbsPos( nX, nY );
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			pDrawPort->FlushRenderingQueue();
		}
	}
	// Passive skill tab
	else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
	{
		// Passive skill button
		iRowS = m_sbSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelSkillID >= 0 && iRowS <= m_nSelSkillID && m_nSelSkillID < iRowE )
		{
			// Set skill learn texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );

			m_pIconsSkill[m_nSelSkillID]->GetAbsPos( nX, nY );
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			pDrawPort->FlushRenderingQueue();
		}
	}	

	nY = PT_NAME_SY;
	// Active skill tab
	int	nCharLevel = 0;
	
	if(pInfo->GetMyPetInfo()->bIsActive)
	{
		nCharLevel = pInfo->GetMyPetInfo()->iLevel;
	}
	else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->bIsActive)
	{
		nCharLevel = pInfo->GetMyApetInfo()->m_nLevel;
	}
	
	if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
	{
		iRowS = m_sbCommandIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if( m_pIconsCommand[iRow]->IsEmpty() )
				continue;
  
			CSkill	&rSkill = _pNetwork->GetSkillData( m_pIconsCommand[iRow]->getIndex() );
			SBYTE	sbLevel = m_vecCommand[iRow].sbSkillLevel;
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", rSkill.GetName() );
			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + PT_NAME_CX, m_nPosY + nY,
													nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2)
			m_strShortDesc.PrintF( "Lv %2d   %s %2d", sbLevel, _S( 90, "���õ�" ), rSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about japan, german, europe3, europe2, netherlands.
			// [2/28/2013 Ranma] support russia string
#if defined (G_RUSSIA)
			m_strShortDesc.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about russia
			m_strShortDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.

			pDrawPort->PutTextExRX( m_strShortDesc, m_nPosX + PT_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}
	// Passive skill tab
	else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
	{
		iRowS = m_sbSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if (m_pIconsSkill[iRow]->IsEmpty())
				continue;
  
			CSkill	&rSkill = _pNetwork->GetSkillData( m_pIconsSkill[iRow]->getIndex() );
			SBYTE	sbLevel = m_vecSkill[iRow].sbSkillLevel;
			int		nNeedLevel = rSkill.GetLearnLevel( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", rSkill.GetName() );
			pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + PT_NAME_CX, m_nPosY + nY,
													nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );
#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2)
			m_strShortDesc.PrintF( "Lv %2d   %s %2d", sbLevel, _S( 90, "���õ�" ), rSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about japan, german, europe3, europe2, netherlands.
			// [2/28/2013 Ranma] support russia string
#if defined (G_RUSSIA)
			m_strShortDesc.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), rSkill.GetLearnSP( sbLevel - 1 ) );
#else	// else about russia
			m_strShortDesc.PrintF( "Lv %2d   SP %2d", sbLevel, rSkill.GetLearnSP( sbLevel - 1 ) );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.
			pDrawPort->PutTextExRX( m_strShortDesc, m_nPosX + PT_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}	
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPetTraining::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Extended button clicked
	static BOOL	bLButtonDownInBtn = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move skill learn
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Learn button
			else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// List box of skill desc
			else if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Active skill tab
			else if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelCommandID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					bLButtonDownInBtn = FALSE;
				}
				// Active icon scroll bar
				else if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
			// Passive skill tab
			else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
			{
				// Reset state of selected button
				if( bLButtonDownInBtn && m_nSelSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
				{
					bLButtonDownInBtn = FALSE;
				}
				// Passive icon scroll bar
				else if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}			
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Skill tab
				else if( IsInsideRect( nX, nY, m_rcTab ) )
				{
					int	nOldTab = m_nCurrentTab;
					m_nCurrentTab = ( nX - m_nPosX - m_rcTab.Left ) / PT_TAB_WIDTH;
					if( m_nCurrentTab < 0 || m_nCurrentTab > PETTRAINING_TAB_SKILL )
						m_nCurrentTab = nOldTab;

					if( nOldTab != m_nCurrentTab )
					{
						if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
						{
							if( m_nSelCommandID >= 0)
							{
								GetSkillDesc( m_pIconsCommand[m_nSelCommandID]->getIndex(),
												m_vecCommand[m_nSelCommandID].sbSkillLevel );
							}
							else
								GetSkillDesc( -1 );
						}
						else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
						{
							if( m_nSelSkillID >= 0)
							{
								GetSkillDesc( m_pIconsSkill[m_nSelSkillID]->getIndex(),
												m_vecSkill[m_nSelSkillID].sbSkillLevel );
							}
							else
								GetSkillDesc( -1 );
						}
					}
				}
				// Learn button
				else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// List box of skill desc
				else if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Active skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelCommandID;
						m_nSelCommandID = -1;
        
						int	iRowS = m_sbCommandIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_pIconsCommand[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelCommandID = iRow;
								if( nOldSelSkillID != m_nSelCommandID )
								{
									if (m_pIconsCommand[iRow]->IsEmpty() == false)
									{
										GetSkillDesc( m_pIconsCommand[iRow]->getIndex(),
													m_vecCommand[iRow].sbSkillLevel );
									}
								}

								bLButtonDownInBtn = TRUE;

								CUIManager::getSingleton()->RearrangeOrder( UI_PETTRAINING, TRUE );
								return WMSG_SUCCESS;
							}
						}

						GetSkillDesc( -1 );
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	nOldSelSkillID = m_nSelSkillID;
						m_nSelSkillID = -1;
        
						int	iRowS = m_sbSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_pIconsSkill[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
							{
								// Update selected skill
								m_nSelSkillID = iRow;
								if( nOldSelSkillID != m_nSelSkillID )
								{
									if (m_pIconsSkill[iRow]->IsEmpty() == false)
									{
										GetSkillDesc( m_pIconsSkill[iRow]->getIndex(),
													m_vecSkill[iRow].sbSkillLevel );
									}
								}

								bLButtonDownInBtn = TRUE;

								CUIManager::getSingleton()->RearrangeOrder( UI_PETTRAINING, TRUE );
								return WMSG_SUCCESS;
							}
						}

						GetSkillDesc( -1 );
					}
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_PETTRAINING, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bLButtonDownInBtn = FALSE;

			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If skill learn isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ClosePetTraining();

					return WMSG_SUCCESS;
				}
				// Learn button
				else if( ( wmsgResult = m_btnLearn.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						SendLearnSkill();

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						ClosePetTraining();

					return WMSG_SUCCESS;
				}
				// List box of skill desc
				else if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbCommandIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_pIconsCommand[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
					// Skill slot
					else if( IsInsideRect( nX, nY, m_rcIcons ) )
					{
						int	iRowS = m_sbSkillIcon.GetScrollPos();
						int	iRowE = iRowS + SLEARN_SLOT_ROW;
						for( int iRow = iRowS; iRow < iRowE; iRow++ )
						{
							if( m_pIconsSkill[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
								return WMSG_SUCCESS;
						}
					}
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of skill desc
				if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Active skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						// Nothing
					}
				}
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// List box of skill desc
				if( m_lbSkillDesc.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				// Active skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
				{
					// Active icon scroll bar
					if( m_sbCommandIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
				// Passive skill tab
				else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
				{
					// Passive icon scroll bar
					if( m_sbSkillIcon.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
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
		else
		{
		}
		break;
	case MSGCMD_PET_DESTRUCTION:		// �� ������ �Ҹ�( �Ҹ�Ǹ鼭 ������������~~ ) eons
		if( bOK )
		{
			// TODO : ��Ʈ��ũ �޼����� ����.
			_pNetwork->SendPetDestruction();
		}
		else
		{
		}
		CUIManager::getSingleton()->GetInventory()->Lock(FALSE, FALSE, LOCK_PET_TRAINING);
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	CTString strMessage;
	switch( nCommandCode )
	{
	case MSGLCMD_PETTRAINING_REQ:
		if( nResult == PET_TRAINING )				// �ֿϵ����� �Ʒý�Ų��.
		{
			if( IsNotPetWear() )		// �� ���� Ȯ��
				return;

			if (pUIManager->GetInventory()->IsLocked() == TRUE ||
				pUIManager->GetInventory()->IsLockedArrange() == TRUE)
			{
				// �̹� Lock �� â�� ���� ��� ���� ���Ѵ�.
				pUIManager->GetInventory()->ShowLockErrorMessage();
				return;
			}

			pUIManager->GetInventory()->Lock(TRUE, TRUE, LOCK_PET_TRAINING);

			pUIManager->RearrangeOrder( UI_PETTRAINING, TRUE );

			InitPetTraining( );
			m_nCurrentTab = PETTRAINING_TAB_COMMAND;
		}
		else if( nResult == PET_CHANGE )			// Ż������ �����.
		{
			pUIManager->CloseMessageBoxL( MSGLCMD_PETCHANGE_REQ );
			// Create skill learn message box
			pUIManager->CreateMessageBoxL( _S(2196,"�ֿϵ��� ����â"), UI_PETTRAINING, MSGLCMD_PETCHANGE_REQ );	

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
			pUIManager->CreateMessageBoxL( _S(2575,"�ֿϵ��� ��ų �ʱ�ȭ"), UI_PETTRAINING, MSGLCMD_PETSKILLINIT_REQ );	

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
			//TODO : NewQuestSystem
			// ����Ʈ â ����
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == PET_STUFF ) // �� ��� ������ ��ȯ eons
		{
			pUIManager->CloseMessageBoxL( MSGLCMD_PETSTUFFCHANGE_REQ );
			pUIManager->CreateMessageBoxL( _S( 2467, "�� ��� ������ ��ȯ" ), UI_PETTRAINING, MSGLCMD_PETSTUFFCHANGE_REQ );
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
			pUIManager->CreateMessageBoxL( _S( 2473, "������ ����" ), UI_PETTRAINING, MSGLCMD_PETUNIQUEPRODUCT_REQ );
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
					UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );
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
			MsgBoxInfo.SetMsgBoxInfo( _S(2200, "Ż �� �Ʒ�" ), UMBS_OKCANCEL, UI_PETTRAINING, MSGCMD_PET_CHANGE );
			strMessage.PrintF( _S(2201, "���� Ż �� �Ʒ��� �Ͻðڽ��ϱ�?" ) );
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		else
		{
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
				MsgBoxInfo.SetMsgBoxInfo( _S(191,"Ȯ��"), UMBS_OKCANCEL, UI_PETTRAINING, MSGCMD_PET_DESTRUCTION );
				strMessage.PrintF( _S( 2476, "���� ���������� ��ȯ �Ͻðڽ��ϱ�?") );
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		else
		{
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
		else
		{
		}
		break;
	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::SendLearnSkill()
{
	// Close message box of skill learn
	CUIManager::getSingleton()->CloseMessageBox( MSGCMD_PETTRAINING_NOTIFY );
	ObjInfo* pInfo = ObjInfo::getSingleton();

	SLONG	slIndex;
	if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
	{
		if( m_nSelCommandID < 0 )
			return;

		if( m_pIconsCommand[m_nSelCommandID]->IsEmpty() )
			return;

		slIndex = m_pIconsCommand[m_nSelCommandID]->getIndex();
	}
	else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
	{
		if( m_nSelSkillID < 0 )
			return;

		if( m_pIconsSkill[m_nSelSkillID]->IsEmpty() )
			return;
		
		slIndex = m_pIconsSkill[m_nSelSkillID]->getIndex();
	}	

	if( pInfo->GetMyPetInfo()->bIsActive)
	{
		_pNetwork->LearnPetSkill( slIndex );
	}else if(pInfo->GetMyApetInfo() != NULL && pInfo->GetMyApetInfo()->m_nIdxServer > 0 )
	{
		_pNetwork->LearnWildPetSkill( slIndex );
	}
}


// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::LearnSkill( int nPetIdx, SLONG slIndex, SBYTE sbLevel, BOOL bAutoLearn, bool bShowMsg )
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
		MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK, UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );
	
		if( bAutoLearn ) 
		{
			strMessage.PrintF( _S(2202, "���ο� �米����(%s)�� �����Ͽ����ϴ�." ), rSkill.GetName() );
		}
		else 
		{
			strMessage.PrintF( _S( 277, "%s ��ų�� �����Ͽ����ϴ�" ), rSkill.GetName() );
		}
	
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}

	MY_INFO()->SetPetSkill(nPetIdx, slIndex, sbLevel);

	if(rSkill.GetJob() == PET_JOB)
	{
		int nPetIdx = MY_PET_INFO()->lIndex;

		if (pUIManager->GetPetInfo()->IsLearnSkill(nPetIdx, slIndex) == FALSE)
		{
			pUIManager->GetPetInfo()->AddSkill( nPetIdx, slIndex, sbLevel );
		}		
	}
	else if(rSkill.GetJob() == WILDPET_JOB)
	{
		if(pUIManager->GetWildPetInfoUI()->IsLearnSkill(slIndex) == FALSE)
		{
			pUIManager->GetWildPetInfoUI()->AddSkill(slIndex,sbLevel);
		}
	}	
	
	BOOL	bUpdate = FALSE;
	
	// Not Special Skill
	{
		int i, nSize = m_vecCommand.size();

		for (i = 0; i < nSize; ++i)
		{
			if (m_vecCommand[i].lSkillIndex == slIndex)
			{
				m_vecCommand[i].sbSkillLevel = sbLevel + 1;
				bUpdate = TRUE;
				break;
			}
		}
	}
	
	if( !bUpdate )
	{
		int i, nSize = m_vecSkill.size();

		for (i = 0; i < nSize; ++i)
		{
			if (m_vecSkill[i].lSkillIndex == slIndex)
			{
				m_vecSkill[i].sbSkillLevel = sbLevel + 1;
				break;
			}
		}
	}

	if( sbLevel >= rSkill.GetMaxLevel() )
	{
		if( m_nCurrentTab == PETTRAINING_TAB_COMMAND )
			m_nSelCommandID = -1;
		else if( m_nCurrentTab == PETTRAINING_TAB_SKILL )
			m_nSelSkillID = -1;			

		GetSkillDesc( -1 );

		InitPetTraining( );

		return;
	}

	GetSkillDesc( slIndex, sbLevel + 1 );
}

// ----------------------------------------------------------------------------
// Name : LearnSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::LearnSkillError( UBYTE ubError )
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
								UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : PetChangeItemError()
// Desc :
// ----------------------------------------------------------------------------
void CUIPetTraining::PetChangeItemError( SBYTE sbResult )
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

	ClosePetTraining();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1519, "��ȯ" ), UMBS_OK, UI_PET_ITEM_MIX, MSG_EX_PET_MIX_ITEM );		

	MsgBoxInfo.AddString( strMessageA );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

void CUIPetTraining::EvolutionError(UBYTE errorcode)
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
		// [091013: selo] ������ ���� ��ȭ ������ ������ �Ǹ� Notice �� �˸���.
	case 5:
		{
			Notice* pNotice = GAMEDATAMGR()->GetNotice();

			if (pNotice != NULL)
				pNotice->AddToNoticeList(4020, Notice::NOTICE_EVENT);

			return;
		}		
	}

	ClosePetTraining();

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(4751, "��ȭ" ), UMBS_OK, UI_PETTRAINING, MSGCMD_PETTRAINING_NOTIFY );		

	MsgBoxInfo.AddString( strMessageA );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}