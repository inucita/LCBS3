#include "stdh.h"

// ��� ����. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GameState.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Sound/SoundLibrary.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/SlaveInfo.h>
#include <Engine/Interface/UISummon.h>
#include <Engine/Contents/function/TeleportUI.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Contents/Base/PetStash.h>
#include <Engine/Contents/Base/UISkillNew.h>
#include <Engine/Contents/Login/UIServerSelect.h>
#include <engine/Contents/Login/UILoginNew.h>
#include <Engine/Interface/UISystemMenu.h>
#include <Engine/Contents/Base/UIAuctionNew.h>
#include <Engine/Interface/UIOption.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Contents/Base/UIPartyNew.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Contents/function/HelperUI.h>
#include <Engine/Contents/function/TargetInfoNewUI.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Interface/UIInitJob.h>
#include <Engine/Contents/function/AffinityInfoUI.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Contents/function/WareHouseUI.h>
#include <Engine/Contents/Login/UICharacterSelect.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Contents/Base/Quest.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Contents/function/HelpWebUI.h>
#include <Engine/Contents/function/News_Web_UI.h>
#include <Engine/Contents/function/News.h>
#include <Engine/Interface/UIGuildBattle.h>

extern INDEX g_iXPosInSystemMenu;
extern INDEX g_iYPosInSystemMenu;

extern CDrawPort* _pdpMain;
extern BOOL g_bAutoRestart;

extern INDEX	g_iCountry;

// ���� ������ �ε���
#define	DEF_RELIC_ITEM1 10951
#define	DEF_RELIC_ITEM2 10952
#define	DEF_RELIC_ITEM3 10953

