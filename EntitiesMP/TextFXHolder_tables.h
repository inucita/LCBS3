/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CTextFXHolder_events[] = {NULL};
#define CTextFXHolder_eventsct 0
#define ENTITYCLASS CTextFXHolder

CEntityProperty CTextFXHolder_properties[] = {
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ef<<8)+1, offsetof(CTextFXHolder, m_strName), "Name", 'N', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ef<<8)+2, offsetof(CTextFXHolder, m_strDescription), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FILENAME, NULL, (0x000000ef<<8)+3, offsetof(CTextFXHolder, m_fnmMessage), "Text file", 'T', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ef<<8)+4, offsetof(CTextFXHolder, m_tmFadeInStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ef<<8)+5, offsetof(CTextFXHolder, m_tmFadeOutStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ef<<8)+6, offsetof(CTextFXHolder, m_tmFadeInLen), "Fade in time", 'I', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ef<<8)+7, offsetof(CTextFXHolder, m_tmFadeOutLen), "Fade out time", 'O', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ef<<8)+8, offsetof(CTextFXHolder, m_tmAutoFadeOut), "Auto fade out time", 'A', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x000000ef<<8)+20, offsetof(CTextFXHolder, m_bDataError), "", 0, 0, 0),
};
#define CTextFXHolder_propertiesct ARRAYCOUNT(CTextFXHolder_properties)

CEntityComponent CTextFXHolder_components[] = {
#define MODEL_MARKER ((0x000000ef<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MARKER, 2, "EFNM" "Data\\Models\\Editor\\MessageHolder.mdl"),
#define TEXTURE_MARKER ((0x000000ef<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MARKER, 2, "EFNM" "Data\\Models\\Editor\\MessageHolder.tex"),
};
#define CTextFXHolder_componentsct ARRAYCOUNT(CTextFXHolder_components)

CEventHandlerEntry CTextFXHolder_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CTextFXHolder::
#line 174 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TextFXHolder.es"
Main),DEBUGSTRING("CTextFXHolder::Main")},
 {0x00ef0000, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0000_Main_01), DEBUGSTRING("CTextFXHolder::H0x00ef0000_Main_01")},
 {0x00ef0001, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0001_Main_02), DEBUGSTRING("CTextFXHolder::H0x00ef0001_Main_02")},
 {0x00ef0002, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0002_Main_03), DEBUGSTRING("CTextFXHolder::H0x00ef0002_Main_03")},
 {0x00ef0003, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0003_Main_04), DEBUGSTRING("CTextFXHolder::H0x00ef0003_Main_04")},
 {0x00ef0004, -1, CEntity::pEventHandler(&CTextFXHolder::
#line 229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TextFXHolder.es"
WaitAndFadeOut),DEBUGSTRING("CTextFXHolder::WaitAndFadeOut")},
 {0x00ef0005, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0005_WaitAndFadeOut_01), DEBUGSTRING("CTextFXHolder::H0x00ef0005_WaitAndFadeOut_01")},
 {0x00ef0006, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0006_WaitAndFadeOut_02), DEBUGSTRING("CTextFXHolder::H0x00ef0006_WaitAndFadeOut_02")},
 {0x00ef0007, -1, CEntity::pEventHandler(&CTextFXHolder::
#line 235 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TextFXHolder.es"
ApplyFadeOut),DEBUGSTRING("CTextFXHolder::ApplyFadeOut")},
 {0x00ef0008, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0008_ApplyFadeOut_01), DEBUGSTRING("CTextFXHolder::H0x00ef0008_ApplyFadeOut_01")},
 {0x00ef0009, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef0009_ApplyFadeOut_02), DEBUGSTRING("CTextFXHolder::H0x00ef0009_ApplyFadeOut_02")},
 {0x00ef000a, -1, CEntity::pEventHandler(&CTextFXHolder::H0x00ef000a_ApplyFadeOut_03), DEBUGSTRING("CTextFXHolder::H0x00ef000a_ApplyFadeOut_03")},
};
#define CTextFXHolder_handlersct ARRAYCOUNT(CTextFXHolder_handlers)

CEntity *CTextFXHolder_New(void) { return new CTextFXHolder; };
void CTextFXHolder_OnInitClass(void) {};
void CTextFXHolder_OnEndClass(void) {};
void CTextFXHolder_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CTextFXHolder_OnWorldEnd(CWorld *pwo) {};
void CTextFXHolder_OnWorldInit(CWorld *pwo) {};
void CTextFXHolder_OnWorldTick(CWorld *pwo) {};
void CTextFXHolder_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CTextFXHolder, CRationalEntity, "TextFXHolder", "Thumbnails\\TextFXHodler.tbn", 0x000000ef);
DECLARE_CTFILENAME(_fnmCTextFXHolder_tbn, "Thumbnails\\TextFXHodler.tbn");
