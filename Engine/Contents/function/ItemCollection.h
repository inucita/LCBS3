#ifndef __ITEMCOLLECTION_H__
#define __ITEMCOLLECTION_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_item_collection.h>

class CItemCollection
{
public:
	CItemCollection();
	~CItemCollection();

	void RecvItemCollectionMsg(CNetworkMessage* istr);
	void SendGiveItem(int nCat, int nTheme, RequestClient::NEED_ITEM_DATA* pData);
	void SendAmendItem(int nCat, int nTheme);
private:
	//������ ��ü ����Ʈ ����(�α���)
	void UpdateDoingList(CNetworkMessage* istr);
	//�Ϸ�� ��ü ����Ʈ ����(�α���)
	void UpdateEndList(CNetworkMessage* istr);
	//������ �׸� ���� ������Ʈ
	void UpdateDoingData(CNetworkMessage* istr);
	//���� �Ϸ�
	void UpdateEndData(CNetworkMessage* istr);

	void ErrorMessage(CNetworkMessage* istr);
};
#endif // __ITEMCOLLECTION_H__