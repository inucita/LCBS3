#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/Base/UIDurability.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/ItemComposeUI.h>

#define	SLEARN_TAB_WIDTH				96
#define	SLEARN_ACTIVE_TAB_CX			60
#define	SLEARN_PASSIVE_TAB_CX			156

#define	SLEARN_TAB_SY					34
#define	SLEARN_SPECIAL_TAB_CX			108

#define SKILL_LEARNED_COLOR				0xF2F2F2FF
#define NOT_SATISFACTION_COL			0xFF0000FF
#define SATISFACTION_COL				0xFFFF00FF

/*
typedef struct tagChangeJobCondition
{
	int		iLevel;					// �ʿ� ����
	int		iNeedSkill;				// �ʿ� ��ų
	int		iNeedSkillLevel;		// �ʿ� ��ų ����

}sJobCondition;

static sJobCondition _JobCondition[TOTAL_JOB] =
{
	{	35, 0, 1,	},		// TITAN
	{	35, 0, 1,	},		// KNIGHT
	{	35, 0, 1,	},		// HEALER
	{	35, 0, 1,	},		// MAGE
	{	35, 0, 1,	},		// ROGUE
};
*/

enum eSelection
{
	SKILL_LEARN,
	SKILL_TALK,
	SKILL_EVENT,
	SKILL_JOB_1,			// �����ϱ� 1��
	SKILL_JOB_2,			// �����ϱ� 2��
	SKILL_NPC_HELP,
	SKILL_TITAN,
	SKILL_KNIGHT,
	SKILL_HEALER,
	SKILL_MAGE,
	SKILL_ROGUE,
	SKILL_SORCERER,
	SKILL_NIGHTSHADOW,
#ifdef CHAR_EX_ROGUE
	SKILL_EX_ROGUE,	// [2012/08/27 : Sora] EX�α� �߰�
#endif
#ifdef CHAR_EX_MAGE
	SKILL_EX_MAGE,	// 2013/01/08 jeil EX������ �߰� 
#endif
	SKILL_QUEST,
#ifdef DURABILITY
	ITEM_DURABILITY,
	ITEM_RECOVERY,
#endif	//	DURABILITY
	ITEM_COMPOSE
};

enum eSatisfactionType
{
	SKILL_CONDITION_LEVEL = 0,
	SKILL_CONDITION_SP,
	SKILL_CONDITION_STR,
	SKILL_CONDITION_DEX,
	SKILL_CONDITION_INT,
	SKILL_CONDITION_CON,
	SKILL_CONDITION_SKILL_0,
	SKILL_CONDITION_SKILL_1,
	SKILL_CONDITION_SKILL_2,
	SKILL_CONDITION_ITEM_0,
	SKILL_CONDITION_ITEM_1,
	SKILL_CONDITION_ITEM_2,
};

extern int _aSummonSkill[5];

#define EXTENSION_LEVEL		(31)

// [KH_07044] 3�� ���� ���� �߰�
extern INDEX g_iShowHelp1Icon;


// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE
#define	SKILLLEARN_NEW_SLOT_SX				16
#define	SKILLLEARN_NEW_SLOT_SY				68
#define	SKILLLEARN_NEW_SLOT_OFFSETY				36
#define	SKILLLEARN_NEW_NAME_CX					132
#define	SKILLLEARN_NEW_NAME_SY					70
#define	SKILLLEARN_NEW_NEED_RX					174

#define	SKILLLEARN_NEW_ACTIVE_TAB_CX			64
#define	SKILLLEARN_NEW_PASSIVE_TAB_CX			192
#define	SKILLLEARN_NEW_TAB_SY					43

#define	SKILLLEARN_NEW_TAB_WIDTH				122

#define	SKILLLEARN_NEW_CURSP_SX					98
#define	SKILLLEARN_NEW_CURSP_RX					243
#define	SKILLLEARN_NEW_CURSP_SY					290

#define SKILL_NOT_EXIST					-1

#define SKILLINFO_LINE_MAX				20


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// NEW_USER_INTERFACE

void CUISkillLearn::CreateNewSkillLearn(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	//Ÿ��Ʋ ���콺 �巡�� ����
	m_rcTitle.SetRect( 0, 0, SKILLLEARN_NEW_WIDTH, 36 );

	//������ ǥ�� ����
	m_rcIcons.SetRect( 11, 65, 231, 285 );

	//�ǿ���
	m_rcTab.SetRect( 6, 37 , 251, 57 );

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewCharacterInfo.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	//���
	m_rtBackground.SetUV(0, 649, 256, 990, fTexWidth, fTexHeight );

	m_ptdButtonTexture = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );
	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();


	//���õ��׵θ�
	m_rtSelOutlineTopL.SetUV( 145, 138, 157, 150, fTexWidth, fTexHeight );;
	m_rtSelOutlineTopM.SetUV( 157, 138, 226, 150, fTexWidth, fTexHeight );;
	m_rtSelOutlineTopR.SetUV( 226, 138, 239, 150, fTexWidth, fTexHeight );;
	m_rtSelOutlineMiddleL.SetUV( 145, 150, 157, 158, fTexWidth, fTexHeight );;
	m_rtSelOutlineMiddleM.SetUV( 157, 150, 226, 158, fTexWidth, fTexHeight );;
	m_rtSelOutlineMiddleR.SetUV( 226, 150, 239, 158, fTexWidth, fTexHeight );;
	m_rtSelOutlineBottomL.SetUV( 145, 158, 157, 171, fTexWidth, fTexHeight );;
	m_rtSelOutlineBottomM.SetUV( 157, 158, 226, 171, fTexWidth, fTexHeight );;
	m_rtSelOutlineBottomR.SetUV( 226, 158, 239, 171, fTexWidth, fTexHeight );;

	m_rtSelectedTab.SetUV(0, 106, 100, 127, fTexWidth, fTexHeight );
	m_rtUnSelectedTab.SetUV(104, 106, 204, 127, fTexWidth, fTexHeight );


	m_btnClose.Create( this, CTString( "" ), 232, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK,229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_DISABLE,155, 74, 169, 88, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );

	// Learn button
	m_btnLearn.Create( this, _S( 269, "����" ), 11, 313, 89, 22 );
	m_btnLearn.SetUV( UBS_IDLE, 113, 0, 182, 21, fTexWidth, fTexHeight );
	m_btnLearn.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnLearn.SetUV( UBS_DISABLE, 145, 178, 214, 199, fTexWidth, fTexHeight );
	m_btnLearn.CopyUV( UBS_IDLE, UBS_ON );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "���" ), 156, 313, 89, 22 );
	m_btnCancel.SetUV( UBS_IDLE, 113, 0, 182, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 186, 0, 255, 21, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_DISABLE, 145, 178, 214, 199, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );

	m_sbScrollBar.Create( this, 232, 67, 10, 213 );
	m_sbScrollBar.CreateButtons( TRUE, 10, 10, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( SKILLLEARN_NEW_SLOT_ROW_TOTAL );
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetItemsPerPage( SKILLLEARN_NEW_SLOT_ROW );

	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 156, 33, 166, 43, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 168, 33, 178, 43, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 185, 32, 195, 42, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 185, 41, 195, 51, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 185, 61, 195, 71, fTexWidth, fTexHeight );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 156, 45, 166, 55, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 168, 45, 178, 55, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -180, 0, 179, 322 );

	// Active skill
	int		iRow;
	// Passive skill
	for( iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL ; iRow++ )
	{
		m_pIconsSpecialSkill[iRow] = new CUIIcon;
		m_pIconsSpecialSkill[iRow]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SKILLLEARN, UBET_SKILL);

	}

	// �ۺκ��� Ŭ���ص� ���õǰ� ��
	m_rcButtonArea.SetRect(0, 0, 212, 34);

}

void CUISkillLearn::RenderNewSkillLearn()
{
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST && !m_bUseCard)
		CloseSkillLearn();

	int nPosX, nPosY;

	GetAbsPos(nPosX, nPosY);

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	pDrawPort->AddTexture( nPosX, nPosY, nPosX + SKILLLEARN_NEW_WIDTH, nPosY + SKILLLEARN_NEW_HEIGHT,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );
	
	pDrawPort->FlushRenderingQueue();
 

	pDrawPort->FlushRenderingQueue();

	pDrawPort->InitTextureData( m_ptdButtonTexture );
	nPosX = m_nPosX + m_rcTab.Left;
	nPosY = m_nPosY + m_rcTab.Top;
	

	// ��ų �� ǥ��
	if(m_nCurrentSkillType == SKILL_ACTIVE)
	{
		pDrawPort->AddTexture( nPosX , nPosY, nPosX + (m_rcTab.GetWidth()/2)-1, nPosY + m_rcTab.GetHeight(),
						m_rtSelectedTab.U0, m_rtSelectedTab.V0, m_rtSelectedTab.U1, m_rtSelectedTab.V1,
						0xFFFFFFFF );

		pDrawPort->AddTexture( nPosX + (m_rcTab.GetWidth()/2)+1, nPosY, nPosX + m_rcTab.GetWidth(), nPosY + m_rcTab.GetHeight(),
					m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
					0xFFFFFFFF );
	}
	else if(m_nCurrentSkillType == SKILL_PASSIVE)
	{
		pDrawPort->AddTexture( nPosX , nPosY, nPosX + (m_rcTab.GetWidth()/2)-1, nPosY + m_rcTab.GetHeight(),
						m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
						0xFFFFFFFF );

		pDrawPort->AddTexture( nPosX + (m_rcTab.GetWidth()/2)+1, nPosY, nPosX + m_rcTab.GetWidth(), nPosY + m_rcTab.GetHeight(),
					m_rtSelectedTab.U0, m_rtSelectedTab.V0, m_rtSelectedTab.U1, m_rtSelectedTab.V1,
					0xFFFFFFFF );
	}
	else
	{
		pDrawPort->AddTexture( nPosX , nPosY, nPosX + (m_rcTab.GetWidth()/2)-1, nPosY + m_rcTab.GetHeight(),
						m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
						0xFFFFFFFF );

		pDrawPort->AddTexture( nPosX + (m_rcTab.GetWidth()/2)+1, nPosY, nPosX + m_rcTab.GetWidth(), nPosY + m_rcTab.GetHeight(),
					m_rtUnSelectedTab.U0, m_rtUnSelectedTab.V0, m_rtUnSelectedTab.U1, m_rtUnSelectedTab.V1,
					0xFFFFFFFF );
	}

	pDrawPort->FlushRenderingQueue();

