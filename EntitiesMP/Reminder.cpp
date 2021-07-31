/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"

#include "StdH.h"

#include <Reminder.h>
#include <Reminder_tables.h>
CEntityEvent *EReminderInit::MakeCopy(void) { 	CEntityEvent *peeCopy = new EReminderInit(*this); 	return peeCopy;}
SLONG EReminderInit::GetSizeOf(void) { 	return sizeof(*this);}
EReminderInit::EReminderInit() : CEntityEvent(EVENTCODE_EReminderInit) {
	ClearToDefault(eidOwner);
	ClearToDefault(fWaitTime);
	ClearToDefault(iValue);
}
BOOL EReminderInit::CheckIDs(void) {	return TRUE && eidOwner.IsValid(); }

void CReminder::SetDefaultProperties(void) {
	m_penOwner = NULL;
	m_fWaitTime = 0.0f;
	m_iValue = 0;
	m_iOwner = -1;
	CRationalEntity::SetDefaultProperties();
}

#line 26 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
void CReminder::InitializeFromNet() {
#line 27 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
EReminderInit  eRInit ;
#line 28 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
eRInit  . eidOwner  = m_iOwner ;
#line 29 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
eRInit  . fWaitTime  = m_fWaitTime ;
#line 30 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
eRInit  . iValue  = m_iValue ;
#line 31 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
Initialize  (eRInit );
#line 32 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
}
BOOL CReminder::
#line 35 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CReminder_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EReminderInit, "CReminder::Main expects 'EReminderInit' as input!");	const EReminderInit &eri = (const EReminderInit &)__eeInput;
#line 37 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
ASSERT  (((CEntity  *) eri  . eidOwner ) != NULL );
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
if(((CEntity  *) eri  . eidOwner ) == NULL ){
#line 40 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
Destroy  (FALSE );
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
Return(STATE_CURRENT,EVoid());
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
return TRUE;
#line 42 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
}
#line 43 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
m_penOwner  = (CEntity  *) eri  . eidOwner ;
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
m_fWaitTime  = eri  . fWaitTime ;
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
m_iValue  = eri  . iValue ;
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
m_iOwner  = ((CEntity  *) eri  . eidOwner ) -> en_ulID ;
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
InitAsVoid  ();
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
SetFlagOn  (ENF_CLIENTHANDLING );
#line 55 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
if(!(m_fWaitTime  > 0.0f)){ Jump(STATE_CURRENT,0x02bf0003, FALSE, EInternal());return TRUE;}
#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
SetTimerAfter(m_fWaitTime );
Jump(STATE_CURRENT, 0x02bf0001, FALSE, EBegin());return TRUE;}BOOL CReminder::H0x02bf0001_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bf0001
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x02bf0002, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CReminder::H0x02bf0002_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bf0002
;
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
EReminder  er ;
#line 58 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
er  . iValue  = m_iValue ;
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
if(m_penOwner  != NULL ){
#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
m_penOwner  -> SendEvent  (er );
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
}Jump(STATE_CURRENT,0x02bf0003, FALSE, EInternal());return TRUE;}BOOL CReminder::H0x02bf0003_Main_03(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x02bf0003

#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
Destroy  (FALSE );
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
Return(STATE_CURRENT,EVoid());
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Reminder.es"
return TRUE; ASSERT(FALSE); return TRUE;};