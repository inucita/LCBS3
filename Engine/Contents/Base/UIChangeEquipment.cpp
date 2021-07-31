#include "stdh.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Contents/Base/UIChangeEquipment.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/ChangeWeapon.h>

#define DEF_LIST_INFO_COUNT 5
#define DEF_GOLD_TOKEN_IDX 5124

enum eERROR_MSG
{
	eERROR_NOT_TRADE_ITEM = 0,
	eERROR_NO_TRADE_ITEM,
	eERROR_NO_SELECT_TRADE_ITEM,
	eERROR_SEALED_ITEM,
	eERROR_ONLY_WEAPON,
	eERROR_ONLY_ARMOR,
	eERROR_WEARING_EQUIPMENT,
	eERROR_USE_SKILL,
	eERROR_USE_TELEPORT,
	eERROR_GOLDTOKEN_NOT_ENOUGH,
	eERROR_FULL_INVENTORY_WEAPON,
	eERROR_FULL_INVENTORY_ARMOR,
};

//--------------------------------------------------------------
// â �ݱ�
class CmdChangeEquipClose : public Command
{
public:
	CmdChangeEquipClose() : pUI_(NULL) {}
	void setData(CUIChangeEquipment* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->close();
	}
private:
	CUIChangeEquipment* pUI_;
};

//--------------------------------------------------------------
// Ȯ�� Ŭ��
class CmdChangeEquipOk : public Command
{
public:
	CmdChangeEquipOk() : pUI_(NULL) {}
	void setData(CUIChangeEquipment* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressOK();
	}
private:
	CUIChangeEquipment* pUI_;
};

//--------------------------------------------------------------
// ����Ʈ Ŭ��
class CmdChangeEquipClick : public Command
{
public:
	CmdChangeEquipClick() : pUI_(NULL) {}
	void setData(CUIChangeEquipment* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClickListItem();
	}
private:
	CUIChangeEquipment* pUI_;
};

CUIChangeEquipment::CUIChangeEquipment()
	: m_ptbContent(NULL)
	, m_psbList(NULL)
	, m_bDrag(false)
	, m_nGoldTokenCount(0)
	, m_nOriX(0), m_nOriY(0)
{
	m_eGuideState = eGUIDE_REGISTER_TRADE_ITEM;
	m_eChangeEquipState	= CHANGEEQUIP_WEAPON;
	int i;	
	for (i = 0; i < eBUTTON_MAX; i++)
	{
		m_pbtn[i] = NULL;
	}

	for (i = 0; i < eTEXT_MAX; i++)
	{
		m_pText[i] = NULL;
	}

	for (i = 0; i < eLIST_MAX; i++)
	{
		m_plist[i] = NULL;
	}

	for (i = 0; i < eICON_MAX; i++)
	{
		m_pIcon[i] = NULL;
	}

	setInherit(false);
}

void CUIChangeEquipment::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

void CUIChangeEquipment::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIChangeEquipment::PressOK()
{
	SendChangeEquipReq();
}

void CUIChangeEquipment::ClickListItem()
{
	refreshListInfo();
}

