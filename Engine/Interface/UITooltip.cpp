#include "StdH.h"
#include <Engine/Entities/ItemData.h>

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITooltipResource.h>
#include <Engine/Interface/UIPetInfo.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UINickName.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Contents/function/ShopUI.h>
#include <Engine/Interface/UICashShopEx.h>

#include <Engine/Help/Util_Help.h>
#include <Engine/Help/ItemHelp.h>

#include <Engine/Contents/Base/InvenData.h>
#include <Engine/Contents/Base/Syndicate.h>
#include <Engine/contents/Base/UICharacterInfoNew.h>
#include <Engine/Contents/function/ItemCollectionData.h>
#include <Engine/Contents/function/PremiumChar.h>

#include <GameDataManager/GameDataManager.h>
#include <Engine/Info/MyInfo.h>

#include <Common/header/def_item_collection.h>

extern INDEX g_iCountry;

#define DEF_LIST_ORIGIN_W	(150)
#define DEF_LIST_ORIGIN_H	(800)
#define DEF_LIST_SECOND_GAP	(15)
#define DEF_LIST_SECOND_GAP_Y	(9)

CUITooltip::CUITooltip()
    : m_pItem(NULL)
	, m_pItemEtc(NULL)
    , m_pItemData(NULL)
    , m_pSkill(NULL)
	, m_pSSkill(NULL)
    , m_pAction(NULL)
    , m_pImgBack(NULL)
    , m_pUI(NULL)
    , m_nIndex(-1)
    , m_nSkillLv(0)
    , m_bSkillLearn(false)
    , m_eType(eTOOLTIP_FIRST)
    , m_nSecondPos(-1)
	, m_nCooltimePos(-1)
	, m_nWhichUI(-1)
{
    Hide(TRUE);

    int i;

    for (i = 0; i < 2; ++i)
    {
        m_pImgTempBack[i] = NULL;
    }

	for (i = 0; i < eLIST_END; ++i)
	{
		m_pList[i] = NULL;
	}
}

CUITooltip::~CUITooltip()
{
    m_pItem		= NULL;
    m_pItemData = NULL;
    m_pSkill	= NULL;
	m_pSSkill	= NULL;
    m_pAction	= NULL;
    m_pImgBack	= NULL;
    m_pUI		= NULL;

    int i;

    for (i = 0; i < 2; ++i)
    {
        m_pImgTempBack[i] = NULL;
    }

	for (i = 0; i < eLIST_END; ++i)
	{
		m_pList[i] = NULL;
	}
}

void CUITooltip::initialize()
{
    CUITooltipMgr* pTooltipMgr = CUITooltipMgr::getSingleton();

    if (m_pImgTempBack[0] == NULL)
    {
        m_pImgTempBack[0] = (CUIImageSplit*)pTooltipMgr->GetTooltipRes()->CloneNormalBack();
        addChild(m_pImgTempBack[0]);
    }

    if (m_pImgTempBack[1] == NULL)
    {
        m_pImgTempBack[1] = (CUIImageSplit*)pTooltipMgr->GetTooltipRes()->CloneStringBack();
        addChild(m_pImgTempBack[1]);
    }

	for (int i = 0; i < eLIST_END; ++i)
	{
		if (m_pList[i] == NULL)
		{
		    m_pList[i] = new CUIList;

		    addChild((CUIBase*)m_pList[i]);

		    m_pList[i]->SetSize(DEF_LIST_ORIGIN_W, DEF_LIST_ORIGIN_H);
		    m_pList[i]->SetGap(1);
		}
	}
}

void CUITooltip::GetTooltipSize(int& w, int& h)
{
    if (m_pImgBack == NULL)
        return;

    w = m_pImgBack->GetWidth();
    h = m_pImgBack->GetHeight();
}

void CUITooltip::SetIndex()
{
    if (_pNetwork->m_ubGMLevel <= 1)
        return;

    CUITooltipMgr* pTooltipMgr = CUITooltipMgr::getSingleton();

    CUIText* pText = (CUIText*)pTooltipMgr->GetTooltipRes()->CloneText();

    if (pText == NULL)
        return;

    CTString strIndex;

    strIndex.PrintF("Index : [%d]", m_nIndex);
    pText->SetText(strIndex);

    m_pList[eLIST_FIRST]->AddListItem((CUIBase*)pText);
}

bool CUITooltip::SetString(CUIBase* pUI)
{
	if (pUI == NULL || m_pUI == pUI)
		return false;

	pUI->procSetTooltipFunc();

	if (pUI->hasTooltip() == false)
		return false;

	Hide(FALSE);

	m_pUI = pUI;

	CUIBase::vec_tooltip vec = pUI->getTooltipCont();
	CUIBase::vec_tooltip_iter bIt = vec.begin();
	CUIBase::vec_tooltip_iter eIt = vec.end();

	CTString strTooltip;
	CDrawPort* pDraw = CUIManager::getSingleton()->GetDrawPort();
	int nTooltipWidth = pUI->getTooltipWidth();
	int nStrW = nTooltipWidth;
	int nTemp = 0;
	bool bTooltip = false;

	// ���� width�� ���������� �ʾҴٸ� �����̳ʿ� �ִ� ������ ���� ū ���ڿ��� width�� ����.
	if (nTooltipWidth <= 0)
	{
		for (; bIt != eIt; ++bIt)
		{	
			strTooltip = (*bIt).str;
	
			if (strTooltip.IsEmpty() == TRUE)
				continue;
	
			if (g_iCountry != RUSSIA)
			{
				nTemp = pDraw->GetTextWidth2(strTooltip);
			}
			else
			{
				extern CFontData *_pfdDefaultFont;
				nTemp = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, strTooltip.str_String);
			}
	
			// �Էµ� ��Ʈ���߿� ���� ū ���ڿ� ���̷� ����.
			if (nStrW < nTemp)
				nStrW = nTemp;
		}

		// begin() �ٽ� ����, �Ʒ����� ��� �ؾ� �ϱ� ����.
		bIt = vec.begin();
	}

	m_pList[eLIST_FIRST]->SetWidth(nStrW);

	for (; bIt != eIt; ++bIt)
	{	
		strTooltip = (*bIt).str;
		
		if (strTooltip.IsEmpty() == TRUE)
			continue;

		AddText(strTooltip, (*bIt).colText, (*bIt).align_h);
		bTooltip = true;
	}

	if (bTooltip == false)
		return false;

	UpdateTooltip(m_pImgTempBack[1]);
	return true;
}

void CUITooltip::ClearData()
{
    Hide(TRUE);

	for (int i = 0; i < eLIST_END; ++i)
	{
		if (m_pList[i] != NULL)
		    m_pList[i]->DeleteAllListItem();
	}

    if (m_pImgBack != NULL)
        m_pImgBack->Hide(TRUE);

    m_nIndex	= -1;
	m_pItem		= NULL;
	m_pItemData = NULL;
    m_pSkill	= NULL;
	m_pSSkill	= NULL;
    m_bSkillLearn = false;
    m_pAction	= NULL;
    m_pUI		= NULL;
	m_pItemEtc	= NULL;
    m_nSecondPos = -1;
	m_nCooltimePos = -1;
	m_nWhichUI = -1;
}

bool CUITooltip::SetCashData( CItems* pItems, int nCashIndex )
{
	if (m_pList[eTOOLTIP_FIRST] == NULL)
		return false;

	if (pItems == NULL)
		return false;

	if (pItems == m_pItem)
		ClearData();

	Hide(FALSE);

	m_pItem = pItems;

	// ĳ������Ÿ ���ÿ����� �������ε����� ������� �ʰ� ĳ���ε����� ����Ѵ�.
	m_nIndex = nCashIndex;

	m_pList[eLIST_FIRST]->SetWidth(210);

	SetCashItemInfo();
	SetCashPackageItemInfo();
	UpdateTooltip(m_pImgTempBack[0]);

	return true;

}

void CUITooltip::SetCashItemInfo()
{
	// ĳ���ε����� 0���� �۴ٸ� ��Ű�� �������̴�.
	if (m_nIndex < 0)
		return;

	CTString strTmp;
	CTString strCash;
	COLOR col = DEF_UI_COLOR_WHITE;

	CCashShopData::CASH_SHOP_DATA* pCashData = _pNetwork->GetCashShopData().GetCashShopData(m_nIndex);

	if (pCashData == NULL)
		return;

	// ĳ�������� �̸�.
	col = 0xFF63B1FF;
	AddText(pCashData->m_itemName, col, eALIGN_H_LEFT, eLIST_FIRST, 34);

	strCash.PrintF("%d", pCashData->m_cash);
	UIMGR()->InsertCommaToString(strCash);
	strTmp.PrintF(_S(255, "����: %s"), strCash);
	col = 0x6BD2FFFF;
	AddText(strTmp, col, eALIGN_H_LEFT, eLIST_FIRST, 34);

	strTmp.PrintF(_S(5217, "����: %d"), pCashData->m_itemListCnt);
	col = 0x6BD2FFFF;
	AddText(strTmp, col, eALIGN_H_LEFT, eLIST_FIRST, 34);

	if (UIMGR()->GetCashShopEX()->IsVFT_GoodList() == true)
	{
		if (pCashData->m_limitCnt > 0)
		{
			col = 0x6BD2FFFF;
			strTmp.PrintF(_S(2890, "���� ����: %d"), pCashData->m_limitCnt);
			AddText(strTmp, col, eALIGN_H_LEFT, eLIST_FIRST, 34);
		}
		else if (pCashData->m_limitCnt == 0)
		{
			col = 0xFF63B1FF;
			strTmp.PrintF(_S(2891, "���� ���� : ǰ��"));
			AddText(strTmp, col, eALIGN_H_LEFT, eLIST_FIRST, 34);
		}
	}

	// ����.
	AddText(pCashData->m_itemDesc, 0xFFF284FF, eALIGN_H_LEFT, eLIST_FIRST, 34);

	if (m_pItem->ItemData->GetType() == CItemData::ITEM_WEAPON || 
		(m_pItem->ItemData->GetType() == CItemData::ITEM_SHIELD && m_pItem->ItemData->GetSubType() != CItemData::ITEM_SHIELD_BACKWING))
	{
		if (!pCashData->IsPackageEx())
		{
			CItemData* pItemData = _pNetwork->GetItemData(m_pItem->Item_Index);

			if (pItemData == NULL)
				return;

			int nJob = -1;

			for (int i = 0; i < TOTAL_JOB; ++i)
			{
				if (pItemData->GetJob() == (1 << i))
				{
					nJob = i;
					break;
				}
			}

			if (nJob < 0)
				return;

			strTmp.PrintF(_S(5222, "������ �� �� �ֽ��ϴ�.(%s)"),
				CJobInfo::getSingleton()->GetName(nJob));
			AddText(strTmp, 0x6060FFFF, eALIGN_H_LEFT, eLIST_FIRST, 34);
		}
	}
}

void CUITooltip::SetCashPackageItemInfo()
{
	// ĳ���ε����� 0���� ũ�ų� ���ٸ� ��Ű�� �������� �ƴϹǷ� ����ó��.
	if (m_nIndex >= 0 && m_pItem->ItemData == NULL)
		return;

	CTString strTmp;
	CTString strCash;
	COLOR col = DEF_UI_COLOR_WHITE;
	
	// ĳ�������� �̸�.
	col = 0xFF63B1FF;
	AddText(_pNetwork->GetItemName(m_pItem->ItemData->GetItemIndex()), col, eALIGN_H_LEFT, eLIST_FIRST, 34);

	strTmp.PrintF(_S(5217, "����: %d"), m_pItem->Item_Sum);
	col = 0x6BD2FFFF;
	AddText(strTmp, col, eALIGN_H_LEFT, eLIST_FIRST, 34);

	// ����.
	AddText(_pNetwork->GetItemDesc(m_pItem->ItemData->GetItemIndex()), 0xFFF284FF, eALIGN_H_LEFT, eLIST_FIRST, 34);

	if (m_pItem->ItemData->GetType() == CItemData::ITEM_WEAPON || 
		(m_pItem->ItemData->GetType() == CItemData::ITEM_SHIELD && m_pItem->ItemData->GetSubType() != CItemData::ITEM_SHIELD_BACKWING))
	{
		int nJob = -1;

		for (int i = 0; i < TOTAL_JOB; ++i)
		{
			if (m_pItem->ItemData->GetJob() == (1 << i))
			{
				nJob = i;
				break;
			}
		}

		if (nJob < 0)
			return;

		strTmp.PrintF(_S(5223, "����Ŭ������ ������ �� �� �ֽ��ϴ�.(%s)"),
			CJobInfo::getSingleton()->GetName(nJob));
		AddText(strTmp, 0x6060FFFF, eALIGN_H_LEFT, eLIST_FIRST, 34);
	}
}

