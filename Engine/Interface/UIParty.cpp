#include "stdh.h"

// �������. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIParty.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIPlayerInfo.h>
#include <Engine/Interface/UIPersonalShop.h>
#include <Engine/Object/ActorMgr.h>

// [7/13/2009 rumist] reject invite party.
extern INDEX g_iRejectParty;
// [sora] RAID_SYSTEM
CTString m_strExpeditionType[4];
CTString m_strDivisionType[3];
// [2012/08/27 : Sora] EX�α� �߰�
const COLOR jobStringCol[TOTAL_JOB] = { 
										/*Ÿ��ź*/0xFFFFCCFF,
										 /*���*/0x00B0F0FF, 
										 /*����*/0xFFFFFFFF, 
										 /*������*/0xFFC000FF, 
										 /*�α�*/0x00B050FF, 
										 /*�Ҽ���*/0xFF66FFFF, 
										 /*����Ʈ������*/0xD8D8D8FF,
#ifdef CHAR_EX_ROGUE
										 /*EX�α�*/0x00B050FF,
#endif
#ifdef CHAR_EX_MAGE	//2013/01/08 jeil EX������ �߰� 
										 /*EX������*/0xFFC000FF, 
#endif
										};
// ----------------------------------------------------------------------------
// Name : CUIParty()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIParty::CUIParty()
{
	Init();
	InitExpedition();

	m_strDivisionType[MSG_DIVITYPE_EXP] = _S(4514, "����ġ");
	m_strDivisionType[MSG_DIVITYPE_ITEM] = _S(4515, "�Ϲ� ������");
	m_strDivisionType[MSG_DIVITYPE_SPECIAL] = _S(4516, "����� ������");

	m_strExpeditionType[MSG_EXPED_TYPE_RANDOM] = _S(4517, "����ȹ��");
	m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET] = _S(4518, "�Լ��켱");
	m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] = _S(4519, "������");
	m_strExpeditionType[MSG_EXPED_TYPE_OPENBOX] = _S(4520, "���ڿ���");
	m_ptdExpeditionTexture = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIParty()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIParty::~CUIParty()
{
	Destroy();

	if (m_ptdExpeditionTexture)
	{
		_pTextureStock->Release(m_ptdExpeditionTexture);
		m_ptdExpeditionTexture = NULL;
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	//m_rcTitle.SetRect( 0, 0, 140, 22 );
	m_rcTitle.SetRect( 0, 0, 161, 15 );
	
	//int	nPosX = 10, nPosY = 39;
	int	nPosX = 32, nPosY = 27;

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		//m_arcHPs[i].SetRect( nPosX, nPosY, nPosX, nPosY + 5 );
		//m_arcMPs[i].SetRect( nPosX, nPosY + 10, nPosX, nPosY + 15 );
		m_arcHPs[i].SetRect( nPosX, nPosY, nPosX, nPosY + 6 );
		m_arcMPs[i].SetRect( nPosX, nPosY + 10, nPosX, nPosY + 16 );
		nPosY += PARTY_STRETCH_HEIGHT;
	}
	
	//m_rcBuff.SetRect( 119, 3, 129, 13 );
	m_rcBuff.SetRect( 126, 3, 136, 13 );

	// Create party texture
	//m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Party.tex" ) );
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\TopUI.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	//m_rtTop.SetUV( 0, 0, 140, 22, fTexWidth, fTexHeight );
	//m_rtMiddle.SetUV( 0, 22, 140, 58, fTexWidth, fTexHeight );
	//m_rtBottom.SetUV( 0, 62, 140, 64, fTexWidth, fTexHeight );
	m_rtTop.SetUV( 721, 0, 883, 15, fTexWidth, fTexHeight );
	m_rtMiddle.SetUV( 721, 15, 883, 42, fTexWidth, fTexHeight );
	m_rtBottom.SetUV( 721, 42, 883, 57, fTexWidth, fTexHeight );

	// HP
	//m_rtHP.SetUV( 187, 0, 188, 5, fTexWidth, fTexHeight );
	m_rtHP.SetUV( 307, 240, 310, 245, fTexWidth, fTexHeight );

	// MP
	//m_rtMP.SetUV( 192, 0, 193, 8, fTexWidth, fTexHeight );
	m_rtMP.SetUV( 312, 240, 315, 245, fTexWidth, fTexHeight );

	// Buff
	//m_rtBuff[0].SetUV( 163, 0, 173, 10, fTexWidth, fTexHeight );
	//m_rtBuff[1].SetUV( 174, 0, 184, 10, fTexWidth, fTexHeight );
	m_rtBuff[0].SetUV( 284, 236, 294, 246, fTexWidth, fTexHeight );
	m_rtBuff[1].SetUV( 295, 236, 305, 246, fTexWidth, fTexHeight );

	// Close Button 
	m_btnClose.Create( this, CTString( "" ), 112, 6, 10, 10 );
	m_btnClose.SetUV( UBS_IDLE, 141, 0, 151, 10, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 152, 0, 162, 10, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	m_btnClose.SetEnable(FALSE);


	// UI_REFORM :Su-won
	m_rcBuff.SetRect( 126, 3, 136, 13 );

//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM
//////////////////////////////////////////////////////////////////////////
	m_ptdExpeditionTexture = CreateTexture( CTString( "Data\\Interface\\Expedition.tex" ) );
	fTexWidth = m_ptdExpeditionTexture->GetPixWidth();
	fTexHeight = m_ptdExpeditionTexture->GetPixHeight();

	m_rtExpPosition[0].SetUV(405, 166, 418, 179, fTexWidth, fTexHeight);
	m_rtExpPosition[1].SetUV(388, 166, 401, 179, fTexWidth, fTexHeight);


	for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
	{
		m_UIExpGroup[j].Create(NULL, 0, 0, EXPEDITION_GROUP_WINDOW_WIDTH, EXPEDITION_GROUP_WINDOW_HEIGHT);
		m_UIExpGroup[j].SetVisible(FALSE);
		m_UIExpGroup[j].SetGroupNum(j+1);
	}

	m_UIExpManage.Create(NULL, 100, 100, EXPEDITION_MANAGE_WINDOW_WIDTH, EXPEDITION_MANAGE_WINDOW_HEIGHT);
	m_UIExpManage.SetVisible(FALSE);

	m_UIViewDetail.Create(NULL, 200, 200, VIEW_DETAIL_WINDOW_WIDTH, VIEW_DETAIL_WINDOW_HEIGHT);
	m_UIViewDetail.SetVisible(FALSE);
}

// ----------------------------------------------------------------------------
// Name : Init()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Init()
{
	m_nType = PT_PEACEEVER;
	m_nMemberCount = 0;
	m_bAmILeader = FALSE;
	m_bIsPartyRequested = FALSE;
	m_bShowBuff = FALSE;

	for( int i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		m_aslIndices[i] = -1;
		m_astrNames[i] = CTString( "" );
		m_abLeaders[i] = FALSE;
		m_asbJobs[i] = -1;
		m_aswLevels[i] = 0;
		m_aswHPs[i] = 0;
		m_aswMaxHPs[i] = 0;
		m_aswMPs[i] = 0;
		m_aswMaxMPs[i] = 0;
		m_afPosX[i] = 0.0f;
		m_afPosZ[i] = 0.0f;
		m_asbLayer[i] = 0;
		m_aslZone[i] = 0;
		m_aubBuff[i] = 0;
		m_bOnline[i] = false;

		m_ultargetID[i] =0;
		m_sbtargetType[i] =0;
	}
	m_strMandateCharName = CTString("");

	// [sora] RAID_SYSTEM
	m_sbDivisionTypeEXP = 0;
	m_sbDivisionTypeITEM = 0;
	m_sbDivisionTypeSPITEM = 0;

	// [091119: selo] ���� ��Ƽ ������ Ȯ�� �ϴ� �÷���
	m_bIsPartyPlay = FALSE;
}

// ----------------------------------------------------------------------------
// Name : TargetMember()
// Desc : 
// ----------------------------------------------------------------------------
void CUIParty::TargetMember( int nIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// [sora] �����뵵 ó���� �� �ְ� ����(�ڱ� �׷� ����� ����)

	INDEX	nCharIndex = 0;
	INDEX   nIndexReal = 0;
	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		nCharIndex = m_aslIndices[nIndex];
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

	ObjectBase* pObject = ACTORMGR()->GetObject(nCharIndex);

	if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
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
		nMaxHP = m_aswMaxHPs[nIndex];
		nHP = m_aswHPs[nIndex];
		nLevel = m_aswLevels[nIndex];
		strName = m_astrNames[nIndex];
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
	( (CPlayerEntity*)CEntity::GetPlayerEntity(0) )->ClearTargetInfo(_pNetwork->_TargetInfo.pen_pEntity);
	
	_pNetwork->_TargetInfo.fMaxHealth = nMaxHP;
	_pNetwork->_TargetInfo.fHealth = nHP;
	_pNetwork->_TargetInfo.bIsActive = TRUE;
	_pNetwork->_TargetInfo.iLevel = nLevel;
	strcpy(_pNetwork->_TargetInfo.TargetName, strName );
	_pNetwork->_TargetInfo.TargetType = CHARACTER;
}

// ----------------------------------------------------------------------------
// Name : AddPartyMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::AddPartyMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
								int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
								FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	if( m_nMemberCount >= PARTY_MAX_MEMBER )
		return;

	m_aslIndices[m_nMemberCount] = slIndex;
	m_astrNames[m_nMemberCount] = strName;
	m_abLeaders[m_nMemberCount] = static_cast<BOOL>(sbLeader);// FALSE
	m_asbJobs[m_nMemberCount] = sbJob;// sbLeader
	m_aswLevels[m_nMemberCount] = iLevel;
	m_aswHPs[m_nMemberCount] = iHP;
	m_aswMaxHPs[m_nMemberCount] = iMaxHP;
	m_aswMPs[m_nMemberCount] = iMP;
	m_aswMaxMPs[m_nMemberCount] = iMaxMP;
	m_afPosX[m_nMemberCount] = fX;
	m_afPosZ[m_nMemberCount] = fZ;
	m_asbLayer[m_nMemberCount] = sbLayer;
	m_aubBuff[m_nMemberCount] = 0;
	m_aslZone[m_nMemberCount] = slZone;
	m_bOnline[m_nMemberCount] = true;

	UpdateMemberInfo( m_nMemberCount );
	m_nMemberCount++;

	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );

	m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;
}

// ----------------------------------------------------------------------------
// Name : RemoveMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::RemoveMember( SLONG slIndex )
{
	int iMem;
	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	if( iMem == m_nMemberCount )
		return;

	CTString	strSysMessage;
	strSysMessage.PrintF( _S2( 421, m_astrNames[iMem], "%s<��> ��Ƽ���� Ż���ϼ̽��ϴ�." ), m_astrNames[iMem] );
	CUIManager::getSingleton()->GetChatting()->AddSysMessage(strSysMessage );

	m_nMemberCount--;
	int iPos;
	for( iPos = iMem; iPos < m_nMemberCount; iPos++ )
	{
		m_aslIndices[iPos] = m_aslIndices[iPos + 1];
		m_astrNames[iPos] = m_astrNames[iPos + 1];
		m_abLeaders[iPos] = m_abLeaders[iPos + 1];
		m_asbJobs[iPos] = m_asbJobs[iPos + 1];
		m_aswLevels[iPos] = m_aswLevels[iPos + 1];
		m_aswHPs[iPos] = m_aswHPs[iPos + 1];
		m_aswMaxHPs[iPos] = m_aswMaxHPs[iPos + 1];
		m_aswMPs[iPos] = m_aswMPs[iPos + 1];
		m_aswMaxMPs[iPos] = m_aswMaxMPs[iPos + 1];
		m_afPosX[iPos] = m_afPosX[iPos + 1];
		m_afPosZ[iPos] = m_afPosZ[iPos + 1];
		m_asbLayer[iPos] = m_asbLayer[iPos + 1];
		m_aubBuff[iPos] = m_aubBuff[iPos + 1];
	}
	m_aslIndices[iPos] = -1;
	m_astrNames[iPos] = CTString( "" );
	m_abLeaders[iPos] = FALSE;
	m_asbJobs[iPos] = -1;
	m_aswLevels[iPos] = 0;
	m_aswHPs[iPos] = 0;
	m_aswMaxHPs[iPos] = 0;
	m_aswMPs[iPos] = 0;
	m_aswMaxMPs[iPos] = 0;
	m_afPosX[iPos] = 0.0f;
	m_afPosZ[iPos] = 0.0f;
	m_asbLayer[iPos] = 0;
	m_aubBuff[iPos] = 0;
	m_bOnline[iPos] = false;

	m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;
}

