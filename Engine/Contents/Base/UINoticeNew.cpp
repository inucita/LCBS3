#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Contents/Base/UINoticeNew.h>
#include <Engine/Entities/QuestSystem.h>
#include <Engine/Interface/UIQuickSlot.h>
#include <Engine/Interface/UIChildQuickSlot.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/Entities/TEventString.h>
#include <algorithm>
#include <Engine/Contents/Base/UIQuestNew.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Help/Util_Help.h>
#include <Engine/Contents/Base/UIQuestAccept.h>
#include <Engine/Contents/function/NewsUI.h>
#include <Engine/Contents/function/News_Web_UI.h>

extern INDEX	g_iCountry;

const int iStopChangeItem		= 521;		// ���� ���.
const int iAffinityRewardNotice = 47;		// [100322: selo] ģȭ�� ���� �˸�

#define NOTICE_HEIGHT_GAP		(20 + CHILD_QUICKSLOT_HEIGHT * 2)

#define CHARGE_BTN_INDEX		1000
#define DEF_QUESTONCE_ICON_IDX	5
#define DEF_QUESTUNLIMITED_ICON_IDX	4000
#define PET_QUEST_HORSE_INDEX	4000		// �� ���� ����Ʈ...
#define PET_QUEST_DRAGON_INDEX	4001
#define EVENT_NETCAFE_INDEX		4002		// (�Ϻ�, �±�) netcafeķ���� �̺�Ʈ
// wooss 060809 add rare item
#define PET_QUEST_BLUE_HORSE_INDEX		4003
#define PET_QUEST_PINK_DRAGON_INDEX		4004
#define PET_QUEST_MYSTERY_HORSE_INDEX	4005
#define PET_QUEST_MYSTERY_DRAGON_INDEX	4006

// [091013: selo] ������ �� ��ȭ ���� ���� �˸�
#define PET_NOTICE_EVOLUTION_ENABLE		4020
#define DEF_TOOLTIP_WIDTH	200

enum eEvent_Index
{
	PANDORA_EVENT_INDEX =26,
	XMAS_EVENT_INDEX =27,
	CHILDREN_DAY_EVENT_INDEX =28,
	SONGKRAN_EVENT_INDEX =32,
};

//--------------------------------------------------------------
// UI Open
class CmdNoticeInfoOpen : public Command
{
public:
	CmdNoticeInfoOpen() : pUI_(NULL), eType_(CUINoticeGroup::NONE_GROUP), x_(0), y_(0) {}
	void setData(CUINoticeGroup* pUI, CUINoticeGroup::GROUP_TYPE eType, int x, int y)
	{ 
		pUI_ = pUI; 
		eType_ = eType;
		x_ = x;
		y_ = y;
	}
	void execute() {
		if (pUI_)
			pUI_->Open(eType_, x_, y_);
	}
private:
	CUINoticeGroup* pUI_;
	CUINoticeGroup::GROUP_TYPE eType_;
	int x_, y_;
};

//--------------------------------------------------------------
// UI Close
class CmdNoticeInfoClose : public Command
{
public:
	CmdNoticeInfoClose() : pUI_(NULL) {}
	void setData(CUINoticeGroup* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_ && pUI_->GetFixUi() == FALSE)
			pUI_->Close();
	}
private:
	CUINoticeGroup* pUI_;
};

//--------------------------------------------------------------
// BTN Click
class CmdNoticeBtnClick : public Command
{
public:
	CmdNoticeBtnClick() : pUI_(NULL), pOpenUI_(NULL), eType_(CUINoticeGroup::NONE_GROUP), x_(0), y_(0) {}
	void setData(CUINoticeNew* pUI, CUINoticeGroup::GROUP_TYPE eType, CUINoticeGroup* pOpenUi, int x, int y)
	{ 
		pUI_ = pUI; 
		eType_ = eType;
		pOpenUI_ = pOpenUi;
		x_ = x;
		y_ = y;
	}
	void execute() {
		if (pOpenUI_)
			pOpenUI_->Open(eType_, x_, y_);

		if (pUI_)
		{
			pUI_->SetIconBlink(eType_, true);
			CUIManager::getSingleton()->RearrangeOrder(UI_NOTICE, TRUE);
		}
	}
private:
	CUINoticeNew* pUI_;
	CUINoticeGroup::GROUP_TYPE eType_;
	CUINoticeGroup* pOpenUI_;
	int x_, y_;
};

//--------------------------------------------------------------
// List Item Click
class CmdNoticeItemClick : public Command
{
public:
	CmdNoticeItemClick() : pUI_(NULL), Type_(Notice::NOTICE_NONE), Index_(-1) {}
	void setData(CUINoticeNew* pUI, Notice::eNoticeType Type, int Index)
	{ 
		pUI_ = pUI; 
		Type_ = Type;
		Index_ = Index;
	}
	void execute() {
		if (pUI_)
		{
			pUI_->PressOK(Type_, Index_);
		}
	}
private:
	CUINoticeNew* pUI_;
	Notice::eNoticeType Type_;
	int Index_;
};

//--------------------------------------------------------------
// List Item Over or Leave
class CmdItemMouseEvent : public Command
{
public:
	CmdItemMouseEvent() : pUI_(NULL), pItem_(NULL), Col_(0xFFFFFFFF) {}
	void setData(CUINoticeGroup* pUI, CUIListItem* pItem, COLOR col = 0xFFFFFFFF)
	{ 
		pUI_ = pUI; 
		pItem_ = pItem;
		Col_ = col;
	}
	void execute() {
		if (pUI_)
		{
			pUI_->SetStringColor(pItem_, Col_);
		}
	}
private:
	CUINoticeGroup*	pUI_;
	CUIListItem*	pItem_;
	COLOR			Col_;
};

extern UINT g_uiEngineVersion;

int nMaxNotice = 4;
int nChargeBtnPos = 0;

// [2011/01/18 : Sora] �⼮ �̺�Ʈ
const int colorBarWidth = 290;
const int attendanceMsgBoxWidth = 350;

CUINoticeNew::CUINoticeNew()
	: m_pList(NULL)
	, m_pGroup(NULL)
{	
	m_bAttendanceEvent = FALSE;
}

CUINoticeNew::~CUINoticeNew()
{
}

void CUINoticeNew::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( pixMaxI - m_nWidth, pixMaxJ - m_nHeight - QUICKSLOT_HEIGHT - NOTICE_HEIGHT_GAP - nChargeBtnPos ); // ���� ��ư �߰� 
}

