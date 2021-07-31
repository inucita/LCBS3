#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIDurability.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Contents/Base/Durability.h>
#include <Engine/Loading.h>

#define RECOVER_ITEM	10128 // �ź��� ��ġ
#define DURABILITY_ITEM 10127 // �ź��� ����
#define NAS_CHARGE	   (19991)
#define DEF_END_TIME	2

class CmdDurabilityClose : public Command
{
public:
	CmdDurabilityClose() : m_pWnd(NULL) {}
	void setData( CUIDurability* pWnd ) { m_pWnd = pWnd; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->Close();
	}
private:
	CUIDurability* m_pWnd;
};

class CmdDurabilityBtn : public Command
{
public:
	CmdDurabilityBtn() : m_pWnd(NULL), m_eType(CUIDurability::eFIRST) {}
	void setData( CUIDurability* pWnd, CUIDurability::eBTN_TYPE eType ) { m_pWnd = pWnd; m_eType = eType; }
	void execute()
	{
		if( m_pWnd )
			m_pWnd->PressBtn(m_eType);
	}
private:
	CUIDurability* m_pWnd;
	CUIDurability::eBTN_TYPE m_eType;
};

CUIDurability::CUIDurability(void)
	: m_bDrag(false)
	, m_pDrag(NULL)
	, m_nOriX(0), m_nOriY(0)
	, m_pBtnClose(NULL)
	, m_pStrTitle(NULL)
	, m_pTbStateGuid(NULL)
	, m_pImgRecover(NULL)
	, m_pPgBar(NULL)
	, m_eOpenType(eDURABILITY)
	, m_eState(eNONE)
	, m_nMaterialCount(0)
	, m_llNas(0)
	, m_uStartTime(0)
	, m_uEndTime(DEF_END_TIME)
	, m_eEventType(eFIRST)
	, m_bProgress(FALSE)
	, m_nMobIndex(-1)
	, m_fNpcPosX(0.0f)
	, m_fNpcPosZ(0.0f)
{
	int i;
	for (i = 0; i < eICON_MAX; i++)
	{
		m_pIcon[i] = NULL;
	}
	for (i = 0; i < eBTN_MAX; i++)
	{
		m_pBtn[i] = NULL;
	}
	
	setInherit(false);
}

CUIDurability::~CUIDurability(void)
{
	Destroy();
}

void CUIDurability::Open(eOPEN_TYPE eType, int iMobVirIdx, FLOAT fX, FLOAT fZ)
{
	CUIManager* pUIMgr = CUIManager::getSingleton();

	// If inventory is already locked
	if (pUIMgr->GetInventory()->IsLocked() == TRUE ||
		pUIMgr->GetInventory()->IsLockedArrange() == TRUE)
	{
		pUIMgr->GetInventory()->ShowLockErrorMessage();
		return;
	}

	if (IsVisible() != FALSE)
		return;

	if (eType < eDURABILITY || eType >= eOPEN_MAX)
		return;

	m_eOpenType = eType;

	SetUI();
	
	SetVisible(TRUE);
	Hide(FALSE);

	SetPos( m_rcOriginPos.Left, m_rcOriginPos.Top );

	m_nMobIndex = iMobVirIdx;
	m_fNpcPosX = fX;
	m_fNpcPosZ = fZ;
	
#ifdef	DURABILITY
	pUIMgr->RearrangeOrder(UI_DURABILITY, TRUE);
	pUIMgr->GetInventory()->Lock(TRUE, TRUE, LOCK_DURABILITY);
#endif	//	UI_DURABILITY
}

void CUIDurability::Close()
{
	SetVisible(FALSE);
	Hide(TRUE);

	ClearIcon();

	m_bProgress = FALSE;
	m_uStartTime = 0;

	m_nMobIndex = -1;
	m_fNpcPosX = 0.0f;
	m_fNpcPosZ = 0.0f;

#ifdef	DURABILITY
	CUIManager* pUIMgr = CUIManager::getSingleton();
	pUIMgr->RearrangeOrder(UI_DURABILITY, FALSE);
	pUIMgr->GetInventory()->Lock(FALSE, FALSE, LOCK_DURABILITY);
#endif	//	UI_DURABILITY
}

