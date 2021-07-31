
#ifndef	UIGUILD_BATTLE_H_
#define	UIGUILD_BATTLE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Include Files
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <algorithm>
// #include <map>
// #include <Engine/Effect/CEffectGroupManager.h>
// #include <Engine/Effect/CEffectGroup.h>

// Define size of Guild Battle
#define	GB_WIDTH						311		// size of Guild Battle ��ûâ�� �ʺ�
#define	GB_HEIGHT						220		// size of Guild Battle ��ûâ�� ����

#define GB_MIN_MEMBER					10		// ��� ���� �ּ� �ο�

//------------------------------------------------------------------------------
// CUIGuildBattle
// Explain:  ��� ���� 
// Date : 2005-03-17,Author: Lee Ki-hwan
// History: 2005. 4. 4	: Test Server Open
//------------------------------------------------------------------------------
class ENGINE_API CUIGuildBattle : public CUIWindow
{
protected:	

	// Guild Battle State : ��� ������ ���� 
	enum eGBState
	{
		GBS_NONE,			// �ʱ� ����
		GBS_REQ,			// ��� ���� ��û
		GBS_IN_REQ,			// ��� ���� ��û ��( ��û�� ������ ��ٸ��� ���� )
		GBS_IN_REQ_ACCEPT,	// ��� ���� ���� ��( ����� ���� �� ��� ���� : 5�� ) 		
		GBS_IN_BATTLE,		// ��� ���� ��
		GBS_BATTILE_NOT_ZONE, // ��� ���� ���� �ƴϴ�.
		GBS_END,			// ��� ���� ����
		GBS_WATCHER			// ����� �����ڸ��
	};
	eGBState				m_eGBState;

// ��� ��û
	// Use m_ptdBaseTexture
	// Controls	
	CUIButton				m_btnOK;						// OK button
	CUIButton				m_btnCancel;					// Cancel button
	CUIListBox				m_lbGBReqdDesc;					// ��� ��û�� ���� ����
	CUIEditBox				m_ebGuildReqNas;				// ��� ��û �ݾ� �Է� ( �� ���� ���� )
	CUIEditBox				m_ebGuildReqTime;				// ��� ��û �ð� ( �д��� )
		
	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar
	
	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background		
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background
		
	UIRectUV				m_rtInputBoxL;					// UV of left region of input box
	UIRectUV				m_rtInputBoxM;					// UV of middle region of input box
	UIRectUV				m_rtInputBoxR;					// UV of right region of input box

// ��� ���� ǥ�� 
	CTextureData*			m_ptdStateTexture;
	
	UIRect					m_rcBox;						// �ð� ǥ�� â (���)
	UIRect					m_rcRedBox;						// RedBox : KillCount
	UIRect					m_rcBlueBox;					// BlueBox : KillCount
		
	UIRectUV				m_rtBox;						
	UIRectUV				m_rtBlueBox;					// BlueBox : KillCount
	UIRectUV				m_rtRedBox;						// RedBox : KillCount
	
	UIRectUV				m_rtSmallNumber[10];			// �ð� ǥ�� �� ����
	UIRectUV				m_rtLargeNumber[10];			// KillCount ǥ�� �� ���� 
	UIRectUV				m_rtColon;						// �ð� ǥ�� �� :(�ݷ�)
	
// Notice 
	UIRect					m_rcNotice1;					// Region of notice1 (�� ��)
	UIRect					m_rcNotice2;					// Region of notice2 (�Ʒ� ��)
	
	UIRectUV				m_rtNoticeL;					// UV of notice left region
	UIRectUV				m_rtNoticeC;					// UV of notice center region
	UIRectUV				m_rtNoticeR;					// UV of notice right region
	
// Info 
	//Notice
	TIME					m_tmNoticeTime;					// ���� ���� ǥ�� �ð�
	BOOL					m_bShowNotice;					// ���� ������ �����ְ� �ִ� ��
	CTString				m_strNoticeMessage;				// ���� ���� 1 ��Ʈ�� (��)
	CTString				m_strNoticeMessage2;			// ���� ���� 2 ��Ʈ�� (�Ʒ�)
	
	// �� ��� ���� ( Guild���� ����Ǿ� ������.. �׳� ó���� �����ؼ� ���� )
	CTString				m_strGuildName;					// ��� �̸�
	int						m_nCharIndex;					// ������� ĳ���� Index (����常�� ������ ����)
	int						m_nGuildIndex;					// ��� Index 
	int						m_nKillPoint;					// Kill Point
	
