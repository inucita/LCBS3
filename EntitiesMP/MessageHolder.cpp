/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"

#include "StdH.h"
#include <Engine/Interface/UISignBoard.h>
extern INDEX ent_bReportBrokenChains;

#include <MessageHolder.h>
#include <MessageHolder_tables.h>
void CMessageHolder::SetDefaultProperties(void) {
	m_strName = "MessageHolder";
	m_iStringIndex = -1;
	m_strRealString = "";
	CRationalEntity::SetDefaultProperties();
}
BOOL CMessageHolder::
#line 46 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CMessageHolder_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CMessageHolder::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 48 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
InitAsEditorModel  ();
#line 49 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 53 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
SetModel  (MODEL_MARKER );
#line 54 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
SetTimerAfter(0.01f);
Jump(STATE_CURRENT, 0x00e20000, FALSE, EBegin());return TRUE;}BOOL CMessageHolder::H0x00e20000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e20000
switch(__eeInput.ee_slEvent) {case EVENTCODE_EBegin: return TRUE;case EVENTCODE_ETimer: Jump(STATE_CURRENT,0x00e20001, FALSE, EInternal()); return TRUE;default: return FALSE; }}BOOL CMessageHolder::H0x00e20001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e20001
;
#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
Jump(STATE_CURRENT, STATE_CMessageHolder_MainMessageType, TRUE, EVoid());return TRUE;
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
Return(STATE_CURRENT,EVoid());
#line 66 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
return TRUE; ASSERT(FALSE); return TRUE;};BOOL CMessageHolder::
#line 113 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
MainMessageType(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CMessageHolder_MainMessageType
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CMessageHolder::MainMessageType expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
Jump(STATE_CURRENT,0x00e20005, FALSE, EInternal());return TRUE;}BOOL CMessageHolder::H0x00e20005_MainMessageType_03(const CEntityEvent &__eeInput)
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
{
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e20005
if(!(TRUE )){ Jump(STATE_CURRENT,0x00e20006, FALSE, EInternal());return TRUE;}
#line 117 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x00e20003, FALSE, EBegin());return TRUE;}BOOL CMessageHolder::H0x00e20003_MainMessageType_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e20003
switch(__eeInput.ee_slEvent)
#line 118 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
{case(EVENTCODE_EBegin):
#line 120 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
{const EBegin&e= (EBegin&)__eeInput;

#line 121 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
return TRUE;
#line 122 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETimer):
#line 124 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
{const ETimer&e= (ETimer&)__eeInput;

#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
UnsetTimer();Jump(STATE_CURRENT,0x00e20004, FALSE, EInternal());return TRUE;
#line 126 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETrigger):
#line 128 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
{const ETrigger&e= (ETrigger&)__eeInput;

#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
ShowSignBoard  (m_iStringIndex );
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
OutputDebugString  (m_strName  . str_String );
#line 133 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
OutputDebugString  ("<- Use Touch Field\n");
#line 134 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
return TRUE;
#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
}return TRUE;}BOOL CMessageHolder::H0x00e20004_MainMessageType_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e20004
Jump(STATE_CURRENT,0x00e20005, FALSE, EInternal());return TRUE;
#line 137 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/MessageHolder.es"
}BOOL CMessageHolder::H0x00e20006_MainMessageType_04(const CEntityEvent &__eeInput) {
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x00e20006
 ASSERT(FALSE); return TRUE;};