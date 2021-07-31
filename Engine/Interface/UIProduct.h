
// ----------------------------------------------------------------------------
//  File : UIProduct.h
//  Desc : Created by �̱�ȯ
// ----------------------------------------------------------------------------

#ifndef	UIPRODUCT_H_
#define	UIPRODUCT_H_
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Interface/UIListBoxEx.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Interface/UIProcess.h>

// Define max char and line of strings
#define	MAX_PRODUCT_STRING			4
#define	PRODUCT_STRING_CHAR_WIDTH	170

// Define text position
#define	PRODUCT_TITLE_TEXT_OFFSETX	25
#define	PRODUCT_TITLE_TEXT_OFFSETY	5
#define	PRODUCT_DESC_TEXT_SX		18
#define	PRODUCT_DESC_TEXT_SY		33

// Define size of Product
#define	PRODUCT_TOP_HEIGHT			339
#define	PRODUCT_BOTTOM_HEIGHT		41

#define	PRODUCT_WIDTH				216
#define	PRODUCT_HEIGHT				380

#define PRODUCT_SLOT_ROW			4
#define PRODUCT_SLOT_ROW_TOTAL		30

// ----------------------------------------------------------------------------
// Name : CUIProduct
// Desc : ���� 
// ----------------------------------------------------------------------------



class CUIProduct : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// �ݱ� ��ư 
	CUIButton				m_btnOK;							// ���� ��ư 
	CUIButton				m_btnCancel;						// ��� ��ư 
	
	// Skill buttons
	std::vector<CUIIcon*>	m_vecIcons;							// ���� ������

	// Etc ...
	CUIScrollBar			m_sbProductItem;					// ���� ������ â ��ũ�� ��
	CUIListBox				m_lbPreconditionDesc;				// �ʿ� ���� ���� ����Ʈ �ڽ�
	
	
//	Product Item Info

	int						m_nProductText;						// ���� ���� ������ �ε���
	SWORD					m_nTab;
	SWORD					m_nInvenIdx;

	int						m_nSelectProductItem;				// ���� ���õ� ������
	CTString				m_StrProductType;					// ���� Ÿ��
	int						m_nProductItemCount;				// ���������� �´� ����ǰ ����
	
	BOOL					m_bSatisfied;						// ������ ����Ѱ�?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// �ʿ� ������ ����

	int						m_nNeedItemCount;					// �ʿ��� ������ ������ ��
	int						m_nMakeItemCount;			
		
// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;

// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitProductResult;				// Wait Message

public:
	CUIProduct();
	~CUIProduct();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close Product
	void	Clear ();
	void	InitProduct();

	ENGINE_API	void	OpenProduct( int nItemIndex, SWORD nTab, SWORD inven_idx );
	void				CloseProduct();
	


// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
// Network message functions ( receive )
	void	ProductRep( SBYTE sbResult );
	
// Send ...
	void	SendProductReq();

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );
};


// ----------------------------------------------------------------------------
// Name : CUIProduct2
// Desc : ������ ���� Ȯ��
// ----------------------------------------------------------------------------
class CUIProduct2: public CUIWindow
{
protected:

	struct sResultItem 
	{
		
		int nitemIndex;
		int nSubType;
		UQUAD lExp;
		CMakeItemData* pItemData;

		sResultItem()
		{
			nitemIndex = -1;
			nSubType	= -1;
			lExp		= -1;
			pItemData = NULL;
		}
		~sResultItem()
		{
			nitemIndex = -1;
			nSubType	= -1;
			lExp		= -1;
			pItemData = NULL;
		}
		bool operator<(const sResultItem &other) const
		{
			if( nSubType < other.nSubType )
			{
				return true;
			}
			else if( nSubType == other.nSubType && lExp < other.lExp )
			{
				return true;
			}

			return false;
		}
				
	};
	CTextureData	*m_ptdaddTexture;
	CUIButton		m_btnClose;
	CUIButton		m_btnMake;
	CUIIcon*		m_pIconsMake;
	CUIIcon*		m_pIconsStuff[5];
	CUIDrawBox		m_bxBackground;
	CUIListBoxEx	m_lbMakeList;

	CMakeItemData*	pSelItemData;
	CTString*		m_pSubTypeString;
	CTString*		m_pstrSealType;
	INDEX			m_nSealType;
	int				m_nSelFactoryIndex;

	BOOL			m_bItemMaking;

	std::vector<INDEX> m_vctMakeLearn;		// �ڽ��� ��� ���� ����Ʈ ���� ����

public:
	CUIProduct2();
	~CUIProduct2();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	ListReset();
	// Render
	void	Render();

	void	AddMakeItemList(INDEX nListIndex);
	BOOL	IsLearn(INDEX nIndex);
	void	SetSealType(INDEX sealtype) { m_nSealType = sealtype; }
	UBYTE	GetSealType() { return m_nSealType; }
	void	OpenProduct2(INDEX sealtype);
	void	CloseProduct2();
	void	SetList(CUIListBoxEx* lbList, INDEX sealtype);
	void	SetStuffItem();

	void	SendMakeItem(UBYTE ubType, ULONG lindex);

	void	SetItemMaking(BOOL bMaking);
	void	ProgressBarRender();
	CTString	GetSealTypeName(int nSealtype)	{ return m_pstrSealType[nSealtype-656];}
	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
};

class CUIProductNPC: public CUIWindow
{
protected:

	CTextureData	*m_ptdaddTexture;
	CUIButton	m_btnClose;
	CUIButton	m_btnLearn;
	CUIButton	m_btnCancel;
	CUIDrawBox		m_bxBackground;

	CUIListBoxEx	m_lbLearnList;
	CMakeItemData*	pSelItemData;

	INDEX			m_nSealType;
	FLOAT			m_fNpcX, m_fNpcZ;
	int				m_nNpcVirIdx;
	int				m_nSealNum;		// ��ǥȹ�� Ȯ�� â�� ����� �� ����

public:
	CUIProductNPC();
	~CUIProductNPC();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Render
	void	Render();

	void	Reset();

	void	OpenProductList(INDEX sealtype);
	void	OpenProductNPC(int iMobIndex, int iMobVirIdx, FLOAT npcx, FLOAT npcz);

	void	CloseProductNPC();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void MsgBoxLCommand( int nCommandCode, int nResult );
	void MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput );
	void SendFactoryLearn(int nIndex);

};

#endif	// UIProduct_H_

