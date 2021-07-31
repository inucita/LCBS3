
// PatchParserToolDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include <vector>

// CPatchParserToolDlg ��ȭ ����
class CPatchParserToolDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CPatchParserToolDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PATCHPARSERTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);		// limit editbox���� enterkey ����


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	std::map<CString, CString> urlMap;
	CString url;
	CString filename;
	int limit;
	BOOL bMakeXml;

	std::vector<int> selectedRev;	// ���õ� revision
	std::vector<int> loadedRev;		// Ž���� revision list

	CListBox NationList;
	CListBox RevisionList;
	CEdit LogBox;

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList2();

	void loadRecent();
	void refreshLog();
};
