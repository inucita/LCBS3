// ----------------------------------------------------------------------------
//  File : UIChatting.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UICHATTING_H_
#define	UICHATTING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <functional>
#include <list>
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIListBox.h>


// Date : 2005-02-16,   By Lee Ki-hwan
#define CHAT_MEMORY_COUNT			5



// Chatting type tab
enum ChattingTypeTab
{
	CHATTAB_ALL		= 0,
	CHATTAB_PARTY,
	CHATTAB_GUILD,
	CHATTAB_TRADE,
	CHATTAB_TOTAL,
};


// Chatting message type
// channel chat level�� common option���� �ϸ� ���� �ٵ�~
// ����� fixed type���� ����
enum ChattingMsgType
{
	CHATMSG_NORMAL					= 0,
	CHATMSG_PARTY					= 1,
	CHATMSG_GUILD					= 2,
	CHATMSG_TRADE					= 3,
	CHATMSG_WHISPER					= 4,
	CHATMSG_SHOUT					= 5,
	CHATMSG_NOTICE					= 6,
	CHATMSG_GM						= 7,
	CHATMSG_DUMMY					= 8,		// �޽��� �ý��۰� ���� ���� �ֱ� ����(�̱�ȯ)
	CHATMSG_LORD					= 9,		// ����
	CHATMSG_RANKER					= 10,		// ��Ŀ
	CHATMSG_RANKER_CONN				= 11,		// ��Ŀ
	CHATMSG_NOTICE_SERVERDOWN		= 12,		// ���� ���� �޽��� 060420
	CHATMSG_DUMMY2					= 13,		// �ε��� �����ֱ� ����.
	CHATMSG_DUMMY3					= 14,		// �ε��� �����ֱ� ����.
	CHATMSG_GMTOOL					= 15,		// GM TOOL�� �̿��� �޽��� ó�� WSS_GMTOOL 070517 (13,14 ������)
	CHATMSG_PRESSCORPS				= 16,
	CHATMSG_EXPEDITION				= 17,		// [sora] ������
	CHATMSG_NPCNOTICE				= 18,
	CHATMSG_CHANNEL_LEVEL			= 19,		// �Ϲ� ��ü ä��(������������ ä�ΰ� ä��)
	CHATMSG_CHANNEL_TRADE			= 20,		// �Ÿ� ä��(������������ ä�ΰ� ä��)
	CHATMSG_TOTAL					,		
	CHATMSG_COMMAND					,			// [/command]
};

// Chatting option type
enum ChattingOptionType
{
	CHATOPT_NORMAL					= 0,
	CHATOPT_PARTY					= 1,
	CHATOPT_GUILD					= 2,
	CHATOPT_TRADE					= 3,
	CHATOPT_WHISPER					= 4,
	CHATOPT_SHOUT					= 5,
	CHATOPT_SYSTEM					= 6,
	CHATOPT_TOTAL					= 7,
};

// System message type
enum SystemMsgType
{
	SYSMSG_NORMAL					= 0,
	SYSMSG_ATTACK					= 1,
	SYSMSG_ATTACKED					= 2,
	SYSMSG_ERROR					= 3,
	SYSMSG_NOTIFY					= 4,
	SYSMSG_USER						= 5, // wooss 070307 kw:WSS_WHITEDAY_2007
	SYSMSG_TOTAL					= 6,
};

struct sCeilWriting
{
	sCeilWriting()
	{
		strCeilWriting = CTString("");
		iCnt		   = 0;
		llStartTime    = 0;
	}

	CTString		strCeilWriting;
	int				iCnt;
	__int64			llStartTime;
};

// Define chatting line
#define	MIN_SHOW_CHATLINE			6
#define	MAX_CHAT_LINE				150
#ifdef	VER_TEST
#	define	MAX_SYSTEM_LINE				1000
#else	// VER_TEST
#	define	MAX_SYSTEM_LINE				100
#endif	// VER_TEST
#define	SYSTEM_SHOW_LINE			5
#define	CHAT_LINESPACING			1


// Define chatting tab size
#define	CHAT_TAB_WIDTH				64
#define	CHAT_TAB_SEL_WIDTH			103
#define	CHAT_TAB_OFFSET				2


// Define whisper history
#define	MAX_WHISPER_SAVE			5


// Define position of text
#define	CHAT_STRING_OFFSETX			23
#define	CHAT_STRING_OFFSTY			7


