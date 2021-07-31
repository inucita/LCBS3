/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EStop_New(void) { return new EStop; };
CDLLEntityEvent DLLEvent_EStop = {
  0x00000000, &EStop_New
};
CEntityEvent *EStart_New(void) { return new EStart; };
CDLLEntityEvent DLLEvent_EStart = {
  0x00000001, &EStart_New
};
CEntityEvent *EActivate_New(void) { return new EActivate; };
CDLLEntityEvent DLLEvent_EActivate = {
  0x00000002, &EActivate_New
};
CEntityEvent *EDeactivate_New(void) { return new EDeactivate; };
CDLLEntityEvent DLLEvent_EDeactivate = {
  0x00000003, &EDeactivate_New
};
CEntityEvent *EEnvironmentStart_New(void) { return new EEnvironmentStart; };
CDLLEntityEvent DLLEvent_EEnvironmentStart = {
  0x00000004, &EEnvironmentStart_New
};
CEntityEvent *EEnvironmentStop_New(void) { return new EEnvironmentStop; };
CDLLEntityEvent DLLEvent_EEnvironmentStop = {
  0x00000005, &EEnvironmentStop_New
};
CEntityEvent *EEnd_New(void) { return new EEnd; };
CDLLEntityEvent DLLEvent_EEnd = {
  0x00000006, &EEnd_New
};
CEntityEvent *ETrigger_New(void) { return new ETrigger; };
CDLLEntityEvent DLLEvent_ETrigger = {
  0x00000007, &ETrigger_New
};
CEntityEvent *ETeleportMovingBrush_New(void) { return new ETeleportMovingBrush; };
CDLLEntityEvent DLLEvent_ETeleportMovingBrush = {
  0x00000008, &ETeleportMovingBrush_New
};
CEntityEvent *EReminder_New(void) { return new EReminder; };
CDLLEntityEvent DLLEvent_EReminder = {
  0x00000009, &EReminder_New
};
CEntityEvent *EStartAttack_New(void) { return new EStartAttack; };
CDLLEntityEvent DLLEvent_EStartAttack = {
  0x0000000a, &EStartAttack_New
};
CEntityEvent *EStopAttack_New(void) { return new EStopAttack; };
CDLLEntityEvent DLLEvent_EStopAttack = {
  0x0000000b, &EStopAttack_New
};
CEntityEvent *EStopBlindness_New(void) { return new EStopBlindness; };
CDLLEntityEvent DLLEvent_EStopBlindness = {
  0x0000000c, &EStopBlindness_New
};
CEntityEvent *EStopDeafness_New(void) { return new EStopDeafness; };
CDLLEntityEvent DLLEvent_EStopDeafness = {
  0x0000000d, &EStopDeafness_New
};
CEntityEvent *EAddHostile_New(void) { return new EAddHostile; };
CDLLEntityEvent DLLEvent_EAddHostile = {
  0x0000000e, &EAddHostile_New
};
EP_ENUMBEG(CauseOfDeath)
  EP_ENUMVALUE(COD_OTHER, ""),
  EP_ENUMVALUE(COD_KNIFE, ""),
  EP_ENUMVALUE(COD_CHAINSAW, ""),
  EP_ENUMVALUE(COD_BOMB, ""),
  EP_ENUMVALUE(COD_CANNON, ""),
EP_ENUMEND(CauseOfDeath);

EP_ENUMBEG(KillType)
  EP_ENUMVALUE(KT_PLAYER, ""),
  EP_ENUMVALUE(KT_ENEMY, ""),
  EP_ENUMVALUE(KT_OTHER, ""),
  EP_ENUMVALUE(KT_MPVICTIM, ""),
EP_ENUMEND(KillType);

CEntityEvent *EKilledEnemy_New(void) { return new EKilledEnemy; };
CDLLEntityEvent DLLEvent_EKilledEnemy = {
  0x0000000f, &EKilledEnemy_New
};
EP_ENUMBEG(BoolEType)
  EP_ENUMVALUE(BET_TRUE, "True"),
  EP_ENUMVALUE(BET_FALSE, "False"),
  EP_ENUMVALUE(BET_IGNORE, "Ignore"),
