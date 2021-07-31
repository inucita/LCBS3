/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"

#include "StdH.h"

#include <ParticleCloudsHolder.h>
#include <ParticleCloudsHolder_tables.h>
void CParticleCloudsHolder::SetDefaultProperties(void) {
	m_ctTilesPerX = 1;
	m_ctTilesPerY = 1;
	m_strName = "Clouds holder";
	m_strDescription = "";
	m_moCloudsParticleTextureHolder.SetData(NULL);
m_moCloudsParticleTextureHolder.mo_toTexture.SetData(NULL);
	m_fnCloudsParticleTexture = CTString("");
	m_penParticleCloudsMarker = NULL;
	m_fnStreaksParticleTexture = CTString("");
	m_moStreaksParticleTextureHolder.SetData(NULL);
m_moStreaksParticleTextureHolder.mo_toTexture.SetData(NULL);
	m_sbtStreaksBelndType = SBT_ADD ;
	m_bBackground = TRUE ;
	CMovableModelEntity::SetDefaultProperties();
}

#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
void CParticleCloudsHolder::RenderParticles(void) 
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
{
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
CTextureData  * ptd  = (CTextureData  *) m_moCloudsParticleTextureHolder  . mo_toTexture  . GetData  ();
#line 47 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
if(ptd  != NULL )
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
{
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
BOOL CParticleCloudsHolder::
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CParticleCloudsHolder_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CParticleCloudsHolder::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
InitAsEditorModel  ();
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetModel  (MODEL_MARKER );
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
if(m_bBackground ){
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetFlags  (GetFlags  () | ENF_BACKGROUND );
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}else {
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetFlags  (GetFlags  () & ~ ENF_BACKGROUND );
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
if(m_fnCloudsParticleTexture  != CTString (""))
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
{
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
try 
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
{
#line 75 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
m_moCloudsParticleTextureHolder  . mo_toTexture  . SetData_t  (m_fnCloudsParticleTexture );
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}catch  (char  * strError ){
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
WarningMessage  (strError );
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
if(m_fnStreaksParticleTexture  != CTString (""))
#line 81 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
{
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
try 
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
{
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
m_moStreaksParticleTextureHolder  . mo_toTexture  . SetData_t  (m_fnStreaksParticleTexture );
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}catch  (char  * strError ){
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
WarningMessage  (strError );
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
}
#line 90 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
SetTimerAfter(0.1f);
Jump(STATE_CURRENT, 0x00f30000, FALSE, EBegin());return TRUE;}BOOL CParticleCloudsHolder::H0x00f30000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00f30000
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00f30001, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CParticleCloudsHolder::H0x00f30001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00f30001
;
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
Return(STATE_CURRENT,EVoid());
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/ParticleCloudsHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};