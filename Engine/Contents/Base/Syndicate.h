#ifndef __SYNDICATE_H__
#define __SYNDICATE_H__

#include <Common/CommonDef.h>
#include <Common/Packet/ptype_syndicate.h>
#include <list>

class CTString;
class CSyndicate
{
public:
	CSyndicate();
	~CSyndicate();

	enum eGRADE
	{
		eGRADE_1ST = 1,
		eGRADE_2ND,
		eGRADE_3RD,
		eGRADE_4TH,
		eGRADE_5TH,
		eGRADE_6TH,
		eGRADE_7TH,
		eGRADE_8TH,
		eGRADE_9TH,
		eGRADE_10TH,

		eGRADE_MAX = eGRADE_10TH
	};

	enum eSYNDICATE_TYPE
	{	
		eTYPE_NONE = 0,		// ���Ҽ�.
		eTYPE_KAILUX,
		eTYPE_DEALERMOON,
		eTYPE_FLOR,

		eTYPE_MAX
	};

	void RecvSyndicateMsg(CNetworkMessage* istr);

	///=============== SendMessage() =============================================>>
	void SendGiveJewel( int nVNpcIndex, int nVIndexByJewel );	// â���� ���� ���.
	void SendSecession(int nVNpcIndex, int nTab, int nInvenIndex);							// ���� Ż�� ��û ��Ŷ.
	void SendSyndicateInfoUser();								// ���� ���� ���� ��û.
	void SendSyndicateInfo();									// ���� ���� ��û.
	///===========================================================================<<

	///=============== Interface =================================================>>
	int IsRVRItem();	// ���� ����߿� ���� ���� �������� �ִٸ� �ε��� ����.
	
	CTString GetGradeName(int nSyndiType, int nGrade);
	CTString GetSyndicateName(int nType);
	LONGLONG GetAccMaxPoint()	{ return m_llMaxAccPoint;	}
	int		 GetHistoryCount()	{ return m_HistoryData.size();	}
	LONGLONG GetSecssionNas()	{ return m_lSecessionNas;	}
	void	 ResetSyndiInfo();
	
	UpdateClient::SyndicateHistoryData::historys* GetHistoryData(int nIndex);
	UpdateClient::RVRSyndicateInfoOfServer* GetSyndicateInfo()	{ return m_pSyndicateInfo;	}
	UpdateClient::SyndicatePercentageData*	GetJewelInfo()		{ return m_pJewelInfo;		}
	UpdateClient::SyndicateKingNameUpdate*	GetKingInfo()		{ return m_pKingInfo;		}
	/// ===========================================================================<<
private:

	///=============Recv Packet====================================================>>
	void RecvUserInfoRep(CNetworkMessage* istr);				// ���� ���� ���� ����.
	void RecvUpdateGrade(CNetworkMessage* istr);				// ��ε�ĳ���� ����.
	void RecvUpdateContributeionPoint(CNetworkMessage* istr);	// ���� ���� ���� �⿩ ����Ʈ
	void RecvUpdateHistoryLog(CNetworkMessage* istr);			// ����, Ż��, PK���� ... ���
	void RecvUpdateSyndicateInfo(CNetworkMessage* istr);		// 1������, �����ε���, ��������
	void RecvUpdateJewelPercent(CNetworkMessage* istr);			// â���� ���� �ۼ�Ʈ ����.
	void RecvUpdateKingInfo(CNetworkMessage* istr);				// 1�� ����.
	void RecvUpdateIncreasePoint(CNetworkMessage* istr);
	void RecvUpdateOutNeedNas(CNetworkMessage* istr);			// Ż�� �ʿ��� ����.
	///============================================================================<<

	void ErrMessage(CNetworkMessage* istr);

	LONGLONG m_llMaxAccPoint;
	int		 m_nHistoryCount;
	LONGLONG m_lSecessionNas;
	bool m_bFirstJoin;

	UpdateClient::RVRSyndicateInfoOfServer*			m_pSyndicateInfo;
	UpdateClient::SyndicatePercentageData*			m_pJewelInfo;
	UpdateClient::SyndicateKingNameUpdate*			m_pKingInfo;

	typedef std::list<UpdateClient::SyndicateHistoryData::historys>	list_history;
	typedef list_history::iterator	list_history_iter;

	list_history	m_HistoryData;
};
#endif // __SYNDICATE_H__