void CUIDurability::initialize()
{
	// img
	m_pImgRecover = (CUIImage*)findUI("img_recover");

	if (m_pImgRecover != NULL)
		m_pImgRecover->Hide(TRUE);

	// close btn
	m_pBtnClose = (CUIButton*)findUI("btn_close");

	if (m_pBtnClose != NULL)
	{
		CmdDurabilityClose* pCmd = new CmdDurabilityClose;
		pCmd->setData(this);
		m_pBtnClose->SetCommandUp(pCmd);
	}

	// drag area
	m_pDrag = (CUIImage*)findUI("img_drag");

	// title text
	m_pStrTitle = (CUIText*)findUI("str_title");
	
	// icon
	CTString strStringIcon[eICON_MAX] = { "icon_item", "icon_material", "icon_result"};

	int		i;

	for (i = 0; i < eICON_MAX; i++)
	{
		m_pIcon[i] = (CUIIcon*)findUI(strStringIcon[i]);

		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}

	// guid
	m_pTbStateGuid = (CUITextBox*)findUI("tb_state");

	// progress bar
	m_pPgBar = (CUIProgressBar*)findUI("pb_progress");

	if (m_pPgBar != NULL)
		m_pPgBar->OffsetProgressPos(0);

	// Btn
	CTString strStringBtn[eBTN_MAX] = { "btn_first", "btn_second" };

	for (i = 0; i < eBTN_MAX; i++)
	{
		m_pBtn[i] = (CUIButton*)findUI(strStringBtn[i]);

		CmdDurabilityBtn* pCmd = new CmdDurabilityBtn;
		pCmd->setData(this, (CUIDurability::eBTN_TYPE)i);
		m_pBtn[i]->SetCommandUp(pCmd);
	}
}

void CUIDurability::OnUpdate( float fDeltaTime, ULONG ElapsedTime )
{
	// Check distance
	FLOAT	fDiffX = _pNetwork->MyCharacterInfo.x - m_fNpcPosX;
	FLOAT	fDiffZ = _pNetwork->MyCharacterInfo.z - m_fNpcPosZ;

	if( m_bPremiumChar == false && (fDiffX * fDiffX + fDiffZ * fDiffZ > UI_VALID_SQRDIST) )
	{
		Close();
		return;
	}

	CalcProgressTimer();
}

WMSG_RESULT CUIDurability::OnKeyMessage( MSG* pMsg )
{
	if (IsFocused() == FALSE)
		return WMSG_FAIL;

	if( pMsg->wParam == VK_ESCAPE )
	{
		Close();
		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CUIDurability::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	if( m_pDrag && m_pDrag->IsInside(x, y))
	{
		m_bDrag = true;
		m_nOriX = x;
		m_nOriY = y;
	}
#ifdef	DURABILITY
	CUIManager::getSingleton()->RearrangeOrder(UI_DURABILITY, TRUE);
#endif	//	UI_DURABILITY

	return WMSG_FAIL;
}

WMSG_RESULT CUIDurability::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	m_bDrag = false;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (CUIIcon* pDrag = pUIMgr->GetDragIcon())
	{
		if (m_pIcon[eICON_ITEM] != NULL && m_pIcon[eICON_ITEM]->IsInside(x, y))
		{
			if (ItemCheck(eICON_ITEM) == TRUE)
			{
				m_pIcon[eICON_ITEM]->copyItem(pDrag);
				CheckBtnState(eICON_ITEM);
			}
		}
		else if (m_pIcon[eICON_MATERIAL] != NULL && m_pIcon[eICON_MATERIAL]->IsInside(x, y))
		{
			if (ItemCheck(eICON_MATERIAL) == TRUE)
			{
				if (m_eOpenType == eRECOVERY)
				{
					ShowMsgBox(eMSG_ITEMCOUNT);
				}
				else
				{
					m_pIcon[eICON_MATERIAL]->copyItem(pDrag);
				}
				
				CheckBtnState(eICON_MATERIAL);
			}
		}

		pUIMgr->ResetHoldBtn();
		return WMSG_SUCCESS;
	}

	pUIMgr->ResetHoldBtn();
	return WMSG_FAIL;
}

