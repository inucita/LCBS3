
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Common/CommonDef.h>
#include <Common/Packet/ptype_tradeAgent_system.h>
#include "UIAuctionNew.h"
#include <Engine/Contents/Base/Auction.h>
#include "AuctionCmd.h"
#include <Engine/Help/Util_Help.h>

//------------------------------------------------------------------------------

void CUIAuctionNew::initRegist()
{
	int		i;
	CUIText* pText = NULL;
	CTString strSign = _S(6028, "��");
	strSign += " ";

	m_pIconRegist = (CUIIcon*)findUI("icon_reg_target");
	m_pListRegist = (CUIList*)findUI("list_regist_input");

	if (m_pListRegist != NULL)
	{
		CUIListItem* pTemp = m_pListRegist->GetListItemTemplate();

		const int nStr[] = {
			2396,	// ����
			2397,	// ����
			1793,	// ������
			5961,	// ������
		};

		int nMax = m_pListRegist->getChildCount();
		CUIBase* pChild;

		for (i = 0; i < nMax; ++i)
		{
			pChild = m_pListRegist->getChildAt(i);

			if (pChild)
			{
				pText = (CUIText*)pChild->findUI("str_title");
				if (pText)
					pText->SetText( strSign + getText(nStr[i]));
			}
		}

		m_pListRegist->UpdateList();
		
	}

	m_pListRegistResult = (CUIList*)findUI("list_regist");
	{
		CmdSelectListReg* pCmd = new CmdSelectListReg;
		pCmd->setData(this);
		m_pListRegistResult->SetCommand(pCmd);
	}

	m_pEditQuantity = (CUIEditBox*)findUI("edit_quantity");
	{
		CmdCheckItemCount* pCmd = new CmdCheckItemCount;
		pCmd->setData(this);
		m_pEditQuantity->setInputCallback(pCmd);
	}
	m_pEditPrice = (CUIEditBox*)findUI("edit_price");
	{
		CmdCheckPrice* pCmd = new CmdCheckPrice;
		pCmd->setData(this);
		m_pEditPrice->setInputCallback(pCmd);
	}

	m_pTxtFee = (CUIText*)findUI("str_fee");
	m_pTxtDeposit = (CUIText*)findUI("str_deposit");

	m_pTxtRegCount = (CUIText*)findUI("str_reg_count");
	m_pTxtRegCount->SetText( CTString(0, "0 / %d", MAX_REG_COUNT) );

	m_pBtnRegist = (CUIButton*)findUI("btn_regist");

	if (m_pBtnRegist != NULL)
	{
		CmdRegist* pCmd = new CmdRegist;
		pCmd->setData(this);
		m_pBtnRegist->SetCommandUp(pCmd);
	}

	m_pBtnRegCancel = (CUIButton*)findUI("btn_reg_cancel");

	if (m_pBtnRegCancel != NULL)
	{
		CmdRegistCancel* pCmd = new CmdRegistCancel;
		pCmd->setData(this);
		m_pBtnRegCancel->SetCommandUp(pCmd);
	}

	m_pTbPremium = (CUITextBox*)findUI("tb_premiumchar");
	
	m_pDropArea = new CUIBase;
	m_pDropArea->InitPos(0, 0, 192, 430);
	CUIBase* pPage = findUI("tab_page_regist");
	if (pPage != NULL)
		pPage->addChild(m_pDropArea);
	else
		SAFE_DELETE(m_pDropArea);

	m_pTxtRegNas = (CUIText*)findUI("str_reg_nas");

	clearRegInput();
}

void CUIAuctionNew::clearRegInput()
{
	if (m_pEditQuantity)
		m_pEditQuantity->ClearInputBuffer();
	if (m_pEditPrice)
		m_pEditPrice->ClearInputBuffer();

	m_pTempRegist = NULL;

//	m_nQty = 0;
//	m_nPrice = 0;

	m_pBtnRegist->SetEnable(FALSE);

	m_pIconRegist->clearIconData();
}

