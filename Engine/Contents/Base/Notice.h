#ifndef __NOTICE__
#define __NOTICE__

#include <vector>

class ENGINE_API Notice
{
public:
	enum eNoticeType
	{
		NOTICE_NONE		= -1,
		NOTICE_NEWS_WEB = 0,	// �� ���� �˾�.
		NOTICE_NEWS_NORMAL,		// �Ϲ� ���� �˾�(echo all�� �Էµ� ����)
		NOTICE_QUEST,		// ����Ʈ(�Ϲ�)
		NOTICE_QUEST_UNLIMITED,		// ����Ʈ(�ݺ�)
		NOTICE_POLYMOPH,			// ����
		NOTICE_EVENT,				// �̺�Ʈ
		NOTICE_AFFINITY,			// ģȭ�� ����
		NOTICE_MAX
	};

	struct sNoticeInfo
	{
		int				iIndex;
		eNoticeType		iType;

		bool operator<(const sNoticeInfo &other) const
		{
			if( iType < other.iType )
				return true;
			return false;
		}
		bool operator>(const sNoticeInfo &other) const
		{			
			return other.operator < (*this);
		}
	};

	struct FindNotice
	{
		int				iIndex;
		eNoticeType		iType;
		FindNotice(sNoticeInfo& info) : iIndex(info.iIndex), iType(info.iType) {};
		bool operator() (const sNoticeInfo& info)
		{
			if( info.iIndex == iIndex && info.iType == iType )
				return true;

			return false;
		}
	};

	Notice(void);
	virtual ~Notice(void);

	// Command functions
	void AddToNoticeList( int iNoticeIndex, eNoticeType iNoticeType );
	void DelFromNoticeList( int iNoticeIndex, eNoticeType iNoticeType );
	bool GetNoticeBlink(eNoticeType iNoticeType);
	void SetNoticeBlink(eNoticeType iNoticeType, bool bOnOff);
	void clear();
	std::vector<sNoticeInfo>*	GetNoticeList(eNoticeType iNoticeType);

private:
	std::vector<sNoticeInfo>	m_vecList[NOTICE_MAX];
	bool						m_bNewNotice[NOTICE_MAX];
};



#endif	//	__NOTICE__