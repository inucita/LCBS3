
#include "stdafx.h"
#include "MainFrm.h"
#include "UITool.h"
#include "ViewTree.h"
#include <Engine/Interface/UIManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_AUTOEXPAND		(1000)		// ���� �ͽ��ҵ带 ���� Ÿ�̸� ID ���� �ð�.
#define ID_TIMER_AUTOSCROLL		(50)		// ���� ��ũ���� ���� Ÿ�̸� ID ���� �ð�.

class CmdCreateControl : public Command
{
public:
	void setData(const char* str_control)	{ str_control_ = str_control; }
	void execute()
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		pMain->AddUIControl(str_control_.c_str());
	}

private:
	std::string		str_control_;
};

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
	: m_bDblclkLoop(false)
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CViewTree::OnTvnSelchanged)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree �޽��� ó����

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}
	
	return bRes;
}


void CViewTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	HTREEITEM hItem = GetSelectedItem();
	
	CUIBase* pUI = (CUIBase*)GetItemData(hItem);
	

	if( pUI )
	{
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		pMain->SelectProperty(pUI, pUI->getType());
	}
	else
	{
		// ���â��
		OutputDebugString(_T("�ش� UI�� ã�� ���߽��ϴ�.\n"));
	}
}


void CViewTree::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if( m_bDblclkLoop == false )
		return;

	HTREEITEM hItem = GetSelectedItem();
	CString str = GetItemText(hItem);

	if( str.IsEmpty() == FALSE )
	{
		CmdCreateControl* pCmd = new CmdCreateControl;
		pCmd->setData((LPSTR)(LPCSTR)str);
		theApp.addQueue(pCmd);

	}
	CTreeCtrl::OnLButtonDblClk(nFlags, point);
}


void CViewTree::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CViewTree::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

// �巡�� ó���� ���� mfc �Լ�.
void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	HTREEITEM hSelectItem = pNMTreeView->itemNew.hItem;	// ���� ���õ� ������.
	CPoint	  pt = pNMTreeView->ptDrag;

	CUIBase* pSelectUI = NULL;
	CUIBase* pDropUI   = NULL;
	if( hSelectItem )
		pSelectUI = (CUIBase*)GetItemData(hSelectItem);	// DWORD_PTR�� ��Ʈ���� �����͸� �Ѱ������.

	HTREEITEM hDropItem = DragItem(hSelectItem, pt);	// ���ϰ����� ���Ÿ���� �ڵ��� ����. �巡���� �������� �ڵ��� �ѱ�.
	if( hDropItem )
		pDropUI = (CUIBase*)GetItemData(hDropItem);

	if( pSelectUI != NULL && pDropUI != NULL )
	{
		if( pSelectUI->getParent() != pDropUI && pSelectUI != pDropUI )
			pSelectUI->getParent()->VecMove(pSelectUI, pDropUI);

		CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd();
		pMain->GetFileView()->FillFileView();
	}
}

