#ifndef __SEVER_SELECT_H__
#define __SEVER_SELECT_H__

enum eSERVER_TYPE
{
	SERVER_TYPE_NONE = 0,
	SERVER_TYPE_RECOMEND,
	SERVER_TYPE_HARDCORE,
};

// ä��.
struct sChannelInfo
{
	ULONG			iSubNum;			// Sub ä�� ��ȣ.
	ULONG			iPlayerNum;			// �÷��̾� ��.
	CTString		strAddress;			// IP Address
	ULONG			iPortNum;			// Port Num

	bool operator<(const sChannelInfo &other) const
	{			
		if( iSubNum < other.iSubNum )
			return true;
		return false;
	}
	bool operator>(const sChannelInfo &other) const
	{			
		return other.operator < (*this);
	}
};

// ����
struct sServerInfo
{
	sServerInfo()
	{
		iServerState		= 0;
	}
	~sServerInfo()
	{
		vecChannelInfo.clear();
	}

	int		GetChannelCount()	{ return vecChannelInfo.size();	}
	sChannelInfo* ChannelListAt(int idx)
	{
		if (idx < 0 || idx >= vecChannelInfo.size())
			return NULL;
		return (&vecChannelInfo[idx]);
	}

	ULONG			iServerNo;		// ���� ��ȣ
	UBYTE			ubServerType;	// ���� Ÿ��
	int				nOrder;

	// FULL�̰ų� ���� ���̸� -1�� ����, �����̸� +1�� ����.
	int				iServerState;						// ���� ���¿� ���� ����.
	std::vector<sChannelInfo>	vecChannelInfo;		// ä�� ���.

	bool operator<(const sServerInfo &other) const
	{			
		if(iServerState > other.iServerState)			return true;
		else if(iServerState < other.iServerState)	return false;
		else
		{
			if(iServerNo < other.iServerNo)			return true;
			else if(iServerNo > other.iServerNo)	return false;
		}
		return false;
	}
	bool operator>(const sServerInfo &other) const
	{			
		return other.operator < (*this);
	}
};

class CServerSelect
{
public:
	CServerSelect();
	~CServerSelect();

	ENGINE_API void ConnectToServer(CTString strIP, ULONG ulPort);	// ������ ���� �õ�.
	void AddServerList(LONG lServerNo, LONG lSubNum, LONG lPlayerNum, CTString &strAddress, LONG lPortNum ,UBYTE ubServerType);
	void ResetData();

	int	GetServerCount()	{ return m_vecServerInfo.size();	}
	sServerInfo* ServerListAt(int idx);

	void SetRecentServer(int iRecentServer, int iRecentChannel)
	{
		m_nRecentServer = iRecentServer;
		m_nRecentChannel = iRecentChannel;
	}
	int GetRecentServer()	{ return m_nRecentServer;	}
	int	GetRecentChannel()	{ return m_nRecentChannel;	}

	// Function Object
	struct FindServer
	{
		int iNum;
		FindServer(sServerInfo& info) : iNum(info.iServerNo) {};
		bool operator() (const sServerInfo& info)
		{
			if(info.iServerNo == iNum)
				return true;
			return false;
		}
	};

	struct FindChannel
	{
		int iNum;
		FindChannel(sChannelInfo& info) : iNum(info.iSubNum) {};
		bool operator() (const sChannelInfo& info)
		{
			if(info.iSubNum == iNum)
				return true;
			return false;
		}
	};

private:

	typedef std::vector<sServerInfo>		vecServerInfo;
	typedef vecServerInfo::iterator		vecServerInfoIter;

	vecServerInfo	m_vecServerInfo;
	int				m_nRecentServer;
	int				m_nRecentChannel;
};
#endif // __SEVER_SELECT_H__