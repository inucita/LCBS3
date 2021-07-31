#ifndef __PETTARGET_H__
#define __PETTARGET_H__

#include <Engine/Object/ObjectBase.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/GlobalDefinition.h>

class  CPetTargetInfom
{
public:
	LONG	lIndex;
	BOOL	bIsActive;	
	FLOAT	fMaxHealth;
	FLOAT	fHealth;
	FLOAT	fMaxHungry;
	FLOAT	fHungry;
	int		iLevel;
	int		iType;
	int		iAge;
	LONG	lAbility;
	INDEX	TransformIndex;
	CTString	strNameCard;

	CEntity	*pen_pEntity;	

public:
	CPetTargetInfom(void);
	~CPetTargetInfom(void);

	ENGINE_API void	Init();
};

// FIXME : ���� ���� �ʿ�...��.��
class ENGINE_API CPetTarget : public ObjectBase
{
public:
	INDEX			pet_iAge;
	CTString		pet_OwnerName;
	CStatusEffect	pet_statusEffect;
	CEffectGroup	*pet_pNormalEffect;
	UWORD			pet_sbAttributePos;
	INDEX			pet_OwnerIndex;	
	CTString		pet_strNameCard;
	
public:
	/* Default constructor. */
	CPetTarget(void);
	/* Destructor. */
	~CPetTarget(void);
	
	CPetTarget(const CPetTarget &other);
	CPetTarget &operator=(const CPetTarget &other);
	
	void	SetData( INDEX index, CTString& strName, CTString& strOwnerName, INDEX iOwnerIndex, INDEX iPetType, INDEX iPetAge, CEntity *pEntity, SBYTE sbyLayer, CTString strNameCard );	
	void	SetClientPetId( INDEX index );
	void	ChangeStatus(CTagManager* pTM, SQUAD status)
	{
		if(pet_statusEffect.IsStatusChanged(status))
			pet_statusEffect.ChangeStatus(pTM, status, CStatusEffect::R_ENEMY);
	}
	void	ResetStatus()
	{
		pet_statusEffect.Reset();
	}
};
enum STATETYPE
{
	STATE_ATTACK		= 0,	// ���ݷ�
	STATE_MAGIC_ATTACK	= 1,	// ���� ���ݷ�
	STATE_DEFENCE		= 2,	// ����	
	STATE_MAGIC_DEFENCE	= 3,	// ���� ����
	STATE_HITPOINT		= 4,	// ���ߵ�
	STATE_AVOID			= 5,	// ȸ�ǵ�
// 	STATE_STRONG		= 6,	// ������	
// 	STATE_MAGIC_AVOID	= 7,	// ���� ȸ�ǵ�
// 	STATE_ATTACK_SPEED	= 8,	// ���� �ӵ�
// 	STATE_RUN_SPEED		= 9,	// �̵� �ӵ�
// 	STATE_CRITICAL		= 10,	// ũ��Ƽ�� Ȯ��
// 	STATE_DEADLY		= 11,	// ���鸮 Ȯ��
	STATE_END,
};

class ENGINE_API CWildPetTarget : public ObjectBase
{
public:
	BOOL		bIsActive;
	BOOL		bDeath;
	INDEX		m_nIndex;		// �� �� �ε���
	INDEX		m_nOwnerIndex;
	INDEX		m_nLevel;		// ���� �� ����
	INDEX		m_nLevelupPoint;	// ���� �� ��������Ʈ
	// Bagic
	INDEX		m_nStr;			// ���� �� ��
	INDEX		m_nStrPlus;		//	���� �� �� �߰�
	INDEX		m_nCon;			// ���� �� ü��
	INDEX		m_nConPlus;		// ���� �� ü�� �߰�	
	INDEX		m_nDex;			// ���� �� ��ø
	INDEX		m_nDexPlus;		// ���� �� ��ø �߰�
	INDEX		m_nInt;			// ���� �� ����
	INDEX		m_nIntPlus;		// ���� �� ���� �߰�
	__int64		m_exp;			// ���� �� ����ġ
	__int64		m_next_exp;		// ���� �� �ƽ�(������) ����ġ
	INDEX		m_nHP;			// ���� �� HP
	INDEX		m_nMP;			// ���� �� MP
	INDEX		m_nMaxHP;		// ���� �� Max HP
	INDEX		m_nMaxMP;		// ���� �� Max MP
	INDEX		m_nFaith;		// ���� �� �漺��
	INDEX		m_nStm;			// ���� �� �����
	INDEX		m_nMaxFaith;	// ���� �� �ִ� �漺��
	INDEX		m_nMaxStm;		// ���� �� �ִ� �����
	INDEX		m_nSpeed;		// ���� �� �̵� �ӵ�
	INDEX		m_nAISlot;		// ���� �� AI ���԰���
	UBYTE		m_sbAttributePos;
	SBYTE		m_sbTransStat;
	BOOL		m_bMount;
	// state
	INDEX	m_nWildPetState[STATE_END];	// ���� �� ����
	INDEX	m_nWildPetStatePlus[STATE_END]; // ���� �÷���
	INDEX	m_nPetWearIndex[WILDPET_WEAR_TOTAL];
	INDEX	m_nPetWearPlus[WILDPET_WEAR_TOTAL];
public:
	CWildPetTarget();
	~CWildPetTarget();

	CWildPetTarget(const CWildPetTarget &other);
	CWildPetTarget &operator=(const CWildPetTarget &other);	

	void SetWildPetToEntity(CEntity *pEntity, INDEX nindex, INDEX *nWearIndex, INDEX *nWearPlus);

};

#endif //__PETTARGET_H__