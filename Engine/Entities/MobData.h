#ifndef INCL_MOBDATA_H
#define INCL_MOBDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/LocalDefine.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Network/ChatMsgBuffer.h>
#include <Engine/Help/LoadLod.h>
#include <common/header/def_npc.h>

#define		NPC_SHOPPER				(1 << 0)	// �������ο���?
#define		NPC_FIRSTATTACK			(1 << 1)	// ��������?
#define		NPC_ATTACK				(1 << 2)	// �ݰݰ���?
#define		NPC_MOVING				(1 << 3)	// �̵�����?
#define		NPC_PEACEFUL			(1 << 4)	// ��ȭ���� ��(��Ŭ������ ���� �� ��ų��)
#define		NPC_ZONEMOVER			(1 << 5)	// �� �̵������ִ� NPC
#define		NPC_CASTLE_GUARD		(1 << 6)	// ��ȣ��
#define		NPC_REFINER				(1 << 7)	// ���ݼ��� NPC
#define		NPC_QUEST				(1 << 8)	// Quest NPC
#define		NPC_CASTLE_TOWER		(1 << 9)	// ������ ��ȣ��
#define		NPC_MINERAL				(1 << 10)	// �̳׶�
#define		NPC_CROPS				(1 << 11)	// ���۹�
#define		NPC_ENERGY				(1 << 12)	// ������
#define		NPC_ETERNAL				(1 << 13)	// ����
#define		NPC_LORD_SYMBOL			(1 << 14)	// ������ ����
#define		NPC_REMISSION			(1 << 15)	// ���˺�
#define		NPC_EVENT				(1 << 16)	// �̺�Ʈ
#define		NPC_GUARD				(1 << 17)	// ���
#define		NPC_KEEPER				(1 << 18)	// â��
#define		NPC_GUILD				(1 << 19)	// ���
#define		NPC_MBOSS				(1 << 20)	// �غ���
#define		NPC_BOSS				(1 << 21)	// ����
#define		NPC_RESETSTAT			(1 << 22)	// ���� �ʱ�ȭ
#define		NPC_CHANGEWEAPON		(1 << 23)	// ���� ��ü
#define		NPC_WARCASTLE			(1 << 24)	// ���� NPC
#define		NPC_DISPLAY_MAP			(1 << 25)	// ������ ǥ�õǴ��� ����
#define		NPC_QUEST_COLLECT		(1 << 26)	// ���� ����Ʈ��
#define		NPC_PARTY_MOB			(1 << 27)	// ��Ƽ�� ǥ��
#define		NPC_RAID				(1 << 28)	// ���̵� ��
#define		NPC_SUBCITY				(1 << 29)	// ���� ���� NPC			// WSS_ADD_FLAG 2007/08/09 ī������ ���� NPC
#define		NPC_CHAOCITY			(1 << 30)	// ī������ ���� ���� NPC
#define		NPC_HUNTERCITY			(1 << 31)	// ���͸��� ���� ���� NPC
// FLAG Extension	// 2009�� 2��
#define		NPC_AUCTION				(1 << 0)	// �ŷ� ���� NPC
#define		NPC_COLLSION_ENABLE		(1 << 1)	// ĳ������ �𵨿� �浹 flag���� ����
#define		NPC_FACTORY				(1 << 2)	// ������ ���� ���� NPC
#define		NPC_TRIGGER_CHOICE		(1 << 3)	// NPC�� ��ȭ�ϸ� Ʈ���� �ߵ�
#define		NPC_DONT_ATTACK				(1 << 6)	// Totem NPC
#define		NPC_AFFINITY			(1 << 7)	// ģȭ�� NPC
#define		NPC_SHADOW				(1 << 8)	// �׸��� ���� & NPC [2/11/2011 rumist]
#define		NPC_CRAFTING			(1 << 9)	// ����
#define		NPC_TOTEM_ITEM			(1 << 10)	// ���������� ��ȯ�� ���� NPC

#define		NPC_NO_SHADOW_CAST		(1 << 31)	// �׸��� ĳ���� ������.
#define		NPC_MASK				( NPC_ZONEMOVER | NPC_SHOPPER | NPC_REFINER | NPC_QUEST | NPC_REMISSION | NPC_EVENT | NPC_KEEPER | NPC_GUILD | NPC_RESETSTAT | NPC_CHANGEWEAPON | NPC_WARCASTLE | NPC_GUARD)
#define		NPC_RESOURCE_MASK		( NPC_MINERAL | NPC_CROPS | NPC_ETERNAL | NPC_ENERGY | NPC_QUEST_COLLECT)

