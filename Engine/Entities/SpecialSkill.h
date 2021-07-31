#ifndef INCL_SPECIALSKILL_H
#define INCL_SPECIALSKILL_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Help/LoadLod.h>
#include <common/header/def_skill.h>

// Special Skill Ÿ��
#define		SSKILL_MINING		0	// ä��
#define		SSKILL_GATHERING	1	// ä��
#define		SSKILL_CHARGE		2	// ����
#define		SSKILL_STONE		3	// �������ü�
#define		SSKILL_PLANT		4	// �Ĺ�������
#define		SSKILL_ELEMENT		5	// ����������

#define		SSKILL_MAKE_WEAPON	6	// ���� ���� ���
#define		SSKILL_MAKE_WEAR	7	// �� ���� ���
#define		SSKILL_MAKE_G_B		8	// �尩 ���� ���� ���
#define		SSKILL_MAKE_ARMOR	9	// ���� ���� ���� ���
#define		SSKILL_MAKE_H_S		10	// ���� ���� ���� ���
#define		SSKILL_MAKE_POTINO	11  // ���� ���� // Date : 2005-01-07,   By Lee Ki-hwan
#define		SSKILL_PROCESS_NPC	12	// ���� NPC

#define		SSKILL_STONE_0		3	// ���� �� ���ü�
#define		SSKILL_STONE_1		4	// ���� ���� ���ü�
#define		SSKILL_STONE_2		5	// ���� ���� ���ü�
#define		SSKILL_STONE_3		6	// ���� ��Ʈ ���ü�
#define		SSKILL_STONE_4		7	// ���� ������ ���ü�

#define		SSKILL_PLANT_0		8	// ũ���� ����� ������
#define		SSKILL_PLANT_1		9	// ũ���� �ٱ� ������
#define		SSKILL_PLANT_2		10	// ũ���� �Ķ��� ������
#define		SSKILL_PLANT_3		11	// ũ���� ���� ������
#define		SSKILL_PLANT_4		12	// ũ���� �� ������

#define		SSKILL_ELEMENT_0	13	// ���� E��� ������
#define		SSKILL_ELEMENT_1	14	// ���� D��� ������
#define		SSKILL_ELEMENT_2	15	// ���� C��� ������
#define		SSKILL_ELEMENT_3	16	// ���� B��� ������
#define		SSKILL_ELEMENT_4	17	// ���� A��� ������

class CSpecialSkill : public stSpecailSkill, public LodLoader<CSpecialSkill>
{
public:
	static bool loadEx(const char* str_path);
	std::string name;
	std::string	desc;
	int		transFlag;	//[sora] �̹��� ��Ʈ�� index ǥ��	

	inline int GetIndex()			const
	{	return index;	}

	inline const char* GetName()	const
	{	return name.c_str();	}

	inline int GetType()			const
	{	return type;	}

	inline int GetLearnLevel( int iIndex) const
	{	return needLevel[iIndex];	}

	inline int GetLearnSP( int iIndex) const
	{	return needSP[iIndex];	}

	inline int GetLearnSkillIndex()		const
	{	return needSSkill;		}

	inline int GetLearnSkillLevel()	const
	{	return needSSkillLevel;	}

	inline int GetMaxLevel()	const
	{	return maxLevel;	}

	inline int GetPreference()	const
	{	return preference;	}

	inline int	GetIconTexID()	const { return textureID; }
	inline int	GetIconTexRow() const { return textureRow; }
	inline int	GetIconTexCol() const { return textureCol; }	

	inline const char* GetDescription()	const
	{	return desc.c_str();	}
	
//	static int LoadSSkillDataFromFile(CStaticArray<CSpecialSkill> &apSkillData, const char* FileName);

	//[sora] �̹��� ��Ʈ�� index ǥ��
	void SetNoTranslate();
	void ClearNoTranslate();

	void SetName(const char* str)		{name = str; }
	void SetDesc(const char* str)		{desc = str; }
	int& GetTransFlag()					{ return transFlag; }

//	static CSpecialSkill* m_pCont;
};

#endif