void CUINoticeNew::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUINoticeNew::PressOK(Notice::eNoticeType iNoticeType, int nIndex)
{	
	CUIManager* pUIManager = CUIManager::getSingleton();
	CTString strMsg;	// Temp String
	UIBtnExType btnType;

	GameDataManager* pGame = GAMEDATAMGR();
	Notice* pData = pGame->GetNotice();

	if (pData == NULL)
		return;

	switch(iNoticeType)
	{
	case Notice::NOTICE_NEWS_WEB:	// �� ����.
		{
			btnType = UBET_QUEST;
		}
		break;
	case Notice::NOTICE_NEWS_NORMAL:	// ���� �˾�
		{
			pData->SetNoticeBlink(iNoticeType, false);
			btnType = UBET_QUEST;
		}
		break;
	case Notice::NOTICE_QUEST:		// ����Ʈ
	case Notice::NOTICE_EVENT:		// �̺�Ʈ
		btnType = UBET_QUEST;
		break;
	case Notice::NOTICE_POLYMOPH:	// ����
		{
			pData->SetNoticeBlink(iNoticeType, false);
			btnType = UBET_ITEM;
		}		
		break;
	case Notice::NOTICE_AFFINITY:	// ģȭ�� ����
		{
			pData->SetNoticeBlink(iNoticeType, false);
			btnType = UBET_ACTION;
		}		
		break;
	default:
		return;
	}

	switch( btnType )
	{
	case UBET_QUEST:		// ����Ʈ ��ư
		{
			const int iNoticeIndex	= nIndex;

			// NOTE : �ֿϵ��� ȹ�� ����Ʈ ���� �ϵ��ڵ��� �κ�.
			if( iNoticeIndex == PET_QUEST_HORSE_INDEX
				|| iNoticeIndex == PET_QUEST_BLUE_HORSE_INDEX 
				|| iNoticeIndex == PET_QUEST_MYSTERY_HORSE_INDEX	)
			{
				pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
				CTString strMessage = _S(2162, "�ֿϵ��� ȹ�� ����Ʈ" );
				pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
				
				if(iNoticeIndex == PET_QUEST_BLUE_HORSE_INDEX )
					strMessage =_S(3350, "�Ķ� ���� �Ǹ��� ������ �η����� ã�ư��� �Ķ� �ֿϵ��� ���ϸ� ���� �� �ִ� ����Ʈ�� ������ �� �ֽ��ϴ�.");
				else if(iNoticeIndex == PET_QUEST_MYSTERY_HORSE_INDEX )

					strMessage =_S(3351, "�Ұ������� ���� �Ǹ��� ������ �η����� ã�ư��� �Ұ������� �ֿϵ��� ���ϸ� ���� �� �ִ� ����Ʈ�� ������ �� �ֽ��ϴ�.");
				else strMessage =_S(2163,"���� �Ǹ��� ������ �η����� ã�ư��� ������ �ֿϵ��� ���ϸ� ���� �� �ִ� ����Ʈ�� ������ �� �ֽ��ϴ�.");
				
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
				
				strMessage = _S( 191, "Ȯ��" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				pData->DelFromNoticeList(nIndex, Notice::NOTICE_EVENT);
			}
			else if( iNoticeIndex == PET_QUEST_DRAGON_INDEX
				|| iNoticeIndex == PET_QUEST_PINK_DRAGON_INDEX
				|| iNoticeIndex == PET_QUEST_MYSTERY_DRAGON_INDEX )
			{
				CTString strMessage;				
				pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
				strMessage = _S(2164, "�ֿϵ��� ȹ�� ����Ʈ" );
				pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );

				// wooss 060810
				// ���� ������ �߰� 
				if( iNoticeIndex == PET_QUEST_PINK_DRAGON_INDEX )
					strMessage = _S(3352, "��ũ �巹��ũ�� ���� ������ �η����� ã�ư��� ��ũ���� �ֿϵ��� �������� ���� �� �ִ� ����Ʈ�� ������ �� �ֽ��ϴ�.");
				if( iNoticeIndex == PET_QUEST_MYSTERY_DRAGON_INDEX )
					strMessage = _S(3353, "�Ұ������� �巹��ũ�� ���� ������ �η����� ã�ư��� �Ұ������� �ֿϵ��� �������� ���� �� �ִ� ����Ʈ�� ������ �� �ֽ��ϴ�.");
				else strMessage = _S(2165,"�巹��ũ�� ���� ������ �η����� ã�ư��� ������ �ֿϵ��� �������� ���� �� �ִ� ����Ʈ�� ������ �� �ֽ��ϴ�.");

				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );

				strMessage = _S( 191, "Ȯ��" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				pData->DelFromNoticeList(nIndex, Notice::NOTICE_EVENT);
			}
			
			// [091013: selo] ������ ���� ��ȭ ������ ������ �Ǿ��� ��
			else if( iNoticeIndex == PET_NOTICE_EVOLUTION_ENABLE )
			{
				CTString strMessage;				
				pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );

				strMessage = _S(4727,"�� ��ȭ");
				pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );

				strMessage = _S(4728,"���� �մϴ�. ���� ������ ��ȭ�� �� �� �ִ� ������ �Ǿ����ϴ�. �ֿϵ��� ���û縦 ���ؼ� ��ȭ�� �غ�����. ��ȭ�� �ϰ� �Ǹ� ���� ���ϰ� �Ǵ� �����ϰ� �����ϰ� �����ϼ���.");

				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );

				strMessage = _S( 191, "Ȯ��" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				pData->DelFromNoticeList(nIndex, Notice::NOTICE_EVENT);
			}
#ifdef NETCAFE_CAMPAIGN
			else if( iNoticeIndex >= EVENT_NETCAFE_INDEX )
			{
				CTString strMessage;				
				pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
				strMessage = _S(2744, "NetCafe ķ����" );
				pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
				strMessage = _S(2745, "���� ���� ���ͳ� ī�信�� ��Ʈ ī������ �÷����ϴ� �������Դ� ���� �ð��� ���� �������ڰ� ���޵˴ϴ�." );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S(2746, "���� ���� ���� ���ڸ� ������ �η����� ã�ư��� �پ��� ����ǰ �� �Ѱ��� ȹ���� �� �ֽ��ϴ�." );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
				strMessage = _S(2747, "���� 10%�� ���ʽ� ����ġ�� 5%�� SP�� �߰��� ȹ�� �Ͻ� �� �ֽ��ϴ�." );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));

				strMessage = _S(2748, "1�ð� �÷��̽�  ����� �������� ����" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
				strMessage = _S(2749, "2�ð� �÷��̽�  �ູ�� �������� ����" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
				strMessage = _S(2750, "3�ð� �÷��̽�  ������ �������� ����" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
				strMessage = _S(2751, "4�ð��̻� �÷��̽�  �γ��� �������� ����" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );

				strMessage = _S( 191, "Ȯ��" );
				pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				pData->DelFromNoticeList(nIndex, Notice::NOTICE_EVENT);
			}
#endif
			// �̺�Ʈ�� ���.
			else if (iNoticeIndex == NEWS_WEB)
			{
				pUIManager->GetNewsWebUI()->OpenUI();
				break;
			}
			else if (iNoticeIndex == NEWS_NORMAL)
			{
				pUIManager->GetNewsUI()->OpenUI();
				break;
			}
			else if( iNoticeIndex >= EVENT_START_INDEX )
			{
				int iEventIndex = iNoticeIndex - EVENT_START_INDEX;

				// ���� ����.
				if( iEventIndex == TEVENT_TREASUREBOX || iEventIndex == TEVENT_JPN_2007_NEWSERVER )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					// Date : 2005-07-05(���� 4:26:58), By Lee Ki-hwan
					// �ؿ� �۾��� ������� (�̺�Ʈ�߰�)
					// Create event message box
					//pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );							
					pUIManager->CreateMessageBoxL( _S( 1940,"�������� ����Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1940, "�������� ����Ʈ" ), -1, 0xE18600FF );	
					
					//pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1676, "ĳ���͸� �����ϸ� ������ ĳ������ �κ��丮�� �������ڸ� 1�� ���޵˴ϴ�." ));		
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(1941,"ĳ���͸� �����ϸ� ������ ĳ���� �κ��丮�� �̺�Ʈ �ǿ� �������ڰ� 1�� ���޵˴ϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1677, "�����ÿ� ���޵Ǵ� �������ڴ� ĳ������ ������ 10Lv�� �Ǿ��� �� �� �������ڸ� ��� ���� ���������� �η����� ���ؼ� ��� ���� �ֽ��ϴ�." ));	
					//pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1678, "���� ���ڸ� �� ��� �ش� �������ڰ� ������ �� �ִ� ǰ���߿��� �������� �������� ���޵˴ϴ�." ));	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1942, "���� ���ڸ� �� ��� �ش� �������ڰ� ������ �� �ִ� ǰ���߿��� �������� �������� ���޵˴ϴ�." ));		
					//pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1679, "10Lv �������ڸ� ���� ���� �ܰ��� ���������� 14Lv �������ڸ� �η����� �����ϰԵǰ� 10Lv, 14Lv, 18Lv, 22Lv, 26Lv, 30Lv �� 6���� �������ڸ� �̺�Ʈ �Ⱓ���� ���޵˴ϴ�." ));	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(1943,"10Lv �������ڸ� ���� ���� �ܰ��� ���������� 14Lv �������ڸ� �η����� �����ϰ� �ǰ� 10Lv, 14Lv, 18Lv, 22Lv, 26Lv, 30Lv �� 6���� �������ڸ� �ܰ������� �����մϴ�." ));	
					//pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ) );
					//pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1680, "�̺�Ʈ �Ⱓ" ), -1, 0xFFCB00FF );	
					//pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1681, "7�� 19�ϱ���" ) );	
					
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				// �İ��� ���ʽ� �̺�Ʈ
				else if( iEventIndex == TEVENT_TEACH )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S(1944,"�İ��� ���ʽ� �̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1944, "�İ��� ���ʽ� �̺�Ʈ" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1945, "�̺�Ʈ�Ⱓ ���� �İ����� �İ��� �ý����� ���� �ű������� �缺�ϴµ� �����ϸ� �� ����Ʈ�� ����� 2���� 20����Ʈ�� �����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1946, "�̺�Ʈ�Ⱓ ���� �߽����� �İ��� �ý����� ���� 20������ ������ ��� ���� �ڽ��� Ŭ������ �´� '25Lv + 3' ���⸦ �����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1947, "�̺�Ʈ�Ⱓ ���� �߽����� �İ��� �ý����� �̿��� ������ ��� 10�������� 19�������� ���� ���� �� ������ 10,000������ �ݷ����� ���޵˴ϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1948, "�Ⱓ : 7�� 29�� 10:00 ~ 8�� 12�� 9:00����" ), -1, 0xE18600FF );			
					
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_FRUIT )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S(1949,"���� ���� �̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1949, "���� ���� �̺�Ʈ" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1950 , "�̺�Ʈ�Ⱓ ���� ���͸� ����ϸ� ���� Ȯ���� ���Ͱ� ��¥�� ���� '����', '����', '�ڵ�' �������� ����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1951, "����" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1952, "���� �������� ����� ��� 10�а� ����ġ ������� 1.5�� ����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1953, "�Ⱓ : 8�� 2�� ~ 5�� ���� �������˱���" ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1954, "����" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1955, "���� �������� ����� ��� 10�а� ���õ� ������� 2�� ����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1956, "�Ⱓ : 8�� 5�� ~ 9�� ���� �������˱���" ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1957, "�ڵ�" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1958, "�ڵ� �������� ����� ��� 10�а� ������ ��ӷ��� 2�� ����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1959, "�Ⱓ : 8�� 9�� ~ 12�� ���� �������˱���" ));			
					
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				// �߼����� �̺�Ʈ // Date : 2005-09-08(���� 2:28:22), By Lee Ki-hwan
				else if( iEventIndex == TEVENT_CHUSEOK )
				{
					CTString strMessage;				
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S( 1960, "�߼����� �̺�Ʈ" );
					pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );			
					
					strMessage = _S( 1961, "������ ���� �߼��� �����Ͽ� �߼����� �̺�Ʈ�� �����մϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					
					strMessage =_S( 1962,  "�̺�Ʈ �Ⱓ ���� ���͸� ����ϸ� ���� �������� ���� �� ������, ������ ���� ����� ���Ͽ� ���� �� �ִ� '�����'�� ���׷��̵尡 �����ϸ�, ���׷��̵�� �η����� ���Ͽ� �Ͻ� �� �ֽ��ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					strMessage = _S( 1963, "������ ����, ����, ����, ����, ���� �ܰ�� ���׷��̵� �� �� ������ �̺�Ʈ �Ⱓ ������ �̺�Ʈ �Ⱓ ���� �� ���׷��̵��� ������ �η��ο��� �������� ����ǰ�� ���� �� �ֽ��ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					strMessage =_S(1964 ,  "���� ��ǰ�� Ȩ������ ���������� �����Ͻñ� �ٶ��ϴ�." );	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );	

				}
				// Date : 2005-09-08(���� 3:38:14), By Lee Ki-hwan ���� ���� �̺�Ʈ 
				else if( iEventIndex == TEVENT_SEPTEMBER )
				{
					CTString strMessage;				
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage =_S( 1965,  "�������� �̺�Ʈ" );
					pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );			
					
					strMessage = _S( 1966, "�������� �̺�Ʈ �Ⱓ ���� ���͸� ����Ͻø� ����Ȯ���� Ư���� ������ ����ġ ������, ���õ� ������, ��ӷ� ������, ���� �������� ���� �� �ֽ��ϴ�." ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 1967, "4���� ������ �� �ڽ��� ���ϴ� �������� �����ϰ� ���͸� ����� ��� ������ ȿ���� ���� ���� Ȯ���� ������ ������ �������ϴ�." ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 1968, "�ڼ��� ������ Ȩ������ ���������� �����Ͻñ� �ٶ��ϴ�." ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 1969, "�Ⱓ : 9�� 17�� ~ 30�� ����" ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					
					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );	

				}
				else if( iEventIndex == TEVENT_2PAN4PAN1 ) // ������ �̺�Ʈ 
				{
					CTString strMessage;				
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S(2166, "���� ��Ż 2pan4pan ���� �̺�Ʈ" );
					pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "�ϳ�!  ���� ���߱� �̺�Ʈ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "���Ͱ� ����ϴ� ���� �������� ������ Ư�� ���ڸ� �����Ͽ� �η��ο��Լ� ������ ���� �� �ִ� �̺�Ʈ �Դϴ�.";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "��!  ���ⱳü �̺�Ʈ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "�̺�Ʈ �Ⱓ���� �������� ����� �ڽ��� ����ϴ� ���⸦ ���� ���� �� �ٸ� Ŭ������ ����� ��ü �� �� �ִ� �̺�Ʈ �Դϴ�. �������� ������ �����Լ� ��ü�� �� �ֽ��ϴ�.";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "��!  ������ ���� �̺�Ʈ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "���� �������� �ǰ��� �����Ͽ� ������ ���� ��ǰ�� �Ϻ� �����Ͽ� ������ �帳�ϴ�.";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = "��!  �İ��� ������ �̺�Ʈ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = "�߽����� ������ 20�� �Ǿ������ �߽������� �̺�Ʈ ���� �����ϰ� �İ����� 2���� ��ġ�� ����ϴ�";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );	

					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );	

				}
				else if( iEventIndex == TEVENT_PEPERO ) // ������ �̺�Ʈ.
				{
					CTString strMessage;				
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S(2326, "������ ���� �̺�Ʈ" );
					pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2327,"��ΰ� �Բ��ϴ� ������ ���̸� �����Ͽ� '������ ���� �̺�Ʈ'�� ���� �մϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2328,"�̺�Ʈ �Ⱓ ���� ���͸� ����ϸ� ����Ȯ���� �̺�Ʈ �������� '������'�� '�Ƹ�� ������'�� ȹ���� �� �ֽ��ϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2329,"'������'�� '�Ƹ�� ������'�� ���� HP�� MP�� ���ÿ� ȸ�� ���� �ָ� '�Ƹ�� ������'�� ��� �Ͻ������� ȸ�������ִ� �����Դϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2330,"�Ⱓ : 2005�� 11�� 11��(��)" ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S(2331, "     ~ 2005�� 11�� 15��(ȭ) ���� �������� ��" ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			

					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_XMAS )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );							
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2286, "2005 ũ�������� �̺�Ʈ" ), -1, 0xE18600FF );	
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2297, "���͸� ����ϸ� ������ Ȯ���� [��],[���] �������� ����� �˴ϴ�."));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2298, "�� �������� ������ ��� ������ ���� ã�ư� ������ ��Ű�� ������ ���� �� �ִ� �̺�Ʈ �Դϴ�." ));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2299, "�ڼ��� ���� ��ǰ�� Ȩ������ ���� ������ �����Ͻñ� �ٶ��ϴ�."));	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2300, "�̺�Ʈ �Ⱓ : 2005.12.23 ~ 2005.12.30" ));	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ) );
									
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_NEWYEAR )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
				
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );							
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2287, "2006 �ų� �γ��� ���� �̺�Ʈ" ), -1, 0xE18600FF );	
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2301, "�̺�Ʈ �Ⱓ���� ������ �ڽ��� ĳ���� ��� �ð��� ���� �׿� ���� ������ ������ �� �ִ� �̺�Ʈ�Դϴ�."));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2302, "����ǰ�� �̺�Ʈ�� ������ ��� ������ ���� ã�ư��� �̺�Ʈ �Ⱓ���� ��ɽð��� ���� ����ǰ�� �־����ϴ�." ));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2299, "�ڼ��� ���� ��ǰ�� Ȩ������ ���� ������ �����Ͻñ� �ٶ��ϴ�."));	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2303, "�̺�Ʈ �Ⱓ : 2005.12.30 ~ 2006.1.3" ));	
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ) );
									
					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if(iEventIndex == TEVENT_CHANGE_ARMOR ) // ���ⱳü �̺�Ʈ
				{
					CTString strMessage;				
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					
					strMessage = _S(2332, "����&�� ��ü �̺�Ʈ" );
					pUIManager->CreateMessageBoxL( strMessage, UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2333,"29Lv�̻��� ����, ���� Ÿ Ŭ������ ���� ����� ��ü�� �ִ� �̺�Ʈ �Դϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2334,"�̺�Ʈ �Ⱓ ���ȿ��� ��븶���� ������ ������ ã�ư��� ����� ��� ��ü�� ������ �̺�Ʈ�� �P���� �������� Nas�� �ް� ��� ��ü�� �ݴϴ�. ");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2335,"���� : ��� ��ü�� �� ���ð��� ��ü�� ������ ���� �ɼǰ��� ������ �˴ϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2336, "�Ⱓ : 2005�� 12�� 20��(ȭ)" ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S(2337, "     ~ 2005�� 12�� 23��(��) ���� �������� ��" ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			

					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				}
				else if(iEventIndex == TEVENT_SEARCHFRIEND ) // ģ�� ã�� �̺�Ʈ
				{
					CTString strMessage;				
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );		
					strMessage = _S( 2433, "ģ��ã�� �̺�Ʈ " );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2434, "���� ����ڰ� Ư�� NPC�� ���� �ּ� 1���� �̻� �޸� ������ ĳ���͸� ��� �մϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2435, "�׷� �Ŀ� �� �޸� ���� ĳ���Ͱ� ���ӿ� �����Ͽ� ���� �ð� �÷��̸� �ϰ� �Ǹ� �޸� ���� ĳ���ʹ� ���� ĳ���ͺ��� ����ġ�� �� ���� ��� �˴ϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2436, "����, ���� �ð� ������ �����ϸ� �ڽ��� ����� ģ���� ���� ������ ���� �� �ִ� �̺�Ʈ �Դϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2437, "��, ���� ���� ĳ���ͳ� ���� �ֹι�ȣ�� ��ϵ� ������ ĳ���ʹ� ģ���� ����� �� �����ϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2438, "�Ⱓ : 2006�� 2�� 14��" ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			
					strMessage = _S( 2439, "     ~ 2006�� 3�� 14�� " ); 
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );			

					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );

				}
				else if( iEventIndex == TEVENT_VALENTINE_2006 ) // �߷� Ÿ�� ���� �̺�Ʈ
				{
					CTString strMessage;
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S(2418, "�߷�Ÿ�� ���� �̺�Ʈ" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2419, "�̺�Ʈ �Ⱓ���ȿ� ���͵��� ���� ���ڷ��� �Ͼ� ���ڷ��� ����մϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2420, "���� ���ڷ� ȿ��: 5�а� ����,���� ���ݷ� 65����, �̵��ӵ� ����� ȿ���� ���ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2421, "�Ͼ� ���ڷ� ȿ��: 5�а� ����,���� ���� 125����, �̵��ӵ� ����� ȿ���� ���ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2422, "�̺�Ʈ �Ⱓ: 2006. 2. 10 ~ 2006. 2. 17" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 1220, "����Ѵ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_WHITEDAY )
				{
					CTString strMessage;
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S( 2520, "ȭ��Ʈ ���� �̺�Ʈ" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";//A3A1A3
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2521, "�̺�Ʈ �Ⱓ ���� ���͵��� ������ �˻����� ����մϴ�. ������ �������� ������ ü���� �����Ͻñ� �ٶ��ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2522, "����ȿ�� : ������ ��� HP�� MP�� �� 600�� 15�� ���� ȸ������ �ݴϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S( 2523, "�˻���ȿ�� : ������ ��� 15�а� �ִ�HP 300�� �ִ�MP 200�� �����˴ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2524, "�̺�Ʈ �Ⱓ: 2006. 3. 10 ~ 2006. 3. 17" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 1220, "����Ѵ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_OLD_SERVER )
				{
					CTString strMessage;
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S(2598, "�ż��� ���� �̺�Ʈ" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S( 2599, "�̺�Ʈ �Ⱓ���� ������ ��� ���Ͱ� �ν��͸� ����� Ȯ���� 2�� ��µ˴ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2600,"���� �ڽ��� ���� �̻��� ���͸� ����ϸ� ����Ȯ���� ����ġ ���ǰ� ���ݷ�, ���� ��������� ȹ���Ͻ� �� �ֽ��ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );					
					strMessage = _S(2560, "���ݷ� �����(��) : 5�а� �������ݷ� 35,�������ݷ� 30 ����" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2561, "���� �����(��) : 5�а� �������� 55 ���" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2562, "���ݷ� �����(��) : 5�а� �������ݷ� 100,�������ݷ� 80 ����" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2563, "���� �����(��) : 5�а� �������� 95 ���" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S( 2601,"����ġ �����: 20�� ���� �������ġ 1.5�� ���" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S( 2602, "�̺�Ʈ �Ⱓ: 2006. 4. 20 ~ 2006. 4. 28" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_NEW_SERVER )
				{
					CTString strMessage;
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100," �̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					strMessage = _S(2557, "ȥ���� ���� �̺�Ʈ ");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xE18600FF );
					strMessage = " ";//A3A1A3
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0xA3A1A3FF );
					strMessage = _S(2566, "�ű� ���� ���¹� ȥ���� ���� ������Ʈ ���� ���� �̺�Ʈ�� �ν��� �̺�Ʈ, ����ġ ���� �̺�Ʈ�� ���� �̺�Ʈ�� ����˴ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2567, "�̺�Ʈ �Ⱓ���� ��� ������ �ν��� ���Ȯ���� 2�谡 ��µǾ� �ս��� �ν��͸� ������ �� �ֽ��ϴ�.");
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2568, "���� �ڽ��� ���� �̻��� ���Ϳ��Լ� ����Ȯ���� ����ġ ���� ���ǰ� ��,�� ��� ������ ��ӵ˴ϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage);
					strMessage = _S(2569, "����ġ ���� : 20�а� ����ġ 1.5�� ȹ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2560, "���ݷ� �����(��) : 5�а� �������ݷ� 35,�������ݷ� 30 ����" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2561, "���� �����(��) : 5�а� �������� 55 ���" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2562, "���ݷ� �����(��) : 5�а� �������ݷ� 100,�������ݷ� 80 ����" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = _S(2563, "���� �����(��) : 5�а� �������� 95 ���" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage, -1, 0x6BD2FFFF );
					strMessage = " ";
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					strMessage = _S(2564, "�̺�Ʈ �Ⱓ: 2006. 3. 30 ~ 2006. 4. 7" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					strMessage = _S( 191, "Ȯ��" );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_SAKURA ) // �Ϻ� ����� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2530, "����� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2531, "������ ����� �̺�Ʈ�� ����� �� �̺�Ʈ�� �Ϻ� �����ǻ� ���� �̺�Ʈ�� ���ÿ� ����˴ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2532, "����� �� �̺�Ʈ: ���� ��ɽ� ����Ȯ���� ���ݷ°� ������ �������ִ� ���� ��ȫ ����, ���� �� ����, ��ȫ ����, �� ������ ��ӵ˴ϴ�." ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2533, "�Ϻ� �����ǻ� ���� �̺�Ʈ: ����� ���Ͽ� ���� ���츮�� ��Ƽ� ��븶���� �η��ο��� �������� �Ϻ� �����ǻ����� ��ȯ �ϽǼ� �ֽ��ϴ�." ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2534, "��, ĳ������ �����̻��� ���Ϳ��Լ��� �������� ��ӵ˴ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2535, "�̺�Ʈ �Ⱓ: 2006�� 3�� 23�� ~ 2006�� 4�� 6��" ) );

					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				else if( iEventIndex == TEVENT_OX_QUIZ ) // O.X �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2603,"O.X ���� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S(2604, "�̺�Ʈ �Ⱓ ���� ��Ʈ ī�������� ���� ��İ� �Ϲ� ����� ��� üũ�� �� �ִ� ��ȸ�� O.X �̺�Ʈ�� �����մϴ�."));
					 pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S(2605, "����, ������ ���� ���� ���� �پ��� ����ǰ�� ���޵˴ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S(2606, "���� ��� : �̺�Ʈ ������ ���������� ������ ���߸� ���ȿ� ������ ����ǰ�� ���� �� ������, Ʋ�� ��� ������� ���� ������ �߹��� ���մϴ�." ) );

					if( g_uiEngineVersion < 10000 )	// ������ ���
					{
						pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
							_S(2607, "�̺�Ʈ�� �� �������� 2ä�ο����� ���� �˴ϴ�." ) );
						
						pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
						
						if( _pNetwork->m_iServerGroup < 4 )
						{
							pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
								_S(2608, "�Ⱓ: 2006�� 4�� 18�� ~ 2006�� 4�� 24��(���� 8��)" ), -1, 0x6BD2FFFF );
						}
						else
						{
							pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
								_S(2609, "�Ⱓ: 2006�� 4�� 20�� ~ 2006�� 4�� 24��(���� 8��)" ), -1, 0x6BD2FFFF );
						}
					}

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2610, "����: 10����: �ν��� 10��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2611, "      15����: ������ 10��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2612, "      18����: ������ü� 5��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(2613, "      20����: ������ü� 10��" ) );

					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				
				// ---------- WORLDCUP EVENT wooss 060530 --------------------------------------------------->>
				else if( iEventIndex == TEVENT_WORLDCUP ) // ������ �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2771, "�౸ �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2772, "�౸ ��ȸ �Ⱓ ���� �� ���� �̺�Ʈ�� ����˴ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2773, "1. ������� ��ƶ�" ), -1, 0xFF44AAFF );
					strMsg = _S( 2774, "�౸ ��ȸ �Ⱓ���� ���Ϳ��� ��ӵǴ� FIFA���� ��� ������ ����� ����Ǵ� �������� ����� ��ȯ�ϰ�" );
					strMsg+= _S( 2775, "��ȯ�� ������ ��ȸ ������ ���� ������ �޴� �̺�Ʈ �Դϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMsg , -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2776, "�̺�Ʈ �Ⱓ" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2777, "�� ����� ���&���� ��ȯ : 2006.06.09(ȭ) ~ 06.30(��) ���� ������" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2778, "�� ��� ���� : 2006.07.10 ����" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2779, "2. ��纼 �̺�Ʈ" ) ,-1,0xFF44AAFF);
					strMsg = _S( 2780, "Ư�� ����� ����� �����ؼ� �����ϴ� �̺�Ʈ�� �̺�Ʈ�� ���۵Ǹ� ���Ϳ��Լ� ��ӵǴ� ��纼�� 10�� ���" ) ;
					strMsg += _S( 2781, "�¸����� ������ �����ϰ� ��纼 ī�带 �޽��ϴ�. ��Ⱑ ����� �� ����� ������ ������ ���� �� �ֽ��ϴ�." ) ;
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMsg , -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
						_S( 2782, "�̺�Ʈ �Ⱓ : 2006.06.09 ~ 07.10" ), -1, 0x6BD2FFFF );

					CTString strMessage;				
					strMessage.PrintF( _S( 191, "Ȯ��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, strMessage );
				}
				// ---------- WORLDCUP EVENT wooss 060530 ---------------------------------------------------<<
				else if( iEventIndex == 100 ) // ��� �� �̺�Ʈ ( ��Ʈ��ũ���� �ε����� ������ �ʴ´�. )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2783, "��� �� �̺�Ʈ" ), -1, 0xE18600FF );
					strMsg = _S( 2784, "��� �� �̺�Ʈ ���� ���� �Ǿ����ϴ�. �����̽�Ʈ �౸�� 10���� �����Ͽ� ����� �������� ���𿡰� ã�ư�" );
					
					CTString strMessage;
					strMessage.PrintF( _S( 2785, " %s VS %s �౸����� ����� ���� ���ñ� �ٶ��ϴ�." ),
						pUIManager->GetQuest()->GetStrTeamA(), pUIManager->GetQuest()->GetStrTeamB() );

					strMsg += strMessage;
					strMsg += _S( 2786, "�̺�Ʈ ���� �� ��� ����� ���� �����۰� ������ �����մϴ�." );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMsg );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2787, "��纼 ���� ���� �ð�" ), -1, 0x6BD2FFFF );
					
					SYSTEMTIME GoldenTime = pUIManager->GetQuest()->GetGoldenTime();
					strMessage.PrintF( _S( 2788, "%d�� %d�� %d�� %d�� %d�� ����" ), 
						GoldenTime.wYear, GoldenTime.wMonth, GoldenTime.wDay, GoldenTime.wHour, GoldenTime.wMinute );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == 101 )// ��� �� �̺�Ʈ ����( ��Ʈ��ũ���� �ε����� ������ �ʴ´�. )
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2789, "��� �� �̺�Ʈ ����" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2790, "��� �� �̺�Ʈ ������ ���� �Ǿ����ϴ�." ) );

					CTString strMessage;
					strMessage.PrintF( _S( 2791, "%s VS %s �౸����� ����� %d : %d �Դϴ�." ),
						pUIManager->GetQuest()->GetStrTeamA(), pUIManager->GetQuest()->GetStrTeamB(),
						pUIManager->GetQuest()->GetScoreTeamA(), pUIManager->GetQuest()->GetScoreTeamB() );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2792, "��� �� �̺�Ʈ�� ���� �Ͻ� ���� ��� ���� ������ ���� ã�ư��� �̺�Ʈ�� ���� ������ �����ñ� �ٶ��ϴ�." ) );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2793, "��纼 ���� �ð�" ), -1, 0x6BD2FFFF );
					
					SYSTEMTIME GoldenTime = pUIManager->GetQuest()->GetGoldenTime();
					strMessage.PrintF( _S( 2788, "%d�� %d�� %d�� %d�� %d�� ����" ), 
						GoldenTime.wYear, GoldenTime.wMonth, GoldenTime.wDay, GoldenTime.wHour, GoldenTime.wMinute );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, strMessage );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_RAIN ) // ����� �̺�Ʈ  -> ��Ӵϳ� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2753, "��Ӵϳ� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2892, "�̺�Ʈ �Ⱓ ���� ���͸� ����ϸ� ����Ȯ���� �𽺹β�, �𽺹β� ���ġ 2������ �������� ��� �մϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2893, "�̰��� ��������� ���𿡰� ������ ����, �𽺹β� 10�� �Ǵ� �𽺹β� ���ġ 2���� 1ȸ�� ���� ������ ���� �� �ֽ��ϴ�.(�ݺ����డ��)" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,
									_S( 2894, "�̺�Ʈ �Ⱓ : 2006�� 8�� 9�� ~ 2006�� 8�� 30��" ), -1, 0x6BD2FFFF );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_BUDDHIST ) // �ұ� �к� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2913, "�ұ� �к� ���� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2914, "�̺�Ʈ �Ⱓ ���� ��� ���͵�� ���� ���� ����(��, ��, Ȳ, ����)�� �𷡰� ��� ���� �������� ��ӵ˴ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2915, "�𷡺����� ��� ���� ã�ư��� ������������� ��ȯ�� ���� �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " " ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2916, "�̺�Ʈ �Ⱓ: 2006�� 7�� 11�� ~ 7�� 26��" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2917, "�ڼ��� ������ ������Ʈ�� �����ϼ���." ) );

					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}				
				else if( iEventIndex == A_EVENT_COLLECT_BUG )
				{	// ���� �̺�Ʈ - ���� ä��
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2940, "���� ä�� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2941, "������ �¾� � ������ ȸ���� �� �� �ִ� ����ä�� �̺�Ʈ�� �����մϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2942, "�̺�Ʈ�Ⱓ ���� �η��ο��� ����ä�����ڸ� �����ϸ� ���Ͱ� ����ϴ� ���� ������ ��Ƽ� ä�����ڿ� ������ �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2943, "ä�����ڰ� ���� ���� �η��ο��� �������� ä���� ���濡 ���� �پ��� ������ ���� �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2944, "ä�����ڴ� ���� �� ������� �� ������ �����Ͽ� ����ؼ� �̺�Ʈ�� ������ �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 2945, "�̺�Ʈ �Ⱓ:2012�� 7�� 20�� ~ 8�� 31��" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_NEWSERVER_BASTARD )
				{	// �ż� �ٽ�Ÿ�� ���� �̺�Ʈ : �ż�
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "������Ʈ�� ���� ���� ��� �̺�Ʈ") , -1, 0xE18600FF );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ1. �ູ�� ������ ��ƶ�"),-1,0xF9E061FF  );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ �Ⱓ ���� �ڽ��� ������ �´� ���͸� ����ϸ� �ູ�� ������ ��ӵ˴ϴ�." ) );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�ູ�� ���� ȿ�� : �����ϸ� 20�а� ������ ����ġ 20%�� �߰� ȹ����." ) );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ2. 100�� ������ ��ƶ�!"),-1, 0xF9E061FF  );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ �Ⱓ���� 35������ ������ ��� ĳ���Ϳ��� 100�� ������ �����մϴ�." ) );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ �Ⱓ:2006�� 7�� 21�� ~ 8�� 4��" ), -1, 0x6BD2FFFF );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_NEWSERVER_BASTARD_OLD )
				{	// �ż� �ٽ�Ÿ�� ���� �̺�Ʈ : ����
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "���� ����Ʈ 'ȥ���� �ô�' ������Ʈ ��� �̺�Ʈ" ), -1, 0xE18600FF );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ �Ⱓ ���� �ڽ��� ������ �´� ���͸� ����ϸ� �ູ�� ������ ��ӵ˴ϴ�." ) );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�ູ�� ���� ȿ�� : �����ϸ� 20�а� ������ ����ġ 20%�� �߰� ȹ����." ) );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "�̺�Ʈ �Ⱓ:2006�� 7�� 21�� ~ 8�� 4��" ), -1, 0x6BD2FFFF );//TRNS
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_CHUSEOK_2006 )
				{	// 2006 �߼� �̺�Ʈ	:Su-won
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3114, "�߼����� ���� ����� �̺�Ʈ"), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3115, "������ ���� �߼��� �����Ͽ� ������� �̺�Ʈ�� �����մϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3116, "�̺�Ʈ �Ⱓ ���� ���͸� ����ϸ� ������ ����� �Ұ���, ��, ������ ���� �� �ִµ�, ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3117, "�� 1���� ��� �η��ο��� �������� ����Ȯ���� ������ ����� �ݴϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3118, "������ ������ 5�а� �̵��ӵ�, ���ݼӵ��� ���Ǹ�, ���� 10���� ��Ƽ� ���������� �����" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3119, "���𿡰� �������� ������ ������ü������� ������ ���� �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( " ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3120, "������� �̺�Ʈ�� �̺�Ʈ �Ⱓ���� �ݺ��ؼ� ������ �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3121, "�̺�Ʈ �Ⱓ:2006�� 9�� 26�� ~ 10�� 10��" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_HALLOWEEN )
				{
					// ������ ���� �̺�Ʈ( == �ǵ��� ���� )
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3154, "�����ε��� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3155, "�����ε��� �̺�Ʈ �Ⱓ���� ���, ���ź, �޶�ũ ������ ���� 15���� �ǵ����� ���ڰ� ��Ÿ���ϴ�. �ǵ����� ���ڸ� ���� �� ���� Ȯ���� ������ �̺�Ʈ ������ Ȥ�� ������ ��ӵǰų� �ǵ����� ���ָ� ���� ���Ͱ� �����ϰ� �˴ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3156, "�ǵ����� ���ڴ� �����ð� ������ ���� ������� �������� �ٸ� ��ġ�� �ڷ���Ʈ �˴ϴ�.") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3157, "�̺�Ʈ �Ⱓ:2006�� 11�� 7�� ~ 11�� 14��" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_XMAS_2006 )
				{
					// 2006 X-MAS Event [12/11/2006 Theodoric]
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3169, "ũ�������� �̺�Ʈ"), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  CTString(" "));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3170, "ũ�������� �̺�Ʈ �Ⱓ ���� �̺�Ʈ ���͸� ����ϸ� ���� ���� ������ �������� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3171, "���� ������ �� ����ũ �������� 3�� ��Ƽ� ������ ��ġ�� ��������� �������ø� �������� ����� ���� �������� ������ �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3172, "����� ������ ũ���������� ��Ʈ ī���� �� �Բ� ���� ��̰� ��������."));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  CTString(" "));
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(1680, "�̺�Ʈ �Ⱓ"), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE,  _S(3173, "2006�� 11�� 21�� ~ 2007�� 1�� 10��"), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));									
				}
				else if( iEventIndex == CHILDREN_DAY_EVENT_INDEX )
				{
					//�±�: ��̳� �̺�Ʈ
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 3184, "��̳� �̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE );					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3184, "��̳� �̺�Ʈ" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 1945, "�̺�Ʈ�Ⱓ ���� �İ����� �İ��� �ý����� ���� �ű������� �缺�ϴµ� �����ϸ� �� ����Ʈ�� ����� 2���� 20����Ʈ�� �����մϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3185, "�̺�Ʈ�Ⱓ ���� �߽����� �İ��� �ý����� ���� 20������ ������ ��� ������ �̺�Ʈ ���� �̿ܿ� 500,000������ �ݷ������� ���޵˴ϴ�." ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString( "  " ));			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3186, "�Ⱓ: 2007�� 1�� 10�� ~ 2007�� 1�� 24��" ), -1, 0xE18600FF );			
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ));
				}
				else if( iEventIndex == A_EVENT_VALENTINE_DAY )
				{
					// ���� ���� �̺�Ʈ
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3203, "���귯�� �̺�Ʈ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3204, "�߷�Ÿ�ε��̿� ������ ������ ������ ���� �Ͽ� ���귯�� �̺�Ʈ�� �ǽ��մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3205, "�̺�Ʈ �Ⱓ ���� ���͵��� īī�� �������� �ٸ� 3������ ���ڷ��� ����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3206, "���ڷ��� �׳� ����� ���� �ְ�, ������������ �Ǹ��ϴ� ���ڻ��ڸ� �����Ͽ� ������ӿ� ���� �� ���� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3207, "���ڻ��ڿ� ���ڷ��� �ֱ� ���ؼ��� ������������ ������������ �����Ͽ� ���ڷ��� ��ƾ߸� ���ڻ��ڿ� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3208, "���� �ϼ��� ���ڻ��ڴ� �̺�Ʈ NPC ���𿡰� �������� ���� ���� ��ǰ�� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3209, "īī�� �������� ���� ���ڷ����� ���� ���� ��� �� ���� ��ǰ�� ���� Ȯ���� �������ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3210, "���ڻ��ڴ� �̺�Ʈ ���� �� 1���ϰ� �����ϸ� ���Ŀ��� ������ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3211, "īī�� ���ڷ� ȿ��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3212, "īī�� �������� 30,60,70�� 3������ ���ڷ��� ����� ��� ����, ���� ���ݷ��� 5%,7%,10% 5�а� ��½��� �ݴϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3213, "�̺�Ʈ �Ⱓ: 2012.02.13 ~ 2012.02.20"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if( iEventIndex == A_EVENT_WHITE_DAY )
				{
					// ȭ��Ʈ���� �̺�Ʈ
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3227, "2012 ȭ��Ʈ���� ������� �̺�Ʈ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3228, "ȭ��Ʈ���̸� �¾� �̺�Ʈ �Ⱓ���� ���Ͱ� �ϴû��� ��ȫ�� ������ ����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3229, "���Ͱ� ����� �ϴû� ������ ���� ĳ����, ��ȫ�� ������ ���� ĳ���͸� ȹ���� �� ������,"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3230, "ȹ���� ���Ŀ��� ĳ���Ͱ� ��ȯ�̳� ���λ����� ���ؼ� �����Ͻ� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3231, "�ϴû� ������ ��ȫ�� ������ ��� ������ ������ �ִ� �η��ο��� ������ �ָ�"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3232, "ĳ������ �ɷ��� ���� �ִ� ������� �ֹ��� ���� ��ü�� ���������� ����� �޽����� ���� �� �ִ� �������� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3233, "���ϴû����� 5�� + ��ȫ������ 5�� : 6���� ������� �� �������� �ϳ��� ȣ���� ��� �ް� �˴ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3234, "���ϴû����� 20�� : �ϴû� ������ 1��"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3235, "�º�ȫ������ 20�� : ��ȫ�� ������ 1��"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3236, "!�̺�Ʈ �Ⱓ���� ������ ��� �ݺ��ؼ� ���� ���� �� ������ ���� ������ �������� �ִ� 5������� ���� �����մϴ�."), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3237, "�̺�Ʈ �Ⱓ: 2012.03.13 ~ 2012.03.27"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				// [KH_070326] ��Ȱ�� �̺�Ʈ
				else if( iEventIndex == A_EVENT_EGGS_HUNT )
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3268, "Multiple Easter Eggs Hunt"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3269, "4��8�� ��Ȱ���� �¾� ��Ȱ�� �ް� ��� �̺�Ʈ�� �����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3270, "�̺�Ʈ�Ⱓ ���� ���� �� ������ GM�� ��Ȱ�� �ް��� ����Ʈ���ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3271, "�ٴڿ� �ִ� �ް��� �ֿ� �����ϸ� �����ϰ� �پ��� ����ǰ�� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3272, "�º�Ȱ�� �ް��� ���ÿ� 1�� �̻� ȹ���� �� �����ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3273, "������� Ȯ���� ���͵� ��Ȱ�� �ް��� ����Ʈ�� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3274, "�̺�Ʈ �Ⱓ: 2012.04.04 ~ "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if( iEventIndex == SONGKRAN_EVENT_INDEX )
				{
					// �±� ��ũ�� �̺�Ʈ
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3275, "��ũ�� ���� �̺�Ʈ" ), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3276, "�̺�Ʈ�Ⱓ ���� 3������ Ư���� ���� �Ͼ�ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3277, "1. ���͸� ����ϴ� ���� �ݱ׸��� ���׸��� ��ӵ˴ϴ�."),-1, 0xF9E061FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3278, "ȹ���� �ݱ׸��� ���׸��� ���� Ŭ���ؼ� �����ϸ� ����ǰ�� ���� �� �ֽ��ϴ�.") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3279, "2. �� �Ѹ��� Ư����� �߰�"),-1, 0xF9E061FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3280, "�Ҽ� �׼�(ALT+C)â���� Ȯ���Ͻ� �� ������, ���� ���� ĳ���ʹ� ������ �Ͻ������� ��µ˴ϴ�.") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3281, "3. ������ ���� ��ȯ"),-1, 0xF9E061FF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3282, "��ũ���� �¾� ������ ���� ��뿡 �����Ͽ� ��� ��ȯ���ݴϴ�.") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S( 3283, "�̺�Ʈ �Ⱓ:2007�� 4�� 11�� ~ 4�� 25��" ), -1, 0x6BD2FFFF );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if( iEventIndex == TEVENT_GOMDOLI )	
				{
					// ������ �̺�Ʈ  35
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3457, "������ ���� ã�ƶ�!"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3458, "���� ��Ʈ�� ���⹫�⸦ ü���ϰ� ��̵鿡�� ��Ʈī���� ���е��� ���� ��� ������ ���� �� �ִ� �� ���� ��ȸ!!!"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3459, "���� ������ '��Ÿ�Ͻ���'�� ���̸��� ����� �� ������ ��� �ִ� ����̵鿡�� �������� ������ '����'�� ����� �־����ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3460, "�� ���� �� '��Ÿ�Ͻ���'�� ������ ����� ������' ������'�� �渶���� ���� ���� �׸� ������ ����� ���Ϳ��� �Ѿ ������ ���ҽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3461, "[��Ÿ�Ͻ���]�� ����� �� '��ξ�'�� ���̸��� ����� ���Ϳ��Լ� ������ ������ ã���� ��縦 ã�� �ִٰ� �մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3462, "ĳ������ �ɷ��� ���� �ִ� ������� �ֹ��� ���� ��ü�� ���������� ����� �޽����� ���� �� �ִ� �������� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3463, "������ ���� 9���� ��� ��� [��ξ�]���� �������� ���������� ������ ���� ������ ���� �� ������ ������ 1�� ĳ���Ϳ� ���� �� �ѹ��� [ü��� ���� ��Ʈ]�� [���⹫��]�� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3464, "�¿ϼ��� ������ ������ ��ü ���� ���� ���� ���� �� ������ �п�ǰ ��Ʈ�� ��̳� õ���� �� ��̵鿡�� �����ϰ� �˴ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3465, "�̺�Ʈ �Ⱓ: 2007�� 4�� 24�� ~ 2007�� 4�� 30��"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
// [KH_070425] �̺�Ʈ ���� ���� �߰�(TEVENT_PARENTS_2007, TEVENT_CHILDREN_2007, TEVENT_TEACHER_2007, TEVENT_UCC_2007)
				else if(iEventIndex == TEVENT_PARENTS_2007)
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3362, "ī���̼��� ��ƶ�~"), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3363, "����̳��� �¾� ��Ʈī������ ����� ���� ��帶���͸� ���� �̺�Ʈ�� �غ��߽��ϴ�." ) );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3364, "�̺�Ʈ �Ⱓ ���� ���Ͱ� ��� �� ���� ī���̼��� �������� ��ȯ�̳� ���λ����� ���ؼ� ������ �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3365, "���� ī���̼��� ���� ��� �����Ϳ� �� ��� �����͸��� NPC�� ���ؼ� ��� ����Ʈ�� ���� ǰ�� ȹ���� �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3366, "����� �̸��� �θ� �˸��� ������ ��������۵� ���� �� �ִ� �̺�Ʈ�� ���� ���� �ٶ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3367, "�� ��� �����Ϳ� �� ��� �����͸��� ���� ī���̼��� ����� �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3368, "�� ��� ����� ��Ƽ����� �ϸ� ���� ī���̼��� ���� ���� ���� �� �ֽ��ϴ�." ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3369, "�̺�Ʈ �Ⱓ: 2007�� 5�� 8�� ~ 2007�� 5�� 15��" ) );
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if(iEventIndex == TEVENT_CHILDREN_2007)
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3370, "�� ���� Ż������"), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3371, "��̳��� �¾� ��Ʈī������ ��� ���� �� ������ ���� �̺�Ʈ�� �غ��߽��ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3372, "�̺�Ʈ �Ⱓ ���� ���Ͱ� ��� �� ��� ���������� �������� ��ȯ�̳� ���λ����� ���ؼ� ������ �� �ֽ��ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3373, "��� ���������� ��� �̺�Ʈ NPC���� ������ �ָ� ���迡 �ʿ��� +3��� �帳�ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3374, "�� �̺�Ʈ ����: 1�������� 24���� ĳ����") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3375, "�� �ѹ� ���� ���� �� �� ������ �� �����ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3376, "�� �ٸ� ������ ���� ���� �� ���� �ڽ��� ĳ���Ϳ� �´� ��� ȹ���� �� �ֽ��ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3377, "�� �̺�Ʈ �Ⱓ ���� �ñ��� ������� �ʴ� ��� ���������� �ϰ� �����˴ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3378, "�̺�Ʈ �Ⱓ: 2007�� 4�� 30�� ~ 2007�� 5�� 15��") );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if(iEventIndex == TEVENT_TEACHER_2007)
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3379, "������ ����"), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3380, "������ ���� �¾� ��Ʈī������ ���� ���� �İ����� ���� �̺�Ʈ�� �غ��߽��ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3381, "�̺�Ʈ �Ⱓ ���� �߽��� �缺�� �����Ͽ��� ��� �߰� ������ �־����ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3382, "�İ��ΰ� �߽��� �� �� �߰� ������ �־����� �̹� �̺�Ʈ�� ���� ���� ��Ź �帮�ڽ��ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3383, "�� �̺�Ʈ �Ⱓ ���� �İ��ΰ� �߽������� ��Ƽ�� ���Ƽ�� �������ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3384, "�� �̺�Ʈ �������� �κ����� �̺�Ʈ ������ ĭ�� ���� �˴ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3385, "�̺�Ʈ �Ⱓ: 2007�� 4�� 30�� ~ 2007�� 5�� 15��") );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if(iEventIndex == TEVENT_UCC_2007)
				{
					pUIManager->CloseMessageBoxL( MSGLCMD_EVENT_NOTICE );
					pUIManager->CreateMessageBoxL( _S( 100, "�̺�Ʈ" ), UI_NOTICE, MSGLCMD_EVENT_NOTICE );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, TRUE, _S(3386, "���� ���� ��ī"), -1, 0xE18600FF );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3387, "��Ʈī������ ���鼭 ����־��� ���Ǽҵ� �� ���� ���� ������� ����") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3388, "��Ʈī������ ���� ��� ����� UCC�� �����Ͽ�") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3389, "Ȩ�������� ���ؼ� �÷��ֽø� �����ɻ� 50% ��ü�ɻ� 50%�� �ɻ縦 ���ؼ� �û��� �� �����Դϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3390, "Ǫ���� ��ǰ���� ��ٸ��� ������ ���� ���� ��Ź �帮�ڽ��ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3391, "�� UCC (������, �Ϸ���Ʈ, ī��, ���Ǽҵ� ��)") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3392, "�� ������ ��Ʈī������ ���� ��� ����� �����Ͽ� �ֽø� �˴ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3393, "�� �̺�Ʈ�� ������ ��ǰ�� ���۱��� ���� ��Ƽ �������θ�Ʈ�� �ͼӵ��� �˷��帳�ϴ�.") );
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" ") );
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3394, "�̺�Ʈ �Ⱓ: 2007�� 4�� 30�� ~ 2007�� 5�� 31��") );
					
					pUIManager->AddMessageBoxLString( MSGLCMD_EVENT_NOTICE, FALSE, _S( 191, "Ȯ��" ) );
				}
				else if(iEventIndex == TEVENT_FLOWERTREE)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3504, "�ɳ��� �̺�Ʈ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3505, "�̺�Ʈ �Ⱓ ���� ���͸� ����ϸ� �ɳ����� Ű�� �� �ִ� [���]�� [��ȭ��]�� ��ӵ˴ϴ�"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3506, "[���]�� [��ȭ��]�� ��� ��� �Ϲ��� �ִ� �����翡�� �������� ������ ���� �ִ� �ӻ��� ������ ���� �ǿ�µ� ������ �� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3509, "�̺�Ʈ �Ⱓ: 2007�� 5�� 15�� ~ 2007�� 5�� 29��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3507, "�� �̺�Ʈ �������: ����� ���� [���]�� [��ȭ��]�� ȹ���Ͽ� �����翡�� ������ ��ȭ�� �����, ������ [���]�� [��ȭ��]�� ���ڸ�ŭ �⿩���� �����Ǹ�, ��ü ������ �� �⿩���� �����ʿ� ���� �� 10�ܰ�� �ɳ����� ��ȭ�ϰ� �˴ϴ�."), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3508, "�� �̺�Ʈ ����1: �ڽ��� �⿩�� 100�� �ɳ��� ��ȯ�� 1���� ���� �� �ֽ��ϴ�. ���� ��ȯ���� Ȯ���ϸ� ������ü��� ������ ���� ����ǰ�� ���� �� �ֽ��ϴ�."), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3524, "�� ��ȯ���� �޾Ƶ� �⿩���� ��� �����Ǿ� ǥ�õ˴ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3525, "�� �̺�Ʈ ��ǰ2: �̺�Ʈ�� ���� �� �� ���� ���� �⿩���� �޼��� ���� ������ �Ѵ� ���� ��õ ���� �̺�Ʈ�� ����˴ϴ�."), -1, 0xE18600FF);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				// [070705: Su-won] EVENT_SUMMER_2007
				else if( iEventIndex == TEVENT_SUMMER_2007)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3561, "�������� �̺�Ʈ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3562, "������ �¾� � ������ ȸ���� �� �� �ִ� �������� �̺�Ʈ�� �����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3563, "�̺�Ʈ �Ⱓ���� ���� ��� �ÿ� ��ӵǴ� �������� �����̸� ��Ƽ� �η��ο��� ã�ư� ��Ź�Ͽ� �����̸� �̿��� ���� ���⸦ �Ͽ� ������ ����� ���� ������ ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3564, "�̺�Ʈ ���� �� ���̴� ��ӵ��� ������ ������ �Ѵ� �ı��� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3565, "�̺�Ʈ �Ⱓ: 2007�� 8�� 16��(��) ~ 2007�� 8�� 28��(ȭ)"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_INDEPENDENCEDAY)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3547, "��������� �̺�Ʈ"), -1, 0xE18600FF);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3548, "�̱��� ����������� �¾� �̺�Ʈ�� �����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3549, "�̺�Ʈ �Ⱓ ���� ���͸� ����ϸ� ��ɿ� ������ �ִ� �ٹ���, ������ ������� �� Ŭ������ ��Ŭ �� ���ڸ� ȹ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3550, "������ ȿ��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3551, "* �ٹ��� ������: ��� �� 5�а� ȹ�� ����ġ �� SP�� 50% ��� �����ش�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3552, "* ������ ������: ��� �� 5�а� ����&���� ���ݷ�, ���������� 50�� �����ȴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3553, "�̺�Ʈ �Ⱓ: 2007�� 7�� 3�� ~2007�� 7�� 4��"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));

				}
				else if( iEventIndex == TEVENT_ADULT_OPEN )
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3655, "�̺�Ʈ 1. ��ħ���� ���!"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3656, "�̺�Ʈ �Ⱓ���� �����ϴ� ��� ĳ���Ϳ���, �η����� ã�ư� ��ȭ�� �ϸ� �̺�Ʈ ĳ�� ��ǰ ��Ű�� ��Ʈ(���� HP/MP Ȯ������, ����â�� Ȯ��ī��, ��Ƽ���� 3�ð� ���ױ�, ���ν���)�� ���� �����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3605, "�̺�Ʈ 2. ����ī�带 ã�ƶ�!"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3657, "�η����� �Ҿ���� ī�带 ����ּ���. �ɷ��� ���� ��½����ִ� ������ �ɾ�帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3565, "�̺�Ʈ �Ⱓ: 2007�� 8�� 16��(��) ~ 2007�� 8�� 28��(ȭ)"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));

				}
				else if( iEventIndex == TEVENT_TG2007_1000DAYS || iEventIndex == TEVENT_TG2007_FRUITFULL ) // WSS_TG2007 2007/09/17 ǳ�� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3761, "ǳ�� �̺�Ʈ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3762, "��Ȯ�� ����� ������ ���� ��Ʈī���������� Ű�� [����, ����, ���, ��Ÿ]�� �̿��ؼ� �����ܰ�� ���׷��̵��ϸ� ���� ���� ������ ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3763, "�����ܰ���� Ű��� ��Ʈī�����ΰ� ȹ���Ͽ� �̺�Ʈ ���������� ����� �������� ������� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3764, "(�̺�Ʈ �������� �̺�Ʈ �Ⱓ�� ������ �ϰ������� �����˴ϴ�)"));



					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(1680, "�̺�Ʈ �Ⱓ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 3765, "2007�� 9�� 20�� ~ 2007�� 10�� 4��"));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));

				}
				else if (iEventIndex == A_EVENT_HOLLOWEEN) // �ҷ��� �̺�Ʈ2007
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3797, "�ҷ��� �̺�Ʈ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3800, "1. ȣ�� �Ӹ� Ż �ޱ�"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3801, "��� �� ��ӵǴ� ""�� ���� �ٱ���""�� ������ ȣ�ڱ��� ����ƺ� ã�ư� ���� �ɸ� �������� 6���� ""����"" �� 1������ ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3802, "6�� ��θ� ���� �� ȣ�ڱ��� ����ƺ� ���� ""ȣ�� �Ӹ� Ż""�� ��ȯ ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3806, "2. ȣ�ڸӸ� �ͽ� ���"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3807, "�ʵ忡 ���� �ִ� ""ȣ�� �Ӹ� �ͽ�""�� ��ȯ ���� ""ȣ�� �Ӹ� Ż""�� ���� ã�� ��� �� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3808, "��ӵǴ� ""ȣ��"" �������� ȣ�ڱ��� ����ƺ� ���Ͽ� �پ��� ���������� ��ȯ �����մϴ�."));
#ifndef NEW_HOLLOWEEN_EVENT
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3803, "3. ������� �� ������ ���� �ޱ�"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3804, "��ȯ�� ""����""�� ����ϸ� �������� ""�ֻ��� �� ��ǥ""�� ""�־��� �� ��ǥ""�� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3805, "�� 2���� ��ǥ�� �̺�ƮNPC�� ���� ""������� �� ���������""�� ��ȯ ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3798, "4. �ҷ��� �̺�Ʈ �ڽ��� �ǻ� �ޱ�"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3799, "�̺�Ʈ �Ⱓ ���� ȣ�ڱ��� ����ƺ� ã�ư� ���� �ɸ� ""�Ǹ� ���� �Ӹ���""�� ���� �� �ֽ��ϴ�."));
#else
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(5112, "3. �ҷ��� ��ũ�� ���� �ޱ�"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3804, "��ȯ�� ""����""�� ����ϸ� �������� ""�ֻ��� �� ��ǥ""�� ""�־��� �� ��ǥ""�� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(5113, "�� 2���� ��ǥ�� �̺�ƮNPC�� ���� ""�ҷ��� ��ũ�� ����""���� ��ȯ ���� �� �ֽ��ϴ�."));
#endif
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(1680, "�̺�Ʈ �Ⱓ"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3809, "2007�� 10�� 23�� ~ 2007�� 11�� 6��"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_XMAS_2007) // ũ�������� �̺�Ʈ 2007
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(5808, "��ŸŬ�ν��� ã�ƶ�!"), -1, 0xE18600FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(5809, "���� ������ '��Ÿ�Ͻ���'�� ���̸��� ����� �� ������ ��� �ִ� ����̵鿡�� �������� ��ŸŬ�ν� ������ ����� �־����ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(5810, "�� ���� �� '��Ÿ�Ͻ���'�� ������ ����� ������' ������'�� �渶���� ���� ��ŸŬ�ν� ������ �׸� ������ ����� ���Ϳ��� �Ѿ ������ ���ҽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(5811, "'��Ÿ�Ͻ���'�� ����� �� '��ξ�'�� ���̸��� ����� ���Ϳ��Լ� ��ŸŬ�ν� ���� ������ ã���� ��縦 ã�� �ִٰ� �մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if( iEventIndex == TEVENT_NEWYEAR_2008) //2008�� �ų� �̺�Ʈ 
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3969, "�ų� �̺�Ʈ"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3970, "�ų��� �����Ͽ� �����λ縦 �ϸ鼭 �ظ��̹����� �� ���ִ� �̺�Ʈ�� �����Ͽ����ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3971, "�̺�Ʈ �Ⱓ: 2007�� 12�� 27��(��) ~ 2008�� 1�� 15��(ȭ)"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3972, "�̺�Ʈ ���: �ظ��� �λ��� ������ �����ϰ� �Ҽ�â���� �λ縦 �ϸ� �ظ��� ������ �����λ縦 �ϰ� �˴ϴ�."), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3973, "�����λ�� ���Ѿ��� ����� �� ������ ������ 1�ð����� �ѹ��� ���� �� �ֽ��ϴ�. ��, �ظ��̹��� �ޱ�� �������θ� ������ �� �ֽ��ϴ�."), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_LUNARNEWYEAR_2008) // 2008 ���� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3996, "2008 ���� ���ָӴ� �̺�Ʈ"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3997, "������ ������ ������ ���� �Ͽ� 2007�� ���귯�� �̺�Ʈ 2ź ���� ���� ���ָӴ� �̺�Ʈ�� �ǽ��մϴ�"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3998, "3������ ���ָӴϴ� ���� �� ���� �ٸ� ȿ���� �ִµ�, �� ���ָӴϸ� ��Ƽ� ��������� ���� ������ �� ���� �ִ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3999, "������ӿ� �����ϱ� ���ؼ��� �������� �Ǹ��ϴ� �Ҹ����ڸ� �����Ͽ��� �ϸ�, ���ָӴϴ� �������� �Ǹ��ϴ� ������� ��� �Ҹ����ڿ� ���� �� �ִ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4000, "���� �Ҹ����ڴ� 1ĳ���ʹ� 1���� ���ڸ� ������ �� ������, ���ڴ� â������ ���� �ʴ´�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4001, "���ڿ��� 3 * 3 �� 9���� ���ָӴϸ� ���� �� ������, ���� ������ ���ָӴϸ� ���� �ٿ� ��ġ�Ͽ� ���� ���� �� �ִ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4002, "�Ҹ����ڸ� ��� ä��� �̺�Ʈ NPC�� ã�ư� ��ǰ���� ��ȯ�� �� �ִ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4003, "�̺�Ʈ �Ⱓ: 2008�� 2�� 5��(ȭ) ~ 2008�� 2�� 19��(ȭ)"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}				
				else if (iEventIndex == TEVENT_RED_TREASUREBOX) // 2008 ���� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4010, "������ �������� �̺�Ʈ"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4011, "�̺�Ʈ �Ⱓ���� �ű� ĳ���͸� �����ϸ� 1ȸ�� ���Ͽ� ������ �������ڿ� ���Ҿ� ������ �������ڸ� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4012, "ó�� �����ϴ� 5Lv ������ �������ڴ� ĳ���� ������ 5������ �Ǿ��� ��, ��� �������� �η��ο��� ã�ư��� �� �� ������, ������ ���޹�ǰ�� ����ް� �˴ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4013, "5Lv ������ �������ڸ� ���� ���� �ܰ��� 12Lv ������ �������ڸ� �ް� �Ǹ�, ���� ������� 16������ �� ������ �� 3���� ������ �������ڸ� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S( 1866, "����ǰ" ), -1, 0xA3A1A3FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4014, "�÷�Ƽ�� ���̸����� �ູ (35���� ���Ͽ�)"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4015, "�÷�Ƽ�� ������ ��� (35���� ���Ͽ�)"));
					
					CTString str1, str2;
					str1.PrintF( _S(4016, "������Ʈ(%d��)"), 7);
					str2.PrintF( "%s + %s", str1, _pNetwork->GetItemName(2664));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, str2);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4017, "�̺�Ʈ �Ⱓ: 2008�� 2�� 14��(��) ~"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if( iEventIndex == TEVENT_SAKURA_2008 ) //2008 ���� �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4104, "���� �̺�Ʈ"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4105, "ȭâ�� ���� ���� �̺�Ʈ�� �ǽ��մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4106, "�̺�Ʈ �Ⱓ ���� ���͵鿡�Լ� ��� ȹ���Ͽ� �̺�Ʈ NPC�� �����縦 ã�ư��� ���� �������ڷ� ��ȯ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4107, "���� �������ڴ� ����Ŭ���Ͽ� ���� ����ǰ�� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4108, "������ü�, �һ�� ������, ��� ������, ����� ������, �ν���, ��ȫ ����, �� ����, ���� ��ȫ ����, ���� �� ����, �ӵ���� ������ ����ǰ���� ȹ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4109, "�̺�Ʈ �Ⱓ: 2008�� 3�� 27�� ~ 2008�� 4�� 10��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				// [080422: Su-won] EVENT_CHAOSBALL
				else if( iEventIndex == TEVENT_CHAOSBALL) //ī������ �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);

					CTString strReward;
					CTString strCount;
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4118, "���̸��� ���� �̺�Ʈ [ī���� ���� �����!]"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4119, "���̸��� �������� ��õ�ϴ� ��ǰ�� �����Ͻø� �� �ϳ��� ����! ī���� ���� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4120, "������ ��� ī���� ���� ��� ���������� �İߵǾ� �ִ� �̺�Ʈ ���������� ����������."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4121, "ī���� ���� ������ �ִ� ���� ������ �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4122, "������ ���� ���"), -1, 0xA3A1A3FF);
										
					strCount = pUIManager->IntegerToCommaString(1);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(2844), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);

					if (IsGamigo(g_iCountry) == TRUE)
					{
						strCount = pUIManager->IntegerToCommaString(1);
						strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(974), strCount);
						pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					}					

					strCount = pUIManager->IntegerToCommaString(1);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(1416), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);

					strCount = pUIManager->IntegerToCommaString(1);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(1288), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);

					strCount = pUIManager->IntegerToCommaString(1);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(85), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4123, "��ũ�� 3��(����/���/���) 1��Ʈ"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4124, "������ 3��(����ġ/��µ�/�����) 1��Ʈ"));

					strCount = pUIManager->IntegerToCommaString(10);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(45), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);

					strCount = pUIManager->IntegerToCommaString(10);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(724), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4125, "12���� ���� �ɸ�/Ǫ�� �ɸ� �� 5��"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4126, "12���� ���� ��/Ǫ�� �� �� 5��"));

					strCount = pUIManager->IntegerToCommaString(3);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(786), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);

					strCount = pUIManager->IntegerToCommaString(10);
					strReward.PrintF(_S(61, "%s %s��"), _pNetwork->GetItemName(556), strCount);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);

					CTString strNas;
					strNas.PrintF("%d", 10000);
					pUIManager->InsertCommaToString(strNas);
					strReward.PrintF(_S(836, "%s ����"), strNas);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, strReward);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4127, "�̺�Ʈ �Ⱓ: 2008�� 4�� 29�� ~"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_SUMMER_2008)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4153, "�̺�Ʈ 1. ��õ���� Ȯ��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4154, "�����̺�Ʈ �Ⱓ ���� ��õ���� ������ 60Lv ĳ���Ϳ��� ���� ã�� �ɴϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4155, "�̺�Ʈ 2. ������ ��������"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4156, "�̺�Ʈ �Ⱓ ���� �ű� ĳ���ʹ� ������ �������ڸ� �帳�ϴ�. ���ڸ� �̺�Ʈ ���������� �������� �ʺ����� ������ ����ǰ�� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4157, "�̺�Ʈ 3. ����뿩 ����"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4158, "�̺�Ʈ �Ⱓ ���� ���� �뿩������ 100������ �뿩�� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4159, "�̺�Ʈ 4. ����ī�带 ã�ƶ�"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(3606, "�̺�Ʈ �������� �Ҿ���� ī�带 ����ּ���. �ɷ��� ���� ��½����ִ� ������ �ɾ�帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4162, "�̺�Ʈ �Ⱓ: 2008�� 8�� 7�� ~ 8�� 28��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_HANARO)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4731, "[��ī�� 22,000 ĳ���� ���!]"),-1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4732, "�ű� �����ڿ��� 22,000ĳ�� ����� '�ʺ� ���� ��Ʈ' ��Ű���� �帳�ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4733, "'ALT + E'�� ���� �κ��丮�� Ȯ���Ͻø� ���� �ٷ� �̺�Ʈ �������� Ȯ���Ͻ� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4734, "* ���ο� ĳ���ͷ� �� ���� �ÿ��� �ߺ� ���޵˴ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4735, "* �ش� �̺�Ʈ �������� 35Lv ������ ��� �����ϸ� �ŷ� �� â���� �Ұ��Դϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_ADD_UP_AFRON_2009)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4766, "���� ���� �̺�Ʈ"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4767, "1. ��� ���� ����ġ 2�� �̺�Ʈ"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4768, "���� ������ ����Ͽ� ������ ������ �������� 2�ְ� ����ġ 2�� �̺�Ʈ�� �ǽ��մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4769, "2. �����м��� ���� ���ձ��ǰ"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4770, "�̹� ���������� ����Ͽ� ������ ��� ĳ���Ϳ��� ��Ű ���̸��� �ູ 5���� �����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4771, "���ǰ�� �̺�Ʈ �������� ���ؼ� �ѹ��� ���޹��� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4772, "�̺�Ʈ �Ⱓ: 2009�� 11�� 12�� ~ 11�� 26��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_QUARTER_FINAL_TOUR)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��Ʈī���� ���� ��ʸ�Ʈ, ���ѹα� 16�� ���� ��� �̺�Ʈ"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("���ѹα� ������ǥ������ ������ �����м����� ""�Ѱ�������"" ��尡 8������ �ƽ��� ��⸦ ������ �Ͽ����ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��� 4�� ���⿡ ���������� �ּ��� ���� ������ǥ���� ������ ����ϸ� ���ѹα� ��ī ���� ��� �е鿡�� ���� ������ �غ��߽��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("Ư���� ���� �����ð�, ���ѹα� ������ǥ�� ""�Ѱ�������"" ��带 ����� ���� ��ʸ�Ʈ ��⵵ ����ؼ� ���Ѻ� �ּ���~!"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("�� ������ ��� �������� �η����� ���� 12�� 17�Ϻ��� 1���ϰ� 1ȸ�� ���޹��� �� �ֽ��ϴ�.(����Ʈ)"), -1, 0xA3A1A3FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("�� �����Ǵ� ��� �����۵��� �̺�Ʈ ������ �ŷ�/��ȯ/â�� ������ ���� �ʽ��ϴ�."), -1, 0xA3A1A3FF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("�̺�Ʈ �Ⱓ: 2009�� 12�� 17�� ~ 12�� 24��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_GOLDWEEK)	// �Ϻ� �����ũ �̺�Ʈ
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4844, "Golden Week Event"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4845, "�����ũ�� �����Ͽ� '�����ũ ���ڸ� ã�ƶ�' �̺�Ʈ�� �����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4846, "�̺�Ʈ �Ⱓ ���ȿ��� ���̸��� ����� ��� ���͵��� �����ũ ��� ���ڸ� ����մϴ�. (�۽��� ���� ����)"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4847, "ȹ���� ���ڸ� �����ϸ� �����ϰ� �������� ȹ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4848, "�°����ũ ���ڴ� �ŷ�����/��ȯ/���λ���/���/â������ �Ұ����մϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4849, "�̺�Ʈ �Ⱓ: 2010.04.22 ~ 2010.05.13"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}

				// ($E_WC2010) [100513: selo] 2010 ���ư� ������ Event1 Ȳ�� �౸���� ��ƶ�
				else if (iEventIndex == TEVENT_WORLDCUP_2010)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4883, "���� ���� �̺�Ʈ - Event1. Ȳ�� �౸���� ��ƶ�!"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4884, "�� �������� ���� ""2010�⵵ ���ư� ������""�� ����Ͽ� ��Ʈ ī���� ������ ���� ""2010 ���� ���� �̺�Ʈ""�� �����մϴ�. "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4885, "�̺�Ʈ �Ⱓ���� ���̸��� ��� �������� 'Ʈ���̾ޱ� ��' NPC�� �����ϸ�, �̺�Ʈ NPC�� ����ϸ� '�౸��' �������� ȹ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4886, "ȹ���� '�౸��' �������� �� ������ �ִ� �� ���ο��� �������� �̺�Ʈ ���� ���������� ��ȯ�� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4887, "�ִ��� ���� �౸���� ȹ���Ͽ� ���� ���� �̺�Ʈ�� �����Ͻñ� �ٶ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4888, "�̺�Ʈ �Ⱓ: 2010.06.10 ~ 2010.07.01"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));					
				}
				// ($E_WC2010) [100514: selo] 2010 ���ư� ������ Event2 ��±� �����ϱ�
				else if (iEventIndex == TEVNET_WORLDCUP_2010_TOTO || iEventIndex == TEVNET_WORLDCUP_2010_TOTO_STATUS || iEventIndex == TEVNET_WORLDCUP_2010_TOTO_GIFT)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4916, "���� ���� �̺�Ʈ - Event2. ��±� �����ϱ�!"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4884, "�� �������� ���� ""2010�⵵ ���ư� ������""�� ����Ͽ� ��Ʈ ī���� ������ ���� ""2010 ���� ���� �̺�Ʈ""�� �����մϴ�. "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4917, "'���� ���� ����'���� ȹ���� �� ���� ���� �������� ������ ��� ������ �ִ� �̺�Ʈ ���� ����� ã�ư� ������."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4918, "�׸���, ������ ������� ������ ������ �����Ͽ� �����Ͻñ� �ٶ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4919, "��±����� �����Ͻ� �� ĳ���� ���� �ϳ��� ������ ������ �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4920, "�ڼ��� ������ �̺�Ʈ ���� ��� NPC�� ���� Ȯ���Ͻñ� �ٶ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _S(4888, "�̺�Ʈ �Ⱓ: 2010.06.10 ~ 2010.07.01"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));

					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}
				else if (iEventIndex == TEVENT_OCEANS_PROMOTION)
				{
					pUIManager->CloseMessageBoxL(MSGLCMD_EVENT_NOTICE);
					pUIManager->CreateMessageBoxL(_S(100, "�̺�Ʈ"), UI_NOTICE, MSGLCMD_EVENT_NOTICE);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��ĭ ��� �̺�Ʈ"));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��ĭ ������� ��ȥ�� ¡ǥ�� ��ƶ�!"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��ĭ����� ����� ���ݾ� �������� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��ĭ����� 8�� ������ �����Դϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("������� ��Ƽ� ��ĭ������� ����� ��ȥ���� ������ �ּ���."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��ȥ�� ¡ǥ�� ���ؿ� ��� ������ �ִ� �̺�Ʈ ���� ������� ��ȯ ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("��ȥ�� ¡ǥ�� ��ĭ����� ������ ���Ϳ��Լ� ���� �� �ֽ��ϴ�."));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, CTString(" "));
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, TRUE, _s("�̺�Ʈ �Ⱓ: 2010�� 9�� 16�� ~ 2010�� 9�� 30��"), -1, 0x6BD2FFFF);
					pUIManager->AddMessageBoxLString(MSGLCMD_EVENT_NOTICE, FALSE, _S(191, "Ȯ��"));
				}

				pData->DelFromNoticeList(nIndex, Notice::NOTICE_EVENT);
			}
			else
			{
				pUIManager->GetQuestAccept()->open( iNoticeIndex );

				pData->DelFromNoticeList(nIndex, Notice::NOTICE_QUEST);
			}
		}
		break;

	case UBET_ITEM:		// ���� ���� ��ư.
		{
			const int iStopChangeItem = 521;
			
			const int iIndex = nIndex;
			// ���� ���� ��ư.
			if( iIndex == iStopChangeItem && _pNetwork->MyCharacterInfo.eMorphStatus == CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION)
			{
				_pNetwork->MyCharacterInfo.eMorphStatus	= CNetworkLibrary::MyChaInfo::eMORPH_TRANSFORMATION_CONVERTING;
				_pNetwork->SendStopChange();
			}
		}
		break;
	case UBET_ACTION:	// [100322: selo] �׼� ��ư
		{
			const int iAffinity = 47;
			const int iIndex = nIndex;

			// ģȭ�� ��ư�� ��� ģȭ�� �������� ó���Ѵ�.
			if( iIndex == iAffinity )
			{
				// �޽��� �ڽ� �����.				
				pUIManager->CloseMessageBox(MSGCMD_AFFINITY_REWARD_NOTICE);
				
				int iNpcIndex = -1;
				CTString strMessage;
				CTString strAffinityList = "";
				CTString strNPCList = "";
				std::vector<int> vectorAffinityIndex;
				std::list<int>::const_iterator iter = _pNetwork->GetAffinityRewardNPCList().begin();

				CUIMsgBox_Info	MsgBoxInfo;
				MsgBoxInfo.SetMsgBoxInfo( _S(4842, "ģȭ�� �˸�"), UMBS_OK, UI_NOTICE, MSGCMD_AFFINITY_REWARD_NOTICE ); 

				while( iter != _pNetwork->GetAffinityRewardNPCList().end() )
				{
					iNpcIndex = *iter;
					CMobData* mob = CMobData::getData(iNpcIndex);
					strMessage.PrintF(_S(4843, "[%s] (��)�κ��� ģȭ�� ������ ���� �� �ֽ��ϴ�."), mob->GetName() );
					MsgBoxInfo.AddString(strMessage);

					++iter;
				}

				pUIManager->CreateMessageBox(MsgBoxInfo);
			}
		}
		break;
	}
}

