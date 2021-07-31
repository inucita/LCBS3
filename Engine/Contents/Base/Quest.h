#ifndef __QUEST__
#define __QUEST__

#include <vector>

class CItems;

#define DEF_MAX_QUEST		16
#define DEF_PAGE_VIEW_COUNT 2

enum eVIEW_PAGE
{
	eVIEW_FIRST = 0,
	eVIEW_SECOND,
	eVIEW_THIRD,
	eVIEW_MAX
};

enum eTREE_TYPE
{
	eTREE_NONE = -1,
	eTREE_PROCEED = 0,
	eTREE_COMPLETE,
	eTREE_RAID,
	eTREE_MAX
};

class ENGINE_API Quest
{
public:
	Quest(void);
	virtual ~Quest(void);

	struct stQuestInfo
	{
		stQuestInfo()
		{
			iQuestIndex		= -1;
			strQuestTitle	= CTString("");
			iQuestType		= -1;
			iQuestScale		= -1;
			dOldTime		= -1;			
			dRemainTime		= -1;
		}

		// �� ������ �����ε�
		inline const bool operator== (const stQuestInfo& rhs)
		{
			return iQuestIndex == rhs.iQuestIndex;
		}		
		// �������� ������ �����ε�
		inline const bool operator< (const stQuestInfo& rhs)
		{
			return iQuestIndex < rhs.iQuestIndex;
		}

		CTString	strQuestTitle;
		int			iQuestIndex;
		int			iQuestType;			// ����Ʈ ���� Ÿ��(�Ϲ�, �ó�����, ����, ����Ʈ ������)
		int			iQuestScale;		// ����Ʈ �Ը�(�Ϲ�, ��Ƽ, ������)
		DOUBLE		dOldTime;			// �ð����� ����Ʈ �� ������ ���� �ð�		
		DOUBLE		dRemainTime;		// �ð����� ����Ʈ ���� �ð�(��)
	};

