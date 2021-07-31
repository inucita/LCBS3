
#ifndef	UIMESSENGER_H_
#define	UIMESSENGER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIGroup.h>
#include <Engine/Interface/UITrackPopup.h>
#include <Engine/GlobalDefinition.h>

// Define size of messenger
//#define	MESSENGER_WIDTH					180
//#define	MESSENGER_HEIGHT				291
#define	MESSENGER_WIDTH					288
#define	MESSENGER_HEIGHT				308

enum eGroup
{
	GROUP_INSERT,		//�׷� �߰�
	GROUP_DELETE,		//�׷� ����
	GROUP_RENAME,		//�׷�� ����
};

enum eMemberMenu
{
	MM_GROUP_CHANGE,	//�׷� �̵�
	MM_CHAT,			//��ȭ�ϱ�
	MM_DELETE,			//����
	MM_BLOCK,			//����
	MM_TELEPORT			//�ڷ���Ʈ
};
//------------------------------------------------------------------------------
// CUIMessenger
// Explain:  
// Date : 2005-05-19,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUIMessenger : public CUIWindow
{
protected:
	// Controls
	CUIButton				m_btmSetState;				//���º�ȭ ��ư
	
	CUIButton				m_btnClose;					//�ݱ� ��ư
	CUIButton				m_btnGroup;					//"�׷����" ��ư
	CUIButton				m_btnBlock;					//"����" ��ư
	CUIButton				m_btnInsert;				//"�߰�" ��ư
	CUIButton				m_btnDelete;				//"����" ��ư
	
	CUIScrollBar			m_sbMemberList;				//��ũ��

	CTString				m_strTitle;					// Ÿ��Ʋ�� �̸�

	// Information of friend & guild
	CMemberInfo				m_MyInfo;					// �� ����
	int						m_nTarIndex;				// ���� �ε�

	
	// Region of each part
	UIRect					m_rcTitle;								// Region of title
	
	// ũ�� ���� ���� ����
	UIRect					m_rcResizeLeft;							// Region of left resizing
	UIRect					m_rcResizeRight;						// Region of right resizing
	UIRect					m_rcResizeBottomL;						// Region of left bottom resizing
	UIRect					m_rcResizeBottomM;						// Region of middle bottom resizing
	UIRect					m_rcResizeBottomR;						// Region of right bottom resizing
	
	//Tab
	UIRect					m_rcFriendTab;				//ģ���� ����
	UIRect					m_rcBlockTab;				//������ ����
	bool					m_bFriendTab;				//Ȱ��ȭ�� ��( true=>ģ��, false=>����)

	// UV of each part
	UIRectUV3				m_rt3BackT;								// UV of upper left background
		
	UIRectUV3				m_rt3BackMT;								// UV of middle left background
	UIRectUV3				m_rt3BackMM;								// UV of middle middle background
	UIRectUV3				m_rt3BackMB;								// UV of middle right background
	
	UIRectUV3				m_rt3Tab;				// UV of Tab

	UIRectUV3				m_rt3BackMST;							// UV of middle left background
	UIRectUV3				m_rt3BackMSM;							// UV of middle middle background
	UIRectUV3				m_rt3BackMSB;							// UV of middle right background

	UIRectUV3				m_rt3BackM;								// UV of middle left background
	UIRectUV3				m_rt3BackL;								// UV of lower left background
	
	int						m_nBoardHeight;				//�׷��� ǥ���� �κ��� ����

	CUITrackPopup			m_tpSetMyState;				//�����º��� ����
	CUITrackPopup			m_tpMemberMenu;				//��� ��Ŭ���� ��Ÿ���� ����
	CUITrackPopup			m_tpBlock;					//���� ��� ��Ŭ���� ��Ÿ���� ����
	
//�׷� ���� ���
protected:
	std::vector<CUIGroup*>	m_vecGroup;					//�׷� ����Ʈ
	CUIGroup				m_mgBlock;					//���� ��� �׷�

	CUITrackPopup			m_tpGroup;					//"�׷����" ��ư Ŭ���� ��Ÿ���� ����
	CUITrackPopup			m_tpGroupMenu;				//�׷� ��Ŭ���� ��Ÿ���� ����
	CUITrackPopup			m_tpGroupList;				//�������� ����ϴ� �׷� ����Ʈ

	float					m_fTexWidth, m_fTexHeight;	//�ؽ����� ũ��

	UIRect					m_rcBoard;					//�׷��� ǥ���� ����
		
	int						m_nSelectGroup;				//���� ������ �׷�(vector<> ���� �ε���)
	int						m_nTalkCount;				//�������� ä��â�� ��
	CMemberInfo				m_miSelectMember;			//���� ������ ���
	
	bool					m_bSelect;					//����� Ŭ��?
	bool					m_bDragging;				//�巡����?
	bool					m_bDrop;					//�巡������ ����� ���?

	int						m_nX, m_nY;					//���콺�� ��ġ(��� �巡�׽� ���)
	int						m_nFriendScrollPos;			//ģ�� �ǿ����� ��ũ�� ��ġ
	int						m_nBlockScrollPos;			//���� �ǿ����� ��ũ�� ��ġ

	int						m_nActiveMenu;				//�׷� ����Ʈ(m_tpGroupList)�� ��Ÿ�� ����
														// 0 => m_tpGroupMenu, 1 =>m_tpGroup
	bool					m_bPremiumChar;

	CTextureData*			m_ptdMenuTexture;			//������ ���� ȭ��ǥ�� ���� �ؽ���
	UIRectUV				m_rtArrow;					//ȭ��ǥ �ؽ��� ��ǥ
	
	UIRectUV				m_rtTab;					//�� �ؽ��� ��ǥ	
			
public:
	CUIMessenger();
	virtual ~CUIMessenger();

	void	Clear();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	CreateTrackPopup( float fTexWidth, float fTexHeight );
	void	CreateGroup( float fTexWidth, float fTexHeight );

	// Toggle visible
	void	ToggleVisible();

	// Render
	void	Render();
	void	RenderBackground( int nX, int nY );
	void	RenderMyInfo( int nX, int nY );
	void	RenderGroup( int nX, int nY );
	void	RenderTab(int nX, int nY);
	void	RenderSelectMember(int nX, int nY);
	void	RenderMenuArrow(int nX, int nY, int nWidth, int nHeight, int nLine);
	
	void	RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Reset
	void	ReSizing( int nResizeRange, int nWidth, int nHeight );
		
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	//wooss 051005
	void	SetMyInfo( int nCharIndex, CTString strName, eJob nJob=TOTAL_JOB, eCondition eState = ONLINE );
	void	SetMyState( eCondition eState );
	
	
	// Member Control
	void AddFriendList( int nCharIndex, int nGroupIndex, CTString strName, eJob nJob, eCondition eState = ONLINE, bool bBlock=false );
	void AddFriendList( CMemberInfo miMember,  bool bBlock );
	void DeleteMember();
	void DeleteMember( int nCharIndex );
	//void SetMemberCondition( eCondition eState );
	void SetMemberCondition( int nCharIndex, eCondition eState );
	eCondition GetMemberCondition(int nCharIndex);
	void SetMemberMenu(eMemberMenu eMenu, int nSubMenu);
	void SetBlock( int nCharIndex, CTString strName, bool bBlock);

	// Group
	void AddGroup(int nGroupIndex, CTString strName);
	void DeleteGroup();
	void RenameGroup(int nGroupIndex, CTString strNewName);
	void SetGroup(eGroup eState, int nSubMenu);
	void SetDrop( bool bDrop ) { m_bDrop =bDrop; };
	void SetDragging( bool bDragging ) { m_bDragging =bDragging; };
	void ChangeGroup(int nGroup);
	int FindGroup(int nGroupIndex);
	int FindGroup(CTString strName);
	CMemberInfo FindMember(CTString strName);
	CMemberInfo FindBlockMember(CTString strName);
	bool IsDrop() { return m_bDrop; };
	CMemberInfo GetSelectMember() { return m_miSelectMember; };
	void SetChat(int nMakeCharIndex, int nChatIndex, CTString strName);
	void AddChatMember(int nMakeCharIndex, int nChatIndex, CTString strName);
	void DeleteChatMember(int nMakeCharIndex, int nChatIndex, CTString strName);
	void SetDefaultGroupName(CTString strName);
	void Block(BYTE cError, int nCharIndex, CTString strName);
	void UnBlock(BYTE cError, int nCharIndex, CTString strName);
	
	// ģ���޴� ������Ʈ
	void UpdateMemberMenu();

	// �����̾� ĳ����
	void SetPremiumBenefit(bool bUse);
	
	//	Network
	void RegistMemberReq( CTString strName );	// ģ����� ��û
	void RegistMemberReq( int nCharIndex, CTString strName );	// ģ����� ��û
	void RegistAllow();		// ģ�� ��� ���� 
	//void RevTalk( int nSendIndex, CTString strSendName, CTString strRecvName, CTString strMessage );
	void RevTalk( int nMakeCharIndex, int nChatIndex, CTString strSendName, CTString strChat, int nColIndex );
	
	
	void ErrorMsg( int nError );

	//void Message( int nCommandCode, CTString strMessage, DWORD dwStyle );
	void Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle );
	void ErrorMessage( CTString strErrorMessage );
	void CloseAllMessageBox();

	void ResizeScrollBar();
	void DeleteMemberRep();

	void Reset();
	void ReSetFriendList();
	bool ReadyOpenTalk( CMemberInfo miMemberInfo, bool bFocus = true );
	int OpenTalk( int nMakeCharIndex, int nChatIndex, CTString strName);
	void CloseTalk( int nIndex );
	void TalkErrorMessage( CTString strMessage );

	CMemberInfo GetMyInfo() 
	{ 
		return m_MyInfo;
	}

	bool IsUseLevel();
	
	std::vector<CUIGroup*>& GetGroup(void) { return m_vecGroup; }

	//  [12/8/2006 KwonYongDae]
	int OpenTalk( int nChatIndex, const CMemberInfo targetInfo );
	void RevOneVsOneTalk( int nSenderIndex, int nResiverIndex, CTString strSendName, CTString strChat, int nColIndex );

	BOOL	CloseWindowByEsc()			{ ToggleVisible();	return TRUE;		}	
};


#endif	// UIMESSENGER_H_