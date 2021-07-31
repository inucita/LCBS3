#include "stdh.h"
#include <vector>
#include <algorithm>
#include "PetFreeUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Contents/function/WildPetInfoUI.h>
#include <Engine/Interface/UIInventory.h>

CPetFreeUI::CPetFreeUI()
	: m_pMoveArea(NULL)
	, m_bDrag(false)
	, m_nOriX(0), m_nOriY(0)
	, m_nMouseX(0)
	, m_nMouseY(0)
	, m_pstrNas(NULL)
	, m_pIconPet(NULL)
	, m_pTbContent(NULL)
	, m_bPremiumChar(false)
{
	for (int i = 0; i < eBTN_MAX; ++ i)
		m_pBtn[i] = NULL;

	m_fNpcPosX = 0.0f;
	m_fNpcPosZ = 0.0f;
	m_nNpcIndex = -1;
}

void CPetFreeUI::PressOK()
{
	_setOkBtnEnable(FALSE);
	SendPetFreeReq();
}

void CPetFreeUI::SendPetFreeReq()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (m_pIconPet == NULL || m_pIconPet->IsEmpty() == true)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2448, "���� ���� �� ���� �����ϴ�." ), SYSMSG_ERROR );		
		return;
	}

	if( ( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->IsSkilling() )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2446, "��ų ����߿��� ���� ������ ���� �� �� �����ϴ�." ), SYSMSG_ERROR );	
		return;
	}

	if( pUIManager->IsCSFlagOn( CSF_TELEPORT ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2447, "���� �̵��߿��� ������ ���� �� �� �����ϴ�." ), SYSMSG_ERROR );	
		return;
	}

	if(m_nPettype == CItemData::ACCESSORY_PET)
	{// ��Ʈ ��ũ �޼��� ~~
		_pNetwork->SendPetRebirth( m_slPetIndex );
	}
	else if(m_nPettype == CItemData::ACCESSORY_WILDPET)
	{
		CItems* pItems = m_pIconPet->getItems();

		if (pItems == NULL)
			return;

		if (pItems->Item_Wearing >= 0)
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S( 4765, "������ �����Ͽ��� ��Ȱ�� �����մϴ�." ), SYSMSG_ERROR );		
			return;
		}

		int nTab = pItems->Item_Tab;
		int	nIdx = pItems->InvenIndex;
		_pNetwork->SendWildPetRebirth( nTab, nIdx, m_nNpcIndex);
	}
}

void CPetFreeUI::PetFreeError( SLONG PetIndex, SBYTE sbResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	CTString	strMessage;
	CUIMsgBox_Info	MsgBoxInfo;
	
	if( PetIndex != m_slPetIndex )
	{
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"Ȯ��"), UMBS_OK, UI_PETFREE, UI_NONE );
		strMessage.PrintF( _S( 2449, "���������� ��û�Ͻ� ��� ������ ���� �ٸ��ϴ�.") );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		ResetUI();
		return;
	}

	switch( sbResult )
	{
	case MSG_EX_PET_REBIRTH_ERROR_OK:			// ����
		MsgBoxInfo.SetMsgBoxInfo( _S(191,"Ȯ��"), UMBS_OK, UI_PETFREE, UI_NONE );
		strMessage.PrintF( _S( 2450, "���� ������ �����ϴµ� �����Ͽ����ϴ�.") );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
		closeUI();
		return;
	case MSG_EX_PET_REBIRTH_ERROR_NOMONEY:		// ���� ����
		pUIManager->GetChattingUI()->AddSysMessage( 
			_S( 2451, "���� ������ �����ϱ� ���� ����� �����մϴ�." ), SYSMSG_ERROR );		
		break;
	case MSG_EX_PET_REBIRTH_ERROR_NOPET:		// �� ���� ����
		pUIManager->GetChattingUI()->AddSysMessage( 
			_S( 2452, "������ ������ ���� �������� �ʽ��ϴ�." ), SYSMSG_ERROR );
		ResetUI();
		break;
	case MSG_EX_PET_REBIRTH_ERROR_NODEAD:		// �� �������
		pUIManager->GetChattingUI()->AddSysMessage( 
			_S( 2453, "���� ���ε��� �ʾҽ��ϴ�." ), SYSMSG_ERROR );
		ResetUI();
		break;
	}
}

void CPetFreeUI::initialize()
{
#ifndef		WORLD_EDITOR

	std::string strBtnID[eBTN_MAX] = { "btn_ok", "btn_cancel", "btn_close" };

	for (int i = 0; i < eBTN_MAX; ++ i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CPetFreeUI::PressOK, this));

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CPetFreeUI::closeUI, this));
	
	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CPetFreeUI::closeUI, this));

	m_pstrNas = (CUIText*)findUI("str_nas");
	m_pIconPet = (CUIIcon*)findUI("icon_petItem");
	m_pTbContent = (CUITextBox*)findUI("tb_contents");

	if (m_pTbContent != NULL)
	{
		CTString strTemp = CTString(_S( 2441, "���ε� ���� ��Ȱ��ŵ�ϴ�."));
		strTemp += "\\n";
		strTemp += CTString(_S( 2442, "������ ���� ��Ȱ����� �����ؾ�"));
		strTemp += CTString(_S( 2443, "�մϴ�."));

		m_pTbContent->SetText(strTemp);
	}

	// drag
	m_pMoveArea = (CUIImage*)findUI("move_area");

