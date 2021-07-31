#include "stdh.h"

// ��� ����. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Interface/UIPortal.h>
#include <Engine/World/World.h>
#include <algorithm>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>

// [KH_07044] 3�� ���� ���� �߰�
extern INDEX g_iShowHelp1Icon;

// ----------------------------------------------------------------------------
// Name : CUIPortal()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIPortal::CUIPortal()
{
	// wooss 060515 
	m_npcIdx =-1;
	m_UniItemIdx = -1;
	m_bUseItem = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ~CUIPortal()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIPortal::~CUIPortal()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 301, 22 );

	// Create portal texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Portal.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 301, 47, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 49, 301, 68, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 71, 301, 101, fTexWidth, fTexHeight );

	// Ok button
	m_btnOK.Create( this, _S( 221, "�̵�" ), 161, 171, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 360, 2, 423, 23, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 426, 2, 489, 23, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );
	m_btnOK.SetEnable(FALSE);

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "���" ), 227, 171, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 360, 2, 423, 23, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 426, 2, 489, 23, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 269, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 306, 1, 320, 15, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 321, 1, 335, 15, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box
	m_lbZoneList.Create( this, 12, 47, 267, 120, 15, 6, 2, 2, TRUE );
	m_lbZoneList.CreateScroll( TRUE, 5, 0, 9, 121, 9, 7, 0, 0, 10 );
	m_lbZoneList.SetSelBar( 267, 15, 325, 26, 335, 41, fTexWidth, fTexHeight );
	m_lbZoneList.SetColumnPosX( 1, 120 );
	m_lbZoneList.SetOverColor( 0xF8E1B5FF );
	m_lbZoneList.SetSelectColor( 0xF8E1B5FF );
	// Up button
	m_lbZoneList.SetScrollUpUV( UBS_IDLE, 337, 2, 346, 9, fTexWidth, fTexHeight );
	m_lbZoneList.SetScrollUpUV( UBS_CLICK, 347, 2, 356, 9, fTexWidth, fTexHeight );
	m_lbZoneList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbZoneList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbZoneList.SetScrollBarTopUV( 307, 16, 316, 26, fTexWidth, fTexHeight );
	m_lbZoneList.SetScrollBarMiddleUV( 307, 27, 316, 29, fTexWidth, fTexHeight );
	m_lbZoneList.SetScrollBarBottomUV( 307, 30, 316, 40, fTexWidth, fTexHeight );
	// Down button
	m_lbZoneList.SetScrollDownUV( UBS_IDLE, 337, 10, 346, 17, fTexWidth, fTexHeight );
	m_lbZoneList.SetScrollDownUV( UBS_CLICK, 347, 10, 356, 17, fTexWidth, fTexHeight );
	m_lbZoneList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbZoneList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::OpenPortal( FLOAT fX, FLOAT fZ )
{
	if (IsVisible()) return;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int	nX = ( pDrawPort->dp_MinI + pDrawPort->dp_MaxI ) / 2 - m_nWidth / 2;
	int	nY = ( pDrawPort->dp_MinJ + pDrawPort->dp_MaxJ ) / 2 - m_nHeight / 2;

	SetPos( nX, nY );

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	for( int i = 0; i < m_vectorListInfo.size(); i++ )
	{
		m_lbZoneList.AddString( 0, CZoneInfo::getSingleton()->GetZoneName( m_vectorListInfo[i].zone ), 0xC0C0C0FF );
		m_lbZoneList.AddString( 1, CZoneInfo::getSingleton()->GetExtraName( m_vectorListInfo[i].zone, m_vectorListInfo[i].extra ),
								0xC0C0C0FF );
	}

	pUIManager->RearrangeOrder( UI_PORTAL, TRUE );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
		Close();

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set portal texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	int	nY = m_nPosY + 47;
	int	nY2 = m_nPosY + m_nHeight - 30;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, nY,
						m_rtBackTop.U0, m_rtBackTop.V0, m_rtBackTop.U1, m_rtBackTop.V1,
						0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY2,
						m_rtBackMiddle.U0, m_rtBackMiddle.V0, m_rtBackMiddle.U1, m_rtBackMiddle.V1,
						0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, nY2, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
						m_rtBackBottom.U0, m_rtBackBottom.V0, m_rtBackBottom.U1, m_rtBackBottom.V1,
						0xFFFFFFFF );

	// Close button
	m_btnClose.Render();

	// Ok button
	m_btnOK.Render();

	// Cancel button
	m_btnCancel.Render();

	// List box
	m_lbZoneList.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// text render
	pDrawPort->PutTextEx( _S( 222, "��Ż" ),
					m_nPosX + PORTAL_TITLE_TEXT_OFFSETX, m_nPosY + PORTAL_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );
	pDrawPort->PutTextEx( _S( 223, "��" ),
					m_nPosX + PORTAL_ZONE_TEXT_POSX, m_nPosY + PORTAL_ZONE_TEXT_POSY, 0xFFB500FF );
	pDrawPort->PutTextEx( _S( 224, "���" ),
					m_nPosX + PORTAL_PLACE_TEXT_POSX, m_nPosY + PORTAL_PLACE_TEXT_POSY, 0xFFB500FF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIPortal::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse message
	switch(pMsg->message)
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}

			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			if( m_lbZoneList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( !IsInside( nX, nY ) )	//���� ����� ���,
			{
				Close();
				break;
			}

			nOldX = nX;		nOldY = nY;
			
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL ||
				m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
			{
			}
			else if( IsInsideRect( nX, nY, m_rcTitle ) )
			{
				bTitleBarClick = TRUE;
			}
			else if( ( wmsgResult = m_lbZoneList.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
					m_btnOK.SetEnable( TRUE );
			}

			CUIManager::getSingleton()->RearrangeOrder( UI_PORTAL, TRUE );
			return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				bTitleBarClick = FALSE;

				if( !IsFocused() )
					return WMSG_FAIL;

				if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL ||
					( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND)
						Close();

					return WMSG_SUCCESS;
				}

				if( ( wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if(wmsgResult == WMSG_COMMAND)
					{
						int	nSel = m_lbZoneList.GetCurSel();
						if( nSel != -1 )
						{
							if (m_bUseItem)
							{
								_pNetwork->SendUseGoDungeon(m_UniItemIdx, m_vectorListInfo[nSel].zone, m_vectorListInfo[nSel].extra);
							}
							else
							{
								//�����̽þ�
								//������ ���� �̿��� �������� �����Ϸ� �̵��� �� �޼���â ����		FRANCE_SPAIN_CLOSEBETA_NA_20081124
#if !defined(G_HONGKONG) && !defined(G_KOR) && !defined(G_THAI)
								if( m_vectorListInfo[nSel].zone ==15 && _pNetwork->MyCharacterInfo.zoneNo !=17
									&& _pNetwork->MyCharacterInfo.zoneNo != 31)
								{
									CUIMsgBox_Info	MsgBoxInfo;
									CTString strTitle	=_S( 221, 	"�̵�" );	
									CTString strMessage	=_S(3223, "������ �������� �̵��ϱ� ���� 10,000,000 ������ �Һ�˴ϴ�. �̵��Ͻðڽ��ϱ�?" );
									MsgBoxInfo.SetMsgBoxInfo(strTitle,UMBS_YESNO,UI_NONE, MSGCMD_GOTO_EGEHA);
									MsgBoxInfo.AddString(strMessage);
									pUIManager->CreateMessageBox(MsgBoxInfo);
									
									return WMSG_SUCCESS;
								}	
#endif
								_pNetwork->GoZone( m_vectorListInfo[nSel].zone, m_vectorListInfo[nSel].extra ,m_npcIdx);
							}
							
							Close();
						}
					}

					return WMSG_SUCCESS;
				}

				if( m_lbZoneList.MouseMessage(pMsg) != WMSG_FAIL )
					return WMSG_SUCCESS;
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
				if( m_lbZoneList.MouseMessage(pMsg) != WMSG_FAIL )
				{
				}

				return WMSG_SUCCESS;
			}
		} break;

	case WM_MOUSEWHEEL:
		{
			if( m_lbZoneList.MouseMessage(pMsg) != WMSG_FAIL )
				return WMSG_SUCCESS;
		} break;
	}

	return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : ResetZoneList()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::ResetZoneList()
{
	// Reset list box
	m_lbZoneList.ResetAllStrings();

	// Reset list info
	m_vectorListInfo.clear();

	// Button
	m_btnOK.SetEnable(FALSE);

	// wooss 060515 Reset Npc Idx
	m_npcIdx = -1;
}

