#ifndef INCL_SKILL_H
#define INCL_SKILL_H
#ifdef PRAGMA_ONCE
#pragma once
#endif

///////////
// ȿ�����

// ȿ�� ����Ÿ�� - ����
#define EST_STAT_ATTACK			0			// ����
#define EST_STAT_DEFENSE		1			// ���
#define EST_STAT_MAGIC			2			// ����
#define EST_STAT_RESIST			3			// ����
#define EST_STAT_HITRATE		4			// ����
#define EST_STAT_AVOID			5			// ȸ��
#define EST_STAT_CRITICAL		6			// ũ��
#define EST_STAT_ATTACKSPD		7			// ����
#define EST_STAT_MAGICSPD		8			// ����
#define EST_STAT_MOVESPD		9			// �̼�
#define EST_STAT_RECOVERHP		10			// HPȸ��
#define EST_STAT_RECOVERMP		11			// MPȸ��
#define EST_STAT_MAXHP			12			// �ִ�HP ����
#define EST_STAT_MAXMP			13			// �ִ�MP ����

// ȿ�� ����Ÿ�� - �����̻�
#define EST_ASSIST_POISON		0			// ��
#define EST_ASSIST_HOLD			1			// Ȧ��
#define EST_ASSIST_CONFUSION	2			// ȥ��
#define EST_ASSIST_STONE		3			// ��ȭ
#define EST_ASSIST_SILENT		4			// ����
#define EST_ASSIST_BLOOD		5			// ����
#define EST_ASSIST_BLIND		6			// ����ε�
#define EST_ASSIST_STURN		7			// ����
#define EST_ASSIST_SLEEP		8			// ����
#define EST_ASSIST_HP			9			// HPȸ��
#define EST_ASSIST_MP			10			// MPȸ��
#define EST_ASSIST_MOVESPD		11			// �̼� ���
#define EST_ASSIST_HP_CANCEL	12			// HP��ȭ(0, MAX���� ���)
#define EST_ASSIST_MP_CANCEL	13			// MP��ȭ(0, MAX���� ���)
#define EST_ASSIST_DIZZY		14			// ������
#define EST_ASSIST_INVISIBLE	15			// �κ�����
#define EST_ASSIST_SLOTH		16			// ���ν� : �����϶�
#define EST_ASSIST_FEAR			17			// ���� : �ش� NPC�� ����
#define EST_ASSIST_FAKEDEATH	18			// ����ô
#define EST_ASSIST_PERFECTBODY	19			// Ÿ��ź ���� �ٵ�
#define EST_ASSIST_FRENZY		20			// Ÿ��ź ������
#define EST_ASSIST_DAMAGELINK	21			// ��������ũ
#define EST_ASSIST_BERSERK		22			// �׷��� ������ �г�- ����
#define EST_ASSIST_DESPAIR		23			// ���� ���̾� ��ũ
#define EST_ASSIST_MANA_SCREEN	24			// �Ҽ��� ���� ��ũ��
#define EST_ASSIST_BLESS		25			// �Ҽ��� ���� ����
#define EST_ASSIST_SAFEGUARD	26			// ��ȣ��(�����Ұ���)
#define EST_ASSIST_MANTLE		27			// ����
#define EST_ASSIST_GUARD		28			// ��� ��ȯ
#define EST_ASSIST_CHARGEATC	29			// ��ź(attack)
#define EST_ASSIST_CHARGEMGC	30			// ��ź(magic)
#define EST_ASSIST_DISEASE		31			// ����
#define EST_ASSIST_CURSE		32			// ����
#define EST_ASSIST_CONFUSED		33			// ���͸� ȥ���� �ɸ��� ��
#define EST_ASSIST_TAMING		34			// ���͸� �����ð� �Ʊ����� ����
#define EST_ASSIST_FREEZE		35			// ���(���� ���� ����, �ɸ� ���� �̵� ����)
#define EST_ASSIST_INVERSE_DAMAGE	36		// �ǰ� ���ϸ� ������ ��ŭ HP ȸ��
#define EST_ASSIST_HP_DOT		37			// ���� ������ ����
#define EST_ASSIST_REBIRTH		38			// ��Ȱ
#define EST_ASSIST_DARKNESS_MODE	39		// ��ũ�Ͻ� ���
#define EST_ASSIST_AURA_DARKNESS	40		// ������ - ��ũ�Ͻ�
#define EST_ASSIST_AURA_WEAKNESS	41		// ������ - ��ũ�Ͻ�
#define EST_ASSIST_AURA_ILLUSION	42		// ������ - �Ϸ���
#define EST_ASSIST_MERCENARY		43		// ���� �뺴
#define EST_ASSIST_SOUL_TOTEM_BUFF	44		// ������ ����
#define EST_ASSIST_SOUL_TOTEM_ATTK	45		// ������ ����
#define EST_ASSIST_TRAP				46		// Ʈ��
#define EST_ASSIST_PARASITE			47		// ����� ���� ����
#define EST_ASSIST_SUICIDE			48		// �ڻ� ����
#define EST_ASSIST_INVINCIBILITY	49		// ���� ���
#define EST_ASSIST_GPS				50		// GPS
#define EST_ASSIST_COUNT			51

