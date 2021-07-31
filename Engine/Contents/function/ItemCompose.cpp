#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "ItemCompose.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Contents/function/ItemComposeUI.h>
#include <Entities/ItemData.h>
#include <Engine/World/World.h>
#include <Common/Packet/ptype_old_do_item.h>
#include <Engine/Interface/UIAutoHelp.h>

std::vector<stComposeData*> CItemCompose::m_vecComposeData;
stComposeData* CItemCompose::_data;

CItemCompose::CItemCompose()
{
}

CItemCompose::~CItemCompose()
{
	SAFE_ARRAY_DELETE(_data);
	m_vecComposeData.clear();
}

void CItemCompose::RecvItemComposeMsg( CNetworkMessage* istr )
{
	ResponseClient::doItemCompose* pRecv = reinterpret_cast<ResponseClient::doItemCompose*>(istr->GetBuffer());

	CItemComposeUI* pUI = UIMGR()->GetItemCompose();

	if (pUI == NULL)
		return;

	int nItemIndex = -1;

	if (pRecv->errorCode == MSG_ITEM_COMPOSE_ERROR_OK) // ����
	{
		nItemIndex = pRecv->item_index;
	}
	else if(pRecv->errorCode == MSG_ITEM_COMPOSE_ERROR_NOT_RESULT) // ������ �����ϴ� �������� ����
		pUI->ShowErrorPopup(eCOMPOSE_POPUP_NO_COMPOSE_ITEM);
	else if(pRecv->errorCode == MSG_ITEM_COMPOSE_ERROR_NOT_ENOUGH_MAT) // ��� ����
		pUI->ShowErrorPopup(eCOMPOSE_POPUP_NOT_ENOUGH_MAT);
	else if(pRecv->errorCode == MSG_ITEM_COMPOSE_ERROR_NOT_ENOUGH_NAS) // ���� ����
		pUI->ShowErrorPopup(eCOMPOSE_POPUP_NOT_ENOUGH_NAS);
	else if(pRecv->errorCode == MSG_ITEM_COMPOSE_ERROR_FAIL) // �ռ� ����
		pUI->ShowErrorPopup(eCOMPOSE_POPUP_COMPOSE_FAIL);
	
	pUI->UpdateResultIcon(nItemIndex);
}

void CItemCompose::UpdateItemComposeMsg( CNetworkMessage* istr )
{
	UpdateClient::Artifact_SysMsg* pUpdate = reinterpret_cast<UpdateClient::Artifact_SysMsg*>(istr->GetBuffer());

	CTString strError = "";

	CTString strName = pUpdate->name;
	CTString strZoneName = CZoneInfo::getSingleton()->GetZoneName(pUpdate->zone_index);
	CTString strItemName = CItemData::getData(pUpdate->item_index)->GetName();

	CItemComposeUI* pUI = UIMGR()->GetItemCompose();

	switch(pUpdate->systype)
	{
	case ARTIFACT_SYSMSG_TAKE:
		strError.PrintF(_S(6400, "%s���� %s�������� %s�� ȹ���Ͽ����ϴ�."), strName, strZoneName, strItemName);
		break;
	case ARTIFACT_SYSMSG_COMPOSE:
		strError.PrintF(_S(6401, "%s���� %s�������� ���� �������� �ռ��Ͽ� %s�� ��������ϴ�."), strName, strZoneName, strItemName);
		break;
	case ARTIFACT_SYSMSG_DIE_TO_NPC:
		strError.PrintF(_S(6406, "%s���� ���Ϳ��� ����Ͽ� ���� ���� %s�� ��������ϴ�."), strName, strItemName);
		break;		
	case ARTIFACT_SYSMSG_LOGOUT:
		strError.PrintF(_S(6405, "%s���� ���ӿ��� ��Ż�Ͽ� ���� ���� %s��(��) ��������ϴ�."), strName, strItemName);
		break;	
	case ARTIFACT_SYSMSG_MOVE_ZONE:
		strError.PrintF(_S(6402, "%s���� %s�������� �̵��Ͽ����ϴ�."), strName, strZoneName);
		break;	
	case ARTIFACT_SYSMSG_EXPIRE_TIME:
		strError.PrintF(_S(6404, "%s���� �������� %s�� ���ð��� �� �Ǿ� ��������ϴ�."), strName, strItemName);
		break;	
	default:
		return;
	}

	_UIAutoHelp->SetGMNotice(strError);
	_pNetwork->ClientSystemMessage(strError, SYSMSG_ERROR);
}