WMSG_RESULT CUIDurability::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	if (m_bHide)
		return WMSG_FAIL;

	if( m_bDrag && ( pMsg->wParam & MK_LBUTTON ) )
	{
		int ndX = x - m_nOriX;
		int ndY = y - m_nOriY;

		m_nOriX = x;
		m_nOriY = y;

		Move( ndX, ndY );
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	return WMSG_FAIL;
}

void CUIDurability::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
	m_rcOriginPos.SetRect(m_nPosX, m_nPosY, m_nWidth, m_nHeight);
}

void CUIDurability::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

void CUIDurability::SetUI()
{
	CTString strTitle = CTString("");
	CTString strBtnFirst = CTString("");
	CTString strSecond = CTString("");
	CTString strProgress = CTString("");
	BOOL	bHide = TRUE;
	
	switch(m_eOpenType)
	{
	case eDURABILITY:
		strTitle.PrintF(_S(6173, "������ ����"));
		strBtnFirst.PrintF(_S(6171, "�Ϲ� ����"));
		strSecond.PrintF(_S(6172, "Ư�� ����"));
		bHide = FALSE;
		break;
	case eRECOVERY:
		strTitle.PrintF(_S(6185, "�ִ� ������ ����"));
		strBtnFirst.PrintF(_S(6267, "����"));
		strSecond.PrintF(_S(870, "�ݱ�"));
		bHide = TRUE;
		break;
	}

	SetState(eNONE);

	if (m_pStrTitle != NULL)
		m_pStrTitle->SetText(strTitle);

	if (m_pImgRecover != NULL)
		m_pImgRecover->Hide(bHide);

	if (m_pBtn[eFIRST] != NULL)
		m_pBtn[eFIRST]->SetText(strBtnFirst);

	if (m_pBtn[eSECOND] != NULL)
		m_pBtn[eSECOND]->SetText(strSecond);

	SetBtnState(eFIRST, FALSE);
	SetBtnState(eSECOND, bHide);	

	strProgress = GetStateString(ePROGRESSING);

	if (m_pPgBar != NULL)
	{
		m_pPgBar->setProgressText( PR_STAT_IDLE, CTString("") );
		m_pPgBar->setProgressText( PR_STAT_DOING, strProgress );
		m_pPgBar->setProgressText( PR_STAT_COMPLETE, CTString("") );
		m_pPgBar->SetProgressPos( 0 );
	}
	
}

void CUIDurability::PressBtn( eBTN_TYPE eType )
{
	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_MATERIAL] == NULL)
		return;

	if (m_bProgress == TRUE)
		return;

	if (m_eOpenType == eDURABILITY)
	{
		switch(eType)
		{
		case eFIRST: // �Ϲ� ����
			ShowMsgBox(eMSG_REPAIR);
			break;
		case eSECOND: // Ư�� ����
			ShowMsgBox(eMSG_SPECIAL_REPAIR);
			break;
		}
	}
	else if (m_eOpenType == eRECOVERY)
	{
		switch(eType)
		{
		case eFIRST: // ����
			ShowMsgBox(eMSG_RECOVER);
			break;
		case eSECOND: // �ݱ�
			Close();
			break;
		}
	}

	SetBtnState(eType, FALSE);
}

void CUIDurability::ClearIcon()
{
	for (int i = 0; i < eICON_MAX; i++)
	{
		if (m_pIcon[i] != NULL)
			m_pIcon[i]->clearIconData();
	}
}

