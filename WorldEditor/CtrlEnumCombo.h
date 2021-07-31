// CtrlEnumCombo.h : header file
//
#ifndef CTRLENUMCOMBO_H
#define CTRLENUMCOMBO_H 1

class CPropertyComboBar;
/////////////////////////////////////////////////////////////////////////////
// CCtrlEnumCombo window

class CCtrlEnumCombo : public CComboBox
{
// Construction
public:
	CCtrlEnumCombo();

// Attributes
public:
  // ptr to parent dialog
  CPropertyComboBar *m_pDialog;

// Operations
public:
  // sets ptr to parent dialog
  void SetDialogPtr( CPropertyComboBar *pDialog);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCtrlEnumCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCtrlEnumCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCtrlEnumCombo)
	afx_msg void OnSelchange();
	afx_msg void OnDropdown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif  // CTRLENUMCOMBO_H
