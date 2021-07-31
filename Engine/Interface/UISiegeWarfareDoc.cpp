#include "stdh.h"


#include <map>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UISiegeWarfareDoc.h>
#include <Engine/Ska/Render.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UISignboard.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Interface/UISiegeWarfare.h>
#include <Engine/Interface/UISiegeWarfareNew.h>
#include <Engine/Interface/UIMap.h>
#include <Engine/Interface/UISelectResource.h>
#include <Engine/Interface/UIGuild.h>
#include <Engine/Object/ActorMgr.h>


#define MUSIC_CASTLE_BATTLE		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_battletheme.ogg")
#define MUSIC_MERAC_FIELD		CTFILENAME("Data\\Sounds\\Game\\BGM\\GB_meracpease.ogg")

// ��ü ���� �ð� �߿� ���� ������ ����Ǵ� ���� �ð� 
// ������ ��ü ���� �ð� 60�� ���� ���� �ð� 15 + ���� �ð� 45������ �Ǿ��ֱ� ������ 
// CASTLETIME�� 45�� ���� �Ǿ�� ������ ���� �׽�Ʈ�� ���ؼ� 
// ��ü ���� �ð� 20 = ���� 10 + ���� 10���� �����Ǿ� �ְ� ������ 10���� ���� �Ͽ� ������. 
// ������ �ð� ������ ���� �ٶ�.
#define	CASTLETIME			45 // �� 

// ���� ���۰� ���� ���۽� Dealy �ð�
#define	DEALY_TIME			3500 //��	 

CUISiegeWarfareDoc* _pUISWDoc = NULL;

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::CUISiegeWarfareDoc
// Explain:  
// Date : 2005-06-20,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISiegeWarfareDoc::CUISiegeWarfareDoc()
{
	Clear();
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::~CUISiegeWarfareDoc
// Explain:  
// Date : 2005-07-01,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUISiegeWarfareDoc::~CUISiegeWarfareDoc()
{
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::Clear
// Explain:  
// Date : 2005-07-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::Clear()
{
	m_nUIState		= SWS_NONE;
	m_nSWTime		= 0;	
	m_lLeftTime		= 0;
	m_tmLeftTime	= 0;
	
	for( int i = 0; i < 3; i++ )
		m_gdGuild[i].SetGuildInfo( -1, " ", 0 );
	
	m_nGuildPoint = 0;
	
	m_nMonth = 0; m_nDay = 0; m_nHour = 0; m_nMin = 0;	
	
	m_mapEG.clear();
	m_mapEGGuildMaster.clear();
	m_mapEGConsensus.clear();  // WSS_DRATAN_SIEGEWARFARE 2007/10/14
	m_bDelay = false;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetUIState
// Explain:  UI ���¸� �����մϴ�.
// �������� ��쿡�� CNetwork�� ga_bGuildWar�÷��׸� �����մϴ�.
// Date : 2005-07-14,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetUIState( int nUIState ) 
{
	_pNetwork->ga_bGuildWar = FALSE;
	
	if( nUIState == SWS_SELECT_BATTLE || nUIState == SWS_BATTLE )
	{
		_pNetwork->ga_bGuildWar = TRUE;
	}
	
	m_nUIState = nUIState;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::GetTime
// Explain:  �������� ��û�� UI�� ���� ���� ���
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::GetTime( int nGuildIndex, CTString strGuildName, int nMonth, int nDay, int nHour, int nMin )
{
	m_gdDefGuild.SetGuildInfo( nGuildIndex, strGuildName );
	
	m_nMonth			= nMonth;
	m_nDay				= nDay;
	m_nHour				= nHour;
	m_nMin				= nMin;
	
	CUIManager::getSingleton()->GetSiegeWarfare()->OpenSWInfo();
}


//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::GuildWarStart
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::GuildWarStart( int nZoneIndex,  int nRemainSec )	
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(nZoneIndex == 7)
	{
		// FIXME : �� �ݾ��ֱ�.	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, FALSE );	

		if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
			return;	

		if( g_slZone == 7 )
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_CASTLE_BATTLE );

		Clear();
		
		_pNetwork->SendCastleMapRecent();

		m_nZoneIndex = nZoneIndex;
			
		CTString strMessage,  strMessage2;
		
		strMessage.PrintF(_S(2012, "[%s]���� [%s]������ ���۵˴ϴ�." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ));	
		strMessage2.PrintF(_S(2013,  "[%s]���� ���� �ִ� ���� ���� �������� ���� ���� �������� �̵��մϴ�."), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ));	

		// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
		// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
		pUIManager->GetSiegeWarfare()->SetNotice( strMessage, strMessage2 );
		pUIManager->GetSiegeWarfare()->OpenSelectBattle();
		
		RestartEffect(nZoneIndex);
		RestartGuildMasterEffect();
		SetLeftTime( nRemainSec );

		m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();


		//SetDealy();	
	}
	else if( nZoneIndex == 4)
	{
		// WSS_DRATAN_SIEGEWARFARE 0070725
		// TODO :: ���ź ���� ���� ��ƾ
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, FALSE );	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, FALSE );
		CTString strMessage;
		strMessage.PrintF( _S( 2012, "[%s]���� ������ ���۵˴ϴ�." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ));	
		// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
		// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
		pUIManager->GetSiegeWarfare()->SetNotice( strMessage );

		m_nZoneIndex = nZoneIndex;
		RestartEffect(nZoneIndex);
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartCastle
// Explain: ������ ���� ���� �˸�(����) 
// Date : !2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartCastle( int nZoneIndex, int nRemainSec, int GuildIndex1, CTString GuildName1, int GuildIndex2, CTString GuildName2, int GuildIndex3, CTString GuildName3 )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(nZoneIndex == 7)
	{	
		// ������ ���۵Ǹ� ��� ���� �ݾ� �д�.
		// FIXME : �� �ݾ��ֱ�.	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, FALSE );	

		if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
			return;

		if( g_slZone == 7 )
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_CASTLE_BATTLE );

		_pNetwork->SendCastleMapRecent();
		
		m_nZoneIndex = nZoneIndex;
		
		m_gdGuild[0].SetGuildInfo( GuildIndex1, GuildName1 );	
		m_gdGuild[1].SetGuildInfo( GuildIndex2, GuildName2 );
		m_gdGuild[2].SetGuildInfo( GuildIndex3, GuildName3 );
		
		CTString strMessage1, strMessage2;
		for( int i = 0; i< 3; i++ )
		{
			if( m_gdGuild[i].m_nGuildIndex > 0 )
			{
				if( i >= 1 )
				{
					strMessage1 += ", ";
				}
				strMessage1 += m_gdGuild[i].m_strGuildName;
				strMessage1 += CTString(" ");
				strMessage1 += _S(75 , "���" );	
			}
		}
		
		strMessage1 += _S( 2014, "�� ������ ���� ���� �Ǿ����ϴ�." );	
		strMessage2.PrintF(_S( 2015,  "������ ���°� �� ���� �ʾҽ��ϴ�. �����Ͻʽÿ�." ) );		
		// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
		// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
		pUIManager->GetSiegeWarfare()->SetNotice( strMessage1, strMessage2 );
		
		pUIManager->GetSiegeWarfare()->OpenBattle();
		
		RestartEffect(nZoneIndex);
		RestartGuildMasterEffect();
		SetLeftTime( nRemainSec );

		//SetDealy();
	}
	else if(nZoneIndex ==4 )
	{
		// WSS_DRATAN_SIEGEWARFARE 0070725
		// TODO :: ���ź ���� ���� ��ƾ
		// ������ ���۵Ǹ� ��� ���� �ݾ� �д�.
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, FALSE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, FALSE );

		CTString strMessage =_S( 3684,"������ ���۵Ǿ����ϴ�." );		
		// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
		// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
		CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );
		pUIManager->GetSiegeWarfareNew()->SetWarState(TRUE);

		m_nZoneIndex = nZoneIndex;
		RestartEffect(nZoneIndex);
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::UpdatePoint
// Explain:  ���� �� ���ŵǴ� ����
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::UpdatePoint( int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint )
{
	m_gdGuild[0]	= Guild1;
	m_gdGuild[1]	= Guild2;
	m_gdGuild[2]	= Guild3;
	
	m_nGuildPoint = nMyPoint;
	m_gdDefGuild.m_nPoint = nDefensPoint;

	SetLeftTime( nRemainSec );
}

