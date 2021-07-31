#ifndef __PARTY__
#define __PARTY__

#include "CommunityDefines.h"

class ENGINE_API Party
{
public:
	Party(void);
	virtual ~Party(void);

	void		Init();
	void		InitParty();
	void		InitExpedition();
	void		ClearPartyTarget( SLONG slTargetIndex );
	void		ClearTargetMark();							// ǥ�� reset

	bool		SearchExpeditionMember(SLONG slIndex, int &nGroup, int &nPos);	// �ε����� ��������� �׷�, ��ġ�� �˻�
	void		UpdateMemberStatus( SLONG slIndex, int iHP, int iMaxHP, int iMP, int iMaxMP );
	void		TargetMember( int nIndex );
	void		MandateBossReq( CTString strManadateChaName );
	void		MandateBoss( CTString strBossName, SLONG nManadateChaIndex, CTString strManadateChaName, SBYTE sbMandate );
	// ��Ƽ ����
	void		AddPartyMember(SBYTE sbLeader, SLONG slIndex, CTString& strName, SBYTE sbJob, SBYTE sbJob2,
	                           int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
	                           FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone);
	void		RemoveMember(SLONG slIndex);
	void		PartyEnd();
	void		PartyInvite(SBYTE sbType, SLONG slSrcIndex, CTString& strSrcName, SBYTE sbPartyTypeItem = -1, SBYTE sbPartyTypeSPItem = -1);
	void		PartyReject(BOOL bLeader);
	void		PartyAddMember(SBYTE sbLeader, SLONG slIndex, CTString& strName, SBYTE sbJob, SBYTE sbJob2,
	                           int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
	                           FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone);
	void		PartyQuit(SLONG slIndex);
	void		PartyKick(SLONG slIndex);
	void		PartyError(SBYTE sbError);
	void		PartyMemberInfo(SLONG slIndex, int iLevel, int iHP, int iMaxHP,
	                            int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone, bool bOnline = true);
	// ������ ����
	void		PartyData2Expedition(SLONG slIndex, int nPos);			// ������ ��ȯ�� ��Ƽ�� �ִ���ġ�� �����뿡 ����
	void		ExpeditionReject( BOOL bIsSrc );						// ������ ���� �޽��� ����
	void		ExpeditionQuit( SLONG slType, SLONG slIndex );			// ������ Ż�� �޽��� ����
	void		ExpeditionKick( SLONG slIndex );						// ������ ���� �޽��� ����
	void		ExpeditionEnd();										// ������ ���� �޽��� ����
	void		ChangeGroup(SLONG slIndex, int nGroupNum, int nPosNum);	// ������� �׷� ���� ����
	void		AddExpeditionMember( int nGroupNum, int nPos, int nPosition, SLONG slIndex,			// ������� �߰�
		CTString &strName, SBYTE sbJob, SBYTE sbJob2, 
		int iLevel, int iHP, int iMaxHP, int iMP, int iMaxMP,
		FLOAT fX, FLOAT fZ, SBYTE sbLayer, SLONG slZone );

	void		AddExpeditionMember( int nGroupNum, int nPos, stExpeditionMember expMember);

	void		RemoveExpeditionMember( int nGroup, int nPos );			// ������� ����

	void		ExpeditionMemberInfo( SLONG slIndex, SLONG slGroup, int iLevel, int iHP, int iMaxHP,// ������� ���� ����
		int iMP, int iMaxMP, FLOAT fX, FLOAT fZ, SBYTE sbLayer, 
		SLONG slZone, bool bOnline = true );
	void		ExpeditionMemberOnline( SLONG slIndex, BOOL bOnline );
	void		SetExpeditionPosition(SLONG slIndex, SLONG slPosition);
	void		ChangeExpeditionLeader( SLONG slIndexSrc, SLONG slIndexDesc, SLONG slChangeMode );

	// Set Infomation
	void		SetPartyLeader();	// ��Ƽ ���� ����	
	void		SetTargetLabel(SLONG slType, SLONG slMode, SLONG slLabel, SLONG slIndex);	// ǥ�� ����
	void		SetPartyTarget(ULONG partyID, ULONG targetID, SBYTE sbtargetType);
	void		SetDivisionType(SBYTE sbDivisionType, SBYTE sbType, BOOL bFirst = FALSE);	// �й��� ����
	void		SetDivisionType(SBYTE sbDivisionTypeEXP, SBYTE sbDivisionTypeITEM, SBYTE sbDivisionTypeSPITEM);

