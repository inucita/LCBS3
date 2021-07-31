
#include "StdH.h"
#include "UITree.h"
#include <Engine/Interface/UIImage.h>
#include <Engine/Interface/UICheckButton.h>
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIFocus.h>


CUITreeItem::CUITreeItem()
{
	setType(eUI_CONTROL_TREEITEM);
}

void CUITreeItem::Render( CDrawPort* pDraw )
{
	if (m_pParent != NULL &&
		m_pParent->getType() == eUI_CONTROL_TREE)
	{
		if (m_pParent->GetHide() == TRUE)
			return;

		CUIBase* ppParent = m_pParent->getParent();

		if (ppParent != NULL && ppParent->getType() == eUI_CONTROL_TREE)
		{
			CUITree* pPTree = static_cast< CUITree* >(ppParent);
			
			if (pPTree->getRoot() == false && pPTree->getUncollapse() == FALSE)
				return;
		}
	}

	CUIBase::Render(pDraw);
}

CUIBase* CUITreeItem::Clone()
{
	CUITreeItem* pUI = new CUITreeItem(*this);

	CUIBase::CloneChild(pUI);

	return pUI;
}

WMSG_RESULT CUITreeItem::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (CheckChildProc() == false)
		return WMSG_FAIL;

	return CUIBase::OnLButtonUp(x, y);
}

WMSG_RESULT CUITreeItem::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (CheckChildProc() == false)
		return WMSG_FAIL;

	return CUIBase::OnLButtonDown(x, y);
}

bool CUITreeItem::CheckChildProc()
{
	if (m_pParent != NULL &&
		m_pParent->getType() == eUI_CONTROL_TREE)
	{
		if (m_pParent->GetHide() == TRUE)
			return false;

		CUIBase* ppParent = m_pParent->getParent();

		if (ppParent != NULL && ppParent->getType() == eUI_CONTROL_TREE)
		{
			CUITree* pPTree = static_cast< CUITree* >(ppParent);

			if (pPTree->getRoot() == false && pPTree->getUncollapse() == FALSE)
				return false;
		}
	}

	return true;
}

// --------------------------------------------------------------

class CmdUncollapse : public Command
{
public:
	CmdUncollapse() : _pTree(NULL) {}

	void setData(CUITree* pTree)	{ _pTree = pTree; }
	void execute()
	{
		if (_pTree != NULL)
		{
			_pTree->updateCollapse();
		}
	}

private:
	CUITree*	_pTree;
};

class CmdOnTreeScroll : public Command
{
public:
	CmdOnTreeScroll() : tree_(NULL) {}

	void setData(CUITree* pTree)	{ tree_ = pTree; }
	void execute()
	{
		if (tree_)
		{
			tree_->UpdateScroll();
		}
	}

private:
	CUITree* tree_;
};

// -----------------------------------------------------------------

CUITree::CUITree()
	: m_pScroll(NULL)
	, m_pCheck(NULL)
	, m_nCount(0)
	, m_bPre(false)
	, m_nGap(0)
	, m_nSumHeight(0)
	, m_bRoot(false)
	, m_bAddCheck(false)
	, m_nShow(0)
{
	setType(eUI_CONTROL_TREE);
}

CUITree::~CUITree()
{
	Destroy();
	
	if (m_bAddCheck == false)
		SAFE_DELETE(m_pCheck);
}

void CUITree::addTree( CUITree* pTree, bool bAddBtn )
{
	if (pTree == NULL)
		return;

	// �⺻ ���̵� ����
	char buf[64];
	int		idx = m_vecTree.size();
	sprintf(buf, "tree_item_%03d", idx + 1);
	pTree->setID(buf);
	pTree->setControlIndex(idx);

	m_vecTree.push_back(pTree);
	addChild(pTree);

	CUICheckButton* pCheck = pTree->getCheck();
	if (bAddBtn == true && pCheck != NULL)
	{
		{
			int x, y;
			pCheck->GetPos(x, y);
		
			int h = getChildMaxHeight();

			h -= pCheck->GetHeight();

			if (h > 0)
			{
				y = y + (h / 2);
				pCheck->SetPos(x, y);
			}
		}
		
		pTree->addChild(pCheck);
		pTree->setAddCheck(true);
		CmdUncollapse* pCmd = new CmdUncollapse;
		pCmd->setData(pTree);
		pCheck->SetCommand(pCmd);
	}
}

