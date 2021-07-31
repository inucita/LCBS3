#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/MobData.h>
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/World/World.h>
#include <Engine/Network/CNetwork.h> // WSS_DRATAN_SEIGEWARFARE 2007/08/08
#include <Engine/Interface/UIManager.h>
#include <Engine/Secure/FileSecure.h>	// [2012/07/18 : Sora]  ���� �����ڵ� �߰�
/*
 *  Constructor.
 */
CMobData::CMobData(void)
	: mob_ulZoneFlag(0)
	, mob_ulExtraFlag(0)
{
	mob_bBoss		= FALSE;
	Owner_Index	= -1;

	channel_flag = (short)0xFFFF;
}

/*
 *  Destructor.
 */
CMobData::~CMobData(void) 
{
	mob_ulZoneFlag	= 0;
	mob_ulExtraFlag	= 0;
}

CTString CMobData::GetAnimAttackNameByID(int iAttackID)
{
	ASSERT(iAttackID > 0 && "Invalid Attack Animation Number");

	// === Attack Animation ��Ģ ===
	// Attack##		--> ##�� ����.
	// ��) Attack01, Attack02...
	CTString strAttackName;
	strAttackName.PrintF(TRANS("%s%02d"), GetAnimAttackName(), iAttackID);
	return strAttackName;
}

bool CMobData::LoadZoneData( const char* ZNFileName )
{
	int iReadBytes = 0;
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	// �� �̵� �÷��� ������ ȭ�Ϸκ��� �о����.
	if(ZNFileName)
	{
		FILE *fp		= NULL;
		if ((fp = fopen(ZNFileName, "rb")) == NULL) 
		{
			MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
			return false;
		}
		int nCount = 0;
		int nIndex;
		LOADINT(nCount);
		ASSERT(nCount > 0 && "Invalid Zone Move Npc Data");

		for (int i = 0; i < nCount; ++i)
		{
			iReadBytes = fread(&nIndex,	sizeof(int), 1, fp);
			
			CMobData* pData = CMobData::getData(nIndex);
			
			if (pData == NULL)
				return false;

			iReadBytes = fread(&pData->mob_ulZoneFlag,	sizeof(ZONEFLAGS), 1, fp);	// Zone Flag
			iReadBytes = fread(&pData->mob_ulExtraFlag,	sizeof(ZONEFLAGS), 1, fp);	// Extra Flag			
		}

		fclose(fp);
	}
	//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return true;
}


bool CMobData::loadEx(const char* FileName)
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stNpc* pdata = new stNpc[_nSize];
	fread(pdata, sizeof(stNpc) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CMobData* ptmp = new CMobData;
		memcpy(ptmp, &pdata[i], sizeof(stNpc));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}

		_vecdata.push_back(ptmp);
	}

	m_dummy = new CMobData; // ���̵���Ÿ ����
	
	memset(m_dummy, 0, sizeof(stNpc));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	SetSDNpc();

	return true;
}

