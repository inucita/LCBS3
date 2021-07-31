206
%{
#include "StdH.h"
#include "EntitiesMP/Projectile.h"
#include "EntitiesMP/Player.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Network/MessageDefine.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Contents/Base/ChattingUI.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Party.h>
#include <Engine/Object/ActorMgr.h>
%}

// Touch Field Type
enum TouchFieldType
{
	0 TOUCHFIELD_DEFAULT	"Default TouchField",
	1 TOUCHFIELD_ITEMCHECK	"Item Check TouchField",
	2 TOUCHFIELD_QUESTCOMPLETE	"Quest Complete TouchField",		// ����Ʈ �Ϸ� ó��
	3 TOUCHFIELD_CHECKPOINT		"Quest Check Point",
	4 TOUCHFIELD_EVENT		"Send Event Message of Raid system"
};

%{

	BOOL ConsiderAll(CEntity*pen) 
	{
  return TRUE;
	}
	BOOL ConsiderPlayers(CEntity*pen) 
	{
  return IsDerivedFromClass( pen, &CPlayer_DLLClass);
	}
%}

class CTouchField: CRationalEntity 
{
	name      "Touch Field";
	thumbnail "Thumbnails\\TouchField.tbn";
	features "HasName", "IsTargetable", "HasRaidObject", "RaidEvent";

properties:
/*** Export Data***/
	1 CTString m_strName            "Name" 'N' = "Touch Field",       // class name
	200 BOOL m_bRaidObject			"This entity is RaidObject" = FALSE,		// ���̵� ������Ʈ ����
	201 INDEX m_RaidEventType		"Raid Event Type" = 0,			// ���̵� �̺�Ʈ Ÿ��
	202 BOOL m_bActiveEffect		"Trigger Active Effect"	= FALSE, // ���̵� �̺�Ʈ�� ����Ʈ �ߵ�
	203 CTString m_strEffectGroupName "Trigger Effect Name(Event)" = "", // ���̵� �̺�Ʈ�� ����Ʈ �ߵ��� ����Ʈ �̸�
/******************/
	2 CEntityPointer m_penEnter     "Enter Target" 'T' COLOR(C_BROWN|0xFF), // target to send event to
	7 CEntityPointer m_penExit      "Exit Target" COLOR(C_dRED|0xFF), // target to send event to
	3 enum EventEType m_eetEnter    "Enter Event" 'E' = EET_TRIGGER,  // event to send on enter
	8 enum EventEType m_eetExit     "Exit Event" = EET_TRIGGER,      // event to send on exit
	4 BOOL m_bActive                "Active" 'A' = TRUE,              // is field active
	5 BOOL m_bPlayersOnly           "Players only" 'P' = TRUE,        // reacts only on players
	6 FLOAT m_tmExitCheck           "Exit check time" 'X' = 0.0f,     // how often to check for exit
	9 BOOL m_bBlockNonPlayers       "Block non-players" 'B' = FALSE,  // everything except players cannot pass
	10 BOOL m_bCollision			"Collision"	  = FALSE,

	100 CEntityPointer m_penLastIn,
	110 CEntityPointer m_penOnly   "Target Only",								// �ش� Ÿ���� �ǵ��������...
	120 enum TouchFieldType eType	 "TouchField Type" = TOUCHFIELD_DEFAULT,	// ��ġ �ʵ��� ����.
	125 INDEX	m_iCheckItemIndex	 "CheckItem Index"	= -1,					// üũ�� �������� �ε���.
	126 INDEX	m_iQuestIndex		"Quest Index(hardcoding)"	= -1,			// ����Ʈ Index
	130 INDEX	m_iCheckPointFlag		"CheckPoint Flag" = 0,					// ���� üũ ����Ʈ
	{
		CFieldSettings m_fsField;
	}

components:
 1 editor texture TEXTURE_FIELD  "Data\\Models\\Editor\\CollisionBox.tex",

functions:

  void SetupFieldSettings(void)
  {
    m_fsField.fs_toTexture.SetData(GetTextureDataForComponent(TEXTURE_FIELD));
    m_fsField.fs_colColor = C_WHITE|CT_OPAQUE;

		if( m_bCollision )
		{
			SetCollisionFlags(ECF_BRUSH);
		}
		else 
		{
			SetCollisionFlags(ECF_MODEL_NO_COLLISION);
		}
  }

	CFieldSettings *GetFieldSettings(void) 
	{
		if (m_fsField.fs_toTexture.GetData()==NULL) 
		{
      SetupFieldSettings();      
    }
    return &m_fsField;
  };

  // returns bytes of memory used by this object
  SLONG GetUsedMemory(void)
  {
    // initial
    SLONG slUsedMemory = sizeof(CTouchField) - sizeof(CRationalEntity) + CRationalEntity::GetUsedMemory();
    // add some more
    slUsedMemory += m_strName.Length();
    return slUsedMemory;
  }

	// wooss 070322 ----------------------------------------------------->>
	// kw : WSS_FIX_SAVE_PRINCESS
	// ����Ʈ�� ���������� �׿����� ��� �Ǵ�
	// �켱 ���ֱ��� ����Ʈ�� ó��...
	// $ : ���ְ� ���� �������� ����Ʈ �Ϸ� ó���� ����������, CheckOthers()�� ������� �ʴ´�. 
	HRESULT CheckOthers(INDEX QuestIndex) 
	{
		switch(QuestIndex)
		{
			case 14: 
				{
					// Princess(Enemy Class)ID : 2044
					CEntity *penEntity	= _pNetwork->ga_World.EntityFromID(2044);
					static BOOL checkOnce = TRUE;
					
					// ���忡 ���� ���� �����ؾ� ��...
					if( penEntity == NULL )
					{
						return S_FALSE;
					}
					
					// 10���ͳ��� ���ְ� �־�� ��...
					FLOAT3D tPos = penEntity->GetPlacement().pl_PositionVector;
					FLOAT3D tMyPos = GetPlayerEntity(0)->GetPlacement().pl_PositionVector;
					FLOAT tLength = (tPos - tMyPos).Length();
					if ( tLength > 10.0f )
					{
						if( checkOnce )
						{
							SE_Get_UIManagerPtr()->GetChattingUI()->AddSysMessage(_S(3264,"���ְ� ������ �ʽ��ϴ�."),SYSMSG_ERROR);
							checkOnce = FALSE;
						}
						return S_FALSE;
					}
				}
				break;
		}
		return S_OK;
	}
	// ---------------------------------------------------------------------<<

procedures:
	// main initialization
	Main(EVoid) 
	{
		InitAsFieldBrush();
		SetPhysicsFlags(EPF_BRUSH_FIXED);
    
		SetFlagOn(ENF_MARKDESTROY);
		SetFlagOn(ENF_NONETCONNECT);
		SetFlagOff(ENF_PROPSCHANGED);

		if ( !m_bBlockNonPlayers ) 
		{
			SetCollisionFlags( ((ECBI_MODEL)<<ECB_TEST) | ((ECBI_BRUSH)<<ECB_IS) | ((ECBI_MODEL)<<ECB_PASS) );
		} 
		else 
		{
			SetCollisionFlags( ((ECBI_MODEL|ECBI_PLAYER|ECBI_PROJECTILE_SOLID|ECBI_PROJECTILE_MAGIC)<<ECB_TEST) 
					| ((ECBI_BRUSH)<<ECB_IS) | ((ECBI_PLAYER|ECBI_PROJECTILE_SOLID|ECBI_PROJECTILE_MAGIC)<<ECB_PASS) );
		}

		if( m_bCollision )
		{
			SetCollisionFlags(ECF_BRUSH);
		}
		else 
		{
			SetCollisionFlags(ECF_MODEL_NO_COLLISION);
		}

		if (m_bActive) 
		{
			jump WaitingEntry();
		} 
		else 
		{
			jump Frozen();
		}

		return;
	};