	struct stFindQuest
	{
		int iQuestIndex;
		stFindQuest(stQuestInfo& info) : iQuestIndex(info.iQuestIndex) {};
		bool operator() (const stQuestInfo& info)
		{
			if(info.iQuestIndex == iQuestIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	struct stRestoreInfo
	{
		int iQuestIndex;
		int iQuestLevel;
	};

	// ���� ����, ���� ����  ����Ʈ ����Ʈ (NPC �޴��� ����)
	BOOL AddQuest( int iQuestIndex, BOOL bComplete );
	BOOL DelQuest( int iQuestIndex, BOOL bComplete );
	void ClearQuest();

	// ����Ʈ �� ����Ʈ
	bool IsCompleteQuest(int nQuestIndex);
	void SetQuestRemainTime(int iQuestIndex, int iRemainTime);
	BOOL AddQuestList( int iQuestIndex, BOOL bComplete );
	BOOL DelQuestList( int iQuestIndex, BOOL bComplete );

	void SetExtend(BOOL bExtend, eTREE_TYPE eType) { m_bListExtend[eType] = bExtend; }
	BOOL GetExtend(eTREE_TYPE eType)				 { return m_bListExtend[eType]; }
	void ClearQuestList();
	
	// ���̵� ���� ����
	void AddRaidMessage(const int& iQuestIndex);
	void RemoveRaidMessageAll(void);
	bool isRaidMessage(const int& iQuestIndex);
	void RaidEnd(void);

	DOUBLE	GetQuestNoticeElapsedTime();
	void	SetRaidRemainTime(INDEX iRemainTime);
	DOUBLE	GetRaidReminTime(void);
	BOOL	IsRaidNow(void);

	// �̸����� ����
	void AddSelectedQuest(const int& iQuestIndex);
	void RemoveSelectedQuest(const int& iQuestIndex);
	void ClearSelectedQuestList();
	bool NextQuestViewPage();
	bool PrevQuestViewPage();
	bool IsSelectedQuest(const int& iQuestIndex);
	int GetSelectedQuestIndex(int Idx);
	int	GetSelectedCount()	{ return m_vectorViewList.size();	}
	int	GetCurrViewPage()	{ return m_nCurrViewPage;			}

	// ����Ʈ ���� ����
	void AddRestoreQuestList(const int& iQuestIndex, int iQuestLevel);
	void ClearAllRestoreList();
	void SortRestoreQuest();

	//////////////////////////////////////////////////////////////////////////
	INDEX GetProceedQuestCount()		{ return m_vectorProceedQuestList.size();	}
	INDEX GetCompleteQuestCount()		{ return m_vectorCompleteQuestList.size();	}
	INDEX GetListProceedCount()			{ return m_vectorListProceed.size();		}
	INDEX GetListCompleteCount()		{ return m_vectorListComplete.size();		}
	INDEX GetListRaidCount()			{ return m_vectorListRaid.size();			}
	INDEX GetRestoreCount()				{ return m_vectorRestoreList.size();		}
	
	const std::vector<int>& GetSelectedQuestList()	{ return m_vectorViewList;		}
	const std::vector<stRestoreInfo>& GetRestoreQuestList()	{ return m_vectorRestoreList;	}
	// ����Ʈ ���� ��������
	INDEX GetProceedQuestIndex(INDEX index);
	INDEX GetCompleteQuestIndex(INDEX index);
	INDEX GetListProceedIndex(INDEX index);
	INDEX GetListCompleteIndex(INDEX index);
	INDEX GetListRaidIndex(INDEX index);
	CTString GetListProceedTitle(INDEX index)
	{
		return m_vectorListProceed[index].strQuestTitle;
	}
	CTString GetListCompleteTitle(INDEX index)
	{
		return m_vectorListComplete[index].strQuestTitle;
	}
	CTString GetListRaidTitle(INDEX index)
	{
		return m_vectorListRaid[index].strQuestTitle;
	}

	// network
	void SendQuestReq( int nNpcIndex );
	void SendAcceptReq( int nQuestIndex, CItems* pQuestItem = NULL );
	void SendQuestPrize( int iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus );
	void SendQuestCancel( int nQuestIndex );
	
private:
	std::vector<stQuestInfo>	m_vectorProceedQuestList; // ���డ�� ����Ʈ ����Ʈ
	std::vector<stQuestInfo>	m_vectorCompleteQuestList; // ���󰡴� ����Ʈ ����Ʈ
	
	std::vector<stQuestInfo>	m_vectorListRaid;		//���̵� ����Ʈ ����Ʈ
	std::vector<stQuestInfo>	m_vectorListProceed;	//������ ����Ʈ ����Ʈ
	std::vector<stQuestInfo>	m_vectorListComplete;	//�Ϸ� ����Ʈ ����Ʈ
	std::vector<stRestoreInfo>	m_vectorRestoreList;	//������ ����Ʈ ����Ʈ
	BOOL						m_bListExtend[eTREE_MAX];

	std::vector<int>			m_vectorViewList;	//���õǾ� �ִ� ����Ʈ���� �ε��� ����
	int							m_nCurrViewPage;

	// ����Ʈ ���� ���� �ð�
	DOUBLE					m_dQuestNoticeStartTime;
	// ����Ʈ ���� �ι� �ȳ����� �ϱ� ���� 
	// �̹� �߻��� ����Ʈ ������ ������ �д�	
	std::vector<int>		m_aAlreadyShowQuestNotice;

	// ���̵� ����, �� ����
	DOUBLE					m_dRaidRemainTime;	// ���̵� ���� �ð�
	BOOL					m_bIsRaidNow;		// ���̵� �� ���� �Ǵ� �÷���
	DOUBLE					m_dRaidRemainTimeRecvTime;	// ���̵� ���� �ð��� ���� �ð�
};

#if		(_MSC_VER >= 1600)
class Compare : std::unary_function<Quest::stRestoreInfo, bool>
{
public:
	bool operator()(const Quest::stRestoreInfo& lhs, const Quest::stRestoreInfo& rhs) const
	{
		bool bResult;

		bResult = lhs.iQuestLevel < rhs.iQuestLevel;
		
		if (lhs.iQuestLevel == rhs.iQuestLevel)
			bResult = lhs.iQuestIndex < rhs.iQuestIndex;
		
		return bResult;
	}
};
#endif	// (_MSC_VER >= 1600)

#endif	//	__QUEST__