// ----------------------------------------------------------------------------
// Name : BreakUpParty()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::BreakUpParty()
{
	Init();
	InitExpedition();	// [sora] RAID_SYSTEM

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY );
	pUIManager->SetCSFlagOff( CSF_EXPEDITION );	// [sora] RAID_SYSTEM

	pUIManager->RearrangeOrder( UI_PARTY, FALSE );

	// [sora] �й� ����â�� �־��ٸ� �ݾ��ش�
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}

	ObjectBase* pObject = ACTORMGR()->GetObject(_pNetwork->MyCharacterInfo.index);

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

// ----------------------------------------------------------------------------
// Name : UpdateMemberInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdateMemberInfo( int nMemIndex )
{
	FLOAT	fHPRatio = (FLOAT)m_aswHPs[nMemIndex] / (FLOAT)m_aswMaxHPs[nMemIndex];
	if( fHPRatio > 1.0f ) fHPRatio = 1.0f;
	m_arcHPs[nMemIndex].Right = m_arcHPs[nMemIndex].Left + PARTY_BAR_WIDTH * fHPRatio;

	FLOAT	fMPRatio = (FLOAT)m_aswMPs[nMemIndex] / (FLOAT)m_aswMaxMPs[nMemIndex];
	if( fMPRatio > 1.0f ) 
	{
		fMPRatio = 1.0f;
	}
	
	m_arcMPs[nMemIndex].Right = m_arcMPs[nMemIndex].Left + PARTY_BAR_WIDTH * fMPRatio;
}

// 051203
/*
// ----------------------------------------------------------------------------
// Name : IsPartyMember()
// Desc :
// ----------------------------------------------------------------------------
bool CUIParty::IsPartyMember( CEntity *pEntity )
{
	ASSERT( pEntity != NULL && "Invalid Entity Pointer!" );
	const int iPartyMemberCount	= GetMemberCount();
	if( iPartyMemberCount > 0 )
	{
		for(INDEX ipl2 = 0; ipl2 < _pNetwork->ga_srvServer.srv_actCha.Count(); ++ipl2) 
		{
			CCharacterTarget &ct = _pNetwork->ga_srvServer.srv_actCha[ipl2];
			if( ct.cha_iClientIndex == pEntity->en_ulID )
			{						
				for( int i = 0; i < iPartyMemberCount; ++i )
				{
					LONG lMemberIndex = GetMemberIndex( i );
					if( lMemberIndex == ct.cha_Index )
					{
						return true;						
					}
				}
				break;
			}
		}
	}
	return false;
}
*/

// ----------------------------------------------------------------------------
// Name : IsPartyMember()
// Desc :
// ----------------------------------------------------------------------------
bool CUIParty::IsPartyMember( INDEX iChaIndex )
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

// ----------------------------------------------------------------------------
// Name : UpdateMemberStatus()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP,
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
			if( m_aslIndices[iMem] == slIndex )
				break;
		}

		// Not found
		if( iMem == m_nMemberCount )
			return;

		// Update infos
		m_aswHPs[iMem] = iHP;
		m_aswMaxHPs[iMem] = iMaxHP;
		m_aswMPs[iMem] = iMP;
		m_aswMaxMPs[iMem] = iMaxMP;

		UpdateMemberInfo( iMem );
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

			int nHpBarWidth = m_ExpeditionGroup[nGroup][nPos].CalcHPBarWidth(EXPEDITION_GROUP_BARWIDTH);
			m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetBarWidth(nHpBarWidth);
			m_UIExpManage.GetMemberBtn(nGroup, nPos).SetBarWidth(nHpBarWidth);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	//SetPos( pixMinI, pixMinJ + pUIManager->GetPlayerInfo()->GetHeight() );
	SetPos( pixMinI, pixMinJ + pUIManager->GetPlayerInfo()->GetHeight() +10);

	int nPosX, nPosY;
	pUIManager->GetChatting()->GetChatWindowEndPos(nPosX, nPosY);

	for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
	{
		m_UIExpGroup[j].SetPos(nPosX + (j*152), nPosY);
	}
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{

	int nPosX, nPosY;
	BOOL bResetPosition = FALSE;
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		nPosX = m_UIExpGroup[i].GetAbsPosX();
		nPosY = m_UIExpGroup[i].GetAbsPosY();
		
		if( nPosX < pixMinI || nPosX + EXPEDITION_GROUP_WINDOW_WIDTH > pixMaxI ||
			nPosY < pixMinJ || nPosY + EXPEDITION_GROUP_WINDOW_HEIGHT > pixMaxJ )
		{
			bResetPosition = TRUE;
			break;
		}
	}

	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ || bResetPosition)
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::Render()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// [sora] ������ ������
	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		RenderExpedition();
		return;
	}

	// If party is not exist
	if( m_nMemberCount == 0 )
		return;

	// Set party texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	// Background
	int	nY = m_nPosY + m_rcTitle.Bottom;

	/********************************
	// Top
	

	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, nY,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		// Middle
		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY + PARTY_STRETCH_HEIGHT,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Buff toggling icon
		pDrawPort->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );

		nY += PARTY_STRETCH_HEIGHT;

		// Gague of HP & MP
		pDrawPort->AddTexture( m_nPosX + m_arcHPs[iMem].Left, m_nPosY + m_arcHPs[iMem].Top,
											m_nPosX + m_arcHPs[iMem].Right, m_nPosY + m_arcHPs[iMem].Bottom,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( m_nPosX + m_arcMPs[iMem].Left, m_nPosY + m_arcMPs[iMem].Top,
											m_nPosX + m_arcMPs[iMem].Right, m_nPosY + m_arcMPs[iMem].Bottom,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );
	}

	// Bottom
	pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY + 2,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );
	********************************/
	
	int nHPWidth = 0;
	int nMPWidth = 0;
	int	iMem;

	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		nY = m_nPosY +PARTY_STRETCH_HEIGHT *iMem;

		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY +15,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

		// Middle
		pDrawPort->AddTexture( m_nPosX, nY +15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT -15,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Bottom
		pDrawPort->AddTexture( m_nPosX, nY +PARTY_HEIGHT -15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

		// Buff toggling icon
		
		pDrawPort->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );

		// [sora] ��Ƽ�� �������� ó��
		if(m_bOnline[iMem] == true)
		{
			nHPWidth = m_arcHPs[iMem].GetWidth();
			nMPWidth = m_arcMPs[iMem].GetWidth();
		}
		else //�������� ���
		{
			nHPWidth = 1;
			nMPWidth = 1;
		}

		// Gague of HP & MP
		pDrawPort->AddTexture( m_nPosX + m_arcHPs[iMem].Left, m_nPosY + m_arcHPs[iMem].Top,
											m_nPosX + m_arcHPs[iMem].Left + nHPWidth, m_nPosY + m_arcHPs[iMem].Bottom,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( m_nPosX + m_arcMPs[iMem].Left, m_nPosY + m_arcMPs[iMem].Top,
											m_nPosX + m_arcMPs[iMem].Left + nMPWidth, m_nPosY + m_arcMPs[iMem].Bottom,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );
	}

	

	// Close button
	//m_btnClose.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Text in party
	//pDrawPort->PutTextEx( _S( 215, "��Ƽ" ), m_nPosX + PARTY_TITLE_TEXT_OFFSETX,
	//									m_nPosY + PARTY_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	nY = m_nPosY + PARTY_NAME_SY;

	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		COLOR	clrPartyLeader = 0xFFFFFFFF;
		
		CTString strLevel;
	// [sora] ��Ƽ�� �������� ó��
		if(m_bOnline[iMem] == true)
		{
			strLevel.PrintF(CTString("%d"), m_aswLevels[iMem]);
		}
		else
		{
			strLevel.PrintF(CTString("X"));	
		}
		//pDrawPort->PutTextExCX(strLevel, m_nPosX + 13, nY, clrPartyLeader);

		pDrawPort->PutTextExCX(strLevel, m_nPosX + 12, nY -PARTY_NAME_SY +18, clrPartyLeader);

		if( m_abLeaders[iMem] )
		{// ��Ƽ�� ���� ��
			clrPartyLeader = 0x72D02EFF;
		}

		if(m_bOnline[iMem] == false) //������ 0�� ����� �������θ��
		{
			clrPartyLeader = 0x777777FF;
		}

		pDrawPort->PutTextEx( m_astrNames[iMem], m_nPosX + PARTY_NAME_SX+13, nY, clrPartyLeader );
		nY += PARTY_STRETCH_HEIGHT;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render party buff
	if( m_bShowBuff )
		_pUIBuff->RenderPartyBuff();
}


