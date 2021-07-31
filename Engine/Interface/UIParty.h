// ----------------------------------------------------------------------------
//  File : UIParty.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIPARTY_H_
#define	UIPARTY_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// 
// #include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UITrackPopup.h>
// #include <Engine/Network/CNetwork.h>
// #include <vector>


// Party type
enum PartyType
{
	PT_PEACEEVER = 0,	// �յ� �й�
	PT_SURVIVAL = 1,	// �Լ��켱 
	PT_ATTACK = 2,		// �����ڰ���ġ �켱
};


// Maximum count of party members
#define	PARTY_MAX_MEMBER			7


// Define text position
#define	PARTY_TITLE_TEXT_OFFSETX	25
#define	PARTY_TITLE_TEXT_OFFSETY	5
//#define	PARTY_NAME_SX				11
//#define	PARTY_NAME_SY				23
#define	PARTY_NAME_SX				15
#define	PARTY_NAME_SY				2


// Define size of option
//#define	PARTY_WIDTH					140
//#define	PARTY_HEIGHT				60
//#define	PARTY_STRETCH_HEIGHT		36
//#define	PARTY_BAR_WIDTH				120
#define	PARTY_WIDTH					161
#define	PARTY_HEIGHT				57
#define	PARTY_STRETCH_HEIGHT		58
#define	PARTY_BAR_WIDTH				99


//////////////////////////////////////////////////////////////////////////
//  [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////
#define EXPEDITION_GROUP_MAX 4
#define EXPEDITION_MEMBER_PER_GROUP 8
#define EXPEDITION_MEMBER_MAX 32


//������ �׷� â
class CUIExpGroup : public CUIWindow
{
protected:
	int	m_nGroupNum;

	int m_nOldX;
	int m_nOldY;

	BOOL m_bTitleBarClick;

	UIRectUV	m_rtBackground;
	UIRect		m_rcTitle;

	CUIButton	m_btnClose;
	CUIButtonEx m_btnExpeditionMember[EXPEDITION_MEMBER_PER_GROUP];

public:
	CUIExpGroup();
	~CUIExpGroup();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	SetGroupNum(int nGroupNum) { m_nGroupNum = nGroupNum; }
	int		GetMemberCount();

	void	ClearExpeditionGroup();

	CUIButtonEx &GetMemberBtn(SLONG slPos) { return m_btnExpeditionMember[slPos]; }
};

//������ ���� â
class CUIExpManage : public CUIWindow
{
protected:

	//���� â POPUP COMMAND
	enum eManageCommandList
	{
		MANAGE_CONNAMD_CHANGE_LEADER = 0,
		MANAGE_CONNAMD_SET_SUBLEADER,
		MANAGE_CONNAMD_RESET_SUBLEADER,
		MANAGE_CONNAMD_KICK_MEMBER,
		MANAGE_CONNAMD_VIEW_MEMBER,
	};

	CTextureData	*m_ptdCommonBtnTexture;
	CTextureData	*m_ptdMessageBoxTexture;

	UIRectUV	m_rtBackground;
	UIRect		m_rcTitle;

	CUIButton	m_btnClose;
	CUIButton	m_btnInvite;
	CUIButton	m_btnCancel;
	CUIButtonEx m_btnExpeditionMember[EXPEDITION_GROUP_MAX][EXPEDITION_MEMBER_PER_GROUP];

	CUITrackPopup		m_tpList;

	BOOL SetCommand(SLONG slPosition, BOOL bIsMyGroup = FALSE, BOOL bIsExpeditionLeader = FALSE);
	void RunCommand(INDEX iCommand);

public:

	SLONG		m_slDestIndex;					// ���õ� ��� index
	SLONG		m_slSeletedBtnGroup;
	SLONG		m_slSeletedBtnPos;

	CTString	m_strDestName;

	BOOL		m_bCanUseBtn;

	CUIExpManage();
	~CUIExpManage();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetButton(SLONG slGroup, SLONG slPos, CUIButtonEx btnExpMember) { m_btnExpeditionMember[slGroup][slPos].Copy(btnExpMember); }
	CUIButtonEx &GetMemberBtn(SLONG slGroup, SLONG slPos) { return m_btnExpeditionMember[slGroup][slPos]; }

	void ClearExpManage();

};

