
// UITool.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UITool.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "UIToolDoc.h"
#include "UIToolView.h"

#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/GameState.h>

#include "CmdUITool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define REG_LOAD_INDEX(s,d) (AfxGetApp()->GetProfileInt(_T("Settings"),_T(s),d));

// CUIToolApp

BEGIN_MESSAGE_MAP(CUIToolApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CUIToolApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	ON_COMMAND(ID_FILE_OPEN, &CUIToolApp::OnFileOpen)
	ON_COMMAND(ID_REFRESH, &CUIToolApp::OnRefresh)
	ON_COMMAND(ID_FILE_SAVE, &CUIToolApp::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CUIToolApp::OnFileSave)
END_MESSAGE_MAP()

// CUIToolApp ����

CUIToolApp::CUIToolApp()
	: m_pDrawPort(NULL)
	, m_pViewPort(NULL)
	, m_pUIMgr(NULL)
	, m_oldTime(0.f)
{
	m_bHiColorIcons = TRUE;

	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UITool.AppID.NoVersion"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.

	m_csLock.cs_iIndex = -1;
}

// ������ CUIToolApp ��ü�Դϴ�.

CUIToolApp theApp;


// CUIToolApp �ʱ�ȭ

BOOL CUIToolApp::InitInstance()
{
	BOOL bResult;
	CTSTREAM_BEGIN {
		bResult = SubInitInstance();
	} CTSTREAM_END;

	return bResult;
}

int CUIToolApp::Run()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	MSG		msg;

	while(TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0) == FALSE)
			{
				ExitInstance();
				return msg.wParam;
			}			

			if (PreTranslateMessage(&msg) == FALSE)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			CTSTREAM_BEGIN {
				Update();
				Draw();
			} CTSTREAM_END;
		}
	}

	return CWinAppEx::Run();
}

BOOL CUIToolApp::SubInitInstance()
{
	// InitInstance ���� �̵�


	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_UIToolTYPE,
		RUNTIME_CLASS(CUIToolDoc),
		RUNTIME_CLASS(CChildFrame), // ����� ���� MDI �ڽ� �������Դϴ�.
		RUNTIME_CLASS(CUIToolView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// �� MDI ������ â�� ����ϴ�.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  MDI ���� ���α׷������� m_pMainWnd�� ������ �� �ٷ� �̷��� ȣ���� �߻��ؾ� �մϴ�.

	// Engine �ʱ�ȭ
	{
		// initialize entire engine
		SE_InitEngine("UITool");
		SE_LoadDefaultFonts();
		// ska studio must use smc files instead of bmc files
		_pShell->SetINDEX("ska_bReplaceSmcWithBmc", FALSE);

		try {
			m_pfdDefaultFont = _pFontStock->Obtain_t( CTFILENAME( "Fonts\\Console1.fnt"));
			m_pfdDefaultFont->fd_pixCharSpacing = 0;
			m_pfdDefaultFont->fd_pixLineSpacing = 0;
		} catch (char *strErr) {
			WarningMessage("Error loading font: %s.", strErr);
			delete m_pfdDefaultFont;
			m_pfdDefaultFont = _pfdConsoleFont;
		}

		// set main window for engine
		SE_UpdateWindowHandle( m_pMainWnd->m_hWnd);

		GfxAPIType eNewAPI = (GfxAPIType)REG_LOAD_INDEX(_T("GAPI"),GAT_D3D);
		// if invalid api
		if(eNewAPI!=GAT_D3D && eNewAPI!=GAT_OGL) {
			// d3d is default
			eNewAPI = GAT_D3D;
		}

		_pGfx->ResetDisplayMode(eNewAPI);

		CUIManager* pUIMgr = CUIManager::getSingleton();

		if( pUIMgr )
		{
			pUIMgr->InitUIString();
		}

		CUITooltipMgr::getSingleton()->initialize();
	}

	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �� â�� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	pMainFrame->ShowWindow(m_nCmdShow);
	//pMainFrame->UpdateWindow();

	return TRUE;
}

int CUIToolApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	_pGfx->DestroyWindowCanvas( m_pViewPort);
	m_pViewPort = NULL;

	if (m_pfdDefaultFont != NULL)
	{
		m_pfdDefaultFont->Clear();
		FONT_STOCK_RELEASE(m_pfdDefaultFont);
	}

	SE_EndEngine();
	CTStream::ClearStreamHandling();

	return CWinAppEx::ExitInstance();
}