void CUIChangeEquipment::ShowSystemMsg( int nErrorCode )
{
	CTString strMessage;
	
	switch (nErrorCode)
	{
	case eERROR_NO_TRADE_ITEM:
		strMessage.PrintF( _S( 1076, "��ü�� �������� �����ϴ�." ) );
		break;
	case eERROR_NO_SELECT_TRADE_ITEM:
		strMessage.PrintF( _S( 2321,"���ο� ����� ���� �������ֽʽÿ�." ) );
		break;
	case eERROR_SEALED_ITEM:
		strMessage.PrintF( _S( 1079, "���ε� �������� ��ü�� �� �����ϴ�." ) );
		break;
	case eERROR_ONLY_WEAPON:
		strMessage.PrintF( _S( 1072, "���⸸ ��ü �� �� �ֽ��ϴ�.") );
		break;
	case eERROR_ONLY_ARMOR:
		strMessage.PrintF( _S( 2320, "���� ��ü �� �� �ֽ��ϴ�." ) );
		break;
	case eERROR_WEARING_EQUIPMENT:
		strMessage.PrintF( _S( 1071, "����� �������� ��ü�� �� �����ϴ�." ) );
		break;
	case eERROR_USE_SKILL:
		strMessage.PrintF( _S( 1074, "��ų ����߿��� �������� ��ü�� �� �����ϴ�." ) );
		break;
	case eERROR_USE_TELEPORT:
		strMessage.PrintF( _S( 1075, "���� �̵��߿��� �����۸� ��ü�� �� �����ϴ�." ) );
		break;
	case eERROR_NOT_TRADE_ITEM:
		strMessage.PrintF( _S( 7027, "�̰����� ��ü �� �� ���� ������ �Դϴ�.") );
		break;
	case eERROR_GOLDTOKEN_NOT_ENOUGH:
		strMessage.PrintF( _S( 7028, "��� ��ū�� �����Ͽ� ��ü�� �� �� �����ϴ�.") );
		break;
	case eERROR_FULL_INVENTORY_WEAPON:
		strMessage.PrintF( _S( 1329, "���⸦ ��ü �� �� �����ϴ�.") );
		break;
	case eERROR_FULL_INVENTORY_ARMOR:
		strMessage.PrintF( _S( 7029, "���� ��ü �� �� �����ϴ�.") );
		break;
	default:
		return;
	}

	_pNetwork->ClientSystemMessage(strMessage, SYSMSG_ERROR);
}

void CUIChangeEquipment::initialize()
{
	m_pIcon[eICON_ITEM] = (CUIIcon*)findUI("icon_item");
	m_pIcon[eICON_SUB] = (CUIIcon*)findUI("icon_sub");
	m_psbList = (CUIScrollBar*)findUI("list_scroll");
	m_ptbContent = (CUITextBox*)findUI("tb_content");

	CTString strBtn[eBUTTON_MAX] = { "btn_close", "btn_ok", "btn_no" };
	CTString strText[eTEXT_MAX] = { "move_title", "str_title", "str_listType" };
	CTString strList[eLIST_MAX] = { "list_item", "list_itemInfo" };
	int i;	
	for (i = 0; i < eBUTTON_MAX; i++)
	{
		m_pbtn[i] = (CUIButton*)findUI(strBtn[i]);
	}

	for (i = 0; i < eTEXT_MAX; i++)
	{
		m_pText[i] = (CUIText*)findUI(strText[i]);
	}

	for (i = 0; i < eLIST_MAX; i++)
	{
		m_plist[i] = (CUIList*)findUI(strList[i]);
	}

	// command ����
	if (m_pbtn[eBUTTON_CLOSE] != NULL)
	{
		CmdChangeEquipClose* pCmd = new CmdChangeEquipClose;
		pCmd->setData(this);
		m_pbtn[eBUTTON_CLOSE]->SetCommandUp(pCmd);
	}

	if (m_pbtn[eBUTTON_NO] != NULL)
	{
		CmdChangeEquipClose* pCmd = new CmdChangeEquipClose;
		pCmd->setData(this);
		m_pbtn[eBUTTON_NO]->SetCommandUp(pCmd);
	}

	if (m_pbtn[eBUTTON_OK] != NULL)
	{
		CmdChangeEquipOk* pCmd = new CmdChangeEquipOk;
		pCmd->setData(this);
		m_pbtn[eBUTTON_OK]->SetCommandUp(pCmd);
	}
}