// ���캸�� â(��û ���� ����� ������ ������( �ǽð� ����X ))
class CUIViewDetail : public CUIWindow
{
protected:
	int			m_nShownItem;					// ������ ������ ǥ���ϰ� �մ� ���� ��ȣ

	CTextureData	*m_ptdCommonBtnTexture;

	CTString	m_strCharInfo;
	UIRectUV	m_rtBackground;
	UIRect		m_rcTitle;

	CUIButton	m_btnClose;
	CUIButtonEx m_btnViewDetail[VIEW_DETAIL_BTN_MAX];
	CNetworkLibrary::sPetInfo			m_sPetInfo;									// ���캸�� �� ����
	sPetItem_Info					m_sWildPetInfo;									// ���캸�� ������ �� ����

public:
	CUIViewDetail();
	~CUIViewDetail();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Render();
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void	SetCharInfo(CTString strCharInfo) { m_strCharInfo = strCharInfo; }
	void	SetPetInfo( CNetworkLibrary::sPetInfo petInfo ) { m_sPetInfo = petInfo; }
	void	SetWildPetInfo( sPetItem_Info wildPetInfo )  { m_sWildPetInfo = wildPetInfo; }
	CUIButtonEx	&GetViewDetailBtn( SLONG slWearPos) { return m_btnViewDetail[slWearPos]; }
	void	ClearViewDetail();
};
//////////////////////////////////////////////////////////////////////////


// ----------------------------------------------------------------------------
// Name : CUIParty
// Desc :
// ----------------------------------------------------------------------------
class CUIParty : public CUIWindow
{
protected:
	// Control
	CUIButton			m_btnClose;									// Close button

	CUIButton			m_btnOption;								// UI_REFORM :Su-won

	// Info
	int					m_nType;									// Party type
	int					m_nMemberCount;								// Count of members
	BOOL				m_bAmILeader;								// If I am a leader or not
	BOOL				m_bIsPartyRequested;						// If party is requested now or not
	CTString			m_strDestName;								// String of dest name when request party
	SLONG				m_aslIndices[PARTY_MAX_MEMBER];				// Indices of members
	SLONG				m_aslZone[PARTY_MAX_MEMBER];				// Zone of members
	CTString			m_astrNames[PARTY_MAX_MEMBER];				// Strings of member's name
	BOOL				m_abLeaders[PARTY_MAX_MEMBER];				// If this member is a leader or not
	SBYTE				m_asbJobs[PARTY_MAX_MEMBER];				// Jobs of members
	int					m_aswLevels[PARTY_MAX_MEMBER];				// Levels of members
	int					m_aswHPs[PARTY_MAX_MEMBER];					// HPs of members
	int					m_aswMaxHPs[PARTY_MAX_MEMBER];				// Maximum HPs of members
	int					m_aswMPs[PARTY_MAX_MEMBER];					// MPs of members
	int					m_aswMaxMPs[PARTY_MAX_MEMBER];				// Maximum MPs of members
	FLOAT				m_afPosX[PARTY_MAX_MEMBER];					// Position x of members
	FLOAT				m_afPosZ[PARTY_MAX_MEMBER];					// Position z of members
	SBYTE				m_asbLayer[PARTY_MAX_MEMBER];				// Layer of members
	UBYTE				m_aubBuff[PARTY_MAX_MEMBER];				// If buff is shown or not
	bool				m_bOnline[PARTY_MAX_MEMBER];				// is online 
	BOOL				m_bShowBuff;								// iF buff is shown or not

	ULONG				m_ultargetID[PARTY_MAX_MEMBER];				// ��Ƽ���� �����ϰ� �ִ� Ÿ�� ID
	SBYTE				m_sbtargetType[PARTY_MAX_MEMBER];			// ��Ƽ���� �����ϰ� �ִ� Ÿ�� Type

	// Region
	UIRect				m_rcTitle;
	UIRect				m_arcHPs[PARTY_MAX_MEMBER];					// Region of member's HP
	UIRect				m_arcMPs[PARTY_MAX_MEMBER];					// Region of member's MP
	UIRect				m_rcBuff;									// Region of buff toggling icon

	// UV
	UIRectUV			m_rtTop;									// UV of top region
	UIRectUV			m_rtMiddle;									// UV of middle region
	UIRectUV			m_rtBottom;									// UV of bottom region
	UIRectUV			m_rtHP;										// UV of HP
	UIRectUV			m_rtMP;										// UV of MP
	UIRectUV			m_rtBuff[2];								// UV of buff toggling icon
	CTString			m_strMandateCharName ;						// �����Ϸ��� �ɸ����� Index

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////
	CTextureData		*m_ptdExpeditionTexture;	

