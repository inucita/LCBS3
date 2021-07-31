1511
%{
#include "StdH.h"
#include <Engine/Effect/CWorldTag.h>
#include <Engine/Effect/CEffectGroup.h>
#include <Engine/Effect/CEffectGroupManager.h>
#include <Engine/Effect/CSkaEffect.h>
//������ ���� ���� 1�� ���� ���� �۾�	08.20
#include <Engine/GlobalDefinition.h>
#include <Engine/GameState.h>
#include "EntitiesMP/Common/CharacterDefinition.h"
#include <Engine/JobInfo.h>
#include <Engine/TransformInfo.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/PetInfo.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Network/CharacterTarget.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/function/TitleData.h>
//������ ���� �� 1�� ���� ���� �۾�		08.20

// Check Attack Flag
#define ENF_EX2_PVP				(1L<<0)
#define ENF_EX2_LEGIT			(1L<<1)
#define ENF_EX2_MYPARTY			(1L<<2)
#define ENF_EX2_MYGUILD			(1L<<3)
#define ENF_EX2_ENEMYGUILD		(1L<<4)
#define ENF_EX2_WAR_OURFORCE	(1L<<5)
#define ENF_EX2_WAR_ENEMY		(1L<<6)
#define MODEL_TREASURE	("Data\\Item\\Common\\ITEM_treasure02.smc")

#ifdef CHAR_EX_ROGUE	// [2012/08/27 : Sora] EX�α� �߰�
	const BOOL charEXRogue = TRUE;
#else
	const BOOL charEXRogue = FALSE;
#endif
#ifdef CHAR_EX_MAGE		//2013/01/10 jeil EX������ �߰�
	const BOOL charEXMage = TRUE;
#else
	const BOOL charEXMage = FALSE;
#endif

%}

//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
uses "EntitiesMP/Missile";
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Global";
uses "EntitiesMP/BasicEffects";

uses "EntitiesMP/CharacterBase";
uses "EntitiesMP/BasicEffects";
uses "EntitiesMP/EnemyBase";
uses "EntitiesMP/Player";
uses "EntitiesMP/PetBase";
uses "EntitiesMP/WildPet";

event EChaAttackDamage {//0629
};
event EChaSkillDamage {
};

%{
	
// info structure
static EntityInfo eiChaBoss = {
	EIBT_FLESH, 200.0f,
	//0.0f, 1.75f*m_fStretch, 0.0f,     // source (eyes)
	//0.0f, 1.30f*m_fStretch, 0.0f,     // target (body)
	0.0f, 1.75f*1.0f, 0.0f,     // source (eyes)
	0.0f, 1.30f*1.0f, 0.0f,     // target (body)
};

//-----------------------------------------------------------------------------
// Purpose: �����̳� ���� ��� ��ƼƼ�� �ѹ��� �������� �ݴϴ�.
// Input  : pEntity - 
//			sTI - 
//			vCenter - 
// Output : static void
//-----------------------------------------------------------------------------
void DamagedTargetsInRange(CEntity* pEntity, CSelectedEntities& dcEntities, enum DamageType dmtType, FLOAT fDamageAmmount, FLOAT3D vCenter, BOOL bSkill);

//������ ���� ����	//(Open beta)(2004-12-08)
void ShotMissile(CEntity *penShoter, const char *szShotPosTagName
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szArrowEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 , INDEX ilArrowType = 0, const char *szMissileModel = NULL, CSelectedEntities* dcEntities = NULL);
void ShotMagicContinued(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed
				 , const char *szHitEffectName, const char *szMagicEffectName
				 , bool bCritical, INDEX iOrbitType, BOOL bDirectTag = FALSE);
void ShotGoWaitBack(CEntity *penShoter, FLOAT3D vStartPos, FLOATquat3D qStartRot
				 , CEntity *penTarget, FLOAT fMoveSpeed, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szMoveEffectName
				 , bool bCritical
				 , FLOAT fHorizonalOffset = 0.0f, FLOAT fVerticalOffset = 0.0f	//-1.0f ~ 1.0f
				 );
void ShotConnect(CEntity *penShoter, CEntity *penTarget, FLOAT fWaitTime
				 , const char *szHitEffectName, const char *szConnectEffectName);
//������ ���� ��	//(Open beta)(2004-12-08)

%}

class CCharacter: CCharacterBase {
name      "Character";
thumbnail "Mods\\test.tbn";

properties:
	//1 enum	ChaType m_ChaType "Type" 'Y' = CHA_BOSS,
	3 FLOAT m_fBodyAnimTime		= -1.0f,
	5 FLOAT m_fStretch			= 1.0f,
	//80 enum AttkType m_AttackType "Attack Type" 'A' = ATTK_NORMAL,
	91 INDEX m_ChaList			= TITAN,	
	93 FLOAT m_tmDireStrikeStart = 0.0f,

//	116 INDEX m_bKillEnemy = FALSE,

	150 CSoundObject m_soMessage,  // message sounds
	151 FLOAT	m_fScale = 1.0f,
	212 BOOL	m_bRide		= FALSE,		// ���� Ÿ�� �ִ°�?
	216 INDEX	m_iRideType	= -1,			// Ÿ�� �ִ� ���� Ÿ��(0�� ��� ��, 1�� ��� ��)
	217	BOOL	m_bWildRide = FALSE,		// �ӽ� Ÿ��. ���߿� �ý���ȭ�ÿ� �����Ұ�. [12/7/2010 rumist]

	223 BOOL	m_bPlayAction = FALSE,	
	225	FLOAT	m_fActionAnimTime = 0.0f,
	226	INDEX	m_nSitDown = 0,//2:�ɱ�, 0:����.

	227 INDEX idBodyAnim = 0,//1105
	228 INDEX m_nWearArmorUp = -1,
	229 INDEX m_nWearWeapon = -1,
	230 INDEX m_nWearArmorDown = -1,
	231 INDEX m_nWearShield = -1,
	232 INDEX m_nWearGlove = -1,
	233 INDEX m_nWearBoots	= -1,
	234 INDEX m_nWearHead	= -1,	//070223_ttos : ��� �߰�
	235 INDEX m_nWearWing	= -1,	// ����
	239 BOOL	m_bExtension	= FALSE,
	240 INDEX	m_nHairStyle = -1,		// ��� ��Ÿ��
	241 INDEX	m_nFaceStyle = -1,		// �� ��Ÿ��

	242 INDEX	m_nPkMode = 0,		//������ ���.
	243 INDEX	m_nPkTitle = 0,	

	244 BOOL m_bPlayProduce = FALSE,//1221
	245 INDEX	m_nPlayProduceNum = -1,
	246 FLOAT 	m_tmDamageTime = 0.0f,
	
	247 BOOL	m_bChangingTwice = FALSE,
	248 INDEX	m_nChangeMonsterIdTwice = -1,

	249 INDEX   m_nPkState = 0,
	250 INDEX   m_nLegit = 0,
	251 BOOL	m_bDie	= FALSE,
	252 BOOL	m_bDisappearWeapon = FALSE,
	253 BOOL    m_bChanging = FALSE,
	//254 BOOL    m_bMobChange = FALSE,
	255 INDEX   m_nChangeMonsterId = -1,
	201 BOOL	m_bTransforming = FALSE,
	202 INDEX	m_ChaState = 0,
	203 INDEX	m_iPolymophStep = 0,
	204 BOOL	m_bTransformMinimize = FALSE,
	 25 INDEX	m_iTransformType = 0,
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	{
		INDEX					m_nEffectStep;	
		CEffectGroup			*m_pSkillReadyEffect;
		CStaticArray<TagInfo>	m_saLastEffectInfo;

		INDEX	m_iSkillEffectStep;
		CStaticArray<FLOAT> m_afHardCodeValue;
		//BOOL	m_bHardcodingSkillLoop;		
		CCharacterAnimation		m_AnimationIndices;

		CModelInstance*		m_pRiderModelInstance;
		CModelInstance*		m_pRidePetModelInstance;
		CModelInstance*		m_pRideCharModelInstance;

		// �ӽ�.
		QVect mi_qvOffset;      // current offset from parent model instance
		INDEX mi_iParentBoneID; // ID of parent bone in parent model instance
		INDEX m_iPlaying_Ani_Attack;
		BOOL m_bReadyAction;
		BOOL m_bStillAction;
		BOOL m_bFireAction;
		BOOL m_bAttachPlayerEffect;
		BOOL m_bAttachEnemyEffect;
		CStaticArray<INDEX> m_iCostumeWear;
		
		INDEX		m_nWildPetType;			// 1 : human 2: beast 3 : deamon [3/21/2011 rumist]
	}
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)

components:	
	// ���� ó���� ���� �����Ǵ� �κ�.
	1 sound SOUND_POLYMOPH				"Data\\sounds\\character\\public\\C_transformation.wav",
	
	3 sound SOUND_DAIR_STRIKE_BLOW1		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow01.wav",
	4 sound SOUND_DAIR_STRIKE_BLOW2		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow02.wav",
	5 sound SOUND_DAIR_STRIKE_BLOW3		"Data\\Sounds\\Character\\rogue\\skill\\CRs_skill_06_blow03.wav",	
	7 sound SOUND_A_DAIR_STRIKE_BLOW1	"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow01.wav",
	8 sound SOUND_A_DAIR_STRIKE_BLOW2	"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow02.wav",
	9 sound SOUND_A_DAIR_STRIKE_BLOW3	"Data\\Sounds\\Character\\rogue\\skill\\CRs_a_skill_06_blow03.wav",

functions:

//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	BOOL AdjustShadingParameters(FLOAT3D &vLightDirection, COLOR &colLight, COLOR &colAmbient)
	{
		INDEX g_iNPCMinBrightness = _pShell->GetINDEX("g_iNPCMinBrightness");
		g_iNPCMinBrightness = Clamp(g_iNPCMinBrightness, (INDEX)0, (INDEX)240);
		INDEX g_iChaLightModifyValue = _pShell->GetINDEX("g_iChaLightModifyValue");

		BOOL bRecalcAmbient = FALSE;
		BOOL bRecalcDiffuse = FALSE;

		UBYTE ubAH, ubAS, ubAV;
		ColorToHSV(colAmbient, ubAH, ubAS, ubAV);
		UBYTE ubDH, ubDS, ubDV;
		ColorToHSV(colLight, ubDH, ubDS, ubDV);

		if(ubAV < g_iNPCMinBrightness)
		{
			UBYTE ubAR, ubAG, ubAB;
			ColorToRGB(colAmbient, ubAR, ubAG, ubAB);
			ubAR = g_iNPCMinBrightness;
			ubAG = g_iNPCMinBrightness;
			ubAB = g_iNPCMinBrightness;
			ColorToHSV(RGBToColor(ubAR, ubAG, ubAB), ubAH, ubAS, ubAV);
			bRecalcAmbient = TRUE;
		}
		if(ubAV < ubDV && g_iChaLightModifyValue < ubDV - ubAV)
		{
			INDEX value = (ubDV - ubAV - g_iChaLightModifyValue) >> 1;
			ubDV = Clamp(INDEX(ubDV) - value, INDEX(0), INDEX(255));
			ubAV = Clamp(INDEX(ubAV) + value, INDEX(0), INDEX(255));
			bRecalcAmbient = bRecalcDiffuse = TRUE;
		}

		if(bRecalcDiffuse) {colLight = HSVToColor(ubDH, ubDS, ubDV);}
		if(bRecalcAmbient) {colAmbient = HSVToColor(ubAH, ubAS, ubAV);}
		return TRUE;
	}
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
//������ ���� ����	//(Option)(0.1)
	void AdjustMipFactor(FLOAT &fMipFactor)
	{
		extern ENGINE_API FLOAT g_fChaLODMul;
		fMipFactor *= g_fChaLODMul;
	}
//������ ���� ��	//(Option)(0.1)
	virtual FLOAT GetLockRotationSpeed(void) 
	{ 
		return AngleDeg(1800.0f); 
	};

	// NOTE : �ֿϵ����� Ÿ�� ������ ������ �Ǵ� �κ��� �� �Լ��� ���ν��Ͻ��� ���ð�.
	CModelInstance* GetCurrentPlayerModelInstance()
	{
		CModelInstance *pMI = NULL;
		if( m_bRide || m_bWildRide)
		{
			CModelInstance *pMITemp = GetModelInstance();

			INDEX ctmi = pMITemp->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				pMI = &pMITemp->mi_cmiChildren[0];
			}
			else
			{
				pMI	= GetModelInstance();
			}
		}
		else
		{
			pMI	= GetModelInstance();
		}
		return pMI;
	};

	virtual void RidingPet(CEntity *pCharacter, INDEX iPetType )
	{
		SBYTE sbPetType	= iPetType & 0xFF;
		SBYTE sbPetColor =iPetType >>8;

		if( pCharacter->IsCharacter() && !m_bRide )
		{	
			mi_qvOffset			= pCharacter->GetModelInstance()->mi_qvOffset;
			mi_iParentBoneID	= pCharacter->GetModelInstance()->mi_iParentBoneID;			
			//DeleteModelInstance(m_pRiderModelInstance);

			FLOAT3D vPos;
			ANGLE3D vAngle;		
			//m_pRidePetModelInstance = ObtainModelInstance_t( PetInfo().GetFileName(sbPetType, 2) );
			m_pRiderModelInstance = ObtainModelInstance_t( PetInfo().GetFileName(sbPetType, 2) );


			// [070824: Su-won] PET_COLOR_CHANGE
			if( sbPetColor > 0 )
			{
				CTString strTexFile =PetInfo().GetColoredTexFileName(sbPetType, 2, sbPetColor);
				m_pRiderModelInstance ->mi_aMeshInst[0].mi_tiTextures[0].ti_toTexture.SetData_t( strTexFile );
			}

			QVect tmp_qvOfset;
			INDEX tmp_iParentBoneID;
			tmp_qvOfset = m_pRiderModelInstance->mi_qvRideOffset;
			tmp_iParentBoneID = m_pRiderModelInstance->mi_iRideParentBoneID;

			m_pRideCharModelInstance = CreateModelInstance( "" );
			m_pRideCharModelInstance->Copy( *pCharacter->GetModelInstance() );
			m_pRiderModelInstance->AddChild( m_pRideCharModelInstance, tmp_iParentBoneID );

			m_bRide		= TRUE;
			m_iRideType = sbPetType;

			pCharacter->GetModelInstance()->Copy(*m_pRiderModelInstance);		
			INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				CModelInstance& child = pCharacter->GetModelInstance()->mi_cmiChildren[0];
				child.mi_qvOffset = tmp_qvOfset;
			}

			pCharacter->GetModelInstance()->NewClearState(0.0f);
			ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();		
			if( ctmi > 0 )
			{
				CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];			
				//_pNetwork->MyCharacterInfo.itemEffect.Refresh(&cmi.m_tmSkaTagManager, 1);
				if( en_pCharacterTarget )
				{
					en_pCharacterTarget->cha_itemEffect.Refresh(&cmi.m_tmSkaTagManager, 1);
					en_pCharacterTarget->cha_statusEffect.Refresh(&cmi.m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}
			IdleAnim();
		}	
	}

	virtual void LeavingPet(CEntity *pCharacter )
	{
		if( pCharacter->IsCharacter() && m_bRide && !m_bWildRide)
		{
			//m_pRiderModelInstance->Clear();
			INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{		
				CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];						
 				m_pRideCharModelInstance->Copy(cmi);

				pCharacter->GetModelInstance()->m_tmSkaTagManager.ClearChild();
				pCharacter->GetModelInstance()->Copy( *m_pRideCharModelInstance );
				DeleteModelInstance(m_pRiderModelInstance);
				m_pRideCharModelInstance = NULL;
				m_pRiderModelInstance = NULL;
				pCharacter->GetModelInstance()->mi_qvOffset			= mi_qvOffset;
				pCharacter->GetModelInstance()->mi_iParentBoneID	= mi_iParentBoneID;
				pCharacter->GetModelInstance()->NewClearState(0.0f);
				//_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
				if( en_pCharacterTarget )
				{
					en_pCharacterTarget->cha_itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
					en_pCharacterTarget->cha_statusEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}
			m_bRide		= FALSE;
			m_iRideType = -1;
			IdleAnim();
		}
	}

	void InitBeastAnimation(void)
	{
		CJobInfo* pInfo = CJobInfo::getSingleton();

		if (pInfo == NULL)
		{
			return;
		}

		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_WALK_1] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_WALK_1));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_IDLE_1));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_2] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_IDLE_2));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1]	= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_RUN_1));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_PICK));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_DAMAGE));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE]	= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_DIE));
		m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_LEVELUP] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RIDE_HORSE_LEVELUP));
	}

	void SetBeastAnimation(CEntity *pWildPet)
	{
		if (pWildPet != NULL)
		{
			INDEX animationIdx = ANIM_RIDE_DEMONBAT_WALK_1;
			if( m_nWildPetType == 2 )
			{
				animationIdx = ANIM_RIDE_HORSE_WALK_1;
			}

			m_AnimationIndices.m_iAnimation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Walk;
			m_AnimationIndices.m_iAnimation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle1;
			m_AnimationIndices.m_iAnimation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle2;
			m_AnimationIndices.m_iAnimation[animationIdx++]	= ((CWildPet*)pWildPet)->iAnimWildPet_Run;
			m_AnimationIndices.m_iAnimation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle1;
			m_AnimationIndices.m_iAnimation[animationIdx++] = ((CWildPet*)pWildPet)->iAnimWildPet_Damage;
			m_AnimationIndices.m_iAnimation[animationIdx++]	= ((CWildPet*)pWildPet)->iAnimWildPet_Death;
			m_AnimationIndices.m_iAnimation[animationIdx] = ((CWildPet*)pWildPet)->iAnimWildPet_Idle1;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	virtual void RidingWildPet(CEntity *pCharacter, CEntity *pWildPet, CTString strFileName )
	{
		if( pCharacter->IsCharacter() && !m_bRide )
		{
			//(static_cast<CPlayerEntity*>(CEntity::GetPlayerEntity(0)))->ClearTargetInfo(pWildPet);
			//static_cast<CPlayerEntity*>(pCharacter)->ClearTargetInfo(pWildPet);
			// pet model copied from wildpet
			m_bRide = TRUE;
			m_bWildRide = TRUE;
			
			m_nWildPetType = pWildPet->en_pWildPetTarget->m_nType;
			SetBeastAnimation(pWildPet);

			m_pRidePetModelInstance	= ObtainModelInstance_t( strFileName );
			m_pRidePetModelInstance->Copy( *pWildPet->GetModelInstance() );

			mi_qvOffset			= pCharacter->GetModelInstance()->mi_qvOffset;
			mi_iParentBoneID	= pCharacter->GetModelInstance()->mi_iParentBoneID;

			// m_pRiderModelInstance -> SkaStudio���� �ֿϵ��� Ż �Ϳ� ĳ���͸� Child�� �ٿ����� ModelInstance

			QVect tmp_qvOfset;
			INDEX tmp_iParentBoneID;

 			tmp_qvOfset = m_pRidePetModelInstance->mi_qvRideOffset;
 			tmp_iParentBoneID = m_pRidePetModelInstance->mi_iRideParentBoneID;
			
			// ������ ��Ʈ�� �ϴ� �÷��̾� ĳ���͸� �ֿ� ���� Ż �Ϳ� �÷��̾� ĳ���͸� Child�� ���� �𵨷� ��ü��.
			// create empty model instance.
			m_pRideCharModelInstance = CreateModelInstance( "" );
			m_pRideCharModelInstance->Copy( *pCharacter->GetModelInstance() );
			m_pRidePetModelInstance->AddChild( m_pRideCharModelInstance, tmp_iParentBoneID );
			pCharacter->GetModelInstance()->Copy( *m_pRidePetModelInstance );

			INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				CModelInstance& child = pCharacter->GetModelInstance()->mi_cmiChildren[0];
				child.mi_qvOffset = tmp_qvOfset;
			}

			//pCharacter->GetModelInstance()->StopAllAnimations(0.0f);
			pCharacter->GetModelInstance()->NewClearState(0.0f);
			ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();		
			//NewClearState(0.2f);
			if( ctmi > 0 )
			{
				// ĳ���Ͱ� Child�� �ٿ��� �����̱� ������, ����Ʈ�� ǥ���� �±׸Ŵ����� Child�� ���� ����ϵ��� ��.
				CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];
				if( en_pCharacterTarget )			
				{
					en_pCharacterTarget->cha_itemEffect.Refresh(&cmi.m_tmSkaTagManager, 1);
					en_pCharacterTarget->cha_statusEffect.Refresh(&cmi.m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}		
			IdleAnim();
			
			pWildPet->Destroy();
		}

	}

	virtual void LeavingWildPet(CEntity *pCharacter )
	{
		if( pCharacter->IsCharacter() && m_bRide && m_bWildRide)
		{
			//m_pRiderModelInstance->Clear();
			INDEX ctmi = pCharacter->GetModelInstance()->mi_cmiChildren.Count();
			if( ctmi > 0 )
			{
				CModelInstance &cmi = pCharacter->GetModelInstance()->mi_cmiChildren[0];						
 				m_pRideCharModelInstance->Copy(cmi);

				pCharacter->GetModelInstance()->m_tmSkaTagManager.ClearChild();
				pCharacter->GetModelInstance()->Copy(*m_pRideCharModelInstance);
				DeleteModelInstance(m_pRidePetModelInstance);
				m_pRideCharModelInstance = NULL;
				m_pRidePetModelInstance = NULL;
				pCharacter->GetModelInstance()->mi_qvOffset			= mi_qvOffset;
				pCharacter->GetModelInstance()->mi_iParentBoneID	= mi_iParentBoneID;
				pCharacter->GetModelInstance()->NewClearState(0.0f);
				//_pNetwork->MyCharacterInfo.itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
				if( en_pCharacterTarget )
				{
					en_pCharacterTarget->cha_itemEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, 1);
					en_pCharacterTarget->cha_statusEffect.Refresh(&pCharacter->GetModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
				}
			}
			m_bRide		= FALSE;
			m_bWildRide = FALSE;
			InitBeastAnimation();
			IdleAnim();
		}
	}

	void OnInitialize(const CEntityEvent &eeInput)
	{
		CCharacterBase::OnInitialize(eeInput);
	};

	void OnEnd(void) 
	{
		CCharacterBase::OnEnd();		
	};	
	

	void CCharacter_OnInitClass(void)
	{
		CDLLEntityClass *pdec = &CCharacter_DLLClass;

		pdec->PrecacheSound(  SOUND_POLYMOPH );
	
		pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW1);
		pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW2);
		pdec->PrecacheSound( SOUND_DAIR_STRIKE_BLOW3);
		
		pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW1);
		pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW2);
		pdec->PrecacheSound( SOUND_A_DAIR_STRIKE_BLOW3);

	}

	//--------------------------------------------------------------
	// ĳ������ �ִϸ��̼� �̸� ����...
	//--------------------------------------------------------------

	void SetAnimation(INDEX iType)
	{
		for( int i = ANIM_START; i <= ANIM_END; ++i )
		{
			m_AnimationIndices.m_iAnimation[i] = ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName( iType, i ));
		}		
	}

	// describe how this enemy killed player
	virtual CTString GetPlayerKillDescription(const CTString &strPlayerName, const EDeath &eDeath)
	{
		CTString str;
		str.PrintF(TRANS("A Orcman sent %s into the halls of Valhalla"), strPlayerName);
		return str;
	}

	/* Entity info */
	void *GetEntityInfo(void)
	{
		return &eiChaBoss;	
	};

	void Precache(void) 
	{
		CCharacterBase::Precache();
//������ ���� ����	//(Fix Snd Effect Bug)(0.1)
		if(en_pmiModelInstance) { en_pmiModelInstance->m_tmSkaTagManager.SetOwner(this); }
//������ ���� ��	//(Fix Snd Effect Bug)(0.1)
	};

