/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _AreaMarker_INCLUDED
#define _AreaMarker_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CAreaMarker_DLLClass;
class CAreaMarker : public CEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	CTString m_strName;
	CTString m_strDescription;
	FLOATaabbox3D m_boxArea;
   
#line 22 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/AreaMarker.es"
void GetAreaBox(FLOATaabbox3D & box);
#define  STATE_CAreaMarker_Main 1
	BOOL 
#line 29 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/AreaMarker.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _AreaMarker_INCLUDED