// ----------------------------------------------------------------------------
// Name : AddToZoneList()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::AddToZoneList(int nZone, int nExtra)
{
	m_vectorListInfo.push_back(sZoneExtraIndex(nZone, nExtra));
	std::sort(m_vectorListInfo.begin(), m_vectorListInfo.end());
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
void CUIPortal::Close()
{
	ResetZoneList();
	m_bUseItem = FALSE;
	CUIManager::getSingleton()->RearrangeOrder( UI_PORTAL, FALSE );
}

// ----------------------------------------------------------------------------
// Name : Close()
// Desc :
// ----------------------------------------------------------------------------
int CUIPortal::GetZoneListCount()
{
	return m_vectorListInfo.size();
}

void CUIPortal::GotoCurSelZone()
{
	int	nSel = m_lbZoneList.GetCurSel();

	if (nSel < 0)
		return;

	_pNetwork->GoZone( m_vectorListInfo[nSel].zone, m_vectorListInfo[nSel].extra ,m_npcIdx);
	Close();
}

///////////////////////////////////////////////////////////////////////////
// [071115: Su-won] DRATAN_SIEGE_DUNGEON
void CUIPortal::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CTString tv_str; 
	
	switch( nCommandCode )
	{
	case MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ:
		{
			// [090527: selo] Ȯ���� ����Ʈ �̾߱� �Ѵ� ó�� ������ ���� ��ƾ
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}

			switch( nResult )
			{
			case DRATAN_SIEGE_DUNGEON_ENTER:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_DUNGEON_ENTER, MSG_DUNGEON_ENTER_FEE);
				}
				break;
			case DRATAN_SIEGE_DUNGEON_STATE:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_DUNGEON_INFO);
				}
				break;
			case DRATAN_SIEGE_DUNGEON_CONTROL:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_CONFIRM);
				}
				break;
			case ciQuestClassifier:
				{
					// ������ ����Ʈ�� ���� ���� �Ǵ� ���� â�� ����.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );
				}
				break;
			}
		}
		break;
	case MSGLCMD_DRATAN_SIEGE_DUNGEON_CONTROL:
		{
			switch(nResult)
			{
			case DRATAN_SIEGE_DUNGEON_CONTROL_ENVIRONMENT:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_ENV_INFO);
				}
				break;
			case DRATAN_SIEGE_DUNGEON_CONTROL_MONSTER:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_MONSTER_INFO);
				}
				break;
			case DRATAN_SIEGE_DUNGEON_CONTROL_ADMISSION_FEE:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_TAX_INFO);
				}
				break;
			case DRATAN_SIEGE_DUNGEON_CONTROL_HUNTING_FEE:
				{
					_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_HUNTER_TAX_INFO);
				}
				break;
			}
			break;
		}
	}
}