EP_ENUMEND(BoolEType);

EP_ENUMBEG(EventEType)
  EP_ENUMVALUE(EET_START, "Start event"),
  EP_ENUMVALUE(EET_STOP, "Stop event"),
  EP_ENUMVALUE(EET_TRIGGER, "Trigger event"),
  EP_ENUMVALUE(EET_IGNORE, "Don't send event"),
  EP_ENUMVALUE(EET_ACTIVATE, "Activate event"),
  EP_ENUMVALUE(EET_DEACTIVATE, "Deactivate event"),
  EP_ENUMVALUE(EET_ENVIRONMENTSTART, "Start environment event"),
  EP_ENUMVALUE(EET_ENVIRONMENTSTOP, "Stop environment event"),
  EP_ENUMVALUE(EET_STARTATTACK, "OBSOLETE! - Start attack event"),
  EP_ENUMVALUE(EET_STOPATTACK, "OBSOLETE! - Stop attack event"),
  EP_ENUMVALUE(EET_STOPBLINDNESS, "Stop blindness event"),
  EP_ENUMVALUE(EET_STOPDEAFNESS, "Stop deafness event"),
  EP_ENUMVALUE(EET_TELEPORTMOVINGBRUSH, "Teleport moving brush"),
  EP_ENUMVALUE(EET_ADDHOSTILE, "Add Hostile event"),
EP_ENUMEND(EventEType);

EP_ENUMBEG(EntityInfoBodyType)
  EP_ENUMVALUE(EIBT_FLESH, "Flesh"),
  EP_ENUMVALUE(EIBT_WATER, "Water"),
  EP_ENUMVALUE(EIBT_ROCK, "Rock "),
  EP_ENUMVALUE(EIBT_FIRE, "Fire "),
  EP_ENUMVALUE(EIBT_AIR, "Air  "),
  EP_ENUMVALUE(EIBT_BONES, "Bones"),
  EP_ENUMVALUE(EIBT_WOOD, "Wood "),
  EP_ENUMVALUE(EIBT_METAL, "Metal"),
  EP_ENUMVALUE(EIBT_ROBOT, "Robot"),
  EP_ENUMVALUE(EIBT_ICE, "Ice"),
EP_ENUMEND(EntityInfoBodyType);

EP_ENUMBEG(MessageSound)
  EP_ENUMVALUE(MSS_NONE, "None"),
  EP_ENUMVALUE(MSS_INFO, "Info"),
EP_ENUMEND(MessageSound);

EP_ENUMBEG(ParticleTexture)
  EP_ENUMVALUE(PT_STAR01, "Star01"),
  EP_ENUMVALUE(PT_STAR02, "Star02"),
  EP_ENUMVALUE(PT_STAR03, "Star03"),
  EP_ENUMVALUE(PT_STAR04, "Star04"),
  EP_ENUMVALUE(PT_STAR05, "Star05"),
  EP_ENUMVALUE(PT_STAR06, "Star06"),
  EP_ENUMVALUE(PT_STAR07, "Star07"),
  EP_ENUMVALUE(PT_STAR08, "Star08"),
  EP_ENUMVALUE(PT_BOUBBLE01, "Boubble01"),
  EP_ENUMVALUE(PT_BOUBBLE02, "Boubble02"),
  EP_ENUMVALUE(PT_WATER01, "Water01"),
  EP_ENUMVALUE(PT_WATER02, "Water02"),
  EP_ENUMVALUE(PT_SANDFLOW, "Sand flow"),
  EP_ENUMVALUE(PT_WATERFLOW, "Water flow"),
  EP_ENUMVALUE(PT_LAVAFLOW, "Lava flow"),
EP_ENUMEND(ParticleTexture);

