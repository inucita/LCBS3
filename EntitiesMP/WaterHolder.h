/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _WaterHolder_INCLUDED
#define _WaterHolder_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CWaterHolder_DLLClass;
class CWaterHolder : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	CTFileName m_fnModel;
	CTFileName m_fnmWaterTex;
	CTFileName m_fnmBumpTex;
	FLOAT m_fStretchAll;
	ANGLE3D m_vStretchXYZ;
	BOOL m_bColliding;
	BOOL m_bActive;
	BOOL m_bWireFrame;
	BOOL m_bReflection;
	BOOL m_bWave;
	FLOAT m_fBumpMat11;
	FLOAT m_fBumpMat12;
	FLOAT m_fBumpMat21;
	FLOAT m_fBumpMat22;
	FLOAT m_fTexFlowX;
	FLOAT m_fTexFlowY;
	COLOR m_colWater;
	CTString m_strName;
   
#line 38 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WaterHolder.es"
void Precache(void);
   
#line 59 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WaterHolder.es"
void InitNiceWater();
   
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WaterHolder.es"
void StretchModel(void);
#define  STATE_CWaterHolder_Main 1
	BOOL 
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WaterHolder.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _WaterHolder_INCLUDED