	//��� �� ����� ����
	CTString				m_strTargetGuildName;			// ����� ��� �̸�
	int						m_nTargetCharIndex;				// ����� ������� Index (����常�� ������ ����)
	int						m_nTargetGuildIndex;			// ����� ��� Index 
	int						m_nTargetKillPoint;				// ������� KillPoint
	
	// Battle Status
	LONG					m_lLeftTime;					// ���� ���� �ð� (1�и���, Ȥ�� KillPoint����� ����)
	int						m_nBattleZone;					// ��� ���� ���� 
	int						m_nPrize;						// ���� ����

	// etc
	CTString				m_strTitle;						// Ÿ��Ʋ �̸� (������� ���� �޼��� �Ѹ��� ���)
	TIME					m_tmLeftTime;					// Ÿ�̸� �� �ӽ� ����
	
	std::map<SLONG, CEffectGroup *> m_mapEG;
public:

	CUIGuildBattle();
	virtual ~CUIGuildBattle();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Clear();	// All Data Clear ( EditBox�� ���� : ��.. ��������.. )
	void	Close();	// ����

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Set focus
	void	SetFocus( BOOL bVisible );
	
	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebGuildReqNas.IsFocused();	}
	void	KillFocusEditBox() { m_ebGuildReqNas.SetFocus( FALSE );		}

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Render
	void	Render();					// Render
	void	RenderGBReq();				// ��� ���� ��ûâ 
	void	RenderGBStatus();			// ��� ���� ����â
	void	DrawNumber( int x, int y, int nNumber, bool bLarge = false );	// ����
	void	RenderLeftTime();			// ���� �ð�
	void	DrawColon( int x, int y );	// Colon Render

	void	RenderKillPoint();			// Render Kill Point
	void	RenderKillPointRed();		// ��� �� 
	void	RenderKillPointBlue();		// �츮 ��
		
	// ��� ���� ��û
	void	OpenGBReq( int nTargetCharIndex, CTString strGuildName ); // ��� ��û â
	void	AddGBReqdDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );
			
	// NetWork Message
	void	GBReq();
	void	GBReq( int nGuildIndex, CTString strGuildName, int nPrize, int nTime );
	void	GBReqReject( int nRejectCharIndex );
	void	GBReqAccept( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int Time );
	void	GBStopReq( int nTargetCharIndex, CTString strGuildName );
	void	GBStopReqRev( int nGuildIndex, CTString strGuildName );
	void	GBStopReqReject( int nRejectCharIndex );
	void	GBStopReqAccept();	
	void	GBStart( int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize, int nZone, int nTime );	
	void	GBStatus( int nGuildIndex1, CTString strGuildName1, int nCount1, int nGuildIndex2, CTString strGuildName2, int nCount2, int Time, int nZone );
	void	GBWatchStatus( int nGuildIndex1, CTString strGuildName1, int nCount1, int nGuildIndex2, CTString strGuildName2, int nCount2, int Time, int nZone );
	void	GBEnd( int nWinnerGuildIndex, int nGuildIndex1, CTString strGuildName1, int nGuildIndex2, CTString strGuildName2, int nPrize );
	
	// ���� ó��
	void	GBErrorProcess ( int nErrorCode ); 

	// MessageBox
	void	CloseAllMsgBox();	// ��� �޼��� â ����
	void	GBErrorMessage( int nCommandCode, CTString strErrorMessage );	// ���� ����Ʈ �ڽ�
	void	GBMessage( int nCommandCode, CTString strMessage, DWORD dwStyle ); // �޼��� �ڽ�
		
	BOOL	IsInBattle();
	BOOL	IsGBReq()	{ if( m_eGBState == GBS_REQ )	return TRUE; return FALSE; }
		
	// ���� â �Ѹ���
	void	SetNotice ( CTString strNoticeMessage, CTString m_strNoticeMessage2 );

	// Guild Battle Effect
	void	StartGuildEffect ( SLONG slCharIndex, CEntity* penEntity, BOOL bEnemy = TRUE );
	void	StopGuildEffect ( SLONG slCharIndex );
	void	StartEffect();
	void	StopEffect();
	
	// ������ Ȯ�� ( ���� �� ������ Ȯ�� �մϴ�. )
	BOOL	IsEnemy( unsigned long ulID );
	int		IsEnemyGuild ( unsigned long ulGuildIndex );
	
};

#endif	// UIGB_H_