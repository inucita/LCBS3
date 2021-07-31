// ----------------------------------------------------------------------------
//  File : CHARACTERDEFINITION.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	CHARACTERDEFINITION_H_
#define	CHARACTERDEFINITION_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif
#include <Engine/GlobalDefinition.h>

// FIXME : GlobalDefinition�� �ִ� ����� �������� �ִ� �κ���.
// FIXME : Player.es������ ���δٸ�, ���ʿ� �ٿ�������.
// FIXME : ĳ���Ϳ� ���� ��üȭ�� �ʿ���.

//---------------------------------------------------------------------
// NOTE : Player.es������ ���̰� ����.
enum eSkills
{
		TI_POWER_MAGNUM		= 1,
		TI_BIGFOOT_ATTACK	= 2,
		TI_MOUNTAIN_HAMMER	= 3,
		TI_CRITICAL_POWER	= 4,
		TI_NITRO_BOMBER		= 5,		// �̱۴����� ��ų
//	KN_CROSS_BASH		= 4,
		KN_ARMOR_BREAK		= 15,
		KN_WEAPON_BREAK		= 16,
		KN_DIVINE_SHIELD	= 17,
		
		HE_SHINING_ARROW	= 27,
		HE_MULTI_STROKE		= 28,
		HE_HARD_REJECTION	= 29,
		HE_STORM_ARROW		= 30,
		HE_SELF_HEAL		= 31,
		HE_HEAL				= 32,
		HE_PARTY_HEAL		= 33,
		HE_DEFENCE_MANA		= 34,
		HE_DOUBLE_STING		= 52,
//	KN_TILENT_RUSH		= 11,
		KN_DASH				= 401,
//	KN_PAITAL_DIVIDE	= 13,
	
};

// �Ӹ� ����
static CTFileName _afnFaceMeshName[TOTAL_JOB][TOTAL_HEAD] =
{
	// TITAN
	{
		CTFileName("Data\\Character\\TITAN\\ti_face_000.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_face_001.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_face_002.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_face_004.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_face_005.bm"),
	},
	// KNIGHT
	{
		CTFileName("Data\\Character\\KNIGHT\\ni_face_000.bm"),
		CTFileName("Data\\Character\\KNIGHT\\ni_face_001.bm"),
		CTFileName("Data\\Character\\KNIGHT\\ni_face_002.bm"),
		CTFileName("Data\\Character\\KNIGHT\\ni_face_004.bm"),
		CTFileName("Data\\Character\\KNIGHT\\ni_face_005.bm"),
	},
	// HEALER
	{
		CTFileName("Data\\Character\\HEALER\\hw_face_000.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_face_001.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_face_002.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_face_004.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_face_005.bm"),
	},
	// MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_face_000.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_001.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_002.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_004.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_005.bm"),
	},
	// ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\ro_face_000.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_001.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_002.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_004.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_005.bm"),
	},
	// SORCERER
	{
		CTFileName("Data\\Character\\Sorcerer\\so_face_000.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_face_001.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_face_002.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_face_004.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_face_005.bm"),
	},
	// NIGHTSHADOW
	{
		CTFileName("Data\\Character\\NightShadow\\ns_face_000.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_face_001.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_face_002.bm"),		
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_face_004.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_face_005.bm"),
	},
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\ro_face_006.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_007.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_008.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_009.bm"),
		CTFileName("Data\\Character\\Rogue\\ro_face_010.bm"),
	},
#endif
		//2013/01/08 jeil EX������ �߰� �ӽ÷� ������ ���ҽ� �׷��� ��� ���ҽ� ������ �߰� ���� �ʿ� 
		//2013/01/09 jeil ���ҽ� ���� ���� �Ϸ� 
#ifdef  CHAR_EX_MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_face_006.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_007.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_008.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_009.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_face_010.bm"),
	},
#endif
};