	//������ ��� ����
	struct stExpeditionMember
	{
		SLONG		m_slIndex;								// ĳ���� �ε���
		SLONG		m_slZone;								// ����ȣ
		CTString	m_strName;								// �̸�
		SBYTE		m_nPosition;							// ��å
		SBYTE		m_sbJob;								// ����
		int			m_nLevel;								// ����
		int			m_nHP;									// HP
		int			m_nMaxHP;								// MAX HP
		int			m_nMP;									// MP
		int			m_nMaxMP;								// MAX MP
		FLOAT		m_fPosX;								// ��ġ(X)
		FLOAT		m_fPosZ;								// ��ġ(Z)
		SBYTE		m_sbLayer;								// Layer
		FLOAT		m_fHPBarRate;							
		FLOAT		m_fMPBarRate;							
		BOOL		m_bOnline;								// ���� ����

		stExpeditionMember()
		{
			Init();
		}

		void Init()
		{
			m_slIndex = -1;		
			m_slZone = -1;	
			m_strName = "";	
			m_nPosition = -1;
			m_sbJob = -1;	
			m_nLevel = -1;	
			m_nHP = -1;		
			m_nMaxHP = -1;	
			m_nMP = -1;		
			m_nMaxMP = -1;	
			m_fPosX = -1;	
			m_fPosZ = -1;	
			m_sbLayer = -1;	
			m_fHPBarRate = 0.0f;
			m_fMPBarRate = 0.0f;
			m_bOnline = FALSE;
		}

		BOOL IsEmpty()
		{
			return m_slIndex == -1 ? TRUE : FALSE;
		}

		//HP,MP ���� ���� ���
		void UpdateBarRate()
		{
			FLOAT	fRatio = (FLOAT)m_nHP / (FLOAT)m_nMaxHP;
			if( fRatio > 1.0f ) fRatio = 1.0f;	

			m_fHPBarRate =  fRatio;

			fRatio = (FLOAT)m_nMP / (FLOAT)m_nMaxMP;
			if( fRatio > 1.0f ) fRatio = 1.0f;	
			
			m_fMPBarRate =  fRatio;
		}

		//�־��� ���̸� ���� ����� ���� ���
		int CalcHPBarWidth(int nTotalBarWidth)
		{
			return nTotalBarWidth * m_fHPBarRate;
		}

		int CalcMPBarWIdth(int nTotalBarWidth)
		{
			return nTotalBarWidth * m_fMPBarRate;
		}
	};

	stExpeditionMember m_ExpeditionGroup[EXPEDITION_GROUP_MAX][EXPEDITION_MEMBER_PER_GROUP];

	CUIExpGroup m_UIExpGroup[EXPEDITION_GROUP_MAX];						// ������ ����UI

	CUIExpManage m_UIExpManage;														// ������ ����UI

	CUIViewDetail m_UIViewDetail;													// ���캸�� UI

	int			m_nMyGroupNum;														// �� �׷��ȣ
	int			m_nGroupPos;														// �׷� �� ��ġ
	SBYTE		m_sbDivisionTypeEXP;												// ����ġ �й� Ÿ��
 	SBYTE		m_sbDivisionTypeITEM;												// ������ �й� Ÿ��
	SBYTE		m_sbDivisionTypeSPITEM;												// ����� �����ۺй� Ÿ��

	UIRectUV	m_rtExpPosition[2];													// ������ ��åUV
	
	// [091119: selo] ���� ��Ƽ ������ �ľ�
	BOOL		m_bIsPartyPlay;

//////////////////////////////////////////////////////////////////////////

protected:
	// Internal functions
	void	AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
							int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
							FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	RemoveMember( SLONG slIndex );
	void	BreakUpParty();
	void	UpdateMemberInfo( int nMemIndex );
	void	TargetMember( int nIndex );
	void	UpdatePartyBuff( int nMemIndex, int nX, int nY );

	// Network message functions ( send )
	void	SendPartyAllow();
	void	SendPartyReject();

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////

	void	RenderExpedition();

	// Network message functions ( send )
	void	SendExpeditionAllow();															// ������ ���� �޽��� ����
	void	SendExpeditionReject();															// ������ ���� �޽��� ����

	void	PartyData2Expedition(SLONG slIndex, int nPos);									// ������ ��ȯ�� ��Ƽ�� �ִ���ġ�� �����뿡 ����
//////////////////////////////////////////////////////////////////////////

public:
	CUIParty();
	~CUIParty();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Init
	void	Init();

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Party full
	BOOL	IsPartyFull() { return m_nMemberCount == PARTY_MAX_MEMBER; }

	// Get information
	int		GetMemberCount() const { return m_nMemberCount; }
	SLONG	GetMemberIndex( int nIndex ) const { return m_aslIndices[nIndex]; }
	SLONG	GetMemberZone( int nIndex ) const { return m_aslZone[nIndex]; }
	FLOAT	GetPosX( int nIndex ) const { return m_afPosX[nIndex]; }
	FLOAT	GetPosZ( int nIndex ) const { return m_afPosZ[nIndex]; }
	SBYTE	GetLayer( int nIndex ) const { return m_asbLayer[nIndex]; }
	int		GetLevel( int nIndex ) const { return m_aswLevels[nIndex]; }
	CTString	GetMemberName( int nIndex ) const { return m_astrNames[nIndex]; }				// [2012/10/11 : Sora] ����� ����

	// 051203
	// ENGINE_API bool IsPartyMember( CEntity *pEntity );
	ENGINE_API bool IsPartyMember( INDEX iChaIndex );