void CUIDurability::UpdateResultIcon( UWORD tab, UWORD InvenIdx)
{
	if (IsVisible() == FALSE)
		return;

	if ( m_eState != ePROGRESSING)
		return;

	CItems* pItems = NULL;

	pItems = &_pNetwork->MySlotItem[tab][InvenIdx];

	if (m_pIcon[eICON_RESULT] != NULL)
	{
		m_pIcon[eICON_RESULT]->setData(pItems);
	}

	if (m_pIcon[eICON_MATERIAL] != NULL)
		m_pIcon[eICON_MATERIAL]->clearIconData();

	if (m_pIcon[eICON_ITEM] != NULL)
		m_pIcon[eICON_ITEM]->clearIconData();

	SetState(eCOMPLETE);
}

void CUIDurability::SetState( eSTATE eState )
{
	if (eState < eNONE || eState >= eSTATE_MAX)
		return;

	m_eState = eState;

	if (m_pTbStateGuid != NULL)
	{
		CTString strState = GetStateString(eState);

		if (m_eState == ePROGRESSING)
			strState = CTString("");

		m_pTbStateGuid->SetText(strState);
	}
}

LONGLONG CUIDurability::CalcConsumNas()
{
	if (m_pIcon[eICON_ITEM] == NULL)
		return 0;

	CItems* pItems = NULL;
	pItems = m_pIcon[eICON_ITEM]->getItems();
	int now, max;
	pItems->GetDurability(now, max);
	
	LONGLONG llmax = max;
	LONGLONG llnow = now;

	LONGLONG fee = ((llmax - llnow) * 19991) / 10;

	return fee;
}

CTString CUIDurability::GetStateString( eSTATE eState )
{
	CTString strState = CTString("");

	if (eState < eNONE || eState >= eSTATE_MAX)
		return strState;
		
	int	nStingIdx[eOPEN_MAX][eSTATE_MAX] = 
	{
		{6181, 6268, 6182, 6183, 6184},
		{6186, 6187, 6188, 6189, 6190}
	};

	switch(eState)
	{
	case eREGISTER_MATERIAL:
		{
			if (m_eOpenType == eRECOVERY)
			{
				CTString strItemName = _pNetwork->GetItemName(RECOVER_ITEM);
				strState.PrintF(_S(nStingIdx[m_eOpenType][eState], "%s �������� ����Ͽ� �ִ� �������� ���� �մϴ�."), strItemName);
				break;
			}
		}
	case eNONE:
	case eCOMPLETE:
	case ePROGRESSING:
		strState.PrintF(_S(nStingIdx[m_eOpenType][eState], "Only String"));
		break;
	case eREGISTER_ITEM:
		{
			if (m_eOpenType == eDURABILITY)
			{
				m_llNas = CalcConsumNas();
				CTString strTemp;
				strTemp.PrintF( "%I64d", m_llNas );
				CUIManager::getSingleton()->InsertCommaToString( strTemp );
				strState.PrintF(_S(nStingIdx[m_eOpenType][eState], "�������:%s"), strTemp);
			}
			else
			{
				CTString strItemName = _pNetwork->GetItemName(RECOVER_ITEM);
				strState.PrintF(_S(nStingIdx[m_eOpenType][eState], "%s �������� �÷� �ּ���."), strItemName);
			}
		}
		break;
	}

	return strState;
}


