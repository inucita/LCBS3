#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIShop.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIMonsterCombo.h>
#include <Engine/Interface/UIMonsterMercenary.h>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Contents/function/GuildWarMixUI.h>
#include <Engine/Contents/function/ShopUI.h>
#include <Engine/info/MyInfo.h>

#define ENABLE_GUILD_CUBE

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

CUIShop::CUIShop()
{
	m_nNpcVirIdx		= -1;
	m_nNpcIndex			= -1;
}

// ----------------------------------------------------------------------------
// Name : ~CUIShop()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIShop::~CUIShop()
{
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

	// Set position of target npc
	m_fNpcX = fX;
	m_fNpcZ = fZ;

	m_nNpcVirIdx = iMobVirIdx;
	m_nNpcIndex = iMobIndex;

	CMobData* MD = CMobData::getData(m_nNpcIndex);
	
	switch( _pNetwork->MyCharacterInfo.sbJoinFlagMerac )
	{
		// �������
	case WCJF_OWNER:
	case WCJF_DEFENSE_GUILD:
	case WCJF_DEFENSE_CHAR:
		{
			if( m_nNpcIndex == 234 || m_nNpcIndex == 235 ) // ������ �����̸� ����
				return;
		}
		break;
		
		// ������ ���
	case WCJF_ATTACK_GUILD:
	case WCJF_ATTACK_CHAR:
		{
			if( m_nNpcIndex == 232 || m_nNpcIndex == 233 ) // ������ �����̸� ���� 
				return;
		}
		break;
	}

	pUIManager->CloseMessageBox( MSGCMD_DROPITEM );

	// Create refine message box
	pUIManager->CreateMessageBoxL( _S( 263, "����" ) , UI_SHOP, MSGLCMD_SHOP_REQ );

	CTString	strNpcName = CMobData::getData(m_nNpcIndex)->GetName();
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, TRUE, strNpcName, -1, 0xE18600FF );

	if( m_nNpcIndex == 272 ) // ���� �뿩��
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
	}
	//------ ���� �޺� NPC
	else if(m_nNpcIndex == 488)
	{

		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4036, "�ȳ��ϼ��� �ڽ��� �ɷ��� �����ϱ� ���Ͽ� ���� �޺���ŭ Ȯ���� �͵� ����"),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4037, "�ڽ��� �ɷ��� ���� ���ϰ� �ڸ��ϴٰ� ���� �޺��� Ŭ���� �ϱ�� �������."),-1,0xa3a1a3ff);
		pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ,TRUE, _S(4038, "�� ���� �ڽ��� �ɷ��� ������ ������"),-1,0xa3a1a3ff);
		
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4039, "���� �޺� �����" ), COMBO_MAKE );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4040, "���� �̿��ϱ�" ), SHOP_BUY );			
		
	}
	else if(m_nNpcIndex == 489)
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
	else if(m_nNpcIndex == 490)
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
	else if(m_nNpcIndex >= 521 && m_nNpcIndex <= 525)			//Cube NPCs
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
		if( m_nNpcIndex == 233 || m_nNpcIndex == 235 ) // ���� ���ջ���
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
	
	if( m_nNpcIndex == 233 || m_nNpcIndex == 235 ) // ���� ���ջ���
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
		(m_nNpcIndex == 90 || m_nNpcIndex == 129 || m_nNpcIndex == 169 || m_nNpcIndex == 279 || m_nNpcIndex == 508 || m_nNpcIndex == 1029) )
	{
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4889, "���� ���� �̺�Ʈ �ȳ�"), EVENT_WORLDCUP_2010_INFO );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4890, "Ȳ�� �౸������ ��ȯ"), EVENT_WORLDCUP_2010_BALL );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S(4881, "���� ���� ���ڷ� ��ȯ"), EVENT_WORLDCUP_2010_GOLDENBALL );
	}

	if(m_nNpcIndex == 226)
	{
		pUIManager->CloseMessageBox( MSGCMD_SUPERGOJE_REQUEST );
		pUIManager->CloseMessageBox( MSGCMD_SUPERGOJE_NOTIFY );
		pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1782, "�� ��� ���ü� �ޱ�" ), SHOP_SUPERGOJE  );
	}

	pUIManager->AddMessageBoxLString(MSGLCMD_SHOP_REQ, FALSE, _S( 1748, "NPC �ȳ�" ), SHOP_NPC_HELP); //ttos : �ȳ��ý��� �߰���
	pUIManager->AddMessageBoxLString( MSGLCMD_SHOP_REQ, FALSE, _S( 1220, "����Ѵ�." ) );	
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
					pUIManager->GetShopUI()->openUI(m_nNpcIndex, m_nNpcVirIdx, m_fNpcX, m_fNpcZ, eSHOP_BUY);					
				}
				break;

			case SHOP_SELL : // Sell
				{	
					pUIManager->GetShopUI()->openUI(m_nNpcIndex, m_nNpcVirIdx, m_fNpcX, m_fNpcZ, eSHOP_SELL);	
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
					pUIManager->GetShopUI()->SetLeaseJob(tv_job);
					pUIManager->GetShopUI()->openUI(m_nNpcIndex, m_nNpcVirIdx, m_fNpcX, m_fNpcZ, eSHOP_LEASE);	
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

	CUIManager* pUIMgr = UIMGR();

	switch( nCommandCode )
	{
	case MSGCMD_SHOP_ADD_PLUSITEM:
		{
			pUIMgr->GetShopUI()->ShopToTrade( 1 );
		}
		break;

	case MSGCMD_SUPERGOJE_REQUEST:
		{
			_pNetwork->SendSuperStoneReq();			
		} break;

	case MSGCMD_SHOP_LEASE_ITEM :
		{
			// Send Network Message
			pUIMgr->GetShopUI()->BuyItems();
			pUIMgr->GetShopUI()->closeUI();
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
	CTString strPoint = pUIManager->IntegerToCommaString(lMyPoint);

	if (bGuild)
	{
		strMsg = _S(4388, "��� ť�� ����Ʈ ��Ȳ");
		strMsgEx = _S(4389, "�ڽ��� �Ҽӵ� ��� ť�� ����Ʈ : %s");
		strGuildName = _S(3451,"����");
		strMyName = _S(4344, "��� ������");
	}
	else
	{
		strMsg = _S(4390, "���� ť�� ����Ʈ ��Ȳ");
		strMsgEx = _S(4391, "�ڽ��� ���� ť�� ����Ʈ : %s");
		strGuildName = _S(4392, "�Ҽ� ���");
		strMyName = _S(4393, "ĳ���� �̸�");
	}

	MsgBoxInfo.SetMsgBoxInfo(strMsg, UMBS_OK | UMBS_LISTBOX, UI_SHOP, MSGCMD_CUBE_STATE);

	strMsg.PrintF(strMsgEx, strPoint);
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

		strMsg = pUIManager->IntegerToCommaString(InfoRank);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(0, strMsg, 0xF2F2F2FF, FALSE);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(1, strGuildName, 0xF2F2F2FF, FALSE);
		pUIManager->GetMessageBox(MSGCMD_CUBE_STATE)->GetListBox().AddString(2, strMyName, 0xF2F2F2FF, FALSE);
		strMsg = pUIManager->IntegerToCommaString(lInfoCubePoint);
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