	void		SetPartyType( SBYTE sbPartyType)		{ m_nType = sbPartyType;			} // ��Ƽ Ÿ�Լ���
	void		SetPartyRequested( BOOL IsRequested )	{ m_bIsPartyRequested = IsRequested;} // ��Ƽ ��û��
	void		SetIsPartyPlay(BOOL bIsPartyPlay)		{ m_bIsPartyPlay = bIsPartyPlay;	}
	void		SetDestName( CTString strDest )			{ m_strDestName = strDest;			}
	void		SetMyGroupNum( int nNum )				{ m_nMyGroupNum = nNum;				}
	void		SetMyGroupPos( int nPos )				{ m_nGroupPos = nPos;				}

	// Get information
	SBYTE		GetDivisionType(int nType);
	SBYTE		GetPartyType()						{ return m_nType;						}
	int			GetMemberCount()			const	{ return m_nMemberCount;				}
	BOOL		GetIsPartyPlay(void)				{ return m_bIsPartyPlay;				}
	SLONG		GetMemberIndex( int nIndex )const	{ return m_PartyInfo[nIndex].slIndice;	}
	SLONG		GetMemberZone( int nIndex )	const	{ return m_PartyInfo[nIndex].slZone;	}
	ULONG		GetMemberTarget( int nIndex)		{ return m_PartyInfo[nIndex].ulTargetID;}
	SBYTE		GetMemberTargetType( int nIndex)	{ return m_PartyInfo[nIndex].sbTargetType;}
	FLOAT		GetPosX( int nIndex )		const	{ return m_PartyInfo[nIndex].fPosX;		}
	FLOAT		GetPosZ( int nIndex )		const	{ return m_PartyInfo[nIndex].fPosZ;		}
	SBYTE		GetLayer( int nIndex )		const	{ return m_PartyInfo[nIndex].sbLayer;	}
	int			GetLevel( int nIndex )		const	{ return m_PartyInfo[nIndex].nLevel;	}
	int			GetHp( int nIndex )			const	{ return m_PartyInfo[nIndex].nHP;		}
	int			GetmaxHp( int nIndex )		const	{ return m_PartyInfo[nIndex].nMaxHP;	}
	int			GetMp( int nIndex )			const	{ return m_PartyInfo[nIndex].nMP;		}
	int			GetmaxMp( int nIndex )		const	{ return m_PartyInfo[nIndex].nMaxMP;	}
	BOOL		IsPartyLeader( int nIndex )			{ return m_PartyInfo[nIndex].bLeader;	}
	bool		IsOnline( int nIndex )				{ return m_PartyInfo[nIndex].bOnline;	}
	int			GetMyGroupNum()						{ return m_nMyGroupNum;					}
	int			GetMyGroupPos()						{ return m_nGroupPos;					}
	SBYTE		GetDiveisionTypeExp()				{ return m_sbDivisionTypeEXP;			}
	SBYTE		GetDiveisionTypeItem()				{ return m_sbDivisionTypeITEM;			}
	SBYTE		GetDiveisionTypeSpItem()			{ return m_sbDivisionTypeSPITEM;		}
	CTString	GetMemberName( int nIndex ) const	{ return m_PartyInfo[nIndex].strName;	}
	CTString	GetMandateName()					{ return m_strMandateCharName;			}

	int			GetExpeditionMyPosition();
	int			GetExpeditionMyGroupCount();
	int			GetExpeditionGroupMemberCount(int nGroup);
	int			GetExpeditionMemberLevel(int nGroup, int nPos);
	int			GetExpeditionMemberHp(int nGroup, int nPos);
	int			GetExpeditionMemberMaxHp(int nGroup, int nPos);
	int			GetExpeditionMemberMp(int nGroup, int nPos);
	int			GetExpeditionMemberMaxMp(int nGroup, int nPos);
	SLONG		GetExpeditionMemberIndex(int nGroup, int nPos);		// ������ index
	SLONG		GetExpeditionMemberIndex(CTString strName);			// �̸����� �˻�
	SLONG		GetExpeditionMemberZone(int nGroup, int nPos);		// Zone
	FLOAT		GetExpeditionMemberPosX(int nGroup, int nPos);		// X
	FLOAT		GetExpeditionMemberPosZ(int nGroup, int nPos);		// Z
	SBYTE		GetExpeditionMemberLayer(int nGroup, int nPos);		// Layer
	SBYTE		GetExpeditionMemberJob(int nGroup, int nPos);
	SBYTE		GetExpeditionMemberPosition(int nGroup, int nPos);
	BOOL		GetExpeditionMemberOnline(int nGroup, int nPos);	// ���ӻ���
	CTString	GetExpeditionMemberName(int nGroup, int nPos);		// [2012/10/11 : Sora] ����� ����
	
