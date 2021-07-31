/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _WorldSettingsController_INCLUDED
#define _WorldSettingsController_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CWorldSettingsController_DLLClass;
class CWorldSettingsController : public CEntity {
public:
virtual BOOL IsTargetable(void) const { return TRUE; };
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsImportant(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	FLOAT m_tmStormStart;
	CTString m_strName;
	FLOAT m_tmLightningStart;
	FLOAT m_fLightningPower;
	FLOAT m_tmStormEnd;
	FLOAT m_tmPyramidPlatesStart;
	FLOAT m_tmActivatedPlate1;
	FLOAT m_tmDeactivatedPlate1;
	FLOAT m_tmActivatedPlate2;
	FLOAT m_tmDeactivatedPlate2;
	FLOAT m_tmActivatedPlate3;
	FLOAT m_tmDeactivatedPlate3;
	FLOAT m_tmActivatedPlate4;
	FLOAT m_tmDeactivatedPlate4;
	FLOAT m_tmPyramidMorphRoomActivated;
	FLOAT m_tmShakeStarted;
	FLOAT3D m_vShakePos;
	FLOAT m_fShakeFalloff;
	FLOAT m_fShakeFade;
	FLOAT m_fShakeIntensityY;
	FLOAT m_tmShakeFrequencyY;
	FLOAT m_fShakeIntensityB;
	FLOAT m_tmShakeFrequencyB;
	FLOAT m_fShakeIntensityZ;
	FLOAT m_tmShakeFrequencyZ;
	BOOL m_bShakeFadeIn;
	CEntityPointer m_penEnvPartHolder;
	BOOL m_bNoSaveGame;
	FLOAT m_tmGlaringStarted;
	FLOAT m_tmGlaringEnded;
	FLOAT m_fGlaringFadeInRatio;
	FLOAT m_fGlaringFadeOutRatio;
	COLOR m_colGlade;
	COLOR m_colBlendStart;
	COLOR m_colBlendStop;
	COLOR m_colShadeStart;
	COLOR m_colShadeStop;
	BOOL m_bApplyShadingToModels;
	FLOAT m_tmStormAppearTime;
	FLOAT m_tmStormDisappearTime;
	CEntityPointer m_penHudPicFXHolder;
   
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WorldSettingsController.es"
BOOL IsTargetValid(SLONG slPropertyOffset,CEntity * penTarget);
   
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WorldSettingsController.es"
FLOAT GetStormFactor(void);
   
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WorldSettingsController.es"
BOOL HandleEvent(const CEntityEvent & ee);
#define  STATE_CWorldSettingsController_Main 1
	BOOL 
#line 185 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/WorldSettingsController.es"
Main(const CEntityEvent &__eeInput);
};
#endif // _WorldSettingsController_INCLUDED