void CUIChangeEquipment::open( eChangeEquipState eType, float x, float z )
{
	m_eChangeEquipState = eType;
	CUIManager* pUIManager = CUIManager::getSingleton();
	
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );

	m_fNpcX = x;
	m_fNpcZ = z;
	SetVisible(TRUE);
	Hide(FALSE);

	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (pData != NULL)
		pData->Clear();

	updateGuide(eGUIDE_REGISTER_TRADE_ITEM);
	UpdateUI();

	pUIManager->RearrangeOrder( UI_CHANGE_EQUIPMENT, TRUE );
	pUIManager->GetInventory()->Lock( TRUE, TRUE, LOCK_CHANGEWEAPON );
}

void CUIChangeEquipment::close()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );
	CUITooltipMgr::getSingleton()->setData(NULL);
	SetVisible(FALSE);
	Hide(TRUE);
	
	pUIManager->RearrangeOrder( UI_CHANGE_EQUIPMENT, FALSE );
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_CHANGEWEAPON );
}

void CUIChangeEquipment::UpdateUI()
{	
	if (refreshString() == false)
		close();

	if (refreshListItem() == false)
		close();

	refreshListInfo();

	clearIcon(eICON_ITEM);
	clearIcon(eICON_SUB);

	m_nGoldTokenCount = 0;
}

void CUIChangeEquipment::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (IsVisible() == FALSE)
		return;

	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		close();
	}
}


WMSG_RESULT CUIChangeEquipment::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

WMSG_RESULT CUIChangeEquipment::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pText[eTEXT_MOVE] && m_pText[eTEXT_MOVE]->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_CHANGE_EQUIPMENT, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CUIChangeEquipment::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_SUB] == NULL)
	{
		CUIManager::getSingleton()->ResetHoldBtn();
		return WMSG_FAIL;
	}

	bool bError = false;

	CUIIcon* pDrag = pUIManager->GetDragIcon();
	bool bHoldBtnEmpty = (pDrag == NULL);
	// ��� ��ū 
	if (m_pIcon[eICON_SUB]->IsInside(x, y) == TRUE && bHoldBtnEmpty == false)
	{
		if (pDrag->getIndex() != DEF_GOLD_TOKEN_IDX)
			bError = true;

		if (m_pIcon[eICON_ITEM]->IsEmpty() != false)
			bError = true;

		if (bError == true)
		{
			CUIManager::getSingleton()->ResetHoldBtn();
			return WMSG_FAIL;
		}

		CItems* pItems = pDrag->getItems();

		if (pItems != NULL)
		{
			CItemData*	pItemData = pItems->ItemData;

			if (pItemData != NULL)
			{
				pData->SetGoldTokenVirIndex(pItems->Item_UniIndex);
				m_pIcon[eICON_SUB]->setData(pItems);
				updateGuide(eGUIDE_ALL_READY);
			}		
		}		
	}
	// ������ �˻�
	else if (m_pIcon[eICON_ITEM]->IsInside(x, y) == TRUE && bHoldBtnEmpty == false)
	{
		// Error check
		if( pDrag->IsWearTab() == true)
		{
			ShowSystemMsg(eERROR_WEARING_EQUIPMENT);
			bError = true;
		}

		CItems* pItems = pDrag->getItems();		

		if (pItems == NULL)
			return WMSG_FAIL;

		CItemData*	pItemData = pItems->ItemData;

		if (pItemData == NULL)
			return WMSG_FAIL;

		if (pItems->Item_Flag & FLAG_ITEM_SEALED || pItems->Item_Flag & FLAG_ITEM_COMPOSITION)
		{
			ShowSystemMsg(eERROR_SEALED_ITEM);
			bError = true;
		}

		if (bError == true)
		{
			CUIManager::getSingleton()->ResetHoldBtn();
			return WMSG_FAIL;
		}

		{
			if(pItemData->IsFlag(ITEM_FLAG_RARE) == false)
			{
				ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
				CUIManager::getSingleton()->ResetHoldBtn();
				return WMSG_FAIL;
			}

			switch(m_eChangeEquipState)
			{
			case CHANGEEQUIP_WEAPON:
				{
					if (pItemData->GetType() != CItemData::ITEM_WEAPON)
					{
						ShowSystemMsg(eERROR_ONLY_WEAPON);
						bError = true;
					}
				}
				break;
			case CHANGEEQUIP_ARMOR:
				{
					if (pItemData->GetType() != CItemData::ITEM_SHIELD)
					{
						ShowSystemMsg(eERROR_ONLY_ARMOR);
						bError = true;
					}
				}
				break;
			}

			if (checkValidItem(pItemData->getindex()) == true && bError == false)
			{
				m_nGoldTokenCount = getConsumeTokenCnt(pItemData->getindex());

				if (m_nGoldTokenCount <= 0)
				{
					ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
					CUIManager::getSingleton()->ResetHoldBtn();
					return WMSG_FAIL;
				}
				
				if (pData != NULL)
				{
					pData->SetInvenTab(pItems->Item_Tab);
					pData->SetInvenIdx(pItems->InvenIndex);
					pData->SetVirtualIndex(pItems->Item_UniIndex);
					pData->SetGoldTokenCount(m_nGoldTokenCount);
				}
				m_pIcon[eICON_ITEM]->setData(pItems);
				clearIcon(eICON_SUB);
				updateGuide(eGUIDE_TRADE_FOR_NEED_ITEM, m_nGoldTokenCount);
				refreshListInfo();
			}
		}
	}

	CUIManager::getSingleton()->ResetHoldBtn();
	return WMSG_FAIL;
}

