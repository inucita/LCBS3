
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

enum eSealType
{
	SEAL_1 = 0,		//������ ����
	SEAL_2,			//ȭ���� ����
	SEAL_3,			//��ī�ο� ����
	SEAL_4,			//�ܴ��� ����
	SEAL_5,			//�Ҹ�ǰ ����
	SEAL_MEMBER,	//ȸ�� ���
};

#define SEALTYPE_START_INDEX 656	 

#define MAX_SEALTYPE	5
#define MAX_SEALSUBTYPE	23

extern int SealTypeCount[MAX_SEALTYPE+1];
extern int SealSubType[MAX_SEALSUBTYPE];


class CUIProduct : public CUIWindow
{
protected:		
	// Skill buttons
	std::vector<CItems*>	m_vecItems;							// ���� ������

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
		
// Network ...
	BOOL					m_bWaitProductResult;				// Wait Message

public:
	CUIProduct();
	~CUIProduct();
	
// Open & close Product
	void	Clear ();
	void	InitProduct();

	ENGINE_API	void	OpenProduct( int nItemIndex, SWORD nTab, SWORD inven_idx );
	void				CloseProduct();

// Messages
	WMSG_RESULT	MouseMessage1( MSG *pMsg );
	
// Network message functions ( receive )
	void	ProductRep( SBYTE sbResult );
	
// Send ...
	void	SendProductReq();

	void	SelectItem(int nIndex = -1);

	void	initialize();
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);

private:

	void	clearItems();
	
	void	AddString(CTString& strText, COLOR colText = 0xffffffff);

	void	pushback_string(CTString& strText, COLOR color);
	void	callback_select();

	CUIText*	m_pTxtSubject;
	CUIList*	m_pListItems;
	CUIList*	m_pListDesc;
	CUIButton*	m_pBtnOK;

};

#endif	// UIProduct_H_

