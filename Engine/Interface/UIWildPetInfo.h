
// ----------------------------------------------------------------------------
//  File : UIWildPetInfo.h
//  Desc : Created by yjpark
// ----------------------------------------------------------------------------

#ifndef	UIWILDPETINFO_H_
#define	UIWILDPETINFO_H_

#include <Engine/Interface/UICheckButton.h>

class CUIIcon;

#define WILDPETINFO_WIDTH			273
#define WILDPETINFO_HEIGHT			401
#define WILDPET_AI_MAXSLOT			6

struct sWildPet_AI_Slot 
{
	BOOL	m_bActive;		// AI ���� Ȱ�� ����
	INDEX	m_nSillType;	// ��ų Ÿ�� Skill = 0, Action = 1
	INDEX	m_nTargetSlot;	// Ÿ�� ������ �ε��� ����
	INDEX	m_nActionSlot;	// �ൿ ��ų �ε��� ����
};

enum WILDPET_SLOT
{
	WILDPET_INFO_SLOT	 = 0,
	WILDPET_SKILL_SLOT	= 1,
	WILDPET_SLOT_TOTAL	= 2,
};

enum WILDPET_POINT
{
	WILDPET_STR	= 0,	// �� ����Ʈ
	WILDPET_CON	= 1,	// ü�� ����Ʈ
	WILDPET_DEX	= 2,	// ��ø ����Ʈ
	WILDPET_INT	= 3,	// ���� ����Ʈ
	POINT_END,
};

enum eAI_TYPE
{
	AI_NONE = -1,
	AI_TARGET_ENEMY = 0,
	AI_TARGET_OWNER,
	AI_TARGET_PET,
	AI_TARGET_ITEM,
};

class CUIWildPetInfo : public CUIWindow
{
protected:
	typedef std::map<SLONG, CUIIcon*> map_Icon;
	typedef std::map<SLONG, sPetItem_Info> WildPetInfo_map;

	CTextureData	*m_ptdButtonTexture;
	CUIRectSurface	m_rtsBace;
	CUIRectSurface	m_rtsAIBace;
	UIRect			m_rtTitle;
	UIRectUV		m_rtInfoBace;
	UIRectUV		m_rtSkillBace;
	UIRectUV		m_rtStmImage;
	UIRectUV		m_rtFaithImage;
	CUIIcon*		m_pIconWearing[WILDPET_WEAR_TOTAL];
	CUIIcon*		m_pIconSkill[8];
	CUIButton		m_btTabSlot[WILDPET_SLOT_TOTAL];
	CUIButton		m_btnPoint[POINT_END];
	CUIButton		m_btnClose;
	CUIButton		m_btnAIEdit;		// AI ���� ��ư
// 	CUIButton		m_btnAION;			// AI Ȱ�� ��ư
//	CUIButton		m_btnAIOFF;			// AI ��Ȱ�� ��ư
	UIRect			m_rtAIONOFF;
	UIRectUV		m_rvAIONOFF;
	CUIButton		m_btnAIMake;		// AI ���� �Ϸ� ��ư
	CUIScrollBar	m_sbSkillList;

	CUIDrawBox		m_rvAccuback;
	UIRectUV		m_rvAccublank;
	CUIButton		m_btnAccumulateUse;
	CWildPetData	*m_Petdata;

	INDEX			m_nTebnum;
	INDEX			m_nAISlotCount;
	BOOL			m_bTitle;
	BOOL			m_bshowItem;
	BOOL			m_bAIActive;
	BOOL			m_bSlideUI;

	map_Icon		m_mapIconSkills;		// Buttons of Skill
	WildPetInfo_map		m_mapWildPetInfo;

	sWildPet_AI_Slot	m_sAISlot[WILDPET_AI_MAXSLOT];
	CUICheckButton		m_cbtnAISlotActive[WILDPET_AI_MAXSLOT];
	CUIIcon*			m_pIconAITarget[WILDPET_AI_MAXSLOT];
	CUIIcon*			m_pIconAIAction[WILDPET_AI_MAXSLOT];
	UIRect				m_rtAITarget[WILDPET_AI_MAXSLOT];
	UIRect				m_rtAIAction[WILDPET_AI_MAXSLOT];

public:
	CUIWildPetInfo();
	~CUIWildPetInfo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	InfoRender();
	void	SkillRender();
	ENGINE_API BOOL	IsLearnSkill( int nSkillIndex );
	void	SkillClear();
	void	AddSkill( int nSkillIndex, SBYTE sbSkillLevel );
	void	sendSkillList();
	void	UseSkill( int nIndex );
	// author by rumist : since : 2010-12-20 21:03
	ENGINE_API	BOOL	IsUsableSkill( LONG nSkillIndex );

	void	PetWearItemReSet();
	void	ReceiveWearItemData(CNetworkMessage *istr);

	void	OpenWildPetInfo();
	void	CloseWildPetInfo();
	void	sendWildPetPointUP(UBYTE ubPoint);
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ToggleVisible();
	void	AddWildPetInfo( sPetItem_Info sPetInfo );
	BOOL	GetWildPetInfo( int nPetIndex, sPetItem_Info& WildPetInfo );

	void	AIPopupRender();

	inline void SetPetAISlotCount(int nSlotcount) { m_nAISlotCount = nSlotcount; }
	inline void	SetPetAIActive(BOOL bActive) {m_bAIActive = bActive; }
	inline BOOL GetAIActive() {return m_bAIActive; }
	inline int GetPetAISlotCount() {return m_nAISlotCount;}
	sWildPet_AI_Slot* GetPetAI() {return m_sAISlot;}
	
	void	RemoveSlot(CUIIcon* pIcon);

	void	AddAIData(int nSlotnum, sWildPet_AI_Slot sAISlotdata);
	void	ErrorMassage(UBYTE errorcode);
	void	SendAIList();
	void	SendAIActive(SBYTE bActive);
	void	PetAIReSet();
	void	AIPopupClose() {m_bSlideUI = FALSE;}
	void	AIClear();
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// esc closing support [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()				{ ToggleVisible();	return TRUE;		}
	void	AccumulateInfoRender();
	BOOL	IsWildPetCoolTime();
	int	GetWildPetFlag() {return m_Petdata->nFlag;}

	void	OnUpdate( float fDeltaTime, ULONG ElapsedTime );

private:
	void	deleteSkill();

	BOOL	CheckAIItem(CItemData* pAIItem, int nSlot);
	BOOL	CheckAIAction(CUIIcon* pIcon, int nSlot);

	BOOL	CheckAIType(int nAIType, int nActionType);
	int		GetActionAIType(CUIIcon* pIcon);
};

#endif