bool CUIChangeEquipment::refreshString()
{
	CTString strString[eTEXT_MAX];

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			strString[eTEXT_TITLE].PrintF( _S(1049, "���� ��ü"));
			strString[eTEXT_LISTNAME].PrintF( _S(1059, "���� ����Ʈ"));
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			strString[eTEXT_TITLE].PrintF( _S(2313, "�� ��ü"));
			strString[eTEXT_LISTNAME].PrintF( _S(2316, "Ŭ���� ����Ʈ"));
		}
		break;
	default:
		return false;
	}

	for (int i = 1; i < eTEXT_MAX; i++)
	{
		if (m_pText[i] == NULL)
		{
			LOG_DEBUG("CUIChangeEquipment::refreshString() ERROR %d String Null ptr", i);
			return false;
		}

		m_pText[i]->SetText(strString[i]);
	}

	return true;
}

bool CUIChangeEquipment::refreshListItem()
{
	if (m_plist[eLIST_ITEM] == NULL)
		return false;

	m_plist[eLIST_ITEM]->DeleteAllListItem();
	m_plist[eLIST_ITEM]->setCurSel(-1);

	if (m_psbList != NULL)
		m_psbList->SetScrollPos(0);

	int i, nMax = 0;
	CUIListItem* pTempItem = m_plist[eLIST_ITEM]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			nMax = DEF_TRADE_WEAPON_MAX - 1; // (�Ѱ��� - ��ȯ���� ��)
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			nMax = TOTAL_JOB - 1; // (����Ʈ ������ ����)
		}
		break;
	default:
		return false;
	}

	int nListString[CHANGEEQUIP_MAX][DEF_TRADE_WEAPON_MAX - 1] = { \
	{1038, 1039, 1040, 1041, 1042, 1043, 1044, 1045, 1046, 1047, 2306, 2307}, \
	{43, 44, 45, 46, 47, 48, 5732, 5820, 0, 0, 0, 0}		\
	};

	CTString strTemp;

	for (i = 0; i < nMax; i++)
	{
		m_plist[eLIST_ITEM]->AddListItem(pTempItem->Clone());
		pItem = (CUIListItem*)m_plist[eLIST_ITEM]->GetListItem( i );

		if (pItem == NULL)
			continue;

		CUIText* pText = (CUIText*)pItem->findUI("str_itemType");

		if (pText != NULL)
		{
			strTemp.PrintF(_S( nListString[m_eChangeEquipState][i], "����Ʈ ��Ʈ��"));
			pText->SetText(strTemp);
		}

		CmdChangeEquipClick* pCmd = new CmdChangeEquipClick;
		pCmd->setData(this);
		pItem->SetCommandUp(pCmd);	
	}

	m_plist[eLIST_ITEM]->UpdateList();
	m_plist[eLIST_ITEM]->UpdateScroll(nMax);
	return true;
}