bool CUITooltip::SetItemData(CItems* pItems, eTOOLTIP_TYPE type /*= eTOOLTIP_FIRST*/, int nWearPos /*= -1*/)
{
    if (m_pList[type] == NULL)
        return false;

    if (pItems == NULL)
        return false;

	if (pItems == m_pItem)
		ClearData();

	if (pItems->ItemData == NULL)
		return false;

	if (pItems->ItemData->GetItemIndex() <= 0)
		return false;

    Hide(FALSE);

	m_pItem = pItems;
    m_pItemData = m_pItem->ItemData;
    m_nIndex = m_pItemData->GetItemIndex();
    m_eType = type;

    if (m_pItemData == NULL)
		return false;

	m_pList[eLIST_FIRST]->SetWidth(DEF_LIST_ORIGIN_W);

 	if (m_eType == eTOOLTIP_FIRST || nWearPos >= 0)
 	{
 		SetIndex();
 
 		if (m_eType == eTOOLTIP_FIRST && nWearPos >= 0)
 			SetWearInfo();
 
 		SetName();
		SetCompositionInfo();
 		SetExpireInfo();
 		SetPriceInfo();
 		SetRuneInfo();
 		SetReformInfo();
		SetCharBoundInfo();
 		SetClassInfo();
 		SetLevelInfo();
 		SetSyndicateInfo();
 		SetPotionBuff();
 		SetQuestInfo();
 		SetDamageInfo();
 		SetDefenceInfo();
 		SetFlightInfo();
 		SetPenaltyInfo();
 		SetDurabilityInfo();
 		SetEndureInfo();
 		SetFameInfo();
 		SetUnknownInfo();
 		SetPetInfo();
 		SetWildPetInfo();
 		SetSocketInfo();
 		SetJewelInfo();
 		SetOptionInfo();
 		SetBloodInfo();
 		SetSealInfo();
 		SetSkillInfo();
 		SetMonsterMercenaryInfo();
 		SetJewelDescInfo();
		SetItemCoolTimeInfo();
 		SetDescInfo();
 		SetTitleInfo();
 		SetSetItemInfo();
 		SetSmcInfo();
 	}
 	else //if (m_eType == eTOOLTIP_SECOND)
 	{
 		if (SetFortuneInfo() == false)
 			return false;
 	}

	UpdateTooltip(m_pImgTempBack[0]);

    return true;
}

bool CUITooltip::SetItemEtcData( stItemEtc* pItemEtc, eUIBTN_ETC_TYPE type )
{
	if (pItemEtc == NULL)
		return false;

	if (pItemEtc == m_pItemEtc)
		ClearData();

	m_pItemEtc = pItemEtc;

	Hide(FALSE);

	m_pList[eLIST_FIRST]->SetWidth(DEF_LIST_ORIGIN_W);

	SetAmendInfo(type);
	SetAffinity(type);

	UpdateTooltip(m_pImgTempBack[0]);
	return true;
}

void CUITooltip::SetAmendInfo(eUIBTN_ETC_TYPE type)
{
	if (type > eETC_TYPE_THEME)
		return;

	CTString strCount;
	CTString strTmp;

	COLOR col = DEF_UI_COLOR_WHITE;

	switch(type)
	{
	case eETC_TYPE_NAS:
		{
			strCount.PrintF("%I64d", m_pItemEtc->llEtc);

			UIMGR()->InsertCommaToString( strCount );
			col = UIMGR()->GetNasColor( m_pItemEtc->llEtc );

			strTmp.PrintF(_S(1762, "����"));

			strTmp = strCount + strTmp;
		}
		break;
	case eETC_TYPE_EXP:
		{
			strCount.PrintF("%I64d", m_pItemEtc->llEtc);
			UIMGR()->InsertCommaToString( strCount );

			strTmp.PrintF(_S(4514, "����ġ"));

			strTmp = strCount + strTmp;
		}
		break;
	case eETC_TYPE_SP:
		{
			strCount.PrintF("%I64d", m_pItemEtc->llEtc);
			UIMGR()->InsertCommaToString( strCount );

			strTmp.PrintF(_S(5249, "���õ�"));

			strTmp = strCount + strTmp;
		}
		break;
	case eETC_TYPE_THEME:
		{
			CItemCollectionData* pData = CItemCollectionData::GetTheme(m_pItemEtc->nIndex);

			if (pData == NULL)
				return;

			// �̸�
			strTmp = pData->GetName();
			AddText(strTmp, col);

			// ����.
			strTmp = pData->GetDesc();
			col = 0x9E9684FF;
		}
		break;
	}

	AddText(strTmp, col);
}

void CUITooltip::SetAffinity(eUIBTN_ETC_TYPE eType)
{
	if (eType < eETC_TYPE_AFFINITY_ITEM)
		return;

	CTString strTemp;
	CTString strPoint;
	int nEtc = (int)m_pItemEtc->llEtc;

	switch(eType)
	{
	case eETC_TYPE_AFFINITY_ITEM:
		{
			strPoint.PrintF("%d", nEtc);
			UIMGR()->InsertCommaToString(strPoint);

			strTemp.PrintF(_S(5857, "%s, ģȭ���� %s ��� �����ݴϴ�."),
				_pNetwork->GetItemDesc(m_pItemEtc->nIndex), strPoint);
		}
		break;
	case eETC_TYPE_AFFINITY_QUEST:
		{
			strPoint.PrintF("%d", nEtc);
			UIMGR()->InsertCommaToString(strPoint);

			strTemp.PrintF(_S(5858, "%s ���� ����Ʈ, �Ϸ�� ģȭ���� %s ��� �����ݴϴ�."),
				_pNetwork->GetAffinityData()->GetAffinityDataByIndex(m_pItemEtc->nIndex)->GetName()
				, strPoint);
		}
		break;
	case eETC_TYPE_AFFINITY_MONSTER:
		{
			int nZoneNum = UTIL_HELP()->GetZoneByMobIndex(m_pItemEtc->nIndex);
			CTString strZone;

			if ( nZoneNum < 0 )
				strZone.PrintF( _S(5892, "�̰���") );
			else
				strZone = CZoneInfo::getSingleton()->GetZoneName( nZoneNum );

			strPoint.PrintF("%d", nEtc);
			UIMGR()->InsertCommaToString(strPoint);

			strTemp.PrintF( _S(5864, "[%s] %d Lv %s óġ �� ģȭ���� %s ��� �����ݴϴ�."),
				strZone, CMobData::getData(m_pItemEtc->nIndex)->GetLevel(),
				CTString(CMobData::getData(m_pItemEtc->nIndex)->GetName()),
				strPoint);
		}
		break;
	case eETC_TYPE_AFFINITY_REWARD:
		{
			strPoint.PrintF("%d", nEtc);
			UIMGR()->InsertCommaToString(strPoint);

			strTemp.PrintF( _S(5866, "ģȭ�� %s �޼� �� %s �������� �������� ���� �� �ֽ��ϴ�."),
				strPoint, _pNetwork->GetItemName(m_pItemEtc->nIndex));
		}
		break;
	}

	if (strTemp.IsEmpty() == TRUE)
		return;

	AddText(strTemp);
}

void CUITooltip::SetWearInfo()
{
	AddText(_S(6208, "�������� ���"));
}

void CUITooltip::SetName()
{
    CTString strTmp;
    CTString strItemName = _pNetwork->GetItemName(m_pItemData->GetItemIndex());
    CTString strItemCnt = "";
    COLOR	 col = DEF_UI_COLOR_WHITE;
    int		 nRareGrade = -1;

    if (strItemName.IsEmpty() == TRUE)
        return;

	 strTmp.PrintF("%s", strItemName);

    CUIManager* pUIManager = CUIManager::getSingleton();
	UtilHelp* pHelp = UtilHelp::getSingleton();

    if (m_pItemData->IsFlag(ITEM_FLAG_RARE))
    {
        CItemRareOption* rItemRare = CItemRareOption::getData(m_pItem->Item_RareIndex);

		if (rItemRare != NULL )
		{
			if (strlen(rItemRare->GetPrefix()) > 0)
				strItemName.PrintF("%s %s", rItemRare->GetPrefix(), strTmp);

			col = pHelp->GetRareItemColor(m_pItem->Item_RareIndex, m_pItemData->GetFlag(), m_pItem->Item_RareIndex > 0 ? 1 : 0);
		}        
    }

    if (m_pItemData->IsFlag(ITEM_FLAG_COUNT))
    {
        strItemCnt.PrintF("%I64d", m_pItem->Item_Sum);

        pUIManager->InsertCommaToString(strItemCnt);
        col = pUIManager->GetNasColor(m_pItem->Item_Sum);

		if (m_pItem->Item_Sum <= 0)
			strTmp.PrintF("%s", strItemName);
		else
			strTmp.PrintF("%s(%s)", strItemName, strItemCnt);
    }
    else if (m_pItemData->GetType() == CItemData::ITEM_ACCESSORY &&
			(m_pItemData->GetSubType() == CItemData::ACCESSORY_PET ||
			 m_pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET))
    {
		const INDEX iPetIndex = m_pItem->Item_Plus;
		SBYTE sbPetTypeGrade = pUIManager->GetPetInfo()->GetPetTypeGrade(iPetIndex);

		if (sbPetTypeGrade > 0)
		{
		    INDEX iPetType = 0;
		    INDEX iPetAge = 0;
		    CTString strPetName = PetInfo().GetName((int)iPetType, (int)iPetAge);

		    _pNetwork->CheckPetType(sbPetTypeGrade, iPetType, iPetAge);

		    strTmp.PrintF("%s - %s", strItemName, strPetName);
		}
    }
	else if (m_pItemData->GetType() == CItemData::ITEM_ONCEUSE &&
			m_pItemData->GetSubType() == CItemData::ITEM_SUB_TITLE)
	{
		int nCustomTitleIndex = m_pItem->Item_Used2;

		if (CustomTitleData::IsKeepTitleInfo(nCustomTitleIndex) == true)
			strTmp = CustomTitleData::m_mapCustomTitleItemInfo[nCustomTitleIndex]->name;
		else
			strTmp = strItemName;
	}
    else
    {
        strTmp = strItemName;

        if (m_pItem->Item_Flag & FLAG_ITEM_BOOSTER_ADDED)
        {
            strTmp.PrintF(_S(1929, "�ν�Ʈ %s"), strItemName);
        }
        else if (m_pItem->Item_Flag & FLAG_ITEM_SILVERBOOSTER_ADDED)
        {
            strTmp.PrintF(_S(2429, "�ǹ� �ν�Ʈ %s"), strItemName);
        }
        else if (m_pItem->Item_Flag & FLAG_ITEM_GOLDBOOSTER_ADDED)
        {
            strTmp.PrintF(_S(2430, "��� �ν�Ʈ %s"), strItemName);
        }
        else if (m_pItem->Item_Flag & FLAG_ITEM_PLATINUMBOOSTER_ADDED)
        {
            strTmp.PrintF(_S(2639, "�÷�Ƽ�� �ν�Ʈ %s"), strItemName);
        }
        else if (m_pItem->Item_Flag & FLAG_ITEM_LENT)
        {
            strTmp = _S(3046, "�뿩 ") + strTmp;
        }
        else if (m_pItem->Item_Plus > 0)
        {
			if( ( ( m_pItemData->GetType() == CItemData::ITEM_WEAPON && !m_pItemData->IsProductionItem() )||
				m_pItemData->GetType() == CItemData::ITEM_SHIELD ) )
			{
				// �� �߿�, �ѹ��ǻ��� ��ȭ ��ġ ǥ�� ����
				if ((m_pItemData->GetType() == CItemData::ITEM_SHIELD &&
					m_pItemData->GetSubType() == CItemData::ITEM_SHIELD_ONE_SUIT) == false)
				{
					int nPlatinum_Ent, nEnt;
					CTString strEnt;
					FLAG_ITEM_PLATINUM_GET(m_pItem->Item_Flag, nPlatinum_Ent);
					
					nEnt = m_pItem->Item_Plus - nPlatinum_Ent;

					// �ǻ� ���� �������� ��� ��þƮ ���� ����������..
					// �� ��Ȳ���� DB�� ����� ��þƮ���� �� �����⿣ ������ �ֱ⿡ Ŭ���̾�Ʈ���� ����� ����ϱ�� ����.
					if (m_pItem->IsFlag(FLAG_ITEM_COMPOSITION) && m_pItem->ComItem_index > 0)
					{
						if (nPlatinum_Ent > 0)
						{
							strEnt.PrintF("+%d", nPlatinum_Ent);
							strTmp += strEnt;
						}

						if (nEnt > 0)
						{
							strEnt.PrintF("[+%d]", nEnt);
							strTmp += strEnt;
						}
					}
					else
					{
						if (nEnt > 0)
						{
							strEnt.PrintF("+%d", nEnt);
							strTmp += strEnt;
						}
						
						if (nPlatinum_Ent > 0)
						{
							strEnt.PrintF("[+%d]", nPlatinum_Ent);
							strTmp += strEnt;
						}
					}
				}
			}
        }
    }

    if (m_pItemData->IsFlag(ITEM_FLAG_ORIGIN))
    {
        col = 0xCC66FFFF;
    }

    AddText(strTmp, col);
}

void CUITooltip::SetCompositionInfo()
{
	if( !(( m_pItemData->GetType() == CItemData::ITEM_WEAPON && !m_pItemData->IsProductionItem()) ||
		m_pItemData->GetType() == CItemData::ITEM_SHIELD) )
		return;

	CTString strTmp;
	COLOR	col = 0xFFAA44FF;

	if(m_pItem->IsFlag(FLAG_ITEM_COMPOSITION))
	{
		strTmp.PrintF( _S( 2706, "�ɷ�ġ ���� ȿ��"));
	}
	else if(m_pItem->IsFlag(PLATINUM_MAX_PLUS))
	{
		strTmp.PrintF( _S( 2675, "�÷�Ƽ�� ���� ȿ��"));
	}
	else
	{
		return;
	}

	AddText(strTmp, col);
}

