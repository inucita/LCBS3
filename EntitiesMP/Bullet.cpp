/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"

#include "StdH.h"
#include "EntitiesMP/Player.h"
//#include "EntitiesMP/Gizmo.h"
//#include "EntitiesMP/Beast.h"

#include <Bullet.h>
#include <Bullet_tables.h>
CEntityEvent *EBulletInit::MakeCopy(void) { 	CEntityEvent *peeCopy = new EBulletInit(*this); 	return peeCopy;}
SLONG EBulletInit::GetSizeOf(void) { 	return sizeof(*this);}
EBulletInit::EBulletInit() : CEntityEvent(EVENTCODE_EBulletInit) {
	ClearToDefault(eidOwner);
	ClearToDefault(fDamage);
}
BOOL EBulletInit::CheckIDs(void) {	return TRUE && eidOwner.IsValid(); }

#line 20 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"

void CBullet_OnPrecache(CDLLEntityClass *pdec, INDEX iUser) 
{
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINSTONE);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINSAND);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINREDSAND);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINWATER);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINSTONENOSOUND);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINSANDNOSOUND);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINREDSANDNOSOUND);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETSTAINWATERNOSOUND);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BLOODSPILL);
  pdec->PrecacheClass(CLASS_BASIC_EFFECT, BET_BULLETTRAIL);
}

void CBullet::SetDefaultProperties(void) {
	m_penOwner = NULL;
	m_fDamage = 0.0f;
	m_vTarget = FLOAT3D(0 , 0 , 0);
	m_vTargetCopy = FLOAT3D(0 , 0 , 0);
	m_vHitPoint = FLOAT3D(0 , 0 , 0);
	m_iBullet = 0;
	m_EdtDamage = DMT_BULLET ;
	m_fBulletSize = 0.0f;
	CEntity::SetDefaultProperties();
}

