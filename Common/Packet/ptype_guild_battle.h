#ifndef __PTYPE_GUILD_BATTLE_H__
#define __PTYPE_GUILD_BATTLE_H__

#include "ptype_base.h"
#ifndef _CLIENT_
#include "ptype_server_to_server.h"
#endif

//subtype
enum
{
	MSG_SUB_GUILD_BATTLE_REGIST,					//������� �ι�° ��Ϲ�ư
	MSG_SUB_GUILD_BATTLE_CHALLENGE,					//������� �ι�° ������ư
	MSG_SUB_GUILD_BATTLE_CANCEL,					//������� ��ҹ�ư
	MSG_SUB_GUILD_BATTLE_LIST,						//������� ����Ʈ
	MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE_UPDATE,	//���� ��������
	MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE,	
	MSG_SUB_GUILD_BATTLE_ERROR,				
};

//error_code
enum
{
	//GuildBattleErrorcode
	GUILD_BATTLE_ERROR_AREADY,					//�̹� ����Ǿ��ų� ��ҵ� ����Դϴ�.
	GUILD_BATTLE_ERROR_CONDITION_GUILD_MASTER,	// [1086]����常 ��������� ��û �� �� �ֽ��ϴ�.
	GUILD_BATTLE_ERROR_CONDITION,				//������� ç������ ����� �� ���� �����Դϴ�.
	GUILD_BATTLE_SUCCESS_REGIST,				//��尡 ������� ç������ ��ϵǾ����ϴ�.
	GUILD_BATTLE_SUCCESS_CANCEL,				//����� ������� ç������ ����� ��ҵǾ����ϴ�.
	GUILD_BATTLE_FAIL_CHALLENGE,				//������� ������ �� �� ���� �����Դϴ�.

	//GuildBattleChallenge
	GUILD_BATTLE_SUCCESS_CHALLENGE,				//��������� ��û�Ͽ����ϴ�.
	
	//GuildBattleChallengeAgree
	GUILD_BATTLE_SUCCESS_AGREE,					//������� ��û�� �޾Ƶ�Ƚ��ϴ�. ��� ��������� ���۵˴ϴ�.
	GUILD_BATTLE_SUCCESS_REFUSE,				//������� ��û�� �ź��Ͽ����ϴ�.
};

#pragma pack(push, 1)
namespace RequestClient
{
	//�˾�â ����ϱ� ��ư
	struct GuildBattleRegist : public pTypeBase
	{
		LONGLONG stake_nas;
		int stake_gp;
		int guild_battle_time; //min
		int zone_index;
	};

	struct GuildBattleChallenge : public pTypeBase
	{
		int guild_index;
	};

	struct GuildBattleRegCancel : public pTypeBase
	{

	};

	struct GuildBattleList : public pTypeBase
	{
		int guild_battle_time;		//0 : ��ü
		int page_num;
	};

	struct GuildBattleChallengeAgree : public pTypeBase
	{
		int char_index;		//��û�� ĳ���� �ε���
		bool isAgree;		//true : yes, false : no
	};
}

namespace ResponseClient
{
	struct GuildBattleData
	{
		int guild_index;
		char guild_name[MAX_GUILD_NAME_LENGTH]; //MAX_GUILD_NAME_LENGTH = 50Byte
		char guild_master_name[MAX_CHAR_NAME_LENGTH]; //MAX_CHAR_NAME_LENGTH = 50Byte
		int guild_member_count;
		int guild_member_level_aver;
		int guild_win_count;
		int guild_battle_count;
		LONGLONG stake_nas;
		int stake_gp;
		int guild_battle_time;	//min
		int zone_index;
	};
	
	//�̺κ� Ŭ���̾�Ʈ���� ���� �ʿ� ���� �������ε� �ϳ� �ϴ� �־�����ϴ�. Ȯ�� �� �ʿ� �����ø� �����ּ���
	struct GuildBattleChallenge : public pTypeBase
	{
		char guild_name[MAX_GUILD_NAME_LENGTH]; //MAX_GUILD_NAME_LENGTH = 50Byte
		int error_code;
	};
	//