EP_ENUMBEG(SoundType)
  EP_ENUMVALUE(SNDT_NONE, ""),
  EP_ENUMVALUE(SNDT_SHOUT, ""),
  EP_ENUMVALUE(SNDT_YELL, ""),
  EP_ENUMVALUE(SNDT_EXPLOSION, ""),
  EP_ENUMVALUE(SNDT_PLAYER, ""),
EP_ENUMEND(SoundType);

EP_ENUMBEG(BulletHitType)
  EP_ENUMVALUE(BHT_NONE, ""),
  EP_ENUMVALUE(BHT_FLESH, ""),
  EP_ENUMVALUE(BHT_BRUSH_STONE, ""),
  EP_ENUMVALUE(BHT_BRUSH_SAND, ""),
  EP_ENUMVALUE(BHT_BRUSH_WATER, ""),
  EP_ENUMVALUE(BHT_BRUSH_UNDER_WATER, ""),
  EP_ENUMVALUE(BHT_ACID, ""),
  EP_ENUMVALUE(BHT_BRUSH_RED_SAND, ""),
  EP_ENUMVALUE(BHT_BRUSH_GRASS, ""),
  EP_ENUMVALUE(BHT_BRUSH_WOOD, ""),
  EP_ENUMVALUE(BHT_BRUSH_SNOW, ""),
EP_ENUMEND(BulletHitType);

EP_ENUMBEG(EffectParticlesType)
  EP_ENUMVALUE(EPT_NONE, ""),
  EP_ENUMVALUE(EPT_BULLET_STONE, ""),
  EP_ENUMVALUE(EPT_BULLET_SAND, ""),
  EP_ENUMVALUE(EPT_BULLET_WATER, ""),
  EP_ENUMVALUE(EPT_BULLET_UNDER_WATER, ""),
  EP_ENUMVALUE(EPT_BULLET_RED_SAND, ""),
  EP_ENUMVALUE(EPT_BULLET_GRASS, ""),
  EP_ENUMVALUE(EPT_BULLET_WOOD, ""),
  EP_ENUMVALUE(EPT_BULLET_SNOW, ""),
EP_ENUMEND(EffectParticlesType);

EP_ENUMBEG(SprayParticlesType)
  EP_ENUMVALUE(SPT_NONE, "None"),
  EP_ENUMVALUE(SPT_BLOOD, "Blood"),
  EP_ENUMVALUE(SPT_BONES, "Bones"),
  EP_ENUMVALUE(SPT_FEATHER, "Feather"),
  EP_ENUMVALUE(SPT_STONES, "Stones"),
  EP_ENUMVALUE(SPT_WOOD, "Wood"),
  EP_ENUMVALUE(SPT_SLIME, "Slime"),
  EP_ENUMVALUE(SPT_LAVA_STONES, "Lava Stones"),
  EP_ENUMVALUE(SPT_ELECTRICITY_SPARKS, "Electricity sparks"),
  EP_ENUMVALUE(SPT_BEAST_PROJECTILE_SPRAY, "Beast projectile spray"),
  EP_ENUMVALUE(SPT_SMALL_LAVA_STONES, "Small Lava Stones"),
  EP_ENUMVALUE(SPT_AIRSPOUTS, "Air"),
  EP_ENUMVALUE(SPT_ELECTRICITY_SPARKS_NO_BLOOD, "Electricity w/o blood"),
  EP_ENUMVALUE(SPT_PLASMA, "Plasma"),
  EP_ENUMVALUE(SPT_GOO, "Goo"),
  EP_ENUMVALUE(SPT_TREE01, "Tree 01"),
  EP_ENUMVALUE(SPT_COLOREDSTONE, "Colored stone"),
EP_ENUMEND(SprayParticlesType);

