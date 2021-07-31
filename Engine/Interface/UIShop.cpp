#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Interface/UIHelp.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Interface/UIGWMix.h>
#include <Engine/Contents/Base/UIMsgBoxNumeric_only.h>
#include <Engine/info/MyInfo.h>

#define ENABLE_GUILD_CUBE

#define EVENT_MAY_LEVELMIN	5
#define EVENT_MAY_LEVELMAX	32

// [KH_07044] 3�� ���� ���� �߰�
extern INDEX g_iShowHelp1Icon;
extern INDEX g_iCountry;

enum eSelection
{
	SHOP_BUY,		// ���� ����
	SHOP_SELL,		// ���� �Ǹ�
	SHOP_TALK,		// �̾߱� �ϱ�
	SHOP_EVENT,		// �̺�Ʈ.
	SHOP_GW_MIX,	// ���� ���� 
	SHOP_SUPERGOJE,	// �ʰ�� ���ü� �ޱ�, Ư�� NPC �ϵ��ڵ�

	SHOP_NPC_HELP,		//NPC �ȳ� �ý���

	SHOP_NPC_AFFINITY,			// [6/1/2009 rumist] npc affinity system. ģȭ��. 

	LEASE_WEAPON_TITAN,			// ���� �뿩��
	LEASE_WEAPON_KNIGHT,
	LEASE_WEAPON_HEALER,
	LEASE_WEAPON_MAGE,
	LEASE_WEAPON_ROGUE,
	LEASE_WEAPON_SOCERER,
	LEASE_WEAPON_EXROGUE = 15,
	LEASE_WEAPON_EXMAGE,		//2013/01/08 jeil EX������ �߰� 

	//ttos: ���� �޺�
	COMBO_MAKE,
	COMBO_MOVE_STAGE,
	COMBO_MOVE_GO_LOBBY,
	COMBO_RECALL_LOBBY,
	COMBO_RECALL_STAGE,
	COMBO_GIVEUP,
	COMBO_COMPLETE,
	COMBO_START,

	//Cube
	CUBE_PARTY_ENTRANCE,
	CUBE_PARTY_GROUP_ENTRANCE,
	CUBE_PARTY_PERSON_ENTRANCE,
	CUBE_GUILD_GROUP_ENTRANCE,
	CUBE_GUILD_PERSON_ENTRANCE,
	CUBE_GUILD_ENTRANCE,
	CUBE_PLAYER_STATE,
	CUBE_GUILD_STATE,
	CUBE_PLAYER_REWARD,

	// ($E_WC2010) [100511: selo] 2010 ������ �̺�Ʈ
	EVENT_WORLDCUP_2010_INFO,				// ������ 2010 �̺�Ʈ �ȳ�
	EVENT_WORLDCUP_2010_BALL,				// ������ 2010 �౸�� ��ȯ
	EVENT_WORLDCUP_2010_BALL_GIFT,			// ������ 2010 �౸�� ����
	EVENT_WORLDCUP_2010_GOLDENBALL,			// ������ 2010 Ȳ���౸�� ��ȯ
	EVENT_WORLDCUP_2010_GOLDENBALL_GIFT,	// ������ 2010 Ȳ���౸�� ����
};

class CmdShopAddItem : public Command
{
public:
	CmdShopAddItem() : m_pWnd(NULL)	{}
	void setData(CUIShop* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->AddItemCallback();
	}
private:
	CUIShop* m_pWnd;
};

class CmdShopDelItem : public Command
{
public:
	CmdShopDelItem() : m_pWnd(NULL)	{}
	void setData(CUIShop* pWnd)	{ m_pWnd = pWnd;	}
	void execute() 
	{
		if (m_pWnd != NULL)
			m_pWnd->DelItemCallback();
	}
private:
	CUIShop* m_pWnd;
};

// ----------------------------------------------------------------------------
// Name : CUIShop()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIShop::CUIShop()
{
	m_bBuyShop			= FALSE;
	m_nSelShopItemID	= -1;
	m_nSelTradeItemID	= -1;
	m_nSelectedShopID	= -1;
	m_nCurRow			= 0;
	m_nNumOfItem		= 0;
	m_nTotalPrice		= 0;
	m_nSendTotalPrice	= 0;
	m_bShowItemInfo		= FALSE;
	m_bDetailItemInfo	= FALSE;
	m_nCurInfoLines		= 0;
	m_strPlayerMoney	= CTString( "0" );
	m_strTotalPrice		= CTString( "0" );
	m_bIsLease			= FALSE;
	m_bIsEvent			= FALSE; //ttos : 2007 ������ �� �̺�Ʈ
	m_iEventjob			= -1;
	m_bIsFieldShop		= FALSE;
	m_nNpcVirIdx		= -1;

	int		i;

	for (i = 0; i < SHOP_SLOT_BUY_MAX; ++i)
		m_pIconsShop[i] = NULL;

	for(i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i)
		m_pIconsTrade[i] = NULL;
}

// ----------------------------------------------------------------------------
// Name : ~CUIShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIShop::~CUIShop()
{
	int		i;

	for (i = 0; i < SHOP_SLOT_BUY_MAX; ++i)
		SAFE_DELETE(m_pIconsShop[i]);

	for(i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i)
		SAFE_DELETE(m_pIconsTrade[i]);

	clearTrade();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);
	
	//m_nBackSplitHeight = 50;
	m_rcMainTitle.SetRect( 0, 0, 216, 22 );

	// Inventory region
	m_rcBuyShopSlot.SetRect( 4, 24, 190, 171 );
	m_rcBuyTradeSlot.SetRect( 4, 175, 190, 270 );
	m_rcSellShopSlot.SetRect( 4, 122, 190, 271 );
	m_rcSellTradeSlot.SetRect( 4, 24, 190, 118 );

	// Create shop texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\Shop.tex" ) );
	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();
	
	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 24, fTexWidth, fTexHeight );
	m_rtBackMiddle.SetUV( 0, 25, 216, 28, fTexWidth, fTexHeight );
	m_rtBackBottom.SetUV( 0, 29, 216, 62, fTexWidth, fTexHeight );

	// Inventory
	m_rtTopInven.SetUV( 0, 131, 216, 282, fTexWidth, fTexHeight );
	m_rtBottomInven.SetUV( 0, 283, 216, 381, fTexWidth, fTexHeight );
	m_rtSeperatorInven.SetUV( 0, 382, 216, 385, fTexWidth, fTexHeight );

	// for lease 
	m_rcBottom1.SetRect(4,175,190,270);
	m_rcBottom2.SetRect(4,270,190,291);
	m_rtBottom1.SetUV(0,284,216,286, fTexWidth, fTexHeight );
	m_rtBottom2.SetUV(0,358,216,381, fTexWidth, fTexHeight );
	m_rtBottom3.SetUV(26,42,97,48, fTexWidth, fTexHeight );
	m_rtleaseMark.SetUV(216,148,231,163,fTexWidth, fTexHeight );

	// Outline of unmovable item
	m_rtUnmovableOutline.SetUV( 218, 86, 250, 118, fTexWidth, fTexHeight );

	// Outline of selected item
	m_rtSelectOutline.SetUV( 218, 51, 250, 83, fTexWidth, fTexHeight );

	// Item information region
	m_rtInfoUL.SetUV( 0, 476, 7, 483, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 10, 476, 12, 483, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 15, 476, 22, 483, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 486, 7, 488, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 10, 486, 12, 488, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 15, 486, 22, 488, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 491, 7, 498, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 10, 491, 12, 498, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 15, 491, 22, 498, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 184, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 217, 0, 231, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 232, 0, 246, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Buy button of shop
	m_btnShopBuy.Create( this, _S( 253, "����" ), 105, 277, 51, 21 );
	m_btnShopBuy.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopBuy.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Sell button of shop
	m_btnShopSell.Create( this, _S( 254, "�Ǹ�" ), 105, 277, 51, 21 );
	m_btnShopSell.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopSell.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopSell.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopSell.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Cancel button of shop
	m_btnShopCancel.Create( this, _S( 139, "���" ), 159, 277, 51, 21 );
	m_btnShopCancel.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_ON );
	m_btnShopCancel.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Event_Weapon button of shop
	m_btnEvent_weapon.Create( this, _S( 2375, "����" ), 24, 328, 51, 21 );
	m_btnEvent_weapon.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnEvent_weapon.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnEvent_weapon.CopyUV( UBS_IDLE, UBS_ON );
	m_btnEvent_weapon.CopyUV( UBS_IDLE, UBS_DISABLE );

	// Event_Shild button of shop
	m_btnEvent_shield.Create( this, _S( 2376, "��" ), 141, 328, 51, 21 );
	m_btnEvent_shield.SetUV( UBS_IDLE, 0, 454, 51, 474, fTexWidth, fTexHeight );
	m_btnEvent_shield.SetUV( UBS_CLICK, 53, 454, 104, 474, fTexWidth, fTexHeight );
	m_btnEvent_shield.CopyUV( UBS_IDLE, UBS_ON );
	m_btnEvent_shield.CopyUV( UBS_IDLE, UBS_DISABLE );

	
	// Top scroll bar
	m_sbTopScrollBar.Create( this, 194, 28, 9, 137 );
	m_sbTopScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbTopScrollBar.SetScrollPos( 0 );
	m_sbTopScrollBar.SetScrollRange( SHOP_SLOT_ROW_BUY_TOTAL );
	m_sbTopScrollBar.SetCurItemCount( SHOP_SLOT_ROW_BUY_TOTAL );
	m_sbTopScrollBar.SetItemsPerPage( SHOP_SLOT_ROW );
	// Up button
	m_sbTopScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbTopScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbTopScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbTopScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbTopScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbTopScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbTopScrollBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbTopScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbTopScrollBar.SetWheelRect( -192, -6, 191, 148 );
	
	// Bottom scroll bar
	m_sbBottomScrollBar.Create( this, 194, 128, 9, 137 );
	m_sbBottomScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10);
	m_sbBottomScrollBar.SetScrollPos( 0 );
	m_sbBottomScrollBar.SetScrollRange( SHOP_SLOT_ROW_BUY_TOTAL );
	m_sbBottomScrollBar.SetCurItemCount( SHOP_SLOT_ROW_BUY_TOTAL );
	m_sbBottomScrollBar.SetItemsPerPage( SHOP_SLOT_ROW );
	// Up button
	m_sbBottomScrollBar.SetUpUV( UBS_IDLE, 228, 16, 237, 22, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetUpUV( UBS_CLICK, 228, 33, 237, 39, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbBottomScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_sbBottomScrollBar.SetDownUV( UBS_IDLE, 228, 25, 237, 32, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetDownUV( UBS_CLICK, 228, 42, 237, 49, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbBottomScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_sbBottomScrollBar.SetBarTopUV( 217, 16, 226, 25, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetBarMiddleUV( 217, 26, 226, 30, fTexWidth, fTexHeight );
	m_sbBottomScrollBar.SetBarBottomUV( 217, 31, 226, 41, fTexWidth, fTexHeight );
	// Wheel region
	m_sbBottomScrollBar.SetWheelRect( -192, -6, 191, 148 );
	

	// Slot items
	// Shop Slot(5x4)
	for( int i = 0; i < SHOP_SLOT_BUY_MAX; i++ )
	{
		m_pIconsShop[i] = new CUIIcon;
		m_pIconsShop[i]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SHOP, UBET_ITEM);
	}

	// Trade Slot(5x2)
	for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		m_pIconsTrade[iItem] = new CUIIcon;
		m_pIconsTrade[iItem]->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_SHOP, UBET_ITEM);
	}
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI ) / 2 - GetWidth(), ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

// ----------------------------------------------------------------------------
// Name : OpenShop()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::OpenShop( int iMobIndex, int iMobVirIdx, BOOL bHasQuest, FLOAT fX, FLOAT fZ )
{
	OBFUSCATE();

	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	ResetShop();

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	m_nNpcVirIdx = iMobVirIdx;

	CMobData* MD = CMobData::getData(iMobIndex);
	CShopData &SD = _pNetwork->GetShopData(iMobIndex);	
	const int iShopID = SD.GetIndex();


	switch( _pNetwork->MyCharacterInfo.sbJoinFlagMerac )
	{
		// �������
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		{
			if( iMobIndex == 234 || iMobIndex == 235 ) // ������ �����̸� ����
				return;
		}
		break;
		
		// ������ ���
	case WCJF_ATTACK_GUILD:
	case WCJF_ATTACK_CHAR:
		{
			if( iMobIndex == 232 || iMobIndex == 233 ) // ������ �����̸� ���� 
				return;
		}
		break;
	}

	ASSERT(iShopID != -1 && "Invalid Shop ID");
	m_nSelectedShopID = iShopID;

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	// Create refine message box
	pUIManager->CreateMessageBoxL( _S( 263, "����" ) , UI_SHOP, MSGLCMD_SHOP_REQ );

	CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	if( iMobIndex == 272 ) // ���� �뿩��
	{
		CTString tv_str;
		tv_str = _S(3054,"�賭�ϰ� ������ ���̸����� �����Ϸ��� ������ ���� �ϳ����� ������ ��ž� �� �̴ϴ�.");
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S(3055, "���Ⱑ �ʿ��Ͻôٰ��? �Ĵ°� �ƴ����� ���� ���⸦ ��ĥ�� �����帱 �� �ִµ� �����?" ) , -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str = _S(3056,"�뿩�� �帮�� ����� +12���� ���õ� ������ �� �̻��� ������ ���ϸ� 1�ϰ� ����� �� �ֽ��ϴ�."); // (+6���� +12�� ����)
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S(3057, "���� ������ ��󺸼���." ) , -1, 0xA3A1A3FF );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3058,"Ÿ��ź �����" ), LEASE_WEAPON_TITAN );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3059,"����Ʈ �����" ), LEASE_WEAPON_KNIGHT  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3060,"���� �����" ), LEASE_WEAPON_HEALER  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3061,"������ �����" ), LEASE_WEAPON_MAGE  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3062,"�α� �����" ), LEASE_WEAPON_ROGUE  );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(3063,"�Ҽ��� �����" ), LEASE_WEAPON_SOCERER  );
#ifdef CHAR_EX_ROGUE
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(5738,"EX�α� �����" ), LEASE_WEAPON_EXROGUE  );	// [2012/08/27 : Sora] EX�α� �߰�
#endif
#ifdef CHAR_EX_MAGE
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(5826,"��ũ������ �����" ), LEASE_WEAPON_EXMAGE  );	// 2013/01/08 jeil EX������ �߰� ��Ʈ�� ������ �߰� ���� �ʿ� 
#endif
		
		//�뿩��������...
		m_bIsLease = TRUE;
		SetHeight(m_nHeight+ 77);
		m_btnShopBuy.SetText(_S(3046,"�뿩"));
		m_btnShopBuy.SetPosY(m_btnShopBuy.GetPosY()+77);
		m_btnShopCancel.SetPosY(m_btnShopCancel.GetPosY()+77);

	}
	//------ ���� �޺� NPC
	else if(iMobIndex == 488)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "�ȳ��ϼ��� �ڽ��� �ɷ��� �����ϱ� ���Ͽ� ���� �޺���ŭ Ȯ���� �͵� ����"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "�ڽ��� �ɷ��� ���� ���ϰ� �ڸ��ϴٰ� ���� �޺��� Ŭ���� �ϱ�� �������."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "�� ���� �ڽ��� �ɷ��� ������ ������"),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4039, "���� �޺� �����" ), COMBO_MAKE );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "���� �̿��ϱ�" ), SHOP_BUY );			
		
	}
	else if(iMobIndex == 489)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "�ȳ��ϼ��� �ڽ��� �ɷ��� �����ϱ� ���Ͽ� ���� �޺���ŭ Ȯ���� �͵� ����"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "�ڽ��� �ɷ��� ���� ���ϰ� �ڸ��ϴٰ� ���� �޺��� Ŭ���� �ϱ�� �������."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "�� ���� �ڽ��� �ɷ��� ������ ������"),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4041, "���� �޺� �������� �̵�" ), COMBO_MOVE_STAGE );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4042, "��Ƽ�� ��ȯ�ϱ�" ), COMBO_RECALL_LOBBY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4043, "���� �޺� ������" ), COMBO_GIVEUP );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "���� �̿��ϱ�" ), SHOP_BUY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "����Ѵ�." ) );
		return;
		
	}
	else if(iMobIndex == 490)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "�ȳ��ϼ��� �ڽ��� �ɷ��� �����ϱ� ���Ͽ� ���� �޺���ŭ Ȯ���� �͵� ����"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "�ڽ��� �ɷ��� ���� ���ϰ� �ڸ��ϴٰ� ���� �޺��� Ŭ���� �ϱ�� �������."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "�� ���� �ڽ��� �ɷ��� ������ ������"),-1,0xa3a1a3ff);
		
		if(pUIManager->GetCombo()->GetComboClear() == 1)
		{	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4044, "���� ���������� �̵�" ), COMBO_MOVE_STAGE );
		}

		if(pUIManager->GetCombo()->GetComboClear() == 2)
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4045, "�޺� �Ϸ� �ʵ�� ���ư���" ), COMBO_COMPLETE );
		}else{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4046, "���� �޺� ���� �̵�" ), COMBO_MOVE_GO_LOBBY );
		}
		
		return;
				
	}
	else if(iMobIndex >= 521 && iMobIndex <= 525)			//Cube NPCs
	{
		if( pUIManager->DoesMessageBoxLExist(MSGLCMD_CUBE_PARTY_ENTRANCE) )
			pUIManager->CloseMessageBoxL(MSGLCMD_CUBE_PARTY_ENTRANCE);

		if( pUIManager->DoesMessageBoxLExist(MSGLCMD_CUBE_PARTY_ENTRANCE) )
			pUIManager->CloseMessageBoxL(MSGLCMD_CUBE_GUILD_ENTRANCE);

		if( pUIManager->DoesMessageBoxExist(MSGCMD_CUBE_STATE) )
			pUIManager->CloseMessageBox(MSGCMD_CUBE_STATE);
			
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4321, "�� ť�꿡 ���� ���ؼ��� ������� �ʿ��մϴ�."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4322, "�� ��Ƽ�� �ؼ� ���� �մϴ�. ��Ƽ�� ���� ���� ������ �� �� �����ϴ�."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4323, "��ü ������ �ϰ� �Ǹ� ��Ƽ���� ������ �̵��ϰ� �˴ϴ�."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4324, "�׷� ����� ���ϴ�."),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4325, "����ť�� ����" ), SHOP_BUY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4326, "��Ƽť������ ��û" ), CUBE_PARTY_ENTRANCE);
#ifdef ENABLE_GUILD_CUBE
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4394, "���ť������ ��û" ), CUBE_GUILD_ENTRANCE);
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(5441, "���� ť�� ���� ��û" ), CUBE_PLAYER_REWARD);
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4390, "���� ť�� ����Ʈ ��Ȳ" ), CUBE_PLAYER_STATE);
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4388, "��� ť�� ����Ʈ ��Ȳ" ), CUBE_GUILD_STATE);
#endif
	}
	else
	{
		if( iMobIndex == 233 || iMobIndex == 235 ) // ���� ���ջ���
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1975, "���忡�� ��縦 �Ѵٴ� �� ����� ���ɰ� �ϴ� ������." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1976, "�׸�ŭ ���� �� �̱⵵ �ϰ� �׷��ٰ� �ٰ����� ����ų� ������ �ʳ�..." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, CTString( " " ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1977, "������ ���ǵ��� ���� ������ ��󺸰�.." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, CTString( " " ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 1978, "�� ���� �����۸� ����ϴ� ������ �ΰ�. ���� ������ ������ ���񽺷� ���ְ� ����. " ) , -1, 0xA3A1A3FF );	
		}
		else
		{
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 621, "� ���ð�." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 622, "�� �ٹ濡���� �׷��� ���Ⱑ ���� ���ݵ� �ΰ� ������ �پ� �� ����." ) , -1, 0xA3A1A3FF );	
			pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, _S( 623, "��, �� �� ������ ���ٸ� �� �� ������ �ٸ� ���� ���� ����������." ) , -1, 0xA3A1A3FF );	
		}


		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 624, "�����Ѵ�." ), SHOP_BUY  );		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 625, "�Ǹ��Ѵ�." ), SHOP_SELL  );		
	}
	
	if( iMobIndex == 233 || iMobIndex == 235 ) // ���� ���ջ���
	{	
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1979, "�������� �����Ѵ�." ), SHOP_GW_MIX );	
	}

	if(bHasQuest)
	{
		// 2009. 05. 27 ������
		// �̾߱��Ѵ� ���� ó��
		CUIQuestBook::AddQuestListToMessageBoxL(MSGLCMD_SHOP_REQ);				
	}

	// FIXME : ����Ʈ�� ���� ��쿡 ������ ��.
	// FIXME : ���, �̺�Ʈ NPC�� �ǵ��� ����Ʈ�� ���� �ִ� ���·�???
	if(MD->IsEvent())
	{
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 100, "�̺�Ʈ" ), SHOP_EVENT  );		
	}

	// ($E_WC2010) [100511: selo] 2010 ���ư� ������ Event1.
	// �ش� �̺�Ʈ�� ���� ���̰� �� ������ �� �޴��� ���δ�
	if(IS_EVENT_ON(TEVENT_WORLDCUP_2010) &&
		(iMobIndex == 90 || iMobIndex == 129 || iMobIndex == 169 || iMobIndex == 279 || iMobIndex == 508 || iMobIndex == 1029) )
	{
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4889, "���� ���� �̺�Ʈ �ȳ�"), EVENT_WORLDCUP_2010_INFO );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4890, "Ȳ�� �౸������ ��ȯ"), EVENT_WORLDCUP_2010_BALL );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4881, "���� ���� ���ڷ� ��ȯ"), EVENT_WORLDCUP_2010_GOLDENBALL );
	}

	if(iMobIndex == 226)
	{
		pUIManager->CloseMessageBox( MSGCMD_SUPERGOJE_REQUEST );
		pUIManager->CloseMessageBox( MSGCMD_SUPERGOJE_NOTIFY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1782, "�� ��� ���ü� �ޱ�" ), SHOP_SUPERGOJE  );
	}

	pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ, FALSE, _S( 1748, "NPC �ȳ�" ), SHOP_NPC_HELP); //ttos : �ȳ��ý��� �߰���
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "����Ѵ�." ) );	
}

