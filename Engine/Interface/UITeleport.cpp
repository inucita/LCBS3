#include "stdh.h"

// ��� ����. [12/3/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITeleport.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Help/Util_Help.h>

extern INDEX g_iCountry;

static CTString	strTempMemo;

#define DEF_EXPIRETIME_SX (34)
#define DEF_EXPIRETIME_SY (136)

// ----------------------------------------------------------------------------
// Name : cipher
// Desc : Inner Function
// ----------------------------------------------------------------------------
int Tcipher(int num)
{
	int nCipher=1;
	int tmp;
	tmp=num/10;
	if(tmp>0) {
		nCipher++;
		Tcipher(tmp);
	}
	return nCipher;
}

// ----------------------------------------------------------------------------
// Name : CUITeleport()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUITeleport::CUITeleport()
{
	m_useTime = 0 ; // wooss �޸𸮽�ũ�� Ȯ��ī�� ��뿩��
	m_sbPage = 0;
	m_sbSelection = -1;

	for( int i = 0; i < TELEPORT_MAX_MEMO; i++)
	{
		m_strZone[i] = CTString( "" );
		m_strCoord[i] = CTString( "" );
		m_strMemo[i] = CTString( "" );
		m_nZoneNum[i] = -1;
	}
}

// ----------------------------------------------------------------------------
// Name : ~CUITeleport()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUITeleport::~CUITeleport()
{
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
//	m_rcTitle.SetRect( 0, 0, 427, 22 );
//	m_rcContents.SetRect( 12, 48, 416, 128 );

	// wooss 050810
	m_rcTitle.SetRect( 0, 0, 472, 22 );

	if (g_iCountry == RUSSIA)
		m_rcContents.SetRect( 12, 48, 452, 123 );
	else
		m_rcContents.SetRect( 12, 48, 418, 123 );

	// Create teleport texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Teleport.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
//	m_rtBackground.SetUV( 0, 0, 427, 160, fTexWidth, fTexHeight );
//	m_rtSelBar.SetUV( 0, 215, 404, 231, fTexWidth, fTexHeight );
	
	//wooss 050810
	m_rtBackground.SetUV( 0, 0, 472, 160, fTexWidth, fTexHeight );
	m_rtSelBar.SetUV( 0, 215, 350, 231, fTexWidth, fTexHeight );
	//scroll bar
	m_sbScrollBar.Create( this, 453, 47, 9, 82 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( TELEPORT_MAX_MEMO );
	m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_MEMO );
	m_sbScrollBar.SetItemsPerPage( TELEPORT_MAX_ROW );
	// Up button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 141,164, 150,170, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 150, 164, 159, 170,  fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 141, 172, 149, 178, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 151, 172, 159, 178, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbScrollBar.SetBarTopUV( 131, 164, 139, 170, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 131, 171, 139, 179, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 131, 180, 139, 188, fTexWidth, fTexHeight );
	// Wheel region
	m_sbScrollBar.SetWheelRect( -184, -1, 183, 176 );



	// Close Button
	m_btnClose.Create( this, CTString( "" ),438, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 182, 164, 196, 178, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 182, 179, 196, 193, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Move button
	m_btnMove.Create( this, _S( 289, "�̵�" ), 268, 133, 63, 21 );
	m_btnMove.SetUV( UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight );
	m_btnMove.SetUV( UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight );
	m_btnMove.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMove.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Memorize button
	m_btnMemorize.Create( this, _S( 290, "����" ), 334, 133, 63, 21 );
	m_btnMemorize.SetUV( UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight );
	m_btnMemorize.SetUV( UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight );
	m_btnMemorize.CopyUV( UBS_IDLE, UBS_ON );
	m_btnMemorize.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "���" ), 400, 133, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 164, 63, 185, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 164, 129, 185, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenTeleport()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::OpenTeleport()
{
	if( IsVisible() )
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 789, "�ٸ� �ൿ �߿��� �޸𸮽�ũ���� ����� �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}
	
	m_btnMove.SetEnable( FALSE );
	m_btnMemorize.SetEnable( FALSE );

	if(m_strZone[0] != "")
	{
		m_sbSelection = 0;
		m_btnMove.SetEnable( TRUE );
		m_btnMemorize.SetEnable( TRUE );
	}
	else
	{
		m_sbSelection = 0;
		m_btnMove.SetEnable( FALSE );
		m_btnMemorize.SetEnable( TRUE );
	}

	//modified by sam  ���þƿ��� ��ũ�ѹ��� �������� �̻��ϴ� �Ͽ� ������� �������� �����ؼ� ��ũ���� �̻��ϰ� ���� 
	if(m_useTime > 0) // 070205_ttos �޸� ���� 5ĭ ��� Ȯ��� 15ĭ ��밡��
	{
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_MEMO );
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_MEMO );
	}else 
	{
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_ROW );
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_ROW );
	}

