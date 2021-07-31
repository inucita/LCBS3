/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"

#include "StdH.h"

#include <Damager.h>
#include <Damager_tables.h>
void CDamager::SetDefaultProperties(void) {
	m_strName = "Damager";
	m_strDescription = "";
	m_dmtType = DMT_ABYSS ;
	m_fAmmount = 1000.0f;
	m_penToDamage = NULL;
	m_bDamageFromTriggerer = FALSE ;
	m_penLastDamaged = NULL;
	m_tmLastDamage = 0.0f;
	CRationalEntity::SetDefaultProperties();
}

#line 26 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
const CTString & CDamager::GetDescription(void)const {
#line 27 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
return m_strDescription ;
#line 28 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}
BOOL CDamager::
#line 31 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDamager_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDamager::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 33 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
InitAsEditorModel  ();
#line 34 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 35 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
SetModel  (MODEL_TELEPORT );
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
SetModelMainTexture  (TEXTURE_TELEPORT );
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
((CTString &) m_strDescription ) . PrintF  ("%s:%g" , 
#line 42 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
DamageType_enum  . NameForValue  (INDEX (m_dmtType )) , m_fAmmount );
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
Jump(STATE_CURRENT,0x00e50002, FALSE, EInternal());return TRUE;}BOOL CDamager::H0x00e50002_Main_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e50002
if(!(TRUE )){ Jump(STATE_CURRENT,0x00e50003, FALSE, EInternal());return TRUE;}
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00e50000, FALSE, EBegin());return TRUE;}BOOL CDamager::H0x00e50000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e50000
switch(__eeInput.ee_slEvent){case(EVENTCODE_ETrigger):{const ETrigger&eTrigger= (ETrigger&)__eeInput;

#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
CEntity  * penInflictor  = this ;
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
if(m_bDamageFromTriggerer ){
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
penInflictor  = eTrigger  . penCaused ;
#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
CEntity  * penVictim  = NULL ;
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
if(m_penToDamage  != NULL ){
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
penVictim  = m_penToDamage ;
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}else if(eTrigger  . penCaused  != NULL ){
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
penVictim  = eTrigger  . penCaused ;
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
if(penVictim  != NULL ){
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
if(! (penVictim  == m_penLastDamaged  && _pTimer  -> CurrentTick  () < m_tmLastDamage  + 0.1f))
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
{
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
InflictDirectDamage  (penVictim  , penInflictor  , m_dmtType  , m_fAmmount  , 
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
penVictim  -> GetPlacement  () . pl_PositionVector  , FLOAT3D (0 , 1 , 0));
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
m_penLastDamaged  = penVictim ;
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
m_tmLastDamage  = _pTimer  -> CurrentTick  ();
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
UnsetTimer();Jump(STATE_CURRENT,0x00e50001, FALSE, EInternal());return TRUE;
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}ASSERT(FALSE);break;default:{
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
return TRUE;
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}ASSERT(FALSE);break;
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}return TRUE;}BOOL CDamager::H0x00e50001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e50001
;Jump(STATE_CURRENT,0x00e50002, FALSE, EInternal());return TRUE;
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Damager.es"
}BOOL CDamager::H0x00e50003_Main_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e50003
 ASSERT(FALSE); return TRUE;};