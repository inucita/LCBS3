/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Global.es"

#include "StdH.h"

#include <Global.h>
#include <Global_tables.h>
CEntityEvent *EStop::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStop(*this); 	return peeCopy;}
SLONG EStop::GetSizeOf(void) { 	return sizeof(*this);}
EStop::EStop() : CEntityEvent(EVENTCODE_EStop) {
}
BOOL EStop::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EStart::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStart(*this); 	return peeCopy;}
SLONG EStart::GetSizeOf(void) { 	return sizeof(*this);}
EStart::EStart() : CEntityEvent(EVENTCODE_EStart) {
	ClearToDefault(penCaused);
}
BOOL EStart::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EActivate::MakeCopy(void) { 	CEntityEvent *peeCopy = new EActivate(*this); 	return peeCopy;}
SLONG EActivate::GetSizeOf(void) { 	return sizeof(*this);}
EActivate::EActivate() : CEntityEvent(EVENTCODE_EActivate) {
}
BOOL EActivate::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EDeactivate::MakeCopy(void) { 	CEntityEvent *peeCopy = new EDeactivate(*this); 	return peeCopy;}
SLONG EDeactivate::GetSizeOf(void) { 	return sizeof(*this);}
EDeactivate::EDeactivate() : CEntityEvent(EVENTCODE_EDeactivate) {
}
BOOL EDeactivate::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EEnvironmentStart::MakeCopy(void) { 	CEntityEvent *peeCopy = new EEnvironmentStart(*this); 	return peeCopy;}
SLONG EEnvironmentStart::GetSizeOf(void) { 	return sizeof(*this);}
EEnvironmentStart::EEnvironmentStart() : CEntityEvent(EVENTCODE_EEnvironmentStart) {
}
BOOL EEnvironmentStart::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EEnvironmentStop::MakeCopy(void) { 	CEntityEvent *peeCopy = new EEnvironmentStop(*this); 	return peeCopy;}
SLONG EEnvironmentStop::GetSizeOf(void) { 	return sizeof(*this);}
EEnvironmentStop::EEnvironmentStop() : CEntityEvent(EVENTCODE_EEnvironmentStop) {
}
BOOL EEnvironmentStop::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EEnd::MakeCopy(void) { 	CEntityEvent *peeCopy = new EEnd(*this); 	return peeCopy;}
SLONG EEnd::GetSizeOf(void) { 	return sizeof(*this);}
EEnd::EEnd() : CEntityEvent(EVENTCODE_EEnd) {
}
BOOL EEnd::CheckIDs(void) {	return TRUE ; }

CEntityEvent *ETrigger::MakeCopy(void) { 	CEntityEvent *peeCopy = new ETrigger(*this); 	return peeCopy;}
SLONG ETrigger::GetSizeOf(void) { 	return sizeof(*this);}
ETrigger::ETrigger() : CEntityEvent(EVENTCODE_ETrigger) {
	ClearToDefault(penCaused);
}
BOOL ETrigger::CheckIDs(void) {	return TRUE ; }

CEntityEvent *ETeleportMovingBrush::MakeCopy(void) { 	CEntityEvent *peeCopy = new ETeleportMovingBrush(*this); 	return peeCopy;}
SLONG ETeleportMovingBrush::GetSizeOf(void) { 	return sizeof(*this);}
ETeleportMovingBrush::ETeleportMovingBrush() : CEntityEvent(EVENTCODE_ETeleportMovingBrush) {
}
BOOL ETeleportMovingBrush::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EReminder::MakeCopy(void) { 	CEntityEvent *peeCopy = new EReminder(*this); 	return peeCopy;}
SLONG EReminder::GetSizeOf(void) { 	return sizeof(*this);}
EReminder::EReminder() : CEntityEvent(EVENTCODE_EReminder) {
	ClearToDefault(iValue);
}
BOOL EReminder::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EStartAttack::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStartAttack(*this); 	return peeCopy;}
SLONG EStartAttack::GetSizeOf(void) { 	return sizeof(*this);}
EStartAttack::EStartAttack() : CEntityEvent(EVENTCODE_EStartAttack) {
}
BOOL EStartAttack::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EStopAttack::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStopAttack(*this); 	return peeCopy;}
SLONG EStopAttack::GetSizeOf(void) { 	return sizeof(*this);}
EStopAttack::EStopAttack() : CEntityEvent(EVENTCODE_EStopAttack) {
}
BOOL EStopAttack::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EStopBlindness::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStopBlindness(*this); 	return peeCopy;}
SLONG EStopBlindness::GetSizeOf(void) { 	return sizeof(*this);}
EStopBlindness::EStopBlindness() : CEntityEvent(EVENTCODE_EStopBlindness) {
}
BOOL EStopBlindness::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EStopDeafness::MakeCopy(void) { 	CEntityEvent *peeCopy = new EStopDeafness(*this); 	return peeCopy;}
SLONG EStopDeafness::GetSizeOf(void) { 	return sizeof(*this);}
EStopDeafness::EStopDeafness() : CEntityEvent(EVENTCODE_EStopDeafness) {
}
BOOL EStopDeafness::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EAddHostile::MakeCopy(void) { 	CEntityEvent *peeCopy = new EAddHostile(*this); 	return peeCopy;}
SLONG EAddHostile::GetSizeOf(void) { 	return sizeof(*this);}
EAddHostile::EAddHostile() : CEntityEvent(EVENTCODE_EAddHostile) {
}
BOOL EAddHostile::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EDoorControll::MakeCopy(void) { 	CEntityEvent *peeCopy = new EDoorControll(*this); 	return peeCopy;}
SLONG EDoorControll::GetSizeOf(void) { 	return sizeof(*this);}
EDoorControll::EDoorControll() : CEntityEvent(EVENTCODE_EDoorControll) {
	ClearToDefault(bOpen);
}
BOOL EDoorControll::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EKilledEnemy::MakeCopy(void) { 	CEntityEvent *peeCopy = new EKilledEnemy(*this); 	return peeCopy;}
SLONG EKilledEnemy::GetSizeOf(void) { 	return sizeof(*this);}
EKilledEnemy::EKilledEnemy() : CEntityEvent(EVENTCODE_EKilledEnemy) {
	ClearToDefault(penEnemy);
	ClearToDefault(iScore);
	ClearToDefault(eCauseOfDeath);
	ClearToDefault(eKillType);
}
BOOL EKilledEnemy::CheckIDs(void) {	return TRUE ; }

