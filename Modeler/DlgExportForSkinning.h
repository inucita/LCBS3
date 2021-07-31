#if !defined(AFX_DLGEXPORTFORSKINNING_H__74008183_5EEE_11D4_84EB_000021291DC7__INCLUDED_)
#define AFX_DLGEXPORTFORSKINNING_H__74008183_5EEE_11D4_84EB_000021291DC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgExportForSkinning.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExportForSkinning dialog

class CDlgExportForSkinning : public CDialog
{
// Construction
public:
  INDEX m_iTextureWidth;

	CDlgExportForSkinning(CTFileName fnExportFile, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExportForSkinning)
	enum { IDD = IDD_EXPORT_FOR_SKINNIG };
	CColoredButton	m_ctrlWireColor;
	CColoredButton	m_ctrlPaperColor;
	CComboBox	m_ctrlExportPictureSize;
	BOOL	m_bColoredSurfaces;
	BOOL	m_bSurfaceNumbers;
	BOOL	m_bWireFrame;
	CString	m_strExportedFileName;
	CString	m_strSurfaceListFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExportForSkinning)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgExportForSkinning)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXPORTFORSKINNING_H__74008183_5EEE_11D4_84EB_000021291DC7__INCLUDED_)
