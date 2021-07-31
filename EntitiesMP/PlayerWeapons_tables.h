/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

CEntityEvent *EWeaponsInit_New(void) { return new EWeaponsInit; };
CDLLEntityEvent DLLEvent_EWeaponsInit = {
  0x01920000, &EWeaponsInit_New
};
CEntityEvent *ESelectWeapon_New(void) { return new ESelectWeapon; };
CDLLEntityEvent DLLEvent_ESelectWeapon = {
  0x01920001, &ESelectWeapon_New
};
CEntityEvent *EBoringWeapon_New(void) { return new EBoringWeapon; };
CDLLEntityEvent DLLEvent_EBoringWeapon = {
  0x01920002, &EBoringWeapon_New
};
CEntityEvent *EFireWeapon_New(void) { return new EFireWeapon; };
CDLLEntityEvent DLLEvent_EFireWeapon = {
  0x01920003, &EFireWeapon_New
};
CEntityEvent *EReleaseWeapon_New(void) { return new EReleaseWeapon; };
CDLLEntityEvent DLLEvent_EReleaseWeapon = {
  0x01920004, &EReleaseWeapon_New
};
CEntityEvent *EReloadWeapon_New(void) { return new EReloadWeapon; };
CDLLEntityEvent DLLEvent_EReloadWeapon = {
  0x01920005, &EReloadWeapon_New
};
CEntityEvent *EWeaponChanged_New(void) { return new EWeaponChanged; };
CDLLEntityEvent DLLEvent_EWeaponChanged = {
  0x01920006, &EWeaponChanged_New
};
CEntityEvent *ENetSelectWeapon_New(void) { return new ENetSelectWeapon; };
CDLLEntityEvent DLLEvent_ENetSelectWeapon = {
  0x01920007, &ENetSelectWeapon_New
};
CEntityEvent *ENetFireWeapon_New(void) { return new ENetFireWeapon; };
CDLLEntityEvent DLLEvent_ENetFireWeapon = {
  0x01920008, &ENetFireWeapon_New
};
CEntityEvent *ENetReleaseWeapon_New(void) { return new ENetReleaseWeapon; };
CDLLEntityEvent DLLEvent_ENetReleaseWeapon = {
  0x01920009, &ENetReleaseWeapon_New
};
CEntityEvent *ENetReloadWeapon_New(void) { return new ENetReloadWeapon; };
CDLLEntityEvent DLLEvent_ENetReloadWeapon = {
  0x0192000a, &ENetReloadWeapon_New
};
EP_ENUMBEG(WeaponType)
  EP_ENUMVALUE(WEAPON_NONE, ""),
  EP_ENUMVALUE(WEAPON_KNIFE, ""),
  EP_ENUMVALUE(WEAPON_CHAINSAW, ""),
  EP_ENUMVALUE(WEAPON_FLAMER, ""),
  EP_ENUMVALUE(WEAPON_LASER, ""),
  EP_ENUMVALUE(WEAPON_LAST, ""),
EP_ENUMEND(WeaponType);

CDLLEntityEvent *CPlayerWeapons_events[] = {
  &DLLEvent_ENetReloadWeapon,
  &DLLEvent_ENetReleaseWeapon,
  &DLLEvent_ENetFireWeapon,
  &DLLEvent_ENetSelectWeapon,
  &DLLEvent_EWeaponChanged,
  &DLLEvent_EReloadWeapon,
  &DLLEvent_EReleaseWeapon,
  &DLLEvent_EFireWeapon,
  &DLLEvent_EBoringWeapon,
  &DLLEvent_ESelectWeapon,
  &DLLEvent_EWeaponsInit,
};
#define CPlayerWeapons_eventsct ARRAYCOUNT(CPlayerWeapons_events)
#define ENTITYCLASS CPlayerWeapons