  // field is active
	WaitingEntry() 
	{
		if (!m_bActive) 
		{
			SetFlagOn(ENF_PROPSCHANGED);
		}
		
		m_bActive = TRUE;

		wait() 
		{
			on (EBegin) : { resume; }
			on (EDeactivate) : { jump Frozen(); }
			// when someone passes the polygons
			on (EPass ep) : 
			{				
				// stop enemy projectiles if blocks non players 
				if( m_bBlockNonPlayers && IsOfClass(ep.penOther, &CProjectile_DLLClass))
				{
					if (!IsOfClass(((CProjectile *)&*ep.penOther)->m_penLauncher, &CPlayer_DLLClass)) 
					{
						EPass epass;
						epass.penOther = this;
						ep.penOther->SendEvent(epass);
					}
				}

				BOOL bPrincessEscape = FALSE;

				  // if should react only on players and not player,
				if( m_bPlayersOnly && !IsDerivedFromClass( ep.penOther, &CPlayer_DLLClass)) 
				{
					resume;
				}
        
				// �ش� �������� ���� �ִ��� üũ��.
				// ����Ʈ �Ϸ�üũ�Ҷ� �����۵� üũ.
				if (m_iCheckItemIndex > 0)
				{
					if (!ItemHelp::HaveItem(m_iCheckItemIndex))
					{
						resume;
					}
				}

				// �÷��̾ �ƴϰ� �ش� Ÿ���� �޼����� ���� Ÿ���� �ƴҰ��...
				if( !m_bPlayersOnly && 
					(m_penOnly != NULL) && 
					ep.penOther != m_penOnly)
				{
					resume;
				}
				
				if(eType == TOUCHFIELD_QUESTCOMPLETE)
				{
					if(m_bActive)
					{
						ASSERT(m_iQuestIndex != -1 && "Invalid Quest Index");
						// FIXME : �ϵ� �ڵ���.
						// ���� ����Ʈ �϶���...
						if(m_iQuestIndex != -1 )
						{
							// FIXME : �ϵ� �ڵ���.
							if( (GAMEDATAMGR()->GetPartyInfo()->GetMemberCount() > 0 && m_iQuestIndex == 14) || m_iQuestIndex == 45 )
							{
								_pNetwork->SendQuestMessage(MSG_QUEST_COMPLETE, m_iQuestIndex);
								m_bActive = FALSE;
							}
						}
					}
					resume;
				}
				else if(eType == TOUCHFIELD_CHECKPOINT )
				{
					// passed the check-point, reset all enemy data 
					if(m_bActive)
					{
						_pNetwork->SetCheckPointData(m_iQuestIndex,m_iCheckPointFlag);
						if( m_iQuestIndex == 13 ) 
						{
							// �߷� ������ ���
							// �ٴ����� �������� �̺�Ʈ �߻��� ���� ������ �����ش�.
							ACTORMGR()->RemoveAll();

							_pNetwork->wo_dwEnemyCount = 0;
						}
						
						m_bActive = FALSE;

						// Debug Output
						if(_pNetwork->m_ubGMLevel > 1)
						{
							CTString strMessage;
							strMessage.PrintF("=====Set Check Point : %X=====\n", m_iCheckPointFlag);
							_pNetwork->ClientSystemMessage(strMessage);
							strMessage.PrintF("=====Check Point : %X=====\n", _pNetwork->wo_stCheckPoint.m_iCheckFlag);
							_pNetwork->ClientSystemMessage(strMessage);
						}
					}
				}
				else if (eType == TOUCHFIELD_EVENT)
				{//TOUCH_FIELD 0
					_pNetwork->SendRaidScene(0, en_ulID);
				}
				
				if (m_bRaidObject && m_bActive) // ���̵� ������Ʈ�� ��� ������ �����Ѱ��� �˸���.
				{
					_pNetwork->SendRaidObjectEvent(en_ulID);
				}

				// send event
				SendToTarget(m_penEnter, m_eetEnter, ep.penOther);
				// if checking for exit

				if (m_tmExitCheck>=0) 
				{
					// remember who entered
					m_penLastIn = ep.penOther;
					SetFlagOn(ENF_PROPSCHANGED);
					// wait for exit
					jump WaitingExit();
				}
				resume;
			}
			// received trigger event message from server
			on (ETrigger) : {
				if (!_pNetwork->m_bSingleMode)
				{
					m_bActive = FALSE; // ��Ȱ��ȭ
				}
				
				if (m_bCollision)
				{
					SetCollisionFlags(ECF_MODEL_NO_COLLISION);
				}

				FLOATaabbox3D tmpBox;

				if (m_bActiveEffect && m_strEffectGroupName != "") // ����Ʈ �ߵ�(��ġ �ʵ� �߽ɿ���)
				{
					GetSize(tmpBox); //altarfloor
					FLOAT3D tmp3D = tmpBox.Center();
					tmp3D(1) += GetPlacement().pl_PositionVector(1);
					tmp3D(2) = GetPlacement().pl_PositionVector(2);
					tmp3D(3) += GetPlacement().pl_PositionVector(3);
					StartEffectGroup(m_strEffectGroupName, _pTimer->GetLerpedCurrentTick(), tmp3D, ANGLE3D(0,0,0));
				}

				//GetSize(tmpBox);
				resume;
			}
		}
  };

