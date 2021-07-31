#ifndef		_UI_LIST_H_
#define		_UI_LIST_H_

class CUIBase;
class CUIListItem;

class ENGINE_API CUIList : public CUIBase
{
public:
	CUIList();
	~CUIList();

	enum eEVENT_IMAGE
	{
		eTYPE_SELECT = 0,
		eTYPE_OVER,
		eTYPE_MAX
	};
	//------------------------------------------------------

	CUIBase* Clone();

	void CmdErase();

	void AddListItem(CUIBase* pItem);
	void RemoveListItem(int idx);
	bool DeleteAllListItem();

	void deleteChildList();
	void deleteListItem(int nPos);
	void deleteListItem(CUIBase* pt);

	// �ε����� ����Ʈ ������ ���
	CUIBase* GetListItem(int idx);

	inline void SetListItemTemplate(CUIListItem* pItem)	{ m_pListItemTemplate = pItem; }
	inline CUIListItem* GetListItemTemplate()			{ return m_pListItemTemplate; }

	//------------------------------------------------------

	void SetItemStart(int x, int y);
	void GetItemStart(int& x, int& y) { x = m_nItemStartX; y = m_nItemStartY; }

	void SetItemShowNum(int nShow)	{ m_nNumShow = nShow; }
	int  GetItemShowNum()		 { return m_nNumShow; }

	void SetGap(int gap)		 { m_nGap = gap; }
	int GetGap()				 { return m_nGap; }

	void UpdateList(bool bUpdateScroll = false);
	void UpdateScroll(int nItemCount);
	void UpdateHeight(int nHeight);

	void SetAlignBottom()	{ m_bAlignTop = false; }
	CUIScrollBar* GetScroll()	{ return m_pScroll;	}
	
	//------------------------------------------------------
	// ������ ���õ� �������� ���� ��� ȣ�� �Ǵ� Ŀ�ǵ�
	void SetCommandUn(Command* pCmd)	{ SAFE_DELETE(m_pCmdUnSelect); m_pCmdUnSelect = pCmd; }
	void SetCommandFSelect(_ui_func func)	{ m_func_select = func; }

	void SetCommandUpdate(Command* pCmd)	{ SAFE_DELETE(m_pCmdUpdate); m_pCmdUpdate = pCmd; }

	//------------------------------------------------------

	void OnRender(CDrawPort* pDraw);
	void OnPostRender(CDrawPort* pDraw);

	//------------------------------------------------------

	void setScroll(CUIScrollBar* pScroll);

	// xml ���� �̸� ����Ʈ�� ��Ͻ�ų���� ���
	void SetItemCount(int cnt)		{ m_nItemCount = cnt; }
	int	GetItemCount()				{ return m_nItemCount;}

	void SetPreCreate(bool bPre)	{ m_bPreCreate = bPre; }
	bool GetPreCreate()				{ return m_bPreCreate; }

	void addEventImage( UIRect rect, UIRectUV uv, int type );
	int HitTest(UINT16 x, UINT16 y);

	void setCurSel(int idx);
	int	 getCurSel()			{ return m_nCurSel; }

	void setMouseOverItem(int idx);
	int	 getMouseOverItem()			{ return m_nMouseOverItem;	}

	int getListItemCount()			{ return m_vecListArray.size(); }

	int	GetItemTotalHeight()		{ return m_nItemTotalHeight;	}

	void SetRectSurface(CUIRectSurface* pRS) { m_pimgEvent = pRS; }
	CUIRectSurface* GetRectSurface()		 { return m_pimgEvent; }

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT	OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDBLClick(UINT16 x, UINT16 y);

	void SetSelectedForeRender(bool bFore)	{ m_bSelectedForeRender = bFore; }

	void SetKeepSelect(bool bKeep)	{ m_bKeepSelect = bKeep; }

#ifdef UI_TOOL
	void		GetEventImg(UIRect& rc, UIRectUV& uv, int type)	{ rc = m_rcEventImg[type]; uv = m_uvEventImg[type];	}

private:
	UIRect		m_rcEventImg[eTYPE_MAX];
	UIRectUV	m_uvEventImg[eTYPE_MAX];
#endif // UI_TOOL

private:
	void UpdateListTop();
	void UpdateListBottom();
	void _render(CDrawPort* pDraw);

protected:

	typedef std::vector< CUIBase*>::iterator	vec_listitem_iter;
	std::vector< CUIBase*> 		m_vecListArray;
	
	bool	m_bUseSelect;
	bool	m_bPreCreate;
	bool	m_bAlignTop;
	bool	m_bKeepSelect;

	int		m_nNumItems;		// ������ ��
	int		m_nGap;				// ������ ����
	int		m_nNumShow;			// ����Ʈ�� ������ ������ ��

	int		m_nItemStartIndex;	// ������ �������� ���� �ε���
	int		m_nItemCount;		// ��ü �������� �� (���̳�, �̸� �����ÿ� �ʿ�)

	int		m_nItemStartX;		// 
	int		m_nItemStartY;		// 

	bool	m_bSelected;
	bool	m_bMouseOver;
	int		m_nCurSel;	// ���õ� ������ �ε���
	int		m_nMouseOverItem;	// ���콺 ������ ������ �ε���
	POINT	m_ptSelOffset;
	POINT	m_ptMouseOverOffset;

	int		m_nCurStart;		// ��ũ���� ���� ��� ���� �ε���
	int		m_nItemTotalHeight;	// ����Ʈ �������� �� ����.

	CUIListItem*	m_pListItemTemplate;	// ����Ʈ ������ ������ �̸� ������� ������ ���ø�. Clone�Ͽ� ����Ѵ�.
	
	CUIScrollBar*	m_pScroll;

	CUIRectSurface*	m_pimgEvent;	

	Command*	m_pCmdUnSelect;
	Command*	m_pCmdUpdate;				// scroll � ���� update �� �� ȣ��Ǵ� �ݹ�

	bool	m_bSelectedForeRender;

	_ui_func	m_func_select;
};


#endif		// _UI_LIST_H_