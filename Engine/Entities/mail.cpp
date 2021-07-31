#include <stdh.h>

#include <Engine/Entities/Mail.h>
#include <Engine/Network/NetworkMessage.h>

//UIManger�� �޽��� ó���� �̴�. 
#include <Engine/Interface/UIManager.h>

CRecvItemDataArray::CRecvItemDataArray()
{
	m_MailIndex = 0;
	reserve(ITEMBUTTONMAX);
}

CRecvItemDataArray::~CRecvItemDataArray()
{
	clear();
}

INDEX	CRecvItemDataArray::GetValueCount(void)
{
	INDEX iCount = 0;

	RecvItemDataArray::iterator Itr;
	for (Itr = begin(); Itr!=end(); ++Itr)
	{
		if ((*Itr).itemIndex < 0)
			continue;
		
		++iCount;
	}

	return iCount;
}

CMailDataList::CMailDataList()
{

}

CMailDataList::~CMailDataList()
{

}

/*---------------------------------------------------------------------------------------------------------------------------*/
// ReceiveItemData ����

ReceiveItemData::ReceiveItemData(void)
{
	init();
}
ReceiveItemData::~ReceiveItemData(void)
{

}
void ReceiveItemData::init()
{
	UniqueIndex = -1;
	itemIndex = -1;
	option1 = -1;
	optionLevel1 = 0;
	option2 = -1;
	optionLevel2 = 0;
	option3 = -1;
	optionLevel3 = 0;
	option4 = -1;
	optionLevel4 = 0;
	option5 = -1;
	optionLevel5 = 0;
	limitTime1 = 0;
	limitTime2 = 0;
	plus = 0;
	flag = 0;
	itemCount = 0;

	for( int i=0; i<MAX_ITEM_OPTION; ++i )
	{
		originOptionVar[i] = ORIGIN_VAR_DEFAULT;
	}
}

// ReceiveItemData ��

/*---------------------------------------------------------------------------------------------------------------------------*/
// CMail ����

CMail* CMail::inst = NULL;

/* *********************************************************************************
	CMail ������
*********************************************************************************  */
CMail::CMail(void)
{
	m_Status = STATUS::done;
}

/* *********************************************************************************
	CMail ���� ������
*********************************************************************************  */
CMail::CMail(const CMail& other)
{

}

/* *********************************************************************************
	CMail �Ҹ���
*********************************************************************************  */
CMail::~CMail()
{

}

STATUS::STATUS	CMail::GetStatus()
{
	return m_Status;
}

void			CMail::SetSTatus(STATUS::STATUS messagetype)
{
	m_Status = messagetype;
}

void CMail::InitSendMailItem()
{
	for (int i=0; i<ITEMBUTTONMAX; ++i)
	{
		//m_AttachItemList[i].init();
		m_SendAttachItemList[i].init();
	}
}

MAILELEMENT& CMail::GetMailElement(void)
{
	return GetMailElement(m_AttachItemList.GetMailIndex());
}

MAILELEMENT& CMail::GetMailElement(INDEX iIndex)
{
	int iElement;
	for (iElement = 0; iElement < m_MailDatas.size(); iElement++)
	{
		if (m_MailDatas[iElement].serverIndex == iIndex)
			break;
	}

	return m_MailDatas[iElement];
}

MAILELEMENT& CMail::GetMailElementNCount(INDEX iIndex)
{
	return m_MailDatas[iIndex];
}

/* *********************************************************************************
	RecvMailData()
*********************************************************************************  */
INDEX CMail::RecvMailData(CNetworkMessage *istr)
{
	INDEX errnum;
	(*istr) >> errnum;

	if(errnum != 0)
		return errnum;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->GetMail()->OpenMailListReq(0, 0, 0, FALSE);

	SWORD			mailType;
	INDEX			size, index, limittime;
	INDEX			iconindex = -1;
	CTString		strTitle, senderName;
	// ÷�� ������ Ÿ�� 1 : ������ ������. 2 : ������ ����. 3. �ݼ۵� ���� UI�󿡼� �������� ���� ó��
	(*istr) >> size;

  	for(int i = 0; i < size;++i)
   	{
		MAILELEMENT mailelement;
		(*istr) >> index;
		(*istr) >> strTitle;
		(*istr) >> senderName;
		(*istr) >> limittime;
		(*istr) >> mailType;
		(*istr) >> iconindex;

		mailelement.serverIndex = index;
		mailelement.senderName = senderName;
		mailelement.title = strTitle;
		mailelement.time = limittime;
		mailelement.iconIndex = iconindex;
		mailelement.mailType = mailType;

		m_MailDatas.push_back(mailelement);
   	}

	return 0;
}