HTREEITEM CViewTree::DragItem( HTREEITEM hDragItem, CPoint pt )
{
	if( hDragItem == NULL || GetCapture() != NULL)	// �ٸ� �����찡 ���콺 ĸ�����̸� ����.
		return NULL;

	SetCapture();
	Invalidate(FALSE);

	CImageList* pDragImage = CreateDragImage(hDragItem);	// �巡�� �̹��� ����.
	if(pDragImage == NULL)
		return NULL;

	CRect	rc;
	GetItemRect(hDragItem, &rc, TRUE);
	rc.left	-= GetIndent();
	pDragImage->BeginDrag(0, pt - rc.TopLeft());
	pDragImage->DragEnter(this, pt);			// �巡�� �̹����� ���콺 ��ġ�� �׸�.

	SelectDropTarget(NULL);
	HTREEITEM hDropTargetItem = NULL;
	MSG	msg;
	
	bool bBreak = FALSE;
	while( GetMessage(&msg, NULL, 0, 0) && !bBreak )	// �巡�� �ϴ� ���� ������ �޽��� ó��.
	{
		if( GetCapture() != this )	// ĸ��� �����찡 �ٸ��ٸ� �巡�׸� ������.
			break;

		switch( msg.message )
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			{
				bBreak = true;	// ���⿡ �ɸ� ��� while���� ���� ������ �ϱ� ������.
			}
			break;
		case WM_MOUSEMOVE:
			{
				pt.x = (short)LOWORD(msg.lParam);
				pt.y = (short)HIWORD(msg.lParam);

				hDropTargetItem = HitTest(pt);	// ���Ÿ�� �������� ����.

				if( hDropTargetItem == hDragItem && hDropTargetItem == GetParentItem(hDragItem))
					hDropTargetItem = NULL;
				else
				{
					HTREEITEM hItem = hDropTargetItem;
					while( (hItem = GetParentItem(hItem)) != NULL )
					{
						if( hItem == hDragItem )
						{
							hDropTargetItem = NULL;
							break;
						}
					}
				}

				if( hDropTargetItem != GetDropHilightItem() )
				{
					SetCursor(AfxGetApp()->LoadStandardCursor((hDropTargetItem == NULL) ? IDC_NO : IDC_ARROW ));
					pDragImage->DragShowNolock(FALSE);
					SelectDropTarget(hDropTargetItem);
					pDragImage->DragShowNolock(TRUE);

					SetTimer(ID_TIMER_AUTOEXPAND, ID_TIMER_AUTOEXPAND, NULL);	// ���� �ͽ���带 ���� Ÿ�̸� ( �ڵ� ��ġ�� ��� )
				}
				GetClientRect(&rc);
				if( ((pt.x >= rc.left) && (pt.x <= rc.right))
					&& ((pt.y < rc.top + 10) || (pt.y > rc.bottom)) )
				{
					SetTimer(ID_TIMER_AUTOSCROLL, ID_TIMER_AUTOSCROLL, NULL);	// ���� ��ũ���� ���� Ÿ�̸�.
				}

				pDragImage->DragMove(pt);
			}
			break;

		case WM_TIMER:
			{
				if( msg.wParam == ID_TIMER_AUTOEXPAND )
				{
					hDropTargetItem = GetDropHilightItem();
					if( hDropTargetItem != NULL )
					{
						pDragImage->DragShowNolock(FALSE);
						Expand(hDropTargetItem, TVE_EXPAND);
						pDragImage->DragShowNolock(TRUE);
					}
					KillTimer(ID_TIMER_AUTOEXPAND);
				}
				else if( msg.wParam == ID_TIMER_AUTOSCROLL )
				{
					GetCursorPos(&pt);
					ScreenToClient(&pt);

					GetClientRect(&rc);
					BOOL bTestX = (pt.x >= rc.left) && (pt.x <= rc.right);
					if( bTestX && (pt.y < rc.top) )
						SendMessage(WM_VSCROLL, SB_LINEUP);
					else if(bTestX && (pt.y > rc.bottom))
						SendMessage(WM_VSCROLL, SB_LINEDOWN);
					else
						KillTimer(ID_TIMER_AUTOSCROLL);
				}
			}
			break;
		case WM_KEYDOWN:
			{
				switch(msg.wParam)
				{
				case VK_ESCAPE:
					{
						SelectDropTarget(NULL);	// �巡���� escŰ�� ���ȴٸ� �巡�� ����.
						bBreak = true;
					}
					break;
				}
			}
			break;
		default:	// �� msg �̿ܿ� ���� ����. �������� ���ϼ���.
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			break;
		}
	}

	KillTimer(ID_TIMER_AUTOSCROLL);
	KillTimer(ID_TIMER_AUTOEXPAND);

	pDragImage->DragLeave(this);
	pDragImage->EndDrag();
	delete pDragImage;

	Invalidate(FALSE);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	hDropTargetItem = GetDropHilightItem();
	SelectDropTarget(NULL);

	return hDropTargetItem;
}