CEntityProperty CPlayerWeapons_properties[] = {
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+1, offsetof(CPlayerWeapons, m_penPlayer), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+2, offsetof(CPlayerWeapons, m_bFireWeapon), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+3, offsetof(CPlayerWeapons, m_bHasAmmo), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENUM, &WeaponType_enum, (0x00000192<<8)+4, offsetof(CPlayerWeapons, m_iCurrentWeapon), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_ENUM, &WeaponType_enum, (0x00000192<<8)+5, offsetof(CPlayerWeapons, m_iWantedWeapon), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_ENUM, &WeaponType_enum, (0x00000192<<8)+6, offsetof(CPlayerWeapons, m_iPreviousWeapon), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+11, offsetof(CPlayerWeapons, m_iAvailableWeapons), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+12, offsetof(CPlayerWeapons, m_bChangeWeapon), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+13, offsetof(CPlayerWeapons, m_bReloadWeapon), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+14, offsetof(CPlayerWeapons, m_bMirrorFire), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+15, offsetof(CPlayerWeapons, m_iAnim), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+16, offsetof(CPlayerWeapons, m_fAnimWaitTime), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+17, offsetof(CPlayerWeapons, m_tmRangeSoundSpawned), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_STRING, NULL, (0x00000192<<8)+18, offsetof(CPlayerWeapons, m_strLastTarget), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+19, offsetof(CPlayerWeapons, m_tmTargetingStarted), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+20, offsetof(CPlayerWeapons, m_tmLastTarget), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+21, offsetof(CPlayerWeapons, m_tmSnoopingStarted), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+22, offsetof(CPlayerWeapons, m_penTargeting), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_MODELOBJECT, NULL, (0x00000192<<8)+25, offsetof(CPlayerWeapons, m_moWeapon), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_MODELOBJECT, NULL, (0x00000192<<8)+26, offsetof(CPlayerWeapons, m_moWeaponSecond), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+27, offsetof(CPlayerWeapons, m_tmWeaponChangeRequired), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+30, offsetof(CPlayerWeapons, m_penRayHit), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+31, offsetof(CPlayerWeapons, m_bTargetHitIsUsable), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+32, offsetof(CPlayerWeapons, m_fRayHitDistance), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+34, offsetof(CPlayerWeapons, m_vRayHit), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+35, offsetof(CPlayerWeapons, m_vRayHitLast), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+36, offsetof(CPlayerWeapons, m_vBulletSource), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+37, offsetof(CPlayerWeapons, m_vBulletTarget), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+40, offsetof(CPlayerWeapons, m_iBullets), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+41, offsetof(CPlayerWeapons, m_iMaxBullets), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+42, offsetof(CPlayerWeapons, m_iShells), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+43, offsetof(CPlayerWeapons, m_iMaxShells), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+48, offsetof(CPlayerWeapons, m_iNapalm), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+49, offsetof(CPlayerWeapons, m_iMaxNapalm), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+50, offsetof(CPlayerWeapons, m_iElectricity), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+51, offsetof(CPlayerWeapons, m_iMaxElectricity), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+52, offsetof(CPlayerWeapons, m_iIronBalls), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+53, offsetof(CPlayerWeapons, m_iMaxIronBalls), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+70, offsetof(CPlayerWeapons, m_iKnifeStand), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+76, offsetof(CPlayerWeapons, m_iLastBulletPosition), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+77, offsetof(CPlayerWeapons, m_iBulletsOnFireStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+84, offsetof(CPlayerWeapons, m_penFlame), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+85, offsetof(CPlayerWeapons, m_iLaserBarrel), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+86, offsetof(CPlayerWeapons, m_iFlare), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+87, offsetof(CPlayerWeapons, m_iSecondFlare), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+88, offsetof(CPlayerWeapons, m_fWeaponDrawPowerOld), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+89, offsetof(CPlayerWeapons, m_fWeaponDrawPower), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+90, offsetof(CPlayerWeapons, m_tmDrawStartTime), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+91, offsetof(CPlayerWeapons, m_tmFlamerStart), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+92, offsetof(CPlayerWeapons, m_tmFlamerStop), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+93, offsetof(CPlayerWeapons, m_tmLastChainsawSpray), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+101, offsetof(CPlayerWeapons, m_penMusicHolder), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_INDEX, NULL, (0x00000192<<8)+110, offsetof(CPlayerWeapons, m_iPlayerID), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+111, offsetof(CPlayerWeapons, m_bResetWeapon), "", 0, 0, EPROPF_NETSEND ),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+113, offsetof(CPlayerWeapons, m_vRayHitTmp), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+114, offsetof(CPlayerWeapons, m_penRayHitTmp), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+115, offsetof(CPlayerWeapons, m_fRayHitDistanceTmp), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+116, offsetof(CPlayerWeapons, m_vRayHitNow), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+117, offsetof(CPlayerWeapons, m_penRayHitNow), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+118, offsetof(CPlayerWeapons, m_fRayHitDistanceNow), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+119, offsetof(CPlayerWeapons, m_vRayHitReserve), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+120, offsetof(CPlayerWeapons, m_penRayHitReserve), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT, NULL, (0x00000192<<8)+121, offsetof(CPlayerWeapons, m_fRayHitDistanceReserve), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+123, offsetof(CPlayerWeapons, m_penRayHitClick), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_ENTITYPTR, NULL, (0x00000192<<8)+125, offsetof(CPlayerWeapons, m_penReservedTarget), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_FLOAT3D, NULL, (0x00000192<<8)+150, offsetof(CPlayerWeapons, m_vRayHitSurfaceNormal), "", 0, 0, 0),
 CEntityProperty(CEntityProperty::EPT_BOOL, NULL, (0x00000192<<8)+151, offsetof(CPlayerWeapons, m_bPickConditionOk), "", 0, 0, 0),
};
#define CPlayerWeapons_propertiesct ARRAYCOUNT(CPlayerWeapons_properties)

