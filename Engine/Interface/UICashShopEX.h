#ifndef UICASHSHOPEX_H_
#define UICASHSHOPEX_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#define MAX_CASHINVEN	10
#define MAX_CASHEQUIP	6

#include <Engine/Entities/CashShopData.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIGroup.h>
#include <Engine/tinyxml/tinyxml.h>
#include <Engine/Interface/UITrackPopup.h>

class CCashTypeNode
{
public:
	enum CTNINFO
	{
		CTN_CATALOG,
		CTN_FIRST,
		CTN_SECOND,
	};

	CCashTypeNode() : m_Parents(NULL) {}
	CCashTypeNode(CCashTypeNode* pNode, CTString strName, CTNINFO _ctnInfo);
	virtual ~CCashTypeNode() {}

	void	Create(CCashTypeNode* pNode, CTString strName, CCashTypeNode::CTNINFO _ctnInfo);
	void	AddNode(CTString strName, CCashTypeNode::CTNINFO _ctnInfo);

	INDEX	GetNodeCount() { return m_SubList.Count(); }
	CCashTypeNode* GetParent(void) { return m_Parents; }
	CTString	GetName(void) { return m_strName; }
	CCashTypeNode& GetSubNode(INDEX iRef) { return m_SubList[iRef]; }

protected:
	CTNINFO		m_CTNINFO;
	CTString	m_strName;
	CCashTypeNode*	m_Parents;
	CStaticStackArray<CCashTypeNode>	m_SubList;
};

class ENGINE_API CUICashShopEX : public CUIWindow
{
public:
	CUICashShopEX();
	~CUICashShopEX();
	// ĳ�ü� ���ο��� ���� �˾� â Ŭ����
	class CCashShopPopup : public CUIWindow{
	public:
		enum CSPopupType{
			CSP_Package, // ��Ű�� ���� 
			CSP_Gift, // ���� ������
			CSP_Purchase, // ĳ�þ����� ����
			CSP_Total,
		};

		CCashShopPopup();
		~CCashShopPopup();

		void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight); // ����
		void SetType(CSPopupType Type) { m_Type = Type; } // ǥ���� Ÿ���� ����(CSPopupType)

		WMSG_RESULT MouseMessage(MSG *pMsg);
		WMSG_RESULT KeyMessage(MSG *pMsg);
		WMSG_RESULT IMEMessage(MSG *pMsg);
		WMSG_RESULT CharMessage(MSG *pMsg);

		BOOL IsEditBoxFocused() {
			if (m_edCharacterName.IsFocused() || m_edGiftContent.IsFocused())
			{
				return TRUE;
			}
			return FALSE; 
		}
		void KillFocusEditBox() {
			m_edCharacterName.SetFocus(FALSE);
			m_edGiftContent.SetFocus(FALSE);
		}

		void OpenPopup(CSPopupType type); // type�� �°� �˾� ����
		void Render();

