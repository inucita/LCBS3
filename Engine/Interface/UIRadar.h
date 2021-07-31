// ----------------------------------------------------------------------------
//  File : UIRadar.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIRADAR_H_
#define	UIRADAR_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif


// #include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UICheckButton.h>


// Radar option
enum RadarOption
{
	RADAR_PARTY			= 0,
	RADAR_GUILD,
	RADAR_NPC,
	RADER_QUESTNPC,
	RADER_QUESTSUCCESS,
	RADAR_MOB,
	RADAR_EXPEDITION,	// [sora] ������� ǥ��
	RADAR_OPTION_TOTAL,
};

#define	RADAR_WIDTH						140
#define	RADAR_HEIGHT					53

class CUIImageArray;
// ----------------------------------------------------------------------------
// Name : CUIRadar
// Desc :
// ----------------------------------------------------------------------------
class CUIMapOption;
class CUIRadar : public CUIWindow
{
	typedef struct _SSignal
	{
		FLOAT	fX, fZ;
		DOUBLE	dStartTime;
		BOOL	bVisible;
	} SSignal;

	class CIconName
	{
		public:
			UIRect rc;
			CTString name;
		
			void SetIconName( int left, int top, int right, int bottom, CTString name )
			{
				rc.SetRect(left, top, right, bottom);
				this->name = name;
			}

			void Clear()
			{
				rc.SetRect(0,0,0,0);
				name = "";
			}

			BOOL IsEmpty()
			{
				return name.IsEmpty();
			}
	};

public:
	CUIRadar();
	~CUIRadar();

	void	initialize();

	void	OnRender(CDrawPort* pDraw);
	void	OnPostRender(CDrawPort* pDraw);

	void	FocusLeave();

	// Messages
	WMSG_RESULT KeyMessage( MSG *pMsg );

	// Message Box event catcher.
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// ���� Signal 
	void SetSignal( float fX, float fZ );
	void SendSignal( float fX, float fZ );
	bool RadarToWorld( float& fX, float& fZ );
	bool IsPossibleSignal();
	bool IsPossibleCastleRadar();
	void RenderSignal();
	void SetSignalOn( bool bOn );

	// Set Time 
	inline void SetTime(int year=0,int month =0 ,int day = 0,int hour = 0,LONG startTime=0) { m_year = year, m_month =month,m_day =day, m_hour = hour, m_startTime = startTime; 	}
	inline time_t GetStartTime() { return m_startTime;}
	// Get cipher
	int CountCipher( int tv_time, int tv_cipher = 1 );
	void DisplayNum( int tv_time,UIRect tv_rect);

	void	RoyalRumbleJoinReqMsgBox();
	void	SetRoyalRumbleStatus( const BOOL bEnable );	
	void	SetRoyalRumbleSandglass( const UBYTE sandglassStat, const INDEX remainTime = 0 );
	void	ResetRoyalRumbleStat();
	void	ShowRoyalRumbleTrophy();
	ENGINE_API void	OpenRoyalrumbleMgrMenu( const INDEX iMobIndex );

	const int		GetHour() const	{ return m_hour; } 
	void	AddIconName(int left, int top, int right, int bottom, CTString name);		// [2012/10/11 : Sora] ����� ����

	void	OnExpressNotice();
	void	OffExpressNotice();

	// PremiumChar
	void	SetPremiumCharBenefit(bool bPremiumChar);
		
	CUIMapOption* GetMapOption() { return m_pMapOption;	}

	void	OnUpdate(float fDeltaTime, ULONG ElapsedTime);

	void	updateZone();
	void	updateChannelInfo();

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

	void	OnUpdatePositionPost();

protected:

	// Internal functinos
	void	RenderObjectLocation();
	void	RenderIconTooltip(CDrawPort* pDrawPort);

	// royal rumble showing functions [5/18/2011 rumist]
	void	_initTrophyData();
	void	_createTrophyData();
	void	_renderTrophy();
	void	_destroyTrophy();
	void	_showWinnerMsgBox();

	void	RenderCurrentMap();		// [2012/10/11 : Sora] ����� ����	
	BOOL	IsRadarUse();			// [2012/10/11 : Sora] ����� ����
	void	RenderMyPosition();		// [2012/10/11 : Sora] ����� ����
	void	RenderGPS(CDrawPort* pDraw);

	void	pressExpress();
	void	pressPetStash();
	void	pressItemCollect();
	void	pressPremium();
	void	pressDurability();

	void	pressZoomPlus();
	void	pressZoomMinus();
	void	pressSignal();
	void	pressOption();
	void	pressMap();

	void	updateServerTime();
	void	setBtnLCETooltip();
	void	updateSandGrassTime();
	void	setBtnPremiumTooltip();
	void	setBtnDurabilityTooltip();

	// Time 
	int m_year;
	int m_month;
	int m_day;
	int m_hour;
	LONG m_startTime;
//	int m_cipher;

