// ----------------------------------------------------------------------------
//  File : UIMinigame.h
//  Desc : Created by ant
// ----------------------------------------------------------------------------

#ifndef	UIMinigame_H_
#define	UIMinigame_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIDrawFigure.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>

// Define size of MINIGAME
#define	MINIGAME_WIDTH				428
#define	MINIGAME_HEIGHT				370

enum _btnState 
{
	BTN_STATE_DISABLE_ALL = 0,	//��ü ��ư disable
	BTN_STATE_OPEN ,			//â ���½� 
	BTN_STATE_WIN_DRAW,			//�¸� ��� ��	
	BTN_STATE_START_CONTINUE,	//���ӽ���/����ϱ� ���� �� 
	BTN_STATE_DEFEAT,			//�й� �� 
	BTN_STATE_ENABEL_ALL,		//��ü ��ư enable
};

enum _gameState
{
	MINIGAME_START =0 ,			//���� ���� �� 
	MINIGAME_LOSE,				//���� �� 	
	MINIGAME_DRAW,				//����� ��
	MINIGAME_WIN_1 =3,				//1��
	MINIGAME_WIN_2,				//2��
	MINIGAME_WIN_3,				//3��
	MINIGAME_WIN_4,				//4��
	MINIGAME_WIN_5,				//5��
	MINIGAME_WIN_6,				//6��
	MINIGAME_WIN_7,				//7��
	MINIGAME_WIN_8,				//8��	
	MINIGAME_STATE_END,			
};

enum _npcState
{
	MINIGAME_NPC_NOTHING =0,	//�⺻ ����
	MINIGAME_NPC_READY,			//���ӽ���
	MINIGAME_NPC_START,			//�������ý�(�������� ���������)
	MINIGAME_NPC_RESULT,		//���(������ ���� ���)
};


// ----------------------------------------------------------------------------
// Name : CUIMinigame
// Desc :
// ----------------------------------------------------------------------------
class CUIMinigame : public CUIWindow
{
protected:
	// Boxies
	CUIDrawBox			m_bxTitle;
	CUIDrawBox			m_bxBack1;
	CUIDrawBox			m_bxBack1_1;
	CUIDrawBox			m_bxBack2;
	CUIDrawBox			m_bxNpcTalk;
	CUIDrawBox			m_bxWinNum;
	
	// Buttones
	CUIButton			m_btnReceiveItem;		// �׸��ϰ� ��ǰ �ޱ�
	CUIButton			m_btnStartGame;			// ���� ����
	CUIButton			m_btnKeepGoing;			// ���� ���
	CUIButton			m_btnScissors;			// ���� �̹���
	CUIButton			m_btnStone;				// ���� �̹���
	CUIButton			m_btnPaper;				// �� �̹���
	CUIButton			m_btnNpcChoice[3];		// NPC ���� �̹���
	CUIButton			m_btnNpcChoiceMove;		// NPC ���� ������ �̹���

	// Other Images
	UIRect				m_rtVS;					// VS Image
	UIRectUV			m_rcVS;
	UIRect				m_rtSelBtn;				// Select Image
	UIRectUV			m_rcSelBtn;
	UIRect				m_rtSelBtn2;			// Another Select Image
	UIRectUV			m_rcSelBtn2;			
	
	// Position of target npc
	INDEX				m_npcIdx;
	FLOAT				m_fNpcX, m_fNpcZ;

	// Addtional Texture
	CTextureData		*m_ptdAddTexture;			// Texture of event image

	// Npc Talk
	CTString			m_strNpcTalk[MINIGAME_STATE_END];	
	CUIListBox			m_listNpcTalk;
	BOOL				m_bTalkOnce;

	// Minigame network messages
	UBYTE m_ubEventType;
	UBYTE m_ubMsgStart;
	UBYTE m_ubMsgSelect;
	UBYTE m_ubMsgContinue;
	UBYTE m_ubMsgEnd;

	DWORD m_dwTimePass;

public:
	CUIMinigame();
	~CUIMinigame();

	// Need Data
	int m_extra1;
	int m_extra2;
	// npc choice
	INDEX				m_npcChoice;
	INDEX				m_userChoice;
	// Others
	INDEX				m_iWinNum;
	INDEX				m_iWinNumOld;
	INDEX				m_whowin;
	INDEX				m_npcState;

	// close
	void	Close();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderNpcSlot(INDEX npcState);
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open portal
	void	OpenMinigame(UBYTE eventType);

	// Set data
	void	SetMinigameResult(int wincount,int usersel,int npcsel,int whowin);
	void	SetNpcTalk(int iTalkIdx);
	void	InitData();
	void	SetButtonState(UBYTE iState);
	void	SetMinigameWinMark();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	
};

#endif	// UIMinigame_H_