//	int iRow;
	int iRowS = m_sbScrollBar.GetScrollPos();
	int iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
	int	nX;
	int nY;

	m_btnClose.Render();

	m_btnLearn.Render();

	m_btnCancel.Render();

	m_sbScrollBar.Render();

	pDrawPort->FlushRenderingQueue();

	RenderNewSkillLearnBtns();



	// ���� ���� ǥ��
	pDrawPort->InitTextureData( m_ptdButtonTexture );
	iRowS = m_sbScrollBar.GetScrollPos();
	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
	if( m_nSelectedSkillID >= 0 && iRowS <= m_nSelectedSkillID && m_nSelectedSkillID < iRowE )
	{

		m_ppIconsSelectedSkill[m_nSelectedSkillID]->GetAbsPos( nX, nY );


		pDrawPort->AddTexture( nX - 1, nY - 1, nX + 12, nY + 12,
											m_rtSelOutlineTopL.U0, m_rtSelOutlineTopL.V0, m_rtSelOutlineTopL.U1, m_rtSelOutlineTopL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 12, nY - 1, nX + 198, nY + 12,
											m_rtSelOutlineTopM.U0, m_rtSelOutlineTopM.V0, m_rtSelOutlineTopM.U1, m_rtSelOutlineTopM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 198, nY - 1, nX + 211, nY + 12,
											m_rtSelOutlineTopR.U0, m_rtSelOutlineTopR.V0, m_rtSelOutlineTopR.U1, m_rtSelOutlineTopR.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( nX - 1, nY + 12, nX + 12, nY + 20,
											m_rtSelOutlineMiddleL.U0, m_rtSelOutlineMiddleL.V0, m_rtSelOutlineMiddleL.U1, m_rtSelOutlineMiddleL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 12, nY + 12, nX + 198, nY + 20,
											m_rtSelOutlineMiddleM.U0, m_rtSelOutlineMiddleM.V0, m_rtSelOutlineMiddleM.U1, m_rtSelOutlineMiddleM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 198, nY + 12, nX + 211, nY + 20,
											m_rtSelOutlineMiddleR.U0, m_rtSelOutlineMiddleR.V0, m_rtSelOutlineMiddleR.U1, m_rtSelOutlineMiddleR.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( nX - 1, nY + 20, nX + 12, nY + 33,
											m_rtSelOutlineBottomL.U0, m_rtSelOutlineBottomL.V0, m_rtSelOutlineBottomL.U1, m_rtSelOutlineBottomL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 12, nY + 20, nX + 198, nY + 33,
											m_rtSelOutlineBottomM.U0, m_rtSelOutlineBottomM.V0, m_rtSelOutlineBottomM.U1, m_rtSelOutlineBottomM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( nX + 198, nY + 20, nX + 211, nY + 33,
											m_rtSelOutlineBottomR.U0, m_rtSelOutlineBottomR.V0, m_rtSelOutlineBottomR.U1, m_rtSelOutlineBottomR.V1,
											0xFFFFFFFF );

	}

	if(m_nSelectedSkillID != -1)
	{
		if(m_pSelectedSkillSatisfied[m_nSelectedSkillID] == ALL_SATISFIED)
		{
			if(!m_btnLearn.IsEnabled())
			{
				m_btnLearn.SetEnable(TRUE);
			}
		}
		else
		{
			if(m_btnLearn.IsEnabled())
			{
				m_btnLearn.SetEnable(FALSE);
			}
		}
	}
	else
	{
		if(m_btnLearn.IsEnabled())
		{
			m_btnLearn.SetEnable(FALSE);
		}
	}

	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextExCX( _S(91, "��ų") , m_nPosX + (SKILLLEARN_NEW_WIDTH / 2),			
										m_nPosY + 15 );

	pDrawPort->EndTextEx();
}

void CUISkillLearn::RenderNewSkillLearnBtns()
{
	if( m_nCurrentSkillType != SKILL_SPECIAL )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int	nX = SKILLLEARN_NEW_SLOT_SX, nY = SKILLLEARN_NEW_SLOT_SY;
	int	iRow, iRowS, iRowE;

	///��ư ������
	iRowS = m_sbScrollBar.GetScrollPos();		
	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;

	for( iRow = iRowS; iRow < iRowE; iRow++, nY += SKILLLEARN_NEW_SLOT_OFFSETY )
	{      
		if( m_ppIconsSelectedSkill[iRow]->IsEmpty() )		
			continue;

		m_ppIconsSelectedSkill[iRow]->SetPos( nX, nY );

		if(m_pSelectedSkillSatisfied[iRow] == ALL_SATISFIED)
			m_ppIconsSelectedSkill[iRow]->Render(pDrawPort);
		else
		{
			m_ppIconsSelectedSkill[iRow]->Render(pDrawPort);
		}
	}

	pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );

	//�̸� ���
	nY = SKILLLEARN_NEW_NAME_SY;
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	SQUAD nCharSP		= _pNetwork->MyCharacterInfo.sp;
	iRowS = m_sbScrollBar.GetScrollPos();
	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
 	for( iRow = iRowS; iRow < iRowE; iRow++, nY += SKILLLEARN_NEW_SLOT_OFFSETY )
 	{
		if( m_ppIconsSelectedSkill[iRow]->IsEmpty() )
			continue;

		CSpecialSkill* pSSkill = CSpecialSkill::getData( m_ppIconsSelectedSkill[iRow]->getIndex() );

		if (pSSkill == NULL)
			return;

		SBYTE	sbLevel = MY_INFO()->GetSkillLevel(pSSkill->GetIndex(), true);
		int		nNeedLevel = pSSkill->GetLearnLevel( sbLevel );
		int		nlearnSp = pSSkill->GetLearnSP( sbLevel );

		sbLevel += 1;

		m_strDesc.PrintF( "%s", pSSkill->GetName() );
		pDrawPort->PutTextExCX( m_strDesc, m_nPosX + SKILLLEARN_NEW_NAME_CX, m_nPosY + nY,
												nCharLevel >= nNeedLevel ? 0xFFC672FF : 0xBCBCBCFF );

#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2)
		m_strDesc.PrintF( "Lv %2d   %s %2d", sbLevel, _S( 90, "���õ�" ), nlearnSp );
#else	// else about japan, german, europe3, europe2, netherlands.
		// [2/28/2013 Ranma] support russia string
#if defined (G_RUSSIA)
		m_strDesc.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), nlearnSp );
#else	// else about russia
		m_strDesc.PrintF( "Lv %2d   SP %2d", sbLevel, nlearnSp );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.
		pDrawPort->PutTextExRX( m_strDesc, m_nPosX + SKILLLEARN_NEW_NEED_RX,
												m_nPosY + nY + 17, 0xBDA99FFF );
 	}

	if( m_nCurrentSkillType == SKILL_SPECIAL )
	{
		CTString	strSubTitle;
		switch( m_nSSkillType )
		{
		case SSKILL_MINING:		// ä��
			strSubTitle = _S( 630, "ä�� ��ų" );		
			break;

		case SSKILL_GATHERING:	// ä��
			strSubTitle = _S( 633, "ä�� ��ų" );		
			break;

		case SSKILL_CHARGE:		// ����
			strSubTitle = _S( 636, "���� ��ų" );		
			break;

		case SSKILL_STONE:		// ��������
			strSubTitle = _S( 639, "�������� ��ų" );	
			break;

		case SSKILL_PLANT:		// �Ĺ�����
			strSubTitle = _S( 642, "�Ĺ����� ��ų" );	
			break;

		case SSKILL_ELEMENT:	// ��������
			strSubTitle = _S( 645, "�������� ��ų" );	
			break;

		case SSKILL_MAKE_WEAPON:	// ��������
			strSubTitle = _S( 648, "�������� ��ų" );	
			break;
		case SSKILL_MAKE_WEAR:	// ������
		case SSKILL_MAKE_G_B:
		case SSKILL_MAKE_ARMOR:
		case SSKILL_MAKE_H_S:
			strSubTitle = _S( 651, "������ ��ų" );	
			break;
			// ���� ���� ���� �� ���� �߰�. [8/26/2010 rumist]
		case SSKILL_MAKE_POTINO:
			strSubTitle = _S( 767, "���� ���� ��ų" );	
			break;
		}
		pDrawPort->PutTextExCX( strSubTitle, m_nPosX + SKILLLEARN_NEW_ACTIVE_TAB_CX, m_nPosY + SKILLLEARN_NEW_TAB_SY, 0xFFCB00FF);
	}

	pDrawPort->PutTextEx(  _S( 90, "���õ�" ) , m_nPosX + SKILLLEARN_NEW_CURSP_SX,			
										m_nPosY + SKILLLEARN_NEW_CURSP_SY,  0xDED9A0FF);

	CTString strString;
	strString.PrintF( "%d", _pNetwork->MyCharacterInfo.sp / 10000 );
	pDrawPort->PutTextExRX( strString, m_nPosX + SKILLLEARN_NEW_CURSP_RX, m_nPosY + SKILLLEARN_NEW_CURSP_SY-2, 0xBDA99FFF );

	pDrawPort->EndTextEx();
}


WMSG_RESULT CUISkillLearn::SKillLearnNewMouseMessage(MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	static BOOL bTitleBarClick = FALSE;

	static BOOL	bLButtonDownInBtn = FALSE;

	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	switch( pMsg->message )
	{
		case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();


			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move inventory
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			if( IsInsideRect( nX, nY, m_rcIcons ) )
			{
				int	iRow;
				int	iRowS = m_sbScrollBar.GetScrollPos();
				int	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
				int	nWhichRow = -1;
//				### skill new			
				for( iRow = iRowS; iRow < iRowE; iRow++ )
				{
					if( m_ppIconsSelectedSkill[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
						nWhichRow = iRow;
				}

				// Show tool tip
				if( nWhichRow != -1 )
				{
					m_bSkillInfoVisible = TRUE;
//					ShowSkillLearnInfo(nWhichRow);
				}
				else
					m_bSkillInfoVisible = FALSE;

				return WMSG_SUCCESS;
			}
			else
			{
				m_bSkillInfoVisible = FALSE;
			}

			if( bLButtonDownInBtn && m_nSelectedSkillID >= 0 && ( pMsg->wParam & MK_LBUTTON ) )
			{
				bLButtonDownInBtn = FALSE;
			}
			// Active icon scroll bar
			else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;
		
		case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ))
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					
				}
				// Title bar
				if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				if( m_btnLearn.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Skill slot
				if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	nOldSelSkillID = m_nSelectedSkillID;
					m_nSelectedSkillID = -1;
    
					int	iRow;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
					int	nWhichRow = -1;

					int nPosX = 0;
					int nPosY = 0;
					int nWidth = m_rcButtonArea.GetWidth();
					int nHeight = m_rcButtonArea.GetHeight();

					iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;
//					### skill new
					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if (m_ppIconsSelectedSkill[iRow]->IsEmpty() == true)
							continue;

						if( m_ppIconsSelectedSkill[iRow]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Update selected skill
							m_nSelectedSkillID = iRow;

							bLButtonDownInBtn = TRUE;
							pUIManager->RearrangeOrder( UI_SKILLLEARN, TRUE );
							return WMSG_SUCCESS;
						}

						if(!m_ppIconsSelectedSkill[iRow]->IsEmpty())
						{
							nPosX =  m_ppIconsSelectedSkill[iRow]->GetPosX();
							nPosY =  m_ppIconsSelectedSkill[iRow]->GetPosY();
							m_rcButtonArea.SetRect(nPosX, nPosY, nPosX + nWidth, nPosY + nHeight);
							if(IsInsideRect(nX, nY, m_rcButtonArea))
							{
								if(m_nSelectedSkillID != iRow)
								{
									m_nSelectedSkillID = iRow;
								}
								pUIManager->RearrangeOrder( UI_SKILLLEARN, TRUE );
								return WMSG_SUCCESS;
							}
						}

					}
				}
				if( IsInsideRect( nX, nY, m_rcTab ) )
				{
					if( m_nCurrentSkillType != SKILL_SPECIAL )
					{
						int nState = ( nX - m_nPosX - m_rcTab.Left ) / SKILLLEARN_NEW_TAB_WIDTH;
						
						SetCurrentSkill(nState);
					}
				}

				pUIManager->RearrangeOrder( UI_SKILLLEARN, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInBtn = FALSE;

			if (pUIManager->GetDragIcon() == NULL)
			 {
				// Title bar
				bTitleBarClick = FALSE;

				// If character information isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CloseSkillLearn();
					}

					return WMSG_SUCCESS;
				}
				// Learn button
				if( ( wmsgResult = m_btnLearn.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						SendLearnSkill();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						CloseSkillLearn();
					}

					return WMSG_SUCCESS;
				}
										
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
						return WMSG_SUCCESS;
				else if( IsInsideRect( nX, nY, m_rcIcons ) )
				{
					int	iRow;
					int	iRowS = m_sbScrollBar.GetScrollPos();
					int	iRowE;
					iRowE = iRowS + SKILLLEARN_NEW_SLOT_ROW;

					for( iRow = iRowS; iRow < iRowE; iRow++ )
					{
						if( ( wmsgResult =  m_ppIconsSelectedSkill[iRow]->MouseMessage( pMsg ) ) != WMSG_FAIL )
						{
						}
							return WMSG_SUCCESS;
					}
				}
			 }
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
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				return WMSG_SUCCESS;
			}
		}
		break;

		case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