//������ ���� ���� ���� ���� �۾�	08.27
	virtual BOOL IsCharacter() const {return TRUE;};
//������ ���� �� ���� ���� �۾�		08.27

	void InflictDirectDamage(CEntity *penToDamage, CEntity *penInflictor, enum DamageType dmtType,
	FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection)
	{
		if(penToDamage) 
		{
			if( !(penToDamage->GetFlags()&ENF_ALIVE ) )	{ return; }
			SE_Get_UIManagerPtr()->ShowDamage( penToDamage->en_ulID );
			INDEX preHealth, curHealth;
			preHealth = ((CUnit*)penToDamage)->m_nPreHealth;
			curHealth = ((CUnit*)penToDamage)->m_nCurrentHealth;

			if(preHealth!= -1 || penToDamage->IsPlayer())// && curHealth > preHealth)
			{
				fDamageAmmount = 1;
				((CUnit*)penToDamage)->m_nCurrentHealth = ((CUnit*)penToDamage)->m_nPreHealth;
			}
			else 
			{
				fDamageAmmount = 0;
			}

			if(penToDamage ==INFO()->GetTargetEntity(eTARGET))
			{
				if( penToDamage->IsCharacter() )
				{
					penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth,((CCharacter*) penToDamage)->m_nPkMode,((CCharacter*) penToDamage)->m_nPkState,((CCharacter*) penToDamage)->m_nLegit);		
				}
				else
				{
					penToDamage->UpdateTargetInfo(((CUnit*) penToDamage)->m_nMaxiHealth,((CUnit*) penToDamage)->m_nCurrentHealth);
				}
			}
	
			if (penToDamage->IsEnemy() && preHealth >= 0) // Character�� Enemy(����)�� ������ ��찡 �ִ�. ������ �� npc�� ������ �� ���� �ٲ�� �ϳ�?
			{ // ������ HP�� �������� �׶� Update�ϸ� ���� �ʴ°�?
				const INDEX iMobIndex = ((CEnemy*)penToDamage)->m_nMobDBIndex;

				if (iMobIndex == LORD_SYMBOL_INDEX) // �޶�ũ ���� ���� NPC(������� �ٲ��� �ʰ�, ���� �ٲ۴�)
				{
					CMobData* MD = CMobData::getData(iMobIndex);
					FLOAT fMaxHealth = ((CUnit*)penToDamage)->m_nMaxiHealth;

					if ((curHealth <= fMaxHealth * 0.25f))
					{
						if (((CUnit*)penToDamage)->m_nPlayActionNum != 0)
						{
							penToDamage->SetSkaModel("data\\npc\\Gguard\\sword04.smc");
							penToDamage->GetModelInstance()->RefreshTagManager();
							penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
							((CUnit*)penToDamage)->m_nPlayActionNum = 0;
						}
					}
					else if ((curHealth <= fMaxHealth * 0.50f))
					{
						if (((CUnit*)penToDamage)->m_nPlayActionNum != 1)
						{
							penToDamage->SetSkaModel("data\\npc\\Gguard\\sword03.smc");
							penToDamage->GetModelInstance()->RefreshTagManager();
							penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
							((CUnit*)penToDamage)->m_nPlayActionNum = 1;
						}
					}
					else if ((curHealth <= fMaxHealth * 0.75f))
					{
						if (((CUnit*)penToDamage)->m_nPlayActionNum != 2)
						{
							penToDamage->SetSkaModel("data\\npc\\Gguard\\sword02.smc");
							penToDamage->GetModelInstance()->RefreshTagManager();
							penToDamage->GetModelInstance()->StretchModel(FLOAT3D(MD->GetScale(), MD->GetScale(), MD->GetScale()));
							((CUnit*)penToDamage)->m_nPlayActionNum = 2;
						}
					}
				}
			}

			((CUnit*)penToDamage)->m_nPreHealth = -1; //1103
		}

		CEntity::InflictDirectDamage(penToDamage, penInflictor, dmtType,
			fDamageAmmount,vHitPoint, vDirection);

		if(penToDamage )
		{
			if(((CUnit*)penToDamage)->m_nCurrentHealth <= 0 && penToDamage != this)
			{
				((CUnit*)((CEntity*)penToDamage))->DeathNow();
			}
		}
	};

	//--------------------------------------------------------------
	// �������� �Ծ��� ����� ó��...
	//--------------------------------------------------------------
	/* Receive damage */
	void ReceiveDamage(CEntity *penInflictor, enum DamageType dmtType,
		FLOAT fDamageAmmount, const FLOAT3D &vHitPoint, const FLOAT3D &vDirection) 
	{
		if(dmtType != DMT_NONE && fDamageAmmount > 0)
		{
			FLOAT3D vAxisY(0,1,0);
			FLOAT angle = acos(vDirection % vAxisY);
			FLOAT3D axis = vAxisY * vDirection;
			axis.Normalize();
			FLOATquat3D quat;
			quat.FromAxisAngle(axis, angle);

			// �̺κ��� �ٰŸ� ������ ��쿡 �ش��ϴ� �κ�.

			INDEX iHitEffect = 0;
			INDEX iJob = 0;

			ObjectBase* pObj = ACTORMGR()->GetObject(eOBJ_CHARACTER, m_enAttackerID);
			
			if (pObj != NULL)
			{
				CCharacterTarget* pCT = static_cast<CCharacterTarget*>(pObj);
	
				if (pCT != NULL)
				{
					iHitEffect = pCT->cha_iHitType;
				}
	
				iJob = pObj->GetType();
			}
	
			if (iHitEffect < 0 || iHitEffect > 2)
			{
				iHitEffect = 0;
			}

			StartEffectGroup(szHitEffect[iJob][iHitEffect], _pTimer->CurrentTick(), vHitPoint, quat);		//Hardcording
		}
		SE_Get_UIManagerPtr()->ShowDamage( en_ulID );

		CCharacterBase::ReceiveDamage(penInflictor, dmtType, fDamageAmmount, vHitPoint, vDirection);

	};

	//--------------------------------------------------------------
	// ĳ������ �ִϸ��̼��� �����ϴ� �Լ���...
	//--------------------------------------------------------------
	// damage anim
	INDEX AnimForDamage(FLOAT fDamage) 
	{
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_DAMAGE]		!= -1	&& "Invalid Animation");
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_EXT_DAMAGE]	!= -1	&& "Invalid Animation");
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		INDEX iDamageAnimation = -1;

		if( !m_bExtension || m_bMobChange )
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE];	
					}
					else
					{
						iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_DAMAGE];
					}
				}
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE];
				}
				else
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DAMAGE];
				}
			}
			else if (m_ChaState & PLAYER_STATE_FLYING)
			{
				iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_FLYING_READY];
			}
			else
			{
				iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_DAMAGE];			
			}
		}
		else
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE];	
					}
					else
					{
						iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_DAMAGE];
					}
				}
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DAMAGE];
				}
				else
				{
					iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DAMAGE];
				}
			}
			else if (m_ChaState & PLAYER_STATE_FLYING)
			{
				iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_FLYING_READY];
			}
			else
			{
				iDamageAnimation = m_AnimationIndices.m_iAnimation[ANIM_EXT_DAMAGE];			
			}
		}

		//GetModelInstance()->AddAnimation(iDamageAnimation,AN_CLEAR,1,0);
		AddAnimation(iDamageAnimation, AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			if (!(m_ChaState & PLAYER_STATE_FLYING))
			{
				AppearWearingWeapon(FALSE);
				m_bDisappearWeapon = FALSE;
			}
		}
		m_tmDamageTime = _pTimer->CurrentTick();		

		return iDamageAnimation;
	};

	// death
	INDEX AnimForDeath(void)
	{	
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_DIE]		!= -1	&& "Invalid Animation");
		ASSERT(m_AnimationIndices.m_iAnimation[ANIM_EXT_DIE]	!= -1	&& "Invalid Animation");		
		ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			AppearWearingWeapon(FALSE);
			m_bDisappearWeapon = FALSE;
		}

		if(m_bMobChange)
		{
			ReturnChange(TRUE);
		}
		else if( m_bIsTransform )
		{
			ReturnSorcerer(TRUE);
		}

		m_bConnectEffect = FALSE;
		m_pSkillTexture.SetData(NULL);
		StopEffectGroupIfValid(m_pAttachPlayerEffect, 0.1f);
		StopEffectGroupIfValid(m_pAttachEnemyEffect, 0.1f);		

		GetModelInstance()->StopAllAnimEffect(0);
		GetModelInstance()->mi_aqAnims.aq_Lists.Clear();

		INDEX iDieAnimation = -1;

		if( !m_bExtension || m_bMobChange )
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE];	
					}
					else
					{
						iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_DIE];
					}
				}
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE];
				}
				else
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DIE];
				}				
			}
			else
			{
				iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_DIE];
			}
		}
		else
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE];	
					}
					else
					{
						iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_DIE];
					}
				}
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_DIE];
				}
				else
				{
					iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_DIE];
				}
			}
			else
			{
				iDieAnimation = m_AnimationIndices.m_iAnimation[ANIM_EXT_DIE];
			}
		}

		AddAnimation(iDieAnimation, AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(iDieAnimation, 0, 1, 0);
		return iDieAnimation;
	};

	// ������ ������...
	FLOAT WaitForDust(FLOAT3D &vStretch)
	{
		//0317 kwon
		vStretch=FLOAT3D(1,1,1);		
		if( GetModelInstance()->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_DIE]) || 
			GetModelInstance()->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_EXT_DIE]) ) 
		{
			return 0.5f;
		} 
		return -1.0f;
	};

	void DeathNotify(void) 
	{
		// ���� ����...
		en_fDensity = 500.0f;
	};

	// Idle Animation
	void IdleAnim(void)
	{
		CModelInstance *pMI = GetModelInstance();
		INDEX iIdleAnimation = -1;
		if(m_nWearWeapon != -1) //���⸦ ��� �ִٸ�,
		{
			if(m_bExtension)
			{
				if( m_bRide )
				{
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_2];	
						}
						else
						{
							iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_IDLE_2];
						}
					}
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_2];
					}
					else
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_IDLE_2];
					}					
				}
				else if (m_ChaState & PLAYER_STATE_FLYING)
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_FLYING_READY];
				}
				else
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_IDLE];
				}
			}
			else
			{
				if( m_bRide )
				{
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1];	
						}
						else
						{
							iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_IDLE_1];
						}
					}
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1];
					}
					else
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_IDLE_1];
					}					
				}
				else if (m_ChaState & PLAYER_STATE_FLYING)
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_FLYING_READY];
				}
				else
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE];
				}
			}
		}
		else
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1];	
					}
					else
					{
						iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_IDLE_1];
					}
				}
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_IDLE_1];
				}
				else
				{
					iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_IDLE_1];
				}
			}
			else if (m_ChaState & PLAYER_STATE_FLYING)
			{
				iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_FLYING_READY];
			}
			else
			{
				iIdleAnimation = m_AnimationIndices.m_iAnimation[ANIM_IDLE];
			}
		}
		
		if (!IsAnimationPlaying(iIdleAnimation))
		{
			AddAnimation(iIdleAnimation, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		}

		//pMI->AddAnimation(iIdleAnimation,AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
	};

	// virtual anim functions
	void StandingAnim(void) 
	{
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_IDLE]	!= -1	&& "Invalid Animation");
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE]	!= -1	&& "Invalid Animation");
		//ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		CModelInstance *pMI = GetModelInstance();
/*
		if(m_fScale != 1.0f)
		{
			pMI->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
			m_fScale = 1.0f;
		}
*/
		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			if (!(m_ChaState & PLAYER_STATE_FLYING))
			{
				AppearWearingWeapon(FALSE);
				m_bDisappearWeapon = FALSE;
			}
		}

		if(m_nSitDown == 2)
		{
			INDEX iAnim = -1;
			if( m_bRide )
			{				
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT_CONTINUE];
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT_CONTINUE];
				}

			}
			else
			{
				iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT_CONTINUE];				
			}
			if(m_ChaList == NIGHTSHADOW )
			{
				DeleteWearingWeapon(FALSE,m_bSkilling);
			}
			AddAnimation(iAnim, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);			
			return;
		}
		IdleAnim();
		
	};
	void RunningAnim(void) 
	{
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_RUN_1]		!= -1	&& "Invalid Animation");
		//ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");

		FLOAT fRunSpeedMul =1.0f;

		CModelInstance *pMI = GetModelInstance();
/*	
		if(m_fScale != 1.0f)
		{
			pMI->StretchModel(FLOAT3D(1.0f,1.0f,1.0f));
			m_fScale = 1.0f;
		}
*/
		if(m_bSpeedUp)
		{
			// FIXME : ��� ���� ������ �𸣰���.
			if(m_ChaList == TITAN)
			{
				fRunSpeedMul = 8.0f/6.2f;
			}
			else if(m_ChaList == HEALER)
			{
				fRunSpeedMul = 8.0f/7.0f;
			}
			else if(m_ChaList == KNIGHT)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(m_ChaList == MAGE)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(m_ChaList == ROGUE)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(m_ChaList == SORCERER)
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(IsEXRogue(m_ChaList))	// [2012/08/27 : Sora] EX�α� �߰�
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
			else if(IsEXMage(m_ChaList))	//2013/01/10 jeil EX������ �߰�
			{
				fRunSpeedMul = 8.0f/6.0f;
			}
		}

		if(IsFlagOn(ENF_HIDDEN) && m_bHardcodingSkillLoop && (_pTimer->GetLerpedCurrentTick() - m_tmDireStrikeStart > 1.7f))
		{
			// NOTE : �����϶� Invisible�� Ǯ���� �κ� ����.

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				if(pTarget->cha_statusEffect.IsState(EST_ASSIST_INVISIBLE) && _pNetwork->m_ubGMLevel < 2)
				{
					SetFlagOn(ENF_HIDDEN);
				}
				else
				{                                                                                                                                                                                                           
					SetFlagOff(ENF_HIDDEN);
				}
			}
		}

		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			if (!(m_ChaState & PLAYER_STATE_FLYING))
			{
				AppearWearingWeapon(FALSE);
				m_bDisappearWeapon = FALSE;
			}
		}
		if(m_nWearWeapon != -1 && !m_bMobChange) //���⸦ ��� �ִٸ�,//1008
		{
			if( !m_bExtension )
			{
				if( m_bRide )
				{
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
						}
						else
						{
							AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
						}
					}
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
					else
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
				}
				else if (m_ChaState & PLAYER_STATE_FLYING)
				{
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
				else
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_2],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_2], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
			}
			else
			{
				if( m_bRide )
				{
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
						}
						else
						{
							AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
						}
					}
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
					else
					{
						//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
				}
				else if (m_ChaState & PLAYER_STATE_FLYING)
				{
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
				else
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_EXT_RUN_2],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_EXT_RUN_2], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
			}
		}
		else
		{
			if( m_bRide )
			{
				if( m_bWildRide )
				{
					if( m_nWildPetType == 2 )
					{
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
					else
					{
						AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
					}
				}
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				else if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1, ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
				else
				{
					//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1, ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
				}
			} 
			else if (m_ChaState & PLAYER_STATE_FLYING)
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
			}
			else
			{
				//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1, ESKA_MASTER_MODEL_INSTANCE,fRunSpeedMul);
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RUN_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, fRunSpeedMul);
			}
		}
	};
	void WalkingAnim(void) 
	{
		//ASSERT(m_AnimationIndices.m_iAnimation[ANIM_WALK_1]		!= -1	&& "Invalid Animation");
		//ASSERT(GetModelInstance() != NULL && "Invalid ModelInstance");
		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			if (!(m_ChaState & PLAYER_STATE_FLYING))
			{
				AppearWearingWeapon(FALSE);
				m_bDisappearWeapon = FALSE;
			}
		}
		if( m_bRide )
		{
			if( m_bWildRide )
			{
				if( m_nWildPetType == 2 )
				{
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				}
				else
				{
					AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				}
			}
			// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
			else if( m_iRideType%2 == CPetInfo::PET_HORSE )
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_WALK_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
			}
			else
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_WALK_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
			}			
		}
		else
		{
			if (m_ChaState & PLAYER_STATE_FLYING)
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_FLYING_MOVE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			}
			else
			{
				AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_WALK_1], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_WALK_1],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
			}
		}
	};
	void RotatingAnim(void)  
	{
		WalkingAnim(); 
	};

	// adjust sound and watcher parameters here if needed
	void EnemyPostInit(void) 
	{
		// set sound default parameters
		m_soSound.Set3DParameters(160.0f, 50.0f, 1.0f, 1.0f);
	};

	void InitCurrentWearing()
	{
		m_nWearHead			= -1;
		m_nWearArmorUp		= -1;
	    m_nWearWeapon		= -1;
	    m_nWearArmorDown	= -1;
	    m_nWearShield		= -1;
	    m_nWearGlove		= -1;
	    m_nWearBoots		= -1;
		m_nWearWing			= -1;

		m_iCostumeWear.New(WEAR_COSTUME_TOTAL);
		for (int i=0; i<WEAR_COSTUME_TOTAL; ++i)
		{
			m_iCostumeWear[i] = -1;
		}
	}

	// FIXME : Player.es�� ���� ������ �Լ���.
	// FIXME : (�� �԰�, ���� �κ�) �ߺ��� ����, ��û ���� ����.
	void WearingDefaultArmor(int type)//����Ʈ �� ������.(������ �����Ŀ� ȣ��) 
	{
		CJobInfo* pInfo = CJobInfo::getSingleton();

		if (pInfo == NULL)
		{
			return;
		}

		CModelInstance *pMI = GetCurrentPlayerModelInstance();

		MeshInstance *mi;
		CTFileName fnFileName;

		// FIXME : type ���� �Դ� ��ġ���� �ٸ��� ������ ���� �߻�...
		int iWearPos = -1;
		switch( type )
		{

		case WEAR_HELMET:
			iWearPos = HEAD;			
			break;

		case WEAR_JACKET:
			iWearPos = BODYUP;
			break;
		case WEAR_PANTS:
			iWearPos = BODYDOWN;
			break;
		case WEAR_GLOVES:
			iWearPos = HAND;
			break;
		case WEAR_BOOTS:
			iWearPos = FOOT;
			break;
		}

		if( iWearPos == -1 )
		{
			return;
		}

		// ������ ���� �������� ġ���� ����...
		if( type == WEAR_PANTS )
		{
			// Mesh
			fnFileName = pInfo->GetMeshName( m_ChaList, SKIRT );
			if(strlen( fnFileName ) > 0)
			{			
				mi = pMI->AddArmor( fnFileName );

				// Texture
				fnFileName = pInfo->GetTextureName( m_ChaList, SKIRT );
				pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );	

				// NormalMap
				fnFileName = pInfo->GetTexNormalName( m_ChaList, SKIRT );
				if(strcmp(fnFileName, ""))
				{					
					pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
				}
			}
		}

		// Mesh
		fnFileName = pInfo->GetMeshName( m_ChaList, iWearPos );
		mi = pMI->AddArmor( fnFileName );

		// Texture
		fnFileName = pInfo->GetTextureName( m_ChaList, iWearPos );
		pMI->AddTexture_t( fnFileName, fnFileName.FileName(), mi );

		// NormalMap
		fnFileName = pInfo->GetTexNormalName( m_ChaList, iWearPos );
		if(strcmp(fnFileName, ""))
		{			
			pMI->AddTexture_t(fnFileName, fnFileName.FileName(), mi);
		}