void CUIPortal::OpenDratanDungeonMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxLExist(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ))
		return;

	pUIManager->CreateMessageBoxL( _S(3901, "���ź ī���� ���� ���� ������"),UI_PORTAL, MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ);
	pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ,TRUE, _S(3902, "�������. �� ī�������� ���� �������Դϴ�."),-1,0xa3a1a3ff);
	pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ,TRUE, _S(3903, "�̰� ������ ũ����Ż�� ���� �游�� ������ �� ���� �̲��� ��뿡������ ������ ������ ���͵��� ���Ӿ��� ���ܳ��� �ֽ��ϴ�."),-1,0xa3a1a3ff);
	pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ,TRUE, _S(3904, "��̷ο� ������ �ִٰ� ������ ��� ���� �Ͼ�� ����� �� �����Ƿ� �ڽ��� ���ٸ� �������� �ʴ� ���� �����ϴ�."),-1,0xa3a1a3ff);
	pUIManager->AddMessageBoxLString(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ,TRUE, _S(3905, "��, ��� �Ͻðڽ��ϱ�?"),-1,0xa3a1a3ff);

	CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ);

	pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, FALSE, _S(3906, "���� �����ϱ�" ), DRATAN_SIEGE_DUNGEON_ENTER);
	pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, FALSE, _S(3907, "���� ����Ȯ��" ), DRATAN_SIEGE_DUNGEON_STATE);
	pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, FALSE, _S(3908, "���� ����" ), DRATAN_SIEGE_DUNGEON_CONTROL);			
	pUIManager->AddMessageBoxLString( MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, FALSE, _S( 1220, "����Ѵ�." ) );		

	return;
}

