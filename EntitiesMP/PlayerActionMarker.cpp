/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"

#include "StdH.h"

#include <EntitiesMP/PlayerActionMarker.h>
#include <EntitiesMP/PlayerActionMarker_tables.h>
void CPlayerActionMarker::SetDefaultProperties(void) {
  m_paaAction = PAA_RUN ;
  m_tmWait = 0.0f;
  m_penDoorController = NULL;
  m_penTrigger = NULL;
  m_fSpeed = 1.0f;
  m_penItem = NULL;
  CMarker::SetDefaultProperties();
}
  
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
const CTString & CPlayerActionMarker::GetDescription(void)const {
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
CTString strAction  = PlayerAutoAction_enum  . NameForValue  (INDEX (m_paaAction ));
#line 59 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
if(m_penTarget  == NULL ){
#line 60 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
((CTString &) m_strDescription ) . PrintF  ("%s (%s)-><none>" , m_strName  , strAction );
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}else {
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
((CTString &) m_strDescription ) . PrintF  ("%s (%s)->%s" , m_strName  , strAction  , 
#line 63 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
m_penTarget  -> GetName  ());
#line 64 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
return m_strDescription ;
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}
  
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
BOOL CPlayerActionMarker::DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const {
#line 70 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
fnmMarkerClass  = CTFILENAME  ("Classes\\PlayerActionMarker.ecl");
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
strTargetProperty  = "Target";
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
return TRUE ;
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}
  
#line 76 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
BOOL CPlayerActionMarker::HandleEvent(const CEntityEvent & ee) 
#line 77 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
{
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
if(ee  . ee_slEvent  == EVENTCODE_ETrigger ){
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
ETrigger  & eTrigger  = (ETrigger  &) ee ;
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
if(IsDerivedFromClass  (eTrigger  . penCaused  , "Player")){
#line 84 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
EAutoAction  eAutoAction ;
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
eAutoAction  . eidFirstMarker  = this ;
#line 86 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
eTrigger  . penCaused  -> SendEvent  (eAutoAction );
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}
#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
return TRUE ;
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
return FALSE ;
#line 91 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
}
BOOL CPlayerActionMarker::
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CPlayerActionMarker_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CPlayerActionMarker::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 97 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
InitAsEditorModel  ();
#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
SetModel  (MODEL_MARKER );
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
m_tmWait  = ClampDn  (m_tmWait  , 0.05f);
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
Return(STATE_CURRENT,EVoid());
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/PlayerActionMarker.es"
return TRUE; ASSERT(FALSE); return TRUE;};