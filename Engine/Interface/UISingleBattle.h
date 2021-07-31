#ifndef	UISINGLE_BATTLE_H_
#define	UISINGLE_BATTLE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Include Files
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>

// Define size of SINGLE Battle
#define	SB_WIDTH						118
#define	SB_HEIGHT						61

//------------------------------------------------------------------------------
// CUISingleBattle
// Explain:  ��� ���� 
//------------------------------------------------------------------------------
class ENGINE_API CUISingleBattle : public CUIWindow
{
protected:
	UIRect					m_rcBox;						// �ð� ǥ�� â (���)
	UIRect					m_rcRedBox;						// RedBox : KillCount
	UIRect					m_rcBlueBox;					// BlueBox : KillCount
		
	UIRectUV				m_rtBox;						
	UIRectUV				m_rtBlueBox;					// BlueBox : KillCount
	UIRectUV				m_rtRedBox;						// RedBox : KillCount
	
	UIRectUV				m_rtSmallNumber[10];			// �ð� ǥ�� �� ����
	UIRectUV				m_rtLargeNumber[10];			// KillCount ǥ�� �� ���� 
	UIRectUV				m_rtColon;						// �ð� ǥ�� �� :(�ݷ�)		

	// etc
	CTString				m_strTitle;						// Ÿ��Ʋ �̸� (������� ���� �޼��� �Ѹ��� ���)	
	
public:

	CUISingleBattle();
	~CUISingleBattle();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Clear();	//
	void	Close();	// ����

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	SetFocus( BOOL bVisible );
	void	OpenSingleBattle();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Render
	void	Render();					// Render	
	void	RenderGBStatus();			// ��� ���� ����â
	void	DrawNumber( int x, int y, int nNumber, bool bLarge = false );	// ����	
	void	DrawColon( int x, int y );	// Colon Render

	void	RenderKillPoint();			// Render Kill Point
};

#endif	// UIGB_H_