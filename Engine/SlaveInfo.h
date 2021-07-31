// ----------------------------------------------------------------------------
//  File : SlaveInfo.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------
#ifndef	SLAVEINFO_H_
#define	SLAVEINFO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Base/CTString.h>
#include <Engine/Base/FileName.h>
#include <Engine/Effect/CEffectGroupManager.h>
// #include <Engine/Effect/CEffectGroup.h>
#include <map>
enum eSlaveAnimation
{
	SLAVE_ANIM_START = 0, 
	SLAVE_ANIM_WALK_1 = SLAVE_ANIM_START,
	SLAVE_ANIM_IDLE_1,
	SLAVE_ANIM_IDLE_2,
	SLAVE_ANIM_RUN_1,	
	SLAVE_ANIM_DAMAGE,
	SLAVE_ANIM_DIE,
	SLAVE_ANIM_ATTACK_0,
	SLAVE_ANIM_ATTACK_1,	
	SLAVE_ANIM_SKILL_0,
	SLAVE_ANIM_SKILL_1,	
	SLAVE_ANIM_END = SLAVE_ANIM_SKILL_1,
	SLAVE_ANIM_TOTAL,
};

enum eSlaveAttackType
{
	SLAVE_ATTACK1 = 0,
	SLAVE_ATTACK2,	
	SLAVE_MAX_ATTACK,
};

//---------------------------------------------------------------------
class ENGINE_API CSlaveInfo
{
private:
	typedef struct _tagSlaveInfo
	{
		_tagSlaveInfo()
		{
			iSkillIndex = -1;
			fWalkSpeed	= 0.0f;
			fRunSpeed	= 0.0f;
		};

		~_tagSlaveInfo()
		{
		};

		CTString		strName;									// �̸� ����.
		CTString		strFileName;								// ���ϸ�.
		CTString		aStrAnimationName[SLAVE_ANIM_TOTAL];		// �ִϸ��̼� �̸�.
		FLOAT			fWalkSpeed;
		FLOAT			fRunSpeed;
		INDEX			iSkillIndex;								// �������̽����� ���̴� ��ų �ε���.
	}sSlaveInfo;	

public:

	enum eSlaveType
	{
		SLAVE_FIRE	= 0,		
		SLAVE_WIND,
		SLAVE_LAND,
		SLAVE_WATER,
		SLAVE_GUARD,		// ���
		SLAVE_TOTAL,
	};

	enum eCommandType
	{
		COMMAND_NONE = -1,
		COMMAND_ATTACK = 0,
		COMMAND_PINCER,
		COMMAND_HOLD,
		COMMAND_PROTECTION,
		COMMAND_TOTAL,
	};
	
	CSlaveInfo();
	~CSlaveInfo();

	static CSlaveInfo &Instance()			{ return m_instance;	}

	// Set Properties
	void		SetName( int iSlave, const CTString& strName );

	// Get Properties
	const CTString	&GetName( int iSlave ) const ;
	const CTString	&GetFileName( int iSlave ) const;
	
	const CTString	&GetAnimationName( int iSlave, int iAnimType ) const;
	void		SetSlaveDataToEntity( int iSlave, CEntity *pEntity, BOOL bUseAI = FALSE );

	FLOAT		GetWalkSpeed( int iSlave ) const;
	FLOAT		GetRunSpeed( int iSlave ) const;

	void		SetSkillIndex( int iSlave, INDEX iSkillIndex );
	INDEX		GetSkillIndex( int iSlave ) const;
	void		InitSkillIndex();

	// Date : 2005-12-15(���� 11:28:11), By Lee Ki-hwan
	// �⺻ ����Ʈ ���� ó��	 
	std::map<SLONG, CEffectGroup *>		m_mapEG;

	void StartIdleEffect( SLONG slSlaveIndex, SBYTE sbElementalType, CEntity* penEntity );
	void StopIdleEffect( SLONG slSlaveIndex,  bool bAllStop = FALSE );

protected:
	static CSlaveInfo m_instance;
	sSlaveInfo		*m_pSlaveInfo;
};

inline CSlaveInfo &SlaveInfo()				{ return CSlaveInfo::Instance();	}

#endif // SLAVEINFO_H_