bool CUIAuctionNew::CanRegister()
{
// 	if (m_bRegister)
// 	{
// 		_pNetwork->ClientSystemMessage(_S(4387, "��Ͽ�û�߿��� �� �� �����ϴ�."), SYSMSG_ERROR);
// 		return false;
// 	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return false;

	if (pDrag->getBtnType() == UBET_ITEM &&
		pDrag->GetWhichUI() == UI_INVENTORY)
	{
		CItems* pItems = pDrag->getItems();

		if (pItems == NULL)
			return false;

		CItemData*	pItemData = pItems->ItemData;

		if (pItemData->GetFlag() & ITEM_FLAG_NOTTRADEAGENT)	// �ŷ������ϺҰ�	
		{
			return false;
		}

		//������ ��� �Ұ�
		if((pItemData->GetType() == CItemData::ITEM_ETC && 
			pItemData->GetSubType() == CItemData::ITEM_ETC_MONEY))
		{
			return false;
		}

#ifdef ADD_SUBJOB
		if (pItemData->IsFlag(ITEM_FLAG_SELLER))
		{
			return (pUIManager->CheckSellerItem(UI_AUCTION, pItemData->GetFlag()) > 0);
		}
		else
#endif
		{
			// [2010/10/20 : Sora] ������� ���� �뺴 ī��� ��� �Ұ�
			if( pItemData->GetType() == CItemData::ITEM_ETC && 
				pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD &&
				pItems->Item_Used > 0 )
			{
				return false;
			}
			//������ �������� �ƴϸ鼭, �뿩, �÷�Ƽ�� �ν��͵� �ƴϾ�� �Ѵ�
			if( (pDrag->IsWearTab() == false) &&
				(pItemData->GetFlag() & ITEM_FLAG_EXCHANGE) && 
				!(pItems->IsFlag(FLAG_ITEM_LENT)) && 
				!(pItems->IsFlag(FLAG_ITEM_PLATINUMBOOSTER_ADDED)) &&
				!(pItems->IsFlag(FLAG_ITEM_BELONG)) )
			{
				return true;
			}
		}
	}

	return false;
}

void CUIAuctionNew::tempRegist()
{
	CUIIcon* pDrag = UIMGR()->GetDragIcon();

	if (pDrag == NULL)
		return;

	m_pTempRegist = pDrag->getItems();

	if (m_pTempRegist == NULL)
		return;
	
	m_pIconRegist->setData(m_pTempRegist);

	if (m_pEditQuantity)
		m_pEditQuantity->SetString("1");

	m_nQty = 1;

	if (m_pEditPrice)
	{
		m_pEditPrice->ClearInputBuffer();
		m_pEditPrice->SetFocus(TRUE);
		CUIFocus::getSingleton()->setUI(m_pEditPrice);

		CUIManager::getSingleton()->RearrangeOrder(UI_AUCTION, TRUE);
	}
}

void CUIAuctionNew::CheckRegist()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	m_nQty = 0;
	m_nPrice = 0;
	const char* pInput;

	pInput = m_pEditQuantity->GetString();

	if (pInput != NULL)
	{
		m_nQty = atoi(pInput);
	}

	pInput = m_pEditPrice->GetString();

	if (pInput != NULL)
	{
		m_nPrice = _atoi64(pInput);
	}

	if (m_pTempRegist != NULL)
		m_strRegname =_pNetwork->GetItemName(m_pTempRegist->Item_Index);

	RegisterItem();
}

void CUIAuctionNew::RegisterItem()
{
	if (m_pAuction == NULL || m_pTempRegist == NULL)
		return;

	m_pAuction->SendRegistItemReq( m_pTempRegist->Item_Tab, m_pTempRegist->InvenIndex,
		m_pTempRegist->Item_Index, m_nQty, m_nPrice );

	// �Է� ������ ����
	clearRegInput();
}

void CUIAuctionNew::RegCancel()
{
	if(m_nCurSelItem < 0)
	{
		return;
	}

	//_pNetwork->SendTradeAgentRegCancelReq(0);
}

