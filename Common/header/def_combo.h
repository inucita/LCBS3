
#ifndef		MONSTERCOMBO_DATA_H_
#define		MONSTERCOMBO_DATA_H_

#include "def_base.h"
#pragma pack(push, 1)
struct stMonsterCombo : public stTbl_base
{
	int nas;// �����
	int texture_id;
	int texture_row;
	int texture_col;
	char bSkill; // �̼� �г�Ƽ ����
	int	point;// �̼� ����Ʈ
};
#pragma pack(pop)
#endif		// MONSTERCOMBO_DATA_H_