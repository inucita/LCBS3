#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UITooltipResource.h>
#include <Engine/Contents/Base/InvenData.h>
#include <Engine/Contents/Base/UISkillNew.h>

#define		DEF_DEFAULT_GAP		8
#define		DEF_ORIG_IMAGE_WIDTH	85
#define		DEF_TB_TEMP_HEIGHT	500
#define		DEF_TOOLTIP_UPDATE_TIME		1.f

CUITooltipMgr::CUITooltipMgr()
    : m_pUI(NULL)
    , m_pTooltipRes(NULL)
	, m_nWearPos(-1)
	, m_fDeltaTime(0.f)
{
    int i;

    for (i = 0; i < eTOOLTIP_END; ++i)
    {
        m_pTooltip[i] = NULL;
    }
}

CUITooltipMgr::~CUITooltipMgr()
{
    clear();
    SAFE_DELETE(m_pTooltipRes);

    int i;

    for (i = 0; i < eTOOLTIP_END; ++i)
    {
        m_pTooltip[i] = NULL;
    }
}

void CUITooltipMgr::initialize()
{
    m_pTooltipRes = new CUITooltipRes;
    CUIManager::getSingleton()->LoadXML("UITooltip.xml", (CUIBase*)m_pTooltipRes);

    int i;

    for (i = 0; i < eTOOLTIP_END; ++i)
    {
        m_pTooltip[i] = new CUITooltip;
        m_pTooltip[i]->initialize();

        addChild(m_pTooltip[i]);
    }
}

void CUITooltipMgr::clear()
{
    clearTooltip();
    Destroy();
}

void CUITooltipMgr::clearTooltip()
{
    if (m_pUI != NULL)
    {
        m_pUI->OnLeave(0, 0);
    }

    m_pUI = NULL;

	int i;

	for (i = 0; i < eTOOLTIP_END; ++i)
	{
		if (m_pTooltip[i] != NULL)
			m_pTooltip[i]->ClearData();
	}

	m_nWearPos = -1;
}

void CUITooltipMgr::setData( CUIBase* pUI )
{
	clearTooltip();

	m_pUI = pUI;

	if (pUI == NULL)
		return;

	if (pUI->getType() == eUI_CONTROL_ICON)
	{
		bool bRet = false;
		bool bSub = false;

		CUIIcon* pIcon = static_cast< CUIIcon* >(pUI);

		if (pIcon->GetDisable() == true)	// Disable�����϶� ������ ������� �ʴ´�.
			return;

		int nWitchUI = pIcon->GetWhichUI();

		m_pTooltip[eTOOLTIP_FIRST]->SetWhichUI(nWitchUI);
		m_pTooltip[eTOOLTIP_SECOND]->SetWhichUI(nWitchUI);

		switch (pIcon->getBtnType())
		{
		case UBET_SKILL:
			{
				if (pIcon->getIndex() > 0)
				{
					if (pIcon->IsSpecial() == false)
					{
						CSkill* pSkill = &(_pNetwork->GetSkillData(pIcon->getIndex()));
	
						if (nWitchUI == UI_GUILD)
							bSub = true;
						else
							bSub  = UIMGR()->GetSkillNew()->IsSkillLearn();
	
						bRet = m_pTooltip[eTOOLTIP_FIRST]->SetSkillData(pSkill, bSub);
	
						if (bSub == true)
							m_bSecond = m_pTooltip[eTOOLTIP_SECOND]->SetSkillData(pSkill, bSub, eTOOLTIP_SECOND);
					}
					else	// ����� ��ų.
					{
						CSpecialSkill* pSSkill = CSpecialSkill::getData(pIcon->getIndex());

						bSub  = (nWitchUI == UI_SKILLLEARN) || (nWitchUI == UI_SSKILLLEARN);

						bRet = m_pTooltip[eTOOLTIP_FIRST]->SetSSkillData(pSSkill, bSub);

						if (bSub == true)
							m_bSecond = m_pTooltip[eTOOLTIP_SECOND]->SetSSkillData(pSSkill, bSub, eTOOLTIP_SECOND);
					}
				}
			}
			break;

		case UBET_ACTION:
			{
				CAction* pAction = CAction::getData(pIcon->getIndex());

				bRet = m_pTooltip[eTOOLTIP_FIRST]->SetActionData(pAction);
			}
			break;

		case UBET_ITEM:
			{
				if (pIcon->getBtnEtcType() >= eETC_TYPE_NAS)
				{
					 stItemEtc* pItem = pIcon->getItemEtc();

					 if (pItem == NULL)
						 return;

					 bRet = m_pTooltip[eTOOLTIP_FIRST]->SetItemEtcData(pItem, pIcon->getBtnEtcType());
				}
				else
				{
					 CItems* pItem = pIcon->getItems();

					 if (pItem == NULL)
						 return;

					 if (nWitchUI == UI_CASHSHOP_EX)
					 {
						  bRet = m_pTooltip[eTOOLTIP_FIRST]->SetCashData(pItem, pIcon->GetCashIndex());
						  break;
					 }

					 if (pItem->Item_Index == NAS_INDEX)
						 return;

					 m_nWearPos = GetWearPos(pItem);

					 if (m_nWearPos >= 0)
						 bRet = m_pTooltip[eTOOLTIP_FIRST]->SetItemData(&_pNetwork->MyWearItem[m_nWearPos], eTOOLTIP_FIRST, m_nWearPos);
					 else
						 bRet = m_pTooltip[eTOOLTIP_FIRST]->SetItemData(pItem, eTOOLTIP_FIRST, m_nWearPos);

					 m_bSecond = m_pTooltip[eTOOLTIP_SECOND]->SetItemData(pItem, eTOOLTIP_SECOND, m_nWearPos);
				 }
			}
			break;
		default:
			{
				if (pIcon->getBtnEtcType() >= eETC_TYPE_NAS)
				{
					stItemEtc* pItem = pIcon->getItemEtc();

					if (pItem == NULL)
						return;

					bRet = m_pTooltip[eTOOLTIP_FIRST]->SetItemEtcData(pItem, pIcon->getBtnEtcType());
				}
				else
				{
					m_pTooltip[eTOOLTIP_FIRST]->SetString(pUI);
					UpdateTooltipPos(UIRect(pUI->GetAbsPosX(), pUI->GetAbsPosY(), pUI->GetWidth(), pUI->GetHeight()), true);
					return;
				}
			}
			break;
		}

		if (bRet == false)
			return;

		UpdateTooltipPos(UIRect(pIcon->GetAbsPosX(), pIcon->GetAbsPosY(), pIcon->GetWidth(), pIcon->GetHeight()), false);
	}
	else
	{
		if (m_pTooltip[eTOOLTIP_FIRST]->SetString(pUI) == true)
			UpdateTooltipPos(UIRect(pUI->GetAbsPosX(), pUI->GetAbsPosY(), pUI->GetWidth(), pUI->GetHeight()), true);
	}
}