// [KH_070315] �����̾��� �� �б�
	pUIManager->RearrangeOrder( UI_TELEPORT, TRUE );
}

// ----------------------------------------------------------------------------
// Name : NotUseMemEx wooss050816
// Desc : expire item-time 
// ----------------------------------------------------------------------------
void CUITeleport::NotUseMemEx()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_NULL ) )
		return;
	CTString	strTitle,strMsg;
	strTitle = _S(191,"Ȯ��");
	strMsg	 = "�޸� ��ũ�� Ȯ�� ī�带 ������� �ƴϰų� �̿�ð��� ����Ͽ����ϴ�.";
 	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( strTitle, UMBS_OK,UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMsg );
	pUIManager->CreateMessageBox(MsgBoxInfo);
}



// ----------------------------------------------------------------------------
// Name : CloseTeleport()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::CloseTeleport()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox( MSGCMD_TELEPORT_MEMO );

	m_sbSelection = -1;
// [KH_070315] �����̾��� �� �б�
	pUIManager->RearrangeOrder( UI_TELEPORT, FALSE );
}

// ----------------------------------------------------------------------------
// Name : OpenMemorizeMsgBox()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::OpenMemorizeMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
		return;

	if( m_sbSelection == -1 )
		return;

	CTString	strZone, strCoord;
	strZone.PrintF( "%-8s: %s", _S( 291, "��" ),
								CZoneInfo::getSingleton()->GetZoneName( _pNetwork->MyCharacterInfo.zoneNo ) );
	strCoord.PrintF( "%-8s: %d,%d", _S( 292, "��ǥ" ),
										(int)_pNetwork->MyCharacterInfo.x, (int)_pNetwork->MyCharacterInfo.z );

	CUIMsgBox_Info	MsgBoxInfo;
