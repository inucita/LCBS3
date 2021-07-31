#if !defined(AFX_DLGPGINFOATTACHINGSOUND_H__5B34DD66_A487_11D2_849B_004095812ACC__INCLUDED_)
#define AFX_DLGPGINFOATTACHINGSOUND_H__5B34DD66_A487_11D2_849B_004095812ACC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPgInfoAttachingSound.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPgInfoAttachingSound dialog

class CDlgPgInfoAttachingSound : public CPropertyPage
{
	DECLARE_DYNCREATE(CDlgPgInfoAttachingSound)

// Construction
public:
	CUpdateable m_udAllValues;

  BOOL OnIdle(LONG lCount);

  CDlgPgInfoAttachingSound();
	~CDlgPgInfoAttachingSound();

// Dialog Data
	//{{AFX_DATA(CDlgPgInfoAttachingSound)
	enum { IDD = IDD_INFO_ATTACHING_SOUND };
	CString	m_strAttachedSound;
	BOOL	m_bLooping;
	BOOL	m_bPlaying;
	float	m_fDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CDlgPgInfoAttachingSound)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgPgInfoAttachingSound)
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowseSound();
	afx_msg void OnIsLooping();
	afx_msg void OnIsPlaying();
	afx_msg void OnAttachingSoundNone();
	afx_msg void OnChangeSoundStartDelay();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPGINFOATTACHINGSOUND_H__5B34DD66_A487_11D2_849B_004095812ACC__INCLUDED_)
