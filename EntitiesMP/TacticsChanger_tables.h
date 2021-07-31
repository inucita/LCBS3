/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CDLLEntityEvent *CTacticsChanger_events[] = {NULL};
#define CTacticsChanger_eventsct 0
#define ENTITYCLASS CTacticsChanger

CEntityProperty CTacticsChanger_properties[] = {
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ec<<8)+1, offsetof(CTacticsChanger, m_strName), "Name", 'N', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x000000ec<<8)+2, offsetof(CTacticsChanger, m_strDescription), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &TacticType_enum, (0x000000ec<<8)+10, offsetof(CTacticsChanger, m_tctType), "Type", 'Y', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ec<<8)+11, offsetof(CTacticsChanger, m_fParam1), "Parameter 1", 'D', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ec<<8)+12, offsetof(CTacticsChanger, m_fParam2), "Parameter 2", 'F', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ec<<8)+13, offsetof(CTacticsChanger, m_fParam3), "Parameter 3", 'G', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ec<<8)+14, offsetof(CTacticsChanger, m_fParam4), "Parameter 4", 'H', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000000ec<<8)+15, offsetof(CTacticsChanger, m_fParam5), "Parameter 5", 'J', 0x7F0000FFUL, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000000ec<<8)+18, offsetof(CTacticsChanger, m_penTacticsHolder), "Tactics holder", 'H', 0x7F0000FFUL, 0),
};
#define CTacticsChanger_propertiesct ARRAYCOUNT(CTacticsChanger_properties)

CEntityComponent CTacticsChanger_components[] = {
#define MODEL_MANAGER ((0x000000ec<<8)+1)
 CEntityComponent(ECT_MODEL, MODEL_MANAGER, 2, "EFNM" "Data\\ModelsMP\\Editor\\TacticsChanger.mdl"),
#define TEXTURE_MANAGER ((0x000000ec<<8)+2)
 CEntityComponent(ECT_TEXTURE, TEXTURE_MANAGER, 2, "EFNM" "Data\\ModelsMP\\Editor\\TacticsChanger.tex"),
};
#define CTacticsChanger_componentsct ARRAYCOUNT(CTacticsChanger_components)

CEventHandlerEntry CTacticsChanger_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CTacticsChanger::
#line 37 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TacticsChanger.es"
Main),DEBUGSTRING("CTacticsChanger::Main")},
 {0x00ec0000, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0000_Main_01), DEBUGSTRING("CTacticsChanger::H0x00ec0000_Main_01")},
 {0x00ec0001, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0001_Main_02), DEBUGSTRING("CTacticsChanger::H0x00ec0001_Main_02")},
 {0x00ec0002, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0002_Main_03), DEBUGSTRING("CTacticsChanger::H0x00ec0002_Main_03")},
 {0x00ec0003, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0003_Main_04), DEBUGSTRING("CTacticsChanger::H0x00ec0003_Main_04")},
 {0x00ec0004, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0004_Main_05), DEBUGSTRING("CTacticsChanger::H0x00ec0004_Main_05")},
 {0x00ec0005, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0005_Main_06), DEBUGSTRING("CTacticsChanger::H0x00ec0005_Main_06")},
 {0x00ec0006, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0006_Main_07), DEBUGSTRING("CTacticsChanger::H0x00ec0006_Main_07")},
 {0x00ec0007, -1, CEntity::pEventHandler(&CTacticsChanger::H0x00ec0007_Main_08), DEBUGSTRING("CTacticsChanger::H0x00ec0007_Main_08")},
};
#define CTacticsChanger_handlersct ARRAYCOUNT(CTacticsChanger_handlers)

CEntity *CTacticsChanger_New(void) { return new CTacticsChanger; };
void CTacticsChanger_OnInitClass(void) {};
void CTacticsChanger_OnEndClass(void) {};
void CTacticsChanger_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CTacticsChanger_OnWorldEnd(CWorld *pwo) {};
void CTacticsChanger_OnWorldInit(CWorld *pwo) {};
void CTacticsChanger_OnWorldTick(CWorld *pwo) {};
void CTacticsChanger_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CTacticsChanger, CRationalEntity, "TacticsChanger", "Thumbnails\\TacticsChanger.tbn", 0x000000ec);
DECLARE_CTFILENAME(_fnmCTacticsChanger_tbn, "Thumbnails\\TacticsChanger.tbn");