	// Update status of member
	ENGINE_API void	UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP, int iMP, int iMaxMP );

	// Toggle visible
	void	ToggleVisible();

	// Hide party buff
	void	HidePartyBuff() { m_bShowBuff = FALSE; }

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );

	// Network message functions ( send )
	void	SendPartyInvite( SBYTE sbType, CTString &strDestName );
	void	SendPartyQuit();
	void	SendPartyKick( CTString &strDestName );

	// Network message functions ( receive )
	void	PartyInvite( SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName, SBYTE sbPartyTypeItem = -1, SBYTE sbPartyTypeSPItem = -1);
	void	PartyReject( BOOL bLeader );
	void	PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
							int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
							FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );
	void	PartyQuit( SLONG slIndex );
	void	PartyKick( SLONG slIndex );
	void	PartyEnd();
	void	PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
							int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline = true);
	void	MandateBossReq( CTString strManadateChaName );
	void	MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate );
	void	PartyError( SBYTE sbError );
	bool	IsOurParty( CTString strName );		// �̸����� ��Ƽ�� �˻�
	int		IsOurPartyPos( CTString strName );	// �̸����� �˻� 
	int		IsOurParty( SLONG slCharIndex );	// �ĺ��ڷ� ��Ƽ�� �˻�
	void	SetPartyLeader();					// ��Ƽ ���� ����
	BOOL	AmILeader() { return m_bAmILeader; }
	void	SetDestName( CTString strDest ) { m_strDestName = strDest; }
	BOOL	IsPartyRequested() { return m_bIsPartyRequested; }
	void	SetPartyRequested( BOOL IsRequested ) { m_bIsPartyRequested = IsRequested; }

	ENGINE_API void	SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType);

	void	SetPartyType( SBYTE sbPartyType) { m_nType = sbPartyType; }

	// [091119: selo] ���� ��Ƽ�������� �ƴ� �÷���
	BOOL	GetIsPartyPlay(void) { return m_bIsPartyPlay; }
	void	SetIsPartyPlay(BOOL bIsPartyPlay) { m_bIsPartyPlay = bIsPartyPlay; }

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////

	void	InitExpedition();																// ������ �ʱ�ȭ

	// Network message functions ( send )
	void	SendChangeExpedition();																	// ������� ��ȯ �޽��� ����
	void	SendExpeditionInvite( CTString &strDestName );											// ������ �ʴ� �޽��� ����
	void	SendDivisionType(SBYTE sbDivisionType, SBYTE sbType);									// �й�Ÿ�� ���� �޽��� ���� 
	void	SendExpeditionQuit();																	// ������ Ż�� �޽��� ����
	void	SendExpeditionKick( SLONG slIndex );													// ������ ���� �޽��� ����
	void	SendExpeditionEnd();																	// ������ ���� �޽��� ����
	void	SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum);	// ������� �׷� ����
	void	SendExpeditionViewDetail(SLONG slIndex, CTString strCharName);							// ���캸�� ��û
	void	SendChangeSubLeader(SLONG slIndex);														// �δ��� ���� ��û
	void	SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex);				// ǥ�� ������û
	void	SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex);								// ǥ�� ���� ��û

	void	SendPartyEnd();																			// ��Ƽ ��ü ��û
	void	SendInitInZone();																		// �δ� �ʱ�ȭ ��û

	// Network message functions ( receive )
	void	ChangeExpedition( CNetworkMessage *istr );
	void	ExpeditionInvite( CNetworkMessage *istr );								// ������ �ʴ� �޽��� ����
	void	ExpeditionError( CNetworkMessage *istr );								// ������ ���� ����
	void	ExpeditionReject( BOOL bIsSrc );										// ������ ���� �޽��� ����
	void	ExpeditionQuit( SLONG slType, SLONG slIndex );										// ������ Ż�� �޽��� ����
	void	ExpeditionKick( SLONG slIndex );										// ������ ���� �޽��� ����
	void	ExpeditionEnd();														// ������ ���� �޽��� ����
	void	ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum);					// ������� �׷� ���� ����
	void	SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst = FALSE); // �й��� ����
	void	SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM);

	// Set Expedition Info
	void	AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex,			// ������� �߰�
						CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
						int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
						FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );

	void	AddExpeditionMember( int nGroupNum, int nPos, stExpeditionMember expMember);

	void	RemoveExpeditionMember( int nGroup, int nPos );										// ������� ����

	void	ExpeditionMemberInfo( SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP,			// ������� ���� ����
								int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, 
								SLONG slZone, bool bOnline = true );
	void	ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline );
	void	SetExpeditionPosition(SLONG slIndex, SLONG slPosition);
	void	ChangeExpeditionLeader( SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode );

	void	OpenExpeditionMemberWindow();													// ������� ����â OPEN
	void	OpenExpeditionManageWindow();													// ������� ����â OPEN

	// Get Expedition Info
	int		GetExpeditionMyPosition();
	SLONG	GetExpeditionMemberIndex(int nGroup, int nPos);									// ������ index
	SLONG	GetExpeditionMemberIndex(CTString strName);										// �̸����� �˻�
	SLONG	GetExpeditionMemberZone(int nGroup, int nPos);									// Zone
	FLOAT	GetExpeditionMemberPosX(int nGroup, int nPos);									// X
	FLOAT	GetExpeditionMemberPosZ(int nGroup, int nPos);									// Z
	SBYTE	GetExpeditionMemberLayer(int nGroup, int nPos);									// Layer
	BOOL	GetExpeditionMemberOnline(int nGroup, int nPos);								// ���ӻ���
	bool	SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos);					// �ε����� ��������� �׷�, ��ġ�� �˻�
	ENGINE_API bool IsExpedetionMember( INDEX iChaIndex );									// �ش� �ε����� ����������� üũ 	
	bool	IsExpedetionDataExist(int nGroup, int nPos);									// �ش� ��ġ�� ������� ������ �ִ°� üũ
	COLOR	GetJobStringColor(SBYTE sbJob);													// ������ �ؽ�Ʈ ���� ��ȯ
	SBYTE	GetPartyType() { return m_nType; }
	SBYTE	GetDivisionType(int nType);
	CTString	GetExpeditionMemberName(int nGroup, int nPos);								// [2012/10/11 : Sora] ����� ����

	void	OpenDivisionInfoWindow();														// �й� ����â�� ����

	void	SetExpeditionKickTargetIndex(SLONG slIndex) { m_UIExpManage.m_slDestIndex = slIndex; }	// ������ �������ε��� ����(simplepop���� �����밭��޴��� ���)

	void	SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex);		// ǥ�� ����
	void	SetViewDetail( SBYTE sbType, CNetworkMessage *istr );							// ���캸�� ���� ����
	void	ClearTargetMark();																// ǥ�� reset

	int		IsLabeled(SLONG slTargetType, SLONG slIndex);									// �ε����� Ÿ�������� ���� üũ�Ǿ� �ֳ� Ȯ��

	void	RaidError( CNetworkMessage *istr );												// ���̵� ���� ����

	BOOL	IsExistOfflineMember();															// �������� ���üũ
	
	ENGINE_API void	ClearPartyTarget( SLONG slTargetIndex );										// [2010/08/31 : Sora]
//////////////////////////////////////////////////////////////////////////
};


#endif	// UIPARTY_H_

