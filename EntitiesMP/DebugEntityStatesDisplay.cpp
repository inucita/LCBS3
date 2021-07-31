/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"

#include "StdH.h"
#include "EntitiesMP/Player.h"
void HUD_SetEntityForStackDisplay(CRationalEntity *pren);
//void HUD_SetSkaDebug(BOOL bSkaDebug);

#include <DebugEntityStatesDisplay.h>
#include <DebugEntityStatesDisplay_tables.h>
void CEntityStateDisplay::SetDefaultProperties(void) {
	m_strName = "EntityStateDisplay";
	m_penManualTarget = NULL;
	m_eTargetType = DTT_MANUAL ;
	m_bSkaStateDebug = FALSE ;
	CRationalEntity::SetDefaultProperties();
}
 CEntityStateDisplay:: ~ CEntityStateDisplay() 
#line 34 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
{
#line 35 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
HUD_SetEntityForStackDisplay  (NULL );
#line 36 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}

#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
CPlayer * CEntityStateDisplay::GetPlayer() 
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
{
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
for(INDEX iPlayer  = 0;iPlayer  < GetMaxPlayers  ();iPlayer  ++){
#line 42 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
CEntity  * pen  = GetPlayerEntity  (iPlayer );
#line 43 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
if(pen  != NULL ){
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
if(_pNetwork  -> IsPlayerLocal  (pen )){
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
return (CPlayer  *) pen ;
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 47 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
return NULL ;
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}

#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
void CEntityStateDisplay::ObtainTarget() 
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
{
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
CEntity  * penTarget  = NULL ;
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
switch(m_eTargetType ){
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_MANUAL : 
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = m_penManualTarget ;
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_PLAYER : 
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = (CEntity  *) & * GetPlayer  ();
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_PLAYERWEAPONS : 
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
if(GetPlayer  () == NULL ){return ;}
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = (CEntity  *) & * GetPlayer  () -> GetPlayerWeapons  ();
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
case DTT_PLAYERANIMATOR : 
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
if(GetPlayer  () == NULL ){return ;}
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
penTarget  = (CEntity  *) & * GetPlayer  () -> GetPlayerAnimator  ();
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
break ;
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
if(penTarget  != NULL ){
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
HUD_SetEntityForStackDisplay  ((CRationalEntity  *) penTarget );
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}else {
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
HUD_SetEntityForStackDisplay  (NULL );
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
BOOL CEntityStateDisplay::
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CEntityStateDisplay_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CEntityStateDisplay::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
InitAsEditorModel  ();
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
SetModel  (MODEL_MARKER );
#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 92 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
GetModelObject  () -> StretchModel  (FLOAT3D (0.4f , 0.4f , 0.4f));
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
if(m_eTargetType  != DTT_MANUAL ){
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
m_penManualTarget  = NULL ;
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}
#line 99 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
SetTimerAfter(0.5f);
Jump(STATE_CURRENT, 0x00e90000, FALSE, EBegin());return TRUE;}BOOL CEntityStateDisplay::H0x00e90000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90000
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00e90001, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CEntityStateDisplay::H0x00e90001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90001
;
#line 103 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
Jump(STATE_CURRENT,0x00e90004, FALSE, EInternal());return TRUE;}BOOL CEntityStateDisplay::H0x00e90004_Main_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90004
if(!(TRUE )){ Jump(STATE_CURRENT,0x00e90005, FALSE, EInternal());return TRUE;}
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
SetTimerAfter(0.2f);
Jump(STATE_CURRENT, 0x00e90002, FALSE, EBegin());return TRUE;}BOOL CEntityStateDisplay::H0x00e90002_Main_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90002
switch(__eeInput.ee_slEvent)
#line 105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
{case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 107 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
ObtainTarget  ();
#line 112 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
return TRUE;
#line 113 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;

#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
UnsetTimer();Jump(STATE_CURRENT,0x00e90003, FALSE, EInternal());return TRUE;
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}return TRUE;}BOOL CEntityStateDisplay::H0x00e90003_Main_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90003
Jump(STATE_CURRENT,0x00e90004, FALSE, EInternal());return TRUE;
#line 118 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
}BOOL CEntityStateDisplay::H0x00e90005_Main_06(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e90005

#line 119 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
Return(STATE_CURRENT,EVoid());
#line 119 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/DebugEntityStatesDisplay.es"
return TRUE; ASSERT(FALSE); return TRUE;};