// ȿ�� ����Ÿ�� - ����
#define EST_ATTACK_NORMAL		0			// �Ϲ� ����

// ȿ�� ����Ÿ�� - ȸ��
#define EST_RECOVER_HP			0			// HPȸ��
#define EST_RECOVER_MP			1			// MPȸ��

// ȿ�� ����Ÿ�� - ġ��
#define EST_CURE_POISON			0			// ��
#define EST_CURE_HOLD			1			// Ȧ��
#define EST_CURE_CONFUSION		2			// ȥ��
#define EST_CURE_HURT			3			// �λ�
#define EST_CURE_SEAL			4			// ����
#define EST_CURE_BLOOD			5			// ����
#define EST_CURE_REBIRTH		6			// ��Ȱ
#define EST_CURE_INVISIBLE		7			// �κ����� ���¸� ����
#define EST_CURE_STURN			8			// ���� ����
#define EST_CURE_SLOTH			9			// ���ν� ����

// ȿ�� ����Ÿ�� - �׿�
#define EST_OTHER_INSTANTDEATH	0			// ���
#define EST_OTHER_SKILLCANCEL	1			// ��ų ���� ���
#define EST_OTHER_TACKLE		2			// ��Ŭ
#define EST_OTHER_TACKLE2		3			// ��Ŭ2
#define EST_OTHER_REFLEX		4			// �ݻ� ����� (�Ϲ� ����, 25m��)

// ����� Ÿ��
#define EDT_ONLYPOWER			0			// ��ų ���¸�
#define EDT_ADDITION			1			// ���ȿ� ����
#define EDT_RATE				2			// ���ȿ� ������ ����

// ���� Ÿ��
#define EHT_CONSTANT			0			// ����
#define EHT_VARIABLE			1			// ����

// ���� Ÿ��
#define ETT_IMMEDIATE			0			// �Ͻ�
#define ETT_DURABLE				1			// ����

////////////
// ��ų ���

// ���/���� ����
#define SCT_DEATH				(1 << 0)	// ���
#define SCT_WEAPON				(1 << 1)	// �������
#define SCT_SITDOWN				(1 << 2)	// �ɱ�
#define SCT_PEACEZONE			(1 << 3)	// ��ȭ����
#define SCT_SHIELD				(1 << 4)	// �ǵ����
#define SCT_STAND				(1 << 5)	// ����
#define SCT_DARKNESS			(1 << 6)	// �絵���
#define SCT_NOCOOLTIME			(1 << 7)	// [2012/11/20 : Sora] ��Ÿ�� ���� ���� ���� ����

// �÷���
#define	SF_SINGLEMODE			(1 << 0)	// �̱۸��
#define	SF_FORHELP				(1 << 1)	// ������ �ִ� ��ų
#define	SF_NOTHELP				(1 << 2)	// �ز��� ��ų
#define SF_ABSTIME				(1 << 3) // 060227 : bs : ����ð� ���
#define SF_NOTDUPLICATE			(1 << 4) // 060227 : bs : �ߺ� ��� �Ұ� : �ߺ��Ǵ� ��ų�� ������ ������ ������� ����� �ȵ�
#define SF_NOCANCEL				(1 << 5) // �ٸ� ��ų/����� ���� ��� �ȵ�
#define SF_COMBO				(1 << 6) // �ð�������� �����(��������, �����̵�, �׾��� �� �����)
#define SF_LOGOUT				(1 << 7) // �������� �� �����
#define SF_NS_DARKNESS			(1 << 8)    // ����Ʈ ������ �絵 ��忡���� ����ϴ� ��ų���� Ȯ��
#define SF_GUILD				(1 << 9)	// ��ų�� ��彺ų���� üũ �ϴ� flag
#define SF_INFINITE				(1 << 10)	// ���ѽ�ų - ��ų �ð��� ����Ǹ� �ٽ� ��ų�� �����
#define SF_ZONE					(1 << 11)	// ���̵��� ���� ����
#define SF_SUMMON_NPC			(1 << 12)	// ĳ���Ͱ� ��ȯ�� ����, Ʈ���� ����ϴ� ��ų
#define SF_ITEM_SPECIAL_SKILL	(1 << 14)	// �����ۿ� �ɼ����� �پ� �ִ� ��ų.
#define SF_NOT_SINGLE			(1 << 16)	// �̱� ���� ��� �Ұ� ��ų.
#define SF_TOGGLE				(1 << 17)	// ����� ��ų.

#define AST_SLOTH				(1 << MST_ASSIST_SLOTH)			// ���ν�(�����϶�)

// �Ҽ��� �÷���.
#define SSF_APP_CHARACTER		(1 << 0)	// ĳ����
#define SSF_APP_FIRE			(1 << 1)	// ��������
#define SSF_APP_WIND			(1 << 2)	// �ٶ�������
#define SSF_APP_EARTH			(1 << 3)	// ����������
#define SSF_APP_WATER			(1 << 4)	// ��������
#define SSF_APP_HELLOUND		(1 << 5)	// ����
#define SSF_APP_ELENEN			(1 << 6)	// ������
#define SSF_APP_ALL				(SSF_APP_CHARACTER | SSF_APP_FIRE | SSF_APP_WIND | SSF_APP_EARTH | SSF_APP_WATER | SSF_APP_HELLOUND | SSF_APP_ELENEN)
#define SSF_USE_CHARACTER		(1 << 16)	// ĳ����
#define SSF_USE_FIRE			(1 << 17)	// ��������
#define SSF_USE_WIND			(1 << 18)	// �ٶ�������
#define SSF_USE_EARTH			(1 << 19)	// ����������
#define SSF_USE_WATER			(1 << 20)	// ��������
#define SSF_USE_HELLOUND		(1 << 21)	// ����
#define SSF_USE_ELENEN			(1 << 22)	// ������
#define SSF_USE_ALL				(SSF_USE_CHARACTER | SSF_USE_FIRE | SSF_USE_WIND | SSF_USE_EARTH | SSF_USE_WATER | SSF_USE_HELLOUND | SSF_USE_ELENEN)

#define WEAPON_COUNT 2

#include <vector>
class  CSkill
{
public:
	struct _SkillData
	{
		std::string	name;						// �̸�
		std::string	client_description;
		std::string client_tooltip;

		int		index;							// ��ų ��ȣ
		int		job;							// ���� �÷���
		int		job2;							// ���� ����
		int		petindex;						// ���� �� �ε���		
		char	type;							// ����
		int		flag;							// �÷���
		int		sorcerer;						// �Ҽ��� ���� �÷���
		char	curLevel;						// WSS_NEW_GUILD_SYSTEM 070716
		char	maxLevel;						// �ִ� ����

		// �Ÿ�
		float	appRange;						// ���� ���� �ݰ�
		float	fireRange;						// �����Ÿ�
		float	fireRange2;						// �����Ÿ� 2

		// Ÿ��
		char	targetType;						// Ÿ���� ����

		// �������
		int		useState;						// �������1
		int		useWeaponType0;					// ��빫�⼭��Ÿ�� 1
		int		useWeaponType1;					// ��빫�⼭��Ÿ�� 2
		int		useMagicIndex1;					// �������2 - ������ȣ1
		char	useMagicLevel1;					// �������2 - ��������1
		int		useMagicIndex2;					// �������2 - ������ȣ2
		char	useMagicLevel2;					// �������2 - ��������2
		int		useMagicIndex3;					// �������2 - ������ȣ3
		char	useMagicLevel3;					// �������2 - ��������3
		int		useSoulCount;					// ������� (��ȥ�� ���� ����)

		// ��������
		int		appState;						// ��������1 - ĳ���� ����

		// �ð�
		int		readyTime;						// �����ð�
		int		waitTime;						// �����ð�
		int		fireTime;						// �߻�ð�
		int		reuseTime;						// ����ð�

		bool	bToggle;						// ��۽�ų Ȱ��ȭ ����.
		
		struct sClientSkillData
		{
			// ����
			char	readyAni[256];
			char	readyEffect1[256];

			// ����
			char	stillAni[256];

			// �߻�
			char	fireAni[256];
			char	fireEffect1[256];
			char	fireEffect2[256];
			char	fireEffect3[256];

			// �߻�ü
			char	fireobjType;
			float	fireobjSpeed;
			float	fireobjX;
			float	fireobjZ;
			float	fireobjH;
			char	fireobjCoord;
			char	fireobjDelayCount;
			float	fireobjDelay[4];
			float	fireobjDestDelay;
		};
		sClientSkillData client[WEAPON_COUNT];

		char	After_AttachEffect[256];		

		// ������
		int		client_icon_texid;
		int		client_icon_row;
		int		client_icon_col;

		DOUBLE	Skill_StartTime;

		int		transFlag;	//[sora] �̹��� ��Ʈ�� index ǥ��		
	};

	_SkillData				Skill_Data;

	struct _SkillLevel
	{
		// ���� �ʿ� ����
		int needHP;						// �Ҹ�HP
		int needMP;						// �Ҹ� MP
		int needGP;						// �Ҹ� GP

		int	durtime;					// ���ӽð�
		int dummyPower;					// ��¿� ����
		int needItemIndex1;					// �Ҹ������ ��ȣ1
		int needItemCount1;					// �Ҹ������ ����1
		int needItemIndex2;					// �Ҹ������ ��ȣ2
		int needItemCount2;					// �Ҹ������ ����2

		// ���� ����
		int learnLevel;						// �������� - ����
		int learnSP;						// �������� - SP
		int learnGP;						// �������� - GP //WSS_NEW_GUILD_SYSTEM 070716
		int learnSkillIndex[3];				// �������� - ��ų��ȣ
		char learnSkillLevel[3];			// �������� - ��ų����
		int learnItemIndex[3];
		int learnItemCount[3];
		int learnStr;						// �������� - ��
		int learnDex;						// �������� - ��ø 
		int learnInt;						// �������� - ����	
		int learnCon;						// �������� - ü��

		// ���� ����
		int appMagicIndex1;		// ��������2 - ȿ����ȣ1
		char appMagicLevel1;	// ��������2 - ȿ������1
		int appMagicIndex2;		// ��������2 - ȿ����ȣ2
		char appMagicLevel2;	// ��������2 - ȿ������2
		int appMagicIndex3;		// ��������2 - ȿ����ȣ3
		char appMagicLevel3;	// ��������2 - ȿ������3

		// ��ų ȿ��
		int magicIndex1;		// ��ųȿ�� ��ȣ
		char magicLevel1;		// ��ųȿ�� ����
		int magicIndex2;		// ��ųȿ�� ��ȣ
		char magicLevel2;		// ��ųȿ�� ����
		int magicIndex3;		// ��ųȿ�� ��ȣ
		char magicLevel3;		// ��ųȿ�� ����

		// �Ӽ� �ý��� ��ų LOD�� �Ӽ����� �߰� [1/21/2013 Ranma]
		char attratt;
		char attrattLv;
		char attrdef;
		char attrdefLv;
		int  targetmax;
	};
	
	std::vector<_SkillLevel>			m_vectorSkillLevels;
	BOOL								bCanCancel;
	BOOL								bNeedTarget;

	INDEX	idPlayer_Anim_Skill[WEAPON_COUNT][3];

	enum	SKILLTYPE
	{
		ST_MELEE				= 0,			// ����
		ST_RANGE				= 1,			// ���Ÿ�
		ST_MAGIC				= 2,			// ����
		ST_PASSIVE				= 3,			// �нú�
		ST_PET_COMMAND			= 4,			// �� �׼�
		ST_PET_SKILL_PASSIVE	= 5,			// �� ��ų 
		ST_PET_SKILL_ACTIVE		= 6,			// ��
		ST_GUILD_SKILL_PASSIVE	= 7,			// ��� ��ų �нú� // WSS_NEW_GUILD_SYSTEM 070716
		ST_SEAL					= 8,
		ST_SUMMON_TOTEM_SKILL	= 9,			// ���� ��ȯ ��ų
	};