CEntityEvent *ESound::MakeCopy(void) { 	CEntityEvent *peeCopy = new ESound(*this); 	return peeCopy;}
SLONG ESound::GetSizeOf(void) { 	return sizeof(*this);}
ESound::ESound() : CEntityEvent(EVENTCODE_ESound) {
	ClearToDefault(EsndtSound);
	ClearToDefault(penTarget);
}
BOOL ESound::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EScroll::MakeCopy(void) { 	CEntityEvent *peeCopy = new EScroll(*this); 	return peeCopy;}
SLONG EScroll::GetSizeOf(void) { 	return sizeof(*this);}
EScroll::EScroll() : CEntityEvent(EVENTCODE_EScroll) {
	ClearToDefault(bStart);
	ClearToDefault(penSender);
}
BOOL EScroll::CheckIDs(void) {	return TRUE ; }

CEntityEvent *ETextFX::MakeCopy(void) { 	CEntityEvent *peeCopy = new ETextFX(*this); 	return peeCopy;}
SLONG ETextFX::GetSizeOf(void) { 	return sizeof(*this);}
ETextFX::ETextFX() : CEntityEvent(EVENTCODE_ETextFX) {
	ClearToDefault(bStart);
	ClearToDefault(penSender);
}
BOOL ETextFX::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EHudPicFX::MakeCopy(void) { 	CEntityEvent *peeCopy = new EHudPicFX(*this); 	return peeCopy;}
SLONG EHudPicFX::GetSizeOf(void) { 	return sizeof(*this);}
EHudPicFX::EHudPicFX() : CEntityEvent(EVENTCODE_EHudPicFX) {
	ClearToDefault(bStart);
	ClearToDefault(penSender);
}
BOOL EHudPicFX::CheckIDs(void) {	return TRUE ; }

CEntityEvent *ECredits::MakeCopy(void) { 	CEntityEvent *peeCopy = new ECredits(*this); 	return peeCopy;}
SLONG ECredits::GetSizeOf(void) { 	return sizeof(*this);}
ECredits::ECredits() : CEntityEvent(EVENTCODE_ECredits) {
	ClearToDefault(bStart);
	ClearToDefault(penSender);
}
BOOL ECredits::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EVoiceMessage::MakeCopy(void) { 	CEntityEvent *peeCopy = new EVoiceMessage(*this); 	return peeCopy;}
SLONG EVoiceMessage::GetSizeOf(void) { 	return sizeof(*this);}
EVoiceMessage::EVoiceMessage() : CEntityEvent(EVENTCODE_EVoiceMessage) {
	ClearToDefault(fnmMessage);
}
BOOL EVoiceMessage::CheckIDs(void) {	return TRUE ; }

CEntityEvent *EHitBySpaceShipBeam::MakeCopy(void) { 	CEntityEvent *peeCopy = new EHitBySpaceShipBeam(*this); 	return peeCopy;}
SLONG EHitBySpaceShipBeam::GetSizeOf(void) { 	return sizeof(*this);}
EHitBySpaceShipBeam::EHitBySpaceShipBeam() : CEntityEvent(EVENTCODE_EHitBySpaceShipBeam) {
}
BOOL EHitBySpaceShipBeam::CheckIDs(void) {	return TRUE ; }

void CGlobal::SetDefaultProperties(void) {
	CEntity::SetDefaultProperties();
}
BOOL CGlobal::
#line 397 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Global.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CGlobal_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CGlobal::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 398 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Global.es"
ASSERTALWAYS  ("DON'T INSTANTIATE THIS CLASS"); ASSERT(FALSE); return TRUE;};