// Define size of chatting box
#define	CHATTING_WIDTH				322
#define	CHATTING_HEIGHT				137


struct sChat
{
	CTString	strName;
	CTString	strMsg;
	// normal mode
	CTString	strSendName[CHATTAB_TOTAL][MAX_CHAT_LINE];
	COLOR		colSendName[CHATTAB_TOTAL][MAX_CHAT_LINE]; // CHATMSG_TOTAL --> CHATTAB_TOTAL
	// channel mode
	CTString	strChannelSendName[CHATTAB_TOTAL][MAX_CHAT_LINE];
	COLOR		colChannelSendName[CHATTAB_TOTAL][MAX_CHAT_LINE];
	COLOR		colSendNameType[CHATMSG_TOTAL];
	COLOR		colName;
};

struct sChatSpamCheck
{
	sChatSpamCheck()
	{		
		nCount						= 0;
		nIndex						= 0;
		llStartTime					= 0;
//		llAccumTime					= 0;
//		llElapsedTime				= 0;
		llLongSentenceCheckTime		= 0;
		llCurTime					= 0;
		nStrLength					= 0;
		bBlocked					= false;		

	}
	__int64  llStartTime;
	__int64	 llCurTime;
	// ó�� �Է� �� �帥 �ð��� �������� �����Ѽ� 1���� Ȯ�� �� ��� ���.
	//ex) 30�ʵ��� 29�� �Է��ϰ� 1�� �� �ٽ� 29�� �Է��Ѵٸ� 1�и��� 29���� �Է°��� �װ� �������� �����ð��� ������ �ؾ� �� �� ) 
	//__int64  llAccumTime; //�����ð� 
	//__int64  llElapsedTime; // ������ Ȯ�ο��� �帥 �ð� 
	__int64	   llLongSentenceCheckTime; //�̱��� 5�ʵ��� �������� ���ڸ� �Է��ϸ� ���Ѵ�. 5�ʸ� �����Ѵ�. 5���Ŀ� �ٽ� 5�ʷ� ���� 

	int		 nCount;
	int		 nIndex;
	int		 nStrLength;
	bool	bBlocked;	
};

typedef struct _SScheduleSystemMessage
{
	void*		pOwner;
	INDEX		idxEventID;
	DWORD		dwTimerID;
	CTString	strMessage;
	DWORD		dwRepeatDelay;
	time_t		timeDateStart;
	time_t		timeDateEnd;
	COLOR		colMessage;
	CTString	strLink;

	void operator = (const _SScheduleSystemMessage& oData)
	{
		pOwner			= oData.pOwner;
		idxEventID		= oData.idxEventID;
		dwTimerID		= oData.dwTimerID;
		strMessage		= oData.strMessage;
		dwRepeatDelay	= oData.dwRepeatDelay;
		timeDateStart	= oData.timeDateStart;
		timeDateEnd		= oData.timeDateEnd;
		colMessage		= oData.colMessage;
		strLink			= oData.strLink;
	}
}SScheduleSystemMessage;

typedef std::map<std::string, sChatSpamCheck> MAP_CHAT_RECV_COUNT; //index, count
typedef MAP_CHAT_RECV_COUNT::iterator MAP_CHAT_RECV_COUNT_IT;

const int UV_TOTAL = 9;
// ----------------------------------------------------------------------------
// Name : CUIChatting
// Desc :
// ----------------------------------------------------------------------------
class CUIChatting : public CUIWindow
{
protected:
	// Controls
	CUIEditBox		m_ebChatInput;										// Input box of chatting string
	CUIButton		m_btnOption;										// Button of chat type filtering option
	CUICheckButton	m_cbtnChatOption[CHATOPT_TOTAL];					// Check button of chat type filtering option
	CUIScrollBar	m_sbChatScrollBar;									// Scroll bar of chatting message
	CUIScrollBar	m_sbSysScrollBar;									// Scroll bar of system message
	int				m_nCurSelTab;										// Current selected chatting tab
	int				m_nOldSelTab;										// Old selected chatting tab
	CUIButton		m_ChannelModeTab_btn[2];							// represent channel mode tab