/*		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}*/
	}

	void DisplayCurrentWear()
	{
		INDEX delcnt = 0,cnt;
		CTFileName strBmName;
		CTFileName fnFileName;
		cnt = GetCurrentPlayerModelInstance()->mi_aMeshInst.Count(); 
		CPrintF("%s \n",m_strName);				
		for(int i=0;i<cnt;++i)
		{
			strBmName = GetCurrentPlayerModelInstance()->mi_aMeshInst[i].mi_pMesh->GetName();
			CPrintF("%s \n",strBmName);
		}
	}

	virtual	void DeleteDefaultArmor(int type)
	{
		if ( (m_iCostumeWear[type] < 0 && type != WEAR_COSTUME_BACKWING) || (type == WEAR_HELMET && (CTString)_pNetwork->GetItemData(m_iCostumeWear[type])->GetItemSmcFileName() == MODEL_TREASURE ) )// costume�� �԰� ���� �ʾҴٸ� Default armor����
		{
			CModelInstance *pMI = GetCurrentPlayerModelInstance();
			_pGameState->DeleteDefaultArmor( pMI, type, m_ChaList );

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				pTarget->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 
					(pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
				pTarget->cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
		}
	}

	// FIXME : Player.es�� ���� ������ �Լ���.	
	// FIXME : ���� ���� ����...
	void DeleteCurrentArmor(int index) // �����ϰ� ������ �����.
	{	
		if (index < 0)
		{
			return;
		}
		CItemData* pItemData = _pNetwork->GetItemData(index);
		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		_pGameState->TakeOffArmor( pMI, *pItemData );

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 
				(pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
			pTarget->cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
	}

//0127
//�米���۽ÿ� ���⸦ �����ٰ� ��Ÿ���� �ϱ�.
	void DeleteWearingWeapon(BOOL bException, BOOL bSkillReady)
	{
//		if((bException && m_bDisappearWeapon) || (!bException && !m_bDisappearWeapon))
		if(!m_bDisappearWeapon)
		{
			int iCurWear;

			if (bSkillReady)
			{
				iCurWear = IsThereWearing(WEAR_BACKWING);

				if (iCurWear) // ���� �� ����
				{
					CItemData* pItemData = _pNetwork->GetItemData(iCurWear);
					CModelInstance *pMI = GetCurrentPlayerModelInstance();

					_pGameState->TakeOffArmor(pMI, *pItemData);
				}
			}

			iCurWear = IsThereWearing(WEAR_WEAPON);

			if (iCurWear > -1)
			{
				CItemData* pItemData = _pNetwork->GetItemData(iCurWear);
				CModelInstance *pMI = GetCurrentPlayerModelInstance();

				_pGameState->TakeOffArmor( pMI, *pItemData );
				m_bExtension = FALSE;
			}
			if(en_pCharacterTarget != NULL)
			{
				en_pCharacterTarget->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 1);
				en_pCharacterTarget->cha_itemEffect.DeleteEffect(WEAR_WEAPON);
				en_pCharacterTarget->cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
			}
			m_bDisappearWeapon = TRUE;

			//�� �Ѹ��� �׼��� ��
			if( m_nPlayActionNum ==ACTION_NUM_WATER_SPREAD )
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();

				//�ٰ��� �� �߰�
				UBYTE ubJob = m_ChaList;
				MeshInstance *mi = pMI->AddArmor( _afnDishMeshName[ubJob][0] );
				pMI->AddTexture_t( _afnDishTextureName, _afnDishTextureName.FileName(),mi);
				pMI->AddArmor( _afnDishMeshName[ubJob][1] );

				//���Ѹ��� ����Ʈ �߰�
				if( ubJob==0 || ubJob==1 || ubJob==5 || ubJob == 6)	//�����϶�...
				{
					StartEffectGroup("water_sm"		//Hardcording
							, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
				else	//�����϶�...
				{
					StartEffectGroup("water_sw"
							, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
			}
		}
/*		for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, (ct.cha_iType == NIGHTSHADOW) ? 1 : ct.cha_sbItemEffectOption);
				ct.cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
				break;
			}
		}*/
	}

	void AppearWearingWeapon(BOOL bSkillEnd)
	{
		if(m_bDisappearWeapon)
		{
			int iCurWear = IsThereWearing(WEAR_WEAPON);
			/*if(iCurWear == -1) 
			{
				return;
			}*/

			if (iCurWear > -1)
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				CItemData* pItemData	= _pNetwork->GetItemData(iCurWear);

				_pGameState->WearingArmor( pMI, *pItemData );
			}

			if (bSkillEnd)
			{
				iCurWear = IsThereWearing(WEAR_BACKWING);

				if (iCurWear > -1)
				{
					CModelInstance *pMI = GetCurrentPlayerModelInstance();
					CItemData* pItemData = _pNetwork->GetItemData(iCurWear);

					_pGameState->WearingArmor(pMI, *pItemData);
				}
			}

			m_bDisappearWeapon = FALSE;

			//�� �Ѹ��� �׼��� �� - �ٰ��� �� ����
			if( m_nPlayActionNum ==ACTION_NUM_WATER_SPREAD )
			{
				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				pMI->DeleteMesh(_afnDishMeshName[m_ChaList][0]);
				pMI->DeleteMesh(_afnDishMeshName[m_ChaList][1]);
			}
		}	

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
			pTarget->cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, CStatusEffect::R_NONE);
		}
	}

	// FIXME : Player.es�� ���� ������ �Լ���.	
	// FIXME : (�� �԰�, ���� �κ�) �ߺ��� ����, ��û ���� ����.
	void TakeOffWearing(int wear_pos ,ObjectBase* pObject) //���� ����� �Ǹ� ���̱�. // �Ϻ��� ����� �ҽ� 
	{
		INDEX iCosWearIndex = -1;
		if (wear_pos != WEAR_BACKWING)	// pos != 11
		{
			if (wear_pos < WEAR_COSTUME_TOTAL)
			{
				iCosWearIndex = m_iCostumeWear[wear_pos]; // �������� �ڽ�Ƭ �ε���
			}
		}
		else	// pos == 11 -> 7
		{
			iCosWearIndex = m_iCostumeWear[WEAR_COSTUME_BACKWING]; // �������� �ڽ�Ƭ �ε���
		}

		if (iCosWearIndex > 0 &&
			_pNetwork->GetItemData(iCosWearIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
		{
			SetWearing(wear_pos, -1);
			return;
		}

		int iCurWear = IsThereWearing(wear_pos);
		
		if(iCurWear < 0 || pObject == NULL)
		{
			return;
		}

		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		CItemData* pItemData	= _pNetwork->GetItemData(iCurWear);

		if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_HEAD && (CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE)
		{
			return;
		}

		if(pItemData->GetType() == CItemData::ITEM_SHIELD || pItemData->GetType() == CItemData::ITEM_WEAPON)
		{
			if((CTString)pItemData->GetItemSmcFileName() != MODEL_TREASURE)
			{
				_pGameState->TakeOffArmor( pMI, *pItemData );
				
				if(pItemData->GetSubType() == CItemData::ITEM_SHIELD_HEAD)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI, pObject->GetType(), m_nHairStyle-1);
				}
				else
				{
					WearingDefaultArmor(pItemData->GetWearingPosition());
				}
			}
		}
						
		SetWearing(wear_pos, -1); //�Ǹ��̴�...
		StandingAnim();
	}
	//���� �ҽ�
	void TakeOffWearing(int wear_pos) //���� ����� �Ǹ� ���̱�.
	{
//������ ���� ���� �̱� ���� �۾�	08.09
		int iCurWear = IsThereWearing(wear_pos);
		if(iCurWear == -1) 
		{
			return;
		}


		CModelInstance *pMI = GetCurrentPlayerModelInstance();
		CItemData* pItemData	= _pNetwork->GetItemData(iCurWear);
//������ ���� �� �̱� ���� �۾�		08.09
		
		//0609 kwon �߰�.
		// FIXME : Ÿ�Ը� üũ�ϸ� �ɰ� ������...
		if((pItemData->GetType() == CItemData::ITEM_SHIELD
			&& 	(pItemData->GetSubType() == CItemData::ITEM_SHIELD_COAT 
			|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_PANTS 
			|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_GLOVE 
			|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_SHOES
			|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_SHIELD
			 )) ||  pItemData->GetType() == CItemData::ITEM_WEAPON //0808 ���⵵ ���� �Ž��� ����.
		)
		//if(pItemData->GetWearingPosition() != CItemData::ITEM_WEAR_NONE)
		{
			_pGameState->TakeOffArmor( pMI, *pItemData );
			WearingDefaultArmor(pItemData->GetWearingPosition());
		}
						
		SetWearing(wear_pos, -1); //�Ǹ��̴�...

		StandingAnim();
		/*for(INDEX i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
			if(ct.cha_iClientIndex == en_ulID)
			{
				ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
				break;
			}
		}*/
	}

	// FIXME : Player.es�� ���� ������ �Լ���.
	// FIXME : (�� �԰�, ���� �κ�) �ߺ��� ����, ��û ���� ����.
	// FIXME : �� Player.es�� Character.es���� ó���ϴ� ��ƾ�� ���ݾ� �ٸ��ɱ�???
	// �Ϲ� ��� �԰�, �ڽ�Ƭ�� �Դ´�.
	void WearingArmor(int index, int pos) //������ ������ ȣ��.
	{
		INDEX iWearIndex = index;	// ������ ������ �ε���
		INDEX iCosWearIndex = -1;
		BOOL isUpdateAnim = FALSE;
		if (pos != WEAR_BACKWING)	// pos != 11
		{
			if (pos < WEAR_COSTUME_TOTAL)
			{
				iCosWearIndex = m_iCostumeWear[pos]; // �������� �ڽ�Ƭ �ε���
			}
		}
		else	// pos == 11 -> 7
		{
			iCosWearIndex = m_iCostumeWear[WEAR_COSTUME_BACKWING]; // �������� �ڽ�Ƭ �ε���
		}
		BOOL bWearingCostume = FALSE; // �ڽ�Ƭ�� �����ϰ� �ִ°�?

		// ���� �ڽ�Ƭ�� �ƴ� ���
		if (iCosWearIndex >= 0 && _pNetwork->GetItemData(iCosWearIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
		{
			bWearingCostume = TRUE;
		}

		CItemData* pItemData	= _pNetwork->GetItemData(iWearIndex);
		CCharacterTarget* ct = (CCharacterTarget*)en_pCharacterTarget;

		if (pItemData->GetSubType() == CItemData::ITEM_SHIELD_BACKWING && (ct->cha_state & PLAYER_STATE_FLYING))
		{
			AddSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
		}

		if((pItemData->GetType() == CItemData::ITEM_SHIELD || 
			(pItemData->GetType() == CItemData::ITEM_WEAPON && !(ct->cha_state & PLAYER_STATE_FLYING))) && // �����忡���� ���� ���� ������ �ʴ´�.
			!((pItemData->GetSubType() == CItemData::ITEM_SHIELD_BACKWING) && (ct->cha_state & PLAYER_STATE_FLYING))) // �����忡���� ���� ���� ������ �ʴ´�.
		{
			BOOL bNotWear = FALSE;

			if( pItemData->GetType() == CItemData::ITEM_WEAPON )
			{
				const BOOL bExtension = _pNetwork->IsExtensionState( m_ChaList, *pItemData );
				//ct->cha_bExtension	= bExtension;
				m_bExtension		= bExtension;
			}

			CModelInstance *pMI = GetCurrentPlayerModelInstance();

			if (pItemData->GetFlag() & ITEM_FLAG_COSTUME2)
			{
				if (bWearingCostume)	// �ڽ�Ƭ ��� �԰� �־��� ��
				{
					if (iCosWearIndex != index)
					{
						DeleteCurrentArmor(iCosWearIndex);
						bNotWear = TRUE;
					}
				}
				else if (IsThereWearing(pos) > 0) // �Ϲ���� �԰� �־��� ��
				{
					DeleteCurrentArmor(IsThereWearing(pos));
					bNotWear = TRUE;
				}
				else // Default ��� ���� �϶�
				{
					_pGameState->DeleteDefaultArmor( pMI, pItemData->GetWearingPosition(), m_ChaList );
					bNotWear = TRUE;
				}
			}
			else
			{
				if (!bWearingCostume)
				{
					if(IsThereWearing(pItemData->GetWearingPosition()) == WEAR_NONE) //���� �ƹ��͵� ���԰� �־��ٸ�,
					{
						_pGameState->DeleteDefaultArmor( pMI, pItemData->GetWearingPosition(), m_ChaList );
						bNotWear = TRUE;
					}
					else
					{
						DeleteCurrentArmor(IsThereWearing(pos));
						bNotWear = TRUE;
					}
				}
				/*else
				{
					�Ϲ� ���� �ڽ�Ƭ ��� �԰� ������ �޽��� ������ �ʴ´�.
				}*/
			}

			if (bNotWear) // �������ϱ�, index ���������� ���� ����
			{
				// �����µ��� �ڽ�Ƭ�ϰ� �Ϲ� ����ϰ� ������ ����.
				if((CTString)pItemData->GetItemSmcFileName() == MODEL_TREASURE &&
					pItemData->GetSubType() == CItemData::ITEM_SHIELD_HEAD)
				{
					((CPlayerEntity*)CEntity::GetPlayerEntity(0))->ChangeHairMesh(pMI,ct->m_nType, m_nHairStyle-1);
				}
				else
				{
					_pGameState->WearingArmor( pMI, *pItemData );
				}

				//StandingAnim();
				isUpdateAnim	= TRUE;
			}
		}

		if (pItemData->GetFlag() & ITEM_FLAG_COSTUME2)
		{
			if (pos != WEAR_BACKWING)
			{
				m_iCostumeWear[pos] = index;
				ct->cha_CosWear[pos] = index;
			}
			else
			{
				m_iCostumeWear[WEAR_COSTUME_BACKWING] = index;
				ct->cha_CosWear[WEAR_COSTUME_BACKWING] = index;
			}
		}
		else
		{
			SetWearing(pos, index);
			if(isUpdateAnim == TRUE)
			{
				StandingAnim();
			}
		}

			//ct->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct->cha_sbItemEffectOption);

//		}		
/*		else
		{
			//0609 kwon �߰�.
			// FIXME : �� ���� Ÿ���� üũ�� �ʿ䰡 �ֳ�?
			if((pItemData->GetType() == CItemData::ITEM_SHIELD
				&& 	(pItemData->GetSubType() == CItemData::ITEM_SHIELD_COAT 
				|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_PANTS 
				|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_GLOVE 
				|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_SHOES
				|| 	pItemData->GetSubType() == CItemData::ITEM_SHIELD_SHIELD
				)) ||  pItemData->GetType() == CItemData::ITEM_WEAPON //0808 ���⵵ ���� �Ž��� ����.
			)
			{
				// FIXME : �� ��ġ�� ���� �Ұ� ������ ������....
				if( pItemData->GetType() == CItemData::ITEM_WEAPON )
				{
					const BOOL bExtension = _pNetwork->IsExtensionState( m_ChaList, *pItemData );
					for(i=0; _pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
					{
						CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[i];
						if(ct.cha_iClientIndex == en_ulID)
						{
							ct.cha_bExtension	= bExtension;
							m_bExtension		= bExtension;
							break;				
						}
					}
				}

				CModelInstance *pMI = GetCurrentPlayerModelInstance();
				//0605 kwon �߰�.
				if(IsThereWearing(pItemData->GetWearingPosition()) == CItemData::ITEM_WEAR_NONE) //���� �ƹ��͵� ���԰� �־��ٸ�,
				{
					_pGameState->DeleteDefaultArmor( pMI, pItemData->GetWearingPosition(), m_ChaList );
				}
				else
				{
					// ���� �����Ա�.
					DeleteCurrentArmor(IsThereWearing(pItemData->GetWearingPosition()));//�԰� �ִ� ���� �����.					
				}
				
				_pGameState->WearingArmor( pMI, *pItemData );
			}

			SetWearing(pItemData->GetWearingPosition(), index );
			//CurrentWearing[pItemData->GetWearingPosition()] = index; //���ο� ���� �ε��� ����.

			StandingAnim();
	//		CPrintF(TRANS("Wear!!!  CurrentWearing[%d] = %d \n"),pItemData->GetWearingPosition(), index);	
			for(i=0; i<_pNetwork->ga_srvServer.srv_actCha.Count(); ++i)
			{
				CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[i];
				if(ct.cha_iClientIndex == en_ulID)
				{
					ct.cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, ct.cha_sbItemEffectOption);
					break;
				}
			}

		}*/
	}

	BOOL IsEXRogue( int job )	// [2012/08/27 : Sora] EX�α� �߰�
	{
		return (charEXRogue && (job == 7));
	}

	BOOL IsEXMage( int job)		//2013/01/10 jeil EX������ �߰�
	{
		if(charEXRogue){		//2013/01/23 jeil EX������ üũ ��ƾ ���� 
			return (charEXMage && (job == 8));
		}else{
			return (charEXMage && (job == 7));
		}
	}


INDEX IsThereWearing(int pos)
{
/*
	ARMOR				= 1,
	WEAPON				= 2,
	PANTS				= 3,
	SHIELD				= 4,
	GLOVE				= 5,
	BOOTS				= 6,

	228 INDEX m_nWearArmorUp = -1,
	229 INDEX m_nWearWeapon = -1,
	230 INDEX m_nWearArmorDown = -1,
	231 INDEX m_nWearShield = -1,
	232 INDEX m_nWearGlove = -1,
	233 INDEX m_nWearBoots = -1,
*/
	int wearId = -1;
	switch(pos)
	{

		case WEAR_HELMET:
			{
				if (m_nWearHead != -1 )
				{
					CItemData* ptmpItemData = _pNetwork->GetItemData(m_nWearHead);
					if ((CTString)ptmpItemData->GetItemSmcFileName() != MODEL_TREASURE)
					{
						wearId = m_nWearHead;
					}
				}
			}
			break;

		case WEAR_JACKET:
			wearId = m_nWearArmorUp;
			break;
		case WEAR_WEAPON:
			wearId = m_nWearWeapon;
			break;
		case WEAR_PANTS:
			wearId = m_nWearArmorDown;
			break;
		case WEAR_SHIELD:
			wearId = m_nWearShield;
			break;
		case WEAR_GLOVES:
			wearId = m_nWearGlove;
			break;
		case WEAR_BOOTS:
			wearId = m_nWearBoots;
			break;
		case WEAR_BACKWING:
			wearId = m_nWearWing;
			break;
	}
	
	return wearId;
}

INDEX IsThereCostumeWearing(int pos)
{
	int wearId = -1;
	if (pos >WEAR_COSTUME_TOTAL || pos < 0)
	{
		return -1;
	}
	
	return m_iCostumeWear[pos];
}

void SetWearing(int pos,int itemId)
{
	switch(pos)
	{

		case WEAR_HELMET:
			m_nWearHead = itemId;
			break;

		case WEAR_JACKET:
			m_nWearArmorUp = itemId;
			break;
		case WEAR_WEAPON:
			m_nWearWeapon = itemId;
			break;
		case WEAR_PANTS:
			m_nWearArmorDown = itemId;
			break;
		case WEAR_SHIELD:
			m_nWearShield = itemId;
			break;
		case WEAR_GLOVES:
			m_nWearGlove = itemId;
			break;
		case WEAR_BOOTS:
			m_nWearBoots = itemId;
			break;
		case WEAR_BACKWING:
			m_nWearWing = itemId;
			break;
	}	
	return; 
}

// �ڽ�Ƭ ���� ���� ����
void SetCostumeWearing(int pos,int itemId)
{
	if (pos < 0 || pos > WEAR_COSTUME_TOTAL)
	{
		return;
	}

	m_iCostumeWear[pos] = itemId;
}

//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
void GetTargetDirection(FLOAT3D &vTargetPos, FLOAT3D &vDirection)
{
	if(m_penEnemy.ep_pen != NULL)
	{
		CEntity *pen = m_penEnemy;
		FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
		((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
		((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
		FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
		FLOAT3D vTargetCenter(0, 0, 0);
		vTargetPos = FLOAT3D(0, 0, 0);
		vDirection = FLOAT3D(0, 0, 0);
		FLOAT size = 0;
		if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
		{
			if(pen->en_RenderType == RT_SKAMODEL)
			{
				// Bounding Box�� �̿��Ͽ� Effect�� ���� ��ġ�� ã�´�.
				FLOATaabbox3D bbox;
				pen->GetModelInstance()->GetCurrentColisionBox(bbox);
				FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*�ܼ��� ������ �ϸ� �ʹ� ŭ. ������ ���� ���*/;
				size = vRadius.Length();
			}
			vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
			((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
			((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
			vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
			vDirection = vTargetPos - vCurrentPos;
			vDirection.Normalize();
			vTargetPos -= vDirection * size;
		}
		vDirection.Normalize(); 				
	}

}
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)

	void SetAttackSpeed(SBYTE attackspeed)
	{
		m_fAttackSpeed = attackspeed;
//		const int Speed = attackspeed;
//		const int iJob	= m_ChaList;
//
//		CModelInstance *pMI = GetModelInstance();
//		float fLength		= pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1]) * 10;
//		if( m_nWearWeapon != -1 && m_bExtension )
//		{
//			fLength		= pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1]) * 10;
//		}		
//		m_fAttackSpeedMul	= (float)fLength / Speed; //���ǵ尡 0.9�� �Դٸ�, 1.1 �� �ȴ�.
//		m_fAttackLengthMul	= (float)Speed / fLength;
	}
			
///////////////////////

	// FIXME : Player.es�� �ߺ��Ǵ� �κ��� ��û ����...��.��
	void TransformSorcerer(int iType, BOOL bAppear)
	{
		CTString strNewLook;			
		
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));			

		SetSkaModel( TransformInfo().GetFileName(iType) );
		for( INDEX t = ANIM_BASIC_BEGIN; t < ANIM_BASIC_END; ++t )
		{
			m_AnimationIndices.m_iAnimation[t]		= 		ska_GetIDFromStringTable(TransformInfo().GetAnimationName( iType, t ));
		}		
		
		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
		m_bIsTransform = TRUE;
		
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->cha_itemEffect.Refresh(NULL, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
// 			pTarget->cha_statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
// 
// 			pTarget->cha_itemEffect.AddHolyWaterEffect(pTarget->cha_iHitType,
// 					&(pTarget->m_pEntity->en_pmiModelInstance->m_tmSkaTagManager));
// 
// 			pTarget->cha_itemEffect.RefreshPetStashEffect(&(pTarget->m_pEntity->en_pmiModelInstance->m_tmSkaTagManager));
// 			pTarget->cha_itemEffect.RefreshRelicEffect(&(pTarget->m_pEntity->en_pmiModelInstance->m_tmSkaTagManager));
		}
	}

	// FIXME : Player.es�� �ߺ��Ǵ� �κ��� ��û ����.
	void ReturnSorcerer(BOOL bDie)
	{
		if(!m_bIsTransform)
		{
			return;
		}

		CTString strNewLook;	
		m_bIsTransform = FALSE;
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		
		SetSkaModel(CJobInfo::getSingleton()->GetFileName(m_ChaList));

		CModelInstance* pMI = GetModelInstance();
		if(pMI)
		{
			pMI->mi_iFaceMesh = -1;
			pMI->mi_iHairMesh = -1;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, m_ChaList, m_nHairStyle, m_nFaceStyle);
		}

		for( INDEX t = ANIM_BASIC_BEGIN; t < ANIM_BASIC_END; ++t )
		{
			m_AnimationIndices.m_iAnimation[t] = ska_GetIDFromStringTable(CJobInfo::getSingleton()->GetAnimationName( m_ChaList, t ));	
		}
		
		int nCosIndex = -1;

		for (int i=0; i<WEAR_COSTUME_TOTAL; ++i)
		{
			nCosIndex = IsThereCostumeWearing(i);

			// ���� �ڽ�Ƭ�� �ƴϾ�� �Ѵ�.
			if(i < WEAR_COSTUME_TOTAL && nCosIndex > 0 && 
				_pNetwork->GetItemData(nCosIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
			{
				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)_pNetwork->GetItemData(IsThereCostumeWearing(i))->GetItemSmcFileName() != MODEL_TREASURE))
				{
					_pGameState->DeleteDefaultArmor( pMI, i, m_ChaList );
					_pGameState->WearingArmor( pMI, *_pNetwork->GetItemData(IsThereCostumeWearing(i)));
				}
			}
			else if(IsThereWearing(i) != -1)
			{
				if ( i == WEAR_BACKWING && IsThereCostumeWearing(WEAR_COSTUME_BACKWING) > 0 )
				{
					continue;
				}
				CItemData* ptmpItemData = _pNetwork->GetItemData(IsThereWearing(i));

				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)ptmpItemData->GetItemSmcFileName() != MODEL_TREASURE))
				{
					_pGameState->DeleteDefaultArmor( pMI, i, m_ChaList );
					_pGameState->WearingArmor( pMI, *ptmpItemData );
				}
			}
		}

		if( pMI )
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}

		m_bIsTransform	= FALSE;
		m_bTransforming	= FALSE;
//		IdleAnim();		
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if (pTarget != NULL)
			{
				pTarget->cha_itemEffect.Refresh(&(pMI->m_tmSkaTagManager), 1);

				if (pTarget->cha_NickType > 0)
				{
					int t_index = pTarget->cha_CustomTitle.nEffectIdx;

					if (t_index < 0)
					{

						pTarget->m_pEntity->SetNickNameDamageEffect(pTarget->cha_NickType, CEntity::NICKNAME_ATTACH_EFFECT);
					}
					else
					{
						pTarget->m_pEntity->SetCustomTitleEffect(CustomTitleData::m_vecCustomTitleEffect[t_index].c_str());
					}
				}
			}
		}
	}

	// FIXME : Player.es�� �ߺ��Ǵ� �κ��� ��û ����...��.��
	void MonsterChange(int iType, BOOL bAppear)
	{
		if(iType == -1)		// ���ŵ��� ĳ���ͷ� �ٽ� �����ϴ� ��찡 �ִ�. (��Ŷ�� ��Ư�� �������� ���ŵǱ� ����) �� ���� �ٽ� ĳ���ͷ� �ǵ����� �Ѵ�.
		{
			m_nChangeMonsterId	= -1;
			m_bMobChange		= TRUE;
			ReturnChange(FALSE);

			return;
		}

		CTString strNewLook;			
		
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));			
		CMobData* MD = CMobData::getData( iType );
	
		SetSkaModel( MD->GetMobSmcFileName() );
		m_AnimationIndices.m_iAnimation[ANIM_WALK_1]		= ska_GetIDFromStringTable(MD->GetAnimWalkName());
		m_AnimationIndices.m_iAnimation[ANIM_WALK_2]		= ska_GetIDFromStringTable(MD->GetAnimWalkName());
		m_AnimationIndices.m_iAnimation[ANIM_IDLE]			= ska_GetIDFromStringTable(MD->GetAnimDefaultName());
		m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE]	= ska_GetIDFromStringTable(MD->GetAnimDefaultName());
		m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_IDLE] = ska_GetIDFromStringTable(MD->GetAnimDefaultName());
		m_AnimationIndices.m_iAnimation[ANIM_RUN_1]			= ska_GetIDFromStringTable(MD->GetAnimRunName());
		m_AnimationIndices.m_iAnimation[ANIM_RUN_2]			= ska_GetIDFromStringTable(MD->GetAnimRunName());
		m_AnimationIndices.m_iAnimation[ANIM_DIE]			= ska_GetIDFromStringTable(MD->GetAnimDeathName());
		m_AnimationIndices.m_iAnimation[ANIM_DAMAGE]		= ska_GetIDFromStringTable(MD->GetAnimWoundName());
		
		//AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE], AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_IDLE],AN_LOOPING|AN_NORESTART|AN_CLEAR,1,0);
		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
		m_bMobChange = TRUE;
		
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			pTarget->cha_itemEffect.Refresh(NULL, (pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
			pTarget->cha_statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
		}

		// GM_GHANGE�� ��� ũ�� ����
		if(iType == GM_CHANGE)
		{
			GetModelInstance()->StretchModel(FLOAT3D( GM_CHANGE_MUL*10,GM_CHANGE_MUL*10,GM_CHANGE_MUL*10 ));
		}
	}

	// FIXME : Player.es�� �ߺ��Ǵ� �κ��� ��û ����.
	void ReturnChange(BOOL bDie)
	{
		if(!m_bMobChange)
		{
			return;
		}

		CJobInfo* pInfo = CJobInfo::getSingleton();

		ReturnSorcerer(bDie);

		CTString strNewLook;	

		m_bMobChange = FALSE;
		SetPhysicsFlags(GetPhysicsFlags() & ~(EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY));
		
		SetSkaModel(pInfo->GetFileName(m_ChaList));

		CModelInstance* pMI = GetModelInstance();
		if(pMI)
		{
			pMI->mi_iFaceMesh = -1;
			pMI->mi_iHairMesh = -1;
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->SetCharacterAppearance(pMI, m_ChaList, m_nHairStyle, m_nFaceStyle);
		}

		m_AnimationIndices.m_iAnimation[ANIM_WALK_1]		= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_WALK_1 ));
		m_AnimationIndices.m_iAnimation[ANIM_WALK_2]		= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_WALK_2 ));
		m_AnimationIndices.m_iAnimation[ANIM_IDLE]			= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_IDLE ));
		m_AnimationIndices.m_iAnimation[ANIM_ATTACK_IDLE]	= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_ATTACK_IDLE ));
		m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_IDLE] = ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_EXT_ATTACK_IDLE ));
		m_AnimationIndices.m_iAnimation[ANIM_RUN_1]			= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RUN_1 ));
		m_AnimationIndices.m_iAnimation[ANIM_RUN_2]			= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_RUN_1 ));
		m_AnimationIndices.m_iAnimation[ANIM_DIE]			= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_DIE ));
		m_AnimationIndices.m_iAnimation[ANIM_DAMAGE]		= ska_GetIDFromStringTable(pInfo->GetAnimationName( m_ChaList, ANIM_DAMAGE ));				
		
		//SetNormalBox("Default");

		//m_bExtension	= FALSE;

		int nCosIndex = -1;

		// FIXME : ���� 6�̰�, ���� 7???
		for (int i=0; i<WEAR_TOTAL; ++i)
		{
			nCosIndex = IsThereCostumeWearing(i);

			// ���� �ڽ�Ƭ�� �ƴϾ�� �Ѵ�.
			if(i < WEAR_COSTUME_TOTAL && nCosIndex > 0 &&
				_pNetwork->GetItemData(nCosIndex)->IsFlag(ITEM_FLAG_INVISIBLE_COSTUME) == false)
			{
				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)_pNetwork->GetItemData(IsThereCostumeWearing(i))->GetItemSmcFileName() != MODEL_TREASURE))
				{
					_pGameState->DeleteDefaultArmor( pMI, i, m_ChaList );
					_pGameState->WearingArmor( pMI, *_pNetwork->GetItemData(IsThereCostumeWearing(i)));
				}
			}
			else if(IsThereWearing(i) != -1)
			{
				if ( i == WEAR_BACKWING && IsThereCostumeWearing(WEAR_COSTUME_BACKWING) > 0 )
				{
					continue;
				}
				CItemData* ptmpItemData = _pNetwork->GetItemData(IsThereWearing(i));

				if (i!=WEAR_HELMET || (i==WEAR_HELMET && (CTString)ptmpItemData->GetItemSmcFileName() != MODEL_TREASURE))
				{
					_pGameState->DeleteDefaultArmor( pMI, i, m_ChaList );
					_pGameState->WearingArmor( pMI, *ptmpItemData );
				}
			}
		}

		if( pMI )
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}

		SetPhysicsFlags(GetPhysicsFlags() | EPF_TRANSLATEDBYGRAVITY|EPF_ORIENTEDBYGRAVITY);
	}

	void MinMaximize()
	{
	//	if(m_bChanging)
		{
			if(m_bTransformMinimize == FALSE)//�۾�����. 
			{
				static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);

				if(m_iPolymophStep == 0)
				{  	
					tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
					m_iPolymophStep++;
				}

				int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
				if(time < 1)
				{
					return;
				}

				m_iPolymophStep += time;

				if(m_iPolymophStep > 9)
				{
					m_iPolymophStep = 9;
				}		
				m_fScale = 0.1f * (10 - m_iPolymophStep);

				if(m_iPolymophStep == 9)
				{				
					if(!m_bMobChange)
					{
						MonsterChange(m_nChangeMonsterId,FALSE);//�� �۾������� �����Ѵ�.
					}
					else
					{
						m_nChangeMonsterId = -1;
						ReturnChange(FALSE);	//�� �۾������� ��������� ���ƿ´�.
					}

					m_bTransformMinimize = TRUE;
					++m_iPolymophStep;
				}			

				// fix. Stretch���� 0.1f���� 1�� ���� �Ǵ� ����.
				// MonsterChange(), ReturnChange(), �Լ� ���ο� SetSkaModel() �Լ� ȣ��� Stretch���� 1�� ���õ�.
				GetModelInstance()->StretchModel(FLOAT3D( m_fScale,m_fScale,m_fScale ));
			}
			else//���� Ŀ�����Ѵ�.
			{
				static unsigned int tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);	
				
				if(m_iPolymophStep == 10)
				{
					tmStartTime = unsigned int(_pTimer->GetLerpedCurrentTick()*1000);		
					m_iPolymophStep--;

					if(m_nChangeMonsterId == GM_CHANGE)
					{
						static INDEX animID = ska_StringToID("cusedrich_skill");
						static FLOAT animTime = GetModelInstance()->GetAnimLength(animID);
						AddAnimation(animID, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
						StartEffectGroup("cusedrich_disguise", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}
					else
					{
						StartEffectGroup("Polymorph", &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						PlaySound(m_soMessage, SOUND_POLYMOPH, SOF_3D | SOF_VOLUMETRIC);	

					}
				}
		

				int time = ((unsigned int(_pTimer->GetLerpedCurrentTick()*1000) - tmStartTime )%1000)/100;
				if(time < 1)
				{
					return;
				}

				m_iPolymophStep -= time;

				if(m_iPolymophStep < 1)
				{
					m_iPolymophStep = 1;
				}	
				
				if(m_nChangeMonsterId == GM_CHANGE)
				{
					GetModelInstance()->StretchModel(FLOAT3D( GM_CHANGE_MUL*(11-m_iPolymophStep),GM_CHANGE_MUL*(11-m_iPolymophStep),GM_CHANGE_MUL*(11-m_iPolymophStep) ));
				}
				else 
				{
					GetModelInstance()->StretchModel(FLOAT3D( 0.1f*(11-m_iPolymophStep),0.1f*(11-m_iPolymophStep),0.1f*(11-m_iPolymophStep) ));
				}

				if(m_iPolymophStep == 1)
				{
					SetSkaColisionInfo(); //�������� �浹�ڽ��� �ٽ� ��������� ������ ũ��� ���õȴ�.

					m_bChanging = FALSE;	//������ ���� ������.
					m_bOfTransforming = FALSE;
					m_bTransformMinimize = FALSE;
					m_bIdleAnim = TRUE;

					//���λ����߿� ������ �����ٸ� �ٽ� �ɾƾ� �Ѵ�.
					if(m_nPlayActionNum == ACTION_NUM_SELL)
					{
						m_nSitDown = 2;//2�� �ɱ�.
						m_nPlayActionNum = 3;	
						ActionNow();		
					}

					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						if (pTarget != NULL)
						{
							pTarget->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 
								(pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
							pTarget->cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 
								CStatusEffect::R_NONE);

							if (pTarget->cha_NickType > 0)
							{
								int t_index = pTarget->cha_CustomTitle.nEffectIdx;

								if (t_index < 0)
								{

									pTarget->m_pEntity->SetNickNameDamageEffect(pTarget->cha_NickType, CEntity::NICKNAME_ATTACH_EFFECT);
								}
								else
								{
									pTarget->m_pEntity->SetCustomTitleEffect(CustomTitleData::m_vecCustomTitleEffect[t_index].c_str());
								}
							}
						}
					}
				}
			}		
		}
	}

	// ���� ���� ó��...
	void Transforming()
	{
		if(!m_bIsTransform)
		{
			TransformSorcerer(m_iTransformType, FALSE);
		}
		else
		{
			m_iTransformType = 0;
			ReturnSorcerer(FALSE);
		}

		SetNoTargetEntity();
		m_bTransforming = FALSE;
	}

	void SetPolymophMobIndex(int mobId)
	{
		if(m_bChanging)		// ��ȯ�� ��ȯ ��û
		{
			m_bChangingTwice		= TRUE;
			m_nChangeMonsterIdTwice	= mobId;
			PolymophEnd();
		}
		m_nChangeMonsterId = mobId;
	}

	void PolymophEnd()
	{
		SetSkaColisionInfo();

		if(m_nChangeMonsterId == GM_CHANGE)
		{
			GetModelInstance()->StretchModel(FLOAT3D( 1.5f, 1.5f, 1.5f ));
		}
		else 
		{
			GetModelInstance()->StretchModel(FLOAT3D( 1.f,1.f,1.f ));
		}

		if (m_bChanging == TRUE)
		{
			if(m_nChangeMonsterId > 0)
			{
				MonsterChange(m_nChangeMonsterId,FALSE);
			}
			else
			{
				ReturnChange(FALSE);
			}
		}

		m_bOfTransforming = FALSE;
		m_bChanging = FALSE;
		m_bTransformMinimize = FALSE;
		m_iPolymophStep = 0;
		m_bIdleAnim = TRUE;
	}

	BOOL PreSkillAnimationEx(BOOL bOnlyTest)
	{
		if (!bOnlyTest)
		{
			m_bReadyAction = FALSE;
			m_bStillAction = FALSE;
			m_bFireAction = FALSE;
			m_bAttachPlayerEffect = FALSE;
			m_bAttachEnemyEffect = FALSE;
		}

		switch(m_nCurrentSkillNum)
		{
		case 661: // �ҿ� �巹��
			{
				if (!bOnlyTest)
				{
					m_bReadyAction = TRUE;
					m_bStillAction = TRUE;
					m_bAttachPlayerEffect = TRUE;
					m_bAttachEnemyEffect = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetReadyAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension) * m_fSkillSpeed;
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
					m_pSkillReadyEffect = StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetReadyEffect(m_bExtension), &en_pmiModelInstance->m_tmSkaTagManager
											, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN
					
				}
			}
			break;
		case 664: // ǻ�� ���� �ҿ�
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());*/
				}
			}
			break;
		case 666: case 667: case 668: case 669: // �޽� �巹��
			{
				if (!bOnlyTest)
				{
/*					m_bReadyAction = TRUE;
					//m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(661).GetReadyAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					/*m_bReadyAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);*/
				}
			}
			break;
		case 676 : // �ҿ� ������
			{
				if (!bOnlyTest)
				{
					//m_bReadyAction = TRUE;
					/*m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_SOUL_BUSTER : This*/
				}
			}
			break;
		case 677: // ��ũ ��ο�
			{
				if (!bOnlyTest)
				{
					m_bReadyAction = TRUE;
					m_bFireAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension) * m_fSkillSpeed;
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN : This
				}
			}
			break;
		case 764: case 765: case 766: // �������� ��
			{
				if (!bOnlyTest)
				{
					m_bReadyAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension) * m_fSkillSpeed;
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);

					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_AURA_% : This
				}
			}
			break;
		case 681 : // ���� ���� ����Ʈ
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);

					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_COPE_OF_LIGHT : This*/
				}
			}
			break;