// ----------------------------------------------------------------------------
// Name : CUISystemMenu()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUISystemMenu::CUISystemMenu()
{
	if (IsGamigo(g_iCountry) == TRUE || g_iCountry == KOREA)
	{
		m_bCharacterMove = FALSE;
		m_llStartTime = 0;
	}

	m_ptdButtonTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUISystemMenu()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUISystemMenu::~CUISystemMenu()
{
	Destroy();

	STOCK_RELEASE(m_ptdButtonTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );

	// Create inventory texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\SystemMenu.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\NewOption.tex" ) );
	m_ptdButtonTexture =CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtBackground.SetUV( 0, 0, 216, 114, fTexWidth, fTexHeight );
	m_rtBackground.SetUV( 270, 104, 448, 295, fTexWidth, fTexHeight );

	fTexWidth = m_ptdButtonTexture->GetPixWidth();
	fTexHeight = m_ptdButtonTexture->GetPixHeight();

	// Close button
	//m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	//m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	//m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.Create( this, CTString( "" ), 153, 4, 16, 16 );
	m_btnClose.SetUV( UBS_IDLE, 211, 33, 227, 49, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 229, 33, 245, 49, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Option button
	//m_btnOption.Create( this, _S( 283, "�ɼ�" ), 31, 39, 63, 21 );
	//m_btnOption.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnOption.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnOption.Create( this, _S( 283, "�ɼ�" ), 9, 44, 78, 22);
	m_btnOption.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnOption.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnOption.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOption.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Help button
	//m_btnHelp.Create( this, _S( 284, "����" ), 121, 39, 63, 21 );
	//m_btnHelp.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnHelp.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnHelp.Create( this, _S( 284, "����" ), 91, 44, 78, 22 );
	m_btnHelp.SetUV( UBS_IDLE, 113, 0, 182, 22, fTexWidth, fTexHeight );
	m_btnHelp.SetUV( UBS_CLICK, 186, 0, 256, 22, fTexWidth, fTexHeight );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_ON );
	m_btnHelp.CopyUV( UBS_IDLE, UBS_DISABLE );

	UIRect rcLeft, rcMiddel, rcRight;
	UIRectUV3 rtIdleBtn, rtClickBtn;

	rcLeft.SetRect(0, 0, 14, 21);
	rcMiddel.SetRect(14, 0, 146, 21);
	rcRight.SetRect(146, 0, 160, 21);

	rtIdleBtn.SetUV3(113, 0, 127, 22, 127, 0, 168, 22, 168, 0, 183, 22, fTexWidth, fTexHeight );
	rtClickBtn.SetUV3(187, 0, 201, 22, 201, 0, 242, 22, 242, 0, 256, 22, fTexWidth, fTexHeight);

	// Restart button
	//m_btnRestart.Create( this, _S( 285, "����ŸƮ" ), 31, 73, 63, 21 );
	//m_btnRestart.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnRestart.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
	m_btnRestart.Create( this, _S(6104, "ĳ���� ���� ȭ������"), 9, 130, 160, 22);
	m_btnRestart.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnRestart.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnRestart.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnRestart.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnRestart.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnRestart.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnRestart.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnRestart.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnRestart.SetNewType(TRUE);

	// Exit button
	//m_btnExit.Create( this, _S( 286, "��������" ), 121, 73, 63, 21 );
	//m_btnExit.SetUV( UBS_IDLE, 0, 117, 63, 138, fTexWidth, fTexHeight );
	//m_btnExit.SetUV( UBS_CLICK, 66, 117, 129, 138, fTexWidth, fTexHeight );
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] ����۽� �ڵ� �α���
	m_btnExit.Create( this, _S( 5778, "�α��� ȭ������" ), 9, 158, 160, 22 );
#else
	m_btnExit.Create( this, _S( 286, "��������" ), 9, 158, 160, 22 );
#endif
	m_btnExit.SetRTSurface( UBS_IDLE, rcLeft, rtIdleBtn.rtL );
	m_btnExit.SetRTSurface( UBS_IDLE, rcMiddel, rtIdleBtn.rtM );
	m_btnExit.SetRTSurface( UBS_IDLE, rcRight, rtIdleBtn.rtR );
	m_btnExit.SetRTSurface( UBS_CLICK, rcLeft, rtClickBtn.rtL );
	m_btnExit.SetRTSurface( UBS_CLICK, rcMiddel, rtClickBtn.rtM );
	m_btnExit.SetRTSurface( UBS_CLICK, rcRight, rtClickBtn.rtR );
	m_btnExit.CopyRTSurface( UBS_IDLE, UBS_ON );
	m_btnExit.CopyRTSurface( UBS_IDLE, UBS_DISABLE );
	m_btnExit.SetNewType(TRUE);
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SetPos( pixMaxI - pUIManager->GetRadar()->GetWidth() - GetWidth(),
			pixMinJ + pUIManager->GetRadar()->GetHeight() + GetHeight() );
}

