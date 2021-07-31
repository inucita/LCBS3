#ifndef __PTYPE_OLD_DO_TITLE_H__
#define __PTYPE_OLD_DO_TITLE_H__

#include "ptype_base.h"

#define CUSTOM_TITLE_NAME_LENGTH	31

enum
{
	MSG_EX_TITLE_SYSTEM_LIST_REQ,					// ȣĪ ����Ʈ ��û Client -> Server
	MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ,			// Ÿ��Ʋ ���� ��û Client -> Server : titleindex(n)
	MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ,			// Ÿ��Ʋ ��� ��û Client -> Server : titleindex(n)
	MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ,			// Ÿ��Ʋ �������� ��û Client -> Server : titleindex(n)

	MSG_EX_TITLE_SYSTEM_LIST_REP_NO_LIST,			// ȣĪ ����Ʈ ���� Server -> Client
	MSG_EX_TITLE_SYSTEM_LIST_REP_EXIST_LIST,		// ȣĪ ����Ʈ ���� Server -> Client :  : nCount(n), titleindex(n), titleindex(n), ...., endtime(n)
	MSG_EX_TITLE_SYSTEM_ALREADY_HAVE_TITLE,			// ������ ��� �� �̹� ������ Ÿ��Ʋ��. Server -> Client : titleindex(n)
	MSG_EX_TITLE_SYSTEM_INSERT_SUCCESS,				// ������ ��� �� Title List�� �Է� ������. Server -> Client : titleindex(n)
	
	MSG_EX_TITLE_SYSTEM_NOT_HAVE_TITLE,				// Ÿ��Ʋ ���� ��û�Ͽ����� Server���� ����� ����. Server -> Client : titleindex(n)
	MSG_EX_TITLE_SYSTEM_TITLE_SELECT_SUCCESS,		// Ÿ��Ʋ ���� ���� Server -> Client : titleindex(n)
	
	MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_SUCCESS,		// Ÿ��Ʋ ��� ���� Server -> Client : titleindex(n)
	
	MSG_EX_TITLE_SYSTEM_TITLE_DELETE_SUCCESS,		// Ÿ��Ʋ �������� ���� Server -> Client : titleindex(n)
	MSG_EX_TITLE_SYSTEM_TITLE_EXPIRED,				// �Ӹ��� �ް� �ִ� Ÿ��Ʋ�� �Ⱓ���� üũ �Ǿ��� �� Server -> Client : titleindex(n)
	MSG_EX_TITLE_SYSTEM_TITLE_CHANGE_TIME,			// // [selo: 101105] ���� �ð� ���� ���� Server -> Client : titleindex(n), endtime(n)

	MSG_EX_TITLE_SYSTEM_TITLE_MAKE,					// Ÿ��Ʋ ����
	MSG_EX_TITLE_SYSTEM_TITLE_MAKE_INFO_LIST,		// ���� Ÿ��Ʋ ���� ����Ʈ
	MSG_EX_TITLE_SYSTEM_TITLE_USER_INFO,			// ���� Ŀ���� Ÿ��Ʋ �������
	MSG_EX_TITLE_SYSTEM_DELETE_INFO,				// ���� Ŀ���� Ÿ��Ʋ ����
};

#pragma pack(push, 1)

struct TitleSystemMakeInfo	//������ ���������� �̸��� �����ָ� �ȴٰ� ��
{
	int custom_title_index;
	char name[CUSTOM_TITLE_NAME_LENGTH];
	int color;
	int background;
	int effect;
	
	int option_index[5];
	int option_level[5];

	TitleSystemMakeInfo()
	{
		memset(name, 0x00, CUSTOM_TITLE_NAME_LENGTH);
		for(int i = 0; i < 5; i++)
		{
			option_index[i] = -1;
			option_level[i] = -1;
		}
	}
};

namespace RequestClient
{
	//MSG_EX_TITLE_SYSTEM_LIST_REQ
	struct TitleSystemList : public pTypeThirdBase
	{
	};

	//MSG_EX_TITLE_SYSTEM_TITLE_SELECT_REQ
	struct TitleSystemSelect : public pTypeThirdBase
	{
		int title_index;
	};

	//MSG_EX_TITLE_SYSTEM_TITLE_CANCEL_REQ
	struct TitleSystemCancel : public pTypeThirdBase
	{
		int title_index;
	};

	//MSG_EX_TITLE_SYSTEM_TITLE_DELETE_REQ
	struct TitleSystemDelete : public pTypeThirdBase
	{
		int title_index;
	};

 	struct TitleSystemMake : public pTypeThirdBase
	{
		int tab;
		int inven_index;
		int vIndex;
		char color;
		char background_color;
		char effect;
		char name[CUSTOM_TITLE_NAME_LENGTH];
	};
}

namespace ResponseClient
{
	struct TitleSystemData
	{
		int index;
		int end_time;
		int custom_title_index;
	};

	struct TitleSystemList : public pTypeThirdBase
	{
		int title_count;
		TitleSystemData data[0];
	};

	struct TitleSystem : public pTypeThirdBase
	{
		int title_index;
		int char_index;
		int custom_title_index;
	};

