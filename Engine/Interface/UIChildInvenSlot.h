// ----------------------------------------------------------------------------
//  File : CUIChildInvenSlot.h
//  Desc : Create by 
// ----------------------------------------------------------------------------

#ifndef	UI_CHAILD_INVENSLOT_H_
#define	UI_CHAILD_INVENSLOT_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

//#include <Engine/Interface/UIInventory.h>

// Define item slot
// UI���� �κ��丮 [10/5/2012 Ranma]
//////////////////////////////////////////////////////////////////
#define ITEM_SLOT_UV_SX				33
#define ITEM_SLOT_UV_SY				214
#define	ITEM_SLOT_UV_WIDTH			(ITEM_SLOT_UV_SX + 190)
#define	ITEM_SLOT_UV_HEIGHT			(ITEM_SLOT_UV_SY + 187)

#define	ITEM_SLOT_TITLE_UV_SX		239
#define	ITEM_SLOT_TITLE_UV_SY		389	
#define	ITEM_SLOT_TITLE_UV_WIDTH	(ITEM_SLOT_TITLE_UV_SX + 273)	
#define	ITEM_SLOT_TITLE_UV_HEIGHT	(ITEM_SLOT_TITLE_UV_SY + 35)

#define	ITEM_SLOT_MIDDLE_UV_SX		239
#define	ITEM_SLOT_MIDDLE_UV_SY		426	
#define	ITEM_SLOT_MIDDLE_UV_WIDTH	(ITEM_SLOT_MIDDLE_UV_SX + 273)	
#define	ITEM_SLOT_MIDDLE_UV_HEIGHT	(ITEM_SLOT_MIDDLE_UV_SY + 78)

#define	ITEM_SLOT_BOTTOM_UV_SX		239	
#define	ITEM_SLOT_BOTTOM_UV_SY		503	
#define	ITEM_SLOT_BOTTOM_UV_WIDTH	(ITEM_SLOT_BOTTOM_UV_SX + 273)
#define	ITEM_SLOT_BOTTOM_UV_HEIGHT	(ITEM_SLOT_BOTTOM_UV_SY + 9)
///////////////////////////////////////////////////////////////////
#define ITEM_SLOT_WIDTH_GAP			12
#define ITEM_SLOT_HEIGHT_GAP		6
#define ITEM_SLOT_TOTAL_WIDTH		(190 + ITEM_SLOT_WIDTH_GAP)

#define	ITEM_SLOT_TITLE_SX			0
#define	ITEM_SLOT_TITLE_SY			0	
#define	ITEM_SLOT_TITLE_HEIGHT		35
	
#define ITEM_SLOT_SX				(ITEM_SLOT_TITLE_SX + ITEM_SLOT_WIDTH_GAP/2)
#define ITEM_SLOT_SY				ITEM_SLOT_TITLE_HEIGHT
#define	ITEM_SLOT_WIDTH				190
#define	ITEM_SLOT_HEIGHT			187

#define	ITEM_SLOT_MIDDLE_SX			ITEM_SLOT_TITLE_SX
#define	ITEM_SLOT_MIDDLE_SY			ITEM_SLOT_TITLE_HEIGHT	
#define	ITEM_SLOT_MIDDLE_HEIGHT		(ITEM_SLOT_HEIGHT + ITEM_SLOT_HEIGHT_GAP)

#define	ITEM_SLOT_BOTTOM_SX			ITEM_SLOT_TITLE_SX	
#define	ITEM_SLOT_BOTTOM_SY			(ITEM_SLOT_MIDDLE_SY + ITEM_SLOT_MIDDLE_HEIGHT)
#define	ITEM_SLOT_BOTTOM_HEIGHT		9	
#define ITEM_SLOT_TOTAL_HEIGHT		(ITEM_SLOT_TITLE_HEIGHT + ITEM_SLOT_MIDDLE_HEIGHT + ITEM_SLOT_BOTTOM_HEIGHT)

#define CLOSE_BTN_SIZE				16
#define CLOSE_BTN_WIDTH_GAP			6

#define START_ITEMBTN_X				(ITEM_SLOT_SX + 7)
#define START_ITEMBTN_Y				(ITEM_SLOT_SY + 6)

class CUIChildInvenSlot : public CUIWindow
{
	
protected:
	// methods.

	BOOL				m_bPickTitle;

	// UI���� �κ��丮 [10/4/2012 Ranma]
	UIRect				m_rcTitle;								// Region of title
	UIRect				m_rcItemSlot;							// Region of item slot
	UIRectUV			m_rcItemSlotUV;							// Region of item slotUV
	CTextureData		*m_ptdBGSlotTexture;					// ���� ��� �ؽ��� 
	CTextureData		*m_ptdBaseTexture;						// �κ� ���� �⺻ ���̽� �ؽ���
	CTextureData		*m_ptdCommonTexture;
	CUIRectSurface		m_rsItemSlotSurface;					// ��� �����̽�
	CUIRectSurface		m_SlotInvenSurface;						// �κ��丮�� �����̽� ������ ����ų ������
	CUIRectSurface		m_NewItemEffectSurface;					// ���� ���� ������ ������ ����Ʈ�� �̹���
	
	btnAllItems*		m_pAllItem;								// �κ��丮�� �������� ����ų ������

	BOOL				m_bNewItem[INVEN_ONE_BAG];
	BOOL				m_bStartEffect[INVEN_ONE_BAG];
	int					m_nPlayCount[INVEN_ONE_BAG];
	
	int					m_nRearrangeOrderID;					// UI ���̵� ���� 1���� 2���� 3���� ����

	int					m_nSelSlotItemID;						// Selected slot item
	
	const int 			m_nIndexBag;							// 

	// Controls
	CUIButton			m_btnClose;								// Close button
	CUIImage*			m_pImgSelect;

protected:
	virtual void	_openUI(const BOOL bType);
	virtual void	_closeUI();
public:
	// interface
	void			ToggleVisible(const BOOL bResultView = FALSE);		// false : select server list view. // true : success or failed result view.
	void			SetRearrangeOrderID(int nRearrangeOrderID) { m_nRearrangeOrderID = nRearrangeOrderID; } // UI ���̵� ����
	void			SetInventoryAllItem(btnAllItems* pAllItem) { m_pAllItem = pAllItem; } // �κ��丮�� ������ ����

public:
	CUIChildInvenSlot(int _bag_index);
	~CUIChildInvenSlot();
	
	void			Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void			Render();
	void			ItemRender();

	// Adjust position
	void			ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void			ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void			AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	WMSG_RESULT		MouseMessage( MSG *pMsg );
	virtual BOOL	CloseWindowByEsc()					{ CUIManager::getSingleton()->GetInventory()->ToggleVisible();	return TRUE; }
	int				GetSelSlotItemID()					{ return m_nSelSlotItemID; }
	void			SetSelSlotItemID(int nSelSlotItemID) { m_nSelSlotItemID = nSelSlotItemID; }

	int				GetChildInvenTab();
	int				GetChildInvenIdx(int slot);

	void	ClearNewItemEffect();
	void	SetNewItemEffect(BOOL bShow, int InvenIdx);
	void	SetStartItemEffect(int InvenIdx);

	BOOL	IsPlayNewItemEffect(int InvenIdx);
	BOOL	IsCheckEffectPlayCount(int InvenIdx);

	void	UpdateSelectItem();

	// attributes.
private:
};
	
#endif	// UI_CHAILD_INVENSLOT_H_
	
