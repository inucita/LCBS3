#include "StdH.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInternalClasses.h>
#include "Party.h"
#include "UIPartyNew.h"
#include "UIExpedition.h"
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Object/ActorMgr.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Info/MyInfo.h>
#include <Engine/Contents/Base/PersonalshopUI.h>

// [7/13/2009 rumist] reject invite party.
extern INDEX g_iRejectParty;

Party::Party(void)
{
	InitParty();
	InitExpedition();
}

Party::~Party(void)
{
}

//////////////////////////////////////////////////////////////////////////

void Party::Init()
{
	m_nMemberCount = 0;
	m_bAmILeader = FALSE;
	m_nType = PT_PEACEEVER;
	m_bIsPartyRequested = FALSE;
	m_sbDivisionTypeEXP = 0;
	m_sbDivisionTypeITEM = 0;
	m_sbDivisionTypeSPITEM = 0;
	m_bIsPartyPlay = FALSE;
	m_strMandateCharName = CTString("");
}

void Party::InitParty()
{
	Init();

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		m_PartyInfo[i].slIndice = -1;
		m_PartyInfo[i].strName = CTString( "" );
		m_PartyInfo[i].bLeader = FALSE;
		m_PartyInfo[i].sbJob = -1;
		m_PartyInfo[i].nLevel = 0;
		m_PartyInfo[i].nHP = 0;
		m_PartyInfo[i].nMaxHP = 0;
		m_PartyInfo[i].nMP = 0;
		m_PartyInfo[i].nMaxMP = 0;
		m_PartyInfo[i].fPosX = 0.0f;
		m_PartyInfo[i].fPosZ = 0.0f;
		m_PartyInfo[i].sbLayer = 0;
		m_PartyInfo[i].slZone = 0;
		m_PartyInfo[i].ubBuff = 0;
		m_PartyInfo[i].bOnline = false;

		m_PartyInfo[i].ulTargetID =0;
		m_PartyInfo[i].sbTargetType =0;
	}
}

void Party::InitExpedition()
{
	Init();
	m_nMyGroupNum = -1;

	int i, j;
	for(i = 0; i < EXPEDITION_GROUP_MAX; i++)
	{
		for(j = 0; j < EXPEDITION_MEMBER_PER_GROUP; j++)
			m_ExpeditionGroup[i][j].Init();
	}

	for(i = 0; i < PARTY_MAX_MEMBER; i++)
	{
		m_PartyInfo[i].ubBuff = 0;
	}
}

void Party::ClearPartyTarget(SLONG slTargetIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY) ||  pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i = 0; i < PARTY_MAX_MEMBER; ++i)
		{
			if( m_PartyInfo[i].ulTargetID == slTargetIndex )
			{
				m_PartyInfo[i].ulTargetID = 0;
				m_PartyInfo[i].sbTargetType = 0;
			}
		}
	}
}

void Party::ClearTargetMark()
{
	// ĳ���� ���� ǥ�� ����
	_pNetwork->MyCharacterInfo.slLabel = -1;

	ACTORMGR()->ClearLabel();
}

void Party::MandateBossReq( CTString strManadateChaName )
{
	m_strMandateCharName = strManadateChaName;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "��Ƽ" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_MANDATE_REQ );

	CTString strMessage;

	strMessage.PrintF( _S(2170, "������ %s�Բ� ��Ƽ���� �����Ͻðڽ��ϱ�?" ), strManadateChaName );

	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

void Party::MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate )
{
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "��Ƽ" ), UMBS_OK, UI_PARTY, MSGCMD_PARTY_MANDATE );
	CTString strMessage;

	if( strManadateChaName == _pNetwork->MyCharacterInfo.name ) 
	{
		int nCharPos = IsOurPartyPos( strBossName );

		if( nCharPos != -1 )
		{
			m_PartyInfo[nCharPos].bLeader = FALSE;
		}
	}
	else 
	{
		for( int nMem=0; nMem<m_nMemberCount; nMem++ )
		{
			if( m_PartyInfo[nMem].slIndice == nManadateChaIndex )
			{
				int nCharPos = IsOurPartyPos( strBossName );

				if( nCharPos != -1 )
					m_PartyInfo[nCharPos].bLeader = FALSE;

				m_PartyInfo[nMem].bLeader = TRUE;
				break;
			}
		}
	}

	SetPartyLeader();

	if( sbMandate )
	{
		strMessage.PrintF( _S(2171, "%s���� ��Ƽ���� �����Ͽ� %s���� ���ο� ��Ƽ������ �Ӹ�Ǿ����ϴ�." ), strBossName, strManadateChaName );
	}
	else
	{
		strMessage.PrintF( _S(2172, "%s���� ���ο� ��Ƽ������ �Ӹ�Ǿ����ϴ�." ), strManadateChaName );
	}

	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}


void Party::SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst /* = FALSE */)
{
	switch(sbType)
	{
	case MSG_DIVITYPE_EXP:
		m_sbDivisionTypeEXP = sbDivisionType;
		break;
	case MSG_DIVITYPE_ITEM:
		m_sbDivisionTypeITEM = sbDivisionType;
		break;
	case MSG_DIVITYPE_SPECIAL:
		m_sbDivisionTypeSPITEM = sbDivisionType;
		break;
	}

	m_strDivisionType[MSG_DIVITYPE_EXP].PrintF(_S(4514, "����ġ"));
	m_strDivisionType[MSG_DIVITYPE_ITEM].PrintF(_S(4515, "�Ϲ� ������"));
	m_strDivisionType[MSG_DIVITYPE_SPECIAL].PrintF( _S(4516, "����� ������"));

	m_strExpeditionType[MSG_EXPED_TYPE_RANDOM].PrintF(_S(4517, "����ȹ��"));
	m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET].PrintF(_S(4518, "�Լ��켱"));
	m_strExpeditionType[MSG_EXPED_TYPE_BATTLE].PrintF(_S(4519, "������"));
	m_strExpeditionType[MSG_EXPED_TYPE_OPENBOX].PrintF(_S(4520, "���ڿ���"));

	CTString strTemp, strTemp2;

	strTemp2 = m_strExpeditionType[sbDivisionType];
	if(sbType == MSG_DIVITYPE_EXP)
	{
		if(sbDivisionType == MSG_EXPED_TYPE_RANDOM)
		{
			strTemp2 = _S(4525, "�յ�й�");
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(!bFirst)
	{
		strTemp.PrintF(_S(4569, "%s �й� ����� %s(��)�� ����Ǿ����ϴ�."), m_strDivisionType[sbType], strTemp2);
		pUIManager->GetChattingUI()->AddSysMessage( strTemp );
	}


	// ����â�� ������Ʈ���� �����Ƿ� �ݾ��ش�.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

void Party::SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM)
{
	m_sbDivisionTypeEXP = sbDivisionTypeEXP;

	m_sbDivisionTypeITEM = sbDivisionTypeITEM;

	m_sbDivisionTypeSPITEM = sbDivisionTypeSPITEM;
}

void Party::SetPartyLeader()
{
	for( SLONG i = 0; i < m_nMemberCount; i++ )
	{
		if( m_PartyInfo[i].bLeader == TRUE )
		{
			m_bAmILeader = FALSE;
			return;
		}
	}

	m_bAmILeader = TRUE;
}

void Party::SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType)
{
	// [sora] �����뵵 ����� �� �ֵ��� ����

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		for(int i = 0; i < PARTY_MAX_MEMBER; ++i)
		{
			if( m_PartyInfo[i].slIndice == partyID)
			{
				m_PartyInfo[i].ulTargetID = targetID;
				m_PartyInfo[i].sbTargetType = sbtargetType;
				return;
			}
		}
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i = 0; i < EXPEDITION_MEMBER_PER_GROUP; ++i)
		{
			if( m_ExpeditionGroup[m_nMyGroupNum][i].m_slIndex == partyID )
			{
				m_PartyInfo[i].ulTargetID = targetID;
				m_PartyInfo[i].sbTargetType = sbtargetType;
				return;
			}
		}
	}

}

void Party::SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex)
{

	if(slType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		if(_pNetwork->MyCharacterInfo.index == slIndex)
		{
			if(slMode == MSG_EXPED_SETLABEL_MODE_SET) // ǥ�� ����
			{
				_pNetwork->MyCharacterInfo.slLabel = slLabel;
			}
			else //ǥ�� ����
			{
				_pNetwork->MyCharacterInfo.slLabel = -1;
			}
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if(slMode == MSG_EXPED_SETLABEL_MODE_SET) // ǥ�� ����
			{
				pTarget->cha_Label = slLabel;
			}
			else //ǥ�� ����
			{
				pTarget->cha_Label = -1;
			}
		}
	}
	else	// mob�� ���
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, slIndex);

		if (pObject != NULL)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			if(slMode == MSG_EXPED_SETLABEL_MODE_SET)
				pMT->mob_Label = slLabel;
			else
				pMT->mob_Label = -1;
		}
	}
}

void Party::UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP,
	int iMP, int iMaxMP )
{
	// [sora] �����뵵 ��� �����ϵ��� ����

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		// Find party member
		int iMem;
		for( iMem = 0; iMem < m_nMemberCount; iMem++ )
		{
			if( m_PartyInfo[iMem].slIndice == slIndex )
				break;
		}

		// Not found
		if( iMem == m_nMemberCount )
			return;

		// Update infos
		m_PartyInfo[iMem].nHP = iHP;
		m_PartyInfo[iMem].nMaxHP = iMaxHP;
		m_PartyInfo[iMem].nMP = iMP;
		m_PartyInfo[iMem].nMaxMP = iMaxMP;

		pUIManager->GetParty()->UpdateMemberInfo( iMem );
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		int nGroup = 0;
		int nPos = 0;

		if(SearchExpeditionMember(slIndex, nGroup, nPos))
		{
			m_ExpeditionGroup[nGroup][nPos].m_nHP = iHP;
			m_ExpeditionGroup[nGroup][nPos].m_nMaxHP = iMaxHP;
			m_ExpeditionGroup[nGroup][nPos].m_nMP = iMP;
			m_ExpeditionGroup[nGroup][nPos].m_nMaxMP = iMaxMP;
			m_ExpeditionGroup[nGroup][nPos].UpdateBarRate();

			if (m_nMyGroupNum == nGroup)
			{
				int nRealIdx = 0;

				for (int i = 0; i < nPos; ++i)
				{
					if (GetExpeditionMemberIndex(nGroup, i) == _pNetwork->MyCharacterInfo.index)
						continue;

					if (IsExpedetionDataExist(nGroup, i) == true)
					{
						nRealIdx++;
					}
				}

				pUIManager->GetParty()->UpdateMemberInfo(nPos, true, nRealIdx);
			}

			pUIManager->GetExpedition()->UpdateMemberInfo(nGroup, nPos);
			pUIManager->GetParty()->UpdateGroupMemberInfo(nGroup, nPos);
		}
	}
}

