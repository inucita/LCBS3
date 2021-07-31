/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"

#include "StdH.h"
#include "EntitiesMP/Effector.h"
#include "EntitiesMP/MovingBrush.h"

#include <EntitiesMP/DestroyableArchitecture.h>
#include <EntitiesMP/DestroyableArchitecture_tables.h>
CEntityEvent *EArchitectureDeath::MakeCopy(void) { CEntityEvent *peeCopy = new EArchitectureDeath(*this); return peeCopy;}
SLONG EArchitectureDeath::GetSizeOf(void) { return sizeof(*this);}
EArchitectureDeath::EArchitectureDeath() : CEntityEvent(EVENTCODE_EArchitectureDeath) {;
};
BOOL EArchitectureDeath::CheckIDs(void) { return TRUE ; }
#line 13 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"

struct DebrisInfo {
  ULONG ulModelID;
  ULONG ulTextureID;
  FLOAT vOffset[3];
};

static struct DebrisInfo _ObeliskDebrisInfo[] =
{
  { MODEL_OBELISK01, TEXTURE_OBELISK, 0.0f, 114.4989f, 0.0f},
  { MODEL_OBELISK02, TEXTURE_OBELISK, 0.035f, 106.8628f, 0.0f},
	{ MODEL_OBELISK03, TEXTURE_OBELISK, 0.0f, 98.628f, 0.0f},
	{ MODEL_OBELISK04, TEXTURE_OBELISK, 0.0f, 90.4996f, 0.0f},
	{ MODEL_OBELISK05, TEXTURE_OBELISK, 0.0f, 82.174f, 0.0f},
	{ MODEL_OBELISK06, TEXTURE_OBELISK, 0.0f, 71.0425f, 0.0f},
	{ MODEL_OBELISK07, TEXTURE_OBELISK, 0.0f, 59.2f, 0.0f},
	{ MODEL_OBELISK08, TEXTURE_OBELISK, 0.0f, 46.65f, 0.0f},
	{ MODEL_OBELISK09, TEXTURE_OBELISK, 0.0f, 36.6f, 0.0f},
};

static struct DebrisInfo _PylonDebrisInfo[] =
{
  { MODEL_PYLON01, TEXTURE_PYLON, -17.3379f, 55.92f, 0},
	{ MODEL_PYLON02, TEXTURE_PYLON, -10.525f, 58.045f, 0},
	{ MODEL_PYLON03, TEXTURE_PYLON, -17.66f, 42.32f, 0},
	{ MODEL_PYLON04, TEXTURE_PYLON, -0.815000f, 54.69f, 0	},
	{ MODEL_PYLON05, TEXTURE_PYLON, 14.795f, 51.65f, 0},
	{ MODEL_PYLON06, TEXTURE_PYLON, 0.02f, 36.18f, 0},
	{ MODEL_PYLON07, TEXTURE_PYLON, -10.289f, 33.982f, 0},
	{ MODEL_PYLON08, TEXTURE_PYLON, -22.9152f, 28.6205f, 0},
	{ MODEL_PYLON09, TEXTURE_PYLON, 21.932f, 47.2453f, 0},
};