bool CMobData::LoadChannelData( const char* FileName )
{
	FILE *fp		= NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	int nSize = 0;

	fread(&nSize, sizeof(int), 1, fp);

	if (nSize > 0)
	{
		struct stChannel
		{
			int npcIndex;
			int channel[16];
		};

		stChannel* pChannel = new stChannel[nSize];
		fread(pChannel, sizeof(stChannel) * nSize, 1, fp);
		fclose(fp);

		for (int i = 0; i < nSize; ++i)
		{
			CMobData* pMD = CMobData::getData(pChannel[i].npcIndex);

			if (pMD == NULL)
				continue;

			short channel = 0;

			for (int j = 0; j < 16; ++j)
			{
				if (pChannel[i].channel[j] > 0)
					channel |= (pChannel[i].channel[j] << j);
			}

			pMD->SetChannelFlag(channel);
		}

		SAFE_ARRAY_DELETE(pChannel);
	}
	else
	{
		fclose(fp);

		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : penEntity - 
//			MD - 
// Output : int
//-----------------------------------------------------------------------------
int CMobData::SetMobDataToNPC(CEntity* penEntity, CMobData* MD, const char* szMobName)
{
	if (MD == NULL)
		return -1;

	ASSERT(penEntity != NULL && "Invalid Entity Pointer!");
	ASSERT(MD->GetMobIndex() != -1 && "Invalid Mob Index!");
	
	if(MD->IsPeaceful())
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
		penEntity->SetFirstExtraFlagOn(ENF_EX1_PEACEFUL);
	}

	if(MD->IsZoneMoving())	
	{
		// WSS_DRATAN_SIEGEWARFARE 2007/10/14 -------------------------------->>
		BOOL bSkip = FALSE;
		if( MD->IsCastleTower() )
		{
			// ���� Ÿ���� ��� �������� ��� �Ϲ� ������ ó����
			if(!(_pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_OWNER ||
				 _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_DEFENSE_GUILD) )
			{
				bSkip = TRUE;				
			}
		}
		// -------------------------------------------------------------------<<

		if( !bSkip )
		{
			// �� �̵� ��ġ �÷��װ� �����ִ� ���, �� �̵� �÷��׸� �־���.
			penEntity->SetFlagOn(ENF_ZONEMOVING);
			penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);

			CEntityProperty &epPropertyZoneFlag	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_ZONEFLAGS_EX, 237));		// Zone Flag
			ENTITYPROPERTY( &*penEntity, epPropertyZoneFlag.ep_slOffset, ZONEFLAGS)		= MD->GetZoneMovingFlag();

			CEntityProperty &epPropertyExtraFlag	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_ZONEFLAGS_EX, 238));	// Extra Flag
			ENTITYPROPERTY( &*penEntity, epPropertyExtraFlag.ep_slOffset, ZONEFLAGS)	= MD->GetExtraFlag();
		}
	}
	else if(MD->IsResource())							// ���깰ǰ
	{
		// ������ �ݵ�� NPC�� �ƴϾ����.
		penEntity->SetFirstExtraFlagOn(ENF_EX1_PRODUCTION);
		penEntity->SetFirstExtraFlagOff(ENF_EX1_NPC);
	}
	else if( MD->IsSkillMaster() || MD->IsSSkillMaster() || MD->IsNPC() )		// NPC
	{
		penEntity->SetFirstExtraFlagOn(ENF_EX1_NPC);
	}
	else
	{
		penEntity->SetFlagOn(ENF_ALIVE);
	}

	CEntityProperty &epPropertyNpcIndex	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91));	// Enemy Index
	ENTITYPROPERTY( &*penEntity, epPropertyNpcIndex.ep_slOffset, INDEX)		= MD->GetMobIndex();

	// NOTE : ���� Ÿ�ӵ� ��������� ��(���� �����Ϳ����� �ʿ���)
	//CEntityProperty &epPropertyNpcIndex	= pdecDLLClass->dec_aepProperties[3];		// Regen Time
	//ENTITYPROPERTY( &*penEntity, epPropertyNpcIndex.ep_slOffset, INDEX)		= MD->GetIndex();

	CEntityProperty &epPropertyAlready	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 250));	// Initialized?
	ENTITYPROPERTY( &*penEntity, epPropertyAlready.ep_slOffset, BOOL)	= TRUE;
	
	//-----------------------------------------------------------------------------
	// �ִϸ��̼� ������ ������.
	//-----------------------------------------------------------------------------
	CEntityProperty &epPropertyWalk		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 100));	// Walk Animation
	ENTITYPROPERTY( &*penEntity, epPropertyWalk.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimWalkName());
	
	CEntityProperty &epPropertyAttack	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 101));	// Attack Animation
	ENTITYPROPERTY( &*penEntity, epPropertyAttack.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD->GetAnimAttackName());
	
	CEntityProperty &epPropertyIdle		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 102));	// Idle Animation
	ENTITYPROPERTY( &*penEntity, epPropertyIdle.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimDefaultName());
	
	CEntityProperty &epPropertyDeath	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 103));	// Death Animation
	ENTITYPROPERTY( &*penEntity, epPropertyDeath.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD->GetAnimDeathName());
	
	CEntityProperty &epPropertyWound	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 104));	// Wound Animation
	ENTITYPROPERTY( &*penEntity, epPropertyWound.ep_slOffset, INDEX)	= ska_GetIDFromStringTable(MD->GetAnimWoundName());
	
	CEntityProperty &epPropertyBox		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 105));	// Normal Box
	ENTITYPROPERTY( &*penEntity, epPropertyBox.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetNormalBoxName());

	CEntityProperty &epPropertyRun		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 107));	// Run Animation
	ENTITYPROPERTY( &*penEntity, epPropertyRun.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimRunName());

	CEntityProperty &epPropertyStretch	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 5));	// Model stretch Scale
	ENTITYPROPERTY( &*penEntity, epPropertyStretch.ep_slOffset, FLOAT)	= MD->GetScale();
	
	CEntityProperty &epPropertyAttack2	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 106));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimAttack2Name());

	CEntityProperty &epPropertyIdle2		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 108));	// Idle Animation
	ENTITYPROPERTY( &*penEntity, epPropertyIdle2.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimDefault2Name());

	CEntityProperty &epPropertyAttack3	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 109));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack3.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimAttack3Name());

	CEntityProperty &epPropertyAttack4	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 110));	// Attack Animation 2
	ENTITYPROPERTY( &*penEntity, epPropertyAttack4.ep_slOffset, INDEX)		= ska_GetIDFromStringTable(MD->GetAnimAttack4Name());

	CEntityProperty &epPropertyName		= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_STRING, 200));	// Mob Name
	ENTITYPROPERTY( &*penEntity, epPropertyName.ep_slOffset, CTString)	= szMobName;

	// FIXME : �̱۴��������� ����...��.��
	if( MD->IsBoss() )
	{
		CEntityProperty &epPropertyBoss	= *(penEntity->PropertyForTypeAndID(CEntityProperty::EPT_BOOL, 160));	// Is Boss Mob?
		ENTITYPROPERTY( &*penEntity, epPropertyBoss.ep_slOffset, BOOL)	= MD->IsBoss();
	}
	
	CDLLEntityClass *pdecDLLBaseClass	= penEntity->GetClass()->ec_pdecDLLClass->dec_pdecBase;
	CDLLEntityClass *pdecDLLBaseBaseClass	= pdecDLLBaseClass->dec_pdecBase;

	CEntityProperty &epPropertyWalkSpeed		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 10);	// NPC Walk Speed
	ENTITYPROPERTY( &*penEntity, epPropertyWalkSpeed.ep_slOffset, FLOAT)			= MD->GetWalkSpeed();	

	CEntityProperty &epPropertyAttackRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 12);	// NPC Attack Run Speed
	ENTITYPROPERTY( &*penEntity, epPropertyAttackRunSpeed.ep_slOffset, FLOAT)		= MD->GetRunSpeed();

	CEntityProperty &epPropertyCloseRunSpeed	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 14);	// NPC Close Run Speed
	ENTITYPROPERTY( &*penEntity, epPropertyCloseRunSpeed.ep_slOffset, FLOAT)		= MD->GetRunSpeed();	

	CEntityProperty &epPropertyAttackDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 20);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyAttackDistance.ep_slOffset, FLOAT)		= MD->GetAttackDistance() * 1.5f;

	CEntityProperty &epPropertyHealth	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 143);	// NPC Health
	ENTITYPROPERTY( &*penEntity, epPropertyHealth.ep_slOffset, FLOAT)		= MD->GetHealth();
	
	// SetMobDataToNPC ���� SetMobData()���� Entity�� ���� HP�� ������ �Ǿ� �ֽ��ϴ�.
	//((CLiveEntity*)penEntity)->SetHealth(MD->GetHealth());
	// --> MD�� �ƴ�, Entity�� HP�� �����սô�.
	CEntityProperty &epPropertyCurHealth	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 202);	// NPC Health
	INDEX MobHp = ENTITYPROPERTY( &*penEntity, epPropertyCurHealth.ep_slOffset, INDEX);

	((CLiveEntity*)penEntity)->SetHealth(MobHp);

	if (MD->GetMobIndex() == LORD_SYMBOL_INDEX) // ���´� hp���� ���� �ٸ���. ���� ���� � Ÿ�������� �����Ѵ�. �� �������� ���� �̹� ������ �ִ�.
	{ // type -1 (������� �ʴ� m_nPlayActionNum ������ type���� ����Ѵ�.)

		CEntityProperty &epPropertyPlayActionNum = *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 224);	// NPC PlayerActionNum

		if (MobHp <= MD->GetHealth() * 0.25f) // type 0
		{
			ENTITYPROPERTY( &*penEntity, epPropertyPlayActionNum.ep_slOffset, INDEX) = 0;
		}
		else if (MobHp <= MD->GetHealth() * 0.50f) // type 1
		{
			ENTITYPROPERTY( &*penEntity, epPropertyPlayActionNum.ep_slOffset, INDEX) = 1;
		}
		else if (MobHp <= MD->GetHealth() * 0.75f) // type 2
		{
			ENTITYPROPERTY( &*penEntity, epPropertyPlayActionNum.ep_slOffset, INDEX) = 2;
		}
	}

	float fStopDistance		= MD->GetAttackDistance() - 0.1f;
	float fCloseDistance	= MD->GetAttackDistance();
	if(fStopDistance < 0.0f)	fStopDistance	= 0.0f;
	if(fCloseDistance < 0.0f)	fCloseDistance	= 0.0f;

	CEntityProperty &epPropertyStopDistance		= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 24);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyStopDistance.ep_slOffset, FLOAT)			= fStopDistance;

	CEntityProperty &epPropertyCloseDistance	= *pdecDLLBaseBaseClass->PropertyForTypeAndID(CEntityProperty::EPT_FLOAT, 21);	// Attack Distance
	ENTITYPROPERTY( &*penEntity, epPropertyCloseDistance.ep_slOffset, FLOAT)		= fCloseDistance;
	
	return -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  Constructor.
 */