void Party::TargetMember( int nIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// [sora] �����뵵 ó���� �� �ְ� ����(�ڱ� �׷� ����� ����)

	INDEX	nCharIndex = 0;
	INDEX   nIndexReal = 0;
	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		nCharIndex = m_PartyInfo[nIndex].slIndice;

		if( nCharIndex == -1 || nCharIndex == 0)
			return;
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		// �� �׷� ������â���� ������, �󽽷��� ǥ������ �����Ƿ� Ŭ���� ����� ������ ������ ������ ��ġ���� �����Ƿ� ����Ѵ�
		int nCount = 0;
		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
		{
			if(m_ExpeditionGroup[m_nMyGroupNum][i].IsEmpty())
			{
				continue;
			}
			if(m_nGroupPos == i)
			{
				continue;
			}
			if(nCount == nIndex)
			{
				nIndexReal = i;
				break;
			}

			nCount++;
		}
		nCharIndex = GetExpeditionMemberIndex(m_nMyGroupNum, nIndexReal);

		if(nCharIndex == -1)
			return;		
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, nCharIndex);

	if (pObject != NULL)
	{
		CEntity			*penPlEntity;
		CPlayerEntity	*penPlayerEntity;
		penPlEntity = CEntity::GetPlayerEntity( 0 );
		penPlayerEntity = static_cast<CPlayerEntity *>(penPlEntity);
		penPlayerEntity->SetTarget( pObject->GetEntity() );
		return;
	}

	// �����̿� ���ٸ� 
	int nMaxHP = 0;
	int nHP = 0;
	int nLevel = 0;
	CTString strName = "";
	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		nMaxHP = m_PartyInfo[nIndex].nMaxHP;
		nHP = m_PartyInfo[nIndex].nHP;
		nLevel = m_PartyInfo[nIndex].nLevel;
		strName = m_PartyInfo[nIndex].strName;
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		nMaxHP = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_nMaxHP;
		nHP = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_nHP;
		nLevel = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_nLevel;
		strName = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_strName; 
	}

	// Ÿ������ Ŭ����
	_pUIBuff->ResetTargetBuff();

	ObjInfo* pInfo = ObjInfo::getSingleton();
	
	( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->ClearTargetInfo(INFO()->GetTargetEntity(eTARGET));

	CTargetInfom tempInfo;
	tempInfo.fMaxHealth = nMaxHP;
	tempInfo.fHealth = nHP;
	tempInfo.bIsActive = TRUE;
	tempInfo.iLevel = nLevel;
	strcpy(tempInfo.TargetName, strName );
	tempInfo.TargetType = CHARACTER;

	pInfo->SetTargetInfo(tempInfo);
}

int Party::GetExpeditionMyPosition()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION) == 0 )
		return -1;

	if(m_nMyGroupNum < 0 )
		return -1;

	if(m_nGroupPos < 0 || m_nGroupPos >= EXPEDITION_MEMBER_PER_GROUP)
		return -1;

	return m_ExpeditionGroup[m_nMyGroupNum][m_nGroupPos].m_nPosition;
}

int Party::GetExpeditionMyGroupCount()
{
	int nCount = 0;
	for (int i = 0; i < EXPEDITION_MEMBER_PER_GROUP; ++i)
	{
		if (m_ExpeditionGroup[m_nMyGroupNum][i].IsEmpty() == FALSE)
		{
			nCount++;
		}
	}

	return nCount - 1; // ���� ������
}

int Party::GetExpeditionGroupMemberCount( int nGroup )
{
	int nCount = 0;
	for (int i = 0; i < EXPEDITION_MEMBER_PER_GROUP; ++i)
	{
		if (m_ExpeditionGroup[nGroup][i].IsEmpty() == FALSE)
		{
			nCount++;
		}
	}

	return nCount;
}

int Party::GetExpeditionMemberLevel( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_nLevel;
}

int Party::GetExpeditionMemberHp( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_nHP;
}

int Party::GetExpeditionMemberMaxHp( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_nMaxHP;
}

int Party::GetExpeditionMemberMp( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_nMP;
}

int Party::GetExpeditionMemberMaxMp( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_nMaxMP;
}


SLONG Party::GetExpeditionMemberIndex(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_slIndex;
}

SLONG Party::GetExpeditionMemberIndex(CTString strName)
{
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
		{
			if(m_ExpeditionGroup[i][j].m_strName == strName)
				return m_ExpeditionGroup[i][j].m_slIndex;
		}
	}

	return -1;
}

SLONG Party::GetExpeditionMemberZone(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_slZone;
}

FLOAT Party::GetExpeditionMemberPosX(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_fPosX;
}

FLOAT Party::GetExpeditionMemberPosZ(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_fPosZ;
}

SBYTE Party::GetExpeditionMemberLayer(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_sbLayer;
}

SBYTE Party::GetExpeditionMemberJob( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_sbJob;
}

SBYTE Party::GetExpeditionMemberPosition( int nGroup, int nPos )
{
	return m_ExpeditionGroup[nGroup][nPos].m_nPosition;
}

BOOL Party::GetExpeditionMemberOnline(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_bOnline;
}

CTString Party::GetExpeditionMemberName(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_strName;
}

SBYTE Party::GetDivisionType(int nType)
{
	switch(nType)
	{
	case MSG_DIVITYPE_EXP:
		return m_sbDivisionTypeEXP;

	case MSG_DIVITYPE_ITEM:
		return m_sbDivisionTypeITEM;

	case MSG_DIVITYPE_SPECIAL:
		return m_sbDivisionTypeSPITEM;
	}

	return -1;
}

bool Party::IsPartyMember( INDEX iChaIndex )
{
	// [sora] �����뵵 ����� �� �ְ� ���� (�������� ��� �ڱ� �׷��� ��� true)

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn(CSF_PARTY) )
	{
		const int iPartyMemberCount	= GetMemberCount();
		if( iPartyMemberCount > 0 )
		{
			for( int i = 0; i < iPartyMemberCount; ++i )
			{
				LONG lMemberIndex = GetMemberIndex( i );
				if( lMemberIndex == iChaIndex )
				{
					return true;
				}
			}
		}
		return false;
	}
	else if( pUIManager->IsCSFlagOn(CSF_EXPEDITION) )
	{
		int nGroup = 0;
		int nPos = 0;

		if(SearchExpeditionMember(iChaIndex, nGroup, nPos))
		{
			return nGroup == m_nMyGroupNum;
		}
	}

	return false;

}

bool Party::IsOurParty( CTString strName )
{
	for( int i = 0; i < m_nMemberCount; i++ )
	{
		if( m_PartyInfo[i].strName == strName )
		{
			return true;
		}
	}
	return false;
}

int Party::IsOurParty( SLONG slCharIndex )
{
	for( SLONG i = 0; i < m_nMemberCount; i++ )
	{
		if( m_PartyInfo[i].slIndice == slCharIndex )
			return i;
	}

	return -1;
}

int	Party::IsOurPartyPos( CTString strName )
{
	for( SLONG i = 0; i < m_nMemberCount; i++ )
	{
		if(m_PartyInfo[i].strName == strName )
			return i;
	}

	return -1;
}

int Party::IsLabeled(SLONG slTargetType, SLONG slIndex)
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION) == FALSE )
		return -1;

	if(slTargetType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		if(_pNetwork->MyCharacterInfo.index == slIndex)
		{
			return _pNetwork->MyCharacterInfo.slLabel;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slIndex);

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			return pTarget->cha_Label;
		}
	}
	else // mob
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, slIndex);

		if (pObject != NULL)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			return pMT->mob_Label;
		}
	}

	return -1;

}

BOOL Party::IsExistOfflineMember()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
		{
			if(m_PartyInfo[iMem].bOnline == false)
			{
				return TRUE;
			}
		}

		return FALSE;
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
		{
			for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
			{
				if( IsExpedetionDataExist(i,j) && (!GetExpeditionMemberOnline(i,j)) )
				{
					return TRUE;
				}
			}

		}

		return FALSE;
	}

	return TRUE;
}

bool Party::IsExpedetionMember( INDEX iChaIndex )
{
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
		{
			if(GetExpeditionMemberIndex(i, j) == iChaIndex)
			{
				return true;
			}
		}
	}

	return false;
}

bool Party::IsExpedetionDataExist(int nGroup, int nPos)
{
	return (m_ExpeditionGroup[nGroup][nPos].IsEmpty() ? false : true);
}

bool Party::SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos)
{

	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
		{
			if(GetExpeditionMemberIndex(i, j) == slIndex)
			{
				nGroup = i;
				nPos = j;
				return true;
			}
		}
	}

	nGroup = -1;
	nPos = -1;
	return false;
}

///////////////////////////////////Party ����/////////////////////////////////

void Party::AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
	int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
	FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	if( m_nMemberCount >= PARTY_MAX_MEMBER )
		return;

	m_PartyInfo[m_nMemberCount].slIndice = slIndex;
	m_PartyInfo[m_nMemberCount].strName = strName;
	m_PartyInfo[m_nMemberCount].bLeader = static_cast<BOOL>(sbLeader);// FALSE
	m_PartyInfo[m_nMemberCount].sbJob = sbJob;// sbLeader
	m_PartyInfo[m_nMemberCount].nLevel = iLevel;
	m_PartyInfo[m_nMemberCount].nHP = iHP;
	m_PartyInfo[m_nMemberCount].nMaxHP = iMaxHP;
	m_PartyInfo[m_nMemberCount].nMP = iMP;
	m_PartyInfo[m_nMemberCount].nMaxMP = iMaxMP;
	m_PartyInfo[m_nMemberCount].fPosX = fX;
	m_PartyInfo[m_nMemberCount].fPosZ = fZ;
	m_PartyInfo[m_nMemberCount].sbLayer = sbLayer;
	m_PartyInfo[m_nMemberCount].ubBuff = 0;
	m_PartyInfo[m_nMemberCount].slZone = slZone;
	m_PartyInfo[m_nMemberCount].bOnline = true;
	m_nMemberCount++;

	UIMGR()->GetParty()->UpdateParty();
	CUIManager::getSingleton()->GetParty()->open();
}