// ----------------------------------------------------------------------------
// Name : RenderExpedition()
// Desc : [sora] ������ Render
// ----------------------------------------------------------------------------
void CUIParty::RenderExpedition()
{
	// ���� �Ҽӵ� �׷��� ���ٸ� �׸� �ʿ����
	if(m_nMyGroupNum < 0)
		return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int	nY;
	int	nPosX = 32, nPosY = 27;
	int nHPWidth;
	int nMPWidth;
	SLONG slPosition = 0;
	int	iMem;


	nY = m_nPosY;
	// ���� ���� �׷� ������ UI
	for( iMem = 0; iMem < EXPEDITION_MEMBER_PER_GROUP; iMem++ )
	{

		// ������, �� ������ �Ѿ
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_slIndex == _pNetwork->MyCharacterInfo.index)
			continue;

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].IsEmpty())
			continue;
		

		pDrawPort->AddTexture( m_nPosX, nY, m_nPosX + m_nWidth, nY +15,
										m_rtTop.U0, m_rtTop.V0, m_rtTop.U1, m_rtTop.V1,
										0xFFFFFFFF );

		// Middle
		pDrawPort->AddTexture( m_nPosX, nY +15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT -15,
											m_rtMiddle.U0, m_rtMiddle.V0, m_rtMiddle.U1, m_rtMiddle.V1,
											0xFFFFFFFF );

		// Bottom
		pDrawPort->AddTexture( m_nPosX, nY +PARTY_HEIGHT -15, m_nPosX + m_nWidth, nY +PARTY_HEIGHT,
										m_rtBottom.U0, m_rtBottom.V0, m_rtBottom.U1, m_rtBottom.V1,
										0xFFFFFFFF );

		// Buff toggling icon
		
		pDrawPort->AddTexture( m_nPosX + m_rcBuff.Left, nY + m_rcBuff.Top,
											m_nPosX + m_rcBuff.Right, nY + m_rcBuff.Bottom,
											m_rtBuff[m_aubBuff[iMem]].U0, m_rtBuff[m_aubBuff[iMem]].V0,
											m_rtBuff[m_aubBuff[iMem]].U1, m_rtBuff[m_aubBuff[iMem]].V1,
											0xFFFFFFFF );


		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_bOnline)
		{
			nHPWidth = m_ExpeditionGroup[m_nMyGroupNum][iMem].CalcHPBarWidth(PARTY_BAR_WIDTH);
			nMPWidth = m_ExpeditionGroup[m_nMyGroupNum][iMem].CalcMPBarWIdth(PARTY_BAR_WIDTH);
		}
		else //�������� ���
		{
			nHPWidth = 1;
			nMPWidth = 1;
		}



		pDrawPort->AddTexture( m_nPosX + nPosX, m_nPosY + nPosY,
											m_nPosX + nPosX + nHPWidth, m_nPosY + nPosY + 6,
											m_rtHP.U0, m_rtHP.V0, m_rtHP.U1, m_rtHP.V1,
											0xFFFFFFFF );

		pDrawPort->AddTexture( m_nPosX + nPosX, m_nPosY + nPosY + 10,
											m_nPosX + nPosX + nMPWidth, m_nPosY + nPosY + 16,
											m_rtMP.U0, m_rtMP.V0, m_rtMP.U1, m_rtMP.V1,
											0xFFFFFFFF );


		nY += PARTY_STRETCH_HEIGHT;
		nPosY += PARTY_STRETCH_HEIGHT;
	}

	// Render all elements
	pDrawPort->FlushRenderingQueue();


	pDrawPort->InitTextureData(m_ptdExpeditionTexture);

	// �� �׷� ������ ��å ������ǥ��
	nY = m_nPosY;
	nPosY = 27;
	for( iMem = 0; iMem < EXPEDITION_MEMBER_PER_GROUP; iMem++)
	{
		// ������, �� ������ �Ѿ
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_slIndex == _pNetwork->MyCharacterInfo.index)
			continue;

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].IsEmpty())
			continue;

		
		slPosition = m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nPosition;
		if(slPosition >= 0 && slPosition < MSG_EXPED_MEMBERTYPE_NORMAL)
		{
			pDrawPort->AddTexture( m_nPosX + PARTY_NAME_SX+13, nY + PARTY_NAME_SY + 1,
												m_nPosX + PARTY_NAME_SX+26, nY + PARTY_NAME_SY + 14,
												m_rtExpPosition[slPosition].U0, m_rtExpPosition[slPosition].V0, 
												m_rtExpPosition[slPosition].U1, m_rtExpPosition[slPosition].V1,
												0xFFFFFFFF );

		}

		nY += PARTY_STRETCH_HEIGHT;
		nPosY += PARTY_STRETCH_HEIGHT;
	}
	
	pDrawPort->FlushRenderingQueue();


	// ���� ���� �׷� ������ Text
	nY = m_nPosY + PARTY_NAME_SY;
	CTString strLevel;
	COLOR strColor;
	int nNamePosPlus = 0;
	for( iMem = 0; iMem < EXPEDITION_MEMBER_PER_GROUP; iMem++ )
	{

		// ������, �� ������ �Ѿ
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_slIndex == _pNetwork->MyCharacterInfo.index)
			continue;

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].IsEmpty())
			continue;
		
		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_bOnline)
		{
			// �¶����̸�
			strColor = 0xFFFFFFFF;
			strLevel.PrintF(CTString("%d"), m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nLevel);
		}
		else
		{
			strColor = 0xFFFFFFFF;
			strLevel.PrintF(_s("X"), m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nLevel);
		}

		pDrawPort->PutTextExCX(strLevel, m_nPosX + 12, nY -PARTY_NAME_SY +18, strColor);

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_bOnline)
		{
			strColor = GetJobStringColor(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_sbJob);
		}
		else
		{
			strColor = 0x777777FF;
		}

		if(m_ExpeditionGroup[m_nMyGroupNum][iMem].m_nPosition == MSG_EXPED_MEMBERTYPE_NORMAL)
		{
			nNamePosPlus = 13;
		}
		else
		{
			// ��å�� ������ ������ ǥ�õ� ���̸�ŭ �ڿ� ǥ��
			nNamePosPlus = 27;
		}
		pDrawPort->PutTextEx( m_ExpeditionGroup[m_nMyGroupNum][iMem].m_strName, m_nPosX + PARTY_NAME_SX+nNamePosPlus, nY, strColor );
		
		nY += PARTY_STRETCH_HEIGHT;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render party buff
	if( m_bShowBuff )
		_pUIBuff->RenderPartyBuff();

	// ������ ���
	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		if(i != m_nMyGroupNum)
		{
			if(m_UIExpGroup[i].IsVisible())
			{
				m_UIExpGroup[i].Render();
			}
		}
	}

	// ������ ����
	if(m_UIExpManage.IsVisible())
		m_UIExpManage.Render();

	// ���캸��
	if(m_UIViewDetail.IsVisible())
		m_UIViewDetail.Render();
}

// ----------------------------------------------------------------------------
// Name : ToggleVisible()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::ToggleVisible()
{
	if( m_nMemberCount == 0 )
		return;

	BOOL	bVisible = !IsVisible();
	CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, bVisible );
}

// ----------------------------------------------------------------------------
// Name : UpdatePartyBuff()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::UpdatePartyBuff( int nMemIndex, int nX, int nY )
{
	// [sora] �����뵵 ����� �� �ֵ��� ����(������� �ڽ��� �׷츸 ó��)

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		if( m_aslIndices[nMemIndex] == _pUIBuff->GetPartyIndex() )
			return;

		m_bShowBuff = FALSE;
		_pUIBuff->ResetPartyBuff();
		_pUIBuff->SetPartyBuffPos( m_aslIndices[nMemIndex], nX, nY );

		ObjectBase* pObject = ACTORMGR()->GetObject(m_aslIndices[nMemIndex]);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if (pTarget->cha_BuffCount <= 0)
			{
				_pUIBuff->ResetPartyBuff();
				return;
			}

			for (SBYTE sbBuff = 0; sbBuff < pTarget->cha_BuffCount; sbBuff++)
				_pUIBuff->AddPartyBuff( m_aslIndices[nMemIndex], pTarget->cha_Buff[sbBuff] );

			m_bShowBuff = TRUE;
			return;
		}
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		// �� �׷� ������â���� ������, �󽽷��� ǥ������ �����Ƿ� Ŭ���� ����� ������ ������ ������ ��ġ���� �����Ƿ� ����Ѵ�
		int nCount = 0;
		int nIndexReal;
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
			if(nCount == nMemIndex)
			{
				nIndexReal = i;
				break;
			}

			nCount++;
		}

		SLONG slIndex = m_ExpeditionGroup[m_nMyGroupNum][nIndexReal].m_slIndex;

		if( slIndex == _pUIBuff->GetPartyIndex() )
			return;

		m_bShowBuff = FALSE;
		_pUIBuff->ResetPartyBuff();
		_pUIBuff->SetPartyBuffPos( slIndex, nX, nY );

		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			for( SBYTE sbBuff = 0; sbBuff < 2; sbBuff++ )
			{
				_pUIBuff->AddPartyBuff( slIndex, _pUIBuff->GetBuffArray()[sbBuff] );
			}

			m_bShowBuff = TRUE;
			return;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if( pTarget->cha_BuffCount <= 0 )
			{
				_pUIBuff->ResetPartyBuff();
				return;
			}

			for( SBYTE sbBuff = 0; sbBuff < pTarget->cha_BuffCount; sbBuff++ )
				_pUIBuff->AddPartyBuff( slIndex, pTarget->cha_Buff[sbBuff] );

			m_bShowBuff = TRUE;
			return;
		}
	}

	_pUIBuff->ResetPartyBuff();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIParty::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	if( m_bShowBuff && ( wmsgResult = _pUIBuff->MouseMessagePartyBuff( pMsg ) ) != WMSG_FAIL )
	{
		if( wmsgResult == WMSG_COMMAND )
			CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );

		return wmsgResult;
	}

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		
		if(m_UIViewDetail.IsVisible())
		{
			if(m_UIViewDetail.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
		}
	
		if(m_UIExpManage.IsVisible())
		{
			if(m_UIExpManage.MouseMessage(pMsg) != WMSG_FAIL)
			{
				return WMSG_SUCCESS;
			}
		}
	
		for(int i=EXPEDITION_GROUP_MAX-1; i>=0; i--)
		{
			if(m_UIExpGroup[i].IsVisible() && (m_nMyGroupNum != i)) // �ڱ� �׷��� ����
			{
				if(m_UIExpGroup[i].MouseMessage(pMsg) != WMSG_FAIL)
					return WMSG_SUCCESS;
				
			}
		}
	}
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			// Move party
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				int	ndX = nX - nOldX;
				int	ndY = nY - nOldY;
				nOldX = nX;	nOldY = nY;

				Move( ndX, ndY );

				return WMSG_SUCCESS;
			}
			// Close button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// UI_REFORM :Su-won
			// Buff toggle
			else
			{
				for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
					m_aubBuff[iMem] = 0;

				int	nBuffX = m_nPosX;
				if( nX >= nBuffX + m_rcBuff.Left && nX <= nBuffX + m_rcBuff.Right )
				{
					//int	nBuffY = m_nPosY + m_rcTitle.Bottom;
					int	nBuffY = m_nPosY;

					for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
					{
						if( nY >= nBuffY + m_rcBuff.Top && nY <= nBuffY + m_rcBuff.Bottom )
						{
							m_aubBuff[iMem] = 1;

							UpdatePartyBuff( iMem, nBuffX + m_rcBuff.Left, nBuffY + m_rcBuff.Bottom );
							return WMSG_SUCCESS;
						}

						nBuffY += PARTY_STRETCH_HEIGHT;
					}
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) == FALSE )
				break;

			nOldX = nX;		nOldY = nY;

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				// Nothing
			}
			// Title bar
			else if( IsInsideRect( nX, nY, m_rcTitle ) )
			{
				bTitleBarClick = TRUE;
			}
			// Targetting party member
			else
			{
				int	iMem = ( nY - m_nPosY) / (PARTY_HEIGHT);
				if( iMem >= 0 && iMem < m_nMemberCount )
					TargetMember( iMem );
			}

			CUIManager::getSingleton()->RearrangeOrder( UI_PARTY, TRUE );
			return WMSG_SUCCESS;
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();

			// If holding button doesn't exist
			if( pUIManager->GetHoldBtn().IsEmpty() )
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If party isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
						pUIManager->RearrangeOrder( UI_PARTY, FALSE );

					return WMSG_SUCCESS;
				}
			}
			// If holding button exists
			else
			{
				if( IsInside( nX, nY ) )
				{
					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_RBUTTONDOWN: // [sora] ������ Ÿ������ �߰�
		{
			if( IsInside( nX, nY ) == FALSE )
				break;

			CUIManager* pUIManager = CUIManager::getSingleton();

			int	iMem = (nY - m_nPosY) / (PARTY_HEIGHT);
			if( iMem >= 0 && iMem < m_nMemberCount )
			{
				if(pUIManager->IsCSFlagOn(CSF_PARTY))
				{
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(m_ultargetID[iMem], m_sbtargetType[iMem]);
				}
				else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
				{
					int nCount = 0;
					int i;
					for(i = 0; i < EXPEDITION_MEMBER_PER_GROUP; i++)
					{
						if(m_ExpeditionGroup[m_nMyGroupNum][i].IsEmpty())
						{
							continue;
						}
						if(m_nGroupPos == i)
						{
							continue;
						}
						if(nCount == iMem)
						{
							break;
						}
						
						nCount++;
					}
					((CPlayerEntity*)(CEntity::GetPlayerEntity(0)))->SetTarget(m_ultargetID[i], m_sbtargetType[i]);
				}
			}

			pUIManager->RearrangeOrder( UI_PARTY, TRUE );
			return WMSG_SUCCESS;
		}
		break;

  		case WM_LBUTTONDBLCLK:
		{
			if( IsInside( nX, nY ) )
				return WMSG_SUCCESS;
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : MsgBoxCommand()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	switch( nCommandCode )
	{
	case MSGCMD_PARTY_INVITE:
		if( !bOK )
		{
			SendPartyReject();
		}

		break;

	case MSGCMD_PARTY_ALLOW:
		if( bOK )
			SendPartyAllow();
		else
			SendPartyReject();

		break;
	case MSGCMD_PARTY_MANDATE_REQ: 
		{
			if( bOK )
			{
				int nCharPos;

				if( ( nCharPos = IsOurPartyPos( m_strMandateCharName ) ) != -1 )
				{
					_pNetwork->MandateBossReq( m_strMandateCharName );
				}
			}
		}
		break;
	// UI_REFORM :Su-won
	case MSGCMD_PARTY_OPTION:
		{
			if( bOK )
			{
				CNetworkMessage	nmMessage( MSG_PARTY );
				nmMessage << (SBYTE)MSG_PARTY_CHANGETYPE;
				nmMessage << (SBYTE) CUIManager::getSingleton()->GetMessageBox(MSGCMD_PARTY_OPTION)->GetComboBox().GetCurSel();;
					
				_pNetwork->SendToServerNew( nmMessage );				
			}
		}
		break;
//////////////////////////////////////////////////////////////////////////
// [sora] RAID_SYSTEM ������ �޽��� �ڽ� ó�� �߰�
//////////////////////////////////////////////////////////////////////////
	case MSGCMD_EXPEDITION_INVITE:
		{
			if(!bOK)
			{
				SendExpeditionReject();
			}
		}
		break;

	case MSGCMD_EXPEDITION_ALLOW:
		{
			if(bOK)
			{
				SendExpeditionAllow();
			}
			else
			{
				SendExpeditionReject();
			}
		}
		break;

	case MSGCMD_EXPEDITION_INVITE_BY_NAME:	// ������ ����â���� �̸����� �ʴ��ϱ�
		{
			if( bOK == FALSE )
				break;

			CUIManager* pUIManager = CUIManager::getSingleton();
			CTString	strMessage;

			if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
			{
				strMessage.PrintF( _S( 4521, "�̹� %s���� �����븦 ��û���Դϴ�." ), m_strDestName );
				pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}

			// Find index of character
			SLONG	slIndex = -1;
			ObjectBase* pObject = ACTORMGR()->GetObjectByName(strInput.str_String, eOBJ_CHARACTER);

			if (pObject != NULL)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				if (pTarget->cha_sbShopType != PST_NOSHOP)
				{
					pUIManager->GetChatting()->AddSysMessage( _S(4522, "���� �������� ĳ���ʹ� �����뿡 ������ �� �����ϴ�." ) , SYSMSG_ERROR );	
					return;
				}

				slIndex = pTarget->GetSIndex();
			}

			if( slIndex == -1 )
			{
				pUIManager->GetChatting()->AddSysMessage( _S(4523, "����� ã�� �� �����ϴ�." ), SYSMSG_ERROR );
				return ;
			}

			if(IsExpedetionMember(slIndex))
			{
				strMessage.PrintF( _S( 4524, "�̹� %s�� ������ �����Դϴ�." ), strInput );
				pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
				return;
			}
			m_strDestName = strInput;

			_pNetwork->ExpeditionAddCharReq(strInput);
		}
		break;

	case MSGCMD_EXPEDITION_SET_BOSS:	// �������� ����
		{
			if(bOK)
			{
				_pNetwork->ExpeditionChangeLeaderReq(m_UIExpManage.m_slDestIndex);
			}
		}
		break;

	case MSGCMD_EXPEDITION_SET_MBOSS:	// �����δ����Ӹ�
		{
			if(bOK)
			{
				CUIManager::getSingleton()->GetParty()->SendChangeSubLeader(m_UIExpManage.m_slDestIndex);
			}
		}
		break;	

	case MSGCMD_EXPEDITION_RESET_MBOSS:	// �����δ��� ����
		{
			if(bOK)
			{
				_pNetwork->ExpeditionChangeSubLeaderReq(FALSE, m_UIExpManage.m_slDestIndex);
			}
		}
		break;	

	case MSGCMD_EXPEDITION_KICK:	// ������� ����
		{
			if(bOK)
			{
				CUIManager::getSingleton()->GetParty()->SendExpeditionKick(m_UIExpManage.m_slDestIndex);
			}
		}
		break;	

	case MSGCMD_EXPEDITION_GO_STARTPLACE:	// ��Ȱ�������� �̵�
		{
			if(bOK)
			{
				//��Ȱ���� �̵�
				_pNetwork->RaidInzoneQuitReq();
			}
		}
		break;

	case MSGCMD_EXPEDITION_ERROR:	//������ ���� �޽���, ������ �Ϲ� �޽����� ���
		{
			if(bOK)
			{
				CUIManager::getSingleton()->CloseMessageBox(MSGCMD_EXPEDITION_ERROR);
			}
		}
		break;

	case MSGCMD_DIVISION_INFO:	//�й��� ����
		{
			if(bOK)
			{
				CUIManager::getSingleton()->CloseMessageBox(MSGCMD_DIVISION_INFO);
			}
		}
		break;
//////////////////////////////////////////////////////////////////////////

	}
}


// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendPartyInvite()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyInvite( SBYTE sbType, CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	SLONG	slIndex = -1;	
	SBYTE	sbShopType = PST_NOSHOP;
	INDEX	iSyndicateType;

	// Find index of character
	if (_pNetwork->_TargetInfo.pen_pEntity != NULL)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(_pNetwork->_TargetInfo.pen_pEntity->GetNetworkID());

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			sbShopType = pTarget->cha_sbShopType;
			slIndex = pTarget->m_nIdxServer;
			iSyndicateType = pTarget->cha_iSyndicateType;
		}
	}