void CDestroyableArchitecture::SetDefaultProperties(void) {
  m_strName = "DestroyableArchitecture";
  m_fHealth = -1.0f;
  m_etType = ET_DESTROY_OBELISK ;
  m_vDamageDir = FLOAT3D(0 , 0 , 0);
  m_fStretch = 1.0f;
  m_penGradient = NULL;
  m_colDebrises = C_WHITE ;
  m_ctDebrises = 12;
  m_fCandyEffect = 0.0f;
  m_fCubeFactor = 1.0f;
  m_bBlowupByDamager = FALSE ;
  m_fDustStretch = 0.0f;
  CMovableBrushEntity::SetDefaultProperties();
}
  
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
BOOL CDestroyableArchitecture::HandleEvent(const CEntityEvent & ee) 
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 107 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(ee  . ee_slEvent  == EVENTCODE_EArchitectureDeath ){
#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(! CMovableBrushEntity  :: HandleEvent  (ee )){
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CPrintF  (" Warning: architecture death event not handled!\n");
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetFlags  (GetFlags  () | ENF_HIDDEN );
#line 111 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 112 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 113 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return FALSE ;
#line 114 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 115 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return CMovableBrushEntity  :: HandleEvent  (ee );
#line 116 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 118 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
void CDestroyableArchitecture::Precache(void) 
#line 119 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 120 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheClass  (CLASS_DEBRIS );
#line 121 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheClass  (CLASS_EFFECTOR  , ET_DESTROY_OBELISK );
#line 122 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheClass  (CLASS_EFFECTOR  , ET_DESTROY_PYLON );
#line 123 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_STONE );
#line 124 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheTexture  (TEXTURE_STONE );
#line 126 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
switch(m_etType )
#line 127 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 128 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
case ET_DESTROY_OBELISK : 
#line 129 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheTexture  (TEXTURE_OBELISK );
#line 130 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK01 );
#line 131 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK02 );
#line 132 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK03 );
#line 133 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK04 );
#line 134 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK05 );
#line 135 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK06 );
#line 136 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK07 );
#line 137 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK08 );
#line 138 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_OBELISK09 );
#line 139 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
break ;
#line 140 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
case ET_DESTROY_PYLON : 
#line 141 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheTexture  (TEXTURE_PYLON );
#line 142 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON01 );
#line 143 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON02 );
#line 144 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON03 );
#line 145 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON04 );
#line 146 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON05 );
#line 147 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON06 );
#line 148 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON07 );
#line 149 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON08 );
#line 150 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
PrecacheModel  (MODEL_PYLON09 );
#line 151 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
break ;
#line 152 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 153 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 156 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
BOOL CDestroyableArchitecture::IsTargetValid(SLONG slPropertyOffset,CEntity * penTarget) 
#line 157 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 158 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(penTarget  == NULL )
#line 159 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 160 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return FALSE ;
#line 161 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 164 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(slPropertyOffset  == offsetof  (CDestroyableArchitecture  , m_penGradient ))
#line 165 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 166 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return (IsDerivedFromClass  (penTarget  , & CGradientMarker_DLLClass ));
#line 167 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 168 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return CEntity  :: IsTargetValid  (slPropertyOffset  , penTarget );
#line 169 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 172 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
const CTString & CDestroyableArchitecture::GetGradientName(INDEX iGradient) 
#line 173 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 174 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
static const CTString strDummyName  ("");
#line 175 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
static const CTString strMarkerUnused  ("Marker not set");
#line 176 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(iGradient  == 1)
#line 177 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 178 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CGradientMarker  * pgm  = (CGradientMarker  *) & * m_penGradient ;
#line 179 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(pgm  != NULL ){
#line 180 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return pgm  -> GetGradientName  ();
#line 181 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}else {
#line 182 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return strMarkerUnused ;
#line 183 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 184 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 185 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return strDummyName ;
#line 186 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 188 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
void CDestroyableArchitecture::UncacheShadowsForGradient(class CGradientMarker * penDiscard) 
#line 189 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 190 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CGradientMarker  * pgm  = (CGradientMarker  *) & * m_penGradient ;
#line 191 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(pgm  == penDiscard )
#line 192 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 193 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CEntity  :: UncacheShadowsForGradient  (1);
#line 194 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 195 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 198 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
BOOL CDestroyableArchitecture::GetGradient(INDEX iGradient,class CGradientParameters & fpGradient) 
#line 199 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 200 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(iGradient  == 1)
#line 201 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 202 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CGradientMarker  * pgm  = (CGradientMarker  *) & * m_penGradient ;
#line 203 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(pgm  != NULL ){
#line 204 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return pgm  -> GetGradient  (0 , fpGradient );
#line 205 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 206 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 207 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return FALSE ;
#line 208 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 211 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
void CDestroyableArchitecture::ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 212 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection) 
#line 213 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 215 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(m_fHealth  < 0){
#line 217 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return ;
#line 218 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 220 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(m_bBlowupByDamager )
#line 221 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 222 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(dmtType  == DMT_DAMAGER )
#line 223 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 224 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CMovableBrushEntity  :: ReceiveDamage  (penInflictor  , dmtType  , fDamageAmmount  , vHitPoint  , vDirection );
#line 225 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 226 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 227 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
else 
#line 228 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 230 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if((dmtType  == DMT_EXPLOSION ) || 
#line 231 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
(dmtType  == DMT_PROJECTILE )
#line 233 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
)
#line 234 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 235 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CMovableBrushEntity  :: ReceiveDamage  (penInflictor  , dmtType  , fDamageAmmount  , vHitPoint  , vDirection );
#line 236 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 237 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 238 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 240 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
void CDestroyableArchitecture::DestroyObelisk() 
#line 241 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 242 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
for(INDEX iDebris  = 0;iDebris  < ARRAYCOUNT  (_ObeliskDebrisInfo );iDebris  ++)
#line 243 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 244 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
DebrisInfo  & di  = _ObeliskDebrisInfo  [ iDebris  ];
#line 245 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vOffset  = FLOAT3D (di  . vOffset  [ 0 ] , di  . vOffset  [ 1 ] , di  . vOffset  [ 2 ]) * m_fStretch ;
#line 246 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vPos  = GetPlacement  () . pl_PositionVector  + vOffset ;
#line 247 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CEntityPointer penDebris  = GetWorld  () -> CreateEntity_t  (
#line 248 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CPlacement3D (vPos  , ANGLE3D (0 , 0 , 0)) , CTFILENAME  ("Classes\\Debris.ecl") , WLD_AUTO_ENTITY_ID  , FALSE );
#line 250 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ESpawnDebris  eSpawn ;
#line 251 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . fDustStretch  = m_fDustStretch ;
#line 252 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . bCustomShading  = FALSE ;
#line 253 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . bImmaterialASAP  = FALSE ;
#line 254 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . colDebris  = C_WHITE  | CT_OPAQUE ;
#line 255 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . Eeibt  = EIBT_ROCK ;
#line 256 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . dptParticles  = DPT_NONE ;
#line 257 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . betStain  = BET_NONE ;
#line 258 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . pmd  = GetModelDataForComponent  (di  . ulModelID );
#line 259 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptd  = GetTextureDataForComponent  (di  . ulTextureID );
#line 260 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptdRefl  = NULL ;
#line 261 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptdSpec  = NULL ;
#line 262 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptdBump  = NULL ;
#line 263 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . iModelAnim  = 0;
#line 264 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . fSize  = m_fStretch ;
#line 265 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . vStretch  = FLOAT3D (1 , 1 , 1);
#line 266 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . penFallFXPapa  = NULL ;
#line 268 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
penDebris  -> Initialize  (eSpawn  , FALSE );
#line 271 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT fHeightRatio  = di  . vOffset  [ 1 ] * m_fStretch  / 120.0f;
#line 272 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vSpeed  = FLOAT3D (FRnd  () - 0.5f , 0.0f , FRnd  () - 0.5f) * (1.0f - fHeightRatio ) * 160.0f;
#line 273 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vRot  = FLOAT3D (FRnd  () - 0.5f , (FRnd  () - 0.5f) * (1.0f - fHeightRatio ) , FRnd  () - 0.5f) * 200.0f;
#line 277 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
((CMovableEntity  &) * penDebris ) . LaunchAsFreeProjectile  (vSpeed  , NULL );
#line 278 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
((CMovableEntity  &) * penDebris ) . SetDesiredRotation  (vRot );
#line 279 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 282 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FOREACHINLIST  (CEntity  , en_lnInParent  , en_lhChildren  , iten ){
#line 283 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
iten  -> SendEvent  (EBrushDestroyed  ());
#line 284 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 285 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
m_fHealth  = - 1;
#line 286 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ForceFullStop  ();
#line 287 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetDefaultProperties  ();
#line 289 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CPlacement3D plObelisk  = GetPlacement  ();
#line 293 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
NotifyCollisionChanged  ();
#line 294 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetFlags  (GetFlags  () | ENF_HIDDEN );
#line 295 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 298 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CEntity  * penEffector  = CreateEntity  (plObelisk  , CLASS_EFFECTOR  , WLD_AUTO_ENTITY_ID  , FALSE );
#line 300 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ESpawnEffector  eSpawnEffector ;
#line 301 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawnEffector  . tmLifeTime  = 6.0f;
#line 302 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawnEffector  . eetType  = ET_DESTROY_OBELISK ;
#line 304 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
penEffector  -> Initialize  (eSpawnEffector  , FALSE );
#line 305 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 307 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
void CDestroyableArchitecture::DestroyPylon() 
#line 308 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 309 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
for(INDEX iDebris  = 0;iDebris  < ARRAYCOUNT  (_PylonDebrisInfo );iDebris  ++)
#line 310 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 311 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
DebrisInfo  & di  = _PylonDebrisInfo  [ iDebris  ];
#line 312 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vOffset  = FLOAT3D (di  . vOffset  [ 0 ] , di  . vOffset  [ 1 ] , di  . vOffset  [ 2 ]) * m_fStretch ;
#line 313 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vPos  = GetPlacement  () . pl_PositionVector  + vOffset ;
#line 314 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CEntityPointer penDebris  = GetWorld  () -> CreateEntity_t  (
#line 315 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CPlacement3D (vPos  , ANGLE3D (0 , 0 , 0)) , CTFILENAME  ("Classes\\Debris.ecl") , WLD_AUTO_ENTITY_ID  , FALSE );
#line 317 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ESpawnDebris  eSpawn ;
#line 318 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . fDustStretch  = m_fDustStretch ;
#line 319 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . bImmaterialASAP  = FALSE ;
#line 320 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . bCustomShading  = FALSE ;
#line 321 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . colDebris  = C_WHITE  | CT_OPAQUE ;
#line 322 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . Eeibt  = EIBT_ROCK ;
#line 323 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . dptParticles  = DPT_NONE ;
#line 324 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . betStain  = BET_NONE ;
#line 325 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . pmd  = GetModelDataForComponent  (di  . ulModelID );
#line 326 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptd  = GetTextureDataForComponent  (di  . ulTextureID );
#line 327 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptdRefl  = NULL ;
#line 328 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptdSpec  = NULL ;
#line 329 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . ptdBump  = NULL ;
#line 330 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . iModelAnim  = 0;
#line 331 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . fSize  = m_fStretch ;
#line 332 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . vStretch  = FLOAT3D (1 , 1 , 1);
#line 333 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawn  . penFallFXPapa  = NULL ;
#line 335 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
penDebris  -> Initialize  (eSpawn  , FALSE );
#line 338 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT fHeightRatio  = di  . vOffset  [ 1 ] * m_fStretch  / 120.0f;
#line 339 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vSpeed  = (m_vDamageDir  * 2.0f + FLOAT3D (FRnd  () - 0.5f , 0.0f , FRnd  ())) * fHeightRatio  * 160.0f;
#line 340 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D vRot  = FLOAT3D (FRnd  () - 0.5f , (FRnd  () - 0.5f) * fHeightRatio  , FRnd  () - 0.5f) * 300.0f;
#line 341 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
((CMovableEntity  &) * penDebris ) . LaunchAsFreeProjectile  (vSpeed  , NULL );
#line 342 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
((CMovableEntity  &) * penDebris ) . SetDesiredRotation  (vRot );
#line 343 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 346 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FOREACHINLIST  (CEntity  , en_lnInParent  , en_lhChildren  , iten ){
#line 347 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
iten  -> SendEvent  (EBrushDestroyed  ());
#line 348 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 349 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
m_fHealth  = - 1;
#line 350 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CPlacement3D plObelisk  = GetPlacement  ();
#line 352 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
CEntity  * penEffector  = CreateEntity  (plObelisk  , CLASS_EFFECTOR  , WLD_AUTO_ENTITY_ID  , FALSE );
#line 354 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ESpawnEffector  eSpawnEffector ;
#line 355 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawnEffector  . eetType  = ET_DESTROY_PYLON ;
#line 356 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawnEffector  . tmLifeTime  = 6.0f;
#line 357 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
eSpawnEffector  . vDamageDir  = m_vDamageDir ;
#line 359 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
penEffector  -> Initialize  (eSpawnEffector  , FALSE );
#line 361 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ForceFullStop  ();
#line 362 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetDefaultProperties  ();
#line 365 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
NotifyCollisionChanged  ();
#line 366 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetFlags  (GetFlags  () | ENF_HIDDEN );
#line 367 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 368 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
  