void CUIDurability::ShowMsgBox( eMSG_BOX eType )
{
	CUIMsgBox_Info MsgInfo;
	CTString strMessage;
	DWORD dwStyle;
	int nWhichUI, MsgCode;
	bool bEnable = true;
#ifdef DURABILITY
	if (eType == eMSG_ITEMCOUNT)
	{
		dwStyle = UMBS_OKCANCEL | UMBS_INPUTBOX;
		nWhichUI = UI_DURABILITY;
		MsgCode = MSGCMD_DURABILITY_MATERIAL_COUNT;
	}
	else if (eType < eMSG_TYPE_MAX)
	{
		dwStyle = UMBS_OKCANCEL;
		nWhichUI = UI_DURABILITY;
		MsgCode = MSGCMD_DURABILITY_REPAIR + eType;
	}
	else
#endif	//	DURABILITY
	{
		dwStyle = UMBS_OK;
		nWhichUI = UI_NONE;
		MsgCode = MSGCMD_NULL;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();
	MsgInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), dwStyle, nWhichUI, MsgCode );
	
	// �Ϲ� ����
	if( eType == eMSG_REPAIR )
	{
		COLOR col = 0xFFFFFFFF;

		if (m_llNas > _pNetwork->MyCharacterInfo.money)
		{
			col = 0xFF0000FF;
			bEnable = false;
		}
		
		CTString strTemp;
		strTemp.PrintF( "%I64d", m_llNas );
		pUIManager->InsertCommaToString( strTemp );
		strMessage.PrintF(_S( 6268, "�������: %s"), strTemp);
		MsgInfo.AddString( _S( 6175, "�Ϲ� ������ �Ͻ� �ڽ��ϱ�? (�Ϲ� ���� �� �ִ� �������� �ս� �˴ϴ�.)") );
		MsgInfo.AddString(strMessage, col);
		
	}
	// Ư�� ����
	else if( eType == eMSG_SPECIAL_REPAIR )
		MsgInfo.AddString( _S( 6176, "Ư�� ������ �Ͻ� �ڽ��ϱ�? (Ư�� ���� �� �ִ� �������� �սǵ��� �ʽ��ϴ�.)") );
	// ������ ����
	else if( eType == eMSG_RECOVER )
	{
		strMessage.PrintF(_S( 6177, "�ִ� ������ %d�� ���� �Ͻðڽ��ϱ�?."), m_nMaterialCount);
		MsgInfo.AddString(strMessage);
		
		if (m_pIcon[eICON_ITEM] != NULL &&
			m_pIcon[eICON_ITEM]->IsEmpty() == false)
		{
			int now = 0, max = 0;
			CItems* pItems = m_pIcon[eICON_ITEM]->getItems();
			if (pItems != NULL)
			{
#ifdef DURABILITY
				now = pItems->Item_durability_now / 10;
				max = pItems->Item_durability_max / 10;
#endif	//	DURABILITY
				strMessage.PrintF(_S( 6178, "���� ������: %d/%d"), now, max);
				MsgInfo.AddString(strMessage);
				strMessage.PrintF(_S( 6179, "���� ������: %d/%d"), now, max + m_nMaterialCount);
				MsgInfo.AddString(strMessage);
			}
		}
	}
	// ��� ������ ��Ͻ�
	else if( eType == eMSG_ITEMCOUNT )
	{
		strMessage.PrintF( _S( 150, "�� ���� %s<��> �ű�ðڽ��ϱ�?."), _pNetwork->GetItemName(RECOVER_ITEM) );
		MsgInfo.AddString(strMessage);
	}
	// �������� ���� ������ ��Ͻ�
	else if( eType == eMSG_ERROR_NO_DURABILITY )
		MsgInfo.AddString( _S( 6180, "�������� ���� ���� �ʴ� ������ �Դϴ�") );
	// ������ ������ ��Ͻ�
	else if( eType == eMSG_ERROR_WEARITEM )
		MsgInfo.AddString( _S( 2898, "���� ���� ���� �������� �÷� �ּ���") );
	// ���� Ȥ�� �ź��� ��ġ�� �ƴ� ��
	else if( eType == eMSG_ERROR_NO_MATERIAL )
	{
		int nIdx;		
		if (m_eOpenType == eDURABILITY)
			nIdx = DURABILITY_ITEM;
		else
			nIdx = RECOVER_ITEM;

		strMessage.PrintF( _S( 6187, "%s �������� �÷� �ּ���."), _pNetwork->GetItemName(nIdx) );
		MsgInfo.AddString(strMessage);
	}
	// ������ �ʿ䰡 ������
	else if( eType == eMSG_ERROR_NOT_DAMAGED_ITEM )
		MsgInfo.AddString( _S( 6265, "�������� �ջ�� �������� �ƴմϴ�."));
	// ������ �ʿ䰡 ������
	else if( eType == eMSG_ERROR_NO_RECOVER_ITEM )
		MsgInfo.AddString( _S( 6264, "�ִ� �������� �սǵ� �������� �ƴմϴ�."));
	// ��ġ ������ �ʹ� ������
	else if( eType == eMSG_ERROR_OVER_RECOVER )
		MsgInfo.AddString( _S( 6266, "������ �� �ִ� �ִ� �������� �ʰ��Ͽ����ϴ�."));
	// ������ ������
	else if( eType == eMSG_ERROR_NOT_ENOUGH_MONEY)
		MsgInfo.AddString( _S( 306, "������ �����մϴ�."));
	// ��ġ Ȥ�� ������ ������
	else if( eType == eMSG_ERROR_NOT_ENOUGH_ITEM)
		MsgInfo.AddString( _S( 1799, "��ᰡ �����մϴ�. Ȯ���ϰ� �ٽ� �õ��Ͽ� �ֽʽÿ�."));
	
	pUIManager->CreateMessageBox( MsgInfo );

	// ������ ������ ��� Ȯ�ι�ư ��Ȱ��ȭ
	if (eType == eMSG_REPAIR && pUIManager->GetMessageBox(MSGCMD_DURABILITY_REPAIR) != NULL && bEnable == false)
	{
		pUIManager->GetMessageBox(MSGCMD_DURABILITY_REPAIR)->GetBtnOK().SetEnable(FALSE);
	}