void CUIPortal::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGCMD_SIEGE_DUNGEON_ENTER:
		{
			if( bOK )
			{
				_pNetwork->SendDratanSiegeDungeonMSG(MSG_DUNGEON_ENTER, MSG_DUNGEON_GO);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL:
		{
			if( bOK )
			{
				if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CHANGE) )
					pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CHANGE);

				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;
				int nRate =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->GetSpinButton().GetDataToNumber();
				
				strMessage.PrintF( _S( 191, "Ȯ��" ) );
				MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CHANGE);
				strMessage.PrintF( _S(3909, "���� ���¿��� %d%%��ŭ �� �����ϰ� �Ͻðڽ��ϱ�?" ), nRate );
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			else
			{
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL);
				pUIManager->GetPortal()->MsgBoxLCommand(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, DRATAN_SIEGE_DUNGEON_CONTROL);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL:
		{
			if( bOK )
			{
				if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_MONSTER_CHANGE) )
					pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CHANGE);

				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;
				int nRate =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->GetSpinButton().GetDataToNumber();
				
				strMessage.PrintF( _S( 191, "Ȯ��" ) );
				MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_MONSTER_CHANGE);
				strMessage.PrintF( _S(3910, "���� ���¿��� %d%%��ŭ �� �¼��ϰ� �Ͻðڽ��ϱ�?" ), nRate );
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			else
			{
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL);
				pUIManager->GetPortal()->MsgBoxLCommand(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, DRATAN_SIEGE_DUNGEON_CONTROL);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL:
		{
			if( bOK )
			{
				if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CHANGE) )
					pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CHANGE);

				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;
				int nFee =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL)->GetSpinButton().GetDataToNumber();
				
				strMessage.PrintF( _S( 191, "Ȯ��" ) );
				MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CHANGE);
				strMessage.PrintF( _S(3911, "����Ḧ %d%%�� �����Ͻðڽ��ϱ�?" ), nFee );
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			else
			{
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL);
				pUIManager->GetPortal()->MsgBoxLCommand(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, DRATAN_SIEGE_DUNGEON_CONTROL);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL:
		{
			if( bOK )
			{
				if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CHANGE) )
					pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CHANGE);

				CUIMsgBox_Info MsgBoxInfo;
				CTString strMessage;
				int nFee =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL)->GetSpinButton().GetDataToNumber();
				
				strMessage.PrintF( _S( 191, "Ȯ��" ) );
				MsgBoxInfo.SetMsgBoxInfo(strMessage,UMBS_YESNO,UI_PORTAL,MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CHANGE);
				strMessage.PrintF( _S(3912, "���Ƽ����� %d%%�� �����Ͻðڽ��ϱ�?" ), nFee );
				MsgBoxInfo.AddString(strMessage);
				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
			else
			{
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL);
				pUIManager->GetPortal()->MsgBoxLCommand(MSGLCMD_DRATAN_SIEGE_DUNGEON_REQ, DRATAN_SIEGE_DUNGEON_CONTROL);
			}
		}
		break;
		
	case MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CHANGE:
		{
			if( bOK )
			{
				int nRate =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->GetSpinButton().GetDataToNumber();
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL);

				_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_ENV_CHANGE, nRate);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_MONSTER_CHANGE:
		{
			if( bOK )
			{
				int nRate =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->GetSpinButton().GetDataToNumber();
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL);

				_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_MONSTER_CHANGE, nRate);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CHANGE:
		{
			if( bOK )
			{
				int nFee =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL)->GetSpinButton().GetDataToNumber();
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL);

				_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_TAX_CHANGE, nFee);
			}
		}
		break;
	case MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CHANGE:
		{
			if( bOK )
			{
				int nFee =pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL)->GetSpinButton().GetDataToNumber();
				pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL);

				_pNetwork->SendDratanSiegeDungeonMSG(MSG_MANAGEMENT, MSG_MANAGEMENT_MANAGER_HUNTER_TAX_CHANGE, nFee);
			}
		}
		break;
	}
}

