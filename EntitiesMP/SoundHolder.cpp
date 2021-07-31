/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 5 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"

#include "StdH.h"
#include <Engine/Sound/SoundData.h>

#include <EntitiesMP/SoundHolder.h>
#include <EntitiesMP/SoundHolder_tables.h>
CEntityEvent *EPlaySoundOnce::MakeCopy(void) { CEntityEvent *peeCopy = new EPlaySoundOnce(*this); return peeCopy;}
SLONG EPlaySoundOnce::GetSizeOf(void) { return sizeof(*this);}
EPlaySoundOnce::EPlaySoundOnce() : CEntityEvent(EVENTCODE_EPlaySoundOnce) {;
};
BOOL EPlaySoundOnce::CheckIDs(void) { return TRUE ; }
void CSoundHolder::SetDefaultProperties(void) {
  m_fnSound = CTFILENAME("Sounds\\Default.wav");
  m_rFallOffRange = 100.0f;
  m_rHotSpotRange = 50.0f;
  m_fVolume = 1.0f;
  m_fPitch = 1.0f;
  m_bLoop = TRUE ;
  m_bVolumetric = TRUE ;
  m_bMusic = FALSE ;
  m_strName = "";
  m_strDescription = "";
  m_bAutoStart = FALSE ;
  m_iPlayType = 0;
  m_soSound.SetOwner(this);
m_soSound.Stop(FALSE);
  m_bDestroyable = FALSE ;
  CRationalEntity::SetDefaultProperties();
}
  
#line 51 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
void CSoundHolder::Precache(void) 
#line 52 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
{
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_aps  . Precache  (m_fnSound );
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}
  
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
void CSoundHolder::MirrorAndStretch(FLOAT fStretch,BOOL bMirrorX) 
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
{
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_rFallOffRange  *= fStretch ;
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_rHotSpotRange  *= fStretch ;
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}
  
#line 67 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SLONG CSoundHolder::GetUsedMemory(void) 
#line 68 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
{
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SLONG  slUsedMemory  = sizeof  (CSoundHolder ) - sizeof  (CRationalEntity ) + CRationalEntity  :: GetUsedMemory  ();
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
slUsedMemory  += m_strName  . Length  ();
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
slUsedMemory  += m_strDescription  . Length  ();
#line 74 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
slUsedMemory  += m_fnSound  . Length  ();
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
slUsedMemory  += 1 * sizeof  (CSoundObject);
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return slUsedMemory ;
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}
BOOL CSoundHolder::
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
PlaySoundOnce(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSoundHolder_PlaySoundOnce
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSoundHolder::PlaySoundOnce expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 83 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_soSound  . Set3DParameters  (FLOAT (m_rFallOffRange ) , FLOAT (m_rHotSpotRange ) , m_fVolume  , m_fPitch  , FALSE );
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
PlaySound  (m_soSound  , m_fnSound  , SOF_3D  | SOF_VOLUMETRIC );
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
FLOAT fSoundDuration  = m_soSound  . so_psdLink  -> GetSecondsLength  ();
#line 86 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetTimerAfter(fSoundDuration  + 0.05f);
Jump(STATE_CURRENT, 0x00cc0002, FALSE, EBegin());return TRUE;}BOOL CSoundHolder::H0x00cc0002_PlaySoundOnce_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0002
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00cc0003, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CSoundHolder::H0x00cc0003_PlaySoundOnce_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0003
;
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Destroy  (FALSE );
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Return(STATE_CURRENT,EVoid());
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CSoundHolder::
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CSoundHolder_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CSoundHolder::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetFlagOn  (ENF_MARKDESTROY );
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetFlagOn  (ENF_NONETCONNECT );
#line 96 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetFlagOff  (ENF_PROPSCHANGED );
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_fVolume  = Clamp  (m_fVolume  , 0.0f , 1.0f);
#line 100 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_fPitch  = Clamp  (m_fPitch  , 0.01f , 100.0f);
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_rHotSpotRange  = ClampDn  (m_rHotSpotRange  , 0.0f);
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_rFallOffRange  = ClampDn  (m_rFallOffRange  , m_rHotSpotRange  + 0.5f);
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_iPlayType  = SOF_3D ;
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
if(m_bLoop ){m_iPlayType  |= SOF_LOOP ;}
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
if(m_bMusic ){m_iPlayType  |= SOF_MUSIC ;}
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
if(m_bVolumetric ){m_iPlayType  |= SOF_VOLUMETRIC ;}
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
InitAsEditorModel  ();
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 116 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
const FLOAT fFactor  = ClampDn  (Log2  (m_rFallOffRange ) , 1.0f);
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
GetModelObject  () -> mo_Stretch  = FLOAT3D (fFactor  , fFactor  , fFactor );
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetModel  (MODEL_MARKER );
#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_strDescription  . PrintF  ("%s" , (CTString &) m_fnSound  . FileName  ());
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
if(!(_pTimer  -> CurrentTick  () <= 0.1f)){ Jump(STATE_CURRENT,0x00cc0006, FALSE, EInternal());return TRUE;}SetTimerAfter(0.5f);
Jump(STATE_CURRENT, 0x00cc0004, FALSE, EBegin());return TRUE;}BOOL CSoundHolder::H0x00cc0004_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0004
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00cc0005, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CSoundHolder::H0x00cc0005_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0005
;Jump(STATE_CURRENT,0x00cc0006, FALSE, EInternal());return TRUE;}BOOL CSoundHolder::H0x00cc0006_Main_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0006

#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00cc0007, FALSE, EBegin());return TRUE;}BOOL CSoundHolder::H0x00cc0007_Main_04(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0007
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
if(m_bAutoStart ){
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_soSound  . Set3DParameters  (FLOAT (m_rFallOffRange ) , FLOAT (m_rHotSpotRange ) , m_fVolume  , m_fPitch );
#line 134 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
PlaySound  (m_soSound  , m_fnSound  , m_iPlayType );
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return TRUE;
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStart):{const EStart&e= (EStart&)__eeInput;

#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_soSound  . Set3DParameters  (FLOAT (m_rFallOffRange ) , FLOAT (m_rHotSpotRange ) , m_fVolume  , m_fPitch );
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
PlaySound  (m_soSound  , m_fnSound  , m_iPlayType );
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return TRUE;
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EPlaySoundOnce):{const EPlaySoundOnce&e= (EPlaySoundOnce&)__eeInput;

#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Jump(STATE_CURRENT, STATE_CSoundHolder_PlaySoundOnce, TRUE, EVoid());return TRUE;
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EStop):{const EStop&e= (EStop&)__eeInput;

#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_soSound  . Stop  ();
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return TRUE;
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_ERangeModelDestruction):{const ERangeModelDestruction&e= (ERangeModelDestruction&)__eeInput;

#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
if(m_bDestroyable ){
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
m_soSound  . Stop  ();
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Return(STATE_CURRENT,TRUE );
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return TRUE;
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_EEnd):{const EEnd&e= (EEnd&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x00cc0008, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 163 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
}return TRUE;}BOOL CSoundHolder::H0x00cc0008_Main_05(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00cc0008

#line 166 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Destroy  ();
#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
Return(STATE_CURRENT,EVoid());
#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/SoundHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};