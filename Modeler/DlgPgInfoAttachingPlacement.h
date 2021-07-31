#if !defined(AFX_DLGPGINFOATTACHINGPLACEMENT_H__01700903_25FE_11D2_8361_004095812ACC__INCLUDED_)
#define AFX_DLGPGINFOATTACHINGPLACEMENT_H__01700903_25FE_11D2_8361_004095812ACC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPgInfoAttachingPlacement.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPgInfoAttachingPlacement dialog

class CDlgPgInfoAttachingPlacement : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgPgInfoAttachingPlacement)

// Construction
public:
	CUpdateable m_udAllValues;
  INDEX m_iActivePlacement;
	CDlgPgInfoAttachingPlacement();
	~CDlgPgInfoAttachingPlacement();

  BOOL BrowseAttachement( CAttachedModel *pam);
  void SetPlacementReferenceVertex(INDEX iCenter, INDEX iFront, INDEX iUp);
  INDEX GetCurrentAttachingPlacement(void);
  BOOL BrowseModelAndTexture(CTFileName &fnModel, CTFileName &fnTexture);
  void FillAttachmentModelAnimationCombo(void);
  BOOL OnIdle(LONG lCount);

// Dialog Data
	//{{AFX_DATA(CDlgPgInfoAttachingPlacement)
	enum { IDD = IDD_INFO_ATTACHING_PLACEMENT };
	CComboBox	m_comboAttachmentModelAnimation;
	float	m_fBanking;
	float	m_fHeading; 
	float	m_fPitch;
	float	m_fXOffset;
	float	m_fYOffset;
	float	m_fZOffset;
	CString	m_strName;
	CString	m_strAttachingModel;
	CString	m_strAttachingVertices;
	CString	m_strPlacementIndex;
	BOOL	m_bIsVisible;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgPgInfoAttachingPlacement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgPgInfoAttachingPlacement)
	afx_msg void OnAddAttachingPlacement();
	afx_msg void OnChangeAttachingPlacementBanking();
	afx_msg void OnChangeAttachingPlacementHeading();
	afx_msg void OnChangeAttachingPlacementName();
	afx_msg void OnChangeAttachingPlacementPitch();
	afx_msg void OnChangeAttachingPlacementXOffset();
	afx_msg void OnChangeAttachingPlacementYOffset();
	afx_msg void OnChangeAttachingPlacementZOffset();
	afx_msg void OnBrowseModel();
	afx_msg void OnNextAttachingPlacement();
	afx_msg void OnPreviousAttachingPlacement();
	afx_msg void OnRemoveAttachingPlacement();
	afx_msg void OnIsVisible();
	afx_msg void OnSelchangeAttachmentModelAnimationCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPGINFOATTACHINGPLACEMENT_H__01700903_25FE_11D2_8361_004095812ACC__INCLUDED_)