		void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		void	SetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ) // �˾� ��ġ ������ ���� ��ũ�� ������(��ũ�� �߾����� ����)
		{
			m_pixMinI = pixMinI;
			m_pixMinJ = pixMinJ;
			m_pixMaxI = pixMaxI;
			m_pixMaxJ = pixMaxJ;
		}

		void ClearGoodsData() { m_GoodsDatas.Clear(); } // ��ǰ ������ Ŭ����
		void AddGoodsData(CCashShopData::CASH_SHOP_DATA* csData) { m_GoodsDatas.Add(csData); } // ��ǰ ������ �߰�
		void SetPackageItems(void); // CSP_Package Ÿ�Կ��� m_GoodsDatas�� �ִ� ��ǰ������ ����
		void SetPackageItems(INDEX iPos); // 5���̻��� ��ǰ ��Ͽ��� ��ũ�� �� ��ġ�� �°� ��ǰ ���� ����
		
		void PopupClose();

		// Accessor
		CUIEditBox& GetCharacterName() { return m_edCharacterName; }
		CUIMultiEditBox& GetGiftContent() { return m_edGiftContent; }
		
	protected:
		void SendPurchaseReq(void); // CSP_Purchase Ÿ�Կ��� ���� ��û
		void AddFriendsList(void); // �޽������� ������ ��� ģ�� ����� ����

		CUIRectSurface m_BgPackagePopup; // CSP_Package Ÿ���� Surface ����
		CUIRectSurface m_BgGiftPopup; // CSP_Gift Ÿ���� Surface ����
		CUIRectSurface m_BgPurchasePopup; // CSP_Purchase Ÿ���� Surface ����

		CUIScrollBar m_PackageScroll; // CSP_Package Ÿ���� ��ũ�� UI
		CUIScrollBar m_GiftScroll; // CSP_Gift Ÿ���� ��ũ�� UI

		CUIButton m_btnClose; // close ��ư
		CUIButton m_btnConfirm; // Ȯ��, ����
		CUIButton m_btnCancel; // ���

		CUIEditBox m_edCharacterName; // ���� ���� ĳ���� ���⸦ ���� EditBox
		CUIMultiEditBox	m_edGiftContent; // ���� ���� �޽��� ���⸦ ���� MultiEditBox
		CUIComboBox m_cbFriends; // ģ�� ��� ǥ���� ���� �޺� �ڽ�

		CUIIcon* m_pIconPackage; // CSP_Package�� Package ������ ���� ButtonEx
		CStaticArray<CUIIcon*> m_PackageItems; // CSP_Package�� ������������ ���� ButtonEx
		CStaticStackArray<CCashShopData::CASH_SHOP_DATA*> m_GoodsDatas; // �˾����� ���� ��ǰ ���
		
		CSPopupType m_Type; // �˾��� Ÿ��

		PIX m_pixMinI;
		PIX m_pixMinJ;
		PIX m_pixMaxI;
		PIX m_pixMaxJ;
	};

	enum CashShopFrame
	{
		CSF_MainTab,
		CSF_Category,
		CSF_View,
		CSF_Recommand,
		CSF_Preview,
		CSF_Inven,
		CSF_Total,
	};

	enum CS_MainTab
	{
		CS_IrisShop, // ���̸��� ����
		CS_PurchaseInfo, // ���� ����
		CS_GotTheGift, // ���� ����
		CS_WishPage, // �Ҹ� ����
		CS_Total,
	};

	enum CashShopCategory
	{
		CSC_NONE = -1,
		CSC_HOTandNew,
		CSC_Platinum,
		CSC_SpendGoods,
		CSC_EquipGoods,
		CSC_Avata,
		CSC_ServiceGoods,
		CSC_PackageGoods,
		CSC_Total,
	};

	enum CategorySubType
	{
		CST_First,
		CST_Second,
		CST_Total,
	};

	enum ViewFrameType
	{
		VFT_GoodsList,
		VFT_WishList,
		VFT_PurchaseList,
		VFT_GiftList,
	};

	enum ViewPageType
	{
		VPT_Begin,
		VPT_Front,
		VPT_Back,
		VPT_End,
		VPT_Total,
	};

	enum BtnInvenType
	{
		BIT_Clear,
		BIT_PurchaseItem,
		BIT_CashCharge,
		BIT_Exit,
		BIT_Total,
	};

	enum GoodsFlagInfo
	{
		CFI_New,
		CFI_Popular,
		CFI_Discount,
		CFI_Total,
	};

	enum UVRectPos {
		UV_UL, UV_UM, UV_UR,
		UV_ML, UV_MM, UV_MR,
		UV_LL, UV_LM, UV_LR,
		UV_Total,
	};

	enum CashShopCommand
	{
		CSC_ItemPurchase, // �ܵ� ����
		CSC_CostumeWear,
		CSC_AddInven,
		CSC_AddWishBox,
		CSC_Cancel,
	};

	typedef struct _GOODSBOX
	{
		CUIRectSurface BoxSurface;
		// Background Image
		CCashShopData::CASH_SHOP_DATA* GoodsData; // ĳ�� ������ ���� ������
		CUIIcon* pIconGoods; // ��ǰ ĳ�� ������ ��ư
		CUIButton btnPkgInfo; // ��Ű�� ���� ��ư
		// �׿��� Ư�� ���� �̹��� �߰� ����, (New, Ư�� �� ���ݿ� �´� �̹���)
		_GOODSBOX() : GoodsData(NULL) {
			pIconGoods = new CUIIcon;
		}
		~_GOODSBOX() {
			SAFE_DELETE(pIconGoods);
		}

	}GoodsBox;

	// Category ���� ����
	typedef struct _CS_FirstSubType 
	{
		CStaticStackArray<INDEX> SecondSubTypeIndexList;
		INDEX iFirstSubType;

		void Clear()
		{
			iFirstSubType = -1;
			SecondSubTypeIndexList.Clear();
		}
	}CS_FirstSubType;

	typedef struct _CS_CategoryTypeInfo
	{
		CStaticStackArray<CS_FirstSubType> FirstSubTypeIndexList;
	}CS_CategoryTypeInfo;

	typedef struct _CS_PurchaseGoods 
	{
		BOOL bEnable;
		BOOL bSelected;
		CUIIcon* pIconGoods;
		_CS_PurchaseGoods () : bEnable(FALSE),bSelected(FALSE) {
			pIconGoods = new CUIIcon;
		}

		~_CS_PurchaseGoods() {
			SAFE_DELETE(pIconGoods);
		}

		void Clear()
		{
			bEnable = FALSE;
			bSelected = FALSE;
			pIconGoods->clearIconData();
		}
	}CS_PurchaseGoods;

	typedef struct _CS_GiftData
	{
		CTString strName;
		CTString strMessage;
		ULONG	ulDate;
		ULONG	ulUniIndex;
		CCashShopData::CASH_SHOP_DATA* GoodsData;
		_CS_GiftData() : GoodsData(NULL), ulDate(0), ulUniIndex(0) {}

		void Clear()
		{
			ulDate = 0;
			ulUniIndex = 0;
			GoodsData = NULL;
		}
	}CS_GiftData;

	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);

	void Render();

	WMSG_RESULT MouseMessage(MSG *pMsg);
	WMSG_RESULT KeyMessage(MSG *pMsg);
	WMSG_RESULT IMEMessage(MSG *pMsg);
	WMSG_RESULT CharMessage(MSG *pMsg);

	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput ,CTString &strConfirm);
	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput);
	void MsgBoxLCommand(int nCommandCode, int nResult);

	void SetCashType(CNetworkMessage *istr); // ������ ���� ����� ī�װ� Ÿ���� �޴´�.(���� Ÿ�� ������)
	void SetMyCash(SQUAD myCash); // ���� ���� ĳ������ ����.(invisible ���¿����� OpenCashShopEX() ȣ��)
	void SetGoodsList(CNetworkMessage *istr); // ��û�� ��ǰ ����Ʈ ���� ����
	void SetRecommandList(CNetworkMessage *istr); // ��õ ��ǰ ����Ʈ ���� ����
	void SetWishList(CNetworkMessage *istr); // �Ҹ� ���� ��ǰ ����Ʈ ���� ����
	void SetPurchaseItemList(CNetworkMessage *istr); // ������ ��ǰ ���� ����
	void RevWishList(CNetworkMessage* istr, BOOL bCommit); // �Ҹ����� ������ ���� ����
	void RevGiftList(CNetworkMessage* istr); // ���� ��� ����
	void RevResultGetGift(void); // ���� �������� ���

	void SendPurchaseReq(); // ���ſ�û �޽���
	void SendBringReq(); // ���� ��ǰ ��������
	void SendGiftReq(void); // ���� ������

	void SetCurrentPageType(INDEX iType) { m_CurrentPageType = iType; } // ���� �����ִ� ViewFrame�� �˻� Ÿ�� ����
	BOOL SetCashIndexToBtn(int nCashIndex,int nUniIndex); // ���������� ĳ�þ������� �߰�
	void SetRClickMenu(INDEX iItem); // ��Ŭ���� �˾� �޴� ����
	void SetCashShopLock(BOOL bLock) { m_bCashShopLock = bLock; } // ���� ���� ������ ���� �� ����
	void SetWearingModel(CUIIcon* srcBtn, INDEX iCashIndex = 0); // Preview�� ĳ���� ��� ������.
	void SetTakeOffModel(INDEX iRef); // Preview�� ĳ���� ��� ���´�.
	void SetPackageWearingModel(CUIIcon* srcBtn, INDEX iCashIndex = 0);
	void SetSuitWearingModel(INDEX iItem, INDEX iCash);

	BOOL GetCashShopLock(void) { return m_bCashShopLock; }
	INDEX GetCurrentPageType(void) { return m_CurrentPageType; } // ���� �� ������ Ÿ��
	ULONG GetMyCash(void) { return (ULONG)m_hasCash; } // ���� ĳ��
	UIRectUV* GetUVbtnEXInfo(void) { return m_UVbtnEXInfo; } // ��ư ������ ǥ���ϱ� ���� ���ڽ�

	void OpenCashShopEX(void); // ���� ����
	void CloseCashShopEX(void);

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void AddBtnItemInfoString(CUIIcon* srcBtn, BOOL bRemain = FALSE); // ButtonEx(ĳ�þ����� ����)�� ������ ���ڿ� ����
	void AddPackageItemInfo(CUIIcon* srcBtn); // ButtonEx(��Ű�� ĳ�� ������ ����)�� ������ ���ڿ� ����

	void CompletePurchase(void); // ���� �Ϸ�
	void CompleteBringItems(void); // ������ �������� �Ϸ�

	BOOL IsEditBoxFocused() {
		return m_Popup.IsEditBoxFocused();
	}
	void KillFocusEditBox() {
		m_Popup.KillFocusEditBox();
	}

	void ForcedExit(void); // ���� ����
	void SetLoadDataState(BOOL bState) { m_bLoadingData = bState; } // ������ ������ ��û ��
	
	void InitRenderTarget(void); // RenderTarget �ʱ�ȭ
	void DestroyRenderTarget(void); // RenderTarget ����

	void Message(int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle); // Error ����� ���� �޼��� �ڽ�
	
	BOOL IsPopupOpened(void) { return m_Popup.IsVisible(); }
	void SetPopupVisible(BOOL bVisible) { m_Popup.SetVisible(bVisible); }