void CUITooltip::SetExpireInfo()
{
    // �Ʒ� ���ǿ� �ش� �ϴ� �������� used�� endure�� ���.
    if (m_pItemData->GetType() == CItemData::ITEM_WEAPON &&
		(m_pItemData->GetSubType() == CItemData::ITEM_WEAPON_MINING ||
        m_pItemData->GetSubType() == CItemData::ITEM_WEAPON_CHARGE ||
        m_pItemData->GetSubType() == CItemData::ITEM_WEAPON_GATHERING) )
        return;

    if (m_pItem->Item_Used <= 0)
        return;

    CTString strTmp;
    COLOR	col = 0xFFAA44FF;

    tm* pTime;
    pTime = localtime((time_t*)&m_pItem->Item_Used);
    strTmp.PrintF(_S(6070, "���� : %d��%d��%d��%d��%d��"), pTime->tm_year + 1900
                  , pTime->tm_mon + 1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min);

    AddText(strTmp, col);
}

void CUITooltip::SetPriceInfo()
{
//     if (m_pItemData->GetType() == CItemData::ITEM_ETC)
//         return;
// 
//     if (m_pItem->Item_Price <= 0)
//         return;

    CTString	strMoney;
    CTString	strTmp;
    CUIManager* pUIManager = CUIManager::getSingleton();
	MyInfo* pInfo = MY_INFO();

	if (m_nWhichUI == UI_SHOP)
	{
		int nShopID = UIMGR()->GetShopUI()->GetShopID();
		BOOL bBuy = UIMGR()->GetShopUI()->IsBuyShop();
		
		__int64 iPrice = UIMGR()->GetShopUI()->CalculateItemPrice(nShopID, *m_pItemData, 1, bBuy);
		__int64 iAddPrice = pInfo->CalcPriceRate(iPrice, bBuy) + pInfo->CalcPriceAddition(bBuy);

		bool bIgnore = false;

		if (m_pItemData->getindex() == 7340 ||
			m_pItemData->getindex() == 7341 ||
			m_pItemData->getindex() == 7342)
			bIgnore = true;
		
		if (iAddPrice != 0 && bIgnore == false)
		{
			CTString strRate;
			CTString strResult;

			strMoney.PrintF("%I64d", iPrice);
			strRate.PrintF("%I64d", iAddPrice);

			pUIManager->InsertCommaToString(strMoney);
			pUIManager->InsertCommaToString(strRate);

			strTmp.PrintF(_S(255, "���� : %s"), strMoney);

			if (iAddPrice >= 0)
				strResult.PrintF("%s(+%s)", strTmp, strRate);
			else
				strResult.PrintF("%s(%s)", strTmp, strRate);

			AddText(strResult, pUIManager->GetNasColor(strMoney));
			return;
		}

		strMoney.PrintF("%I64d", iPrice);
	}
	else if (m_nWhichUI == UI_PERSONALSHOP)
	{
		strMoney.PrintF("%I64d", m_pItem->Item_Price);
	}
	else
	{
		return;
	}

    pUIManager->InsertCommaToString(strMoney);
    strTmp.PrintF(_S(255, "���� : %s"), strMoney);

    AddText(strTmp, pUIManager->GetNasColor(strMoney));
}

void CUITooltip::SetRuneInfo()
{
    if (!m_pItemData->IsFlag(ITEM_FLAG_UPGRADE))
        return;

    CTString strTmp;
    COLOR col = 0xFFFF40FF;

    if (m_pItem->Item_Flag & (FLAG_ITEM_LENT))
    {
        strTmp = (_S(3047, "���ü� ���Ұ�"));
        col = 0xFF4040FF;
    }
    else // �� �׷��̵� ���� ������
    {
		if (m_pItemData->GetJob() & (1<<WILDPET_JOB))
		{
			if (!(m_pItem->Item_Flag & FLAG_ITEM_SUPER_STONE_USED))
				strTmp.PrintF(_S(1658, "�� ��� ���ü� ��� ����"));
			else
				return;
		}
		else
		{
			if (m_pItemData->GetLevel() >= RUNE_ITEM_LEVEL)
			{
				if (!(m_pItem->Item_Flag & FLAG_ITEM_SUPER_RUNE_USED))
				{
					strTmp = (_S(5726, "�� ��� �� ��� ����"));

					// �� ��� �� ����� ���� �� ��� �ؽ�Ʈ �ϳ� �� �߰�.
					AddText(strTmp, col);
				}

				strTmp.PrintF(_S(4484, "������ ���׷��̵� ����"));
			}
			else if (!(m_pItem->Item_Flag & FLAG_ITEM_SUPER_STONE_USED))
			{
				// �ʰ��� ����� �� ���� ���� ǥ��
				strTmp.PrintF(_S(1658, "�� ��� ���ü� ��� ����"));
			}
			else
			{
				return;
			}
		}        
    }

    AddText(strTmp, col);
}

void CUITooltip::SetReformInfo()
{
    if (!m_pItemData->IsFlag(ITEM_FLAG_ORIGIN) && !m_pItemData->IsFlag(ITEM_FLAG_RARE))
        return;

    CTString strTmp;
    COLOR col = 0xFFFF40FF;

    if (m_pItemData->GetFlag() & ITEM_FLAG_NOT_REFORM)
    {
        strTmp = _S(5774, "���� �Ұ�");
    }
    else
    {
        if (m_pItemData->IsFlag(ITEM_FLAG_ORIGIN))
            strTmp = _S(5773, "��� ���� ����");
        else
            strTmp = _S(5772, "�߱� ���� ����");
    }

    AddText(strTmp, col);
}

void CUITooltip::SetCharBoundInfo()
{
	if (m_pItem->Item_Flag & (FLAG_ITEM_BELONG))
	{
		CTString strTmp = _S(6452, "�ͼ�");
		COLOR col = 0xFF0000FF;

		AddText(strTmp, col);
	}	
}

void CUITooltip::SetClassInfo()
{
    // ���⳪ �� �̿ܿ��� �� Ŭ���� ��� ����.
    if (m_pItemData->GetType() != CItemData::ITEM_WEAPON &&
        m_pItemData->GetType() != CItemData::ITEM_SHIELD)
        return;

    CTString strTmp;
    CUIManager::getSingleton()->GetClassOfItem(m_pItemData, strTmp);

    if (strTmp.IsEmpty() == TRUE)
        return;

    AddText(strTmp);
}

void CUITooltip::SetLevelInfo()
{
	int nItemLevel = 0;
	CTString strTmp;
	COLOR	col = DEF_UI_COLOR_WHITE;

	if ((m_pItemData->GetType() == CItemData::ITEM_ETC && m_pItemData->GetSubType() == CItemData::ITEM_ETC_REFINE) ||
		(m_pItemData->GetType() == CItemData::ITEM_POTION && m_pItemData->GetSubType() == CItemData::POTION_UP))
	{
		nItemLevel = m_pItem->Item_Flag;
		
		if (nItemLevel <= 0)
			return;

		strTmp.PrintF(_S(160, "����: %d"), nItemLevel);
	}
	else
	{
		nItemLevel = m_pItemData->GetLevel();

		if (nItemLevel <= 0)
			return;

		int nJewelReduction = m_pItem->GetJewelLevelReduction();
		int nWearReduction = m_pItem->GetWearLevelReduction();

		int nLvDown = nJewelReduction + nWearReduction;
		bool Premium = false;
#ifdef PREMIUM_CHAR
		CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();
		if (pChar != NULL)
		{
			if (pChar->getType() == PREMIUM_CHAR_TYPE_FIRST)
			{
				if (nItemLevel <= 180 && 
					(m_pItemData->GetType() == CItemData::ITEM_WEAPON ||
					 m_pItemData->GetType() == CItemData::ITEM_SHIELD ||
					 m_pItemData->GetType() == CItemData::ITEM_ACCESSORY))
				{
					if (nLvDown >= DEF_PC1_LEVEL_DOWN)
						Premium = false;
					else
					{
						// ����
						// �� ���� ���� ���� ��Ŵ
						if (m_pItemData->GetWearingPosition() >= 0 &&
							m_pItemData->CanUse(WILDPET_JOB))
						{
							Premium = false;
							nLvDown = 0;
						}
						else
						{
							Premium = true;
							nLvDown = DEF_PC1_LEVEL_DOWN;
						}
					}
				}				
			}
		}
#endif	//	PREMIUM_CHAR

		if (nLvDown > 0)
			strTmp.PrintF(_S(160, "����: %d") + " (-%d)", nItemLevel, nLvDown);
		else
			strTmp.PrintF(_S(160, "����: %d"), nItemLevel);

		if (nJewelReduction > 0)
			col = 0xFFFF40FF;

		if (Premium == true)
			col = 0xD67FFFFF;
	}

    AddText(strTmp, col);
}

void CUITooltip::SetSyndicateInfo()
{
    if (m_pItemData->GetSyndicateType() <= 0)
        return;

    GameDataManager* pGameData = GameDataManager::getSingleton();

    if (pGameData != NULL)
    {
        CSyndicate* pSyndicate = pGameData->GetSyndicate();

        if (pSyndicate != NULL)
        {
            CTString strTmp;

            strTmp.PrintF(_S(6169, "����:%s"), pSyndicate->GetSyndicateName(m_pItemData->GetSyndicateType()));
            AddText(strTmp);

            strTmp.PrintF(_S(6170, "����:%s �̻�"), pSyndicate->GetGradeName(m_pItemData->GetSyndicateType(), m_pItemData->GetSyndicateGrade()));
            AddText(strTmp);
        }
    }
}

void CUITooltip::SetPotionBuff()
{
    if (m_pItemData->GetType() != CItemData::ITEM_POTION)
        return;

    if (m_pItemData->GetSubType() != CItemData::POTION_UP)
        return;

    if (m_pItem->Item_Flag <= 0)
        return;

    CTString strTmp;
    COLOR	col = 0xDEC05BFF;

    strTmp.PrintF(_S(160, "����: %d"), m_pItem->Item_Flag);
    AddText(strTmp, 0xD28060FF);

    int nSkillType = m_pItemData->GetSkillType();
    CSkill&	rSkill = _pNetwork->GetSkillData(nSkillType);
    int nPower = rSkill.GetPower(m_pItem->Item_Flag - 1);

    if (nPower <= 0)
        return;

    if (m_pItemData->GetNum1() == CItemData::POTION_UP_PHYSICAL)   // ����
    {
        if (m_pItemData->GetNum2() == CItemData::POTION_UP_ATTACK)   // ����
        {
            strTmp.PrintF(_S(790, "���� ���ݷ� +%d ���"), nPower);
        }
        else if (m_pItemData->GetNum2() == CItemData::POTION_UP_DEFENSE)  // ���
        {
            strTmp.PrintF(_S(791, "���� ���� +%d ���"),  nPower);
        }
    }
    else if (m_pItemData->GetNum1() == CItemData::POTION_UP_MAGIC)  // ����
    {
        if (m_pItemData->GetNum2() == CItemData::POTION_UP_ATTACK)   // ����
        {
            strTmp.PrintF(_S(792, "���� ���ݷ� +%d ���"),  nPower);
        }
        else if (m_pItemData->GetNum2() == CItemData::POTION_UP_DEFENSE)  // ���
        {
            strTmp.PrintF(_S(793, "���� ���� +%d ���"),  nPower);
        }
    }

    if (strTmp.IsEmpty() == FALSE)
        AddText(strTmp, col);
}

void CUITooltip::SetQuestInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_ETC)
        return;

    if (m_pItemData->GetSubType() != CItemData::ITEM_SUB_QUEST_SCROLL)
        return;

    int nQIndex = m_pItemData->GetNum0();

    if (nQIndex <= 0)
        return;

    CQuestSystem& rQuest = CQuestSystem::Instance();
    CTString strTmp;
    COLOR	col = 0xDEC05BFF;

    // ����Ʈ �̸� ���
    AddText((CTString)rQuest.GetQuestName(nQIndex), col);

    // ���� ���� ���.
    const int iMinLevel = rQuest.GetQuestMinLevel(nQIndex);
    const int iMaxLevel = rQuest.GetQuestMaxLevel(nQIndex);

    strTmp.PrintF(_S(1660, "���� ���� : %d ~ %d"), iMinLevel, iMaxLevel);

    AddText(strTmp, col);
}