#line 60 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::SetDamage(FLOAT fDamage) {
#line 61 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_fDamage  = fDamage ;
#line 62 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 65 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::CalcTarget(FLOAT fRange) {
#line 67 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
AnglesToDirectionVector  (GetPlacement  () . pl_OrientationAngle  , m_vTarget );
#line 68 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  *= fRange ;
#line 69 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  += GetPlacement  () . pl_PositionVector ;
#line 70 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTargetCopy  = m_vTarget ;
#line 71 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 73 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::CalcTarget(CEntity * pen,FLOAT fRange) 
#line 74 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 76 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(! pen ){
#line 77 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CalcTarget  (fRange );
#line 78 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
return ;
#line 79 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 82 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
EntityInfo  * peiTarget  = (EntityInfo  *) (pen  -> GetEntityInfo  ());
#line 83 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(IsDerivedFromClass  (pen  , & CEnemyBase_DLLClass )){
#line 84 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CEnemyBase  * penEnemy  = (CEnemyBase  *) pen ;
#line 85 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ScaleEntityInfo  (peiTarget  , penEnemy  -> m_fStretchMultiplier );
#line 86 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 87 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vTarget ;
#line 88 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
GetEntityInfoPosition  (pen  , peiTarget  -> vTargetCenter  , vTarget );
#line 91 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  = (vTarget  - GetPlacement  () . pl_PositionVector ) . Normalize  ();
#line 92 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  *= fRange ;
#line 93 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  += GetPlacement  () . pl_PositionVector ;
#line 94 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTargetCopy  = m_vTarget ;
#line 95 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 98 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::CalcJitterTarget(FLOAT fR) {
#line 99 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vJitter ;
#line 110 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fZ  = FRnd  () * 2.0f - 1.0f;
#line 111 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fA  = FRnd  () * 360.0f;
#line 112 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fT  = Sqrt  (1 - (fZ  * fZ ));
#line 113 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vJitter  (1) = fT  * CosFast  (fA );
#line 114 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vJitter  (2) = fT  * SinFast  (fA );
#line 115 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vJitter  (3) = fZ ;
#line 116 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vJitter  = vJitter  * fR  * FRnd  ();
#line 119 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  = m_vTargetCopy  + vJitter ;
#line 120 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 123 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::CalcJitterTargetFixed(FLOAT fX,FLOAT fY,FLOAT fJitter) {
#line 124 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fRndX  = FRnd  () * 2.0f - 1.0f;
#line 125 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fRndY  = FRnd  () * 2.0f - 1.0f;
#line 126 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vX  , vY ;
#line 127 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
const FLOATmatrix3D & m  = GetRotationMatrix  ();
#line 128 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vX  (1) = m  (1 , 1);vX  (2) = m  (2 , 1);vX  (3) = m  (3 , 1);
#line 129 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vY  (1) = m  (1 , 2);vY  (2) = m  (2 , 2);vY  (3) = m  (3 , 2);
#line 131 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vTarget  = m_vTargetCopy  + (vX  * (fX  + fRndX  * fJitter )) + (vY  * (fY  + fRndY  * fJitter ));
#line 132 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 135 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::LaunchBullet(BOOL bSound,BOOL bTrail,BOOL bHitFX) 
#line 136 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 138 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CCastRay  crRay  (m_penOwner  , GetPlacement  () . pl_PositionVector  , m_vTarget );
#line 139 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_bHitPortals  = TRUE ;
#line 140 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_bHitTranslucentPortals  = TRUE ;
#line 141 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_ttHitModels  = CCastRay  :: TT_COLLISIONBOX ;
#line 142 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_bPhysical  = FALSE ;
#line 143 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_fTestR  = m_fBulletSize ;
#line 144 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vHitDirection ;
#line 145 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
AnglesToDirectionVector  (GetPlacement  () . pl_OrientationAngle  , vHitDirection );
#line 147 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
INDEX ctCasts  = 0;
#line 148 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
while(ctCasts  < 10)
#line 149 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 150 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(ctCasts  == 0)
#line 151 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 153 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
GetWorld  () -> CastRay  (crRay );
#line 154 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 155 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
else 
#line 156 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 158 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
GetWorld  () -> ContinueCast  (crRay );
#line 159 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 160 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ctCasts  ++;
#line 163 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(crRay  . cr_penHit  == NULL )
#line 164 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 165 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
break ;
#line 166 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 168 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
const FLOAT fDamageMul  = GetSeriousDamageMultiplier  (m_penOwner );
#line 169 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
InflictDirectDamage  (crRay  . cr_penHit  , m_penOwner  , m_EdtDamage  , m_fDamage  * fDamageMul  , 
#line 170 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_vHit  , vHitDirection );
#line 172 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_vHitPoint  = crRay  . cr_vHit ;
#line 175 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(crRay  . cr_penHit  -> GetRenderType  () == RT_BRUSH  && crRay  . cr_pbpoBrushPolygon  != NULL )
#line 176 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 177 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CBrushPolygon  * pbpo  = crRay  . cr_pbpoBrushPolygon ;
#line 178 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vHitNormal  = FLOAT3D (pbpo  -> bpo_pbplPlane  -> bpl_plAbsolute );
#line 180 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
INDEX iSurfaceType  = pbpo  -> bpo_bppProperties  . bpp_ubSurfaceType ;
#line 181 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
BulletHitType  bhtType  = BHT_BRUSH_STONE ;
#line 183 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
INDEX iContent  = pbpo  -> bpo_pbscSector  -> GetContentType  ();
#line 184 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CContentType  & ct  = GetWorld  () -> wo_actContentTypes  [ iContent  ];
#line 186 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
bhtType  = (BulletHitType ) GetBulletHitTypeForSurface  (iSurfaceType );
#line 188 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(ct  . ct_ulFlags  & CTF_BREATHABLE_GILLS )
#line 189 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 191 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(iSurfaceType  == SURFACE_WATER )
#line 192 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 193 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vHitNormal  = - vHitNormal ;
#line 195 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
bhtType  = BHT_BRUSH_WATER ;
#line 196 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 198 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
else 
#line 199 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 200 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
bhtType  = BHT_BRUSH_UNDER_WATER ;
#line 201 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 202 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 206 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
BOOL bPassable  = pbpo  -> bpo_ulFlags  & (BPOF_PASSABLE );
#line 208 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(! bPassable  || iSurfaceType  == SURFACE_WATER ){
#line 209 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
SpawnHitTypeEffect  (this  , bhtType  , bSound  , vHitNormal  , crRay  . cr_vHit  , vHitDirection  , FLOAT3D (0.0f , 0.0f , 0.0f));
#line 210 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 211 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(! bPassable ){
#line 212 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
break ;
#line 213 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 215 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}else {
#line 218 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
const BOOL bCoop  = ! GetSP  () -> sp_bSinglePlayer  && GetSP  () -> sp_bCooperative ;
#line 219 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(! bCoop  && crRay  . cr_penHit  -> GetEntityInfo  () != NULL  && 
#line 220 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
((EntityInfo  *) crRay  . cr_penHit  -> GetEntityInfo  ()) -> Eeibt  == EIBT_FLESH )
#line 221 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 222 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CEntity  * penOfFlesh  = crRay  . cr_penHit ;
#line 223 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vHitNormal  = (GetPlacement  () . pl_PositionVector  - m_vTarget ) . Normalize  ();
#line 224 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vOldHitPos  = crRay  . cr_vHit ;
#line 225 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vDistance ;
#line 228 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
GetWorld  () -> ContinueCast  (crRay );
#line 229 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(crRay  . cr_penHit  != NULL  && crRay  . cr_pbpoBrushPolygon  != NULL  && 
#line 230 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
crRay  . cr_penHit  -> GetRenderType  () == RT_BRUSH )
#line 231 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 232 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vDistance  = crRay  . cr_vHit  - vOldHitPos ;
#line 233 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vHitNormal  = FLOAT3D (crRay  . cr_pbpoBrushPolygon  -> bpo_pbplPlane  -> bpl_plAbsolute );
#line 234 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 235 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
else 
#line 236 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 237 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vDistance  = FLOAT3D (0.0f , 0.0f , 0.0f);
#line 238 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vHitNormal  = FLOAT3D (0 , 0 , 0);
#line 239 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 252 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
SpawnHitTypeEffect  (this  , BHT_FLESH  , bSound  , vHitNormal  , crRay  . cr_vHit  , vHitDirection  , vDistance );
#line 254 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
break ;
#line 255 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 257 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
break ;
#line 258 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 259 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 261 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(bTrail )
#line 262 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 263 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
SpawnTrail  ();
#line 264 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 265 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 268 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::DestroyBullet(void) {
#line 269 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
Destroy  ();
#line 270 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}