// ttos : ������ �� �̺�Ʈ(��̳�) �߰�
void CUIShop::EventOpenShop( int iMobIndex, BOOL bHasQuest,FLOAT fX, FLOAT fZ )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if(pUIManager->DoesMessageBoxLExist( MSGLCMD_SHOP_REQ ))
		return;

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	ResetShop();

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	pUIManager->CreateMessageBoxL(_S(169,"�̺�Ʈ"), UI_SHOP, MSGLCMD_SHOP_REQ ); // ���� �̺�Ʈ Ÿ��Ʋ��
	
	CTString	strNpcName = CMobData::getData(iMobIndex)->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	if( iMobIndex == 254 /*&& IS_EVENT_ON(TEVENT_MAY)*/)
	{
		CTString tv_str;
		tv_str = _S(3401, "���� ���� ������ �ϱ⿡�� ��� ������ �� ������ ���� +3������ ������ �帱 �� �ִµ��� �� �� �����ֽǷ���?");
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str = _S(3402, "���� ���� �ʹ� �ٺ��� �����ϴ� ��� ���������� ���ش� �ֽø� ���� ��� �帱���� �ѹ� ���� ���� �� �ٽ� �־����� �ʽ��ϴ�.");
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, tv_str,-1, 0xA3A1A3FF );
		tv_str.PrintF("%s %s",_S(43,"Ÿ��ź"),_S(2736,"��� ������"));
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_TITAN );
		tv_str.PrintF("%s %s",_S(44,"���"),_S(2736,"��� ������"));
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_KNIGHT  );
		tv_str.PrintF("%s %s",_S(45,"����"),_S(2736,"��� ������"));
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_HEALER  );
		tv_str.PrintF("%s %s",_S(46,"������"),_S(2736,"��� ������"));
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_MAGE  );
		tv_str.PrintF("%s %s",_S(47,"�α�"),_S(2736,"��� ������"));
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_ROGUE  );
		tv_str.PrintF("%s %s",_S(48,"�Ҽ���"),_S(2736,"��� ������"));
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_SOCERER  );
#ifdef CHAR_EX_ROGUE
		tv_str.PrintF("%s %s",_S(5732,"EX�α�"),_S(2736,"��� ������"));	// [2012/08/27 : Sora] EX�α� �߰�
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_ROGUE  );
#endif
#ifdef CHAR_EX_MAGE		//2013/01/08 jeil EX������ �߰� ��Ʈ�� ������ �߰� ���� �ʿ� 
		tv_str.PrintF("%s %s",_S(5820,"��ũ������"),_S(2736,"��� ������"));	
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, tv_str, LEASE_WEAPON_MAGE  );
#endif
		
		//�뿩������ ������ �� �̺�Ʈ��...
		m_bIsLease = TRUE;
		m_bIsEvent = TRUE;
		SetHeight(m_nHeight+ 77);
		m_btnShopBuy.SetText(_S(191,"Ȯ��"));
		m_btnShopBuy.SetPosY(m_btnShopBuy.GetPosY()+77);
		m_btnShopCancel.SetPosY(m_btnShopCancel.GetPosY()+77);

	}

	pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ, FALSE, _S( 1748, "NPC �ȳ�" ), SHOP_NPC_HELP); //ttos : �ȳ��ý��� �߰���
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "����Ѵ�." ) );

}

