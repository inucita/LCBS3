#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <map>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Interface/UIGuildBattle.h>
#include <Engine/Ska/Render.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Object/ActorMgr.h>

static int	_iMaxMsgStringChar = 0;

// Define text position
#define	GB_TITLE_TEXT_OFFSETX		25		// Ÿ��Ʋ �� ��Ʈ�� 
#define	GB_TITLE_TEXT_OFFSETY		5

#define GB_TITLE_HEIGHT				26		// Ÿ��Ʋ �� ����
#define START_BOTTOM_BUTTON_Y		(GB_HEIGHT - 29)	// ��ư�� ���� ����

#define	GB_DESC_CHAR_WIDTH			(168	 + 95)	// ���� �� �ʺ�
#define DESC_LIST_BOX_HEIGHT		135		// ���� �� ����

#define GB_REQ_NAS_EDIT_POS_X		80		// ���� �Է� ���� �ڽ� ����
#define GB_REQ_NAS_EDIT_POS_Y		166
#define GB_REQ_NAS_EDIT_WIDTH		60
#define GB_REQ_NAS_EDIT_HEIGHT		16
#define EDITBOX_OFFSETX				4


#define GB_REQ_TIME_EDIT_POS_X		200		// �ð� �Է� ���� �ڽ� ����
#define GB_REQ_TIME_EDIT_POS_Y		166
#define GB_REQ_TIME_EDIT_WIDTH		60
#define GB_REQ_TIME_EDIT_HEIGHT		16
#define EDITBOX_OFFSETX				4


// �ִ� �ּ� ��� ����
#define GB_MAX_NAS					1000000		
#define GB_MIN_NAS					100000
#define GB_COMMISSION				5			// ���� : %

//����� ���� ǥ�� ����
#define GB_MAX_NAS_EX				"1,000,000"			// ����	: ����
#define GB_MIN_NAS_EX				"100,000"

// ���� �ð� ( �д��� )
#define GB_MIN_TIME					10
#define GB_MAX_TIME					300

#define SMALL_NUMBER_WIDTH			16
#define SMALL_NUMBER_HEIGHT			20
#define LARGE_NUMBER_WIDTH			21
#define LARGE_NUMBER_HEIGHT			27

#define LEFT_TIME_X					922
#define LEFT_TIME_Y					259

#define RIGHT_SPACE					12
#define BOX_WIDTH					118
#define BOX_HEIGHT					61

#define BLUE_BOX_Y					302
#define RED_BOX_Y					369

#define GUILD_NAME_Y				308
#define GUILD_NAME2_Y				371

#define BATTLE_ZONE_NAME_Y			LEFT_TIME_Y + SMALL_NUMBER_HEIGHT + 2		

#define BOX_Y						256
#define BASE_BOX_HEIHGT				42

#define RED_KILL_POINT_Y			393
#define BLUE_KILL_POINT_Y			329

#define GB_MIN_MEMBER				10
#define COLON_WIDTH					9
#define GB_LEFT_TIME_HOUR			30
#define GB_LEFT_TIME				( 30 * 60 )
#define GB_START_MINUTE				1
	
// Macro Func �� ���� ������ ���� �͵�...
#define SYSERROR_MSG(X)	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( CTString (X), SYSMSG_ERROR );

extern BOOL g_bNasTrans; //����� ���� ǥ�� ����
//------------------------------------------------------------------------------
// CUIGuildBattle::CloseAllMsgBox
// Explain:  
// Date : 2005-03-22(���� 11:29:42) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::CloseAllMsgBox()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_ERROR);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_REQ);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_ACCEPT);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_STOP_REQ);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_STOP_ACCEPT);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_STOP_REQ_REP);
	pUIManager->CloseMessageBox(MSGCMD_GUILD_BATTLE_MESSAGE);
}


//------------------------------------------------------------------------------
// CUIGuildBattle::SetNotice
// Explain:  
// Date : 2005-03-21(���� 9:53:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::SetNotice ( CTString strNoticeMessage, CTString strNoticeMessage2 )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	CloseAllMsgBox();

	pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
	m_bShowNotice = TRUE;
	
	m_strNoticeMessage = strNoticeMessage;
	m_strNoticeMessage2 = strNoticeMessage2;

	// ���ʿ� ��� �� ����
	int nWidth = ( m_strNoticeMessage.Length() ) * 
			 ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;
		
	m_rcNotice1.Left = nCX - nWidth / 2;
	m_rcNotice1.Right = m_rcNotice1.Left + nWidth;


	// �Ʒ��ʿ� ��µ� ����
	nWidth = ( strNoticeMessage2.Length() ) *
			 ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;
		
	m_rcNotice2.Left = nCX - nWidth / 2;
	m_rcNotice2.Right = m_rcNotice2.Left + nWidth;

	// �ð� ����
	m_tmNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}



