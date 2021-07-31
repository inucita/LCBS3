/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EBulletInit_New(void) { return new EBulletInit; };
CDLLEntityEvent DLLEvent_EBulletInit = {
	0x01f60000, &EBulletInit_New
};
CDLLEntityEvent *CBullet_events[] = {
	&DLLEvent_EBulletInit,
};
#define CBullet_eventsct ARRAYCOUNT(CBullet_events)
#define ENTITYCLASS CBullet

CEntityProperty CBullet_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x000001f6<<8)+1, offsetof(CBullet, m_penOwner), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f6<<8)+2, offsetof(CBullet, m_fDamage), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f6<<8)+3, offsetof(CBullet, m_vTarget), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f6<<8)+4, offsetof(CBullet, m_vTargetCopy), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x000001f6<<8)+6, offsetof(CBullet, m_vHitPoint), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x000001f6<<8)+8, offsetof(CBullet, m_iBullet), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &DamageType_enum, (0x000001f6<<8)+9, offsetof(CBullet, m_EdtDamage), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x000001f6<<8)+10, offsetof(CBullet, m_fBulletSize), "", 0, 0, 0),
};
#define CBullet_propertiesct ARRAYCOUNT(CBullet_properties)

CEntityComponent CBullet_components[] = {
#define CLASS_BASIC_EFFECT ((0x000001f6<<8)+1)
 CEntityComponent(ECT_CLASS, CLASS_BASIC_EFFECT, 0, "EFNM" "Classes\\BasicEffect.ecl"),
};
#define CBullet_componentsct ARRAYCOUNT(CBullet_components)

CEventHandlerEntry CBullet_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CBullet::
#line 322 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
Main),DEBUGSTRING("CBullet::Main")},
};
#define CBullet_handlersct ARRAYCOUNT(CBullet_handlers)

CEntity *CBullet_New(void) { return new CBullet; };
void CBullet_OnInitClass(void) {};
void CBullet_OnEndClass(void) {};
void CBullet_OnPrecache(CDLLEntityClass *pdec, INDEX iUser);
void CBullet_OnWorldEnd(CWorld *pwo) {};
void CBullet_OnWorldInit(CWorld *pwo) {};
void CBullet_OnWorldTick(CWorld *pwo) {};
void CBullet_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CBullet, CEntity, "Bullet", "", 0x000001f6);
DECLARE_CTFILENAME(_fnmCBullet_tbn, "");
