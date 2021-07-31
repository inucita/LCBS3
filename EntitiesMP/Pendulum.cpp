/*
 * This file is generated by Entity Class Compiler, (c) CroTeam 1997-98
 */

#line 2 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"

#include "StdH.h"

#include <EntitiesMP/Pendulum.h>
#include <EntitiesMP/Pendulum_tables.h>
void CPendulum::SetDefaultProperties(void) {
  m_strName = "Pendulum";
  m_bDynamicShadows = FALSE ;
  m_fMaxAngle = 60.0f;
  m_fSpeed = 0.0f;
  m_fDampFactor = 0.9f;
  m_fPendulumFactor = 1.0f;
  m_fImpulseFactor = 0.01f;
  m_fTriggerImpulse = 10.0f;
  m_bActive = TRUE ;
  CMovableBrushEntity::SetDefaultProperties();
}
  
#line 25 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
void CPendulum::ReceiveDamage(CEntity * penInflictor,enum DamageType dmtType,
#line 26 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
FLOAT fDamageAmmount,const FLOAT3D & vHitPoint,const FLOAT3D & vDirection) 
#line 27 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{
#line 28 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
if(! m_bActive )
#line 29 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{
#line 30 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
return ;
#line 31 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
#line 33 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
FLOAT3D vOscilatingDirection ;
#line 34 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
GetHeadingDirection  (- 90.0f , vOscilatingDirection );
#line 36 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
FLOAT fImpulse  = vDirection  % vOscilatingDirection ;
#line 38 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
fImpulse  *= fDamageAmmount  * m_fImpulseFactor ;
#line 40 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
m_fSpeed  += fImpulse ;
#line 41 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetDesiredRotation  (ANGLE3D (0 , 0 , m_fSpeed ));
#line 42 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
  
#line 45 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
void CPendulum::PostMoving() 
#line 46 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{
#line 47 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
CMovableBrushEntity  :: PostMoving  ();
#line 48 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
ANGLE fCurrentBanking  = GetPlacement  () . pl_OrientationAngle  (3);
#line 49 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
FLOAT fNewSpeed  = m_fSpeed  * m_fDampFactor  - m_fPendulumFactor  * fCurrentBanking ;
#line 52 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
if(Abs  (fCurrentBanking ) > m_fMaxAngle  && Sgn  (fNewSpeed ) == Sgn  (fCurrentBanking ))
#line 53 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{
#line 54 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
fNewSpeed  = 0.0f;
#line 55 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
#line 57 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
m_fSpeed  = fNewSpeed ;
#line 58 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetDesiredRotation  (ANGLE3D (0 , 0 , fNewSpeed ));
#line 61 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
if(Abs  (fCurrentBanking ) > 1.0f)
#line 62 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{
#line 64 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetFlags  (GetFlags  () & ~ ENF_INRENDERING );
#line 65 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
#line 66 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
BOOL CPendulum::
#line 69 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
Main(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT STATE_CPendulum_Main
  ASSERTMSG(__eeInput.ee_slEvent==EVENTCODE_EVoid, "CPendulum::Main expects 'EVoid' as input!");  const EVoid &e = (const EVoid &)__eeInput;
#line 71 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
InitAsBrush  ();
#line 72 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetPhysicsFlags  (EPF_BRUSH_MOVING );
#line 73 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetCollisionFlags  (ECF_BRUSH );
#line 75 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetFlags  (GetFlags  () & ~ ENF_ZONING );
#line 78 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
if(m_bDynamicShadows ){
#line 79 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetFlags  (GetFlags  () | ENF_DYNAMICSHADOWS );
#line 80 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}else {
#line 81 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetFlags  (GetFlags  () & ~ ENF_DYNAMICSHADOWS );
#line 82 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
#line 85 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
SetTimerAt(THINKTIME_NEVER);
Jump(STATE_CURRENT, 0x006a0000, FALSE, EBegin());return TRUE;}BOOL CPendulum::H0x006a0000_Main_01(const CEntityEvent &__eeInput) {
#undef STATE_CURRENT
#define STATE_CURRENT 0x006a0000
switch(__eeInput.ee_slEvent){case(EVENTCODE_EActivate):
#line 87 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{const EActivate&e= (EActivate&)__eeInput;

#line 88 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
m_bActive  = TRUE ;
#line 89 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
return TRUE;
#line 90 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}ASSERT(FALSE);break;case(EVENTCODE_EDeactivate):
#line 92 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{const EDeactivate&e= (EDeactivate&)__eeInput;

#line 93 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
m_bActive  = FALSE ;
#line 94 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
return TRUE;
#line 95 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}ASSERT(FALSE);break;case(EVENTCODE_ETrigger):
#line 97 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{const ETrigger&e= (ETrigger&)__eeInput;

#line 98 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
if(m_bActive )
#line 99 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
{
#line 101 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
m_fSpeed  += m_fTriggerImpulse ;
#line 102 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
AddToMovers  ();
#line 103 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}
#line 104 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
return TRUE;
#line 105 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}ASSERT(FALSE);break;default: return FALSE; break;
#line 106 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
}return TRUE;}BOOL CPendulum::H0x006a0001_Main_02(const CEntityEvent &__eeInput){
ASSERT(__eeInput.ee_slEvent==EVENTCODE_EInternal);
#undef STATE_CURRENT
#define STATE_CURRENT 0x006a0001

#line 108 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
Destroy  ();
#line 109 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
UnsetTimer();Jump(STATE_CURRENT,0x006a0001, FALSE, EInternal());return TRUE;
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
Return(STATE_CURRENT,EVoid());
#line 110 "E:/질산/Korea/Client_USA/EntitiesMP/Pendulum.es"
return TRUE; ASSERT(FALSE); return TRUE;};