//////////////////////////////////////////////////////////////////////////
//
// ���� ��ų�� ������ �����ϴ����� üũ�մϴ�.
//
//////////////////////////////////////////////////////////////////////////
int CUISkillLearn::IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial )
{
	int nSatisfied = 0;

	if( nSkillIndex <= 0 )
		return SKILL_NOT_EXIST;

	if (nLevel > 0)
		--nLevel; // ������ ������ 1����, ��ų������ ���� �迭�� 0 ����

	if (nLevel < 0)
	{
		LOG_DEBUG("��ų ������ %d �Դϴ�.", nLevel);
		return SKILL_NOT_EXIST;
	}

	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );

		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
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

		// �ʿ� ���� üũ
		if(_pNetwork->MyCharacterInfo.level < rSkill.GetLearnLevel( nLevel ))
			nSatisfied |= NOT_SATISFIED_LEVEL;
		// �ʿ� ���õ� üũ
		if((_pNetwork->MyCharacterInfo.sp / 10000) < rSkill.GetLearnSP( nLevel ))
			nSatisfied |= NOT_SATISFIED_SP;
		// �ʿ� �ɷ�ġ üũ

		const int iLearnStr = rSkill.GetLearnStr( nLevel );
		const int iLearnDex = rSkill.GetLearnDex( nLevel );
		const int iLearnInt = rSkill.GetLearnInt( nLevel );
		const int iLearnCon = rSkill.GetLearnCon( nLevel );

		if( iLearnStr > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.str - _pNetwork->MyCharacterInfo.opt_str  < iLearnStr )
				nSatisfied |= NOT_SATISFIED_STR;
		}

		if( iLearnDex > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.dex - _pNetwork->MyCharacterInfo.opt_dex < iLearnDex )
				nSatisfied |= NOT_SATISFIED_DEX;
		}

		if( iLearnInt > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.intel - _pNetwork->MyCharacterInfo.opt_intel < iLearnInt )
				nSatisfied |= NOT_SATISFIED_INT;
		}

		if( iLearnCon > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.con - _pNetwork->MyCharacterInfo.opt_con < iLearnCon )
				nSatisfied |= NOT_SATISFIED_CON;
		}

		CUIManager* pUIManager = CUIManager::getSingleton();

		// �ʿ� ��ų üũ
		if( bLearnSkill )
		{
			for( int i = 0; i < 3; i++ )
			{
				if (nLearnSkillIndex[i] != -1)
				{
					//�� ��ų �������� ����� ã��.
					// ĳ���� �������ΰ�?
					int nCurrentSkillLevel = MY_INFO()->GetSkillLevel(nLearnSkillIndex[i]);
					if(nCurrentSkillLevel < sbLearnSkillLevel[i])
					{
						nSatisfied |= (NOT_SATISFIED_SKILL_0 << i);
					}
				}
			}
		}

		// �ʿ� ������ üũ
		if( bLearnItem )
		{
			for( int i = 0; i < 3; i++ )
			{
				//2013/04/10 jeil
				if( nLearnItemIndex[i] != -1 )
				{
					if(nLearnItemIndex[i] == 19)
					{
						if(_pNetwork->MyCharacterInfo.money < nLearnItemCount[i])
						{
							nSatisfied |= (NOT_SATISFIED_ITEM_0 << i);
						}
					}
					else
					{
						SQUAD nCurrentItemCount = pUIManager->GetInventory()->GetItemCount(nLearnItemIndex[i]);
						if(nCurrentItemCount < nLearnItemCount[i])
						{
							nSatisfied |= (NOT_SATISFIED_ITEM_0 << i);
						}
					}
				}
			}
		}
	}
	else
	{
		CSpecialSkill* pSSkill = CSpecialSkill::getData( nSkillIndex );

		if (pSSkill == NULL)
			return SKILL_NOT_EXIST;

		// Get learning condition
		int		nLearnSkillIndex;
		SBYTE	sbLearnSkillLevel;
		
		nLearnSkillIndex	= pSSkill->GetLearnSkillIndex();
		sbLearnSkillLevel	= pSSkill->GetLearnSkillLevel();
		
		// �ʿ� ���� üũ
		if(_pNetwork->MyCharacterInfo.level < pSSkill->GetLearnLevel( nLevel ))
			nSatisfied |= NOT_SATISFIED_LEVEL;
		// �ʿ� ���õ� üũ
		if((_pNetwork->MyCharacterInfo.sp / 10000) < pSSkill->GetLearnSP( nLevel ))
			nSatisfied |= NOT_SATISFIED_SP;

		if( nLearnSkillIndex != -1 )
		{
			int nCurrentSkillLevel = MY_INFO()->GetSkillLevel(nLearnSkillIndex, true);
			if(nCurrentSkillLevel < sbLearnSkillLevel)
			{
				nSatisfied |= NOT_SATISFIED_SKILL_0;
			}
		}
	}

	return nSatisfied;
}


void CUISkillLearn::UpdateSkillLearn()
{
	m_strDesc.Clear();

	int		iRow;
	for( iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++ )
	{

		m_nActiveSkillSatisfied[iRow] = 0;
		m_nPassiveSkillSatisfied[iRow] = 0;
		m_nSpecialSkillSatisfied[iRow] = 0;
	}

	for( iRow = 0; iRow < SKILLLEARN_NEW_SLOT_TOTAL; iRow++)
	{
		if (m_pIconsSpecialSkill[iRow]->IsEmpty() == true)
			break;
		
		m_nSpecialSkillSatisfied[iRow] = IsSatisfiedSkill(m_pIconsSpecialSkill[iRow]->getIndex(), 
			MY_INFO()->GetSkillLevel(m_pIconsSpecialSkill[iRow]->getIndex(), true), TRUE);
	}
}

//
// ��ų ���濡���� ��� ���� ��ų ��ư�� ����
//
void CUISkillLearn::SetCurrentSkill(int skill)
{
	m_nCurrentSkillType = skill;

	m_nSelectedSkillID = -1;

	switch(skill)
	{
		case SKILL_SPECIAL:
			m_ppIconsSelectedSkill = m_pIconsSpecialSkill;
			m_pSelectedSkillSatisfied = m_nSpecialSkillSatisfied;
			m_sbScrollBar.SetCurItemCount( m_nNumOfSpecialSkill );
			break;
	}
	
	// [090821 sora] �� ����� ��ũ�ѹ� ��ġ �ʱ�ȭ ������
	m_sbScrollBar.SetScrollPos(0);
}

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Name : CUISkillLearn()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISkillLearn::CUISkillLearn()
{
	m_nSelActiveSkillID		= -1;
	m_nSelPassiveSkillID	= -1;
	m_nSelMemorizeSkillID	= -1;
	m_nSelSpecialSkillID	= -1;

	m_nCurrentTab			= SLEARN_TAB_ACTIVE;
	m_nSSkillType			= -1;
	m_iSelChangeJob			= -1;
	m_bUseCard				= FALSE;

	m_nSelectedSkillID = -1;
	m_nNumOfActiveSkill = 0;
	m_nNumOfPassiveSkill = 0;
	m_nNumOfSpecialSkill = 0;
	m_nCurrentSkillType = -1;
	m_bUseCard				= FALSE;
	m_nSSkillType = -1;
	m_bSkillInfoVisible = FALSE;
	m_ptdButtonTexture = NULL;

	m_iMobIdx = -1;
	m_iMobVirIdx = -1;
	m_bQuest = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUISkillLearn()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISkillLearn::~CUISkillLearn()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);

	for (int i = 0; i < SKILLLEARN_NEW_SLOT_TOTAL ; ++i)
		SAFE_DELETE(m_pIconsSpecialSkill[i]);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CreateNewSkillLearn(pParentWnd, nX ,nY, nWidth, nHeight);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

BOOL CheckSummonSkill( INDEX iSkillIndex )
{
	CSkill	&rSelSkill = _pNetwork->GetSkillData( iSkillIndex );
	if( rSelSkill.GetSorcererFlag() & ( SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER ) )
	{
		return TRUE;
	}
	else
	{
		const int iSummonSkillCount = sizeof( _aSummonSkill ) / sizeof(int);
		for( int i = 0; i < iSummonSkillCount; ++i )
		{
			if( _aSummonSkill[i] == iSkillIndex )
			{					
				return TRUE;
			}
		}
	}
	return FALSE;
}