void CUINoticeNew::Clear()
{
	m_bAttendanceEvent = FALSE;
}

void CUINoticeNew::initialize()
{
#if (defined(G_THAI))
	//nMaxNotice = 3;             //���� ��û���� 4���� �״�� �д�. modified by sam 101021
	nChargeBtnPos = BTN_SIZE+10; // wooss 050926 
	nHeight = BTN_SIZE * 3;
#endif

	m_pList = (CUIList*)findUI("list_notice");

	m_pGroup = new CUINoticeGroup;
	CUIManager::getSingleton()->LoadXML("NoticeInfo.xml", m_pGroup);

	m_pGroup->initialize();
	addChild((CUIBase*)m_pGroup);
	m_pGroup->postInit();
}

void CUINoticeNew::UpdateBtn()
{
	if (m_pList == NULL)
		return;

	m_pList->DeleteAllListItem();
	CUITooltipMgr::getSingleton()->setData(NULL);

	GameDataManager* pGame = GameDataManager::getSingleton();
	Notice*	pData = pGame->GetNotice();

	if (pData == NULL)
		return;

	CUIListItem* pTempItem = m_pList->GetListItemTemplate();
	CUIListItem* pItem = NULL;
	int nMax = 0, listcount = 0, BtnCount = 0;
	std::vector<Notice::sNoticeInfo>* pVec;
	Notice::eNoticeType NoticeType;

	CUIIcon* pIcon;
	CUIText* pText;

	// �� ���� �˾�.
	pVec = pData->GetNoticeList(Notice::NOTICE_NEWS_WEB);

	if (pVec != NULL)
	{
		nMax = pVec->size();

		if (nMax > 0)
		{
			listcount = m_pList->getListItemCount();

			if (BtnCount >= listcount)
			{
				m_pList->AddListItem(pTempItem->Clone());
				listcount = m_pList->getListItemCount();
			}

			pItem = (CUIListItem*)m_pList->GetListItem( BtnCount );

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if ( pIcon != NULL)
			{
				pIcon->clearIconData();
				pIcon->setData(UBET_QUEST, NEWS_WEB);
				pIcon->SetHighLightMode(pData->GetNoticeBlink(Notice::NOTICE_NEWS_WEB));

				CTString strTooltip;
				strTooltip.PrintF(_S(6423,"���ο� �˾� ������ ���� �մϴ�. Ŭ�� �Ͽ� ������ Ȯ���� �ּ���"));
				pIcon->setTooltipWidth(DEF_TOOLTIP_WIDTH);
				pIcon->setTooltip(strTooltip.str_String);

				CmdNoticeItemClick* pCmd = new CmdNoticeItemClick;
				pCmd->setData(this, Notice::NOTICE_NEWS_WEB, NEWS_WEB);
				pIcon->SetCommandUp(pCmd);
			}

			pText = (CUIText*)pItem->findUI("str_count");

			if ( pText != NULL)
			{
				pText->Hide(TRUE);
			}

			BtnCount++;
		}		
	}

	// ���� �˾�.
	pVec = pData->GetNoticeList(Notice::NOTICE_NEWS_NORMAL);

	if (pVec != NULL)
	{
		nMax = pVec->size();

		if (nMax > 0)
		{
			listcount = m_pList->getListItemCount();

			if (BtnCount >= listcount)
			{
				m_pList->AddListItem(pTempItem->Clone());
				listcount = m_pList->getListItemCount();
			}

			pItem = (CUIListItem*)m_pList->GetListItem( BtnCount );

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if ( pIcon != NULL)
			{
				pIcon->clearIconData();
				pIcon->setData(UBET_QUEST, NEWS_NORMAL);
				pIcon->SetHighLightMode(pData->GetNoticeBlink(Notice::NOTICE_NEWS_NORMAL));

				CTString strTooltip;
				strTooltip.PrintF(_S(6422,"���ο� �Ϲ� ������ ���� �մϴ�. Ŭ�� �Ͽ� ������ Ȯ���� �ּ���"));
				pIcon->setTooltipWidth(DEF_TOOLTIP_WIDTH);
				pIcon->setTooltip(strTooltip.str_String);

				CmdNoticeItemClick* pCmd = new CmdNoticeItemClick;
				pCmd->setData(this, Notice::NOTICE_NEWS_NORMAL, NEWS_NORMAL);
				pIcon->SetCommandUp(pCmd);
			}

			pText = (CUIText*)pItem->findUI("str_count");

			if ( pText != NULL)
			{
				pText->Hide(TRUE);
			}

			BtnCount++;
		}		
	}

	int nIndex[CUINoticeGroup::GROUP_MAX] = { EVENT_START_INDEX, DEF_QUESTONCE_ICON_IDX, DEF_QUESTUNLIMITED_ICON_IDX };
	SBYTE sbType[CUINoticeGroup::GROUP_MAX] = { -1, 0, 0 };

	// event, quest
	for( int i = 0; i < CUINoticeGroup::GROUP_MAX; i++)
	{
		switch (i)
		{
		case CUINoticeGroup::EVENT:
			NoticeType = Notice::NOTICE_EVENT;
			break;
		case CUINoticeGroup::QUEST_ONCE:
			NoticeType = Notice::NOTICE_QUEST;			
			break;
		case CUINoticeGroup::QUEST_UNLIMITED:
			NoticeType = Notice::NOTICE_QUEST_UNLIMITED;
			break;
		default:
			continue;
		}

		pVec = pData->GetNoticeList(NoticeType);

		if (pVec != NULL)
		{
			nMax = pVec->size();

			if (nMax > 0)
			{
				listcount = m_pList->getListItemCount();

				if (BtnCount >= listcount)
				{
					m_pList->AddListItem(pTempItem->Clone());
					listcount = m_pList->getListItemCount();
				}

				pItem = (CUIListItem*)m_pList->GetListItem( BtnCount );
				pIcon = (CUIIcon*)pItem->findUI("icon_item");

				if ( pIcon != NULL)
				{
					pIcon->clearIconData();					
					pIcon->setData(UBET_QUEST, nIndex[i]);
					pIcon->SetHighLightMode(pData->GetNoticeBlink(NoticeType));
					pIcon->SetCustomTooltip(TRUE);
				}

				pText = (CUIText*)pItem->findUI("str_count");

				if ( pText != NULL)
				{
					CTString strCount;
					strCount.PrintF("%d", pVec->size());

					pText->SetText(strCount);
				}
				
				int nBtnPosY = (GetHeight() / 2) - (m_pList->GetHeight() / 2);
				int nY = nBtnPosY + (BTN_SIZE / 2) - (m_pGroup->GetHeight() / 2) + (BTN_SIZE * BtnCount);
								
				if (pIcon != NULL)
				{
					CmdNoticeInfoOpen* pCmd1 = new CmdNoticeInfoOpen;
					pCmd1->setData(m_pGroup, (CUINoticeGroup::GROUP_TYPE)i, 0, nY);
					pIcon->SetCommandEnter(pCmd1);

					CmdNoticeInfoClose* pCmd2 = new CmdNoticeInfoClose;
					pCmd2->setData(m_pGroup);
					pIcon->SetCommandLeave(pCmd2);

					CmdNoticeBtnClick* pCmd3 = new CmdNoticeBtnClick;
					pCmd3->setData(this, (CUINoticeGroup::GROUP_TYPE)i, m_pGroup, 0, nY);
					pIcon->SetCommandUp(pCmd3);
				}

				BtnCount++;
			}
			else
			{
				if (m_pGroup != NULL)
					m_pGroup->Close();
			}				
		}
	}
	
	// ģȭ��
	pVec = pData->GetNoticeList(Notice::NOTICE_AFFINITY);

	if (pVec != NULL)
	{
		nMax = pVec->size();

		if (nMax > 0)
		{
			listcount = m_pList->getListItemCount();

			if (BtnCount >= listcount)
			{
				m_pList->AddListItem(pTempItem->Clone());
				listcount = m_pList->getListItemCount();
			}

			pItem = (CUIListItem*)m_pList->GetListItem( BtnCount );

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if ( pIcon != NULL)
			{
				pIcon->clearIconData();
				pIcon->setData(UBET_ACTION, iAffinityRewardNotice);
				pIcon->SetHighLightMode(pData->GetNoticeBlink(Notice::NOTICE_AFFINITY));

				CTString strTooltip;
				strTooltip.PrintF(_S(4842,"ģȭ�� �˸�"));
				pIcon->setTooltip(strTooltip.str_String);

				CmdNoticeItemClick* pCmd = new CmdNoticeItemClick;
				pCmd->setData(this, Notice::NOTICE_AFFINITY, iAffinityRewardNotice);
				pIcon->SetCommandUp(pCmd);
			}

			pText = (CUIText*)pItem->findUI("str_count");

			if ( pText != NULL)
			{
				pText->Hide(TRUE);
			}

			BtnCount++;
		}		
	}

	// ���� ����
	pVec = pData->GetNoticeList(Notice::NOTICE_POLYMOPH);

	if (pVec != NULL)
	{
		nMax = pVec->size();

		if (nMax > 0)
		{
			listcount = m_pList->getListItemCount();

			if (BtnCount >= listcount)
			{
				m_pList->AddListItem(pTempItem->Clone());
				listcount = m_pList->getListItemCount();
			}

			pItem = (CUIListItem*)m_pList->GetListItem( BtnCount );

			pIcon = (CUIIcon*)pItem->findUI("icon_item");

			if ( pIcon != NULL)
			{
				pIcon->clearIconData();
				pIcon->setData(UBET_ITEM, iStopChangeItem);
				pIcon->SetHighLightMode(pData->GetNoticeBlink(Notice::NOTICE_POLYMOPH));

				CTString strTooltip;
				strTooltip.PrintF(_S(5305, "���� ����"));
				pIcon->setTooltip(strTooltip.str_String);

				CmdNoticeItemClick* pCmd = new CmdNoticeItemClick;
				pCmd->setData(this, Notice::NOTICE_POLYMOPH, iStopChangeItem);
				pIcon->SetCommandUp(pCmd);
			}

			pText = (CUIText*)pItem->findUI("str_count");

			if ( pText != NULL)
			{
				pText->Hide(TRUE);
			}

			BtnCount++;
		}		
	}

	m_pList->UpdateList();	
}

