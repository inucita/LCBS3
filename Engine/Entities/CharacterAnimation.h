#ifndef	CHARACTER_ANIMATION_H_
#define CHARACTER_ANIMATION_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif
#include <Engine/JobInfo.h>
#include <Engine/PetInfo.h>
//#include <Engine/SlaveInfo.h>
#include <Engine/Entities/MobData.h>

// ĳ���� �ִϸ��̼�.
class ENGINE_API CCharacterAnimation
{
public:
	INDEX	m_iAnimation[ANIM_TOTAL];
};

class ENGINE_API CEnemyAnimation
{
public:
	INDEX	m_iAnimation[NPC_ANIM_TOTAL];
};


// �ֿϵ��� �ִϸ��̼�.
class ENGINE_API CPetAnimation
{
public:
	INDEX	m_iAnimation[PET_ANIM_TOTAL];
};
/*
// ��ȯ�� �ִϸ��̼�
class ENGINE_API CSlaveAnimation
{
public:
	INDEX	m_iAnimation[SLAVE_ANIM_TOTAL];
};
*/

#endif