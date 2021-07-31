// ----------------------------------------------------------------------------
//  File : JobInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------
#ifndef	JOBINFO_H_
#define	JOBINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/DesignClasses/Singleton/SingletonBase.h>

enum eAnimation
{
	ANIM_START = 0,
	ANIM_BASIC_BEGIN = ANIM_START,			// �⺻ �ִϸ��̼� ����
	ANIM_WALK_1 = ANIM_START,
	ANIM_WALK_2,
	ANIM_IDLE,
	ANIM_RUN_1,
	ANIM_RUN_2,
	ANIM_ATTACK_IDLE,
	ANIM_ATTACK_1,
	ANIM_ATTACK_2,
	ANIM_ATTACK_3,
	ANIM_ATTACK_4,
	ANIM_SIT,
	ANIM_SIT_CONTINUE,
	ANIM_STANDUP,	
	ANIM_PICK,
	ANIM_DAMAGE,
	ANIM_DIE,	
	ANIM_EXT_IDLE_1,
	ANIM_EXT_RUN_2,
	ANIM_EXT_WALK_2,
	ANIM_EXT_ATTACK_IDLE,
	ANIM_EXT_ATTACK_1,
	ANIM_EXT_ATTACK_2,
	ANIM_EXT_ATTACK_3,
	ANIM_EXT_ATTACK_4,
	ANIM_EXT_PICK,
	ANIM_EXT_DAMAGE,
	ANIM_EXT_DIE,
	ANIM_BASIC_END = ANIM_EXT_DIE,			// �⺻ �ִϸ��̼� ��
	ANIM_BASIC_TOTAL = ANIM_BASIC_END,
	ANIM_MINE,
	ANIM_COLLECT,
	ANIM_GATHER,
	ANIM_SOCIAL_0,
	ANIM_SOCIAL_1,
	ANIM_SOCIAL_2,
	ANIM_SOCIAL_3,
	ANIM_SOCIAL_4,
	ANIM_SOCIAL_5,
	ANIM_SOCIAL_6,
	ANIM_SOCIAL_7,
	ANIM_SOCIAL_8,
	ANIM_SOCIAL_9,
	ANIM_SOCIAL_10,
	ANIM_SOCIAL_11,

	// �ֿϵ����� Ÿ����...
	ANIM_RIDE_HORSE_WALK_1,
	ANIM_RIDE_HORSE_IDLE_1,
	ANIM_RIDE_HORSE_IDLE_2,
	ANIM_RIDE_HORSE_RUN_1,
	ANIM_RIDE_HORSE_PICK,
	ANIM_RIDE_HORSE_DAMAGE,
	ANIM_RIDE_HORSE_DIE,
	ANIM_RIDE_HORSE_LEVELUP,
	ANIM_RIDE_HORSE_SIT_CONTINUE,
	ANIM_RIDE_HORSE_SKILL_1,
	ANIM_RIDE_HORSE_SKILL_2,
	ANIM_RIDE_HORSE_SKILL_3,
	ANIM_RIDE_HORSE_SKILL_4,
	ANIM_RIDE_HORSE_SIT,
	ANIM_RIDE_HORSE_STANDUP,

	// �ֿϵ����� Ÿ����...
	ANIM_RIDE_DRAGON_WALK_1,
	ANIM_RIDE_DRAGON_IDLE_1,
	ANIM_RIDE_DRAGON_IDLE_2,
	ANIM_RIDE_DRAGON_RUN_1,
	ANIM_RIDE_DRAGON_PICK,
	ANIM_RIDE_DRAGON_DAMAGE,
	ANIM_RIDE_DRAGON_DIE,
	ANIM_RIDE_DRAGON_LEVELUP,
	ANIM_RIDE_DRAGON_SIT_CONTINUE,
	ANIM_RIDE_DRAGON_SKILL_1,
	ANIM_RIDE_DRAGON_SKILL_2,
	ANIM_RIDE_DRAGON_SKILL_3,
	ANIM_RIDE_DRAGON_SKILL_4,
	ANIM_RIDE_DRAGON_SIT,
	ANIM_RIDE_DRAGON_STANDUP,
	
