/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _PlayerAnimator_INCLUDED
#define _PlayerAnimator_INCLUDED 1
#include <EntitiesMP/Player.h>
#include <EntitiesMP/PlayerWeapons.h>
#include <EntitiesMP/Enemy.h>
#include <EntitiesMP/PetBase.h>
#define EVENTCODE_EAnimatorInit 0x01960000
class DECL_DLL EAnimatorInit : public CEntityEvent {
public:
	EAnimatorInit();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
	CEntityID eidPlayer;
};
DECL_DLL inline void ClearToDefault(EAnimatorInit &e) { e = EAnimatorInit(); } ;
extern "C" DECL_DLL CDLLEntityClass CPlayerAnimator_DLLClass;
class DECL_DLL  CPlayerAnimator : public CRationalEntity {
public:
virtual CEntity *GetPredictionPair(void) { return m_penPrediction; };
virtual void SetPredictionPair(CEntity *penPair) { m_penPrediction = penPair; };
	 virtual void SetDefaultProperties(void);
	CEntityPointer m_penPlayer;
	BOOL m_bReference;
	FLOAT m_fLastActionTime;
	INDEX m_iContent;
	BOOL m_bWaitJumpAnim;
	BOOL m_bCrouch;
	BOOL m_iCrouchDownWait;
	BOOL m_iRiseUpWait;
	BOOL m_bChangeWeapon;
	BOOL m_bSwim;
	INDEX m_iFlare;
	INDEX m_iSecondFlare;
	BOOL m_bAttacking;
	FLOAT m_tmAttackingDue;
	FLOAT m_tmFlareAdded;
	BOOL m_bDisableAnimating;
	FLOAT3D m_vLastPlayerPosition;
	FLOAT m_fEyesYLastOffset;
	FLOAT m_fEyesYOffset;
	FLOAT m_fEyesYSpeed;
	BOOL m_bMdl;
	FLOAT m_fWeaponYLastOffset;
	FLOAT m_fWeaponYOffset;
	FLOAT m_fWeaponYSpeed;
	BOOL m_bMoving;
	FLOAT m_fMoveLastBanking;
	FLOAT m_fMoveBanking;
	BOOL m_iMovingSide;
	BOOL m_bSidestepBankingLeft;
	BOOL m_bSidestepBankingRight;
	FLOAT m_fSidestepLastBanking;
	FLOAT m_fSidestepBanking;
	INDEX m_iWeaponLast;
	FLOAT m_fBodyAnimTime;
	INDEX m_iPlayerID;
	FLOAT m_fAttack1AnimTime;
	FLOAT m_fAttack2AnimTime;
	FLOAT m_fSkill1AnimTime;
	FLOAT m_fSkill2AnimTime;
	FLOAT m_fAttack3AnimTime;
	FLOAT m_fAttack4AnimTime;
	FLOAT m_fExtAttack1AnimTime;
	FLOAT m_fExtAttack2AnimTime;
	FLOAT m_fExtAttack3AnimTime;
	FLOAT m_fExtAttack4AnimTime;
	CEntityPointer m_penPrediction;
CModelObject * pmoModel;
  
#line 481 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
virtual void InitializeFromNet();
   
#line 487 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void Read_t(CTStream * istr,BOOL bNetwork);
   
#line 492 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void Precache(void);
   
#line 498 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
CPlayer * GetPlayer(void);
   
#line 502 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
CModelObject * GetBody(void);
   
#line 510 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
CModelObject * GetBodyRen(void);
   
#line 520 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void SetComponents(CModelObject * mo,ULONG ulIDModel,ULONG ulIDTexture,
#line 521 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture);
   
#line 548 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void AddAttachmentModel(CModelObject * mo,INDEX iAttachment,ULONG ulIDModel,ULONG ulIDTexture,
#line 549 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture);
   
#line 555 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void AddWeaponAttachment(INDEX iAttachment,ULONG ulIDModel,ULONG ulIDTexture,
#line 556 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
ULONG ulIDReflectionTexture,ULONG ulIDSpecularTexture,ULONG ulIDBumpTexture);
   
#line 562 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void SetAttachment(INDEX iAttachment);
   
#line 817 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void SetItem(CModelObject * pmo);
   
#line 835 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void SetBodyAnimation(INDEX iAnimation,ULONG ulFlags);
   
#line 851 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void InitializeProperties(void);
   
#line 897 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void StoreLast(void);
   
#line 908 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void AnimateBanking(void);
   
#line 965 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void AnimateSoftEyes(void);
   
#line 1003 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void ChangeView(CPlacement3D & pl);
   
#line 1035 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyAndHeadOrientation(CPlacement3D & plView);
   
#line 1057 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void WalkAnim();
   
#line 1164 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void RunAnim(float fRunSpeedMul);
   
#line 1272 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void PickAnim();
   
#line 1336 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void IdleAnim();
   
#line 1453 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void DamageAnim();
   
#line 1524 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void AnimatePlayer(void);
   
#line 2089 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void Rise(void);
   
#line 2105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void Fall(void);
   
#line 2130 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void Swim(void);
   
#line 2153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void Stand(void);
   
#line 2177 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void FireAnimation(INDEX iAnim,ULONG ulFlags);
   
#line 2220 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void FireAnimationOff(void);
   
#line 2224 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
BOOL IsEXRogue(int job);
   
#line 2229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
BOOL IsEXMage(int job);
   
#line 2274 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyWalkAnimation();
   
#line 2284 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyStillAnimation();
   
#line 2299 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void SetWholeAnimation(INDEX iAnimation,ULONG ulFlags,FLOAT fBlendTime,FLOAT fSpeed,BOOL bRun);
   
#line 2328 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyPushAnimation();
   
#line 2408 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyPullAnimation();
   
#line 2429 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyPullItemAnimation();
   
#line 2443 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyPickItemAnimation();
   
#line 2457 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void BodyRemoveItem();
   
#line 2469 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void OnPreRender(void);
   
#line 2490 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void ShowFlare(INDEX iAttachWeapon,INDEX iAttachObject,INDEX iAttachFlare);
   
#line 2502 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
void HideFlare(INDEX iAttachWeapon,INDEX iAttachObject,INDEX iAttachFlare);
#define  STATE_CPlayerAnimator_Main 1
	BOOL 
#line 2591 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
Main(const CEntityEvent &__eeInput);
	BOOL H0x01960001_Main_01(const CEntityEvent &__eeInput);
	BOOL H0x01960002_Main_02(const CEntityEvent &__eeInput);
#define  STATE_CPlayerAnimator_ReminderAction 0x01960003
	BOOL 
#line 2618 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerAnimator.es"
ReminderAction(const CEntityEvent &__eeInput);
};
#endif // _PlayerAnimator_INCLUDED