// 	if (_pNetwork->IsRvrZone() && _pNetwork->MyCharacterInfo.iSyndicateType != iSyndicateType)
// 	{	// RVR���� �ƴϰų� ���� Ÿ���� �ٸ� ���.
// 		pUIManager->GetChatting()->AddSysMessage( _S( 6090, "���� �������� ���� ������ ��� ����� �� ���� ����Դϴ�."), SYSMSG_ERROR);
// 		return;
// 	}

	// If I'm not a leader
	if( m_nMemberCount > 0 && !m_bAmILeader )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 364, "��Ƽ ��û ������ �����ϴ�" ), SYSMSG_ERROR );
		return;
	}

	// If party is already requested
	CTString	strMessage;
	if( m_bIsPartyRequested )
	{
		strMessage.PrintF( _S( 361, "�̹� %s���� ��Ƽ�� ��û���Դϴ�." ), m_strDestName );
		pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	if( m_nMemberCount > 0 && sbType != m_nType )
	{
		// If party type is different
		switch( m_nType )
		{
		case PT_PEACEEVER:
			pUIManager->GetChatting()->AddSysMessage( _S( 362, "�̹� �յ� �й� ��Ƽ���Դϴ�." ), SYSMSG_ERROR );
			break;
		case PT_SURVIVAL:
			pUIManager->GetChatting()->AddSysMessage( _S( 363, "�̹� �Լ� �켱 ��Ƽ���Դϴ�." ), SYSMSG_ERROR );
			break;
		case PT_ATTACK:
			pUIManager->GetChatting()->AddSysMessage( _S( 1651,  "�̹� ������ ��Ƽ���Դϴ�."  ), SYSMSG_ERROR ); 
			break;
		}
		return;
	}

	if( sbShopType != PST_NOSHOP )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 940, "���� �������� ��Ƽ�� �� �� �����ϴ�." ) , SYSMSG_ERROR );	
		return;
	}

	// If target is already my party member
	for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( slIndex == m_aslIndices[iMem] )
		{
			strMessage.PrintF( _S2( 420, m_strDestName, "�̹� %s<��> ��Ƽ���Դϴ�." ), m_strDestName );
			pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );

			return;
		}
	}

	// TO-KR-T20090903-005 ��Ƽ ��û ����. [11/27/2009 rumist]
	/*************
	// If character doesn't exist
	if( slIndex == -1 )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 310, "��Ƽ ��븦 ã�� �� �����ϴ�." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyInvite( sbType, slIndex );
	*************/
	_pNetwork->PartyInvite( sbType, slIndex, strDestName );
	m_strDestName = strDestName;
}

// ----------------------------------------------------------------------------
// Name : SendPartyAllow()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyAllow()
{
	_pNetwork->PartyAllow();
}

// ----------------------------------------------------------------------------
// Name : SendPartyReject()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyReject()
{
	_pNetwork->PartyReject();
}

// ----------------------------------------------------------------------------
// Name : SendPartyQuit()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyQuit()
{
	if( m_nMemberCount > 0 )
		_pNetwork->PartyQuit();
}

// ----------------------------------------------------------------------------
// Name : SendPartyKick()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::SendPartyKick( CTString &strDestName )
{
	if( !m_bAmILeader )
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 365, "��Ƽ�� ���� ������ �����ϴ�." ), SYSMSG_ERROR );
		return;
	}

	// Find index of character
/*	SLONG	slIndex = -1;
	INDEX	ctCha = _pNetwork->ga_srvServer.srv_actCha.Count();
	for( INDEX iPlyaer = 0; iPlyaer < ctCha; iPlyaer++ ) 
	{
		CCharacterTarget	&ct = _pNetwork->ga_srvServer.srv_actCha[iPlyaer];
		if( ct.cha_strName == strDestName )
		{
			slIndex = ct.cha_Index;
			break;
		}
	}
*/
	int nIndex = IsOurPartyPos( strDestName );

	// If character doesn't exist
	if( nIndex == -1 )
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 366, "���� ��븦 ã�� �� �����ϴ�." ), SYSMSG_ERROR );
		return ;
	}

	_pNetwork->PartyKick( m_aslIndices[nIndex] );
}


// ========================================================================= //
//                         Receive message functions                         // 
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : PartyInvite()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyInvite(SBYTE sbType, SLONG slSrcIndex, CTString &strSrcName, SBYTE sbPartyTypeItem /* = -1 */, SBYTE sbPartyTypeSPItem /* = -1 */)
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

// ----------------------------------------------------------------------------
// Name : PartyReject()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyReject( BOOL bLeader )
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

	pUIManager->GetChatting()->AddSysMessage( _S( 219, "��Ƽ ��û�� ��ҵǾ����ϴ�." ) );

	if( m_bAmILeader && m_nMemberCount <= 0 )
		m_bAmILeader = FALSE;

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY_REQ );
}

// ----------------------------------------------------------------------------
// Name : PartyAddMember()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyAddMember( SBYTE sbLeader, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2,
								int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
								FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	int nIsCharPos;
	m_bIsPartyRequested = FALSE;

	if( m_bAmILeader )
	{ pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE ); }
	else// ��Ƽ ��Ī���� ���� �޼��� â�� �ݴ´�.
	{ 
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );
		pUIManager->CloseMessageBox( MSGCMD_PARTYAUTO_JOIN_ALLOW );
	}

	if( ( nIsCharPos = IsOurParty( slIndex ) ) != -1 )// �̹� ��Ƽ�� ��� �ִ� ����(����)
	{
		m_aslIndices[nIsCharPos] = slIndex;
		m_astrNames[nIsCharPos] = strName;
		m_abLeaders[nIsCharPos] = static_cast<BOOL>(sbLeader);
		m_asbJobs[nIsCharPos] = sbJob;
		m_aswLevels[nIsCharPos] = iLevel;
		m_aswHPs[nIsCharPos] = iHP;
		m_aswMaxHPs[nIsCharPos] = iMaxHP;
		m_aswMPs[nIsCharPos] = iMP;
		m_aswMaxMPs[nIsCharPos] = iMaxMP;
		m_afPosX[nIsCharPos] = fX;
		m_afPosZ[nIsCharPos] = fZ;
		m_asbLayer[nIsCharPos] = sbLayer;
		m_aubBuff[nIsCharPos] = 0;
		m_aslZone[nIsCharPos] = slZone;

		UpdateMemberInfo( nIsCharPos );
	}
	else	// ��Ƽ�� ���� ����
	{
		AddPartyMember( sbLeader, slIndex, strName, sbJob, sbJob2, iLevel, iHP, iMaxHP, iMP, iMaxMP, fX, fZ, sbLayer, slZone );
	}
	// ���� ���� ��Ƽ�� ������ ���� ������ ���� �Ѵ�.
	SetPartyLeader();

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_PARTY_REQ );
	pUIManager->SetCSFlagOn( CSF_PARTY );
}

