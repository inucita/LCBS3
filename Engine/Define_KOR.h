#define ADD_SUBJOB
#define AUTO_RESTART	// [2012/10/18 : Sora] 재시작시 자동 로그인
#define CHAR_EX_MAGE	//2013/01/08 jeil ex 메이지
#define CHAR_EX_ROGUE	// [2012/08/27 : Sora] EX로그 추가
#define CHATTING_BAN
#define ENABLE_GUILD_STASH	// new guild stash by rumist
#define GM_INVISIBLE_MODE
#define GUILD_MARK	// sora guild mark define 2011-07-14
#define IMPROV1107_NOTICESYSTEM	// 공지 강화 시스템 [trylord 11/14/11]
#define MULTI_CLIENT
#define NEW_CHAO_SYS
#define NEW_WEAPONDROP
#define NEW_WORLD_MAP	// [2012/10/11 : Sora] 월드맵 개편
#define PET_TRANSFORM_COMMAND
#define RESTART_GAME
#define SIGEWAR_ATTACKRULE	//20110519 이후 적용 예정
#define STASH_PASSWORD
#define UPDATE1107_GUILDSYSTEM	// 길드 시스템 개편
#define WEARING_ARMOR_COMMAND

#if defined VER_TEST
#define DEV_LOGIN	// 개발 서버에서만 사용 Login.txt로 아이디/패스워드 저장
#define UI_TEST
	//#define ENABLE_GUILD_STASH_CASHITEM_USAGE_TIME_LIMIT		// 길드 창고 캐시 아이템 - 창고 이용 시간 제한 [8/9/2012 rumist]
	//#define EUROUPEAN_SERVER_LOGIN
#endif	//	VER_TEST