bool CUIChangeEquipment::refreshListInfo()
{
	if (m_plist[eLIST_INFO] == NULL)
		return false;
	// ���� �ʱ�ȭ
	m_plist[eLIST_INFO]->DeleteAllListItem();

	// ���� ���� �˻�
	if (m_pIcon[eICON_ITEM] == NULL)
		return false;

	if (m_pIcon[eICON_ITEM]->IsEmpty() != false)
		return false;

	if (m_plist[eLIST_ITEM] == NULL)
		return false;

	// ��ȯ�� ������ �ε��� ���
	int nSelectItemIdx = m_pIcon[eICON_ITEM]->getIndex();
	int nSelectListIdx = m_plist[eLIST_ITEM]->getCurSel();

	if (nSelectListIdx < 0)
		return false;

	// ���� ����
	int i;
	int nTradeItemIdx = 0;
	CUIListItem* pTempItem = m_plist[eLIST_INFO]->GetListItemTemplate();
	CUIListItem* pItem = NULL;

	// ��Ȱ�� ������ ���� ���
	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			nTradeItemIdx = CChangeEquipment::getTradeWeaponIndex(nSelectItemIdx, nSelectListIdx);
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			nTradeItemIdx = CChangeEquipment::getTradeArmorIndex(nSelectItemIdx, nSelectListIdx);
		}
		break;
	default:
		return false;
	}

	if (nTradeItemIdx <= 0)
		return false;

	if (GAMEDATAMGR()->GetChangeWeapon() != NULL)
		GAMEDATAMGR()->GetChangeWeapon()->SetTradeItemIndex(nTradeItemIdx);

	// ���� Ÿ��Ʋ ��Ʈ�� �ε���
	int nListString[CHANGEEQUIP_MAX][DEF_LIST_INFO_COUNT] = { \
	{1065, 1066, 1067, 1068, 1069}, \
	{2317, 2318, 1067, 87, 0}		\
	};

	// String XML ID
	CTString strTitleID[DEF_LIST_INFO_COUNT] = {"str_nameTitle", "str_levelTitle", "str_classTitle", \
		"str_point1Title", "str_point2Title" };
	// String XML ID
	CTString strValueID[DEF_LIST_INFO_COUNT] = {"str_name", "str_level", "str_class", "str_point1", "str_point2" };

	CTString strTemp;
	m_plist[eLIST_INFO]->AddListItem(pTempItem->Clone());
	pItem = (CUIListItem*)m_plist[eLIST_INFO]->GetListItem(0);

	if (pItem == NULL)
	{
		m_plist[eLIST_INFO]->DeleteAllListItem();
		return false;
	}

	CUIText* pTextString = NULL;
	CUIText* pTextValue = NULL;

	// Ÿ��Ʋ ����
	for (i = 0; i < DEF_LIST_INFO_COUNT; i++)
	{
		pTextString = (CUIText*)pItem->findUI(strTitleID[i]);

		if (pTextString != NULL)
		{
			strTemp.PrintF(_S( nListString[m_eChangeEquipState][i], "Ÿ��Ʋ ��Ʈ��"));
			pTextString->SetText(strTemp);
		}
	}

	
	pTextString = (CUIText*)pItem->findUI(strTitleID[4]);

	if (pTextString != NULL)
	{
		if (m_eChangeEquipState == CHANGEEQUIP_ARMOR)
			pTextString->Hide(TRUE);
		else
			pTextString->Hide(FALSE);
	}

	// ������ ���� ����
	CTString strValue[DEF_LIST_INFO_COUNT];

	CItemData* pItemData = CItemData::getData(nTradeItemIdx);

	if (pItemData == NULL)
	{
		m_plist[eLIST_INFO]->DeleteAllListItem();
		return false;
	}	

	// ������ �̸�
	strValue[0] = pItemData->GetName();
	// ������ ����
	strValue[1].PrintF("%d", pItemData->GetLevel());
	// ������ ��� Ŭ����
	CUIManager::getSingleton()->GetClassOfItem(pItemData, strValue[2]);

	int nPoint1 = 0, nPoint2 = 0;
	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		{
			nPoint1 = pItemData->GetPhysicalAttack();
			nPoint2 = pItemData->GetMagicAttack();
		}
		break;
	case CHANGEEQUIP_ARMOR:
		{
			nPoint1 = pItemData->GetPhysicalDefence();
		}
		break;
	}
	// ���ݷ�, ����
	strValue[3].PrintF("%d", nPoint1);
	// ���� ���ݷ�, ����
	strValue[4].PrintF("%d", nPoint2);

	for (i = 0; i < DEF_LIST_INFO_COUNT; i++)
	{
		pTextValue = (CUIText*)pItem->findUI(strValueID[i]);

		if (pTextValue != NULL)
		{
			pTextValue->SetText(strValue[i]);
		}
	}

	pTextValue = (CUIText*)pItem->findUI(strValueID[4]);

	if (pTextValue != NULL)
	{
		if (m_eChangeEquipState == CHANGEEQUIP_ARMOR)
			pTextValue->Hide(TRUE);
		else
			pTextValue->Hide(FALSE);
	}

	m_plist[eLIST_INFO]->UpdateList();
	m_plist[eLIST_INFO]->UpdateScroll(1);
	return true;
}