EP_ENUMBEG(WeaponBits)
  EP_ENUMVALUE(WB_00, "Knife"),
  EP_ENUMVALUE(WB_09, "Chainsaw"),
  EP_ENUMVALUE(WB_10, "Flamer"),
  EP_ENUMVALUE(WB_14, ""),
  EP_ENUMVALUE(WB_15, ""),
  EP_ENUMVALUE(WB_16, ""),
  EP_ENUMVALUE(WB_17, ""),
  EP_ENUMVALUE(WB_18, ""),
  EP_ENUMVALUE(WB_19, ""),
  EP_ENUMVALUE(WB_20, ""),
  EP_ENUMVALUE(WB_21, ""),
  EP_ENUMVALUE(WB_22, ""),
  EP_ENUMVALUE(WB_23, ""),
  EP_ENUMVALUE(WB_24, ""),
  EP_ENUMVALUE(WB_25, ""),
  EP_ENUMVALUE(WB_26, ""),
  EP_ENUMVALUE(WB_27, ""),
  EP_ENUMVALUE(WB_28, ""),
  EP_ENUMVALUE(WB_29, ""),
  EP_ENUMVALUE(WB_30, ""),
  EP_ENUMVALUE(WB_31, ""),
EP_ENUMEND(WeaponBits);

EP_ENUMBEG(ClasificationBits)
  EP_ENUMVALUE(CB_00, "Bit 16"),
  EP_ENUMVALUE(CB_01, "Bit 17"),
  EP_ENUMVALUE(CB_02, "Bit 18"),
  EP_ENUMVALUE(CB_03, "Bit 19"),
  EP_ENUMVALUE(CB_04, "Bit 20"),
  EP_ENUMVALUE(CB_05, "Bit 21"),
  EP_ENUMVALUE(CB_06, "Bit 22"),
  EP_ENUMVALUE(CB_07, "Bit 23"),
  EP_ENUMVALUE(CB_08, "Bit 24"),
  EP_ENUMVALUE(CB_09, "Bit 25"),
  EP_ENUMVALUE(CB_10, "Bit 26"),
  EP_ENUMVALUE(CB_11, "Bit 27"),
  EP_ENUMVALUE(CB_12, "Bit 28"),
  EP_ENUMVALUE(CB_13, "Bit 29"),
  EP_ENUMVALUE(CB_14, "Bit 30"),
  EP_ENUMVALUE(CB_15, "Bit 31"),
EP_ENUMEND(ClasificationBits);

EP_ENUMBEG(VisibilityBits)
  EP_ENUMVALUE(VB_00, "Bit 00"),
  EP_ENUMVALUE(VB_01, "Bit 01"),
  EP_ENUMVALUE(VB_02, "Bit 02"),
  EP_ENUMVALUE(VB_03, "Bit 03"),
  EP_ENUMVALUE(VB_04, "Bit 04"),
  EP_ENUMVALUE(VB_05, "Bit 05"),
  EP_ENUMVALUE(VB_06, "Bit 06"),
  EP_ENUMVALUE(VB_07, "Bit 07"),
  EP_ENUMVALUE(VB_08, "Bit 08"),
  EP_ENUMVALUE(VB_09, "Bit 09"),
  EP_ENUMVALUE(VB_10, "Bit 10"),
  EP_ENUMVALUE(VB_11, "Bit 11"),
  EP_ENUMVALUE(VB_12, "Bit 12"),
  EP_ENUMVALUE(VB_13, "Bit 13"),
  EP_ENUMVALUE(VB_14, "Bit 14"),
  EP_ENUMVALUE(VB_15, "Bit 15"),
EP_ENUMEND(VisibilityBits);

