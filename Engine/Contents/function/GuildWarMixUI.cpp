#include "stdh.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/function/GuildWarMixUI.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>

#define ITEM_GWMIX_TYPE1		0 // ����� ���� ��� 0�� ��
#define ITEM_GWMIX_TYPE2		1 // ����� ���� ��� 1�� ����Ʈ
#define ITEM_GWMIX_TYPE3		2 // ����� ���� ��� 2�� ����

class CmdGuildWarMixDropItem : public Command
{
public:
	CmdGuildWarMixDropItem() : m_pWnd(NULL), m_nIdx(-1)	{}
	void setData(CGuildWarMixUI* pWnd, int nIdx)	{ m_pWnd = pWnd; m_nIdx = nIdx;	}
	void execute()
	{
		if (m_pWnd == NULL)
			return;

		m_pWnd->ResetMixItem(m_nIdx);
	}

private:
	CGuildWarMixUI* m_pWnd;
	int m_nIdx;
};
CGuildWarMixUI::CGuildWarMixUI()
	: m_pBtnOk(NULL)
{
	setInherit(false);
	m_nCurItemCount		= 0;

	for (int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; ++i)
		m_pIconsSlot[i] = NULL;

	m_dwWndState = UWS_ENABLE | UWS_VISIBLE;
}

CGuildWarMixUI::~CGuildWarMixUI()
{
}

void CGuildWarMixUI::initialize()
{
#ifndef		WORLD_EDITOR

	CUIBase* pBase = NULL;

	if (pBase = findUI("btn_close"))
		pBase->SetCommandFUp(boost::bind(&CGuildWarMixUI::CloseGWMix, this));

	if (pBase = findUI("btn_cancel"))
		pBase->SetCommandFUp(boost::bind(&CGuildWarMixUI::CloseGWMix, this));

	if (pBase = findUI("text_title"))
	{
		int l = pBase->GetPosX();
		int t = pBase->GetPosY();
		int r = l + pBase->GetWidth();
		int b = t + pBase->GetHeight();

		setTitleRect(l, t, r, b);
	}

	if (m_pBtnOk = (CUIButton*)findUI("btn_ok"))
		m_pBtnOk->SetCommandFUp(boost::bind(&CGuildWarMixUI::SendMixReq, this));

	CTString strName[GW_MIX_ITEM_SLOT_COUNT] = {"gem", "dust", "stone"};

	for (int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; ++i)
	{
		m_pIconsSlot[i] = (CUIIcon*)findUI(CTString("icon_" + strName[i]).str_String);

		if (m_pIconsSlot[i] == NULL)
			continue;

		m_pIconsSlot[i]->clearIconData();

		CmdDragIcon* pCmdD = new CmdDragIcon();
		pCmdD->setData(m_pIconsSlot[i]);
		m_pIconsSlot[i]->SetCommandDrag(pCmdD);

		CmdGuildWarMixDropItem* pCmdDBL = new CmdGuildWarMixDropItem();
		pCmdDBL->setData(this, i);
		m_pIconsSlot[i]->SetCommandDBL(pCmdDBL);

		m_pIconsSlot[i]->SetWhichUI(UI_GW_MIX);
	}

	SetCommandFUp(boost::bind(&CGuildWarMixUI::SetMixItem, this));

#endif		// WORLD_EDITOR
}

void CGuildWarMixUI::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CGuildWarMixUI::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CGuildWarMixUI::OpenGWMix()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If this is already exist
	if( pUIManager->DoesMessageBoxLExist( MSGLCMD_GW_MIX_REQ ) || IsVisible() )
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if( !pUIManager->GetInventory()->IsVisible() )
		pUIManager->GetInventory()->ToggleVisible();

	Hide(FALSE);
	pUIManager->RearrangeOrder( UI_GW_MIX, TRUE );
	
	m_nCurItemCount		= 0;
	setBtnEnable(false);
}

void CGuildWarMixUI::CloseGWMix()
{
	// Reset slot item
	for ( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		m_pIconsSlot[i]->clearIconData();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close Mix
	pUIManager->RearrangeOrder( UI_GW_MIX, FALSE );
	Hide(TRUE);
	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_GW_MIX );
}

