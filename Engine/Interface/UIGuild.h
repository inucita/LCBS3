// ----------------------------------------------------------------------------
//  File : UIGuild.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIGUILD_H_
#define	UIGUILD_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIMultiEditBox.h>
#include <Engine/Interface/UIWebBoard.h>
#include <Engine/Interface/UIComboBox.h>
#include <vector>
#include <Engine/Interface/UIDrawFigure.h>	//WSS_NEW_GUILD_SYSTEM 070611
#include <Engine/LocalDefine.h>				//WSS_NEW_GUILD_SYSTEM 070611
#include <Engine/Interface/UIListBoxEx.h>
#include <Engine/Interface/UIGuildArrangeMemberCont.h>
#include <Common/Packet/ptype_old_do_guild.h>


// Define text position
#define	GUILD_TITLE_TEXT_OFFSETX		25
#define	GUILD_TITLE_TEXT_OFFSETY		5

// Define size of Guild
#define	GUILD_WIDTH						216
#define	GUILD_HEIGHT					284

// Define member level
enum eMemberLevel
{
	GUILD_MEMBER_BOSS			= 0,		// ����
	GUILD_MEMBER_VICE_BOSS		= 1,		// �δ���
	GUILD_MEMBER_MEMBER			= 2,		// �ܿ�
	GUILD_MEMBER_RUSH_CAPTAIN	= 3,		// ���ݴ���
	GUILD_MEMBER_SUPPORT_CAPTAIN= 4,		// ��������
	GUILD_MEMBER_RECON_CAPTAIN	= 5,		// ��������
	GUILD_MEMBER_RUSH			= 6,		// ���ݴ��
	GUILD_MEMBER_SUPPORT		= 7,		// �������
	GUILD_MEMBER_RECON			= 8,		// �������
	GUILD_MEMBER_TOTAL			= 9,		// TOTAL
	GUILD_MEMBER_NOMEMBER	= -1,		// ��� �ƴ�
};

#define	MAX_APPLICANTS			(10)		// �ִ� ���� ��û��.
#define GUILD_SKILL_VIEW_MAX	(5)			// ��� ��ų â�� ���̴� �ִ� ��ų ����
// WSS_NEW_GUILD_SYSTEM 070716 ------------------------------->>
// #if defined(G_KOR)
// 	#define MAX_GUILDINFO_TAB	(6)
// #else
	#define MAX_GUILDINFO_TAB	(5)			//�ؿܷ����� ��� �Խ��� ��� ���� ���� (6)���� ����
	#define LOCAL_NEW_GUILD					//�ؿ� ����
#define LIMIT_GUILD_LEVEL	(6)
// Define Guild Info Tab
enum eGUILDNEW
{
	NEW_GUILD_INFO				= 0,		// ��� ����
	NEW_GUILD_MEMBER_INFO		= 1,		// ���� ����
	NEW_GUILD_SKILL				= 2,		// ��� ��ų
#ifndef LOCAL_NEW_GUILD
	NEW_GUILD_BOARD,						// ��� �Խ���
#endif
	NEW_GUILD_NOTICE,					// ��� ��������
	NEW_GUILD_MANAGE,					// ��� ����		
};

enum eGUILD_MEMBER_LIST
{
	eGML_NAME = 0,
	eGML_TITLE,
	eGML_LV,
	eGML_CLASS,
	eGML_LOCAL,
	eGML_CONTRIBUTION,
	eGML_POINT,
	eGML_MAX,
	eGML_LOGOUT_DATE = eGML_CONTRIBUTION + 1 // ���� �ε����� ����ϱ� ���� �߰�
};
class CUIIcon;
class CUIList;
class CUIListItem;
class CGuildChangeSetUI;
class CGuildDonateUI;
// -----------------------------------------------------------<<

class CGuildMemberDesign : public CUIWindow
{
public:
	CGuildMemberDesign();

	void initialize();
	void resetList();
	void SetFontColor(COLOR col);

