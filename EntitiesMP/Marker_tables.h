/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CMarker_events[] = {NULL};
#define CMarker_eventsct 0
#define ENTITYCLASS CMarker

CEntityProperty CMarker_properties[] = {
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ca<<8)+1, offsetof(CMarker, m_strName), "Name", 'N', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ca<<8)+3, offsetof(CMarker, m_strDescription), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000ca<<8)+2, offsetof(CMarker, m_penTarget), "Target", 'T', C_dGREEN  | 0xFF, 0),
};
#define CMarker_propertiesct ARRAYCOUNT(CMarker_properties)

CEntityComponent CMarker_components[] = {
#define MODEL_MARKER ((0x000000ca<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MARKER, 2, "EFNM" "Data\\Models\\Editor\\Axis.mdl"),
#define TEXTURE_MARKER ((0x000000ca<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MARKER, 2, "EFNM" "Data\\Models\\Editor\\Vector.tex"),
};
#define CMarker_componentsct ARRAYCOUNT(CMarker_components)

CEventHandlerEntry CMarker_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CMarker::
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Marker.es"
Main),DEBUGSTRING("CMarker::Main")},
};
#define CMarker_handlersct ARRAYCOUNT(CMarker_handlers)

CEntity *CMarker_New(void) { return new CMarker; };
void CMarker_OnInitClass(void) {};
void CMarker_OnEndClass(void) {};
void CMarker_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CMarker_OnWorldEnd(CWorld *pwo) {};
void CMarker_OnWorldInit(CWorld *pwo) {};
void CMarker_OnWorldTick(CWorld *pwo) {};
void CMarker_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CMarker, CEntity, "Marker", "Thumbnails\\Marker.tbn", 0x000000ca);
DECLARE_CTFILENAME(_fnmCMarker_tbn, "Thumbnails\\Marker.tbn");