EP_ENUMBEG(ZoneBits)
  EP_ENUMVALUE(ZONE_00, "Zone 00"),
  EP_ENUMVALUE(ZONE_01, "Zone 01"),
  EP_ENUMVALUE(ZONE_02, "Zone 02"),
  EP_ENUMVALUE(ZONE_03, "Zone 03"),
  EP_ENUMVALUE(ZONE_04, "Zone 04"),
  EP_ENUMVALUE(ZONE_05, "Zone 05"),
  EP_ENUMVALUE(ZONE_06, "Zone 06"),
  EP_ENUMVALUE(ZONE_07, "Zone 07"),
  EP_ENUMVALUE(ZONE_08, "Zone 08"),
  EP_ENUMVALUE(ZONE_09, "Zone 09"),
  EP_ENUMVALUE(ZONE_10, "Zone 10"),
  EP_ENUMVALUE(ZONE_11, "Zone 11"),
  EP_ENUMVALUE(ZONE_12, "Zone 12"),
  EP_ENUMVALUE(ZONE_13, "Zone 13"),
  EP_ENUMVALUE(ZONE_14, "Zone 14"),
  EP_ENUMVALUE(ZONE_15, "Zone 15"),
  EP_ENUMVALUE(ZONE_16, "Zone 16"),
  EP_ENUMVALUE(ZONE_17, "Zone 17"),
  EP_ENUMVALUE(ZONE_18, "Zone 18"),
  EP_ENUMVALUE(ZONE_19, "Zone 19"),
  EP_ENUMVALUE(ZONE_20, "Zone 20"),
  EP_ENUMVALUE(ZONE_21, "Zone 21"),
  EP_ENUMVALUE(ZONE_22, "Zone 22"),
  EP_ENUMVALUE(ZONE_23, "Zone 23"),
  EP_ENUMVALUE(ZONE_24, "Zone 24"),
  EP_ENUMVALUE(ZONE_25, "Zone 25"),
  EP_ENUMVALUE(ZONE_26, "Zone 26"),
  EP_ENUMVALUE(ZONE_27, "Zone 27"),
  EP_ENUMVALUE(ZONE_28, "Zone 28"),
  EP_ENUMVALUE(ZONE_29, "Zone 29"),
  EP_ENUMVALUE(ZONE_30, "Zone 30"),
  EP_ENUMVALUE(ZONE_31, "Zone 31"),
EP_ENUMEND(ZoneBits);

EP_ENUMBEG(ExtraBits)
  EP_ENUMVALUE(Extra_00, "Extra 00"),
  EP_ENUMVALUE(Extra_01, "Extra 01"),
  EP_ENUMVALUE(Extra_02, "Extra 02"),
  EP_ENUMVALUE(Extra_03, "Extra 03"),
  EP_ENUMVALUE(Extra_04, "Extra 04"),
  EP_ENUMVALUE(Extra_05, "Extra 05"),
  EP_ENUMVALUE(Extra_06, "Extra 06"),
  EP_ENUMVALUE(Extra_07, "Extra 07"),
  EP_ENUMVALUE(Extra_08, "Extra 08"),
  EP_ENUMVALUE(Extra_09, "Extra 09"),
  EP_ENUMVALUE(Extra_10, "Extra 10"),
  EP_ENUMVALUE(Extra_11, "Extra 11"),
  EP_ENUMVALUE(Extra_12, "Extra 12"),
  EP_ENUMVALUE(Extra_13, "Extra 13"),
  EP_ENUMVALUE(Extra_14, "Extra 14"),
  EP_ENUMVALUE(Extra_15, "Extra 15"),
  EP_ENUMVALUE(Extra_16, "Extra 16"),
  EP_ENUMVALUE(Extra_17, "Extra 17"),
  EP_ENUMVALUE(Extra_18, "Extra 18"),
  EP_ENUMVALUE(Extra_19, "Extra 19"),
  EP_ENUMVALUE(Extra_20, "Extra 20"),
  EP_ENUMVALUE(Extra_21, "Extra 21"),
  EP_ENUMVALUE(Extra_22, "Extra 22"),
  EP_ENUMVALUE(Extra_23, "Extra 23"),
  EP_ENUMVALUE(Extra_24, "Extra 24"),
  EP_ENUMVALUE(Extra_25, "Extra 25"),
  EP_ENUMVALUE(Extra_26, "Extra 26"),
  EP_ENUMVALUE(Extra_27, "Extra 27"),
  EP_ENUMVALUE(Extra_28, "Extra 28"),
  EP_ENUMVALUE(Extra_29, "Extra 29"),
  EP_ENUMVALUE(Extra_30, "Extra 30"),
  EP_ENUMVALUE(Extra_31, "Extra 31"),