// ----------------------------------------------------------------------------
// Name : InitSkillLearn()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::InitSkillLearn( BOOL bSpecial )
{
	// Reset description
	m_strDesc.Clear();
	
	// Reset buttons
	for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_ROW_TOTAL; iRow++ )
	{
		m_pIconsSpecialSkill[iRow]->clearIconData();
	}
	
	// Collect skills
	INDEX	ctPosActive = 0, ctPosPassive = 0, ctPosSpecial = 0, ctPosMemorize = 0;				// Possible
	INDEX	ctImposActive = 0, ctImposPassive = 0, ctImposSpecial = 0, ctImposMemorize = 0;		// Impossible

	CUIManager* pUIManager = CUIManager::getSingleton();

	int iSkill;

	if (bSpecial == TRUE)
	{
		std::vector<sCollectSkill>		vectorPosSpecialSkill;
		std::vector<sCollectSkill>		vectorImposSpecialSkill;
		
		vectorPosSpecialSkill.resize(SKILLLEARN_NEW_SLOT_ROW_TOTAL);		
		vectorImposSpecialSkill.resize(SKILLLEARN_NEW_SLOT_ROW_TOTAL);

		CSpecialSkill::_map::iterator	iter = CSpecialSkill::_mapdata.begin();
		CSpecialSkill::_map::iterator	eiter = CSpecialSkill::_mapdata.end();

		for (;iter != eiter; ++iter)
		{
			CSpecialSkill* pSSkill = (*iter).second;

			if (pSSkill == NULL)
				continue;
			
			// ������
			if(m_nSSkillType == SSKILL_MAKE_WEAR)
			{
				if(pSSkill->GetType() != SSKILL_MAKE_WEAR)
					continue;
			}
			// If skill is different
			else if( pSSkill->GetType() != m_nSSkillType)
				continue;
			
			// ���� üũ...
			const int iPreference = pSSkill->GetPreference();
			if(iPreference != -1)
			{
				int iIndex = pUIManager->GetCharacterInfo()->CheckSSkill( pSSkill->GetType() );
				if(iIndex != -1)
				{
					CSpecialSkill* pPrefer = CSpecialSkill::getData(iIndex);

					if (pPrefer == NULL)
						continue;

					if( iPreference < pPrefer->GetPreference() )
						continue;
				}
			}
			
			int		nSkillIndex		= pSSkill->GetIndex();
			SBYTE	sbSkillLevel	= MY_INFO()->GetSkillLevel(nSkillIndex, true);
			
			// If this skill is already max level
			if( sbSkillLevel >= pSSkill->GetMaxLevel() )
				continue;
			
			if( sbSkillLevel < 1 )
				sbSkillLevel = 0;
			ULONG	ulNeedCharLevel = pSSkill->GetLearnLevel( sbSkillLevel );
			sbSkillLevel++;
			
			// Special Skill
			if( ulNeedCharLevel <= _pNetwork->MyCharacterInfo.level )
			{
				vectorPosSpecialSkill[ctPosSpecial++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);
			}
			else
			{
				vectorImposSpecialSkill[ctImposSpecial++].SetData(nSkillIndex, sbSkillLevel, ulNeedCharLevel);	
			}
		}
		
		std::sort(vectorPosSpecialSkill.begin(), vectorPosSpecialSkill.end());
		std::sort(vectorImposSpecialSkill.begin(), vectorImposSpecialSkill.end());

		iSkill = 0;
		int		iRow;
		for( iRow = 0; iRow < ctPosSpecial; iRow++ )
		{
			m_pIconsSpecialSkill[iSkill]->setSkill(vectorPosSpecialSkill[iRow].lSkillIndex, true);
			m_nSpecialSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorPosSpecialSkill[iRow].lSkillIndex, vectorPosSpecialSkill[iRow].sbSkillLevel, TRUE  );
			iSkill++;
		}
		for( iRow = 0; iRow < ctImposSpecial; iRow++ )
		{
			m_pIconsSpecialSkill[iSkill]->setSkill(vectorImposSpecialSkill[iRow].lSkillIndex, true);
			m_nSpecialSkillSatisfied[iSkill] = IsSatisfiedSkill( vectorImposSpecialSkill[iRow].lSkillIndex, vectorImposSpecialSkill[iRow].sbSkillLevel, TRUE  );
			iSkill++;
		}

		m_nNumOfSpecialSkill = iSkill;
		// Set scroll bar

		m_sbScrollBar.SetScrollPos( 0 );
		SetCurrentSkill(SKILL_SPECIAL);
	}
}

// ----------------------------------------------------------------------------
// Name : OpenSkillLearn()
// Desc : nMasterType�� �Ϲ� ��ų�϶��� ����, Ư�� ��ų�϶��� Ư�� ��ųŸ���� �˴ϴ�.
// ----------------------------------------------------------------------------
void CUISkillLearn::OpenSkillLearn( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SKILLLEARN_REQ ) || IsVisible() )
		return;
	
	// Special Skill
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_SSKILLLEARN_REQ ) || IsVisible() )
		return;

	m_rcIcons.SetRect( 7, 53, 203, SKILLLEARN_NEW_HEIGHT - 70 );
	m_sbScrollBar.SetPos(232, 67);
	m_sbScrollBar.SetHeight(213);
	m_btnClose.SetPos( 232, 4);
	SetWidth(SKILLLEARN_NEW_WIDTH);
	SetHeight(SKILLLEARN_NEW_HEIGHT);
	m_sbScrollBar.SetItemsPerPage( SKILLLEARN_NEW_SLOT_ROW );

	m_rcTitle.SetRect( 0, 0, SKILLLEARN_NEW_WIDTH, 36 );
	m_rcTab.SetRect( 6, 37 , 251, 57 );

	m_nSelectedSkillID = -1;
	m_nNumOfActiveSkill = 0;
	m_nNumOfPassiveSkill = 0;
	m_nCurrentSkillType = 0;

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;
	m_iMobIdx = iMobIndex;
	m_bQuest = bHasQuest;

	m_iMobVirIdx = iMobVirIdx;

	CMobData* MD	= CMobData::getData(iMobIndex);
	m_nSSkillType	= MD->GetSpecialSkillMaster();
	CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
	m_bSpecial		= MD->IsSSkillMaster() ? TRUE : FALSE;

	if( MD->IsSSkillMaster() )		// Ư�� ��ų �������϶�...
	{
		// FIXME : �Ʒ��� ���� �޼����� �׶� �׶� �ٸ��� �뷫 ����.
		switch( m_nSSkillType )
		{
		case SSKILL_MINING:			// ä��
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 630, "ä�� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 631, "������ ä���ϴ°Ŵ� �׸� ����� �ʾ�." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 632, "������ �󸶸�ŭ ���� �ð��� ���� ���� ĳ�����İ� �Ƿ��� �¿� ����." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_GATHERING:		// ä��
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 633, "ä�� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 634, "ũ���� ä���� ���� ���� ����� �ʿ�����." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 635, "����� ������ �������� �� ���� ���� ä���ϰ� �߸� �ϸ� ���� ����ٴ� ũ���� �ɵ� ���� �� �ִٳ�." ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_CHARGE:			// ����
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 636, "���� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 637, "���� ����� ���� ���� �˰� �ͽ��ϱ�?" ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 638, "�� �� ����� ���� ����� ���ٸ� �������� ���Ҹ� ���� �� ���� ���Դϴ�.  ��� ���ðڽ��ϱ�?" ), -1, 0xA3A1A3FF );	
			}
			break;
		case SSKILL_STONE:			// �������ü�
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 639, "�������� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
#ifdef DURABILITY
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 6194, "���� ��� ���°͵� �߿� ������, ��� �������� ������ �ƹ� �����ۿ��� ����." ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 6195, "�ڳ� ��� ������ ���� ���°�? �� ��� ���� ����� ������" ), -1, 0xA3A1A3FF );

				CTString strTemp;
				strTemp.PrintF( _S( 6429, "�ռ��Ѵ�." ) );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strTemp, ITEM_COMPOSE );
				strTemp.PrintF( _S( 6192, "�������� �����Ѵ�." ) );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strTemp, ITEM_DURABILITY );
				strTemp.PrintF( _S( 6193, "�ִ� �������� �����Ѵ�." ) );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strTemp, ITEM_RECOVERY );
#else	//	DURABILITY
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 640, "������ ���� �ϴ� ���� �ƹ��� �ϴ� ���� �Ƴ�! �ڳװ� ��� ���ڴٰ�?" ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 641, "���� �����̵��� ���� ���� �ϴ� ���� �ʹ� ���� ���� ������ �־�." ), -1, 0xA3A1A3FF );

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, _S( 6429, "�ռ��Ѵ�." ), ITEM_COMPOSE );
#endif	//	DURABILITY
			}
			break;
		case SSKILL_PLANT:			// �Ĺ�������
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 642, "�Ĺ����� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 643, "ũ���� �̿��� ���� ����� �ʹٰ�? ������ ������." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 644, "ũ������ �ʱ��� ������� ��û���� ���� �ð��� �ɷ�! ���� ����� �� �� ������?" ), -1, 0xA3A1A3FF );		
			}
			break;
		case SSKILL_ELEMENT:		// ����������
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 645, "�������� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 646, "���Ҹ� ���� �Ϸ��� ����� ������ ���� �γ��� �ʿ� �մϴ�." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 647, "������ �������� ���⸸ �Ѵٸ� �׿� �����ϴ� ������ ���� �� ���� ���Դϴ�. �ڿ��� �������� ���� �����ϱ��." ), -1, 0xA3A1A3FF );		
			}
			break;

		case SSKILL_MAKE_WEAPON:	// ��������
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 648, "�������� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 649, "�̷�! �� ���� ������ ���� �; �� ģ����." ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 650, "�� �� �̻� ���⸦ ������ �ʾ� ! �� �̻� ������ ���� �ƴ϶��! ���� �ص� �ҿ� ����." ), -1, 0xA3A1A3FF );		
			}
			break;

		case SSKILL_MAKE_POTINO: // ���� ����
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 767, "���� ���� ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 768, "���� ������ ���ɽ����� ����� �۾��Դϴ�." ), -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 769, "������ ã�� ����� ������ ���� ����� ���� �����̱⵵ �մϴ�. �������� ���� ���۱���� ��� ���ðڳ���?" ), -1, 0xA3A1A3FF );
			}
			break;
		case SSKILL_MAKE_WEAR:
		case SSKILL_MAKE_G_B:
		case SSKILL_MAKE_ARMOR:
		case SSKILL_MAKE_H_S:
			{
				// Create skill learn message box
				pUIManager->CreateMessageBoxL( _S( 651, "������ ��ų" ), UI_SKILLLEARN, MSGLCMD_SSKILLLEARN_REQ );		

				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 652, "�����̳��� ���� ��� �ʹٸ� �� ���� ����� ��� ����!" ), -1, 0xA3A1A3FF );		
				pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, TRUE, _S( 653, "������ ���䰡 ���Ƽ� ���� �Ҹ� �ϴٱ�!" ), -1, 0xA3A1A3FF );		
			}
			break;
		}
		
		CTString strMessage;		
		strMessage.PrintF( _S( 1218, "��ų�� �����Ѵ�." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_LEARN );

		if( bHasQuest )
		{
			// 2009. 05. 27 ������
			// �̾߱��Ѵ� ���� ó��
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SSKILLLEARN_REQ);				
		}
			
		strMessage.PrintF(_S( 1748, "NPC �ȳ�" ));
		pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage, SKILL_NPC_HELP );

		strMessage.PrintF( _S( 1220, "����Ѵ�." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SSKILLLEARN_REQ, FALSE, strMessage );
	}
	// ITS# 4252 : BUG-FIXED  [9/26/2011 rumist]
	else if( iMobIndex == 1200 || iMobIndex == 1198 )	// Ÿ���� NPC - ��ų ������ Ǫġ��
	{	// ����� ��ų ������ UI�� �����ش�
		PriorityOpen(iMobIndex, bHasQuest, FALSE);
	}
	else						// �Ϲ� ��ų �������϶�...
	{
		const int iJob = _pNetwork->MyCharacterInfo.job;
		// If job is different
		if( MD->GetSkillMaster() != iJob )
		{
			// WSS_BUG_FIX 070802 ------>><<
			if( pUIManager->DoesMessageBoxExist( MSGCMD_SKILLLEARN_NOTIFY ) ) { return;}

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK,
				UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
			MsgBoxInfo.AddString( _S(3589,"�ش� ������ ��ų�� ������ �� �����ϴ�.") );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}
		
		// Create skill learn message box
		pUIManager->CreateMessageBoxL( _S( 270, "��ų" ), UI_SKILLLEARN, MSGLCMD_SKILLLEARN_REQ );

		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, strNpcName, -1, 0xE18600FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, _S( 271, "���Ӿ��� ����ϴ� �ڸ��� ������ ������ �տ� ���� �� �ִ� ��!\n\n�׷� ������ ������ �ִ� �ڸ� �ε��ϴ� ���� ���� �ϴ� ������.\n" ), -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, TRUE, _S( 272, "������ �ڳ� ���ؿ� �´� ��ų�� �����Ͽ� �������� �ϰԳ�." ), -1, 0xA3A1A3FF );

		CTString strMessage;
		strMessage.PrintF( _S( 1218, "��ų�� �����Ѵ�." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_LEARN );

		if( bHasQuest )
		{
			// 2009. 05. 27 ������
			// �̾߱��Ѵ� ���� ó��
			CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SKILLLEARN_REQ);				
		}

		// ���� ���� �����̰� ������ ���°� �ƴҶ�...
		// ����Ʈ ������� ������ �����Ƿ�. [8/6/2010 rumist]
		if( _pNetwork->MyCharacterInfo.level >= EXTENSION_LEVEL && _pNetwork->MyCharacterInfo.job2 == 0 &&
			_pNetwork->MyCharacterInfo.job != NIGHTSHADOW )
		{
			strMessage.PrintF( _S( 1223, "[%s]�� ����" ), CJobInfo::getSingleton()->GetExtensionName( iJob, 0 ) );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_JOB_1 );

			strMessage.PrintF( _S( 1223, "[%s]�� ����" ), CJobInfo::getSingleton()->GetExtensionName( iJob, 1 ) );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_JOB_2 );
		}

		strMessage.PrintF(_S( 1748, "NPC �ȳ�" ));
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage, SKILL_NPC_HELP );

		strMessage.PrintF( _S( 1220, "����Ѵ�." ) );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SKILLLEARN_REQ, FALSE, strMessage );

		
	}
	
	m_nSelActiveSkillID		= -1;
	m_nSelPassiveSkillID	= -1;
	m_nSelMemorizeSkillID	= -1;
	m_nSelSpecialSkillID	= -1;
}