// ----------------------------------------------------------------------------
// Name : PartyQuit()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyQuit( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		BreakUpParty();
		pUIManager->GetChatting()->AddSysMessage( _S( 367, "��Ƽ���� Ż���ϼ̽��ϴ�." ) );
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

// ----------------------------------------------------------------------------
// Name : PartyKick()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyKick( SLONG slIndex )
{
	m_bIsPartyRequested = FALSE;

	if( slIndex == _pNetwork->MyCharacterInfo.index )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		BreakUpParty();
		pUIManager->GetChatting()->AddSysMessage( _S( 368, "��Ƽ���� ���� �Ǽ̽��ϴ�." ) );
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

// ----------------------------------------------------------------------------
// Name : PartyEnd()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyEnd()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	m_bIsPartyRequested = FALSE;

	BreakUpParty();

	pUIManager->GetChatting()->AddSysMessage( _S( 220, "��Ƽ�� ��ü�Ǿ����ϴ�." ) );

	if( pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO) == TRUE )
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

// ----------------------------------------------------------------------------
// Name : PartyMemberInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyMemberInfo( SLONG slIndex, int iLevel, int iHP, int iMaxHP,
								int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline )
{
	// Find party member
	int iMem;
	for( iMem = 0; iMem < m_nMemberCount; iMem++ )
	{
		if( m_aslIndices[iMem] == slIndex )
			break;
	}

	// Not found
	if( iMem == m_nMemberCount )
		return;

	// Update infos
	m_aswLevels[iMem] = iLevel;
	m_aswHPs[iMem] = iHP;
	m_aswMaxHPs[iMem] = iMaxHP;
	m_aswMPs[iMem] = iMP;
	m_aswMaxMPs[iMem] = iMaxMP;
	m_afPosX[iMem] = fX;
	m_afPosZ[iMem] = fZ;
	m_asbLayer[iMem] = sbLayer;
	m_aslZone[iMem] = slZone;
	m_bOnline[iMem] = bOnline;

	UpdateMemberInfo( iMem );
}

// ----------------------------------------------------------------------------
// Name : PartyError()
// Desc :
// ----------------------------------------------------------------------------
void CUIParty::PartyError( SBYTE sbError )
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

	pUIManager->GetChatting()->AddSysMessage( strSysMessage, SYSMSG_ERROR );

	if( pUIManager->DoesMessageBoxExist(MSGCMD_PARTY_INVITE) == TRUE )
	{
		pUIManager->CloseMessageBox( MSGCMD_PARTY_INVITE );
	}
}

//------------------------------------------------------------------------------
// CUIParty::MandateBossReq
// Explain:  
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIParty::MandateBossReq( CTString strManadateChaName )
{
	m_strMandateCharName = strManadateChaName;

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "��Ƽ" ), UMBS_YESNO, UI_PARTY, MSGCMD_PARTY_MANDATE_REQ );

	CTString strMessage;

	strMessage.PrintF( _S(2170, "������ %s�Բ� ��Ƽ���� �����Ͻðڽ��ϱ�?" ), strManadateChaName );
		
	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIParty::MandateBoss
// Explain:  ��Ƽ�� ���� �޽��� 
// Date : 2005-10-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIParty::MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate )
{
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(97, "��Ƽ" ), UMBS_OK, UI_PARTY, MSGCMD_PARTY_MANDATE );
	CTString strMessage;
	
	if( strManadateChaName == _pNetwork->MyCharacterInfo.name ) 
	{
		int nCharPos = IsOurPartyPos( strBossName );

		if( nCharPos != -1 )
		{
			m_abLeaders[nCharPos] = FALSE;
		}
	}
	else 
	{
		for( int nMem=0; nMem<m_nMemberCount; nMem++ )
		{
			if( m_aslIndices[nMem] == nManadateChaIndex )
			{
				int nCharPos = IsOurPartyPos( strBossName );

				if( nCharPos != -1 )
					m_abLeaders[nCharPos] = FALSE;

				m_abLeaders[nMem] = TRUE;
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

//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  �ش� ��Ƽ���� �츮 ��Ƽ������ �ƴ��� Ȯ�� 
// Date : 2005-10-19,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
bool CUIParty::IsOurParty( CTString strName )
{
	for( int i = 0; i < m_nMemberCount; i++ )
	{
		if( m_astrNames[i] == strName )
		{
			return true;
		}
	}
	return false;
}
//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  �ĺ��ڷ� �ش� ��Ƽ���� �츮 ��Ƽ������ �ƴ��� Ȯ�� ( ������ ������ -1 ���� )
// Date : 2006-05-19(���� 9:54:23), By eons
//------------------------------------------------------------------------------
int CUIParty::IsOurParty( SLONG slCharIndex )
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_aslIndices[i] == slCharIndex )
			return i;
	}

	return -1;
}
//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  �̸����� �ش� ��Ƽ���� �츮 ��Ƽ������ �ƴ��� Ȯ�� ( ������ ������ -1 ���� )
// Date : 2006-05-19(���� 9:54:23), By eons
//------------------------------------------------------------------------------
int	CUIParty::IsOurPartyPos( CTString strName )
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_astrNames[i] == strName )
			return i;
	}

	return -1;
}

//------------------------------------------------------------------------------
// CUIParty::IsOurParty
// Explain:  ���� ���� ��Ƽ ���� �� ������ ������ �ڽ��� ������ �ȴ�.
// Date : 2006-05-19(���� 10:11:07), By eons
//------------------------------------------------------------------------------
void CUIParty::SetPartyLeader()
{
	for( SLONG i=0; i<m_nMemberCount; i++ )
	{
		if( m_abLeaders[i] == TRUE )
		{
			m_bAmILeader = FALSE;
			return;
		}
	}

	m_bAmILeader = TRUE;
}

void CUIParty::SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType)
{
	// [sora] �����뵵 ����� �� �ֵ��� ����

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		for(int i=0; i<PARTY_MAX_MEMBER; ++i)
		{
			if( m_aslIndices[i] ==partyID)
			{
				m_ultargetID[i] =targetID;
				m_sbtargetType[i] =sbtargetType;
				return;
			}
		}
	}
	else if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; ++i)
		{
			if( m_ExpeditionGroup[m_nMyGroupNum][i].m_slIndex == partyID )
			{
				m_ultargetID[i] =targetID;
				m_sbtargetType[i] =sbtargetType;
				return;
			}
		}
	}

}


//////////////////////////////////////////////////////////////////////////
// RAID_SYSTEM 
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// CUIParty::SendChangeExpedition()
// Explain: [sora] ������� ��ȯ �޽��� ����
//------------------------------------------------------------------------------									
void CUIParty::SendChangeExpedition()
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
		if( comboZone == m_aslZone[iMem] )
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

//------------------------------------------------------------------------------
// CUIParty::ChangeExpedition()
// Explain: [sora] ������� ��ȯ �޽��� ����
//------------------------------------------------------------------------------									
void CUIParty::ChangeExpedition( CNetworkMessage *istr )
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

	pUIManager->GetChatting()->AddSysMessage( _S( 4530, "��Ƽ�� ������� ��ȯ�Ǿ����ϴ�." ) );

	// ����â�� ������Ʈ���� �����Ƿ� �ݾ��ش�.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
	
	// �÷��̾� �޴� �˾�â�� �����ִٸ� ����
	pUIManager->GetPlayerInfo()->HideCommandPopUp();
}



//------------------------------------------------------------------------------
// CUIParty::SendExpeditionInvite()
// Explain: [sora] ������ �ʴ� �޽��� ����
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionInvite( CTString &strDestName )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// ��������� �ʴ� �Ұ�
	if( GetExpeditionMyPosition() == MSG_EXPED_MEMBERTYPE_NORMAL)
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4531, "������ ��û ������ �����ϴ�" ), SYSMSG_ERROR );
		return;
	}

	CTString	strMessage;

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
	{
		strMessage.PrintF( _S( 4521, "�̹� %s���� �����븦 ��û���Դϴ�." ), m_strDestName );
		pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}
	
	// Find index of character
	SLONG	slIndex = -1;

	if (_pNetwork->_TargetInfo.pen_pEntity != NULL)
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(_pNetwork->_TargetInfo.pen_pEntity->GetNetworkID());

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			if ( pTarget->cha_sbShopType != PST_NOSHOP )
			{
				pUIManager->GetChatting()->AddSysMessage( _S(4522, "���� �������� ĳ���ʹ� �����뿡 ������ �� �����ϴ�." ) , SYSMSG_ERROR );	
				return;
			}
			
			slIndex = pTarget->m_nIdxServer;
		}
	}

	if( slIndex == -1 )
	{
		pUIManager->GetChatting()->AddSysMessage( _S(4523, "����� ã�� �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}

	if(IsExpedetionMember(slIndex))
	{
		strMessage.PrintF( _S( 4524, "�̹� %s�� ������ �����Դϴ�." ), strDestName );
		pUIManager->GetChatting()->AddSysMessage( strMessage, SYSMSG_ERROR );
		return;
	}

	_pNetwork->ExpeditionInviteReq(slIndex);
	m_strDestName = strDestName;
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionReject()
// Explain: [sora] ������ ���� �޽��� ����
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionReject()
{
	_pNetwork->ExpeditionRejectReq();
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionAllow()
// Explain: [sora] ������ ���� �޽��� ����
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionAllow()
{
	_pNetwork->ExpeditionAllowReq();
}


//------------------------------------------------------------------------------
// CUIParty::SendExpeditionQuit()
// Explain: [sora] ������ Ż�� �޽��� ����
//------------------------------------------------------------------------------									
void CUIParty::SendExpeditionQuit()
{
	_pNetwork->ExpeditionQuitReq();
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionKick()
// Explain: [sora] ������ ���� �޽��� ����
//------------------------------------------------------------------------------															
void CUIParty::SendExpeditionKick( SLONG slIndex )
{
	if(!IsExpedetionMember(slIndex))
		return;

	_pNetwork->ExpeditionKickReq(slIndex);
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionEnd()
// Explain: [sora] ������ ���� �޽��� ����
//------------------------------------------------------------------------------											
void CUIParty::SendExpeditionEnd()
{
	_pNetwork->ExpeditionEndReq();
}

//------------------------------------------------------------------------------
// CUIParty::SendDivisionType()
// Explain: [sora] �й��� ���� �޽��� ����
//------------------------------------------------------------------------------														
void CUIParty::SendDivisionType(SBYTE sbDivisionType, SBYTE sbType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsCSFlagOn( CSF_PARTY_REQ ) || pUIManager->IsCSFlagOn( CSF_EXPEDITION_REQ ) )
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4798, "��Ƽ�� ������ ��û�߿��� ������ �� �����ϴ�." ), SYSMSG_ERROR );
		return;
	}
	else if(IsExistOfflineMember())
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4799,"�������� ����� �־ ������ �� �����ϴ�." ), SYSMSG_ERROR );
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

//------------------------------------------------------------------------------
// CUIParty::SendChangeGroup()
// Explain: [sora] ������ �׷� ���� �޽��� ����
//------------------------------------------------------------------------------																
void CUIParty::SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum)
{
	SLONG slIndex = m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_slIndex;

	if(m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_nPosition == MSG_EXPED_MEMBERTYPE_BOSS || m_ExpeditionGroup[nSrcGroupNum][nSrcPosNum].m_nPosition == MSG_EXPED_MEMBERTYPE_MBOSS)
	{
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( _S( 4532,"��������� �����δ����� �׷��� �̵��� �� �����ϴ�.") );
		m_UIExpManage.m_bCanUseBtn = TRUE;	// ��ư ��� �Ұ� ����
		return;
	}

	_pNetwork->ExpeditionChangeGroupReq(nSrcGroupNum, slIndex, nDestGroupNum, nDestPosNum);
}