void CUIAuctionNew::updateRegistList()
{
	if (m_pListRegistResult == NULL)
		return;

	int		i;
	int		nMax = m_pAuction->getRegistCount();

	m_pTxtRegCount->SetText( CTString(0, "%d/%d", nMax, MAX_REG_COUNT) );

	TradeAgentItem* pItem;	

	int nListItemCnt = m_pListRegistResult->getListItemCount();

	CUIListItem*	pListItem = m_pListRegistResult->GetListItemTemplate();
	CUIListItem*	pTmpItem;

	m_pListRegistResult->SetItemShowNum(nMax);

	for (i = 0; i < MAX_REG_COUNT; ++i)
	{
		if (i >= nListItemCnt)
		{
			m_pListRegistResult->AddListItem((CUIListItem*)pListItem->Clone());
		}

		pTmpItem = (CUIListItem*)m_pListRegistResult->GetListItem(i);

		if (pTmpItem == NULL)
			continue;

		if (i >= nMax)
		{
			clearResult(eRESULT_TYPE_REGIST, pTmpItem);
			continue;
		}

		pItem = m_pAuction->getRegItem(i);
		if (pItem == NULL)
		{
			continue;
		}

		updateResult(eRESULT_TYPE_REGIST, pItem, pTmpItem);
	}

	m_pListRegistResult->setCurSel(-1);
	m_pListRegistResult->UpdateList();

	updateFeeDeposit( m_pAuction->getDeposit(), m_pAuction->getFee() );

	m_pBtnRegCancel->SetEnable(FALSE);
}

void CUIAuctionNew::updateFeeDeposit( __int64 deposit, float fee )
{
	CTString	str;
	float fCommission = fee;
	
	str.PrintF("%.1f%%", fCommission);
	m_pTxtFee->SetText(str);
	str.PrintF("%I64d", deposit);
	UIMGR()->InsertCommaToString(str);
	m_pTxtDeposit->SetText(str);
}

void CUIAuctionNew::updatePremiumBenefit()
{
	if (m_pTbPremium != NULL && m_pAuction != NULL)
	{
		if (m_pAuction->GetPremiumBenefit() == true)
			m_pTbPremium->Hide(FALSE);
		else
			m_pTbPremium->Hide(TRUE);
	}
}

void CUIAuctionNew::checkItemCount()
{
	if (m_pTempRegist == NULL)
		return;

	const char* pInput;

	pInput = m_pEditQuantity->GetString();

	if (pInput != NULL)
	{
		m_nQty = _atoi64(pInput);

		if (m_nQty > 0 && m_nPrice > 0) 
			m_pBtnRegist->SetEnable(TRUE);
		else 
			m_pBtnRegist->SetEnable(FALSE);
	}
}

void CUIAuctionNew::checkPrice()
{
	const char* pInput;

	pInput = m_pEditPrice->GetString();

	if (pInput != NULL)
	{
		m_nPrice = _atoi64(pInput);	
		
		if (m_nQty > 0 && m_nPrice > 0)
			m_pBtnRegist->SetEnable(TRUE);
		else 
			m_pBtnRegist->SetEnable(FALSE);

	}
}

void CUIAuctionNew::SelListRegist()
{
	int nSel = m_pListRegistResult->getCurSel();

	if (nSel >= m_pAuction->getRegistCount())
		nSel = -1;

	if (nSel >= 0)
		m_pBtnRegCancel->SetEnable(TRUE);
	else
		m_pBtnRegCancel->SetEnable(FALSE);
}

void CUIAuctionNew::showRegOK()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(4287, "�ŷ� ���� ����"), UMBS_OK, UI_NONE, MSGCMD_NULL );

	MsgBoxInfo.AddString(_S(4304, "�ŷ� ���� ���񽺸� �̿��� �ּż� �����մϴ�"));

	CTString strNas;
	strNas.PrintF("%I64d", m_nPrice);
	pUIMgr->InsertCommaToString(strNas);
	MsgBoxInfo.AddString( CTString(0, _S(4305, "%s %ld���� %s������ ����ϼ̽��ϴ�."), m_strRegname, m_nQty, strNas) );

	LOG_DEBUG("deposti %I64d", m_pAuction->getDeposit());
	strNas.PrintF("%I64d", m_pAuction->getDeposit());
	pUIMgr->InsertCommaToString(strNas);
	MsgBoxInfo.AddString( CTString(0, _S(4306, "���������� %s������ �����ϼ̽��ϴ�."), strNas) );

	pUIMgr->CreateMessageBox(MsgBoxInfo);
}

void CUIAuctionNew::showMsgRegCancel()
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	CUIMsgBox_Info MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(4302, "������ ���"), UMBS_YESNO, UI_AUCTION, MSGCMD_AUCTION_CANCELREGISTER );

	MsgBoxInfo.AddString(_S(5981, "������ ����� ����Ͻðڽ��ϱ�?"));
	MsgBoxInfo.AddString(_S(5982, "(��� �� �������� ������ �� �����ϴ�.)"));

	pUIMgr->CreateMessageBox(MsgBoxInfo);
}