void CItemCompose::UpdateGPSRelicMsg( CNetworkMessage* istr )
{
	UpdateClient::ArtifactFindUserCountMsg* pUpdate = reinterpret_cast<UpdateClient::ArtifactFindUserCountMsg*>(istr->GetBuffer());

	CTString strError = "";

	if (pUpdate->count <= 0)
		strError.PrintF(_S(6409, "���� �������� ���� ������ �����ڰ� �����ϴ�."));
	else
		strError.PrintF(_S(6408, "%d���� ���� ������ �����ڸ� �߰� �߽��ϴ�."), pUpdate->count);

	_pNetwork->ClientSystemMessage(strError, SYSMSG_ERROR);
}

void CItemCompose::SendComposeReq(int nMatIdx, stSendComposeInfo materialInfo)
{
	CNetworkMessage nmList;
	RequestClient::doItemCompose* packet = reinterpret_cast<RequestClient::doItemCompose*>(nmList.nm_pubMessage);

	packet->type = MSG_ITEM;
	packet->subType = MSG_ITEM_COMPOSE;

	packet->matItem_Index = nMatIdx;

	for (int i = 0; i < MAX_COMPOSE_SLOT_COUNT; ++i)
	{
		packet->matInfo[i].tab = materialInfo.tab[i];
		packet->matInfo[i].inven_index = materialInfo.invenIdx[i];
		packet->matInfo[i].virtual_index = materialInfo.virIdx[i];
	}
		
	nmList.setSize(sizeof(*packet));
	_pNetwork->SendToServerNew(nmList);
}

bool CItemCompose::LoadStaticComposeDataFromFile( const char* fileName )
{
	FILE *fp		= NULL;
	if ((fp = fopen(fileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "ItemCompose.bin File is not Exist.", "error!", MB_OK);
		return false;
	}

	fflush(fp);

	int	i;
	int count = 0;

	fread(&count, sizeof(int), 1, fp);

	if (count <= 0)
	{
		fclose(fp);
		return false;
	}

	_data = new stComposeData[count];
	fread(_data, sizeof(stComposeData) * count, 1, fp);
	fclose(fp);

	for( i = 0; i < count; ++i )
	{
		m_vecComposeData.push_back(&_data[i]);
	}	
	return true;
}

int CItemCompose::GetComposeMatCount( int nMatIdx )
{
	if (m_vecComposeData.size() <= 0)
		return 0;

	vecComposIter it = m_vecComposeData.begin();
	vecComposIter it_end = m_vecComposeData.end();

	for (; it != it_end; ++it)
	{
		if ( (*it)->nMaterialItem == nMatIdx)
			return (*it)->nMaterialCount;
	}

	return 0;
}

int CItemCompose::GetComposeResultItem( int nMatIdx )
{
	if (m_vecComposeData.size() <= 0)
		return 0;

	vecComposIter it = m_vecComposeData.begin();
	vecComposIter it_end = m_vecComposeData.end();

	for (; it != it_end; ++it)
	{
		if ( (*it)->nMaterialItem == nMatIdx)
			return (*it)->nResultItem;
	}

	return 0;
}

LONGLONG CItemCompose::GetComposeNas( int nMatIdx )
{
	if (m_vecComposeData.size() <= 0)
		return 0;

	vecComposIter it = m_vecComposeData.begin();
	vecComposIter it_end = m_vecComposeData.end();

	for (; it != it_end; ++it)
	{
		if ( (*it)->nMaterialItem == nMatIdx)
			return (*it)->GetComposeNas();
	}

	return 0;
}

