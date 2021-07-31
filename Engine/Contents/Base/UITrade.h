#ifndef _UI_TRADE_H_
#define _UI_TRADE_H_

class CUITrade : public CUIWindow
{
public:
	CUITrade();
	~CUITrade();

	void initialize();
	void OpenUI();
	void CloseUI();
	
	BOOL CloseWindowByEsc()		{ CloseUI();	return TRUE;	}

	void SetTitle(std::string strTitle);

	// ���� �ްڴ�.
	void SendTrade();

	// ���� ������ ����Ʈ.
	void UpdateItemList();
	// ����ǰ�� �ޱ� ���� �ʿ� ������.
	void UpdateAmendCondition(int nAmendIndex);
	// ����ǰ�� �ޱ� ���� �ʿ� ������ ����. 
	// amendindex�� 0���� ���� ��� ���� ���õ� ������ ī��Ʈ�� ������Ʈ.
	void UpdateAmendConditionCount(int nAmendIndex);
	// ���� ������ �� ����.
	void UpdateAmendItem(int nAmendIndex);
	// ���� ������ ����.
	void UpdateAmendItemCount(int nAmendIndex);
	
	// ���� ������ �ִ��� �˻�.
	bool IsAmend(int nAmendIndex);

	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDBLClick(UINT16 x, UINT16 y);

private:
	enum
	{
		eLIST_AMEND_ITEM = 0,
		eLIST_AMEND_CONDITION,
		eLIST_END
	};

	bool CheckItem(int nAmendIndex, int nConditionIndex);

	CUIBase*	m_pDragArea;
	CUIButton*	m_pBtnOk;
	CUIText*	m_pTextTitle;

	CUIList*	m_pList[eLIST_END];
	CUIBase*	m_pBaseAmendInfo;
	

	int m_nOriX;
	int m_nOriY;
	bool m_bDrag;
};
#endif // _UI_TRADE_H_