//------------------------------------------------------------------------------
// CUIParty::InitExpedition()
// Explain: [sora] ������ ���� �ʱ�ȭ
//------------------------------------------------------------------------------
void CUIParty::InitExpedition()
{
	int		i;
	m_nType = 0;
	m_nMemberCount = 0;
	m_nMyGroupNum = -1;		
	m_sbDivisionTypeEXP = 0;
	m_sbDivisionTypeITEM = 0;
	m_sbDivisionTypeSPITEM = 0;

	for(i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		for(int j=0; j<EXPEDITION_MEMBER_PER_GROUP; j++)
			m_ExpeditionGroup[i][j].Init();
	}
	
	for( i = 0; i < PARTY_MAX_MEMBER; i++ )
	{
		m_aubBuff[i] = 0;
	}

	for( i = 0; i < EXPEDITION_GROUP_MAX; i++)
	{
		m_UIExpGroup[i].ClearExpeditionGroup();
		m_UIExpGroup[i].SetVisible(FALSE);
	}
	m_UIExpManage.ClearExpManage();
	m_UIViewDetail.ClearViewDetail();
	ClearTargetMark();
}


//------------------------------------------------------------------------------
// CUIParty::PartyData2Expedition
// Explain: [sora] ��Ƽ->������ ��ȯ�� ��Ƽ�� �ִ� ���� 0�� �׷� �ش� ��ġ�� �ű��
//------------------------------------------------------------------------------
void CUIParty::PartyData2Expedition(SLONG slIndex, int nPos)
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
		
		for( SLONG i=0; i<PARTY_MAX_MEMBER; i++ )
		{
			if( m_aslIndices[i] == slIndex )
				nPartyPos = i;
		}

		if(nPartyPos >= 0)
		{
			expMember.m_slZone = m_aslZone[nPartyPos];															
			expMember.m_nLevel = m_aswLevels[nPartyPos];				
			expMember.m_nHP = m_aswHPs[nPartyPos];					
			expMember.m_nMaxHP = m_aswMaxHPs[nPartyPos];				
			expMember.m_nMP = m_aswMPs[nPartyPos];					
			expMember.m_nMaxMP = m_aswMaxMPs[nPartyPos];				
			expMember.m_fPosX = m_afPosX[nPartyPos];				
			expMember.m_fPosZ = m_afPosZ[nPartyPos];				
			expMember.m_sbLayer = m_asbLayer[nPartyPos];	
			expMember.m_slIndex = m_aslIndices[nPartyPos];			
			expMember.m_strName = m_astrNames[nPartyPos];	
			expMember.m_nPosition = m_abLeaders[nPartyPos] ? MSG_EXPED_MEMBERTYPE_BOSS : MSG_EXPED_MEMBERTYPE_NORMAL;;
			expMember.m_sbJob = m_asbJobs[nPartyPos];	
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

//------------------------------------------------------------------------------
// CUIParty::ExpeditionInvite
// Explain: [sora] ������ ���� ��û ����
//------------------------------------------------------------------------------																
void CUIParty::ExpeditionInvite( CNetworkMessage *istr )
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


//------------------------------------------------------------------------------
// CUIParty::ExpeditionError
// Explain: [sora] ������ ���� �޽��� ����
//------------------------------------------------------------------------------
void CUIParty::ExpeditionError(CNetworkMessage *istr )
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
			pUIManager->GetChatting()->AddSysMessage( _S( 4535, "������ ����" ) );
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


//------------------------------------------------------------------------------
// CUIParty::RaidError
// Explain: [sora] �δ� ���� �޽���
//------------------------------------------------------------------------------
void CUIParty::RaidError( CNetworkMessage *istr )
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
			{
				strMessage = _S(4549, "�ν���Ʈ ���� ������ �Ұ����մϴ�.");
			}
			break;

		default:
			{
				bShowMsgBox = FALSE;	// �޽��� �ڽ��� ����� �ʴ´�
				pUIManager->GetChatting()->AddSysMessage( _S( 4559, "�ν���Ʈ �� ����" ) );
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

//------------------------------------------------------------------------------
// CUIParty::ExpeditionReject
// Explain: [sora] ������ ���� ���� ����
//------------------------------------------------------------------------------	
void CUIParty::ExpeditionReject( BOOL bIsSrc )
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

	pUIManager->GetChatting()->AddSysMessage( _S( 4564, "������ ��û�� ��ҵǾ����ϴ�." ) );

	pUIManager->SetCSFlagOff(CSF_EXPEDITION_REQ);
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionQuit
// Explain: [sora] ������ Ż�� ����
//------------------------------------------------------------------------------											
void CUIParty::ExpeditionQuit( SLONG slType, SLONG slIndex )
{
	if(slType == MSG_EXPED_QUITMODE_NORMAL)	// �������� Ż���� ���
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		if(slIndex == _pNetwork->MyCharacterInfo.index)
		{
			ExpeditionEnd();
			pUIManager->GetChatting()->AddSysMessage( _S( 4565, "�����뿡�� Ż���ϼ̽��ϴ�." ) );
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

			SearchExpeditionMember(slIndex, nGroup, nPos);
			
			CTString	strSysMessage;
			strSysMessage.PrintF( _S( 4566, "%s�� �����뿡�� Ż���ϼ̽��ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
			pUIManager->GetChatting()->AddSysMessage(strSysMessage );
			
			RemoveExpeditionMember(nGroup, nPos);
		}
	}
	else if(slType == MSG_EXPED_QUITMODE_UNUSUAL)	// ���������� Ż���� ���
	{
		ExpeditionMemberOnline(slIndex, FALSE);
	}

}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionKick
// Explain: [sora] ������ ����
//------------------------------------------------------------------------------																
void CUIParty::ExpeditionKick( SLONG slIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		pUIManager->GetChatting()->AddSysMessage( _S( 4687, "�����뿡�� ����Ǽ̽��ϴ�." ) );
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

		SearchExpeditionMember(slIndex, nGroup, nPos);

		CTString	strSysMessage;
		strSysMessage.PrintF( _S( 4566, "%s�� �����뿡�� Ż���ϼ̽��ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);
		pUIManager->GetChatting()->AddSysMessage(strSysMessage );

		RemoveExpeditionMember(nGroup, nPos);
	}

}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionEnd
// Explain: [sora] ������ ����
//------------------------------------------------------------------------------												
void CUIParty::ExpeditionEnd()
{
	InitExpedition();

	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->SetCSFlagOff( CSF_EXPEDITION );
	pUIManager->SetCSFlagOff( CSF_PARTY );

	pUIManager->RearrangeOrder( UI_PARTY, FALSE );

	// �й� ����â�� �־��ٸ� �ݾ��ش�
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

//------------------------------------------------------------------------------
// CUIParty::ChangeGroup
// Explain: [sora] ����ĳ���� ������ �׷� ����
//------------------------------------------------------------------------------																
void CUIParty::ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum) // ĳ���� �ε���, �̵��� �׷�, ��ġ
{

	int nGroup = 0;	// ���� �׷�
	int nPos = 0;	// ���� ��ġ

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		stExpeditionMember expMember = m_ExpeditionGroup[nGroup][nPos];
		RemoveExpeditionMember(nGroup, nPos);
		AddExpeditionMember(nGroupNum, nPosNum, expMember);

		m_UIExpManage.GetMemberBtn(nGroup, nPos).InitBtn();
		m_UIExpManage.GetMemberBtn(nGroup, nPos).SetBtnType(UBET_EXPEDITION);

		CTString strMessage;
		if(nGroup != nGroupNum)
		{
			strMessage.PrintF(_S( 4567, "%s�� %d�׷� %d������ %d�׷� %d������ �̵��Ͽ����ϴ�." ), m_ExpeditionGroup[nGroupNum][nPosNum].m_strName, nGroup+1, nPos+1, nGroupNum+1, nPosNum+1);	//�׷�� ǥ���Ҷ��� +1(0~3) -> (1~4)
		}
		else
		{
			strMessage.PrintF(_S( 4568, "%s�� %d�׷� %d������ %d������ �̵��Ͽ����ϴ�." ), m_ExpeditionGroup[nGroupNum][nPosNum].m_strName, nGroupNum+1, nPos+1, nPosNum+1);	//�׷�� ǥ���Ҷ��� +1(0~3) -> (1~4)
		}
		
		CUIManager::getSingleton()->GetChatting()->AddSysMessage( strMessage );
	}

	m_UIExpManage.m_bCanUseBtn = TRUE;	// ��ư Drag&Drop ���

}

//------------------------------------------------------------------------------
// CUIParty::SetDivisionType
// Explain: [sora] �й��� ���� (bFirst -> �� ���ӽ� ��Ƽ��������)
//------------------------------------------------------------------------------										
void CUIParty::SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst /* = FALSE */)
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
		strTemp.PrintF(_S(4569, "%s �й� ����� %s(��)�� ����Ǿ����ϴ�."), strDivisionType[sbType], strTemp2);
		pUIManager->GetChatting()->AddSysMessage( strTemp );
	}


	// ����â�� ������Ʈ���� �����Ƿ� �ݾ��ش�.
	if(pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO))
	{
		pUIManager->CloseMessageBox(MSGCMD_DIVISION_INFO);
	}
}

//------------------------------------------------------------------------------
// CUIParty::SetDivisionType
// Explain: [sora] �й��� ����
//------------------------------------------------------------------------------
void CUIParty::SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM)
{
	m_sbDivisionTypeEXP = sbDivisionTypeEXP;

	m_sbDivisionTypeITEM = sbDivisionTypeITEM;

	m_sbDivisionTypeSPITEM = sbDivisionTypeSPITEM;
}

//------------------------------------------------------------------------------
// CUIParty::AddExpeditionMember
// Explain: [sora] ������� �߰�
//------------------------------------------------------------------------------								
void CUIParty::AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex, CTString &strName, SBYTE sbJob, SBYTE sbJob2, int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone )
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