void CMail::DeleteMail(INDEX iMailIndex)
{
	if (m_MailDatas.empty())
		return;

	MAILLIST::iterator Itr = m_MailDatas.begin();

	for (; Itr != m_MailDatas.end(); ++Itr)
	{
		MAILELEMENT compareMail = (*Itr);
		if (compareMail.serverIndex == iMailIndex)
		{
			m_MailDatas.erase(Itr);
			break;
		}
	}
}

void CMail::SetUpdateMail(INDEX iTarget, CTString strContent, SQUAD llMoney)
{
	if (m_MailDatas.empty())
		return;

	m_AttachItemList.SetMailIndex(iTarget);

	MAILLIST::iterator Itr = m_MailDatas.begin();

	for (; Itr != m_MailDatas.end(); ++Itr)
	{
		if ((*Itr).serverIndex == iTarget)
		{
			(*Itr).text = strContent;
			(*Itr).money = llMoney;
			SetSTatus(STATUS::post_system_mail_read);
			break;
		}
	}
}

void CMail::SetUpdateMailMoney(INDEX iTarget, SQUAD llMoney)
{
	if (m_MailDatas.empty())
		return;

	MAILLIST::iterator Itr = m_MailDatas.begin();

	for (; Itr != m_MailDatas.end(); ++Itr)
	{
		if ((*Itr).serverIndex == iTarget)
		{
			(*Itr).money = llMoney;
			break;
		}
	}
}

void CMail::SetInitMailItems(INDEX iSlot, INDEX iTarget)
{
	if (m_MailDatas.empty())
		return;

	if (iSlot == 10) // ����
	{
		MAILLIST::iterator Itr = m_MailDatas.begin();

		for (; Itr != m_MailDatas.end(); ++Itr)
		{
			if ((*Itr).serverIndex == iTarget)
			{
				(*Itr).money = 0;
				break;
			}
		}
	}
	else
	{
		m_AttachItemList[iSlot].init();
	}
}

void CMail::InitMailData()
{
	m_MailDatas.clear();
}

ReceiveItemData&	CMail::GetRecvItemData(INDEX iIndex)
{
	return m_AttachItemList[iIndex];
}

ReceiveItemData&	CMail::GetSendItemData(INDEX iIndex)
{
	return m_SendAttachItemList[iIndex];
}

// ���Ŀ� �����ͷ��� �� ���� �� �� �����Ƿ�, ���ҷ� ���� �� �ֵ��� ������ �ʿ� �� �� �ִ�.
void CMail::RecvAttachItem(CNetworkMessage *istr)
{
	INDEX		count;
	UBYTE		ubMailIndex;
	INDEX		itemIndex;
	SBYTE		option1;
	SBYTE		optionLevel1;
	SBYTE		option2;
	SBYTE		optionLevel2;
	SBYTE		option3;
	SBYTE		optionLevel3;
	SBYTE		option4;
	SBYTE		optionLevel4;
	SBYTE		option5;
	SBYTE		optionLevel5;
	CTString	socket;
	INDEX		limitTime1;
	INDEX		limitTime2;
	INDEX		plus;
	INDEX		flag;
	SQUAD		itemCount;
	LONG		lOriginOptionVar =ORIGIN_VAR_DEFAULT;
	(*istr) >> count;
	//ũ�� �ȵȴ�.
	if(count > ITEMBUTTONMAX)
		return;

	(*istr) >> ubMailIndex; // �ش� ���� Index

	ReceiveItemData itemdata;

	m_AttachItemList.clear();
	m_AttachItemList.SetMailIndex(ubMailIndex);

	for(int i = 0;i < count; ++i)
	{
		(*istr) >> itemIndex;
		(*istr) >> option1;
		(*istr) >> optionLevel1;
		(*istr) >> option2;
		(*istr) >> optionLevel2;
		(*istr) >> option3;
		(*istr) >> optionLevel3;
		(*istr) >> option4;
		(*istr) >> optionLevel4;
		(*istr) >> option5;
		(*istr) >> optionLevel5;
		//(*istr) >> socket;
		(*istr) >> limitTime1;
		(*istr) >> limitTime2;
		(*istr) >> plus;
		(*istr) >> flag;
		(*istr) >> itemCount;
#ifdef REFORM_SYSTEM
		for( int j=0; j<MAX_ITEM_OPTION; ++j )
		{
			if ( _pNetwork->GetItemData(itemIndex).GetFlag() & ITEM_FLAG_ORIGIN )
			{
				(*istr) >> lOriginOptionVar;
				itemdata.originOptionVar[j] = lOriginOptionVar;
			}
		}
#endif
		itemdata.itemIndex = itemIndex;
		itemdata.option1 = option1; 
		itemdata.optionLevel1 = optionLevel1;
		itemdata.option2 = option2;
		itemdata.optionLevel2 = optionLevel2;
 		itemdata.option3 = option3;
		itemdata.optionLevel3 = optionLevel3;
		itemdata.option4 = option4;
		itemdata.optionLevel4 = optionLevel4;
		itemdata.option5 = option5;
		itemdata.optionLevel5 = optionLevel5;
		itemdata.limitTime1 = limitTime1;
		itemdata.limitTime2 = limitTime2;
		itemdata.plus = plus;
		itemdata.flag = flag;
		itemdata.itemCount = itemCount;

		m_AttachItemList.push_back(itemdata);
	}
}