void CUIToolApp::initDraw(HWND hWnd)
{
	_pGfx->CreateWindowCanvas(hWnd, &m_pViewPort, &m_pDrawPort);

	if( m_pDrawPort )
		m_pDrawPort->SetFont(m_pfdDefaultFont);

	if( m_pUIMgr == NULL )
	{
		m_pUIMgr = CUIManager::getSingleton();
		//m_pUIMgr->SetDrawPort( m_pDrawPort );
		if (m_pUIMgr)
		{
			SE_UpdateWindowHandle( hWnd );

			RECT rect;		
			::GetClientRect(hWnd, &rect);

			m_pUIMgr->SetDrawPort(m_pDrawPort);

			m_pUIMgr->InitPos(0, 0, rect.right - rect.left, rect.bottom - rect.top);
		}
	}
}

// CUIToolApp �޽��� ó����


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CUIToolApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUIToolApp ����� ���� �ε�/���� �޼���

void CUIToolApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CUIToolApp::LoadCustomState()
{
}

void CUIToolApp::SaveCustomState()
{
}

// CUIToolApp �޽��� ó����

void CUIToolApp::OnFileOpen()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	CFileDialog dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString strFilter;
	strFilter += "UI Files(*.xml)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += "*.xml";
	strFilter += (TCHAR)'\0';   // last string

	dlgFile.m_ofn.lpstrTitle = "UI ���� ����";
	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrInitialDir = "";

	if( dlgFile.DoModal() == IDOK )
	{
		 CString strPath = dlgFile.GetPathName();
		 m_strLoadFileName = dlgFile.GetFileTitle();	// ���� �̸� ����.
		 
		 if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
			 pMain->SetPath(strPath.GetString());

		 CmdOpenFile* pCmd = new CmdOpenFile;
		 pCmd->setData((LPSTR)(LPCSTR)strPath);
		 addQueue(pCmd);
	}	
}


void CUIToolApp::OnRefresh()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.
	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{
		pMain->Refresh();

		if( CChildFrame *pChild = (CChildFrame *)pMain->GetActiveFrame() )
		{
			if( CUIToolView* pView = (CUIToolView*)pChild->GetActiveView() )
			{
				pView->Invalidate();
				pView->UpdateWindow();
			}
		}
	}
}


void CUIToolApp::OnFileSave()
{
}

void CUIToolApp::Draw()
{
	if( m_pDrawPort )
	{
		m_pDrawPort->SetAsCurrent();

		m_pDrawPort->Fill( 0x7F7F7FFF | CT_OPAQUE);
		m_pDrawPort->FillZBuffer(ZBUF_BACK);


		m_pDrawPort->SetOrtho();
		shaBlendFunc(GFX_SRC_ALPHA, GFX_INV_SRC_ALPHA);

		if( m_pUIMgr )
			m_pUIMgr->Render( m_pDrawPort );

		if( m_pViewPort != NULL )
			m_pViewPort->SwapBuffers();
	}
}

void CUIToolApp::Update()
{
	// Update
	if (_pTimer != NULL)
	{
		ULONG	cur = (ULONG)_pTimer->GetHighPrecisionTimer().GetMilliseconds();
		float	delta = (float)(cur - m_oldTime) * 0.001f;

		if (m_pUIMgr != NULL)
			m_pUIMgr->Update(delta, cur);

		m_oldTime = cur;
	}

	{
		CTSingleLock sl(&m_csLock, TRUE);

		while (m_queue.size())
		{
			Command* pCmd = m_queue.front();
			if (pCmd != NULL)
				pCmd->execute();

			SAFE_DELETE(pCmd);

			m_queue.pop();
		}
	}
}

void CUIToolApp::addQueue( Command* pCmd )
{
	CTSingleLock sl(&m_csLock, TRUE);

	m_queue.push(pCmd);
}