void Party::RemoveMember( SLONG slIndex )
{
	int iMem;
	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_PartyInfo[iMem].slIndice == slIndex )
			break;
	}

	if( iMem == m_nMemberCount )
		return;

	CTString	strSysMessage;
	strSysMessage.PrintF( _S2( 421, m_PartyInfo[iMem].strName, "%s<��> ��Ƽ���� Ż���ϼ̽��ϴ�." ), m_PartyInfo[iMem].strName );
	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage(strSysMessage );

	m_nMemberCount--;
	int iPos;
	for( iPos = iMem; iPos < m_nMemberCount; iPos++ )
	{
		m_PartyInfo[iPos].slIndice = m_PartyInfo[iPos + 1].slIndice;	
		m_PartyInfo[iPos].slZone = m_PartyInfo[iPos + 1].slZone;	
		m_PartyInfo[iPos].strName = m_PartyInfo[iPos + 1].strName;	
		m_PartyInfo[iPos].bLeader = m_PartyInfo[iPos + 1].bLeader;	
		m_PartyInfo[iPos].sbJob = m_PartyInfo[iPos + 1].sbJob;	
		m_PartyInfo[iPos].nLevel = m_PartyInfo[iPos + 1].nLevel;	
		m_PartyInfo[iPos].nHP = m_PartyInfo[iPos + 1].nHP;		
		m_PartyInfo[iPos].nMaxHP = m_PartyInfo[iPos + 1].nMaxHP;	
		m_PartyInfo[iPos].nMP = m_PartyInfo[iPos + 1].nMP;		
		m_PartyInfo[iPos].nMaxMP = m_PartyInfo[iPos + 1].nMaxMP;	
		m_PartyInfo[iPos].fPosX = m_PartyInfo[iPos + 1].fPosX;	
		m_PartyInfo[iPos].fPosZ = m_PartyInfo[iPos + 1].fPosZ;	
		m_PartyInfo[iPos].sbLayer = m_PartyInfo[iPos + 1].sbLayer;	
		m_PartyInfo[iPos].ubBuff = m_PartyInfo[iPos + 1].ubBuff;	
		m_PartyInfo[iPos].bOnline = m_PartyInfo[iPos + 1].bOnline;	
	}
	m_PartyInfo[iPos].slIndice = -1;
	m_PartyInfo[iPos].strName = CTString( "" );
	m_PartyInfo[iPos].bLeader = FALSE;
	m_PartyInfo[iPos].sbJob = -1;
	m_PartyInfo[iPos].nLevel = 0;
	m_PartyInfo[iPos].nHP = 0;
	m_PartyInfo[iPos].nMaxHP = 0;
	m_PartyInfo[iPos].nMP = 0;
	m_PartyInfo[iPos].nMaxMP = 0;
	m_PartyInfo[iPos].fPosX = 0.0f;
	m_PartyInfo[iPos].fPosZ = 0.0f;
	m_PartyInfo[iPos].sbLayer = 0;
	m_PartyInfo[iPos].ubBuff = 0;
	m_PartyInfo[iPos].bOnline = false;

	UIMGR()->GetParty()->UpdateParty();
}

void Party::PartyEnd()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsPartyRequested = FALSE;

	BreakUpParty();

	pUIManager->GetChattingUI()->AddSysMessage( _S( 220, "��Ƽ�� ��ü�Ǿ����ϴ�." ) );

	if( pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO) == TRUE )
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

void Party::BreakUpParty()
{
	InitParty();
	InitExpedition();
	
	CUIManager* pUIManager = CUIManager::getSingleton();

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY );
	pUIManager->SetCSFlagOff( CSF_EXPEDITION );	// [sora] RAID_SYSTEM

	pUIManager->GetParty()->close();

	// [sora] �й� ����â�� �־��ٸ� �ݾ��ش�
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, _pNetwork->MyCharacterInfo.index);

	if (pObject != NULL)
	{
		if (CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject))
		{
			// ��Ƽ �÷��� ����. ���� ������Ʈ cpp�� ���������� ���� �Ǿ� �־ �ϵ��ڵ���..
			if (pTarget->m_pEntity != NULL)
				pTarget->m_pEntity->SetSecondExtraFlagOff(1L<<2);
		}
	}
}

void Party::PartyInvite(SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName, SBYTE sbPartyTypeItem, SBYTE sbPartyTypeSPItem)
{
	m_bAmILeader = slSrcIndex == _pNetwork->MyCharacterInfo.index;
	m_nType = sbType;
	m_sbDivisionTypeITEM = sbPartyTypeItem;
	m_sbDivisionTypeSPITEM = sbPartyTypeSPItem;

	CUIManager* pUIManager = CUIManager::getSingleton();

	CTString	strMessage;
	if( m_bAmILeader )
	{
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 215, "��Ƽ" ), UMBS_CANCEL, UI_PARTY, MSGCMD_PARTY_INVITE );
		strMessage.PrintF( _S( 216, "%s���� ��Ƽ�� ��û���Դϴ�. ����Ͻðڽ��ϱ�?" ), m_strDestName );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );

		m_bIsPartyRequested = TRUE;
	}
	else
	{
		// rejection bug fix [8/5/2010 rumist]
		if( !g_iRejectParty )
		{
 			CTString strPartyType[3]; 
 			strPartyType[PT_PEACEEVER] = _S(4525, "�յ�й�");
 			strPartyType[PT_SURVIVAL] = m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET]; //_s("�Լ��켱")
 			strPartyType[PT_ATTACK] = m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] ; // _s("������");


			// ���� ��Ƽ Ÿ���� �ִٸ�
			if(sbPartyTypeItem >= 0 && sbPartyTypeSPItem >= 0)
			{
				strMessage.PrintF( _S(4526, "%s�� ��Ƽ(%s)(�����ۺй�:%s, ����Ⱦ����� �й�:%s)��û�� �߽��ϴ�. �����Ͻðڽ��ϱ�?" ), strSrcName, 
					strPartyType[sbType], m_strExpeditionType[sbPartyTypeItem], m_strExpeditionType[sbPartyTypeSPItem]);
			}
			else // ���� ������ ���ٸ�
			{
				strMessage.PrintF( _S(4527, "%s�� ��Ƽ(%s)��û�� �߽��ϴ�. �����Ͻðڽ��ϱ�?" ), strSrcName, strPartyType[sbType]);
			}

			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 215, "��Ƽ" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_ALLOW );
			MsgBoxInfo.AddString( strMessage );
			pUIManager->CreateMessageBox( MsgBoxInfo );
		}
		else// [7/13/2009 rumist] reject invite message.
		{
			SendPartyReject();
			return;
		}
	}


	// Character state flags
	pUIManager->SetCSFlagOn( CSF_PARTY_REQ );
}

void Party::PartyReject( BOOL bLeader )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsPartyRequested = FALSE;

	if( m_bAmILeader && !bLeader )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );

	}
	else if( !m_bAmILeader && bLeader )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_ALLOW );
	}
	// ��Ƽ ��Ī �³��ÿ� �޼��� â ����
	pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );

	pUIManager->GetChattingUI()->AddSysMessage( _S( 219, "��Ƽ ��û�� ��ҵǾ����ϴ�." ) );

	if( m_bAmILeader && m_nMemberCount <= 0 )
		m_bAmILeader = FALSE;

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY_REQ );
}

void Party::PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
	int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
	FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nIsCharPos;
	m_bIsPartyRequested = FALSE;

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY_REQ );
	pUIManager->SetCSFlagOn( CSF_PARTY );

	if( m_bAmILeader )
	{ pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE ); }
	else// ��Ƽ ��Ī���� ���� �޼��� â�� �ݴ´�.
	{ 
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );
		pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );
	}

	if( ( nIsCharPos = IsOurParty( slIndex ) ) != -1 )// �̹� ��Ƽ�� ��� �ִ� ����(����)
	{
		m_PartyInfo[nIsCharPos].slIndice = slIndex;
		m_PartyInfo[nIsCharPos].strName = strName;
		m_PartyInfo[nIsCharPos].bLeader = static_cast<BOOL>(sbLeader);
		m_PartyInfo[nIsCharPos].sbJob = sbJob;
		m_PartyInfo[nIsCharPos].nLevel = iLevel;
		m_PartyInfo[nIsCharPos].nHP = iHP;
		m_PartyInfo[nIsCharPos].nMaxHP = iMaxHP;
		m_PartyInfo[nIsCharPos].nMP = iMP;
		m_PartyInfo[nIsCharPos].nMaxMP = iMaxMP;
		m_PartyInfo[nIsCharPos].fPosX = fX;
		m_PartyInfo[nIsCharPos].fPosZ = fZ;
		m_PartyInfo[nIsCharPos].sbLayer = sbLayer;
		m_PartyInfo[nIsCharPos].ubBuff = 0;
		m_PartyInfo[nIsCharPos].slZone = slZone;

		pUIManager->GetParty()->UpdateParty();
	}
	else	// ��Ƽ�� ���� ����
	{
		AddPartyMember( sbLeader, slIndex, strName, sbJob, sbJob2, iLevel, iHP, iMaxHP, iMP, iMaxMP, fX, fZ, sbLayer, slZone );
	}
	// ���� ���� ��Ƽ�� ������ ���� ������ ���� �Ѵ�.
	SetPartyLeader();
}

void Party::PartyQuit( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		BreakUpParty();
		pUIManager->GetChattingUI()->AddSysMessage( _S( 367, "��Ƽ���� Ż���ϼ̽��ϴ�." ) );
		// [sora] �ν���Ʈ�� �÷������̾��ٸ� ��Ȱ�������� �̵�
		if(pUIManager->IsPlayInZone())
		{
			_pNetwork->RaidInzoneQuitReq();
		}
	}
	else
	{
		RemoveMember( slIndex );
	}
}