void CUITooltip::SetDamageInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_WEAPON)
        return;

    int nPhysicalDamage = m_pItemData->GetPhysicalAttack();
    int nMagicDamage = m_pItemData->GetMagicAttack();
    int nPlusDamage = 0;
    int nPhysicalOptionDmg = 0;
    int nMagicOptionDmg = 0;

    if (nPhysicalDamage <= 0 && nMagicDamage <= 0)
        return;

    if (m_pItemData->IsFlag(ITEM_FLAG_RARE))
    {
        CItemRareOption* rRareOp = CItemRareOption::getData(m_pItem->Item_RareIndex);

		if(rRareOp != NULL)
		{
			nPhysicalOptionDmg = rRareOp->GetPhysicalAttack();
			nMagicOptionDmg	= rRareOp->GetMagicAttack();
		}        
    }

    CTString strTmp;

    if (nPhysicalDamage)
    {
		nPhysicalDamage += nPhysicalOptionDmg + m_pItem->Item_State_Plus;

        if (m_pItem->Item_Plus > 0)
        {
			if ((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0)
				nPlusDamage = CItems::ItemUpgradeFuckingFunction(nPhysicalDamage,
                          m_pItem->Item_Plus, m_pItemData->GetLevel() >= 146 ? TRUE : FALSE);
			else
				nPlusDamage = calc_pet_item_plus(nPhysicalDamage, m_pItem->Item_Plus);
        }
		        
        if (nPlusDamage > 0)
            strTmp.PrintF(_S(355, "���ݷ� : %d + %d"), nPhysicalDamage, nPlusDamage);
        else
            strTmp.PrintF(_S(161, "���ݷ� : %d"), nPhysicalDamage);

        AddText(strTmp);

        if (((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0) &&
			m_pItem->Item_Plus >= 15 && m_pItemData->GetLevel() < 146)
        {
            strTmp.PrintF(_S(1891, "���� ���ݷ� + 75"));
            AddText(strTmp);
        }
    }

    if (nMagicDamage)
    {
		nMagicDamage += nMagicOptionDmg + m_pItem->Item_State_Plus;

        if (m_pItem->Item_Plus > 0)
        {
			if ((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0)
				nPlusDamage = CItems::ItemUpgradeFuckingFunction(nMagicDamage,
                          m_pItem->Item_Plus, m_pItemData->GetLevel() >= 146 ? TRUE : FALSE);
			else
				nPlusDamage = calc_pet_item_plus(nMagicDamage, m_pItem->Item_Plus);
        }

        if (nPlusDamage > 0)
            strTmp.PrintF(_S(356, "���� ���ݷ� : %d + %d"), nMagicDamage, nPlusDamage);
        else
            strTmp.PrintF(_S(162, "���� ���ݷ� : %d"), nMagicDamage);

        AddText(strTmp);

        if (((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0) &&
			m_pItem->Item_Plus >= 15 && m_pItemData->GetLevel() < 146)
        {
            strTmp.PrintF(_S(1892, "���� ���ݷ� + 50"));
            AddText(strTmp);
        }
    }

    if (((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0) &&
		m_pItemData->GetLevel() >= RUNE_ITEM_LEVEL)
    {
        int nRuneBonus = CItems::CalculateRuneItemBonus(m_pItemData->GetType(), m_pItem->Item_Plus);

        if (nRuneBonus > 0)
        {
            strTmp.PrintF(_S(4810, "�߰� ���ݷ� %d���"), nRuneBonus);
            AddText(strTmp, 0xFF6A00FF);
        }
    }
}

void CUITooltip::SetDefenceInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_SHIELD)
        return;

    int nPhysicalDefence = m_pItemData->GetPhysicalDefence();
    int nMagicDefence = m_pItemData->GetMagicDefence();
    int nPlusDefence = 0;
    int nPhysicalOptionDefence = 0;
    int nMagicOptionDefence = 0;

    CUITooltipMgr* pTooltipMgr = CUITooltipMgr::getSingleton();

    if (nPhysicalDefence <= 0 && nMagicDefence <= 0)
        return;

	if (m_pItemData->IsFlag(ITEM_FLAG_RARE))
	{
		CItemRareOption* rRareOp = CItemRareOption::getData(m_pItem->Item_RareIndex);

		if (rRareOp != NULL)
		{
			nPhysicalOptionDefence = rRareOp->GetPhysicalDefence();
			nMagicOptionDefence	= rRareOp->GetMagicDefence();
		}        
	}

    CTString strTmp;

    if (nPhysicalDefence)
    {
		nPhysicalDefence += nPhysicalOptionDefence + m_pItem->Item_State_Plus;

        if (m_pItem->Item_Plus > 0)
        {
			if ((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0)
				nPlusDefence = CItems::ItemUpgradeFuckingFunction(nPhysicalDefence,
                           m_pItem->Item_Plus, m_pItemData->GetLevel() >= 146 ? TRUE : FALSE);
			else
				nPlusDefence = calc_pet_item_plus(nPhysicalDefence, m_pItem->Item_Plus);
        }

        if (nPlusDefence > 0)
            strTmp.PrintF(_S(357, "���� : %d + %d"), nPhysicalDefence, nPlusDefence);
        else
            strTmp.PrintF(_S(163, "���� : %d"), nPhysicalDefence);

        AddText(strTmp);
    }

    if (nMagicDefence)
    {
		nMagicDefence += nMagicOptionDefence + m_pItem->Item_State_Plus;

        if (m_pItem->Item_Plus > 0)
        {
			if ((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0)
				nPlusDefence = CItems::ItemUpgradeFuckingFunction(nMagicDefence,
                           m_pItem->Item_Plus, m_pItemData->GetLevel() >= 146 ? TRUE : FALSE);
			else
				nPlusDefence = calc_pet_item_plus(nMagicDefence, m_pItem->Item_Plus);
        }

        if (nPlusDefence > 0)
            strTmp.PrintF(_S(358, "���� ���� : %d + %d"), nMagicDefence, nPlusDefence);
        else
            strTmp.PrintF(_S(164, "���� ���� : %d"), nMagicDefence);

        AddText(strTmp);
    }

	if ((m_pItemData->GetJob() & (1 << WILDPET_JOB)) == 0)
	{
		if (m_pItem->Item_Plus >= 15 && m_pItemData->GetLevel() < 146)
		{
			strTmp.PrintF(_S(1893, "���� ���� + 100"));
			AddText(strTmp);

			strTmp.PrintF(_S(1894, "���� ���� + 50"));
			AddText(strTmp);
		}

		if (m_pItemData->GetLevel() >= RUNE_ITEM_LEVEL)
		{
			int nRuneBonus = CItems::CalculateRuneItemBonus(m_pItemData->GetType(), m_pItem->Item_Plus);

			if (nRuneBonus > 0)
			{
				strTmp.PrintF(_S(4811, "�߰� ���� %d���"), nRuneBonus);
				AddText(strTmp, 0xFF6A00FF);
			}

			if ((m_pItem->Item_Plus >= 10) && (m_pItemData->GetSubType() != CItemData::ITEM_SHIELD_SHIELD))
			{
				strTmp.PrintF(_S(5700, "�߰� HP ���� %d"), (m_pItem->Item_Plus - 9) * 100);
				AddText(strTmp, 0xFF6A00FF);
			}
		}

		int nItemPlusPvpOptionBonus = CItems::CalculatePlusItemPVPOptionBonus(m_pItemData->GetType(), m_pItem->Item_Plus);

		if (nItemPlusPvpOptionBonus > 0)
		{
			COptionData*	odItem = COptionData::getData(OPTION_PVP_DAMAGE_ABSOLB);

			if (odItem != NULL)
			{
				float fValue = odItem->GetValue(nItemPlusPvpOptionBonus - 1) / 100.f;
				strTmp.PrintF(_S(6468, "�߰� PVP��������� %.1f"), fValue);
				AddText(strTmp, 0xFF6A00FF);
			}
		}
	}	
}

void CUITooltip::SetFlightInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_SHIELD)
        return;

    if (m_pItemData->GetSubType() != CItemData::ITEM_SHIELD_BACKWING)
        return;

    // ����Ʈ ������ ���� ����ӵ� ǥ��
    CTString strTmp;

    strTmp.PrintF(_S(4485, "����ӵ� : %.1f"), (m_pItemData->GetNum2() / 10.0f));

    AddText(strTmp);
}

void CUITooltip::SetPenaltyInfo()
{
	int nReductionLv = 0;
#ifdef PREMIUM_CHAR
	CPremiumChar* pChar = GAMEDATAMGR()->GetPremiumChar();

	if (pChar != NULL)
	{
		switch(pChar->getType())
		{
		case PREMIUM_CHAR_TYPE_FIRST:
			nReductionLv = 3;
			break;
		default:
			nReductionLv = 0;
			break;
		}
	}
#endif	//	PREMIUM_CHAR
	// ��� �� ���� ���뷹�� ���� ����
	int nOptionReductionLv = m_pItem->GetJewelLevelReduction() + m_pItem->GetWearLevelReduction();
	nReductionLv = nReductionLv <= nOptionReductionLv ? nOptionReductionLv : nReductionLv;

	int	nDiffLevel = (m_pItemData->GetLevel() - nReductionLv) - _pNetwork->MyCharacterInfo.level;
    int	nPenalty = 0;

	// ������ �� �����
	if (m_pItemData->GetJob() & (1 << WILDPET_JOB))
	{
		nReductionLv = m_pItem->GetWearLevelReduction();
		ObjInfo* pInfo = ObjInfo::getSingleton();

		if (pInfo->GetMyApetInfo() != NULL)
		{
			nDiffLevel = (m_pItemData->GetLevel() - nReductionLv) - pInfo->GetMyApetInfo()->m_nLevel;
		}
		else
		{
			// ����� ���� ���ٸ�
			nDiffLevel = 0;
		}		
	}

    CTString strTmp;
    COLOR	col = 0xE53535FF;

    if (m_pItemData->GetType() == CItemData::ITEM_WEAPON)
    {
        if (nDiffLevel > 12)
            nPenalty = 90;
        else if (nDiffLevel > 8)
            nPenalty = 70;
        else if (nDiffLevel > 4)
            nPenalty = 50;
        else
            return;

        strTmp.PrintF(_S(1029, "�г�Ƽ : ���ݷ� %d ����"), nPenalty);
    }
    else if (m_pItemData->GetType() == CItemData::ITEM_SHIELD)
    {
        if (nDiffLevel > 15)
            nPenalty = 90;
        else if (nDiffLevel > 10)
            nPenalty = 70;
        else if (nDiffLevel > 5)
            nPenalty = 50;
        else
            return;

        strTmp.PrintF(_S(1030, "�г�Ƽ : ���� %d ����"), nPenalty);
    }

    if (strTmp.IsEmpty() == TRUE)
        return;

    AddText(strTmp, col);
}

void CUITooltip::SetDurabilityInfo()
{
#ifdef DURABILITY

    if (m_pItemData->IsFlag(ITEM_FLAG_DURABILITY))
    {
        CTString strTmp;
        COLOR	col = 0xFFC000FF;

        int nCurDur = m_pItem->Item_durability_now / 10;
        int nMaxDur = m_pItem->Item_durability_max / 10;

        strTmp.PrintF(_S(6191, "������: %d/%d"), nCurDur, nMaxDur);

        if (nCurDur <= 0)
            col = 0xFF0000FF;

        AddText(strTmp, col);
    }

#endif	//	DURABILITY
}

void CUITooltip::SetEndureInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_SHIELD)
        return;

    if (m_pItemData->GetSubType() != CItemData::ITEM_WEAPON_MINING ||
        m_pItemData->GetSubType() != CItemData::ITEM_WEAPON_CHARGE ||
        m_pItemData->GetSubType() != CItemData::ITEM_WEAPON_GATHERING)
        return;

    CTString strTmp;
    COLOR	col = 0xDEC05BFF;

    strTmp.PrintF(_S(510, "������ : %ld"), m_pItem->Item_Used);

    AddText(strTmp, col);
}

void CUITooltip::SetFameInfo()
{
    int nFame = m_pItemData->GetFame();

    if (nFame <= 0)
        return;

    CTString strTmp;

    strTmp.PrintF(_S(1096, "�� %d �ʿ�"), nFame);

    AddText(strTmp, 0xDEC05BFF);
}

void CUITooltip::SetUnknownInfo()
{
    // �÷��װ� ��� �ƴϸ� ����.
    if (!m_pItemData->IsFlag(ITEM_FLAG_RARE))
        return;

    int nRareIndex = m_pItem->Item_RareIndex;

    // 0�� �̰��� �ɼ�. 0���� ũ�� �ɼ��� �ִٴ� ���.
    if (nRareIndex != 0)
        return;

    CTString strTmp;

    strTmp = _S(3165, "<�̰��� �ɼ�>");

    AddText(strTmp, 0xFF4040FF);
}

void CUITooltip::SetPetInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_ACCESSORY)
        return;

    if (m_pItemData->GetSubType() != CItemData::ACCESSORY_PET)
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();
    SPetExchangeInfoString pInfo;

    // ������ ���� �е帰 �� ������ �ִ��� Ȯ�� ����Ÿ�� ���ٸ� ����.
    if (pUIManager->GetPetInfo()->GetPetExchangeInfo(m_pItem->Item_Plus, pInfo) == FALSE)
        return;

    if (pInfo.strNameCard.Length() > 0)
        AddText(pInfo.strNameCard, 0xF2F2F2FF);

    AddText(pInfo.strLevel, 0xDEC05BFF);
    AddText(pInfo.strHP, 0xDEC05BFF);
    AddText(pInfo.strlExp, 0xDEC05BFF);
    AddText(pInfo.strHungry, 0xDEC05BFF);
    AddText(pInfo.strSympathy, 0xDEC05BFF);
    AddText(pInfo.strAbility, 0xDEC05BFF);
#ifdef PET_SEAL_TIME
    AddText(pInfo.strRemainTime, 0xDEC05BFF);
#endif // PET_SEAL_TIME
}