	enum	SKILLPROPERTY
	{		
		// �Ӽ� ���
		AT_NONE		= 0,			// ��
		AT_FIRE		= 1,			// ȭ
		AT_WATER	= 2,			// ��
		AT_EARTH	= 3,			// ��
		AT_WIND		= 4,			// ǳ
		AT_DARK		= 5,			// ��
		AT_LIGHT	= 6,			// ��
	};

	enum	SKILLTARGET	//Skill_TargetType
	{		
		STT_SELF_ONE			= 0,			// ����
		STT_SELF_RANGE			= 1,			// ���� ����
		STT_TARGET_ONE			= 2,			// Ÿ��
		STT_TARGET_RANGE		= 3,			// Ÿ�� ����
		STT_PARTY_ONE			= 4,			// ��Ƽ 1��
		STT_PARTY_ALL			= 5,			// ��Ƽ ��ü
		STT_TARGET_D120			= 6,			// Ÿ�� 120��
		STT_TARGET_RECT			= 7,			// Ÿ�� ���簢
		STT_GUILD_ALL			= 9,			// �����ü���� �ִ� ��ų
		STT_GUILD_ONE			= 10,			// ����Ѹ��� �ִ� ��ų
		STT_GUILD_SELF_RANGE	= 11,			// ������ ���� �������� �ִ� ��ų
		STT_GUILD_MEMBER_SELF	= 12,			// ���� ��ü ��� ���� ��ų
		/*
		STT_SELF				= 0,			// ����
		STT_SELFRANGE_ENEMY		= 1,			// ���� ����(��)
		STT_TARGET_FRIEND		= 3,			// ���(�Ʊ�)
		STT_TARGET_ENEMY		= 4,			// ���(��)
		STT_TARGET_ENEMY_LOW	= 5,			// ���(�� & ����)
		STT_TARGETRANGE_ENEMY	= 6,			// ������(��)
		STT_PARTY				= 7,			// ��Ƽ
		*/
	};

	enum	STYLE
	{
		Damage      = 0,
		TACKLE		= 1,
		STERN		= 2,
		HOLD		= 3,
		SLEEP		= 4,
		ABILITY		= 5,
		USE_PC		= 6,
	};

	enum	CANUSESTATE
	{
		RUN         = 0,
		STAND	    = 1,
		SIT_DOWN    = 2,
		AFTERNOON   = 3,
		NIGHT		= 4,
		DEATH		= 5,
		SKILLING	= 6,
		SET_WEAPON	= 7,
		OFF_WEAPON	= 8,
		IN_WATER	= 9,
	};

//������ ���� ����	//(Open beta)(2004-12-03)
	//for client only
	enum MISSILE_TYPE
	{
		MT_NONE			= 0,//��������.
		MT_ARROW		= 1,//�Ϲ� ȭ���� �ְ� �߰��� Effect�� �ٴ� ����.
		MT_DIRECT		= 2,//�߻�ü�� ���� ����. �������� ���� �ʴ´�.
		MT_CONTINUE		= 3,//�Ⱥ��̴� �߻�ü�� Effect�� �ٴ� ����. ��, ���� ����Ʈ���� �߻�ü�� ����Ʈ�� �̾���.
		MT_INVISIBLE	= 4,//�Ⱥ��̴� �߻�ü�� Effect�� �ٴ� ����, �̾����� ����.
		MT_MAGIC		= 5,//�߻�ü�� �ִµ� �����ڿ��� ���۵��� ����.(ex:��Ƽ�� ��Ʈ����ũ)
		MT_INVERT		= 6,//������ ������������ ���� �����. Only Effect.
		MT_MAGECUTTER	= 7,//������ ��ų �� Ŀ�� ���󰡴°�.
		MT_DIRECTDAMAGE	= 8,//�߻�ü�� ���� ����. �������� �ش�.
		MT_NOTHING		= 9,//������ ����Ʈ ����. Network���� �޾Ƽ��� ó���ϴ� ��ų��. ex>��Ƽ������ ���.
		MT_GOWAITBACK	= 10,//�߻�ü �Ⱥ���. ��ǥ�� ���ٰ� �����ð� �� �ٽ� ���ƿ�.
							 //Ÿ�� ����Ʈ�� ��ǥ�� ���޽� ����. Ÿ������Ʈ�� '(�ٽ�)�� ���� ����Ʈ�� ���ƿ����� ����.
		MT_CONNECT		= 11,//�߻�ü �Ⱥ���. ��ǥ�� �����ڸ� mdl����Ʈ�� ������.(������ ��ũ)
		MT_FALLDOWN		= 12,//�߻�ü �Ⱥ���. �ϴÿ��� ������. ���� ���̸� ���� �� �����ǿ� ���޽� Ÿ������Ʈ ����.
		MT_DASH			= 13,//�Ⱥ��̴� �߻�ü�� Effect�� �ٴ� ����. ĳ���Ͱ� �������, HIT�� �ٽ� ��Ÿ ��(Worp) :: ����Ʈ( ��ų:�뽬 )
	};
//������ ���� ��	//(Open beta)(2004-12-03)

public:
	/* Default constructor. */
	CSkill(void);
	/* Destructor. */
	~CSkill(void);