//------------------------------------------------------------------------------
// CUIGuildBattle::CUIGuildBattle()
// Explain:  
// Date : 2005-03-17(���� 11:24:53) Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildBattle::CUIGuildBattle()
{
//	Clear();
	m_ptdStateTexture = NULL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::~CUIGuildBattle()
// Explain:  
// Date : 2005-03-17(���� 11:24:51) Lee Ki-hwan
//------------------------------------------------------------------------------
CUIGuildBattle::~CUIGuildBattle()
{
	if( m_ptdStateTexture )
	{
		_pTextureStock->Release( m_ptdStateTexture );
		m_ptdStateTexture = NULL;
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::Clear
// Explain:  
// Date : 2005-03-17(���� 4:33:11) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Clear()
{
	// Controls	
	//m_ebGuildReqNas.ResetString();				// ��� ��û �ݾ� �Է�
	m_lbGBReqdDesc.ResetAllStrings();

	m_tmNoticeTime	= 0;	
	m_bShowNotice	= FALSE;
	m_strNoticeMessage.Clear();
	m_strNoticeMessage2.Clear();
		
	//��� �� ����� ����
	m_strTargetGuildName.Clear();
	m_nTargetCharIndex = -1;
	m_nTargetGuildIndex = -1;

	m_eGBState = GBS_NONE;
	m_bShowNotice = FALSE;

	m_strGuildName.Clear();
	m_nCharIndex = -1;
	m_nGuildIndex = -1;

	m_lLeftTime = GB_LEFT_TIME;
	
	m_nKillPoint = 0;
	m_nTargetKillPoint = 0;

	m_nBattleZone = -1;
	m_nPrize = 0;

	m_mapEG.clear();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::Create
// Explain:  
// Date : 2005-03-17(���� 11:24:46) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	Clear();
	m_strTitle = _S( 1097, "�������" ); 

	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	_iMaxMsgStringChar = GB_DESC_CHAR_WIDTH / ( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

	// ��� ��û
	// Region of each part
	m_rcTitle.SetRect( 0, 0, 216, 22 );
	
	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );
	
	// Nas Input box
	m_rtInputBoxL.SetUV( 131, 46, 135, 59, fTexWidth, fTexHeight );
	m_rtInputBoxM.SetUV( 137, 46, 139, 59, fTexWidth, fTexHeight );
	m_rtInputBoxR.SetUV( 141, 46, 145, 59, fTexWidth, fTexHeight );

	m_ebGuildReqNas.Create( this, GB_REQ_NAS_EDIT_POS_X+2, GB_REQ_NAS_EDIT_POS_Y, GB_REQ_NAS_EDIT_WIDTH-2, GB_REQ_NAS_EDIT_HEIGHT, 16 );
	m_ebGuildReqNas.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebGuildReqNas.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	m_ebGuildReqTime.Create( this, GB_REQ_TIME_EDIT_POS_X+2, GB_REQ_TIME_EDIT_POS_Y, GB_REQ_TIME_EDIT_WIDTH-2, GB_REQ_TIME_EDIT_HEIGHT, 16 );
	m_ebGuildReqTime.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebGuildReqTime.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );


	// OK button
	m_btnOK.Create( this, _S( 191, "Ȯ��" ), 164, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnOK.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnOK.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnOK.CopyUV( UBS_IDLE, UBS_ON );
	m_btnOK.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button
	m_btnCancel.Create( this, _S( 139, "���" ), 234, START_BOTTOM_BUTTON_Y, 63, 21 );
	m_btnCancel.SetUV( UBS_IDLE, 0, 46, 63, 67, fTexWidth, fTexHeight );
	m_btnCancel.SetUV( UBS_CLICK, 66, 46, 129, 67, fTexWidth, fTexHeight );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box of guild description
	m_lbGBReqdDesc.Create( this, 8, 28, 283, DESC_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, FALSE );
	m_lbGBReqdDesc.CreateScroll( TRUE, 0, 0, 9, DESC_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );

	// Up button
	m_lbGBReqdDesc.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbGBReqdDesc.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbGBReqdDesc.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbGBReqdDesc.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbGBReqdDesc.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbGBReqdDesc.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// Guild State In Battle
	m_ptdStateTexture = _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\GuildBattle.tex" ) );
	
	fTexWidth	= m_ptdStateTexture->GetPixWidth();
	fTexHeight	= m_ptdStateTexture->GetPixHeight();


	m_rcRedBox.SetRect( 0, RED_BOX_Y, 0, RED_BOX_Y + BOX_HEIGHT );		
	m_rcBlueBox.SetRect( 0, BLUE_BOX_Y, 0, BLUE_BOX_Y + BOX_HEIGHT );		
	m_rcBox.SetRect( 0, BOX_Y, 0, BOX_Y + BASE_BOX_HEIHGT );	

	m_rtRedBox.SetUV( 0, 52, 118, 113, fTexWidth, fTexHeight );				// UV of background		
	m_rtBlueBox.SetUV( 119, 52, 237, 113, fTexWidth, fTexHeight );			// UV of background		
	m_rtBox.SetUV( 8, 57, 108, 72, fTexWidth, fTexHeight );					// UV of background		
	
	for( int i = 0 ; i < 10; i++ )
	{
		m_rtSmallNumber[i].SetUV( i*SMALL_NUMBER_WIDTH, 0, 
								i*SMALL_NUMBER_WIDTH+SMALL_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT, fTexWidth, fTexHeight );

		m_rtLargeNumber[i].SetUV( i*LARGE_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT, 
								i*LARGE_NUMBER_WIDTH+LARGE_NUMBER_WIDTH, SMALL_NUMBER_HEIGHT + LARGE_NUMBER_HEIGHT, fTexWidth, fTexHeight );					// UV of background		
	}



	m_rtColon.SetUV ( 164, 0, 164 + COLON_WIDTH, SMALL_NUMBER_HEIGHT, fTexWidth, fTexHeight );		
		
	m_rcNotice1.SetRect( 0, 100, 0, 119 );
	m_rcNotice2.SetRect ( 0, 120, 0, 139 );

	// Notice
	m_rtNoticeL.SetUV( 189, 0, 221, 19, fTexWidth, fTexHeight );
	m_rtNoticeC.SetUV( 243, 22, 250, 41, fTexWidth, fTexHeight );
	m_rtNoticeR.SetUV( 223, 0, 255, 19, fTexWidth, fTexHeight );
	
	

}


//------------------------------------------------------------------------------
// CUIGuildBattle::Close
// Explain:  
// Date : 2005-03-21(���� 5:13:24) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Close()
{

	CloseAllMsgBox();
	CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, FALSE );

	Clear();
}

// Define notice delay time
#define	NOTICE_DELAY		8000			// milliseconds
#define	NOTICE_FADEOUT		7000
#define	NOTICE_FADETIME		( NOTICE_DELAY - NOTICE_FADEOUT )

//------------------------------------------------------------------------------
// CUIGuildBattle::Render
// Explain:  
// Date : 2005-03-17(���� 11:28:56) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::Render()
{
	if ( m_eGBState == GBS_REQ )
	{
		RenderGBReq();
	}
	else if ( m_eGBState == GBS_IN_BATTLE || m_eGBState == GBS_WATCHER)
	{
		RenderGBStatus();
	}

	if( m_bShowNotice == FALSE )
		return;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmNoticeTime;
	if( llCurDelay < CHATMSG_NOTICE_DELAY )
	{
		COLOR	colBackground = 0xFFFFFFFF;
		COLOR	colText = 0x18ff00ff;
		if( llCurDelay > NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( NOTICE_DELAY - llCurDelay ) / (FLOAT)NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;
			
			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}

		CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

		pDrawPort->InitTextureData( m_ptdStateTexture );

		// Add render regions
		// Background
		pDrawPort->AddTexture( m_rcNotice1.Left - 32, m_rcNotice1.Top,
												m_rcNotice1.Left, m_rcNotice1.Bottom,
												m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice1.Left, m_rcNotice1.Top,
												m_rcNotice1.Right, m_rcNotice1.Bottom,
												m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice1.Right, m_rcNotice1.Top,
												m_rcNotice1.Right + 32, m_rcNotice1.Bottom,
												m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
												colBackground );

		// bottom
		pDrawPort->AddTexture( m_rcNotice2.Left - 32, m_rcNotice2.Top,
												m_rcNotice2.Left, m_rcNotice2.Bottom,
												m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice2.Left, m_rcNotice2.Top,
												m_rcNotice2.Right, m_rcNotice2.Bottom,
												m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
												colBackground );

		pDrawPort->AddTexture( m_rcNotice2.Right, m_rcNotice2.Top,
												m_rcNotice2.Right + 32, m_rcNotice2.Bottom,
												m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
												colBackground );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		pDrawPort->PutTextEx( m_strNoticeMessage, m_rcNotice1.Left, m_rcNotice1.Top + 4, colText );
		pDrawPort->PutTextEx( m_strNoticeMessage2, m_rcNotice2.Left, m_rcNotice2.Top + 4, colText );

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		m_bShowNotice = FALSE;

		// ���� ���¿��� ������ ��� ���ٸ� ������� ���� �ȴ�.
		if ( m_eGBState == GBS_END ) 
		{
			Close();
		}
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderGBReq 
// Explain: ��� ��û ȭ��
// Date : 2005-03-17(���� 4:29:46) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderGBReq()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY, nX2, nY2;

	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + GB_TITLE_HEIGHT;


	pDrawPort->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
		
	// Not Manager
	pDrawPort->AddTexture( m_nPosX, nY, nX, nY + DESC_LIST_BOX_HEIGHT+ 2,
											m_rtBackMiddle1.U0, m_rtBackMiddle1.V0,
											m_rtBackMiddle1.U1, m_rtBackMiddle1.V1,
											0xFFFFFFFF );
	
	nY += DESC_LIST_BOX_HEIGHT+ 2;

	// Middle 2
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight - 7,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );


	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	m_lbGBReqdDesc.Render();

	// Edit Box
	// Lower left

	nX	= m_nPosX + GB_REQ_NAS_EDIT_POS_X;
	nX2 = nX + GB_REQ_NAS_EDIT_WIDTH;

	nY	= m_nPosY + GB_REQ_NAS_EDIT_POS_Y;
	nY2 = nY + GB_REQ_NAS_EDIT_HEIGHT;

	pDrawPort->AddTexture( nX, nY, nX + EDITBOX_OFFSETX, nY2,
		m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
		0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + EDITBOX_OFFSETX, nY, nX2 - EDITBOX_OFFSETX, nY2,
		m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
		0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - EDITBOX_OFFSETX, nY, nX2, nY2,
		m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
		0xFFFFFFFF );

	m_ebGuildReqNas.Render();

	nX	= m_nPosX + GB_REQ_TIME_EDIT_POS_X;
	nX2 = nX + GB_REQ_TIME_EDIT_WIDTH;

	nY	= m_nPosY + GB_REQ_TIME_EDIT_POS_Y;
	nY2 = nY + GB_REQ_TIME_EDIT_HEIGHT;

	pDrawPort->AddTexture( nX, nY, nX + EDITBOX_OFFSETX, nY2,
		m_rtInputBoxL.U0, m_rtInputBoxL.V0, m_rtInputBoxL.U1, m_rtInputBoxL.V1,
		0xFFFFFFFF );
	// Lower middle
	pDrawPort->AddTexture( nX + EDITBOX_OFFSETX, nY, nX2 - EDITBOX_OFFSETX, nY2,
		m_rtInputBoxM.U0, m_rtInputBoxM.V0, m_rtInputBoxM.U1, m_rtInputBoxM.V1,
		0xFFFFFFFF );
	// Lower right
	pDrawPort->AddTexture( nX2 - EDITBOX_OFFSETX, nY, nX2, nY2,
		m_rtInputBoxR.U0, m_rtInputBoxR.V0, m_rtInputBoxR.U1, m_rtInputBoxR.V1,
		0xFFFFFFFF );

	m_ebGuildReqTime.Render();
	
	m_btnOK.Render();
	m_btnCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	pDrawPort->PutTextEx( m_strTitle, m_nPosX + GB_TITLE_TEXT_OFFSETX,		
										m_nPosY + GB_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S( 1098, "�� ����" ), m_nPosX + GB_REQ_NAS_EDIT_POS_X + GB_REQ_NAS_EDIT_WIDTH + 4,		
										m_nPosY + GB_REQ_NAS_EDIT_POS_Y + 3, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S ( 1902, "��" ), m_nPosX + GB_REQ_TIME_EDIT_POS_X + GB_REQ_TIME_EDIT_WIDTH + 4,		
										m_nPosY + GB_REQ_TIME_EDIT_POS_Y + 3, 0xFFFFFFFF );  


	// Flush all render text queue
	pDrawPort->EndTextEx();

}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderGBStatus
// Explain:  
// Date : 2005-03-17(���� 4:29:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderGBStatus()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdStateTexture );

	// ���� �ð� ǥ�� ( ImageFont );
	

	int nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH;

	m_rcBox.Left = m_rcBlueBox.Left = m_rcRedBox.Left = nOffsetX;
	m_rcBox.Right = m_rcBlueBox.Right = m_rcRedBox.Right = nOffsetX + BOX_WIDTH;

	// Ren, Blue Box ǥ��
	pDrawPort->AddTexture( m_rcBox.Left, m_rcBox.Top, m_rcBox.Right, m_rcBox.Bottom,
									m_rtBox.U0, m_rtBox.V0,
									m_rtBox.U1, m_rtBox.V1,
									0xFFFFFFFF );

	pDrawPort->AddTexture( m_rcBlueBox.Left, m_rcBlueBox.Top, m_rcBlueBox.Right, m_rcBlueBox.Bottom,
									m_rtBlueBox.U0, m_rtBlueBox.V0,
									m_rtBlueBox.U1, m_rtBlueBox.V1,
									0xFFFFFFFF );
	
	pDrawPort->AddTexture( m_rcRedBox.Left, m_rcRedBox.Top, m_rcRedBox.Right, m_rcRedBox.Bottom,
									m_rtRedBox.U0, m_rtRedBox.V0,
									m_rtRedBox.U1, m_rtRedBox.V1,
									0xFFFFFFFF );

	// ���� �ð� ǥ�� ( Image Font : Small )
	RenderLeftTime();

	// Kill Point ǥ�� ( ImageFont : Large);
	RenderKillPoint();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	CTString strMessage;
	
	strMessage.PrintF ( _S( 1099, "�������� : %s" ), CZoneInfo::getSingleton()->GetZoneName( m_nBattleZone ) ); 	
	
	int nX = m_rcRedBox.Left + ( m_rcRedBox.Right - m_rcRedBox.Left ) / 2;

	// �� ǥ�� (for TEXT)
	pDrawPort->PutTextExCX( strMessage, nX, BATTLE_ZONE_NAME_Y, 0xFFFFFFE5, TRUE, 0x181818FF );

	// ���� ǥ�� (for TEXT)
	pDrawPort->PutTextExCX( m_strGuildName, nX, GUILD_NAME_Y, 0xA6C0FFE5 );
	pDrawPort->PutTextExCX( m_strTargetGuildName, nX, GUILD_NAME2_Y, 0xFF7E6EE5 );

	// Flush all render text queue
	pDrawPort->EndTextEx();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::DrawNumber 
// Explain:  
// Date : 2005-03-22(���� 7:54:56) Lee Ki-hwan
// bool bLarge : ū ����
// bool bRight : ������ ����
//------------------------------------------------------------------------------
void CUIGuildBattle::DrawNumber ( int x, int y, int nNumber, bool bLarge )
{
	UIRectUV rtNumber	= m_rtSmallNumber[nNumber];
	int nWidth			= SMALL_NUMBER_WIDTH;
	int nHeight			= SMALL_NUMBER_HEIGHT;

	if ( bLarge )
	{
		rtNumber	= m_rtLargeNumber[nNumber];
		nWidth		= LARGE_NUMBER_WIDTH;	
		nHeight		= LARGE_NUMBER_HEIGHT;
	}

	CUIManager::getSingleton()->GetDrawPort()->AddTexture( x, y, x + nWidth, y + nHeight,
							rtNumber.U0, rtNumber.V0, rtNumber.U1, rtNumber.V1,
							0xFFFFFFFF );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderLeftTime
// Explain:  
// Date : 2005-03-22(���� 8:44:31) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderLeftTime()
{
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmLeftTime;

	int lLeftTime = m_lLeftTime - (llCurDelay/1000);

	if ( lLeftTime < 0 ) lLeftTime = 0;

	int iSec = lLeftTime % 60;
	lLeftTime /= 60;

	int iMin = lLeftTime % 60;
	int iTime = lLeftTime /= 60;

	int nX = CUIManager::getSingleton()->GetDrawPort()->GetWidth() - RIGHT_SPACE - ( SMALL_NUMBER_WIDTH * 6 + COLON_WIDTH * 2 );
	int nY = LEFT_TIME_Y;

	int t10;
	// �ð� ���
	
	t10 = iTime / 10;
	iTime %= 10;

	DrawNumber ( nX, nY, t10 );			nX += SMALL_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iTime );		nX += SMALL_NUMBER_WIDTH;

	DrawColon( nX, nY );		nX += COLON_WIDTH;

	// �� ��� 
	t10 = iMin / 10;
	iMin %= 10;

	DrawNumber ( nX, nY, t10 );			nX += SMALL_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iMin );		nX += SMALL_NUMBER_WIDTH;

	DrawColon ( nX, nY );		nX += COLON_WIDTH;

	// �� ��� 
	t10 = iSec / 10;
	iSec %= 10;

	DrawNumber ( nX, nY, t10 );			nX += SMALL_NUMBER_WIDTH;
	DrawNumber ( nX, nY, iSec );	

}


//------------------------------------------------------------------------------
// CUIGuildBattle::RenderKillPoint
// Explain:  
// Date : 2005-03-23(���� 3:02:24) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::RenderKillPoint()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	int nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH/2;
	
	int nKillPoint = m_nKillPoint;
	int nTargetKillPoint = m_nTargetKillPoint;

	int nHalfWidth = LARGE_NUMBER_WIDTH / 2 ;

	int i = 0;
	int nRenderNumber = 0;

	// Blue Kill Point
	for( i = 1;  ; i *= 10 )
	{
		if ( nKillPoint / i > 0 )
		{
			nOffsetX -= nHalfWidth;		
		}
		else
		{
			break;
		}
	}

	if ( i != 1 ) 
	{
		i /= 10;
	}
	else 
	{
		nOffsetX -= nHalfWidth;	
	}
	
	while( i )
	{		
		nRenderNumber = nKillPoint / i;
		nKillPoint %= i;

		DrawNumber ( nOffsetX, BLUE_KILL_POINT_Y, nRenderNumber, TRUE);

		printf ( "%d", nRenderNumber );
		nOffsetX += LARGE_NUMBER_WIDTH;	
		i /= 10;
	}

	// Red Kill Point
	nOffsetX = pDrawPort->GetWidth() - RIGHT_SPACE - BOX_WIDTH/2;

	for( i = 1;  ; i *= 10 )
	{
		if ( nTargetKillPoint / i > 0 )
		{
			nOffsetX -= nHalfWidth;		
		}
		else
		{
			break;
		}
	}

	if ( i != 1 ) 
	{
		i /= 10;
	}
	else 
	{
		nOffsetX -= nHalfWidth;	
	}
	
	while( i )
	{		
		nRenderNumber = nTargetKillPoint / i;
		nTargetKillPoint %= i;

		DrawNumber ( nOffsetX, RED_KILL_POINT_Y, nRenderNumber, TRUE);

		printf ( "%d", nRenderNumber );
		nOffsetX += LARGE_NUMBER_WIDTH;	
		i /= 10;
	}

}


