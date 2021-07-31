#ifndef __MAIL_H__
#define __MAIL_H__

#include <Engine/Base/CTString.h>
#include <list>
#include <Engine/Interface/UIButtonEx.h>

#define ITEMBUTTONMAX 1		// ÷�� ������ ������ ������ ����

class ReceiveItemData;
typedef std::vector<ReceiveItemData> RecvItemDataArray;

class CRecvItemDataArray : public std::vector<ReceiveItemData>
{
public:
	CRecvItemDataArray();
	~CRecvItemDataArray();

protected:
	INDEX	m_MailIndex;	// ÷�� ������ ������ �ش� ���� Index

public:
	void	SetMailIndex(INDEX iIndex)
	{
		m_MailIndex = iIndex;
	}

	INDEX	GetMailIndex(void)
	{
		return m_MailIndex;
	}

	INDEX	GetValueCount(void);
};

struct MAILELEMENT
{
	CTString senderName;
	CTString title;
	CTString text;
	SQUAD	money;
	int		time;
	int		iconIndex;
	int		serverIndex;
	short	mailType;// ÷�� ������ Ÿ�� 1 : ������ ������. 2 : ������ ����. 3. �ݼ۵� ����
};

typedef std::vector<MAILELEMENT> MAILLIST;

namespace STATUS
{
	typedef enum uistatus
	{
		done,
		post_system_mail_list,		// ���� ����Ʈ ��û
		post_system_mail_send_ok,	// ���� ������ ��û ����
		post_system_mail_send_err,	// ���� ������ ��û ����
		post_system_mail_read,		// ���� �б� ��û
		post_system_mail_del,		// ���� ���� ��û
		post_system_mail_item,		// ���� ÷������ ����
		post_system_mail_itemrecv,	// ��÷ ������ �κ� �ֱ�
		end,
	}STATUS;
}

class ReceiveItemData
{
private:
public:
	ReceiveItemData();
	~ReceiveItemData();
	void init();
	int			UniqueIndex;	// server index
	int			itemIndex;	// dbindex
	char		option1;
	char		optionLevel1;
	char		option2;
	char		optionLevel2;
	char		option3;
	char		optionLevel3;
	char		option4;
	char		optionLevel4;
	char		option5;
	char		optionLevel5;
	int			limitTime1;
	int			limitTime2;
	int			plus;
	int			flag;
	__int64		itemCount;
	LONG		originOptionVar[MAX_ITEM_OPTION];
};

class  CMailDataList : public std::vector<MAILELEMENT>
{
private:

public:
	CMailDataList();
	~CMailDataList();

	
}; 

class ENGINE_API CMail
{
private:
	CMail();
	CMail(const CMail& other);
	~CMail();
	static CMail*		inst;
	CMailDataList		m_MailDatas;		// ���� ����Ʈ
	
	CRecvItemDataArray	m_AttachItemList; // �ϳ��� ����� �� (������ �ޱ� ��� OpenMail UI���� ���)
	CRecvItemDataArray	m_SendAttachItemList;
	
 	STATUS::STATUS		m_Status;

public:
	static CMail*	getInstance() 
	{
		if (inst == 0) inst = new CMail();
		return inst;
	}

	void				InitMailData();
	void				InitSendMailItem();

	void				SetSTatus(STATUS::STATUS messagetype);
  	STATUS::STATUS		GetStatus();

	void				SetUpdateMail(INDEX iTarget, CTString strContent, SQUAD llMoney);
	void				SetUpdateMailMoney(INDEX iTarget, SQUAD llMoney);
	void				SetInitMailItems(INDEX iSlot, INDEX iTarget);

	ReceiveItemData&	GetRecvItemData(INDEX iIndex);
	ReceiveItemData&	GetSendItemData(INDEX iIndex);
	MAILELEMENT&		GetMailElement(void);				// ���� �޴������� �ش� ���� �����͸� �����´�.
	MAILELEMENT&		GetMailElement(INDEX iINdex);
	MAILELEMENT&		GetMailElementNCount(INDEX iIndex); // ���ͳ����� n��° �ִ� �����͸� �����´�.
	CMailDataList&		GetMailDataList() { return m_MailDatas; }

	void				DeleteMail(INDEX iMailIndex);	// ������ ã�Ƽ� ����
	void				RecvAttachItem(CNetworkMessage *istr);
	INDEX				RecvMailData(CNetworkMessage *istr);
};

namespace PostSystem
{
	void	DispatchMessage(CNetworkMessage *istr);
	void	RecvPostMessage(CNetworkMessage *istr);
	void	ReceiveErrMessage( int code, CTString message );

	void	MyMailListRequest(LONG uid);
	void	ReadtoMailRequest(int sequences);
	void	DeletetoMailRequest(int sequences);
	void	GetItemsfromMailRequest(int mailSlotint, int slot, int itemIndex, SQUAD count, LONG uid, int type);
	void	SendMailMessage(CTString reciver, CTString title, CTString contents, LONGLONG money, SWORD billReqFlag, SWORD type, INDEX iconIndex, INDEX itemcount, LONG npcuid);
	void	SendDisconnectedSubHelper();
}

#endif //__MAIL_H__