// ----------------------------------------------------------------------------
// Name : CloseSkillLearn()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::CloseSkillLearn()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
	pUIManager->RearrangeOrder( UI_SKILLLEARN, FALSE );
	
	m_bUseCard = FALSE;
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::Render()
{
	RenderNewSkillLearn();
}

// ----------------------------------------------------------------------------
// Name : RenderSkillBtns()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::RenderSkillBtns()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int	nX = SLEARN_SLOT_SX, nY = SLEARN_SLOT_SY;
	int	iRow, iRowS, iRowE;

	if (m_nCurrentTab == SLEARN_TAB_SPECIAL)
	{
		// Special skill button
		iRowS = m_sbSpecialSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
	    for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			m_pIconsSpecialSkill[iRow]->SetPos( nX, nY );
			if( m_pIconsSpecialSkill[iRow]->IsEmpty() )		
				continue;
			
			m_pIconsSpecialSkill[iRow]->Render(pDrawPort);
		}
	}

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_SKILL );

	// Outline of selected button
	if (m_nCurrentTab == SLEARN_TAB_SPECIAL)
	{
		// Special skill button
		iRowS = m_sbSpecialSkillIcon.GetScrollPos();		
		iRowE = iRowS + SLEARN_SLOT_ROW;
		if( m_nSelSpecialSkillID >= 0 && iRowS <= m_nSelSpecialSkillID && m_nSelSpecialSkillID < iRowE )
		{
			// Set skill learn texture
			pDrawPort->InitTextureData( m_ptdBaseTexture );

			m_pIconsSpecialSkill[m_nSelSpecialSkillID]->GetAbsPos( nX, nY );
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtSelOutline.U0, m_rtSelOutline.V0,
												m_rtSelOutline.U1, m_rtSelOutline.V1,
												0xFFFFFFFF );

			// Render all elements
			pDrawPort->FlushRenderingQueue();
		}
	}

	nY = SLEARN_NAME_SY;
	// Active skill tab
	int	nCharLevel	= _pNetwork->MyCharacterInfo.level;
	SQUAD nCharSP		= _pNetwork->MyCharacterInfo.sp / 10000;

	if (m_nCurrentTab == SLEARN_TAB_SPECIAL)
	{
		iRowS = m_sbSpecialSkillIcon.GetScrollPos();
		iRowE = iRowS + SLEARN_SLOT_ROW;
		for( iRow = iRowS; iRow < iRowE; iRow++, nY += SLEARN_SLOT_OFFSETY )
		{
			if( m_pIconsSpecialSkill[iRow]->IsEmpty() )
				continue;
  
			CSpecialSkill* pSSkill = CSpecialSkill::getData( m_pIconsSpecialSkill[iRow]->getIndex() );

			if (pSSkill == NULL)
				continue;

			SBYTE	sbLevel = MY_INFO()->GetSkillLevel(pSSkill->GetIndex());
			int		nNeedLevel = pSSkill->GetLearnLevel( sbLevel - 1 );
			int	nNeedSP	= pSSkill->GetLearnSP( sbLevel - 1 );

			m_strShortDesc.PrintF( "%s", pSSkill->GetName() );

			if(nCharLevel > nNeedLevel && nCharSP > nNeedSP)
			{
				pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
														0xFFC672FF );
			}
			else
			{
				pDrawPort->PutTextExCX( m_strShortDesc, m_nPosX + SLEARN_NAME_CX, m_nPosY + nY,
														0xBCBCBCFF );
			}


#if defined (G_GERMAN) || defined (G_EUROPE3) || defined (G_EUROPE2)
			m_strShortDesc.PrintF( "Lv %2d   %s %2d", sbLevel, _S( 90, "���õ�" ), pSSkill->GetLearnSP( sbLevel - 1 ) );
#else	// else about japan, german, europe3, europe2, netherlands.
			// [2/28/2013 Ranma] support russia string
#if defined (G_RUSSIA)
			m_strShortDesc.PrintF( "%s %2d   %s %2d",_S( 4414, "LV" ), sbLevel, _S( 4415, "SP" ), pSSkill->GetLearnSP( sbLevel - 1 ) );
#else	// else about russia
			m_strShortDesc.PrintF( "Lv %2d   SP %2d", sbLevel, pSSkill->GetLearnSP( sbLevel - 1 ) );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.
			pDrawPort->PutTextExRX( m_strShortDesc, m_nPosX + SLEARN_NEED_RX,
													m_nPosY + nY + 17, 0xBDA99FFF );
		}
	}
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISkillLearn::MouseMessage( MSG *pMsg )
{
	return SKillLearnNewMouseMessage(pMsg);
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SKILLLEARN_NOTIFY:
		{
			CUIManager::getSingleton()->CloseMessageBox(MSGCMD_SKILLLEARN_NOTIFY);
		}
		break;

	case MSGCMD_CHANGEJOB:			// ���� �Ҳ�����?
		if( bOK )
		{
			// ������ �޼��� ������.
			_pNetwork->ChangeJobReq( m_iSelChangeJob, m_iMobVirIdx );
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CJobInfo* pInfo = CJobInfo::getSingleton();

	CTString strMessage;

	switch( nCommandCode )
	{
	case MSGLCMD_SKILLLEARN_REQ:
		if( nResult == SKILL_LEARN )					// ��ų�� �����Ѵ�.
		{
			//pUIManager->RearrangeOrder( UI_SKILLLEARN, TRUE );

// [KH_07044] 3�� ���� ���� �߰�
			if(g_iShowHelp1Icon)
			{
				pUIManager->GetHelp3()->ClearHelpString();
				pUIManager->GetHelp3()->AddHelpString(_S(3308, "ĳ���Ͱ� ��� �� �ִ� ��ų�� ����� �������ϴ�."));
				pUIManager->GetHelp3()->AddHelpString(_S(3309, "����� ��ų �������� Ŭ���ϸ� �ش� ��ų�� ����� �ʿ������� ǥ�õǸ� ���� ���� ��ų�� �����ߴٸ� ��ų �������� Ŭ�� �� �� ���� ��ư�� �����ϴ�."));
				pUIManager->GetHelp3()->AddHelpString(_S(3310, "�� ��ų�� ���µ� �ʿ��� ���ǿ� ������ ��쿡�� ��ų�� ������ �� �ֽ��ϴ�."));
				pUIManager->GetHelp3()->AddHelpString(_S(3311, "�� ��ų�� ��� ���� ��ų ���� �� ������ ���� �ʿ��� ���õ��� �ڵ� �谨�˴ϴ�."));
				pUIManager->GetHelp3()->AddHelpString(_S(3312, "�� ������ ��ų�� ĳ������ ��ų â(ALT+S)���� Ȯ���� �� �ֽ��ϴ�."));
				pUIManager->GetHelp3()->OpenHelp(this);
			}

// 			InitSkillLearn( FALSE );
// 			m_nCurrentTab = SLEARN_TAB_ACTIVE;

			m_nCurrentTab = -1;	// ��ų ������
			pUIManager->GetSkillNew()->OpenUI(m_iMobIdx, m_iMobVirIdx, m_bQuest, m_fNpcX, m_fNpcZ);

		}
		else if( nResult == SKILL_TALK )				// �̾߱� �Ѵ�.
		{
			//TODO : NewQuestSystem
			// ����Ʈ â ����
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == SKILL_JOB_1 )				// ���� Ŭ���� 1
		{
			pUIManager->CreateMessageBoxL( _S( 1224, "���� ����" ), UI_SKILLLEARN, MSGLCMD_CHANGEJOB_REQ );		
			const int iNeedLevel	= EXTENSION_LEVEL;
			const SQUAD	llNeedNas	= 50000;
			const int iJob			= _pNetwork->MyCharacterInfo.job;

			// FiXME : �Ʒ� �κ��� ������ �ʿ��� �κ�.

			switch( iJob )
			{
			case TITAN:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_HIGHLANDER ), -1, 0xFFC672FF );				
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1225, "�� Ŭ���� ����� ���ݷ��� ������ Ŭ�����μ� ������ �Ͱ��� �ۺ״� ���ݼ��μ��� ���� ������ �� ���� Ŭ�����Դϴ�.\n" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1226, "��� ���� : ����" ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1227, "���� ���� : %dlv�޼�, ���� ũ���� lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );					
					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1229, "�� : ���ݻ��, ������� ���" ), -1, 0xA3A1A3FF );	
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1230, "��ø : ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );								
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1231, "���� : ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1232, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );				
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1233, "���� ���ʽ� ���� : ��" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "Ÿ�� ������ ��ü���� ���� ������ ��ȥ�� Ÿ��ź �߿����� �������� �����޴� �ڵ��� �ٷ� ���̷��� �Դϴ�." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "�̵��� ���忡�� �� ������ ���������� ������ �����Ӱ� ��Ⱦ�����ϸ� �Ŵ��� ������ �ڽ��� ��üó�� �����Ӱ� ����մϴ�.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case KNIGHT:		//  �ο� ����Ʈ
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_ROYALKNIGHT ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1234, "�⺻������ �ܷõ� �ܴ��� ���°� ������ ���ݷ��� ����� ���� �������� ���� �����ϸ� ���ΰ� �������� �������� ������ �����ִ� Ŭ�����Դϴ�." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1235, "��� ���� : �̵���" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1236, "���� ���� : %dlv�޼�, ���� �극��ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1238, "�� : ���ݻ��, ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1239, "��ø : ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1240, "���� : ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1241, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1242, "���� ���ʽ� ���� : ����" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���Ǹ� ��õ�ϴ� ������μ� �����κ��� ������ ���縦 ��ȣ�ϴ� �Ϳ���, ���� ��¡�ϴ� ������ ��ȸ�� �ڵ��� �ٷ� �ο� ����Ʈ�Դϴ�. " ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "�ο� ����Ʈ�� �׵��� �����ִ� ������ ���д� �ڷ� �ϰ� ��� �����ϰ� ���� �� �� �ڷ��� ���� ���Ͽ� �׵��� ���� �ɷ� �̻����� ���ݷ��� ���� �÷Ƚ��ϴ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case HEALER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_ARCHER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1243, "���̿��� ��ó���� ȭ�� �ѹ߸����ε� �ټ��� ���� �����ϰų�, ���� �̵��ӵ�, Ȱ�� �̿��� ��Ÿ� �������� ��Ʈ �� �� ����� ���ݿ� Ưȭ�� �ɷ��� ������ �ֽ��ϴ�." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1244, "��� ���� : Ȱ" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1245, "���� ���� : %dlv�޼�, ���� ���ü� lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1246, "�� :  ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1247, "��ø : ���� ���, ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1248, "���� : ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1249, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1250, "���� ���ʽ� ���� : ��" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���� ���̿������� �׵��� ��õ������ Ÿ�� ���� ������ �̿��� ������� ������ ���ϸ� ġ���� ����� �� ������ ���� �������� ������, �������� ���� ������ ���� �̿� �����ϴ� �ڵ��� ���̿��� ��ó�Դϴ�.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case MAGE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_WIZARD ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1251, "���� ���ݷ����� �ı����̰� ���� ������ �ϼ����� �ҹٴٷ� ����� ������ ���������� Ưȭ �� �ڵ�� ������ Ȱ�뿡 �־� ������ Ŭ�����Դϴ�." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1252, "��� ���� : ������" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1253, "���� ���� : %dlv�޼�, ��� �극��ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1254, "�� :  ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1255, "��ø : ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1256, "���� : ���� ���ݻ��, ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1257, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1258, "���� ���ʽ� ���� : ����" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���ڵ�� ���� �� ��ü�� �������ϰ� �����ϴ� �������� �߿� ������ �������� �������� �̾� �Ŵ��� ���� �����ϴ� �ڵ��Դϴ�." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���ڵ尡 ����� ���� ��ȣ�� ���� ������ ������ ���� ��ü�� �Ҹ� �� ������ ������ ���ϱ⿡ �� �� �� �� ���� �ϳ��� ���ܱ����� �򰡵ȴ�.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case ROGUE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_ASSASSIN ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1378, "�������� ����, ��� �ɷ°� ���� ��ų �ӵ��� ����� Ŭ�����μ� 1:1�� �������� �� Ȱ���� ��ġ�� ����� Ŭ�����Դϴ�." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1379, "��� ���� : �ܰ�" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1380, "���� ���� : %dlv�޼�, �κ�������Ƽ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1381, "�� :  ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1382, "��ø : ���� ��� ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1383, "���� : ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1384, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1385, "���� ���ʽ� ���� : ��ø" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "����� ª�� �ܰ˰� ������ ����� �̿��� �� �������� ��ġ�� ���� �ֵ� �������̸�, ��밡 ���� �ܷ����� ���� �κ��̳� �������� ������ ���� �̼��� ƴ�� ��� �����ϸ� �����̶� ��ƴ�� ���̸� ���� �޼Ҹ� �����Ͽ� ���İ��� ����� ����� ���� ���� ����� Ư���Դϴ�.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
			case SORCERER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, 1 ), -1, 0xFFC672FF );									
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 2342,"������Ż ����Ʈ�� ���� ��ȯ�� �������Ͽ� ��, ��, �ٶ�, ������ ���еǴ� 4�� ���ɵ��� ��ȭ ��Ű�� �׵��� ��ȯ�Ͽ� �Բ� ������ ���ϴ� Ŭ�����Դϴ�."  ), -1, 0xA3A1A3FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 2343,"��� ���� : ����"  ), -1, 0xA3A1A3FF );

					strMessage.PrintF( _S(2344, "���� ���� : %dlv�޼�, ���̽� ������ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2345, "�� :  ������� ���"  ), -1, 0xA3A1A3FF );	
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2346, "��ø : ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2347, "���� : ���� ���� ���, ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2348, "ü�� : ����, ���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2349, "���� ���ʽ� ���� : ����"  ), -1, 0xA3A1A3FF );											
					
				}
				break;
#ifdef CHAR_EX_ROGUE
			case EX_ROGUE:	// [2012/08/27 : Sora] EX�α� �߰�
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_ASSASSIN ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5739, "�������� ����, ��� �ɷ°� ���� ��ų �ӵ��� ����� Ŭ�����μ� 1:1�� �������� �� Ȱ���� ��ġ�� ����� Ŭ�����Դϴ�." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5740, "��� ���� : �ܰ�" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 5741, "���� ���� : %dlv�޼�, �κ�������Ƽ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5742, "�� :  ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5743, "��ø : ���� ��� ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5744, "���� : ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5745, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5746, "���� ���ʽ� ���� : ��ø" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "����� ª�� �ܰ˰� ������ ����� �̿��� �� �������� ��ġ�� ���� �ֵ� �������̸�, ��밡 ���� �ܷ����� ���� �κ��̳� �������� ������ ���� �̼��� ƴ�� ��� �����ϸ� �����̶� ��ƴ�� ���̸� ���� �޼Ҹ� �����Ͽ� ���İ��� ����� ����� ���� ���� ����� Ư���Դϴ�.\n" ), -1, 0xA3A1A3FF );							
				}
				break;