void CUITree::updateTree()
{
	if (m_pParent == NULL)
	{
		onUpdateTree(m_pScroll);
		return;
	}

	if (m_pParent->getType() == eUI_CONTROL_TREE)
	{
		static_cast< CUITree* >(m_pParent)->updateTree();
	}
	else
	{
		onUpdateTree(m_pScroll);
	}
}

void CUITree::onUpdateTree(CUIScrollBar* pScroll)
{
	int		i, nMax;
	CUITree* pItem;

	int		x = 0;
	int		accumY = 0;
	int		nStart = 0;
	int		startY = getParentoffset();
	int		offY = getChildMaxHeight();
	int		nScrollStart = 0;
	int		nScrollEnd = 0;

	if (pScroll != NULL)
	{
		nScrollStart = pScroll->GetScrollPos();
		nScrollEnd = nScrollStart + pScroll->GetItemsPerPage();
	}

	nMax = m_vecTree.size();

	for (i = 0; i < nMax; ++i)
	{
		pItem = (CUITree*)m_vecTree[i];

		if (pItem == NULL)
			continue;

		int nIdx = pItem->getTreeIndex();

		// Scroll ������ ���Դ��� �˻�
		// �θ��� ���� ��ġ�� ����ؾ� �ϹǷ� Hide �� �ǳʶٸ� �ȵȴ�.		
		if (pScroll != NULL &&
			(nIdx < nScrollStart || nIdx >= nScrollEnd))
		{
			pItem->Hide(TRUE);
			
			if (pItem->getCheck() != NULL)
				pItem->getCheck()->Hide(TRUE);
		}
		else
		{
			pItem->Hide(FALSE);

			if (pItem->getCheck() != NULL)
				pItem->getCheck()->Hide(FALSE);
		}

		//if (GetHide() == FALSE && getRoot() == false)
		if (getRoot() == false)
			accumY = (i + 1) * (pItem->getTreeItemHeight() + m_nGap);
		else
			accumY = (nIdx - nScrollStart) * (pItem->getTreeItemHeight() + m_nGap);

		x = pItem->GetPosX();
		pItem->SetPos(x, accumY);
	}

	m_nSumHeight = accumY;

	if (m_bRoot == false)
	{
		if (getUncollapse() == FALSE)
		{
			SetHeight(offY);
		}
		else
		{
			SetHeight(offY + m_nSumHeight);
		}
	}

	updateTreeChild(pScroll);
}

void CUITree::updateTreeChild(CUIScrollBar* pScroll)
{
	if (m_pParent == NULL)
		return;

	vec_uinode_iter iter = m_vecTree.begin();
	vec_uinode_iter eiter = m_vecTree.end();

	for (; iter != eiter; ++iter)
	{
		CUITree* pTree = static_cast< CUITree* >(*iter);

		pTree->onUpdateTree(pScroll);
	}
}

void CUITree::updateCollapse()
{
	BOOL bUncollapse = getUncollapse();

	vec_uinode_iter iter = m_vecTree.begin();
	vec_uinode_iter eiter = m_vecTree.end();

	for (; iter != eiter; ++iter)
	{
		CUITree* pTree = (CUITree*)(*iter);		

		if (bUncollapse == FALSE)
			pTree->Hide(TRUE);
		else
			pTree->Hide(FALSE);
	}

	UpdateScroll();
	updateTree();
}

void CUITree::setUncollapse( BOOL bUncollapse )
{
	if (m_pCheck != NULL)
	{
		m_pCheck->SetCheck(bUncollapse);

		updateCollapse();
	}
}

BOOL CUITree::getUncollapse()
{
	if (m_pCheck != NULL)
	{
		return m_pCheck->IsChecked();
	}

	return FALSE;
}

CUITree* CUITree::getChildTree( int nIdx )
{
	if (nIdx < 0 || nIdx >= m_vecTree.size())
		return NULL;

	return (CUITree*)m_vecTree[nIdx];
}

int CUITree::getChildMaxHeight()
{
	int		max = 0, h;

	vec_uinode_iter iter = m_VecChild.begin();
	vec_uinode_iter eiter = m_VecChild.end();

	for (; iter != eiter; ++iter)
	{
		if ((*iter)->getType() != eUI_CONTROL_TREE)
		{		
			h = (*iter)->GetHeight();
			
			if (h > max)
				max = h;
		}		
	}

	return max;
}
// ------------------------------------------------------------------

