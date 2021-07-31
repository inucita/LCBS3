#include "StdH.h"
#include "Durability.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/Base/UIDurability.h>


CDurability::CDurability()
{
}

CDurability::~CDurability()
{
}

void CDurability::RecvDurabilityMsg( CNetworkMessage* istr )
{
	pTypeBase* pPacket = reinterpret_cast<pTypeBase*>(istr->GetBuffer());

	switch(pPacket->subType)
	{
	case MSG_SUB_DURABILITY_NORMAL_REPAIR:		// ������ �Ϲ� ����
		RecvRepair(istr);
		break;
	case MSG_SUB_DURABILITY_SPECIAL_REPAIR:		// ������ Ư�� ����
		RecvRepairSpecial(istr);
		break;
	case MSG_SUB_DURABILITY_RECOVERY:			// ������ ����
		RecvRecovery(istr);
		break;
	}
}

void CDurability::RecvRepair( CNetworkMessage* istr )
{
#ifdef DURABILITY
	ResponseClient::repairDurability* pRecv = reinterpret_cast<ResponseClient::repairDurability*>(istr->GetBuffer());
	
	if (pRecv == NULL)
		return;

	CUIManager* pUiMgr = CUIManager::getSingleton();
	CUIDurability* pUIptr = pUiMgr->GetDurability();

	if (pUIptr == NULL)
		return;

	if (pRecv->result > ERR_NO_DURABILITY_OK)
	{
		if (pRecv->result == ERR_NO_DURABILITY_DO_NOT_REPAIR) // ������ �ʿ䰡 ����
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_DAMAGED_ITEM);
		else if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_MONEY) // ���� ����
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_MONEY);

		pUIptr->CheckBtnState(CUIDurability::eICON_ITEM);
	}
#endif	//	DURABILITY
}

void CDurability::RecvRepairSpecial( CNetworkMessage* istr )
{
#ifdef DURABILITY
	ResponseClient::specialRepairDurability* pRecv = reinterpret_cast<ResponseClient::specialRepairDurability*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	CUIManager* pUiMgr = CUIManager::getSingleton();
	CUIDurability* pUIptr = pUiMgr->GetDurability();

	if (pUIptr == NULL)
		return;

	if (pRecv->result > ERR_NO_DURABILITY_OK)
	{
		if (pRecv->result == ERR_NO_DURABILITY_DO_NOT_REPAIR) // ������ �ʿ䰡 ����
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_DAMAGED_ITEM);
		else if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_MONEY) // ���� ����
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_MONEY);
		else if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_ITEM_COUNT) // ���� ������ ������
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_ITEM);

		pUIptr->CheckBtnState(CUIDurability::eICON_ITEM);
	}
#endif	//	DURABILITY
}

void CDurability::RecvRecovery( CNetworkMessage* istr )
{
#ifdef	DURABILITY
	ResponseClient::recoveryDurability* pRecv = reinterpret_cast<ResponseClient::recoveryDurability*>(istr->GetBuffer());

	if (pRecv == NULL)
		return;

	CUIManager* pUiMgr = CUIManager::getSingleton();
	CUIDurability* pUIptr = pUiMgr->GetDurability();

	if (pUIptr == NULL)
		return;

	if (pRecv->result > ERR_NO_DURABILITY_OK)
	{
		if (pRecv->result == ERR_NO_DURABILITY_NOT_ENOUGH_ITEM_COUNT) // ��ġ ������ ������
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NOT_ENOUGH_ITEM);
		else if (pRecv->result == ERR_NO_DURABILITY_TOO_MUSH_ITEM_COUNT) // ��ġ ������ �ʹ� �����ϴ�.
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_OVER_RECOVER);
		else if (pRecv->result == ERR_NO_DURABILITY_DO_NOT_RECOVERY) // ������ �ʿ䰡 ����
			pUIptr->ShowMsgBox(CUIDurability::eMSG_ERROR_NO_RECOVER_ITEM);

		pUIptr->CheckBtnState(CUIDurability::eICON_ITEM);
	}
#endif	//	DURABILITY
}

void CDurability::SendRepair( UWORD tab_no, UWORD invenIdx, int virualIdx, LONGLONG fee, int nNpcIndex )
{
#ifdef DURABILITY
	CNetworkMessage nmList;
	RequestClient::repairDurability* packet = reinterpret_cast<RequestClient::repairDurability*>(nmList.nm_pubMessage);

	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_NORMAL_REPAIR;
	packet->tab_no = tab_no;
	packet->invenIndex = invenIdx;
	packet->virualIndex = virualIdx;
	packet->fee = fee;
	packet->npcIndex = nNpcIndex;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
#endif	//	DURABILITY
}

void CDurability::SendRepairSpecial( UWORD tab_item, UWORD invenIdx_item, int virIdx_item, UWORD tab_material, UWORD invenIdx_material, int virIdx_material, int nNpcIndex )
{
#ifdef DURABILITY
	CNetworkMessage nmList;
	RequestClient::specialRepairDurability* packet = reinterpret_cast<RequestClient::specialRepairDurability*>(nmList.nm_pubMessage);

	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_SPECIAL_REPAIR;
	packet->tab_no = tab_item;
	packet->invenIndex = invenIdx_item;
	packet->virualIndex = virIdx_item;

	packet->material_tab_no = tab_material;
	packet->material_invenIndex = invenIdx_material;
	packet->material_virtualIndex = virIdx_material;

	packet->npcIndex = nNpcIndex;
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
#endif	//	DURABILITY
}

void CDurability::SendRecovery( UWORD tab_item, UWORD invenIdx_item, int virIdx_item, UWORD tab_material, UWORD invenIdx_material, int virIdx_material, UWORD Count_material )
{
#ifdef DURABILITY
	CNetworkMessage nmList;
	RequestClient::recoveryDurability* packet = reinterpret_cast<RequestClient::recoveryDurability*>(nmList.nm_pubMessage);

	packet->type = MSG_DURABILITY;
	packet->subType = MSG_SUB_DURABILITY_RECOVERY;
	packet->tab_no = tab_item;
	packet->invenIndex = invenIdx_item;
	packet->virualIndex = virIdx_item;

	packet->material_tab_no = tab_material;
	packet->material_invenIndex = invenIdx_material;
	packet->material_virtualIndex = virIdx_material;
	packet->material_count = Count_material;

	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
#endif	//	DURABILITY
}