void CUISiegeWarfareDoc::SetLeftTime ( int nRemainSec )
{
	m_lLeftTime = nRemainSec;

	if( IsSelectBattle() )
	{
		m_lLeftTime -= MINUTE_TO_SEC( CASTLETIME );
	}

	m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::RemainFiledTime
// Explain: ���������� ����Ǳ� 5�������� 1�� �������� ������ ������ �������� ��ü���� 
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RemainFiledTime( int nZoneIndex, int nRemainSec )
{
	m_nZoneIndex	= nZoneIndex;
	SetLeftTime( nRemainSec );
	
	CTString strMessage;
	strMessage.PrintF(_S( 2016,  "[%s]���� [��������]�� %d�� ���ҽ��ϴ�." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), (m_lLeftTime) / 60);	
	// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
	// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
	CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::TimeConfirm
// Explain:  ���� �ð� Ȯ�� �˸�(����)
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::TimeConfirm( int nZoneIndex, int nMonth, int nDay, int nHour, int nMin )
{
	m_nZoneIndex = nZoneIndex;
	
	CTString strMessage;
	strMessage.PrintF( _S( 2017, "[%s]���� [%s]������ %d�� %d�� %d�÷� �����Ǿ����ϴ�." ),	
		CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), nMonth, nDay, nHour );	
	// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
	// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
	CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::RemainStartTime
// Explain: ���� ���� ���� ���� �ð��� ���� 10�������� 1�а������� ����
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RemainStartTime( int nZoneIndex, int nMin )
{
	m_nZoneIndex = nZoneIndex;
	
	CTString strMessage;
	
	strMessage.PrintF( _S( 2018, "[%s]���� [%s]������ %d�� ���ҽ��ϴ�." ), CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), nMin );	
	// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
	// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
	CUIManager::getSingleton()->GetSiegeWarfare()->SetNotice( strMessage );	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::CastleState
// Explain:  ���� ��Ȳ �˸�(����) �� ������Ȳ ����
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint )
{
	// Date : 2005-07-22(���� 10:00:32), By Lee Ki-hwan 
	// �� �������� ��쿡�� 
	SetGateState( 0, nGateState ); 
	
	if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
		return;
	
	m_nZoneIndex	= nZoneIndex;
		
	m_gdGuild[0]	= Guild1;
	m_gdGuild[1]	= Guild2;
	m_gdGuild[2]	= Guild3;
	
	m_nGuildPoint	= nMyPoint;
	m_gdDefGuild.m_nPoint = nDefensPoint;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( nState == WCSF_WAR_FIELD )
	{
		pUIManager->GetSiegeWarfare()->OpenSelectBattle();
	}
	else if( nState == WCSF_WAR_CASTLE )
	{
		pUIManager->GetSiegeWarfare()->OpenBattle();
	}
	else
	{
		return;
	}
	
	SetLeftTime( nRemainSec );
	RestartEffect(nZoneIndex);
	RestartGuildMasterEffect();
}


//------------------------------------------------------------------------------
// WSS_DRATAN_SIEGEWARFARE 070725
// Explain:  ���� ��Ȳ �˸�(����) �� ������Ȳ ���� for Dratan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec)
{
	SetGateState( 0, nGateState ); 
	
	if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_NONE )
		return;
	
	m_nZoneIndex	= nZoneIndex;
		
	if( nState == WCSF_WAR_CASTLE )
	{
		// ������ ����
		CUIManager::getSingleton()->GetSiegeWarfareNew()->SetWarState(TRUE);
	}
	else return;
	
	SetLeftTime( nRemainSec );
	RestartEffect(nZoneIndex);
	RestartGuildMasterEffect();
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::EndWar
// Explain:  �޼��� ǥ���� ���� ����
// Date : 2005-07-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::EndWar( int nZoneIndex, int nWinDefense, int nOwnerGuildIndex, CTString strOwnerGuildName, int nOwnerIndex, CTString strOwnerName, int nNextMonth, int nNextDay, int nNextWeek, int nNextHour )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	INDEX	i;
	if( nZoneIndex == 7 )
	{
		SetUIState( SWS_END );	
		// ������ ������ ��� ���� ���� �д�.
		// FIXME : �� �����ֱ�.	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, TRUE );

		if( _pNetwork->MyCharacterInfo.sbJoinFlagMerac == WCJF_NONE )
			return;	

		if( g_slZone == 7 )
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayBGM( MUSIC_MERAC_FIELD );	

		// �̹��� ��� �߰� 
		_UISignBoard->ShowSingBoard ( 12, 7 );
			
		m_tmLeftTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		_pNetwork->MyCharacterInfo.sbJoinFlagMerac = WCJF_NONE;
		
		// Start My Guild, Enemy Guild Effect Start
		ACTORMGR()->SetJoinFlagMerac(WCJF_NONE);
	}
	else if( nZoneIndex == 4)
	{		
		// WSS_DRATAN_SEIGEWARFARE 2007/10/1 -------------------------------->>
		// ���� ���� ���� ����...
		SetUIState( SWS_END ); // WSS_DRATAN_SEIGEWARFARE 2007/10/11 
		
		if( _pNetwork->MyCharacterInfo.sbJoinFlagDratan == WCJF_NONE )
			return;	
		// WSS_DRATAN_SIEGEWARFARE 2007/10/16
		//_pNetwork->MyCharacterInfo.sbJoinFlagDratan = WCJF_NONE;
		
		// Start My Guild, Enemy Guild Effect Start
		ACTORMGR()->SetJoinFlagMerac(WCJF_NONE);

		// -------------------------------------------------------------------<<

		// WSS_DRATAN_SIEGEWARFARE 0070725
		// TODO :: ���ź ���� ���� ��ƾ
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, TRUE );
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, TRUE );	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, TRUE );	
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, TRUE );	

		pUIManager->GetSiegeWarfareNew()->SetWarState(FALSE);

		// WSS_DRATAN_SEIGEWARFARE 2007/08/14 -------------------------------->>
		// 390 ~ 399 ���� ��Ȱ���� �ʱ�ȭ
		CTString tStr;
		for( i = 0; i < 10; i++)
		{	
			INDEX tNpcIdx = 390+i;
			_pNetwork->MyCharacterInfo.mQuarter[tNpcIdx] = -1;
			_pNetwork->MyCharacterInfo.mQuarterName[tNpcIdx] = CTString("");

			// Hard Cording ^^;; ��¿�� ����....
			// �� �̸� ����
			CMobData* MD = CMobData::getData(tNpcIdx);
			if(MD->GetMobIndex()>0)
			{
				tStr.PrintF(_S( 3685,"���� ��Ȱ����%d"),tNpcIdx-389);
				MD->SetName(tStr);
			}		
		}	
		// -------------------------------------------------------------------<<

		if( pUIManager->GetSiegeWarfareNew()->IsEnabled()&&
			pUIManager->GetSiegeWarfareNew()->IsVisible() )
		{
			pUIManager->RearrangeOrder(UI_SIEGE_WARFARE_NEW,FALSE);
		}
		
		// WSS_DRATAN_SIEGEWARFARE 2007/10/17
		// ������ ���� ���� ó��
		if( _pNetwork->MyCharacterInfo.bConsensus )
		{
			pUIManager->GetSiegeWarfareNew()->StopConsensus(_pNetwork->MyCharacterInfo.index);
		}			
		StopConsensusEffect(0,TRUE);
		
		// WSS_DRATAN_SIEGEWARFARE 2007/10/18
		// ���� �ʱ�ȭ
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->PlayerInit(false);

	}

	m_nZoneIndex = nZoneIndex;
	
	CTString strMessage1;
	CTString strMessage2;
	
	// ���� ������ ������ �ʱ�ȭ 
	pUIManager->GetMap()->SetCurrentWorldMap( _pNetwork->MyCharacterInfo.zoneNo, 0 );
	pUIManager->GetMap()->ReSetData();

	if( nWinDefense )
	{
		if( strOwnerGuildName.Length() <= 0 )
		{
			strMessage1.PrintF( _S( 2019, "[%s]���� [%s]������ ���� �Ͽ����ϴ�." ), 
			CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ) );	
		}
		else
		{
			strMessage1.PrintF( _S(2020 , "[%s]���� [%s]������ ����Ǿ����ϴ�. [%s]��尡 ������ �����Ͽ����ϴ�." ),
			CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), strOwnerGuildName );		
		}
		
	}
	else
	{
		strMessage1.PrintF( _S(2021 , "[%s]���� [%s]������ ����Ǿ����ϴ�. ���ο� ���ַ� [%s]����� [%s]������� ���� �Ǿ����ϴ�." ), 
			CZoneInfo::getSingleton()->GetZoneName( nZoneIndex ), GetCastleName( nZoneIndex ), strOwnerGuildName, strOwnerName );	
	}
	
	strMessage2.PrintF(_S( 2022, "���� ������ ��û�� %d�� %d�� %s���� ���� %d�ú��� ��û�� �����մϴ�." ), 
		nNextMonth, nNextDay, pUIManager->GetWeekToString( nNextWeek ), nNextHour );	
	// [2012/06/04 : Sora] ITS 8986 ���������� �������� �������� �̵� �� ĳ���� �̵��Ұ� ���� �߻� ����
	// ���� �޽��� ����ϸ鼭 �ش� UI�� Ȱ��ȭ �����־��� �Ѵ�. ���� �޽��� ��� ��� ����
	pUIManager->GetSiegeWarfare()->SetNotice( strMessage1, strMessage2 );
	
	// Date : 2005-11-18(���� 4:27:13), By Lee Ki-hwan
	// �޼��� �ڽ� ǥ�� �߰� 
	pUIManager->CloseMessageBox( MSGCMD_NULL );
	
	CUIMsgBox_Info	MsgBoxInfo;
	MsgBoxInfo.SetMsgBoxInfo( _S(2341, "���� ����" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
	MsgBoxInfo.AddString( strMessage1 );
	pUIManager->CreateMessageBox( MsgBoxInfo );
		
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetTimeRep
// Explain:  
// Date : 2005-07-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetTimeRep( int nMonth, int nDay, int nHour, int nMin )
{
	CTString strMessage;
	strMessage.PrintF(_S(2147 ,  "���� �ð��� %d�� %d�� %d�÷� �����Ǿ����ϴ�." ), nMonth, nDay, nHour );  
	CUIManager::getSingleton()->GetSiegeWarfare()->Message( MSGCMD_SIEGE_WARFARE_INFO, strMessage, UMBS_OK  );
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  
// Date : 2005-07-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::InvalidCommand( int nReqMsgType )
{
	static int nMaxMsgType = 17;
	int nReqMsg = nMaxMsgType;
	
	
	static  int naReqMsgType[17] = { 
		MSG_GUILD_CREATE,				// 0
			MSG_GUILD_LEVELUP,
			MSG_GUILD_BREAKUP,
			MSG_GUILD_REGIST_REQ,
			MSG_GUILD_REGIST_ALLOW,
			MSG_GUILD_REGIST_CANCEL,		// 5
			MSG_GUILD_OUT_REQ,
			MSG_GUILD_KICK,
			MSG_GUILD_CHANGE_BOSS,
			MSG_GUILD_APPOINT_OFFICER,		// 9
			MSG_GUILD_FIRE_OFFICER,
			MSG_GUILD_BATTLE_REQ_REQ,
			MSG_GUILD_BATTLE_REQ_REJECT,
			MSG_GUILD_BATTLE_REQ_ACCEPT,
			MSG_GUILD_BATTLE_STOP_REQ,
			MSG_GUILD_BATTLE_STOP_REJECT,	// 15
			MSG_GUILD_BATTLE_STOP_ACCEPT,	// 16
	};
	
	for( int i = 0; i < nMaxMsgType; i++ )
	{
		if( naReqMsgType[i] == nReqMsgType )
		{
			nReqMsg = i;
			break;
		}
	}
	
	
	static CTString strReqMsgType[] = {
	_S( 2023, 	"������ �߿��� ��带 ������ �� �����ϴ�." ),					// MSG_GUILD_CREATE			
		_S( 2024, 	"������ �߿��� ��带 �������� �� �����ϴ�." ),				// MSG_GUILD_LEVELUP	
		_S(2025 , 	"������ �߿��� ��鸦 ��ü�� �� �����ϴ�." ),					// MSG_GUILD_BREAKUP	
			_S( 2026, "������ �߿��� ��� ��� ��û�� �� �� �����ϴ�." ),			// MSG_GUILD_REGIST_REQ	
			_S(2027 , "������ �߿��� ��� ��� ������ �� �� �����ϴ�." ),			// MSG_GUILD_REGIST_ALLOW	
		_S( 2028, 	"������ �߿��� ��� ��� ��Ҹ� �� �� �����ϴ�." ),			// MSG_GUILD_REGIST_CANCEL	
			_S( 2029, "������ �߿��� ��� Ż�� ��û�� �� �� �����ϴ�." ),			// MSG_GUILD_OUT_REQ	
			_S( 2030, "������ �߿��� ��� ������ �� �� �����ϴ�." ),				// MSG_GUILD_KICK	
			_S( 2031, "������ �߿��� ������� ������ �� �����ϴ�." ),				// MSG_GUILD_CHANGE_BOSS	
		_S(2032 , 	"������ �߿��� �δ����� �Ӹ��� �� �����ϴ�." ),				// MSG_GUILD_APPOINT_OFFICER	
		_S( 2033, 	"������ �߿��� �δ����� ������ �� �����ϴ�." ),				// MSG_GUILD_FIRE_OFFICER	
			_S( 2034, "������ �߿��� ������� ��û�� �� �� �����ϴ�." ),			// MSG_GUILD_BATTLE_REQ_REQ	
			_S( 2035, "������ �߿��� ������� ��û�� ������ �� �����ϴ�." ),		// MSG_GUILD_BATTLE_REQ_REJECT	
			_S( 2036, "������ �߿��� ������� ��û�� ������ �� �����ϴ�." ),		// MSG_GUILD_BATTLE_REQ_ACCEPT	
		_S( 2037, 	"������ �߿��� ������� �ߴ� ��û�� �� �� �����ϴ�." ),		// MSG_GUILD_BATTLE_STOP_REQ	
			_S( 2038, "������ �߿��� ������� �ߴ� ��û�� ������ �� �����ϴ�." ),	// MSG_GUILD_BATTLE_STOP_REJECT	
		_S( 2039, 	"������ �߿��� ������� �ߴ� ��û�� ������ �� �����ϴ�." ),	// MSG_GUILD_BATTLE_STOP_ACCEPT	
			"UnKnow Message"
	};
	
	CUIManager::getSingleton()->GetSiegeWarfare()->ErrorMessage( strReqMsgType[nReqMsg-1] );
	
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetGateState
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetGateState( int nOldGatesate, int nGateState )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// 0, 1 ���ʹ�
	if (g_slZone == 7) //  �޶�ũ
	{
		if( (nGateState & ( 1 << 0 )) && (nGateState & ( 1 << 1 )))
		{		
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, TRUE );
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 0, FALSE );
		}
		
		// 2, 3 �����ʹ�
		if( (nGateState & ( 1 << 2 )) && (nGateState & ( 1 << 3 )))
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, TRUE );
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 1, FALSE );
		}
		
		// 4, 5 �߾ӹ�.
		if( (nGateState & ( 1 << 4 )) && (nGateState & ( 1 << 5 )))
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, TRUE );
			
			// �� ���� ����
			if (nOldGatesate != nGateState)
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 2040, "�������� ���������� �����Ͽ����ϴ�. �������� ���¸� ��ȣ�Ͻʽÿ�." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 2, FALSE );
		}
	}

