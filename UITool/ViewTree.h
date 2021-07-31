
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree â�Դϴ�.
class CUIBase;

class CViewTree : public CTreeCtrl
{
// �����Դϴ�.
public:
	CViewTree();
	void	setDblClkLoop(bool b) { m_bDblclkLoop = b; }
// �������Դϴ�.
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	HTREEITEM	DragItem(HTREEITEM hDragItem, CPoint pt);

// �����Դϴ�.
public:
	virtual ~CViewTree();

protected:
	DECLARE_MESSAGE_MAP()

private:
	bool m_bDblclkLoop;
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
};