bool CUIChangeEquipment::checkValidItem( int nItemIdx )
{
	bool bValid = false;

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		bValid = CChangeEquipment::IsValidWeapon(nItemIdx);
		break;
	case CHANGEEQUIP_ARMOR:
		bValid = CChangeEquipment::IsValidArmor(nItemIdx);
		break;
	}

	if (bValid == false)
	{
		ShowSystemMsg(eERROR_NOT_TRADE_ITEM);
		return false;
	}

	return true;
}

void CUIChangeEquipment::updateGuide( eGUIDE_STATE eType, int nCount )
{
	m_eGuideState = eType;

	if (m_ptbContent == NULL)
		return;

	int nGuideString[eGUIDE_MAX] = { 7030, 7031, 7032, };
	CTString strTemp;

	switch(m_eGuideState)
	{
	case eGUIDE_REGISTER_TRADE_ITEM:
		strTemp.PrintF(_S(7030, "��ü �ϰ� ���� �������� ��� �� �ּ���."));
		break;
	case eGUIDE_TRADE_FOR_NEED_ITEM:
		strTemp.PrintF(_S(7031, "��� �� �������� ��ü �ϱ� ���ؼ��� ��� ��ū %d���� �ʿ� �մϴ�."), nCount);
		break;
	case eGUIDE_ALL_READY:
		strTemp.PrintF(_S(7032, "��ü �� �������� ���� �� Ȯ�� ��ư�� Ŭ�� �ϸ� ��ü�� �Ϸ� �˴ϴ�."));
		break;
	default:
		return;
	}

	m_ptbContent->SetText(strTemp);
}

int CUIChangeEquipment::getSelectIdx( int nSelIdx )
{
	int ret = -1;

	switch(m_eChangeEquipState)
	{
	case CHANGEEQUIP_WEAPON:
		ret = nWeaponSelType[nSelIdx];
		break;
	case CHANGEEQUIP_ARMOR:
		ret = nClassSelType[nSelIdx];
		break;
	}
	return ret;
}