	void Init(void);

	inline int GetIndex()		const
	{	return Skill_Data.index;	}

	inline int GetJob()			const
	{	return Skill_Data.job;	}

	inline int GetJob2()		const
	{	return Skill_Data.job2; }

	inline int GetWildpetIndex() const
	{	return Skill_Data.petindex;	}

	inline const char* GetName()		const
	{	return Skill_Data.name.c_str();	}

	inline int GetType()			const
	{	return Skill_Data.type;	}

	inline int GetFlag()		const
	{	return Skill_Data.flag;	}

	inline int GetSorcererFlag()	const
	{	return Skill_Data.sorcerer;	}

	inline int GetUsingSorcererFlag()	const
	{	return ( ( Skill_Data.sorcerer & 0xFFFF0000 ) >> 16 ); };

	inline int GetApplicationSorcererFlag()	const
	{	return ( ( Skill_Data.sorcerer & 0xFFFF ) ); };

	inline char GetTargetType()		const
	{	return Skill_Data.targetType;	}

	// ��빫�⼭��Ÿ�� 1
	inline int GetUseWeaponType0()	const			
	{	return Skill_Data.useWeaponType0;	}

	// ��빫�⼭��Ÿ�� 2
	inline int GetUseWeaponType1()	const
	{	return Skill_Data.useWeaponType1;	}

	// �ִ� Ÿ�� ��
	inline int GetTargetNum(int CurLv)	const
	{	
		if (CurLv - 1 < 0 || CurLv - 1 >= m_vectorSkillLevels.size())
			return 0;
		return m_vectorSkillLevels[CurLv-1].targetmax;
	}
	// ���緹��
	inline int GetCurLevel()	const
	{	return Skill_Data.curLevel;	}
	// �ִ뷹��
	inline int GetMaxLevel()	const
	{	return Skill_Data.maxLevel;	}

	inline float GetAppRange()		const
	{	return Skill_Data.appRange;	}

	inline float GetFireRange()		const
	{	return Skill_Data.fireRange;	}
	
	inline float GetFireRange2()	const
	{	return Skill_Data.fireRange2;	}

	inline int GetReadyTime()		const
	{	return Skill_Data.readyTime;	}

	inline int GetFireTime()		const
	{	return Skill_Data.fireTime;	}

	inline int GetReUseTime()		const
	{	return Skill_Data.reuseTime;	}