//////////////////////////////////////////////////////////////////////////

void CUINoticeNew::ShowAttendanceMessage( SLONG attendanceRate )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	pUIManager->CloseMessageBox(MSGCMD_ATTENDANCE);

	CUIMsgBox_Info	MsgBoxInfo;
	CTString strTemp;

	MsgBoxInfo.m_nColorBoxCount =1;

	MsgBoxInfo.SetMsgBoxInfo( _S( 100, "�̺�Ʈ" ), UMBS_OK , UI_NOTICE, MSGCMD_ATTENDANCE, attendanceMsgBoxWidth );

	MsgBoxInfo.AddString( _S( 5334, "�⼮ üũ�ϰ�! ���� �ް�!" ), 0xE18600FF ,TEXT_CENTER);
	MsgBoxInfo.AddString( _s( " " ));
	MsgBoxInfo.AddString( _S( 5335, "2011�� ���ο� ���ظ� �¾� ���� ��� �⼮ �̺�Ʈ�� �����մϴ�." ));
	MsgBoxInfo.AddString( _S( 5336, "�̺�Ʈ �Ⱓ ���� ��(����)�� ������ ��� ĳ���Ͱ� ������ ���� �⼮ �� ū ������ ���� �� �ֽ��ϴ�." ));
	MsgBoxInfo.AddString( _S( 5337, "�����⼮���ε� ū ������ ���� �� ������ ���� �⼮ �̺�Ʈ���� ���� ���ʽÿ�." ));
	MsgBoxInfo.AddString( _s( " " ));
	MsgBoxInfo.AddString( _S( 5338, "�� ������ ��ü �⼮���� ���� ������ �޶����ϴ�." ));
	MsgBoxInfo.AddString( _S( 5339, "�⼮üũ �������� ��þƮ Ȯ���� ���� ȿ���� ���� �� �ֽ��ϴ�." ));
	MsgBoxInfo.AddString( _s( " " ));
	MsgBoxInfo.AddString( _S( 5340, "���� ���� �ð�: PM 19:00����" ), 0xF2F2F2FF, TEXT_CENTER);
	MsgBoxInfo.AddString( _S( 5341, "�̺�Ʈ �Ⱓ: 2011�� 2�� �� ~ 2�� �ϱ���" ), 0xF2F2F2FF, TEXT_CENTER);
	MsgBoxInfo.AddString( _s( " " ));
	
	strTemp.PrintF(_S( 5342, "��ü �⼮�� %d%" ), attendanceRate);
	MsgBoxInfo.AddString( strTemp, 0xF2F2F2FF, TEXT_CENTER);
	MsgBoxInfo.m_nMaxRow +=1;

	pUIManager->CreateMessageBox( MsgBoxInfo );

	float fEnvRate = attendanceRate / 100.0f;
	int iPosY = MSGBOX_MESSAGE_OFFSETY + MsgBoxInfo.m_nMaxRow * _pUIFontTexMgr->GetLineHeight();

	pUIManager->GetMessageBox(MSGCMD_ATTENDANCE)->SetColorBox(0, 5, WRect(30, iPosY, 30 + ( colorBarWidth * fEnvRate ), iPosY+9));	
	
	m_bAttendanceEvent = TRUE;
}