//------------------------------------------------------------------------------
// CUIGuildBattle::DrawColon
// Explain:  
// Date : 2005-03-22(���� 9:57:29) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::DrawColon( int x, int y )
{
	CUIManager::getSingleton()->GetDrawPort()->AddTexture( x, y, x + COLON_WIDTH, y + SMALL_NUMBER_HEIGHT,
							m_rtColon.U0, m_rtColon.V0, m_rtColon.U1, m_rtColon.V1,
							0xFFFFFFFF );	
}


//------------------------------------------------------------------------------
// CUIGuildBattle::AddGBReqdDescString
// Explain:  
// Date : 2005-03-17(���� 5:03:57) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::AddGBReqdDescString( CTString &strDesc, COLOR colDesc )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

#if defined (THAI)
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strDesc);
	INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
		for( int iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbGBReqdDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( int iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strDesc,0,iPos))
				break;
		}
		nSplitPos = iPos;

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
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

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

			AddGBReqdDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}

	}
#else		
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character
		int iPos;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbGBReqdDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		int		iPos;
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
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

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

			AddGBReqdDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbGBReqdDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddGBReqdDescString( strTemp, colDesc );
		}
	}
#endif
}


//------------------------------------------------------------------------------
// CUIGuildBattle::OpenGB
// Explain:  
// Date : 2005-03-17(���� 11:39:11) Lee Ki-hwan
//
//------------------------------------------------------------------------------
void CUIGuildBattle::OpenGBReq( int nTargetCharIndex, CTString strGuildName )
{
	if( IsVisible() == TRUE && m_eGBState != GBS_WATCHER)
		return;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( m_eGBState == GBS_IN_REQ_ACCEPT )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1100, "�̹� ��� ������ �����Ǿ����ϴ�."), SYSMSG_ERROR ); 	
		return;
	}
	else if( m_eGBState == GBS_IN_BATTLE )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1101, "�̹� ��� ������ ���۵Ǿ����ϴ�."), SYSMSG_ERROR );  	
		return;
	}
	else if( m_eGBState == GBS_IN_REQ )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1102, "�̹� ��� ���� ��û ���Դϴ�."), SYSMSG_ERROR ); 	
		return;
	}
	
	if (m_eGBState == GBS_WATCHER)
		Close();
	else
		Clear();

	m_ebGuildReqNas.ResetString();
	m_ebGuildReqTime.ResetString();

	// ��带 ��û�� ĳ������ Index�� �����ؼ� ���߿� ������ ��û �޼����� ���� �� ��� �Ѵ�.
	m_nTargetCharIndex = nTargetCharIndex;
	m_strTargetGuildName = strGuildName;
		
	CTString strMessage;

	strMessage.PrintF( _S( 1103, "[%s]��忡�� ��� ������ ��û�մϴ�." ), m_strTargetGuildName ); 	
	AddGBReqdDescString ( strMessage );

	strMessage.PrintF( _S( 1386, "���� ��û�ݰ� �����ð��� �Է��ϼ���." ), m_strTargetGuildName ); 	
	AddGBReqdDescString ( strMessage );
	
	strMessage.PrintF( _S( 1387, "����������� �¸��ϰ� �Ǹ� %d%%�� �����Ḧ ������ ���� ����� ��� ��û���� �����ϴ�." ), GB_COMMISSION ); 	
	AddGBReqdDescString( strMessage );
	strMessage = " ";
	AddGBReqdDescString( strMessage );

	CTString strMinNas, strMaxNas;
	strMinNas = pUIManager->IntegerToCommaString(GB_MIN_NAS);
	strMaxNas = pUIManager->IntegerToCommaString(GB_MAX_NAS);

	strMessage.PrintF( "%s %s ~ %s", _S( 1388, "���� ��û�� :"), strMinNas, strMaxNas);
	AddGBReqdDescString( strMessage );

	strMessage.PrintF( "%s %d ~ %d", _S( 1389, "���� �ð� :" ), GB_MIN_TIME, GB_MAX_TIME );
	AddGBReqdDescString( strMessage );

	m_ebGuildReqNas.SetFocus( TRUE );
	m_ebGuildReqNas.SetString( "10" );	// 10�� ���� �⺻ ����

	m_ebGuildReqTime.SetFocus( FALSE );
	m_ebGuildReqTime.SetString( "30" );	// 30�� �⺻ ����

	pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );

	m_eGBState = GBS_REQ;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::ResetPosition