/*		case 683: // ���鸮 ����Ʈ
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_penStillTarget = m_penEnemy;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Pink_Ray.tex"));
					m_bConnectEffect = TRUE;
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
						&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
			}
			break;*/
		case 684: // ������Ʈ
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());*/
				}
			}
			break;
		case 685: // �̽�Ƽ ü��
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_bFireAction = TRUE;
					m_penStillTarget = m_penEnemy;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Green_Ray.tex"));
					m_bConnectEffect = TRUE;*/
				}
			}
			break;
		case 686: // ���μ�
		case 683: // ���鸮 ����Ʈ
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_bFireAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());*/
				}
			}
			break;
		case 687: // Ŀ������ ����
			{
				if (!bOnlyTest)
				{
					m_bReadyAction = TRUE;
					m_bFireAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension) * m_fSkillSpeed;
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
				}
			}
			break;
		case 670: case 671: case 672: // �ҿ� ��ũ��
			{
				if (!bOnlyTest)
				{
					/*m_bReadyAction = TRUE;
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
						&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_SOUL_SCREAM*/
				}
			}
			break;
		default:
			{
				return FALSE;
			}
			break;
		}

		return TRUE;
	}

	BOOL FireSkillAnimation(BOOL bOnlyTest)
	{
		switch(m_nCurrentSkillNum)
		{
		case 661: // �ҿ� �巹��
			{
				if (!bOnlyTest)
				{
					if (!m_bFireAction)
					{
						m_bFireAction = TRUE;
						StopEffectGroupIfValid(m_pAttachPlayerEffect, 0.1f);
						StopEffectGroupIfValid(m_pAttachEnemyEffect, 0.1f);

						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // END_SOUL_DRAIN : This
						
						m_bConnectEffect = FALSE;
						m_pSkillTexture.SetData(NULL);
						m_fSkillAnimTime = 0.0f;
					}
				}
			}
			break;
		case 664: // ǻ�� ���� �ҿ�
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // 
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								ShotMissile(this, "HEAD", m_penEnemy, _pNetwork->GetSkillData(m_nCurrentSkillNum).GetMissileSpeed(m_bExtension),
									_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension), _pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect2(m_bExtension),
									false, 0.0f, 0.0f, 2);
							}

							m_bFireAction = TRUE;
						}
					}
				}
			}
			break;
		case 666: case 667: case 668: case 669: // �޽� �巹��
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
					}

					if (!m_bFireAction)
					{
						if (m_dcEnemies.Count() > 0)
						{
							ENTITIES_ITERATOR Itr;

							for (Itr = m_dcEnemies.vectorSelectedEntities.begin(); Itr!=m_dcEnemies.vectorSelectedEntities.end(); ++Itr)
							{
								CEntity& tmpEntity = *(*Itr);
								if (tmpEntity.en_RenderType == RT_SKAMODEL)
								{
									StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect2(m_bExtension),
										&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_DRAIN1 : Enemy
								}
							}
						}

						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // END_SOUL_DRAIN : This

						m_bFireAction = TRUE;
					}
				}
			}
			break;
		case 676 : // �ҿ� ������
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_SOUL_BUSTER : This
					}
					else if (m_dcEnemies.Count() > 0 && !m_bFireAction)
					{
						ENTITIES_ITERATOR Itr;

						for (Itr = m_dcEnemies.vectorSelectedEntities.begin(); Itr!=m_dcEnemies.vectorSelectedEntities.end(); ++Itr)
						{
							CEntity& tmpEntity = *(*Itr);
							
							if (&tmpEntity != NULL)
							{
								if (tmpEntity.en_RenderType == CEntity::RT_SKAMODEL)
								{
									StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
										&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_BUSTER : Enemy
								}
							}
						}

						m_bFireAction = TRUE;
					}
				}
			}
			break;
		case 677: // ��ũ ��ο�
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						//m_bFireAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN : This
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								ShotMissile(this, "LHAND", m_penEnemy, _pNetwork->GetSkillData(m_nCurrentSkillNum).GetMissileSpeed(m_bExtension),
									_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension), _pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect2(m_bExtension),
									false, 0.0f, 0.0f, 2);
							}

							m_bFireAction = TRUE;
						}
					}
				}
			}
			break;
		case 764: case 765: case 766: // �������� ��
			{
				m_fSkillAnimTime = 0.0f;
			}
			break;
		case 681 : // ���� ���� ����Ʈ
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);

						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_COPE_OF_LIGHT : This
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
									&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // TGT_COPE_OF_LIGHT : Enemy
							}
						}
					}
				}
			}
			break;