enum eNpcAnimation
{
	NPC_ANIM_START = 0, 
	NPC_ANIM_WALK_1,
	NPC_ANIM_ATTACK_1,
	NPC_ANIM_ATTACK_2,
	NPC_ANIM_ATTACK_3,
	NPC_ANIM_ATTACK_4,
	NPC_ANIM_IDLE_1,	
	NPC_ANIM_IDLE_2,	
	NPC_ANIM_DIE,
	NPC_ANIM_DAMAGE,	
	NPC_ANIM_RUN_1,		
	NPC_ANIM_END = NPC_ANIM_RUN_1,
	NPC_ANIM_TOTAL,
};
// WSS_DRATAN_SEIGEWARFARE 2007/08/13 --------------->>
enum eSiegewarfareQuarters
{
	DRATAN_SIEGEWARFARE_QUARTER1 = 390,
	DRATAN_SIEGEWARFARE_QUARTER2,
	DRATAN_SIEGEWARFARE_QUARTER3,
	DRATAN_SIEGEWARFARE_QUARTER4,
	DRATAN_SIEGEWARFARE_QUARTER5,
	DRATAN_SIEGEWARFARE_QUARTER6,
	DRATAN_SIEGEWARFARE_QUARTER7,
	DRATAN_SIEGEWARFARE_QUARTER8,
	DRATAN_SIEGEWARFARE_QUARTER9,
	DRATAN_SIEGEWARFARE_QUARTER10,	
};

enum eSiegewarfareQuartersSetup
{
	QUARTER_NONE,
	QUARTER_MINE,
	QUARTER_INSTALL,	
	QUARTER_ATTACK,
};
// ---------------------------------------------------<<
class ENGINE_API CMobData : public stNpc, public LodLoader<CMobData>
{	
private:
	UBYTE	attrdef;
	UBYTE	attratt;
	UBYTE	attrdefLv;
	UBYTE	attrattLv;		

	std::string	name;
	std::string desc;
	int		transFlag;	//[sora] �̹��� ��Ʈ�� index ǥ��
private:
	ZONEFLAGS	mob_ulZoneFlag;
	ZONEFLAGS	mob_ulExtraFlag;

	char		motion_Attack3[DEF_ANI_LENGTH];
	char		motion_Attack4[DEF_ANI_LENGTH];
	BOOL		mob_bBoss;
		
	//---------------------------------------------
	// �Ǹ� ���� NPC
	INDEX		Owner_Index;
	short		channel_flag;
public:
	/* Default constructor. */
	CMobData(void);
	/* Destructor. */
	~CMobData(void);

	inline BOOL IsBoss()	const
	{	return mob_bBoss;		}

	BOOL IsNPC() const ;	// WSS_DRATAN_SEIGEWARFARE 2007/08/08 ������...

	int  IsMyQuarter() const ;	// WSS_DRATAN_SEIGEWARFARE 2007/08/13

	inline BOOL IsResource()	const
	{	return (flag & NPC_RESOURCE_MASK);		}

	inline BOOL	IsShopper()		const
	{ 
		return (flag & NPC_SHOPPER);	
	}	

	inline BOOL IsMovable()		const
	{
		return (flag & NPC_MOVING);
	}

	inline BOOL IsZoneMoving()	const
	{ 
		return (flag & NPC_ZONEMOVER);
	}

	inline BOOL IsCastleGuard() const
	{
		return (flag & NPC_CASTLE_GUARD);
	}

	inline BOOL IsLordSymbol() const
	{
		return (flag & NPC_LORD_SYMBOL);
	}

	inline BOOL IsCastleTower() const
	{
		return (flag & NPC_CASTLE_TOWER);
	}
		
	inline BOOL IsRefiner()		const
	{ 
		return (flag & NPC_REFINER); 
	}

	inline BOOL IsPeaceful()	const
	{
		return (flag & NPC_PEACEFUL); 
	}	

	inline BOOL IsQuest()		const			// ����Ʈ NPC
	{
		return (flag & NPC_QUEST);
	}
	
	inline BOOL IsSkillMaster() const			// ��ų ������
	{
		return (skillmaster != -1);
	}

	inline BOOL IsMineral()		const			// ����
	{
		return (flag & NPC_MINERAL);
	}
	
	inline BOOL IsCrops()		const			// �۹�
	{
		return (flag & NPC_CROPS);
	}

	inline BOOL IsEnergy()		const			// ������
	{
		return (flag & NPC_ENERGY);
	}

	inline BOOL IsCollect()		const			// ������
	{
		return (flag & NPC_QUEST_COLLECT);
	}

	inline BOOL IsEternal()		const			// ���� �ڿ�
	{
		return (flag & NPC_ETERNAL);
	}

	inline BOOL IsSSkillMaster()	const		//Ư�� ��ų ������.
	{
		return (sskillmaster != -1);
	}

	inline BOOL IsRemission()		const		// ���˺�
	{
		return (flag & NPC_REMISSION);
	}
	
	inline char GetSkillMaster()	const
	{
		return skillmaster;
	}