void CUITooltipMgr::hideUI(CUIBase* pUI)
{
	int i;

	if (m_pUI == pUI)
	{
		for (i = 0; i < eTOOLTIP_END; ++i)
		{
			if (m_pTooltip[i] != NULL)
				m_pTooltip[i]->ClearData();
		}

		m_pUI = NULL;
	}
}

void CUITooltipMgr::OnRender(CDrawPort* pDraw)
{
}

void CUITooltipMgr::UpdateTooltipPos(UIRect rcPos, bool bString)
{
    int w, h;

    m_pTooltip[eTOOLTIP_FIRST]->GetTooltipSize(w, h);

	int nPosX = (w / 2) - (rcPos.Right / 2);

	if (bString == false)
    {
        rcPos.Top += rcPos.Bottom;
        rcPos.Left -= nPosX;

		if (m_nWearPos >= 0)
			rcPos.Left -= (m_pTooltip[eTOOLTIP_SECOND]->GetWidth() / 2) + (w / 2);
    }
    else
    {
        rcPos.Top += rcPos.Bottom / 2;
        rcPos.Left += rcPos.Right / 2;
    }

    CUIManager* pUIManager = CUIManager::getSingleton();

    int nClientW = pUIManager->GetWidth();
    int nClientH = pUIManager->GetHeight();
	int nSecondW = 0;
	bool bTop = false;

	if (m_bSecond == true)
	{
		w += m_pTooltip[eTOOLTIP_SECOND]->GetWidth();
	}

    if (rcPos.Left < 0)
        rcPos.Left = 0;

    if (rcPos.Left + w> nClientW)
	{
		if (bString == false)
			rcPos.Left = nClientW - w;
		else
			rcPos.Left -= w;
	}

    if (rcPos.Top + h > nClientH)
	{
        rcPos.Top -= (rcPos.Bottom + h);
		bTop = true;
	}

    m_pTooltip[eTOOLTIP_FIRST]->SetPos(rcPos.Left, rcPos.Top);

    if (m_bSecond == true)
    {
		if (m_nWearPos >= 0 && bTop)
		{
			int nPosY = m_pTooltip[eTOOLTIP_FIRST]->GetAbsPosY() + m_pTooltip[eTOOLTIP_FIRST]->GetHeight();

			nPosY -= m_pTooltip[eTOOLTIP_SECOND]->GetHeight();

			m_pTooltip[eTOOLTIP_SECOND]->SetPos(rcPos.Left + m_pTooltip[eTOOLTIP_FIRST]->GetWidth(), nPosY);
		}
		else
		{
			m_pTooltip[eTOOLTIP_SECOND]->SetPos(rcPos.Left + m_pTooltip[eTOOLTIP_FIRST]->GetWidth(), m_pTooltip[eTOOLTIP_FIRST]->GetSecondPos());
		}
    }
}