void CUITooltip::SetWildPetInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_ACCESSORY)
        return;

    if (m_pItemData->GetSubType() != CItemData::ACCESSORY_WILDPET)
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();
    sPetItem_Info pInfo;
    CWildPetData* pData = CWildPetData::getData(m_pItemData->GetPetType());

    CTString strTmp;

    // ����Ÿ�� ���ٸ� ����.
    if (pUIManager->GetWildPetInfoUI()->GetWildPetInfo(m_pItem->Item_Plus, pInfo) == FALSE)
        return;

    strTmp.PrintF(_S(4215, "�̸�: %s"), pInfo.pet_name);
    AddText(strTmp, 0xF2F2F2FF);

    strTmp.PrintF(_S(4216, "�� ����: %d"), pInfo.pet_level);
    AddText(strTmp, 0xDEC05BFF);

    if (pData->nFlag & WILDPET_FLAG_EXP)
    {
        INDEX temCooltime = pInfo.pet_cooltime - (unsigned int)_pTimer->GetLerpedCurrentTick();

        if (temCooltime > 0)
        {
            strTmp.PrintF(_S(5644, "��Ÿ��"));

            int iSec = temCooltime % 60;
            temCooltime /= 60;

            int iMin = temCooltime % 60;
            int iHour = temCooltime /= 60;

            CTString temtime;

            temtime.PrintF(": ");
            strTmp += temtime;

            if (iHour > 0)
            {
                temtime.PrintF(_S(2512, "%d�ð�"), iHour);
                strTmp += temtime;
            }

            if (iMin > 0)
            {
                temtime.PrintF(_S(2513, "%d��"), iMin);
                strTmp += temtime;
            }

            if (iSec > 0)
            {
                temtime.PrintF(_S(2514, "%d��"), iSec);
                strTmp += temtime;

            }

        }
        else
        {
			CTString strExp;
			strExp.PrintF("%I64d", pInfo.pet_accexp);
			pUIManager->InsertCommaToString(strExp);
            strTmp.PrintF(_S(5639, "���� ����ġ: %s"), strExp);
        }

        AddText(strTmp, 0xfd9d28FF);
    }
    else
    {
        strTmp.PrintF(_S(4217, "��: %d"), pInfo.pet_str);
        AddText(strTmp, 0xDEC05BFF);
        strTmp.PrintF(_S(4218, "��ø: %d"), pInfo.pet_dex);
        AddText(strTmp, 0xDEC05BFF);
        strTmp.PrintF(_S(4219, "����: %d"), pInfo.pet_int);
        AddText(strTmp, 0xDEC05BFF);
        strTmp.PrintF(_S(4220, "ü��: %d"), pInfo.pet_con);
        AddText(strTmp, 0xDEC05BFF);
    }
}

void CUITooltip::SetSocketInfo()
{
    CTString strTmp;

    CUITooltipMgr* pTooltipMgr = CUITooltipMgr::getSingleton();

    if (m_pItemData->IsFlag(ITEM_FLAG_SOCKET) == false)
        return;

    if (!(m_pItem->Item_Flag & FLAG_ITEM_SOCKET))
    {
        if (m_pItem->Item_SocketCount <= 0)
        {
            strTmp = _S(4993, "���� ���� ����");
            AddText(strTmp, 0x00FF00FF);
        }
    }

    // �����ۿ� ������ �ո� ����.
    if (m_pItem->Item_SocketCount <= 0)
        return;

    int i, nCount = 0;

    for (i = 0; i < MAX_SOCKET_OPTION; ++i)
    {
        // �շ� �ִ� ���ۿ� ���� �ִ� ������ ����.
        if (m_pItem->Item_SocketJewelIndex[i] > 0)
            ++nCount;
    }

    strTmp.PrintF("%s ( %d / %d )", _S(4994, "���� ����"), nCount, m_pItem->Item_SocketCount);
    AddText(strTmp);

    int nImgIndex = -1;
    COLOR col = DEF_UI_COLOR_WHITE;

    for (i = 0; i < m_pItem->Item_SocketCount; ++i)
    {
        LONG lIndex = m_pItem->Item_SocketJewelIndex[i];

        if (lIndex < 0)
            continue;

        if (lIndex == 0)
        {
            // 0�� ī���� ����.
            if (i == 0)
            {
                strTmp = _S(5901, "ī���� ����Ȧ") + _S(4996, "�������");
                nImgIndex = eCHAOS_EMPTY;
            }
            else
            {
                strTmp = _S(4995, "����Ȧ") + _S(4996, "�������");
                nImgIndex = eGENERAL_EMPTY;
            }

			col = DEF_UI_COLOR_WHITE;
        }
        else
        {
            CItemData* pItemData = _pNetwork->GetItemData(lIndex);

            if (pItemData == NULL)
                continue;

            int nJewelOption = pItemData->GetPetAISet1();
            SBYTE sbOptionType = pItemData->GetSocketOptionIndex();
            LONG  lOptionLevel = pItemData->GetSocketOptionLevel();

            if (UtilHelp* pHelp = UtilHelp::getSingleton())
            {
                switch (pHelp->GetOptionType(nJewelOption))
                {
                case OPTION_TYPE_PACIVE:
                    {
                        CTString strOption = UTIL_HELP()->GetOptionPaciveInfo(sbOptionType, lOptionLevel);
                        strTmp.PrintF("%s : %s", pItemData->GetName(), strOption);
                    }
                    break;

                case OPTION_TYPE_ACTIVE:
                    strTmp = UTIL_HELP()->GetOptionActiveSkillInfo(pItemData);
                    break;
                }
            }

            if (i == 0)
            {
                col = 0xFFFF50FF;
                nImgIndex = eCHAOS_FILL;
            }
            else
            {
                col = 0xFF610CFF;
                nImgIndex = eGENERAL_FILL;
            }
        }

		CUIImageText* pIT = (CUIImageText*)pTooltipMgr->GetTooltipRes()->CloneImageText();

		if (pIT == NULL)
			continue;

		pIT->SetWidth(m_pList[eLIST_FIRST]->GetWidth());
		pIT->SetSocket(strTmp.str_String, col, nImgIndex, true);
        m_pList[eLIST_FIRST]->AddListItem((CUIBase*)pIT);
    }
}

void CUITooltip::SetJewelInfo()
{
	if (m_pItemData->GetType() != CItemData::ITEM_ETC)
		return;

	if (m_pItemData->GetSubType() != CItemData::ITEM_ETC_JEWEL &&
		m_pItemData->GetSubType() != CItemData::ITEM_ETC_CHAOSJEWEL)
		return;

	CTString strTmp;

	int nJewelOption = m_pItemData->GetPetAISet1();
	SBYTE sbOptionType = m_pItemData->GetSocketOptionIndex();
	LONG  lOptionLevel = m_pItemData->GetSocketOptionLevel();

	if (UtilHelp* pHelp = UtilHelp::getSingleton())
	{
		switch (pHelp->GetOptionType(nJewelOption))
		{
		case OPTION_TYPE_PACIVE:
			{
				CTString strOption = UTIL_HELP()->GetOptionPaciveInfo(sbOptionType, lOptionLevel);
				strTmp.PrintF("%s : %s", m_pItemData->GetName(), strOption);
			}
			break;

		case OPTION_TYPE_ACTIVE:
			strTmp = UTIL_HELP()->GetOptionActiveSkillInfo(m_pItemData);
			break;
		}
	}

	AddText( strTmp, 0x94B7C6FF );
}

void CUITooltip::SetOptionInfo()
{
    if (m_pItemData->GetType() == CItemData::ITEM_ETC)
        return;

    if (m_pItemData->GetSubType() == CItemData::ITEM_ETC_JEWEL ||
        m_pItemData->GetSubType() == CItemData::ITEM_ETC_CHAOSJEWEL)
        return;

    int i;
    SBYTE sbType;
    LONG lLevel, lOptionVar;
    CTString strTmp;
    COLOR	col = 0x94B7C6FF;

    for (i = 0; i < MAX_OPTION_INC_ORIGIN; ++i)
    {
        sbType = m_pItem->Item_OptionType[i];
        lLevel = m_pItem->Item_OptionLevel[i];
        lOptionVar = m_pItem->Item_OriginOptionVar[i];

        if (sbType < 0 || lLevel == 0)
            continue;

        COptionData* odItem = COptionData::getData(sbType);

        if (UtilHelp* pHelp = UtilHelp::getSingleton())
        {
            if (pHelp->GetPaciveOptionType(sbType) < STRING_TYPE_TRANS)
            {
                strTmp = UTIL_HELP()->GetOptionPaciveInfo(sbType, lLevel, lOptionVar);
            }
            else if (pHelp->GetPaciveOptionType(sbType) == STRING_TYPE_TRANS)
            {
				if (odItem == NULL)
					continue;

                if (odItem->GetValue(lLevel - 1) == 0)
                {
                    strTmp.PrintF("%s", _S(5305, "���� ����"));

                }
                else if (odItem->GetValue(lLevel - 1) == 100)
                {
                    strTmp.PrintF("%s", _S(5306, "���� ���� ����"));
                }
                else
                {
                    strTmp.PrintF("%s : %d", odItem->GetName(), odItem->GetValue(lLevel - 1));
                }
            }
        }

        if (strTmp.IsEmpty() == TRUE)
            continue;

        AddText(strTmp, col);
    }
}

void CUITooltip::SetSkillInfo()
{
    if (!(m_pItemData->GetFlag() & ITEM_FLAG_ORIGIN))
        return;

    CTString strTmp;
    CTString strSkill;
    COLOR	col = 0xCC66FFFF;
    int i;

    for (i = 0; i < MAX_ITEM_SKILL; ++i)
    {
        if (m_pItem->Item_SkillIndex[i] < 0)
            continue;

        CSkill& rSkill = _pNetwork->GetSkillData(m_pItem->Item_SkillIndex[i]);

        strSkill = rSkill.GetDescription();
        strTmp.PrintF(_S(4486, "��ų ȿ�� : %s"), strSkill);

        if (strTmp.IsEmpty() == TRUE)
            continue;

        AddText(strTmp, col);
    }
}

void CUITooltip::SetBloodInfo()
{
    if (!(m_pItem->Item_Flag & FLAG_ITEM_OPTION_ENABLE))
        return;

    CTString strTmp;

    strTmp = _S(511, "���� �ɼ� ����");

    AddText(strTmp, 0xE53535FF);
}

void CUITooltip::SetSealInfo()
{
    if (!(m_pItem->Item_Flag & FLAG_ITEM_SEALED))
        return;

    CTString strTmp;

    strTmp = _S(512, "���ε� ������");

    AddText(strTmp, 0xE53535FF);
}

void CUITooltip::SetMonsterMercenaryInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_ETC)
        return;

    if (m_pItemData->GetSubType() != CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD)
        return;

    if (m_pItem->Item_Used2 <= 0)
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();
    CTString strTmp;

    COLOR col = 0x94B7C6FF;

    strTmp.PrintF(_s("[%s]%s"), pUIManager->GetMonsterMercenary()->GetMonsterGrade(m_pItem->Item_Used2),
                  CMobData::getData(m_pItem->Item_Plus)->GetName());

    AddText(strTmp, col);

    int attackPlus = (int)((m_pItem->Item_State_Plus & 0x0000FF00) >> 8);
    int defencePlus = (int)(m_pItem->Item_State_Plus & 0x000000FF);

    if (attackPlus > 0)
    {
        strTmp.PrintF(_S(5147, "����,���� ���ݷ� + %d"), attackPlus);
        AddText(strTmp, col);
    }

    if (defencePlus > 0)
    {
        strTmp.PrintF(_S(5148, "���� + %d"), defencePlus);
        AddText(strTmp, col);
    }
}

void CUITooltip::SetJewelDescInfo()
{
    if (m_pItemData->GetType() != CItemData::ITEM_ETC)
        return;

    if (m_pItemData->GetSubType() != CItemData::ITEM_ETC_JEWEL && m_pItemData->GetSubType() != CItemData::ITEM_ETC_CHAOSJEWEL)
        return;

    if (m_pItemData->GetPetAISet0() <= 0)
        return;

    CTString strPos;
    CTString strTmp;
    CTString strComma = ", ";

    COLOR	 col = 0x94B7C6FF;

    bool	bComma = false;

    if (m_pItemData->GetPetAISet0() == 511)
    {
        strPos.PrintF(_S(506, "��ü"));
    }
    else
    {
        // ����, ����, ����, ����, �尩, �Ź�, ����, ��, �Ǽ�����
        int nPosition[9] = {4775, 2544, 4273, 4274, 4275, 4276, 4277, 5916, 726};

        int i;

        for (i = 0; i < 9; ++i)
        {
            if (m_pItemData->GetPetAISet0() & 1 << i)
            {
                if (bComma)
                    strPos += strComma;
                else
                    bComma = true;

                if (i == 0)
                    bComma = true;

				strPos += _S(nPosition[i], "WarePos");;
            }
        }
    }

    if (strPos.IsEmpty() == TRUE)
        return;

    strTmp.PrintF(_S(5915, "���� ���� ���� : %s"), strPos);

    AddText(strTmp, col);
}

void CUITooltip::SetItemCoolTimeInfo(bool bUpdate /*= false*/)
{
	if (m_eType == eTOOLTIP_SECOND)
		return;

	CTString strTmp;
	COLOR	col = DEF_UI_COLOR_RED;

	DOUBLE dCoolTime;
	DOUBLE dReUseTime = 0.0;

	dReUseTime = MY_INFO()->GetReuseItem(m_nIndex);

	dCoolTime = ItemHelp::GetCoolTime(dReUseTime, m_pItemData->StartTime);

	if (dCoolTime <= 0.0f)
	{
		if (bUpdate && m_nCooltimePos >= 0)
			SetItemData(m_pItem, m_eType);
		return;
	}

	if (bUpdate == true && m_nCooltimePos < 0)
	{
		SetItemData(m_pItem, m_eType);
		return;
	}
	else if (bUpdate == false && m_nCooltimePos < 0)
	{
		AddText(strTmp, col);
		m_nCooltimePos = m_pList[eLIST_FIRST]->getChildCount() - 1;
	}

	int nMin = dCoolTime / 60;
	int nSec = (int)dCoolTime % 60;

	if (dCoolTime > 60.f)
		strTmp.PrintF(_S(4736, "��Ÿ�� : %d�� %d��"), nMin, nSec);
	else if (dCoolTime >= 3.f)
		strTmp.PrintF(_S(4737, "��Ÿ�� : %d��"), nSec);
	else
		strTmp.PrintF(_S(6256, "��Ÿ�� : %.1f��"), dCoolTime);

	CUIBase* pItem = m_pList[eLIST_FIRST]->getChildAt(m_nCooltimePos);

	if (pItem == NULL)
		return;

	pItem->Hide(FALSE);

	if (pItem->getType() == eUI_CONTROL_TEXT)
	{
		((CUIText*)pItem)->SetText(strTmp);
		((CUIText*)pItem)->setFontColor(col);
	}
	else if (pItem->getType() == eUI_CONTROL_TEXTBOX)
	{
		((CUITextBox*)pItem)->SetText(strTmp, col);
	}
}

