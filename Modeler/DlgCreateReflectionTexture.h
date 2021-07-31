#if !defined(AFX_DLGCREATEREFLECTIONTEXTURE_H__AE69A18C_E288_11D2_8535_004095812ACC__INCLUDED_)
#define AFX_DLGCREATEREFLECTIONTEXTURE_H__AE69A18C_E288_11D2_8535_004095812ACC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCreateReflectionTexture.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCreateReflectionTexture dialog

class CDlgCreateReflectionTexture : public CDialog
{
// Construction
public:
  BOOL m_bCustomWindowsCreated;
  
  CTString m_strBase;
  CTString m_strExt;
  CPlacement3D m_plPlacement;
  CModelObject m_moModel;
  CTextureObject m_toBackground;

  int m_iTimerID;

  CWnd m_wndPreview;
  CWnd m_wndN;
  CWnd m_wndS;
  CWnd m_wndE;
  CWnd m_wndW;
  CWnd m_wndC;
  CWnd m_wndF;
  CDrawPort *m_apdp[7];
  CViewPort *m_apvp[7];

  CTextureObject m_toN;
  CTextureObject m_toS;
  CTextureObject m_toE;
  CTextureObject m_toW;
  CTextureObject m_toC;
  CTextureObject m_toF;

	CDlgCreateReflectionTexture(CWnd* pParent = NULL);   // standard constructor
  void CreateReflectionTexture_t( CTFileName fnTexture);  // throw char *
  void PutPicture(CWnd &wnd, CTextureObject &to, INDEX iwin);
  void DrawPreview( CDrawPort *pdp);
  void RenderPreview(void);
  void AutoSetTextures( CTFileName fnFile);

// Dialog Data
	//{{AFX_DATA(CDlgCreateReflectionTexture)
	enum { IDD = IDD_CREATE_REFLECTION_TEXTURE };
	CComboBox	m_comboSizeInPixels;
	CColoredButton	m_colorLight;
	CColoredButton	m_colorAmbient;
	CColoredButton	m_colorReflection;
	BOOL	m_bAutoRotate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCreateReflectionTexture)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCreateReflectionTexture)
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnAutoRotate();
	afx_msg void OnSelchangeSizeInPixels();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCREATEREFLECTIONTEXTURE_H__AE69A18C_E288_11D2_8535_004095812ACC__INCLUDED_)