void Party::PartyKick( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		BreakUpParty();
		pUIManager->GetChattingUI()->AddSysMessage( _S( 368, "��Ƽ���� ���� �Ǽ̽��ϴ�." ) );
		// [sora] �ν���Ʈ�� �÷������̾��ٸ� ��Ȱ�������� �̵�
		if(pUIManager->IsPlayInZone())
		{
			_pNetwork->RaidInzoneQuitReq();
		}
	}
	else
	{
		RemoveMember( slIndex );
	}
}

void Party::PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
	int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline )
{
	// Find party member
	int iMem;
	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_PartyInfo[iMem].slIndice == slIndex )
			break;
	}

	// Not found
	if( iMem == m_nMemberCount )
		return;

	// Update infos
	m_PartyInfo[iMem].nLevel = iLevel;
	m_PartyInfo[iMem].nHP = iHP;
	m_PartyInfo[iMem].nMaxHP = iMaxHP;
	m_PartyInfo[iMem].nMP = iMP;
	m_PartyInfo[iMem].nMaxMP = iMaxMP;
	m_PartyInfo[iMem].fPosX = fX;
	m_PartyInfo[iMem].fPosZ = fZ;
	m_PartyInfo[iMem].sbLayer = sbLayer;
	m_PartyInfo[iMem].slZone = slZone;
	m_PartyInfo[iMem].bOnline = bOnline;

	UIMGR()->GetParty()->UpdateParty();
}

void Party::PartyError( SBYTE sbError )
{
	CTString	strSysMessage;
	switch( sbError )
	{
	case MSG_PARTY_ERROR_NOTFOUND:
		strSysMessage = _S( 397, "��Ƽ ����� ã�� �� �����ϴ�." );
		break;

	case MSG_PARTY_ERROR_ALREADY_ME:
		strSysMessage = _S( 311, "�̹� ��Ƽ ���Դϴ�." );
		break;

	case MSG_PARTY_ERROR_FULL:
		strSysMessage = _S( 312, "��Ƽ���� ���� á���ϴ�." );
		break;

	case MSG_PARTY_ERROR_ALREADY_TARGET:
		strSysMessage = _S( 396, "����� �̹� ��Ƽ���Դϴ�." );
		break;

	case MSG_PARTY_ERROR_ALREADY_REQUEST:
		strSysMessage = _S( 324, "����� �̹� ��Ƽ ��û���Դϴ�." );
		break;

	case MSG_PARTY_ERROR_DIFF_TYPE:
		strSysMessage = _S( 325, "��Ƽ ������ �ٸ��ϴ�." );
		break;

	case MSG_PARTY_ERROR_NOT_BOSS:
		strSysMessage = _S( 326, "��Ƽ���� �ƴ϶� ������ �����ϴ�." );
		break;

	case MSG_PARTY_ERROR_INVALID_LEVEL:
		strSysMessage = _S( 1653, "������ �����ʾ� ��Ƽ ��û�� �� �� �����ϴ�." ); 
		break;

	case MSG_PARTY_ERROR_NOT_PARTY:
		strSysMessage = _S(2167, "����� ��Ƽ���� �ƴմϴ�." ); 
		break;

	case MSG_PARTY_ERROR_NOT_PARTY_MEMBER:
		strSysMessage = _S(2168, "����� ��Ƽ���� �ƴմϴ�." ); 
		break;

	case MSG_PARTY_ERROR_PVP:
		strSysMessage = _S(3073, "PvP���� ĳ���Ϳʹ� ��Ƽ�� �� �� �����ϴ�" );
		break;

		// [sora] RAID_SYSTEM
	case MSG_PARTY_ERROR_INZONECLEAR_FAIL:
		strSysMessage = _S(4528, "�ν���Ʈ�� �ʱ�ȭ�� �����Ͽ����ϴ�.");
		break;

		// [090728: selo] ���� �޺��� ��Ƽ���� ���� �� �� ����.
	case MSG_PARTY_ERROR_DO_NOT_CHANGE_BOSS:
		strSysMessage = _S(4695, "���� �޺� �߿��� ��Ƽ���� ���� �� �� �����ϴ�.");
		break;
	case MSG_PARTY_ERROR_LEVEL_CHANGE_BOSS:
		strSysMessage = _S(4823, "������ ���� �ʾ� ��Ƽ�� ������ �� �� �����ϴ�.");
		break;
	default:
		strSysMessage = _S(4529, "��Ƽ���� �����Դϴ�.");
		break;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetChattingUI()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

	if( pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_INVITE) == TRUE )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );
	}
}

///////////////////////////////////������ ����//////////////////////////////////

void Party::PartyData2Expedition( SLONG slIndex, int nPos )
{
	stExpeditionMember expMember;

	if(slIndex == _pNetwork->MyCharacterInfo.index) // �ڽ��� ���
	{
		m_nGroupPos = nPos;		
		expMember.m_slZone = _pNetwork->MyCharacterInfo.zoneNo;															
		expMember.m_nLevel = _pNetwork->MyCharacterInfo.level;				
		expMember.m_nHP = _pNetwork->MyCharacterInfo.hp;					
		expMember.m_nMaxHP = _pNetwork->MyCharacterInfo.maxHP;				
		expMember.m_nMP = _pNetwork->MyCharacterInfo.mp;					
		expMember.m_nMaxMP = _pNetwork->MyCharacterInfo.maxMP;				
		expMember.m_fPosX = _pNetwork->MyCharacterInfo.x;				
		expMember.m_fPosZ = _pNetwork->MyCharacterInfo.z;				
		expMember.m_sbLayer = _pNetwork->MyCharacterInfo.yLayer;	
		expMember.m_slIndex = _pNetwork->MyCharacterInfo.index;			
		expMember.m_strName = _pNetwork->MyCharacterInfo.name;	
		expMember.m_nPosition = m_bAmILeader ? MSG_EXPED_MEMBERTYPE_BOSS : MSG_EXPED_MEMBERTYPE_NORMAL;
		expMember.m_sbJob = _pNetwork->MyCharacterInfo.job;	

	}
	else	// �ٸ� ĳ���ϰ��
	{
		int nPartyPos = -1;

		for( SLONG i = 0; i < PARTY_MAX_MEMBER; i++ )
		{
			if( m_PartyInfo[i].slIndice == slIndex )
				nPartyPos = i;
		}

		if(nPartyPos >= 0)
		{
			expMember.m_slZone = m_PartyInfo[nPartyPos].slZone;															
			expMember.m_nLevel = m_PartyInfo[nPartyPos].nLevel;				
			expMember.m_nHP = m_PartyInfo[nPartyPos].nHP;					
			expMember.m_nMaxHP = m_PartyInfo[nPartyPos].nMaxHP;				
			expMember.m_nMP = m_PartyInfo[nPartyPos].nMP;					
			expMember.m_nMaxMP = m_PartyInfo[nPartyPos].nMaxMP;				
			expMember.m_fPosX = m_PartyInfo[nPartyPos].fPosX;				
			expMember.m_fPosZ = m_PartyInfo[nPartyPos].fPosZ;				
			expMember.m_sbLayer = m_PartyInfo[nPartyPos].sbLayer;	
			expMember.m_slIndex = m_PartyInfo[nPartyPos].slIndice;			
			expMember.m_strName = m_PartyInfo[nPartyPos].strName;	
			expMember.m_nPosition = m_PartyInfo[nPartyPos].bLeader ? MSG_EXPED_MEMBERTYPE_BOSS : MSG_EXPED_MEMBERTYPE_NORMAL;;
			expMember.m_sbJob = m_PartyInfo[nPartyPos].sbJob;	
		}
		else
		{
			return;
		}
	}

	expMember.m_bOnline = TRUE;
	expMember.UpdateBarRate();

	AddExpeditionMember(0, nPos, expMember);
}

void Party::ExpeditionReject( BOOL bIsSrc )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(bIsSrc)
	{
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_ALLOW);
	}
	else
	{
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_INVITE);
	}

	pUIManager->GetChattingUI()->AddSysMessage( _S( 4564, "������ ��û�� ��ҵǾ����ϴ�." ) );

	pUIManager->SetCSFlagOff(CSF_EXPEDITION_REQ);
}

void Party::ExpeditionQuit( SLONG slType, SLONG slIndex )
{
	if(slType == MSG_EXPED_QUITMODE_NORMAL)	// �������� Ż���� ���
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			ExpeditionEnd();
			pUIManager->GetChattingUI()->AddSysMessage( _S( 4565, "�����뿡�� Ż���ϼ̽��ϴ�." ) );
			// [sora] �ν���Ʈ�� �÷������̾��ٸ� ��Ȱ�������� �̵�
			if(pUIManager->IsPlayInZone())
			{
				_pNetwork->RaidInzoneQuitReq();
			}
		}
		else
		{
			int nGroup = 0;
			int nPos = 0;

			if (SearchExpeditionMember(slIndex, nGroup, nPos) == true)
			{
				CTString	strSysMessage;
				strSysMessage.PrintF( _S( 4566, "%s�� �����뿡�� Ż���ϼ̽��ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
				pUIManager->GetChattingUI()->AddSysMessage(strSysMessage );

				RemoveExpeditionMember(nGroup, nPos);
			}
		}
	}
	else if(slType == MSG_EXPED_QUITMODE_UNUSUAL)	// ���������� Ż���� ���
	{
		ExpeditionMemberOnline(slIndex, FALSE);
	}

}

void Party::ExpeditionKick( SLONG slIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 4687, "�����뿡�� ����Ǽ̽��ϴ�." ) );
		ExpeditionEnd();
		// [sora] �ν���Ʈ�� �÷������̾��ٸ� ��Ȱ�������� �̵�
		if(pUIManager->IsPlayInZone())
		{
			_pNetwork->RaidInzoneQuitReq();
		}
	}
	else
	{
		int nGroup = 0;
		int nPos = 0;

		if (SearchExpeditionMember(slIndex, nGroup, nPos) == true)
		{
			CTString	strSysMessage;
			strSysMessage.PrintF( _S( 4566, "%s�� �����뿡�� Ż���ϼ̽��ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
			pUIManager->GetChattingUI()->AddSysMessage(strSysMessage );

			RemoveExpeditionMember(nGroup, nPos);
		}
	}
}

void Party::ExpeditionEnd()
{
	InitExpedition();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff( CSF_EXPEDITION );
	pUIManager->SetCSFlagOff( CSF_PARTY );

	pUIManager->GetParty()->close();

	// �й� ����â�� �־��ٸ� �ݾ��ش�
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);

	if (pUIManager->GetExpedition()->IsVisible() != FALSE)
		pUIManager->GetExpedition()->closeUI();

	if (pUIManager->GetExpeditionView()->IsVisible() != FALSE)
		pUIManager->GetExpeditionView()->closeUI();
}