/*		case 683: // ���鸮 ����Ʈ
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_penStillTarget = m_penEnemy;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Pink_Ray.tex"));
						m_bConnectEffect = TRUE;

						if (m_penEnemy)
						{
							StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
								&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						}
					}
					else
					{
						m_bConnectEffect = FALSE;
						m_pSkillTexture.SetData(NULL);
					}
				}
			}
			break;*/
		case 684: // ������Ʈ
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
									&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // END_SOUL_DRAIN : This
							}

							m_bFireAction = TRUE;
						}
					}
				}
			}
			break;
		case 685: // �̽�Ƽ ü��
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_penStillTarget = m_penEnemy;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
						m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Green_Ray.tex"));
						m_bConnectEffect = TRUE;
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
									&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
							}
						}

						m_bConnectEffect = FALSE;
						m_penStillTarget = NULL;
						m_pSkillTexture.SetData(NULL);
					}
				}
			}
			break;
		case 686: // ���μ�
		case 683: // ���鸮 ����Ʈ
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								ShotMissile(this, "LHAND", m_penEnemy, _pNetwork->GetSkillData(m_nCurrentSkillNum).GetMissileSpeed(m_bExtension),
									_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension), _pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect2(m_bExtension),
									false, 0.0f, 0.0f, 2, NULL, &m_dcEnemies);
							}

							m_bFireAction = TRUE;
						}
					}
				}
			}
			break;
		case 687: // Ŀ������ ����
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_bFireAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
					}
					else
					{
						if (m_penEnemy)
						{
							if (m_penEnemy->en_RenderType == RT_SKAMODEL)
							{
								StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
									&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
							}

							m_bFireAction = TRUE;
						}
					}
				}
			}
			break;
		case 670: case 671: case 672: // �ҿ� ��ũ��
			{
				if (!bOnlyTest)
				{
					if (!m_bReadyAction)
					{
						m_bReadyAction = TRUE;
						m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
						m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
						m_fSkillAnimTime = _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDelay(0, m_bExtension);
						AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
						StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(m_bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // CST_SOUL_SCREAM
					}
					else if (m_dcEnemies.Count() > 0 && !m_bFireAction)
					{
						ENTITIES_ITERATOR Itr;

						for (Itr = m_dcEnemies.vectorSelectedEntities.begin(); Itr!=m_dcEnemies.vectorSelectedEntities.end(); ++Itr)
						{
							CEntity& tmpEntity = *(*Itr);

							if (&tmpEntity != NULL)
							{
								if (tmpEntity.en_RenderType == CEntity::RT_SKAMODEL)
								{
									StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect2(m_bExtension),
										&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_SCREAM
									
									StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect3(m_bExtension),
										&tmpEntity.GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_SCREAM
								}
							}
						}

						m_bFireAction = TRUE;
					}
				}
			}
			break;
		default:
			{
				return FALSE;
			}
			break;
		}

		return TRUE;
	}

	BOOL HardCodingSkill(BOOL bOnlyTest)
	{
		if( m_nCurrentSkillNum != 128	//�α�,���̾� ��Ʈ����ũ
		&& m_nCurrentSkillNum != 155	//Ÿ��ź, ���� �����Ʈ.
		&& m_nCurrentSkillNum != 191	//�α�, ���� ���
		&& m_nCurrentSkillNum != 1540	// [2012/08/27 : Sora] EX�α� �߰�
		)
		{
			return FALSE;
		}
		if(bOnlyTest) {return TRUE;	}

		CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
		CEntityPointer penTarget = m_penEnemy;
		//if(penTarget == NULL) {return TRUE;}
		if(m_afHardCodeValue.Count() == 0) {m_afHardCodeValue.New(32);}


        m_bHardcodingSkillLoop = TRUE;

		switch(m_nCurrentSkillNum)
		{
		case 128://�α�, ���̾� ��Ʈ����ũ, ���� �ܰ˸� ó����
			{
				FLOAT fFirstAttackTime	= 0.25f/* * m_fSkillSpeed*/;
				//FLOAT fAttackTime		= 0.7f * m_fSkillSpeed;
				static INDEX iFireAnimID		= ska_StringToID(skill.GetFireAnim(m_bExtension));
				static FLOAT fAttackTimeOnce	= GetModelInstance()->GetAnimLength(iFireAnimID);
				static FLOAT fDistFromCB		= 1.0f;
				static FLOAT fDamageTime		= 999.0f;
				if(penTarget == NULL) {break;}
				
				if(m_iSkillEffectStep == 0)
				{					
					m_soMessage.Set3DParameters(25.0f, 5.0f, 1.0f, 1.0f);
					++m_iSkillEffectStep;
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					m_tmDireStrikeStart = _pTimer->GetLerpedCurrentTick();
					//���� ��ġ�� ���� ����
					CPlacement3D pl = GetPlacement();
					m_afHardCodeValue[11] = pl.pl_OrientationAngle(1);
					m_afHardCodeValue[12] = pl.pl_OrientationAngle(2);
					m_afHardCodeValue[13] = pl.pl_OrientationAngle(3);
					m_afHardCodeValue[14] = pl.pl_PositionVector(1);
					m_afHardCodeValue[15] = pl.pl_PositionVector(2);
					m_afHardCodeValue[16] = pl.pl_PositionVector(3);

					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						pTarget->cha_itemEffect.Refresh(NULL, 1);
						pTarget->cha_statusEffect.Refresh(NULL, CStatusEffect::R_NONE);
					}
					
					SetFlagOn(ENF_HIDDEN);
					FLOAT3D vDir = penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector;

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					return TRUE;
				}
				//first
				if(m_iSkillEffectStep == 1)
				{
					//if((_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime) < fFirstAttackTime)
					{
						/*FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
						m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
						FLOAT3D vDir = penTarget->GetPlacement().pl_PositionVector
										- GetPlacement().pl_PositionVector;
						vDir.SafeNormalize();
						CPlacement3D plNew = GetPlacement();
						plNew.pl_PositionVector += vDir * m_afHardCodeValue[9] * delta;*/
						FLOAT3D vDelta = penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector;
						FLOAT3D vNormal = vDelta;
						vNormal.SafeNormalize();
						CPlacement3D plNew = GetPlacement();
						plNew.pl_PositionVector += (vNormal * (vDelta.Length()* 0.9f));
						SetPlacement(plNew);
						//return TRUE;
					}
					++m_iSkillEffectStep;
					//���� xz������ ũ�� ���ϱ�
					FLOATaabbox3D aabb;
					penTarget->GetModelInstance()->GetCurrentColisionBox(aabb);
					FLOAT3D size = aabb.Size();
					m_afHardCodeValue[0] = sqrtf(size(1) * size(1) + size(3) * size(3)) * 0.5f;//����ũ�������
					//ĳ���� ����ȭ ����
					SetFlagOff(ENF_HIDDEN);

					ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

					if (pObject != NULL)
					{
						CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

						CStatusEffect::eRelation rel = CStatusEffect::R_NONE;

						if( pTarget->GetEntity()->IsSecondExtraFlagOn( ENF_EX2_MYPARTY ) ) {rel = CStatusEffect::R_PARTY;}
						pTarget->cha_itemEffect.Refresh(&GetModelInstance()->m_tmSkaTagManager, 1);
						pTarget->cha_statusEffect.Refresh(&GetModelInstance()->m_tmSkaTagManager, rel);
					}
					
					//�÷��̾��� ��ġ �ű��
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(fRadius*(0.5f*1.7320f), 0, fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//�ִϸ��̼� �ǽ�
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0);

					if(!m_bExtension)
					{
						PlaySound(m_soMessage, SOUND_DAIR_STRIKE_BLOW1, SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{					
						PlaySound(m_soMessage, SOUND_A_DAIR_STRIKE_BLOW1, SOF_3D | SOF_VOLUMETRIC);
					}			

					StartEffectGroup(skill.GetFireEffect1(m_bExtension), &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//�ð�
					m_afHardCodeValue[2] = 2 * (m_afHardCodeValue[0] + fDistFromCB) / fAttackTimeOnce;//�ӵ�
					fDamageTime = skill.GetDelay(0, m_bExtension);
					return TRUE;
				}
				if(m_iSkillEffectStep == 2)
				{
					//�÷��̾��� ��ġ �ű��
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, m_bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//second
				if(m_iSkillEffectStep == 3)
				{
					++m_iSkillEffectStep;
					//�÷��̾��� ��ġ �ű��
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(-fRadius*(0.5f*1.7320f), 0, fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//�ִϸ��̼� �ǽ�
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0);

					if(!m_bExtension)
					{
						PlaySound(m_soMessage, SOUND_DAIR_STRIKE_BLOW2, SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{					
						PlaySound(m_soMessage, SOUND_A_DAIR_STRIKE_BLOW2, SOF_3D | SOF_VOLUMETRIC);
					}		

					StartEffectGroup(skill.GetFireEffect1(m_bExtension), &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//�ð�
					return TRUE;
				}
				if(m_iSkillEffectStep == 4)
				{
					//�÷��̾��� ��ġ �ű��
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + 2*fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, m_bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//third
				if(m_iSkillEffectStep == 5)
				{
					++m_iSkillEffectStep;
					//�÷��̾��� ��ġ �ű��
					CPlacement3D plNew;
					FLOAT fRadius = m_afHardCodeValue[0] + fDistFromCB;
					FLOAT3D deltaPos(0, 0, -fRadius*0.5f);
					plNew.pl_PositionVector = penTarget->GetPlacement().pl_PositionVector + deltaPos;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir((-deltaPos).SafeNormalize());
					SetPlacement(plNew);
					//�ִϸ��̼� �ǽ�
					NewClearState(0);
					AddAnimation(iFireAnimID, AN_NORESTART, 1, 0);

					if(!m_bExtension)
					{
						PlaySound(m_soMessage, SOUND_DAIR_STRIKE_BLOW3, SOF_3D | SOF_VOLUMETRIC);
					}
					else
					{					
						PlaySound(m_soMessage, SOUND_A_DAIR_STRIKE_BLOW3, SOF_3D | SOF_VOLUMETRIC);
					}		

					StartEffectGroup(skill.GetFireEffect1(m_bExtension), &GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());

					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();//�ð�
					return TRUE;
				}
				if(m_iSkillEffectStep == 6)
				{
					//�÷��̾��� ��ġ �ű��
					FLOAT delta = _pTimer->GetLerpedCurrentTick() - m_afHardCodeValue[1];
					m_afHardCodeValue[1] = _pTimer->GetLerpedCurrentTick();
					FLOAT deltaDist = m_afHardCodeValue[2] * delta;
					CPlacement3D plNew;
					FLOAT3D deltaDir = (penTarget->GetPlacement().pl_PositionVector - GetPlacement().pl_PositionVector).SafeNormalize();
					plNew.pl_PositionVector = GetPlacement().pl_PositionVector + deltaDir * deltaDist;
					plNew.pl_OrientationAngle = GetEulerAngleFromDir(deltaDir);
					SetPlacement(plNew);

					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + fDamageTime)
					{
						FLOAT3D vHitPoint = penTarget->GetPlacement().pl_PositionVector + deltaDir * (-m_afHardCodeValue[0]);
						//InflictDirectDamage(penTarget, this, DMT_CLOSERANGE, 1, vHitPoint, deltaDir);
						StartEffectGroup("Normal Hit", _pTimer->GetLerpedCurrentTick(), vHitPoint, deltaDir);
						fDamageTime = 999.0f;					
					}
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime > fFirstAttackTime + 3*fAttackTimeOnce)
					{
						fDamageTime = skill.GetDelay(0, m_bExtension);
						++m_iSkillEffectStep;
					}
					return TRUE;
				}
				//finalize
				if(m_iSkillEffectStep == 7)
				{
					InflictDirectDamage(penTarget, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0));
					CPlacement3D plNew;
					plNew.pl_OrientationAngle(1) = m_afHardCodeValue[11];
					plNew.pl_OrientationAngle(2) = m_afHardCodeValue[12];
					plNew.pl_OrientationAngle(3) = m_afHardCodeValue[13];
					plNew.pl_PositionVector(1) = m_afHardCodeValue[14];
					plNew.pl_PositionVector(2) = m_afHardCodeValue[15];
					plNew.pl_PositionVector(3) = m_afHardCodeValue[16];
					SetPlacement(plNew);
					StartEffectGroup(skill.GetFireEffect3(m_bExtension)
						, &penTarget->GetModelInstance()->m_tmSkaTagManager
						, _pTimer->GetLerpedCurrentTick());
					fDamageTime = 999.0f;
				}
			} break;
		case 155://Ÿ��ź ���� �����Ʈ.
			{
				StartEffectGroup(skill.GetFireEffect1(m_bExtension),
								&GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
			} break;
		case 191://�α�, ���� ���
		case 1540:// [2012/08/27 : Sora] EX�α� �߰�
			{
				static INDEX animID = ska_StringToID(skill.GetFireAnim(m_bExtension));
				static FLOAT animTime = GetModelInstance()->GetAnimLength(animID);
				if(m_iSkillEffectStep == 0)
				{
					++m_iSkillEffectStep;
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(animID, AN_NORESTART|AN_CLEAR, 1.0f, 0);
					m_bIdleAnim = FALSE;
					return TRUE;
				}
				if(m_iSkillEffectStep == 1)
				{
					if(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime <= animTime)
					{
						return TRUE;
					}
					
					m_bIdleAnim = FALSE;
					++m_iSkillEffectStep;
				}
			} break;
		default: {return FALSE;}
		}

		m_iSkillEffectStep = 0;

		m_idCurrentSkillAnim = -1;//��ų ����.
		m_tmSkillStartTime = 0.0f;
		m_bSkilling = FALSE;
		m_nCurrentSkillNum = -1;
		m_fSkillAnimTime = 0.0f;

		m_bHardcodingSkillLoop = FALSE;
		if(m_bIdleAnim)
		{
			NewClearState(0.2f); 
			IdleAnim();			
		}
		
		SetNoTargetEntity();

		SetFlagOff(ENF_HIDDEN);
		StopRotating();
		
		return TRUE;
	}
	
	void SetNickNameDamageEffect(INDEX iNickIndex, NickNameEffectType iType)
	{
		CTString  strNickDamageFile;
		
		if(iNickIndex > 0 && iType == NICKNAME_ATTACK_EFFECT)	// ȣĪ�� ������
		{
			strNickDamageFile = TitleStaticData::getData(iNickIndex)->GetAttackEffectFile();
		}
		else if(iNickIndex > 0 && iType == NICKNAME_DAMAGE_EFFECT)
		{
			strNickDamageFile = TitleStaticData::getData(iNickIndex)->GetDamageEffectFile();
		}
		else if(iNickIndex > 0 && iType == NICKNAME_ATTACH_EFFECT)
		{
			strNickDamageFile = TitleStaticData::getData(iNickIndex)->GetEffectFile();
		}

		if(strNickDamageFile != CTString("") && iType != NICKNAME_ATTACH_EFFECT)
		{
			StartEffectGroup(strNickDamageFile
						, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager
						, _pTimer->GetLerpedCurrentTick());
		}
		else if(strNickDamageFile != CTString("") && iType == NICKNAME_ATTACH_EFFECT)
		{
			en_pCharacterTarget->cha_itemEffect.AddNickEffect(strNickDamageFile, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager);
		}
	}

	void SetCustomTitleEffect(CTString effectName)
	{
		if(effectName != CTString(""))
		{
			en_pCharacterTarget->cha_itemEffect.AddNickEffect(effectName, &GetCurrentPlayerModelInstance()->m_tmSkaTagManager);
		}
	}
/////////////////////////

	void SkillAnimationStop()
	{
		//LOG_DEBUG("### SkillAnimationStop");
		// ���ེų
		if (m_nCurrentSkillNum == 688)
		{
 			if (HasSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc")) == true)
 			{ 
 				// �������� state������ ���� �ع�����.
 				m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(1));
 				DelSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
				AppearWearingWeapon(TRUE);
				StopRotating();
 			}
		}
	};

procedures:
/************************************************************
 *                       M  A  I  N                         *
 ************************************************************/
	Main(EVoid) 
	{
		// initialize [3/21/2011 rumist]
		m_pRiderModelInstance = NULL;
		m_pRidePetModelInstance = NULL;
		m_pRideCharModelInstance = NULL;
		m_nWildPetType = -1;
		if(IsFlagOn(ENF_ZONEMOVING) && en_pmiModelInstance != NULL)
		{
			StartEffectGroup("(EG)Portal", &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->CurrentTick());
		}
		
		//	StartEffectGroup( "GuildBattle", &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->CurrentTick());

		CModelInstance *pMI = GetModelInstance();

		if(pMI == NULL)
		{ 
			return EEnd();
		}
		// declare yourself as a model
		SetPhysicsFlags(EPF_MODEL_WALKING|EPF_HASLUNGS
		| EPF_ABSOLUTETRANSLATE);//1125
		
//		|EPF_NOACCELERATION);
/*	SetPhysicsFlags(EPF_ONBLOCK_CLIMBORSLIDE
			| EPF_TRANSLATEDBYGRAVITY
			| EPF_PUSHABLE
			| EPF_MOVABLE
			| EPF_HASLUNGS
			| EPF_ABSOLUTETRANSLATE);
*/
// NOTE : �������õǼ�...
		SetCollisionFlags(ECF_MODEL);
		//SetCollisionFlags(ECF_MODEL_NO_COLLISION);
//		SetCollisionFlags(GetCollisionFlags() & ~ 
		
		en_sbNetworkType = MSG_CHAR_PC;

		SetFlags(GetFlags()|ENF_ALIVE);		
		en_tmMaxHoldBreath = 5.0f;
		en_fDensity = 2000.0f;

		// setup moving speed
		m_fWalkSpeed			= 3.0f;
		m_aWalkRotateSpeed		= AngleDeg(1800.0f);
		m_fAttackRunSpeed		= 5.0f;
		m_aAttackRotateSpeed	= AngleDeg(1800.0f);
		m_fCloseRunSpeed		= 5.0f;
		m_aCloseRotateSpeed		= AngleDeg(1800.0f);

		// setup attack distances
		m_fAttackDistance		= 5.0f;
		m_fCloseDistance		= 2.0f;			// �ٰŸ� ���????
		m_fStopDistance			= 0.1f;			// (Stop Distance���� �����ٸ� ���� �Ѿư��� ����.)
		m_fAttackFireTime		= 0.3f;
		m_fCloseFireTime		= 1.5f;
		m_fBlowUpAmount			= 80.0f;
		m_fBodyParts			= 4;
		m_fDamageWounded		= 0.0f;
		
		// set stretch factors for height and width
		pMI->StretchModel(FLOAT3D(m_fStretch, m_fStretch, m_fStretch));
		pMI->mi_bRenderShadow = TRUE;

		//0605 kwon �߰�.				
		InitCurrentWearing();


		SetHealth(10000.0f);//0703 ĳ������ ü��.

		ModelChangeNotify();

		if(m_nCurrentHealth ==0)
		{
			AnimForDeath();
		}
		else
		{
			StandingAnim();
		}
		
//������ ���� ����	//(Open beta)(2004-12-14)
		if(pMI)
		{
			CSkaTag tag;
			tag.SetName("__ROOT");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			pMI->m_tmSkaTagManager.Register(&tag);
			tag.SetName("__TOP");
			tag.SetOffsetRot(GetEulerAngleFromQuaternion(en_pmiModelInstance->mi_qvOffset.qRot));
			FLOATaabbox3D aabb;
			pMI->GetAllFramesBBox(aabb);
			tag.SetOffsetPos(0, aabb.Size()(2) * pMI->mi_vStretch(2), 0);
			pMI->m_tmSkaTagManager.Register(&tag);
		}
//������ ���� ��	//(Open beta)(2004-12-14)

		// continue behavior in base class
		jump CCharacterBase::MainLoop();
	};

/************************************************************
 *                A T T A C K   E N E M Y                   *
 ************************************************************/
	Fire(EVoid) : CCharacterBase::Fire
	{
		jump CCharacterBase::Fire();
	}

	Hit(EVoid) : CCharacterBase::Hit 
	{		
		autocall NormalAttack() EEnd;
		return EReturn();
	};
		//0628 kwon
	AttackTarget(EVoid) : CCharacterBase::AttackTarget // �� ���ν����� call�ϱ����� SetTargetEntity()�� ����Ǿ�� �Ѵ�.
	{	
		m_vDesiredPosition = PlayerDestinationPos();
		
		autocall AttackAnimation() EReturn;

		return EReturn();
	};

	AttackAnimation(EVoid) //0628
	{
		//INDEX idBodyAnim;
		float frandom = FRnd();
		INDEX iAttack_Type = 0;

		if( m_nWearWeapon != -1 )
		{
		}		

		// NOTE : ���⿡ ���� �ִϸ��̼ǵ� �޶�� �Ѵ�.
		// FIXME : Ȯ���� �ϴ� ���ѵ�... ĳ���Ϳ� ���� �ִϸ��̼� ������ �ٸ��� ��� ����???
		if( !m_bExtension )
		{
			if( frandom < 0.25f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1];
				m_iPlaying_Ani_Attack = ANIM_ATTACK_1;
				iAttack_Type = 0;
			}
			else if(frandom < 0.5f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2];
				m_iPlaying_Ani_Attack = ANIM_ATTACK_2;
				iAttack_Type = 1;
			}
			else if(frandom < 0.75f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3];
				m_iPlaying_Ani_Attack = ANIM_ATTACK_3;
				iAttack_Type = 2;
			}
			else
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4];
				m_iPlaying_Ani_Attack = ANIM_ATTACK_4;
				iAttack_Type = 3;
			}
		}
		// ���� �� �α� ����....
		else
		{
			if( frandom < 0.25f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1];
				m_iPlaying_Ani_Attack = ANIM_EXT_ATTACK_1;
				iAttack_Type = 4;
			}
			else if(frandom < 0.5f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2];
				m_iPlaying_Ani_Attack = ANIM_EXT_ATTACK_2;
				iAttack_Type = 5;
			}
			else if(frandom < 0.75f )
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3];
				m_iPlaying_Ani_Attack = ANIM_EXT_ATTACK_3;
				iAttack_Type = 6;
			}
			else
			{
				idBodyAnim = m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4];
				m_iPlaying_Ani_Attack = ANIM_EXT_ATTACK_4;
				iAttack_Type = 7;
			}
		}

		// ���� �ִϸ��̼� ���� �� Ÿ�̹�
		FLOAT fLength = GetAnimLength(m_AnimationIndices.m_iAnimation[m_iPlaying_Ani_Attack]) * 10.0f;
		m_fAttackSpeedMul	= 1.0f / (fLength / m_fAttackSpeed);
		m_fAttackLengthMul	= m_fAttackSpeed / fLength;

		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			AppearWearingWeapon(FALSE);
			m_bDisappearWeapon = FALSE;
		}