CUIBase* CUITree::Clone()
{
	CUITree* pTree = new CUITree(*this);

	CUIBase* pCheck = getCheck();

	if (pCheck != NULL)
		pTree->setCheck((CUICheckButton*)pCheck->Clone());	

	// tree �� ��ͷ� ����� �� �����Ƿ� m_vecTree ���� ����.
	pTree->clearChildList();
	pTree->setScroll(NULL);

	CUIBase::CloneChild(pTree);

	return pTree;
}

void CUITree::clearChildList()
{
	// clone �� �޸� ���� ���ϴ� ��Ȳ���� ���
	m_vecTree.clear();
}

void CUITree::setScroll( CUIScrollBar* pScroll )
{
	m_pScroll = pScroll;

	if (m_pScroll != NULL)
	{
		CmdOnTreeScroll* pCmd = new CmdOnTreeScroll;
		pCmd->setData(this);
		m_pScroll->SetCommand(pCmd);
	}
}

void CUITree::UpdateScroll()
{
	if (m_pParent->getType() == eUI_CONTROL_TREE)
	{
		static_cast< CUITree* >(m_pParent)->UpdateScroll();
	}
	else
	{
		onUpdateScroll();
	}
}

void CUITree::onUpdateScroll()
{
	if (m_pScroll == NULL)
		return;

	int nMax = getShowTreeCount();

	if (nMax != m_pScroll->GetCurItemCount())
	{
		m_pScroll->SetCurItemCount(nMax);		
	}

	updateTree();
}

int CUITree::getShowTreeCount()
{
	int nCnt = 0;

	if (getRoot() == false && getUncollapse() == FALSE)
		return nCnt;

	vec_uinode_iter iter = m_VecChild.begin();
	vec_uinode_iter eiter = m_VecChild.end();

	for( ; iter != eiter; ++iter)
	{
		if ((*iter)->getType() == eUI_CONTROL_TREE)
		{
			CUITree* pTree = static_cast< CUITree* >(*iter);

			++nCnt;

			if (pTree->getUncollapse() == FALSE)
				continue;

			int nChild = pTree->getShowTreeCount();

			nCnt += nChild;
		}
	}

	return nCnt;
}

int CUITree::getTreeIndex()
{
	// Tree �� Root Tree �� �����Ƿ�, �θ� ���ٸ� 0
	if (m_pParent == NULL)
		return 0;

	int nRet = 0;

	if (m_pParent->getType() == eUI_CONTROL_TREE)
	{
		CUITree* pPTree = static_cast< CUITree* >(m_pParent);

		// �θ��� �ε����� ã�´�.
		nRet += pPTree->getTreeIndex();

		if (pPTree->getRoot() == false)
			++nRet;

		int i, nChild = pPTree->getChildTreeCount();

		for (i = 0; i < nChild; ++i)
		{
			CUIBase* pUI = pPTree->getChildTree(i);

			if (pUI->getType() == eUI_CONTROL_TREE)
			{
				// ���� ��ġ
				if (pUI == this)
					return (nRet + i);
				else
					nRet += static_cast< CUITree* >(pUI)->getShowTreeCount();
			}
		}
	}

	return nRet;
}

void CUITree::Render( CDrawPort* pDraw )
{
	// tree �� ��� child scroll ������ Hide �˻� �ǳ� ��
	RenderChild(pDraw);

#ifdef UI_TOOL
	RenderBorder(pDraw);
#endif // UI_TOOL
}

int CUITree::getParentoffset()
{
	int		nPos = 0;

	if (m_pParent != NULL)
	{
		if (m_pParent->getType() == eUI_CONTROL_TREE)
		{
			CUITree* pParent = static_cast< CUITree* >(m_pParent);

			nPos += pParent->getParentoffset();
			
			// ��Ʈ�� ��ü �����̱� ������ �����Ѵ�.
			if (pParent->getRoot() == false)
				nPos += pParent->GetHeight();

			int i, nCnt = pParent->getChildTreeCount();

			for (i = 0; i < nCnt; ++i)
			{
				CUITree* pTree = pParent->getChildTree(i);

				if (pTree == this)
				{
					if (pTree->GetHide() == FALSE)
					{
						nPos += pTree->getTreeItemHeight();
					}
					
					break;
				}

				nPos += pTree->GetHeight();
			}
		}
	}

	return nPos;
}