void Party::ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum) // ĳ���� �ε���, �̵��� �׷�, ��ġ
{

	int nGroup = 0;	// ���� �׷�
	int nPos = 0;	// ���� ��ġ

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		stExpeditionMember expMember = m_ExpeditionGroup[nGroup][nPos];
		RemoveExpeditionMember(nGroup, nPos);
		AddExpeditionMember(nGroupNum, nPosNum, expMember);

		CTString strMessage;
		if(nGroup != nGroupNum)
		{
			strMessage.PrintF(_S( 4567, "%s�� %d�׷� %d������ %d�׷� %d������ �̵��Ͽ����ϴ�." ), m_ExpeditionGroup[nGroupNum][nPosNum].m_strName, nGroup+1, nPos+1, nGroupNum+1, nPosNum+1);	//�׷�� ǥ���Ҷ��� +1(0~3) -> (1~4)
		}
		else
		{
			strMessage.PrintF(_S( 4568, "%s�� %d�׷� %d������ %d������ �̵��Ͽ����ϴ�." ), m_ExpeditionGroup[nGroupNum][nPosNum].m_strName, nGroupNum+1, nPos+1, nPosNum+1);	//�׷�� ǥ���Ҷ��� +1(0~3) -> (1~4)
		}

		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strMessage );
	}

	UIMGR()->GetExpedition()->SetBtnMove(true);
}

void Party::AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2, int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	stExpeditionMember expMember;
	expMember.m_slZone = slZone;															
	expMember.m_nLevel = iLevel;				
	expMember.m_nHP = iHP;					
	expMember.m_nMaxHP = iMaxHP;				
	expMember.m_nMP = iMP;					
	expMember.m_nMaxMP = iMaxMP;				
	expMember.m_fPosX = fX;				
	expMember.m_fPosZ = fZ;				
	expMember.m_sbLayer = sbLayer;	
	expMember.m_slIndex = slIndex;			
	expMember.m_strName = strName;	
	expMember.m_nPosition = nPosition;
	expMember.m_sbJob = sbJob;	
	expMember.m_bOnline = TRUE;
	expMember.UpdateBarRate();

	AddExpeditionMember(nGroupNum, nPos, expMember);
}

void Party::AddExpeditionMember(int nGroupNum, int nPos, stExpeditionMember expMember)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_INVITE))
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_INVITE);

	// ������ 0�̸� �������� ���
	if(expMember.m_nLevel == 0)
	{
		expMember.m_bOnline = FALSE;
	}

	m_ExpeditionGroup[nGroupNum][nPos] = expMember;

	if(expMember.m_slIndex == _pNetwork->MyCharacterInfo.index)
	{
		m_nMyGroupNum = nGroupNum;
		m_nGroupPos = nPos;

		int nPosX, nPosY;
		pUIManager->GetChattingUI()->GetChatWindowEndPos(nPosX, nPosY);
	}

	m_PartyInfo[nPos].ubBuff = 0;

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
		pUIManager->SetCSFlagOff( CSF_EXPEDITION_REQ );

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
		pUIManager->SetCSFlagOff( CSF_PARTY );

	pUIManager->SetCSFlagOn(CSF_EXPEDITION);

	pUIManager->GetExpedition()->UpdateUI();
	pUIManager->GetParty()->OpenExpeditionMemberWindow();
	

	if(m_nMyGroupNum >= 0)
	{
		if(nGroupNum == m_nMyGroupNum)
		{
			pUIManager->GetParty()->open();
			pUIManager->GetParty()->UpdateExpedition();
		}
	}

	// ���ο� ����� ������ ��Ƽâ�� �����ش�.
	if(!pUIManager->IsUIVisible(UI_PARTY))
		pUIManager->GetParty()->open();

}

void Party::RemoveExpeditionMember( int nGroup, int nPos )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_ExpeditionGroup[nGroup][nPos].Init();	

	if(nGroup == m_nMyGroupNum)
	{
		m_nMemberCount--;
		m_PartyInfo[nPos].ubBuff = 0;
		pUIManager->GetParty()->UpdateExpedition();
	}

	pUIManager->GetExpedition()->UpdateUI();
	pUIManager->GetParty()->OpenExpeditionMemberWindow();
}

void Party::ExpeditionMemberInfo(SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP, int iMP,
	int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	for(int i = 0; i < EXPEDITION_MEMBER_PER_GROUP; i++)
	{
		if(GetExpeditionMemberIndex(slGroup, i) == slIndex)
		{
			if(bOnline == true)
			{
				if(m_ExpeditionGroup[slGroup][i].m_bOnline == FALSE)
				{
					m_ExpeditionGroup[slGroup][i].m_bOnline = TRUE;
				}

				BOOL bIsLevelChanged = FALSE;
				m_ExpeditionGroup[slGroup][i].m_slZone = slZone;		
				if(m_ExpeditionGroup[slGroup][i].m_nLevel != iLevel)
					bIsLevelChanged = TRUE;

				m_ExpeditionGroup[slGroup][i].m_nLevel = iLevel;				
				m_ExpeditionGroup[slGroup][i].m_nHP = iHP;					
				m_ExpeditionGroup[slGroup][i].m_nMaxHP = iMaxHP;				
				m_ExpeditionGroup[slGroup][i].m_nMP = iMP;					
				m_ExpeditionGroup[slGroup][i].m_nMaxMP = iMaxMP;				
				m_ExpeditionGroup[slGroup][i].m_fPosX = fX;				
				m_ExpeditionGroup[slGroup][i].m_fPosZ = fZ;				
				m_ExpeditionGroup[slGroup][i].m_sbLayer = sbLayer;		
				m_ExpeditionGroup[slGroup][i].UpdateBarRate();

				if(bIsLevelChanged)
				{
					// ������ �ö��ٸ� ���, ����â�� ������ ������Ʈ�Ͽ� �ش�
					pUIManager->GetExpedition()->UpdateUI();
					pUIManager->GetParty()->UpdateExpedition();
					pUIManager->GetParty()->OpenExpeditionMemberWindow();
				}

				pUIManager->GetExpedition()->UpdateMemberInfo(slGroup, i);
				pUIManager->GetParty()->UpdateGroupMemberInfo(slGroup, i);

				if (m_nMyGroupNum == slGroup && slIndex != _pNetwork->MyCharacterInfo.index)
				{
					int nRealIdx = 0;
					for (int j = 0; j < i; ++j)
					{
						if (GetExpeditionMemberIndex(slGroup, j) == _pNetwork->MyCharacterInfo.index)
							continue;

						if (IsExpedetionDataExist(slGroup, j) == true)
						{
							nRealIdx++;
						}
					}
					pUIManager->GetParty()->UpdateMemberInfo(i, true, nRealIdx);
				}
			}
			else // ��������
			{
				m_ExpeditionGroup[slGroup][i].m_bOnline = FALSE;
				pUIManager->GetExpedition()->UpdateUI();
				pUIManager->GetParty()->UpdateExpedition();
				pUIManager->GetParty()->OpenExpeditionMemberWindow();
			}
			break;
		}
	}
}

void Party::ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline )
{
	int nGroup = 0; 
	int nPos = 0;

	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		ExpeditionEnd();
		return;
	}

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_ExpeditionGroup[nGroup][nPos].m_bOnline = bOnline;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetParty()->UpdateExpedition();
	pUIManager->GetExpedition()->UpdateUI();
	pUIManager->GetParty()->OpenExpeditionMemberWindow();
}

void Party::SetExpeditionPosition(SLONG slIndex, SLONG slPosition)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_ExpeditionGroup[nGroup][nPos].m_nPosition = slPosition;

		if(slPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
		{
			CUIMsgBox_Info	MsgBoxInfo;
			MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "������" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

			CTString strMessage;

			strMessage.PrintF(_S( 4572, "%s���� �����δ����� �Ǿ����ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);

			MsgBoxInfo.AddString( strMessage );
			CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		}
		else if(slPosition == MSG_EXPED_MEMBERTYPE_NORMAL)
		{	// �Ϲݴ������ ��ȯ �����
			// ������ ȹ��
			int slMyPosition = GetExpeditionMyPosition();
			int slMyGroup, slMyPos;
			SearchExpeditionMember(_pNetwork->MyCharacterInfo.index ,slMyGroup, slMyPos);

			if(slMyGroup == nGroup && slMyPos == nPos)
			{
				// �� �ڽ��� �Ϲݴ������ ����Ǿ��� ������ ����â�� �����ִٸ� �ݾ���
				UIMGR()->GetExpedition()->closeUI();
			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->GetParty()->UpdateExpedition();
	pUIManager->GetExpedition()->UpdateUI();
	pUIManager->GetParty()->OpenExpeditionMemberWindow();
}

void Party::ChangeExpeditionLeader(SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode )
{
	int nGroup = 0; 
	int nPos = 0;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( SearchExpeditionMember(slIndexDesc, nGroup, nPos) == false)
		return;

	SetExpeditionPosition(slIndexSrc, MSG_EXPED_MEMBERTYPE_NORMAL);
	SetExpeditionPosition(slIndexDesc, MSG_EXPED_MEMBERTYPE_BOSS);

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "������" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	CTString strMessage;

	strMessage.PrintF(_S( 4573, "%s���� ���������� �Ǿ����ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);

	MsgBoxInfo.AddString( strMessage );
	pUIManager->CreateMessageBox( MsgBoxInfo );
	pUIManager->GetParty()->UpdateExpedition();
	pUIManager->GetParty()->OpenExpeditionMemberWindow();
		
	if(UIMGR()->GetExpedition()->IsVisible()) // �ڱⰡ ���������� �����Ͽ��ٸ� ����â�� �ݾ��ش�.
	{
		UIMGR()->GetExpedition()->closeUI();
	}
}
///////////////////////////// NETWORK /////////////////////////////////////////////

void Party::SendPartyAllow()
{
	_pNetwork->PartyAllow();
}

void Party::SendPartyReject()
{
	_pNetwork->PartyReject();
}

void Party::SendExpeditionReject()
{
	_pNetwork->ExpeditionRejectReq();
}

void Party::SendExpeditionAllow()
{
	_pNetwork->ExpeditionAllowReq();
}

void Party::SendPartyInvite( SBYTE sbType, CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SLONG	slIndex = -1;	
	SBYTE	sbShopType = PST_NOSHOP;
	INDEX	iSyndicateType;

	ObjInfo* pInfo = ObjInfo::getSingleton();

	// Find index of character
	if (pInfo->GetTargetEntity(eTARGET) != NULL)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pInfo->GetTargetServerIdx(eTARGET));

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			sbShopType = pTarget->cha_sbShopType;
			slIndex = pTarget->m_nIdxServer;
			iSyndicateType = pTarget->cha_iSyndicateType;
		}
	}

	if( sbShopType != PST_NOSHOP )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 940, "���� �������� ��Ƽ�� �� �� �����ϴ�." ) , SYSMSG_ERROR );	
		return;
	}

	SendPartyInviteReq(sbType, slIndex, strDestName);
}

