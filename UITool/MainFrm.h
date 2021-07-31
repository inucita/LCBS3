
// MainFrm.h : CMainFrame Ŭ������ �������̽�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CUIBase;

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:
	BOOL OnLoadFile( const char* strPath );
	BOOL OnSaveFile( const char* strPath );
	void Refresh();
	//CPropertiesWnd* getProperties()  { return &m_wndProperties; }

	void SelectProperty(CUIBase* pUI, DWORD dwFlag);
	void AddUIControl(CString strControl);
	void Message(const char* message, ...);

	CUIBase* GetSelectControl()		{ return m_pOldSelect; }
	void	 ResetSelectUI( )		{ m_pOldSelect = NULL; }
	CFileView* GetFileView()		{ return &m_wndFileView; }
	CPropertiesWnd* GetPropWnd()	{ return &m_wndProperties;	}

	void SetMousePosition(DWORD pos);
	void udpateRect();
	void SetPath(std::string strPath)	{ m_strPath = strPath;	}
	std::string GetPath()	{ return m_strPath;	}

private:
	BOOL initData();

protected:

	std::string m_strPath;

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// �����Դϴ�.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // ��Ʈ�� ������ ���Ե� ����Դϴ�.
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CUIBase*		  m_pOldSelect;

// ������ �޽��� �� �Լ�
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
};


