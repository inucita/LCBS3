/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _SoundHolder_INCLUDED
#define _SoundHolder_INCLUDED 1
#include <EntitiesMP/ModelDestruction.h>
#define EVENTCODE_EPlaySoundOnce 0x00cc0000
class DECL_DLL EPlaySoundOnce : public CEntityEvent {
public:
	EPlaySoundOnce();
	CEntityEvent *MakeCopy(void);
	BOOL CheckIDs(void);
	SLONG GetSizeOf(void);
};
DECL_DLL inline void ClearToDefault(EPlaySoundOnce &e) { e = EPlaySoundOnce(); } ;
extern "C" DECL_DLL CDLLEntityClass CSoundHolder_DLLClass;
class CSoundHolder : public CRationalEntity {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual const CTString &GetDescription(void) const { return m_strDescription; };
virtual BOOL IsTargetable(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	CTFileName m_fnSound;
	RANGE m_rFallOffRange;
	RANGE m_rHotSpotRange;
	FLOAT m_fVolume;
	FLOAT m_fPitch;
	BOOL m_bLoop;
	BOOL m_bVolumetric;
	BOOL m_bMusic;
	CTString m_strName;
	CTString m_strDescription;
	BOOL m_bAutoStart;
	INDEX m_iPlayType;
	CSoundObject m_soSound;
	BOOL m_bDestroyable;
CAutoPrecacheSound m_aps;
   
#line 51 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SoundHolder.es"
void Precache(void);
   
#line 57 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SoundHolder.es"
void MirrorAndStretch(FLOAT fStretch,BOOL bMirrorX);
   
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SoundHolder.es"
SLONG GetUsedMemory(void);
#define  STATE_CSoundHolder_Main 1
	BOOL 
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SoundHolder.es"
Main(const CEntityEvent &__eeInput);
	BOOL H0x00cc0001_Main_01(const CEntityEvent &__eeInput);
	BOOL H0x00cc0002_Main_02(const CEntityEvent &__eeInput);
	BOOL H0x00cc0003_Main_03(const CEntityEvent &__eeInput);
	BOOL H0x00cc0004_Main_04(const CEntityEvent &__eeInput);
	BOOL H0x00cc0005_Main_05(const CEntityEvent &__eeInput);
#define  STATE_CSoundHolder_PlaySoundOnce 0x00cc0006
	BOOL 
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/SoundHolder.es"
PlaySoundOnce(const CEntityEvent &__eeInput);
	BOOL H0x00cc0007_PlaySoundOnce_01(const CEntityEvent &__eeInput);
	BOOL H0x00cc0008_PlaySoundOnce_02(const CEntityEvent &__eeInput);
};
#endif // _SoundHolder_INCLUDED