#endif
#ifdef CHAR_EX_MAGE
			case EX_MAGE: //2013/01/08 jeil EX������ �߰� ��Ʈ�� ������ �߰� ���� �ʿ� 
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_WIZARD ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5827, "���� ���ݷ����� �ı����̰� ���� ������ �ϼ����� �ҹٴٷ� ����� ������ ���������� Ưȭ �� �ڵ�� ������ Ȱ�뿡 �־� ������ Ŭ�����Դϴ�." ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5828, "��� ���� : ������" ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 5829, "���� ���� : %dlv�޼�, ��� ��� �극��ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					
					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5830, "�� :  ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5831, "��ø : ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5832, "���� : ���� ���ݻ��, ���� ��� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5833, "ü�� : ����,���� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5834, "���� ���ʽ� ���� : ����" ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���ڵ�� ���� �� ��ü�� �������ϰ� �����ϴ� �������� �߿� ������ �������� �������� �̾� �Ŵ��� ���� �����ϴ� �ڵ��Դϴ�." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���ڵ尡 ����� ���� ��ȣ�� ���� ������ ������ ���� ��ü�� �Ҹ� �� ������ ������ ���ϱ⿡ �� �� �� �� ���� �ϳ��� ���ܱ����� �򰡵ȴ�.\n" ), -1, 0xA3A1A3FF );			
				}
				break;