void Party::SendPartyInviteReq( SBYTE sbType, SLONG	slIndex, CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If I'm not a leader
	if( m_nMemberCount > 0 && !m_bAmILeader )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 364, "��Ƽ ��û ������ �����ϴ�" ), SYSMSG_ERROR );
		return;
	}

	// If party is already requested
	CTString	strMessage;
	if( m_bIsPartyRequested )
	{
		strMessage.PrintF( _S( 361, "�̹� %s���� ��Ƽ�� ��û���Դϴ�." ), m_strDestName );
		pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	if( m_nMemberCount > 0 && sbType != m_nType )
	{
		// If party type is different
		switch( m_nType )
		{
		case PT_PEACEEVER:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 362, "�̹� �յ� �й� ��Ƽ���Դϴ�." ), SYSMSG_ERROR );
			break;
		case PT_SURVIVAL:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 363, "�̹� �Լ� �켱 ��Ƽ���Դϴ�." ), SYSMSG_ERROR );
			break;
		case PT_ATTACK:
			pUIManager->GetChattingUI()->AddSysMessage( _S( 1651,  "�̹� ������ ��Ƽ���Դϴ�."  ), SYSMSG_ERROR ); 
			break;
		}
		return;
	}

	// If target is already my party member
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( slIndex == m_PartyInfo[iMem].slIndice )
		{
			strMessage.PrintF( _S2( 420, m_strDestName, "�̹� %s<��> ��Ƽ���Դϴ�." ), m_strDestName );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );

			return;
		}
	}

	_pNetwork->PartyInvite( sbType, slIndex, strDestName );
	m_strDestName = strDestName;
}

void Party::SendPartyQuit()
{
	if( m_nMemberCount > 0 )
		_pNetwork->PartyQuit();
}

void Party::SendPartyKick( CTString &strDestName )
{
	if( !m_bAmILeader )
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 365, "��Ƽ�� ���� ������ �����ϴ�." ), SYSMSG_ERROR );
		return;
	}

	int nIndex = IsOurPartyPos( strDestName );

	// If character doesn't exist
	if( nIndex == -1 )
	{
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 366, "���� ��븦 ã�� �� �����ϴ�." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyKick( m_PartyInfo[nIndex].slIndice );
}

void Party::SendPartyEnd()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_PARTY) == FALSE )
		return;

	_pNetwork->PartyEndReq();
}

void Party::SendInitInZone()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_PARTY) == FALSE)
		return;

	_pNetwork->InitInZoneReq();
}

void Party::SendExpeditionViewDetail(SLONG slIndex, CTString strCharName)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		SBYTE sbJob = GetExpeditionMemberJob(nGroup, nPos);

		UIMGR()->GetExpeditionView()->ClearViewDetail();
		UIMGR()->GetExpeditionView()->SetCharInfo(strCharName, sbJob);
		_pNetwork->ExpeditionViewDetailReq(nGroup, slIndex);
	}
}

void Party::SendChangeSubLeader(SLONG slIndex)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
		{
			if(m_ExpeditionGroup[nGroup][i].m_nPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
			{
				// �� �׷쿡 �����δ����� 1��
				pUIManager->GetChattingUI()->AddSysMessage( _S( 4574, "�׷쿡 �̹� �����δ����� �ֽ��ϴ�. �δ����Ӹ��� �����ϰ� �ٽ� �õ��� �ֽʽÿ�." ) );
				return;
			}
		}

		if(m_ExpeditionGroup[nGroup][nPos].m_nPosition == MSG_EXPED_MEMBERTYPE_BOSS)
		{
			// ���������� �δ����� �� �� ����
			pUIManager->GetChattingUI()->AddSysMessage( _S( 4575, "���������� �δ����� �� �� �����ϴ�." ) );
			return;
		}
	}

	_pNetwork->ExpeditionChangeSubLeaderReq(TRUE, slIndex);
}

void Party::SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex)
{
	_pNetwork->ExpeditionSetLabelReq( sbTargetType, MSG_EXPED_SETLABEL_MODE_SET, slLabelIndex, slIndex);
}

void Party::SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex)
{
	SLONG slLabel = -1;

	if( sbTargetType == CHARACTER )
	{
		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			slLabel = _pNetwork->MyCharacterInfo.slLabel;
		}
		else
		{
			ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, slIndex);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				slLabel = pTarget->cha_Label;
			}
		}

	}
	else //sbTargetType == MOB
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_MOB, slIndex);

		if (pObject != NULL)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			slLabel = pMT->mob_Label;
		}
	}

	if(slLabel >= 0)
	{
		_pNetwork->ExpeditionSetLabelReq( sbTargetType, MSG_EXPED_SETLABEL_MODE_RESET, slLabel, slIndex);	
	}
}

void Party::SendChangeExpedition()
{
	// [100311 sora : ���� �޺������� ������ ��ȯ �Ұ�]
	const int comboZone = 22;
	BOOL partyIsComboZone = FALSE;


	// ��Ƽ���� �޺����� �������
	if( comboZone == _pNetwork->MyCharacterInfo.zoneNo )
	{
		partyIsComboZone = TRUE;
	}

	// ��Ƽ���� �޺����� ���� ���
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( comboZone == m_PartyInfo[iMem].slZone )
		{	
			partyIsComboZone = TRUE;
			break;
		}
	}

	if( partyIsComboZone )
	{
		CUIMsgBox_Info MsgBoxInfo;

		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_EXPEDITION_ERROR );
		MsgBoxInfo.AddString(_S( 4825, "���� �޺� �߿��� ������ ��ȯ�� �� �� �����ϴ�." ));
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
		return;
	}

	_pNetwork->ExpeditionCreateReq();
}

void Party::SendExpeditionInvite( CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();
	// Find index of character
	SLONG	slIndex = -1;
	if (pInfo->GetTargetEntity(eTARGET) != NULL)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(eOBJ_CHARACTER, pInfo->GetTargetServerIdx(eTARGET));

		if (pObject != NULL)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if ( pTarget->cha_sbShopType != PST_NOSHOP )
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S(4522, "���� �������� ĳ���ʹ� �����뿡 ������ �� �����ϴ�." ) , SYSMSG_ERROR );	
				return;
			}

			slIndex = pTarget->m_nIdxServer;
		}
	}
	SendExpeditionInviteReq(slIndex, strDestName);
}

