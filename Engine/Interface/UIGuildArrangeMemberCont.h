
#ifndef __UIGUILD_ARRANGE_MEMBER_CONT_H__
#define __UIGUILD_ARRANGE_MEMBER_CONT_H__

#if		!defined(WORLD_EDITOR)

using namespace boost::multi_index;

class clsGuildMemberNew
{	
public:
	typedef boost::shared_ptr<clsGuildMemberNew>	SP;

	clsGuildMemberNew()
	{
		lIndex			= -1;
		eRanking		= 2;	// GUILD_MEMBER_MEMBER		
		bOnline			= FALSE;
		sPosName		= "";	
		iLevel			= 0;				
		iJob			= -1;		
		iJob2			= -1;
		sLocName		= "";
		iLocation		= -1;	
		iServDegree		= 0;		
		iCumulPoint		= 0;
		iExpServ		= 0;	
		iFameServ		= 0;	
		bStashPermission = 0;
		sRanking		= "";
		sJobName		= "";
		nLogoutDate		= 0;
		iDonateExpMin		= 0;
		iDonateExpMax		= 0;
		iDonateFameMin		= 0;
		iDonateFameMax		= 0;
	}	

	SLONG			lIndex;
	std::string		strMemberName;		// �����?
	int				eRanking;			// ����?
	int				nArrangeRank;		// ���� ������ ���� �߰�.
	BOOL			bOnline;			// ������?
	// WSS_NEW_GUILD_SYSTEM 070705 ------------------------------------------->>
	// �߰� ��� ����
	std::string		sRanking;			// ����Ʈ ������
	std::string		sPosName;			// ������
	INT				iLevel;				// ����
	std::string		sJobName;			// Ŭ���� ��
	INT				iJob;				// Ŭ����
	INT				iJob2;				// ���� Ŭ����
	std::string		sLocName;			// ���� ��ġ
	INT				iLocation;			// ���� ��ġ
	INT				iServDegree;		// �⿩�� 
	INT				iCumulPoint;		// ���� ����Ʈ
	INT				iExpServ;			// ����ġ �󳳵�
	INT				iDonateExpMin;
	INT				iDonateExpMax;
	INT				iFameServ;			// ��ġ �󳳵�
	INT				iDonateFameMin;
	INT				iDonateFameMax;
	BYTE			bStashPermission;
	int				nLogoutDate;		// �α׾ƿ��� �ð�
	// -----------------------------------------------------------------------<<
};

typedef boost::multi_index_container <
	clsGuildMemberNew::SP,
	indexed_by <
		ordered_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, std::string, strMemberName) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, int, nArrangeRank) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, INT, iLevel) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, std::string, sJobName) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, std::string, sLocName) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, INT, iCumulPoint) 
		>,
		ordered_non_unique <
			BOOST_MULTI_INDEX_MEMBER(clsGuildMemberNew, int, nLogoutDate) 
		>
	>
> ContGuild;

#else	// WORLD_EDITOR

#include <set>

class clsGuildMemberNew
{	
public:
	static clsGuildMemberNew* SP(clsGuildMemberNew* ptr)	{
		return ptr;
	}
	
	clsGuildMemberNew()
	{
		lIndex			= -1;
		eRanking		= 2;	// GUILD_MEMBER_MEMBER		
		bOnline			= FALSE;
		sPosName		= "";	
		iLevel			= 0;				
		iJob			= -1;		
		iJob2			= -1;
		sLocName		= "";
		iLocation		= -1;	
		iServDegree		= 0;		
		iCumulPoint		= 0;
		iExpServ		= 0;	
		iFameServ		= 0;	
		bStashPermission = 0;
		sRanking		= "";
		sJobName		= "";
		nLogoutDate		= 0;
		iDonateExpMin		= 0;
		iDonateExpMax		= 0;
		iDonateFameMin		= 0;
		iDonateFameMax		= 0;
	}	
	
	SLONG			lIndex;
	std::string		strMemberName;		// �����?
	int				eRanking;			// ����?
	int				nArrangeRank;		// ���� ������ ���� �߰�.
	BOOL			bOnline;			// ������?
	// WSS_NEW_GUILD_SYSTEM 070705 ------------------------------------------->>
	// �߰� ��� ����
	std::string		sRanking;			// ����Ʈ ������
	std::string		sPosName;			// ������
	INT				iLevel;				// ����
	std::string		sJobName;			// Ŭ���� ��
	INT				iJob;				// Ŭ����
	INT				iJob2;				// ���� Ŭ����
	std::string		sLocName;			// ���� ��ġ
	INT				iLocation;			// ���� ��ġ
	INT				iServDegree;		// �⿩�� 
	INT				iCumulPoint;		// ���� ����Ʈ
	INT				iExpServ;			// ����ġ �󳳵�
	INT				iDonateExpMin;
	INT				iDonateExpMax;
	INT				iFameServ;			// ��ġ �󳳵�
	INT				iDonateFameMin;
	INT				iDonateFameMax;
	BYTE			bStashPermission;
	int				nLogoutDate;		// �α׾ƿ��� �ð�
	// -----------------------------------------------------------------------<<
};

typedef		std::set< clsGuildMemberNew* >	ContGuild;

#endif	// WORLD_EDITOR

#endif // __UIGUILD_ARRANGE_MEMBER_CONT_H__