	CUIList* m_pList;
};

// ----------------------------------------------------------------------------
// Name : CUIGuild
// Desc :
// ----------------------------------------------------------------------------
class CUIGuild : public CUIWindow
{
protected:	
	// Guild State
	enum eGuildState
	{
		GUILD_REQ,
		GUILD_CREATE,		// ����
		GUILD_UPGRADE,		// �±�
		GUILD_DESTROY,		// ��ü
		GUILD_MANAGER,		// ����
		GUILD_MANAGER_NEW,	// �ű� ��� �ý��� WSS_NEW_GUILD_SYSTEM 070704
	};
	
	// Define guild tab
	enum GUILD_TAB
	{
		GUILD_TAB_NONE		= -1,
		GUILD_TAB_MEMBER	= 0,		// ��� ���
		GUILD_TAB_TREE		= 1,		// ��� Ʈ��
		GUILD_TAB_TOTAL		= 2,
	};

	// internal structure
	struct sGuildMember
	{
		sGuildMember()
		{
			lIndex			= -1;
			eRanking		= GUILD_MEMBER_MEMBER;
			bOnline			= FALSE;
		}

		~sGuildMember()
		{
			lIndex			= -1;
			eRanking		= GUILD_MEMBER_MEMBER;			
			bOnline			= FALSE;
		}

		bool operator<(const sGuildMember &other) const
		{
			if( eRanking < other.eRanking )
				return true;
			return false;
		}
		bool operator>(const sGuildMember &other) const
		{			
			return other.operator < (*this);
		}

		SLONG			lIndex;
		CTString		strMemberName;		// �����?
		int				eRanking;			// ����?
		BOOL			bOnline;			// ������?

	};