void CUINoticeNew::ShowAttendanceNotice( SLONG attendanceRate, SLONG enchantRate, SLONG expSpRate )
{
	CTString strTemp;
	strTemp.PrintF( _S(5343, "�̺�Ʈ�� ���� ���� ��þƮ Ȯ���� %d%, ���õ��� ����ġ�� %d%�߰� ȹ���Ͻ� �� �ֽ��ϴ�."),
					enchantRate, expSpRate );

	_UIAutoHelp->SetGMNotice ( strTemp, 0xFFFF40FF );
	_pNetwork->ClientSystemMessage( strTemp, SYSMSG_NOTIFY );

	// ����...
	PCStartEffectGroup("squid", _pNetwork->MyCharacterInfo.index);
}

void CUINoticeNew::ShowUsedPartyItemNotice( const INDEX& nItemIdx, const CTString& strNickName )
{
	CTString strTemp;
	strTemp.PrintF( _S( 5395, "%s�Բ��� %s��� ��Ĩ�ϴ�."), strNickName, _pNetwork->GetItemDesc( nItemIdx ) );
	_UIAutoHelp->SetGMNotice ( strTemp, 0xA8A8F8FF );
}

void CUINoticeNew::ShowRoyalRumbleNotice(const UBYTE uType )
{
	CTString strTemp;
	if( uType == 0 )
	{
		strTemp.PrintF( _S( 5396, "�ξⷳ�� ��û�� ���۵Ǿ����ϴ�.") );
	}
	else if( uType == 1 )
	{
		strTemp.PrintF( _S( 5397, "�ξⷳ�� ��û�ð��� 5�� ���ҽ��ϴ�.") );
	}
	else if( uType == 2 )
	{
		strTemp.PrintF( _S(5398, "�ξⷳ�� ��û�� ����Ǿ����ϴ�.") );
	}
	
	_UIAutoHelp->SetGMNotice( strTemp, 0xFFFF40FF );
	_pNetwork->ClientSystemMessage( strTemp, SYSMSG_NOTIFY );
}