// Explain:  
// Date : 2005-03-17(���� 11:24:41) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::AdjustPosition
// Explain:  
// Date : 2005-03-17(���� 11:24:34) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}



//------------------------------------------------------------------------------
// CUIGuildBattle::SetFocus
// Explain:  
// Date : 2005-03-17(���� 11:24:29) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );

	//m_ebGuildReqNas.SetFocus( TRUE );
	//m_ebGuildReqTime.SetFocus( FALSE );
	if( !bVisible )
	{
		m_ebGuildReqNas.SetFocus( FALSE );
		m_ebGuildReqTime.SetFocus ( FALSE );
	}

}


//------------------------------------------------------------------------------
// CUIGuildBattle::MouseMessage
// Explain:  
// Date : 2005-03-17(���� 11:24:22) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::MouseMessage( MSG *pMsg )
{
	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

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

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			// ��� ��û ������ 
			if ( IsGBReq() )
			{
				// OK Button
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_ebGuildReqNas.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_ebGuildReqTime.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsGBReq() )
			{
				if( IsInside( nX, nY ) )
				{
					CUIManager* pUIManager = CUIManager::getSingleton();
					nOldX = nX;		nOldY = nY;
				
					// Title bar
					if( IsInsideRect( nX, nY, m_rcTitle ) )
					{
						bTitleBarClick = TRUE;
					}

				
					// OK Button
					if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						GBReq();	// ��� ���� ��û
						return WMSG_SUCCESS;
					}
					else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						return WMSG_SUCCESS;
					}						
					else if( m_ebGuildReqNas.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_ebGuildReqTime.SetFocus ( FALSE );
						pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
						return WMSG_SUCCESS;
					}
					else if( m_ebGuildReqTime.MouseMessage( pMsg ) != WMSG_FAIL )
					{
						m_ebGuildReqNas.SetFocus ( FALSE );
						pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
						return WMSG_SUCCESS;
					}
				
					pUIManager->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
		
			if ( IsGBReq() )
			{
				// OK Button
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_btnCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					Close();
					return WMSG_SUCCESS;
				}
				else if( m_ebGuildReqNas.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebGuildReqTime.SetFocus ( FALSE );
					CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
				else if( m_ebGuildReqTime.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebGuildReqNas.SetFocus ( FALSE );
					CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
					
			}
		}
		break;
	}

	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::KeyMessage