//		GetModelInstance()->AddAnimation(idBodyAnim,AN_CLEAR,1.0f,0,1.2f);//1009
		AddAnimation(idBodyAnim, AN_CLEAR, 1.0f, 0x03,
						ESKA_MASTER_MODEL_INSTANCE, m_fAttackSpeedMul);

		m_fAttackFrequency = 0.25f;
		m_fAttackStartTime = _pTimer->CurrentTick();
		
		// FIXME : �� ���⼭�� �⺻ �ִ� 1���� �ִϸ��̼� �ð��� ���ٰ� ����? �ٸ� �ִϸ��̼���???
		// �ʿ� ���µ�~~
		//m_fImpactTime = GetAnimLength(m_AnimationIndices.m_iAnimation[m_iPlaying_Ani_Attack])*0.8f/2 * m_fAttackLengthMul;//0630 �켱 ���� �ִϸ��̼��� ���ݿ��� ����Ʈ.
		//m_fImpactTime = JobInfo().GetImpactTime( m_ChaList, iAttack_Type ) * m_fAttackLengthMul;
		// ����ü �϶�...
		if( m_bIsTransform  )
		{
			if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || 
				idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1])
			{
				m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK1 ) * m_fAttackLengthMul;
			}
			else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] ||
				idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2])
			{
				m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK2 ) * m_fAttackLengthMul;
			}
			else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3] ||
				idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
			{
				m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK3 ) * m_fAttackLengthMul;
			}
			else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4] ||
				idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
			{
				m_fImpactTime = TransformInfo().GetImpactTime( m_iTransformType, ATTACK4 ) * m_fAttackLengthMul;
			}
		}
		else
		{
			m_fImpactTime = CJobInfo::getSingleton()->GetImpactTime(m_ChaList, iAttack_Type) * m_fAttackLengthMul;
		}

//������ ���� ����	//(Closed Beta Effect)(0.1)
		m_nEffectStep = 0;
		m_saLastEffectInfo.Clear();
//������ ���� ��	//(Closed Beta Effect)(0.1)

		while(_pTimer->CurrentTick() - m_fAttackStartTime < GetAnimLength(m_AnimationIndices.m_iAnimation[m_iPlaying_Ani_Attack])*m_fAttackLengthMul)
		{//1009
			wait(m_fAttackFrequency) 
			{
				on (EBegin) : 
				{ 
					
					if(m_bMoving)
					{
						return EReturn();
					}
					
					m_fMoveSpeed = 0.0f; 
					ULONG ulFlags = SetDesiredMovement(); 	
					
//������ ���� ����	//(Closed Beta Effect)(0.1)
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;

						const int iJob = m_ChaList;
						if(iJob == KNIGHT)
						{							
							if( !m_bExtension )
							{
								if(	idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || 
									idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3] )
								{								
									StartEffectGroup("Knight Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] || 
										idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
	
									StartEffectGroup("Knight Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
							// �α� �� ���� ����...
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{								
									StartEffectGroup("Knight Attack 1_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
	
									StartEffectGroup("Knight Attack 2_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if(iJob == TITAN)
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if( iJob == MAGE )
						{
							if( !m_bExtension )
							{
								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] )
								{
									StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.38f * m_fAttackLengthMul;
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] )
								{
									StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.46f * m_fAttackLengthMul;
								}
							}
							else
							{
								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] )
								{
									StartEffectGroup( "mEnerBall0_a", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.38f * m_fAttackLengthMul;
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] )
								{
									StartEffectGroup( "mEnerBall1_a", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.46f * m_fAttackLengthMul;
								}
							}
						}
						else if( iJob == ROGUE )
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1])
								{
									StartEffectGroup("Rogue Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2])
								{
									StartEffectGroup("Rogue Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if(iJob == SORCERER)
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
							else
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_3])
								{
									StartEffectGroup("Titan Attack 1_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] || idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_4])
								{
									StartEffectGroup("Titan Attack 2_a"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if( IsEXRogue(iJob) )	// [2012/08/27 : Sora] EX�α� �߰�
						{
							if( !m_bExtension )
							{
								if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1])
								{
									StartEffectGroup("Rogue Attack 1"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
								else if(idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2])
								{
									StartEffectGroup("Rogue Attack 2"		//Hardcording
										, &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								}
							}
						}
						else if( IsEXMage(iJob) )	// 2013/01/10 jeil EX������ �߰� 
						{
														if( !m_bExtension )
							{
								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] )
								{
									StartEffectGroup( "mEnerBall0", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.38f * m_fAttackLengthMul;
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] )
								{
									StartEffectGroup( "mEnerBall1", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.46f * m_fAttackLengthMul;
								}
							}
							else
							{
								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] )
								{
									StartEffectGroup( "mEnerBall0_a", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.38f * m_fAttackLengthMul;
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] )
								{
									StartEffectGroup( "mEnerBall1_a", &en_pmiModelInstance->m_tmSkaTagManager,
														_pTimer->GetLerpedCurrentTick() );
									//m_fImpactTime = 0.46f * m_fAttackLengthMul;
								}
							}
						}

						// FIXME : ���ݰ��� ID�� �迭�� ����� ����???
					}
//������ ���� ��	//(Closed Beta Effect)(0.1)

					CPrintF(TRANS("AttAnimType : %d, Freq time : %f, Impact time : %f\n"), m_iPlaying_Ani_Attack, (_pTimer->CurrentTick() - m_fAttackStartTime), m_fImpactTime);

					// ���Ÿ��� �����ϴ� ��쿡 ���� ó��.
					if(_pTimer->CurrentTick() - m_fAttackStartTime > m_fImpactTime-0.1f)
					{


						INDEX iHitType = 0;

						ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

						if (pObject != NULL)
						{
							CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

							iHitType = pTarget->cha_iHitType;
						}

						if( m_penEnemy.ep_pen != NULL && ( m_penEnemy->GetFlags() & ENF_ALIVE ) )
						{
							if(m_ChaList == HEALER)
							{
								if( m_bExtension )
								{
									ShotMissile(this, "STAFF", m_penEnemy.ep_pen, 40.0f
												, szHitEffect[m_ChaList][iHitType], "Normal Arrow Trace", false
												, 0.0f, 0.0f, 1);	//Hardcoding
								}
								else
								{
									ShotMissile(this, "RHAND", m_penEnemy.ep_pen, 40.0f
												, szHitEffect[m_ChaList][iHitType], "Normal Arrow Trace", false);	//Hardcoding
								}
							}
							else if (m_ChaList == NIGHTSHADOW)
							{
								CTString strMissile = CTString("ns_basic"); // �߻�ü ����Ʈ
								CTString strHit = szHitEffect[m_ChaList][iHitType]; // ��Ʈ ����Ʈ
								
								if (m_nWearWeapon > -1)
								{
									CItemData* pItemData = _pNetwork->GetItemData(m_nWearWeapon);
									CTString strShotEffect = pItemData->GetMissileShotEffect();
									CTString strHitEffect = pItemData->GetShotHitEffect();
									
									if (strShotEffect.Length() > 0)
									{
										strMissile = strShotEffect;
									}

									if (strHitEffect.Length() > 0)
									{
										strHit = strHitEffect;
									}
								}

								switch (m_iPlaying_Ani_Attack)
								{
								case ANIM_ATTACK_1: case ANIM_ATTACK_3:
								case ANIM_EXT_ATTACK_1: case ANIM_EXT_ATTACK_3:
									{ // Left Attack
										ShotMissile(this, "LHAND", m_penEnemy.ep_pen, 40.0f, strHit, strMissile, false, 0.0f, 0.0f, 2);
									}
									break;
								case ANIM_ATTACK_2: case ANIM_ATTACK_4:
								case ANIM_EXT_ATTACK_2: case ANIM_EXT_ATTACK_4:
									{ // Right Attack
										ShotMissile(this, "RHAND", m_penEnemy.ep_pen, 40.0f, strHit, strMissile, false, 0.0f, 0.0f, 2);
									}
									break;
								}
							}
							else if( m_ChaList == MAGE)
							{
								FLOAT3D		vStartPos;
								FLOATquat3D	qStartRot;
								CTag		*pTag = NULL;

								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "WAND" );
									if( pTag == NULL )
									{
										pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
									}
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}

								if( pTag != NULL )
								{
									ShotMagicContinued( this, vStartPos, qStartRot, m_penEnemy.ep_pen, 35.0F,
														szHitEffect[m_ChaList][iHitType], "mMissileEnerBall",
														false, 0 );
								}
							}
							else if( m_ChaList == SORCERER)
							{
								if( m_bIsTransform && m_iTransformType == CTransformInfo::TYPE_1 )
								{
									FLOAT3D		vStartPos;
									FLOATquat3D	qStartRot;
									CTag		*pTag = NULL;

									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}

									if( pTag != NULL )
									{
										ShotMagicContinued( this, vStartPos, qStartRot, m_penEnemy.ep_pen, 35.0F,
															szHitEffect[m_ChaList][iHitType], "mMissileEnerBall",
															false, 0 );
									}
								}
								else
								{
									SendEvent(EChaAttackDamage());
								}
							}
							else if( IsEXMage(m_ChaList) )	// 2013/01/21 jeil EX������ �߰� 
							{
								FLOAT3D		vStartPos;
								FLOATquat3D	qStartRot;
								CTag		*pTag = NULL;

								if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "WAND" );
									if( pTag == NULL )
									{
										pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
									}
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_ATTACK_2] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "LHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_1] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}
								else if( idBodyAnim == m_AnimationIndices.m_iAnimation[ANIM_EXT_ATTACK_2] )
								{
									pTag = this->en_pmiModelInstance->m_tmSkaTagManager.Find( "RHAND" );
									if( pTag != NULL )
									{
										vStartPos = pTag->CurrentTagInfo().m_vPos;
										qStartRot = pTag->CurrentTagInfo().m_qRot;
									}
								}

								if( pTag != NULL )
								{
									ShotMagicContinued( this, vStartPos, qStartRot, m_penEnemy.ep_pen, 35.0F,
														szHitEffect[m_ChaList][iHitType], "mMissileEnerBall_G",
														false, 0 );
								}

							}
							// �ٰŸ��� �����ϴ� ��쿡 ���� ó��.
							else
							{
								SendEvent(EChaAttackDamage());
							}
							m_fImpactTime = 1000.0f;//����� ��ð�.
						}
					}

					resume; 
				}
				on (EChaAttackDamage eAttackDamage) : 
				{
					CEntity *pen = m_penEnemy;
					FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
						((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
						((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
					FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
					FLOAT3D vTargetCenter(0, 0, 0);
					FLOAT3D vTargetPos(0, 0, 0);
					FLOAT3D vDirection(0, 0, 0);
					FLOAT size = 0;
					if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
					{
						if(pen->en_RenderType == RT_SKAMODEL)
						{
							// Bounding Box�� �̿��Ͽ� Effect�� ���� ��ġ�� ã�´�.
							FLOATaabbox3D bbox;
							pen->GetModelInstance()->GetCurrentColisionBox(bbox);
							FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*�ܼ��� ������ �ϸ� �ʹ� ŭ. ������ ���� ���*/;
							size = vRadius.Length();
						}
						vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
							((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
							((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
						vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
						vDirection = vTargetPos - vCurrentPos;
						vDirection.Normalize();
						vTargetPos -= vDirection * size;
					}
					vDirection.Normalize();
					if(m_penEnemy != NULL)//0817 ���� ����.
					{
						this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
					}					
					m_fImpactTime = 1000.0f;//����� ��ð�.

					// NOTE : �� �κ��� �� �ִ� ���� �𸣰���..-_-;;;
					if(m_bKillEnemy)
					{
						if(m_penKillEnemy!=NULL)
						{
							if(m_penKillEnemy->IsEnemy())
							{
								if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
								{
									CMobData* MD = CMobData::getData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
									if(!MD->IsCastleTower())
									{
										((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
									}
									else
									{
										m_penKillEnemy->SetFlagOff(ENF_ALIVE);
									}
								}
							}
							else
							{
								if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
								{
									((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
								}
							}
						}
						m_penKillEnemy = NULL;
						m_bKillEnemy = FALSE;
					}

					stop;
				}
				on (ETimer) : { stop; }			
			}
		}

		// NOTE : �� �κ��� �� �ִ� ���� �𸣰���..-_-;;;
		if(m_bKillEnemy)
		{
			if(m_penKillEnemy!=NULL)
			{
				if(m_penKillEnemy->IsEnemy())
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						CMobData* MD = CMobData::getData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
						if(!MD->IsCastleTower())
						{
							((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
						}
						else
						{
							m_penKillEnemy->SetFlagOff(ENF_ALIVE);
						}
					}
				}
				else
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
					}
				}
			}
			m_penKillEnemy = NULL;
			m_bKillEnemy = FALSE;
		}

		m_bAttack = FALSE;
		m_bMoving = FALSE;
		m_bStop = FALSE;		
		StandingAnim();
		SetNoTargetEntity();	

		return EReturn();
	};

	SkillingTarget(EVoid) : CCharacterBase::SkillingTarget // �� ���ν����� call�ϱ����� SetTargetEntity()�� ����Ǿ�� �Ѵ�.
	{
		if (m_penEnemy != NULL)
		{
			m_vDesiredPosition = PlayerDestinationPos();
		}

		autocall SkillAnimation() EReturn;

		return EReturn();
	};
	
	SpellSkill(EVoid) : CCharacterBase::SpellSkill
	{
		m_vDesiredPosition = PlayerDestinationPos();
		
		autocall PreSkillAnimation() EReturn;
	
		return EReturn();
	};

	ProduceAnimation(EVoid): CCharacterBase::ProduceAnimation //1221
	{
		CModelInstance *pMI = GetModelInstance();
		if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
		{
			AppearWearingWeapon(FALSE);
			m_bDisappearWeapon = FALSE;
		}
		if(m_nPlayProduceNum == 1)//ä��
		{
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_MINE], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_MINE],AN_CLEAR,1.0f,0);
			m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_MINE]);
		}
		else if(m_nPlayProduceNum == 2)//ä��
		{
			//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_COLLECT],AN_CLEAR,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_COLLECT], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_COLLECT]);
		}
		else if(m_nPlayProduceNum == 3)//����.
		{
			//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER],AN_CLEAR,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_GATHER]);
		}
		else
		{
			IdleAnim();			
		}
		if((pMI->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_MINE])|| 
			pMI->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_COLLECT]) || pMI->IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_GATHER])))
		{
			autowait(m_fActionAnimTime-0.1f);
			//autowait(m_fActionAnimTime*0.9f);

			NewClearState(0.1f);  
			StandingAnim();				
		}		
		return EReturn();
	};
	
	// WSS_DRATAN_SEIGEWARFARE 2007/08/01
	ConsensusAnimation(EVoid): CCharacterBase::ConsensusAnimation //1221
	{
		CModelInstance *pMI = GetModelInstance();
		//pMI->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER],AN_CLEAR,1.0f,0);	
		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_GATHER], AN_CLEAR|AN_LOOPING, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		m_fActionAnimTime = pMI->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_GATHER]);
		return EReturn();
	};