#ifdef DURABILITY
	if ( eType == eMSG_ITEMCOUNT )
	{
		if ( m_pIcon[eICON_MATERIAL] == NULL)
			return;

		CUIIcon* pDrag = pUIManager->GetDragIcon();

		if (pDrag == NULL)
			return;

		pUIManager->GetMessageBox(MSGCMD_DURABILITY_MATERIAL_COUNT)->GetInputBox().SetMaxChar(4);
		m_pIcon[eICON_MATERIAL]->cloneItem(pDrag);
		m_pIcon[eICON_MATERIAL]->setCount(1);

		m_nMaterialCount = 1;
	}
#endif	//	DURABILITY
}

void CUIDurability::MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput )
{
	CUIManager* pUIManager = CUIManager::getSingleton();

#ifdef DURABILITY
	switch(nCommandCode)
	{
	case MSGCMD_DURABILITY_REPAIR:
	case MSGCMD_DURABILITY_RECOVER:
		{			
			if(bOK)
				StartProgress(eFIRST);
			else
				SetBtnState(eFIRST, TRUE);
		}
		break;

	case MSGCMD_DURABILITY_SPECIAL_REPAIR:
		{			
			if(bOK)
				StartProgress(eSECOND);
			else
				SetBtnState(eSECOND, TRUE);
		}
		break;

	case MSGCMD_DURABILITY_MATERIAL_COUNT:
		{
			if ( m_pIcon[eICON_MATERIAL] == NULL)
				return;

			if(bOK)
			{
				int nIndex = m_pIcon[eICON_MATERIAL]->getIndex();
				int nMax = CUIManager::getSingleton()->GetInventory()->GetItemCount(nIndex);
				
				if (strInput.IsInteger() && _atoi64(strInput) <= nMax && _atoi64(strInput) > 0)
					m_nMaterialCount = _atoi64(strInput);
				else if (strInput.IsInteger() && _atoi64(strInput) > nMax)
					m_nMaterialCount = nMax;
				else
				{
					m_pIcon[eICON_MATERIAL]->clearIconData();
					CheckBtnState(eICON_ITEM);
					m_nMaterialCount = 0;
					break;
				}

				m_pIcon[eICON_MATERIAL]->setCount(m_nMaterialCount);
			}
			else
			{
				m_pIcon[eICON_MATERIAL]->clearIconData();
				CheckBtnState(eICON_ITEM);
				m_nMaterialCount = 0;
			}
		}
		break;
	}

	if (pUIManager->DoesMessageBoxExist(nCommandCode))
		pUIManager->CloseMessageBox(nCommandCode);
#endif	//	DURABILITY
}

