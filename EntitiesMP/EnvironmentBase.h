/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _EntitiesMP_EnvironmentBase_INCLUDED
#define _EntitiesMP_EnvironmentBase_INCLUDED 1
#include <EntitiesMP/EnvironmentMarker.h>
#include <EntitiesMP/WatchPlayers.h>
extern "C" DECL_DLL CDLLEntityClass CEnvironmentBase_DLLClass;
class CEnvironmentBase : public CMovableEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
  DECL_DLL virtual void SetDefaultProperties(void);
  CTString m_strName;
  CTString m_strDescription;
  RANGE m_fDistance;
  FLOAT m_fStretch;
  CEntityPointer m_penTarget;
  CEntityPointer m_penWatcher;
  FLOAT m_fWatcherFrequency;
  FLOAT3D m_vDesiredPosition;
  FLOAT m_fMoveSpeed;
  FLOAT m_fRotateSpeed;
  FLOAT m_fMoveFrequency;
  BOOL m_bUseWatcher;
  BOOL m_bFlying;
  FLOAT m_fWaitTime;
  CTFileName m_fnMdl;
  CTFileName m_fnTex;
  ANIMATION m_iAnim;
  CTFileName m_fnAtt1Mdl;
  CTFileName m_fnAtt1Tex;
  INDEX m_iAtt1Position;
  ANIMATION m_iAtt1Anim;
  CTFileName m_fnAtt2Mdl;
  CTFileName m_fnAtt2Tex;
  INDEX m_iAtt2Position;
  ANIMATION m_iAtt2Anim;
  CTFileName m_fnAtt3Mdl;
  CTFileName m_fnAtt3Tex;
  INDEX m_iAtt3Position;
  ANIMATION m_iAtt3Anim;
   
#line 56 "E:/USA/EntitiesMP/EnvironmentBase.es"
BOOL MovesByTargetedRoute(CTString & strTargetProperty)const;
   
#line 61 "E:/USA/EntitiesMP/EnvironmentBase.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 66 "E:/USA/EntitiesMP/EnvironmentBase.es"
const CTString & GetDescription(void)const;
   
#line 74 "E:/USA/EntitiesMP/EnvironmentBase.es"
CAnimData * GetAnimData(SLONG slPropertyOffset);
   
#line 104 "E:/USA/EntitiesMP/EnvironmentBase.es"
BOOL NextMarker(void);
   
#line 132 "E:/USA/EntitiesMP/EnvironmentBase.es"
void CalcRotation(ANGLE aWantedHeadingRelative,ANGLE3D & aRotation);
   
#line 151 "E:/USA/EntitiesMP/EnvironmentBase.es"
void StopMoving(void);
   
#line 157 "E:/USA/EntitiesMP/EnvironmentBase.es"
void MoveToPosition(void);
   
#line 181 "E:/USA/EntitiesMP/EnvironmentBase.es"
void CalcDestination();
   
#line 190 "E:/USA/EntitiesMP/EnvironmentBase.es"
void MarkerParameters();
   
#line 207 "E:/USA/EntitiesMP/EnvironmentBase.es"
void Initialize(void);
   
#line 259 "E:/USA/EntitiesMP/EnvironmentBase.es"
void InitializeWatcher(FLOAT fWaitTime);
   
#line 280 "E:/USA/EntitiesMP/EnvironmentBase.es"
void PlayDefaultAnim(void);
   
#line 285 "E:/USA/EntitiesMP/EnvironmentBase.es"
void PlayMarkerAnimation(void);
   
#line 292 "E:/USA/EntitiesMP/EnvironmentBase.es"
void ChangeDefaultAnim(void);
   
#line 299 "E:/USA/EntitiesMP/EnvironmentBase.es"
void WaitOnMarker(void);
#define  STATE_CEnvironmentBase_MoveToMarker 0x03840000
  BOOL 
#line 318 "E:/USA/EntitiesMP/EnvironmentBase.es"
MoveToMarker(const CEntityEvent &__eeInput);
  BOOL H0x03840001_MoveToMarker_01(const CEntityEvent &__eeInput);
  BOOL H0x03840002_MoveToMarker_02(const CEntityEvent &__eeInput);
  BOOL H0x03840003_MoveToMarker_03(const CEntityEvent &__eeInput);
  BOOL H0x03840004_MoveToMarker_04(const CEntityEvent &__eeInput);
  BOOL H0x03840005_MoveToMarker_05(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_Activate 0x03840006
  BOOL 
#line 345 "E:/USA/EntitiesMP/EnvironmentBase.es"
Activate(const CEntityEvent &__eeInput);
  BOOL H0x03840007_Activate_01(const CEntityEvent &__eeInput);
  BOOL H0x03840008_Activate_02(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_Stop 0x03840009
  BOOL 
#line 353 "E:/USA/EntitiesMP/EnvironmentBase.es"
Stop(const CEntityEvent &__eeInput);
  BOOL H0x0384000a_Stop_01(const CEntityEvent &__eeInput);
  BOOL H0x0384000b_Stop_02(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_DoAction 0x0384000c
  BOOL 
#line 362 "E:/USA/EntitiesMP/EnvironmentBase.es"
DoAction(const CEntityEvent &__eeInput);
  BOOL H0x0384000d_DoAction_01(const CEntityEvent &__eeInput);
  BOOL H0x0384000e_DoAction_02(const CEntityEvent &__eeInput);
  BOOL H0x0384000f_DoAction_03(const CEntityEvent &__eeInput);
  BOOL H0x03840010_DoAction_04(const CEntityEvent &__eeInput);
  BOOL H0x03840011_DoAction_05(const CEntityEvent &__eeInput);
  BOOL H0x03840012_DoAction_06(const CEntityEvent &__eeInput);
  BOOL H0x03840013_DoAction_07(const CEntityEvent &__eeInput);
  BOOL H0x03840014_DoAction_08(const CEntityEvent &__eeInput);
  BOOL H0x03840015_DoAction_09(const CEntityEvent &__eeInput);
  BOOL H0x03840016_DoAction_10(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_MainLoop 0x03840017
  BOOL 
#line 388 "E:/USA/EntitiesMP/EnvironmentBase.es"
MainLoop(const CEntityEvent &__eeInput);
  BOOL H0x03840018_MainLoop_01(const CEntityEvent &__eeInput);
  BOOL H0x03840019_MainLoop_02(const CEntityEvent &__eeInput);
#define  STATE_CEnvironmentBase_Main 1
  BOOL 
#line 400 "E:/USA/EntitiesMP/EnvironmentBase.es"
Main(const CEntityEvent &__eeInput);
  BOOL H0x0384001a_Main_01(const CEntityEvent &__eeInput);
  BOOL H0x0384001b_Main_02(const CEntityEvent &__eeInput);
};
#endif // _EntitiesMP_EnvironmentBase_INCLUDED