	COLOR			m_ChannelcolChatString[CHATTAB_TOTAL][MAX_CHAT_LINE]; // Channel mode color of each chatting strings
	CTString		m_ChannelstrChatString[CHATTAB_TOTAL][MAX_CHAT_LINE];		// Channel mode strings of each chatting type
	int				m_nChannelCurChatCount[CHATTAB_TOTAL];						// Channel mode current count of chatting strings
	int				m_nChannelCurChatInsert[CHATTAB_TOTAL];					// Channel mode index of current inserted chatting string
	int				m_nChannelFirstChatRow[CHATTAB_TOTAL];						// Channel mode first row of chatting strings in list box

	UIRectUV		m_UVbtnInfo[UV_TOTAL];
	BOOL			m_bChannelMode;										// Is channel mode on now?
	BOOL			m_bRevChannelMsg;									// Is receive channel message alert?
	BOOL			m_bRecvMessage[2][CHATMSG_TOTAL];					// Is received message for alert? [0] == Channelmode, [1] == Normalmode
	// Chatting
	COLOR			m_colChatTypeColor[CHATMSG_TOTAL];					// Color of each chatting type

	COLOR			m_colChatString[CHATTAB_TOTAL][MAX_CHAT_LINE];		// Color of each chatting strings
	CTString		m_strChatString[CHATTAB_TOTAL][MAX_CHAT_LINE];		// Strings of each chatting type
	int				m_nCurChatCount[CHATTAB_TOTAL];						// Current count of chatting strings
	int				m_nCurChatInsert[CHATTAB_TOTAL];					// Index of current inserted chatting string
	int				m_nFirstChatRow[CHATTAB_TOTAL];						// First row of chatting strings in list box

	int				m_nCurChatShowLine;									// Count of current shown chatting line
	int				m_nMaxChatShowLine;									// Maximum count of shown chatting line
	int				m_nChatLineHeight;									// Height of one chatting line
	int				m_nMaxCharCount;									// Maximum count of one line characters

	// System
	COLOR			m_colSysTypeColor[SYSMSG_TOTAL];					// Color of system type
	CTString		m_strSysString[MAX_SYSTEM_LINE];					// Strings of system message
	COLOR			m_colSysString[MAX_SYSTEM_LINE];					// Color of system message
	int				m_nCurSysCount;										// Current count of system message
	int				m_nCurSysInsert;									// Index of current inserted system message
	int				m_nCurSysShowLine;									// Count of current shown system msg line
	int				m_nFirstSysRow;										// First row of system strings in list box

	// Whisper
	BOOL			m_bShowWhisperHistory;								// If whisper history is shown or not
	int				m_nWhisperCount;									// Count of whisper history
	int				m_nCurWhisper;										// Current index of whisper history
	CTString		m_strWhisper[MAX_WHISPER_SAVE];						// Strings of whisper target
	CTString		m_strWhisperTarget;									// String of old whisper targer

	// Option
	BOOL			m_bShowOption;										// If option is shown or not
	BOOL			m_bChatOption[CHATTAB_TOTAL][CHATOPT_TOTAL];		// If current chatting type is shown or not

	// Region of each part
	UIRect			m_rcResizeFrame;									// Region of top frame for resizing(Chat)
	UIRect			m_rcSysResizeFrame;									// Region of top frame for resizing(System)
	UIRect			m_rcSystem;											// Region of system message
	UIRect			m_rcChatting;										// Region of chatting message
	UIRect			m_rcAllTab;											// Region of all tabs
	UIRect			m_rcTab[CHATTAB_TOTAL];								// Region of each chatting type tab
	UIRect			m_rcInput;											// Region of input box
	UIRect			m_rcWhisper;										// Region of whisper history
	UIRect			m_rcSelWhisper;										// Region of selected whisper target
	UIRect			m_rcOption;											// Region of chatting option