//0824 �׼� �߰�.
	// FiXME : �Ʒ� ��ƾ ������ �ʿ���...��.��
	ActionAnimation(EVoid): CCharacterBase::ActionAnimation
	{
		if( (m_nPlayActionNum != ACTION_NUM_SITDOWN) && 
			(m_nPlayActionNum != ACTION_NUM_SELL) && 
			(m_nPlayActionNum != ACTION_NUM_PICKITEM) )
		{
			DeleteWearingWeapon(FALSE, FALSE);

			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				pTarget->cha_itemEffect.DeleteEffect(WEAR_WEAPON);
			}
		}
		else
		{
			if(m_bDisappearWeapon)//���Ⱑ ������ �ִٸ�, ��Ÿ���� ����� �Ѵ�.
			{
				AppearWearingWeapon(FALSE);
				m_bDisappearWeapon = FALSE;
				
				ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, GetNetworkID());

				if (pObject != NULL && en_pmiModelInstance != NULL)
				{
					CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

					pTarget->cha_itemEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 
						(pTarget->m_nType == NIGHTSHADOW) ? 1 : pTarget->cha_sbItemEffectOption);
					pTarget->cha_statusEffect.Refresh(&GetCurrentPlayerModelInstance()->m_tmSkaTagManager, 
						CStatusEffect::R_NONE);
				}
			}
		}		

		if(m_nPlayActionNum == ACTION_NUM_SITDOWN) //�ɱ�,����.
		{
			if(m_nSitDown == 0) //����.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_STANDUP];
					}
					else
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_STANDUP];
					}
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_STANDUP];					
				}
				AppearWearingWeapon(m_bSkilling);
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);				
			}
			else//�ɱ�.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT];
					}
					else
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT];
					}					
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT];
				}
				if(m_ChaList == NIGHTSHADOW )
				{
					DeleteWearingWeapon(FALSE,m_bSkilling);
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);		
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(iAnim,AN_NORESTART,1.0f,0);					
			}			
			
		}
		else if(m_nPlayActionNum == ACTION_NUM_SELL)
		{
			if(m_nSitDown == 0) //����.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_STANDUP];
					}
					else 
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_STANDUP];
					}
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_STANDUP];					
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
				//GetModelInstance()->AddAnimation(iAnim,AN_NORESTART,1.0f,0);				
			}
			else//�ɱ�.
			{
				INDEX iAnim = -1;
				if( m_bRide )
				{
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT];
					}
					else
					{
						iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT];
					}					
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT];										
				}
				if(m_ChaList == NIGHTSHADOW )
				{
					DeleteWearingWeapon(FALSE,m_bSkilling);
				}
				m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnim);				
				AddAnimation(iAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);				
			}			
		}
		else if(m_nPlayActionNum == ACTION_NUM_PICKITEM)
		{
			INDEX iAnimationPick = -1;
			if( !m_bExtension || m_bMobChange )
			{
				if( m_bRide )
				{
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK];
						}
						else
						{
							iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_PICK];
						}
					}
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK];
					}
					else
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_PICK];
					}					
				}
				else
				{
					iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_PICK];
				}
			}
			else
			{
				if( m_bRide )
				{
					if( m_bWildRide )
					{
						if( m_nWildPetType == 2 )
						{
							iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK];
						}
						else
						{
							iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DEMONBAT_PICK];
						}
					}
					// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
					else if( m_iRideType%2 == CPetInfo::PET_HORSE )
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_PICK];
					}
					else
					{
						iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_PICK];
					}					
				}
				else
				{
					iAnimationPick = m_AnimationIndices.m_iAnimation[ANIM_EXT_PICK];
				}
			}
			//GetModelInstance()->AddAnimation(iAnimationPick,AN_NORESTART,1.0f,0);	
			AddAnimation(iAnimationPick, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(iAnimationPick);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_GREET)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_0],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_0], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_0]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_SMILE)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_1],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_1], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_1]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_CRY)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_2],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_2], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_2]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_NUMBER_ONE)//�ְ�
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_3],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_3], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_3]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_HANDCLAP)//�ڼ�
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_4],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_4], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_4]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_REFUSE)//����
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_5],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_5], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_5]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_GOOD_LOOKS || m_nPlayActionNum ==  ACTION_NUM_GOOD_LOOKS2)//��ü��
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_6],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_6], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_6]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_BANTER)//����
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_7],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_7], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_7]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_CHEER)//ȯȣ
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_8],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_8], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_8]);
		}
		else if(m_nPlayActionNum ==  ACTION_NUM_COMBAT)//����
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_9],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_9], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_9]);
		}

		else if(m_nPlayActionNum ==  ACTION_NUM_SUBMISSION)//����
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_10],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_10], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_10]);
		}
		else if(m_nPlayActionNum == ACTION_NUM_WATER_SPREAD)
		{
			//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_11],AN_NORESTART,1.0f,0);	
			AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_11], AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			m_fActionAnimTime = GetModelInstance()->GetAnimLength(m_AnimationIndices.m_iAnimation[ANIM_SOCIAL_11]);
		}

		autowait(m_fActionAnimTime-0.1f);

		if( (m_nPlayActionNum == 3 || m_nPlayActionNum == ACTION_NUM_SELL) && m_nSitDown==2)
		{
			INDEX iAnim = -1;
			if( m_bRide )
			{				
				// ���� �� �߰��� ¦�� Ÿ���� ��...060822 wooss
				if( m_iRideType%2 == CPetInfo::PET_HORSE )
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_HORSE_SIT_CONTINUE];
				}
				else
				{
					iAnim = m_AnimationIndices.m_iAnimation[ANIM_RIDE_DRAGON_SIT_CONTINUE];
				}

			}
			else
			{
				iAnim = m_AnimationIndices.m_iAnimation[ANIM_SIT_CONTINUE];
				if(m_ChaList == NIGHTSHADOW )
				{
					DeleteWearingWeapon(FALSE,m_bSkilling);
				}
				
			}	
			AddAnimation(iAnim, AN_LOOPING|AN_NORESTART|AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
			//GetModelInstance()->AddAnimation(iAnim,AN_LOOPING|AN_NORESTART|AN_CLEAR,1.0f,0);			
		}
		else
		{
			AppearWearingWeapon(FALSE);//0127
			NewClearState(0.1f);  
			StandingAnim();				
		}
			
		return EReturn();
	};

	PreSkillAnimation(EVoid)
	{		
		m_vDesiredPosition = PlayerDestinationPos();
		
		StopMoving();//0831

		if (PreSkillAnimationEx(TRUE)) // ����Ʈ ������ ��ų �ϵ� �ڵ�
		{
			PreSkillAnimationEx(FALSE);
		}
		else
		{
			m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetReadyAnim(m_bExtension));

			if (m_idCurrentSkillAnim >= 0)
			{
				m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;

				AddAnimation(m_idCurrentSkillAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
				m_tmSkillStartTime = _pTimer->CurrentTick();
			}
			else
			{
				m_fSkillAnimTime = 0.f;
			}

	//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
			m_nEffectStep = 0;
			m_saLastEffectInfo.Clear();
	//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
		}

		wait(m_fSkillAnimTime) {
			on (EBegin) : { 
				if (!PreSkillAnimationEx(TRUE))
				{ // ���� ���� ��ų
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
					CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);
					if(m_nEffectStep == 0)
					{
						++m_nEffectStep;
						m_pSkillReadyEffect = StartEffectGroup(skill.GetReadyEffect(m_bExtension)
										, &en_pmiModelInstance->m_tmSkaTagManager
										, _pTimer->GetLerpedCurrentTick());
						if(m_pSkillReadyEffect) {m_pSkillReadyEffect->SetSpeedMul(m_fSkillSpeed);}
					}
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
				}
				resume; 
			};
			on (ETimer) : {
				if (m_bStillAction && PreSkillAnimationEx(TRUE))
				{ // �ҿ� �巹���� StillAction
					m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetStillAnim(m_bExtension));
					m_tmSkillStartTime = _pTimer->GetLerpedCurrentTick();
					AddAnimation(m_idCurrentSkillAnim, AN_NORESTART|AN_CLEAR, 1.0f, 0, ESKA_MASTER_MODEL_INSTANCE, 1.0f);
					StopEffectGroupIfValid(m_pSkillReadyEffect, 0.1f);
					m_fSkillAnimTime =  _pNetwork->GetSkillData(m_nCurrentSkillNum).GetDestDelay(m_bExtension);

					if (m_bAttachPlayerEffect)
					{
						m_pAttachPlayerEffect = StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect1(_pNetwork->MyCharacterInfo.bExtension),
							&en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // PJT_SOUL_DRAIN1 : This
					}

					if (m_bAttachEnemyEffect && m_penEnemy != NULL)
					{
						m_pAttachEnemyEffect = StartEffectGroup(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension),
							&m_penEnemy->GetModelInstance()->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick()); // SHK_SOUL_DRAIN1 : Enemy
					}
					
					m_pSkillTexture.SetData_t(CTFILENAME("Data\\Models\\Weapons\\GhostBuster\\Projectile\\Pink_Ray.tex"));
					m_penStillTarget = m_penEnemy;
					m_bConnectEffect = TRUE;
				}
				stop;
			};
		}

		//autowait(0.1f);//0114 �ӽ��ڵ� still�ִϸ��̼� 0.1��

		return EReturn();
	}

	SkillAnimation(EVoid) //0628
	{
		//LOG_DEBUG("### SkillAnimation");

		BOOL bSkillEx = FireSkillAnimation(TRUE);

		if (m_nCurrentSkillNum == 688) // ���ེų
		{ 
			// �׼� ���
			if (IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_FLYING_READY]) ||
				IsAnimationPlaying(m_AnimationIndices.m_iAnimation[ANIM_FLYING_MOVE])) // ���� ��� �ִϸ��̼� ���̸� ���� ���ΰ���~
			{ // �������� state������ ���� �ع�����.
				m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(1));
				DelSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
				AppearWearingWeapon(TRUE);
			}
			else
			{
				m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(0));
				AddSkaModel(CTString("Data\\Item\\shield\\Nightshadow\\ns_fly.smc"));
				DeleteWearingWeapon(FALSE, TRUE);
			}
		}
		else
		{
			m_idCurrentSkillAnim = ska_StringToID(_pNetwork->GetSkillData(m_nCurrentSkillNum).GetFireAnim(m_bExtension));
		}

		if(m_idCurrentSkillAnim == -1 && !bSkillEx)
		{
			return EReturn();
		}

		m_bIdleAnim = TRUE;

		StopRotating();
		if( m_penEnemy == NULL )
		{
			if( m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated( m_pSkillReadyEffect ) )
			{
				m_pSkillReadyEffect->Stop( 0.1f );
				m_pSkillReadyEffect = NULL;
			}
		}

