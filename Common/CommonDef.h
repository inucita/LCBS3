#ifndef		COMMON_DEF_H_
#define		COMMON_DEF_H_

#define		DURABILITY
#define		NAS_ITEM_DB_INDEX			(19)

#if defined ADD_SUBJOB
	#define ENABLE_SUBJOB
#endif	//	ADD_SUBJOB

#if defined GUILD_MARK
	#define DEV_GUILD_MARK
#endif	//	GUILD_MARK

#define DEV_EXP_APET

// �ӽ� define //////
#define WEARING_SHOW_END	6
#define MAX_GUILD_NAME_LENGTH 50
#define APET_WEARPOINT		4
#define MAX_SERIAL_LENGTH	20
#define MAX_SOCKET_COUNT	7
#define	MAX_ITEM_OPTION		5
#define MAX_ORIGIN_OPTION	6
#define	MAX_CHAR_NAME_LENGTH	50
#define	MAX_ID_NAME_LENGTH		64


#define		MEMPOS_COMMENT_LENGTH_RUS	25
#define		MEMPOS_COMMENT_LENGTH		32


#define		PREMIUM_CHAR // ���̰��� ��û�� �����̾� ĳ����

typedef __int64		GoldType_t;

#define MAX_PWD_LENGTH				32	

#endif		// COMMON_DEF_H_