	inline float GetMissileSpeed(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].fireobjSpeed;	}

	inline char GetDelayCount(INDEX iWeapon)		const
	{	return Skill_Data.client[iWeapon].fireobjDelayCount;	}

	inline char GetDestDelay(INDEX iWeapon)		const
	{	return Skill_Data.client[iWeapon].fireobjDestDelay;	}

	inline float GetDelay(int i, INDEX iWeapon)		const
	{	return Skill_Data.client[iWeapon].fireobjDelay[i];	}

	inline MISSILE_TYPE GetMissileType(INDEX iWeapon)	const
	{	return (MISSILE_TYPE)(int)Skill_Data.client[iWeapon].fireobjType;	}

	inline bool IsSingleDungeonSkill() const
	{	return Skill_Data.flag & SF_SINGLEMODE;		}

	inline const char*	GetReadyAnim(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].readyAni;	}

	inline const char*	GetStillAnim(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].stillAni;	}

	inline BOOL IsNeedTarget() const
	{	return bNeedTarget;		}

	inline const char*	GetFireAnim(INDEX iWeapon)	const
	{	return Skill_Data.client[iWeapon].fireAni;	}

	inline const char* GetReadyEffect(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].readyEffect1;
	}

	inline const char* GetFireEffect1(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].fireEffect1;
	}

	inline const char* GetFireEffect2(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].fireEffect2;
	}

	inline const char* GetFireEffect3(INDEX iWeapon)	const
	{
		return Skill_Data.client[iWeapon].fireEffect3;
	}

	inline const char* GetAfter_AttachEffect() const
	{
		return Skill_Data.After_AttachEffect;
	}

	inline const char* GetDescription()		const
	{	return Skill_Data.client_description.c_str();	}

	inline const char* GetToolTip()		const
	{	return Skill_Data.client_tooltip.c_str();	}

	inline int GetNeedMP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].needMP;	}

	inline int GetNeedHP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].needHP;	}

	inline int GetNeedItemIndex1(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].needItemIndex1;	}

	inline int GetNeedItemIndex2(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].needItemIndex2;	}

	inline int GetLearnLevel(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].learnLevel;	}

	inline int GetLearnSP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].learnSP;	}
	// WSS_NEW_GUILD_SYSTEM 070716 ------------------->>
	inline int GetLearnGP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].learnGP;	}
	// -----------------------------------------------<<
	inline int GetLearnStr(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnStr;	}

	inline int GetLearnDex(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnDex;	}

	inline int GetLearnInt(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnInt;	}		
	
	inline int GetLearnCon(int iLevel) const
	{	return m_vectorSkillLevels[iLevel].learnCon;	}

	inline int GetDurTime(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].durtime;	}

	inline int GetPower(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].dummyPower; }

	inline int GetLearnSkillIndex( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnSkillIndex[iIndex];
	}
	inline SBYTE	GetLearnSkillLevel( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnSkillLevel[iIndex];
	}
	inline int	GetLearnItemIndex( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnItemIndex[iIndex];
	}
	inline int	GetLearnItemCount( int iLevel, int iIndex ) const
	{
		return m_vectorSkillLevels[iLevel].learnItemCount[iIndex];
	}

	inline int	GetIconTexID() const	{ return Skill_Data.client_icon_texid; }
	inline int	GetIconTexRow() const	{ return Skill_Data.client_icon_row; }
	inline int	GetIconTexCol() const	{ return Skill_Data.client_icon_col; }

	ENGINE_API void	SetStartTime( int nRemainTime = 0 );			// yjpark
	ENGINE_API void ResetStartTime();
	
	void SetStartSkillDelay(int nSkillIndex);

	// WSS_NEW_GUILD_SYSTEM 070716 ------------------->>
	inline void SetCurLevel(int iLevel)	
	{ Skill_Data.curLevel = iLevel;	}
	// -----------------------------------------------<<

	inline int GetNeedGP(int iLevel)	const
	{	return m_vectorSkillLevels[iLevel].needGP;	}
	inline int GetNeedItemCount1(int iLevel) const
	{
		return m_vectorSkillLevels[iLevel].needItemCount1;
	}
	inline int GetNeedItemCount2(int iLevel) const
	{
		return m_vectorSkillLevels[iLevel].needItemCount2;
	}


	// �Ӽ� �ý��� ��ų �Ӽ� ���� ��� [1/21/2013 Ranma]
	inline char GetAttrAtt(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attratt;	
	}

	inline char GetAttrAttLv(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attrattLv;
	}

	inline char GetAttrDef(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attrdef;	
	}

	inline char GetAttrDefLv(int iLevel)	const
	{
		if (iLevel < 0)
			return 0;

		return m_vectorSkillLevels[iLevel].attrdefLv;	
	}

	//[sora] �̹��� ��Ʈ�� index ǥ��
	void SetNoTranslate();
	void ClearNoTranslate();

	void SetName(const char* str)			{ Skill_Data.name = str; }
	void SetDescription(const char* str)	{ Skill_Data.client_description = str; }
	void SetTooltip(const char* str)		{ Skill_Data.client_tooltip = str; }

	int& GetTransFlag()						{ return Skill_Data.transFlag; }
	void SetToggle(bool bToggle)			{ Skill_Data.bToggle = bToggle;	}
	bool GetToggle()						{ return Skill_Data.bToggle;	}

	static int LoadSkillDataFromFile(CStaticArray<CSkill> &apSkillData, const char* FileName);
};

#endif