void CUIPortal::Create_SiegeDungeon_State_MsgBox(int nEnvRate, int nMonRate)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_STATE) )
		pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_STATE);

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S(3907, "���� ����Ȯ��" ), UMBS_OK | UMBS_INPUTBOX | UMBS_SECOND_INPUTBOX , UI_PORTAL, MSGCMD_SIEGE_DUNGEON_STATE, 400 );
	
	MsgBoxInfo.AddString( _S(3913, "�� ���� ���� ȯ��") );
	MsgBoxInfo.m_nMaxRow +=1;
	MsgBoxInfo.AddString( _S(3914, "������") );
	--MsgBoxInfo.m_nMaxRow;
	MsgBoxInfo.AddString( _s("50%"), 0xF2F2F2FF, TEXT_CENTER );
	--MsgBoxInfo.m_nMaxRow;
	MsgBoxInfo.AddString( _S(3915, "������"), 0xF2F2F2FF, TEXT_RIGHT );
	MsgBoxInfo.m_nMaxRow +=2;
	MsgBoxInfo.AddString( _S(3916, "(�����Ҽ��� ĳ������ ���ݷ�&������ ��µ˴ϴ�.)") );
	MsgBoxInfo.m_nMaxRow +=2;

	MsgBoxInfo.AddString( _S(3917, "�� ������ ������") );
	MsgBoxInfo.m_nMaxRow +=1;
	MsgBoxInfo.AddString( _S(3918, "�ſ� ������") );
	--MsgBoxInfo.m_nMaxRow;
	MsgBoxInfo.AddString( _s("50%"), 0xF2F2F2FF, TEXT_CENTER );
	--MsgBoxInfo.m_nMaxRow;
	MsgBoxInfo.AddString( _S(3919, "�¼���"), 0xF2F2F2FF, TEXT_RIGHT );
	MsgBoxInfo.m_nMaxRow +=2;
	MsgBoxInfo.AddString( _S(3920, "(�¼��Ҽ��� ������ ���ݷ�&������ �϶��մϴ�.)") );
	MsgBoxInfo.m_nMaxRow +=1;

	MsgBoxInfo.SetInputBox( 3, 0, 0, 370 );
	MsgBoxInfo.SetSEInputBox( 11, 0, 0, 370 );
	MsgBoxInfo.m_nInputPosY += _pUIFontTexMgr->GetLineHeight() *0.5f;
	MsgBoxInfo.m_nSEInputPosY += _pUIFontTexMgr->GetLineHeight() *0.5f;

	MsgBoxInfo.m_nColorBoxCount =2;

	pUIManager->CreateMessageBox( MsgBoxInfo );

	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_STATE)->GetInputBox().SetFocus(FALSE);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_STATE)->GetInputBox().SetInValidEditBox(TRUE);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_STATE)->GetSEInputBox().SetFocus(FALSE);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_STATE)->GetSEInputBox().SetInValidEditBox(TRUE);


	float fEnvRate =nEnvRate/100.0f;
	float fMonRate =nMonRate/100.0f;

	int iPosX[2], iPosY[2];
	int iWidth[2], iHeight;

	iPosX[0] =MsgBoxInfo.m_nInputPosX+2;
	iPosX[1] =MsgBoxInfo.m_nSEInputPosX+2;

	iPosY[0] =MsgBoxInfo.m_nInputPosY +2;
	iPosY[1] =MsgBoxInfo.m_nSEInputPosY +2;

	iWidth[0] =(MsgBoxInfo.m_nInputWidth-4)*fEnvRate;
	iWidth[1] =(MsgBoxInfo.m_nSEInputWidth-4)*fMonRate;

	iHeight =9;

	WRect rcBox_Environment;
	WRect rcBox_Monster;

	rcBox_Environment.SetRect( iPosX[0], iPosY[0], iPosX[0]+iWidth[0], iPosY[0]+iHeight);
	rcBox_Monster.SetRect( iPosX[1], iPosY[1], iPosX[1]+iWidth[1], iPosY[1]+iHeight);

	UBYTE ubEnvCol =5;
	UBYTE ubMonCol =7;

	if( nEnvRate <=10 )
		ubEnvCol =6;

	if( nMonRate <=10 )
		ubMonCol =6;

	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_STATE)->SetColorBox(0, ubEnvCol, rcBox_Environment);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_STATE)->SetColorBox(1, ubMonCol, rcBox_Monster);
}

