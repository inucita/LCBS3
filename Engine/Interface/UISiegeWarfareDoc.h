
#ifndef _UISIEGE_WARFARE_DOC_H
#define _UISIEGE_WARFARE_DOC_H

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Network/CharacterTarget.h>
// #include <Engine/Interface/UISiegeWarfare.h>
// #include <Engine/Interface/UISiegeWarfareNew.h> // WSS_NEW_GUILD_SYSTEM 070725
// #include <map>

#define SWS_NONE					0
#define SWS_SW_REQ					1
#define	SWS_MERCEAR_REQ				2
#define SWS_SET_SW_TIME				3
#define SWS_SW_INFO					4
#define SWS_SELECT_BATTLE			5
#define SWS_BATTLE					6
#define SWS_END						7
// WSS_DRATAN_SIEGEWARFARE 070727
#define SWS_APPLY_TOWER				8
#define SWS_UPGRADE_TOWER			9
#define	SWS_REPAIR_TOWER			10
#define	SWS_CONSENSUS_CRYSTAL		11
#define	SWS_WAIT_REBIRTH			12


// ���� ����
#define WCSF_NORMAL					0  // �Ϲ�
#define WCSF_WAR_FIELD				1  // ����
#define WCSF_WAR_CASTLE				2  // ����

class SGuild
{
public:
	int			m_nGuildIndex;
	CTString	m_strGuildName;
	int			m_nPoint;

	SGuild()
	{
		m_nGuildIndex = -1;
		//m_strGuildName;
		m_nPoint = 0;
	}

	void SetGuildInfo ( int nGuildIndex, CTString strGuildName, int nPoint = -1 )
	{
		m_nGuildIndex = nGuildIndex;
		m_strGuildName = strGuildName;
		if( nPoint != -1 ) m_nPoint = nPoint;
	}

};



//------------------------------------------------------------------------------
// CUISiegeWarfareDoc
// Explain:  
// Date : 2005-07-01,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
class CUISiegeWarfareDoc
{
	int						m_nUIState;					// UI���� ����
	int						m_nSWTime;					// ���� �ð�
	LONG					m_lLeftTime;				// ���� ���� �ð� 
	TIME					m_tmLeftTime;				// ���� ���� �ð� 

	SGuild					m_gdGuild[3];				// ���� 3���� ��� ����
	SGuild					m_gdDefGuild;				// �������

	int						m_nGuildPoint;
	int						m_nZoneIndex;
	
	// ���� ���� 
	int						m_nMonth, m_nDay, m_nHour, m_nMin;
	int						m_nGateState;
	
	std::map<SLONG, CEffectGroup *>		m_mapEG;
	std::map<SLONG, CEffectGroup *>		m_mapEGGuildMaster;
	std::map<SLONG, CEffectGroup *>		m_mapEGConsensus;   // WSS_DRATAN_SIEGEWARFARE 2007/10/14 ��������Ʈ
		
public:	

	friend CUISiegeWarfare;
	friend CUISiegeWarfareNew;
	
	CUISiegeWarfareDoc();
	~CUISiegeWarfareDoc();

	void Clear();

//�޴� �޼��� 
	// ���� �ð� Ȯ��
	void GetTime( int nGuildIndex, CTString strGuildName, int nMonth, int nDay, int nHour, int nMin );
	
	// ���� ���� �˸�(����)
	void GuildWarStart( int nZoneIndex, int nRemainSec );	

	// ������ ���� ���� �˸�(����)
	void StartCastle( int nZoneIndex, int nRemainSec, int GuildIndex1, CTString GuildName1, int GuildIndex2, CTString GuildName2, int GuildIndex3, CTString GuildName3 );

	// ������ �������� �޴� �޼���
	void UpdatePoint( int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint );
		
	// ���� ���� 5�������� 1�а������� ����(����)
	void RemainFiledTime( int nZoneIndex, int nRemainSec );

// ���� ���ӽ� �޴� �޼���
	// ���� �ð� Ȯ�� �˸�(����)
	void TimeConfirm( int nZoneIndex, int nMonth, int nDay, int nHour, int nMin );
	
	// ���� ���۱��� ���� �ð� �˸�(����) - ���� 10���� ���� 1�а������� ����
	void RemainStartTime( int nZoneIndex, int nMin );
	
	// ���� ��Ȳ �˸�(����) - ���� ��Ȳ ����
	void CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec, SGuild Guild1, SGuild Guild2, SGuild Guild3, int nMyPoint, int nDefensPoint );
	// WSS_DRATAN_SIEGEWARFARE 070725
	void CastleState( int nZoneIndex, int nState, int nGateState, int nRemainSec);

	void EndWar( int nZoneIndex, int nWinDefense, int nOwnerGuildIndex, CTString strOwnerGuildName, int nOwnerIndex, CTString strOwnerName, int nNextMonth, int nNextDay, int nNextWeek, int nNextHour );
	void SetTimeRep( int nMonth, int nDay, int nHour, int nMin );
	
	void InvalidCommand( int nReqMsgType );

	// ���� ó�� �ڵ�
	void ErrorProc( int nErrorCode, UBYTE ubMsgType = -1 );

	
// Effect 
	void StartEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlagMerac, int nGuildIndex );
	void StopEffect( SLONG slCharIndex,  bool bAllStop = FALSE );
	void RestartEffect(int nZoneIndex);

	void StartGuildMasterEffect( SLONG slCharIndex, CEntity* penEntity, SBYTE sbJoinFlagMerac,  int nGuildIndex, LONG lGuildPosition );
	void StopGuildMasterEffect( SLONG slCharIndex,  bool bAllStop = FALSE );
	void RestartGuildMasterEffect();

	// WSS_DRATAN_SIEGEWARFARE 2007/10/14
	void StartConsensusEffect( SLONG slCharIndex, CEntity* penEntity);
	void StopConsensusEffect( SLONG slCharIndex,  bool bAllStop = FALSE );

// Etc
	// ���� �ð� ���� �Լ�
	void SetLeftTime ( int nRemainSec );

// UI���� ����
	ENGINE_API void SetUIState( int nUIState );

	// WSS_DRATAN_SIEGEWARFARE 070727
	inline int GetUIState() {return m_nUIState;}

	BOOL IsUIState( int nUIState )
	{
		return ( m_nUIState == nUIState )?TRUE:FALSE;
	}

	BOOL IsWar() // ������ ��ü ������
	{
		if( IsBattle() || IsSelectBattle() ) return TRUE;
		return FALSE;
	}

	BOOL IsBattle() // ������
	{
		return ( IsUIState( SWS_BATTLE ) )?TRUE:FALSE;
	}

	BOOL IsSelectBattle() // ������
	{
		return ( IsUIState( SWS_SELECT_BATTLE ) )?TRUE:FALSE;
	}

	BOOL IsSetSWTime()
	{
		return ( IsUIState( SWS_SET_SW_TIME ) )?TRUE:FALSE;
	}
	
	void SetGateState( int nOldGatesate, int nGateState );
	
	CTString GetCastleName( int nZoneIndex );

// Dealy�� ���� 
	void SetDealy( BOOL bStart = TRUE );
	void RunDalyTime();
	
	bool m_bDelay;
	TIME m_tmNoticeTime;
};


ENGINE_API extern CUISiegeWarfareDoc* _pUISWDoc;

#endif // _UISIEGE_WARFARE_DOC_H