void CMobData::SetNoTranslate()
{
	char buff[MAX_PATH];
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
	{
		sprintf( buff, "[%d] : npc name", index );
		SetName(buff);
	}

	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
	{
		sprintf( buff, "[%d] : npc desc", index );
		SetDesc(buff);
	}
}

void CMobData::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		name = "";
	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		desc = "";
}

void CMobData::SetTranslateFlag( int flag )
{
	transFlag |= flag;
}

// WSS_DRATAN_SEIGEWARFARE 2007/08/08 ---------------------------------------->>
// ���ź ������ Ÿ�� ���� ����...
// NPC�� �Ǹ� ��ǥ�ð� �ǰ� ���� â�� ������ �� �ְ�, NPC�� �ƴϸ� 
// CheckTarget�� ���� ���ݿ��θ� �Ǵ��� �� �ִ� 
BOOL CMobData::IsNPC() const
{
	// ���� ���� ����
	// NPC_CASTLE_TOWER�� �����Ǿ� ������ �⺻������ NPC�� �������� ����
	// ������ ��� ������ ������ ���� �������� �ٸ��� ������...
	if( (flag&NPC_CASTLE_TOWER) )
	{
		int tRes = IsMyQuarter();
		if (tRes != QUARTER_NONE )
		{
			if ( tRes == QUARTER_INSTALL || tRes == QUARTER_MINE) return TRUE;
			else if ( tRes == QUARTER_ATTACK ) return FALSE; 
		}
		if (_pNetwork->MyCharacterInfo.sbJoinFlagDratan!=WCJF_OWNER)
			return FALSE;
		else return TRUE;	
	}		
	else 
		return (flag & NPC_MASK);	
}
// ---------------------------------------------------------------------------<<

