#include "stdh.h"

#include <Engine/Base/Console.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Network/MobTarget.h>
#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Network/CNetwork.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Ska/ModelInstance.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIFlowerTree.h>
#include <Engine/Info/MyInfo.h>

/*
 *  Constructor.
 */
CMobTarget::CMobTarget(void) 
{
	m_eType = eOBJ_MOB;

	m_nIdxServer = -1;
	m_nType = -1;                
	m_nIdxClient = -1;
	mob_bNPC = FALSE;
	m_yLayer = 0;
	mob_BuffCount = 0;
	m_pEntity = NULL;
	mob_pNormalEffect = NULL;
	mob_sbAttributePos	= MATT_UNWALKABLE;
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
	mob_Label = -1;	// [sora] ������ �ý��� ��� ǥ�� index
	mob_iOwnerIndex = 0;	// [2010/10/20 : Sora] ���� �뺴 ī��
	mob_iSubType	= MST_NONE;
	ChatMsg.Reset();
	mob_iSyndicateType = 0;
	mob_iSyndicateGrade = 0;
}

/*
 *  Destructor.
 */
CMobTarget::~CMobTarget(void) 
{
	CUIManager* pUIManager = CUIManager::getSingleton();	

	if(mob_pNormalEffect)
	{
		DestroyEffectGroupIfValid(mob_pNormalEffect);
		mob_pNormalEffect = NULL;
	}

	if(	m_pEntity != NULL &&
		(m_nType == 310 || m_nType == 311 ||
		 m_nType == 312 || m_nType == 313) )
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->DropDeathItem(m_pEntity);

	//�ش� NPC�� Ÿ�� ����Ʈ�� ����...
	pUIManager->StopTargetEffect( m_nIdxServer );

	if (m_pEntity != NULL)
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ClearTargetInfo(m_pEntity);

	if (m_pEntity != NULL)
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();

		m_pEntity->en_pMobTarget = NULL;
		if( m_pEntity == pInfo->GetTargetEntity(eTARGET_REAL) )
		{
			pInfo->TargetClear(eTARGET_REAL);
		}

		if( m_pEntity == pInfo->GetTargetEntity(eTARGET) )
		{
			pInfo->TargetClear(eTARGET);
		}

		for (int iBuff=0; iBuff < mob_BuffCount; ++iBuff)
		{
			mob_Buff[iBuff].Destroy_pEG();
		}

		m_pEntity->Destroy( FALSE );

		m_pEntity = NULL;
	}

	if (m_nType == MOB_FLOWERTREE_INDEX || m_nType == GAMIGO_10TH_CAKE)
	{ // �ɳ��� ���� ����Ʈ ���� ����
		pUIManager->GetFlowerTree()->ClearEffect();
	}
}

CMobTarget::CMobTarget(const CMobTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer = other.m_nIdxServer;
	m_nType = other.m_nType;
	m_nIdxClient = other.m_nIdxClient;
	mob_iLevel = other.mob_iLevel;
	mob_bNPC = other.mob_bNPC;
	m_pEntity = other.m_pEntity;
	m_strName	= other.m_strName;		
	mob_sbNameLength = other.mob_sbNameLength;
	m_yLayer = other.m_yLayer;
	mob_BuffCount = other.mob_BuffCount;
	memcpy(mob_Buff, other.mob_Buff, sizeof(mob_Buff[0])*20);
	//Hardcoding, status effect�� vtable�� �߰��ǰų� ����� �Ǵ� ��� ���� ����.
	memcpy(&mob_statusEffect, &other.mob_statusEffect, sizeof(mob_statusEffect));
	memset((void*)&other.mob_statusEffect, 0, sizeof(other.mob_statusEffect));
	mob_pNormalEffect = other.mob_pNormalEffect;
	mob_sbAttributePos	= other.mob_sbAttributePos;
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
	mob_Label = other.mob_Label; // [sora] ������ �ý��� ��� ǥ�� index
	mob_iOwnerIndex = other.mob_iOwnerIndex;	// [2010/10/20 : Sora] ���� �뺴 ī��
	mob_iSubType = other.mob_iSubType;
	ChatMsg = other.ChatMsg;
	SetSyndicateData(other.mob_iSyndicateType, other.mob_iSyndicateGrade);
}

CMobTarget &CMobTarget::operator=(const CMobTarget &other)
{
	m_eType = other.m_eType;
	m_nIdxServer = other.m_nIdxServer;
	m_nType = other.m_nType;
	m_nIdxClient = other.m_nIdxClient;
	mob_iLevel = other.mob_iLevel;
	mob_bNPC = other.mob_bNPC;
	m_pEntity = other.m_pEntity;	
	m_strName	= other.m_strName;	
	mob_sbNameLength = other.mob_sbNameLength;
	m_yLayer = other.m_yLayer;
	mob_BuffCount = other.mob_BuffCount;
	//Hardcoding, status effect�� vtable�� �߰��ǰ��� ����� �Ǵ� ��� ���� ����.
	memcpy(mob_Buff, other.mob_Buff, sizeof(mob_Buff[0])*20);
	memcpy(&mob_statusEffect, &other.mob_statusEffect, sizeof(mob_statusEffect));
	memset((void*)&other.mob_statusEffect, 0, sizeof(other.mob_statusEffect));
	mob_pNormalEffect = other.mob_pNormalEffect;
	mob_sbAttributePos	= other.mob_sbAttributePos;
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
	mob_Label = other.mob_Label; // [sora] ������ �ý��� ��� ǥ�� index
	mob_iOwnerIndex = other.mob_iOwnerIndex;	// [2010/10/20 : Sora] ���� �뺴 ī��
	mob_iSubType = other.mob_iSubType;
	ChatMsg = other.ChatMsg;
	SetSyndicateData(other.mob_iSyndicateType, other.mob_iSyndicateGrade);
	return *this;
}

