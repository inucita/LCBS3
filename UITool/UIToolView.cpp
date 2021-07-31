
// UIToolView.cpp : CUIToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "UITool.h"
#endif

#include "UIToolDoc.h"
#include "UIToolView.h"
#include "MainFrm.h"

#include <Engine/Interface/UIManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUIToolView

IMPLEMENT_DYNCREATE(CUIToolView, CView)

BEGIN_MESSAGE_MAP(CUIToolView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CUIToolView ����/�Ҹ�

CUIToolView::CUIToolView()
	: m_pUIMgr(NULL)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CUIToolView::~CUIToolView()
{
}

BOOL CUIToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.
	return CView::PreCreateWindow(cs);
}

// CUIToolView �׸���

void CUIToolView::OnDraw(CDC* /*pDC*/)
{
	CUIToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
}

void CUIToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	theApp.initDraw(m_hWnd);
	 
	m_pUIMgr = CUIManager::getSingleton();
}

void CUIToolView::OnDestroy()
{
	CView::OnDestroy();
}

void CUIToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUIToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUIToolView ����

#ifdef _DEBUG
void CUIToolView::AssertValid() const
{
	CView::AssertValid();
}

void CUIToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CUIToolDoc* CUIToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUIToolDoc)));
	return (CUIToolDoc*)m_pDocument;
}

#endif //_DEBUG


// CUIToolView �޽��� ó����


LRESULT CUIToolView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	
	static bool bCtrl = false;
	static bool bDrag = false;
	static bool bDraging = false;
	static POINT Oldpt;

	POINT	pt;

	pt.x = (short)LOWORD( lParam );
	pt.y = (short)HIWORD( lParam );

	DWORD dwPram = MAKELPARAM(pt.x, pt.y);

	int nX = (short)LOWORD( dwPram );
	int nY = (short)HIWORD( dwPram );

	switch(message)
	{
	case WM_MOUSEMOVE:
		{
			if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
			{
				pMain->SetMousePosition(dwPram);
				
				if (( GetKeyState(VK_SHIFT) & 0x8000 ) && bDrag == true)
				{
					CUIBase* pControl = pMain->GetSelectControl();
	
					if (pControl != NULL)
					{
						int x, y;
						x = nX - Oldpt.x;
						y = nY - Oldpt.y;

						pControl->SetPos(pControl->GetOrigX() + x, pControl->GetOrigY() + y);
						pControl->updatePosition(true);
						pMain->GetPropWnd()->UpdatePos( pControl->GetOrigX(), pControl->GetOrigY());

						bDraging = true;
					}
				}
				else
				{
					bDraging = false;
				}
			}
			
			Oldpt.x = nX;
			Oldpt.y = nY;

			bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );
		}
		break;
	case WM_LBUTTONDOWN:
		{
			bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );

			if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
			{
				if (pMain->GetSelectControl() && pMain->GetSelectControl()->IsInside(nX, nY))
					bDrag = true;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			if (bDraging == false)
			{
				if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
				{
					if (pMain->GetFileView() != NULL)
					{
						if (pUIManager != NULL)
						{	// RFindControl�� ���ϵ���� �˻��ϱ� ������ UIManager�� �־��ش�.
							pMain->GetFileView()->RFindControl(bCtrl, (CUIBase*)pUIManager, nX, nY);
						}
					}
				}
			}
			
			bDrag = false;
		}
		break;
	case WM_KEYDOWN:
		{
			bCtrl = ( GetKeyState(VK_CONTROL) & 0x8000 );

			if (CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd())
			{
				CUIBase* pControl = pMain->GetSelectControl();

				if (pControl != NULL)
				{
					if (GetKeyState('W') & 0x8000 || GetKeyState('S') & 0x8000 || 
						GetKeyState('A') & 0x8000 || GetKeyState('D') & 0x8000)
					{
						int nX = 0, nY = 0;

						if (GetKeyState('W') & 0x8000)
							nY -= 1;
						if (GetKeyState('S') & 0x8000)
							nY += 1;
						if (GetKeyState('A') & 0x8000)
							nX -= 1;
						if (GetKeyState('D') & 0x8000)
							nX += 1;

						pControl->SetPos(pControl->GetOrigX() + nX, pControl->GetOrigY() + nY);
						pControl->updatePosition(true);
						pMain->GetPropWnd()->UpdatePos( pControl->GetOrigX(), pControl->GetOrigY());
					}
				}
			}
		}
		break;
	case WM_KEYUP:
		{
			bCtrl = FALSE;
		}
		break;
	}

	return CView::DefWindowProc(message, wParam, lParam);
}


BOOL CUIToolView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (m_pUIMgr)
	{
		m_pUIMgr->MsgSimpleProc(pMsg);
	}
	return CView::PreTranslateMessage(pMsg);
}


void CUIToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);	

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (m_pUIMgr)
	{
		_pGfx->ResetDisplayMode();

		m_pUIMgr->SetSize(cx, cy, true);
	}
}