void CUINoticeNew::ShowRoyalRumbleWinner(const INDEX& iLevel, const CTString& strNickName )
{
	CTString strTemp;
	CTString strType;
	if( iLevel == 0 )
	{
		strType = _S(5399, "���� ������");
	}
	else if( iLevel == 1 )
	{
		strType = _S(5400, "���� ������");
	}
	else if( iLevel == 2 )
	{
		strType = _S(5401, "��� ������");
	}

	strTemp.PrintF( _S( 5402, "%s�Բ��� %s �������� ����ϼ̽��ϴ�!" ), strNickName, strType );
	_UIAutoHelp->SetGMNotice( strTemp, 0xFFFF40FF );
}

void CUINoticeNew::ShowRoyalRumbleNextPlayerTime( const INDEX iLeftTime )
{
	CTString strTemp;
	strTemp.PrintF( _S( 5403, "���� ���� ����ð��� %d�� ���ҽ��ϴ�." ), iLeftTime );
	_UIAutoHelp->SetGMNotice( strTemp, 0xFFFF40FF );
}

void CUINoticeNew::SetIconBlink( CUINoticeGroup::GROUP_TYPE eType, bool bClick )
{
	if (m_pGroup == NULL || m_pList == NULL)
		return;

	int nIndex[CUINoticeGroup::GROUP_MAX] = { EVENT_START_INDEX, DEF_QUESTONCE_ICON_IDX, DEF_QUESTUNLIMITED_ICON_IDX };
	Notice::eNoticeType NoticeType;

	switch(eType)
	{
	case CUINoticeGroup::EVENT:
		NoticeType = Notice::NOTICE_EVENT;
		break;
	case CUINoticeGroup::QUEST_ONCE:
		NoticeType = Notice::NOTICE_QUEST;			
		break;
	case CUINoticeGroup::QUEST_UNLIMITED:
		NoticeType = Notice::NOTICE_QUEST_UNLIMITED;
		break;
	}
	

	int listcount = m_pList->getChildCount();
	CUIListItem* pItem = NULL;
	CUIIcon* pIcon = NULL;

	for (int i = 0; i < listcount; i++)
	{
		pItem = (CUIListItem*)m_pList->GetListItem(i);

		if (pItem == NULL)
			continue;

		pIcon = (CUIIcon*)pItem->findUI("icon_item");

		if (pIcon != NULL)
		{
			if (nIndex[eType] == pIcon->getIndex() && bClick == true)
			{
				m_pGroup->SetFixUi(!m_pGroup->GetFixUi());
				pIcon->SetHighLightMode(false);
				pIcon->SetHighLight(m_pGroup->GetFixUi());
				Notice* pdata = GAMEDATAMGR()->GetNotice();
				if (pdata != NULL)
					pdata->SetNoticeBlink(NoticeType, false);
			}
			else
			{
				if (pIcon->GetHighLightMode() == false)
					pIcon->SetHighLight(false);
			}
		}
	}
}

