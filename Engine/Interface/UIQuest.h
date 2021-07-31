// ----------------------------------------------------------------------------
//  File : UIQuest.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIQUEST_H_
#define	UIQUEST_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <vector>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIButtonEx.h>


#define	QUEST_SLOT_SX					18
#define	QUEST_SLOT_SY					56
#define	QUEST_SLOT_OFFSETY				37

#define	QUESTLIST_SLOT_ROW				4
#define	QUESTLIST_SLOT_ROW_TOTAL		30

#define	QUEST_NAME_CX					122
#define	QUEST_NAME_SY					58
#define	QUEST_NEED_RX					184
#define	QUEST_CURSP_SX					97
#define	QUEST_CURSP_RX					199
#define	QUEST_CURSP_SY					343
#define	QUEST_TAB_CX					108
#define	QUEST_TAB_SY					34
#define	MAX_QUEST_DESC_CHAR				28

#define	QUEST_TITLE_TEXT_OFFSETX		25
#define	QUEST_TITLE_TEXT_OFFSETY		5

#define	QUEST_TOP_HEIGHT				339
#define	QUEST_BOTTOM_HEIGHT				41

#define	QUEST_WIDTH						216
#define	QUEST_HEIGHT					380

#define	QUESTINFO_CHAR_WIDTH			170
#define	MAX_QUESTINFO_LINE				10

// �������̺�Ʈ
#define WORLDCUP_MAX_COUNTRY 32

enum e24Event
{
	EXCH_FIVE,						// �°� �Ӵ� ��ȯ
	EXCH_FOUR,						// ��Ŀ �Ӵ� ��ȯ 
	OPEN_TREA,						// ���� ���� ����
};

const int ciQuestClassifier			=	1000;

enum eSelectionQuest
{
	QUEST_TALK,
	QUEST_EVENT,
	QUEST_TREASURE_BOX,				// ���� ����.
	QUEST_CHANGEWEAPON,
	QUEST_KILL_BOSS,				// �߷� ����
	QUEST_SAVE_PRINCESS,			// ���� ����
	QUEST_AZAKA_RAVINE,				// ����ī ����
	QUEST_GATE_OF_DIMENSION,		// ������ ��
	QUEST_HARVEST_MOON_DAY_EVENT,	// �߼��̺�Ʈ
	QUEST_HARVEST_MOON_DAY_EVENT1,	// 2006 �߼��̺�Ʈ(���� �����)
	QUEST_HARVEST_MOON_DAY_EVENT2,	// 2006 �߼��̺�Ʈ(�������� ����ǰ�� ��ȯ�ϱ�)
	QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE1,	// 2006 �߼��̺�Ʈ(���� �����)
	QUEST_HARVEST_MOON_DAY_EVENT_UPGRADE2,	// 2006 �߼��̺�Ʈ(�������� �����)
	QUEST_HARVEST_MOON_DAY_EVENT_GIVE_ITEM,	// ����ǰ�� ���޹޴´�.
	QUEST_RENUAL_EVENT,
	QUEST_RENUAL_EVENT_OK,
	QUEST_EXCHANGE_MONEY,			// ���� ���ǻ��� �Ӵ� ��ȯ wooss 051031
	QUEST_OPEN_TREASURE,			// ���� ���ǻ��� �������� ����
	QUEST_CHARACTER_CARD,			// 
	EVENT_NEWYEAR1,					// 2006 �ް� ��� �̺�Ʈ
	EVENT_NEWYEAR2,					// �γ��� ���� �̺�Ʈ 
	EVENT_FIND_FRIEND,
	EVENT_COUPON ,					// �Ϻ� ���� �̺�Ʈ 
	EVENT_CONNECT,					// �Ϻ� ������ �̺�Ʈ	

	EVENT_CLOTHES_EXCHANGE,			// �Ϻ� ���� �ǻ� ��ȯ
	EVENT_OX_GOZONE,				// O.X�̺�Ʈ �� ����

	PLATINUM_EXCHANGE_STONE,        // �÷�Ƽ�� ���ü� ��ȯ
	WIN_SELECT,						// ��±� �˾� ���߱�
	EVENT_GOLDENBALL,				// ��� �� �̺�Ʈ	
	EVENT_OPENBETAITEMGIFT,			// �̱� �̺�Ʈ ���� ����
	EVENT_NETCAFE_BOX,				// net cafe ķ����
	EVENT_RAINYDAY,
	EVENT_BUDDHISM,					// �ұ� �к� ���� �̺�Ʈ( �±� )
	EVENT_COLLECTBUGS,				// ���� ���� ä�� �̺�Ʈ
	EVENT_MOTHERDAY,					// �±� ��Ӵϳ� �̺�Ʈ
	EVENT_PROMOPACK,				// ���θ� ��Ű�� �̺�Ʈ ( ������ )
	EVENT_PROMO_KEY_ENTER,			// ���θ� ��Ű�� ����Ű �Է�

	EVENT_XMAS_2006,				// 2006 X-MAS Event [12/11/2006 Theodoric]
	EVENT_LOVE_LOVE,				// 2007�߷�Ÿ��'s ����(���귯�� �̺�Ʈ)
	EVENT_WHITEDAY_2007,			// 2007 WhiteDay Event
	NPC_HELP,						// NPC �ȳ� �ý���
	
	EVENT_MAY,						// WSS_GUILD_MASTER 070411 -->><<
	EVENT_MINIGAME,					// WSS_MINIGAE 070420 ������ �̺�Ʈ
	EVENT_FLOWERS_SUPPORT,			// �ɳ��� �̺�Ʈ ��ȭ ����
	EVENT_SUPPORT_EXCHANGE,			// �ɳ��� �̺�Ʈ �⿩���� ��ȯ��
	EVENT_SUMMER_2007,				// [070705: Su-won] EVENT_SUMMER_2007
	SIEGEWARFARE_MASTERTOWER1,		// WSS_DRATAN_SEIGEWARFARE 2007/07/30 // �����ϱ�
	SIEGEWARFARE_MASTERTOWER2,		// Ÿ�� ��ȭ�ϱ� 
	SIEGEWARFARE_MASTERTOWER3,		// ���� ��ȭ�ϱ�
	SIEGEWARFARE_MASTERTOWER4,		// ������ Ÿ�� �����ϱ�
	
	////////////////////////////////////////////////////////////////////////////////////////////////
	// [070708: Su-won] EVENT_ADULT_OPEN
	EVENT_ADULT_MAGICCARD,					// ���μ��� ���� �̺�Ʈ(����ī�带 ã�ƶ�!)
	EVENT_ADULT_CHANGEJOB,					// ���μ��� ���� �̺�Ʈ(�����޼�!�����Ǻ����� �޾ƶ�!)
	EVENT_ADULT_ALCHEMIST,					// ���μ��� ���� �̺�Ʈ(��� ���� ���ݼ� �̺�Ʈ)
	EVENT_SHOOT,							// ���μ��� ���� �̺�Ʈ(���ε鸸�� Ư��)
	// [070708: Su-won] EVENT_ADULT_OPEN
	////////////////////////////////////////////////////////////////////////////////////////////////

	EVENT_BJMONO_2007_REQ,					// SK BJ MONO ����� �ޱ�
	EVENT_BJMONO_2007_CHANGE_REQ,			// SK BJ MONO ����� ��ȯ
	
	EVENT_TG2007_1000DAYS_CAP,				// ��Ʈī���� 1000�� ������ �ޱ�
	EVENT_TG2007_1000DAYS_CAP_UPGRADE,		// ������ ���׷��̵�
	EVENT_TG2007_1000DAYS_FIRECRACKER,		// ���� �ޱ� �̺�Ʈ
	EVENT_TG2007_FRUITFULL,					// ǳ���̺�Ʈ  	
	EVENT_TG2007_SCREENSHOT,				// ��ũ���� �̺�Ʈ
	
	EVENT_HALLOWEEN2007_DEVILHAIR,			// �Ǹ����� �Ӹ��� ���� �ޱ�
	EVENT_HALLOWEEN2007_CANDYBASKET,		// �����ٱ��Ͽ� ���� �ޱ�
	EVENT_HALLOWEEN2007_PUMKINHEAD,			// ȣ�ڸӸ� Ż ��ȯ �ޱ�
	EVENT_HALLOWEEN2007_WITCHHAT,			// ������� ��ȯ �ޱ�
	EVENT_HALLOWEEN2007_PUMKIN,				// ȣ�ڱ�ȯ �ޱ�
	
