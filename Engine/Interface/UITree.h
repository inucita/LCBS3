
#ifndef		UI_TREE_H_
#define		UI_TREE_H_

#include <Engine/Interface/UIBase.h>

#pragma warning(disable:4150)

class CUIScrollBar;
class CUIImage;
class CUICheckButton;

class ENGINE_API CUITreeItem : public CUIBase
{
public:
	CUITreeItem();

	CUIBase* Clone();

	void Render(CDrawPort* pDraw);
	bool CheckChildProc();

	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT	OnLButtonDown(UINT16 x, UINT16 y);
};

// ----------------------------------------------------------------
// UITree �� ���, ������ UITree �� ���� �� �ִ� �����̱� ������
// ���� UITool ���� ���� UITree �� ���ø� �������� ����ؾ� �Ѵ�.

class ENGINE_API CUITree : public CUIBase
{
public:
	CUITree();
	~CUITree();

	void addTree(CUITree* pTree, bool bAddBtn = true);
	
	int  getCount()						{ return m_nCount; }
	void setCount(int nMax)				{ m_nCount = nMax; }

	void setUncollapse(BOOL bUncollapse);	// ���� �ִ� ����
	BOOL getUncollapse();

	void updateTree();
	void updateCollapse();

	CUITree* getChildTree(int nIdx);

	int	getChildTreeCount()				{ return m_vecTree.size(); }

	int getSumHeight()					{ return m_nSumHeight; }

	int getChildMaxHeight();

	// ------------------------------------------------------------------
	CUIBase* Clone();
	void clearChildList();

	bool getPreCreate()					{ return m_bPre; }
	void setPreCreate(bool bPre)		{ m_bPre = bPre; }

	void setShow(int nCnt)				{ m_nShow = nCnt; }
	int	 GetShow()						{ return m_nShow; }

	void setCheck(CUICheckButton* pUI)	{ m_pCheck = pUI; }
	CUICheckButton* getCheck()			{ return m_pCheck; }
	void setAddCheck(bool bAdd)			{ m_bAddCheck = bAdd; }

	void setRoot()						{ m_bRoot = true; }
	bool getRoot()						{ return m_bRoot; }

	void setScroll(CUIScrollBar* pScroll);
	void UpdateScroll();

	void Render(CDrawPort* pDraw);
	void SetScrollPos(int nPos);

	WMSG_RESULT		LButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT		LButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT		LButtonDBLClick(UINT16 x, UINT16 y);
	WMSG_RESULT		RButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT		RButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT		RButtonDBLClick(UINT16 x, UINT16 y);
	WMSG_RESULT		MouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT		MouseWheel(UINT16 x, UINT16 y, int wheel);
	WMSG_RESULT		KeyMessageProc(MSG* pMsg);		// ���� �̸� �̹� ���� Proc ����
	WMSG_RESULT		CharMessageProc(MSG* pMsg);
	WMSG_RESULT		IMEMessageProc(MSG* pMsg);

	WMSG_RESULT		OnLButtonUp(UINT16 x, UINT16 y);

protected:	
	void onUpdateTree(CUIScrollBar* pScroll);
	void updateTreeChild(CUIScrollBar* pScroll);
	void onUpdateScroll();
	int getShowTreeCount();
	int getTreeIndex();					// ���°���� �˻��Ѵ�.

	int getParentoffset();
	int getTreeItemHeight();
	
	vec_uinode		m_vecTree;

	CUICheckButton* m_pCheck;
	CUIScrollBar*	m_pScroll;
	
	int				m_nCount;
	bool			m_bPre;					// pre Create
	int				m_nShow;
	int				m_nGap;
	int				m_nSumHeight;
	bool			m_bRoot;
	bool			m_bAddCheck;
};

#endif		// UI_TREE_H_