void CUITooltip::SetDescInfo()
{
    CTString strDesc = _pNetwork->GetItemDesc(m_nIndex);

    if (strDesc.IsEmpty() == TRUE)
        return;

    AddText(strDesc, 0x9E9684FF);
}

void CUITooltip::SetTitleInfo()
{
	if (m_pItemData->GetType() != CItemData::ITEM_ONCEUSE ||
		m_pItemData->GetSubType() != CItemData::ITEM_SUB_TITLE)
		return;

	int nTitleIndex = UIMGR()->GetNickName()->GetTitleIndex(m_nIndex);
	int nCustomTitleIndex = m_pItem->Item_Used2;
	bool bCustomTitle = CustomTitleData::IsKeepTitleInfo(nCustomTitleIndex);
	
	if (nTitleIndex < 0 && bCustomTitle == false)
		return;

	INDEX iOptionIndex, iOptionLevelIndex;
	COLOR col = 0x59B1FFFF;

	CTString strTmp;
	UtilHelp* pHelp = UtilHelp::getSingleton();

	for (int j = 0 ; j < 5 ; ++j)
	{
		if (bCustomTitle == true)
		{
			iOptionIndex		= CustomTitleData::m_mapCustomTitleItemInfo[nCustomTitleIndex]->option_index[j];
			iOptionLevelIndex	= CustomTitleData::m_mapCustomTitleItemInfo[nCustomTitleIndex]->option_level[j];
		}
		else
		{
			iOptionIndex		= TitleStaticData::getData(nTitleIndex)->GetOptionIndex()[j];
			iOptionLevelIndex	= TitleStaticData::getData(nTitleIndex)->GetOptionLevel()[j];
		}


		if ( iOptionIndex < 0 || iOptionLevelIndex < 0)
			continue;

		if (pHelp->GetPaciveOptionType(iOptionIndex) < STRING_TYPE_TRANS)
		{
			strTmp = UTIL_HELP()->GetOptionPaciveInfo(iOptionIndex, iOptionLevelIndex);
		}
		else
			continue;

		AddText( strTmp, col );
	}

	if (strTmp.IsEmpty() == TRUE)
		AddText(_S(4837, "�ɼ� ����"), col);
}

void CUITooltip::SetSetItemInfo()
{
	if ( m_pItemData->GetFlag() & ITEM_FLAG_QUEST ||
		m_pItemData->GetSetItemIndex() <= 0)
		return;

	CTString strTmp;
	CSetItemData* pData = CSetItemData::getData(m_pItemData->GetSetItemIndex());

	if (pData == NULL)
		return;

	strTmp.PrintF("%s", pData->GetName());
	AddText(strTmp, 0x20A51CFF, eALIGN_H_LEFT, eLIST_SECOND);

	int	i;
	int nWearTotal = 0;
	int nWearingCnt = 0;
	LONG applyItem = 0;
	LONG cosApplyItem = 0;
	LONG cosApplyItemIdx = 0;
	COLOR col = 0xB3B3B3FF;

	for (i = 0; i < WEAR_TOTAL; ++i)
	{
		applyItem = pData->GetApplyItemIndex(i);

		if (applyItem > 0)
		{
			++nWearTotal;
			col = 0xB3B3B3FF;

			if (_pNetwork->MyWearItem[i].IsEmptyItem() == FALSE)
			{
				CItemData* pItemData = _pNetwork->GetItemData(_pNetwork->MyWearItem[i].ItemData->GetItemIndex());

				if (pItemData->IsFlag(ITEM_FLAG_COMPOSITE))
				{
					cosApplyItem = _pNetwork->MyWearItem[i].GetComItem_index();
					cosApplyItemIdx = 0;
					for (int idx = 0; idx < ITEM_COUNT_IN_INVENTORY_NORMAL; ++idx)
					{
						if (cosApplyItemIdx > 0)
							break;

						if (cosApplyItem <= 0)
							break;

						if (cosApplyItem == _pNetwork->MySlotItem[ITEM_TAB_NORMAL][idx].Item_UniIndex)
						{
							cosApplyItemIdx = _pNetwork->MySlotItem[ITEM_TAB_NORMAL][idx].Item_Index;
							break;
						}
					}

					if (cosApplyItemIdx == applyItem)
					{
						col = 0xFFFFFFFF;
						nWearingCnt++;
					}
				}
				else
				{
					if (applyItem == _pNetwork->MyWearItem[i].ItemData->GetItemIndex())
					{
						col = 0xFFFFFFFF;
						nWearingCnt++;
					}
				}
			}
			strTmp.PrintF("%s", _pNetwork->GetItemName(applyItem));
			AddText(strTmp, col, eALIGN_H_LEFT, eLIST_SECOND);
		}
	}

	strTmp.PrintF(_S(4487, "��Ʈ ���� ����(%d / %d)"), nWearingCnt, nWearTotal );
	AddText(strTmp, 0x20A51CFF, eALIGN_H_LEFT, eLIST_SECOND);

	for (i = 0; i < pData->GetMaxOption(); i++)
	{
		col = 0xB3B3B3FF;

		if (pData->GetOption(i).nSetType == CSetItemData::SET_ITEM_TYPE_OPTION)
		{
			SBYTE sbOptionType = pData->GetOption(i).nSetOptionIndex;
			COptionData* temOP = COptionData::getData(sbOptionType);

			if (temOP == NULL)
				continue;
			
			LONG  lOptionLevel = pData->GetOption(i).nOptionLevel;

			if (UtilHelp* pHelp = UtilHelp::getSingleton())
			{
				CTString strOption = UTIL_HELP()->GetOptionPaciveInfo(sbOptionType, lOptionLevel);
				strTmp.PrintF( _S(4488, "(%d)��Ʈ �ɼ� ȿ�� [%s]"), pData->GetOption(i).nSetCount, strOption );
			}
		}else	// ��ų
		{
			CSkill& temSkill = _pNetwork->GetSkillData(pData->GetOption(i).nSetOptionIndex);
			strTmp.PrintF( _S(4489, "(%d)��Ʈ ��ų ȿ�� : %s"), pData->GetOption(i).nSetCount, temSkill.GetDescription() );
		}

		if (nWearingCnt >= pData->GetOption(i).nSetCount)
		{
			col = 0x20A51CFF;
		}

		AddText(strTmp, col, eALIGN_H_LEFT, eLIST_SECOND);
	}

	return;
}

bool CUITooltip::SetFortuneInfo()
{
	if (m_eType == eTOOLTIP_FIRST)
		return false;

	if (m_pItem->Item_Plus <= 0)
		return false;

	if (m_pItemData->GetType() != CItemData::ITEM_SHIELD ||
		m_pItemData->GetSubType() != CItemData::ITEM_SHIELD_ONE_SUIT)
		return false;

	if (m_pItemData->GetFortuneIndex() <= 0)
		return false;

	CUITooltipMgr* pUITooltipMgr = CUITooltipMgr::getSingleton();
	CUIImageText* pIT = (CUIImageText*)pUITooltipMgr->GetTooltipRes()->CloneImageText();
	
	if (pIT == NULL)
		return false;

	int nFortuneIndex = m_pItem->Item_Plus >> 16;
	int nFortuneLevel = m_pItem->Item_Plus & 0x0000FFFF;

	// ���� Ÿ��Ʋ.
	CTString strTmp = _S(5893, "���� ����");
	AddText(strTmp, DEF_UI_COLOR_GREEN, eALIGN_H_CENTER);

	CSkill &rSkillData = _pNetwork->GetSkillData(nFortuneIndex);

	pIT->SetWidth(m_pList[eLIST_FIRST]->GetWidth());
	pIT->SetFortune(rSkillData.GetName(), nFortuneIndex, 0xBBC200FF, true);

	m_pList[eLIST_FIRST]->AddListItem((CUIBase*)pIT);

	// ��ũ����.
	AddText(rSkillData.GetDescription(), 0xBBBDBAFF);

	CFortuneData::_map::iterator	iter = CFortuneData::_mapdata.begin();
	CFortuneData::_map::iterator	eiter = CFortuneData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		stFortune* pInfo = (*iter).second;

		if (pInfo == NULL)
			continue;

		if (pInfo->skill_index != nFortuneIndex ||
			pInfo->skill_level != nFortuneLevel)
			continue;

		strTmp.PrintF(_S(5894, "����: %s"), _S(pInfo->string_index, "����"));
		AddText(strTmp, 0xBD6F3EFF);

		break;
	}

	return true;
}

void CUITooltip::SetSmcInfo()
{
#ifdef VER_TEST
	if (_pNetwork->m_ubGMLevel <= 1)
		return;

	AddText(m_pItemData->GetItemSmcFileName());
#endif // VER_TEST
}

//=============================================================>>
//=============================================================>>
//====================== Skill Info ===========================>>
//=============================================================>>
//=============================================================<<

bool CUITooltip::SetSkillData(CSkill* pSkill, bool bLearn /* = false */, eTOOLTIP_TYPE type /*= eTOOLTIP_FIRST*/)
{
    if (m_pList[type] == NULL)
        return false;

    if (pSkill == NULL || pSkill == m_pSkill)
        return false;

    Hide(FALSE);

    m_pSkill = pSkill;
    m_nIndex = m_pSkill->GetIndex();
    m_bSkillLearn = bLearn;
    m_eType = type;

    CUIManager* pUIManager = CUIManager::getSingleton();

	if (pSkill->GetJob() == PET_JOB)
	{
		LONG	nPetIndex = MY_PET_INFO()->lIndex;

		if (nPetIndex > 0)
			m_nSkillLv = MY_INFO()->GetPetSkillLevel(nPetIndex, m_nIndex);
	}
	else if (pSkill->GetJob() == WILDPET_JOB)
	{
		m_nSkillLv = MY_INFO()->GetPetSkillLevel(0, m_nIndex);
	}
	else
		m_nSkillLv = MY_INFO()->GetSkillLevel(m_nIndex);

    if (m_nSkillLv > 0)
    {
        if (m_bSkillLearn == false)
        {
            m_nSkillLv -= 1;
        }
        else
        {
            if (m_nSkillLv >= m_pSkill->GetMaxLevel())
            {
				if (m_eType == eTOOLTIP_FIRST)
				{
					m_bSkillLearn = false;
					m_nSkillLv -= 1;
				}
				else
				{
					Hide(TRUE);
					return false;
				}
            }
        }
    }
    else
    {
        if (m_eType == eTOOLTIP_SECOND)
        {
            Hide(TRUE);
            return false;
        }
    }

	m_pList[eLIST_FIRST]->SetWidth(DEF_LIST_ORIGIN_W);

    if ( m_bSkillLearn == false ||
	   (m_bSkillLearn == true && m_eType == eTOOLTIP_FIRST) )
        SetIndex();

    SetSkillName();
    SetTypeInfo();

    if (m_bSkillLearn == true && m_eType == eTOOLTIP_FIRST)
        SetSkillDescInfo();

    SetNeed4Stat();
    SetNeed4Skill();
    SetNeed4Item();
    SetNeed4MPHPInfo();
    SetNeed4ItemInfo();
    SetNeed4GPInfo();
    SetPowerInfo();
    SetRangeInfo();
    SetSkillCoolTimeInfo();

    if (m_bSkillLearn == false && m_eType == eTOOLTIP_FIRST)
        SetSkillDescInfo();

    SetAttributeInfo();
	SetGuildSkillTarget();

    UpdateTooltip(m_pImgTempBack[0]);

    return true;
}


bool CUITooltip::SetSSkillData( CSpecialSkill* pSSkill, bool bLearn /*= false*/, eTOOLTIP_TYPE type /*= eTOOLTIP_FIRST*/ )
{
	if (m_pList[type] == NULL)
		return false;

	if (pSSkill == NULL || pSSkill == m_pSSkill)
		return false;

	Hide(FALSE);

	m_pSSkill = pSSkill;
	m_nIndex = m_pSSkill->GetIndex();
	m_bSkillLearn = bLearn;
	m_eType = type;

	CUIManager* pUIManager = CUIManager::getSingleton();
	m_nSkillLv = MY_INFO()->GetSkillLevel(m_nIndex, true);

	if (m_nSkillLv > 0)
	{
		if (m_bSkillLearn == false)
		{
			m_nSkillLv -= 1;
		}
		else
		{
			if (m_nSkillLv >= m_pSSkill->GetMaxLevel())
			{
				Hide(TRUE);
				return false;
			}
		}
	}
	else
	{
		if (m_eType == eTOOLTIP_SECOND)
		{
			Hide(TRUE);
			return false;
		}
	}

	m_pList[eLIST_FIRST]->SetWidth(DEF_LIST_ORIGIN_W);

	if ( m_bSkillLearn == false ||
		(m_bSkillLearn == true && m_eType == eTOOLTIP_FIRST) )
		SetIndex();

	SetSSkillName();

	if (m_bSkillLearn == true && m_eType == eTOOLTIP_FIRST)
		SetSSkillDesc();

	SetSSKillNeed4Stat();
	SetSSkillNeed4Skill();

	if (m_bSkillLearn == false && m_eType == eTOOLTIP_FIRST)
		SetSSkillDesc();


	UpdateTooltip(m_pImgTempBack[0]);

	return true;
}