void CUISystemMenu::ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );

	g_iXPosInSystemMenu = GetPosX();
	g_iYPosInSystemMenu = GetPosY();
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set system menu texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Set Button texture
	pDrawPort->InitTextureData( m_ptdButtonTexture );

	// Close button
	m_btnClose.Render();

	// Option button
	m_btnOption.Render();

	// Help button
	m_btnHelp.Render();

	// Restart button
	m_btnRestart.Render();

	// Exit button
	m_btnExit.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in system menu
	//pDrawPort->PutTextEx( _S( 287, "�ý��� �޴�" ), m_nPosX + SYSMENU_TITLE_TEXT_OFFSETX,
	//									m_nPosY + SYSMENU_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( _S( 287, "�ý��� �޴�" ), m_nPosX + m_nWidth/2,
										m_nPosY + 18, 0xFFFFFFFF );


	CTString strServerInfo;
	strServerInfo.PrintF("%s - %d", pUIManager->GetServerSelect()->GetServerName(_pNetwork->m_iServerGroup), _pNetwork->m_iServerCh );

	pDrawPort->PutTextExCX( _S(4167, "�������� ����"), m_nPosX + m_nWidth/2,
										m_nPosY + 82, 0xFFFFFFFF );
	pDrawPort->PutTextExCX( strServerInfo, m_nPosX +m_nWidth/2,
										m_nPosY +99 , 0xFFFFFFFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::ToggleVisible()
{
	BOOL	bVisible = !IsVisible();
	ResetPosition( _pdpMain->dp_MinI, _pdpMain->dp_MinJ,
					_pdpMain->dp_MaxI, _pdpMain->dp_MaxJ );

	CUIManager::getSingleton()->RearrangeOrder( UI_SYSTEMMENU, bVisible );
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUISystemMenu::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

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

			// Move system menu
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
			// Option button
			else if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Help button
			else if( m_btnHelp.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Restart button
			else if( m_btnRestart.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Exit button
			else if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
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
				// Option button
				if( m_btnOption.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Help button
				if( m_btnHelp.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Restart button
				if( m_btnRestart.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Exit button
				if( m_btnExit.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_SYSTEMMENU, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If system menu isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						pUIManager->RearrangeOrder( UI_SYSTEMMENU, FALSE );

					return WMSG_SUCCESS;
				}
				// Option button
				else if( ( wmsgResult = m_btnOption.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						OpenOption();
					}

					return WMSG_SUCCESS;
				}
				// Help button
				else if( ( wmsgResult = m_btnHelp.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						OpenHelp();
					}

					return WMSG_SUCCESS;
				}
				// Restart button
				else if( ( wmsgResult = m_btnRestart.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_bCharacterMove = TRUE;
						Restart();
					}

					return WMSG_SUCCESS;
				}
				// Exit button
				else if( ( wmsgResult = m_btnExit.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						m_bCharacterMove = FALSE;
#ifdef AUTO_RESTART	// [2012/10/18 : Sora] ����۽� �ڵ� �α���
						Restart();
#else
						Exit();
#endif
					}

					return WMSG_SUCCESS;
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
				return WMSG_SUCCESS;
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
// Name : OpenOption()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::OpenOption()
{
	CUIManager::getSingleton()->GetOption()->ToggleOption();
}

// ----------------------------------------------------------------------------
// Name : OpenHelp()
// Desc : ���� ����.
// ----------------------------------------------------------------------------
void CUISystemMenu::OpenHelp()
{
	UIMGR()->GetHelpWebUI()->OpenUI();
}

// ----------------------------------------------------------------------------
// Name : Restart()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Restart()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_SYSMENU_RESTART ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "�ý��� �޴�" ), UMBS_YESNO,
								UI_SYSTEMMENU, MSGCMD_SYSMENU_RESTART );

	if( _pUIBuff->IsBuffBySkill( 1060 ) != NULL )	// �⼮��� ���� index�� 1060��
	{
		MsgBoxInfo.AddString( _S( 5592, "�⼮üũ�� ���� ���� �����Դϴ�." ) );
	}

	MsgBoxInfo.AddString( _S( 451, "������ ����� �Ͻðڽ��ϱ�?" ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : Restart_Internal()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Restart_Internal()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_btnRestart.SetBtnState(UBS_IDLE);
	m_btnExit.SetBtnState(UBS_IDLE);

	pUIManager->GetChattingUI()->ResetChatting();

	// Character state flags
	pUIManager->ResetCSFlag();

	_pNetwork->RestartGame();
	
	// �����ý��� �������� �ʱ�ȭ
	pUIManager->GetQuickSlot()->SetGiftRecv(FALSE);

	SlaveInfo().InitSkillIndex();

	// Reset buff
	_pUIBuff->ResetMyBuff();

	// Reset quest
	CQuestSystem::Instance().ClearAllDynamicData();
	CQuestSystem::Instance().ClearAllAllowList();
	//TODO : NewQuestSystem
	GAMEDATAMGR()->GetQuest()->ClearQuest();
	GAMEDATAMGR()->GetQuest()->ClearQuestList();
		
	// [090803 sora] ����ŸƮ�Ҷ� �����ߴ� ����Ʈ ������ �����ش�
	pUIManager->GetPlayerInfo()->ClearSelectedQuest();

	// Reset Notice
	pUIManager->GetNotice()->Clear();
	GAMEDATAMGR()->GetNotice()->clear();

	pUIManager->GetSiegeWarfare()->CloseSiegeWarfare();
	pUIManager->GetGuildBattle()->Clear();

	// �� AI Ŭ����
	pUIManager->GetWildPetInfoUI()->AIClear();
	
	// Reset Memory Scroll
	pUIManager->GetTeleport()->ClearTeleportList();
	pUIManager->GetTeleport()->SetUseTime(0);
	
	pUIManager->GetAffinityInfo()->AffinityReset();

	// clear all data in lacaball. [1/25/2011 rumist]
	pUIManager->GetLacaBall()->InitLacaballData();

	pUIManager->GetMonsterMercenary()->ToggleMonsterMercenary(-1);	

	// [11/3/2010 kiny8216] Face Change ��� �ʱ�ȭ
	pUIManager->GetInitJob()->SetFaceDecoMode( FALSE );	

	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ReturnSorcerer();

	// �ɳ��� ���� �ʱ�ȭ
	pUIManager->GetFlowerTree()->ClearList();
	pUIManager->GetTargetInfoUI()->ShowPKTargetHP(FALSE);

	extern BOOL	_bLoginProcess;
	_bLoginProcess = TRUE;
	_pGameState->GetGameMode() = CGameState::GM_NONE;
	pUIManager->GetGame()->StopGame();

	pUIManager->GetSimplePlayerInfo()->StopDungeonTime();

	GameDataManager* pGameData = GameDataManager::getSingleton();

	pGameData->GetExpressData()->ClearNPCInfo();
	pGameData->GetExpressData()->SetNoticeFlag(0);
	
	if (pUIManager->GetRadar())
		pUIManager->GetRadar()->OffExpressNotice();

	// ���Ŵ��� �ʱ�ȭ
	pUIManager->GetAuction()->reset();
	pUIManager->GetAuction()->CloseAuction();
	pGameData->GetAuction()->reset();

	// New skill tree UI reset
	pUIManager->GetSkillNew()->InitArrayData();

	// ���� â��
	pUIManager->GetWareHouseUI()->SetUseTime(0);
	pGameData->GetSyndicate()->ResetSyndiInfo();

	// ĳ���� ����â �ʱ�ȭ (���������� �ʱ�ȭ �ȵǴ� ������ �����. ����ڵ�� �߰�)
	if (_pNetwork->bMoveCharacterSelectUI == FALSE)
	{
		_pGameState->ClearCharacterSlot();
		_pGameState->SetCreatableNightShadow( FALSE );
	}

	pUIManager->ResetHoldBtn();

	for (int i = 0; i < _pNetwork->wo_iNumOfSkill; ++i)
	{
		CSkill* pSkill = &_pNetwork->GetSkillData(i);

		if (pSkill == NULL)
			continue;

		if (pSkill->GetFlag() & SF_TOGGLE)
			pSkill->SetToggle(false);
	}

	CItemCollectionData::ClearData();
	TOOLTIPMGR()->clearTooltip();

	// Ŀ���� Ÿ��Ʋ ���� ����
	CustomTitleData::clearCustomItemInfo();

	pUIManager->GetNewsWebUI()->clear_param();

	CNews* pNews = GAMEDATAMGR()->GetNews();

	if (pNews != NULL)
		pNews->ClearData();
}

// ----------------------------------------------------------------------------
// Name : Exit()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::Exit()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_SYSMENU_EXIT ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "�ý��� �޴�" ), UMBS_YESNO,
								UI_SYSTEMMENU, MSGCMD_SYSMENU_EXIT );
// [2011/09/22 : Sora] ���� ����� �⼮Ȯ�� �޽��� �߰� Gamigo���� ������ ������ ���� ����
	if( _pUIBuff->IsBuffBySkill( 1060 ) != NULL )	// �⼮��� ���� index�� 1060��
	{
		MsgBoxInfo.AddString( _S( 5592, "�⼮üũ�� �������� �����Դϴ�." ) );
	}
	MsgBoxInfo.AddString( _S( 288, "������ �����Ͻðڽ��ϱ�?" ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUISystemMenu::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_SYSMENU_EXIT:
		if( bOK )
		{
			if(IsHaveRelic() == true)
			{
				LogoutRelicWarring();
				return;
			}
			g_iXPosInSystemMenu = GetPosX();
			g_iYPosInSystemMenu = GetPosY();
			
			_pNetwork->SendClickObject(-1);
			_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );

			_pGameState->Running() = FALSE;
			_pGameState->QuitScreen() = FALSE;	
		}
		break;

	case MSGCMD_SYSMENU_RESTART:
		if( bOK )
		{
			if(IsHaveRelic() == true)
			{
				LogoutRelicWarring();
				return;
			}

			g_iXPosInSystemMenu = GetPosX();
			g_iYPosInSystemMenu = GetPosY();

			_pNetwork->SendClickObject(-1);
			_pGameState->SetRestartGameValue(TRUE);
			m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

			m_btnRestart.SetBtnState(UBS_DISABLE);
			m_btnExit.SetBtnState(UBS_DISABLE);
		}
		else
		{
			g_bAutoRestart = FALSE;
		}
		break;

	case MSGCMD_LOGOUT_RELIC_WARRING:
		{
			if (bOK)
			{
				if (m_bCharacterMove == TRUE)
				{
					g_iXPosInSystemMenu = GetPosX();
					g_iYPosInSystemMenu = GetPosY();

					_pNetwork->SendClickObject(-1);
					_pGameState->SetRestartGameValue(TRUE);
					m_llStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				}
				else
				{
					g_iXPosInSystemMenu = GetPosX();
					g_iYPosInSystemMenu = GetPosY();

					_pNetwork->SendClickObject(-1);
					_pNetwork->LeavePet( (CPlayerEntity*)CEntity::GetPlayerEntity(0) );

					_pGameState->Running() = FALSE;
					_pGameState->QuitScreen() = FALSE;	
				}
			}
			else
			{
				m_btnRestart.SetBtnState(UBS_IDLE);
				m_btnExit.SetBtnState(UBS_IDLE);
			}
		}
		break;
	}
}

void CUISystemMenu::ReStartNow()
{
	if ( m_bCharacterMove == FALSE)
	{
		STAGEMGR()->SetNextStage(eSTAGE_GAMEEND);
		return;
	}
	
	_pNetwork->SendRestartGame();	
}

void CUISystemMenu::CancelRestart()
{
	_UIAutoHelp->ClearGMNNotice();
	ResetRestartTime();
	_pGameState->SetRestartGameValue(FALSE);
	m_btnRestart.SetBtnState(UBS_IDLE);
	m_btnExit.SetBtnState(UBS_IDLE);
}

bool CUISystemMenu::IsHaveRelic()
{
	CUIInventory* pInven = UIMGR()->GetInventory();

	// �κ��丮�� ������ �ִ��� üũ
	int nTab, nInvenIdx;
	if (pInven->GetItemSlotInfo(DEF_RELIC_ITEM1, nTab, nInvenIdx) ||
		pInven->GetItemSlotInfo(DEF_RELIC_ITEM2, nTab, nInvenIdx) ||
		pInven->GetItemSlotInfo(DEF_RELIC_ITEM3, nTab, nInvenIdx) )
	{
		return true;
	}

	// ������ �������� ������ ������ �ִ��� üũ
	if (pInven->IsWearingItem(DEF_RELIC_ITEM1) ||
		pInven->IsWearingItem(DEF_RELIC_ITEM2) ||
		pInven->IsWearingItem(DEF_RELIC_ITEM3))
	{
		return true;
	}
	
	return false;
}

void CUISystemMenu::LogoutRelicWarring()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_LOGOUT_RELIC_WARRING ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 287, "�ý��� �޴�" ), UMBS_YESNO,
		UI_SYSTEMMENU, MSGCMD_LOGOUT_RELIC_WARRING );
	MsgBoxInfo.AddString( _S( 6411, "�α׾ƿ� �ϰų� ĳ���� ���� �� ���� �������� ������ϴ�. �׷��� �����ðڽ��ϱ�??" ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );

	m_btnRestart.SetBtnState(UBS_DISABLE);
	m_btnExit.SetBtnState(UBS_DISABLE);
}