void CUIPortal::Create_SiegeDungeon_EnvCtr_MsgBox(int nCurRate)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL) )
		pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL);

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S(3921, "���� ���� ȯ�� ����" ), UMBS_OKCANCEL | UMBS_SPINBUTTON , UI_PORTAL, MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL, 400 );
	
	MsgBoxInfo.AddString( _S(3922, "ũ����Ż�� ����� ����� ������ ���� ȯ���� �����ϰ� ���� �� �ֽ��ϴ�. ������ ȯ���� ���������� ĳ������ ���ݷ°� ������ ��µ˴ϴ�.") );
	MsgBoxInfo.AddString( _S(3923, "��, �������� ��� ��Ű�� ���ؼ��� ������ �ʿ��ϸ�, �������� 1�ð�(���� �ð�)�� 10%�� �϶��˴ϴ�.") );
	++MsgBoxInfo.m_nMaxRow;
	MsgBoxInfo.AddString( _S(3924, "��� ������ �����ϰ� ����ðڽ��ϱ�?") );
	MsgBoxInfo.m_nMaxRow +=2;

	int nNeedNas = 50000*10;
		
	strMessage.PrintF( _s("%s: %d %s%s"), _S(1058, "�ʿ䳪��"), nNeedNas, _S(1762, "����"), _s("   ") );
	MsgBoxInfo.AddString( strMessage, 0xF2F2F2FF, TEXT_RIGHT );

	
	MsgBoxInfo.m_nColorBoxCount =4;

	MsgBoxInfo.SetSpinButton(MsgBoxInfo.m_nMaxRow-2, 1, 230, _S(3925, "������ ���%") + _s("   "));

	pUIManager->CreateMessageBox( MsgBoxInfo );

	int iPosX[4], iPosY[4];
	int iWidth[4], iHeight[4];

	iPosX[0] =MsgBoxInfo.m_nSpinButtonPosX -6;
	iPosY[0] =MsgBoxInfo.m_nSpinButtonPosY -3;
	iWidth[0] =0;
	iHeight[0] = 32;

	iPosX[1] =iPosX[0];
	iPosY[1] =iPosY[0];
	iWidth[1] =MsgBoxInfo.m_nWidth -iPosX[0]*2;
	iHeight[1] = 0;

	iPosX[2] =MsgBoxInfo.m_nWidth -iPosX[0];
	iPosY[2] =iPosY[0];
	iWidth[2] =0;
	iHeight[2] = 34;

	iPosX[3] =iPosX[0];
	iPosY[3] =iPosY[0] +32;
	iWidth[3] =iWidth[1];
	iHeight[3] = 0;


	WRect rcBox[4];

	rcBox[0].SetRect( iPosX[0], iPosY[0], iPosX[0]+iWidth[0], iPosY[0]+iHeight[0]);
	rcBox[1].SetRect( iPosX[1], iPosY[1], iPosX[1]+iWidth[1], iPosY[1]+iHeight[1]);
	rcBox[2].SetRect( iPosX[2], iPosY[2], iPosX[2]+iWidth[2], iPosY[2]+iHeight[2]);
	rcBox[3].SetRect( iPosX[3], iPosY[3], iPosX[3]+iWidth[3], iPosY[3]+iHeight[3]);

	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->SetColorBox(0, 0, rcBox[0]);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->SetColorBox(1, 0, rcBox[1]);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->SetColorBox(2, 0, rcBox[2]);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->SetColorBox(3, 0, rcBox[3]);
	
	for(int i=1; i<=10; ++i)
	{
		if( (i*10 +nCurRate) >100 )
			break;
		
		CTString strData;
		strData.PrintF("%d", i*10);
		pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ENVIRONMENT_CONTROL)->GetSpinButton().AddData(strData);
	}
}