	// 071129_ttos: �Ǹ� ������ȸ ȸ��
	CASH_PERSONSHOP_CLOSE,					// ������ �Ǹ� ���� ����
	CASH_PERSONSHOP_ITEM,					// ������ ��ǰ ȸ��
	CASH_PERSONSHOP_NAS,					// ������ ���� ȸ��

	EVENT_XMAS2007_DECO,					// ũ��������Ʈ�� ����ϱ�
	EVENT_XMAS2007_BLESSEDNESS,				// ũ�������� �ູ �ޱ�

	EVENT_RED_TREASUREBOX,					//������ �������� �̺�Ʈ

	EVENT_SAKURA_2008,						//2008�� ���� �̺�Ʈ

	EVENT_CHAOSBALL,						//ī������ �̺�Ʈ

	EVENT_PHOENIX_REQ,							//�Ǵн� �̺�Ʈ
	
	EVENT_HANAPOS_SK_EVENT,					//[ttos_2009_3_18]: �ϳ����� �� SK��ε��� ������ �̺�Ʈ
	
	MIDLEVEL_SUPPORT_REQ,					// �������� ������ ��ų ��û
	
	EVENT_REQUITAL_1,						//[ttos_2009_4_13]: ���� ���� �����̺�Ʈ
	EVENT_REQUITAL_2,						//[ttos_2009_4_13]: �⼮ üũ ����
	EVENT_ADD_UP_NEW_AFRON,					// ������ , ���� ���� �����̺�Ʈ���ǰ ����(������ �ٿ�)
	
	QUEST_RESTORE_ITEM,						// [100208: selo] ����Ʈ ������ �ٽ� �ޱ�
	
	EVENT_WOLRDCUP_2010_EVENT2,				// ($E_WC2010) [100514: selo] 2010 ���ư� ������ Event2 ����, ��Ȳ
	EVENT_WOLRDCUP_2010_EVENT2_STATUS,		// ($E_WC2010) [100524: selo] 2010 ���ư� ������ Event2 ��Ȳ
	EVENT_WOLRDCUP_2010_EVENT2_GIFT,		// ($E_WC2010) [100517: selo] 2010 ���ư� ������ Event2 ��Ȳ, ����
	EVENT_WOLRDCUP_2010_EVENT3,				// ($E_WC2010) [100514: selo] 2010 ���ư� ������ Event3 ���� ���� �����ϱ�

	ATTENDANT_DATE_REQ,						// [7/2/2010 kiny8216] �⼮ �ý��� (���� �⼮�� ��û)
	DUNGEON_DIFFICULTY_NORMAL,				// ��ĭ��� Normal
	DUNGEON_DIFFICULTY_HARD,				// ��ĭ��� Hard
	EVENT_TEMP,								// �ܹ߼� �̺�Ʈ ��(100907 ��ĭ ��� �̺�Ʈ)
	
	REGISTER_MERCHANT,						// [2010/08/25 : Sora] ADD_SUBJOB ���� ���

	EVENT_PROMOTION2,						// [10/6/2010 kiny8216] ���� ���θ�� �̺�Ʈ

	EVENT_ATTENDANCE_2011,					// [2011/01/18 : Sora] �⼮ �̺�Ʈ

	EVENT_COMEBACK,							// �޸� ���� ���� �̺�Ʈ

	EVENT_BIRTHDAY_GIFT,					// ���� �̺�Ʈ : ���� ��û
	EVENT_BIRTHDAY_INFO,					// ���� �̺�Ʈ : ���� ���� ��û

	EVENT_TREASURE_MAP,						// ����ã�� �̺�Ʈ�� [����]���� ���� ��ȯ ��û

	EVENT_LOI_KRATHONG,						// �±� ũ���� �̺�Ʈ �������� ��ȯ ��û