void CUITooltip::SetSkillName()
{
    if (m_eType == eTOOLTIP_SECOND)
        return;

    CTString strTmp;
    CTString strLv;
    COLOR col = 0x0077FFFF;

    if (m_bSkillLearn == true)
    {
        strLv.PrintF("(%d/%d)", m_nSkillLv, m_pSkill->GetMaxLevel());

        if (m_eType == eTOOLTIP_FIRST && m_nSkillLv > 0)
            --m_nSkillLv;
    }
    else
    {
        strLv.PrintF("%d", m_nSkillLv + 1);
    }

	strTmp.PrintF("%s %s %s", m_pSkill->GetName(), _S(4414, "LV"), strLv);

    AddText(strTmp, col);
}

void CUITooltip::SetTypeInfo()
{
    if (m_eType == eTOOLTIP_SECOND)
        return;

    if (m_pSkill->GetType() == CSkill::ST_SEAL)
        return;

    CTString strTmp;
    COLOR col = 0xCACACAFF;

    if (m_pSkill->GetType() == CSkill::ST_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_PET_SKILL_PASSIVE)
    {
        strTmp = _S(67, "�нú� ��ų");
    }
	else if (m_pSkill->GetType() == CSkill::ST_PET_COMMAND)
	{
		strTmp = _S(2174, "Ŀ�ǵ�");
	}
    else
    {
        if (m_pSkill->GetFlag() & SF_SINGLEMODE)
            strTmp = _S(499, "�۽��δ��� ���� ��ų");
        else if (m_pSkill->GetFlag() & SF_GUILD)
            strTmp = _S(3874, "��� ��ų");
        else
            strTmp = _S(63, "��Ƽ�� ��ų");
    }

    AddText(strTmp, col);
}

void CUITooltip::SetNeed4Stat()
{
    if (m_bSkillLearn == false)
        return;

    CTString strTmp;
    COLOR colLearn = DEF_UI_COLOR_YELLOW;

    // �ʿ� ����.
    int nCondition[6] = { m_pSkill->GetLearnLevel(m_nSkillLv),
                          m_pSkill->GetLearnSP(m_nSkillLv)	,
                          m_pSkill->GetLearnStr(m_nSkillLv)	,
                          m_pSkill->GetLearnDex(m_nSkillLv)	,
                          m_pSkill->GetLearnInt(m_nSkillLv)	,
                          m_pSkill->GetLearnCon(m_nSkillLv)
                        };

    // ������. �ɼ����� ����� �ɷ�ġ�� ����.
    int nStat[6]	  = { _pNetwork->MyCharacterInfo.level		,
                         (_pNetwork->MyCharacterInfo.sp / 10000),
                         (_pNetwork->MyCharacterInfo.str   - _pNetwork->MyCharacterInfo.opt_str)	,
                         (_pNetwork->MyCharacterInfo.dex   - _pNetwork->MyCharacterInfo.opt_dex)	,
                         (_pNetwork->MyCharacterInfo.intel - _pNetwork->MyCharacterInfo.opt_intel)	,
                         (_pNetwork->MyCharacterInfo.con   - _pNetwork->MyCharacterInfo.opt_con)
                     };

    // �ʿ� ����, SP, STR, DEX, INT, CON ��Ʈ�� �ε���.
    int nStr[6] = {256, 257, 1391, 1392, 1393, 1394};

    int i;

    for (i = 0; i < 6; ++i)
    {
        // sp�� 0�϶��� ǥ���������.
        if (i != 1 && nCondition[i] <= 0)
            continue;

        if (m_eType == eTOOLTIP_SECOND || m_nSkillLv <= 0)
        {
            if (nCondition[i] <= nStat[i])
                colLearn = DEF_UI_COLOR_YELLOW;
            else
                colLearn = DEF_UI_COLOR_RED;
        }

		if (nStr[i] == 257)
		{
			CTString strCount = UIMGR()->IntegerToCommaString(nCondition[i]);
			strTmp.PrintF(_S(257, "�ʿ� ���õ� : %s"), strCount);
		}
		else
			strTmp.PrintF(_S(nStr[i], "�ʿ� �ɷ�ġ : %d"), nCondition[i]);

        AddText(strTmp, colLearn);
    }
}

void CUITooltip::SetNeed4Skill()
{
    if (m_bSkillLearn == false)
        return;

    int i, need, lv;

    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

    for (i = 0; i < 3; ++i)
    {
        need = 	m_pSkill->GetLearnSkillIndex(m_nSkillLv, i);

        if (need < 0)
            continue;

        lv = m_pSkill->GetLearnSkillLevel(m_nSkillLv, i);

        CSkill* pSkill = &_pNetwork->GetSkillData(need);

        strTmp.PrintF(" : %s %s.%d", pSkill->GetName(), _S(4414, "LV"), lv);

        strTmp = _S(258, "�ʿ� ��ų") + strTmp;

        if (m_eType == eTOOLTIP_SECOND || m_nSkillLv <= 0)
        {
            int MySkillLv = MY_INFO()->GetSkillLevel(need);

            if (MySkillLv < lv)
                col = DEF_UI_COLOR_RED;
            else if (MySkillLv >= lv)
                col = DEF_UI_COLOR_YELLOW;
        }

        AddText(strTmp, col);
    }
}

void CUITooltip::SetNeed4Item()
{
    if (m_bSkillLearn == false)
        return;

    int i, need, count;

    CTString strTmp;
	CTString strCount;
    COLOR	col = DEF_UI_COLOR_YELLOW;

    for (i = 0; i < 3; ++i)
    {
        need = m_pSkill->GetLearnItemIndex(m_nSkillLv, i);
		
        if (need < 0)
            continue;

        count = m_pSkill->GetLearnItemCount(m_nSkillLv, i);
		strCount = UIMGR()->IntegerToCommaString(count);
        strTmp.PrintF(_S(260, "  %s %s��"), _pNetwork->GetItemName(need), strCount);
        strTmp = _S(259, "�ʿ� ������") + strTmp;

        // eType == eTOOLTIP_FIRST : ���� ��ų ������ �����ͼ� ��. 0���� �۰ų� ���� ��쿡�� ���� �˻�.
        if ((m_eType == eTOOLTIP_SECOND && m_nSkillLv <= 0) ||
            (m_eType == eTOOLTIP_FIRST && MY_INFO()->GetSkillLevel(m_nIndex) <= 0))
        {
            if (need == 19)
            {
                if (_pNetwork->MyCharacterInfo.money < count)
                    col = DEF_UI_COLOR_RED;
            }
            else
            {
                SQUAD nCurrentItemCount = CUIManager::getSingleton()->GetInventory()->GetItemCount(need);

                if (nCurrentItemCount < count)
                    col = DEF_UI_COLOR_RED;
            }
        }

        AddText(strTmp, col);
    }
}

void CUITooltip::SetNeed4MPHPInfo()
{
    if (m_pSkill->GetType() == CSkill::ST_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_PET_SKILL_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_SEAL)
        return;

    int	nNeedMP = m_pSkill->GetNeedMP(m_nSkillLv);
    int	nNeedHP = m_pSkill->GetNeedHP(m_nSkillLv);

    CUIManager* pUIManager = CUIManager::getSingleton();
    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

    int nNeedMPRed = pUIManager->GetNeedMPReductionRate();

    if (nNeedMPRed > 0)
        nNeedMPRed /= 100;

    if (nNeedHP == 0 && nNeedMP > 0)
    {
        if (m_pSkill->GetJob2() != WILDPET_JOB && nNeedMPRed > 0)
            strTmp.PrintF(_S(64, "�Ҹ� MP : %d") + " (-%d)", nNeedMP, nNeedMPRed);
        else
            strTmp.PrintF(_S(64, "�Ҹ� MP : %d"), nNeedMP);

        AddText(strTmp, col);
    }
    else
    {
        if (nNeedMP == 0)
        {
            strTmp.PrintF(_S(500, "�Ҹ� HP : %d"), nNeedHP);
        }
        else
        {
            if (m_pSkill->GetJob2() != WILDPET_JOB && nNeedMPRed > 0)
                strTmp.PrintF(_S(64, "�Ҹ� MP : %d") + " (-%d)", nNeedMP, nNeedMPRed);
            else
                strTmp.PrintF(_S(64, "�Ҹ� MP : %d"), nNeedMP);

            AddText(strTmp, col);
            strTmp.PrintF(_S(500, "�Ҹ� HP : %d"), nNeedHP);
        }

        AddText(strTmp, col);
    }
}

void CUITooltip::SetNeed4ItemInfo()
{
    if (m_pSkill->GetType() == CSkill::ST_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_PET_SKILL_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_SEAL)
        return;

    int		nNeed[2], nCnt[2];

    nNeed[0] = m_pSkill->GetNeedItemIndex1(m_nSkillLv);
    nNeed[1] = m_pSkill->GetNeedItemIndex2(m_nSkillLv);
    nCnt[0] = m_pSkill->GetNeedItemCount1(m_nSkillLv);
    nCnt[1] = m_pSkill->GetNeedItemCount2(m_nSkillLv);

    bool bRet[2];
    SWORD swTab, swIdx;

    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

    int i;

    for (i = 0; i < 2; ++i)
    {
        bRet[i] = ItemHelp::HaveItem(nNeed[i], &swTab, &swIdx, nCnt[i]);

        if (bRet[i] == true && m_bSkillLearn == true)
            col = DEF_UI_COLOR_YELLOW;
    }

    if (nNeed[0] > 0 || nNeed[1] > 0)
    {
        strTmp.PrintF(_S(4405, "�Ҹ� ������"));
        AddText(strTmp, col);

        for (int i = 0; i < 2; ++i)
        {
            if (bRet[i] == true && m_bSkillLearn == true)
                col = DEF_UI_COLOR_YELLOW;
            else
                col = DEF_UI_COLOR_RED;

            if (nNeed[i] > 0)
            {
                const char*	pItemName = _pNetwork->GetItemName(nNeed[i]);
                strTmp.PrintF("- %s(%d)", pItemName, nCnt[i]);
                AddText(strTmp, col);
            }
        }
    }
}

void CUITooltip::SetNeed4GPInfo()
{
    if (m_pSkill->GetType() == CSkill::ST_PET_SKILL_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_SEAL)
        return;

    int nGP = m_pSkill->GetNeedGP(m_nSkillLv);

    CTString strTmp, strTmp2;
    COLOR col = DEF_UI_COLOR_YELLOW;

	if (nGP > 0)
	{
		if (m_eType == eTOOLTIP_SECOND && _pNetwork->MyCharacterInfo.iGP < nGP)
			col = DEF_UI_COLOR_RED;
			
		strTmp2.PrintF("%d", nGP);
		UIMGR()->InsertCommaToString(strTmp2);
		strTmp.PrintF( _S( 5032, "�Ҹ� GP : %s" ), strTmp2);
		AddText(strTmp, col);
	}

	nGP = m_pSkill->GetLearnGP(m_nSkillLv);
	if (nGP > 0)
	{
		if (m_eType == eTOOLTIP_SECOND && _pNetwork->MyCharacterInfo.iGP < nGP)
			col = DEF_UI_COLOR_RED;

		strTmp2.PrintF("%d", nGP);
		UIMGR()->InsertCommaToString(strTmp2);
		strTmp.PrintF(_S(5119, "�ʿ� GP : %s"), strTmp2);
		AddText(strTmp, col);
	}
}

void CUITooltip::SetPowerInfo()
{
    int nPower = m_pSkill->GetPower(m_nSkillLv);

    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

    strTmp.PrintF(_S(65, "���� : %d"), nPower);
    AddText(strTmp, col);
}

void CUITooltip::SetDurationInfo()
{
    if (m_bSkillLearn == false)
        return;

    int nDur = m_pSkill->GetDurTime(m_nSkillLv);

    if (nDur <= 0)
        return;

    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

    strTmp.PrintF(_S(4172, "���ӽð� : %d�� "), nDur / 10);

    AddText(strTmp, col);
}

void CUITooltip::SetRangeInfo()
{
    // ��ų �����϶� ������ ǥ��
    if (m_bSkillLearn == false)
    {
        if (m_pSkill->GetType() == CSkill::ST_PASSIVE ||
            m_pSkill->GetType() == CSkill::ST_PET_SKILL_PASSIVE ||
            m_pSkill->GetType() == CSkill::ST_SEAL)
            return;
    }

    float fRange = m_pSkill->GetFireRange();

    if (fRange <= 0.f)
        return;

    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

    strTmp.PrintF(_S(66, "��ȿ �Ÿ� : %.1f"), fRange);
    AddText(strTmp, col);
}

