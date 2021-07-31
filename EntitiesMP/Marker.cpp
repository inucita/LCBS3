/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"

#include "StdH.h"

#include <Marker.h>
#include <Marker_tables.h>
void CMarker::SetDefaultProperties(void) {
	m_strName = "Marker";
	m_strDescription = "";
	m_penTarget = NULL;
	CEntity::SetDefaultProperties();
}

#line 26 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
const CTString & CMarker::GetDescription(void)const 
#line 27 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
{
#line 28 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
((CTString &) m_strDescription ) . PrintF  ("-><none>");
#line 29 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
if(m_penTarget  != NULL ){
#line 30 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
((CTString &) m_strDescription ) . PrintF  ("->%s" , m_penTarget  -> GetName  ());
#line 31 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
}
#line 32 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
return m_strDescription ;
#line 33 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
}

#line 36 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
BOOL CMarker::MovesByTargetedRoute(CTString & strTargetProperty)const {
#line 37 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
strTargetProperty  = "Target";
#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
return TRUE ;
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
}

#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
BOOL CMarker::DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const {
#line 42 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
fnmMarkerClass  = CTFILENAME  ("Classes\\Marker.ecl");
#line 43 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
strTargetProperty  = "Target";
#line 44 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
return TRUE ;
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
}
BOOL CMarker::IsMarker(void)const 
#line 50 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
{
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
return TRUE ;
#line 52 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
}

#line 56 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
SLONG CMarker::GetUsedMemory(void) 
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
{
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
SLONG  slUsedMemory  = sizeof  (CMarker ) - sizeof  (CEntity ) + CEntity  :: GetUsedMemory  ();
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
slUsedMemory  += m_strName  . Length  ();
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
slUsedMemory  += m_strDescription  . Length  ();
#line 63 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
return slUsedMemory ;
#line 64 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
}
BOOL CMarker::
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CMarker_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CMarker::Main expects 'EVoid' as input!");	const EVoid &e = (const EVoid &)__eeInput;
#line 72 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
InitAsEditorModel  ();
#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
SetModel  (MODEL_MARKER );
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
SetModelMainTexture  (TEXTURE_MARKER );
#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
Return(STATE_CURRENT,EVoid());
#line 80 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
return TRUE; ASSERT(FALSE); return TRUE;};