// Explain:  
// Date : 2005-03-17(���� 11:24:17) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::KeyMessage( MSG *pMsg )
{
	if( !IsFocused() )
		return WMSG_FAIL;

	if ( IsGBReq() )
	{
		if ( pMsg->wParam == VK_RETURN )
		{
			GBReq();// ��� ���� ��û
			return WMSG_SUCCESS; 
		}
		if( m_ebGuildReqNas.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
		else if( m_ebGuildReqTime.KeyMessage( pMsg ) != WMSG_FAIL )
		{
			return WMSG_SUCCESS; 
		}
	}
	return WMSG_FAIL;

}


//------------------------------------------------------------------------------
// CUIGuildBattle::IMEMessage
// Explain:  
// Date : 2005-03-17(���� 11:24:16) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::IMEMessage( MSG *pMsg )
{
	if ( IsGBReq() )
	{
		WMSG_RESULT wmsgResult;

		if( ( wmsgResult = m_ebGuildReqNas.IMEMessage( pMsg ) ) != WMSG_FAIL )
		{
			return wmsgResult;
		}
		else 
		{
			return m_ebGuildReqTime.IMEMessage( pMsg );
		}
	}
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::CharMessage
// Explain:  
// Date : 2005-03-17(���� 11:24:14) Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIGuildBattle::CharMessage( MSG *pMsg )
{
	if ( IsGBReq() )
	{
		if( m_ebGuildReqNas.CharMessage( pMsg ) != WMSG_FAIL )
		{	
			return WMSG_SUCCESS;
		}
		else if( m_ebGuildReqTime.CharMessage( pMsg ) != WMSG_FAIL )
		{	
			return WMSG_SUCCESS;
		}
	}
	return WMSG_FAIL;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::MsgBoxCommand
// Explain:  
// Date : 2005-03-17(���� 11:24:26) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{

	switch( nCommandCode )
	{
	
	case MSGCMD_GUILD_BATTLE_STOP_REQ:
		{
			if( !bOK )	return;
			else _pNetwork->GBStopReq( m_nTargetCharIndex );
		}
		break;
	case MSGCMD_GUILD_BATTLE_STOP_REQ_REP:
		{
			if ( bOK )	// ��� �ߴ� ��û ���� 
				_pNetwork->GBStopReqAccept();
			else		// ��� �ߴ� ��û ����
				_pNetwork->GBStopReqReject();						
		}
		break;
	case MSGCMD_GUILD_BATTLE_REQ:
		{
			// ��� ��û ���
			if( !bOK )	return;
			else _pNetwork->GBReqReject();
		}
		break;
	case MSGCMD_GUILD_BATTLE_ACCEPT:
		{
			if ( bOK )	// ��� ��û ���� 
				_pNetwork->GBReqAccept();
			else		// ��� ��û ����
				_pNetwork->GBReqReject();			
		}
		break;
	case MSGCMD_GUILD_BATTLE_ERROR:	// ���� �߻�
		{
			if( !bOK )	return;
			else Close();			
		}
		break;
	case MSGCMD_GUILD_BATTLE_MESSAGE: //�Ϲ� �˸� �޼���
		{
			if( !bOK )	return;
		}
		break;
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::MsgBoxLCommand
// Explain:  
// Date : 2005-03-17(���� 11:24:24) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::MsgBoxLCommand( int nCommandCode, int nResult )
{
	switch( nCommandCode )
	{
	case MSGCMD_GUILD_BATTLE_ERROR:
		{

		}
		break;
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReq
// Explain:  
// Date : 2005-03-18(���� 4:48:09) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReq()
{
	CloseAllMsgBox();
	CTString strMessage;
	
	// ���� üũ 
	// 1. ������ ���� ���� �ִ°� 
	int nGuildReqNas = atoi( m_ebGuildReqNas.GetString() );
	if ((nGuildReqNas * 10000) > GB_MAX_NAS || (nGuildReqNas * 10000) < GB_MIN_NAS)
	{
		CUIManager* pUIMgr = UIMGR();

		CTString strMinNas, strMaxNas;
		strMinNas = pUIMgr->IntegerToCommaString(GB_MIN_NAS);
		strMaxNas = pUIMgr->IntegerToCommaString(GB_MAX_NAS);

		strMessage.PrintF( _S( 1106, "%s���� %s ������ �ݾ��� �Է��Ͽ� �ֽʽÿ�." ), strMinNas, strMaxNas );	
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, strMessage );
		m_ebGuildReqNas.ResetString();
		return;
	}

	// 1.5 �ð��� ���� ���� �ִ°� 
	int nGuildReqTime = atoi ( m_ebGuildReqTime.GetString() );
	if( nGuildReqTime > GB_MAX_TIME || nGuildReqTime < GB_MIN_TIME )
	{
		strMessage.PrintF( _S ( 1903, "%d�п��� %d�� ������ �ð��� �Է��Ͽ� �ֽʽÿ�." ), GB_MIN_TIME, GB_MAX_TIME );	

		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, strMessage );
		m_ebGuildReqTime.ResetString();
		return;
	}
	
	nGuildReqTime *= 60; // �ʴ����� ��ȯ

	// 2. �������� ����Ѱ�?
	if( _pNetwork->MyCharacterInfo.money/10000 < nGuildReqNas )
	{
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1107, "���� ������ �����Ͽ� ������û�� �� �� �����ϴ�." ) ); 	
		m_ebGuildReqNas.ResetString();
		return;
	}
	
	_pNetwork->GBReq ( m_nTargetCharIndex, nGuildReqNas, nGuildReqTime );
		
	GBMessage ( MSGCMD_GUILD_BATTLE_REQ, _S( 1108, "��� ������ ��û ���Դϴ�.\n����Ͻðڽ��ϱ�?" ), UMBS_OK ); 	
	
	m_eGBState = GBS_IN_REQ;	
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReg
// Explain: ��� ���� ��û ���� 
// Date : 2005-03-18(���� 2:26:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReq( int nGuildIndex, CTString strGuildName, int nPrize, int nTime )
{
	CloseAllMsgBox();
	CTString strMessage;
	if (g_bNasTrans)//����� ���� ǥ�� ������
	{
		CTString strTempNas;
		strTempNas.PrintF("%d0000",nPrize);

		CUIManager::getSingleton()->InsertCommaToString( strTempNas ); 
		strMessage.PrintF( _S( 1904, "[%s]��尡 ��� ������ ��û�Ͽ����ϴ�. ���� ��û���� %s ���� ���� �ð��� %d���Դϴ�.\n���� �Ͻðڽ��ϱ�?" ), 
			strGuildName.str_String, strTempNas,(nTime/60) ); 	
	}else
	{
		strMessage.PrintF( _S( 1904, "[%s]��尡 ��� ������ ��û�Ͽ����ϴ�. ���� ��û���� %d������ ���� �ð��� %d���Դϴ�.\n���� �Ͻðڽ��ϱ�?" ), strGuildName.str_String, nPrize, (nTime/60) ); 	
	}
	GBMessage ( MSGCMD_GUILD_BATTLE_ACCEPT, strMessage, UMBS_YESNO );

}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReqReject
// Explain:  ��� ���� ��û�� �������� 
// Date : 2005-03-18(���� 2:53:16) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReqReject( int nRejectCharIndex )
{
	CloseAllMsgBox();
		
	if( nRejectCharIndex == _pNetwork->MyCharacterInfo.index ) // ��� �� ĳ���Ͱ� �ڽ��̸�
	{
		SYSERROR_MSG( _S( 1110, "��� ���� ��û�� �ߴܵǾ����ϴ�." ) ); 	
	}
	else // ��� �� ĳ���Ͱ� ��� ���̶��
	{
		SYSERROR_MSG( _S( 1110, "��� ���� ��û�� �ߴܵǾ����ϴ�." ) );	
	}

	Close();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBReqAccept
// Explain:  ��� ���� ��û�� ���� ���� ALL
// Date : 2005-03-18(���� 2:53:22) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBReqAccept( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int nTime )
{
	CloseAllMsgBox();

	// DATA Settting....
	// ������ ����
	// �����
	if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex1 )
	{
		m_nGuildIndex			= nGuildIndex1;
		m_strGuildName			= strGuildName1; 
		m_nTargetGuildIndex		= nGuildIndex2;
		m_strTargetGuildName	= strGuildName2;
	}
	else
	{
		m_nGuildIndex			= nGuildIndex2;
		m_strGuildName			= strGuildName2; 
		m_nTargetGuildIndex		= nGuildIndex1;
		m_strTargetGuildName	= strGuildName1;
	}
	
	m_nBattleZone			= nZone;
	m_nPrize				= nPrize;
	
	m_lLeftTime				= nTime;

	//����� ��ǥ �޼��� ���	
	CTString strMessage, strMessage2;
	strMessage.PrintF( _S( 1111, "[%s]���� [%s]��尣�� ��� ������ ���� �Ǿ����ϴ�." ), strGuildName1, strGuildName2 ); 	
	strMessage2.PrintF( _S( 1112, "%d�� �ĺ��� %d�� ���� �� ���� ���� ���°� �˴ϴ�." ), GB_START_MINUTE, (m_lLeftTime/60) ); 	
	
	SetNotice ( strMessage, strMessage2 );

	m_eGBState = GBS_IN_REQ_ACCEPT;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStart
// Explain:  ��� ���� ����
// Date : 2005-03-18(���� 2:53:39) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStart( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int nTime )
{
	// ������ ����
	if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex1 )
	{
		m_nGuildIndex			= nGuildIndex1;
		m_strGuildName			= strGuildName1; 
		m_nTargetGuildIndex		= nGuildIndex2;
		m_strTargetGuildName	= strGuildName2;
	}
	else
	{
		m_nGuildIndex			= nGuildIndex2;
		m_strGuildName			= strGuildName2; 
		m_nTargetGuildIndex		= nGuildIndex1;
		m_strTargetGuildName	= strGuildName1;
	}
	
	m_nBattleZone			= nZone;
	m_nPrize				= nPrize;

	m_lLeftTime				= nTime;
		
	// ����� ���� �޼��� ��� // Dealy Time �� ���� �ֿ��� �Ѵ�!!!
	CTString strMessage, strMessage2;
	strMessage.PrintF( _S( 1116, "[%s]���� [%s]��尣�� ��� ������ ���� �Ǿ����ϴ�." ), strGuildName1, strGuildName2 );
	if (g_bNasTrans)//����� ���� ǥ�� ����
	{
		CTString strTempNas;
		strTempNas.PrintF("%d0000",nPrize);

		CUIManager::getSingleton()->InsertCommaToString( strTempNas );
		strMessage2.PrintF( _S( 1117, "�� ����� ���� ��û�� �� ���� %s ���� �Դϴ�." ), strTempNas );
	}else
	{
		strMessage2.PrintF( _S( 1117, "�� ����� ���� ��û�� �� ���� %d�� ���� �Դϴ�." ), nPrize );
	}

	SetNotice ( strMessage, strMessage2 );

	//!! Start Effect
	StartEffect();

	m_eGBState = GBS_IN_BATTLE;

	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();
}


//------------------------------------------------------------------------------
// CUIGuildbattle::GBStatue
// Explain:  ��� ���� ���� ǥ�� 
// Date : 2005-03-18(���� 4:31:27) Lee Ki-hwan
// - 1�и��� �ѹ��� ����, ������ ����� ���� 
// - int Time; // �ʴ��� (�����ð�)
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStatus( int nGuildIndex1, CTString strGuildName1, int nCount1, int nGuildIndex2, CTString strGuildName2, int nCount2, int Time, int nZone )
{
	if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex1 )
	{
		m_nGuildIndex			= nGuildIndex1;
		m_strGuildName			= strGuildName1; 
		m_nKillPoint			= nCount1;
		m_nTargetGuildIndex		= nGuildIndex2;
		m_strTargetGuildName	= strGuildName2;
		m_nTargetKillPoint		= nCount2;
	}
	else if ( _pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex2 )
	{
		m_nGuildIndex			= nGuildIndex2;
		m_strGuildName			= strGuildName2; 
		m_nKillPoint			= nCount2;
		m_nTargetGuildIndex		= nGuildIndex1;
		m_strTargetGuildName	= strGuildName1;
		m_nTargetKillPoint		= nCount1;
	}
	else
	{
		GBWatchStatus(nGuildIndex1, strGuildName1, nCount1, nGuildIndex2, strGuildName2, nCount2, Time, nZone);
		return;
	}
	
	m_nBattleZone			= nZone;
	m_lLeftTime				= Time;

	// ������ �ٽ� �������� �� ó��
	if (g_slZone == m_nBattleZone )
	{
		if (m_eGBState != GBS_IN_BATTLE)
		{
			m_eGBState = GBS_IN_BATTLE; 	
			StartEffect();
		}		

		if( IsVisible() == FALSE )
		{
			SetEnable( TRUE );
			SetVisible ( TRUE );
			CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
		}
	}
	else
	{ // �� �̵� �� ó��
		m_eGBState = GBS_BATTILE_NOT_ZONE;
		StopGuildEffect(_pNetwork->MyCharacterInfo.index);
	}
	
	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

void CUIGuildBattle::GBWatchStatus( int nGuildIndex1, CTString strGuildName1, int nCount1, int nGuildIndex2, CTString strGuildName2, int nCount2, int Time, int nZone )
{
	if (m_eGBState == GBS_IN_BATTLE || m_eGBState == GBS_REQ || m_eGBState == GBS_IN_REQ_ACCEPT || m_eGBState == GBS_IN_REQ)
		return;

	if (g_slZone != nZone)
		return;

	m_nGuildIndex			= nGuildIndex1;
	m_strGuildName			= strGuildName1; 
	m_nKillPoint			= nCount1;
	m_nTargetGuildIndex		= nGuildIndex2;
	m_strTargetGuildName	= strGuildName2;
	m_nTargetKillPoint		= nCount2;

	m_nBattleZone			= nZone;
	m_lLeftTime				= Time;
	m_eGBState				= GBS_WATCHER;

	if( IsVisible() == FALSE )
	{
		SetEnable( TRUE );
		SetVisible ( TRUE );
		CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, TRUE );
	}

	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReq
// Explain: ��� ���� �ߴ� ��û
// Date : 2005-03-21(���� 7:58:07) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReq( int nTargetCharIndex, CTString strGuildName )
{
	CloseAllMsgBox();

	m_nTargetCharIndex = nTargetCharIndex;
	
	CTString strMessage;
	strMessage.PrintF( _S( 1118, "[%s]��忡�� ��� �����ߴ� ��û�� �Ͻðڽ��ϱ�?" ), strGuildName ); 	

	GBMessage ( MSGCMD_GUILD_BATTLE_STOP_REQ, strMessage, UMBS_YESNO );	

}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReq
// Explain: ��� ���� �ߴ� ��û�� ����
// Date : 2005-03-18(���� 2:53:27) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReqRev( int nGuildIndex, CTString strGuildName )
{
	CloseAllMsgBox();

	CTString strMessage;
	strMessage.PrintF( _S( 1119, "[%s]��尡 ��� �����ߴ� ��û�� �Ͽ����ϴ�. �����Ͻðڽ��ϱ�?" ), strGuildName ); 	
	
	GBMessage ( MSGCMD_GUILD_BATTLE_STOP_REQ_REP, strMessage, UMBS_YESNO );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReqReject
// Explain:  ��� ���� �ߴ� ��û�� ���� ����
// Date : 2005-03-18(���� 2:53:33) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReqReject( int nRejectCharIndex )
{
	CloseAllMsgBox();
	
	if( nRejectCharIndex == _pNetwork->MyCharacterInfo.index ) // �����ڰ� �ڱ� �ڽ�
	{
		SYSERROR_MSG( _S( 1120, "��� ���� �ߴ� ��û�� ��ҵǾ����ϴ�." ) ); 	
	}
	else  // ��� ���� ����
	{
		SYSERROR_MSG( _S( 1120, "��� ���� �ߴ� ��û�� ��ҵǾ����ϴ�." ) ); 	
	}
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBStopReqAccept
// Explain: ��� ���� �ߴ� ��û�� ���� ����
// Date : 2005-03-18(���� 3:27:01) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBStopReqAccept()
{
	CloseAllMsgBox();

	GBMessage ( MSGCMD_GUILD_BATTLE_MESSAGE, _S ( 1121, "��� ������ �ߴ� �Ǿ����ϴ�." ), UMBS_OK );	
}

//------------------------------------------------------------------------------
// CUIGuildBattle::GBEnd
// Explain: ��� ���� ����
// Date : 2005-03-18(���� 2:53:46) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBEnd( int nWinnerGuildIndex, int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize )
{
	CloseAllMsgBox();
	StopEffect();
	
	m_eGBState = GBS_END;

	CTString strMessage, strMessage2;
	CTString strWinnerGuildName;

	if ( nWinnerGuildIndex == -1 )
	{
		strMessage.PrintF( _S( 1122, "[%s]���� [%s]��尣�� ��������� ���ºΰ� �Ǿ����ϴ�." ), strGuildName1, strGuildName2 );

		CTString strTempNas;
		strTempNas.PrintF("%d",nPrize);
		CUIManager::getSingleton()->InsertCommaToString( strTempNas );
		strMessage2.PrintF( _S( 1123, "���� %d%%�� ������ ��� ���� ��û�� %s������ �� ����忡�� �ǵ��� ���ϴ�." ), GB_COMMISSION, strTempNas ); 	
	}
	else
	{
		strWinnerGuildName = (nGuildIndex1 == nWinnerGuildIndex)?strGuildName1:strGuildName2;
		strMessage.PrintF( _S( 1124, "[%s]���� [%s]��尣�� ����������� [%s]��尡 �¸��Ͽ����ϴ�." ), strGuildName1, strGuildName2, strWinnerGuildName ); 	

		CTString strTempNas;
		strTempNas.PrintF("%d",nPrize);
		CUIManager::getSingleton()->InsertCommaToString( strTempNas );
		strMessage2.PrintF( _S( 1125, "[%s]��忡�� ���� %d%%�� ������ ���� ��û�� %s������ �����մϴ�." ), strWinnerGuildName, GB_COMMISSION, strTempNas ); 	
	}

	SetNotice ( strMessage, strMessage2 );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorMessage
// Explain:  
// Date : 2005-03-19(���� 12:28:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBErrorMessage( int nCommandCode, CTString strErrorMessage )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(nCommandCode);
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, UMBS_OK, UI_GUILD_BATTLE, nCommandCode );
	MsgBoxInfo.AddString( strErrorMessage );	
	pUIManager->CreateMessageBox( MsgBoxInfo );		
}


//------------------------------------------------------------------------------
// CUIGuildBattle::GBMessage
// Explain:  
// Date : 2005-03-21(���� 4:46:32) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBMessage( int nCommandCode, CTString strMessage, DWORD dwStyle )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist( nCommandCode ) )
		return;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( m_strTitle, dwStyle, UI_GUILD_BATTLE, nCommandCode );
	
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}



//------------------------------------------------------------------------------
// CUIGuildBattle::GBErrorProcess
// Explain:  
// Date : Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::GBErrorProcess( int nErrorCode )
{
	CloseAllMsgBox();
	switch ( nErrorCode )
	{
	case MSG_GUILD_ERROR_BATTLE_NOGUILD:			// ����� - ���� ��尡 ����
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1126, "���� ��尡 �����ϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_NOTBOSS:			// ����� - ������� ����
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1127, "������� �������� �ʽ��ϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_MEMBERCOUNT:		// ����� - �ο� �� ����
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1128, "��� �ο��� �����Ͽ� ��� ������ �� �� �����ϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_ZONE:				// ����� - ���� ���� �ƴ�
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1129, "��� ������ ������ ���� �ƴմϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_ALREADY_BATTLE:			// ����� - �̹� ����� ��
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1130, "�̹� ��� ������ ���� ���Դϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_PRIZE:				// ����� - ���ñݾ� ��ü ���� (����� ���� ���ݾ��� �ƴ�)
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S ( 1131, "��û �ݾ��� �ʹ� �۰ų� Ů�ϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_NOT_REQ:			// ����� - ��û���� ����
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1132, "��� ������ ��û�� ���� �����ϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_SHORT_PRIZE:		// ����� - ���ñݾ� ������
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1133, "������ �����Ͽ� ��� ������ ������ �� �����ϴ�." ) ); 	
		break;
	case MSG_GUILD_ERROR_BATTLE_NOT_BATTLE:			// ����� = ����� ���� �ƴ�
		GBErrorMessage( MSGCMD_GUILD_BATTLE_ERROR, _S( 1134, "���� ���� ���� �ƴմϴ�." ) ); 	
		break;
	}
	
	// Date : 2005-04-06(���� 5:08:43), By Lee Ki-hwan
	// ���� �޼��� �Ŀ��� ������ ��Ȳ ����...
	//Close();
	CUIManager::getSingleton()->RearrangeOrder( UI_GUILD_BATTLE, FALSE );
	Clear();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StartGuildEffect
// Explain:  
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StartGuildEffect( SLONG slCharIndex, CEntity* penEntity, BOOL bEnemy )
{
	// �̹� ���� �ϴ��� Ȯ�� �ؼ� �����ϸ� return 
	if( m_mapEG.find( slCharIndex ) != m_mapEG.end() ) return;
		
	CEffectGroup *pEG = StartEffectGroup( bEnemy?"cpp_gb_red":"cpp_gb_blue"
						, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
						,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEG.insert(tmpVal);
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StopGuildEffect
// Explain:  
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StopGuildEffect( SLONG slCharIndex )
{
	// �����Ͱ� Ȱ��ȭ �Ǿ� ���� �ʴٸ� ������..
	if( m_mapEG.find( slCharIndex ) == m_mapEG.end() ) return;

	if( m_mapEG[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEG[slCharIndex] ) )
		m_mapEG[slCharIndex]->Stop(0.0f);

	m_mapEG.erase( slCharIndex );
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StartEffect
// Explain:  
// Date : 2005-03-22(���� 11:16:10) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StartEffect()
{
	// Start Effect My Character
	StartGuildEffect ( _pNetwork->MyCharacterInfo.index, CEntity::GetPlayerEntity(0), FALSE );

	// Start My Guild, Enemy Guild Effect Start
	ACTORMGR()->StartGuildEffect();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::StopEffect
// Explain:  ��� ĳ������ Effect�� ������Ų��.
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIGuildBattle::StopEffect()
{
	// All Character Effect Stop
	std::map<SLONG, CEffectGroup *>::iterator	itEG;

	for ( itEG = m_mapEG.begin(); itEG != m_mapEG.end(); ++itEG )
	{
		if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
			(*itEG).second->Stop(0.0f);
	}

	m_mapEG.clear();
}


//------------------------------------------------------------------------------
// CUIGuildBattle::IsEnemy
// Explain:  ������ Ȯ�� (���ݽ� ���)
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL ENGINE_API CUIGuildBattle::IsEnemy( unsigned long ulID )
{
	if ( !IsInBattle() ) return FALSE;

	ObjectBase* pObject = ACTORMGR()->GetObjectByCIndex(eOBJ_CHARACTER, ulID);

	if (pObject != NULL)
	{
		CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

		// Guild Indexx Check
		if (pTarget->cha_lGuildIndex == m_nTargetGuildIndex)
		{
			return TRUE;
		}		
	}

	return FALSE;
}


//------------------------------------------------------------------------------
// CUIGuildBattle::IsEnemyGuild
// Explain: �� ��� ���� Ȯ�� (Effect ǥ�� �Ҷ� ���)
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
int ENGINE_API CUIGuildBattle::IsEnemyGuild( unsigned long ulGuildIndex )
{
	if( ulGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
	{
		return -1; // �Ʊ�
	}
	else if( ulGuildIndex == m_nTargetGuildIndex )
	{
		return 1; // ���� 
	}
	return 0;
}



//------------------------------------------------------------------------------
// CUIGuildBattle::IsInBattle
// Explain: ���� ������ Ȯ�� 
// Date : 2005-04-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIGuildBattle::IsInBattle()
{ 
	if( m_eGBState == GBS_IN_BATTLE && g_slZone == m_nBattleZone ) 
		return TRUE; 
	
	return FALSE; 
}
