/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _PlayerActionMarker_INCLUDED
#define _PlayerActionMarker_INCLUDED 1
#include <EntitiesMP/Marker.h>
#include <EntitiesMP/Player.h>
extern DECL_DLL CEntityPropertyEnumType PlayerAutoAction_enum;
enum PlayerAutoAction {
	PAA_RUN = 1,
	PAA_WAIT = 2,
	PAA_USEITEM = 3,
	PAA_STOREWEAPON = 4,
	PAA_DRAWWEAPON = 5,
	PAA_LOOKAROUND = 6,
	PAA_RUNANDSTOP = 7,
	PAA_RECORDSTATS = 8,
	PAA_ENDOFGAME = 9,
	PAA_SHOWSTATS = 10,
	PAA_APPEARING = 11,
	PAA_WAITFOREVER = 12,
	PAA_TELEPORT = 13,
	PAA_PICKITEM = 14,
	PAA_FALLDOWN = 15,
	PAA_FALLTOABYSS = 16,
	PAA_RELEASEPLAYER = 17,
	PAA_STARTCOMPUTER = 18,
	PAA_TRAVELING_IN_BEAM = 19,
	PAA_LOGO_FIRE_MINIGUN = 20,
	PAA_STARTCREDITS = 21,
	PAA_STARTINTROSCROLL = 22,
	PAA_STOPSCROLLER = 23,
	PAA_NOGRAVITY = 24,
	PAA_TURNONGRAVITY = 25,
	PAA_LOGO_FIRE_INTROSE = 26,
	PAA_INTROSE_SELECT_WEAPON = 27,
	PAA_STOPANDWAIT = 28,
};
DECL_DLL inline void ClearToDefault(PlayerAutoAction &e) { e = (PlayerAutoAction)0; } ;
extern "C" DECL_DLL CDLLEntityClass CPlayerActionMarker_DLLClass;
class CPlayerActionMarker : public CMarker {
public:
	DECL_DLL virtual void SetDefaultProperties(void);
	enum PlayerAutoAction m_paaAction;
	FLOAT m_tmWait;
	CEntityPointer m_penDoorController;
	CEntityPointer m_penTrigger;
	FLOAT m_fSpeed;
	CEntityPointer m_penItem;
   
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerActionMarker.es"
const CTString & GetDescription(void)const;
   
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerActionMarker.es"
BOOL DropsMarker(CTFileName & fnmMarkerClass,CTString & strTargetProperty)const;
   
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerActionMarker.es"
BOOL HandleEvent(const CEntityEvent & ee);
#define  STATE_CPlayerActionMarker_Main 1
	BOOL 
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/PlayerActionMarker.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _PlayerActionMarker_INCLUDED
