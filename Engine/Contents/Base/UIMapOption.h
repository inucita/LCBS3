#ifndef __UI_MAP_OPTION_H__
#define __UI_MAP_OPTION_H__

enum eNPC_TYPE
{
	eNT_QUEST_Q = 0,
	eNT_QUEST_A ,
	eNT_SHOP,
	eNT_STASH,
	eNT_TELEPORT,
	eNT_GUILD,
	eNT_AFFINITY,
	eNT_AUCTION,
	eNT_CHAR_MANAGER,
	eNT_JOB_MASTER,
	eNT_WAR,
	eNT_PET_MANAGER,
	eNT_EXPRESS,
	eNT_ELSE_NPC,
	eNT_MONSTER,
	eNT_GUILD_GROUP,	// ����.
	eNT_PARTY,
	eNT_FELLOWSHIP,		// ������,

	eNT_MAX
};

class CUIMapOption : public CUIWindow
{
public:

	CUIMapOption();

	void initialize();

	bool IsCheck(eNPC_TYPE eType);
	UIRectUV	GetUV(eNPC_TYPE eType);
	BOOL		IsFlag(int nType, int npcIndex);

	void OnLeave(UINT16 x, UINT16 y);

private:
	CUICheckButton* m_pCheckBtn[eNT_MAX];
	CUIImage*		m_pImgIcon[eNT_MAX];

};
#endif // __UI_MAP_OPTION_H__