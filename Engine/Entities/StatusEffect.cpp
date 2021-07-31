#include "stdh.h"

#include "StatusEffect.h"
#include <Engine/Base/Timer.h>

/*
//skill.h���� ����.
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
#define EST_ASSIST_AURA_DARKNESS	40			// ������ - ��ũ�Ͻ�
#define EST_ASSIST_AURA_WEAKNESS	41			// ������ - ��ũ�Ͻ�
#define EST_ASSIST_AURA_ILLUSION	42			// ������ - �Ϸ���
#define EST_ASSIST_COUNT		43
*/
const char	*CStatusEffect::m_szStatusEffectName[EST_ASSIST_COUNT][T_COUNT] = 
{
	{NULL, NULL, NULL}//"STATE_POISON"
	, {NULL, NULL, NULL}//"STATE_HOLD"
	, {NULL, NULL, NULL}//"STATE_CONFUSION"
	, {"STATE_STONE", "STATE_STONE", "STATE_STONE"}
	, {"STATE_SILENT", "STATE_SILENT", "STATE_SILENT"}
	, {"croits_skill02_hits", "croits_skill02_hits", NULL}//"STATE_BLOOD"
	, {"STATE_BLIND", NULL, NULL}
	, {"STATE_STUN", "STATE_STUN", "STATE_STUN"}
	, {"STATE_SLEEP","STATE_SLEEP","STATE_SLEEP"}//"STATE_SLEEP"
	, {NULL, NULL, NULL}//"STATE_HP"
	, {NULL, NULL, NULL}//"STATE_MP"
	, {"DK Normal State", "DK Normal State", NULL}//"STATE_MOVESPD"
	, {NULL, NULL, NULL}//"HP_CANCEL"
	, {NULL, NULL, NULL}//"MP_CANCEL"
	, {"STATE_DIZZY", "STATE_DIZZY", "STATE_DIZZY"}
	, {NULL, NULL, NULL}//EST_ASSIST_INVISIBLE
	, {"TGT_MISTY_CHAIN", "TGT_MISTY_CHAIN", "TGT_MISTY_CHAIN"}//EST_ASSIST_SLOTH
	, {NULL, NULL, NULL}//EST_ASSIST_FEAR
	, {NULL, NULL, NULL}//EST_ASSIST_FAKEDEATH
	, {"STATE_PERFECTBODY", "STATE_PERFECTBODY", "STATE_PERFECTBODY"}
	, {"STATE_FRENZY", "STATE_FRENZY", "STATE_FRENZY"}
	, {"STATE_DAMAGELINK", "STATE_DAMAGELINK", "STATE_DAMAGELINK"}
	, {NULL, NULL, NULL}//EST_ASSIST_BERSERK
	, {NULL, NULL, NULL}//EST_ASSIST_DESPAIR
	, {NULL, NULL, NULL}//EST_ASSIST_MANA_SCREEN
	, {NULL, NULL, NULL}//EST_ASSIST_BLESS
	, {NULL, NULL, NULL}//EST_ASSIST_SAFEGUARD
	, {NULL, NULL, NULL}//EST_ASSIST_MANTLE
	, {NULL, NULL, NULL}//EST_ASSIST_GUARD
	, {NULL, NULL, NULL}//EST_ASSIST_CHARGEATC
	, {NULL, NULL, NULL}//EST_ASSIST_CHARGEMGC
	, {NULL, NULL, NULL}//EST_ASSIST_DISEASE
	, {NULL, NULL, NULL}//EST_ASSIST_CURSE
	, {NULL, NULL, "STATE_DIZZY"}//EST_ASSIST_CONFUSED
	, {NULL, NULL, "STATE_RED"}//EST_ASSIST_TAMING
	, {"TGT_COPE_OF_LIGHT1", "TGT_COPE_OF_LIGHT1", "TGT_COPE_OF_LIGHT1"}//EST_ASSIST_FREEZE
	, {NULL, NULL, NULL}//EST_ASSIST_INVERSE_DAMAGE
	, {"TGT_PAIN_SHOT", "TGT_PAIN_SHOT", "TGT_PAIN_SHOT"}//EST_ASSIST_HP_DOT
	, {NULL, NULL, NULL}//EST_ASSIST_REBIRTH
	, {"STATE_STONE", "STATE_STONE", NULL}//EST_ASSIST_DARKNESS_MODE
	, {"OWN_AURA_OF_DARKNESS", "OWN_AURA_OF_DARKNESS", NULL}//EST_ASSIST_AURA_DARKNESS				// ������ - ��ũ�Ͻ�
	, {"OWN_AURA_OF_ILLUSION", "OWN_AURA_OF_ILLUSION", NULL}//EST_ASSIST_AURA_WEAKNESS	41			// ������ - ��ũ�Ͻ�
	, {"OWN_AURA_OF_WEAKNESS", "OWN_AURA_OF_WEAKNESS", NULL}//EST_ASSIST_AURA_ILLUSION	42			// ������ - �Ϸ���
	, {NULL, NULL, NULL}//EST_ASSIST_MERCENARY	43
	, {NULL, NULL, NULL}//EST_ASSIST_SOUL_TOTEM_BUFF 44
	, {NULL, NULL, NULL}//EST_ASSIST_SOUL_TOTEM_ATTK 45
	, {NULL, NULL, NULL}//EST_ASSIST_TRAP 46
	, {NULL, NULL, NULL}//EST_ASSIST_PARASITE 47
	, {NULL, NULL, NULL}//EST_ASSIST_SUICIDE  48
};