	struct TitleSystemCantUse : public pTypeThirdBase
	{
		int title_index;
	};

	struct TitleSystemForceChangeTime : public pTypeThirdBase
	{
		int type;
		int title_index;
		int end_time;
	};

	struct TitleSystemMake : public pTypeThirdBase
	{
		TitleSystemMakeInfo info;
	};
}

#ifndef _CLIENT_
inline void TitleSystemMakeInfoMsg(CNetMsg::SP& rmsg, int make_title_index, char* name, int color, int background_color, int effect, int* option_index, int* option_level)
{
	ResponseClient::TitleSystemMake* packet = reinterpret_cast<ResponseClient::TitleSystemMake*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_TITLE_MAKE;
	packet->info.custom_title_index = make_title_index;
	strncpy(packet->info.name, name, CUSTOM_TITLE_NAME_LENGTH);
	packet->info.color = color;
	packet->info.background = background_color;
	packet->info.effect = effect;

	for(int i = 0; i < 5; i ++)
	{
		packet->info.option_index[i] = option_index[i];
		packet->info.option_level[i] = option_level[i];
	}

	rmsg->setSize(sizeof(ResponseClient::TitleSystemMake));
}

inline void TitleSystemListMsg(CNetMsg::SP& rmsg, int thirdType, int title_count)
{
	ResponseClient::TitleSystemList* packet = reinterpret_cast<ResponseClient::TitleSystemList*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = thirdType;
	packet->title_count = title_count;

	rmsg->setSize(sizeof(ResponseClient::TitleSystemList));
}

inline void TitleSystemMsg(CNetMsg::SP& rmsg, int thirdType, int title_index, int char_index, int custom_title_index)
{
	ResponseClient::TitleSystem* packet = reinterpret_cast<ResponseClient::TitleSystem*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = thirdType;
	packet->title_index = title_index;
	packet->char_index = char_index;
	packet->custom_title_index = custom_title_index;

	rmsg->setSize(sizeof(ResponseClient::TitleSystem));
}

inline void TitleSystemCantUseMsg(CNetMsg::SP& rmsg, int title_index)
{
	ResponseClient::TitleSystemCantUse* packet = reinterpret_cast<ResponseClient::TitleSystemCantUse*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_ALREADY_HAVE_TITLE;
	packet->title_index = title_index;

	rmsg->setSize(sizeof(ResponseClient::TitleSystemCantUse));
}

inline void TitleSystemForceChangeTimeMsg(CNetMsg::SP& rmsg, int thirdType, int title_index, int end_time)
{
	ResponseClient::TitleSystemForceChangeTime* packet = reinterpret_cast<ResponseClient::TitleSystemForceChangeTime*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = thirdType;
	packet->title_index = title_index;
	packet->end_time = end_time;

	rmsg->setSize(sizeof(ResponseClient::TitleSystemForceChangeTime));
}
#endif

namespace UpdateClient
{
	struct TitleUse : public pTypeThirdBase
	{
		int title_index;
		int end_time;
		int custom_title_index;
	};

	struct TitleSystemMakeList : public pTypeThirdBase
	{
		int	count;
		TitleSystemMakeInfo info[0];
	};

	struct TitleSystemUserInfo : public pTypeThirdBase
	{
		int char_index;
		int color;
		int background;
		int effect;
		char name[CUSTOM_TITLE_NAME_LENGTH];
	};

	struct TitleSystemDeleteInfo : public pTypeThirdBase
	{
		int title_index;
	};
}

#ifndef _CLIENT_
inline void TitleUseMsg(CNetMsg::SP& rmsg, int title_index, int end_time, int custom_title_index)
{
	UpdateClient::TitleUse* packet = reinterpret_cast<UpdateClient::TitleUse*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_INSERT_SUCCESS;
	packet->title_index = title_index;
	packet->end_time = end_time;
	packet->custom_title_index = custom_title_index;

	rmsg->setSize(sizeof(UpdateClient::TitleUse));
}

inline void TitleUserInfoMsg(CNetMsg::SP& rmsg, int char_index, int color, int background, int effect, char* name)
{
	UpdateClient::TitleSystemUserInfo* packet = reinterpret_cast<UpdateClient::TitleSystemUserInfo*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_TITLE_USER_INFO;
	packet->char_index = char_index;
	packet->color = color;
	packet->background = background;
	packet->effect = effect;
	strncpy(packet->name, name, CUSTOM_TITLE_NAME_LENGTH);

	rmsg->setSize(sizeof(UpdateClient::TitleSystemUserInfo));
}

inline void TitleDeleteInfoMsg(CNetMsg::SP& rmsg, int title_index)
{
	UpdateClient::TitleSystemDeleteInfo* packet = reinterpret_cast<UpdateClient::TitleSystemDeleteInfo*>(rmsg->m_buf);
	packet->type = MSG_EXTEND;
	packet->subType = MSG_EX_TITLE_SYSTEM;
	packet->thirdType = MSG_EX_TITLE_SYSTEM_DELETE_INFO;
	packet->title_index = title_index;
	
	rmsg->setSize(sizeof(UpdateClient::TitleSystemDeleteInfo));
}

#endif

#pragma pack(pop)

#endif