// [KH_070315] �����̾��� �� �б�
	MsgBoxInfo.SetMsgBoxInfo( _S( 375, "�޸� ��ũ��" ), UMBS_OKCANCEL | UMBS_INPUTBOX,
								UI_TELEPORT, MSGCMD_TELEPORT_MEMO );
	MsgBoxInfo.AddStringEx( strZone, 0, 0 );
	MsgBoxInfo.AddStringEx( strCoord, 1, 0 );
	MsgBoxInfo.AddStringEx( _S( 293, "�޸�" ), 2, 0 );

	if (g_iCountry == RUSSIA)
		MsgBoxInfo.SetInputBox( 3, 10, 25 ); // 2->3 ���� ������ ��ħ modified by sam 11/02/22
	else
		MsgBoxInfo.SetInputBox( 2, 10, 32 );

	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUITeleport::SetUseTime(int t)
{
	m_useTime = t;

	//modified by sam  ���þƿ��� ��ũ�ѹ��� �������� �̻��ϴ� �Ͽ� ������� �������� �����ؼ� ��ũ���� �̻��ϰ� ���� 
	if (m_useTime > 0)
	{
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_MEMO ); // added by sam 11/02/22
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_MEMO );
	}
	else
	{	
		m_sbScrollBar.SetScrollRange( TELEPORT_MAX_ROW ); // added by sam 11/02/22
		m_sbScrollBar.SetCurItemCount( TELEPORT_MAX_ROW );
	}
}
// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set teleport texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackground.U0, m_rtBackground.V0, m_rtBackground.U1, m_rtBackground.V1,
										0xFFFFFFFF );
	
	// Selection bar
	int	nY;
	if( m_sbSelection >= 0 )
	{
		nY = m_nPosY + m_rcContents.Top + m_sbSelection * TELEPORT_CONTENT_OFFSETY;
		pDrawPort->AddTexture( m_nPosX + m_rcContents.Left, nY,
											m_nPosX + m_rcContents.Right, nY + TELEPORT_CONTENT_OFFSETY,
											m_rtSelBar.U0, m_rtSelBar.V0, m_rtSelBar.U1, m_rtSelBar.V1,
											0xFFFFFFFF );
	}

	// Close button
	m_btnClose.Render();

	// Move butotn
	m_btnMove.Render();

	// Memorize button
	m_btnMemorize.Render();

	// Cancel button
	m_btnCancel.Render();

	//wooss 050810
	// Scroll Bar button
	m_sbScrollBar.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in teleport
	pDrawPort->PutTextEx( _S( 375, "�޸� ��ũ��" ), m_nPosX + TELEPORT_TITLE_OFFSETX,
										m_nPosY + TELEPORT_TITLE_OFFSETY, 0xFFFFFFFF );

	pDrawPort->PutTextExCX( _S( 291, "��" ), m_nPosX + TELEPORT_ZONE_CX,
											m_nPosY + TELEPORT_CONTENT_NAME_SY );

	pDrawPort->PutTextExCX( _S( 292, "��ǥ" ), m_nPosX + TELEPORT_COORD_CX,
											m_nPosY + TELEPORT_CONTENT_NAME_SY );
	pDrawPort->PutTextExCX( _S( 293, "�޸�" ), m_nPosX + TELEPORT_MEMO_CX,
											m_nPosY + TELEPORT_CONTENT_NAME_SY );

	//wooss 050810
	pDrawPort->PutTextExCX( _S( 2119, "�Ⱓ" ), m_nPosX + TELEPORT_REMAIN_CX,		
											m_nPosY + TELEPORT_CONTENT_NAME_SY );

	nY = m_nPosY + TELEPORT_CONTENT_SY;
	
	//wooss 050810
	int j;
	int i=j=m_sbScrollBar.GetScrollPos();
	
	for( ; i < TELEPORT_MAX_ROW+j; i++ )
	{
		char sNum[10] = {'\0',};
		itoa(i+1,sNum,10);
		int tv_c = Tcipher(i+1);
		pDrawPort->PutTextCharEx( sNum, tv_c, m_nPosX + TELEPORT_NUM_SX - (tv_c-1)*3, nY );

		if( m_nZoneNum[i] != -1 )
		{
			pDrawPort->PutTextExCX( m_strZone[i], m_nPosX + TELEPORT_ZONE_CX, nY );
			pDrawPort->PutTextExCX( m_strCoord[i], m_nPosX + TELEPORT_COORD_CX, nY );
			pDrawPort->PutTextEx( m_strMemo[i], m_nPosX + TELEPORT_MEMO_SX, nY );
			if(i<TELEPORT_MAX_ROW)	pDrawPort->PutTextEx( _S(2120,"����"), m_nPosX + TELEPORT_TIME_SX, nY );	
			else 
			{
				pDrawPort->PutTextEx( _S(6031,"Ȯ��") , m_nPosX + TELEPORT_TIME_SX, nY );	//wooss 050816
			}
		}

		nY += TELEPORT_CONTENT_OFFSETY;
	}

	if (m_useTime > 0)
	{
		tm* pTime = NULL;
		pTime = localtime((time_t*)&m_useTime);
		CTString strExpireTime;
	
		strExpireTime.PrintF(  _S( 6070,"���� : %d��%d��%d��%d��%d��"),pTime->tm_year + 1900
			,pTime->tm_mon + 1,pTime->tm_mday,pTime->tm_hour, pTime->tm_min);

		pDrawPort->PutTextEx( strExpireTime , m_nPosX + DEF_EXPIRETIME_SX, m_nPosY + DEF_EXPIRETIME_SY );
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUITeleport::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// check use extend memory scroll position
	int chkMemPos=(m_sbScrollBar.GetScrollPos()+m_sbSelection);

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move teleport
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
			// Move button
			else if( m_btnMove.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Memorize button
			else if( m_btnMemorize.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button
			else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;

			// wooss 050810
			// Scroll bar
			else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND )
				{
					//Nothing
				}
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
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
				// Move button
				else if( m_sbSelection >= 0 &&
						_pNetwork->MyCharacterInfo.zoneNo == m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()] &&
						m_btnMove.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Memorize button
				else if( m_sbSelection >= 0 && m_btnMemorize.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel button
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				
				// wooss 050810
				// Scroll bar
				else if( ( wmsgResult = m_sbScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						//Nothing
					}
				}
				
				
				// Selection content
				else if( IsInsideRect( nX, nY, m_rcContents ) )
				{
					if( !pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
					{
						UBYTE	ubOldSel = m_sbSelection;
						m_sbSelection = ( nY - m_nPosY - m_rcContents.Top ) / TELEPORT_CONTENT_OFFSETY;

// [KH_070316] �߰� 15�� 20�� ����
						if( m_sbSelection >= TELEPORT_MAX_MEMO )
							m_sbSelection = ubOldSel;

						if( m_sbSelection >= 0 )
						{
							m_btnMemorize.SetEnable( TRUE );

							int	nZoneNum = _pNetwork->MyCharacterInfo.zoneNo;
							if( nZoneNum == m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()] )
								m_btnMove.SetEnable( TRUE );
							else
								m_btnMove.SetEnable( FALSE );
						}
						else
						{
							m_btnMemorize.SetEnable( FALSE );
							m_btnMove.SetEnable( FALSE );
						}
					}
				}

// [KH_070315] �����̾��� �� �б�
				pUIManager->RearrangeOrder( UI_TELEPORT, TRUE );
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

				// If teleport isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseTeleport();

					return WMSG_SUCCESS;
				}
				// Move button
				else if( ( wmsgResult = m_btnMove.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
// 						if (_pNetwork->IsRvrZone())
// 						{
// 							pUIManager->GetChattingUI()->AddSysMessage( _S( 47, "�޸� ��ũ���� ����� �� ���� �����Դϴ�." ), SYSMSG_ERROR );
// 							return WMSG_FAIL;
// 						}

						if(m_useTime<=0 && chkMemPos>=TELEPORT_MAX_ROW) 
							NotUseMemEx();
						else 
							SendTeleportMove();
					}

					return WMSG_SUCCESS;
				}
				// Memorize button
				else if( ( wmsgResult = m_btnMemorize.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_useTime<=0 && chkMemPos>=TELEPORT_MAX_ROW)
							NotUseMemEx();
						else
							OpenMemorizeMsgBox();
					}

					return WMSG_SUCCESS;
				}
				// Cancel button
				else if( ( wmsgResult = m_btnCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						CloseTeleport();

					return WMSG_SUCCESS;
				}

				// Scroll bar
				else if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
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
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ClearTeleportList()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::ClearTeleportList()
{
	m_sbSelection = -1;

	for( int i = 0; i < TELEPORT_MAX_MEMO; i++ )
	{
		m_nZoneNum[i] = -1;
	}
}

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_TELEPORT_MEMO:
		{
			strTempMemo = strInput;
			SendTeleportWrite();
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowTeleportError()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::ShowTeleportError()
{
	CTString	strSysMessage;
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_SKILL ) )
		strSysMessage = _S( 376, "��ų�� ����ϰ� �ֽ��ϴ�." );
	else if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
		strSysMessage = _S( 377, "�ٸ� ��ҷ� �̵��� �غ��ϰ� �ֽ��ϴ�." );
	else if( pUIManager->IsCSFlagOn( CSF_PARTY_REQ ) )
		strSysMessage = _S( 378, "��Ƽ ��û�� �������Դϴ�." );
	else if( pUIManager->IsCSFlagOn( CSF_EXCHANGE ) )
		strSysMessage = _S( 379, "��ȯ�� �������Դϴ�." );
	else if( pUIManager->IsCSFlagOn( CSF_EXPEDITION_REQ ) ) // [sora] ������ ��û��
		strSysMessage = _S( 4656, "������ ��û�� �������Դϴ�.." );
	else
		strSysMessage = _S( 384, "�̵��� �� �����ϴ�." );

	pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendTeleportWrite()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::SendTeleportWrite()
{
	int tv_scrollbarPos=m_sbScrollBar.GetScrollPos();				// wooss 050816 
	if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 789, "�ٸ� �ൿ �߿��� �޸𸮽�ũ���� ����� �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}
// [KH_070316] ���� �����̾� ����

	if (UTIL_HELP()->IsSpecialChar(strTempMemo.str_String) == true)
	{
		UIMGR()->GetChattingUI()->AddSysMessage ( _S( 437, "���忡 ������ �ܾ ���ԵǾ� �ֽ��ϴ�." ) );
		return;
	}

	_pNetwork->SendTeleportWrite( MSG_MEMPOS, m_sbSelection + m_sbScrollBar.GetScrollPos(), strTempMemo );	//wooss 050816
	strTempMemo.Clear();
}

// ----------------------------------------------------------------------------
// Name : SendTeleportMove()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::SendTeleportMove()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( MSGCMD_TELEPORT_MEMO ) )
		return;

	if( m_sbSelection == -1 )
		return;

	if( _pNetwork->MyCharacterInfo.zoneNo != m_nZoneNum[m_sbSelection+m_sbScrollBar.GetScrollPos()] )
		return;

	if( pUIManager->IsCSFlagOn( CSF_CANNOT_TELEPORT_MASK ) )
		ShowTeleportError();
	else if( !((CPlayerEntity*)CEntity::GetPlayerEntity(0))->IsIdle() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 789, "�ٸ� �ൿ �߿��� �޸𸮽�ũ���� ����� �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}
	else
	{
		_pNetwork->SendTeleportMove( MSG_MEMPOS, m_sbSelection +m_sbScrollBar.GetScrollPos());
	}
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SetTeleportInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUITeleport::SetTeleportInfo( UBYTE ubSlot, SLONG slZone, FLOAT fX, FLOAT fZ, CTString &strComment )
{
	if (ubSlot > TELEPORT_MAX_MEMO)
		return;

	m_nZoneNum[ubSlot] = slZone;
	m_strZone[ubSlot] = CZoneInfo::getSingleton()->GetZoneName( slZone );
	m_strCoord[ubSlot].PrintF( "%4d,%-4d", (int)fX, (int)fZ );
	m_strMemo[ubSlot] = strComment;
	
	if( m_sbSelection >= 0 )
	{
		m_btnMemorize.SetEnable( TRUE );
		m_btnMove.SetEnable( FALSE );
	}
	else
	{
		m_btnMemorize.SetEnable( FALSE );
		m_btnMove.SetEnable( FALSE );
	}
}