const char	*CStatusEffect::m_szNPCQuestMark[CQuestSystem::NQT_COUNT] =
{
	NULL,
	"QMARK_HAVE_QUEST",
	"QMARK_CAN_PRIZE",
};


CStatusEffect::CStatusEffect()
: m_slCurrentStatus( 0 )
, m_eType( T_NPC )
, m_eCurrentNPCQuestType( CQuestSystem::NQT_NONE )
{
	ASSERT(EST_ASSIST_COUNT == 51);//���� �ٲ�� ������ m_szStatusEffectName�� ���� ���Ķ�-_-
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	for (int i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		m_pNPCQuestMark[i] = NULL;
	}
}

CStatusEffect::~CStatusEffect()
{
	Reset();
}

CStatusEffect::CStatusEffect(const CStatusEffect &other)
{
	int		i;

	m_slCurrentStatus = other.m_slCurrentStatus;
	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		m_pStatusEffectArray[i] = NULL;
	}

	m_eCurrentNPCQuestType = other.m_eCurrentNPCQuestType;
	
	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		m_pNPCQuestMark[i] = NULL;
	}
}

CStatusEffect &CStatusEffect::operator=(const CStatusEffect &other)
{
	int		i;

	m_slCurrentStatus = other.m_slCurrentStatus;
	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		m_pStatusEffectArray[i] = NULL;
	}

	m_eCurrentNPCQuestType = other.m_eCurrentNPCQuestType;
	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		m_pNPCQuestMark[i] = NULL;
	}
	return *this;
}

void CStatusEffect::Reset()
{
	int		i;

	m_slCurrentStatus = 0;
	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		if( m_pStatusEffectArray[i] != NULL
		&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
		{
			m_pStatusEffectArray[i]->Stop(0.5f);
		}
	}
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	m_eCurrentNPCQuestType = CQuestSystem::NQT_NONE;

	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		if(m_pNPCQuestMark[i]
		&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark[i]))
		{
			m_pNPCQuestMark[i]->Stop(0.5f);
		}

		m_pNPCQuestMark[i] = NULL;
	}
}