void Party::SendExpeditionInviteReq(int slIndex, CTString &strDestName)
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	ObjInfo* pInfo = ObjInfo::getSingleton();

	// ��������� �ʴ� �Ұ�
	if( GetExpeditionMyPosition() == MSG_EXPED_MEMBERTYPE_NORMAL)
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 4531, "������ ��û ������ �����ϴ�" ), SYSMSG_ERROR );
		return;
	}

	CTString	strMessage;

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
	{
		strMessage.PrintF( _S( 4521, "�̹� %s���� �����븦 ��û���Դϴ�." ), m_strDestName );
		pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	// Find index of character
	if( slIndex == -1 )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S(4523, "����� ã�� �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}

	if(IsExpedetionMember(slIndex))
	{
		strMessage.PrintF( _S( 4524, "�̹� %s�� ������ �����Դϴ�." ), strDestName );
		pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	_pNetwork->ExpeditionInviteReq(slIndex);
	m_strDestName = strDestName;
}

void Party::SendExpeditionQuit()
{
	_pNetwork->ExpeditionQuitReq();
}

void Party::SendExpeditionKick( SLONG slIndex )
{
	if(!IsExpedetionMember(slIndex))
		return;

	_pNetwork->ExpeditionKickReq(slIndex);
}
										
void Party::SendExpeditionEnd()
{
	_pNetwork->ExpeditionEndReq();
}
													
void Party::SendDivisionType(SBYTE sbDivisionType, SBYTE sbType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_PARTY_REQ ) || pUIManager->IsCSFlagOn( CSF_EXPEDITION_REQ ) )
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 4798, "��Ƽ�� ������ ��û�߿��� ������ �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}
	else if(IsExistOfflineMember())
	{
		pUIManager->GetChattingUI()->AddSysMessage( _S( 4799,"�������� ����� �־ ������ �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}

	UBYTE msgType = 0;
	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		msgType = MSG_EXPEDITION;

		_pNetwork->ExpeditionChangeDivisionTypeReq(msgType, sbType, sbDivisionType);
	}
	else if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		CNetworkMessage	nmMessage( MSG_PARTY );
		nmMessage << (SBYTE)MSG_PARTY_CHANGETYPE;
		nmMessage << sbType;
		nmMessage << sbDivisionType;
		nmMessage << (sbDivisionType >= 0 ? (SBYTE)MSG_PARTY_SETALLONE_ONE : (SBYTE)MSG_PARTY_SETALLONE_ALL);
		// �й����� -1�̸� ��ü ���� �׿ܴ� �����׸�����

		_pNetwork->SendToServerNew( nmMessage );
	}
	else
	{
		return;
	}
}
															
void Party::SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum)
{
	SLONG slIndex = m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_slIndex;

	if(m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_nPosition == MSG_EXPED_MEMBERTYPE_BOSS || m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_nPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
	{
		UIMGR()->GetChattingUI()->AddSysMessage( _S( 4532,"��������� �����δ����� �׷��� �̵��� �� �����ϴ�.") );
		// ��ư ��� �Ұ� ����
		UIMGR()->GetExpedition()->SetBtnMove(true);
		return;
	}

	_pNetwork->ExpeditionChangeGroupReq(nSrcGroupNum, slIndex, nDestGroupNum, nDestPosNum);
}

//////////////////////////////RECEIVE/////////////////////////////////

void Party::RecvChangeExpedition( CNetworkMessage *istr )
{

	SBYTE sbDivisionTypeEXP;												
	SBYTE sbDivisionTypeITEM;	
	SBYTE sbDivisionTypeSPITEM;
	SLONG slLeaderIndex;
	CTString strLeaderName;
	SLONG	 nMemberCount;

	SLONG slIndex;
	CTString strName;
	SLONG	nGroupType;
	SLONG nMemberType;

	(*istr) >> sbDivisionTypeEXP  >> sbDivisionTypeITEM >> sbDivisionTypeSPITEM;
	(*istr) >> slLeaderIndex;
	(*istr) >> strLeaderName;
	(*istr) >> nMemberCount;

	m_nMyGroupNum = 0;	//��Ƽ->������� ��ȯ�ÿ��� �׻� 1�� �׷�

	PartyData2Expedition(slLeaderIndex, 0);

	for(int i=0; i<nMemberCount; i++)
	{
		(*istr) >> slIndex;
		(*istr) >> strName;
		(*istr) >> nGroupType;
		(*istr) >> nMemberType;
		PartyData2Expedition(slIndex, i+1);
	}


	Init();

	// ������ ��ü �ع��� - 1���� m_nMemberCount���̴�. (nMemberCount�� ���������� ������ ��ü ������̹Ƿ� ���� ����)
	m_nMemberCount = nMemberCount;

	SetDivisionType(sbDivisionTypeEXP, sbDivisionTypeITEM, sbDivisionTypeSPITEM);

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff(CSF_PARTY);
	pUIManager->SetCSFlagOn(CSF_EXPEDITION);

	pUIManager->GetChattingUI()->AddSysMessage( _S( 4530, "��Ƽ�� ������� ��ȯ�Ǿ����ϴ�." ) );

	// ����â�� ������Ʈ���� �����Ƿ� �ݾ��ش�.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

void Party::RecvExpeditionInvite( CNetworkMessage *istr )
{

	SBYTE sbDivisionTypeEXP;												
	SBYTE sbDivisionTypeITEM;						
	SBYTE sbDivisionTypeSPITEM;
	SLONG slIndex;
	CTString strName;

	CUIMsgBox_Info	MsgBoxInfo;
	CTString	strMessage;

	(*istr) >> sbDivisionTypeEXP >> sbDivisionTypeITEM >> sbDivisionTypeSPITEM;
	(*istr) >> slIndex;
	(*istr) >> strName;

	SetDivisionType(sbDivisionTypeEXP, sbDivisionTypeITEM, sbDivisionTypeSPITEM);

	CUIManager* pUIManager = CUIManager::getSingleton();

	// ��û�� ����� ���
	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "������" ), UMBS_CANCEL, UI_PARTY, MSGCMD_EXPEDITION_INVITE );
		strMessage.PrintF( _S( 4533, "%s���� �����븦 ��û���Դϴ�. ����Ͻðڽ��ϱ�?" ), m_strDestName );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else // ��û�޴� ����� ���
	{
		// BUGFIX : ITS(#2544) ������ �Ӽ� ������ �ٸ� �ο� �ʴ�� �Ӽ��� ǥ�� ����  [6/10/2011 rumist]
 		strMessage.PrintF(_S(4534, "%s�� ������(����ġ:%s, �Ϲݾ�����:%s, ����Ⱦ�����:%s) ��û�� �߽��ϴ�. �����Ͻðڽ��ϱ�?"),
 			strName, m_strExpeditionType[sbDivisionTypeEXP], m_strExpeditionType[sbDivisionTypeITEM],
 			m_strExpeditionType[sbDivisionTypeSPITEM]);

		MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "������" ), UMBS_YESNO, UI_PARTY, MSGCMD_EXPEDITION_ALLOW );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}

	pUIManager->SetCSFlagOn(CSF_EXPEDITION_REQ);

}