void CUINoticeNew::SetIconBlink( Notice::eNoticeType eType, bool bBlink )
{
	if (m_pList == NULL)
		return;

	GameDataManager* pGame = GameDataManager::getSingleton();
	Notice*	pData = pGame->GetNotice();

	if (pData == NULL)
		return;
	
	std::vector<Notice::sNoticeInfo>* pVec;

	CUIListItem* pItem = NULL;
	CUIIcon* pIcon = NULL;

	int i = 0, nBtnCount = 0;

	// ����Ʈ�� �߰��� ��ư ������ ���Ѵ�.
	for (; i < eType + 1; ++i)
	{
		pVec = pData->GetNoticeList((Notice::eNoticeType)i);

		if (pVec == NULL)
			continue;

		if (pVec->size() <= 0)
			continue;

		++nBtnCount;
	}
	
	if (nBtnCount <= 0)
		return;

	pItem = (CUIListItem*)m_pList->GetListItem(nBtnCount - 1);

	if (pItem == NULL)
		return;

	pIcon = (CUIIcon*)pItem->findUI("icon_item");
	
	if (pIcon == NULL)
		return;

	pData->SetNoticeBlink(eType, bBlink);
	pIcon->SetHighLightMode(bBlink);
}

//////////////////////////////////////////////////////////////////////////

