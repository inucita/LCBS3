// ----------------------------------------------------------------------------
//  File : UIAuction.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIAUCTION_H_
#define	UIAUCTION_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define AUCTION_WIDTH	644
#define AUCTION_HEIGHT	510
#define AUCTION_SLOT_MAX 10

//�Ǳ���
enum eAuctionTab 
{
	AUCTION_TAB_REFER = 0,		//��ȸ
	AUCTION_TAB_REGISTER,		//���
	AUCTION_TAB_SETTLEMENT,		//����
	AUCTION_TAB_TOTAL,
};

//���Ĺ��
enum eSortType
{
	ITEM_SORT_INDEX = 0,		//�ε���
	ITEM_SORT_COUNT,			//����
	ITEM_SORT_TOTALNAS,			//����
	ITEM_SORT_LEVEL,			//����
	ITEM_SORT_FINISHDAY,		//������
	ITEM_SORT_PASSDAY,			//���
	ITEM_SORT_TOTAL
};

typedef struct _AuctionItems
{
	int AuctionItemIndex; //�ŷ����� ������ ��ȣ
	int AuctionItemStatus; //�ŷ����� ������ ����(�����ϴ���, ���� ���µ�)		
	SQUAD llNasTotal;			//�����۵� �ݾ�
	int	nEtc;				//����, ��������, ���¸� ǥ��

	_AuctionItems()
	{
		Init();
	}

	void Init()
	{
		AuctionItemIndex = 0; AuctionItemStatus = -1;
		llNasTotal = 0; nEtc = 0;
	}
}AuctionItems;

class CUIAuction : public CUIWindow
{
protected:
	CStaticStackArray<AuctionItems> m_AuctionItemData; // ���� ������ ���� �ŷ����� ������ ����

	int m_nCurrentSelectedItem;							//���� ���õ� �����۽���
	int m_nCurrnetTab;									//���� ��
	int m_nCurrentPage;									//���� ������
	int m_nMaxPage;										//�ִ� ������

	int m_nSearchedItemType;							//////////////////////////////////////////////////////////////////////////
	int m_nSearchedItemSubType;							//�˻� ���� ������ ������ �̵��� ������ �˻��ߴ� ������
	int m_nSearchedClass;								//�����ϱ� ���� ����
	int m_nCurretnSorting;								//////////////////////////////////////////////////////////////////////////
	int m_nSelectedItemCancel;							// ��� ����� ������ ��ư �ε���

	FLOAT m_fNpcX, m_fNpcZ;								//npc��ġ(�Ÿ� üũ��)

	CTString m_strSearched;								//���� �˻� ���� ����� ���� ��쿡 ���(�˻��� ����)

	BOOL m_bReverse;									//���Ľ� ���� ���� ��������
	BOOL m_bRegister;									// ��� ��û ���ΰ�?
	BOOL m_bSettle;										// ������ �������� �ִ�.

	SQUAD m_nRegisterItemCount;							//����� ������ ����
	SQUAD m_nRegisterItemPrice;							//����� ������ ����
	SQUAD m_nDepositMoney;								//��� �� �� ������

	CTString			m_strTitleName;					//�ŷ�����â �̸�

	UIRect				m_rcTitle;						//���콺 �巡�׷� �̵���ų �� �ִ� ����
	UIRect				m_rcSelectArea;					//������ ���� ����
	UIRect				m_rcRegisterArea;				//��� ������ �巡��&��� ����
	UIRect				m_rcItemArea;					//�ŷ����� �����۵��� ��ġ�Ǵ� ����
	UIRect				m_rcTabArea[AUCTION_TAB_TOTAL];	//����� Ŭ�� ����

	UIRectUV			m_rtSelectedTab;				//���õ� ��(texture)
	UIRectUV			m_rtUnSelectedTab;				//���õ������� ��(texture)

	//�ŷ����࿡ ����� �̹�����
	//�� ������ ��
	//���� ������ 0
	//����Ʈ �ڽ� 1
	//���� ��� 2
	//�����̹��� 3
	CUIRectSurface		m_AuctionSurface;				
	CTextureData		*m_ptdButtonTexture;
	