CEntityComponent CPlayerWeapons_components[] = {
#define CLASS_PROJECTILE ((0x00000192<<8)+1)
 CEntityComponent(ECT_CLASS, CLASS_PROJECTILE, 0, "EFNM" "Classes\\Projectile.ecl"),
#define CLASS_BULLET ((0x00000192<<8)+2)
 CEntityComponent(ECT_CLASS, CLASS_BULLET, 0, "EFNM" "Classes\\Bullet.ecl"),
#define CLASS_WEAPONEFFECT ((0x00000192<<8)+3)
 CEntityComponent(ECT_CLASS, CLASS_WEAPONEFFECT, 0, "EFNM" "Classes\\PlayerWeaponsEffects.ecl"),
#define CLASS_PIPEBOMB ((0x00000192<<8)+4)
 CEntityComponent(ECT_CLASS, CLASS_PIPEBOMB, 0, "EFNM" "Classes\\Pipebomb.ecl"),
#define CLASS_GHOSTBUSTERRAY ((0x00000192<<8)+5)
 CEntityComponent(ECT_CLASS, CLASS_GHOSTBUSTERRAY, 0, "EFNM" "Classes\\GhostBusterRay.ecl"),
#define CLASS_BASIC_EFFECT ((0x00000192<<8)+8)
 CEntityComponent(ECT_CLASS, CLASS_BASIC_EFFECT, 0, "EFNM" "Classes\\BasicEffect.ecl"),
#define SOUND_CS_FIRE ((0x00000192<<8)+155)
 CEntityComponent(ECT_SOUND, SOUND_CS_FIRE, 0, "EFNM" "data\\sounds\\Default.wav"),
#define SOUND_CS_BEGINFIRE ((0x00000192<<8)+156)
 CEntityComponent(ECT_SOUND, SOUND_CS_BEGINFIRE, 0, "EFNM" "data\\sounds\\Default.wav"),
#define SOUND_CS_ENDFIRE ((0x00000192<<8)+157)
 CEntityComponent(ECT_SOUND, SOUND_CS_ENDFIRE, 0, "EFNM" "data\\sounds\\Default.wav"),
#define SOUND_CS_BRINGUP ((0x00000192<<8)+158)
 CEntityComponent(ECT_SOUND, SOUND_CS_BRINGUP, 0, "EFNM" "data\\sounds\\Default.wav"),
#define SOUND_CS_IDLE ((0x00000192<<8)+159)
 CEntityComponent(ECT_SOUND, SOUND_CS_IDLE, 0, "EFNM" "data\\sounds\\Default.wav"),
#define SOUND_CS_BRINGDOWN ((0x00000192<<8)+162)
 CEntityComponent(ECT_SOUND, SOUND_CS_BRINGDOWN, 0, "EFNM" "data\\sounds\\Default.wav"),
};
#define CPlayerWeapons_componentsct ARRAYCOUNT(CPlayerWeapons_components)