void CStatusEffect::Refresh(CTagManager *pTM, eRelation relation)
{
	int		i;

	for (i = 0; i < EST_ASSIST_COUNT; ++i)
	{
		if( m_pStatusEffectArray[i] != NULL
		&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
		{
			m_pStatusEffectArray[i]->Stop(0.5f);
		}
	}
	memset(m_pStatusEffectArray, 0, sizeof(CEffectGroup*)*EST_ASSIST_COUNT);

	for (i = 0; i < CQuestSystem::NQT_COUNT; ++i)
	{
		if(m_pNPCQuestMark[i]
		&& CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark[i]))
		{
			m_pNPCQuestMark[i]->Stop(0.5f);
		}

		m_pNPCQuestMark[i] = NULL;
	}

	if(pTM != NULL)
	{
		ChangeStatus(pTM, m_slCurrentStatus, relation);
		ChangeNPCQuestMark(pTM, m_eCurrentNPCQuestType);
	}
}

#define IS_FLAG_ON(flagAll, flagVal) ( (flagAll) & (static_cast<SQUAD>((1L))<<(flagVal)) )

void CStatusEffect::ChangeStatus(CTagManager *pTM, SQUAD newStatus, eRelation relation)
{
	for(INDEX i=0; i<EST_ASSIST_COUNT; ++i)
	{
		if(m_szStatusEffectName[i][m_eType] == NULL) continue;
		if(i == EST_ASSIST_DAMAGELINK && relation != R_PARTY)  continue;
		
		SQUAD tmpStatus = IS_FLAG_ON(m_slCurrentStatus, i);
		SQUAD tmpNewStatus = IS_FLAG_ON(newStatus, i);

		//Off -> On, Create Effect
		if( !IS_FLAG_ON(m_slCurrentStatus, i) && IS_FLAG_ON(newStatus, i))
		{
			ASSERT(m_pStatusEffectArray[i] == NULL);
			m_pStatusEffectArray[i] = StartEffectGroup(m_szStatusEffectName[i][m_eType], pTM, _pTimer->GetLerpedCurrentTick());
		}
		//On -> Off, Destroy Effect
		else if( IS_FLAG_ON(m_slCurrentStatus, i) && !IS_FLAG_ON(newStatus, i) )
		{
			if( m_pStatusEffectArray[i] != NULL
			&& CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i]) )
			{
				m_pStatusEffectArray[i]->Stop(0.1f);
			}
			m_pStatusEffectArray[i] = NULL;
		}
		//On -> On, Effect Life Time Refresh
		else if( IS_FLAG_ON(m_slCurrentStatus, i) && IS_FLAG_ON(newStatus, i) )
		{
			if( m_pStatusEffectArray[i] == NULL
				|| (
					m_pStatusEffectArray[i] != NULL
					&& !CEffectGroupManager::Instance().IsValidCreated(m_pStatusEffectArray[i])
				) )
			{
				m_pStatusEffectArray[i] = StartEffectGroup(m_szStatusEffectName[i][m_eType], pTM, _pTimer->GetLerpedCurrentTick());
			}
		}
	}
	m_slCurrentStatus = newStatus;
}

BOOL CStatusEffect::IsState(INDEX state)
{
	if(IS_FLAG_ON(m_slCurrentStatus, state)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsSturn()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_STURN)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsConfuse()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_CONFUSION)) return TRUE;
	else return FALSE;
}

BOOL CStatusEffect::IsHold()
{
	if(IS_FLAG_ON(m_slCurrentStatus, EST_ASSIST_HOLD)) return TRUE;
	else return FALSE;
}

void CStatusEffect::ChangeNPCQuestMark(CTagManager *pTM, CQuestSystem::eNpcQuestType newStatus)
{
	if(m_pNPCQuestMark[m_eCurrentNPCQuestType] && CEffectGroupManager::Instance().IsValidCreated(m_pNPCQuestMark[m_eCurrentNPCQuestType]))
	{
		m_pNPCQuestMark[m_eCurrentNPCQuestType]->Stop(0.f);
	}

	m_eCurrentNPCQuestType = newStatus;

	if(m_szNPCQuestMark[m_eCurrentNPCQuestType] != NULL)
	{
		m_pNPCQuestMark[m_eCurrentNPCQuestType] = StartEffectGroup(m_szNPCQuestMark[newStatus], pTM, _pTimer->GetLerpedCurrentTick() + 0.5f);
	}
}