void CUITooltip::SetSkillCoolTimeInfo(bool bUpdate /*= false*/)
{
    if (m_pSkill->GetType() == CSkill::ST_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_PET_SKILL_PASSIVE ||
        m_pSkill->GetType() == CSkill::ST_SEAL)
        return;

    CTString strTmp;
    COLOR col = DEF_UI_COLOR_YELLOW;

	DOUBLE dStartTime = 0.0;
	DOUBLE dCoolTime;
	DOUBLE dReUseTime = 0.0;

	dStartTime = _pNetwork->GetSkillData(m_nIndex).Skill_Data.Skill_StartTime;
	dReUseTime = MY_INFO()->GetReuseSkill(m_nIndex);

	dCoolTime = ItemHelp::GetCoolTime(dReUseTime, dStartTime);

	if (dCoolTime >= 0.01f)
	{
		strTmp.PrintF(_S(6257, "���� �ð� : %.1f��"), dCoolTime);
		col = DEF_UI_COLOR_RED;
	}
	else
	{
		strTmp.PrintF(_S(6257, "���� �ð� : %.1f��"), dReUseTime);
	}

	if (bUpdate == false)
	{
		AddText(strTmp, col);
		m_nCooltimePos = m_pList[eLIST_FIRST]->getChildCount() - 1;
	}
	else
	{
		// ��Ÿ�� ������Ʈ�� ���� �߰�.
		CUIBase* pItem = m_pList[eLIST_FIRST]->getChildAt(m_nCooltimePos);

		if (pItem == NULL)
			return;

		if (pItem->getType() == eUI_CONTROL_TEXT)
		{
			((CUIText*)pItem)->SetText(strTmp);
			((CUIText*)pItem)->setFontColor(col);
		}
		else if (pItem->getType() == eUI_CONTROL_TEXTBOX)
		{
			((CUITextBox*)pItem)->SetText(strTmp, col);
		}
	}
}

void CUITooltip::SetSkillDescInfo()
{
    CTString strTmp = m_pSkill->GetDescription();
    COLOR col = 0x72D02EFF;

    if (strTmp.IsEmpty() == TRUE)
        return;

    AddText(strTmp, col);

    if (m_pList[eLIST_FIRST] == NULL)
        return;

    // �ι�° ������ ���� ���� ������ �����Ѵ�.
    m_nSecondPos = m_pList[eLIST_FIRST]->getChildCount() - 1;
}

void CUITooltip::SetAttributeInfo()
{
    if (m_pSkill->GetType() == CSkill::ST_SEAL)
        return;

    int nAttrAtt = int(m_pSkill->GetAttrAtt(m_nSkillLv));
    int nAttrDef = int(m_pSkill->GetAttrDef(m_nSkillLv));

    // ��, ��, ����, �ٶ�, ����, ��
    int nAttribute[] = {5846, 5847, 5849, 5848, 5850, 5851};

    CTString strTmp;
    CTString strAtt;

    if (nAttrAtt > 0 && nAttrAtt <= 6)
    {
        int nAttLv = int(m_pSkill->GetAttrAttLv(m_nSkillLv));

        strAtt = _S(nAttribute[nAttrAtt - 1], "Att");
        strTmp.PrintF(_S(5845, "�Ӽ� ���� : [%s]�Ӽ� [%d]�ܰ�"), strAtt, nAttLv);

        AddText(strTmp, DEF_UI_COLOR_RED);
    }

    if (nAttrDef > 0 && nAttrDef <= 6)
    {
        int nAttLv = int(m_pSkill->GetAttrDefLv(m_nSkillLv));

        strAtt = _S(nAttribute[nAttrDef - 1], "Def");
        strTmp.PrintF(_S(5845, "�Ӽ� ���� : [%s]�Ӽ� [%d]�ܰ�"), strAtt, nAttLv);

        AddText(strTmp, DEF_UI_COLOR_BLUE);
    }
}

void CUITooltip::SetGuildSkillTarget()
{
	char nTargetType = m_pSkill->GetTargetType();
	CTString strTmp;

	COLOR	col = 0x00ffffff;

	if ( nTargetType == CSkill::STT_GUILD_ALL )
		strTmp.PrintF( _S( 5121, "Ÿ�ٴ�� : ���� ��ü" ));
	else if ( nTargetType == CSkill::STT_GUILD_ONE )
		strTmp.PrintF( _S( 5122, "Ÿ�ٴ�� : ���� �Ѹ�" ));
	else if ( nTargetType == CSkill::STT_GUILD_SELF_RANGE )
		strTmp.PrintF( _S ( 5123, "Ÿ�ٴ�� : �ֺ� ��" ));
	else if ( nTargetType == CSkill::STT_GUILD_MEMBER_SELF)
		strTmp.PrintF( _S ( 6437, "Ÿ�ٴ�� : ���� �ڽ�" ));
	else
		return;

	AddText(strTmp, col);
}

//======================================================>>
//======================================================>>
//=============== ����� ��ų ���� =====================>>
//======================================================>>
//======================================================<<

void CUITooltip::SetSSkillName()
{
	if (m_eType == eTOOLTIP_SECOND)
		return;

	CTString strTmp;
	CTString strLv;
	COLOR col = 0x0077FFFF;

	if (m_bSkillLearn == true)
	{
		strLv.PrintF("(%d/%d)", m_nSkillLv, m_pSSkill->GetMaxLevel());

		if (m_eType == eTOOLTIP_FIRST && m_nSkillLv > 0)
			--m_nSkillLv;
	}
	else
	{
		strLv.PrintF("%d", m_nSkillLv + 1);
	}

	strTmp.PrintF("%s Lv %s", m_pSSkill->GetName(), strLv);

	AddText(strTmp, col);
}

void CUITooltip::SetSSKillNeed4Stat()
{
	if (m_bSkillLearn == false)
		return;

	CTString strTmp;
	COLOR colLearn = DEF_UI_COLOR_YELLOW;

	int nCondition, nStat;

	// �ʿ� ����
	nCondition = m_pSSkill->GetLearnLevel(m_nSkillLv);
	nStat = _pNetwork->MyCharacterInfo.level;

	if (nCondition > 0)
	{
		if (m_eType == eTOOLTIP_SECOND || m_nSkillLv <= 0)
		{
			if (nCondition <= nStat)
				colLearn = DEF_UI_COLOR_YELLOW;
			else
				colLearn = DEF_UI_COLOR_RED;
		}

		strTmp.PrintF(_S(256, "�ʿ� ���� : %d"), nCondition);
		AddText(strTmp, colLearn);
	}

	// �ʿ� SP
	nCondition = m_pSSkill->GetLearnSP(m_nSkillLv);
	nStat = _pNetwork->MyCharacterInfo.sp / 10000;
	
	// sp�� 0�϶��� ǥ���������.
	if (m_eType == eTOOLTIP_SECOND || m_nSkillLv <= 0)
	{
		if (nCondition <= nStat)
			colLearn = DEF_UI_COLOR_YELLOW;
		else
			colLearn = DEF_UI_COLOR_RED;
	}

	CTString strSp = UIMGR()->IntegerToCommaString(nCondition);
	strTmp.PrintF(_S(257, "�ʿ� ���õ� : %s"), strSp);
	AddText(strTmp, colLearn);
}

void CUITooltip::SetSSkillNeed4Skill()
{
	if (m_bSkillLearn == false)
		return;

	int need, lv;

	CTString strTmp;
	COLOR col = DEF_UI_COLOR_YELLOW;

	need = 	m_pSSkill->GetLearnSkillIndex();

	if (need < 0)
		return;

	lv = m_pSSkill->GetLearnSkillLevel();

	CSpecialSkill* pSSkill = CSpecialSkill::getData(need);

	strTmp.PrintF(" : %s %s.%d", pSSkill->GetName(), _S(4414, "LV"), lv);

	strTmp = _S(258, "�ʿ� ��ų") + strTmp;

	if (m_eType == eTOOLTIP_SECOND || m_nSkillLv <= 0)
	{
		int MySkillLv = MY_INFO()->GetSkillLevel(need, true);

		if (MySkillLv < lv)
			col = DEF_UI_COLOR_RED;
		else if (MySkillLv >= lv)
			col = DEF_UI_COLOR_YELLOW;
	}

	AddText(strTmp, col);
}

void CUITooltip::SetSSkillDesc()
{
	CTString strTmp = m_pSSkill->GetDescription();
	COLOR col = 0x72D02EFF;

	if (strTmp.IsEmpty() == TRUE)
		return;

	AddText(strTmp, col);

	if (m_pList[eLIST_FIRST] == NULL)
		return;

	// �ι�° ������ ���� ���� ������ �����Ѵ�.
	m_nSecondPos = m_pList[eLIST_FIRST]->getChildCount() - 1;
}

//======================================================>>
//======================================================>>
//================== �׼� ���� =========================>>
//======================================================>>
//======================================================<<

bool CUITooltip::SetActionData(CAction* pAction, eTOOLTIP_TYPE type /*= eTOOLTIP_FIRST*/)
{
    if (m_pList[type] == NULL)
        return false;

    if (pAction == NULL || pAction == m_pAction)
        return false;

    Hide(FALSE);

    m_pAction = pAction;
    m_nIndex = pAction->GetIndex();
    m_eType = type;

	m_pList[eLIST_FIRST]->SetWidth(DEF_LIST_ORIGIN_W);

    SetIndex();
    SetActionName();
    SetActionDescInfo();

    UpdateTooltip(m_pImgTempBack[0]);

    return true;
}

void CUITooltip::SetActionName()
{
    AddText(m_pAction->GetName());
}

void CUITooltip::SetActionDescInfo()
{
    CTString strTmp = m_pAction->GetDesc();

    if (strTmp.IsEmpty() == TRUE)
        return;

    AddText(strTmp, 0x9E9684FF);
}

void CUITooltip::AddText(CTString str, COLOR col /*= 0xFFFFFFFF*/, eALIGN_H alingn /*= eALIGN_H_LEFT*/, eLIST_TYPE eType /*=eLIST_FIRST*/, int nMaxChar /*= 0*/)
{
    CUITooltipMgr* pTooltipMgr = CUITooltipMgr::getSingleton();
    CUIManager* pUIMgr = CUIManager::getSingleton();

    CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

    ULONG ulTextWidth = 0;

	if (g_iCountry != RUSSIA)
	{
		ulTextWidth = pDrawPort->GetTextWidth2(str);
	}
	else
	{
		extern CFontData *_pfdDefaultFont;
		ulTextWidth = UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, str.str_String);
	}

    if (ulTextWidth <= m_pList[eType]->GetWidth())
    {
        CUIText* pText = (CUIText*)pTooltipMgr->GetTooltipRes()->CloneText();
		pText->setAlignTextH(alingn);
        pText->SetText(str);
        pText->setFontColor(col);
		pText->SetWidth(m_pList[eType]->GetWidth());

        m_pList[eType]->AddListItem((CUIBase*)pText);
    }
    else
    {
        CUITextBox* pTextBox = (CUITextBox*)pTooltipMgr->GetTooltipRes()->CloneTextBox();
		pTextBox->SetAlignTextH(alingn);
		pTextBox->SetWidth(m_pList[eType]->GetWidth());

		if (nMaxChar > 0)
		{
			pTextBox->SetSplitMode((eSPLIT_TYPE)4);
			pTextBox->SetMaxChar(nMaxChar);
		}

        pTextBox->SetText(str, col, TRUE);
        m_pList[eType]->AddListItem((CUIBase*)pTextBox);
    }
}

int CUITooltip::GetSecondPos()
{
    if (m_pList[eLIST_FIRST] == NULL)
        return 0;

	if (m_nSecondPos == -1)
		return GetAbsPosY();

    CUIBase* pChild = m_pList[eLIST_FIRST]->GetListItem(m_nSecondPos);

    if (pChild == NULL)
        return GetAbsPosY();

    return pChild->GetAbsPosY() + pChild->GetHeight() - DEF_LIST_SECOND_GAP_Y;
}

void CUITooltip::UpdateTooltip(CUIImageSplit* pBackGround)
{
	if (m_pList[eLIST_FIRST] == NULL || m_pList[eLIST_SECOND] == NULL)
		return;

	m_pImgBack = pBackGround;

	int nWidth = 0, nHeight = 0;
	// ���� + ������ ��� ��
	int nGapW = 30;
	// �� + �Ʒ� ��� ��.
	int nGapH = 18;

	for (int i = 0; i < eLIST_END; ++i)
	{
		if (m_pList[i]->getListItemCount() <= 0)
			continue;

		m_pList[i]->UpdateList();

		m_pList[i]->GetItemTotalHeight();

		// ��� ����� ����ϱ� ���Ѱ��̸�, Second�� Height�� �� ũ�ٸ� ��� ������� Second�� Height�� ����.
		if (nHeight < m_pList[i]->GetItemTotalHeight())
			nHeight = m_pList[i]->GetItemTotalHeight();

		nWidth += m_pList[i]->GetWidth();

		if (i == eLIST_FIRST)
		{
			// ����Ʈ ���� ����.
			m_pList[i]->SetPosX(((nWidth + nGapW) / 2) - (m_pList[i]->GetWidth() / 2));
		}
		else
		{
			m_pList[i]->SetPosX(m_pList[eLIST_FIRST]->GetPosX() + m_pList[i]->GetWidth() + DEF_LIST_SECOND_GAP);
			nGapW += DEF_LIST_SECOND_GAP;
		}

		m_pList[i]->SetPosY(((nHeight + nGapH) / 2) - (nHeight / 2));		
	}

	// UI��ü ���̽� ������. ����Ʈ + ���� ���� ����Ʈ���� �� ũ�� ����.
	SetSize(nWidth + nGapW, nHeight + nGapH);

	// ��� ������.
	m_pImgBack->SetSize(GetWidth(), GetHeight());
	m_pImgBack->UpdateSplit();
	m_pImgBack->Hide(FALSE);
}

void CUITooltip::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_pSkill != NULL && m_nCooltimePos >= 0)
		SetSkillCoolTimeInfo(true);
	else if (m_pItem != NULL && m_pItemData != NULL)
		SetItemCoolTimeInfo(true);
	else
		TOOLTIPMGR()->updateTooltipText(fDeltaTime);
}

int CUITooltip::calc_pet_item_plus( int orig_value, ULONG plus )
{
	int nRet = orig_value * pow(1.09, (double)plus);

	return (nRet - orig_value);
}