#endif
			}

			CTString strMessage;
			strMessage.PrintF( _S( 1267, "[%s]�� �����Ѵ�." ), pInfo->GetExtensionName( _pNetwork->MyCharacterInfo.job, 0 ) );		
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, strMessage, 0 );
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, _S( 1268, "����ϱ�") );			

			m_iSelChangeJob = 1;
		}
		else if( nResult == SKILL_JOB_2 )				// ���� Ŭ���� 2
		{
			pUIManager->CreateMessageBoxL( _S( 1224, "���� ����" ), UI_SKILLLEARN, MSGLCMD_CHANGEJOB_REQ );		
			const int	iNeedLevel	= EXTENSION_LEVEL;
			const SQUAD	llNeedNas	= 50000;
			const int	iJob		= _pNetwork->MyCharacterInfo.job;

			// FiXME : �Ʒ� �κ��� ������ �ʿ��� �κ�.
			switch( iJob )
			{
			case TITAN:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_WARMASTER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1269,  "��Ը� �������� �ܿ� ���� �ٴ� Ȱ���� �����ָ� ���̷����� ��ȸ�ϴ� ���ݷ°� �� Ŭ������ ��Ʋ�� ���� ���� ü���� ������ �ֽ��ϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1270,  "��� ���� : ���"  ), -1, 0xA3A1A3FF );							
					strMessage.PrintF( _S( 1271, "���� ���� : %dlv�޼�, ���� ũ���� lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1272,  "�� : ���ݻ��, ������� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1273,  "��ø : ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1274,  "���� : ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1275,  "ü�� : ����,���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1276,  "���� ���ʽ� ���� : ü��"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "ȣ������ �������� ���� �ð� ������ ���� ���� Ÿ��ź �߿� Ź���� �������� ������ ���� �а� ���߷��ְ� ��ó�ϰų� �Ʊ��� ��⸦ ���� �Լ����� �ѵ��� �ִ� Ŭ������ �̵� �� �������Դϴ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case KNIGHT:		// ���� ����Ʈ
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_TEMPLEKNIGHT ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1277,  "��Ƽ���� �����ؼ� ���ϰų�, ���͸� �����Ͽ� �ڽſ��� ������� �ϴ� ���� ��Ƽ���� ����� ���� �Ǿ� �ָ�, ��Ը� ���������� �������� ������ �����ϰ� �˴ϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1278,  "��� ���� : �˰� ����"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1279, "���� ���� : %dlv�޼�, ���� �극��ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1280,  "�� : ���ݻ��, ������� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1281,  "��ø : ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1282,  "���� : ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1283,  "ü�� : ����,���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1284,  "���� ���ʽ� ���� : ü��"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "����� �ڽŵ��� ���� �ְ��� �̴����� ����� �ִ� ���ó���Ʈ�� ��Ƽ���� ���迡�� ��ȣ�ϸ� �׵��� �����Ͽ�, �� �÷��̸� ��Ȱ�ϰ� �� �ִ� ��Ȱ�� ���� ������ �� �ִ� �����Դϴ�. " ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "������ �̵��� ���ݷ��� �ο� ����Ʈ�� ���� ��� ����������, ���� ���°� ưư�� ü���� �̵��� ���� ū �����Դϴ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case HEALER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_CLERIC ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1285, "���� ȸ�������� ���� ġ���ϸ�, ������ �ɷ��� �ִ�ġ�� ����÷��ִ� �������� ���� ����ϴ� �� ��Ƽ �÷����� ����� ������ ������ �����ϴ� Ŭ���� �Դϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1286, "��� ���� : �ϵ�"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1287, "���� ���� : %dlv�޼�, ���� ���ü� lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1288, "�� :  ������� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1289, "��ø : ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1290, "���� : ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1291, "ü�� : ����,���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1292, "���� ���ʽ� ���� : ����"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "���̿����� ��� �ı��� �����ϰ�, â���� ����� ���� �����ϴ� ���̿��� Ŭ������ ���� ���������� ����鿡�� ������� �Ҿ� �־� �ָ�, ��, ���, �����̻��� �������� ��ü�� �����ο� �� �ֵ��� �����ݴϴ�.\n" ), -1, 0xA3A1A3FF );				
					
					
				}
				break;
			case MAGE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_WITCH ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1293, "�����̻� ���� �� ���� ������ ����ϴ� Ŭ�����μ� ���� ������ ���� ������ �ϴ��� �� ���� ��õ������ �����ϴµ��� �������� ������ �Դϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1294, "��� ���� : �ϵ�"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1295, "���� ���� : %dlv�޼�, ��� �극��ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1296,  "�� :  ������� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1297,  "��ø : ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1298,  "���� : ���� ���ݻ��, ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1299,  "ü�� : ����,���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1300,  "���� ���ʽ� ���� : ��ø"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "��ġ�� ���轺����, ġ�����̸� ������ ������ �����ϴ� �ڵ��Դϴ�. �̵��� ���ϴ� ������ ��������� �ٸ��ƴ� �ΰ��� ��ü�̸�, ���� ������ �帧���� ���� ȥ���� ���߸��ų�, ������, ��ü������ ���� ��Ű�� ���� ������ ����� ���� ������ϰ� ����ϴ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case ROGUE:	
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_RANGER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1301,  "�������� �ֺ��� �ڿ������� �̿��� ������ ������ �̿��� ��Ÿ� ������ �ϸ�, �ӻ��� ����Դϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1302,  "��� ���� : ����"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 1303, "���� ���� : %dlv�޼�, �κ�������Ƽ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1304,  "�� :  ������� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1305,  "��ø : ���� ��� ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1306,  "���� : ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1307,  "ü�� : ����,���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 1308,  "���� ���ʽ� : ��"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "�������� �η����� �ٰŸ��� �ƴ� ��, ���Ÿ������� ������ �پ��� ������ ������ Ȱ�뿡�� ��Ÿ����." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "ª�� �ð� ������ �� ���� ȯ���� Ȱ���� �پ��� Ʈ���� ��ġ�ϴ� ���� ���� � ��Ȳ������ ������ �޼Ҹ� ���� ��Ȯ�� ������ ������ �������� �ڽ��� ����� Ȱ���Ͽ� ����ȹ��� ������ Ȱ���Ѵ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
			case SORCERER:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, 2 ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2350,"����ȸ���Ʈ�� ������ ���� ���� ��, �ٶ��� ���� ���Ű� ��, ���� ���� ������ ���� �ڽ��� ��ü�� ��ȭ���� ���� ������ ���ϴ� ���� ��Ÿ���� ���� Ŭ���� �Դϴ�. "), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2351, "��� ���� : ���̵�" ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S(2344, "���� ���� : %dlv�޼�, ���̽� ������ũ lv5 ����" ), iNeedLevel );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2345, "�� : ������� ���" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2346, "��ø : ����, ȸ�ǻ��" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2347, "���� : ���� ���ݻ��, ���� ��� ���, ���� ������ ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2348, "ü�� : ����,���� ���, ���� ȿ�� ����" ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S(2349, "���� ���ʽ� ���� : ����" ), -1, 0xA3A1A3FF );												
				}
				break;
#ifdef CHAR_EX_ROGUE
			case EX_ROGUE:	// [2012/08/27 : Sora] EX�α� �߰�
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_RANGER ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5747,  "�������� �ֺ��� �ڿ������� �̿��� ������ ������ �̿��� ��Ÿ� ������ �ϸ�, �ӻ��� ����Դϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5748,  "��� ���� : ����"  ), -1, 0xA3A1A3FF );							

					strMessage.PrintF( _S( 5749, "���� ���� : %dlv�޼�, �κ�������Ƽ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							

					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5750,  "�� :  ������� ���"  ), -1, 0xA3A1A3FF );	
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5751,  "��ø : ���� ��� ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5752,  "���� : ���� ��� ���, ���� ������ ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5753,  "ü�� : ����,���� ���, ���� ȿ�� ����"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5754,  "���� ���ʽ� : ��"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "�������� �η����� �ٰŸ��� �ƴ� ��, ���Ÿ������� ������ �پ��� ������ ������ Ȱ�뿡�� ��Ÿ����." ), -1, 0xA3A1A3FF );							
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "ª�� �ð� ������ �� ���� ȯ���� Ȱ���� �پ��� Ʈ���� ��ġ�ϴ� ���� ���� � ��Ȳ������ ������ �޼Ҹ� ���� ��Ȯ�� ������ ������ �������� �ڽ��� ����� Ȱ���Ͽ� ����ȹ��� ������ Ȱ���Ѵ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
#endif
#ifdef CHAR_EX_MAGE		//2013/01/08 jeil EX������ �߰� ��Ʈ�� ������ �߰� ���� �ʿ� 
			case EX_MAGE:
				{
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, pInfo->GetName( iJob, JOB_2ND_EX_WITCH ), -1, 0xFFC672FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5835, "�����̻� ���� �� ���� ������ ����ϴ� Ŭ�����μ� ���� ������ ���� ������ �ϴ��� �� ���� ��õ������ �����ϴµ��� �������� ������ �Դϴ�."  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "  " ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5836, "��� ���� : �ϵ�"  ), -1, 0xA3A1A3FF );							
					
					strMessage.PrintF( _S( 5837, "���� ���� : %dlv�޼�, ��� ��� �극��ũ lv5 ����" ), iNeedLevel );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );							
					
					strMessage.PrintF( _S( 1228, "�ʿ� ���� : %I64d ����" ), llNeedNas );		
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, strMessage, -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5838,  "�� :  ������� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5839,  "��ø : ����, ȸ�ǻ��"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5840,  "���� : ���� ���ݻ��, ���� ��� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5841,  "ü�� : ����,���� ���"  ), -1, 0xA3A1A3FF );							
					pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, _S( 5842,  "���� ���ʽ� ���� : ��ø"  ), -1, 0xA3A1A3FF );							
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, TRUE, CTString( "��ġ�� ���轺����, ġ�����̸� ������ ������ �����ϴ� �ڵ��Դϴ�. �̵��� ���ϴ� ������ ��������� �ٸ��ƴ� �ΰ��� ��ü�̸�, ���� ������ �帧���� ���� ȥ���� ���߸��ų�, ������, ��ü������ ���� ��Ű�� ���� ������ ����� ���� ������ϰ� ����ϴ�.\n" ), -1, 0xA3A1A3FF );							
					
					
				}
				break;
#endif
			}
			
			
			CTString strMessage;
			strMessage.PrintF( _S( 1267, "[%s]�� �����Ѵ�." ), pInfo->GetExtensionName( _pNetwork->MyCharacterInfo.job, 1 ) );		
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, strMessage, 0 );
			pUIManager->AddMessageBoxLString( MSGLCMD_CHANGEJOB_REQ, FALSE, _S( 1268, "����ϱ�" ) );			

			m_iSelChangeJob = 2;
		}
		else if(nResult == SKILL_NPC_HELP)											// ����Ѵ�.
		{
			pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
		}
		// [090527: selo] Ȯ���� ����Ʈ ����
		else if( ciQuestClassifier < nResult )	
		{
			// ������ ����Ʈ�� ���� ���� �Ǵ� ���� â�� ����.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}

		else
		{
			CloseSkillLearn();
		}
		break;

	case MSGLCMD_CHANGEJOB_REQ:							// ���� ����â.
		{
			if( nResult == 0 )
			{				
				pUIManager->CloseMessageBox( MSGCMD_CHANGEJOB );				
				
				CTString	strMessage;
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 1317, "����" ), UMBS_OKCANCEL, UI_SKILLLEARN, MSGCMD_CHANGEJOB );		
				strMessage.PrintF( _S( 1318, "%s�� ���� �Ͻðڽ��ϱ�?" ), pInfo->GetExtensionName( _pNetwork->MyCharacterInfo.job, m_iSelChangeJob - 1 ) );	
				MsgBoxInfo.AddString( strMessage );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
			else
			{
			}
		}
		break;

		// Special Skill
	case MSGLCMD_SSKILLLEARN_REQ:
		if( nResult == SKILL_LEARN )					// ��ų�� �����Ѵ�.
		{
			pUIManager->RearrangeOrder( UI_SKILLLEARN, TRUE );

			InitSkillLearn( TRUE );
			m_nCurrentTab = SLEARN_TAB_SPECIAL;
		}
		else if( nResult == SKILL_TALK )				// �̾߱� �Ѵ�.
		{
			//TODO : NewQuestSystem
			// ����Ʈ â ����
			CUIQuestBook::TalkWithNPC();
		}
		else if( nResult == SKILL_NPC_HELP)								
		{
			pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
		}
#ifdef DURABILITY
		else if (nResult == ITEM_DURABILITY)
		{
			pUIManager->GetDurability()->Open(CUIDurability::eDURABILITY);
		}
		else if (nResult == ITEM_RECOVERY)
		{
			pUIManager->GetDurability()->Open(CUIDurability::eRECOVERY);
		}
