/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EChangeGravity_New(void) { return new EChangeGravity; };
CDLLEntityEvent DLLEvent_EChangeGravity = {
	0x00e30000, &EChangeGravity_New
};
CDLLEntityEvent *CGravityRouter_events[] = {
	&DLLEvent_EChangeGravity,
};
#define CGravityRouter_eventsct ARRAYCOUNT(CGravityRouter_events)
#define ENTITYCLASS CGravityRouter

CEntityProperty CGravityRouter_properties[] = {
	CEntityProperty()
};
#define CGravityRouter_propertiesct 0


CEntityComponent CGravityRouter_components[] = {
#define MODEL_MARKER ((0x000000e3<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MARKER, 2, "EFNM" "Data\\Models\\Editor\\GravityRouter.mdl"),
#define TEXTURE_MARKER ((0x000000e3<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MARKER, 2, "EFNM" "Data\\Models\\Editor\\GravityRouter.tex"),
};
#define CGravityRouter_componentsct ARRAYCOUNT(CGravityRouter_components)

CEventHandlerEntry CGravityRouter_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CGravityRouter::
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/GravityRouter.es"
Main),DEBUGSTRING("CGravityRouter::Main")},
};
#define CGravityRouter_handlersct ARRAYCOUNT(CGravityRouter_handlers)

CEntity *CGravityRouter_New(void) { return new CGravityRouter; };
void CGravityRouter_OnInitClass(void) {};
void CGravityRouter_OnEndClass(void) {};
void CGravityRouter_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CGravityRouter_OnWorldEnd(CWorld *pwo) {};
void CGravityRouter_OnWorldInit(CWorld *pwo) {};
void CGravityRouter_OnWorldTick(CWorld *pwo) {};
void CGravityRouter_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CGravityRouter, CMarker, "Gravity Router", "Thumbnails\\GravityRouter.tbn", 0x000000e3);
DECLARE_CTFILENAME(_fnmCGravityRouter_tbn, "Thumbnails\\GravityRouter.tbn");