//------------------------------------------------------------------------------
// CUIParty::AddExpeditionMember
// Explain: [sora] ������� �߰�
//------------------------------------------------------------------------------
void CUIParty::AddExpeditionMember(int nGroupNum, int nPos, stExpeditionMember expMember)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxExist(MSGCMD_EXPEDITION_INVITE))
		pUIManager->CloseMessageBox(MSGCMD_EXPEDITION_INVITE);

	// ������ 0�̸� �������� ���
	if(expMember.m_nLevel == 0)
	{
		expMember.m_bOnline = FALSE;
	}


	if(m_UIExpGroup[nGroupNum].GetMemberCount() == 0)
		m_UIExpGroup[nGroupNum].SetVisible(TRUE);

	m_ExpeditionGroup[nGroupNum][nPos] = expMember;

	int nHPBarWidth = m_ExpeditionGroup[nGroupNum][nPos].CalcHPBarWidth(EXPEDITION_GROUP_BARWIDTH);

	if(expMember.m_slIndex == _pNetwork->MyCharacterInfo.index)
	{
		m_nMyGroupNum = nGroupNum;
		m_nGroupPos = nPos;

		int nPosX, nPosY;
		pUIManager->GetChatting()->GetChatWindowEndPos(nPosX, nPosY);

		int nCnt = 0;
		for(int j=0; j<EXPEDITION_GROUP_MAX; j++)
		{
			if(j > m_nMyGroupNum)
				nCnt = j - 1;
			else
				nCnt = j;
			m_UIExpGroup[j].SetPos(nPosX + (nCnt*152), nPosY);
		}
	}

	m_UIExpGroup[nGroupNum].GetMemberBtn(nPos).SetExpeditionInfo(expMember.m_sbJob, expMember.m_nPosition, expMember.m_strName, nHPBarWidth, expMember.m_nLevel);
	m_UIExpGroup[nGroupNum].GetMemberBtn(nPos).SetOnline(expMember.m_bOnline);


	m_UIExpManage.GetMemberBtn(nGroupNum, nPos).SetExpeditionInfo(expMember.m_sbJob, expMember.m_nPosition, expMember.m_strName, nHPBarWidth, expMember.m_nLevel);
	m_UIExpManage.GetMemberBtn(nGroupNum, nPos).SetOnline(expMember.m_bOnline);

	if(m_nMyGroupNum >= 0)
	{
		if(nGroupNum == m_nMyGroupNum)
		{
			m_nMemberCount = m_UIExpGroup[m_nMyGroupNum].GetMemberCount() - 1; //�ڱ�� ����
			m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;	// ��Ƽâ ���� ���
		}

	}

	m_aubBuff[nPos] = 0;

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION_REQ))
		pUIManager->SetCSFlagOff( CSF_EXPEDITION_REQ );

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
		pUIManager->SetCSFlagOff( CSF_PARTY );

	pUIManager->SetCSFlagOn(CSF_EXPEDITION);

	// ���ο� ����� ������ ��Ƽâ�� �����ش�.
	if(!pUIManager->IsUIVisible(UI_PARTY))
		pUIManager->RearrangeOrder( UI_PARTY, TRUE );

}

//------------------------------------------------------------------------------
// CUIParty::RemoveExpeditionMember
// Explain: [sora] ������� ����
//------------------------------------------------------------------------------	
void CUIParty::RemoveExpeditionMember( int nGroup, int nPos )
{

	m_ExpeditionGroup[nGroup][nPos].Init();	


	m_UIExpGroup[nGroup].GetMemberBtn(nPos).InitBtn();
	m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetBtnType(UBET_EXPEDITION);

	if(m_UIExpGroup[nGroup].GetMemberCount() == 0)
	{
		m_UIExpGroup[nGroup].SetVisible(FALSE);
	}


	m_UIExpManage.GetMemberBtn(nGroup, nPos).InitBtn();
	m_UIExpManage.GetMemberBtn(nGroup, nPos).SetBtnType(UBET_EXPEDITION);

	if(nGroup == m_nMyGroupNum)
	{
		m_nMemberCount--;
		m_nHeight = m_rcTitle.GetHeight() + m_nMemberCount * PARTY_STRETCH_HEIGHT;	// ��Ƽâ ���� ���
		m_aubBuff[nPos] = 0;
	}
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionMemberInfo
// Explain: [sora] �ش� �׷� ������ ����� ���� ���� �Լ�.
//------------------------------------------------------------------------------									
void CUIParty::ExpeditionMemberInfo(SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP, int iMP,
									int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline )
{

	for(int i=0; i<EXPEDITION_MEMBER_PER_GROUP; i++)
	{
		if(GetExpeditionMemberIndex(slGroup, i) == slIndex)
		{
			if(bOnline == true)
			{
				if(m_ExpeditionGroup[slGroup][i].m_bOnline == FALSE)
				{
					m_ExpeditionGroup[slGroup][i].m_bOnline = TRUE;

					m_UIExpGroup[slGroup].GetMemberBtn(i).SetOnline(TRUE);
					m_UIExpManage.GetMemberBtn(slGroup, i).SetOnline(TRUE);
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

				int nHPBarWidth = m_ExpeditionGroup[slGroup][i].CalcHPBarWidth(EXPEDITION_GROUP_BARWIDTH);

				if(bIsLevelChanged)
				{
					// ������ �ö��ٸ� ���, ����â�� ������ ������Ʈ�Ͽ� �ش�
					m_UIExpGroup[slGroup].GetMemberBtn(i).SetExpeditionCharLevel(m_ExpeditionGroup[slGroup][i].m_nLevel);
					m_UIExpManage.GetMemberBtn(slGroup, i).SetExpeditionCharLevel(m_ExpeditionGroup[slGroup][i].m_nLevel);
				}

				m_UIExpGroup[slGroup].GetMemberBtn(i).SetBarWidth(nHPBarWidth);

				m_UIExpManage.GetMemberBtn(slGroup, i).SetBarWidth(nHPBarWidth);
			}
			else // ��������
			{
				m_ExpeditionGroup[slGroup][i].m_bOnline = FALSE;

				m_UIExpGroup[slGroup].GetMemberBtn(i).SetOnline(FALSE);
				m_UIExpManage.GetMemberBtn(slGroup, i).SetOnline(FALSE);
			}

			break;
		}
	}
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMyPosition
// Explain: [sora] �� ��å��ȯ
//------------------------------------------------------------------------------	
int CUIParty::GetExpeditionMyPosition()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION) == 0 )
		return -1;

	if(m_nMyGroupNum < 0 )
		return -1;

	if(m_nGroupPos < 0 || m_nGroupPos >= EXPEDITION_MEMBER_PER_GROUP)
		return -1;
	
	return m_ExpeditionGroup[m_nMyGroupNum][m_nGroupPos].m_nPosition;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberIndex
// Explain: [sora] �ش� �׷� ������ ����� index���� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------	
SLONG CUIParty::GetExpeditionMemberIndex(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_slIndex;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberIndex
// Explain: [sora] �ش� �׷� ������ ����� index���� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------	
SLONG CUIParty::GetExpeditionMemberIndex(CTString strName)
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

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberZone
// Explain: [sora] �ش� �׷� ������ ����� Zone���� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------								
SLONG CUIParty::GetExpeditionMemberZone(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_slZone;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberPosX
// Explain: [sora] �ش� �׷� ������ ����� X���� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------								
FLOAT CUIParty::GetExpeditionMemberPosX(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_fPosX;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberPosZ
// Explain: [sora] �ش� �׷� ������ ����� Z���� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------									
FLOAT CUIParty::GetExpeditionMemberPosZ(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_fPosZ;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberLayer
// Explain: [sora] �ش� �׷� ������ ����� Layer���� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------								
SBYTE CUIParty::GetExpeditionMemberLayer(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_sbLayer;
}

//------------------------------------------------------------------------------
// CUIParty::GetExpeditionMemberOnline
// Explain: [sora] �ش� �׷� ������ ����� �¶��� ����Ȯ��
//------------------------------------------------------------------------------
BOOL CUIParty::GetExpeditionMemberOnline(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_bOnline;
}

//------------------------------------------------------------------------------
// CUIParty::SearchExpeditionMember
// Explain: [sora] �ε����� �̿��ؼ� �ش� ����� �׷�� ��ġ�� ã�´�. 
//------------------------------------------------------------------------------								
bool CUIParty::SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos)
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

// ----------------------------------------------------------------------------
// Name : GetJobStringColor()
// Desc : [sora] ������ �ؽ�Ʈ ������ ��ȯ
// ----------------------------------------------------------------------------
COLOR CUIParty::GetJobStringColor(SBYTE sbJob)
{
	if(sbJob < 0 || sbJob >= TOTAL_JOB)
		return 0xFFFFFFFF;
	
	return jobStringCol[sbJob];
}


// ----------------------------------------------------------------------------
// Name : GetDivisionType()
// Desc : [sora] ������ �й�Ÿ���� ���� ��ȯ
// ----------------------------------------------------------------------------
SBYTE CUIParty::GetDivisionType(int nType)
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

// ----------------------------------------------------------------------------
// Name : OpenDivisionInfoWindow()
// Desc : [sora] ��Ƽ, �������� �й� ����â�� ����
// ----------------------------------------------------------------------------
void CUIParty::OpenDivisionInfoWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// ����â�� ������ ��쿡�� ó������ �ʴ´�.
	if( pUIManager->DoesMessageBoxExist(MSGCMD_DIVISION_INFO) == TRUE )
	{
		return;
	}
	
	CUIMsgBox_Info MsgBoxInfo;
	CTString strTemp;

	MsgBoxInfo.SetMsgBoxInfo(_S(4570, "�й��� ����"), UMBS_OK, UI_PARTY, MSGCMD_DIVISION_INFO);

	CTString strTemp2;
	strTemp2 = m_strExpeditionType[m_sbDivisionTypeEXP];

	if(pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		if(m_sbDivisionTypeEXP == MSG_EXPED_TYPE_RANDOM)
		{
			strTemp2 = _S(4525, "�յ�й�");
		}

		strTemp.PrintF(_s("%s : %s"),m_strDivisionType[MSG_DIVITYPE_EXP], strTemp2);
	}
	else if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		CTString strPartyType[3]; 
		strPartyType[PT_PEACEEVER] = _S(4525, "�յ�й�");
		strPartyType[PT_SURVIVAL] = m_strExpeditionType[MSG_EXPED_TYPE_FIRSTGET]; //_s("�Լ��켱")
		strPartyType[PT_ATTACK] = m_strExpeditionType[MSG_EXPED_TYPE_BATTLE] ; // _s("������");

		strTemp.PrintF(_S(4571, "��Ƽ ����: %s"), strPartyType[m_nType]);
	}
	// ����ġ �й� Ÿ�� (��Ƽ������ ��Ƽ Ÿ��)
	MsgBoxInfo.AddString(strTemp);

	// ������ �й� Ÿ��
	strTemp.PrintF(_s("%s : %s"), m_strDivisionType[MSG_DIVITYPE_ITEM], m_strExpeditionType[m_sbDivisionTypeITEM]);
	MsgBoxInfo.AddString(strTemp);

	// ����� ������ �й� Ÿ��
	strTemp.PrintF(_s("%s : %s"), m_strDivisionType[MSG_DIVITYPE_SPECIAL], m_strExpeditionType[m_sbDivisionTypeSPITEM]);
	MsgBoxInfo.AddString(strTemp);

	pUIManager->CreateMessageBox( MsgBoxInfo );
}

//------------------------------------------------------------------------------
// CUIParty::IsExpedetionMember
// Explain: [sora] �ش� �ε����� ������ ������� üũ�Ѵ�. 
//------------------------------------------------------------------------------	
bool CUIParty::IsExpedetionMember( INDEX iChaIndex )
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


//------------------------------------------------------------------------------
// CUIParty::IsExpedetionDataExist
// Explain: [sora] ������� ���� ���� üũ
//------------------------------------------------------------------------------	
bool CUIParty::IsExpedetionDataExist(int nGroup, int nPos)
{
	return (m_ExpeditionGroup[nGroup][nPos].IsEmpty() ? FALSE : TRUE);
}

//------------------------------------------------------------------------------
// CUIParty::ExpeditionMemberOnline
// Explain: [sora] ������� online����
//------------------------------------------------------------------------------	
void CUIParty::ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline )
{
	int nGroup = 0; 
	int nPos = 0;

	if(slIndex == _pNetwork->MyCharacterInfo.index)
	{
		ExpeditionEnd();
	}

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_ExpeditionGroup[nGroup][nPos].m_bOnline = bOnline;

		m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetOnline(bOnline);
		m_UIExpManage.GetMemberBtn(nGroup, nPos).SetOnline(bOnline);
	}
}