void CUIDurability::SendEvent( eBTN_TYPE eType )
{
	if (m_pIcon[eICON_ITEM] == NULL || m_pIcon[eICON_MATERIAL] == NULL)
		return;

	CDurability* pData = GameDataManager::getSingleton()->GetDurability();

	if (pData == NULL)
		return;

	CItems* pItem = m_pIcon[eICON_ITEM]->getItems();
	CItems* pMaterial = m_pIcon[eICON_MATERIAL]->getItems();

	if (pItem == NULL)
	{
		SetUI();
		return;
	}

	// item info
	UWORD	item_Tab = pItem->Item_Tab;
	UWORD	item_InvenIdx = pItem->InvenIndex;
	int		item_VirIdx = pItem->Item_UniIndex;

	// material info
	UWORD	material_Tab = 0;
	UWORD	material_InvenIdx = 0;
	int		material_VirIdx = -1;
	UWORD	material_Count = 0;

	if (pMaterial != NULL)
	{
		material_Tab = pMaterial->Item_Tab;
		material_InvenIdx = pMaterial->InvenIndex;
		material_VirIdx = pMaterial->Item_UniIndex;
		material_Count = m_nMaterialCount;
	}

	if (m_eOpenType == eDURABILITY)
	{
		switch(eType)
		{
		case eFIRST: // �Ϲ� ����
			pData->SendRepair(item_Tab, item_InvenIdx, item_VirIdx, m_llNas, m_nMobIndex);
			break;
		case eSECOND: // Ư�� ����
			pData->SendRepairSpecial(item_Tab, item_InvenIdx, item_VirIdx, material_Tab, material_InvenIdx, material_VirIdx, m_nMobIndex);
			break;
		}
	}
	else if (m_eOpenType == eRECOVERY)
	{
		switch(eType)
		{
		case eFIRST: // ����
			pData->SendRecovery(item_Tab, item_InvenIdx, item_VirIdx, material_Tab, material_InvenIdx, material_VirIdx, material_Count);
			break;
		case eSECOND: // �ݱ�
			Close();
			break;
		}
	}
}