	inline char GetSpecialSkillMaster() const
	{
		return sskillmaster;
	}

	inline BOOL IsEvent()			const		// �̺�Ʈ
	{
		return (flag & NPC_EVENT);
	}

	inline BOOL IsGuard() const
	{
		return ( flag & NPC_GUARD );
	}

	inline BOOL IsWareHouse() const				// â��
	{
		return ( flag & NPC_KEEPER );
	}

	inline BOOL IsGuild() const					// ���
	{
		return ( flag & NPC_GUILD );
	}

	inline BOOL IsChangeWeapon() const			// ���� ��ȯ
	{
		return ( flag & NPC_CHANGEWEAPON );
	}

	inline BOOL IsCollectQuest() const			// ���� ����Ʈ NPC
	{
		return ( flag & NPC_QUEST_COLLECT );
	}

	inline BOOL IsPartyMob() const			// ��Ƽ�� 
	{
		return ( flag & NPC_PARTY_MOB );
	}

	inline BOOL IsWarCastle() const
	{
		return ( flag & NPC_WARCASTLE );
	}

	inline BOOL IsResetStat() const				// ���� �ʱ�ȭ
	{
		return ( flag & NPC_RESETSTAT );
	}

	inline BOOL IsChaoVill() const				// ī�� ����  // WSS_REMOVE_NAMEPOPUP 2007/08/14
	{
		return ( flag & NPC_CHAOCITY );
	}

	inline BOOL IsHunterVill() const				// ���� ���� // WSS_REMOVE_NAMEPOPUP 2007/08/14
	{
		return ( flag & NPC_HUNTERCITY );
	}

	inline BOOL IsAuctionChar() const				// �ŷ� ����NPC
	{
		return ( flag1 & NPC_AUCTION );
	}

	inline BOOL IsCollsionModel()					// �𵨿� �浹 ����� NPC
	{
		return ( flag1 & NPC_COLLSION_ENABLE);
	}

	inline BOOL IsMakeItemNPC()						// ������ ���� ���� NPC
	{
		return ( flag1 & NPC_FACTORY);
	}

	inline BOOL	IsAffinityNPC()						// ģȭ�� NPC
	{
		return ( flag1 & NPC_AFFINITY );
	}

	inline BOOL	IsDontAttack()					// Totem MOB
	{
		return ( flag1 & NPC_DONT_ATTACK );
	}
	
	inline BOOL	IsShadowNPC() const					// shadow type npc.
	{
		return ( flag1 & NPC_SHADOW );
	}

	inline BOOL IsCrafting() const
	{
		return ( flag1 & NPC_CRAFTING );
	}
	
	inline BOOL IsTotemItem() const
	{
		return ( flag1 & NPC_TOTEM_ITEM );
	}

	inline BOOL IsNPCFlag2nd(ULONG flag) const
	{
		return (flag1 & flag);
	}

	CTString	GetAnimAttackNameByID(int iAttackID);	
	inline void SetZoneMovingFlag(ZONEFLAGS ulZoneFlag)
	{	mob_ulZoneFlag = ulZoneFlag; };
	
	inline void SetExtraFlag(ZONEFLAGS ulExtraFlag)
	{	mob_ulExtraFlag = ulExtraFlag;	};

	inline int GetMobIndex()			const
	{	return index;}
	
	inline const char* GetMobSmcFileName()
	{	return fileSMC;}

	inline void SetMobSmcFileName(CTString MobName)
	{
		ZeroMemory(fileSMC, strlen(fileSMC));
		strcpy(fileSMC, MobName.str_String);
	}
	
	inline int GetLevel()				const
	{	return level;}
	
	inline float GetWalkSpeed()			const
	{	return walkSpeed;}

	inline float GetRunSpeed()			const
	{	return runSpeed;}

	inline float GetAttackDistance()	const
	{	return attackArea;	}
	
	inline float GetHealth()			const
	{	return hp;}
	
	inline int	GetMP()					const
	{	return mp;}

	inline int	GetAttackSpeed()		const
	{	return attackSpeed;	}

	inline float GetScale()				const
	{	return scale;}

	inline float GetSize()				const
	{	return size;}

	inline float GetScaledSize()		const
	{	return fBound * scale;}

	inline int	GetType()				const
	{	return flag;	}
	inline ZONEFLAGS GetZoneMovingFlag()const	
	{	return mob_ulZoneFlag; }

	inline ZONEFLAGS GetExtraFlag()		const
	{	return mob_ulExtraFlag;	};
	
	// NOTE : ���� �����Ϳ����� ������ �մϴ�.
	void SetName(const char* str)	{ name = str; }
	inline const char*	GetName()
	{	return name.c_str();}
	
	inline const char*	GetAnimWalkName()
	{	return motion_Walk;}
	
