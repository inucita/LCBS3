#if !defined(AFX_DLGSTRETCHCHILDOFFSET_H__55BF42F3_BE28_11D5_8763_00002103143B__INCLUDED_)
#define AFX_DLGSTRETCHCHILDOFFSET_H__55BF42F3_BE28_11D5_8763_00002103143B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStretchChildOffset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStretchChildOffset dialog

class CDlgStretchChildOffset : public CDialog
{
// Construction
public:
	CDlgStretchChildOffset(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStretchChildOffset)
	enum { IDD = IDD_STRETCH_CHILD_OFFSET };
	float	m_fStretchValue;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStretchChildOffset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStretchChildOffset)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTRETCHCHILDOFFSET_H__55BF42F3_BE28_11D5_8763_00002103143B__INCLUDED_)