//������ ���� ���� �̱� ���� �۾�	07.27
void CMobTarget::Init()
{
	if (m_pEntity != NULL)
	{
		ObjInfo* pInfo = ObjInfo::getSingleton();

		m_pEntity->en_pMobTarget = NULL;

		if( m_pEntity == pInfo->GetTargetEntity(eTARGET_REAL) )
		{
			pInfo->TargetClear(eTARGET_REAL);
		}

		if( m_pEntity == pInfo->GetTargetEntity(eTARGET) )
		{
			pInfo->TargetClear(eTARGET);
		}

		for (int iBuff=0; iBuff < mob_BuffCount; ++iBuff)
		{
			mob_Buff[iBuff].Destroy_pEG();
		}

		m_pEntity->Destroy( FALSE );
	}

	m_eType = eOBJ_MOB;
	m_nIdxServer = -1;
	m_nType = -1;                
	m_nIdxClient = -1;
	mob_bNPC = FALSE;
	m_yLayer = 0;
	mob_BuffCount = 0;
	m_pEntity = NULL;
	mob_statusEffect.Reset();
	mob_sbAttributePos = MATT_UNWALKABLE;
	m_strName	= CTString("");
	mob_Label = -1; // [sora] ������ �ý��� ��� ǥ�� index
	mob_iOwnerIndex = 0;	// [2010/10/20 : Sora] ���� �뺴 ī��
	mob_iSyndicateType = 0;
	mob_iSyndicateGrade = 0;
	if(mob_pNormalEffect)
	{
		DestroyEffectGroupIfValid(mob_pNormalEffect);
		mob_pNormalEffect = NULL;
	}
	mob_statusEffect.SetType(CStatusEffect::T_NPC);
	mob_iSubType = MST_NONE;
	ChatMsg.Reset();
}

void CMobTarget::Clear()
{
	int		i;

	for (i = 0; i < mob_BuffCount; ++i)
	{
		mob_Buff[i].Destroy_pEG();
		mob_Buff[i].Init();
	}
	
	mob_BuffCount = 0;
}

//������ ���� �� �̱� ���� �۾�		07.27
#include <Engine/Network/Server.h>	// TEST
void CMobTarget::SetData( INDEX index, INDEX type, CTString& strName, INDEX level, CEntity *pEntity,
							BOOL bNPC, SBYTE sbyLayer )
{
	m_nIdxServer = index;
	m_nType = type;    
	m_strName = strName;	
	mob_sbNameLength = m_strName.size();
	mob_iLevel = level;
	m_pEntity = pEntity;
	mob_bNPC = bNPC;
	m_yLayer = sbyLayer;
}

void CMobTarget::SetClientMobId(INDEX index)
{	
	m_nIdxClient = index;	
}

void CMobTarget::RemoveBuff( SLONG slItemIndex, SLONG slSkillIndex )
{
	int iBuff;
	for( iBuff = 0; iBuff < mob_BuffCount; iBuff++ )
	{
		if( mob_Buff[iBuff].m_slItemIndex == slItemIndex &&
			mob_Buff[iBuff].m_slSkillIndex == slSkillIndex )
			break;
	}

	ASSERT( iBuff < mob_BuffCount );
	if( iBuff == mob_BuffCount )
		return;
	
	mob_Buff[iBuff].Destroy_pEG();

	for( iBuff++; iBuff < mob_BuffCount; iBuff++ )
	{
		mob_Buff[iBuff - 1] = mob_Buff[iBuff];
	}

	mob_BuffCount--;
	mob_Buff[mob_BuffCount].Init();
}

void CMobTarget::BuffsStartGroupEffect(void)
{
	for (int iBuff=0; iBuff < mob_BuffCount; ++iBuff)
	{
		if (mob_Buff[iBuff].m_slSkillIndex > 0)
		{
			CEffectGroup* pEG = NULL;
			std::string strEffect = _pNetwork->GetSkillData(mob_Buff[iBuff].m_slSkillIndex).GetAfter_AttachEffect();
			if (strEffect.size() > 0 && m_pEntity != NULL)
			{
				if (mob_Buff[iBuff].m_pEG != NULL)
				{
					mob_Buff[iBuff].Destroy_pEG();
					mob_Buff[iBuff].m_pEG = NULL;
				}

				CEffectGroup* pEG = StartEffectGroup(strEffect.c_str(),
					&m_pEntity->en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

				mob_Buff[iBuff].m_pEG = pEG;
			}
		}
	}
}

void CMobTarget::SetSyndicateData( INDEX type, INDEX grade )
{
	mob_iSyndicateType = type;
	mob_iSyndicateGrade = grade;
}