	inline const char* GetAnimRunName()
	{	return motion_Run;	}
	
	inline const char*	GetAnimAttackName()
	{	return motion_Attack;}
	
	inline const char*	GetAnimDefaultName()
	{	return motion_Idle;}
	
	inline const char*	GetAnimDeathName()
	{	return motion_Die;}
	
	inline const char*	GetAnimWoundName()
	{	return motion_Dam;}
	
	inline const char* GetNormalBoxName()
	{	return ("Default");}	

	inline const char* GetAnimAttack2Name()
	{	return motion_Attack2;}

	inline const char*	GetAnimDefault2Name()
	{	return motion_Idle2;}

	inline const char* GetAnimAttack3Name()
	{	return motion_Attack3;}

	inline const char* GetAnimAttack4Name()
	{	return motion_Attack4;}

	inline char GetAttackType() const
	{	return attackType;	}	

	inline char GetDelayCount()		const
	{	return fireDelayCount;	}

	inline float GetDelay(int i)		const
	{	return fireDelay[i];	}

	inline const char* GetFireEffect0()
	{	return fireEffect0;	}

	inline const char* GetFireEffect1()
	{	return fireEffect1;	}

	inline const char* GetFireEffect2()
	{	return fireEffect2;	}

	inline int GetMissileType()	const
	{	return (int)fireObject;	}	

	inline float GetMissileSpeed()	const
	{	return fireSpeed;	}

	inline int GetSkill0Index()	const
	{	return skill0_index;	}

	inline char	GetSkill0Level()	const
	{	return skill0_level;	}

	inline int GetSkill1Index()	const
	{	return skill1_index;	}

	inline char	GetSkill1Level()	const
	{	return skill1_level;	}

	inline void	SetOwnerIndex(INDEX Ownerindex)
	{
		Owner_Index = Ownerindex;
	}

	inline INDEX GetOwnerIndex()
	{
		return Owner_Index;
	}

	inline UBYTE GetAttratt() const
	{
		return attratt;
	}
	
	inline UBYTE GetAttrdef() const
	{
		return attrdef;
	}

	inline UBYTE GetAttrattLv() const
	{
		return attrattLv;
	}
	
	inline UBYTE GetAttrdefLv() const
	{
		return attrdefLv;
	}
	
	inline void	SetAttribute(UBYTE ubAttratt, UBYTE ubAttrdef, UBYTE ubAttrattLv, UBYTE ubAttrdefLv)
	{
		attratt = ubAttratt;
		attrdef = ubAttrdef;
		attrattLv = ubAttrattLv;
		attrdefLv = ubAttrdefLv;
	}

	int	GetSyndicateType()
	{
		return rvr_value;
	}

	int GetSyndicateGrade()
	{
		return rvr_grade;
	}

	float GetBound()
	{
		return fBound;
	}

	void SetChannelFlag(short channelFlag)	{ channel_flag = channelFlag;	}
	bool IsChannelFlag(int nChannel)	
	{ 
		if( channel_flag & ( 1 << (nChannel - 1) )) 
			return true;
		return false; 
	}

public:
	void SetNoTranslate();
	void ClearNoTranslate();

	int& GetTranslateFlag()		{ return transFlag; }
	void SetTranslateFlag(int flag);

	inline const char* GetDesc()
	{
		return desc.c_str();
	}
	
	void SetDesc(const char* strDesc) { desc = strDesc; }

	//-----------------------------------------------------------------------------
	// Purpose: NPC �����͸� �о�鿩�� MobData �迭�� �����մϴ�.
	// Input  : &apMobData - �����Ͱ� ����� �迭.
	//			FileName - �о���� ȭ�ϸ� MOB�����ȣ.lod
	// Output : int - ȭ�Ϸκ��� �о���� NPC �������� ������ ��ȯ�մϴ�.
	//-----------------------------------------------------------------------------
#if		(_MSC_VER <= 1200)
	
	static CMobData* getData(int idx) {
		
		std::map<int, CMobData*>::iterator it = _mapdata.find(idx);
		
		if (it != _mapdata.end())
			return (*it).second;
		
		return m_dummy;
	}
	
	static _map* getmapPtr() { return &_mapdata; } 

	static int getsize()	{ return (int)_mapdata.size(); }

	static CMobData* getDataSeq(int Idx)
	{
		if (Idx < 0 || Idx >= _vecdata.size())
			return m_dummy;
		
		return _vecdata[Idx];
	}

#endif	// (_MSC_VER <= 1200)

	static bool loadEx(const char* FileName);
	static bool LoadZoneData(const char* ZNFileName);
	static void SetSDNpc(); // �̱۴��� NPC ����
	static bool LoadChannelData(const char* FileName);

	static int SetMobDataToNPC(CEntity* penEntity, CMobData* MD, const char* szMobName);	
};

#endif