//---------------------------------------------------------------------->>
// Name : PrepareLeaseShop()
// Desc : 
// Date : [6/29/2006] , Wooss
//----------------------------------------------------------------------<<
void CUIShop::PrepareLeaseShop(int jobIdx, int iType)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if( pUIManager->GetInventory()->IsLocked() )
	{
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_SHOP );

	// Lock inventory
	if(!m_bIsEvent)
		pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );

	// Set region
	m_rcTop.CopyRect( m_rcBuyShopSlot );
	m_rcBottom.CopyRect( m_rcBuyTradeSlot );

	m_bBuyShop			= TRUE;

	int	ctShopItems;
	if(m_bIsEvent) // ttos : ������ �� �̺�Ʈ
	{
		ClearItems();
		m_iEventjob = jobIdx;
		ctShopItems = RefreshEventItem( m_iEventjob , iType); // ttos : �̺�Ʈ ������ ����
	}else
	{
		ctShopItems = RefreshLeaseItem( jobIdx ); // �뿩 ������ ����
	}
	
	
	int	nScrollItem = ( ctShopItems + SHOP_SLOT_COL - 1 ) / SHOP_SLOT_COL;
	m_sbTopScrollBar.SetCurItemCount( nScrollItem );

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( m_strPlayerMoney );
	}

	// Set size & position
	//SetSize( SHOP_MAIN_WIDTH, SHOP_MAIN_HEIGHT );
	CDrawPort	*pdp = pUIManager->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade itemstest5	
	nX = SHOP_BUY_BOTTOM_SLOT_SX;
	nY = SHOP_BUY_BOTTOM_SLOT_SY;
	for( int nItem = 0; nItem < SHOP_TRADE_SLOT_TOTAL; nItem++ )
	{
		if( nItem == SHOP_TRADE_SLOT_COL )
		{
			nY += 35;	nX = SHOP_BUY_BOTTOM_SLOT_SX;
		}
		m_pIconsTrade[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_SHOP, TRUE );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::PrepareBuyShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		// �̹� Lock �� â�� ���� ��� ���� ���Ѵ�.
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_SHOP );

	// Set region
	m_rcTop.CopyRect( m_rcBuyShopSlot );
	m_rcBottom.CopyRect( m_rcBuyTradeSlot );

	m_bBuyShop			= TRUE;
	int	ctShopItems = RefreshShopItem( m_nSelectedShopID );
	int	nScrollItem = ( ctShopItems + SHOP_SLOT_COL - 1 ) / SHOP_SLOT_COL;
	m_sbTopScrollBar.SetCurItemCount( nScrollItem );

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( m_strPlayerMoney );
	}

	// Set size & position
	//SetSize( SHOP_MAIN_WIDTH, SHOP_MAIN_HEIGHT );
	CDrawPort	*pdp = pUIManager->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade itemstest5	
	nX = SHOP_BUY_BOTTOM_SLOT_SX;
	nY = SHOP_BUY_BOTTOM_SLOT_SY;
	for( int nItem = 0; nItem < SHOP_TRADE_SLOT_TOTAL; nItem++ )
	{
		if( nItem == SHOP_TRADE_SLOT_COL )
		{
			nY += 35;	nX = SHOP_BUY_BOTTOM_SLOT_SX;
		}
		m_pIconsTrade[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_SHOP, TRUE );

	// does not used help system in russia [9/1/2010 rumist]
	if (g_iCountry != RUSSIA)
	{
		if(g_iShowHelp1Icon)
		{
			pUIManager->GetHelp3()->ClearHelpString();
			pUIManager->GetHelp3()->AddHelpString(_S(3301, "����� ��ǰ �� �����ǻ簡 �ִ� ��ǰ�� ���� Ŭ���ϰų� �ϴ��� ��ĭ�� �巡�� �� �� ���� ��ư�� Ŭ���ϸ� ���Ű� �̷�� ���ϴ�."));
			pUIManager->GetHelp3()->AddHelpString(_S(3302, "�� �ִ� 10���� �������� �ѹ��� ������ �� �ֽ��ϴ�."));
			pUIManager->GetHelp3()->AddHelpString(_S(3303, "�� ������ ���� �� �ڵ����� ǥ�� �� ������ ���ҵǰ� ������ �������� ĳ������ �κ��丮�� ���ɴϴ�."));
			pUIManager->GetHelp3()->OpenHelp(this);
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::PrepareSellShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	// If inventory is already locked
	if (pUIManager->GetInventory()->IsLocked() == TRUE ||
		pUIManager->GetInventory()->IsLockedArrange() == TRUE)
	{
		// �̹� Lock �� â�� ���� ��� ���� ���Ѵ�.
		pUIManager->GetInventory()->ShowLockErrorMessage();
		return;
	}

	// Lock inventory
	pUIManager->GetInventory()->Lock( TRUE, FALSE, LOCK_SHOP );

	// Character state flags
	pUIManager->SetCSFlagOn( CSF_SHOP );

	// Set region
	m_rcTop.CopyRect( m_rcSellShopSlot );
	m_rcBottom.CopyRect( m_rcSellTradeSlot );

	m_bBuyShop			= FALSE;
	RefreshPlayerItem();

	// Set money
	if( _pNetwork->MyCharacterInfo.money > 0 )
	{
		m_strPlayerMoney.PrintF( "%I64d", _pNetwork->MyCharacterInfo.money );
		pUIManager->InsertCommaToString( m_strPlayerMoney );
	}

	// Set size & position
	//SetSize( SHOP_MAIN_WIDTH, SHOP_MAIN_HEIGHT );
	CDrawPort	*pdp = pUIManager->GetDrawPort();
	int	nX = ( pdp->dp_MinI + pdp->dp_MaxI ) / 2 - GetWidth() / 2;
	int	nY = ( pdp->dp_MinJ + pdp->dp_MaxJ ) / 2 - GetHeight() / 2;
	SetPos( nX, nY );

	// Set position of trade items
	nX = SHOP_SELL_TOP_SLOT_SX;
	nY = SHOP_SELL_TOP_SLOT_SY;
	for( int nItem = 0; nItem < SHOP_TRADE_SLOT_TOTAL; nItem++ )
	{
		if( nItem == SHOP_TRADE_SLOT_COL )
		{
			nY += 35;	nX = SHOP_SELL_BOTTOM_SLOT_SX;
		}
		m_pIconsTrade[nItem]->SetPos( nX, nY );
		nX += 35;
	}

	pUIManager->RearrangeOrder( UI_SHOP, TRUE );

// [KH_07044] 3�� ���� ���� �߰�
	if(g_iShowHelp1Icon)
	{
		pUIManager->GetHelp3()->ClearHelpString();
		pUIManager->GetHelp3()->AddHelpString(_S(3304, "�ϴ��� �� �κ��丮���� ������ �Ǹ��Ϸ��� ��ǰ�� ����Ŭ�� �ϰų� ����� ��ĭ�� �巡�� �� �� �Ǹ� ��ư�� Ŭ���ϸ� �ǸŰ� �̷�� ���ϴ�."));
		pUIManager->GetHelp3()->AddHelpString(_S(3305, "�� �ִ� 10���� �������� �ѹ��� �Ǹ��� �� �ֽ��ϴ�."));
		pUIManager->GetHelp3()->AddHelpString(_S(3306, "�� ����׵θ��� �ѷ��� �������� ���ο��� �Ǹ��� �� ���� �������Դϴ�."));
		pUIManager->GetHelp3()->AddHelpString(_S(3307, "�� �� �� �Ǹ��� �������� �ٽ� �������� ������ �����Ͻñ� �ٶ��ϴ�."));
		pUIManager->GetHelp3()->OpenHelp(this);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::RefreshPlayerItem()
{
	// �ǸŸ���̹Ƿ� Inventory ������ŭ�� ������Ʈ �Ѵ�.

	int		t, i, idx;
	for (i = 0; i < SHOP_SLOT_SELL_MAX; ++i)
	{

		if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			t = INVENTORY_TAB_CASH_2;
			idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			t = INVENTORY_TAB_CASH_1;
			idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			t = INVENTORY_TAB_NORMAL;
			idx = i;
		}

		CItems*	pItems = &_pNetwork->MySlotItem[t][idx];
		if (pItems->Item_Sum > 0)
		{
			// �κ��丮 ���� �ÿ��� t, idx ��ȯ�ϰ�, shop �� ���ȴ� ���� ���� �ε��� ���
			CItems* pCopy = new CItems;
			memcpy(pCopy, pItems, sizeof(CItems));
			m_pIconsShop[i]->setData(pCopy, false);
			m_pIconsShop[i]->setCount(pItems->Item_Sum);		// BtnEx ���ſ�
			m_pIconsShop[i]->Hide(FALSE);
		}
	}
}

// WSS_SHOP_BUGFIX 070530 -------------------------------------->>
void CUIShop::CloseShop()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->RearrangeOrder( UI_SHOP, FALSE );

	// Unlock inventory
	pUIManager->GetInventory()->Lock( FALSE, FALSE, LOCK_SHOP );

	// Character state flags
	pUIManager->SetCSFlagOff( CSF_SHOP );

	// Close message box of shop
	pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
	pUIManager->GetMsgBoxNumOnly()->CloseBox();

	m_nNpcVirIdx = -1;
}
// -------------------------------------------------------------<<

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::ResetShop()
{
	m_nSelShopItemID = -1;
	m_nSelTradeItemID = -1;
	m_nSelectedShopID = -1;
	m_strTotalPrice = CTString( "0" );
	m_strPlayerMoney = CTString( "0" );
	m_nTotalPrice = 0;
	m_nSendTotalPrice = 0;
	m_nNumOfItem = 0;
	
	ClearItems();	
	
	m_nCurRow = 0;
	m_sbBottomScrollBar.SetScrollPos( 0 );
	m_sbTopScrollBar.SetScrollPos( 0 );

	// Character state flags
	CUIManager::getSingleton()->SetCSFlagOff( CSF_SHOP );

	// Lease shop 
	if(m_bIsLease)
	{
		m_bIsLease	= FALSE;
		m_bIsEvent	= FALSE; // ttos : 2007 ������ �� �̺�Ʈ
		SetHeight(m_nHeight - 77);
		m_btnShopBuy.SetText(_S(253,"����"));
		m_btnShopBuy.SetPosY(m_btnShopBuy.GetPosY()-77);
		m_btnShopCancel.SetPosY(m_btnShopCancel.GetPosY()-77);
	}

	clearTrade();
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CUIShop::ClearItems()
{
	int		i;
	for( i = 0; i < SHOP_TRADE_SLOT_TOTAL; i++)
		m_pIconsTrade[i]->clearIconData();

	for( i = 0; i < SHOP_SLOT_BUY_MAX; i++)
	{
		m_pIconsShop[i]->clearIconData();
	}
}

// ----------------------------------------------------------------------------
// Name : RenderSellItems()
// Desc : ���Ը��...
// ----------------------------------------------------------------------------
void CUIShop::RenderShopItems()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	int iShopX, iShopY;
	
	if(m_bBuyShop)
	{
		iShopX = SHOP_BUY_TOP_SLOT_SX;
		iShopY = SHOP_BUY_TOP_SLOT_SY;
	}
	else
	{
		iShopX = SHOP_SELL_BOTTOM_SLOT_SX;
		iShopY = SHOP_SELL_BOTTOM_SLOT_SY;
	}

	int	i;
	int	nX = iShopX;
	int	nY = iShopY;
	int	iRowS = m_nCurRow * SHOP_SLOT_COL;
	int	iRowE = (m_nCurRow + SHOP_SLOT_ROW) * SHOP_SLOT_COL;

	for( i = iRowS; i < iRowE; i++ )
	{
		if ((i % SHOP_SLOT_COL) == 0)
		{
			nX = iShopX;	
			if (i > iRowS)	nY += 35;
		}

		m_pIconsShop[i]->SetPos( nX, nY );
		nX += 35;

		if( m_pIconsShop[i]->IsEmpty() )
			continue;

		m_pIconsShop[i]->Render(pDrawPort);
	}

	// ---Trade slot items---
	for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		if( m_pIconsTrade[iItem]->IsEmpty() )
			continue;
			
		m_pIconsTrade[iItem]->Render(pDrawPort);
	}	

	// Render all button elements
	pDrawPort->FlushBtnRenderingQueue( UBET_ITEM );


	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Outline of items in shop slot ( unmovable )
	if( !m_bBuyShop )
	{
		for( i = iRowS; i < iRowE; i++ )
		{
			// If button is empty
			if( m_pIconsShop[i]->IsEmpty() )
				continue;

			// Not wearing, not refine stone, can trade
			int			nIndex = m_pIconsShop[i]->getIndex();
			CItems*		pItems = m_pIconsShop[i]->getItems();
			CItemData*	pItemData = pItems->ItemData;

			m_pIconsShop[i]->GetAbsPos( nX, nY );
#ifdef ADD_SUBJOB
			if (pItemData->IsFlag(ITEM_FLAG_SELLER))
			{
				if( pUIManager->CheckSellerItem(UI_SHOP, pItemData->GetFlag()) )
					continue;
			}
			else
			{
				if ((pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE) &&
					(pItemData->GetFlag() & ITEM_FLAG_TRADE) && !pItems->IsFlag(FLAG_ITEM_LENT))
					continue;
			}
#else
			if ((pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE) &&
				(pItemData->GetFlag() & ITEM_FLAG_TRADE) && !pItems->IsFlag(FLAG_ITEM_LENT))
				continue;
#endif
			pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
												m_rtUnmovableOutline.U0, m_rtUnmovableOutline.V0,
												m_rtUnmovableOutline.U1, m_rtUnmovableOutline.V1,
												0xFFFFFFFF );
				
		}
	}

	// Mark Lease Items
	for( i = iRowS; i < iRowE; i++ )
	{
		if (m_pIconsShop[i]->IsEmpty() || m_bIsEvent)
			continue;
		// Mark lease item
			CItems*	pItems = &_pNetwork->MySlotItem[0][i];
			if(m_bIsLease/*|| pItems->IsFlag(FLAG_ITEM_LENT)*/) //m_abtnShopItems[iRow][iCol].GetItemFlag()&FLAG_ITEM_LENT)
		{
			int tv_posX,tv_posY;
			m_pIconsShop[i]->GetAbsPos( tv_posX, tv_posY );
			pDrawPort->AddTexture( tv_posX+17, tv_posY+17, tv_posX+32, tv_posY+32,
												m_rtleaseMark.U0, m_rtleaseMark.V0,
												m_rtleaseMark.U1, m_rtleaseMark.V1,
												0xFFFFFFFF );
		}
	}


	// Outline of selected item
	if( m_nSelTradeItemID >= 0 )
	{
		m_pIconsTrade[m_nSelTradeItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
		
	}
	if( m_nSelShopItemID >= 0 )
	{
		m_pIconsShop[m_nSelShopItemID]->GetAbsPos( nX, nY );
		pDrawPort->AddTexture( nX, nY, nX + BTN_SIZE, nY + BTN_SIZE,
											m_rtSelectOutline.U0, m_rtSelectOutline.V0,
											m_rtSelectOutline.U1, m_rtSelectOutline.V1,
											0xFFFFFFFF );
	}

	// ----------------------------------------------------------------------------
	// Item information ( name and property etc... )
	if( m_bShowItemInfo && !m_bIsLease)
	{
		// Item information region
		pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top,
											m_rcItemInfo.Right, m_rcItemInfo.Top + 7,
											m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Top + 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom - 7,
											m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom,
											m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Left + 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom,
											m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
											0xFFFFFFFF );
		pDrawPort->AddTexture( m_rcItemInfo.Right - 7, m_rcItemInfo.Bottom - 7,
											m_rcItemInfo.Right, m_rcItemInfo.Bottom,
											m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
											0xFFFFFFFF );

		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}
	else
	{
		// Render all elements
		pDrawPort->FlushRenderingQueue();
	}
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::Render()
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcZ;
	if( fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST )
	{
		ResetShop();
		CloseShop();
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();

	// Set shop texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int	nX, nY;

	// Add render regions
	pDrawPort->AddTexture( m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + 24,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + 24,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight - 33,
										m_rtBackMiddle.U0, m_rtBackMiddle.V0,
										m_rtBackMiddle.U1, m_rtBackMiddle.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_nPosX, m_nPosY + m_nHeight - 33,
										m_nPosX + m_nWidth, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	
	// Inventory
	if(m_bBuyShop)
	{
		// Buy
		nX = m_nPosX;
		nY = m_nPosY + 22;
		pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_SHOP_HEIGHT,
											m_rtTopInven.U0, m_rtTopInven.V0,
											m_rtTopInven.U1, m_rtTopInven.V1,
											0xFFFFFFFF );

		nY += SHOP_SHOP_HEIGHT;
		pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + 1,
											m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
											m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
											0xFFFFFFFF );

		nY += 1;
		if(m_bIsLease)
		{
			// �뿩���� �Ʒ��κ�
			pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_LEASE_HEIGHT+77 ,
											m_rtBottom1.U0, m_rtBottom1.V0,
											m_rtBottom1.U1, m_rtBottom1.V1,
											0xFFFFFFFF );
			pDrawPort->AddTexture( nX+7, nY+4,
											nX + SHOP_SHOP_WIDTH -7, nY + SHOP_LEASE_HEIGHT+77-4,
											m_rtBottom3.U0, m_rtBottom3.V0,
											m_rtBottom3.U1, m_rtBottom3.V1,
											0xFFFFFFFF );
			nY += SHOP_LEASE_HEIGHT + 77;

			if(m_bIsEvent)
			{
				m_btnEvent_weapon.Render();
				m_btnEvent_shield.Render();

			}else{
				pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + 21,
											m_rtBottom2.U0, m_rtBottom2.V0,
											m_rtBottom2.U1, m_rtBottom2.V1,
											0xFFFFFFFF );
			}
			
		}
		else 
		{
			pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_TRADE_HEIGHT,
											m_rtBottomInven.U0, m_rtBottomInven.V0,
											m_rtBottomInven.U1, m_rtBottomInven.V1,
											0xFFFFFFFF );			
		}
		// Bottom scroll bar		
		m_sbTopScrollBar.Render();
	}
	else
	{
		// Sell
		nX = m_nPosX;
		nY = m_nPosY + 22;
		pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_TRADE_HEIGHT,
											m_rtBottomInven.U0, m_rtBottomInven.V0,
											m_rtBottomInven.U1, m_rtBottomInven.V1,
											0xFFFFFFFF );

		nY += SHOP_TRADE_HEIGHT;
		pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + 1,
											m_rtSeperatorInven.U0, m_rtSeperatorInven.V0,
											m_rtSeperatorInven.U1, m_rtSeperatorInven.V1,
											0xFFFFFFFF );

		nY += 1;
		pDrawPort->AddTexture( nX, nY,
											nX + SHOP_SHOP_WIDTH, nY + SHOP_SHOP_HEIGHT,
											m_rtTopInven.U0, m_rtTopInven.V0,
											m_rtTopInven.U1, m_rtTopInven.V1,
											0xFFFFFFFF );

		// Bottom scroll bar		
		m_sbBottomScrollBar.Render();		
	}

	// Close button
	m_btnClose.Render();

	// Buy & sell button of shop
	if(m_bBuyShop)
		m_btnShopBuy.Render();
	else
		m_btnShopSell.Render();
	
	// Cancel button of shop
	m_btnShopCancel.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();

	// Render item information
	if(m_bIsEvent) // ttos : ������ �� �̺�Ʈ
	{
		pDrawPort->PutTextEx( _S( 100, "�̺�Ʈ" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );
	}else
	{
		pDrawPort->PutTextEx( _S( 263, "����" ), m_nPosX + INVEN_TITLE_TEXT_OFFSETX,
										m_nPosY + INVEN_TITLE_TEXT_OFFSETY );
	}
	

	COLOR colNas,colNasP;
	colNas = pUIManager->GetNasColor( m_strTotalPrice );
	colNasP = pUIManager->GetNasColor( m_strPlayerMoney );
	
	if(m_bIsLease)
	{
		int tv_posY = m_nPosY + LEASE_INFO_TEXT_OFFSETX + 10;
		int tv_gap	= _pUIFontTexMgr->GetLineHeight() +2;
		
		CTString tv_str;
		int tv_int=0;
		COLOR tv_clr = 0xFFF377FF;

		tv_posY+= tv_gap;
		if(m_nSelShopItemID < 0)
		{
			pDrawPort->PutTextEx( _S(3065, "�����̸�" ), m_nPosX + 10, tv_posY,tv_clr);
		}
		tv_posY+= tv_gap;
		pDrawPort->PutTextEx( _S(3066, "���ⷹ��" ), m_nPosX + 10, tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		pDrawPort->PutTextEx( _S(3067, "���Ŭ����" ), m_nPosX + 10,	tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		pDrawPort->PutTextEx( _S(85, "���ݷ�" ), m_nPosX + 10,	tv_posY ,tv_clr);
		tv_posY+= tv_gap;
		pDrawPort->PutTextEx( _S(86, "�������ݷ�" ), m_nPosX + 10,	tv_posY ,tv_clr);
		tv_posY+= tv_gap;

		if(m_bIsEvent) // ttos : ������ �� �̺�Ʈ
		{
		//	tv_str = CTString("������ ���� ");
			tv_str.PrintF("%s %s", _pNetwork->GetItemName(2344), _S(2396,"����"));
			pDrawPort->PutTextEx( tv_str, m_nPosX + 10, tv_posY ,tv_clr);
		}else
		{
			pDrawPort->PutTextEx( _S(3071, "�뿩�Ⱓ" ), m_nPosX + 10, tv_posY ,tv_clr);
		}
		tv_posY+= tv_gap;
		
		if(m_nSelShopItemID >= 0)
		{
			//WSS_SHOP_BUGFIX_70530 --->><<
			int tv_itemIdx = m_pIconsShop[m_nSelShopItemID]->getIndex();
			CItemData	*pItemData = _pNetwork->GetItemData( tv_itemIdx );

			tv_posY = m_nPosY + LEASE_INFO_TEXT_OFFSETX + 10;
			// Weapon Name
			const char* szItemName = _pNetwork->GetItemName( tv_itemIdx );
			
			if(m_bIsEvent) 
				tv_str = CTString("+3 ");
			else if ( g_iCountry == KOREA || g_iCountry == USA || g_iCountry == RUSSIA || g_iCountry == THAILAND )
				tv_str = CTString("+12 ");	// ���� ����뿩������ ���� +6 ���� +12�� ����
			else
				tv_str = CTString("+6 ");

			tv_str +=szItemName;
			pDrawPort->PutTextExCX( tv_str, m_nPosX + 108, tv_posY, tv_clr);
			tv_posY+= tv_gap*2;
			
			int iExtraPos =0;
			
			if (g_iCountry == THAILAND)
			{
				iExtraPos =52;
			}			

			// Weapon Level
			tv_int =pItemData->GetLevel();
			tv_str.PrintF("%d",tv_int);
			pDrawPort->PutTextEx( tv_str, m_nPosX + 80 +iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			// Class 
			tv_int = pItemData->GetJob();	//�α�Ex�α� 144 ������ ��ũ������264
			switch(tv_int)
			{
			case 1:
					tv_str = _S(43,"Ÿ��ź");
					break;
			case 2:
					tv_str = _S(44,"���");
					break;
			case 4:
					tv_str = _S(45,"����");
					break;
			case 8:
				tv_str = _S(46,"������");
				break;
			case 264:	//2013/01/22 jeil ���� �뿩�󿡼� ������� ���� �߸��������� ����
					tv_str = _S(46,"������");
#ifdef CHAR_EX_MAGE
					tv_str += _S(5820,"��ũ������");	// 2013/01/08 jeil EX������ �߰� ��Ʈ�� ������ �߰����� �ʿ� 
#endif
					break;
			case 16:
				tv_str = _S(47,"�α�");
				break;
			case 144:	//2013/01/22 jeil ���� �뿩�󿡼� ������� ���� �߸��������� ����
					tv_str = _S(47,"�α�");
#ifdef CHAR_EX_ROGUE
					tv_str += _S(5732,"EX�α�");	// [2012/08/27 : Sora] EX�α� �߰�
#endif
					break;
			case 32:
					tv_str = _S(48,"�Ҽ���");
					break;
			default : 
				tv_str = CTString("Alien");
				break;
			}		
			pDrawPort->PutTextEx( tv_str, m_nPosX + 80 +iExtraPos, tv_posY);
			tv_posY+= tv_gap;

			int tv_plus = m_bIsEvent ? 3 : 6 ;  // ������ +6���� ����

			if (g_iCountry == KOREA || g_iCountry == USA || g_iCountry == RUSSIA || g_iCountry == THAILAND)
			{
				tv_plus = m_bIsEvent ? 3 : 12 ;  // +6���� ����(+6���� +12�� ����)
			}
			
			// attack ability
			tv_int = CItems::CalculatePlusDamage( pItemData->GetPhysicalAttack(), tv_plus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
			if( tv_int > 0 )
				tv_str.PrintF( CTString("%d + %d" ), pItemData->GetPhysicalAttack(), tv_int );
			else
				tv_str.PrintF( CTString( "%d" ), pItemData->GetPhysicalAttack() );
			pDrawPort->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			// magic attack ability
			tv_int = CItems::CalculatePlusDamage( pItemData->GetMagicAttack(), tv_plus, pItemData->GetLevel() >= 146 ? TRUE : FALSE );
			if( tv_int > 0 )
				tv_str.PrintF( CTString("%d+%d" ), pItemData->GetMagicAttack(), tv_int );
			else
				tv_str.PrintF( CTString( "%d" ), pItemData->GetMagicAttack() );
			pDrawPort->PutTextEx( tv_str, m_nPosX + 80 +iExtraPos, tv_posY);
			tv_posY+= tv_gap;
			
			if(m_bIsEvent) //ttos : ������ �� �̺�Ʈ
			{
				tv_int = 1;
				tv_str.PrintF(_S(766,"%d ��"),(int)((pItemData->GetLevel()/2.0f)+0.5f));
				pDrawPort->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
				tv_posY+= tv_gap;

			}else
			{
				tv_int = 1;
				tv_str.PrintF(_S(3079,"%d��"),tv_int);
				pDrawPort->PutTextEx( tv_str, m_nPosX + 80+iExtraPos, tv_posY);
				tv_posY+= tv_gap;

			}
			
			if(!m_bIsEvent)
			{
				// Lease Price
				tv_int =(pItemData->GetPrice()*1.5) + 50000;
				tv_str.PrintF("%d",tv_int);
				pUIManager->InsertCommaToString( tv_str );
	//			pDrawPort->PutTextEx( tv_str, m_nPosX + 80, tv_posY);
			}
			
		}
		// Set Nas
		if(!m_bIsEvent) // ttos : ������ �� �̺�Ʈ�� ���� ���� ����
		{
			pDrawPort->PutTextExRX( tv_str, m_nPosX + SHOP_TRADEPRICE_POSX, m_nPosY + SHOP_BUY_TRADEPRICE_POSY + 77, colNas );
		}		
		pDrawPort->PutTextExRX( m_strPlayerMoney, m_nPosX + SHOP_MYMONEY_POSX, m_nPosY + SHOP_MYMONEY_POSY + 77, colNasP );
		
	}
	else
	{
		if(m_bBuyShop)
			pDrawPort->PutTextExRX( m_strTotalPrice, m_nPosX + SHOP_TRADEPRICE_POSX, m_nPosY + SHOP_BUY_TRADEPRICE_POSY, colNas );
		else
			pDrawPort->PutTextExRX( m_strTotalPrice, m_nPosX + SHOP_TRADEPRICE_POSX, m_nPosY + SHOP_SELL_TRADEPRICE_POSY, colNas );

		
		pDrawPort->PutTextExRX( m_strPlayerMoney, m_nPosX + SHOP_MYMONEY_POSX, m_nPosY + SHOP_MYMONEY_POSY, colNasP );
		
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render Items
	RenderShopItems();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIShop::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}			

			CUIManager* pUIManager = CUIManager::getSingleton();			

			// Close button
			if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Buy button of shop
			else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Sell button of shop
			else if( !m_bBuyShop && m_btnShopSell.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Cancel button of shop
			else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			// Top Scroll bar
			else if( m_bBuyShop && ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			// Bottom Scroll bar
			else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if( wmsgResult == WMSG_COMMAND)
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();

				return WMSG_SUCCESS;
			}
			else if (pUIManager->GetDragIcon() == NULL && bLButtonDownInItem && 
					 (pMsg->wParam& MK_LBUTTON) && (ndX != 0 || ndY != 0))
			{
				if(m_bIsLease) break;

				// Shop items
				if( m_nSelShopItemID >= 0 )
				{
					if( m_bBuyShop )
					{
						// Close message box of shop
						pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
						pUIManager->GetMsgBoxNumOnly()->CloseBox();

						pUIManager->SetHoldBtn(m_pIconsShop[m_nSelShopItemID]);
					}
					else
					{
						int		t, idx;

						if (m_nSelShopItemID >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
						{
							t = INVENTORY_TAB_CASH_2;
							idx = m_nSelShopItemID - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
						}
						else if (m_nSelShopItemID >= ITEM_COUNT_IN_INVENTORY_NORMAL)
						{
							t = INVENTORY_TAB_CASH_1;
							idx = m_nSelShopItemID - ITEM_COUNT_IN_INVENTORY_NORMAL;
						}
						else
						{
							t = INVENTORY_TAB_NORMAL;
							idx = m_nSelShopItemID;
						}


						// Not wearing, not refine stone, can trade
						CItems		*pItems = &_pNetwork->MySlotItem[t][idx];
						CItemData	*pItemData = pItems->ItemData;

						if (pItemData == NULL)
							return WMSG_FAIL;

#ifdef ADD_SUBJOB
						if (pItemData->IsFlag( ITEM_FLAG_SELLER ) ? 
							pUIManager->CheckSellerItem(UI_SHOP, pItemData->GetFlag()) : 
							pItems->Item_Wearing == -1 && 
							(pItemData->GetType() != CItemData::ITEM_ETC ||
							 pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE) &&
							pItemData->GetFlag() & ITEM_FLAG_TRADE && !pItems->IsFlag(FLAG_ITEM_LENT))
#else
						if (pItems->Item_Wearing == -1 &&
							(pItemData->GetType() != CItemData::ITEM_ETC || 
							 pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE) &&
							pItemData->GetFlag() & ITEM_FLAG_TRADE && !pItems->IsFlag(FLAG_ITEM_LENT))
#endif
						{
							// Close message box of shop
							pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							pUIManager->GetMsgBoxNumOnly()->CloseBox();

							pUIManager->SetHoldBtn(m_pIconsShop[m_nSelShopItemID]);
						}
					}
				}
				// Trade items
				else if( m_nSelTradeItemID >= 0 )
				{
					// Close message box of shop
					pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
					pUIManager->GetMsgBoxNumOnly()->CloseBox();

					pUIManager->SetHoldBtn(m_pIconsTrade[m_nSelTradeItemID]);
				}

				bLButtonDownInItem	= FALSE;
				TOOLTIPMGR()->clearTooltip();
			}
			else if (pUIManager->GetDragIcon() == NULL)
			{
				// Shop
				if (IsInsideRect(nX, nY, m_rcTop))
				{
					int	i;
					int	iRowS = m_nCurRow * SHOP_SLOT_COL;							// Start Row
					int	iRowE = (m_nCurRow + SHOP_SLOT_ROW) * SHOP_SLOT_COL;			// End Row
					int	nWhichIdx = -1;

					for( i = iRowS; i < iRowE; i++ )
					{
						if( m_pIconsShop[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							nWhichIdx = i;
						}
					}

					return WMSG_SUCCESS;
				}
				// Trade
				else if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					int	iItem;
					int	nWhichItem = -1;
					for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
					{
						if (m_pIconsTrade[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
							nWhichItem = iItem;	
					}

					return WMSG_SUCCESS;
				}
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcMainTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				// Buy button of shop
				else if( m_bBuyShop && m_btnShopBuy.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Sell button of shop
				else if( !m_bBuyShop && m_btnShopSell.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Cancel Button of shop
				else if( m_btnShopCancel.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;
					m_nSelTradeItemID = -1;

					int	i;
					int	iRowS = m_nCurRow * SHOP_SLOT_COL;
					int	iRowE = (m_nCurRow + SHOP_SLOT_ROW) * SHOP_SLOT_COL;	// 4��~
					
					for( i = iRowS; i < iRowE; i++ )
					{
						if (m_pIconsShop[i]->MouseMessage( pMsg ) != WMSG_FAIL)
						{
							// Update selected item
							m_nSelShopItemID = i;	// ���õ� ������ ������ ID

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_SHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) && !m_bIsLease )
				{
					m_nSelTradeItemID = -1;
					m_nSelShopItemID = -1;

					for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
					{
						if (m_pIconsTrade[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Update selected item
							m_nSelTradeItemID = iItem;			// ���õ� ������ ������ ID

							bLButtonDownInItem	= TRUE;

							pUIManager->RearrangeOrder( UI_SHOP, TRUE );
							return WMSG_SUCCESS;
						}
					}
				}
				// Top Scroll bar
				else if( m_bBuyShop && ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}
				// Bottom Scroll bar
				else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
				}

				pUIManager->RearrangeOrder( UI_SHOP, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONUP:
		{
			CUIManager* pUIManager = CUIManager::getSingleton();
			bLButtonDownInItem = FALSE;

			// If holding button doesn't exist
			if (pUIManager->GetDragIcon() == NULL)
			{
				// Title bar
				bTitleBarClick = FALSE;

				// If shop isn't focused
				if( !IsFocused() )
					return WMSG_FAIL;

				// Close button
				if( ( wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetShop();
						//WSS_SHOP_BUGFIX_70530 --->><<
						CloseShop();
					
					}

					return WMSG_SUCCESS;
				}
				// Buy button of shop
				else if( m_bBuyShop && ( wmsgResult = m_btnShopBuy.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						if(m_bIsLease)
						{
							if(m_nSelShopItemID >= 0)
							{								
								CTString	strMessage;
								CUIMsgBox_Info	MsgBoxInfo;
								if(m_bIsEvent)
								{
									int iItemidx = m_pIconsShop[m_nSelShopItemID]->getIndex();
									CItemData* pID = _pNetwork->GetItemData(iItemidx);

									if( pID->GetJob()&(1<<_pNetwork->MyCharacterInfo.job))
									{
										MsgBoxInfo.SetMsgBoxInfo( _S(169,"�̺�Ʈ"),UMBS_YESNO,UI_SHOP,MSGLCMD_EVENT_MAY);
										strMessage.PrintF(_S(3403, "%s �������� �����Ͽ����ϴ�."),pID->GetName());

									}else
									{
										MsgBoxInfo.SetMsgBoxInfo( _S(169,"�̺�Ʈ"),UMBS_NO,UI_SHOP,MSGLCMD_EVENT_MAY);
										strMessage = _S(3404, "�����Ͻ� ���� �ڽſ� �´� ��� �ƴմϴ�. Ȯ���ϰ� �ٽ� �õ��Ͽ� �ֽʽÿ�");
									}
									

								}else
								{
									if (pUIManager->DoesMessageBoxExist(MSGCMD_SHOP_LEASE_ITEM))
										pUIManager->CloseMessageBox(MSGCMD_SHOP_LEASE_ITEM);

									MsgBoxInfo.SetMsgBoxInfo( _S( 263, "����" ), UMBS_YESNO,
															UI_SHOP, MSGCMD_SHOP_LEASE_ITEM );
									strMessage = _S(3080, "�뿩�� ����� �뿩�� ���κ��� 1�ϰ��� �뿩�Ⱓ�� ����Ǹ� �ڵ����� �Ҹ�˴ϴ�.")
										+ _S(3081,"�����Ͻ� ���⸦ �뿩�Ͻðڽ��ϱ�?");
								}
								
								MsgBoxInfo.AddString( strMessage );
								pUIManager->CreateMessageBox( MsgBoxInfo );	
							}
							return WMSG_SUCCESS;							
						}
						// NOTE : �ŷ� ���Կ� �ϳ��� �������� ������ ������ �ϵ��� �ϰ�,
						// NOTE : ������ �޼��� ��ü�� �Ⱥ������� ��...
						for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
						{
							if (!m_pIconsTrade[i]->IsEmpty())
							{
								// Send Network Message
								BuyItems();
								
								// Clear Member Value
								ResetShop();

								//WSS_SHOP_BUGFIX_70530 --->><<
								CloseShop();
								
							}
						}
						return WMSG_SUCCESS;
					}

					return WMSG_SUCCESS;
				}
				// Sell button of shop
				else if( !m_bBuyShop && ( wmsgResult = m_btnShopSell.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						// NOTE : �ŷ� ���Կ� �ϳ��� �������� ������ ������ �ϵ��� �ϰ�,
						// NOTE : ������ �޼��� ��ü�� �Ⱥ������� ��...
						for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i )
						{
							if (!m_pIconsTrade[i]->IsEmpty())
							{
								// Send Network Message
								SellItems();
								
								// Clear Member Value
								ResetShop();

								//WSS_SHOP_BUGFIX_70530 --->><<
								CloseShop();

								return WMSG_SUCCESS;
							}
						}
					}

					return WMSG_SUCCESS;
				}
				// Cancel button of shop
				else if( ( wmsgResult = m_btnShopCancel.MouseMessage( pMsg ) ) != WMSG_FAIL )
				{
					if( wmsgResult == WMSG_COMMAND )
					{
						ResetShop();
						//WSS_SHOP_BUGFIX_70530 --->><<
						CloseShop();
					}

					return WMSG_SUCCESS;
				}
				// Event button of change weapon
				else if(m_bIsEvent && m_btnEvent_weapon.MouseMessage(pMsg) != WMSG_FAIL)
				{
					PrepareLeaseShop(m_iEventjob, CItemData::ITEM_WEAPON);
					return WMSG_SUCCESS;
				}
				// Event button of change shild
				else if(m_bIsEvent && m_btnEvent_shield.MouseMessage(pMsg) != WMSG_FAIL)
				{
					PrepareLeaseShop( m_iEventjob, CItemData::ITEM_SHIELD);
					return WMSG_SUCCESS;
				}
				// Top Scroll bar
				else if( m_bBuyShop && m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Bottom Scroll bar
				else if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
					return WMSG_SUCCESS;
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					int	i;
					int	iRowS = m_nCurRow * SHOP_SLOT_COL;
					int	iRowE = (m_nCurRow + SHOP_SLOT_ROW) * SHOP_SLOT_COL;
					for (i = iRowS; i < iRowE; ++i)
					{
						if( m_pIconsShop[i]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
				// Trade items
				else if( IsInsideRect( nX, nY, m_rcBottom ) &&!m_bIsLease)
				{
					for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
					{
						if( m_pIconsTrade[iItem]->MouseMessage( pMsg ) != WMSG_FAIL )
						{
							// Nothing
							return WMSG_SUCCESS;
						}
					}
				}
			}		
			else
			{
				//////////////////////////////////////////////////////////////////////////
				if( IsInside( nX, nY ) )
				{
					CUIIcon* pDrag = pUIManager->GetDragIcon();

					if (pDrag == NULL)
						return WMSG_FAIL;

					CItems* pItems = pDrag->getItems();

					if (pItems == NULL)
						return WMSG_FAIL;

					// If holding button is item and is from shop
					if (pDrag->getBtnType() == UBET_ITEM &&
						pDrag->GetWhichUI() == UI_SHOP)
					{
						// Trade items
						if( IsInsideRect( nX, nY, m_rcBottom ) )
						{
							// If this item is moved from shop slot
							if (m_nSelTradeItemID < 0 ||
								m_pIconsTrade[m_nSelTradeItemID]->getItems()->Item_UniIndex != pItems->Item_UniIndex)
							{
								int		nTab = 0;

								if (pItems->Item_Tab > INVENTORY_TAB_NORMAL)
									nTab += ITEM_COUNT_IN_INVENTORY_NORMAL;
								if (pItems->Item_Tab > INVENTORY_TAB_CASH_1)
									nTab += ITEM_COUNT_IN_INVENTORY_CASH_1;

								AddShopItem(pItems->InvenIndex + nTab, pItems->Item_UniIndex, pItems->Item_Sum);

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
						// Shop items
						else if( IsInsideRect( nX, nY, m_rcTop ) )
						{
							// If this item is moved from trade slot
							if (m_nSelShopItemID < 0 ||
								m_pIconsShop[m_nSelShopItemID]->getItems()->Item_UniIndex != pItems->Item_UniIndex)
							{
								int nInvenIdx = pItems->InvenIndex;

								if (pItems->Item_Tab > INVENTORY_TAB_NORMAL)
								{
									nInvenIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
								}

								if (pItems->Item_Tab > INVENTORY_TAB_CASH_1)
								{
									nInvenIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
								}

								DelShopItem(nInvenIdx, pItems->Item_UniIndex, 
									pItems->Item_Sum, m_nSelTradeItemID );

								// Reset holding button
								pUIManager->ResetHoldBtn();

								return WMSG_SUCCESS;
							}
						}
					} // If - If holding button is item

					// Reset holding button
					pUIManager->ResetHoldBtn();

					return WMSG_SUCCESS;
				} // If - IsInside
				//////////////////////////////////////////////////////////////////////////
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if(m_bIsLease) 
				return WMSG_SUCCESS;

			if( IsInside( nX, nY ) )
			{
				CUIManager* pUIManager = CUIManager::getSingleton();

				// Trade items
				if( IsInsideRect( nX, nY, m_rcBottom ) )
				{
					int	nTradeItemID = m_nSelTradeItemID;
					m_nSelTradeItemID = -1;

					for( int iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
					{
						if (m_pIconsTrade[iItem]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							// Close message box of shop
							pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
							pUIManager->GetMsgBoxNumOnly()->CloseBox();

							CItems* pItems = m_pIconsTrade[iItem]->getItems();

							if (pItems != NULL)
							{
								int nInvenIdx = pItems->InvenIndex;

								if (pItems->Item_Tab > INVENTORY_TAB_NORMAL)
								{
									nInvenIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;
								}

								if (pItems->Item_Tab > INVENTORY_TAB_CASH_1)
								{
									nInvenIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
								}

								DelShopItem( nInvenIdx, pItems->Item_UniIndex,
									pItems->Item_Sum, iItem );
							}
							
							return WMSG_SUCCESS;
						}
					}
				}
				// Shop items
				else if( IsInsideRect( nX, nY, m_rcTop ) )
				{
					m_nSelShopItemID = -1;

					int	i;
					int	iRowS = m_nCurRow * SHOP_SLOT_COL;
					int	iRowE = (m_nCurRow + SHOP_SLOT_ROW) * SHOP_SLOT_COL;
					
					for( i = iRowS; i < iRowE; i++ )
					{
						if (m_pIconsShop[i]->MouseMessage(pMsg) != WMSG_FAIL)
						{
							if( m_bBuyShop )
							{
								// Close message box of shop
								pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
								pUIManager->GetMsgBoxNumOnly()->CloseBox();

								if (m_pIconsShop[i]->getItems() == NULL)
									return WMSG_FAIL;

								AddShopItem(i, m_pIconsShop[i]->getItems()->Item_UniIndex,
									m_pIconsShop[i]->getItems()->Item_Sum);

							}
							else
							{
								int		t, idx;

								if (i >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
								{
									t = INVENTORY_TAB_CASH_2;
									idx = i - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
								}
								else if (i >= ITEM_COUNT_IN_INVENTORY_NORMAL)
								{
									t = INVENTORY_TAB_CASH_1;
									idx = i - ITEM_COUNT_IN_INVENTORY_NORMAL;
								}
								else
								{
									t = INVENTORY_TAB_NORMAL;
									idx = i;
								}

								// Not wearing, not refine stone, can trade
								CItems		*pItems = &_pNetwork->MySlotItem[t][idx];
								CItemData	*pItemData = pItems->ItemData;

								if (pItemData == NULL)
									continue;
#ifdef ADD_SUBJOB
								if( pItemData->IsFlag( ITEM_FLAG_SELLER ) ? 
									pUIManager->CheckSellerItem(UI_SHOP, pItemData->GetFlag()) : 
									pItems->Item_Wearing == -1 && ( pItemData->GetType() != CItemData::ITEM_ETC ||
									pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
									pItemData->GetFlag() & ITEM_FLAG_TRADE && !pItems->IsFlag(FLAG_ITEM_LENT) )
#else
								if( pItems->Item_Wearing == -1 &&
									( pItemData->GetType() != CItemData::ITEM_ETC || pItemData->GetSubType() != CItemData::ITEM_ETC_REFINE ) &&
									pItemData->GetFlag() & ITEM_FLAG_TRADE && !pItems->IsFlag(FLAG_ITEM_LENT))
#endif
								{
									// Close message box of shop
									pUIManager->CloseMessageBox( MSGCMD_SHOP_ADD_PLUSITEM );
									pUIManager->GetMsgBoxNumOnly()->CloseBox();

									if (m_pIconsShop[i]->getItems()->GetToggle() == true)
										return WMSG_FAIL;

									AddShopItem(i, m_pIconsShop[i]->getItems()->Item_UniIndex,
										m_pIconsShop[i]->getItems()->Item_Sum);
								}												
							}

							return WMSG_SUCCESS;
						}
					}
				}
				// Top Scroll bar
				else if( m_bBuyShop && ( wmsgResult = m_sbTopScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbTopScrollBar.GetScrollPos();
				}
				// Bottom Scroll bar
				else if( !m_bBuyShop && ( wmsgResult = m_sbBottomScrollBar.MouseMessage( pMsg ) ) != WMSG_SUCCESS )
				{
					if( wmsgResult == WMSG_COMMAND)
						m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
				}

				return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				// Top Scroll bar
				if( m_bBuyShop && m_sbTopScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbTopScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}
				// Bottom Scroll bar
				else if( !m_bBuyShop && m_sbBottomScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_nCurRow = m_sbBottomScrollBar.GetScrollPos();
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}

	return WMSG_FAIL;
}


// ========================================================================= //
//                             Command functions                             //
// ========================================================================= //
static int		nTempIndex;
static int		nTempUniIndex;
static int		nTempTab;
static int		nTempIdx;
static SQUAD	llTempCount;
static int		nTempTradeItemID;

// ----------------------------------------------------------------------------
// Name : AddShopItem()
// Desc : From shop to trade
// ----------------------------------------------------------------------------
void CUIShop::AddShopItem( int invenIdx, int nUniIndex, SQUAD llCount )
{
	if (m_bBuyShop == FALSE)
	{
		if (invenIdx >= (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1))
		{
			nTempTab		= INVENTORY_TAB_CASH_2;
			nTempIdx		= invenIdx - (ITEM_COUNT_IN_INVENTORY_NORMAL + ITEM_COUNT_IN_INVENTORY_CASH_1);
		}
		else if (invenIdx >= ITEM_COUNT_IN_INVENTORY_NORMAL)
		{
			nTempTab		= INVENTORY_TAB_CASH_1;
			nTempIdx		= invenIdx - ITEM_COUNT_IN_INVENTORY_NORMAL;
		}
		else
		{
			nTempTab		= INVENTORY_TAB_NORMAL;
			nTempIdx		= invenIdx;
		}
	}
	else
	{
		nTempTab = 0;
		nTempIdx = invenIdx;
	}

	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	CItems*	pItems	= m_pIconsShop[invenIdx]->getItems();
	nTempIndex		= pItems->Item_Index;
	ULONG ulItemPlus= pItems->Item_Plus;

	CItemData*	pItemData = pItems->ItemData;
	const char* szItemName = pItemData->GetName();

	CUIManager* pUIManager = CUIManager::getSingleton();

	// Ask quantity
	if( ( m_bBuyShop && ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) ) ||
		( !m_bBuyShop && ( pItemData->GetFlag() & ITEM_FLAG_COUNT ) && llTempCount > 1 ) )
	{
		CTString	strMessage;
		strMessage.PrintF( _S2( 150, szItemName, "�� ���� %s<��> �ű�ðڽ��ϱ�?" ), szItemName );

		CmdShopAddItem* pCmd = new CmdShopAddItem;
		pCmd->setData(this);

		int nMax = 0;

		if( !m_bBuyShop )
			nMax = pItems->Item_Sum;
		else
			nMax = pItemData->GetStack();

		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "����" ), strMessage, 1, nMax);
	}
	else if( ulItemPlus > 0 && !(pItemData->GetType() == CItemData::ITEM_ETC && (pItemData->GetSubType() == CItemData::ITEM_ETC_MONSTER_MERCENARY_CARD || nTempIndex == 6941) ) )
	{
		CTString	strMessage;
		CUIMsgBox_Info	MsgBoxInfo;
		MsgBoxInfo.SetMsgBoxInfo( _S( 263, "����" ), UMBS_YESNO,
									UI_SHOP, MSGCMD_SHOP_ADD_PLUSITEM );
		//const char	*szItemName = pItemData->GetName();
		strMessage.PrintF( _S2( 264, szItemName, "[%s +%d]<��> �ű�ðڽ��ϱ�?" ), szItemName, ulItemPlus );
		MsgBoxInfo.AddString( strMessage );
		pUIManager->CreateMessageBox( MsgBoxInfo );
	}
	else
	{
		ShopToTrade( llTempCount );
	}
}

// ----------------------------------------------------------------------------
// Name : DelShopItem()
// Desc : From trade to shop
// ----------------------------------------------------------------------------
void CUIShop::DelShopItem( int invenIdx, int nUniIndex, SQUAD llCount, int nTradeItemID )
{
	if (nTradeItemID < 0)
		return;

	nTempIdx		= invenIdx;
	nTempUniIndex	= nUniIndex;
	llTempCount		= llCount;
	nTempTradeItemID= nTradeItemID;
	nTempIndex		= m_pIconsTrade[nTempTradeItemID]->getIndex();

	CItemData* pItemData = _pNetwork->GetItemData( nTempIndex );
	
	// Ask quantity
	if( llTempCount > 1 )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();

		CTString	strMessage;
		const char	*szItemName = _pNetwork->GetItemName( nTempIndex );
		strMessage.PrintF( _S2( 150, szItemName, "�� ���� %s<��> �ű�ðڽ��ϱ�?" ), szItemName );

		CmdShopDelItem* pCmd = new CmdShopDelItem;
		pCmd->setData(this);
		UIMGR()->GetMsgBoxNumOnly()->SetInfo(pCmd, _S( 263, "����" ), strMessage, 1, llTempCount);
	}
	else
	{
		TradeToShop( llTempCount );
	}
}

// ----------------------------------------------------------------------------
// Name : BuyItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::BuyItems()
{
	// Buy nothing
	if( m_nSendTotalPrice < 0 )
		return;

	// Not enough money
	if(_pNetwork->MyCharacterInfo.money < m_nTotalPrice )
	{
		// Add system message
		CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 266, "�������� �����մϴ�." ), SYSMSG_ERROR );
		return;
	}

	if(m_bIsLease)
	{
		if(m_bIsEvent)
		{
			int iItemidx = m_pIconsShop[m_nSelShopItemID]->getIndex();
			CItemData* pID = _pNetwork->GetItemData(iItemidx);
			_pNetwork->SendEventItem(iItemidx,(int)((pID->GetLevel()/2.0f)+0.5));
		}else
		{
			_pNetwork->SendLeaseItem( m_pIconsShop[m_nSelShopItemID]->getIndex());
		}			
	}
	else
	{
		if(m_bIsFieldShop)
		{
			_pNetwork->FieldShopBuyItem(m_nNumOfItem, m_nSendTotalPrice);
			m_bIsFieldShop = FALSE;
		}else
		{
			_pNetwork->BuyItem( m_nSelectedShopID, m_nNumOfItem, m_nSendTotalPrice );
		}
		
	}
}

// ----------------------------------------------------------------------------
// Name : SellItems()
// Desc :
// ----------------------------------------------------------------------------
void CUIShop::SellItems()
{
	// Sell nothing
	if( m_nSendTotalPrice < 0 )
		return;

	if (m_nNumOfItem <= 0)
		return;

	if(m_bIsFieldShop)
	{
		_pNetwork->FieldShopSellItem(m_nNumOfItem, m_nSendTotalPrice);
		m_bIsFieldShop = FALSE;
	}else
	{
		_pNetwork->SellItem( m_nSelectedShopID, m_nNumOfItem, m_nSendTotalPrice );
	}
		
}

// ----------------------------------------------------------------------------
// Name : MsgBoxLCommand()
// Desc :
//		cpp2angel edit (04.12.18)
// ----------------------------------------------------------------------------
void CUIShop::MsgBoxLCommand( int nCommandCode, int nResult )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	switch( nCommandCode )
	{
	case MSGLCMD_SHOP_REQ:
		{
			// [090527: selo] Ȯ���� ����Ʈ �̾߱� �Ѵ� ó�� ������ ���� ��ƾ
			int iQuestIndex = -1;
			if( ciQuestClassifier < nResult )	
			{
				iQuestIndex = nResult;
				nResult = ciQuestClassifier;
			}

			switch ( nResult ) 
			{
			case SHOP_BUY : //  Buy
				{
					PrepareBuyShop();
				}
				break;

			case SHOP_SELL : // Sell
				{	
					PrepareSellShop();
				}
				break;
			case SHOP_GW_MIX: // ���� ����
				{
					pUIManager->GetGWMix()->OpenGWMix();
				}
				break;
			case SHOP_TALK :// Quest ...
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );

					//TODO : NewQuestSystem
					// Open quest
					CUIQuestBook::TalkWithNPC();
				}
				break;
				// [090527: selo] Ȯ���� ����Ʈ ����
			case ciQuestClassifier:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );

					// ������ ����Ʈ�� ���� ���� �Ǵ� ���� â�� ����.
					CUIQuestBook::SelectQuestFromMessageBox( iQuestIndex );					
				}
				break;
			case SHOP_SUPERGOJE:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );

					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(  CTString(""), UMBS_YESNO, UI_SHOP, MSGCMD_SUPERGOJE_REQUEST);
					MsgBoxInfo.AddString( _S( 1739, "�ʰ�� ���ü��� ���� �ްڽ��ϱ�?" ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				} break;

			case LEASE_WEAPON_TITAN:
			case LEASE_WEAPON_KNIGHT:
			case LEASE_WEAPON_HEALER:
			case LEASE_WEAPON_MAGE:
			case LEASE_WEAPON_ROGUE:
			case LEASE_WEAPON_SOCERER:
			case LEASE_WEAPON_EXROGUE:	
			case LEASE_WEAPON_EXMAGE:	//2013/01/21 jeil EX������ �߰� 
				{
					int tv_job = nResult - LEASE_WEAPON_TITAN;
					PrepareLeaseShop( tv_job );
				}
				break;
			case SHOP_NPC_HELP:
				{
					pUIManager->RearrangeOrder( UI_NPCHELP, TRUE );
				}
				break;
			case COMBO_MAKE:
				{
					//�޺� �������� �̵��� CSF_SHOP �÷��׸� �����Ѵ�
					pUIManager->SetCSFlagOff( CSF_SHOP );
					
					pUIManager->GetCombo()->OpenMonsterCombo(TRUE, m_fNpcX, m_fNpcZ, m_nNpcVirIdx);
				}
				break;
			
			case COMBO_MOVE_STAGE:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );

					pUIManager->GetCombo()->SendComboMessage( MSG_EX_MONSTERCOMBO_GOTO_STAGE);
				}
				break;
			case COMBO_MOVE_GO_LOBBY:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );

					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GOTO_WAITROOM);

				}
				break;
			case COMBO_RECALL_LOBBY:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );

					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_RECALL_TO_COMBO);
				}
				break;
			case COMBO_RECALL_STAGE:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );

					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_RECALL_TO_STAGE);
				}
				break;
			case COMBO_GIVEUP:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );

					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo(  _S(4047,"���� �޺�"), UMBS_YESNO, UI_SHOP, MSGCMD_COMBO_GIVEUP);
					MsgBoxInfo.AddString( _S(4048, "���ź���� �̵��Ǹ� �ٽ� �����ϱ� ���ؼ��� ����Ḧ �����ؾ� �մϴ�. \n �׷��� ���� �޺��� �����Ͻðڽ��ϱ�?" ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );

				//	pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP);
				}
				break;
			case COMBO_START:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );

					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_START);
				}
				break;
			case COMBO_COMPLETE:
				{
					pUIManager->SetCSFlagOff( CSF_SHOP );

					pUIManager->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP, FALSE, m_nNpcVirIdx);
				}
				break;
			///////////////////////////////////////////////////////////////////////////////////
			// Cube
			case CUBE_PARTY_ENTRANCE:
				{
					pUIManager->CreateMessageBoxL( _S(4327, "ť�����弱��" ) , UI_SHOP, MSGLCMD_CUBE_PARTY_ENTRANCE);

					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4328, "ť���������� �����Ͽ� �ֽʽÿ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, CTString(" "),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4329, "���ǻ���"),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4330, "��ü����� ��Ƽ�� ��ΰ� �����̵��˴ϴ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4331, "��������� ������ �¾ƾ� ������ �����մϴ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_PARTY_ENTRANCE,TRUE, _S(4332, "��ü������ ��õ�帳�ϴ�."),-1,0xa3a1a3ff);
					
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(4333, "��ü����" ), CUBE_PARTY_GROUP_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(4334, "��������" ), CUBE_PARTY_PERSON_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_PARTY_ENTRANCE, FALSE, _S(139, "���" ) );
				}
				break;
			case CUBE_GUILD_ENTRANCE:
				{
					pUIManager->CreateMessageBoxL( _S(4327, "ť�����弱��" ) , UI_SHOP, MSGLCMD_CUBE_GUILD_ENTRANCE);

					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4328, "ť���������� �����Ͽ� �ֽʽÿ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, CTString(" "),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4329, "���ǻ���"),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4335, "�������� ���� ��ΰ� �����̵��˴ϴ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4331, "��������� ������ �¾ƾ� ������ �����մϴ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_CUBE_GUILD_ENTRANCE,TRUE, _S(4332, "��ü������ ��õ�帳�ϴ�."),-1,0xa3a1a3ff);
					
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(4336, "�������" ), CUBE_GUILD_GROUP_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(4334, "��������" ), CUBE_GUILD_PERSON_ENTRANCE);
					pUIManager->AddMessageBoxLString( MSGLCMD_CUBE_GUILD_ENTRANCE, FALSE, _S(139, "���" ) );
				}
				break;
			case CUBE_PLAYER_STATE:
			case CUBE_GUILD_STATE:
				{
					CNetworkMessage nmMessage(MSG_EXTEND);
			
					nmMessage << (LONG)MSG_EX_EXTREME_CUBE;

					if (nResult == CUBE_GUILD_STATE)
						nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_STATE_REQ; // ��� ť�� ��Ȳ
					else
						nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_STATE_PERSONAL_REQ; // ���� ť�� ��Ȳ

					_pNetwork->SendToServerNew(nmMessage);
				}
				break;
			case CUBE_PLAYER_REWARD:
				{
					CNetworkMessage nmMessage(MSG_EXTEND);
					
					nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
					nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_REWARD_PERSONAL_REQ;

					_pNetwork->SendToServerNew(nmMessage);
				}break;
			// Cube
			///////////////////////////////////////////////////////////////////////////////////	
				
			//////////////////////////////////////////////////////////////////////////
			// ($E_WC2010) [100511: selo] 2010 ������ �̺�Ʈ
			case EVENT_WORLDCUP_2010_INFO:
				{
					if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
						pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->CreateMessageBoxL( _S(4889, "���� ���� �̺�Ʈ �ȳ�" ) , UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4892, "������� ���ÿ�!! ���� �౸���� �����´ٸ� ��𿡼��� �� �� ���� Ȳ�� �౸���� �帮����."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4893, "�౸���� ���� �ۿ� �ִ� 'Ʈ���̾ޱ� ��'���� ������ �ּ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4894, "�丮���� �� ���� �ٴϱ� �ص� ��� ������ ���� ������ ó���� �� �����Կ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 191, "Ȯ��" ) );
				}
				break;
			case EVENT_WORLDCUP_2010_BALL:
				{
					if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
						pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->CreateMessageBoxL( _S(4890, "Ȳ�� �౸������ ��ȯ"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4895, "�౸�� 10���� ������ ���� Ȳ�� �౸�� 1���� �ٲ��ְڼ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 149, "��ȯ" ), EVENT_WORLDCUP_2010_BALL_GIFT );

				}
				break;
			case EVENT_WORLDCUP_2010_GOLDENBALL:
				{
					if( pUIManager->DoesMessageBoxLExist(MSGLCMD_EVENT_WORLDCUP_EVENT1) )
						pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->SetCSFlagOff( CSF_SHOP );
					pUIManager->CreateMessageBoxL( _S(4881, "���� ���� ���ڷ� ��ȯ"), UI_SHOP, MSGLCMD_EVENT_WORLDCUP_EVENT1);
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4896, "Ȳ�� �౸�� 50�� ������ ������ �´ٸ�, ����� ���ϴ� ���� ���� ���ڸ� �帮�ڼ�."),-1,0xa3a1a3ff);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, TRUE, _S(4897, "���� ���� ���̴� �й��Ͻÿ�. ����... ����Ͻÿ�."),-1,0xa3a1a3ff);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_WORLDCUP_EVENT1, FALSE, _S( 149, "��ȯ" ), EVENT_WORLDCUP_2010_GOLDENBALL_GIFT );
				}
				break;
			// 2010 ������ �̺�Ʈ
			//////////////////////////////////////////////////////////////////////////


			default:
				{
					// Character state flags
					pUIManager->SetCSFlagOff( CSF_SHOP );
				}
				break;
		
			}
		}  // case MSGLCMD_SHOP_REQ
		break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSGLCMD_CUBE_PARTY_ENTRANCE:
		{
			switch ( nResult )
			{
			case CUBE_PARTY_GROUP_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_PARTY_GROUP_ENTRANCE);
					MsgBoxInfo.AddString( _S(4339, "��ü������ ���ؼ� ť�꿡 �����Ͻðڽ��ϱ�? ��Ƽť�갡 ������ ������ �� �����ϴ�.") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case CUBE_PARTY_PERSON_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_PARTY_PERSON_ENTRANCE);
					MsgBoxInfo.AddString( _S(4338, "���������� ���ؼ� ť�꿡 �����Ͻðڽ��ϱ�?") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	case MSGLCMD_CUBE_GUILD_ENTRANCE:
		{
		switch( nResult )
			{
			case CUBE_GUILD_GROUP_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_GUILD_GROUP_ENTRANCE);
					MsgBoxInfo.AddString( _S(4337, "��������� ���ؼ� ť�꿡 �����Ͻðڽ��ϱ�? ���ť�갡 ������ ������ �� �����ϴ�.") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case CUBE_GUILD_PERSON_ENTRANCE:
				{
					CUIMsgBox_Info	MsgBoxInfo;
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OKCANCEL, UI_SHOP, MSGCMD_CUBE_GUILD_PERSON_ENTRANCE);
					MsgBoxInfo.AddString( _S(4338, "���������� ���ؼ� ť�꿡 �����Ͻðڽ��ϱ�?") );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;
	// Cube
	///////////////////////////////////////////////////////////////////////////////////	

	//////////////////////////////////////////////////////////////////////////
	// ($E_WC2010) [100511: selo] 2010 ������ �̺�Ʈ
	case MSGLCMD_EVENT_WORLDCUP_EVENT1:
		{
			switch( nResult )
			{
			case EVENT_WORLDCUP_2010_BALL_GIFT:
				{
					_pNetwork->SendWorldCup2010_Event(MSG_EVENT_WORLDCUP2010_TRADE_REQ, 1);
				}
				break;
			case EVENT_WORLDCUP_2010_GOLDENBALL_GIFT:
				{
					_pNetwork->SendWorldCup2010_Event(MSG_EVENT_WORLDCUP2010_TRADE_REQ, 2);
				}
				break;
			}
		}
		break;
	// 2010 ������ �̺�Ʈ
	//////////////////////////////////////////////////////////////////////////
	}// switch( nCommandCode )
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : nCommandCode - 
//			bOK - 
//			&strInput - 
//-----------------------------------------------------------------------------
void CUIShop::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	if( !bOK )
		return;

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			ShopToTrade( 1 );
		}
		break;

	case MSGCMD_SUPERGOJE_REQUEST:
		{
			_pNetwork->SendSuperStoneReq();			
		} break;
	case MSGCMD_SUPERGOJE_NOTIFY:
		{
		} break;

	case MSGCMD_SHOP_LEASE_ITEM :
	case MSGLCMD_EVENT_MAY:
		{
			// Send Network Message
			BuyItems();
			// Clear Member Value
			ResetShop();
			//WSS_SHOP_BUGFIX_70530 --->><<
			CloseShop();
		}
		break;
	case MSGCMD_COMBO_GIVEUP:
		{
			CUIManager::getSingleton()->GetCombo()->SendComboMessage(MSG_EX_MONSTERCOMBO_GIVEUP, FALSE, m_nNpcVirIdx);
		}
		break;

	///////////////////////////////////////////////////////////////////////////////////	
	// Cube
	case MSGCMD_CUBE_PARTY_GROUP_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_PARTY;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_PARTY_PERSON_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_PARTY_PERSON;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_GUILD_GROUP_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);

			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_GUILD;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_GUILD_PERSON_ENTRANCE:
		{
			UIMGR()->SetCSFlagOn(CSF_TELEPORT);

			CNetworkMessage nmMessage(MSG_EXTEND);
			
			nmMessage << (LONG)MSG_EX_EXTREME_CUBE;
			nmMessage << (UBYTE)MSG_EX_EXTREME_CUBE_GUILD_PERSON;

			_pNetwork->SendToServerNew(nmMessage);
		}
		break;
	case MSGCMD_CUBE_STATE:
		{
			CUIManager::getSingleton()->GetMessageBox(MSGCMD_CUBE_STATE)->InitMessageBox();
		}
		break;
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIShop::ShopToTrade( SQUAD llCount )
{
	if( m_bBuyShop )
	{
		CItemData	*pItemData = _pNetwork->GetItemData( nTempIndex );

		MyInfo* pInfo = MY_INFO();
		__int64 llitemPrice = CalculateItemPrice( m_nSelectedShopID, *pItemData, llCount, m_bBuyShop );
		__int64 iAddPrice = pInfo->CalcPriceRate(llitemPrice, m_bBuyShop) + pInfo->CalcPriceAddition(m_bBuyShop);

		SQUAD	llPrice = m_nTotalPrice + llitemPrice + iAddPrice;

		// Not enough money
		if(_pNetwork->MyCharacterInfo.money < llPrice )
		{
			// Add system message
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 266, "�������� �����մϴ�." ), SYSMSG_ERROR );
			return;
		}

		const int iFame = pItemData->GetFame();

		// Not enough fame
		if( iFame > 0 && iFame > _pNetwork->MyCharacterInfo.fame )
		{
			// Add system message
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 1217, "��ġ�� �����մϴ�." ), SYSMSG_ERROR );		
			return;
		}
	}

	// Find same item in trade slot
	int iItem;

	if (m_bBuyShop)
	{
		for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsTrade[iItem]->getIndex() == nTempIndex)
			{
				CItemData	*pItemData = _pNetwork->GetItemData( nTempIndex );

				if (m_pIconsTrade[iItem]->getItems()->Item_Sum + llCount <= pItemData->GetStack())
					break;
			}
		}
	}
	else
	{
		for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
		{
			if (m_pIconsTrade[iItem]->getItems() != NULL &&  
				m_pIconsTrade[iItem]->getItems()->Item_UniIndex == nTempUniIndex)
			{
				CItemData	*pItemData = _pNetwork->GetItemData( nTempIndex );

				if (m_pIconsTrade[iItem]->getItems()->Item_Sum + llCount <= pItemData->GetStack())
					break;
			}
		}
	}

	// If there is same item
	if( iItem < SHOP_TRADE_SLOT_TOTAL )
	{
		// Check if item is countable
		CItemData*	pItemData = _pNetwork->GetItemData( nTempIndex );
		if( pItemData->GetFlag() & ITEM_FLAG_COUNT )
		{
			// Update count of trade item
			SQUAD	llNewCount = m_pIconsTrade[iItem]->getItems()->Item_Sum;
			llNewCount += llCount;
			m_pIconsTrade[iItem]->setCount(llNewCount);
												
			if( !m_bBuyShop )
			{
				if (nTempTab > INVENTORY_TAB_NORMAL)
					nTempIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;

				if (nTempTab > INVENTORY_TAB_CASH_1)
					nTempIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;

				// Update count of shop item
				llNewCount = m_pIconsShop[nTempIdx]->getItems()->Item_Sum;
				llNewCount -= llCount;

				m_pIconsShop[nTempIdx]->setCount(llNewCount);

				if( llNewCount <= 0 )
				{
					m_pIconsShop[nTempIdx]->Hide(TRUE);

					// Unselect item
					if( m_nSelShopItemID == nTempIdx )
						m_nSelShopItemID = -1;
				}
			}

			m_nTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop );
			m_nSendTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop, FALSE );
			m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
			CUIManager::getSingleton()->InsertCommaToString( m_strTotalPrice );
			return;
		}
	}

	// Find empty slot
	for( iItem = 0; iItem < SHOP_TRADE_SLOT_TOTAL; iItem++ )
	{
		if (m_pIconsTrade[iItem]->IsEmpty())
			break;
	}

	// If there is not empty slot
	if( iItem == SHOP_TRADE_SLOT_TOTAL )
	{
		// Add system message
		if( m_bBuyShop )
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 267, "�ִ� 10���� �������� ������ �� �ֽ��ϴ�." ), SYSMSG_ERROR );
		else
			CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( _S( 268, "�ִ� 10���� �������� �Ǹ��� �� �ֽ��ϴ�." ), SYSMSG_ERROR );

		return;
	}

	if (m_bBuyShop == FALSE)
	{
		if (nTempTab > INVENTORY_TAB_NORMAL)
			nTempIdx += ITEM_COUNT_IN_INVENTORY_NORMAL;

		if (nTempTab >= INVENTORY_TAB_CASH_2)
			nTempIdx += ITEM_COUNT_IN_INVENTORY_CASH_1;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();	

	CItems* pCopy = new CItems;
	memcpy(pCopy, m_pIconsShop[nTempIdx]->getItems(), sizeof(CItems));
	m_vecTrade.push_back(pCopy);
	
	m_pIconsTrade[iItem]->setData(m_vecTrade[iItem]);	
	m_pIconsTrade[iItem]->setCount(llCount);	

	m_nTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop );
	m_nSendTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop, FALSE );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	pUIManager->InsertCommaToString( m_strTotalPrice );

	if (m_bBuyShop == FALSE)
	{
		// Update count of shop item
		SQUAD	llNewCount = m_pIconsShop[nTempIdx]->getItems()->Item_Sum;
		llNewCount -= llCount;

		m_pIconsShop[nTempIdx]->setCount(llNewCount);

		if( llNewCount <= 0 )
		{
			m_pIconsShop[nTempIdx]->Hide(TRUE);

			// Unselect item
			if( m_nSelShopItemID == nTempIdx )
				m_nSelShopItemID = -1;
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  :
//-----------------------------------------------------------------------------
void CUIShop::TradeToShop( SQUAD llCount )
{
	// Update count of trade item
	SQUAD	llNewCount = m_pIconsTrade[nTempTradeItemID]->getItems()->Item_Sum;
	llNewCount -= llCount;
	m_pIconsTrade[nTempTradeItemID]->setCount(llNewCount);

	if( llNewCount <= 0 )
	{
		// Rearrange buttons
		for( int iArrItem = nTempTradeItemID; iArrItem < SHOP_TRADE_SLOT_TOTAL; iArrItem++ )
		{
			if ((iArrItem + 1) == SHOP_TRADE_SLOT_TOTAL)
			{
				m_pIconsTrade[iArrItem]->clearIconData();
				break;
			}

			if (m_pIconsTrade[iArrItem + 1]->IsEmpty())
			{
				m_pIconsTrade[iArrItem]->clearIconData();
				break;
			}

			m_pIconsTrade[iArrItem]->setData(m_vecTrade[iArrItem + 1]);
		}

		SAFE_DELETE(m_vecTrade[nTempTradeItemID]);
		m_vecTrade.erase(m_vecTrade.begin() + nTempTradeItemID);

		// Unselect item
		if( nTempTradeItemID == m_nSelTradeItemID )
			m_nSelTradeItemID = -1;
	}

	if( !m_bBuyShop )
	{
		m_pIconsShop[nTempIdx]->Hide(FALSE);
		// Update count of shop item
		llNewCount = m_pIconsShop[nTempIdx]->getItems()->Item_Sum;
		llNewCount += llCount;
		m_pIconsShop[nTempIdx]->setCount(llNewCount);
	}

	m_nTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop );
	m_nSendTotalPrice = CalculateTotalPrice( m_nSelectedShopID, m_nNumOfItem, m_bBuyShop, FALSE );
	m_strTotalPrice.PrintF( "%I64d", m_nTotalPrice );
	CUIManager::getSingleton()->InsertCommaToString( m_strTotalPrice );
}


//-----------------------------------------------------------------------------
// Purpose: ������ ����մϴ�.
// Input  : iShopID - 
//			bSell - 
// Output : __int64
//-----------------------------------------------------------------------------
__int64 CUIShop::CalculateTotalPrice(int iShopID, int& iCount, BOOL bSell, BOOL bRate)
{
	if(iShopID	<= 0)								return 0;

	MyInfo* pInfo = MY_INFO();
	__int64	iTotalPrice	= 0;
	iCount				= 0;

	for( int i = 0; i < SHOP_TRADE_SLOT_TOTAL; i++ )
	{
		if( m_pIconsTrade[i]->IsEmpty() )
			continue;

		const int iIndex		= m_pIconsTrade[i]->getIndex();
		SQUAD iItemCount		= m_pIconsTrade[i]->getItems()->Item_Sum;
		CItemData* pID			= _pNetwork->GetItemData(iIndex);
		
		if( pID->GetItemIndex() == -1 )
			continue;

		
		__int64 llitemPrice = CalculateItemPrice( iShopID, *pID, iItemCount, bSell );
		__int64 iAddPrice = pInfo->CalcPriceRate(llitemPrice, bSell) + pInfo->CalcPriceAddition(bSell);

		if (bRate == TRUE)
			llitemPrice += iAddPrice;

		iTotalPrice += llitemPrice;
		iCount++;
	}

	return iTotalPrice;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iShopID - 
//			&ID - 
//			iNumOfItem - 
//			bSell - 
//-----------------------------------------------------------------------------
__int64	CUIShop::CalculateItemPrice(int iShopID, const CItemData &ID, int iNumOfItem, BOOL bSell)
{
	if(iShopID <= 0)
		return 0;
	
	const CShopData &SD = _pNetwork->GetShopData(iShopID);	
	const int iShopSellRate	= SD.GetSellRate();
	int iShopBuyRate	= SD.GetBuyRate();
	__int64	iPrice		= ID.GetPrice();
	if( bSell )
		//iPrice = (pID->GetPrice() * iShopSellRate) / 100;
		iPrice = (iPrice * iShopBuyRate) / 100;
	else
	{
		//iPrice = (pID->GetPrice() * iShopBuyRate) / 100;
		iPrice = (iPrice * iShopSellRate) / 100;
		
		// [090617: selo] �̱��� ��� ������ ������ 97�̻��̸� �������� ������ �ݰ����� �Ѵ�.
		if (g_iCountry == USA)
		{
			if( 97 <= ID.GetLevel() )
				iPrice /= 2;
		}
	}

#ifdef NEW_CHAO_SYS
	int fpkpenalty = abs(_pNetwork->MyCharacterInfo.pkpenalty);
	
	if(_pNetwork->MyCharacterInfo.pkpenalty < 0 && bSell)
	{
		if (g_iCountry == RUSSIA)
		{
			iPrice = iPrice + (iPrice * fpkpenalty / 32000 * (20));
		}
	}
#endif

	iPrice *= iNumOfItem;
	return iPrice;
}
//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.16

void CUIShop::CreateCubeStateMsgBox(CNetworkMessage *istr, BOOL bGuild)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_CUBE_STATE) )
		pUIManager->CloseMessageBox(MSGCMD_CUBE_STATE);

	LONG lMyPoint = 0, lInfoCubePoint = 0; // �ڽ��� �Ҽӵ� ��� �� ���� ����Ʈ , �� ����Ʈ�� ť�� ����Ʈ
	BYTE InfoCount = 0, InfoRank = 0; // ����Ʈ ����, ��ũ ����

	CTString strGuildName, strMyName; // ��� �̸�, ��� ������ �� ĳ���� �̸�

	(*istr) >> lMyPoint;
	(*istr) >> InfoCount;

	CUIMsgBox_Info MsgBoxInfo;
	CTString strMsg, strMsgEx;

	if (bGuild)
	{
		strMsg = _S(4388, "��� ť�� ����Ʈ ��Ȳ");
		strMsgEx = _S(4389, "�ڽ��� �Ҽӵ� ��� ť�� ����Ʈ : %d");
		strGuildName = _S(3451,"����");
		strMyName = _S(4344, "��� ������");
	}
	else
	{
		strMsg = _S(4390, "���� ť�� ����Ʈ ��Ȳ");
		strMsgEx = _S(4391, "�ڽ��� ���� ť�� ����Ʈ : %d");
		strGuildName = _S(4392, "�Ҽ� ���");
		strMyName = _S(4393, "ĳ���� �̸�");
	}

	MsgBoxInfo.SetMsgBoxInfo(strMsg, UMBS_OK | UMBS_LISTBOX, UI_SHOP, MSGCMD_CUBE_STATE);

	strMsg.PrintF(strMsgEx, lMyPoint);
	MsgBoxInfo.AddStringEx(strMsg, 0, 1);
	MsgBoxInfo.AddStringEx(_S(4343, "ť������Ʈ ��Ȳ"), 2, 1);
	MsgBoxInfo.AddStringEx(_S(1714, "����"), 4, 1);
	MsgBoxInfo.AddStringEx( strGuildName, 4, 9 );
	MsgBoxInfo.AddStringEx( strMyName, 4, 28 );
	MsgBoxInfo.AddStringEx( _S(4345, "ť������Ʈ"), 4, 47 );
	MsgBoxInfo.SetListBoxPlacement(20, 113, 350, 84, 5, 4); // 173 - 118 = 55
	MsgBoxInfo.m_nColorBoxCount = 9;
	pUIManager->CreateMessageBox( MsgBoxInfo );
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetWidth(400);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().ResetAllStrings();
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(0, 6);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(1, 42);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(2, 156);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().SetColumnPosX(3, 335, TEXT_RIGHT);

	int i;

	for (i=0; i<InfoCount; ++i)
	{
		(*istr) >> InfoRank >> strGuildName >> strMyName >> lInfoCubePoint;

		strMsg.PrintF("%d", InfoRank);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(0, strMsg, 0xF2F2F2FF, FALSE);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(1, strGuildName, 0xF2F2F2FF, FALSE);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(2, strMyName, 0xF2F2F2FF, FALSE);
		strMsg.PrintF("%d", lInfoCubePoint);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(3, strMsg, 0xF2F2F2FF, FALSE );
	}

	WRect rcHorizonalLine, rcVerticalLine0, rcVerticalLine1;
	// ��ü������ ���� -48 , �߰� -12
	rcHorizonalLine.SetRect( 15, 58, 386, 58);
	rcVerticalLine0.SetRect( 15, 58, 15, 204);
	rcVerticalLine1.SetRect( 51, 82, 51, 204);

	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(0, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 24);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(1, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 25);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(2, 0, rcHorizonalLine);
	rcHorizonalLine.Offset(0, 95);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(3, 0, rcHorizonalLine);

	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(4, 0, rcVerticalLine0);
	rcVerticalLine0.Offset(370, 0);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(5, 0, rcVerticalLine0);

	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(6, 0, rcVerticalLine1);
	rcVerticalLine1.Offset(114, 0);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(7, 0, rcVerticalLine1);
	rcVerticalLine1.Offset(114, 0);
	pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->SetColorBox(8, 0, rcVerticalLine1);
}