	// UV of each part
	UIRectUV		m_rtWhisperUL;										// UV of upper left background
	UIRectUV		m_rtWhisperUM;										// UV of upper middle background
	UIRectUV		m_rtWhisperUR;										// UV of upper right background
	UIRectUV		m_rtWhisperML;										// UV of middle left background
	UIRectUV		m_rtWhisperMM;										// UV of middle middle background
	UIRectUV		m_rtWhisperMR;										// UV of middle right background
	UIRectUV		m_rtWhisperLL;										// UV of lower left background
	UIRectUV		m_rtWhisperLM;										// UV of lower middle background
	UIRectUV		m_rtWhisperLR;										// UV of lower right background
	UIRectUV		m_rtSelWhisper;										// UV of selected whisper target
	UIRectUV		m_rtTabL;											// UV of left region of chatting type tab
	UIRectUV		m_rtTabM;											// UV of middle region of chatting type tab
	UIRectUV		m_rtTabR;											// UV of right region of chatting type tab
	UIRectUV		m_rtInputBoxL;										// UV of left region of input box
	UIRectUV		m_rtInputBoxM;										// UV of middle region of input box
	UIRectUV		m_rtInputBoxR;										// UV of right region of input box
	UIRectUV		m_rtChatUL;											// UV of upper left background with resizing region
	UIRectUV		m_rtChatUM;											// UV of upper middle background with resizing region
	UIRectUV		m_rtChatUR;											// UV of upper right background with resizing region
	UIRectUV		m_rtChatML;											// UV of middle left background
	UIRectUV		m_rtChatMM;											// UV of middle middle background
	UIRectUV		m_rtChatMR;											// UV of middle right background
	UIRectUV		m_rtChatLL;											// UV of lower left background
	UIRectUV		m_rtChatLM;											// UV of lower middle background
	UIRectUV		m_rtChatLR;											// UV of lower right background
	UIRectUV		m_rtChatUL2;										// UV of upper left background without resizing region
	UIRectUV		m_rtChatUM2;										// UV of upper middle background without resizing region
	UIRectUV		m_rtChatUR2;										// UV of upper right background without resizing region

	// Date : 2005-02-16,   By Lee Ki-hwan
	std::vector<CTString>		m_strMsgBuffer;
	int							m_iCurrentMsgBuffer;
	
	CTextureData	*m_ptdButtonTexture;			// Texture of Button
		// [090826: selo] �ý��� �޽��� ����� ����
	CTString		m_strSaveSysMsgFileName;
	bool			m_bSaveSysMsg;
	FILE*			m_fpSaveSysMsg;
	const int		m_iCeilWritingMaxCnt;
	BOOL			m_bIsCeilWritingCheck;
	std::list<sCeilWriting>		m_sCeilWriting;
	sChat			m_sChatInfo;

	MAP_CHAT_RECV_COUNT	m_mapChatCount; //�д� 30ȸ Ȯ�� added by sam 11/02/22

	// ===>> IMPROV1107_NOTICESYSTEM ���� �ý��� ��ȭ [trylord 11/12/26]
	std::vector<SScheduleSystemMessage*>	m_vScheduleSystemMessage;
	CTString		m_strUserNoticeOwner;
	CUIListBox		m_lbUserNoticeMessage;		// �׸��� ������ ��Ʈ���� ������ ����
	time_t			m_timeUserNoticeDelay;
	UIRect			m_rcUserNoticeActiveArea;
	UIRect			m_rcUserNotice;
	UIRectUV		m_rtUserNoticeUV;
	BOOL			m_bShowUserNotice;
	BOOL			m_bActiveUserNotice;
	BOOL			m_bClickUserNotice;
	// <<=== IMPROV1107_NOTICESYSTEM


protected:
	// Internal finctions
	void	ChangeChattingBoxHeight( int ndHeight );
	void	ChangeSystemBoxHeight(int nHorLine, BOOL bUpdate=FALSE);
	void	AddChatString( CTString &strChatString, COLOR colChatString, int nCurTab, BOOL bNotice, BYTE Channel = 0);
	void	AddSysString( CTString &strSysString, COLOR colSysString );
	void	RenderChatStringList();
	void	RenderSysStringList();
	void	RenderChatNameMsgString(const int iPosX, const int iPosY, const int iChatLine);
	void	ProcessParty( char *pcPartyString );
	void	InsertChatPrefix( ChattingMsgType cmtType );
	void	AddWhisperTarget( CTString &strWhisper );
	void	PetTransform( char *pet_changeString );
	void	WearingArmortest(char *item_string);
	void	TakeOffArmortest(char *item_string);
	void	RenderConditionsysMsg(ChattingMsgType cmtType);

	// Network message functions ( send )
	void	SendChatCommand( char *pcChatString, int nLength );
	int		CheckExpeditionChatGroup( char* pcChatString);					// [sora] ������ä�ý� �׷� ä������ üũ

	BOOL	CeilWritingCut_CompareStr(const CTString& strChat);
	void	AddListCeilWriting(const CTString& str);
	void	EraseListCeilWriting();

public:
	void	SendChatMessage( char *pcChatString, BOOL bLord = FALSE );
	