  // waiting for entity to exit
	WaitingExit() 
	{
		while(TRUE) 
		{
      // wait
			wait(m_tmExitCheck) 
			{
				on (EBegin) : { resume; }
				on (EDeactivate) : { jump Frozen(); }
				on (ETimer) : 
				{
					// check for entities inside
					CEntity *penNewIn;

					if (m_bPlayersOnly) 
					{
			            penNewIn = TouchingEntity(ConsiderPlayers, m_penLastIn);
					} 
					else 
					{
			            penNewIn = TouchingEntity(ConsiderAll, m_penLastIn);
					}
					
					// if there are no entities in anymore
					if (penNewIn==NULL) 
					{
						// send event
						SendToTarget(m_penExit, m_eetExit, m_penLastIn);
						SetFlagOn(ENF_PROPSCHANGED);

						if (eType == TOUCHFIELD_EVENT) // ������ ��
						{

						}

						// wait new entry
						jump WaitingEntry();
					}
					m_penLastIn = penNewIn;
					stop;
				}
				on (ETrigger) : {
					//m_bActive = FALSE; // ��Ȱ��ȭ
					
					/*if (m_bCollision)
					{
						SetCollisionFlags(ECF_MODEL_NO_COLLISION);
					}*/

					FLOATaabbox3D tmpBox;

					if (m_bActiveEffect && m_strEffectGroupName != "") // ����Ʈ �ߵ�(��ġ �ʵ� �߽ɿ���)
					{
						GetSize(tmpBox); //altarfloor
						FLOAT3D tmp3D = tmpBox.Center();
						tmp3D(1) += GetPlacement().pl_PositionVector(1);
						tmp3D(2) = GetPlacement().pl_PositionVector(2);
						tmp3D(3) += GetPlacement().pl_PositionVector(3);
						StartEffectGroup(m_strEffectGroupName, _pTimer->GetLerpedCurrentTick(), tmp3D, ANGLE3D(0,0,0));
					}

					//GetSize(tmpBox);
					resume;
				}
			}
		}
	};

  // field is frozen
	Frozen() 
	{
		if (m_bActive) 
		{
			SetFlagOn(ENF_PROPSCHANGED);
		}
		
		m_bActive = FALSE;

		wait() 
		{
			on (EBegin) : { resume; }
			on (EActivate) : { jump WaitingEntry(); }
		}
	};
};