#endif	//	DURABILITY
		else if (nResult == ITEM_COMPOSE)
		{
			pUIManager->GetItemCompose()->openUI();
		}
		// [090527: selo] Ȯ���� ����Ʈ ����
		else if( ciQuestClassifier < nResult )	
		{
			// ������ ����Ʈ�� ���� ���� �Ǵ� ���� â�� ����.
			CUIQuestBook::SelectQuestFromMessageBox( nResult );
		}

		else// ����Ѵ�.
		{
			CloseSkillLearn();
		}
		break;
	case MSGLCMD_RESERVE_REQ:
		{
			// [090527: selo] Ȯ���� ����Ʈ �̾߱� �Ѵ� ó�� ������ ���� ��ƾ
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}
			
			switch(nResult)
			{
			pUIManager->SetCSFlagOff( CSF_SHOP );

			case SKILL_TITAN:
				{
					OpenSkillLearn(75, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_KNIGHT:
				{
					OpenSkillLearn(76, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_HEALER:
				{
					OpenSkillLearn(77, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_MAGE:
				{
					OpenSkillLearn(123, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_ROGUE:
				{
					OpenSkillLearn(176, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_SORCERER:
				{
					OpenSkillLearn(255, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
			case SKILL_NIGHTSHADOW:
				{
					OpenSkillLearn(894, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
#ifdef CHAR_EX_ROGUE
			case SKILL_EX_ROGUE:	// [2012/08/27 : Sora] EX�α� �߰�
				{
					OpenSkillLearn(1504, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
#endif
#ifdef CHAR_EX_MAGE
			case SKILL_EX_MAGE:		//2013/01/08 jeil EX������ �߰� �ǹ̸� ���� ���߿� ����� �߰� ���� �ʿ� NPC ���ؽ��� ���� 
				{
					OpenSkillLearn(1521, m_iMobVirIdx, FALSE, m_fNpcX, m_fNpcZ);
				}
				break;
#endif
			case SKILL_QUEST:
				{
					CUIQuestBook::TalkWithNPC();
				}
				
				// [090527: selo] Ȯ���� ����Ʈ ����
			case ciQuestClassifier:
				{
					// ������ ����Ʈ�� ���� ���� �Ǵ� ���� â�� ����.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;

			default:
				{
					m_bUseCard = FALSE;
				}
			}
		}break;


	}
}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::SendLearnSkill()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
	
	if( _pNetwork->MyCharacterInfo.job == SORCERER )
	{
		// ���Ż��¿� ��ȯ���¿����� ��ų�� ���� �����ϴ�.
		if( _pNetwork->MyCharacterInfo.nEvocationIndex > 0 )
		{
			// ���Ż����̹Ƿ�, ���� ����.
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK,
				UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
			MsgBoxInfo.AddString( _S(2352,"���� ���¿����� ��ų�� ���� �����ϴ�.") );
			pUIManager->CreateMessageBox( MsgBoxInfo );
			return;
		}	
		
		// ��ȯ���¿����� ��ų�� ���� ����.
		for( int i = UI_SUMMON_START; i <= UI_SUMMON_END; ++i )
		{
			CUISummon* pUISummon = (CUISummon*)pUIManager->GetUI(i);
			if( pUISummon->GetSummonEntity() )
			{
				// Create message box of skill learn
				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK,
					UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
				MsgBoxInfo.AddString( _S(2353,"��ȯ ���¿����� ��ų�� ���� �����ϴ�.") );
				pUIManager->CreateMessageBox( MsgBoxInfo );
				return;
			}
		}
	}

	SLONG	slIndex;

	if( m_nSelectedSkillID < 0 )
		return;

	if( m_ppIconsSelectedSkill[m_nSelectedSkillID]->IsEmpty() )
		return;

	slIndex = m_ppIconsSelectedSkill[m_nSelectedSkillID]->getIndex();
	if(m_nCurrentSkillType == SKILL_SPECIAL)
	{
		_pNetwork->SendSSkillLearn( slIndex, m_iMobVirIdx );
	}
}


// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : LearnSkill()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSkill( SLONG slIndex, SBYTE sbIsNew, SBYTE sbLevel, BOOL bSpecial )
{
 	CUIManager* pUIManager = CUIManager::getSingleton();
 
 	if( !bSpecial )
		return;

	CSpecialSkill* pSSkill = CSpecialSkill::getData( slIndex );

	if (pSSkill == NULL)
		return;
	
	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );
	
	// Create message box of skill learn
	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK, UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	strMessage.PrintF( _S( 277, "%s ��ų�� �����Ͽ����ϴ�" ), pSSkill->GetName() );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	MY_INFO()->SetSkill(slIndex, (int)sbLevel, bSpecial > 0);
	
	if( sbIsNew == 0 )
	{
		//pUIManager->GetQuickSlot()->UpdateSkillLevel( slIndex, sbLevel );
	}
	else
		pUIManager->GetCharacterInfo()->AddSkill( slIndex, sbLevel, bSpecial );
	
	BOOL	bUpdate = FALSE;		
	if( !bUpdate )
	{
// 		for( int iRow = 0; iRow < SKILLLEARN_NEW_SLOT_ROW_TOTAL; iRow++ )
// 		{
// 			if( m_pIconsSpecialSkill[iRow]->getIndex() == slIndex )
// 			{
// 				m_pIconsSpecialSkill[iRow].SetSkillLevel( sbLevel + 1 );
// 				break;
// 			}
// 		}
	}
	
	if( sbLevel >= pSSkill->GetMaxLevel() )
	{
		if( sbLevel >= pSSkill->GetMaxLevel() )
		{
			m_nSelectedSkillID = -1;
			
			InitSkillLearn( bSpecial );
			
			return;
		}
	}
}

// ----------------------------------------------------------------------------
// Name : LearnSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch( ubError )
	{
	case MSG_SKILL_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "������ �����Ͽ� ��ų�� ������ �� �����ϴ�." );
		break;

	case MSG_SKILL_LEARN_ERROR_SP:
		strMessage = _S( 279, "���õ��� �����Ͽ� ��ų�� ������ �� �����ϴ�." );
		break;

	case MSG_SKILL_LEARN_ERROR_ITEM:
		strMessage = _S( 280, "�������� �������� �ʾ� ��ų�� ������ �� �����ϴ�." );
		break;

	case MSG_SKILL_LEARN_ERROR_SKILL:
		strMessage = _S( 281, "��ų ������ ���� �ʾ� ��ų�� ������ �� �����ϴ�." );
		break;

	case MSG_SKILL_LEARN_ERROR_SYSTEM:
		strMessage = _S( 282, "��ų�� ������ �� �����ϴ�." );
		break;

	case MSG_SKILL_LEARN_ERROR_STR:			// �� ����
		strMessage = _S( 1319, "���� �����Ͽ� ��ų�� ������ �� �����ϴ�." );		
		break;

	case MSG_SKILL_LEARN_ERROR_DEX:			// ���� ����
		strMessage = _S( 1320, "��ø�� �����Ͽ� ��ų�� ������ �� �����ϴ�." );		
		break;

	case MSG_SKILL_LEARN_ERROR_INT:			// ���� ����
		strMessage = _S( 1321, "������ �����Ͽ� ��ų�� ������ �� �����ϴ�." );		
		break;

	case MSG_SKILL_LEARN_ERROR_CON:			// ü�� ����
		strMessage = _S( 1322, "ü���� �����Ͽ� ��ų�� ������ �� �����ϴ�." );		
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 270, "��ų" ), UMBS_OK,
								UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : LearnSSkillError()
// Desc :
// ----------------------------------------------------------------------------
void CUISkillLearn::LearnSSkillError( UBYTE ubError )
{
	CTString	strMessage;

	switch(ubError)
	{
	case MSG_SSKILL_LEARN_ERROR_LEVEL:
		strMessage = _S( 278, "������ �����Ͽ� ��ų�� ������ �� �����ϴ�." );
		break;
	case MSG_SSKILL_LEARN_ERROR_SP:
		strMessage = _S( 279, "���õ��� �����Ͽ� ��ų�� ������ �� �����ϴ�." );
		break;
	case MSG_SSKILL_LEARN_ERROR_SSKILL:
		strMessage = _S( 654, "�ش� ����� ���µ� �ʿ��� ����� ����� �ʾҽ��ϴ�." );			
		break;
	case MSG_SSKILL_LEARN_ERROR_SSKILL_LEVEL:
		strMessage = _S( 655, "��ų ������ �����մϴ�." );	
		break;
	case MSG_SSKILL_LEARN_ERROR_SYSTEM:
		strMessage = _S( 282, "��ų�� ������ �� �����ϴ�." );
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box of skill learn
	pUIManager->CloseMessageBox( MSGCMD_SKILLLEARN_NOTIFY );

	// Create message box of skill learn
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 656, "���꽺ų" ), UMBS_OK,			
								UI_SKILLLEARN, MSGCMD_SKILLLEARN_NOTIFY );
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}
// ----------------------------------------------------------------------------
// Name : PriorityOpen()
// Desc : ����� ��ų �������� UI â
//			��ų ���� UI â���� ���� ����
// ----------------------------------------------------------------------------	
// BUF FIX : ITS# 4472 [10/13/2011 rumist]
void CUISkillLearn::PriorityOpen(int iIndex, BOOL bHasQuest, BOOL bUseCard/* =FALSE */ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxLExist(MSGLCMD_RESERVE_REQ)) return;
	
	m_bUseCard = bUseCard; // ī�� ��� ����

	CTString	strNpcName = CMobData::getData(iIndex)->GetName();
	// Create skill learn message box
	pUIManager->CreateMessageBoxL( _S( 270, "��ų" ), UI_SKILLLEARN, MSGLCMD_RESERVE_REQ );

	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, strNpcName, -1, 0xE18600FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, _S( 271, "���Ӿ��� ����ϴ� �ڸ��� ������ ������ �տ� ���� �� �ִ� ��!\n\n�׷� ������ ������ �ִ� �ڸ� �ε��ϴ� ���� ���� �ϴ� ������.\n" ), -1, 0xA3A1A3FF );
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, TRUE, _S( 3541,"�״� ���� ������ �Ѵ� ���ΰ�? �׷� ������ ������ �ִ� �ڸ� �ε� �ϴ� ���� �� ������! ������ �Ǿ� �ְ���?"), -1, 0xA3A1A3FF);

	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3555, "Ÿ��ź ��ų�� �����Ѵ�."),SKILL_TITAN);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3556, "����Ʈ ��ų�� �����Ѵ�."),SKILL_KNIGHT);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3557, "���� ��ų�� �����Ѵ�."),SKILL_HEALER);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3558, "������ ��ų�� �����Ѵ�."),SKILL_MAGE);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3559, "�α� ��ų�� �����Ѵ�."),SKILL_ROGUE);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 3560, "�Ҽ��� ��ų�� �����Ѵ�."),SKILL_SORCERER);
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 4797, "����Ʈ ������ ��ų�� �����Ѵ�."),SKILL_NIGHTSHADOW);
#ifdef CHAR_EX_ROGUE
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 5755, "EX�α� ��ų�� �����Ѵ�."),SKILL_EX_ROGUE);	// [2012/08/27 : Sora] EX�α� �߰�
#endif
#ifdef CHAR_EX_MAGE
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 5843, "��ũ������ ��ų�� �����Ѵ�."),SKILL_EX_MAGE);	// 2013/01/08 jeil EX������ �߰� ��Ʈ�� ������ �߰� ���� �ʿ� 
#endif
	
	// 2009. 05. 27 ������
	// �̾߱��Ѵ� ���� ó��
	// quest bug FIX : ITS#4472  [10/13/2011 rumist]
	if( bHasQuest )
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_RESERVE_REQ);				
			
	pUIManager->AddMessageBoxLString( MSGLCMD_RESERVE_REQ, FALSE, _S( 1220, "����Ѵ�." ) );
}