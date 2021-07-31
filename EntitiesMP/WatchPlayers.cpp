/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"

#include "StdH.h"

#include <WatchPlayers.h>
#include <WatchPlayers_tables.h>
void CWatchPlayers::SetDefaultProperties(void) {
	m_penOwner = NULL;
	m_penFar = NULL;
	m_fWaitTime = 0.1f;
	m_fDistance = 100.0f;
	m_bRangeWatcher = TRUE ;
	m_eetEventClose = EET_TRIGGER ;
	m_eetEventFar = EET_ENVIRONMENTSTOP ;
	m_penCurrentWatch = NULL;
	m_bActive = TRUE ;
	m_strName = "";
	CRationalEntity::SetDefaultProperties();
}

#line 32 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
BOOL CWatchPlayers::IsAnyPlayerClose(void) {
#line 34 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
FLOAT fClosest  = 100000.0f;
#line 35 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
FLOAT fDistance ;
#line 37 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
m_penCurrentWatch  = NULL ;
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
for(INDEX iPlayer  = 0;iPlayer  < GetMaxPlayers  ();iPlayer  ++){
#line 40 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
CEntity  * penPlayer  = GetPlayerEntity  (iPlayer );
#line 42 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(penPlayer  != NULL  && penPlayer  -> GetFlags  () & ENF_ALIVE  && ! (penPlayer  -> GetFlags  () & ENF_INVISIBLE )){
#line 43 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
fDistance  = 100000.0f;
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_bRangeWatcher ){
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
fDistance  = (penPlayer  -> GetPlacement  () . pl_PositionVector  - 
#line 47 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
GetPlacement  () . pl_PositionVector ) . Length  ();
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}else {
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_penOwner  != NULL ){
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
fDistance  = (penPlayer  -> GetPlacement  () . pl_PositionVector  - 
#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
m_penOwner  -> GetPlacement  () . pl_PositionVector ) . Length  ();
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(fDistance  < fClosest ){
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
fClosest  = fDistance ;
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
m_penCurrentWatch  = penPlayer ;
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
return (fClosest  < m_fDistance );
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}

#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
void CWatchPlayers::SendCloseEvent(void) {
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_bRangeWatcher  && m_penOwner  == NULL ){
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}else {
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SendToTarget  (m_penOwner  , m_eetEventClose  , m_penCurrentWatch );
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}

#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
void CWatchPlayers::SendFarEvent(void) {
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_bRangeWatcher  && m_penOwner  == NULL ){
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}else {
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_penFar  != NULL ){
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SendToTarget  (m_penFar  , m_eetEventFar );
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}else {
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SendToTarget  (m_penOwner  , m_eetEventFar );
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 89 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
BOOL CWatchPlayers::
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatchPlayers_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatchPlayers::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
InitAsEditorModel  ();
#line 96 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 97 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 100 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetModel  (MODEL_WATCHPLAYERS );
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetModelMainTexture  (TEXTURE_WATCHPLAYERS );
#line 103 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_fWaitTime  < 0.1f){
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
m_fWaitTime  = 0.1f;
#line 105 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 107 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(m_bActive ){
#line 108 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT, STATE_CWatchPlayers_Active, TRUE, EVoid());return TRUE;
#line 109 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}else {
#line 110 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT, STATE_CWatchPlayers_Inactive, TRUE, EVoid());return TRUE;
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
} ASSERT(FALSE); return TRUE;};BOOL CWatchPlayers::
#line 114 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Active(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatchPlayers_Active
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatchPlayers::Active expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
STATE_CWatchPlayers_FarWatch, TRUE;
Jump(STATE_CURRENT, 0x02be0001, FALSE, EBegin());return TRUE;}BOOL CWatchPlayers::H0x02be0001_Active_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: Call(STATE_CURRENT, STATE_CWatchPlayers_FarWatch, TRUE, EVoid());return TRUE;case EVENTCODE_EDeactivate: Jump(STATE_CURRENT,0x02be0002, FALSE, __eeInput); return TRUE;default: return FALSE; }}BOOL CWatchPlayers::H0x02be0002_Active_02(const CEntityEvent &__eeInput){
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0002
const EDeactivate&__e= (EDeactivate&)__eeInput;
;
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT, STATE_CWatchPlayers_Inactive, TRUE, EVoid());return TRUE; ASSERT(FALSE); return TRUE;};BOOL CWatchPlayers::
#line 120 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Inactive(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatchPlayers_Inactive
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatchPlayers::Inactive expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 122 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x02be0004, FALSE, EBegin());return TRUE;}BOOL CWatchPlayers::H0x02be0004_Inactive_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0004
switch(__eeInput.ee_slEvent){case(EVENTCODE_EActivate):{const EActivate&e= (EActivate&)__eeInput;

#line 124 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
UnsetTimer();Jump(STATE_CURRENT,0x02be0005, FALSE, EInternal());return TRUE;
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}ASSERT(FALSE);break;default:{
#line 127 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
return TRUE;
#line 128 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}ASSERT(FALSE);break;
#line 129 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}return TRUE;}BOOL CWatchPlayers::H0x02be0005_Inactive_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0005

#line 130 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT, STATE_CWatchPlayers_Active, TRUE, EVoid());return TRUE; ASSERT(FALSE); return TRUE;};BOOL CWatchPlayers::
#line 134 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
CloseWatch(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatchPlayers_CloseWatch
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatchPlayers::CloseWatch expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT,0x02be0009, FALSE, EInternal());return TRUE;}BOOL CWatchPlayers::H0x02be0009_CloseWatch_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0009
if(!(TRUE )){ Jump(STATE_CURRENT,0x02be000a, FALSE, EInternal());return TRUE;}
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetTimerAfter(m_fWaitTime );
Jump(STATE_CURRENT, 0x02be0007, FALSE, EBegin());return TRUE;}BOOL CWatchPlayers::H0x02be0007_CloseWatch_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0007
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 138 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(! IsAnyPlayerClose  ()){
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SendFarEvent  ();
#line 141 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT, STATE_CWatchPlayers_FarWatch, TRUE, EVoid());return TRUE;
#line 142 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 143 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
return TRUE;
#line 144 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x02be0008, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 146 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}return TRUE;}BOOL CWatchPlayers::H0x02be0008_CloseWatch_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be0008
Jump(STATE_CURRENT,0x02be0009, FALSE, EInternal());return TRUE;
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}BOOL CWatchPlayers::H0x02be000a_CloseWatch_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be000a
 ASSERT(FALSE); return TRUE;};BOOL CWatchPlayers::
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
FarWatch(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CWatchPlayers_FarWatch
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CWatchPlayers::FarWatch expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 152 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT,0x02be000e, FALSE, EInternal());return TRUE;}BOOL CWatchPlayers::H0x02be000e_FarWatch_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be000e
if(!(TRUE )){ Jump(STATE_CURRENT,0x02be000f, FALSE, EInternal());return TRUE;}
#line 153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SetTimerAfter(m_fWaitTime );
Jump(STATE_CURRENT, 0x02be000c, FALSE, EBegin());return TRUE;}BOOL CWatchPlayers::H0x02be000c_FarWatch_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be000c
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 155 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
if(IsAnyPlayerClose  ()){
#line 157 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
SendCloseEvent  ();
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
Jump(STATE_CURRENT, STATE_CWatchPlayers_CloseWatch, TRUE, EVoid());return TRUE;
#line 159 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
return TRUE;
#line 161 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):{const ETimer&e= (ETimer&)__eeInput;
UnsetTimer();Jump(STATE_CURRENT,0x02be000d, FALSE, EInternal());return TRUE;}ASSERT(FALSE);break;default: return FALSE; break;
#line 163 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}return TRUE;}BOOL CWatchPlayers::H0x02be000d_FarWatch_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be000d
Jump(STATE_CURRENT,0x02be000e, FALSE, EInternal());return TRUE;
#line 164 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WatchPlayers.es"
}BOOL CWatchPlayers::H0x02be000f_FarWatch_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02be000f
 ASSERT(FALSE); return TRUE;};