	// ��ť���� �߰�
	ANIM_RIDE_DEMONBAT_WALK_1,
	ANIM_RIDE_DEMONBAT_IDLE_1,
	ANIM_RIDE_DEMONBAT_IDLE_2,
	ANIM_RIDE_DEMONBAT_RUN_1,
	ANIM_RIDE_DEMONBAT_PICK,
	ANIM_RIDE_DEMONBAT_DAMAGE,
	ANIM_RIDE_DEMONBAT_DIE,
	ANIM_RIDE_DEMONBAT_LEVELUP,
	ANIM_RIDE_DEMONBAT_SIT_CONTINUE,
	ANIM_RIDE_DEMONBAT_SKILL_1,
	ANIM_RIDE_DEMONBAT_SKILL_2,
	ANIM_RIDE_DEMONBAT_SKILL_3,
	ANIM_RIDE_DEMONBAT_SKILL_4,
	ANIM_RIDE_DEMONBAT_SIT,
	ANIM_RIDE_DEMONBAT_STANDUP,
	
	// ����Ʈ ������ ���� ������ ����
	ANIM_FLYING_READY,
	ANIM_FLYING_MOVE,
	ANIM_END = ANIM_FLYING_MOVE,
	// ĳ���� ����, ���� â������ ���
	ANIM_LOGIN_GAMESTART,
	ANIM_LOGIN_IDLE01,
	ANIM_LOGIN_IDLE02,
	ANIM_TOTAL,
};

enum eAttackType
{
	ATTACK1 = 0,
	ATTACK2,
	ATTACK3,
	ATTACK4,
	EXT_ATTACK1,
	EXT_ATTACK2,
	EXT_ATTACK3,
	EXT_ATTACK4,
	MAX_ATTACK,
};

enum eCharacterWearing 
{
	HEAD		= 0,
	BODYDOWN	= HEAD+1,
	BODYUP		= HEAD+2,
	FOOT		= HEAD+3,
	HAND		= HEAD+4,
	SKIRT		= HEAD+5,
	WEARTOTAL	= HEAD+6,
};	
	
class ENGINE_API CJobInfo : public CSingletonBase< CJobInfo >
{
private:
	typedef struct _tagJobInfo
	{
		_tagJobInfo()
		{
			iAttackSpeed				= 0;		
		};

		~_tagJobInfo()
		{
		};

		CTString		strName;									// �̸� ����.
		CTString		strExtensionName[2];						// ���� Ŭ���� �̸�.
		CTString		strFileName;								// ���ϸ�.
		CTString		aStrAnimationName[ANIM_TOTAL];				// �ִϸ��̼� �̸�.
		int				iAttackSpeed;								// �⺻ ����
		int				iSkillWeaponType[2];						// ��ų ��� ���� ���� ����		
		float			afImpactTimeTable[MAX_ATTACK];				// ����Ʈ�� ���� �� �������� ����Ǵ� Ÿ�̹�.
		CTFileName		aStrMeshName[WEARTOTAL];					// �޽���.
		CTFileName		aStrTextureName[WEARTOTAL];					// �ؽ��ĸ�.
		CTFileName		aStrTexNormalName[WEARTOTAL];				// �븻��.
		
	}sJobInfo;

public:
	CJobInfo();
	~CJobInfo();

	// Set Properties
	void		SetName( int iJob, const CTString& strName );
	void		SetExtensionName( int iJob, int iType, const CTString& strName );

	// Get Properties
	CTString	GetName( int iJob, int iJob2 = 0 );
	CTString	GetExtensionName( int iJob, int iType );
	CTString	GetFileName( int iJob );

	int			GetSkillWeponType( int iJob, int iType )	const;
	float		GetImpactTime( int iJob, int iAttackType )	const;
	CTString	GetAnimationName( int iJob, int iAnimType );

	CTFileName	GetMeshName( int iJob, int iWearPos );
	CTFileName	GetTextureName( int iJob, int iWearPos );
	CTFileName	GetTexNormalName( int iJob, int iWearPos );

	//CTString	GetIdleAnim( int iJob );
	//CTString	GetWalkAnim( int iJob );
	//CTString	GetAttackAnim( int iJob );
	//CTString	GetDieAnim( int iJob );
	//CTString	GetDamageAnim( int iJob );

protected:
	sJobInfo		*m_pJobInfo;
};

#endif // JOBINFO_H_