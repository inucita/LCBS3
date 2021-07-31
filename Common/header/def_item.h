
#ifndef		ITEM_DATA_H_
#define		ITEM_DATA_H_

#include "def_base.h"

#define	MAX_MAKE_ITEM_MATERIAL		10
#define DEF_MAX_ORIGIN_OPTION		10
#define MAX_WEARING					12
#define MAX_SET_ITEM_OPTION			11			// ��Ʈ �����ۿ� �ٴ� �ִ� �ɼ� ����

#define DEF_SMC_DEFAULT_LENGTH		64
#define DEF_EFFECT_DEFAULT_LENGTH	32

/*

union
{
int	num0;
int pAttack;		// ���� ���ݷ�
int	pDefense;		// ���� ����
int sec;			// ��(s)(����) ��ȿ(-1)
int skillType;		// ����.
int	warpType;		// ���� Ÿ��
int questnum;		// ����Ʈ ��ȣ.
int processtype;	// ���� Ÿ��
int optiontype;		// �ɼ� Ÿ��
int	producttype;	// ���� Ÿ��
int boxtype;		// Box Type // Date : 2005-01-12,   By Lee Ki-hwan
int	refinetype;		// Refine Type // Date : 2005-01-12,   By Lee Ki-hwan
int pettype;		// �ֿϵ��� ����.
int jumpinglevel;	// ���� ���� ����
};

union
{
int num1;
int	mAttack;		// ���� ���ݷ�
int	mDffense;		// ���� ����
int recoverHP;		// HP ȸ��...
int skillLevel;		// ����
int zonenum;		// ���� ��ȣ.
int processsubtype;	// ���� ���� Ÿ��.
int	productsubtype;	// ���� Ÿ��
};

union
{
int num2;
int attackSpeed;	// ���� �ӵ�.
int recoverMP;		// MP ȸ��
int extranum;		// Extra ��ȣ.
int sskill;			// Ư�� ��ų
int grade;			// ���
};

union
{
int num3;
int count;			// ���� ���� ���� 
};

*/
#pragma pack(push, 1)

struct stItem : public stTbl_base
{
	int		job;
	int		stack;
	int		fame;
	int		level;
	__int64	flag;
	int		wearing;
	int		type;
	int		subType;
	int		needItemIndex[MAX_MAKE_ITEM_MATERIAL];
	int		needItemCount[MAX_MAKE_ITEM_MATERIAL];
	int		needSSkillIndex;
	int		needSSkillCount;
	int		needSSkillIndex2;
	int		needSSkillCount2;
	int		textureID;
	int		textureRow;
	int		textureCol;
	int		num0;
	int		num1;
	int		num2;
	int		num3;
	int		price;

	int     set0;
	int     set1;
	int     set2;
	int     set3;
	int     set4;

	char	fileSMC[DEF_SMC_DEFAULT_LENGTH];
	char	efffectName[DEF_EFFECT_DEFAULT_LENGTH];
	char	attackEffectName[DEF_EFFECT_DEFAULT_LENGTH];
	char	damageEffectName[DEF_EFFECT_DEFAULT_LENGTH];

	int		JewelOptionType;
	int		JewelOptionLevel;

	int		rareOptionType[DEF_MAX_ORIGIN_OPTION];
	int		rareOptionLevel[DEF_MAX_ORIGIN_OPTION];
	int		syndicate_type;
	int		syndicate_grade;

	int		fortuneIndex;
	char	castleWar;
};

struct stFortune
{
	int		skill_index;
	int		skill_level;
	int		string_index;
	int		prob;
};

struct stSetItem : public stTbl_base
{
	int		job;
	int		optionCnt;
	int		item[MAX_WEARING];
	int		wearCnt[MAX_SET_ITEM_OPTION];
	int		optionType[MAX_SET_ITEM_OPTION];
	int		option[MAX_SET_ITEM_OPTION];
	int		optionLevel[MAX_SET_ITEM_OPTION];
};

#pragma pack(pop)

#endif		// ITEM_DATA_H_