	int	m_nZone;											// [2012/10/11 : Sora] ����� ����
	int	m_nLayer;											// [2012/10/11 : Sora] ����� ����
	FLOAT m_fZoomRate;										// [2012/10/11 : Sora] ����� ����
	int	m_nSelectedIcon;									// [2012/10/11 : Sora] ����� ����
	std::vector<CIconName>	m_iconVec;						// [2012/10/11 : Sora] ����� ����
	UIRectUV				m_rtSubZone[9];					// [2012/10/11 : Sora] ����� ����
	UIRect					m_rcSubZone[9];					// [2012/10/11 : Sora] ����� ����


	// Signal 
	SSignal	m_Signal;

	// Controls
	CUICheckButton		m_cbtnOption[RADAR_OPTION_TOTAL];	// Option check buttons
	BOOL				m_bShowOptionPopup;					// If popup of radar option is shown or not
	BOOL				m_bSignalBtnOn;						// �ñ׳� ��ư ��� 
	BOOL				m_bInsideMouse;						// ���콺�� �ش� ������ �ȿ� �ִ��� 
	// String
	char				m_szCoord[9];						// String of coordinate

	// Region of each part
	UIRect				m_rcTop;							// Region of top background
	UIRect				m_rcCompassIn;						// Region of inner compass
	UIRect				m_rcCompassOut;						// Region of outer compass
	UIRect				m_rcPoint[RADAR_OPTION_TOTAL];		// Region of point of objects
	UIRect				m_rcMyPoint;						// Region of my point
	UIRect				m_rcTarget;							// Region of target object
	UIRect				m_rcSignal;							// Region of Signal object
	UIRect				m_rcOutSignal;						// Region of Out Signal ( Direction ) object

	int					m_nOptionIconSX;					// Position x of option icon

	// UV of each part
	UIRectUV			m_rtBackTop;						// UV of top background
	UIRectUV			m_rtCompassIn;						// UV of inner compass
	UIRectUV			m_rtCompassOut;						// UV of outer compass
	UIRectUV			m_rtOptionL;						// UV of left region of option popup
	UIRectUV			m_rtOptionM;						// UV of middle region of option popup
	UIRectUV			m_rtOptionR;						// UV of right region of option popup
	UIRectUV			m_rtPoint[RADAR_OPTION_TOTAL];		// UV of point of objects
	UIRectUV			m_rtMyPoint;						// UV of my point
	UIRectUV			m_rtTarget;							// UV of point of target object
	UIRectUV			m_rtSignal;							// UV of Signal object
	UIRectUV			m_rtOutSignal;						// UV of Out Signal ( Direction ) object
	UIRectUV			m_rtTargetAni;						// UV of animation point of target object
	UIRectUV			m_rtToolTipL;						// UV of left region of tool tip
	UIRectUV			m_rtToolTipM;						// UV of middle region of tool tip
	UIRectUV			m_rtToolTipR;						// UV of right region of tool tip

	//RAID_SYSTEM
	UIRectUV			m_rtPointExpedition[4];				// [sora] ������ ���̴� ǥ�� �̹���

	// time rect
	UIRectUV			m_rtFigure[10];

	// -------------------------------------------------------------------<<
	
	CTextureData		*m_ptdButtonTexture;			// Texture of Button

	CTextureData		*m_ptdRoyalRumbleTexture;		// royal rumble texture [4/25/2011 rumist]

	CTextureData		*m_ptdMapTexture;				// [2012/10/11 : Sora] ����� ����

	CTextureData		*m_ptdMapObjTexture;			// [2012/10/11 : Sora] ����� ����
	__int64				m_liEndTime;					// royal rumble left time at join.
	BOOL				m_bShowLeftTimeTooltip;			// royal rumble left time showing flag;

	// royal rumble winner trophy [5/18/2011 rumist]
	CTextureData*		m_ptdTrophyTexture;			
	BOOL				m_bShowTrophy;
	BOOL				m_bRenderTrophy;
	UIRect				m_rcTrophy;
	UIRectUV			m_rtTrophy;
	UIRectUV			m_rtTrophyEffect;
	TIME				m_tStartTime;
	UBYTE				m_ubTrophyAlpha;
	UBYTE				m_ubTrophyEffectAlpha;

private:
	UIRectUV			m_ExpressNotice;
	DWORD				m_nStartTime;
	bool				m_bNotice;
	bool				m_bNoticeRenderFlag;

	CTString			m_strLocalTime;
	CTString			m_strLocalDay;
	int					m_nLocalTimeY;
	bool				m_bMapOptionOver;
	CUIMapOption*		m_pMapOption;

	UIRect				m_rcGPS;
	UIRectUV			m_uvGPS;

	CTString			m_strPos;
	float				m_fUpdateServerTime;

	CUIButton*			m_pBtnExpress;
	CUIButton*			m_pBtnPremium;
	CUIButton*			m_pBtnDurability;
	CUIText*			m_pTxtPos;

	CUIBase*			m_pRadar;
	CUIBase*			m_pInfo;
	CUIText*			m_pTxtServerTime;
	CUIText*			m_pTxtChInfo;

	CUISpriteAni*		m_pSprAniRR;
	CUIImageArray*		m_pImgArrSandGrass;
};

#endif	// UIRADAR_H_