//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
		m_nEffectStep = 0;
		m_iSkillEffectStep = 0;
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)

		NewClearState(0.2f);    

		if(HardCodingSkill(TRUE))//�ϵ��ڵ�...���̾� ��Ʈ����ũ
		{
			m_fSkillAnimTime = 999.0f;
			m_tmSkillStartTime = _pTimer->CurrentTick();

			m_fAttackFrequency = 0.050f;
			m_fImpactTime = 2.3f;
		}
		else if (FireSkillAnimation(TRUE))
		{
			FireSkillAnimation(FALSE);
			
			while ((_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime < m_fSkillAnimTime))
			{
				wait(m_fSkillAnimTime*0.8f)
				{
					on (EBegin) :
					{
						resume;
					}

					on (ETimer) :
					{
						FireSkillAnimation(FALSE);
						stop;
					}
				}

				m_tmSkillStartTime = 0.0f;
				m_fSkillAnimTime = 0.0f;
			}
		}
		else
		{
			m_fSkillAnimTime = GetModelInstance()->GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed;
			m_tmSkillStartTime = _pTimer->CurrentTick();
			
			AddAnimation(m_idCurrentSkillAnim, AN_NORESTART, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE, m_fSkillSpeed);
			
			m_fAttackFrequency = 0.25f;
			m_fImpactTime = GetAnimLength(m_idCurrentSkillAnim) * m_fSkillSpeed / 2;//0630 �켱 ��ų �ִϸ��̼��� ���ݿ��� ����Ʈ.
		}

		if (!FireSkillAnimation(TRUE))
		{
			while(_pTimer->GetLerpedCurrentTick() - m_tmSkillStartTime < m_fSkillAnimTime)
			{
				wait(m_fAttackFrequency)
				{
					on (EBegin) : 
					{
						//LOG_DEBUG("### SkillAnim Begin");

						FLOAT tmTemp = _pTimer->GetLerpedCurrentTick();

						if(m_bMoving)
						{
							m_bHardcodingSkillLoop = FALSE;
							return EReturn();
						}

	//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
						CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);

						if(HardCodingSkill(TRUE))//�ϵ��ڵ�...���̾� ��Ʈ����ũ
						{
							HardCodingSkill(FALSE);
						}
						else
						{
							if(m_nEffectStep == 0)
							{
								++m_nEffectStep;

								CEffectGroup *pFireEffect = StartEffectGroup(skill.GetFireEffect1(m_bExtension), &en_pmiModelInstance->m_tmSkaTagManager, _pTimer->GetLerpedCurrentTick());
								if(pFireEffect) {pFireEffect->SetSpeedMul(m_fSkillSpeed);}
								if(m_pSkillReadyEffect != NULL && CEffectGroupManager::Instance().IsValidCreated(m_pSkillReadyEffect))
								{
									for(INDEX i=0; i<m_pSkillReadyEffect->GetEffectCount(); ++i)
									{
										CEffect *pEffect = m_pSkillReadyEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
										if(pEffect != NULL && pEffect->GetType() == ET_SKA)
										{
											CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
											TagInfo ti;
											ti.m_vPos = pSkaEffect->GetInstancePosition();
											ti.m_qRot = FLOATquat3D(1,0,0,0);
											INDEX cnt = m_saLastEffectInfo.Count();
											m_saLastEffectInfo.Expand(cnt + 1);
											m_saLastEffectInfo[cnt] = ti;
										}
									}
									m_pSkillReadyEffect->Stop(0.04f);
									m_pSkillReadyEffect = NULL;
								}
								else if(pFireEffect != NULL)
								{
									pFireEffect->Process(_pTimer->GetLerpedCurrentTick());
									for(INDEX i=0; i<pFireEffect->GetEffectCount(); ++i)
									{
										CEffect *pEffect = pFireEffect->GetEffectKeyVector()[i].m_pCreatedEffect;
										if(pEffect != NULL && pEffect->GetType() == ET_SKA)
										{
											CSkaEffect *pSkaEffect = (CSkaEffect*)pEffect;
											TagInfo ti;
											ti.m_vPos = pSkaEffect->GetInstancePosition();
											ti.m_qRot = FLOATquat3D(1,0,0,0);
											INDEX cnt = m_saLastEffectInfo.Count();
											m_saLastEffectInfo.Expand(cnt + 1);
											m_saLastEffectInfo[cnt] = ti;
											pSkaEffect->SetLifeTime(0);
										}
									}
								}
							}
							
							ASSERT(skill.GetDelay(m_nEffectStep-1, m_bExtension) < GetAnimLength(m_idCurrentSkillAnim) && "SkillAnimTime�� �ݵ�� MissileSpeedFireTime���� Ŀ���մϴ�.");

							FLOAT tmpTime = _pTimer->CurrentTick() - m_tmSkillStartTime;
							FLOAT tmpAnim = GetAnimLength(m_idCurrentSkillAnim);

							// �� �κп��� iSkillEffectStep�� ���� 1~skill.GetDelayCount()�̴�.
							if( m_nEffectStep <= skill.GetDelayCount(m_bExtension)
							 && _pTimer->CurrentTick() - m_tmSkillStartTime > skill.GetDelay(m_nEffectStep-1, m_bExtension) * m_fSkillSpeed)
							{
								++m_nEffectStep;
								if(skill.GetMissileType(m_bExtension) == CSkill::MT_NONE && m_penEnemy.ep_pen != NULL)	// ���� ���� - ���� ����
								{
									FLOAT3D vHitPoint;
									FLOAT3D vHitDir;
									GetTargetDirection(vHitPoint, vHitDir);

									if( m_dcEnemies.Count() > 0 )
									{
										DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
										m_dcEnemies.Clear();
									}
									else
									{
										//damage effect ó��
										if(m_penEnemy != NULL)//0817 ���� ����.
										{
											this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, vHitPoint, vHitDir);
										}
									}
									FLOAT3D vAxisY(0,1,0);
									FLOAT angle = acos(vHitDir % vAxisY);
									FLOAT3D axis = vAxisY * vHitDir;
									axis.Normalize();
									FLOATquat3D quat;
									quat.FromAxisAngle(axis, angle);
									StartEffectGroup(skill.GetFireEffect3(m_bExtension), _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
								}
								else if(skill.GetMissileType(m_bExtension) == CSkill::MT_ARROW && m_penEnemy.ep_pen != NULL)	// ȭ��
								{
									ASSERT(en_pmiModelInstance != NULL);

									FLOAT fHoriOffset = 0.0f;
									FLOAT fVertiOffset = 0.0f;
									if(skill.GetDelayCount(m_bExtension) > 1)
									{
										FLOAT fDist = (m_penEnemy->en_plPlacement.pl_PositionVector - en_plPlacement.pl_PositionVector).Length();
										FLOAT factor = 1.3f * fDist / skill.GetFireRange();
										factor = Clamp(factor, 0.0f, 1.0f);
										if(m_nEffectStep == 2) {fHoriOffset = +1.0f * factor;}
										if(m_nEffectStep == 3) {fHoriOffset = -1.0f * factor;}
										//if(m_nEffectStep == 4) {fVertiOffset = +0.0f * factor;}
										if(m_nEffectStep == 5) {fVertiOffset = -1.0f * factor;}
									}
									if( m_bExtension && ( m_ChaList != ROGUE && !IsEXRogue(m_ChaList)))	// [2012/08/27 : Sora] EX�α� �߰�
									{
										if( m_dcEnemies.Count() > 0 )
										{
											for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
												it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
											{																		
												CEntity *pEn = (*it);
												if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
												{
													ShotMissile(this, "STAFF", pEn, skill.GetMissileSpeed(m_bExtension)
														, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
														, false, fHoriOffset, fVertiOffset, 0);
												}
											}
											m_dcEnemies.Clear();
										}
										else
										{
											ShotMissile(this, "STAFF", m_penEnemy.ep_pen, skill.GetMissileSpeed(m_bExtension)
														, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
														, false, fHoriOffset, fVertiOffset, 1);
										}
									}
									else
									{
										if( m_dcEnemies.Count() > 0 )
										{
											for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
												it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
											{																		
												CEntity *pEn = (*it);
												if(pEn != NULL && pEn->IsFlagOff(ENF_DELETED))
												{
													ShotMissile(this, "RHAND", pEn, skill.GetMissileSpeed(m_bExtension)
														, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
														, false, fHoriOffset, fVertiOffset, 0);
												}
											}
											m_dcEnemies.Clear();
										}
										else
										{
											ShotMissile(this, "RHAND", m_penEnemy.ep_pen, skill.GetMissileSpeed(m_bExtension)
														, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
														, false, fHoriOffset, fVertiOffset, 0);
										}
									}
								}
								else if(skill.GetMissileType(m_bExtension) == CSkill::MT_DIRECT && m_penEnemy.ep_pen != NULL)	// �߻�ü ����, ������ ����, ���� ȿ�� ��
								{
									if( m_penEnemy->GetFlags() & ENF_ALIVE && m_penEnemy->en_pmiModelInstance != NULL)
									{
										StartEffectGroup(skill.GetFireEffect3(m_bExtension)
														, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
														, _pTimer->GetLerpedCurrentTick());
									}
								}
								else if(skill.GetMissileType(m_bExtension) == CSkill::MT_DIRECTDAMAGE && m_penEnemy.ep_pen != NULL)	// �߻�ü ����, ������ ����
								{
									if( m_dcEnemies.Count() > 0 )
									{						
										for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
											it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
										{									
											CEntity &en = *(*it);
											if(/*en.IsFlagOn(ENF_ALIVE) && */en.en_pmiModelInstance != NULL)
											{
												StartEffectGroup(skill.GetFireEffect3(m_bExtension)
																, &(en.en_pmiModelInstance->m_tmSkaTagManager)
																, _pTimer->GetLerpedCurrentTick());								
											}
										}

										// �������� �̱۴����� ��ų�� ���� ���� ���ױ淡...
										if(m_dcEnemies.Count() > 0)
										{
											FLOAT3D vHitPoint;
											FLOAT3D vHitDir;
											GetTargetDirection(vHitPoint, vHitDir);
											DamagedTargetsInRange(this, m_dcEnemies, DMT_EXPLOSION, 1, vHitPoint, TRUE);
											m_dcEnemies.Clear();
										}
									}
									else
									{
										this->InflictDirectDamage( m_penEnemy, this, DMT_NONE, 1, FLOAT3D(0,0,0), FLOAT3D(0,0,0) );

										if(m_penEnemy->en_pmiModelInstance != NULL)
										{
											StartEffectGroup( skill.GetFireEffect3(m_bExtension)
															, &m_penEnemy->en_pmiModelInstance->m_tmSkaTagManager
															, _pTimer->GetLerpedCurrentTick() );
										}
									}
								}
								else if( (skill.GetMissileType(m_bExtension) == CSkill::MT_CONTINUE || 
										skill.GetMissileType(m_bExtension) == CSkill::MT_MAGECUTTER )
									&& m_penEnemy != NULL && m_saLastEffectInfo.Count() != 0)	// �������� ����Ʈ�� �߻�ü�� �Ǿ� ���ư�
								{
									if( m_dcEnemies.Count() > 0 )
									{
										for( ENTITIES_ITERATOR it = m_dcEnemies.vectorSelectedEntities.begin();
											it != m_dcEnemies.vectorSelectedEntities.end(); ++it )
										{															
											CEntity &en = *(*it);
											if(en.IsFlagOff(ENF_DELETED))
											{
												ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
															, &en, skill.GetMissileSpeed(m_bExtension)
															, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
															, false, skill.GetMissileType(m_bExtension));
											}
										}
										m_dcEnemies.Clear();
									}
									else
									{
										ShotMagicContinued(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
															, m_penEnemy, skill.GetMissileSpeed(m_bExtension)
															, skill.GetFireEffect3(m_bExtension), skill.GetFireEffect2(m_bExtension)
															, false, skill.GetMissileType(m_bExtension));
									}
									INDEX cnt = m_saLastEffectInfo.Count();
									CStaticArray<TagInfo> temp;
									temp.New(cnt-1);
									for(INDEX i=1; i<m_saLastEffectInfo.Count(); ++i)
									{
										temp[i-1] = m_saLastEffectInfo[i];
									}
									m_saLastEffectInfo = temp;
								}
								else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_GOWAITBACK)
								{
									if( m_penEnemy != NULL )
									{
										ShotGoWaitBack(this, m_saLastEffectInfo[0].m_vPos, m_saLastEffectInfo[0].m_qRot
											, m_penEnemy, skill.GetMissileSpeed(_pNetwork->MyCharacterInfo.bExtension)
											, skill.GetDestDelay(_pNetwork->MyCharacterInfo.bExtension)
											, skill.GetFireEffect3(_pNetwork->MyCharacterInfo.bExtension)
											, skill.GetFireEffect2(_pNetwork->MyCharacterInfo.bExtension)
											, false);
									}
									INDEX cnt = m_saLastEffectInfo.Count();
									CStaticArray<TagInfo> temp;
									temp.New(cnt-1);
									for(INDEX i=1; i<m_saLastEffectInfo.Count(); ++i)
									{
										temp[i-1] = m_saLastEffectInfo[i];
									}
									m_saLastEffectInfo = temp;
								}
								else if(skill.GetMissileType(_pNetwork->MyCharacterInfo.bExtension) == CSkill::MT_CONNECT)
								{
									if( m_penEnemy != NULL )
									{
										ShotConnect(this, m_penEnemy, skill.GetDestDelay(m_bExtension)
											, skill.GetFireEffect3(m_bExtension)
											, skill.GetFireEffect2(m_bExtension));
									}
								}
							}
							
						}

						m_fMoveSpeed = 0.0f;
						if(m_penEnemy != NULL
							&& skill.GetMissileType(m_bExtension) != CSkill::MT_NOTHING)
						{
							ULONG ulFlags = SetDesiredMovement();
						}

						resume; 
					}
					on (EChaSkillDamage eSkillDamage) : 
					{
						CEntity *pen = m_penEnemy;
						FLOAT3D vCurrentCenter(((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[0],
							((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[1],
							((EntityInfo*)(this->GetEntityInfo()))->vTargetCenter[2] );
						FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vCurrentCenter;
						FLOAT3D vTargetCenter(0, 0, 0);
						FLOAT3D vTargetPos(0, 0, 0);
						FLOAT3D vDirection(0, 0, 0);
						FLOAT size = 0;
						if(pen != NULL && pen->GetFlags()&ENF_ALIVE)
						{
							if(pen->en_RenderType == RT_SKAMODEL)
							{
								// Bounding Box�� �̿��Ͽ� Effect�� ���� ��ġ�� ã�´�.
								FLOATaabbox3D bbox;
								pen->GetModelInstance()->GetCurrentColisionBox(bbox);
								FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*�ܼ��� ������ �ϸ� �ʹ� ŭ. ������ ���� ���*/;
								size = vRadius.Length();
							}
							vTargetCenter = FLOAT3D(((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[0],
								((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[1],
								((EntityInfo*)(pen->GetEntityInfo()))->vTargetCenter[2] );
							vTargetPos = pen->en_plPlacement.pl_PositionVector + vTargetCenter;
							vDirection = vTargetPos - vCurrentPos;
							vDirection.Normalize();
							vTargetPos -= vDirection * size;
						}
						vDirection.Normalize();
						if( m_dcEnemies.Count() > 0 )
						{
							DamagedTargetsInRange(this, m_dcEnemies, DMT_CLOSERANGE, 1, vTargetPos, TRUE);
							m_dcEnemies.Clear();
						}
						else
						{
							if(m_penEnemy!=NULL)//0817 ���� ����.
							{
								this->InflictDirectDamage(m_penEnemy, this , DMT_CLOSERANGE, (int)1, vTargetPos, vDirection);
							}
						}
						m_fImpactTime = 1000.0f;//����� ��ð�.

						if(m_penKillEnemy)
						{
							if(m_penKillEnemy->IsEnemy())
							{
								if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
								{
									CMobData* MD = CMobData::getData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
									if(!MD->IsCastleTower())
									{
										((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
									}
									else
									{
										m_penKillEnemy->SetFlagOff(ENF_ALIVE);
									}
								}
							}
							else
							{
								if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
								{
									((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
								}
							}
							m_penKillEnemy = NULL;
							m_bKillEnemy = FALSE;
	//							CPrintF("DEATH NOW !!!!!!!!!!!!!!!!!!!!!!\n");
						}

						stop;
					}
					on (EReconsiderBehavior) :
					{
						//LOG_DEBUG("### EReconsiderBehavior resume");

						if (m_bSkilling == TRUE || m_bPreSkilling == TRUE || m_bMoving == TRUE || m_bAttack == TRUE)
						{
							// ��ų �ִϸ��̼��� ������ ���� �� ��Ȳ�� �߻� �ϸ� ������ ����� ���� �ʱ� ������ �������� ������ش�.
							if(m_penEnemy != NULL)
							{
								FLOAT3D vHitPoint;
								FLOAT3D vHitDir;
								GetTargetDirection(vHitPoint, vHitDir);

								this->InflictDirectDamage(m_penEnemy, this, DMT_NONE, 1, vHitPoint, vHitDir);

								CSkill &skill = _pNetwork->GetSkillData(m_nCurrentSkillNum);

								// ��Ʈ ����Ʈ�� ������ �ʾҴٸ� ��Ʈ ����Ʈ ���.
								if( m_nEffectStep <= skill.GetDelayCount(m_bExtension)
									&& _pTimer->CurrentTick() - m_tmSkillStartTime > skill.GetDelay(m_nEffectStep-1, m_bExtension) * m_fSkillSpeed)
								{
									if(skill.GetMissileType(m_bExtension) == CSkill::MT_NONE && m_penEnemy.ep_pen != NULL)	// ���� ���� - ���� ����
									{
										FLOAT3D vAxisY(0,1,0);
										FLOAT angle = acos(vHitDir % vAxisY);
										FLOAT3D axis = vAxisY * vHitDir;
										axis.Normalize();
										FLOATquat3D quat;
										quat.FromAxisAngle(axis, angle);
										StartEffectGroup(skill.GetFireEffect3(m_bExtension), _pTimer->GetLerpedCurrentTick(), vHitPoint, quat);
									}
								}
							}

							SkillAnimationStop();
							return EReturn();
						}

						resume;
					}				
					on (ETimer) : 
					{
						//LOG_DEBUG("### ETimer stop");
						stop;
					}
				}
			}
	//		}
		}

		// NOTE : �� �κ��� �� �ִ� ���� �𸣰���..-_-;;;
		if(m_bKillEnemy)
		{
			if(m_penKillEnemy!=NULL)
			{
				if(m_penKillEnemy->IsEnemy())
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						CMobData* MD = CMobData::getData(((CEnemy*)(CEntity*)m_penKillEnemy)->m_nMobDBIndex);
						if(!MD->IsCastleTower())
						{
							((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
						}
						else
						{
							m_penKillEnemy->SetFlagOff(ENF_ALIVE);
						}
					}
				}
				else
				{
					if(((CUnit*)((CEntity*)m_penKillEnemy))->m_nCurrentHealth <= 0)
					{
						((CUnit*)((CEntity*)m_penKillEnemy))->DeathNow();
					}
				}
			}
			m_penKillEnemy = NULL;
			m_bKillEnemy = FALSE;
		}
		m_bMoving = FALSE;
		
		m_bStop = FALSE;
		m_bSkilling = FALSE;
		
 		if(m_bIdleAnim)
 		{
 			StandingAnim();
 		}
		SetNoTargetEntity();

		return EReturn();
	};
		
		
	// Normal attack
	NormalAttack(EVoid)
	{
		StandingAnim();
		autowait(0.8f + FRnd()*0.25f);

		AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1], AN_CLEAR, 1.0f, 0x03, ESKA_MASTER_MODEL_INSTANCE);
		//GetModelInstance()->AddAnimation(m_AnimationIndices.m_iAnimation[ANIM_ATTACK_1],AN_CLEAR,1.0f,0);
		//PlaySound(m_soSound, SOUND_ATTACK, SOF_3D);

		autowait(0.90f);
		if(m_penEnemy)
		{
			FLOAT3D vEnemyCenter( ((EntityInfo*)m_penEnemy->GetEntityInfo())->vSourceCenter[0], ((EntityInfo*)m_penEnemy->GetEntityInfo())->vSourceCenter[1], ((EntityInfo*)m_penEnemy->GetEntityInfo())->vSourceCenter[2] );
			FLOAT3D vMyCenter( ((EntityInfo*)this->GetEntityInfo())->vSourceCenter[0], ((EntityInfo*)this->GetEntityInfo())->vSourceCenter[1], ((EntityInfo*)this->GetEntityInfo())->vSourceCenter[2] );
			FLOAT size = 0;

			if(m_penEnemy->en_RenderType == RT_SKAMODEL)
			{
				// Bounding Box�� �̿��Ͽ� Effect�� ���� ��ġ�� ã�´�.
				FLOATaabbox3D bbox;
				m_penEnemy->GetModelInstance()->GetCurrentColisionBox(bbox);
				FLOAT3D vRadius = (bbox.maxvect - bbox.minvect) * 0.5f * 0.4f/*�ܼ��� ������ �ϸ� �ʹ� ŭ. ������ ���� ���*/;
				size = vRadius.Length();
			}
			FLOAT3D vTargetPos = m_penEnemy->en_plPlacement.pl_PositionVector + vEnemyCenter;
			FLOAT3D vCurrentPos = this->en_plPlacement.pl_PositionVector + vMyCenter;

			FLOAT3D vDirection = vTargetPos - vCurrentPos;
			vDirection.Normalize();
			vTargetPos -= vDirection * size;
			if(m_penEnemy!=NULL)//0817 ���� ����.
			{
				this->InflictDirectDamage(m_penEnemy, this, DMT_CLOSERANGE, 1, vTargetPos, vDirection);
			}
		}
		autowait(0.60f);

		return EEnd();
	};

	Polymoph(EVoid): CCharacterBase::Polymoph //����.
	{
		m_fActionAnimTime = _pTimer->CurrentTick();
		m_bChanging = TRUE;
		m_bOfTransforming = TRUE;
		m_iPolymophStep = 0;
		m_bTransformMinimize = FALSE;
		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopMoving(); 

		while(m_bChanging == TRUE)//���� �ð��� ���� 1.8��
		{

			wait(0.1f) {
				on (EBegin) : {
					if(!m_bChanging)
					{
						if(m_bChangingTwice)
						{
							m_bChangingTwice	= FALSE;
							m_bChanging			= TRUE;
							m_nChangeMonsterId	= m_nChangeMonsterIdTwice;
							resume;
						}
						return EReturn();
					}
					MinMaximize();
					resume; 
				}

				on (ETimer) : { stop; }	
			}
		}

		return EReturn();
	}

	Evocate(EVoid) : CCharacterBase::Evocate
	{
		m_fActionAnimTime = _pTimer->CurrentTick();
		m_bTransforming = TRUE;
		
		m_vDesiredPosition = GetPlacement().pl_PositionVector;
		StopMoving(); 

		while(_pTimer->CurrentTick() - m_fActionAnimTime < 1.8f)//���� �ð��� ���� 1.8��
		{
			wait(0.1f) 
			{
				on (EBegin) : 
				{
					if(!m_bTransforming)
					{
						return EReturn();
					}
					if(GetFlags() & ENF_ALIVE)
					{
						Transforming();
					}
					resume; 
				}

				on (ETimer) : { stop; }	
			}
		}

		return EReturn();
	};

};