/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#ifndef _itemObjects_INCLUDED
#define _itemObjects_INCLUDED 1
#include <EntitiesMP/Item.h>
#include <EntitiesMP/BasicEffects.h>
extern DECL_DLL CEntityPropertyEnumType ItemObjType_enum;
enum ItemObjType {
	ITEMOBJ_DEFAULT = 0,
	ITEMOBJ_RAID = 1,
};
DECL_DLL inline void ClearToDefault(ItemObjType &e) { e = (ItemObjType)0; } ;
extern "C" DECL_DLL CDLLEntityClass CItemObjects_DLLClass;
class CItemObjects : public CItem {
public:
virtual const CTString &GetName(void) const { return m_strName; };
virtual BOOL IsTargetable(void) const { return TRUE; };
	DECL_DLL virtual void SetDefaultProperties(void);
	CTFileName m_fnModel;
	CTString m_strAnimation;
	BOOL m_bActive;
	FLOAT m_fStretch;
	INDEX m_iCollectItem;
	CSoundObject m_soDrop;
	INDEX idItem_NormalBox;
	enum ItemObjType eType;
	CTString m_strName;
   
#line 41 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
void Precache(void);
   
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
void RenderParticles(void);
   
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
void SetNormalBox(const char * BoxName);
#define  STATE_CItemObjects_Main 1
	BOOL 
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
Main(const CEntityEvent &__eeInput);
#define  STATE_CItemObjects_ItemCollected 0x05e20000
	BOOL 
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
ItemCollected(const CEntityEvent &__eeInput);
#define  STATE_CItemObjects_ItemActivate 0x05e20001
	BOOL 
#line 153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
ItemActivate(const CEntityEvent &__eeInput);
#define  STATE_CItemObjects_ItemDeActivate 0x05e20002
	BOOL 
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/itemObjects.es"
ItemDeActivate(const CEntityEvent &__eeInput);
};
#endif // _itemObjects_INCLUDED