void Party::RecvExpeditionError(CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SBYTE	sbType;
	CTString strError;
	BOOL	bShowMsgBox = TRUE;	// �޽��� �ڽ� ��� ����

	(*istr) >> sbType;

	switch(sbType)
	{
	case MSG_EXPED_ERROR_NORMAL:     // �Ϲ�
		{
			strError = _S(4535, "������ ����");
		}
		break;

	case MSG_EXPED_ERROR_ALREADY_REQUEST:  // �ٸ� ������� �̹� ��û ��
		{
			strError.PrintF(_S( 4536, "�̹� %s���� �����븦 ��û���Դϴ�." ), m_strDestName);
		}
		break;

	case MSG_EXPED_ERROR_INVITE_PVP:    // ��û�ڳ� ����ڰ� PVP ��
		{
			strError = _S(4537, "�����뿡 ������ �� ���� �����Դϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_NOT_PARTY:    // ��Ƽ�� �Ҽӵ��� ����(��Ƽ ��ȯ �Ұ�)
		{
			strError = _S(4538, "��Ƽ�� �����ϰ� ���� �ʽ��ϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_NOT_EXPED:    // ������ �Ҽӵ��� ����
		{
			strError = _S(4539, "�����뿡 �����ϰ� ���� �ʽ��ϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_NOT_PARTYBOSS:   // ��Ƽ���� �ƴϴ�. (��Ƽ��ȯ �Ұ�) 
		{
			strError = _S(4540, "��Ƽ�常 ������� ��ȯ�� �����մϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_NOT_EXPEDBOSS:   // ���������� �ƴϴ�.
		{
			strError = _S(4541, "���������� �ƴմϴ�.");
		}
		break;

	case MSG_EXPED_ERROR_ALREADY_JOIN_ME:  // �̹� �����뿡 �ҼӵǾ� ����
		{
			strError = _S(4542, "�̹� �����뿡 �������Դϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_ALREADY_JOIN_OTHER:  // �ٸ� �����뿡 �ҼӵǾ� ����
		{
			strError = _S(4543, "�ٸ� �׷쿡 �������Դϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_NOT_BE_CHAR:   // ���� ĳ���� 
		{
			strError = _S(4544, "�������� �ʴ� ĳ�����Դϴ�.");
		}
		break;		 

	case MSG_EXPED_ERROR_FULL_EXPED:    // �� �̻� ������� �߰��� �� ����) 
		{
			strError = _S(4545, "��������� ���� á���ϴ�.");
		}
		break;		 	 

	case MSG_EXPED_ERROR_INVALID_LEVEL:   // ������ ���� ����
		{
			strError = _S(4546, "������ ���� �ʽ��ϴ�.");
		}
		break;

	case MSG_EXPED_ERROR_CREATE_INVALIDZONE:
		{
			strError = _S(4547, "������� ��ȯ�� �� ���� �����Դϴ�.");
		}
		break;

	case MSG_EXPED_ERROR_SETLABEL_NOTINZONE:
		{
			strError = _S(4548, "ǥ���� �ν���Ʈ ���� ���ο����� ������ �� �ֽ��ϴ�.");
		}
		break;

	case MSG_EXPED_ERROR_EXIST_LOGOUT_MEMBER:
		{
			strError = _S( 4799,"�������� ����� �־ ������ �� �����ϴ�." );
		}
		break;

	default:
		{
			bShowMsgBox = FALSE;	// �޽��� �ڽ��� ����� �ʴ´�
			pUIManager->GetChattingUI()->AddSysMessage( _S( 4535, "������ ����" ) );
		}
		break;

	}

	if(bShowMsgBox)
	{
		CUIMsgBox_Info MsgBoxInfo;

		MsgBoxInfo.SetMsgBoxInfo(_S(4493, "������"), UMBS_OK, UI_NONE, MSGCMD_EXPEDITION_ERROR);
		MsgBoxInfo.AddString(strError);
		pUIManager->CreateMessageBox(MsgBoxInfo);
	}
}

void Party::RecvRaidError( CNetworkMessage *istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff(CSF_TELEPORT);

	ULONG	ulType;
	CTString strMessage;
	BOOL	bShowMsgBox = TRUE;	// �޽��� �ڽ� ��� ����

	(*istr) >> ulType;

	switch(ulType)
	{
	case MSG_RAID_ERROR_INZONE_JOIN_CLEAR_COMPLETE: // ���� ���� �Ұ�(���� Ŭ���� �Ϸ�) - %�� %�� �ڿ� ���尡��
		{
			SLONG slDay, slHour;

			(*istr) >> slDay;
			(*istr) >> slHour;

			strMessage = _S(4549, "�ν���Ʈ ���� ������ �Ұ����մϴ�.");
			CTString strTemp;
			if(slDay >= 0 && slHour >= 0)
			{
				strTemp.PrintF(_S(4550, "%d�� %d�ð� �ڿ� ������ �����մϴ�."), slDay, slHour);
			}
			else // ������ �ν���Ʈ ���� �ʱ�ȭ�� �Ǿ����� ����
			{
				strTemp.PrintF(_S(4551, "�ν���Ʈ ���� �ʱ�ȭ �� �ٽ� �õ��Ͽ� �ֽñ� �ٶ��ϴ�."));
			}

			strMessage = strMessage + CTString(" ") + strTemp;

		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_ROOMCOUNT_FULL:
		{
			strMessage = _S(4552, "���� ������ �ν���Ʈ ���� �ʹ� ���� �� �̻� ������ �� �����ϴ�. ��� �� �ٽ� �õ����ֽñ� �ٶ��ϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_MEMBER_FULL:
		{
			strMessage = _S(4553, "�ν���Ʈ ���� ���� ������ �ִ� �ο����� �ʰ��Ͽ� ������ �� �����ϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_THISZONE_PARTYZONE:
		{
			strMessage = _S(4554, "��Ƽ ���¿����� ������ �����մϴ�. ��Ƽ�� ��ȯ �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_THISZONE_EXPEDZONE:
		{
			strMessage = _S(4555, "������ ���¿����� ������ �����մϴ�. ������� ��ȯ �� �ٽ� �õ��� �ֽñ� �ٶ��ϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_ALREADY:
		{
			strMessage = _S(4556, "�̹� �ν���Ʈ ���� ������ �����Դϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_ALREADY_OTHERTEAM: // �ش� ���� �ٸ� ��Ƽ,�����밡 �ֽ��ϴ�.
		{
			if(pUIManager->IsCSFlagOn(CSF_PARTY))
			{
				strMessage = _S(4557, "�̹� �ٸ� ��Ƽ�� �ν���Ʈ ���� ������ �ֽ��ϴ�.");
			}
			else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
			{
				strMessage = _S(4558, "�̹� �ٸ� �����밡 �ν���Ʈ ���� ������ �ֽ��ϴ�.");
			}
		}
		break;

	case MSG_RAID_ERROR_SUCESS:
		{
			strMessage = _S(4559, "�ν���Ʈ �� ����");
		}
		break;

	case MSG_RAID_ERROR_INZONE_JOIN_NOTJOIN_PARTYOREXPED:
		{
			strMessage = _S(4560, "��Ƽ�� �����뿡 ���ԵǾ� ���� �ʽ��ϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_QUIT_NOTINZONEJOINED:	// ������ ������ ���� ���¿��� ���� ������ ��û�� �Ұ��..
		{
			strMessage = _S(4561, "�ν���Ʈ���� ������ ���°� �ƴմϴ�.");	
		}
		break;

	case MSG_RAID_ERROR_INZONE_NOT_5MIN_GETBOX:  // �� �̻� ���� �ֿ� �� ���� 
		{
			strMessage = _S(4562, "���̻� ���ڸ� �ֿ� �� �����ϴ�.");
		}
		break;		 

	case MSG_RAID_ERROR_INZONE_ALREADY_GETBOX:         // ������ ���� ���� ���� �ϰ� �ٽ� �õ�
		{
			strMessage = _S(4563, "������ ���� ���ڸ� ���� �ٽ� �õ��� �ֽʽÿ�.");
		}
		break;
	case MSG_RAID_ERROR_INZONE_JOIN_INVALIDELEVEL:         // [090729 sora] ������������ ������ �� ����
		{
			strMessage = _S(4546, "������ ���� �ʽ��ϴ�.");
		}
		break;	

	case MSG_RAID_ERROR_INZONE_JOIN_NOTSAMEBOSSROOMNO:		// [090813: selo] �ͼӵ� ���� ������ �������� �޶� ���� �Ұ�
		{
			strMessage = _S(4697, "���������� �ν���Ʈ��� �� ������ �޶� ������ �Ұ����մϴ�. ���������� �����ϱ� ���ؼ� ���������� �Ǿ� ������ �õ��ϰų�, �ͼ� ��� ������� �Բ� ������ �õ��ؾ� �մϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_NOT_EXPED_RAID_SUBNUMBER:
		{
			strMessage = _S(4760,"�ν���Ʈ ���� ���尡���� ä���� �ƴմϴ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_NOT_JOIN_BOSS:
		{
			strMessage = _S(4822,"���������� �ν���Ʈ ���� �������� �ʾҽ��ϴ�.");
		}
		break;
	case MSG_RAID_ERROR_INZONE_INVALID_DIFFICULTY:
		{
			strMessage = _S(5319, "������ �Ұ����մϴ�. ���� �����밡 ������ ������ ���̵��� Ȯ���Ͻʽÿ�.");
		}
		break;
	case MSG_RAID_ERROR_INZONE_INIT_TIME:
		{
			strMessage = _S( 5371, "���̵� ���� �ʱ�ȭ ���Դϴ�." ) + CTString(" ") +
				_S( 5372, "����� �ٽ� �õ��Ͽ� �ֽʽÿ�.");
		}
		break;

	case MSG_RAID_ERROR_INZONE_NOT_QUEST:
	case MSG_RAID_ERROR_INZONE_NOT_FOUND_ZONE_INFO:
		{
			strMessage = _S(4549, "�ν���Ʈ ���� ������ �Ұ����մϴ�.");
		}
		break;

	default:
		{
			bShowMsgBox = FALSE;	// �޽��� �ڽ��� ����� �ʴ´�
			pUIManager->GetChattingUI()->AddSysMessage( _S( 4559, "�ν���Ʈ �� ����" ) );
		}
		break;
	}

	if(bShowMsgBox)
	{
		if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_ERROR))
			pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_ERROR);

		CUIMsgBox_Info MsgBoxInfo;

		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_EXPEDITION_ERROR );
		MsgBoxInfo.AddString(strMessage);
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
}

void Party::RecvViewDetail(SBYTE sbType, CNetworkMessage *istr)
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	switch(sbType)
	{
	case MSG_VIEWDETAIL_PET:
		{
			// ������
			CNetworkLibrary::sPetInfo petinfo;

			(*istr) >> petinfo.lIndex;
			(*istr) >> petinfo.sbPetTypeGrade;
			(*istr) >> petinfo.lLevel;
			(*istr) >> petinfo.llExp;
			(*istr) >> petinfo.llNeedExp;
			(*istr) >> petinfo.lHP;
			(*istr) >> petinfo.lMaxHP;
			(*istr) >> petinfo.lAbility;
			(*istr) >> petinfo.lHungry;
			(*istr) >> petinfo.lMaxHungry;
			(*istr) >> petinfo.lSympathy;
			(*istr) >> petinfo.lMaxSympathy;
#ifdef PET_SEAL_TIME
			(*istr) >> petinfo.lRemainRebirth;
#endif
			(*istr) >> petinfo.strNameCard;

			pUIMgr->GetExpeditionView()->SetPetInfo(petinfo);
		}
		break;
	case MSG_VIEWDETAIL_APET:
		{

			// �뺴�� ����
			sPetItem_Info wildPetInfo;
			INDEX   pet_type, pet_str_plus ,pet_con_plus, pet_dex_plus, pet_int_plus;

			(*istr) >> wildPetInfo.pet_index;
			(*istr) >> wildPetInfo.pet_name;
			(*istr) >> wildPetInfo.pet_level;
			(*istr) >> pet_type;
			(*istr)	>> wildPetInfo.pet_str >> pet_str_plus;
			(*istr)	>> wildPetInfo.pet_con >> pet_con_plus;
			(*istr)	>> wildPetInfo.pet_dex >> pet_dex_plus;
			(*istr)	>> wildPetInfo.pet_int >> pet_int_plus;

			pUIMgr->GetExpeditionView()->SetWildPetInfo(wildPetInfo);
		}
		break;
	case MSG_VIEWDETAIL_INVEN:
		{
			// ���
			SLONG	slWearingTotal;
			ULONG	uniIndex,index, plus, flag, used, used2;
			SBYTE	Wearing, sbOptionCount, sbOptionType;
			LONG	lOptionLevel;
			SQUAD	count;

			(*istr) >> slWearingTotal;

			for(int i = 0; i < slWearingTotal; i++)
			{
				(*istr) >> uniIndex;
				(*istr) >> index;
				(*istr) >> Wearing;
				(*istr) >> plus;
				(*istr) >> flag;
				(*istr) >> used;
				(*istr) >> used2;
				(*istr) >> count;
				(*istr) >> sbOptionCount;

				CItemData* pItemData	= _pNetwork->GetItemData(index);
				CItems* pItem = NULL;
				pItem = new CItems;

				//pItem->Item_Wearing = Wearing;
				pItem->ItemData = pItemData;
				pItem->SetData(index, uniIndex, -1, -1, plus, flag, -1, used, used2, Wearing, count);

				//���� �������϶�...
				if( pItemData->GetFlag() & ITEM_FLAG_RARE )
				{
					//�ɼ� ������ 0�̸� �̰��� ���������
					if( sbOptionCount ==0)
					{
						pItem->SetRareIndex(0);
					}
					//������ ����������̸�...
					else
					{
						//���� �ɼ� �ε���
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;

						LONG iRareIndex = lOptionLevel;

						pItem->SetRareIndex(iRareIndex);

						//���� �ɼ� ����
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;

						WORD iRareOption = lOptionLevel;
						WORD wCBit =1;
						SBYTE sbOption =-1;
						for(int iBit=0; iBit<10; ++iBit)
						{
							if(iRareOption & wCBit)
							{
								CItemRareOption* pItemOption = CItemRareOption::getData(iRareIndex);

								if (pItemOption == NULL)
									continue;

								if (pItemOption->GetIndex() < 0)
									continue;

								int OptionType = pItemOption->rareOption[iBit].OptionIdx;
								int OptionLevel = pItemOption->rareOption[iBit].OptionLevel;
								pItem->SetOptionData(++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT);
							}
							wCBit <<=1;
						}						
					}
				}
				//���� �������� �ƴϸ�.....
				else
				{
					LONG lOriginOptionVar = ORIGIN_VAR_DEFAULT;

					for( SBYTE sbOption = 0; sbOption < sbOptionCount; sbOption++ )
					{
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;

						if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN )
						{
							(*istr) >> lOriginOptionVar;
						}

						pItem->SetOptionData(sbOption, sbOptionType, lOptionLevel, lOriginOptionVar);
					}
				}

				if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
				{
					SBYTE sbBelong, sbSkillcont;
					LONG lSkillIndex;
					SBYTE sbSkillLevel;

					(*istr) >> sbBelong;
					(*istr) >> sbSkillcont;

					pItem->SetItemBelong(sbBelong);

					for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
					{
						(*istr) >> lSkillIndex;
						(*istr) >> sbSkillLevel;

						pItem->SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
					}
				}

				ULONG	plus2;
				(*istr) >> plus2;
				pItem->SetItemPlus2(plus2);

				pUIMgr->GetExpeditionView()->SetViewItem(Wearing, pItem);
			}

			pUIMgr->GetExpeditionView()->openUI();
		}
		break;
	}
}