// WSS_DRATAN_SEIGEWARFARE 2007/08/13------------------------>>
int CMobData::IsMyQuarter() const
{
	std::map<int,int>::const_iterator tFound	= _pNetwork->MyCharacterInfo.mQuarter.find(index);
	
	if( tFound == _pNetwork->MyCharacterInfo.mQuarter.end() ) return QUARTER_NONE; // ��Ȱ������ �ƴ�
	// �ι� ��ȸ �ʿ����.  [2/24/2011 rumist]
	int  tIdx = tFound->second;
	if (tIdx==-1)
		return QUARTER_INSTALL;	// ��ġ	
	else if ( tIdx == _pNetwork->MyCharacterInfo.lGuildIndex)
		return QUARTER_MINE;	// ���� ����

	return QUARTER_ATTACK;		// ����
}

void CMobData::SetSDNpc()
{
	CMobData::_map::iterator	iter = CMobData::_mapdata.begin();
	CMobData::_map::iterator	eiter = CMobData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CMobData* pNpc = (*iter).second;

		if (pNpc == NULL)
			continue;

		CTString strAttackAnim;
		// FIXME : �˴� �ϵ� �ڵ�~~~~
		// FIXME : ���︸ ���� �ִϸ��̼��� 4������...
		// FIXME : �����ѹ� ����!!!
		// ������ ���.
		if( pNpc->getindex() == 193 )
		{			
			strAttackAnim.PrintF( "%s_att%02d", "ghoulworker", 3 );
			strcpy( pNpc->motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att%02d", "ghoulworker", 4 );
			strcpy( pNpc->motion_Attack4, strAttackAnim );
		}
		else if( pNpc->getindex() == 194 )
		{
			strAttackAnim.PrintF( "%s_att_%02d", "ghoulsucker", 3 );
			strcpy( pNpc->motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att_%02d", "ghoulsucker", 4 );
			strcpy( pNpc->motion_Attack4, strAttackAnim );
		}
		else if( pNpc->getindex() == 195 )
		{
			strAttackAnim.PrintF( "%s_att%02d", "ghoulanker", 3 );
			strcpy( pNpc->motion_Attack3, strAttackAnim );
			strAttackAnim.PrintF( "%s_att%02d", "ghoulanker", 4 );
			strcpy( pNpc->motion_Attack4, strAttackAnim );
		}
		else
		{
			// FIXME : �̱۴����� ���ؼ��� �ʿ��� �κ�....��.��
			strcpy( pNpc->motion_Attack3, pNpc->motion_Attack );
			strcpy( pNpc->motion_Attack4, pNpc->motion_Attack2 );
		}

		// FIXME : ���� ������ �ε����� Ȯ�� �Ŀ� �÷��׸� ����...��.��		
		// NOTE : 201 -> �̱۴����� ���� �� ī�̶�.
		if( pNpc->getindex() == 201 )
		{
			pNpc->mob_bBoss	= TRUE;
		}

		// stl �� �迭�� ������ �� �ִ�. �Ʒ��� ���� �ڵ� ����. [2/17/2011 rumist]
		std::sort( pNpc->fireDelay, pNpc->fireDelay+pNpc->fireDelayCount);
	}
}


// ----------------------------------------------------------<<