protected:
	// Frame���� ����� �Ǵ� Surface �ʱ�ȭ
	void BasicBackgroundInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void MainTabFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void CategoryFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void ViewFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void RecommandFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void PreviewFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);
	void InvenFrameInit(FLOAT fTexWidth, FLOAT fTexHeight);

	// MouseMove �ϰ� ó��
	WMSG_RESULT MouseMoveLoop(MSG *pMsg, CashShopFrame type);

	// Ŭ���� �Լ���
	void ClearCashType();
	void ClearGoodsList();
	void ClearRecommandList();
	void ClearWishList();
	void ClearPurchaseItemList();

	// ���� �˻� ������ Indexȭ �Ѵ�.
	INDEX GetSearchType(void);
	// ��Ŭ���� �˾��޴����� ���õ� ���� ����
	void RunRClickMenu(INDEX iType);

	// �޺� �ڽ� ����
	void SetComboBox(CashShopCategory categorytype);
	void SetSubComboBox(CashShopCategory categorytype, INDEX iType, INDEX iSubType);
	// ��ũ�� �� ���� ���� ����
	void SetRevGiftDatas(INDEX iPos);

	void ArrangePurchaseGetGoods(void); // �ű� ��ǰ ����
	void MovePurchaseGoods(BOOL bOver = FALSE); // �ִ� 10���� �Ѳ����� �ڸ� �ű�

	// ������ ������ ������ Popup Ŭ������ ���
	void AddGoodsBringData(void);

	void SendWishItems(ULONG ulCTID, BOOL bCommit = TRUE); // �Ҹ����ڷ� ��� ��û
	void SendGetRevGiftItem(INDEX iPos); // ���� ������(���� �ϳ��� ����)

	void InitCatalogNameNodes(void);

	TiXmlElement* FromXmltoCashData(CCashTypeNode& CashNode, TiXmlElement* pNode, CCashTypeNode::CTNINFO NodeType);

	// Background
	CUIRectSurface m_Background;

	// Frame ������ �־��..
	UIRect m_CashShopFrame[CSF_Total];

	// CSF_MainTab
	CUIButton m_BtnMainTab[CS_Total];
	CS_MainTab m_SelMainTab;

		// CS_WishPage �Ҹ� ����
	CStaticArray<GoodsBox> m_GoodsBoxList;	// Max 8(page�� 8�� ��ǰ cpp���� ��������.
	CStaticArray<CS_PurchaseGoods> m_PurchaseItemList;
	CStaticArray<CUIIcon*> m_PurchaseGetItemList;
	CStaticArray<GoodsBox> m_WishBoxList;
	CStaticStackArray<CS_GiftData> m_RevGiftList;

	// CSF_Category
	CUIButton m_BtnCategory[CSC_Total];
	CUIEditBox m_ebCategoryName; // ī�װ� ���ý� ī�װ� �̸� ǥ��
	CUIComboBox m_cbCategoryType[CST_Total];
	CUIButton m_BtnSearch; // �˻� ��ư

	UIRectUV m_UVSelCategory;

	CashShopCategory m_SelCategoryTab;
	CashShopCategory m_OverCategoryTab;

	INDEX m_CurrentPageType; // ���� �����ְ� �ִ� �˻� type

	// CSF_View
	CUIRectSurface m_blueBoxGoodsList;
	CUIRectSurface m_blueBoxPurchase;
	CUIRectSurface m_BlackBoxPage;
	ViewFrameType m_ViewFrameType;
	CUITrackPopup m_tpCommand; // �˾� �޴�

	CUIRectSurface m_BlackBoxRevGiftList;
	CUIScrollBar m_GiftScroll;

	INDEX m_iCurrentPage;
	INDEX m_iMaxPage;
	CUIEditBox m_ebPage; // ���� ������ ǥ�ÿ�
	CUIButton m_BtnPage[VPT_Total];

	UIRectUV m_uvFlag[CFI_Total];

	CUIMultiEditBox	m_edGiftContent; // ���� ���� ���� ��
	UIRect m_rtGiftDataList;

	UIRect m_PurchaseItemRect; // ������ ��ǰ ����
	UIRect m_PurchaseGetItemRect; // �ű� ��ǰ ����
	CUIButton m_BtnMove; // ��� �ű��
	CUIButton m_BtnGet; //  ��������(���� ����, ��������)
	CUIButton m_BtnGift; // �����ϱ�
	INDEX m_SelectedCount; // �ִ� 10������ ���� ����
	
	CUIIcon* m_pIconRevGift[5];
	INDEX	m_SelectedGift;

	// CSF_Recommand
	CStaticArray<GoodsBox> m_RecommandBoxList; // Max 5 ( 1~5 Rank Info)
	UIRectUV m_uvRecommandImage; // ����?

	// CSF_Preview
	UIRect m_rtPreview; // Preview ����
	CUIButton m_btnRestore; // ȸ�� ��ư
	CUIIcon* m_pIconEquip[MAX_CASHEQUIP];
	INDEX m_iEquipCashIndex[MAX_CASHEQUIP+1]; // +1�� ���и� �����ߴ����� ���ؼ� ������.
	INDEX m_iEquipDropCashIndex;
	CRenderTexture* m_ModelRenderTarget; // 3D ���� ���� ������ �� ����

	// CSF_Inven
	CUIIcon* m_pIconInven[10];		// Japan�� MAX_CASHINVEN�� 1�̴�. ��ư ������ �ؾ��ؼ� 10���� ����.
	CUIButton m_btnInven[BIT_Total];
	SQUAD	m_GoodsPrice;
	SQUAD	m_hasCash;
	UIRect	m_btnExInvenRect;

	// Common UV and button
	UIRectUV m_UVbtnEXLock;
	CUIButton m_btnClose;
	UIRectUV m_UVbtnEXInfo[UV_Total];

	// Popup instance
	CCashShopPopup m_Popup;

	CS_CategoryTypeInfo m_CashSubTypeInfo[CSC_Total]; // Cashshop���� ����� type �����̴�.

	int m_SelectedGoods;
	int m_SelectedGetPurchase;
	int m_SelectedIven;
	int m_SelectedRecommand;
	int m_SelectedEquip;
	int m_SelectedWishList;

	INDEX m_iRClickMenuIndex;

	BOOL m_bCashShopLock;

	CUIRectSurface m_LoadingBackBox;
	CUIAnim m_LoadingAnim;
	BOOL m_bLoadingData;
	BOOL m_bPackageInfo;
	BOOL m_bShowPurchaseOver;

	CCashTypeNode	m_CatalogNamesNodes[CSC_Total];

	bool m_bPreviewDrag;
	bool m_bMoveStartInven;
public:
	bool IsVFT_GoodList() 
	{ 
		if (m_ViewFrameType == VFT_GoodsList)
			return true;
		return false;
	}
};

#endif