	//��ư��
	CUIButton			m_btnClose;								//�ݱ�
	CUIButton			m_btnSearch;							//�˻�
	CUIButton			m_btnFirst;								//ó������
	CUIButton			m_btnPrev;								//1�������ڷ�
	CUIButton			m_btnNext;								//1������������
	CUIButton			m_btnLast;								//�ǳ�����
	CUIButton			m_btnBuy;								//����
	CUIButton			m_btnCancelRegister;					//������
	CUIButton			m_btnExecuteSettlement;					//����
	CUIButton			m_btnSortItem;							//����(�ε���)
	CUIButton			m_btnSortCount;							//����(����)
	CUIButton			m_btnSortTotalNas;						//����(����)
	CUIButton			m_btnSortLevel;							//����(����)
	CUIButton			m_btnSortFinishDay;						//����(������)
	CUIButton			m_btnSortPassDay;						//����(���)

	//////////////////////////////////////////////////////////////////////////
	CUIComboBox			m_cmbItemType;							//������ Ÿ��(����,��...)
	CUIComboBox			m_cmbItemSubType;						//������ ����Ÿ��(������ Ÿ�Կ� ���� �ٸ�)
	CUIComboBox			m_cmbClass;								//��밡�� Ŭ����

	CUIEditBox			m_ebSearch;								//�˻��� �Է��ϴ� ��

	CUIButtonEx			m_btnAuctionItem[AUCTION_SLOT_MAX];		//������ ǥ�ù�ư��
	CUIButtonEx			m_btnRegisterItem;						//����� ������

public :
	CUIAuction();
	~CUIAuction();

	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage(MSG *pMsg );
	WMSG_RESULT CharMessage(MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm);
	void	MsgBoxLCommand(int nCommandCode, int nResult );
	BOOL	CloseWindowByEsc()	{ CloseAuction(); return TRUE; }

	BOOL	IsEditBoxFocused() { return m_ebSearch.IsFocused(); }

	// ��Ÿ �Լ�
	void	OpenAuction(int nNpcIndex, FLOAT fNpcPosX, FLOAT fNpcPosZ);
	void	UseAuction();
	void	CloseAuction();

	void	CloseAuctionMessageBox();
	void	InitSlotItem();
	void	InitAll();

	void	SetTab(int nIndex);
	void	SetSlotItem(int nIndex, int nAuctionIndex, CUIButtonEx &btnAuctionItem, SQUAD nNasTotal, int nEtc, int nItemStatus = -1);
	void	SetItemSubType(int nItemType);
	void	SetCurrentPage(int nPageNum){ m_nCurrentPage = nPageNum; }
	void	SetMaxPage(int nMaxPageNum){ m_nMaxPage = nMaxPageNum; }
	void	SetDepositMoney(SQUAD nDepositMoney){ m_nDepositMoney = nDepositMoney; }
	void	SetRegistering(BOOL bEnable) { m_bRegister = bEnable; }
	void	SetSettling(BOOL bEnable) { m_bSettle = bEnable; m_btnExecuteSettlement.SetEnable(bEnable);}

	void	GetRegisterItemData(CTString &strItemName, SQUAD& nItemCount, SQUAD& nItemPrice); // ������ ��� ����
	int		GetCurrentPage(){ return m_nCurrentPage; } // ���� ������
	SQUAD	GetDepositMoney(){ return m_nDepositMoney; } // ������
	int		GetCurrentTab(){ return m_nCurrnetTab;} // ���� �� ����
	int		GetCurrentAuctionItemCount(); // ���� �������� ��ǰ ����
	BOOL	GetRegistering() { return m_bRegister; }
	BOOL	GetSettling() { return m_bSettle; }

	BOOL	IsSearchConditionChanged(); // �˻� ���� ���� ����
	void	SortItem(int nSortType, BOOL bReverse = FALSE); // ���� (���� ��û)
	void	SearchItem(int nPageNum, int nAlignType);
	void	EnableSortBtn(); // ���� Ȱ��ȭ
	void	BuyItem(); // ������ ����

	void	CancelRegister(); // ��� ���
	void	RegisterItem(); // ������ ���
	BOOL	CanRegister(); // ��� ���� ����
	
	void	GoPage(int nPageNum); // ������ �̵�
	CTString GetItemFullName(CUIButtonEx &btnItem);

	BOOL	IsFarNPC();

public :
	
protected:
	void initialize();
	void OnUpdate( float fElapsedTime );
#ifdef UI_TEST
// 	CUIComboBox*			m_cmbItemType;							//������ Ÿ��(����,��...)
// 	CUIComboBox*			m_cmbItemSubType;						//������ ����Ÿ��(������ Ÿ�Կ� ���� �ٸ�)
// 	CUIComboBox*			m_cmbClass;								//��밡�� Ŭ����
#endif //UI_TEST

};
#endif // UIAUCTION_H_