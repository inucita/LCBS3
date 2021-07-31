/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _Classes_PlayerEntity_INCLUDED
#define _Classes_PlayerEntity_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CPlayerEntity_DLLClass;
class DECL_DLL  CPlayerEntity : public CMovableModelEntity {
public:
	 virtual void SetDefaultProperties(void);
	FLOAT en_tmPing;
CPlayerCharacter en_pcCharacter;
CPlacement3D en_plViewpoint;
CPlacement3D en_plLastViewpoint;
   
#line 27 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
void InitializePlayer(ULONG ulWeaponsID,ULONG ulAnimatorID);
  
#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetWeaponAndAnimator();
  
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsPlayer();
  
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void DumpToConsole();
  
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Mob(int index,CNetworkMessage * istr);
  
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Character(int type,int sub_type,CNetworkMessage * istr);
  
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Pet(int index,CNetworkMessage * istr);
  
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_WildPet(int index,CNetworkMessage * istr);
  
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Summon(int index,CNetworkMessage * istr);
  
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Damage_Character(CNetworkMessage * istr);
  
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_Damage(SBYTE sbType,CNetworkMessage * istr);
  
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_DissappearEffect(CNetworkMessage * istr);
  
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_StatusPC(CNetworkMessage * istr);
  
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_StatusNPC(CNetworkMessage * istr);
  
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_StatusPet(CNetworkMessage * istr);
  
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Read_net_StatusElemental(CNetworkMessage * istr);
  
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ProcessProduce(SBYTE sbChaType,SLONG slChaIndex,SBYTE sbTargetType,SLONG slTargetIndex,SLONG slTargetHP);
  
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void StandDown(BOOL bStand);
  
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void OpenGate(int iGateNumber,BOOL bOpenGate);
  
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetMobData(CEntity * penEntity,SLONG hp,SLONG maxHp,int level,BOOL bNpc,int mobIdx);
  
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetChaData(int index,int type,ULONG hp,ULONG maxHp,SBYTE hairstyle,SBYTE facestyle,CTString & strName,UWORD state,SBYTE pkTitle,SLONG pkstate);
  
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetPetData(CEntity * penEntity,SLONG hp,SLONG maxHP);
  
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetWildPetData(CEntity * penEntity,SLONG hp,SLONG maxHP);
  
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetElementalData(CEntity * penEntity,SLONG hp,SLONG maxHP);
  
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetElementalStatus(CEntity * penEntity,SBYTE sbAttackSpeed,SBYTE sbMagicSpeed,LONG lSkillSpeed,FLOAT fWalkSpeed,FLOAT fRunSpeed,FLOAT fAttackRange);
  
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetShopData(INDEX index,SBYTE sbShopType);
  
#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetImportantValues(FLOAT fWalkspeed,FLOAT fRunspeed,FLOAT fAttackrange,SBYTE sbAttackspeed,SBYTE sbMagicspeed);
  
#line 81 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual FLOAT GetWalkspeed();
  
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual FLOAT GetRunspeed();
  
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual FLOAT GetAttackrange();
  
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual SBYTE GetAttackspeed();
  
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual SBYTE GetMagicspeed();
  
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetSkillCancel(BOOL bCanSkillCancel);
  
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void StartCamera(CEntity * pTarget,BOOL bRestart);
  
#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetWideScreen(CEntity * pTarget,BOOL bWide,FLOAT fFov);
  
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void GetProjection(CAnyProjection3D & apr);
  
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void LostTarget();
  
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void UseSkill(int skillIndex);
  
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void UsePetPickItem();
  
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void UsePetSkill(int skillIndex);
  
#line 97 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void UseSlaveSkill(CEntity * pEntity,int skillIndex);
  
#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void UseWildSkill(CEntity * pEntity,int skillIndex);
  
#line 100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void CancelSkill(BOOL bLostTarget,BOOL bAutoAttack,BOOL bSkillError);
  
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void LoadSkillAnimID();
  
#line 102 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void UseAction(int ActionIndex);
  
#line 103 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void StopMove();
  
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ClearMove();
  
#line 107 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetProduct(SLONG slChaIndex,SBYTE slProductType);
  
#line 108 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetConsensus(SLONG slChaIndex);
  
#line 109 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void WarpTeleport(FLOAT fX,FLOAT fZ,FLOAT fH,FLOAT fR,SBYTE sbLayer);
  
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SingleModeOn();
  
#line 112 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void FieldModeOn();
  
#line 114 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetLegit(UBYTE sbType,SLONG chaindex);
  
#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetRepairLegit(SLONG chaindex,CCharacterTarget * Ct);
  
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsLegit();
  
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ClearMultiTargets();
  
#line 119 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void RidingPet(CEntity * pCharacter,INDEX iPetType);
  
#line 120 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void LeavingPet(CEntity * pCharacter);
  
#line 121 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsRidePet(CEntity * pCharacter);
  
#line 123 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void RidingWildPet(CEntity * pCharacter,CEntity * pWildPet,CTString strFileName);
  
#line 124 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void LeavingWildPet(CEntity * pCharacter);
  
#line 127 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsMoving();
  
#line 128 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsActionSitting();
  
#line 129 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsIdle();
  
#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual const BOOL IsTransform()const;
  
#line 133 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ClearTargetInfo(CEntity * penEntity);
  
#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetCharacterHairChange(INDEX nSIdx,SBYTE sbType,BOOL IsMe);
  
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetCharacterAppearance(CModelInstance * pMI,int iJob,SBYTE sbHairStyle,SBYTE sbFaceStyle);
  
#line 137 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ChangeFaceMesh(CModelInstance * pMI,int iJob,SBYTE sbType);
  
#line 138 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ChangeHairMesh(CModelInstance * pMI,int iJob,SBYTE sbType);
  
#line 141 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void CancelProduct();
  
#line 142 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void DeleteWearingWeapon(BOOL bException,BOOL bMode);
  
#line 143 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void AppearWearingWeapon(BOOL bSkillEnd);
  
#line 146 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void DeleteDefaultArmor(int type);
  
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void WearingDefaultArmor(int type);
  
#line 148 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void DeleteCurrentArmor(int weartype);
  
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetChaWearing(class CCharacterTarget * pTarget,CEntity * penEntity,CNetworkMessage * istr);
  
#line 150 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetCharCostumeWearing(class CCharacterTarget * pTarget,CEntity * penEntity,CNetworkMessage * istr);
  
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void WearingCostumeArmor(CNetworkMessage * istr);
  
#line 152 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void WearingCostumeArmorOneSuit(CNetworkMessage * istr);
  
#line 153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsHelmet(INDEX iCharIndex);
  
#line 154 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void Rebirth();
  
#line 155 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void DeathYou();
  
#line 156 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayerInit(bool bDeath);
  
#line 157 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsAlreadyDie();
  
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetDie();
  
#line 159 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetTarget(CEntity * penEntity);
  
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetTarget(SLONG targetID,SBYTE targetType);
  
#line 161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSameTarget(CEntity * penEntity);
  
#line 163 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetTargetMe();
  
#line 164 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayItemSound(BOOL bPick,BOOL bMoney);
  
#line 165 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayButtonSound();
  
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayJewelDullSound();
  
#line 167 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayJewelRefinementSound();
  
#line 168 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayJewelShinySound();
  
#line 169 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayPetStashEffectSound();
  
#line 170 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayPetStashCardSelSound();
  
#line 171 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayHolyWaterSound();
  
#line 173 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void CommandAttack();
  
#line 174 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL CommandAction();
  
#line 176 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayBGM(CTFileName fnFileName);
  
#line 178 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SearchNearItem();
  
#line 179 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetCharacterState(CEntity * cha_Entity,INDEX ch_state);
  
#line 181 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSkilling();
  
#line 182 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSitting();
  
#line 183 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsAttacking();
  
#line 184 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsPetActing();
  
#line 185 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsProduct();
  
#line 186 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetSpeedUp(BOOL bMe,INDEX index,BOOL bSpeedup);
  
#line 188 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckEscKey(void);
  
#line 189 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlayItemEffect(SLONG itemindex,SBYTE effecttype);
  
#line 190 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsSocialActing();
  
#line 191 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsPolymophing();
  
#line 192 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsTransforming();
  
#line 193 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsChanging();
  
#line 194 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void AppearChaPolymoph(CEntity * pentity,INDEX mobindex);
  
#line 195 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void TransfromPet(CEntity * pentity,INDEX mobIndex,INDEX mobSize);
  
#line 196 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ReturnTransformPet(CEntity * pentity);
  
#line 198 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void TransformSorcerer(int iType);
  
#line 199 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ReturnSorcerer();
  
#line 200 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void AppearChaTransform(CEntity * pEntity,INDEX iTransformType);
  
#line 203 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void PlAddAccessoryEffect(CEntity * penEntity,CCharacterTarget * pTarget);
  
#line 205 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void EnemyTargetSelected_InputTab();
  
#line 208 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void EvocationStart(LONG lIndex,int nSkillIndex);
  
#line 209 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void EvocationStop(LONG lIndex);
  
#line 210 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckChangeCondition(INDEX level,INDEX zone);
  
#line 211 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetSummonCommand(CEntity * pEntity,INDEX iCommand);
  
#line 213 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckNormalAttack(CEntity * pEntity,FLOAT fDist);
  
#line 214 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual CModelInstance * GetPlayerModelInstance(CEntity * pEntity);
  
#line 215 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckAttackTarget(INDEX iSkillIndex,CEntity * pEntity,FLOAT fDist);
  
#line 216 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL CheckEntityOfTarget(CEntity * pEntity,CEntity * pTarget);
  
#line 218 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ReturnChange(BOOL bAppear);
  
#line 222 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void AddDeathItem(CEntity * penEnemy,FLOAT3D pos,class CItemTarget * pItemTarget,class CItemData * pItemData);
  
#line 225 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ReceiveTriggerEvent(INDEX iIn,INDEX iOut,INDEX iContinued,CNetworkMessage * iStr);
  
#line 226 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void ReceiveRaidScene(CNetworkMessage * istr);
  
#line 227 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SendTriggerEvent(INDEX iTarget,INDEX eetEventType);
  
#line 228 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual CEntity * GetClickObject(void);
  
#line 230 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void DropDeathItem(CEntity * penEnemy);
  
#line 232 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_DrawModel(CDrawPort * pdp,FLOAT fMinX,FLOAT fMinY,FLOAT fMaxX,FLOAT fMaxY,INDEX iType,FLOAT fDistance,FLOAT fHeight);
  
#line 233 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual FLOAT GetAnimationTime();
  
#line 234 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_SetCharModelData(INDEX iJob,INDEX iHairStyle,INDEX iFaceStyle);
  
#line 235 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_SetModelData(INDEX iType,INDEX iIndex,INDEX iUIType);
  
#line 236 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_WearingModel(INDEX iType,INDEX iJob,INDEX iItem,BOOL bLogin);
  
#line 237 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_ChangeHairMesh(INDEX iJob,INDEX iHairStyle);
  
#line 238 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_ChangeFaceMesh(INDEX iJob,INDEX iFaceStyle);
  
#line 239 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_PlayAnimation(INDEX iJob,INDEX AnimID);
  
#line 240 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_PlayAnimation(CTString strAniName);
  
#line 241 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_SetModelColor(const COLOR ModelColor);
  
#line 242 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void HUD_SetTitleEffect(CTString strEffectName);
  
#line 243 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual BOOL IsHudModel_Used();
  
#line 244 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetHudModel_Use(BOOL bUse);
  
#line 245 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetDecoModeCamera(FLOAT fDistance,FLOAT fHeight);
  
#line 246 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
virtual void SetAppearanceData(int inde,SBYTE hairstyle,SBYTE facestyle);
   CTString GetPlayerName(void);
   const CTString & GetName(void)const;
   INDEX GetMyPlayerIndex(void);
   void DoMoving(void);
   void Copy(CEntity & enOther,ULONG ulFlags);
   void Read_t(CTStream * istr,BOOL bNetwork);
   void Write_t(CTStream * ostr,BOOL bNetwork);
  virtual void ApplyAction(const CPlayerAction & pa,FLOAT tmLatency);
  virtual void Disconnect(void);
  virtual void CharacterChanged(const CPlayerCharacter & pcNew);
#define  STATE_CPlayerEntity_Dummy 0x00040000
	BOOL 
#line 325 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/Engine/Classes/PlayerEntity.es"
Dummy(const CEntityEvent &__eeInput);
};
#endif // _Classes_PlayerEntity_INCLUDED