void CUIPortal::Create_SiegeDungeon_MonCtr_MsgBox(int nCurRate)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL) )
		pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL);

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S(3926, "���� ���� ���� ����" ), UMBS_OKCANCEL | UMBS_SPINBUTTON , UI_PORTAL, MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL, 400 );
	
	MsgBoxInfo.AddString( _S(3927, "ũ����Ż�� ������� ������ �ִ� ���͵��� �¼��ϰ� ���� �� �ֽ��ϴ�. ������ ���Ͱ� �¼������� ������ ���ݷ°� ����, �ִ� HP�� �϶��մϴ�.") );
	MsgBoxInfo.AddString( _S(3928, "��, ������ �¼����� ��� ��Ű�� ���ؼ��� ������ �ʿ��ϸ�, �¼����� 1�ð�(����ð�)�� 10%�� �϶��˴ϴ�.") );
	++MsgBoxInfo.m_nMaxRow;
	MsgBoxInfo.AddString( _S(3929, "��� ������ �¼��ϰ� ����ðڽ��ϱ�?") );
	MsgBoxInfo.m_nMaxRow +=2;

	int nNeedNas = 100000*10;
		
	strMessage.PrintF( _s("%s: %d %s%s"), _S(1058, "�ʿ䳪��"), nNeedNas, _S(1762, "����"), _s("   ") );
	MsgBoxInfo.AddString( strMessage, 0xF2F2F2FF, TEXT_RIGHT );
	
	MsgBoxInfo.m_nColorBoxCount =4;

	MsgBoxInfo.SetSpinButton(MsgBoxInfo.m_nMaxRow-2, 1, 200, _S(3930, "�¼��� ���%") + _s("   "));

	pUIManager->CreateMessageBox( MsgBoxInfo );

	int iPosX[4], iPosY[4];
	int iWidth[4], iHeight[4];

	iPosX[0] =MsgBoxInfo.m_nSpinButtonPosX -6;
	iPosY[0] =MsgBoxInfo.m_nSpinButtonPosY -3;
	iWidth[0] =0;
	iHeight[0] = 32;

	iPosX[1] =iPosX[0];
	iPosY[1] =iPosY[0];
	iWidth[1] =MsgBoxInfo.m_nWidth -iPosX[0]*2;
	iHeight[1] = 0;

	iPosX[2] =MsgBoxInfo.m_nWidth -iPosX[0];
	iPosY[2] =iPosY[0];
	iWidth[2] =0;
	iHeight[2] = 34;

	iPosX[3] =iPosX[0];
	iPosY[3] =iPosY[0] +32;
	iWidth[3] =iWidth[1];
	iHeight[3] = 0;


	WRect rcBox[4];

	rcBox[0].SetRect( iPosX[0], iPosY[0], iPosX[0]+iWidth[0], iPosY[0]+iHeight[0]);
	rcBox[1].SetRect( iPosX[1], iPosY[1], iPosX[1]+iWidth[1], iPosY[1]+iHeight[1]);
	rcBox[2].SetRect( iPosX[2], iPosY[2], iPosX[2]+iWidth[2], iPosY[2]+iHeight[2]);
	rcBox[3].SetRect( iPosX[3], iPosY[3], iPosX[3]+iWidth[3], iPosY[3]+iHeight[3]);

	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->SetColorBox(0, 0, rcBox[0]);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->SetColorBox(1, 0, rcBox[1]);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->SetColorBox(2, 0, rcBox[2]);
	pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->SetColorBox(3, 0, rcBox[3]);
	
	for(int i=1; i<=10; ++i)
	{
		if( (i*10 +nCurRate) >100 )
			break;

		CTString strData;
		strData.PrintF("%d", i*10);
		pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_MONSTER_CONTROL)->GetSpinButton().AddData(strData);
	}
}