	EVENT_KB_EXCHAGE_HEART,					// [2011/11/14 : Sora] �±� ���� ź���� �̺�Ʈ �������� ��ȯ
	EVENT_KB_EXCHAGE_FLAG,					// [2011/11/14 : Sora] �±� ���� ź���� �̺�Ʈ �̺�Ʈ�ǻ� ��ȯ

	EVENT_SONGKRAN,							// ��ũ�� �̺�Ʈ.

	SYNDICATE_JEWEL_GIVE,
	SYNDICATE_SECESSION,
};

enum eEvent
{
	EVENT_NEWYEAR1_REWARD,
	EVENT_NEWYEAR2_TIMECHECK,
	EVENT_NEWYEAR2_REWARD,

	EVENT_FF_REG,
	EVENT_FF_ISREWARD,
	EVENT_FF_TIMECHECK,
	EVENT_FF_REWARD,

	EVENT_REWARD_1,
	EVENT_REWARD_2,
	EVENT_REWARD_3,

	EVENT_MAY_CHILDREN,
	EVENT_MAY_PARENTS,
	EVENT_MAY_MASTER,

	EVENT_WORLDCUP_2010_EVENT2_SEL1,		// ($E_WC2010) [100514: selo] ��� ���� ����	
	EVENT_WORLDCUP_2010_EVENT2_SEL2,		// ($E_WC2010) [100514: selo] �� ���� ��Ȳ
	EVENT_WORLDCUP_2010_EVENT2_SEL3,		// ($E_WC2010) [100517: selo] ��÷ ���� �ޱ�

	EVENT_WORLDCUP_2010_EVENT3_SEL1,		// ($E_WC2010) [100518: selo] ����ī�� �ޱ�
	EVENT_WORLDCUP_2010_EVENT3_SEL2,		// ($E_WC2010) [100518: selo] ���ϸ��� ���� ���� �ޱ�
	EVENT_WORLDCUP_2010_EVENT3_GIFT1,		// ($E_WC2010) [100518: selo] ��� ��� ���� �ޱ�
	EVENT_WORLDCUP_2010_EVENT3_GIFT2,		// ($E_WC2010) [100518: selo] 4�� ��� ���� �ޱ�
	EVENT_WORLDCUP_2010_EVENT3_GIFT3,		// ($E_WC2010) [100518: selo] 8�� ��� ���� �ޱ�
	EVENT_WORLDCUP_2010_EVENT3_GIFT4,		// ($E_WC2010) [100518: selo] 16�� ��� ���� �ޱ�
};

class CUIQuest : public CUIWindow
{
protected:
	struct sQuestList
	{		
		INDEX	iQuestIndex;
		SBYTE	sbQuestFlag;
		
		bool operator<(const sQuestList &other) const
		{
			if( sbQuestFlag > other.sbQuestFlag )
				return true;
			return false;
		}
		bool operator>(const sQuestList &other) const
		{			
			return other.operator < (*this);
		}
	};
	
	// Controls
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnOK;								// Learn button
	CUIButton				m_btnCancel;							// Cancel button
	CUIScrollBar			m_sbQuestIcon;							// Scrollbar of special remission icon
	CUIListBox				m_lbQuestDesc;							// List box of remission description

	BOOL					m_bShowQuestInfo;						// If skill tool tip is shown or not	
	UIRect					m_rcQuestInfo;							// Action information region
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information
	int						m_nCurQuestInfoLines;					// Count of current Quest information lines
	CTString				m_strQuestInfo[MAX_QUESTINFO_LINE];		// Quest information string
	COLOR					m_colQuestInfo[MAX_QUESTINFO_LINE];		// Color of Quest information string

	// Quest buttons
	std::vector<CUIButtonEx>	m_vectorbtnQuests;					// Buttons of special remission

	// Quest information
	int						m_nSelQuestID;							// Selected special remission ID
	CTString				m_strShortDesc;							// Short remission information string

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar
	UIRect					m_rcIcons;								// Region of icons

	// UV of each part
	UIRectUV				m_rtBackgroundTop;						// UV of background top
	UIRectUV				m_rtBackgroundBottom;					// UV of background bottom
	UIRectUV				m_rtSelOutline;							// UV of outline of selected button

