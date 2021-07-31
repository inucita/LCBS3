/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"

#include "StdH.h"

#include <MusicChanger.h>
#include <MusicChanger_tables.h>
CEntityEvent *EMusicChangerTrigger::MakeCopy(void) { 	CEntityEvent *peeCopy = new EMusicChangerTrigger(*this); 	return peeCopy;}
SLONG EMusicChangerTrigger::GetSizeOf(void) { 	return sizeof(*this);}
EMusicChangerTrigger::EMusicChangerTrigger() : CEntityEvent(EVENTCODE_EMusicChangerTrigger) {
}
BOOL EMusicChangerTrigger::CheckIDs(void) {	return TRUE ; }

#line 11 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"


void CMusicChanger::SetDefaultProperties(void) {
	m_strName = "";
	m_strDescription = "";
	m_fnMusic = CTFILENAME("");
	m_fVolume = 1.0f;
	m_mtType = MT_EVENT ;
	m_bForceStart = TRUE ;
	CRationalEntity::SetDefaultProperties();
}

#line 36 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
void CMusicChanger::Precache(void) 
#line 37 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
{
#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
if(m_fnMusic  != ""){
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
m_aps  . Precache  (m_fnMusic );
#line 40 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}
BOOL CMusicChanger::
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CMusicChanger_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CMusicChanger::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
InitAsEditorModel  ();
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SetModel  (MODEL_MARKER );
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
m_strDescription  . PrintF  ("%s: %s (%g)" , 
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
MusicType_enum  . NameForValue  ((INDEX) m_mtType ) , 
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
(CTString &) m_fnMusic  . FileName  () , 
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
m_fVolume );
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00f60001, FALSE, EBegin());return TRUE;}BOOL CMusicChanger::H0x00f60001_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00f60001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00f60002, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CMusicChanger::H0x00f60002_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00f60002
;
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00f60003, FALSE, EBegin());return TRUE;}BOOL CMusicChanger::H0x00f60003_Main_03(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00f60003
switch(__eeInput.ee_slEvent){case(EVENTCODE_ETrigger):{const ETrigger&e= (ETrigger&)__eeInput;

#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
if(_pNetwork  -> IsServer  ()){
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
SendEvent  (EMusicChangerTrigger  () , TRUE );
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
return TRUE;
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}ASSERT(FALSE);break;case(EVENTCODE_EMusicChangerTrigger):{const EMusicChangerTrigger&e= (EMusicChangerTrigger&)__eeInput;

#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
CEntity  * penMusicHolder  = _pNetwork  -> GetEntityWithName  ("MusicHolder" , 0);
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
if(penMusicHolder  == NULL ){
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
CPrintF  ("No MusicHolder on this level, cannot change music!\n");
#line 81 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}else {
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
EChangeMusic  ecm ;
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
ecm  . fnMusic  = m_fnMusic ;
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
ecm  . fVolume  = m_fVolume ;
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
ecm  . mtType  = m_mtType ;
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
ecm  . bForceStart  = m_bForceStart ;
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
penMusicHolder  -> SendEvent  (ecm );
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}
#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
return TRUE;
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 92 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
}return TRUE;}BOOL CMusicChanger::H0x00f60004_Main_04(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00f60004

#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
Return(STATE_CURRENT,EVoid());
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MusicChanger.es"
return TRUE; ASSERT(FALSE); return TRUE;};