/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _HazeMarker_INCLUDED
#define _HazeMarker_INCLUDED 1
#include <EntitiesMP/Marker.h>
#include <EntitiesMP/FogMarker.h>
extern "C" DECL_DLL CDLLEntityClass CHazeMarker_DLLClass;
class CHazeMarker : public CMarker {
public:
virtual BOOL IsImportant(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	enum FogAttenuationType m_faType;
	FLOAT m_fDensity;
	FLOAT m_fNear;
	FLOAT m_fFar;
	BOOL m_bVisibleFromOutside;
	INDEX m_iSize;
	COLOR m_colBase;
	COLOR m_colUp;
	COLOR m_colDown;
	COLOR m_colNorth;
	COLOR m_colSouth;
	COLOR m_colEast;
	COLOR m_colWest;
   
#line 39 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/HazeMarker.es"
const CTString & GetHazeName(void);
   
#line 45 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/HazeMarker.es"
void GetHaze(class CHazeParameters & hpHaze,FLOAT3D & vViewDir);
#define  STATE_CHazeMarker_Main 1
	BOOL 
#line 101 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/HazeMarker.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _HazeMarker_INCLUDED