	struct GuildBattleList : public pTypeBase
	{
		int count;
		bool isNext;			//���������� ���� ����
		bool isReg;				//������� ��Ͽ���
		GuildBattleData data[0];
	};
	
	struct GuildBattleErrorcode : public pTypeBase
	{
		int error_code;
	};

#ifndef _CLIENT_
	struct GuildBattleCheckNasGp : public pTypeBase
	{
		int char_index;
		LONGLONG guild_nas;
		int guild_point;
	};
#endif

	struct GuildBattleChallengeAgree : public pTypeBase
	{
		char guild_name[MAX_GUILD_NAME_LENGTH];
		int zone_index;		//error_code ������ zone_index -1 �� ���� (��� �ʿ�X)
		int error_code;
	};
}

namespace UpdateClient
{
	struct GuildBattleChallengeAgreeUp : public pTypeBase
	{
		char guild_name[MAX_GUILD_NAME_LENGTH];
		int guild_level;
		int guild_member_count;
		int guile_member_level_aver;
		int guild_online_member_count;
		int guile_online_member_level_aver;
		int guild_win_count;
		int guild_battle_count;
		LONGLONG guild_nas;
		int guild_point;
		int char_index;		//��û�� ĳ���� �ε���
	};
}

#ifndef _CLIENT_

inline void makeGuildBattleErrorMsg(CNetMsg::SP& rmsg, int error_code)
{
	ResponseClient::GuildBattleErrorcode* p = reinterpret_cast<ResponseClient::GuildBattleErrorcode*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_ERROR;
	p->error_code = error_code;
	rmsg->setSize(sizeof(ResponseClient::GuildBattleErrorcode));
}

inline void makeGuildBattleChallengeAgreeMsgUpdate(CNetMsg::SP& rmsg, int level, int member_count, int level_aver, int win_count, int battle_count, LONGLONG nas, int guild_point, int char_index, const char* name)
{
	UpdateClient::GuildBattleChallengeAgreeUp* p = reinterpret_cast<UpdateClient::GuildBattleChallengeAgreeUp*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE_UPDATE;
	p->guild_level = level;
	strncpy(p->guild_name, name, MAX_GUILD_NAME_LENGTH);
	p->guild_member_count = member_count;
	p->guile_member_level_aver = level_aver;
	p->guild_win_count = win_count;
	p->guild_battle_count = battle_count;
	p->guild_nas = nas;
	p->guild_point = guild_point;
	p->char_index = char_index;
	rmsg->setSize(sizeof(UpdateClient::GuildBattleChallengeAgreeUp));
}

inline void MakeGuildBattleChallengeAgreeMsgRes(CNetMsg::SP& rmsg, const char* guild_name, int error_code, int zone_index)
{
	ResponseClient::GuildBattleChallengeAgree* p = reinterpret_cast<ResponseClient::GuildBattleChallengeAgree*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE_AGREE;
	strncpy(p->guild_name, guild_name, MAX_GUILD_NAME_LENGTH);
	p->error_code = error_code;
	rmsg->setSize(sizeof(ResponseClient::GuildBattleChallengeAgree));
}

inline void MakeGuildBattleChallengeMsg(CNetMsg::SP& rmsg, const char* guild_name, int error_code)
{
	ResponseClient::GuildBattleChallenge* p = reinterpret_cast<ResponseClient::GuildBattleChallenge*>(rmsg->m_buf);
	p->type = MSG_GUILD_BATTLE;
	p->subType = MSG_SUB_GUILD_BATTLE_CHALLENGE;
	p->error_code = error_code;
	strncpy(p->guild_name, guild_name, MAX_GUILD_NAME_LENGTH);
	rmsg->setSize(sizeof(ResponseClient::GuildBattleChallenge));
}





#endif //_CLIENT_

#pragma pack(pop)
#endif