#line 372 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SLONG CDestroyableArchitecture::GetUsedMemory(void) 
#line 373 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 375 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SLONG  slUsedMemory  = sizeof  (CDestroyableArchitecture ) - sizeof  (CMovableBrushEntity ) + CMovableBrushEntity  :: GetUsedMemory  ();
#line 377 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
slUsedMemory  += m_strName  . Length  ();
#line 378 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return slUsedMemory ;
#line 379 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
BOOL CDestroyableArchitecture::
#line 386 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CDestroyableArchitecture_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CDestroyableArchitecture::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 388 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
InitAsBrush  ();
#line 389 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetPhysicsFlags  (EPF_BRUSH_MOVING );
#line 390 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetCollisionFlags  (ECF_BRUSH );
#line 392 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetFlags  (GetFlags  () & ~ ENF_ZONING );
#line 393 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetHealth  (m_fHealth );
#line 396 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x006b0001, FALSE, EBegin());return TRUE;}BOOL CDestroyableArchitecture::H0x006b0001_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x006b0001
switch(__eeInput.ee_slEvent){case(EVENTCODE_EBegin):{const EBegin&e= (EBegin&)__eeInput;

#line 398 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return TRUE;
#line 399 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeath):{const EDeath&e= (EDeath&)__eeInput;

#line 401 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(_pNetwork  -> IsServer  ()){
#line 402 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
EArchitectureDeath  ead ;
#line 403 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SendEvent  (ead  , TRUE );
#line 404 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 405 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return TRUE;
#line 406 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}ASSERT(FALSE);break;case(EVENTCODE_EArchitectureDeath):{const EArchitectureDeath&e= (EArchitectureDeath&)__eeInput;

#line 409 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOATaabbox3D box ;
#line 410 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
GetSize  (box );
#line 411 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
if(m_ctDebrises  <= 0)
#line 412 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{
#line 413 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
m_ctDebrises  = 1;
#line 414 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 415 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT fEntitySize  = pow  (box  . Size  () (1) * box  . Size  () (2) * box  . Size  () (3) / m_ctDebrises  , 1.0f / 3.0f) * m_fCubeFactor ;
#line 417 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
Debris_Begin  (EIBT_ROCK  , DPT_NONE  , BET_NONE  , fEntitySize  , FLOAT3D (1.0f , 2.0f , 3.0f) , 
#line 418 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D (0 , 0 , 0) , 1.0f + m_fCandyEffect  / 2.0f , m_fCandyEffect  , m_colDebrises );
#line 419 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
for(INDEX iDebris  = 0;iDebris  < m_ctDebrises ;iDebris  ++){
#line 420 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
Debris_Spawn  (this  , this  , MODEL_STONE  , TEXTURE_STONE  , 0 , 0 , 0 , IRnd  () % 4 , 1.0f , 
#line 421 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FLOAT3D (FRnd  () * 0.8f + 0.1f , FRnd  () * 0.8f + 0.1f , FRnd  () * 0.8f + 0.1f));
#line 422 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 425 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
FOREACHINLIST  (CEntity  , en_lnInParent  , en_lhChildren  , iten ){
#line 426 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
iten  -> SendEvent  (EBrushDestroyed  ());
#line 427 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}
#line 429 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
m_fHealth  = - 1;
#line 430 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
ForceFullStop  ();
#line 431 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetDefaultProperties  ();
#line 434 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
NotifyCollisionChanged  ();
#line 435 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetFlags  (GetFlags  () | ENF_HIDDEN );
#line 436 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 439 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{FOREACHINLIST  (CEntity  , en_lnInParent  , en_lhChildren  , itenChild ){
#line 441 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
itenChild  -> SendEvent  (ERangeModelDestruction  ());
#line 442 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}}
#line 444 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
UnsetTimer();Jump(STATE_CURRENT,0x006b0002, FALSE, EInternal());return TRUE;
#line 445 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}ASSERT(FALSE);break;case(EVENTCODE_EReturn):
#line 447 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
{const EReturn&e= (EReturn&)__eeInput;

#line 448 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
UnsetTimer();Jump(STATE_CURRENT,0x006b0002, FALSE, EInternal());return TRUE;
#line 449 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 450 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
}return TRUE;}BOOL CDestroyableArchitecture::H0x006b0002_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x006b0002

#line 451 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
Return(STATE_CURRENT,EVoid());
#line 451 "E:/질산/Korea/Client_USA/EntitiesMP/DestroyableArchitecture.es"
return TRUE; ASSERT(FALSE); return TRUE;};