	// FIXME : 
	int						m_iNpcIndex;
	int						m_iNpcVirIdx;
	int						m_iCurQuestIndex;
	BOOL					m_bLockQuest;

	// Quest List
	std::vector<sQuestList>		m_vectorQuestList;

	// Event save data
	CTString				m_strTeamA, m_strTeamB;			// ��� �� �̺�Ʈ ��
	int						m_ScoreTeamA, m_ScoreTeamB;		
	SYSTEMTIME				m_GoldenEndTime;
	BOOL					m_bGoldenBallEntry;				// ���� ����
	BOOL					m_bGoldenBallRequital;			// ���� ����
	BOOL					m_bStartGoldenBall;
	CTString				m_strCountry[WORLDCUP_MAX_COUNTRY];
	std::map<INDEX, BYTE>	m_mapNationFlag;				// ($E_WC2010) [100517: selo] ���� ������ Ȯ�ο�(map�� �˻����� ������)
	
	// [2011/02/09 : Sora]	����Ʈ������ �ٽ� �ޱ�
	int						m_restoreQuestIndex;
	SLONG					m_restoreQuestItemIndex;
protected:
	// Internal functions	
	void	RenderQuestBtns();
	void	GetQuestDesc( BOOL bFullDesc, int nQuestIndex, SBYTE sbFlag );
	void	AddQuestDescString( CTString &strDesc, const COLOR colDesc );
	void	PressOK( );

	void	AddQuestInfoString( CTString &strQuestInfo, COLOR colQuestInfo = 0xF2F2F2FF );
	void	GetQuestInfo( int nQuestIndex, SBYTE sbQuestFlag, int &nInfoWidth, int &nInfoHeight );
	void	ShowQuestInfo( BOOL bShowInfo, CUIButtonEx *pQuestBtn = NULL );

	// Network message functions ( send )
	void	SendQuest();
	
public:
	CUIQuest();
	~CUIQuest();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	ENGINE_API void	OpenQuest( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ );
	
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	void	QuestError( UBYTE ubError );

	BOOL	InitQuest( );
	void	CloseQuest();

	void	AddToQuestList(INDEX iQuestIndex, SBYTE sbQuestFlag);
	void	ClearQuestList();

	void	LockQuest(BOOL bLock);
	void	SetCurQuest(int iQuestIndex) { m_iCurQuestIndex = iQuestIndex; }

	int 	GetNpcPosX(){ return m_fNpcX;}
	int		GetNpcPosZ(){ return m_fNpcZ;}
	
	int		GetNpcIndex(){ return m_iNpcIndex; }

	void	SetStrTeamName( CTString strTeamA, CTString strTeamB, SYSTEMTIME GoldenEndTime, BOOL bFlag )
	{ 
		m_strTeamA = strTeamA; m_strTeamB = strTeamB;
		if( bFlag ) m_GoldenEndTime = GoldenEndTime;
	}

	void	SetTeamScore( int ScoreA, int ScoreB ) { m_ScoreTeamA = ScoreA; m_ScoreTeamB = ScoreB; }
	void	SetGoldenBallEntry( BOOL bType ) {
		m_bGoldenBallEntry = bType;
		if( bType == TRUE ) m_bStartGoldenBall = TRUE;
	}
	void	SetGoldenBallRequital( BOOL bType ) {
		m_bGoldenBallRequital = bType;
		if( bType == FALSE ) m_bStartGoldenBall = FALSE;
	}

	CTString	GetStrTeamA( void ) { return m_strTeamA; }
	CTString	GetStrTeamB( void ) { return m_strTeamB; }

	int			GetScoreTeamA( void ) { return m_ScoreTeamA; }
	int			GetScoreTeamB( void ) { return m_ScoreTeamB; }

	SYSTEMTIME	GetGoldenTime( void ) { return m_GoldenEndTime; }

	void	CreateMessageBox_WorldCup2010_Event2( INDEX nType );
	void	InitNationFlag();
	BOOL	IsNationFlag(INDEX nItemIndex)
	{
		return m_mapNationFlag.find(nItemIndex) == m_mapNationFlag.end() ? FALSE : TRUE;
	}
};


#endif	// UIQUEST_H_
