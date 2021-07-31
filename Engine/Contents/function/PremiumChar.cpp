#include "StdH.h"
#include "PremiumChar.h"
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Auction.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIInvenCashBag.h>
#include <Engine/Contents/Base/ExpressSystem.h>
#include <Engine/Contents/function/PetFreeUI.h>
#include <Engine/Interface/UIMessenger.h>
#include <Engine/Interface/UIRadar.h>
#include <Engine/Contents/function/SimplePlayerInfoUI.h>
#include <Engine/Contents/Base/UIDurability.h>

CPremiumChar::CPremiumChar()
	: m_sbPmType(0)
	, m_nTeleportCount(0)
	, m_nExpireTime(0)
{
}

CPremiumChar::~CPremiumChar()
{

}

void CPremiumChar::Clear()
{
	m_sbPmType = PREMIUM_CHAR_TYPE_NONE;
	m_nTeleportCount = 0;
	m_nExpireTime = 0;

	// �����̾� ĳ���� ���� ������Ʈ
	PremiumBenefit(false);
	UIMGR()->GetSimplePlayerInfo()->SetPremiumChar(m_sbPmType);
}

void CPremiumChar::updateInfo( char type, int nCount, int nTime )
{
	m_sbPmType = type;
	m_nTeleportCount = nCount;
	m_nExpireTime = nTime;

	UIMGR()->GetSimplePlayerInfo()->SetPremiumChar(type);

	bool bUse = false;

	switch(m_sbPmType)
	{
	case PREMIUM_CHAR_TYPE_NONE:
		bUse = false;
		break;
	case PREMIUM_CHAR_TYPE_FIRST:
		bUse = true;
		break;
	}

	// �����̾� ĳ���� ���� ������Ʈ
	PremiumBenefit(bUse);	
}

void CPremiumChar::RecvPremiumCharMsg( CNetworkMessage* istr )
{
#ifdef PREMIUM_CHAR
	pTypeBase* pPacket = reinterpret_cast<pTypeBase*>(istr->GetBuffer());
	int errorType = 0;

	switch(pPacket->subType)
	{
	case MSG_SUB_PREMIUM_CHAR_USE_ITEM:
		{
			ResponseClient::premiumChar_UseItem* pErr = reinterpret_cast<ResponseClient::premiumChar_UseItem*>(istr->GetBuffer());
			errorType = pErr->result;
		}		
		break;

	case MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR:
		{
			ResponseClient::premiumChar_JumpToChar* pErr = reinterpret_cast<ResponseClient::premiumChar_JumpToChar*>(istr->GetBuffer());
			errorType = pErr->result;
		}		
		break;

	default:
		return;
	}

	CTString strErrorMsg = CTString("");

	switch(errorType)
	{
	case ERR_PREMIUM_CHAR_NONE:					// ����
		{
			if (pPacket->subType == MSG_SUB_PREMIUM_CHAR_USE_ITEM)
				strErrorMsg.PrintF( _S( 6324, "%s ���� �����̾� ������ ���� �Ǿ����ϴ�.") , _pNetwork->MyCharacterInfo.name);
			else if (pPacket->subType == MSG_SUB_PREMIUM_CHAR_JUMP_TO_CHAR)
				strErrorMsg.PrintF( _S( 395, "�̵��� �Ϸ�Ǿ����ϴ�."));
		}		
		break;
	case ERR_PREMIUM_CHAR_ALREADY_USED:			// �̹� �����
		strErrorMsg.PrintF(_S( 6322, "�����̾� ������ �̹� ����Ǿ� �־� ��� �� �� �����ϴ�."));
		break;
	case ERR_PREMIUM_CHAR_NOT_FOUNC_CHAR:		// ���� ����� ã�� �� ����
		strErrorMsg.PrintF(_S( 4523, "����� ã�� �� �����ϴ�."));
		break;
	case ERR_PREMIUM_CHAR_INVALID_JUMPING_POSITION:		// ������ �� ���� ��ġ
		strErrorMsg.PrintF(_S( 2127,  "���� �̵� ���� ������ �ƴմϴ�.(�ٸ� ���̰ų� �۽��δ��������� ����� �� �����ϴ�.)" )); 
		break;
	case ERR_PREMIUM_CHAR_MAX_JUMP_COUNT:		// ���μ��� �ʰ���
		strErrorMsg.PrintF(_S( 6345, "���� �̵� ������ Ƚ���� ��� ����Ͽ����ϴ�."));
		break;
	case ERR_PREMIUM_CHAR_NOT_PREMIUM_CHAR:		// �����̾� ĳ���� ���°� �ƴ�
		strErrorMsg.PrintF(_S( 6327, "�����̾� ĳ���� ���� ����"));
		break;
	case ERR_PREMIUM_CHAR_FRIEND_IS_OFFLINE:
		strErrorMsg.PrintF(_S( 1628, "�ش� �ɸ��Ͱ� ���� ���� �ʰų� [��������] �����Դϴ�."));
		break;
	case ERR_PREMIUM_CHAR_NOT_JUMP_STATUS:
		strErrorMsg.PrintF( _S( 415, "�̵��� �� ���� �����Դϴ�." ));
		break;
	}

	_pNetwork->ClientSystemMessage(strErrorMsg, SYSMSG_ERROR);
#endif	//	PREMIUM_CHAR
}

void CPremiumChar::PremiumBenefit( bool bUse )
{
	CUIManager* pUImgr = CUIManager::getSingleton();

	// ���̴� ������Ʈ
	pUImgr->GetRadar()->SetPremiumCharBenefit(bUse);

	// �� ��Ȱ ���� ��� ����
	pUImgr->GetPetFree()->SetPremiumBenefit(bUse);

	// ���̸��� ���� 1 ��� ����
	CUIInvenCashBag* pCash = ((CUIInvenCashBag*)pUImgr->GetUI(UI_INVEN_CASH1));

	if (pCash != NULL)
		pCash->SetPremiumBenefit(bUse);

	// �ŷ����� ������ 0
	Auction* pAuction = GAMEDATAMGR()->GetAuction();

	if (pAuction != NULL)
		pAuction->SetPremiumBenefit(bUse);

	// �ź��� ���� ���� ��� ����
	ExpressSystem* pExpress = GAMEDATAMGR()->GetExpressData();

	if (pExpress != NULL)
		pExpress->SetPremiumBenefit(bUse);

	// ģ������ �ڷ���Ʈ ��� ����
	pUImgr->GetMessenger()->SetPremiumBenefit(bUse);

	// ������ ���� UI ���� ��밡��
	pUImgr->GetDurability()->SetPremiumBenefit(bUse);
	// ��񷹺� 3 ���Ҵ� CUITooltip::SetLevelInfo() �ȿ��� ó��
	// ���������� ��� ���� �ܵ�
	// ����ġ �� SP ������ ��� ���� �ܵ�
}