//------------------------------------------------------------------------------
// CUIParty::SetExpeditionPosition
// Explain:  [sora]
//------------------------------------------------------------------------------
void CUIParty::SetExpeditionPosition(SLONG slIndex, SLONG slPosition)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_ExpeditionGroup[nGroup][nPos].m_nPosition = slPosition;

		m_UIExpGroup[nGroup].GetMemberBtn(nPos).SetExpeditionPosition(slPosition);
		m_UIExpManage.GetMemberBtn(nGroup, nPos).SetExpeditionPosition(slPosition);

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
			int slMyPosition = CUIManager::getSingleton()->GetParty()->GetExpeditionMyPosition();
			int slMyGroup, slMyPos;
			SearchExpeditionMember(_pNetwork->MyCharacterInfo.index ,slMyGroup, slMyPos);

			if(slMyGroup == nGroup && slMyPos == nPos)
			{
				if(m_UIExpManage.IsVisible()) // �� �ڽ��� �Ϲݴ������ ����Ǿ��� ������ ����â�� �����ִٸ� �ݾ���
				{
					m_UIExpManage.SetVisible(FALSE);
				}
			}
		}
	}
}


//------------------------------------------------------------------------------
// CUIParty::ChangeExpeditionLeader
// Explain: [sora] �������� ����
//------------------------------------------------------------------------------
void CUIParty::ChangeExpeditionLeader(SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode )
{
	int nGroup = 0; 
	int nPos = 0;

	if( SearchExpeditionMember(slIndexDesc, nGroup, nPos) == false)
		return;

	SetExpeditionPosition(slIndexSrc, MSG_EXPED_MEMBERTYPE_NORMAL);
	SetExpeditionPosition(slIndexDesc, MSG_EXPED_MEMBERTYPE_BOSS);

	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S( 4493, "������" ), UMBS_OK, UI_NONE, MSGCMD_NULL );

	CTString strMessage;

	strMessage.PrintF(_S( 4573, "%s���� ���������� �Ǿ����ϴ�." ), m_ExpeditionGroup[nGroup][nPos].m_strName);

	MsgBoxInfo.AddString( strMessage );
	CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

	if(m_UIExpManage.IsVisible()) // �ڱⰡ ���������� �����Ͽ��ٸ� ����â�� �ݾ��ش�.
	{
		m_UIExpManage.SetVisible(FALSE);
	}
}

//------------------------------------------------------------------------------
// CUIParty::SendExpeditionViewDetail
// Explain: [sora] ���캸�� ��û
//------------------------------------------------------------------------------
void CUIParty::SendExpeditionViewDetail(SLONG slIndex, CTString strCharName)
{
	int nGroup = 0; 
	int nPos = 0;

	if(SearchExpeditionMember(slIndex, nGroup, nPos))
	{
		m_UIViewDetail.ClearViewDetail();
		m_UIViewDetail.SetCharInfo(strCharName);
		_pNetwork->ExpeditionViewDetailReq(nGroup, slIndex);
	}
}


//------------------------------------------------------------------------------
// CUIParty::SendChangeSubLeader
// Explain: [sora] ���� �δ��� �Ӹ�
//------------------------------------------------------------------------------
void CUIParty::SendChangeSubLeader(SLONG slIndex)
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
				pUIManager->GetChatting()->AddSysMessage( _S( 4574, "�׷쿡 �̹� �����δ����� �ֽ��ϴ�. �δ����Ӹ��� �����ϰ� �ٽ� �õ��� �ֽʽÿ�." ) );
				return;
			}
		}

		if(m_ExpeditionGroup[nGroup][nPos].m_nPosition == MSG_EXPED_MEMBERTYPE_BOSS)
		{
			// ���������� �δ����� �� �� ����
			pUIManager->GetChatting()->AddSysMessage( _S( 4575, "���������� �δ����� �� �� �����ϴ�." ) );
			return;
		}
	}

	_pNetwork->ExpeditionChangeSubLeaderReq(TRUE, slIndex);
}

//------------------------------------------------------------------------------
// CUIParty::SendTargetLabelSet
// Explain: [sora] ǥ�� set ��û
//------------------------------------------------------------------------------
void CUIParty::SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex)
{
	_pNetwork->ExpeditionSetLabelReq( sbTargetType, MSG_EXPED_SETLABEL_MODE_SET, slLabelIndex, slIndex);
}

//------------------------------------------------------------------------------
// CUIParty::SendTargetLabelReSet
// Explain: [sora] ǥ�� Reset ��û
//------------------------------------------------------------------------------
void CUIParty::SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex)
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
			ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

			if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
			{
				CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

				slLabel = pTarget->cha_Label;
			}
		}
		
	}
	else //sbTargetType == MOB
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

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

//------------------------------------------------------------------------------
// CUIParty::SetTargetLabel
// Explain: [sora] ǥ�� ����(���� appear�Ǿ��ִ� ��� ǥ�� ����( appear���� ���� ����� appear�� ǥ�İ��� ���� ) )
//------------------------------------------------------------------------------
void CUIParty::SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex)
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

		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
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
		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

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


//------------------------------------------------------------------------------
// CUIParty::SetViewDetail
// Explain: [sora] ���캸�� �޽��� ���� (ĳ���� ��������� ���������� ������, ������ ������ �����츦 ���)
//------------------------------------------------------------------------------
void CUIParty::SetViewDetail(SBYTE sbType, CNetworkMessage *istr)
{
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

			m_UIViewDetail.SetPetInfo(petinfo);
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
			
			m_UIViewDetail.SetWildPetInfo(wildPetInfo);
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

				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemInfo(-1, -1, index, uniIndex, Wearing);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemPlus(plus);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemFlag(flag);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemUsed(used);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemUsed2(used2);
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemCount(count);


				//���� �������϶�...
				if( pItemData->GetFlag() & ITEM_FLAG_RARE )
				{
					//�ɼ� ������ 0�̸� �̰��� ���������
					if( sbOptionCount ==0)
						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemRareIndex(0);
					//������ ����������̸�...
					else
					{
						//���� �ɼ� �ε���
						(*istr) >> sbOptionType;
						(*istr) >> lOptionLevel;
						
						LONG iRareIndex = lOptionLevel;

						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemRareIndex(iRareIndex);

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
								CItemRareOption* pItem = CItemRareOption::getData(iRareIndex);

								if (pItem == NULL)
									continue;

								if (pItem->GetIndex() < 0)
									continue;

								int OptionType = pItem->rareOption[iBit].OptionIdx;
								int OptionLevel = pItem->rareOption[iBit].OptionLevel;
								m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemOptionData(++sbOption, OptionType, OptionLevel, ORIGIN_VAR_DEFAULT );
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

						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemOptionData( sbOption, sbOptionType, lOptionLevel, lOriginOptionVar );
					}
				}
/*
				for(int j = 0; j < sbOptionCount; j++)
				{
					(*istr) >> sbOptionType;
					(*istr) >> lOptionLevel;
					
					m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemOptionData(j, sbOptionType, lOptionLevel);
				}
*/
				
				if ( pItemData->GetFlag() & ITEM_FLAG_ORIGIN)
				{
					SBYTE sbBelong, sbSkillcont;
					LONG lSkillIndex;
					SBYTE sbSkillLevel;

					(*istr) >> sbBelong;
					(*istr) >> sbSkillcont;

					m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemBelong(sbBelong);

					for (SBYTE sbSkillpos = 0; sbSkillpos < sbSkillcont; sbSkillpos++)
					{
						(*istr) >> lSkillIndex;
						(*istr) >> sbSkillLevel;

						m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemSkill(sbSkillpos, lSkillIndex, sbSkillLevel);
					}
				}

				ULONG	plus2;
				(*istr) >> plus2;
				m_UIViewDetail.GetViewDetailBtn(Wearing).SetItemPlus2(plus2);
				
			}

			m_UIViewDetail.SetVisible(TRUE);
		}
			break;
	}
}


//------------------------------------------------------------------------------
// CUIParty::IsLabeled
// Explain: [sora] �ش� Ÿ���� �ε����� ǥ���� �ֳ� üũ(������ �ش� �� ��ȣ ������ -1)
//------------------------------------------------------------------------------
int CUIParty::IsLabeled(SLONG slTargetType, SLONG slIndex)
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_EXPEDITION) == FALSE )
		return -1;

	if(slTargetType == MSG_EXPED_SETLABEL_TYPE_PC)
	{
		if(_pNetwork->MyCharacterInfo.index == slIndex)
		{
			return _pNetwork->MyCharacterInfo.slLabel;
		}

		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_CHARACTER)
		{
			CCharacterTarget* pTarget = static_cast< CCharacterTarget* >(pObject);

			return pTarget->cha_Label;
		}
	}
	else // mob
	{
		ObjectBase* pObject = ACTORMGR()->GetObject(slIndex);

		if (pObject != NULL && pObject->m_eType == eOBJ_MOB)
		{
			CMobTarget* pMT = static_cast< CMobTarget* >(pObject);

			return pMT->mob_Label;
		}
	}

	return -1;

}


//------------------------------------------------------------------------------
// CUIParty::ClearTargetMark
// Explain: [sora] �����Ǿ��� ǥ�� ��� ����
//------------------------------------------------------------------------------
void CUIParty::ClearTargetMark()
{
	// ĳ���� ���� ǥ�� ����
	_pNetwork->MyCharacterInfo.slLabel = -1;
	
	ACTORMGR()->ClearLabel();
}

//------------------------------------------------------------------------------
// CUIParty::SendPartyEnd
// Explain: [sora] ��Ƽ ���� �۽�
//------------------------------------------------------------------------------
void CUIParty::SendPartyEnd()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_PARTY) == FALSE )
		return;

	_pNetwork->PartyEndReq();
}


//------------------------------------------------------------------------------
// CUIParty::SendInitInZone
// Explain: [sora] �δ� �ʱ�ȭ ����
//------------------------------------------------------------------------------
void CUIParty::SendInitInZone()
{
	if( CUIManager::getSingleton()->IsCSFlagOn(CSF_PARTY) == FALSE)
		return;

	_pNetwork->InitInZoneReq();
}

//------------------------------------------------------------------------------
// CUIParty::OpenExpeditionMemberWindow
// Explain: [sora] ������ ���â OPEN & CLOSE
//------------------------------------------------------------------------------
void CUIParty::OpenExpeditionMemberWindow()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// ���̸� �ݱ�
	if( IsVisible() == TRUE)
	{
		pUIManager->RearrangeOrder(UI_PARTY, FALSE);
		return;
	}

	for(int i=0; i<EXPEDITION_GROUP_MAX; i++)
	{
		if(m_UIExpGroup[i].GetMemberCount() > 0)
			m_UIExpGroup[i].SetVisible(TRUE);
	}
	
	pUIManager->RearrangeOrder(UI_PARTY, TRUE);
}


//------------------------------------------------------------------------------
// CUIParty::OpenExpeditionManageWindow
// Explain: [sora] ������ ����â OPEN
//------------------------------------------------------------------------------
void CUIParty::OpenExpeditionManageWindow()
{
	m_UIExpManage.SetVisible(TRUE);

	CUIManager::getSingleton()->RearrangeOrder(UI_PARTY, TRUE);
}

BOOL CUIParty::IsExistOfflineMember()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY))
	{
		for( int iMem = 0; iMem < m_nMemberCount; iMem++ )
		{
			if(m_bOnline[iMem] == false)
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

void CUIParty::ClearPartyTarget(SLONG slTargetIndex )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->IsCSFlagOn(CSF_PARTY) ||  pUIManager->IsCSFlagOn(CSF_EXPEDITION))
	{
		for(int i=0; i<PARTY_MAX_MEMBER; ++i)
		{
			if( m_ultargetID[i] == slTargetIndex )
			{
				m_ultargetID[i] = 0;
				m_sbtargetType[i] =0;
			}
		}
	}
}

CTString CUIParty::GetExpeditionMemberName(int nGroup, int nPos)
{
	return m_ExpeditionGroup[nGroup][nPos].m_strName;
}
//////////////////////////////////////////////////////////////////////////