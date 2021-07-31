/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _TimeController_INCLUDED
#define _TimeController_INCLUDED 1
extern "C" DECL_DLL CDLLEntityClass CTimeController_DLLClass;
class CTimeController : public CRationalEntity {
public:
virtual BOOL IsTargetable(void) const { return TRUE; };
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsImportant(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	FLOAT m_fTimeStretch;
	FLOAT m_tmFadeIn;
	FLOAT m_tmInterval;
	BOOL m_bAbsolute;
	FLOAT m_fMyTimer;
	FLOAT m_tmStretchChangeStart;
	CTString m_strName;
	FLOAT m_fOldTimeStretch;
	FLOAT m_fNewTimeStretch;
#define  STATE_CTimeController_Main 1
	BOOL 
#line 30 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TimeController.es"
Main(const CEntityEvent &__eeInput);
	BOOL H0x02650000_Main_01(const CEntityEvent &__eeInput);
	BOOL H0x02650001_Main_02(const CEntityEvent &__eeInput);
	BOOL H0x02650002_Main_03(const CEntityEvent &__eeInput);
	BOOL H0x02650003_Main_04(const CEntityEvent &__eeInput);
#define  STATE_CTimeController_ChangeTimeStretch 0x02650004
	BOOL 
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TimeController.es"
ChangeTimeStretch(const CEntityEvent &__eeInput);
	BOOL H0x02650005_ChangeTimeStretch_01(const CEntityEvent &__eeInput);
	BOOL H0x02650006_ChangeTimeStretch_02(const CEntityEvent &__eeInput);
	BOOL H0x02650007_ChangeTimeStretch_03(const CEntityEvent &__eeInput);
	BOOL H0x02650008_ChangeTimeStretch_04(const CEntityEvent &__eeInput);
#define  STATE_CTimeController_ApplyTimeStretch 0x02650009
	BOOL 
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TimeController.es"
ApplyTimeStretch(const CEntityEvent &__eeInput);
	BOOL H0x0265000a_ApplyTimeStretch_01(const CEntityEvent &__eeInput);
	BOOL H0x0265000b_ApplyTimeStretch_02(const CEntityEvent &__eeInput);
	BOOL H0x0265000c_ApplyTimeStretch_03(const CEntityEvent &__eeInput);
	BOOL H0x0265000d_ApplyTimeStretch_04(const CEntityEvent &__eeInput);
	BOOL H0x0265000e_ApplyTimeStretch_05(const CEntityEvent &__eeInput);
	BOOL H0x0265000f_ApplyTimeStretch_06(const CEntityEvent &__eeInput);
	BOOL H0x02650010_ApplyTimeStretch_07(const CEntityEvent &__eeInput);
#define  STATE_CTimeController_ResetTimeStretch 0x02650011
	BOOL 
#line 104 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/TimeController.es"
ResetTimeStretch(const CEntityEvent &__eeInput);
	BOOL H0x02650012_ResetTimeStretch_01(const CEntityEvent &__eeInput);
	BOOL H0x02650013_ResetTimeStretch_02(const CEntityEvent &__eeInput);
};
#endif // _TimeController_INCLUDED