int CUITooltipMgr::GetWearPos( CItems* pItems )
{
	if (pItems->Item_Index <= 0)
		return -1;

	// �����ϰ� �ִ� �����ۿ� ���콺�� �÷�����.
	if (pItems->Item_Wearing >= 0)
		return -1;

	if (!(pItems->ItemData->GetType() == CItemData::ITEM_WEAPON) &&
		!(pItems->ItemData->GetType() == CItemData::ITEM_SHIELD) && 
		!(pItems->ItemData->GetType() == CItemData::ITEM_ACCESSORY))
		return -1;

	// ���� ��� ����~
	if (pItems->ItemData->GetType() == CItemData::ITEM_ACCESSORY &&
		(pItems->ItemData->GetSubType() == CItemData::ACCESSORY_WILDPET ||
		pItems->ItemData->GetSubType() == CItemData::ACCESSORY_PET))
		return -1;

	if (pItems->ItemData->GetFlag() & ITEM_FLAG_COSTUME2)
		return -1;

	int nWearingPos = pItems->ItemData->GetWearingPosition();

	// ���ϰ��� �ϴ� �������� �Ǽ��縮�� ���.
	if (nWearingPos == WEAR_ACCESSORY1)
	{
		for (int i = WEAR_ACCESSORY1; i < WEAR_ACCESSORY4; ++i)
		{
			if (GetItemCompare(i, pItems) != -1)
				return i;
		}
	}
	else
	{
		for (int i = 0; i < WEAR_TOTAL; ++i)
		{
			if (GetItemCompare(i, pItems) != -1)
				return i;
		}
	}

	return -1;
}

int CUITooltipMgr::GetItemCompare( int pos, CItems* pItems )
{
	if (_pNetwork->MyWearItem[pos].IsEmptyItem() == TRUE)
		return -1;

	if (_pNetwork->MyWearItem[pos].Item_UniIndex == pItems->Item_UniIndex)
		return -1;

	if (pItems->IsFlag(FLAG_ITEM_COMPOSITION) ||
		_pNetwork->MyWearItem[pos].IsFlag(FLAG_ITEM_COMPOSITION))
		return -1;

	if (pItems->ItemData->GetType() == _pNetwork->MyWearItem[pos].ItemData->GetType() &&
		pItems->ItemData->GetSubType() == _pNetwork->MyWearItem[pos].ItemData->GetSubType() &&
		pItems->ItemData->GetJob() == _pNetwork->MyWearItem[pos].ItemData->GetJob())
		return pos;

	return -1;
}

void CUITooltipMgr::updateTooltipText( float fDelta )
{
	if (m_pUI == NULL)
		return;

	if (m_pUI->hasUpdateTooltipF() == false)
		return;

	m_fDeltaTime += fDelta;

	// ����(150904) �ʴ��� ������Ʈ�� �ʿ��ϹǷ�, 1�� �������� ������Ʈ �Ѵ�.
	if (m_fDeltaTime < DEF_TOOLTIP_UPDATE_TIME)
		return;

	while (m_fDeltaTime > DEF_TOOLTIP_UPDATE_TIME)
		m_fDeltaTime -= DEF_TOOLTIP_UPDATE_TIME;

	m_pUI->updateTooltipTextF();

	// setData�� ȣ�� �� ���, 
	// ���ο��� clearTooltip() ȣ��Ǿ�, OnLeave �� �ҷ���. (m_bEnter ���� ����)
	//setData(m_pUI);

	// �Ϲ� ������ ������ (150923)
	// ��� ������ �����Ϸ��� setData ���� �ʿ�.

	m_pTooltip[eTOOLTIP_FIRST]->ClearData();

	if (m_pTooltip[eTOOLTIP_FIRST]->SetString(m_pUI) == true)
		UpdateTooltipPos(UIRect(m_pUI->GetAbsPosX(), m_pUI->GetAbsPosY(), m_pUI->GetWidth(), m_pUI->GetHeight()), true);
}