	// Function Object
	struct FindMember
	{
		SLONG lIndex;
		FindMember(sGuildMember& info) : lIndex(info.lIndex) {};
		bool operator() (const sGuildMember& info)
		{
			if(info.lIndex == lIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	std::vector<sGuildMember>	m_vectorMemberList;			// �ܿ� ���
	
	// Controls
	CUIButton				m_btnClose;						// Close button
	CUIButton				m_btnOK;						// OK button
	CUIButton				m_btnCancel;					// Cancel button

	CUIButton				m_btnChangeBoss;				// ���� ����
	CUIButton				m_btnAccept;					// �δ��� �Ӹ�(appointment)
	CUIButton				m_btnReject;					// �δ��� ����(dismissal)
	CUIButton				m_btnMemberFire;				// �ܿ� ����
	CUIButton				m_btnExit;						// �ݱ�

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar
	UIRect					m_rcTab;

	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background		
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackManagerTop;				//
	UIRectUV				m_rtBackManagerMiddle;			//
	UIRectUV				m_rtBackManagerBottom;			//
	UIRectUV				m_rtBackBottom;					// UV of bottom background
	UIRectUV				m_rtInputBoxL;					// UV of left region of input box
	UIRectUV				m_rtInputBoxM;					// UV of middle region of input box
	UIRectUV				m_rtInputBoxR;					// UV of right region of input box

	UIRectUV				m_rtTab;						// UV of section tab

	CUIListBox				m_lbGuildDesc;					// List box of guild description
	CUIListBox				m_lbMemberList;					// List box of guild member list
	CUIListBox				m_lbApplicantList;				// List box of guild new user list

	// edit box of guild name
	CUIEditBox				m_ebGuildName;

protected:
	int						m_nCurrentTab;					// Current tab
	eGuildState				m_eGuildState;

	CTString				m_strGuildName;					// Name of Guild
	CTString				m_strBossName;					// Name of Boss
	CTString				m_strViceBoss[2];				// Name of first vice boss
	int						m_iGuildIndex;
	int						m_iGuildLevel;					// Level of Guild
	int						m_iNumOfMember;					// Number of member
	int						m_iUserRanking;					// Ranking of user
	BOOL					m_bGuildJoinReq;				//

	BOOL					m_bChackNpc;					//ī������ NPC ��� ������ üũ
	// WSS_NEW_GUILD_SYSTEM 070608 ------------------------->>

	// ��� ����------------------------------------>>
	// ��� �ý��� ���� �߰� ����
	INDEX					m_iSelTab;						// ������ ��(0 ~ 5)
	INDEX					m_iGuildAverageLevel;			// ��� ��� ����
	INDEX					m_iGuildOwnLand;				// ��� ���� ����
	INDEX					m_iGuildTotalPoint;				// ��� �� ����Ʈ
	INDEX					m_iGuildMyPoint;				// ���� �⿩ ����Ʈ		
	INDEX					m_iNumOfMaxMember;				// Number of Max Member 

	CTString				m_strRemoteGuildJoinTaget;		// ���� ��� ���� ��� �̸�

	// ������ ���� �߰� ����
	// Region of each part
	UIRect					m_rcTitleNew;					// Region of title bar
	UIRect					m_rcWindowNew;					// Region of Entire Window
	UIRect					m_rcTabNew[MAX_GUILDINFO_TAB];

	// UV of each part
	UIRectUV				m_uvTabNew;						// UV of section tab
	UIRectUV				m_uvInfoTabTL;					// UV of top left background
	UIRectUV				m_uvInfoTabTR;					// UV of top right background
	UIRectUV				m_uvInfoTabBL;					// UV of Bottom left background
	UIRectUV				m_uvInfoTabBRL;					// UV of Bottom Right1 background
	UIRectUV				m_uvInfoTabBRR;					// UV of Bottom Right2 background

	// UV of Lines
	UIRectUV				m_uvLineV;						// UV of vertical line
	UIRectUV				m_uvLineH;						// UV of Horizon line

	// Common Box
	CUIDrawBox				m_bxBox1;						// Box(1) - ���� �� �ڽ�
	CUIDrawBox				m_bxBox1_1;						// Box(1) - ���� �� �ڽ� ���� ���°�
	CUIDrawBox				m_bxBox1_2;						// Box(1) - ���� �� �ڽ� ���� ���°�
	CUIDrawBox				m_bxBox2;						// Box(2) - £�� �� �ڽ�
	CUIDrawBox				m_bxBackGroundBox;				// Box of Top Background
	CUIDrawBox				m_bxBackGroundBox2;				// Box of Bottom Background

	// ListBox
	CUIListBox				m_lbMemberAllianceList;			// List box of Alliance member list
	CUIListBox				m_lbMemberHostilityList;		// List box of Alliance member list

	// Buttons
	CUIButton				m_btnEdit;						// ���� ��ư
	CUIButton				m_btnCloseNew;					// X ��ư
	CUIButton				m_btnExitNew;					// �ݱ� ��ư
	CUIButton				m_btnGuildMarkBack;				// ��� ��ũ ���� ��ư
	// --------------------------------------------<<

	// ���� ���� -------------------------------->>	
	
	CGuildMemberDesign*		m_pGuildMember;
	std::vector<clsGuildMemberNew> m_vecGuildMember;
	INDEX					m_iOnlineMembers;				// ������ ����
	CUIButton				m_btnDonateChange;				// �� ���� ����
	// --------------------------------------------<<

	// ��� ��ų ---------------------------------->>
	UIRect					m_rcGuildSkillList;
	CUIButton				m_btnGetSkill;					// �����ϱ�
	CUIScrollBar			m_sbGuildSkillBar;				// Guild Skill Scroll Bar
	INDEX					m_iGuildSkillPos;				// Guild Skill Position
	BOOL					m_bIsSelList;					// ��� ��ų üũ ����
	CUIListBox				m_lbGuildSkillDesc;
	
	// --------------------------------------------<<

	// ��� �Խ��� -------------------------------->>
	CUIWebBoard*			m_pWebBoard;					// ��� �Խ��ǿ��� ���(���� �Խ��ǿ���)
	CUIDrawBox				m_bxBox3;						// Box(3) - ��� �ڽ�
	CUIDrawBox				m_bxBox4;						// Box(4) - �׵θ����� �����ڽ�

	// Controls	
	CUIButton			m_btnPrev;							// Prev button
	CUIButton			m_btnNext;							// Next button
	CUIButton			m_btnList;							// List button
	CUIButton			m_btnWrite;							// Write button
	CUIButton			m_btnReply;							// Reply button
	CUIButton			m_btnModify;						// Modifu button
	CUIButton			m_btnDelete;						// Delete button
	CUIButton			m_btnPage[10];						// Buttons of page
	CUIButton			m_btnSearch;						// Search button
	CUIEditBox			m_ebSearch;							// Search edit box
	CUIComboBox			m_cmbSearch;						// Combobox of search

	// List type
	CUIListBox			m_lbListContent;					// Listbox of list content ( col : 0-No, 1-Subject, 2-Writer, 3-Date, 4-Hit )	

	// Read type
	CUIListBox			m_lbReadContent;					// Listbox of reading content	

	// Write type
	CUIEditBox			m_ebWriteSubject;					// Subject edit box in writing

	// Write ...
	CUIMultiEditBox		m_mebContent;						// ���� ����
	
	// -------------------------------------------->>

	// ��� ���� ---------------------------------->>
	// ���� �Է�	
	CUIEditBox				m_ebNoticeTitle;					// ����
	CUIMultiEditBox			m_mebNoticeContent;					// ����
	CUIButton				m_btnNotice;						// ���� ��ư
	CUIButton				m_btnNoticeCorrect;					// ���� ��ư	
	CUIButton				m_btnUpdateNotice;					// �ۼ��Ϸ� ��ư
	BOOL					m_bEnableCorrect;					// ���� ����
	// --------------------------------------------<<

	// ��� ���� ---------------------------------->>
	std::vector<INDEX>		m_vManageMemberIndex;			// ��� �ε���(list box�� ������ ����)
	CUIListBox				m_lbManageMemberList;			// ���� ����Ʈ
	CUIListBox				m_lbManageMemberListEx;			// ���� ����Ʈ (����ġ, ��ġ �ּ� �ִ� ����)
	CUIButton				m_btnChangeBossNew;				// ���� ����
	CUIButton				m_btnAcceptNew;					// �δ��� �Ӹ�
	CUIButton				m_btnRejectNew;					// �δ��� ����
	CUIButton				m_btnMemberFireNew;				// ��� ����
	CUIButton				m_btnChangeSetting;				// ���� ����
	CUIButton				m_btnChangeSettingAll;			// ���� ���� ���� ��ü
	CGuildChangeSetUI*		m_pGuildChangeSetUI;
	CGuildDonateUI*			m_pGuildDonateUI;

	// ���� ������
	BOOL					m_bApplySettingOn;				// ���� ����â ����
	BOOL					m_bApplyDonateUI;
	
	int						m_nSelSkillTab;					// ���õ� ��� ��ų �� ( 0 : Passive Guild Skill, 1 : Active Guild Skill )
	CTString				m_strSkillTabPopupInfo;			// �� �˾� ���� (��Ƽ��, �нä�)
	UIRect					m_rcSkillTab[2];				// ��� ��ų ��
	UIRect					m_rcSkillBtn[5];				// ��� ��ų ��ư ��ġ
	UIRect					m_rcSkillTabPopupInfo;			// �� �˾� ����
	UIRectUV				m_rtSelBoxUV;					// ���õ� ��� ��ų UV
	UIRectUV				m_rtSelSideTabUV;				// ���õ� ���� ��
	UIRectUV				m_rtUnSelSideTabUV;				// ���� �ȵ� ���� ��
	UIRectUV				m_rtPopupUL;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupUM;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupUR;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupML;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupMM;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupMR;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupLL;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupLM;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPopupLR;					// ��� ��ų �� �˾� UV
	UIRectUV				m_rtPassiveTabIconUV[2];		// Passive Tab Icon UV
	UIRectUV				m_rtActiveTabIconUV[2];			// Active Tab Icon UV
	CTextureData			*m_ptdSelBoxTexture;			// ���õ� ��� ��ų Texture
	CUIButton				m_btnUseSkill;					// ��� ��ų ����ϱ� ��ư
	CUIListBox				m_lbUseInfo;					// ��ų ��� ���� ǥ�� �ڽ�
	CUIListBox				m_lbLearnInfo;					// ��ų ���� ���� ǥ�� �ڽ�
	// --------------------------------------------<<
	// [1/3/2011 kiny8216] NEW_CORPS
	INDEX					m_iCorpsMember;						// �Ϲ����� ��� ��
	INDEX					m_iCorpsBoss;						// �������� ��� ��
	CTString				m_strCorps[GUILD_MEMBER_TOTAL];		// ���� ��Ʈ��
	// [1/11/2011 kiny8216] GUILD_SKILL_FIX
	CUIIcon*				m_pIconsGuildSkillEx[GUILD_SKILL_VIEW_MAX];	// ��� ��ų ��ư
	std::vector<CSkill>		m_vecGuildPassiveSkill;						// ��� �нú� ��ų
	std::vector<CSkill>		m_vecGuildActiveSkill;						// ��� ��Ƽ�� ��ų

	LONG					m_guildMarkTime;					// [sora] GUILD_MARK
	CTString				m_strGuildMarkTime;
	CUIGuildMarkIcon*		m_pIconGuildMark;

	// ������ ���� �߰�.
	ContGuild				m_ContGuild;

	INDEX			m_iSyndiType;

	// -------------------------------------------------------<<

	// Position of target npc
	FLOAT					m_fNpcX, m_fNpcZ;

	// Command
	void	PressOKBtn();
	
	// Description
	void	GetGuildDesc( BOOL bShow = TRUE );
	void	AddGuildDescString( CTString &strDesc, COLOR colDesc = 0xF2F2F2FF );

	// ��� ��ü�� ���� ó��
	void	CreateGuild();			// ��� ����
	void	UpgradeGuild();			// ��� �±�
	void	DestroyGuild();			// ��� ��ü
	void	GoGuildZone();			// ��� ���� ��������...

	// ����� ���� ó��
	void	ChangeBoss();			// ���� ����
	void	AddViceBoss();			// �δ��� �Ӹ�
	void	DelViceBoss();			// �δ��� ����
	void	MemberFire();			// �ܿ� ����
	
	//void	ApplicantReject();		// ���� �ź�

public:
	CUIGuild();
	~CUIGuild();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild
	void	OpenGuild( int iMobIndex, BOOL bHasQuest, int iUserRanking, int iGuildLevel );
	void	OpenGuildManager( int iUserRanking );
	void	ResetGuild();
	void	ClearMemberList();

	void	JoinGuild( LONG lGuildIndex, LONG lChaIndex, const CTString& strName, INDEX iSyndiType );		// ��� ����
	void	QuitGuild();		// ��� Ż��

	// ��� ��� ����
	void	AddToMemberList( LONG lIndex, const CTString& strName, int eLevel = GUILD_MEMBER_MEMBER, BOOL bOnline = TRUE );		// ��� ��Ͽ� �߰�
	void	DelFromMemberList( LONG lIndex );
	void	ChangeMemberLevel( LONG lIndex, int eLevel );
	void	ChangeMemberOnline( LONG lIndex, BOOL bOnline );

	// ��� ��� ����
	// NOTE : �������̽��� ������ �ݵ�� ���ŵǾ�� �ϴ� �κ��̹Ƿ� TRUE�� �����ϰ�,
	// NOTE : ���̴� �����϶��� �����ϰ��� �Ѵٸ� FALSE�� �����Ұ�.
	void	RefreshMemberList( BOOL bInit = FALSE );
	void	RefreshApplicantList( BOOL bInit = FALSE);

	// �����ڿ� ���� ó��
	void	ApplicantAccept( LONG lIndex );		// ���� ����

	// ��� ����
	void	PressChangeSetUIOK();
	void	PressChangeSetAllUIOK();
	void	PressDonateUIOK();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// WSS_NEW_GUILD_SYSTEM 070702
	// ���ʿ��ؼ� �ּ� ó��
	// Set focus
//	void	SetFocus( BOOL bVisible );
			
	// Edit box focus
	BOOL	IsEditBoxFocused();
	void	KillFocusEditBox();

	// For using name change card  060120
	void	SetMemberName(int charIndex,CTString strName) ;
	
	int GetGuildMemberCount() const 
	{
		return m_vectorMemberList.size();
	}

	int GetUserRanking()
	{
		return m_iUserRanking;
	}

	int GetGuildLevel()
	{
		return m_iGuildLevel;
	}

	// WSS_NEW_GUILD_SYSTEM 070716 -------------------------------------->>
	void	InitNewGuildSystem();
	void	CreateNew( CUIWindow *pParentWnd, int nX, int nY);
	void	OpenGuildManagerNew();
	
	void	RenderNew();
	void	RenderNewGuildInfo(int nx,int ny);
	void	RenderNewGuildMemberInfo(int nx,int ny);
	void	RenderNewGuildNoticeInput();
	void	RenderNewGuildManage(int nx,int ny);
	void	RenderNewGuildManagePopup();
	void	RenderNewGuildManageSetAllPopup();
	void	RenderNewGuildBoard(int nx,int ny);
	void	RenderNewGuildBoardList();
	void	RenderNewGuildBoardRead();
	void	RenderNewGuildBoardWrite();	
		
	// ��� ��� ���� �߰�
	void	AddGuildMemberInfo(clsGuildMemberNew tMemberInfo);
	void	updateGuildMemberList();
	void	AddGuildManageInfo(clsGuildMemberNew tMemberInfo);
	CTString SetPosName(CTString tName);

	// ��� �Խ���
	void	GetBoardListContent();
	void	GetBoardReadContent();	

	// ��� ���� ���� ���� ��ü ��ư ����
	void	SetManagePopup(BOOL bEnable);
	void	SetManageSetAllPopup(BOOL bEnable, int nOpenType = -1);
	void	ResetManagePopupString();
	void	ResetManageSetAllPopupString();
	// ���� ���� �׸� üũ
	BOOL	CheckDataValidation();
	BOOL	CheckDataValidationDonate();
	
	// Message Send
	void	SendRequestGuildTab(int iTabNum);
	void	SendChangeGuildInclination(UBYTE ubIncline);
	void	SendAdjustGuildMemberInfo( int charIdx, CTString posName ,int expMin, int expMax, int fameMin, int fameMax, int corps = -1, UBYTE ubStashAuth = 0 );
	void	SendAdjustGuildMemberInfoAll(int expMin, int expMax, int fameMin, int fameMax, int AdjustExp, int AdjustFame);
	void	SendGuildDonateInfo();
	void	SendAdjustGuildDonateInfo(int AdjustExp, int AdjustFame);
	void	SendUpdateGuildNotice( CTString strTitle, CTString strContents);
	void	SendRequestGuildNotice();	
	void	SendLearnGuildSkill(int skillIdx); 
	// Message Receive
	void	ReceiveNewGuildMessage(UBYTE ubType,CNetworkMessage* istr);
	void	UpdateGuildSkillLearnMessage(UpdateClient::doNewGuildSkillLearnToMemberMsg& SkillInfo);

	// Mouse Message
	WMSG_RESULT	MouseMessageNew( MSG *pMsg );
	WMSG_RESULT	MouseMessageNewBoard( MSG *pMsg );

	// Encode Notice String
	// \r\n MultiBox�� ������� ��Ʈ�� �Լ� ����
	void EncodeNoticeString(CTString& tStr);
	void DecodeNoticeString(CTString& tStr);

	// Adjust Guild Member Info 
	void AdjustGuildMemberInfo();
	void AdjustDonateMinMaxInfo();
	void AdjustMyDonateInfo();
	// Guild Skill Setup
	void SetGuildSkillList();
	// Guild Skill Check
	ENGINE_API int GetGuildSkillLevel(int skillIdx);
	// Set Guild Skill Member Extend
	void ResetGuildMaxMember(int guildSkillLevel);
	// Set Guild Position Name
	CTString ResetPosName(CTString tPosName,int ulPosition);

	void RenderNewGuildSkillExtend(int nX,int nY);			// Ȯ�� ��� ��ųâ ����
	
	void ReceiveGuildSkillExtend(CNetworkMessage* istr);	// Ȯ�� ��� ��ų ���� �ޱ� (Server->Client) : (int)skill_type, (int)count, (int)skill_index, (int)skill_level
	
	void SetGuildSkillInfo();								// Ȯ�� ��� ��ų â ��� ����, ���� ���� ����
	void SetSkillPopupInfo(int nTab);

	int	GetGuildSkillCount();								// ��� ��ų �� ���� ���
	CUIIcon* GetSkillButton(int nPos);					// ��� ��ų ��ư ���
	CSkill* GetGuildSkill(int nPos);						// (��ųâ���� Ŭ�� �� ����������) ��� ��ų ���
	BOOL CheckUseGuildSkill(int nSkillIndex);				// ��� ��ų ��� ���� üũ
	BOOL StartGuildSkillDelay( int nSkillIndex );			// ��� ��ų ������ ����
	INDEX GetSelectedPositon();								// �޺��ڽ����� ���õ� �δ� ���� ���
	void SetSkillBtnInfo();									// ��� ��ų ��ư ���� ����
	BOOL GetSkillDelay( INDEX index );						// ��ų ������ ���
	DOUBLE GetReuseTime( INDEX SkillIndex );				// ����ð� ���
	DOUBLE GetCoolTime( DOUBLE ReuseTime, DOUBLE StartTime );	// ��Ÿ�� ���
	void SetCorpsInfo( INDEX memberCount, INDEX bossCount );	// ����� �δ� ���� ����
	void InitString();										// �δ� ���� ��Ʈ�� ����
	void SetCorpsComboBox();								// �δ� ���� �޺� �ڽ� ����
	void ReceiveCorpsInfo(CNetworkMessage* istr);			// ��� �δ� ���� �ޱ�
	CTString GetMemberLogoutInfo(int nLogoutTime);			// �α׾ƿ��� ��� ���� ���
	CTString GetMemberLogoutTooltip(int nLogoutTime, int& nWidth);// �α׾ƿ��� ��� �����������
	void SetMemberListStrColor(CUIListItem* pListItem, COLOR col);// ��� ����Ʈ ������ ���� �� ���� 
	// --------------------------------------------------------------------<<

	void		SetRemoteGuildJoinTaget(CTString &strTargetName)		{ m_strRemoteGuildJoinTaget = strTargetName;	}
	CTString	GetRemoteGuildJoinTaget()								{ return m_strRemoteGuildJoinTaget;				}
	void		ArrangeMemList(const int eType);
	CUICheckButton* m_pCbMemberArrange[eGML_MAX];
	BOOL		IsOnline(CTString strName);
	int			GetMemberNetIdx( CTString strName );
	WMSG_RESULT OpenGuildPop(int nMemberIdx, int nX, int nY);

	void		ResetArrangeState();
	void		ClearGuildSkill();
	void		ClearGuildSkillCooltime();
};

#endif	// UIGUILD_H_