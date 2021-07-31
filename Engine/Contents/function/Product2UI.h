#ifndef		_UI_PRODUCT2_H_
#define		_UI_PRODUCT2_H_

// ----------------------------------------------------------------------------
// Name : CUIProduct2
// Desc : ������ ���� Ȯ�� 
//
// ���� : Action -> ���� -> ������ Ŭ��
// ----------------------------------------------------------------------------

class CUIProduct2: public CUIWindow
{
protected:

	INDEX			m_nSealType;
	int				m_nSelFactoryIndex;

	BOOL			m_bItemMaking;

	std::vector<INDEX> m_vctMakeLearn;		// �ڽ��� ��� ���� ����Ʈ ���� ����

public:
	CUIProduct2();
	~CUIProduct2();

	void	ListReset();

	void	SetSealType(INDEX sealtype) { m_nSealType = sealtype; }
	UBYTE	GetSealType() { return m_nSealType; }
	void	OpenProduct2(INDEX sealtype);
	void	CloseProduct2();

	void	SetStuffItem();

	void	SendMakeItem(UBYTE ubType, ULONG lindex);

	void	SetItemMaking(BOOL bMaking);

	void	initialize();	
	void	callback_select(int nsub, int idx, int fac_idx, COLOR col);
	void	callback_over(int nsub, int idx, bool bover, COLOR col);

private:

	void	callback_make();

	bool	set_data();

	CUITree* getTree(int nsub, int idx);
	void	set_color(CUITree* pTree, COLOR col);

	void	set_list_item();

	CUIText*		m_pTxtTitle;
	CUIText*		m_pTxtSealExp;
	CUIButton*		m_pBtnClose;
	
	CUIBase*		m_pTreePos;			// Tree ���� ��ġ
	CUITree*		m_pTreeTemp;		// Tree Template
	CUITree*		m_pTree;
	CUITreeItem*	m_pTreeItemTemp;
	CUIScrollBar*	m_pScroll;


	CUIList*		m_pList;
	CUIIcon*		m_pIconMake;
	CUIText*		m_pTxtMakeName;

	int				m_nsel_sub, m_nsel_idx;		// ���õ� �ε��� ���.
	COLOR			m_colPrev;
};



#endif		// _UI_PRODUCT2_H_