EP_ENUMEND(ExtraBits);

CEntityEvent *ESound_New(void) { return new ESound; };
CDLLEntityEvent DLLEvent_ESound = {
  0x00000010, &ESound_New
};
CEntityEvent *EScroll_New(void) { return new EScroll; };
CDLLEntityEvent DLLEvent_EScroll = {
  0x00000011, &EScroll_New
};
CEntityEvent *ETextFX_New(void) { return new ETextFX; };
CDLLEntityEvent DLLEvent_ETextFX = {
  0x00000012, &ETextFX_New
};
CEntityEvent *EHudPicFX_New(void) { return new EHudPicFX; };
CDLLEntityEvent DLLEvent_EHudPicFX = {
  0x00000013, &EHudPicFX_New
};
CEntityEvent *ECredits_New(void) { return new ECredits; };
CDLLEntityEvent DLLEvent_ECredits = {
  0x00000014, &ECredits_New
};
CEntityEvent *EVoiceMessage_New(void) { return new EVoiceMessage; };
CDLLEntityEvent DLLEvent_EVoiceMessage = {
  0x00000015, &EVoiceMessage_New
};
CEntityEvent *EHitBySpaceShipBeam_New(void) { return new EHitBySpaceShipBeam; };
CDLLEntityEvent DLLEvent_EHitBySpaceShipBeam = {
  0x00000016, &EHitBySpaceShipBeam_New
};
CDLLEntityEvent *CGlobal_events[] = {
  &DLLEvent_EHitBySpaceShipBeam,
  &DLLEvent_EVoiceMessage,
  &DLLEvent_ECredits,
  &DLLEvent_EHudPicFX,
  &DLLEvent_ETextFX,
  &DLLEvent_EScroll,
  &DLLEvent_ESound,
  &DLLEvent_EKilledEnemy,
  &DLLEvent_EAddHostile,
  &DLLEvent_EStopDeafness,
  &DLLEvent_EStopBlindness,
  &DLLEvent_EStopAttack,
  &DLLEvent_EStartAttack,
  &DLLEvent_EReminder,
  &DLLEvent_ETeleportMovingBrush,
  &DLLEvent_ETrigger,
  &DLLEvent_EEnd,
  &DLLEvent_EEnvironmentStop,
  &DLLEvent_EEnvironmentStart,
  &DLLEvent_EDeactivate,
  &DLLEvent_EActivate,
  &DLLEvent_EStart,
  &DLLEvent_EStop,
};
#define CGlobal_eventsct ARRAYCOUNT(CGlobal_events)
#define ENTITYCLASS CGlobal

CEntityProperty CGlobal_properties[] = {
  CEntityProperty()
};
#define CGlobal_propertiesct 0


CEntityComponent CGlobal_components[] = {
  CEntityComponent()
};
#define CGlobal_componentsct 0


CEventHandlerEntry CGlobal_handlers[] = {
 {1, -1, CEntity::pEventHandler(&CGlobal::
#line 392 "E:/USA/EntitiesMP/Global.es"
Main),DEBUGSTRING("CGlobal::Main")},
};
#define CGlobal_handlersct ARRAYCOUNT(CGlobal_handlers)

CEntity *CGlobal_New(void) { return new CGlobal; };
void CGlobal_OnInitClass(void) {};
void CGlobal_OnEndClass(void) {};
void CGlobal_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CGlobal_OnWorldEnd(CWorld *pwo) {};
void CGlobal_OnWorldInit(CWorld *pwo) {};
void CGlobal_OnWorldTick(CWorld *pwo) {};
void CGlobal_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CGlobal, CEntity, "", "", 0x00000000);
DECLARE_CTFILENAME(_fnmCGlobal_tbn, "");