// CMail ��
/*---------------------------------------------------------------------------------------------------------------------------*/
namespace PostSystem
{
	void DispatchMessage(CNetworkMessage *istr)
	{
		switch(istr->GetType())
		{
		case MSG_USERSERVER_AUTH:
			{

			}
			break;
		case MSG_USERSERVER_POST_SYSTEM:
			{
				RecvPostMessage(istr);
			}
			break;
		}
	}

	void RecvPostMessage(CNetworkMessage *istr)
	{
		INDEX	lType;
		(*istr) >> lType;
		//�޽��� �ڽ��� �⺻ ����
		CUIMsgBox_Info	MsgBoxInfo;
		INDEX			iconindex = -1;
		CTString		strTitle,senderName;
		// ÷�� ������ Ÿ�� 1 : ������ ������. 2 : ������ ����. 3. �ݼ۵� ���� UI�󿡼� �������� ���� ó��
		CUIManager* pUIManager = CUIManager::getSingleton();
		switch(lType)
		{
		case MSG_USERSERVER_POST_SYSTEM_MAIL_LIST_REP:
			{
				INDEX iError = CMail::getInstance()->RecvMailData(istr);

				if (iError < 0)
				{
					ReceiveErrMessage(iError, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_LIST_REP"));
				}
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_SEND_REP:
			{
				INDEX errorNum;
				(*istr) >> errorNum;
				//0�̸� ���� �Ϸ�
				if(errorNum == 0)
				{
					CMail::getInstance()->InitSendMailItem();
					CMail::getInstance()->SetSTatus(STATUS::post_system_mail_send_ok);
				}
				else
				{
					ReceiveErrMessage(errorNum, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_SEND_REP"));
					CMail::getInstance()->SetSTatus(STATUS::post_system_mail_send_err);
				}
			}
			
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_READ_REP:
			{
				UBYTE ubMailIndex;
				// ������ ���� ����
				CTString text;
				// ��
				SQUAD	money;
				(*istr) >> ubMailIndex;
				(*istr) >> text;
				(*istr) >> money;

				//���⼭ �޾Ƽ� ����Ʈ�� ���� �����Ѵ�.
				CMail::getInstance()->SetUpdateMail(ubMailIndex, text, money);
				for(int lol = 0 ;lol < ITEMBUTTONMAX;++lol)
				{
					CMail::getInstance()->GetRecvItemData(lol).init();
				}
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_DEL_REP:
			{
				UBYTE ubMailIndex;
				INDEX deleteok;
				(*istr) >> ubMailIndex;
				(*istr) >> deleteok;
				//���⼭ �޾Ƽ� ����Ʈ���� ����ó��
				if (deleteok == 0)
				{
					CMail::getInstance()->DeleteMail(ubMailIndex);
				}
				else if(deleteok < 0)
				{
					ReceiveErrMessage(deleteok, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_DEL_REP"));
				}
				CMail::getInstance()->SetSTatus(STATUS::post_system_mail_del);
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_ACK:
			{
				CMail::getInstance()->RecvAttachItem(istr);
			}
			break;
		case MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_REP:
			{
				UBYTE ubMailIndex;
				INDEX errorNum;
				INDEX slot;

				(*istr) >> ubMailIndex;
				(*istr) >> errorNum;
				(*istr) >> slot;

				if(errorNum == 0)
				{
					CMail::getInstance()->SetInitMailItems(slot, ubMailIndex);
				}
				else if(errorNum < 0)
				{
					ReceiveErrMessage(errorNum, CTString("MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_REP"));
				}
				CMail::getInstance()->SetSTatus(STATUS::post_system_mail_itemrecv);
			}
			break;
		default:
			break;
		}
	}


	void ReceiveErrMessage( int code, CTString message )
	{
		if(code >= 0)
		{
			return;
		}
		else if(code == -700 || code == -704 || code == -770)
		{
			_pNetwork->ClientSystemMessage(_S( 5675, "������ ���� �����ϴ�. ��� �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -702)
		{
			_pNetwork->ClientSystemMessage(_S( 5676, "�����Կ� �� ������ �����ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -703 || code == -705 || code == -708 || code == -709 || code == -773)
		{
			_pNetwork->ClientSystemMessage(_S( 5677, "�޽��� ���ۿ� ���� �Ͽ����ϴ�. ��� �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -704)
		{
			_pNetwork->ClientSystemMessage(_S( 5686, "÷�ε� �������� ã�� �� �����ϴ�. ��� �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -706 || code == -707)
		{
			_pNetwork->ClientSystemMessage(_S( 5678, "�������̳� ���� �����ֽ��ϴ�. ������ Message�� �����Ͻðڽ��ϱ�?"), SYSMSG_ERROR);
		}
		else if(code == -701 )
		{
			_pNetwork->ClientSystemMessage(_S( 5679, " �ݼ۵� ������ �о� �� �� �����ϴ�. ��� �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -720)
		{
			_pNetwork->ClientSystemMessage(_S( 5680, "�ݾ��� �����Ͽ� ������ ���� �� �����ϴ�."), SYSMSG_ERROR);
		}
 		else if(code == -721)
 		{
 			_pNetwork->ClientSystemMessage(_S( 5693, "�Էµ� ���� �����δ� ���� �� �����ϴ�. Ȯ�� �� �̿� ��Ź�մϴ�."), SYSMSG_ERROR);
 		}
		else if(code == -722)
 		{
 			_pNetwork->ClientSystemMessage(_S( 5696, "�ڽſ��Դ� ������ ���� �� �����ϴ�. Ȯ�� �� �ٽ� �Է��� �ּ���."), SYSMSG_ERROR);
 		}
		else if(code == -741)
 		{
 			_pNetwork->ClientSystemMessage(_S( 5698, "������ ����  ���̻� �޼����� ���� �� ���� �����Դϴ�."), SYSMSG_ERROR);
 		}
		
		else if(code == -750 || code == -751 || code == -752 || code == -753 || code == -754)
		{
			_pNetwork->ClientSystemMessage(_S( 5681, "�ش� �ɸ��ʹ� �������� �ʽ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -755)
		{
			_pNetwork->ClientSystemMessage(_S( 5682, "�ѹ��� �������� ���� �ɸ��ʹ� ���� ������ �Ұ��մϴ�."), SYSMSG_ERROR);
		}
		else if(code == -771)
		{
			_pNetwork->ClientSystemMessage(_S( 5683, "������ �κ��丮�� ���� �� �����ϴ�. ��� �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -772 || code == -774)
		{
			_pNetwork->ClientSystemMessage(_S( 5684, "������ ���� �� �� �����ϴ�. ��� �� �ٽ� �õ��Ͻñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else if(code == -796)
		{
			_pNetwork->ClientSystemMessage(_S( 4237, "�κ��丮�� �����ϰų� �Ѱ� ���Ը� �ʰ��Ͽ����ϴ�. �κ��丮�� ��� �� �ٽ� �õ����ֽñ� �ٶ��ϴ�."), SYSMSG_ERROR);
		}
		else
		{
			if(_pNetwork->m_ubGMLevel > 1)
			{
				CUIMsgBox_Info	MsgBoxInfo;
				CTString		strMessage;
				MsgBoxInfo.SetMsgBoxInfo( message, UMBS_OK, UI_NONE, MSGCMD_NULL );
				strMessage.PrintF( "err code is %d.", code);
				MsgBoxInfo.AddString( strMessage );
				CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
			}
		}
	}
/* *********************************************************************************
	MyMailListRequest()
	: ���� ��� ��û(����Ʈ ��û)
*********************************************************************************  */
	void MyMailListRequest(LONG uid)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_LIST_REQ;
		nmPostsys << (INDEX)uid;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	ReadtoMailRequest()
	: ���� �б� ��û
*********************************************************************************  */
	void ReadtoMailRequest(int sequences)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_READ_REQ;
		nmPostsys << (INDEX)sequences;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	DeletetoMailRequest()
	: ���� ���� ��û
*********************************************************************************  */
	void DeletetoMailRequest(int sequences)
	{
		if (sequences < 0)
			return;

		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_DEL_REQ;
		nmPostsys << (INDEX)sequences;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	GetItemsfromMailRequest()
	: ÷�ε� ������ �������� ��û
*********************************************************************************  */
	void GetItemsfromMailRequest(int mailSlotint, int slot, int itemIndex, SQUAD count, LONG uid, int type)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_ATTACH_REQ;
		nmPostsys << (INDEX)type;
		nmPostsys << (INDEX)uid;
		nmPostsys << (INDEX)mailSlotint;
		nmPostsys << (INDEX)slot;
		nmPostsys << (INDEX)itemIndex;
		nmPostsys << (SQUAD)count;
		FGC->AddSendBuffer(nmPostsys);
	}
/* *********************************************************************************
	SendMailMessage()
	: ���� ������ �޽���
*********************************************************************************  */
	void SendMailMessage(CTString reciver, CTString title, CTString contents, LONGLONG money, SWORD billReqFlag, SWORD type, INDEX iconIndex, INDEX itemcount, LONG npcuid)
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_SEND_REQ;
		nmPostsys << npcuid;
		nmPostsys << reciver;
		nmPostsys << title;
		nmPostsys << contents;
		nmPostsys << money;
		nmPostsys << billReqFlag;
		nmPostsys << type;
		nmPostsys << iconIndex;
		
		CUIManager* pUIManager = CUIManager::getSingleton();
		nmPostsys << (INDEX)itemcount;
		for(int i = 0; i < itemcount; ++i)
		{
			/*int uID = sendmailitem[i].uID;
			int count = sendmailitem[i].count;
			int slot = sendmailitem[i].slot;*/
			//Item info
			int uID = CMail::getInstance()->GetSendItemData(i).UniqueIndex;
			int count = CMail::getInstance()->GetSendItemData(i).itemCount;
			int slot = i;

			if (uID < 0)
				continue;
			
			nmPostsys << (INDEX)uID;
			nmPostsys << (SQUAD)count;
			nmPostsys << (INDEX)slot;
		}
		
		FGC->AddSendBuffer(nmPostsys);
		pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SENDMAIL );
	}
/* *********************************************************************************
	SendDisconnectedSubHelper()
	: ���� ���� ������ ���ڴٰ� �˷��ش�.
*********************************************************************************  */
	void SendDisconnectedSubHelper()
	{
		CNetworkMessage nmPostsys(MSG_USERSERVER_POST_SYSTEM);
		nmPostsys << (INDEX)MSG_USERSERVER_POST_SYSTEM_MAIL_DISCONNECT;

#ifdef XOR_ENCIPHERMENT
		Xor_enc(nmPostsys.nm_pubMessage, nmPostsys.nm_slSize);
		FGC->SendPacket(nmPostsys.nm_pubMessage, nmPostsys.nm_slSize);
#else
		FGC->MsgCryption(nmPostsys); // ������ ���� ����, SendBuffer�� ��ġ�� �ʰ� �ٷ� ������.
#endif // XOR_ENCIPHERMENT
		FGC->DisConnectSubHelper(); // ������ ������ ���´�.
	}
}