BOOL CUIDurability::ItemCheck( eICON_TYPE eType )
{
	if (eType < 0 || eType >= eICON_MAX)
		return FALSE;

	if (m_pIcon[eType] == NULL)
		return FALSE;

	CUIManager* pUIMgr = CUIManager::getSingleton();

	CUIIcon* pDrag = pUIMgr->GetDragIcon();
	
	if (pDrag == NULL)
		return FALSE;

	CItems* pItems = pDrag->getItems();

	if (pItems == NULL)
		return FALSE;

	CItemData* pItemData = pItems->ItemData;

	if (pItemData == NULL)
		return FALSE;

	if (eType == eICON_ITEM)
	{
		if (!pItemData->IsFlag( ITEM_FLAG_DURABILITY))
		{
			ShowMsgBox(eMSG_ERROR_NO_DURABILITY);
			return FALSE;
		}

		if (pDrag->IsWearTab() == true)
		{
			ShowMsgBox(eMSG_ERROR_WEARITEM);
			return FALSE;
		}

		if (m_eOpenType == eDURABILITY)
		{
#ifdef DURABILITY
			int now = pItems->Item_durability_now / 10.0;
			int max = pItems->Item_durability_max / 10.0;

			if ( max - now <= 0)
#endif	//	DURABILITY
			{
				ShowMsgBox(eMSG_ERROR_NOT_DAMAGED_ITEM);
				return FALSE;
			}
		}
	}
	else if (eType == eICON_MATERIAL)
	{
		if (m_eOpenType == eDURABILITY)
		{
			if (pDrag->getIndex() != DURABILITY_ITEM)
			{
				ShowMsgBox(eMSG_ERROR_NO_MATERIAL);
				return FALSE;
			}
		}
		else if (m_eOpenType == eRECOVERY)
		{
			if (pDrag->getIndex() != RECOVER_ITEM)
			{
				ShowMsgBox(eMSG_ERROR_NO_MATERIAL);
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CUIDurability::SetBtnState( eBTN_TYPE eType, BOOL bEnable )
{
	if (eType < 0 || eType >= eBTN_MAX)
		return;

	if (m_pBtn[eType] == NULL)
		return;

	m_pBtn[eType]->SetEnable(bEnable);
}

void CUIDurability::CheckBtnState(eICON_TYPE eType)
{
	if (eType < 0 || eType >= eICON_MAX)
		return;

	if (m_pIcon[eType] == NULL)
		return;

	if (m_pIcon[eICON_RESULT] != NULL)
		m_pIcon[eICON_RESULT]->clearIconData();

	CItems* pItems = m_pIcon[eType]->getItems();

	if (pItems == NULL)
		return;

	eSTATE eId = eNONE;

	if (eType == eICON_ITEM)
	{
		if (m_eOpenType == eDURABILITY)
		{
			eId = eREGISTER_ITEM;
			SetBtnState(eFIRST, TRUE);

			if (m_pIcon[eICON_MATERIAL] != NULL && 
				m_pIcon[eICON_MATERIAL]->IsEmpty() == false)
			{
				eId = eREGISTER_MATERIAL;
				SetBtnState(eSECOND, TRUE);
				SetBtnState(eFIRST, FALSE);

			}
			SetState(eId);

		}
		else if (m_eOpenType == eRECOVERY)
		{
			eId = eREGISTER_ITEM;
			SetBtnState(eFIRST, FALSE);

			if (m_pIcon[eICON_MATERIAL] != NULL &&
				m_pIcon[eICON_MATERIAL]->IsEmpty() == false)
			{
				eId = eREGISTER_MATERIAL;
				SetBtnState(eFIRST, TRUE);
			}
			SetState(eId);
		}
	}
	else if (eType == eICON_MATERIAL)
	{
		eId = eREGISTER_MATERIAL;

		if (m_pIcon[eICON_ITEM] != NULL)			
		{
			if (m_pIcon[eICON_ITEM]->IsEmpty() == false)
			{
				if (m_eOpenType == eDURABILITY)
				{
					SetBtnState(eSECOND, TRUE);
					SetBtnState(eFIRST, FALSE);
				}
				else if (m_eOpenType == eRECOVERY)
					SetBtnState(eFIRST, TRUE);
			}
			else
				eId = eNONE;
		}

		SetState(eId);
	}
}

void CUIDurability::CalcProgressTimer()
{
	if( m_bProgress )
	{
		__int64 tmpTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
		
		if( tmpTime - m_uStartTime > ( m_uEndTime * 1000 ) )
		{	
			SendEvent(m_eEventType);
			m_bProgress = FALSE;

			if (m_pPgBar != NULL)
				m_pPgBar->CompleteProgress();
		}
		else
		{
			if (m_pPgBar != NULL)
				m_pPgBar->OffsetProgressPos( (tmpTime - m_uStartTime) );	
		}
	}
}

void CUIDurability::StartProgress( eBTN_TYPE eType )
{
	m_bProgress = TRUE;
	
	m_eEventType = eType;

	if (eType == eSECOND && m_eOpenType == eRECOVERY)
		return;

	if (m_pPgBar != NULL)
	{
		m_pPgBar->SetProgressPos( 0 );
		m_pPgBar->StartProgress();
	}
	
	SetState(ePROGRESSING);
	if (m_pIcon[eICON_RESULT] != NULL)
	{
		m_pIcon[eICON_RESULT]->clearIconData();
	}

	m_uStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
}