void CUIPortal::Create_SiegeDungeon_AdmissionCtr_MsgBox(int nCurFree)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL) )
		pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL);

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S(3931, "���� ����� ����" ), UMBS_OKCANCEL | UMBS_SPINBUTTON , UI_PORTAL, MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL, 300 );
	
	MsgBoxInfo.AddString( _S(3932, "������ ����� ������ �������ּ���.") );

	CTString strMsg;
	strMsg.PrintF(_S(3933, "���� ����� ������ %d%%�Դϴ�."), nCurFree);
	MsgBoxInfo.AddString(strMsg);
	++MsgBoxInfo.m_nMaxRow;

	MsgBoxInfo.SetSpinButton(MsgBoxInfo.m_nMaxRow, 1, 200, _S(3934, "����� ����%") + _s("   "));

	pUIManager->CreateMessageBox( MsgBoxInfo );

	for(int i=0; i<17; ++i)
	{
		CTString strData;
		strData.PrintF("%d", 70+i*5);
		pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_ADMISSION_FEE_CONTROL)->GetSpinButton().AddData(strData);
	}

}

void CUIPortal::Create_SiegeDungeon_HuntingCtr_MsgBox(int nCurFree)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL) )
		pUIManager->CloseMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL);

	CTString strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	MsgBoxInfo.SetMsgBoxInfo( _S(3935, "���� ���Ƽ��� ����" ), UMBS_OKCANCEL | UMBS_SPINBUTTON , UI_PORTAL, MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL, 300 );
	
	MsgBoxInfo.AddString( _S(3936, "���� ���� ���Ͱ� ����ϴ� ���� �� ���Ƽ�����ŭ�� ������ ���ֱ���� �������� ���޵˴ϴ�.") );
	MsgBoxInfo.AddString( _S(3937, "������ ���Ƽ����� �������ּ���.") );
	CTString strMsg;
	strMsg.PrintF(_S(3938, "���� ���Ƽ����� %d%%�Դϴ�."), nCurFree);
	MsgBoxInfo.AddString(strMsg);
	++MsgBoxInfo.m_nMaxRow;

	MsgBoxInfo.SetSpinButton(MsgBoxInfo.m_nMaxRow, 1, 200, _S(3939, "���Ƽ���%") + _s("   "));

	pUIManager->CreateMessageBox( MsgBoxInfo );

	for(int i=0; i<5; ++i)
	{
		CTString strData;
		strData.PrintF("%d", i*5);
		pUIManager->GetMessageBox(MSGCMD_SIEGE_DUNGEON_HUNTING_FEE_CONTROL)->GetSpinButton().AddData(strData);
	}
}
// [071115: Su-won] DRATAN_SIEGE_DUNGEON
///////////////////////////////////////////////////////////////////////////

BOOL CUIPortal::IsWarpItem(int nItemIndex)
{
	switch (nItemIndex)
	{
	case 2860: // ���ǽ�Ʈ ���� �̵� �ֹ���
	case 2861: // ����Ű�� ���� �̵� �ֹ���
	case 2862: // ������ ���� �̵� �ֹ���
	case 2863: // �������� ���ϰ��� �̵� �ֹ���
	case 2864: // �罺Ʈ Ʈ�� �̵� �ֹ���
		return TRUE;
	}

	return FALSE;
}