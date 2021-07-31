
#ifndef _UISIEGE_WARFARE_H
#define _UISIEGE_WARFARE_H

#include <vector>
#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UISpinButton.h>

class CUISiegeWarfare : public CUIWindow
{
// Control
	// Ÿ��Ʋ 
	CTString				m_strTitle;
	UIRect					m_rcTitle;

	// ���� �ð� ������ ���
	CUIButton				m_btnOK;				
	CUIButton				m_btnClose;
	CUIButton				m_btnCancel;
	CUICheckButton			m_cbtnSaturday;				// ���� �� ����
	CUICheckButton			m_cbtnSunday;
	CUISpinButton			m_sbtnSWTime;				// ���� �ð� ����
	CUIListBox				m_lbDesc;					// ���� ���� �ð�
		
	// Back UV
	UIRectUV3				m_rt3BackT;								
	UIRectUV3				m_rt3BackSM;							
	UIRectUV3				m_rt3BackNM;
	UIRectUV3				m_rt3BackM;	
	UIRectUV3				m_rt3BackL;	

	// BattleBack UV
	UIRectUV				m_rtBattleBack;
	UIRectUV				m_rtSelectBattleBack;

	// Number UV
	UIRectUV				m_rtNumber[11];
	UIRectUV				m_rtColon;
	
	// Notice 
	UIRect					m_rcNotice1;				// Region of notice1 (�� ��)
	UIRect					m_rcNotice2;				// Region of notice2 (�Ʒ� ��)
	
	UIRectUV				m_rtNoticeL;				// UV of notice left region
	UIRectUV				m_rtNoticeC;				// UV of notice center region
	UIRectUV				m_rtNoticeR;				// UV of notice right region
	
	//Notice Info
	TIME					m_tmNoticeTime;				// ���� ���� ǥ�� �ð�
	BOOL					m_bShowNotice;				// ���� ������ �����ְ� �ִ� ��
	CTString				m_strNoticeMessage;			// ���� ���� 1 ��Ʈ�� (��)
	CTString				m_strNoticeMessage2;		// ���� ���� 2 ��Ʈ�� (�Ʒ�)

	CUISpinButton			m_sbtnSWZone;				// [2010/11/01 : Sora] ���� ����
public:	
	CUISiegeWarfare();
	virtual ~CUISiegeWarfare();
	
	void	Clear();
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );
	
	// Adjust position
	void	ResetPos( BOOL bCenter = TRUE );
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Message
	WMSG_RESULT MouseMessage( MSG *pMsg );

	// Open SiegeWarfare
	void	OpenSiegeWarfare();
	void	OpenSWReq();
	void	OpenMercearReq();
	void	OpenSetSWTime();
	void	OpenSWInfo();
	void	OpenSelectBattle();
	void	OpenBattle();
	
	// Close
	void	CloseSiegeWarfare();	
	
	// Render
	void	RenderNotice();
	void	RenderSetSWTime();
	void	RenderBattle();
	void	RenderPoint( int nX, int nY, int nPoint );
	void	DrawNumber( int nX, int nY, int nNumber, int nWidth = 16 , int nHeight = 19 );
	void	RenderLeftTime();
	
	// MessageBox
	void	CloseAllMsgBox();
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	ErrorMessage( CTString strErrorMessage );					// ���� ����Ʈ �ڽ�
	void	Message( int nCommandCode, CTString strMessage, DWORD dwStyle );	// �޼��� �ڽ�
	
	// Etc
	void	SetNotice( CTString strNoticeMessage, CTString strNoticeMessage2 = " " );
	BOOL	m_bBottomNotice;

	void	SetDayOfWeek( int eDayOfWeek );

	void	OpenCostumeReq();

	// [2010/11/01 : Sora] ���� ����
	struct stSiegeWarfareTimeInfo
	{
		UBYTE zone;
		UBYTE wday;
		UBYTE hour;

		stSiegeWarfareTimeInfo()
		{
			Init();
		}

		void Init()
		{
			zone = 0;
			wday = 0;
			hour = 0;
		}
	};

	std::vector<stSiegeWarfareTimeInfo> m_vecSWInfo;
	
	void	InitSiegeWarfareTimeInfo() { m_vecSWInfo.clear(); }
	void	SetSiegeWarfareTimeInfo( stSiegeWarfareTimeInfo swInfo );
	void	OpenSWInfoRenew();
};

// [2010/11/01 : Sora] ���� ����
class CUISiegeWarfareInfo : public CUIWindow
{
protected:
	UIRect			m_rcTitle;
	CUIDrawBox		m_bxBackground[4];
	CUIButton		m_btnOK;
	
	CUIRectString	m_rsMyGuildName;
	CUIRectString	m_rsMyGuildBossName;
public:
	struct stSiegeWarfareDefGuildInfo // ������� ����
	{
		char zone;
		char month;
		char day;
		char hour;
		CTString guildName;

		stSiegeWarfareDefGuildInfo()
		{
			Init();
		}

		void Init()
		{
			zone = 0;
			month = 0;
			day = 0;
			hour = 0;
			guildName = "";
		}
	};

	struct stMySiegeWarfareInfo	// �� ���� ����
	{
		std::vector<char> vecZone;
		std::vector<char> vecJoinFlag;
		CTString guildName;
		CTString guildBossName;

		stMySiegeWarfareInfo()
		{
			Init();
		}

		void Init()
		{
			vecZone.clear();
			vecJoinFlag.clear();
			guildName = "";
			guildBossName = "";
		}
	};

	stMySiegeWarfareInfo m_MySiegeWarfareInfo;
	std::vector<stSiegeWarfareDefGuildInfo> m_vecDefGuildInfo;

	CUISiegeWarfareInfo();
	~CUISiegeWarfareInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void	Render();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	WMSG_RESULT MouseMessage( MSG *pMsg );

	void	InitSiegeWarfareInfo();
	void	SetSiegeWarfareDefGuildInfo( stSiegeWarfareDefGuildInfo defGuildInfo );
	void	SetMySiegeWarfareGuildInfo( CTString guildName, CTString guildBossName );
	void	SetMySiegeWarfareJoinInfo(  char zone, char joinFlag );
	void	ToggleSiegeWarfareInfo( BOOL bShown );
	CTString	GetJoinStatusString( char joinFlag );
};


#endif // _UISIEGE_WARFARE_H