	BOOL		IsPartyFull()				{ return m_nMemberCount == PARTY_MAX_MEMBER; }
	BOOL		IsPartyRequested()			{ return m_bIsPartyRequested; }
	bool		IsPartyMember( INDEX iChaIndex );
	bool		IsOurParty( CTString strName );					// �̸����� ��Ƽ�� �˻�
	int			IsOurPartyPos( CTString strName );				// �̸����� �˻� 
	int			IsOurParty( SLONG slCharIndex );				// �ĺ��ڷ� ��Ƽ�� �˻�
	bool		IsExpedetionMember( INDEX iChaIndex );			// �ش� �ε����� ����������� üũ 	
	bool		IsExpedetionDataExist(int nGroup, int nPos);	// �ش� ��ġ�� ������� ������ �ִ°� üũ
	int			IsLabeled(SLONG slTargetType, SLONG slIndex);	// �ε����� Ÿ�������� ���� üũ�Ǿ� �ֳ� Ȯ��
	BOOL		IsExistOfflineMember();							// �������� ���üũ

	BOOL		AmILeader() { return m_bAmILeader; }
	//////////////  ////////////////////////////////////////////////////////////
	// network (Send)
	void		SendDivisionType(SBYTE sbDivisionType, SBYTE sbType);									// �й�Ÿ�� ���� �޽��� ���� 
	void		SendChangeExpedition();																	// ������� ��ȯ �޽��� ����
	void		SendChangeGroup(int nSrcGroupNum, int nSrcPosNum, int nDestGroupNum, int nDestPosNum);	// ������� �׷� ����
	void		SendChangeSubLeader(SLONG slIndex);														// �δ��� ���� ��û

	void		SendPartyAllow();
	void		SendPartyReject();
	void		SendPartyQuit();
	void		SendPartyInvite( SBYTE sbType, CTString &strDestName );
	void		SendPartyInviteReq( SBYTE sbType, SLONG	slIndex, CTString &strDestName );
	void		SendPartyKick( CTString &strDestName );
	void		SendPartyEnd();																// ��Ƽ ��ü ��û

	void		SendExpeditionAllow();														// ������ ���� �޽��� ����
	void		SendExpeditionReject();
	void		SendExpeditionInvite( CTString &strDestName );								// ������ �ʴ� �޽��� ����
	void		SendExpeditionInviteReq(int slIndex, CTString &strDestName);
	void		SendExpeditionQuit();														// ������ Ż�� �޽��� ����
	void		SendExpeditionKick( SLONG slIndex );										// ������ ���� �޽��� ����
	void		SendExpeditionEnd();														// ������ ���� �޽��� ����
	void		SendExpeditionViewDetail(SLONG slIndex, CTString strCharName);				// ���캸�� ��û
	void		SendTargetLabelSet(SBYTE sbTargetType, SLONG slIndex, SLONG slLabelIndex);	// ǥ�� ������û
	void		SendTargetLabelReSet(SBYTE sbTargetType, SLONG slIndex);					// ǥ�� ���� ��û
	void		SendInitInZone();

	// Receive
	void		RecvChangeExpedition( CNetworkMessage *istr );
	void		RecvExpeditionInvite( CNetworkMessage *istr );	// ������ �ʴ� �޽��� ����
	void		RecvExpeditionError( CNetworkMessage *istr );	// ������ ���� ����
	void		RecvViewDetail( SBYTE sbType, CNetworkMessage *istr );	// ���캸�� ���� ����
	void		RecvRaidError( CNetworkMessage *istr );	// ���̵� ���� ����
protected:
	void		BreakUpParty();


private:
	stPartyInfo			m_PartyInfo[PARTY_MAX_MEMBER];
	stExpeditionMember	m_ExpeditionGroup[EXPEDITION_GROUP_MAX][EXPEDITION_MEMBER_PER_GROUP];

	int				m_nMemberCount;
	int				m_nMyGroupNum;					// �� �׷��ȣ
	int				m_nGroupPos;					// �׷� �� ��ġ
	int				m_nType;						// Party type
	BOOL			m_bAmILeader;					// If I am a leader or not	
	BOOL			m_bIsPartyRequested;			// If party is requested now or not
	BOOL			m_bIsPartyPlay;					// ���� ��Ƽ������ �ƴ� �÷���
	SBYTE			m_sbDivisionTypeEXP;			// ����ġ �й� Ÿ��
	SBYTE			m_sbDivisionTypeITEM;			// ������ �й� Ÿ��
	SBYTE			m_sbDivisionTypeSPITEM;			// ����� �����ۺй� Ÿ��
	CTString		m_strDestName;					// String of dest name when request party
	CTString		m_strMandateCharName ;		// �����Ϸ��� �ɸ����� Index
	CTString		m_strDivisionType[3];
	CTString		m_strExpeditionType[4];
};

#endif	//	__PARTY__