#line 278 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
void CBullet::SpawnTrail(void) 
#line 279 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
{
#line 281 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
const FLOAT3D & v0  = GetPlacement  () . pl_PositionVector ;
#line 282 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
const FLOAT3D & v1  = m_vHitPoint ;
#line 284 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vD  = v1  - v0 ;
#line 285 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fD  = vD  . Length  ();
#line 287 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(fD  < 1.0f){
#line 289 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
return ;
#line 290 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 293 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT fLen  = Min  (20.0f , fD );
#line 295 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vPos ;
#line 296 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
if(fLen  < fD ){
#line 297 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vPos  = Lerp  (v0  , v1  , FRnd  () * (fD  - fLen ) / fD );
#line 298 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}else {
#line 299 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vPos  = v0 ;
#line 300 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
#line 302 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ESpawnEffect  ese ;
#line 303 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
UBYTE  ubRndH  = UBYTE  (8 + FRnd  () * 32);
#line 304 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
UBYTE  ubRndS  = UBYTE  (8 + FRnd  () * 32);
#line 305 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
UBYTE  ubRndV  = UBYTE  (224 + FRnd  () * 32);
#line 306 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
UBYTE  ubRndA  = UBYTE  (32 + FRnd  () * 128);
#line 307 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ese  . colMuliplier  = HSVToColor  (ubRndH  , ubRndS  , ubRndV ) | ubRndA ;
#line 308 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ese  . betType  = BET_BULLETTRAIL ;
#line 309 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ese  . vNormal  = vD  / fD ;
#line 310 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ese  . vStretch  = FLOAT3D (0.1f , fLen  , 1.0f);
#line 313 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
FLOAT3D vBulletIncommingDirection ;
#line 314 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
vBulletIncommingDirection  = (m_vTarget  - GetPlacement  () . pl_PositionVector ) . Normalize  ();
#line 315 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CPlacement3D plHit  = CPlacement3D (vPos  - vBulletIncommingDirection  * 0.1f , GetPlacement  () . pl_OrientationAngle );
#line 316 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
CEntityPointer penHit  = CreateEntity  (plHit  , CLASS_BASIC_EFFECT );
#line 317 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
penHit  -> Initialize  (ese );
#line 318 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
}
BOOL CBullet::
#line 322 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CBullet_Main
	ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EBulletInit, "CBullet::Main expects 'EBulletInit' as input!");	const EBulletInit &eInit = (const EBulletInit &)__eeInput;
#line 325 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
ASSERT  (((CEntity  *) eInit  . eidOwner ) != NULL );
#line 326 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_penOwner  = eInit  . eidOwner ;
#line 327 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_fDamage  = eInit  . fDamage ;
#line 329 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
InitAsVoid  ();
#line 330 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
SetPhysicsFlags  (EPF_MODEL_IMMATERIAL );
#line 331 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
SetCollisionFlags  (ECF_IMMATERIAL );
#line 334 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
m_iBullet  = 0;
#line 335 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
Return(STATE_CURRENT,EVoid());
#line 335 "C:/Users/pwesty/Desktop/SD-Source/nov-source/Reco_Csrc/EntitiesMP/Bullet.es"
return TRUE; ASSERT(FALSE); return TRUE;};