CUINoticeGroup::CUINoticeGroup()
	: m_pNoticeList(NULL)
	, m_bFixUi(false)
	, m_eType(NONE_GROUP)
	, m_pScroll(NULL)
{
	setInherit(false);
}

CUINoticeGroup::~CUINoticeGroup()
{

}

CUIBase* CUINoticeGroup::Clone()
{
	CUINoticeGroup* pGroup = new CUINoticeGroup(*this);

	if (pGroup == NULL)
		return NULL;

	CUIBase::CloneChild(pGroup);

	return (CUIBase*)pGroup;
}

void CUINoticeGroup::initialize()
{
	m_pNoticeList = (CUIList*)findUI("list_notice");
	m_pstrTitle = (CUIText*)findUI("str_title");
	m_pScroll = (CUIScrollBar*)findUI("list_scroll");
}

void CUINoticeGroup::UpdateSlot()
{
	if (m_pNoticeList == NULL)
		return;

	m_pNoticeList->DeleteAllListItem();

	CUIManager* pUIMgr = CUIManager::getSingleton();
	GameDataManager* pGame = GameDataManager::getSingleton();
	UtilHelp* pHelp = UtilHelp::getSingleton();

	Notice*	pData = pGame->GetNotice();
	
	if (pData == NULL)
		return;

	CUIListItem* pTempItem = m_pNoticeList->GetListItemTemplate();
	CUIListItem* pItem = NULL;
	int listcount = 0;
	int i = 0;
	std::vector<Notice::sNoticeInfo>* pVec = NULL;
	Notice::eNoticeType eType = Notice::NOTICE_QUEST;;

	CUIText* pText;
	CTString strName;

	switch(m_eType)
	{
	case QUEST_ONCE:
		pVec = pData->GetNoticeList(Notice::NOTICE_QUEST);
		break;
	case QUEST_UNLIMITED:
		pVec = pData->GetNoticeList(Notice::NOTICE_QUEST_UNLIMITED);
		break;

	case EVENT:
		pVec = pData->GetNoticeList(Notice::NOTICE_EVENT);
		eType = Notice::NOTICE_EVENT;
		break;
	}

	if (pVec == NULL)
		return;

	listcount = m_pNoticeList->getListItemCount();

	std::vector<Notice::sNoticeInfo>::const_iterator end = pVec->end();
	std::vector<Notice::sNoticeInfo>::iterator it = pVec->begin();

	for (i = 0; it != end; i++, it++)
	{
		if (i >= listcount)
		{
			m_pNoticeList->AddListItem(pTempItem->Clone());
			listcount = m_pNoticeList->getListItemCount();
		}

		pItem = (CUIListItem*)m_pNoticeList->GetListItem( i );

		pText = (CUIText*)pItem->findUI("str_name");

		if (pText != NULL)
		{
			switch(m_eType)
			{
			case QUEST_ONCE:
				{
					CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData((*it).iIndex));
					strName.PrintF( _S( 559, "%s ����Ʈ(1ȸ�� ���డ��)" ), QuestDD.GetName());
				}				
				break;

			case QUEST_UNLIMITED:
				{
					CQuestDynamicData QuestDD(CQuestSystem::Instance().GetStaticData((*it).iIndex));
					strName.PrintF( "%s", QuestDD.GetName());
				}				
				break;

			case EVENT:
				{
					int nIdx;
					int nString;
					if ((*it).iIndex >= PET_QUEST_HORSE_INDEX)
						nIdx = (*it).iIndex;
					else
						nIdx = (*it).iIndex - EVENT_START_INDEX;
					
					if (CTEventString::IsValidEvent(nIdx) == TRUE)
					{
						nString = CTEventString::GetTEventName(nIdx);

						if (nString > 0)
						{
							strName = pUIMgr->GetString(nString);
							break;
						}						
					}
					CTString strTemp;
					strTemp.PrintF("%d",i);
					strName = pUIMgr->GetString(100) + strTemp;
				}				
				break;
			}
			
			strName = pHelp->GetCalcStringEllipsis(strName, pText->GetWidth(), "...");
			pText->SetText(strName);

			CmdItemMouseEvent* pCmdOver = new CmdItemMouseEvent;
			pCmdOver->setData(this, pItem, 0xFF6107FF);
			pText->SetCommandEnter(pCmdOver);

			CmdItemMouseEvent* pCmdLeave = new CmdItemMouseEvent;
			pCmdLeave->setData(this, pItem, 0xFFFFFFFF);
			pText->SetCommandLeave(pCmdLeave);

			CmdNoticeItemClick* pCmd = new CmdNoticeItemClick;
			pCmd->setData(pUIMgr->GetNotice(), eType, (*it).iIndex);
			pText->SetCommandUp(pCmd);
		}
	}

	m_pScroll->SetCurItemCount(i);
	m_pNoticeList->UpdateList();
}

void CUINoticeGroup::Open(GROUP_TYPE eType, int x, int y)
{
	if(IsVisible() != FALSE && m_eType == eType )
		return;

	m_bFixUi = false;
	CUIManager::getSingleton()->GetNotice()->SetIconBlink(EVENT, false);
	SetGroup(eType);
	SetPos(x, y);
	UpdateSlot();

	SetVisible(TRUE);
	Hide(FALSE);
}

void CUINoticeGroup::Close()
{
	SetVisible(FALSE);
	Hide(TRUE);
}

void CUINoticeGroup::SetStringColor( CUIListItem* pItem, COLOR col /*= 0xFFFFFFFF*/ )
{
	if (pItem == NULL)
		return;

	int nMax = pItem->getChildCount();
	CUIText* pTex;

	for (int i = 0; i < nMax; i++)
	{
		pTex = (CUIText*)pItem->getChildAt(i);

		if ( pTex != NULL)
		{
			pTex->setFontColor(col);
		}
	}
}

void CUINoticeGroup::SetGroup( GROUP_TYPE eType /*= QUEST_ONCE*/ )
{
	m_eType = eType;

	if (m_pstrTitle != NULL)
	{
		CTString strTitle = CTString("");

		switch(eType)
		{
		case EVENT:
			strTitle.PrintF(_S(100, "�̺�Ʈ"));
			break;
		case QUEST_ONCE:
			strTitle.PrintF(_S(99, "����Ʈ"));
			break;
		case QUEST_UNLIMITED:
			strTitle.PrintF(_S(6274, "�ݺ� ����Ʈ"));
			break;
		}

		m_pstrTitle->SetText(strTitle);
	}
}