#endif		// WORLD_EDITOR
}

void CPetFreeUI::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	if (m_bPremiumChar == false)
	{
		FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcPosX;
		FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcPosZ;

		if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
			closeUI();
	}
}

void CPetFreeUI::openUI(float fX, float fZ, int nIdex)
{
	if (GetHide() == FALSE)
		return;

	CUIManager* pUIMgr = UIMGR();
	CUIInventory* pInven = pUIMgr->GetInventory();

	if( pInven->IsLocked() == TRUE ||
		pInven->IsLockedArrange() == TRUE)
	{
		pInven->ShowLockErrorMessage();
		return;
	}

	Hide(FALSE);
	SetVisible(TRUE);

	m_fNpcPosX = fX;
	m_fNpcPosZ = fZ;
	m_nNpcIndex = nIdex;

	ResetUI();

	pUIMgr->RearrangeOrder( UI_PETFREE, TRUE );
	pInven->Lock(TRUE, TRUE, LOCK_PET_FREE);
}

void CPetFreeUI::closeUI()
{
	Hide(TRUE);
	SetVisible(FALSE);

	m_fNpcPosX = 0.0f;
	m_fNpcPosZ = 0.0f;
	m_nNpcIndex = -1;

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_PETFREE, FALSE );
	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_PET_FREE);
}

WMSG_RESULT CPetFreeUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_nMouseX = x;
	m_nMouseY = y;

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

WMSG_RESULT CPetFreeUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pMoveArea && m_pMoveArea->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_PETFREE, TRUE );

	return WMSG_FAIL;
}

WMSG_RESULT CPetFreeUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag != NULL && pDrag->IsEmpty() == false)
	{
		if (m_pIconPet != NULL)
		{
			if (_CheckPetItem() == true)
			{
				m_pIconPet->copyItem(pDrag);
				_setOkBtnEnable(TRUE);
			}
		}
	}

	pUIManager->ResetHoldBtn();
	return WMSG_FAIL;
}

void CPetFreeUI::ResetUI()
{
	// �� ������ �ʱ�ȭ
	_resetPetSlot();
	// ���� �ʱ�ȭ
	_setNas();
	// ���� ���� ��ư �ʱ�ȭ
	_setOkBtnEnable(FALSE);
}

void CPetFreeUI::_resetPetSlot()
{
	if (m_pIconPet != NULL)
		m_pIconPet->clearIconData();
}

void CPetFreeUI::_setNas( SQUAD llNas )
{
	if (m_pstrNas != NULL)
	{
		CTString strTemp = CTString("");
		if (llNas >= 0)
			strTemp.PrintF( "%I64d", llNas );

		UIMGR()->InsertCommaToString(strTemp);
		m_pstrNas->SetText(strTemp);
	}
}

void CPetFreeUI::_setOkBtnEnable( BOOL bEnable )
{
	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetEnable(bEnable);
}

SQUAD CPetFreeUI::_calcPetFreeConsumeNas( int nPetLevel )
{
	if( nPetLevel < 0 )
		return 0;

	SQUAD llPrice = 0;
	if (m_nPettype == CItemData::ACCESSORY_PET)
		llPrice = ( nPetLevel - 1 ) * ( nPetLevel * nPetLevel * 7 );
	else if (m_nPettype == CItemData::ACCESSORY_WILDPET)
		llPrice = (nPetLevel - 1) * ( ( nPetLevel * nPetLevel * 8 ) + 20000);
	return llPrice;
}

bool CPetFreeUI::_CheckPetItem()
{
	if (m_pIconPet == NULL)
		return false;

	CUIManager* pUIManager = CUIManager::getSingleton();
	CUIIcon* pDrag = pUIManager->GetDragIcon();

	if (pDrag == NULL)
		return false;

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return false;

	CItemData*	pItemData = pItems->ItemData;

	if( !(pItemData->GetSubType() == CItemData::ACCESSORY_PET || pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET))
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 2445, "�� �����۸� �����մϴ�." ), SYSMSG_ERROR );		
		return false;
	}

	int nPetLevel = 0;
	if(pItemData->GetSubType() == CItemData::ACCESSORY_PET )
	{
		m_nPettype = CItemData::ACCESSORY_PET;
		CNetworkLibrary::sPetInfo	TempPet;
		TempPet.lIndex = pItems->Item_Plus;	// �� �ĺ��ε���
		m_slPetIndex = TempPet.lIndex; // �� �ĺ� �ε��� ����

		std::vector<CNetworkLibrary::sPetInfo>::iterator iter = 
			std::find_if(_pNetwork->m_vectorPetList.begin(), _pNetwork->m_vectorPetList.end(), CNetworkLibrary::FindPet(TempPet) );

		if( iter != _pNetwork->m_vectorPetList.end() )	// ���� ã�Ҵ�.
			nPetLevel = (*iter).lLevel;
		else
			return false;
	}
	else if(pItemData->GetSubType() == CItemData::ACCESSORY_WILDPET )
	{
		m_nPettype = CItemData::ACCESSORY_WILDPET;
		sPetItem_Info temPet;
		pUIManager->GetWildPetInfoUI()->GetWildPetInfo(pItems->Item_Plus, temPet);
		nPetLevel = temPet.pet_level;
	}

	_setNas(_calcPetFreeConsumeNas(nPetLevel));
	return true;
}