void CUIShop::AddItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
		ShopToTrade( UIMGR()->GetMsgBoxNumOnly()->GetData() );
}

void CUIShop::DelItemCallback()
{
	if (UIMGR()->GetMsgBoxNumOnly()->GetData() > 0)
		TradeToShop( UIMGR()->GetMsgBoxNumOnly()->GetData() );
}

int CUIShop::RefreshShopItem(int iShopIndex)
{
	if(iShopIndex == -1)	return - 1;

	CShopData &SD = _pNetwork->GetShopData(iShopIndex);
	if(SD.GetIndex() == -1) return - 1;

	CUIManager* pUIManager = CUIManager::getSingleton();

	int t;
	for(t = 0; t < SD.m_iNumOfItem; ++t)
	{						
		int iItemIndex = SD.m_vectorSellItems[t];
		CItemData* pID = _pNetwork->GetItemData(iItemIndex);
		if(pID->GetItemIndex() == -1) continue;

		CItems* pItems = new CItems;
		pItems->Item_Index = iItemIndex;
		pItems->InvenIndex = t;
		pItems->ItemData = pID;
		m_pIconsShop[t]->setData(pItems, false);
		m_pIconsShop[t]->setCount(1);
		m_pIconsShop[t]->Hide(FALSE);
	}

	return t;
}