// WSS_DRATAN_SIEGEWARFARE 070723
	// ���ź �ܹ� ������
	if (g_slZone == 4)	// ���ź
	{
		if( (nGateState & ( 1 << 6 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, TRUE );
			// �� ���� ����
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 �������̸�...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 3811,"�ܼ����� �ı��Ǿ����ϴ�." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 3, FALSE );
		}
		// ���ź ���� 2
		if( (nGateState & ( 1 << 7 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, TRUE );
			
			// �� ���� ����
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 �������̸�...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S(  3686,"�������� ���������� �����Ͽ����ϴ�. �������� ũ����Ż�� ��ȣ�Ͻʽÿ�." ) ); 
			}
			
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 6, FALSE );
		}
		// ���ź ���� 3
		if( (nGateState & ( 1 << 8 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, TRUE );
			
			// �� ���� ����
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 �������̸�...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S(  3686,"�������� ���������� �����Ͽ����ϴ�. �������� ũ����Ż�� ��ȣ�Ͻʽÿ�." ) ); 
			}
			
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 7, FALSE );
		}
		// ���ź �ܼ��� ����
		if( (nGateState & ( 1 << 9 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, TRUE );
			// �� ���� ����
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 �������̸�...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 3811,"�ܼ����� �ı��Ǿ����ϴ�." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 4, FALSE );
		}
		// ���ź �ܼ��� ���
		if( (nGateState & ( 1 << 10 )) )
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, TRUE );
			// �� ���� ����
			if(pUIManager->GetSelectResource()->GetWndState() && nOldGatesate != nGateState) // WSS_DRATAN_SIEGEWARFARE 071008 �������̸�...
			{
				pUIManager->GetChattingUI()->AddSysMessage( _S( 3811,"�ܼ����� �ı��Ǿ����ϴ�." ) ); 
			}
		}
		else
		{
			((CPlayerEntity*)CEntity::GetPlayerEntity(0))->OpenGate( 5, FALSE );
		}
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  ������ ���� ���̸� ... �ϵ��ڵ�
// Date : 2005-07-07,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CTString CUISiegeWarfareDoc::GetCastleName( int nZoneIndex )
{	
	CTString strCastleName = _S( 2041, "��������" );	
	switch( nZoneIndex )
	{
	case 4://���ź // WSS_DRATAN_SIEGEWARFARE 070723
		strCastleName = _S(  3687, "ī����" ); 
		break;
	case 7://�޶�ũ
		strCastleName = _S( 2042, "ĭ��Ʈ" ); // �̸� ���� Ȯ�� �� ����(20)�� ����
		break;
	}
	
	return strCastleName;
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::ErrorProc
// Explain:  
// Date : 2005-07-06,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::ErrorProc( int nErrorCode, UBYTE ubMsgType )
{
	static CTString strErrorMessage[MSG_GUILD_WAR_ERROR_TOTAL]	= 
	{
	_S( 2043, 	"�� �� �Ƴ׿�" ),	
		_S( 2044, 	"�̹� ������ ������ �Դϴ�." ),		
		_S( 2045, 	"������ ���� ���� ��û�� �� �� �����ϴ�." ),	
			_S( 2046, "��忡 ���ԵǾ� ���� ������ ���� ��û�� �� �� �����ϴ�." ),	
			_S( 2047, "�̹� ����/���� ��û�� �Ǿ��ֽ��ϴ�." ),	
			_S( 2048, "���� ���� �ð��� �ƴմϴ�." ),	
			_S( 2049, "��� ���� �����Ͽ� ���� ��û�� �� �� �����ϴ�." ),	
			_S( 2050, "��� �ο��� �����Ͽ� ���� ��û�� �� �� �����ϴ�." ),	
			_S( 2051, "������ ��ǥ�� ������ ���� ��û�� �� �� �����ϴ�." ),	
			_S( 2052, "������ �����Ͽ� ���� ��û�� �� �� �����ϴ�." ),	
			_S( 2053, "���ְ� ���� ������ ���� �뺴 ��û�� �� �� �����ϴ�." ),	
			_S( 2054, "����� ��ħ�� ���� ���� �뺴 ��û�� �� �� �����ϴ�." ),	
			_S( 2055, "������ �����Ͽ� ���� �뺴 ��û�� �� �� �����ϴ�." ),	
			_S( 2056, "���ְ� �ƴϸ� ���� ������ ������ �� �����ϴ�." ),	
			_S( 2057, "���� ���� �ð��� �ƴմϴ�." ),	
			_S( 2057, "���� ���� �ð��� �ƴմϴ�." ),
			_S( 1688, "������ ���� �ʽ��ϴ�."),		
	};

	CUIManager* pUIManager = CUIManager::getSingleton();

	// ���� ó��
	if( nErrorCode == MSG_GUILD_ERROR_OK && ubMsgType != -1 )
	{
		CTString strMessage;
		switch ( ubMsgType )
		{
		case MSG_GUILD_WAR_JOIN_ATTACK_GUILD: // ������û �Ϸ�
			{
				strMessage = _S(2058 , "���� ��û�� �Ϸ�Ǿ����ϴ�." );		
			}
			break;
		case MSG_GUILD_WAR_JOIN_DEFENSE_GUILD: // ���� ��� ��û�Ϸ� 
			{
				strMessage = _S( 2059, "������ �뺴 ��� ��û�� �Ϸ�Ǿ����ϴ�." );	
			}
			break;
		case MSG_GUILD_WAR_JOIN_ATTACK_CHAR: // ���� �뺴 ��û �Ϸ� 
			{
				strMessage = _S( 2060, "������ �뺴 ��û�� �Ϸ�Ǿ����ϴ�." );	
			}
			break;
		case MSG_GUILD_WAR_JOIN_DEFENSE_CHAR: // ���� �뺴��û �Ϸ� 
			{
				strMessage =_S(2061 ,  "������ �뺴��û�� �Ϸ�Ǿ����ϴ�." );	
			}
			break;
			
		default : return;
		}
		
		pUIManager->GetSiegeWarfare()->Message( MSGCMD_SIEGE_WARFARE_INFO, strMessage, UMBS_OK );
	}
	else 
	{
		pUIManager->GetSiegeWarfare()->ErrorMessage( strErrorMessage[nErrorCode] );
	}
}
//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartEffect
// Explain:  
// Date : 2005-07-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlag, int nGuildIndex )
{
	CTString strEffectName;
	
	// �޶�ũ
	if( IsWar() )
	{	
		if( m_mapEG.find( slCharIndex ) != m_mapEG.end() ) return; // �̹� ���� �ϴ°�?
		if( sbJoinFlag == WCJF_NONE || sbJoinFlag <= -1 ) return;
		if( slCharIndex == -1 ) return;
		if( penEntity == NULL ) return;
		
		// �뺴�� ��쿡��... ��� Index�� -1�� �ɴϴ�. �׷��� ���� üũ ����~
		//if( nGuildIndex == -1 ) return;
		
		switch( sbJoinFlag )
		{
			// �������
		case WCJF_OWNER:
			strEffectName = "cpp_gb_defens";
			break;
			
			// ���������
		case WCJF_DEFENSE_GUILD:
		case WCJF_DEFENSE_CHAR:
			strEffectName = "cpp_gb_blue";
			break;
			
			// ������ ���
		case WCJF_ATTACK_GUILD:
		case WCJF_ATTACK_CHAR:
			{
				strEffectName = "cpp_gb_red";
				if( nGuildIndex != -1 && nGuildIndex == _pNetwork->MyCharacterInfo.lGuildIndex )
				{
					strEffectName = "cpp_gb_guild";
					break;
				}

				if( IsBattle() ) // ������
				{
					for( int i =0 ; i < 3; i++ )
					{
						if( nGuildIndex == m_gdGuild[i].m_nGuildIndex )
						{
							strEffectName = "cpp_gb_attack";
							break;
						}
					}
				}
			}
			break;
			
		default :
			return;
		}
	}
	// WSS_DRATAN_SEIGEWARFARE 2007/08/30 --------------------------->>
	// ���ź
	else if( CUIManager::getSingleton()->GetSiegeWarfareNew()->GetWarState())
	{
		if( m_mapEG.find( slCharIndex ) != m_mapEG.end() ) return; // �̹� ���� �ϴ°�?
		if( sbJoinFlag == WCJF_NONE || sbJoinFlag <= -1 ) return;
		if( slCharIndex == -1 ) return;
		if( penEntity == NULL ) return;
		
		switch( sbJoinFlag )
		{
			// ���� ���
		case WCJF_OWNER:
		case WCJF_DEFENSE_GUILD: // WSS_DRATAN_SIEGEWARFARE 2007/10/1
		case WCJF_DEFENSE_CHAR:
			{
				if(_pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex) strEffectName = "DEFFEND_FORCE_OUR";	
				else strEffectName = "DEFFEND_FORCE_OTHER";
			}
			break;		
			
			// ���� ���
		case WCJF_ATTACK_GUILD:
			{
				if(_pNetwork->MyCharacterInfo.lGuildIndex == nGuildIndex) strEffectName = "ATTACK_FORCE_OUR";
				else strEffectName = "ATTACK_FORCE_OTHER";				
			}
			break;
			
		default :
			return;
		}

	}
	// ��Ÿ
	else return;
	// --------------------------------------------------------------<<

	
	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEG.insert(tmpVal);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StopEffect
// Explain:  
// Date : 2005-07-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StopEffect( SLONG slCharIndex, bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEG.begin(); itEG != m_mapEG.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEG.clear();
	}
	else
	{
		// �����Ͱ� Ȱ��ȭ �Ǿ� ���� �ʴٸ� ������..
		if( m_mapEG.find( slCharIndex ) == m_mapEG.end() ) return;
		
		if( m_mapEG[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEG[slCharIndex] ) )
			m_mapEG[slCharIndex]->Stop(0.0f);
		
		m_mapEG.erase( slCharIndex );
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  
// Date : 2005-07-13,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RestartEffect(int nZoneIndex)
{
	StopEffect( -1, TRUE );

	bool bJoinFlag = false;
	SBYTE sbJoinFlag = _pNetwork->MyCharacterInfo.sbJoinFlagMerac;

	if (nZoneIndex == 4)
	{
		bJoinFlag = true;
		sbJoinFlag = _pNetwork->MyCharacterInfo.sbJoinFlagDratan;
	}

	// Start Effect My Character
	StartEffect ( _pNetwork->MyCharacterInfo.index, CEntity::GetPlayerEntity(0), sbJoinFlag, _pNetwork->MyCharacterInfo.lGuildIndex );
	
	// Start My Guild, Enemy Guild Effect Start
	ACTORMGR()->RefreshSiegeWarfareEffect(bJoinFlag);
}


//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartEffect
// Explain:  
// Date : 2005-11-18(���� 3:49:15), By Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartGuildMasterEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlagMerac, int nGuildIndex, LONG lGuildPosition )
{
	CTString strEffectName = "GuildMaster";
	
	if( !IsWar() ) return; // �������� �ƴ϶�� ����~
	if( m_mapEGGuildMaster.find( slCharIndex ) != m_mapEGGuildMaster.end() ) return; // �̹� ���� �ϴ°�?
	
	if( sbJoinFlagMerac == WCJF_NONE || 
		sbJoinFlagMerac <= -1 || 
		sbJoinFlagMerac == WCJF_DEFENSE_CHAR ||
		 sbJoinFlagMerac == WCJF_ATTACK_CHAR  ) return;

	if( slCharIndex == -1 ) return;
	if( penEntity == NULL ) return;
	if( nGuildIndex == -1 ) return;
	if( lGuildPosition != GUILD_MEMBER_BOSS ) return;

	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEGGuildMaster.insert(tmpVal);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StopEffect
// Explain:  
// Date : 2005-11-18(���� 3:49:12), By Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StopGuildMasterEffect( SLONG slCharIndex, bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEGGuildMaster.begin(); itEG != m_mapEGGuildMaster.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEGGuildMaster.clear();
	}
	else
	{
		// �����Ͱ� Ȱ��ȭ �Ǿ� ���� �ʴٸ� ������..
		if( m_mapEGGuildMaster.find( slCharIndex ) == m_mapEGGuildMaster.end() ) return;
		
		if( m_mapEGGuildMaster[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEGGuildMaster[slCharIndex] ) )
			m_mapEGGuildMaster[slCharIndex]->Stop(0.0f);
		
		m_mapEGGuildMaster.erase( slCharIndex );
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::
// Explain:  
// Date : 2005-11-18(���� 3:49:10), By Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RestartGuildMasterEffect()
{
	StopGuildMasterEffect( -1, TRUE );

	StartGuildMasterEffect ( _pNetwork->MyCharacterInfo.index, CEntity::GetPlayerEntity(0), 
							_pNetwork->MyCharacterInfo.sbJoinFlagMerac, 
							_pNetwork->MyCharacterInfo.lGuildIndex, 
							_pNetwork->MyCharacterInfo.lGuildPosition );
		
	// Start My Guild, Enemy Guild Effect Start
	ACTORMGR()->RefreshGuildMasterEffect();
}


//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StartConsensusEffect
// WSS_DRATAN_SIEGEWARFARE 2007/10/14
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StartConsensusEffect( SLONG slCharIndex, CEntity* penEntity )
{
	CTString strEffectName = "JEWEL";	
	
	if( m_mapEGConsensus.find( slCharIndex ) != m_mapEGConsensus.end() ) return; // �̹� ���� �ϴ°�?

	if( slCharIndex == -1 ) return;
	if( penEntity == NULL ) return;

	// Start Effect
	CEffectGroup *pEG = StartEffectGroup( strEffectName
		, &penEntity->en_pmiModelInstance->m_tmSkaTagManager
		,_pTimer->GetLerpedCurrentTick());
	
	std::map<SLONG, CEffectGroup *>::value_type tmpVal( slCharIndex, pEG);
	m_mapEGConsensus.insert(tmpVal);
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::StopConsensusEffect
// WSS_DRATAN_SIEGEWARFARE 2007/10/14
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::StopConsensusEffect( SLONG slCharIndex, bool bAllStop )
{
	if( bAllStop )
	{
		// All Character Effect Stop
		std::map<SLONG, CEffectGroup *>::iterator	itEG;
		
		for( itEG = m_mapEGConsensus.begin(); itEG != m_mapEGConsensus.end(); ++itEG )
		{
			if( (*itEG).second != NULL && CEffectGroupManager::Instance().IsValidCreated( (*itEG).second ) )
				(*itEG).second->Stop(0.0f);
		}
		
		m_mapEGConsensus.clear();
	}
	else
	{
		// �����Ͱ� Ȱ��ȭ �Ǿ� ���� �ʴٸ� ������..
		if( m_mapEGConsensus.find( slCharIndex ) == m_mapEGConsensus.end() ) return;
		
		if( m_mapEGConsensus[slCharIndex] != NULL && CEffectGroupManager::Instance().IsValidCreated( m_mapEGConsensus[slCharIndex] ) )
			m_mapEGConsensus[slCharIndex]->Stop(0.0f);
		
		m_mapEGConsensus.erase( slCharIndex );
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::SetDealy
// Explain:  
// Date : 2005-08-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::SetDealy( BOOL bStart )
{
	CTString strMessage;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( bStart ) // Start
	{
		m_bDelay = true;
		m_tmNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();	

		// Set Stop Move
		pUIManager->SetCSFlagOn( CSF_CASTLE_WAR_READY );
				
		if( _pNetwork->m_ubGMLevel > 1 )
		{
			strMessage.PrintF( "INFO-START-Dealy (%d)", m_tmNoticeTime );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		}
		((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StopPlayer();
	}
	else // Stop
	{
		m_bDelay = false;
		
		// ReSet Stop Move
		pUIManager->SetCSFlagOff( CSF_CASTLE_WAR_READY );
		
		if( _pNetwork->m_ubGMLevel > 1 )
		{
			strMessage.PrintF( "INFO-STOP-Dealy" );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_ERROR );
		}
		
	}
}

//------------------------------------------------------------------------------
// CUISiegeWarfareDoc::RunDalyTime
// Explain:  CUISiegeWarfare�� Render���� ��� Time üũ 
// Date : 2005-08-08,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUISiegeWarfareDoc::RunDalyTime()
{
	if( !m_bDelay ) return;
	
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmNoticeTime;
	
	static __int64 IDealyTime = DEALY_TIME;	
	
	if( llCurDelay > IDealyTime )
	{
		SetDealy( FALSE ); // Stop
		llCurDelay = 0;
	}
	((CPlayerEntity*)CEntity::GetPlayerEntity(0))->StopPlayer();
}