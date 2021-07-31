#ifndef __MOBTARGET_H__
#define __MOBTARGET_H__


#include <Engine/Object/ObjectBase.h>
#include <Engine/Base/CTString.h>
#include <Engine/Interface/UIBuff.h>
#include <Engine/Entities/StatusEffect.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Network/ChatMsgBuffer.h>

// FIXME : ���� ���� �ʿ�...��.��
class ENGINE_API CMobTarget : public ObjectBase
{
public:
	enum eMobSubType
	{
		MST_NONE			= 0,		// None
		MST_MERCENARY		= 1,		// ���� �뺴
		MST_BUFF_TOTEM		= 2,		// ������ ����
		MST_ATTACK_TOTEM	= 3,		// ������ ����
		MST_TRAP			= 4,		// Ʈ��
		MST_SUICIDE			= 5,		// �����(�з�����Ʈ)
	};
	
	INDEX			mob_iLevel;
	BOOL			mob_bNPC;
	SBYTE			mob_sbNameLength;
	SBYTE			mob_BuffCount;
	BuffInfo		mob_Buff[20];
	CStatusEffect	mob_statusEffect;
	CEffectGroup	*mob_pNormalEffect;
	UWORD			mob_sbAttributePos;
	SLONG			mob_Label;				// [sora] ������ �ý��� ��� ǥ�� index
	INDEX			mob_iOwnerIndex;		// [2010/10/20 : Sora] ���� �뺴 ī��
	INDEX			mob_iSubType;			//	Mob SubType ()
	
	ChatMsgBuffer	ChatMsg;
	INDEX			mob_iSyndicateType;		// ���� �ý��� �Ҽ�.
	INDEX			mob_iSyndicateGrade;
	
public:
	/* Default constructor. */
	CMobTarget(void);
	/* Destructor. */
	~CMobTarget(void);
	
	CMobTarget(const CMobTarget &other);
	CMobTarget &operator=(const CMobTarget &other);
	
	//������ ���� ���� �̱� ���� �۾�	07.27
	void	Init(void);
	void	Clear();
	//������ ���� �� �̱� ���� �۾�		07.27
	void	SetData( INDEX index, INDEX type, CTString& strName, INDEX level, CEntity *pEntity,
						BOOL bNPC, SBYTE sbyLayer );
	void	SetClientMobId( INDEX index );
	BOOL	IsNPC() const { return mob_bNPC; }
	void	AddBuff( BuffInfo &rBuffInfo )
	{
		mob_Buff[mob_BuffCount++] = rBuffInfo;
	}
	void	RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex );
	void	ChangeStatus(CTagManager* pTM, SQUAD status)
	{
		if(mob_statusEffect.IsStatusChanged(status))
			mob_statusEffect.ChangeStatus(pTM, status, CStatusEffect::R_ENEMY);
	}
	void	ResetStatus()
	{
		mob_statusEffect.Reset();
	}
	
	void	BuffsStartGroupEffect(void);
	BOOL	IsTotem() { return ( mob_iSubType == MST_BUFF_TOTEM || mob_iSubType == MST_ATTACK_TOTEM ); }
	BOOL	IsMercenary() { return ( mob_iSubType == MST_MERCENARY ); }
	BOOL	IsTrap() { return ( mob_iSubType == MST_TRAP ); }
	BOOL	IsParasite() { return ( mob_iSubType == MST_SUICIDE ); }
	void	SetSubType( eMobSubType SubType )	{ mob_iSubType = SubType; }
	void	SetSyndicateData(INDEX type, INDEX grade);
	int		GetSyndiType() { return mob_iSyndicateType; }
	int		GetSyndiGrade() { return mob_iSyndicateGrade; }
};

#endif //__MOBTARGET_H__