bool ItemLevelOfComp( const int& x, const int& y )
{
	CItemData*	pItemData[2];

	pItemData[0] = _pNetwork->GetItemData(x);
	pItemData[1] = _pNetwork->GetItemData(y);

	return pItemData[0]->GetLevel() < pItemData[1]->GetLevel();
}

int CUIShop::RefreshLeaseItem(int iJobIndex)
{
	if(iJobIndex == -1)	return - 1;

	CItemData*	pItemData[2];

	int			tv_itemIdx;
	int			tv_itemLevel;

	std::vector<int> vecbtnItems;

	int selNum;
	int max = CItemData::getsize();

	CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
	CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

	for (selNum = 0; iter != eiter; ++iter)
	{	
		CItemData* pData = (*iter).second;

		if (pData == NULL)
			continue;

		if(	pData->GetJob() & 0x01 << iJobIndex &&
			pData->GetType() == CItemData::ITEM_WEAPON )
		{
			tv_itemLevel = pData->GetLevel();
			if(	tv_itemLevel >= LEASE_MIN_LEVEL &&
				tv_itemLevel <= LEASE_MAX_LEVEL )
			{
				if(pData->IsFlag(ITEM_FLAG_LENT) )
				{
					tv_itemIdx = pData->GetItemIndex();

					vecbtnItems.push_back(tv_itemIdx);

					selNum++;
				}
			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// ������ ������ ����
	std::sort(vecbtnItems.begin(), vecbtnItems.end(), ItemLevelOfComp );

	std::vector<int>::iterator btnItr;
	for ( btnItr = vecbtnItems.begin(), selNum = 0; btnItr != vecbtnItems.end(); btnItr++ )
	{
		tv_itemIdx = (*btnItr);

		if (selNum != 0)
		{
			pItemData[0] = _pNetwork->GetItemData((*btnItr));
			pItemData[1] = _pNetwork->GetItemData(m_pIconsShop[selNum - 1]->getIndex());

			if(pItemData[1]->GetLevel() == pItemData[0]->GetLevel())
			{
				// ���߿� ����Ÿ���� �۴ٸ� �ٲ��ش�.
				if(pItemData[1]->GetSubType() > pItemData[0]->GetSubType())
				{
					CItems* pCopy = new CItems;
					memcpy(pCopy, m_pIconsShop[selNum - 1]->getItems(), sizeof(CItems));
					m_pIconsShop[selNum]->setData(pCopy, false);
					
					{
						CItems* pItems = new CItems;
						pItems->Item_Index = tv_itemIdx;
						pItems->ItemData = CItemData::getData(tv_itemIdx);
						m_pIconsShop[selNum - 1]->setData(pItems, false);
						m_pIconsShop[selNum - 1]->setCount(1);

						++selNum;

						continue;
					}					
				}
			}
		}

		// �̺κ��� Shop �κ��丮 ������ ����...
		CItems* pItems = new CItems;
		pItems->Item_Index = tv_itemIdx;
		pItems->ItemData = CItemData::getData(tv_itemIdx);
		m_pIconsShop[selNum]->setData(pItems, false);
		m_pIconsShop[selNum]->setCount(1);

		selNum++;
	}

	return selNum;
}

int CUIShop::RefreshEventItem(int iJobIndex, int itype)
{
	if(iJobIndex == -1)	return - 1;

	CItemData*	pItemData[2];

	int			tv_itemIdx;
	int			tv_itemLevel;

	std::vector<int> vecbtnItems;

	CShopData SD;

	if(itype == CItemData::ITEM_SHIELD)		// ���� NPC�� �������� ������
	{
		SD = (CShopData)_pNetwork->GetShopData(90);			// �� ����
	}else
	{
		SD = (CShopData)_pNetwork->GetShopData(33);			// �������
	}	

	int t, selNum;
	for( t = 0, selNum = 0; t < SD.m_iNumOfItem; ++t )
	{	
		int iItemIndex = SD.m_vectorSellItems[t];
		CItemData* pID = _pNetwork->GetItemData(iItemIndex);

		if( pID->GetJob() & 0x01 << iJobIndex &&	pID->GetType() == itype )
		{
			tv_itemLevel = pID->GetLevel();
			if(	tv_itemLevel >= EVENT_MAY_LEVELMIN &&
				tv_itemLevel <= EVENT_MAY_LEVELMAX )
			{			
				tv_itemIdx = pID->GetItemIndex();

				vecbtnItems.push_back(tv_itemIdx);

				selNum++;

			}
		}
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	// ������ ������ ����
	std::sort(vecbtnItems.begin(), vecbtnItems.end(), ItemLevelOfComp );

	std::vector<int>::iterator btnItr;

	for( btnItr=vecbtnItems.begin(), selNum = 0; btnItr!=vecbtnItems.end(); btnItr++ )
	{
		tv_itemIdx = (*btnItr);

		if(selNum !=0)
		{
			pItemData[0] = _pNetwork->GetItemData((*btnItr));
			pItemData[1] = _pNetwork->GetItemData(
				m_pIconsShop[selNum - 1]->getIndex());

			if(pItemData[1]->GetLevel() == pItemData[0]->GetLevel())
			{
				// ���߿� ����Ÿ���� �۴ٸ� �ٲ��ش�.
				if(pItemData[1]->GetSubType() > pItemData[0]->GetSubType())
				{
					CItems* pCopy = new CItems;
					memcpy(pCopy, m_pIconsShop[selNum - 1]->getItems(), sizeof(CItems));
					m_pIconsShop[selNum - 1]->setData(pCopy, false);
					tv_itemIdx = pItemData[1]->GetItemIndex();																										 
				}
			}
		}

		// �̺κ��� Shop �κ��丮 ������ ����...
		CItems* pItems = new CItems;
		pItems->Item_Index = tv_itemIdx;
		pItems->ItemData = CItemData::getData(tv_itemIdx);
		m_pIconsShop[selNum]->setData(pItems, false);
		m_pIconsShop[selNum]->setCount(1);
		selNum++;
	}

	return selNum;
}

CItems* CUIShop::GetTradeItem( int idx )
{
	if (idx < 0 || idx >= m_vecTrade.size())
		return NULL;

	return m_vecTrade[idx];
}

void CUIShop::clearTrade()
{
	for (int i = 0; i < m_vecTrade.size(); ++i)
		SAFE_DELETE(m_vecTrade[i]);

	m_vecTrade.clear();	
}

void CUIShop::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	int		i;

	for (i = 0; i < SHOP_SLOT_BUY_MAX; ++i)
		m_pIconsShop[i]->Update(fDeltaTime, ElapsedTime);

	for(i = 0; i < SHOP_TRADE_SLOT_TOTAL; ++i)
		m_pIconsTrade[i]->Update(fDeltaTime, ElapsedTime);
}
