// ----------------------------------------------------------------------------
//  File : UILogin.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UILOGIN_H_
#define	UILOGIN_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIScrollBar.h>
// #include <Engine/Interface/UIButtonEx.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <vector>


// Define text position
#define	LOGIN_TITLE_TEXT_OFFSETX	25
#define	LOGIN_TITLE_TEXT_OFFSETY	5


// Define size
#define	LOGIN_WIDTH					183
#define	LOGIN_HEIGHT				116

// �����Ǿ� ���� ���� [10/18/2012 Ranma]
#ifdef EUROUPEAN_SERVER_LOGIN
#define LOGIN_TITLE_HEIGHT			22	
#define EUROUPEAN_WIDTH_GAP			10			
#endif


// ----------------------------------------------------------------------------
// Name : CUILogin
// Desc :
// ----------------------------------------------------------------------------
class ENGINE_API CUILogin : public CUIWindow
{
public:
	CUILogin();
	~CUILogin();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Reset
	void	Reset();
	void	ResetBtn();
	// Render
	void	Render();
	void	SecurityRender();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	CharMessage( MSG *pMsg );

	CTString	GetUserId(){ return m_strUserID; };
	CTString	GetPassword(){ return m_strUserPW; };

	// [090715: selo] ���̵� ����
	void ReadPreviousId();
#ifdef DEV_LOGIN
	void ReadTextDevPreviousIdPw(); // Login.txt �� ����� ���̵� �н����� ������
#endif // DEV_LOGIN
	void WritePreviousId();

	void	Lock(BOOL bLock);	
	void	SetPWFocus() { m_ebPW.SetFocus(TRUE); }

	// Set focus
	void	SetFocus( BOOL bVisible )
	{
		CUIWindow::SetFocus( bVisible );
		if( !bVisible )
		{
			m_ebID.SetFocus( FALSE );
			m_ebPW.SetFocus( FALSE );
		}
	}

	// Edit box focus
	BOOL	IsEditBoxFocused() { return m_ebID.IsFocused() || m_ebPW.IsFocused(); }
	void	KillFocusEditBox()
	{
		m_ebID.SetFocus( FALSE );
		m_ebPW.SetFocus( FALSE );
	}
	void SecurityCardSet(UBYTE* SecurityNum);

	void initialize();

	void setVersion(const char* strVer);
	const char* getVersion()		{ return m_strVersion.c_str(); }
protected:
	// Press OK button
	void	PressOKBtn();

	BOOL	TryToLogin();			// �α��� �õ�

	// Connect to Server
	BOOL	ConnectToLoginServer();
	//070901_ttos: ����ī�� UI	
	BOOL	ConnectToSecurityCard();
	
private:
	// User Login
	UIRect				m_rcSecurityBack;
	UIRectUV			m_rtLogin;
	UIRectUV			m_rtClassification;	
	UIRectUV			m_rtSecurityTop;
	UIRectUV			m_rtSecurityMid;
	UIRectUV			m_rtSecurityBottom;

#ifdef	_USE_UI_XML_
	CUIEditBox*		m_ebID;								// Input Box for User ID
	CUIEditBox*		m_ebPW;								// Input Box for User Password
#else	// _USE_UI_XML_
	CUIEditBox			m_ebID;								// Input Box for User ID
	CUIEditBox			m_ebPW;								// Input Box for User Password
#endif	// _USE_UI_XML_
	CUIEditBox			m_ebSecurity;						// 070823_ttos: Input Box for Security Code
	int					m_nLoginMsgPosX, m_nLoginMsgPosY;	// Position of error message box for login
	
	// Buttons
	CUIButton			m_btnOK;							// OK button
	CUIButton			m_btnCancel;						// Cancel button

	// [090715: selo] - checkbox
	CUICheckButton		m_btnCheck;							// Check button
	// �����Ǿ� ���� ���� üũ [10/18/2012 Ranma]
#ifdef EUROUPEAN_SERVER_LOGIN
	CUICheckButton		m_btnEuroupeanCheck;					// Check button
#endif
	BOOL				m_bChecked;
	
private:
	CTString 			m_strUserID;						// User ID
	CTString			m_strUserPW;						// User Password
	BOOL				m_bUserLogin;
//	BOOL				m_bConnectToLoginServer;
	CTextureData*		m_ptdClassification;					// ��� ǥ�� 
//	int					m_LocalVersion;
	CHAR				m_chSecurityNum[4];					//���� ī�� �ѹ�
	BOOL				m_bSecurity;						//����ī�� üũ
	CTString			m_strSecurityCode;					//���� ī�� �ڵ�
	CTString			m_strSecurityPW;					//���� ī�� �н�����

	// [090715: selo] üũ��ư �ؽ��� 
	CTextureData*		m_ptdButtonTexture;
	std::string			m_strVersion;
};

#endif // UISELCHAR_H_