int CUITree::getTreeItemHeight()
{
	int		nHeight = 0;

	// TreeItem ���̸� ã�´�.
	int n, nChild = getChildCount();

	for (n = 0; n < nChild; ++n)
	{
		CUIBase* pChild = getChildAt(n);

		if (pChild->getType() == eUI_CONTROL_TREEITEM)
		{
			nHeight = pChild->GetHeight();
			break;
		}
	}

	return nHeight;
}

WMSG_RESULT CUITree::LButtonDown( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnLButtonDown(x, y);
	}
	
	if (ret == WMSG_FAIL)
		ret = LButtonDownChild(x, y);

	return ret;
}

WMSG_RESULT CUITree::LButtonUp( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnLButtonUp(x, y);
	}

	if (ret == WMSG_FAIL)
		ret = LButtonUpChild(x, y);

	return ret;
}

WMSG_RESULT CUITree::LButtonDBLClick( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnLButtonDBLClick(x, y);
	}
	
	if (ret == WMSG_FAIL)
		ret = LButtonDBLClickChild(x, y);

	return ret;
}

WMSG_RESULT CUITree::RButtonDown( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnRButtonDown(x, y);
	}
	
	if (ret == WMSG_FAIL)
		ret = RButtonDownChild(x, y);

	return ret;
}

WMSG_RESULT CUITree::RButtonUp( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnRButtonUp(x, y);
	}
	
	if (ret == WMSG_FAIL)
		ret = RButtonUpChild(x, y);

	return ret;
}

WMSG_RESULT CUITree::RButtonDBLClick( UINT16 x, UINT16 y )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnRButtonDBLClick(x, y);
	}
	
	if (ret == WMSG_FAIL)
		ret = RButtonDBLClickChild(x, y);

	return ret;
}

WMSG_RESULT CUITree::MouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide)
	{
		if (m_bEnter == true)
			OnLeave(x, y);

		return MouseMoveChild(x, y, pMsg);
	}

	// â�� ����� �ֵ� ������ ������~
	if (IsInside(x, y) == TRUE)
	{
		if (m_bEnter == false)
			OnEnter(x, y);
	}
	else
	{
		if (m_bEnter == true)
			OnLeave(x, y);
	}

	if (m_bHide == FALSE)
	{
		ret = OnMouseMove(x, y, pMsg);
	}

	if (ret == WMSG_FAIL)
		ret = MouseMoveChild(x, y, pMsg);

	return ret;
}

WMSG_RESULT CUITree::MouseWheel( UINT16 x, UINT16 y, int wheel )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (wheel > 0)
		OnMouseWheelUp();
	else if (wheel < 0)
		OnMouseWheelDown();

	if (m_bHide == FALSE)
	{
		ret = OnMouseWheel(x, y, wheel);
	}

	if (ret == WMSG_FAIL)
		ret = MouseWheelChild(x, y, wheel);

	return ret;
}

WMSG_RESULT CUITree::KeyMessageProc( MSG* pMsg )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnKeyMessage(pMsg);
	}

	if (ret == WMSG_FAIL)
		ret = KeyMessageChild(pMsg);

	return ret;
}

WMSG_RESULT CUITree::CharMessageProc( MSG* pMsg )
{
	WMSG_RESULT ret = WMSG_FAIL;

	if (m_bHide == FALSE)
	{
		ret = OnCharMessage(pMsg);
	}

	if (ret == WMSG_FAIL)
		ret = CharMessageChild(pMsg);

	return ret;
}

WMSG_RESULT CUITree::IMEMessageProc( MSG* pMsg )
{
	WMSG_RESULT ret = WMSG_FAIL;
	
	if (m_bHide == FALSE)
	{
		ret = OnIMEMessage(pMsg);
	}

	if (ret == WMSG_FAIL)
		ret = IMEMessageChild(pMsg);

	return ret;
}

// UIProductNPC ó�� ����Ʈ ���� �޴��� ������ �����Ű��, ������ �Ǿ� ������ ��찡 �ֱ� ������
// Enter ���� üũ �߰���.
WMSG_RESULT CUITree::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bEnter == true)
		CUIBase::OnLButtonUp(x, y);

	return WMSG_FAIL;
}

void CUITree::SetScrollPos( int nPos )
{
	if (m_pScroll == NULL)
		return;

	m_pScroll->changeDragPos(nPos);
}
