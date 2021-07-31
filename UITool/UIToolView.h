
// UIToolView.h : CUIToolView Ŭ������ �������̽�
//

#pragma once


class CDrawPort;
class CViewPort;
class CUIManager;


class CUIToolView : public CView
{
protected: // serialization������ ��������ϴ�.
	CUIToolView();
	DECLARE_DYNCREATE(CUIToolView)

// Ư���Դϴ�.
public:
	CUIToolDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual void OnInitialUpdate();
	virtual void OnDestroy();
protected:

// �����Դϴ�.
public:
	virtual ~CUIToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// ������ �޽��� �� �Լ�
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	CUIManager*		m_pUIMgr;
};

#ifndef _DEBUG  // UIToolView.cpp�� ����� ����
inline CUIToolDoc* CUIToolView::GetDocument() const
   { return reinterpret_cast<CUIToolDoc*>(m_pDocument); }
#endif

