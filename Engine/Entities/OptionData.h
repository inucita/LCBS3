#ifndef INCL_OPTIONDATA_H
#define INCL_OPTIONDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/LocalDefine.h>
#include <Engine/Help/LoadLod.h>
#include <common/header/def_option.h>
#include <common/header/def_title.h>
#include <vector>

#define		OPTION_STR_UP				0		// �� ���
#define		OPTION_DEX_UP				1		// ��ø ���
#define		OPTION_INT_UP				2		// ���� ���
#define		OPTION_CON_UP				3		// ü�� ���
#define		OPTION_HP_UP				4		// ����� ���
#define		OPTION_MP_UP				5		// ���� ���

#define		OPTION_DAMAGE_UP			6		// ���� ���ݷ� ���
#define		OPTION_MELEE_DAMAGE_UP		7		// ���� �ٰŸ� ���ݷ� ���
#define		OPTION_RANGE_DAMAGE_UP		8		// ���� ���Ÿ� ���ݷ� ���
#define		OPTION_MELEE_HIT_UP			9		// ���� ���� ���߷� ���
#define		OPTION_RANGE_HIT_UP			10		// ���� ���Ÿ� ���߷� ���

#define		OPTION_DEFENSE_UP			11		// ���� ���� ���
#define		OPTION_MELEE_DEFENSE_UP		12		// ���� �ٰŸ� ���� ���
#define		OPTION_RANGE_DEFENSE_UP		13		// ���� ���Ÿ� ���� ���
#define		OPTION_MELEE_AVOID_UP		14		// ���� ���� ȸ���� ���
#define		OPTION_RANGE_AVOID_UP		15		// ���� ���Ÿ� ȸ���� ���

#define		OPTION_MAGIC_UP				16		// ���� ���ݷ� ���
#define		OPTION_MAGIC_HIT_UP			17		// ���� ���߷� ���
#define		OPTION_RESIST_UP			18		// ���� ���� ���
#define		OPTION_RESIST_AVOID_UP		19		// ���� ȸ���� ���

#define		OPTION_ALL_DAMAGE_UP		20		// ��ü ���ݷ� ��� (��������, ���ݹ���, ����)
#define		OPTION_ALL_HIT_UP			21		// ��ü ���߷� ��� (��������, ���ݹ���, ����)

#define		OPTION_ALL_DEFENSE_UP		22		// ��ü ���� ��� (��������, ���ݹ���, ����)
#define		OPTION_ALL_AVOID_UP			23		// ��ü ȸ���� ��� (��������, ���ݹ���, ����)

#define		OPTION_NOT_USED_24			24		// ��� ����
#define		OPTION_NOT_USED_25			25		// ��� ����

#define		OPTION_ATTR_FIRE			26
#define		OPTION_ATTR_WATER			27
#define		OPTION_ATTR_WIND			28
#define		OPTION_ATTR_EARTH			29
#define		OPTION_ATTR_DARK			30
#define		OPTION_ATTR_LIGHT			31

// ���� ���� �����ۿ� �ɼ�
#define		OPTION_MIX_STR				36		// ��
#define		OPTION_MIX_DEX				37		// ��ø
#define		OPTION_MIX_INT				38		// ����
#define		OPTION_MIX_CON				39		// ü��

#define		OPTION_MIX_ATTACK			40		// ����
#define		OPTION_MIX_MAGIC			41		// ����
#define		OPTION_MIX_DEFENSE			42		// ����
#define		OPTION_MIX_RESIST			43		// ����

#define		OPTION_MIX_STURN			44		// ����
#define		OPTION_MIX_BLOOD			45		// ����
#define		OPTION_MIX_MOVE				46		// ����
#define		OPTION_MIX_POISON			47		// ������
#define		OPTION_MIX_SLOW				48		// ���ο�
// -- ���� ���� �����ۿ� �ɼ�

// 2006 �Ϲݱ� �ű� �ɼ�
#define		OPTION_DOWN_LIMITLEVEL		49		// �������� ���� �ٿ�
#define		OPTION_STEAL_MP				51		// ���� ���
#define		OPTION_STEAL_HP				52		// ����� ���
#define		OPTION_ATTACK_BLIND			53		// ���� ����
#define		OPTION_ATTACK_POISON		54		// �� ����
#define		OPTION_ATTACK_CRITICAL		55		// ũ��Ƽ�� Ȯ�� ����
#define		OPTION_RECOVER_HP			56		// HP ȸ���� ���
#define		OPTION_RECOVER_MP			57		// MP ȸ���� ���
#define		OPTION_DECREASE_SKILL_DELAY	58		// ��ų ��Ÿ�� ����
#define		OPTION_DECREASE_SKILL_MP	59		// MP �Ҹ� ����
#define		OPTION_RESIST_STONE			60		// ���� ���� ����
#define		OPTION_RESIST_STURN			61		// ���� ���� ����
#define		OPTION_RESIST_SILENT		62		// ħ�� ���� ����
#define		OPTION_BLOCKING				63		// ���� ����
#define		OPTION_MOVESPEED			64		// �̵� �ӵ� ���
// --- 2006 �Ϲݱ� �ű� �ɼ�
#define		OPTION_FLYSPEED				65		// ���� �ӵ� ���
#define     OPTION_ATTACK_DEADLY        66      // ���鸮 Ȯ�� ����

#define		OPTION_STR_UP_RATE			67
#define		OPTION_DEX_UP_RATE			68
#define		OPTION_INT_UP_RATE			69
#define		OPTION_CON_UP_RATE			70
#define		OPTION_HP_UP_RATE			71
#define		OPTION_MP_UP_RATE			72
#define		OPTION_WEAPON_UP_RATE		73
#define		OPTION_ARMOR_UP_RATE		74
#define		OPTION_MELEE_HIT_UP_RATE	75
#define		OPTION_MAGIC_HIT_UP_RATE	76
#define		OPTION_MELEE_AVOID_RATE		77
#define		OPTION_MAGIC_AVOID_RATE		78
#define		OPTION_RECOVER_HP_RATE		79
#define		OPTION_RECOVER_MP_RATE		80
#define		OPTION_TEST_QUEST			81
#define		OPTION_EXP_UP_RATE			82
#define		OPTION_SP_UP_RATE			83
#define		OPTION_APET_TRANS_END		84		// ���� ���� �ð�
#define		OPTION_APET_OWNER_HPUP		85
#define		OPTION_APET_OWNER_ATTUP		86
#define		OPTION_ATT_FIRE_UP			87		// ȭ���Ӽ� ���� �������
#define		OPTION_ATT_WATER_UP			88		// �����Ӽ� ���� �������
#define		OPTION_ATT_WIND_UP			89		// ��ǳ�Ӽ� ���� �������
#define		OPTION_ATT_EARTH_UP			90		// �����Ӽ� ���� �������
#define		OPTION_ATT_DARK_UP			91		// ��ҼӼ� ���� �������
#define		OPTION_ATT_LIGHT_UP			92		// ���Ӽ� ���� �������
#define		OPTION_DEF_FIRE_UP			93		// ȭ���Ӽ� ��� �������
#define		OPTION_DEF_WATER_UP			94		// �����Ӽ� ��� �������
#define		OPTION_DEF_WIND_UP			95		// ��ǳ�Ӽ� ��� �������
#define		OPTION_DEF_EARTH_UP			96		// �����Ӽ� ��� �������
#define		OPTION_DEF_DARK_UP			97		// ��ҼӼ� ��� �������
#define		OPTION_DEF_LIGHT_UP			98		// ���Ӽ� ��� �������
#define		OPTION_ALL_STAT_UP			99		// ��罺��(4��) ���
#define		OPTION_PVP_DAMAGE_ABSOLB	100		// PVP ������ ���(%)
#define		OPTION_DEBUF_DECR_TIME		101		// ������ð� ����(%)
#define		OPTION_RECOVER_HP_NOTRATE	102		// HPȸ�� ������
#define		OPTION_RECOVER_MP_NOTRATE	103		// MPȸ�� ������
#define		OPTION_INCREASE_STRONG		104		// ���������� (����)
#define		OPTION_INCREASE_HARD		105		// �ϵ� ���ߵ� ���� (����)
#define		OPTION_INCREASE_HP			106		// hp ȸ���� ��� (����)
#define		OPTION_PERSONAL_ALL_STAT_UP	107		// [����] ��ü ���� ���
#define		OPTION_PERSONAL_EXP_UP		108		// [����] ��ü ���� ���
#define		OPTION_PERSONAL_GOLD_UP		109		// [����] ��ü ���� ���
#define		OPTION_PARTY_ALL_STAT_UP	110		// [��Ƽ] ��ü ���� ���
#define		OPTION_PARTY_EXP_UP			111		// [��Ƽ] ��ü ���� ���
#define		OPTION_PARTY_GOLD_UP		112		// [��Ƽ] ��ü ���� ���

#define		MAX_NUM_OPTION				120		// �ɼ� �� ����

#define		MAX_OPTION_INC_ORIGIN		6		// �������� ���� �ִ� �ɼ� ��

#define		MAX_SOCKET_OPTION			7		// socket system : max count about socket hole [5/10/2010 rumist]
#define		OPTION_VALUE_SHIFT			8		// 2byte ���� ���� 1Byte �ɼ� ��ȣ ������ 1Byte �ɼ� ����
#define		OPTION_VALUE_AND			0x11111111
#define		OPTION_MAX_LEVEL			7		// �ɼ� �ִ� ���� ���� 7

#define		ORIGIN_VAR_DEFAULT			100

enum eJewel_OptionType
{
	JEWEL_OPTION_PACIVE = 0,
	JEWEL_OPTION_ACTIVE_SKILL_ATT = 1,
	JEWEL_OPTION_ACTIVE_SKILL_DEF = 2,
};

class ENGINE_API COptionData : public stOption, public LodLoader<COptionData>
{
private:
	std::vector<int> optionVal;
	std::string	name;
public:
	inline int GetIndex()			
	{	return getindex(); 	}
	void SetName(const char* str)	{ name = str; }
	inline const char* GetName() const
	{	return name.c_str(); }

	inline int GetValue(int iLevel) const
	{
		if (iLevel >= 0 && iLevel < DEF_OPTION_MAX_LEVEL)
			return value[iLevel];

		return 0;
	}

	//-----------------------------------------------------------------------------
	// Purpose: �ɼ� �����͸� ȭ�Ϸκ��� �о���Դϴ�.
	// Input  : &apOptionData - ����� �迭.
	//			FileName - ���ϸ�
	// Output : 	static int
	//-----------------------------------------------------------------------------
	static bool loadEx(const char* FileName);

	//[sora] �̹��� ��Ʈ�� index ǥ��
	void SetNoTranslate()
	{
		char buff[MAX_PATH] = {0,};
		sprintf( buff, "[%d] : option", getindex() );
		name = buff;
	}

	void ClearNoTranslate()
	{
		name = "";
	}
};

#endif