void CGuildWarMixUI::SetMixItem ()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return;

	if (pDrag->getBtnType() != UBET_ITEM &&
		pDrag->GetWhichUI() != UI_INVENTORY)
	{
		pUIManager->ResetHoldBtn();
		return;
	}

	if (pDrag->GetWhichUI() == UI_GW_MIX)
	{
		ResetMixItem(pDrag);
		pUIManager->ResetHoldBtn();
		return;
	}

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return;

	CItemData*	pItemData = pItems->ItemData;	

	// �������� ���� ��������� Ȯ�� �Ѵ�.
	for( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		if (m_pIconsSlot[i]->IsEmpty() == false)
		{
			if (pItems->Item_UniIndex == m_pIconsSlot[i]->getItems()->Item_UniIndex)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 556, "�̹� ������� ������ �Դϴ�." ), SYSMSG_ERROR );	
				pUIManager->ResetHoldBtn();
				return;
			}
		}
	}

	int nSlot = 0;
	// �ش� �������� ��ġ�� ã�� �ش�.
	if( pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::IETC_MIX_TYPE1 )		// ��
	{
		nSlot = ITEM_GWMIX_TYPE1;
	}
	else if( pItemData->GetType() == CItemData::ITEM_ETC && 
			pItemData->GetSubType() == CItemData::IETC_MIX_TYPE2 )	// ����Ʈ
	{
		nSlot = ITEM_GWMIX_TYPE2;
	}
	else if( pItemData->GetType() == CItemData::ITEM_ETC && 
		pItemData->GetSubType() == CItemData::IETC_MIX_TYPE3 )		// ����
	{
		nSlot = ITEM_GWMIX_TYPE3;
	}
	else
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 1899, "���հ����� �������� �ƴմϴ�." ), SYSMSG_ERROR ); 
		pUIManager->ResetHoldBtn();
		return;
	}
	
	// ������� �ƴ϶�� �Է� �� �ش�.
	if (m_pIconsSlot[nSlot]->IsEmpty())
	{
		m_nCurItemCount++;	
	}

	CItems* pCopy = new CItems;
	memcpy(pCopy, pItems, sizeof(CItems));
	pCopy->Item_Sum = 1;
	m_pIconsSlot[nSlot]->setData(pCopy, false);

	if ( m_nCurItemCount >= GW_MIX_ITEM_SLOT_COUNT )
	{
		setBtnEnable(true);
	}
	else 
	{
		setBtnEnable(false);
	}
	
	pUIManager->ResetHoldBtn();

	// Lock arrange of inventory
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_GW_MIX );
}

void CGuildWarMixUI::ResetMixItem(int nIdx)
{
	if ( nIdx != -1 )
	{
		m_pIconsSlot[nIdx]->clearIconData();
		m_nCurItemCount--;
		setBtnEnable(false);
	}
}

void CGuildWarMixUI::ResetMixItem( CUIIcon* pIcon )
{
	if (pIcon == NULL)
		return;

	for (int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; ++i)
	{
		if (m_pIconsSlot[i] == NULL || m_pIconsSlot[i]->IsEmpty() == true)
			continue;

		if (m_pIconsSlot[i]->getItems()->Item_Index == pIcon->getItems()->Item_Index)
		{
			ResetMixItem(i);
			break;
		}
	}
}

void CGuildWarMixUI::SendMixReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 937, "��ų ����߿��� �������� ������ �� �����ϴ�." ), SYSMSG_ERROR );		
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 938, "���� �̵��߿��� �������� ������ �� �����ϴ�." ), SYSMSG_ERROR );	
		return;
	}

	SWORD	arrTab[GW_MIX_ITEM_SLOT_COUNT];
	SWORD	arrIdx[GW_MIX_ITEM_SLOT_COUNT];
	
	for( int i = 0; i < GW_MIX_ITEM_SLOT_COUNT; i++ )
	{
		arrTab[i] = m_pIconsSlot[i]->getItems()->Item_Tab;
		arrIdx[i] = m_pIconsSlot[i]->getItems()->InvenIndex;
	}
	
	_pNetwork->WarItemMixReq( arrTab, arrIdx );

	setBtnEnable(false);

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_GW_MIX );
}

void CGuildWarMixUI::MixRep( SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Close message box
	pUIManager->CloseMessageBox( MSGCMD_GW_MIX_REP );

	// Show result
	CTString	strMessage;
	switch( sbResult )
	{
	case MSG_ITEM_MIX_WARITEM_ERROR_OK:			// ���� ����
		strMessage = _S( 1900, "���� ������ ���տ� �����Ͽ����ϴ�." );	
		break;

	case MSG_ITEM_MIX_WARITEM_ERROR_NOTFOUND: // ��� ���� 
		strMessage =  _S( 1901, "��ᰡ �����Ͽ� ���� ������ ���տ� �����Ͽ����ϴ�." );		
		break;
	}

	CloseGWMix ();
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 1898, "���� ������ ����" ), UMBS_OK, UI_GW_MIX, MSGCMD_MIX_REP ); 
	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

WMSG_RESULT CGuildWarMixUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (GetHide() == TRUE || IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	UIMGR()->RearrangeOrder(UI_GW_MIX, TRUE);

	return CUIWindow::OnLButtonDown(x, y);
}