CEventHandlerEntry CPlayerWeapons_handlers[] = {
 {0x0192000b, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 4323 "E:/USA/EntitiesMP/PlayerWeapons.es"
Fire),DEBUGSTRING("CPlayerWeapons::Fire")},
 {0x0192000c, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192000c_Fire_01), DEBUGSTRING("CPlayerWeapons::H0x0192000c_Fire_01")},
 {0x0192000d, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192000d_Fire_02), DEBUGSTRING("CPlayerWeapons::H0x0192000d_Fire_02")},
 {0x0192000e, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192000e_Fire_03), DEBUGSTRING("CPlayerWeapons::H0x0192000e_Fire_03")},
 {0x0192000f, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192000f_Fire_04), DEBUGSTRING("CPlayerWeapons::H0x0192000f_Fire_04")},
 {0x01920010, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 4424 "E:/USA/EntitiesMP/PlayerWeapons.es"
SwingKnife),DEBUGSTRING("CPlayerWeapons::SwingKnife")},
 {0x01920011, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920011_SwingKnife_01), DEBUGSTRING("CPlayerWeapons::H0x01920011_SwingKnife_01")},
 {0x01920012, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920012_SwingKnife_02), DEBUGSTRING("CPlayerWeapons::H0x01920012_SwingKnife_02")},
 {0x01920013, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920013_SwingKnife_03), DEBUGSTRING("CPlayerWeapons::H0x01920013_SwingKnife_03")},
 {0x01920014, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920014_SwingKnife_04), DEBUGSTRING("CPlayerWeapons::H0x01920014_SwingKnife_04")},
 {0x01920015, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920015_SwingKnife_05), DEBUGSTRING("CPlayerWeapons::H0x01920015_SwingKnife_05")},
 {0x01920016, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920016_SwingKnife_06), DEBUGSTRING("CPlayerWeapons::H0x01920016_SwingKnife_06")},
 {0x01920017, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920017_SwingKnife_07), DEBUGSTRING("CPlayerWeapons::H0x01920017_SwingKnife_07")},
 {0x01920018, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920018_SwingKnife_08), DEBUGSTRING("CPlayerWeapons::H0x01920018_SwingKnife_08")},
 {0x01920019, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920019_SwingKnife_09), DEBUGSTRING("CPlayerWeapons::H0x01920019_SwingKnife_09")},
 {0x0192001a, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192001a_SwingKnife_10), DEBUGSTRING("CPlayerWeapons::H0x0192001a_SwingKnife_10")},
 {0x0192001b, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192001b_SwingKnife_11), DEBUGSTRING("CPlayerWeapons::H0x0192001b_SwingKnife_11")},
 {0x0192001c, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192001c_SwingKnife_12), DEBUGSTRING("CPlayerWeapons::H0x0192001c_SwingKnife_12")},
 {0x0192001d, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192001d_SwingKnife_13), DEBUGSTRING("CPlayerWeapons::H0x0192001d_SwingKnife_13")},
 {0x0192001e, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 5552 "E:/USA/EntitiesMP/PlayerWeapons.es"
ChainsawFire),DEBUGSTRING("CPlayerWeapons::ChainsawFire")},
 {0x0192001f, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192001f_ChainsawFire_01), DEBUGSTRING("CPlayerWeapons::H0x0192001f_ChainsawFire_01")},
 {0x01920020, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920020_ChainsawFire_02), DEBUGSTRING("CPlayerWeapons::H0x01920020_ChainsawFire_02")},
 {0x01920021, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920021_ChainsawFire_03), DEBUGSTRING("CPlayerWeapons::H0x01920021_ChainsawFire_03")},
 {0x01920022, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920022_ChainsawFire_04), DEBUGSTRING("CPlayerWeapons::H0x01920022_ChainsawFire_04")},
 {0x01920023, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920023_ChainsawFire_05), DEBUGSTRING("CPlayerWeapons::H0x01920023_ChainsawFire_05")},
 {0x01920024, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920024_ChainsawFire_06), DEBUGSTRING("CPlayerWeapons::H0x01920024_ChainsawFire_06")},
 {0x01920025, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920025_ChainsawFire_07), DEBUGSTRING("CPlayerWeapons::H0x01920025_ChainsawFire_07")},
 {0x01920026, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920026_ChainsawFire_08), DEBUGSTRING("CPlayerWeapons::H0x01920026_ChainsawFire_08")},
 {0x01920027, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 5628 "E:/USA/EntitiesMP/PlayerWeapons.es"
ChainsawBringUp),DEBUGSTRING("CPlayerWeapons::ChainsawBringUp")},
 {0x01920028, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920028_ChainsawBringUp_01), DEBUGSTRING("CPlayerWeapons::H0x01920028_ChainsawBringUp_01")},
 {0x01920029, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920029_ChainsawBringUp_02), DEBUGSTRING("CPlayerWeapons::H0x01920029_ChainsawBringUp_02")},
 {0x0192002a, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 5806 "E:/USA/EntitiesMP/PlayerWeapons.es"
Reload),DEBUGSTRING("CPlayerWeapons::Reload")},
 {0x0192002b, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 5829 "E:/USA/EntitiesMP/PlayerWeapons.es"
ChangeKnifeStand),DEBUGSTRING("CPlayerWeapons::ChangeKnifeStand")},
 {0x0192002c, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 5885 "E:/USA/EntitiesMP/PlayerWeapons.es"
BoringWeaponAnimation),DEBUGSTRING("CPlayerWeapons::BoringWeaponAnimation")},
 {0x0192002d, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192002d_BoringWeaponAnimation_01), DEBUGSTRING("CPlayerWeapons::H0x0192002d_BoringWeaponAnimation_01")},
 {0x0192002e, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192002e_BoringWeaponAnimation_02), DEBUGSTRING("CPlayerWeapons::H0x0192002e_BoringWeaponAnimation_02")},
 {0x0192002f, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x0192002f_BoringWeaponAnimation_03), DEBUGSTRING("CPlayerWeapons::H0x0192002f_BoringWeaponAnimation_03")},
 {0x01920030, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 5915 "E:/USA/EntitiesMP/PlayerWeapons.es"
Idle),DEBUGSTRING("CPlayerWeapons::Idle")},
 {0x01920031, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920031_Idle_01), DEBUGSTRING("CPlayerWeapons::H0x01920031_Idle_01")},
 {0x01920032, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920032_Idle_02), DEBUGSTRING("CPlayerWeapons::H0x01920032_Idle_02")},
 {0x01920033, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 6009 "E:/USA/EntitiesMP/PlayerWeapons.es"
Stopped),DEBUGSTRING("CPlayerWeapons::Stopped")},
 {0x01920034, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920034_Stopped_01), DEBUGSTRING("CPlayerWeapons::H0x01920034_Stopped_01")},
 {0x01920035, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920035_Stopped_02), DEBUGSTRING("CPlayerWeapons::H0x01920035_Stopped_02")},
 {1, -1, CEntity::pEventHandler(&CPlayerWeapons::
#line 6043 "E:/USA/EntitiesMP/PlayerWeapons.es"
Main),DEBUGSTRING("CPlayerWeapons::Main")},
 {0x01920036, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920036_Main_01), DEBUGSTRING("CPlayerWeapons::H0x01920036_Main_01")},
 {0x01920037, -1, CEntity::pEventHandler(&CPlayerWeapons::H0x01920037_Main_02), DEBUGSTRING("CPlayerWeapons::H0x01920037_Main_02")},
};
#define CPlayerWeapons_handlersct ARRAYCOUNT(CPlayerWeapons_handlers)

CEntity *CPlayerWeapons_New(void) { return new CPlayerWeapons; };
void CPlayerWeapons_OnInitClass(void) {};
void CPlayerWeapons_OnEndClass(void) {};
void CPlayerWeapons_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) {};
void CPlayerWeapons_OnWorldEnd(CWorld *pwo) {};
void CPlayerWeapons_OnWorldInit(CWorld *pwo) {};
void CPlayerWeapons_OnWorldTick(CWorld *pwo) {};
void CPlayerWeapons_OnWorldRender(CWorld *pwo) {};
ENTITY_CLASSDEFINITION(CPlayerWeapons, CRationalEntity, "Player Weapons", "", 0x00000192);
DECLARE_CTFILENAME(_fnmCPlayerWeapons_tbn, "");