int CUIChangeEquipment::getConsumeTokenCnt( int nItemIdx )
{
	CItemData* pItemData = CItemData::getData(nItemIdx);

	if (pItemData == NULL)
		return 0;

	int nlevel = pItemData->GetLevel();
	int nCnt = 0;

	if (nlevel >= 70 && nlevel <= 100)
		nCnt = 1;
	else if (nlevel > 100 && nlevel <= 130)
		nCnt = 2;
	else if (nlevel > 130 && nlevel <= 140)
		nCnt = 3;
	else if (nlevel > 140 && nlevel <= 150)
		nCnt = 4;
	else if (nlevel > 150 && nlevel <= 175)
		nCnt = 5;
	else
		nCnt = 0;

	return nCnt;
}

void CUIChangeEquipment::clearIcon( int nType )
{
	if (nType < eICON_ITEM || nType >= eICON_MAX)
		return;

	int nStringIdx[eICON_MAX] = { 7033, 7034, };
	CTString strToolTip;

	if (m_pIcon[nType] != NULL)
	{
		m_pIcon[nType]->clearIconData();
		strToolTip.PrintF(_S(nStringIdx[nType], "���� ����"));
		m_pIcon[nType]->setTooltip(strToolTip);
	}
}

void CUIChangeEquipment::SendChangeEquipReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_pIcon[eICON_ITEM] == NULL)
	{
		ShowSystemMsg(eERROR_NO_TRADE_ITEM);
		return;
	}

	if (m_pIcon[eICON_ITEM]->IsEmpty() != false)
	{
		ShowSystemMsg(eERROR_NO_TRADE_ITEM);
		return;
	}

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		ShowSystemMsg(eERROR_USE_SKILL);
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		ShowSystemMsg(eERROR_USE_TELEPORT);
		return;
	}

	if (m_plist[eLIST_ITEM] == NULL)
	{
		ShowSystemMsg(eERROR_NO_SELECT_TRADE_ITEM);
		return;
	}

	int nSelIdx = m_plist[eLIST_ITEM]->getCurSel();

	if( nSelIdx < 0 )
	{
		ShowSystemMsg(eERROR_NO_SELECT_TRADE_ITEM);
		return;
	}

	if (m_pIcon[eICON_SUB] == NULL)
	{
		ShowSystemMsg(eERROR_GOLDTOKEN_NOT_ENOUGH);
		return;
	}

	if (m_pIcon[eICON_SUB]->IsEmpty() != false)
	{
		ShowSystemMsg(eERROR_GOLDTOKEN_NOT_ENOUGH);
		return;
	}

	int nCount = pUIManager->GetInventory()->GetItemCount(DEF_GOLD_TOKEN_IDX);
	if ( m_nGoldTokenCount > nCount )
	{
		ShowSystemMsg(eERROR_GOLDTOKEN_NOT_ENOUGH);
		return;
	}

	ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

	if (pData == NULL)
		return;

	pData->SetSelectIdx(getSelectIdx(nSelIdx));

	pUIManager->CloseMessageBox( MSGCMD_CHANGEWEAPON_REP );

	// Create message box of remission
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2308, "����� �� ��ü" ), UMBS_OKCANCEL,UI_CHANGE_EQUIPMENT, MSGCMD_CHANGEWEAPON_REP );
	MsgBoxInfo.AddString( _S(6290, "������ ���� �� �� ��ü �� ���� �� ��ϰ� ���� �� ������ �Ű������� ������ �ɼ��� �̰��� ���·� ���� �˴ϴ�." ) );
	pUIManager->CreateMessageBox( MsgBoxInfo );
}

void CUIChangeEquipment::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_CHANGEWEAPON_REP:		// ���� ��ü Ȯ��.
		{
			if( bOK )
			{
				ChangeWeapon* pData = GAMEDATAMGR()->GetChangeWeapon();

				if (pData != NULL)
					pData->SendChangeEquipment(FALSE);
			}
		}
		break;
	}
}