static CTFileName _afnFaceTextureName[TOTAL_JOB][TOTAL_HEAD*2] =
{
	// TITAN
	{
		CTFileName("Data\\Character\\Titan\\ti_face_000.tex"),		// Default
		CTFileName("Data\\Character\\Titan\\ti_face_001.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_002.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_004.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_005.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_N_000.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_N_001.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_N_002.tex"),
		CTFileName(""),
		CTFileName(""),
	},
	// KNIGHT
	{
		CTFileName("Data\\Character\\Knight\\ni_face_000.tex"),		// Default
		CTFileName("Data\\Character\\Knight\\ni_face_001.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_002.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_004.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_005.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_N_000.tex"),	// Normal
		CTFileName("Data\\Character\\Knight\\ni_face_N_001.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_N_002.tex"),
		CTFileName(""),
		CTFileName(""),
	},
	// HEALER
	{
		CTFileName("Data\\Character\\HEALER\\hw_face_000.tex"),		// Default
		CTFileName("Data\\Character\\HEALER\\hw_face_001.tex"),
		CTFileName("Data\\Character\\HEALER\\hw_face_002.tex"),
		CTFileName("Data\\Character\\HEALER\\hw_face_004.tex"),
		CTFileName("Data\\Character\\HEALER\\hw_face_005.tex"),
		CTFileName(""),		// Normal Map
		CTFileName(""),		// Normal Map
		CTFileName(""),		// Normal Map
		CTFileName(""),
		CTFileName(""),
	},
	// MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_face_000.tex"),		// Default
		CTFileName("Data\\Character\\MAGE\\ma_face_001.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_002.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_004.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_005.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\ro_face_000.tex"),		// Default
		CTFileName("Data\\Character\\Rogue\\ro_face_001.tex"),
		CTFileName("Data\\Character\\Rogue\\ro_face_002.tex"),
		CTFileName("Data\\Character\\Rogue\\ro_face_004.tex"),
		CTFileName("Data\\Character\\Rogue\\ro_face_005.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// SORCERER
	{
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_face_000.tex"),
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_face_001.tex"),
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_face_002.tex"),
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_face_004.tex"),
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_face_005.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// NIGHTSHADOW
	{
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_face_000.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_face_001.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_face_002.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_face_004.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_face_005.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_face_006.tex"),		// Default
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_face_007.tex"),
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_face_008.tex"),
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_face_009.tex"),
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_face_010.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
#endif
		//2013/01/08 jeil EX������ �߰� ���ҽ� ��� �ӽ÷� ������ ���ҽ� ��� ���ҽ� ������ �߰� ���� �ʿ� 
		//2013/01/09 jeil ���� ���ҽ��� ���� 
#ifdef CHAR_EX_MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_face_06.tex"),		// Default
		CTFileName("Data\\Character\\MAGE\\ma_face_07.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_08.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_09.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_10.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
#endif

};

// ��� ��Ÿ��
static CTFileName _afnHairMeshName[TOTAL_JOB][TOTAL_HAIR] =
{
	// TITAN
	{
		CTFileName("Data\\Character\\TITAN\\ti_hair_000.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_hair_001.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_hair_002.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_hair_004.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_hair_005.bm"),
	},
	// KNIGHT
	{
		CTFileName("Data\\Character\\Knight\\ni_hair_000.bm"),
		CTFileName("Data\\Character\\Knight\\ni_hair_001.bm"),
		CTFileName("Data\\Character\\Knight\\ni_hair_002.bm"),
		CTFileName("Data\\Character\\Knight\\ni_hair_004.bm"),
		CTFileName("Data\\Character\\Knight\\ni_hair_005.bm"),
	},
	// HEALER
	{
		CTFileName("Data\\Character\\HEALER\\hw_hair_000.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_001.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_002.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_004.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_005.bm"),
	},
	// MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_hair_000.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_001.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_002.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_004.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_005.bm"),
	},
	// ROGUE
	{
		CTFileName("Data\\Character\\ROGUE\\ro_hair_000.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_001.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_002.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_004.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_005.bm"),
	},
	// SORCERER
	{
		CTFileName("Data\\Character\\Sorcerer\\so_hair_000.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_hair_001.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_hair_002.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_hair_004.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_hair_005.bm"),
	},
	// NIGHTSHADOW
	{
		CTFileName("Data\\Character\\NightShadow\\ns_hair_000.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_hair_001.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_hair_002.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_hair_004.bm"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\ns_hair_005.bm"),
	},
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Character\\ROGUE\\ro_hair_008.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_009.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_010.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_011.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_hair_012.bm"),
	},
#endif
	//2013/01/08 jeil EX������ �߰� ���ҽ� ��� �ӽ÷� ������ ���ҽ� ��� ���ҽ� �߰��� �߰� ���� �ʿ� 
	//2013/01/09 jeil ���� ���ҽ��� ���� �Ϸ� 
#ifdef CHAR_EX_MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_hair_008.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_009.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_010.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_011.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_012.bm"),
	},
#endif
};

// ��� �ؽ���...(�̰� ����...)
static CTFileName _afnHairTextureName[TOTAL_JOB][TOTAL_HAIR*3] =
{
	// TITAN
	{
		CTFileName("Data\\Character\\TITAN\\ti_face_000.tex"),		// Diffuse
		CTFileName("Data\\Character\\TITAN\\ti_face_001.tex"),
		CTFileName("Data\\Character\\TITAN\\ti_face_002.tex"),
		CTFileName("Data\\Character\\TITAN\\ti_hair_004.tex"),
		CTFileName("Data\\Character\\TITAN\\ti_hair_005.tex"),
		CTFileName("Data\\Character\\TITAN\\ti_face_N_000.tex"),	// Normal
		CTFileName("Data\\Character\\Titan\\ti_face_N_001.tex"),
		CTFileName("Data\\Character\\Titan\\ti_face_N_002.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// KNIGHT
	{
		CTFileName("Data\\Character\\Knight\\ni_face_000.tex"),		// Diffuse
		CTFileName("Data\\Character\\Knight\\ni_face_001.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_002.tex"),
		CTFileName("Data\\Character\\Knight\\ni_hair_004.tex"),
		CTFileName("Data\\Character\\Knight\\ni_hair_005.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_N_000.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_N_001.tex"),
		CTFileName("Data\\Character\\Knight\\ni_face_N_002.tex"),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// HEALER
	{
		CTFileName("Data\\Character\\HEALER\\hw_hair_000.tex"),		// Diffuse
		CTFileName("Data\\Character\\HEALER\\hw_hair_001.tex"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_002.tex"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_004.tex"),
		CTFileName("Data\\Character\\HEALER\\hw_hair_005.tex"),
		CTFileName(""),												// Normal
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_face_000.tex"),		// Diffuse
		CTFileName("Data\\Character\\MAGE\\ma_face_001.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_face_002.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_004.tex"),
		CTFileName("Data\\Character\\MAGE\\ma_hair_005.tex"),
		CTFileName(""),												// Normal
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName("Data\\Character\\MAGE\\ma_hair_000.tex"),		// Tail
		CTFileName("Data\\Character\\MAGE\\ma_hair_001_01.tex"),
		CTFileName(""),
		CTFileName("Data\\Character\\MAGE\\ma_hair_000.tex"),		// Tail
		CTFileName("Data\\Character\\MAGE\\ma_hair_001_01.tex"),
	},
	// ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\ro_hair_000.tex"),		// Diffuse
		CTFileName("Data\\Character\\Rogue\\ro_hair_001.tex"),		
		CTFileName("Data\\Character\\Rogue\\ro_hair_002.tex"),
		CTFileName("Data\\Character\\Rogue\\ro_hair_004.tex"),
		CTFileName("Data\\Character\\Rogue\\ro_hair_005.tex"),
		CTFileName(""),												// Normal
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// SORCERER
	{
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_hair_000.tex"),		// Diffuse
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_hair_001.tex"),
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_hair_002.tex"),
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_hair_004.tex"),		// Diffuse
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_hair_005.tex"),
		CTFileName(""),												// Normal
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// NIGHTSHADOW
	{
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_hair_000.tex"),		// Diffuse
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_hair_001.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_hair_002.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_hair_004.tex"),
		CTFileName("Data\\Item\\Shield\\NightShadow\\Texture\\ns_hair_005.tex"),
		CTFileName(""),												// Normal
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_hair_008.tex"),		// Diffuse
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_hair_009.tex"),		
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_hair_010.tex"),
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_hair_011.tex"),
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_hair_012.tex"),
		CTFileName(""),												// Normal
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),												// Tail
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
		CTFileName(""),
	},
#endif
	//2013/01/08 jeil EX������ �߰� ���ҽ� ��� �ӽ÷� ������ ���ҽ� ��� ���ҽ� �߰��� �߰� ���� �ʿ� 
	//2013/01/09 jeil �ؽ��ĸ� ��� �߰��ؾ� ���� �� �𸣰��� ����� ���� 
#ifdef CHAR_EX_MAGE
	{
			CTFileName("Data\\Character\\MAGE\\ma_hair_08_01.tex"),		// Diffuse
				CTFileName("Data\\Character\\MAGE\\ma_hair_09_01.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_10_01.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_11_01.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_12_01.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_08_02.tex"),		// Diffuse
				CTFileName("Data\\Character\\MAGE\\ma_hair_09_02.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_10_02.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_11_02.tex"),
				CTFileName("Data\\Character\\MAGE\\ma_hair_12_02.tex"),
				CTFileName(""),		// Tail
				CTFileName(""),
				CTFileName(""),
				CTFileName(""),		// Tail
				CTFileName(""),
	},
#endif
};

// ���(��Ÿ) �޽�
static CTFileName _afnHelmetMeshName[TOTAL_JOB][2] =
{
	// TITAN
	{
		CTFileName("Data\\Character\\TITAN\\ti_santa.bm"),
		CTFileName("Data\\Character\\TITAN\\ti_santa_1.bm"),
	},
	// KNIGHT
	{
		CTFileName("Data\\Character\\Knight\\ni_santa.bm"),
		CTFileName("Data\\Character\\Knight\\ni_santa_1.bm"),
	},
	// HEALER
	{
		CTFileName("Data\\Character\\HEALER\\hw_santa.bm"),
		CTFileName("Data\\Character\\HEALER\\hw_santa_1.bm"),
	},
	// MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_santa.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_santa_1.bm"),
	},
	// ROGUE
	{
		CTFileName("Data\\Character\\ROGUE\\ro_santa.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_santa_1.bm"),
	},
	// SORCERER
	{
		CTFileName("Data\\Character\\Sorcerer\\so_santa.bm"),
		CTFileName("Data\\Character\\Sorcerer\\so_santa_1.bm"),
	},
	// NIGHTSHADOW
	{
		CTFileName(""),
		CTFileName(""),
	},
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Character\\ROGUE\\ro_santa.bm"),
		CTFileName("Data\\Character\\ROGUE\\ro_santa_1.bm"),
	},
#endif
	//2013/01/08 jeil EX������ �߰� ���ҽ� ��� �ӽ÷� ������ ���ҽ� ��� ���ҽ� �߰��� �߰� ���� �ʿ� 
#ifdef CHAR_EX_MAGE
	{
		CTFileName("Data\\Character\\MAGE\\ma_santa.bm"),
		CTFileName("Data\\Character\\MAGE\\ma_santa_1.bm"),
	},
#endif
};

// ���(��Ÿ) �ؽ�ó
static CTFileName _afnHelmetTextureName[TOTAL_JOB][6] =
{
	// TITAN
	{
		CTFileName("Data\\Character\\TITAN\\Texture\\ti_santa.tex"),		// Diffuse
		CTFileName("Data\\Character\\TITAN\\Texture\\ti_santa_1.tex"),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
	// KNIGHT
	{
		CTFileName("Data\\Character\\Knight\\Texture\\ni_snata.tex"),		// Diffuse
		CTFileName("Data\\Character\\Knight\\Texture\\ni_snata_1.tex"),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
	// HEALER
	{
		CTFileName("Data\\Character\\HEALER\\Texture\\hw_santa.tex"),		// Diffuse
		CTFileName("Data\\Character\\HEALER\\Texture\\hw_santa_1.tex"),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
	// MAGE
	{
		CTFileName("Data\\Character\\MAGE\\Texture\\ma_santa.tex"),			// Diffuse
		CTFileName("Data\\Character\\MAGE\\Texture\\ma_santa_1.tex"),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
	// ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_santa.tex"),		// Diffuse
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_santa_1.tex"),		
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
	// SORCERER
	{
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_santa.tex"),	// Diffuse
		CTFileName("Data\\Character\\Sorcerer\\Texture\\so_santa_1.tex"),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},	
	// NIGHTSHADOW
	{
		CTFileName(""),	// Diffuse
		CTFileName(""),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},	
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_santa.tex"),		// Diffuse
		CTFileName("Data\\Character\\Rogue\\Texture\\ro_santa_1.tex"),		
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
#endif
	//2013/01/08 jeil EX������ �߰� ���ҽ� ��� �ӽ÷� ������ ���ҽ� ��� ���ҽ� �߰��� �߰� ���� �ʿ� 
#ifdef CHAR_EX_MAGE
	{
		CTFileName("Data\\Character\\MAGE\\Texture\\ma_santa.tex"),			// Diffuse
		CTFileName("Data\\Character\\MAGE\\Texture\\ma_santa_1.tex"),
		CTFileName(""),														// Normal
		CTFileName(""),
		CTFileName(""),														// Tail
		CTFileName(""),
	},
#endif
};

//���Ѹ��� �׼ǿ� ���Ǵ� ���� �� �޽�����
static CTFileName _afnDishMeshName[TOTAL_JOB][2] =
{
	//Ÿ��ź
	{
		CTFileName("Data\\Item\\Common\\ti_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_TDISH.bm"),
	},
	//����Ʈ
	{
		CTFileName("Data\\Item\\Common\\ni_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_KDISH.bm"),
	},
	//����
	{
		CTFileName("Data\\Item\\Common\\hw_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_HDISH.bm"),
	},
	//������
	{
		CTFileName("Data\\Item\\Common\\ma_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_MDISH.bm"),
	},
	//�α�
	{
		CTFileName("Data\\Item\\Common\\ro_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_RDISH.bm"),
	},
	//�Ҽ���
	{
		CTFileName("Data\\Item\\Common\\so_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_SDISH.bm"),
	},
	//����Ʈ������
	{
		CTFileName("Data\\Item\\Common\\ns_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_NSDISH.bm"),
	},
	// [2012/08/27 : Sora] EX�α� �߰�
#ifdef CHAR_EX_ROGUE
	{
		CTFileName("Data\\Item\\Common\\ro_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_RDISH.bm"),
	},
#endif
	//2013/01/08 jeil EX������ �߰� ���ҽ� ��� �ӽ÷� ������ ���ҽ� ��� ���ҽ� �߰��� �߰� ���� �ʿ� 
#ifdef CHAR_EX_MAGE
	{
		CTFileName("Data\\Item\\Common\\ma_dish.bm"),
		CTFileName("Data\\Item\\Common\\_TAG_T_MDISH.bm"),
	},
#endif
};

#define DEF_HIT_TYPE_MAX	(5)
static char* szHitEffect[TOTAL_JOB][DEF_HIT_TYPE_MAX] = {
										   {"Normal Hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
										   {"Normal Hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
										   {"Normal Hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
										   {"mHitEnerBall"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
										   {"Normal Hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
										   {"so2_Att02_Hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
										   {"ns_basic_hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
#ifdef CHAR_EX_ROGUE					   
										   {"Normal Hit"	, "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"},
#endif // CHAR_EX_ROGUE					   
#ifdef CHAR_EX_MAGE						   
										   {"mHitEnerBall_G", "SHK_POWER_HOLYWATER01", "SHK_MYSTERIOUS_HOLYWATER01", "SHK_RUSH_HOLYWATER01", "SHK_MI_HOLYWATER01"}
#endif // CHAR_EX_MAGE
										 };

//���Ѹ��� �׼ǿ� ���Ǵ� ���� �� �ؽ�������
static CTFileName _afnDishTextureName = CTFileName("Data\\Item\\Common\\Texture\\dish.tex");

#define		PLAYER_STATE_SITDOWN		(1 << 0)	// �ɱ�/����
#define		PLAYER_STATE_MOVING			(1 << 1)	// �̵�/����
#define		PLAYER_STATE_WARP			(1 << 2)	// ��ȯ ��
#define		PLAYER_STATE_PKMODE			(1 << 3)	// PK ���
#define		PLAYER_STATE_PKMODEDELAY	(1 << 4)	// PK ��� ���� ������
//PKMODE -> PKMODE + PKMODEDELAY -> ��ȭ��� ((state & PLAYER_STATE_PKMODE) && (state & PLAYER_STATE_PKMODEDELAY))
#define		PLAYER_STATE_RAMODE			(1 << 5)	// ������� ���
#define		PLAYER_STATE_CHANGE			(1 << 6)	// ������
#define		PLAYER_STATE_BILLITEM		(1 << 7)	// ���������� ������
#define		PLAYER_STATE_CASHITEM		(1 << 8)	// ĳ�������� ������
#define		PLAYER_STATE_SUPPORTER		(1 << 9)	// ���� ������ �÷��� : Ư�� ������ ����� ����Ʈ
#define		PLAYER_STATE_OTHERZONE		(1 << 10)	// �ٸ� �Ӽ��� ������ ������
#define		PLAYER_STATE_CRISTAL_RESPOND	(1 << 11)	// ���ź ���� ũ����Ż ������ // WSS_DRATAN_SEIGEWARFARE 2007/07/31 
#define		PLAYER_STATE_FLYING			(1 << 12) // ������(����Ʈ������)
#define		PLAYER_STATE_DARKNESS		(1 << 13) // �絵���(����Ʈ������)
#define		PLAYER_STATE_SKILL			(1 << 14) // ��ų�ߵ����(����Ʈ������)

#define     ACTION_NUM_WALK_RUN			2
#define     ACTION_NUM_SITDOWN			3
#define     ACTION_NUM_PICKITEM			4
#define     ACTION_NUM_GREET			10
#define     ACTION_NUM_SMILE			11
#define     ACTION_NUM_CRY				13


#define     ACTION_NUM_NUMBER_ONE		14 //�ְ�
#define     ACTION_NUM_HANDCLAP			15 //�ڼ�		
#define     ACTION_NUM_REFUSE			16 //����			
#define     ACTION_NUM_GOOD_LOOKS		17 //��ü�� ����.
#define     ACTION_NUM_GOOD_LOOKS2		18 //��ü�� ����
#define     ACTION_NUM_BANTER			19 //����
#define     ACTION_NUM_CHEER			20//ȯȣ
#define     ACTION_NUM_COMBAT			21//����
#define     ACTION_NUM_SUBMISSION		22//����
#define     ACTION_NUM_WATER_SPREAD		42//���Ѹ���

#define		ACTION_NUM_WILDPET_RIDING	50 // ���ϵ� �� Ÿ��(��ť����)


#define     ACTION_NUM_PK				23
#define		ACTION_NUM_SELL				28

#define		PK_TITLE_CHAOSLEGION		(-5)
#define		PK_TITLE_NECROKNIGHT		(-4)
#define		PK_TITLE_OUTLAW				(-3)
#define		PK_TITLE_FEARCASTER			(-2)
#define		PK_TITLE_MURDERER			(-1)
#define		PK_TITLE_NORMAL				( 0)
#define		PK_TITLE_AVENGER			(+1)
#define		PK_TITLE_LIGHTPORTER		(+2)
#define		PK_TITLE_GUARDIAN			(+3)
#define		PK_TITLE_SAINTKNIGHT		(+4)
#define		PK_TITLE_DIVINEPROTECTOR	(+5)

#define		GM_CHANGE				301		// GM - cursed rich	
#define		GM_CHANGE_MUL			0.15f		// GM - cursed rich	

#define		DEF_SOUND_HOLYWATER_HIT		  "Data\\Sounds\\Character\\public\\HolyWater_hit.wav"
#define		DEF_CAM_SHAKE				  "CAM_HIT01"

#endif