	CUIChatting();
	~CUIChatting();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustChatPosition(int nMaxChatShowLine);
	void	AdjustSysPosition(void);
	void	TopFrameMoving(int &nHeightStretch, int nCurrentShowLine, BOOL bSysMsg=FALSE);

	// Get input box
	CUIEditBox	&GetInputBox() { return m_ebChatInput; }

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
			m_ebChatInput.SetFocus( FALSE );
	}
	
// [KH_070423] �߰�
	void OpenAndSetString(CTString strIn);

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebChatInput.IsFocused(); }
	void	KillFocusEditBox() { m_ebChatInput.SetFocus( FALSE ); }

	// Reset chatting
	void	ResetChatting();

	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	void	SetIsCeilWritingCheck(BOOL bCeil)			{ m_bIsCeilWritingCheck = bCeil; }
	BOOL	GetIsCeilWirtingCheck()						{ return m_bIsCeilWritingCheck; }
	// [sora] RAID_SYSTEM 
	void	GetChatWindowEndPos(int &nPosX, int &nPosY);		//ä��â ���� ������ â�� ��ġ��Ű�� ���� ä��â ��ġ�� ���ϴ� �Լ�


	// [090826: selo] �ý��� �޽��� ����� ����
	void	Begin_SaveSysMessage(void);
	void	Write_SaveSysMessage(const CTString& strSysMessage);
	void	End_SaveSysMessage(int iType = 0);

	// Network message functions ( receive )
	void ENGINE_API	AddChatMessage( UBYTE ubChatType, SLONG slSendIndex, CTString &strSendName, CTString &strChatMessage, BYTE channelNumber = 0, SLONG slGroup = -1 ); // [sora] ������ ä������ slGroup�߰�
	void ENGINE_API	AddSysMessage( CTString &strSysMessage, int nSysType = SYSMSG_NORMAL ,COLOR colMsg = 0xFFFFFFFF); // wooss 070307 kw : WSS_WHITEDAY_2007

	//added by sam 11/02/22 ���и� ��
	BOOL CheckSpamCount ( CTString& strName , CTString& strChat );	
	void SpamerBlock ( CTString& strName );
	void SpamerLift ( CTString& strName );	//����
	//void RemoveSpamerFromMap ( CTString& strName );
	void RemoveSpamerFromMap ( std::string& strTemp );
	
	// ĳ���� �̸��� �ƴ� �ε����� �� ��� �� �Լ��� ���� map�� first���ڸ� �����ؾ� �Ѵ�. 
	//void CheckSpamCount ( int  nIndex, CTString& strName, CTString& strChat );
	// ĳ���Ͱ� ���� ��� ����Ʈ���� �����ְ� �ʹٸ� ��� ���� �� �� ���� 
	//void RemoveSpamerFromMap ( int nIndex );
	void ClearSpamMap ();
	BOOL CheckInputChannelChat(ChattingMsgType cmtType, CTString strMessage); // ä�� ä�� �Է��� �������� Ȯ��
	ENGINE_API void ShowNPCTalk( INDEX iMobIdx, INDEX iStrIndex );

	void	ResetChattingAlert();
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

#ifdef	IMPROV1107_NOTICESYSTEM
	void					LoadScheduleSystemMessage();
	void					AddScheduleSystemMessage(time_t timeStart, time_t timeEnd, DWORD dwRepeatDelday, CTString strMessage, COLOR colMessage);
	static void CALLBACK	ScheduleSystemMessageCallback(UINT uTimerID, UINT uMsg, DWORD_PTR pdwUser, DWORD dw1, DWORD dw2);
	void					StopScheduleSystemMessage(INDEX idxEventID);

	void					RenderUserNotice();
	void					PopupUserNotice();
	void					AddUserNotice(CTString strOwner, CTString strMessage);
#endif

	void	SystemMessageColorInI();
private:
	void	CmdGMExeCmdLine(const char* filename);
	void	CmdTranslate(const char* args);

#ifdef VER_TEST
#endif // VER_TEST
	
	void	reg_gm_cmd();

#if		!defined(WORLD_EDITOR)
	typedef		std::function<bool(const char* args)>		_func_gm;
	typedef		std::map< std::string, _func_gm >			_map_gmCmd;
	typedef		std::map< std::string, _func_gm >::iterator _map_gmCmd_iter;

	_map_gmCmd	m_CmdGM;
#endif	// WORLD_EDITOR
};

#define		UICHAT()	CUIManager::getSingleton()->GetChatting()


#endif	// UICHATTING_H_

