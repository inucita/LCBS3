#include "StdH.h"

// ��� ����. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <vector>
#include <Engine/Contents/function/BingoBoxUI.h>

class CmdBingoBoxClose : public Command
{
public:
	CmdBingoBoxClose() : m_pWnd(NULL) {}
	void setData(CBingoBoxUI* pWnd) { m_pWnd = pWnd;	}
	void execute()
	{
		if (m_pWnd != NULL)
			m_pWnd->CloseUI();
	}

private:
	CBingoBoxUI* m_pWnd;
};

const int ctnBgNum = 8;
const __int64 tmLimitHighlight = 200; // 0.2��

extern INDEX g_iCountry;

int BingoOfCase[ctnBgNum][3] =
{
	{0,1,2}, //0
	{3,4,5}, //1
	{6,7,8}, //2
	{0,3,6}, //3
	{1,4,7}, //4
	{2,5,8}, //5
	{0,4,8}, //6
	{2,4,6}  //7
};

CBingoBoxUI::CBingoBoxUI()
	: m_pIconTemp(NULL)
	, m_pArrayData(NULL)
	, m_pTextScore(NULL)
	, m_pTextTitle(NULL)
	, m_nOldX(0)
	, m_nOldY(0)
	, m_bDrag(false)
	, m_bBtnDownInItem(false)
{
	setInherit(false);

	m_nSelectItem = -1;
	m_nTempItemArray = -1;
	m_nTempItemIndex = -1;
	m_bSelectLock = FALSE;

	int i;
	for (i = 0; i < ctnBgNum; i++)
	{
		m_BingoBtnInfo[i].bBingoEnable =FALSE;
		m_BingoBtnInfo[i].bHighlight = FALSE;
		m_BingoBtnInfo[i].tmBingoStart = 0;
	}

	for (i = 0; i < 9; ++i)
	{
		m_pIconInsectItem[i] = NULL;
	}

	m_BingoOfCase[0].vecBingo.push_back(0);	m_BingoOfCase[0].vecBingo.push_back(3);
	m_BingoOfCase[0].vecBingo.push_back(6); // 0 3 6

	m_BingoOfCase[1].vecBingo.push_back(0);	m_BingoOfCase[1].vecBingo.push_back(4); // 0 4

	m_BingoOfCase[2].vecBingo.push_back(0);	m_BingoOfCase[2].vecBingo.push_back(5);
	m_BingoOfCase[2].vecBingo.push_back(7); //0 5 7

	m_BingoOfCase[3].vecBingo.push_back(1); m_BingoOfCase[3].vecBingo.push_back(3); // 1 3

	m_BingoOfCase[4].vecBingo.push_back(1); m_BingoOfCase[4].vecBingo.push_back(4);
	m_BingoOfCase[4].vecBingo.push_back(6); m_BingoOfCase[4].vecBingo.push_back(7); // 1 4 6 7

	m_BingoOfCase[5].vecBingo.push_back(1); m_BingoOfCase[5].vecBingo.push_back(5); // 1 5

	m_BingoOfCase[6].vecBingo.push_back(2); m_BingoOfCase[6].vecBingo.push_back(3);
	m_BingoOfCase[6].vecBingo.push_back(7); // 2 3 7

	m_BingoOfCase[7].vecBingo.push_back(2); m_BingoOfCase[7].vecBingo.push_back(4); // 2 4

	m_BingoOfCase[8].vecBingo.push_back(2); m_BingoOfCase[8].vecBingo.push_back(5);
	m_BingoOfCase[8].vecBingo.push_back(6); // 2 5 6

	m_pIconTemp = new CUIIcon;
	m_pIconTemp->initialize();
}

CBingoBoxUI::~CBingoBoxUI()
{
	Destroy();
	SAFE_DELETE(m_pIconTemp);
}

void CBingoBoxUI::initialize()
{
	if (CUIButton* pBtnClose = (CUIButton*)findUI("btn_close"))
	{
		CmdBingoBoxClose* pCmd = new CmdBingoBoxClose();
		pCmd->setData(this);
		pBtnClose->SetCommandUp(pCmd);
	}

	m_pArrayData = (CUIArray*)findUI("array_bingo");
	m_pTextScore = (CUIText*)findUI("text_score");
	m_pTextTitle = (CUIText*)findUI("text_title");

	if (m_pArrayData == NULL)
		return;

	for (int i = 0; i < 9; ++i)
	{
		CUIArrayItem* pItem = m_pArrayData->GetArrayItem(i);
		m_pIconInsectItem[i] = (CUIIcon*)pItem->findUI("icon_item");
	}
}

void CBingoBoxUI::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	SetPos((pixMaxI + pixMinI) / 2 - GetWidth(), (pixMaxJ + pixMinJ - GetHeight()) / 2);
}

void CBingoBoxUI::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
	if ( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
}

//============================================================================================================
// Name : Init()
// Desc :
//============================================================================================================
void CBingoBoxUI::Init()
{
	int i;

	for ( i=0; i<9; i++ )
	{
		m_BingoOfCase[i].bMultiply = FALSE;
		m_pIconInsectItem[i]->clearIconData();
	}

	for ( i=0; i<ctnBgNum; i++ )
	{
		m_BingoBtnInfo[i].bBingoEnable =FALSE;
		m_BingoBtnInfo[i].bHighlight = FALSE;
		m_BingoBtnInfo[i].tmBingoStart = 0;
	}

	m_bSelectLock = FALSE;
	m_nSelectItem = -1;
	m_nTab = -1;
	m_nInvenIdx = -1;

	m_nBingo = 0;
	UpdateBingo();
}

void CBingoBoxUI::OpenBingoBox(int nTab, int inven_idx)
{
	if(IsVisible() == TRUE || GetHide() == FALSE )
		return;

	Init();

	m_nTab = nTab;
	m_nInvenIdx = inven_idx;

	UpDateItem();

	Hide(FALSE);
	CUIManager::getSingleton()->RearrangeOrder(UI_BINGOBOX, TRUE);
}

void CBingoBoxUI::CloseUI()
{
	Hide(TRUE);
	CUIManager::getSingleton()->RearrangeOrder(UI_BINGOBOX, FALSE);
}

void CBingoBoxUI::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString &strInput)
{
	switch ( nCommandCode )
	{
	case MSGCMD_SAVE_BINGOITEM:
		{
			m_bSelectLock = FALSE;

			if ( bOK )
			{
				_pNetwork->SendSaveBingoItem(m_nTempItemArray, m_pIconTemp->getIndex(), m_nTempItemIndex);
			}
			else
			{
				m_pIconTemp->clearIconData();
				m_nTempItemArray = -1;
			}
		}
		break;
	}
}

void CBingoBoxUI::SetBtnItem(int num, int nIndex)
{
	int nItemIndex; 

	if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
		nItemIndex = 2634;
	else
		nItemIndex = 2048;

	if ( num == 0 ) return;

	switch( num )
	{
	case 1:
		break;
	case 2:
		{
			nItemIndex += 1; // 2049 or 2644
		}
		break;
	case 4:
		{
			nItemIndex += 2; // 2050 or 2645
		}
		break;
	default :
		return;
	}

	CItemData* pItemData = _pNetwork->GetItemData(nItemIndex);

	m_pIconInsectItem[nIndex]->setData(UBET_ITEM, nItemIndex);
}

void CBingoBoxUI::FindBingo(int num, BOOL bAllSearch/*=FALSE*/)
{
	int i;

	if ( bAllSearch )
	{
		for ( i=0; i<ctnBgNum; i++ )
		{
			if ( m_pIconInsectItem[BingoOfCase[i][0]]->getIndex() == m_pIconInsectItem[BingoOfCase[i][1]]->getIndex() &&
				m_pIconInsectItem[BingoOfCase[i][0]]->getIndex() == m_pIconInsectItem[BingoOfCase[i][2]]->getIndex() )
			{ // ����
				if ( m_pIconInsectItem[BingoOfCase[i][0]]->getIndex() < 0 ) continue;
		
				m_BingoBtnInfo[i].bBingoEnable = TRUE;
			}
		}
	}
	else
	{
		if ( num < 0 ) return;
	
		__int64 tmTemp = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
		for (i=0; i<m_BingoOfCase[num].vecBingo.size(); i++)
		{
			int Temp0, Temp1, Temp2, Temp3;
	
			Temp3 = m_BingoOfCase[num].vecBingo[i];
	
			if (m_BingoBtnInfo[Temp3].bBingoEnable)	continue;
	
			Temp0 = BingoOfCase[Temp3][0];
			Temp1 = BingoOfCase[Temp3][1];
			Temp2 = BingoOfCase[Temp3][2];
	
			if (m_pIconInsectItem[Temp0]->getIndex() == m_pIconInsectItem[Temp1]->getIndex() &&
				m_pIconInsectItem[Temp0]->getIndex() == m_pIconInsectItem[Temp2]->getIndex() )
			{
				m_BingoBtnInfo[Temp3].bHighlight = TRUE;
				m_BingoBtnInfo[Temp3].tmBingoStart = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			}
		}
	}
}

void CBingoBoxUI::UpDateItem(void)
{
	CItems &rItems = _pNetwork->MySlotItem[m_nTab][m_nInvenIdx];

	if (m_pTextTitle != NULL)
	{
		m_pTextTitle->SetText(CTString(rItems.ItemData->GetName()));
	}

	m_nTempItemIndex = rItems.ItemData->GetItemIndex();

	ULONG ulTemp;
	ULONG ulBit = rItems.Item_Plus;

	int i;

	for( i=0; i<9; i++ )
	{
		ulTemp = ((ulBit >> i*3) & 0x00000007);
		SetBtnItem(ulTemp, i);
	}

	m_nBingo = rItems.Item_Used;
	UpdateScore(m_nBingo);

	FindBingo(0, TRUE);
}

void CBingoBoxUI::ErrorMessage(CNetworkMessage *istr)
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	ULONG ulErrorType;
	ULONG ulTemp;
	CUIMsgBox_Info MsgBoxInfo;
	CTString strMessage;

	MsgBoxInfo.SetMsgBoxInfo(_S( 191, "Ȯ��"), UMBS_OK, UI_NONE,	MSGCMD_NULL);

	(*istr) >> ulErrorType;

	switch ( ulErrorType )
	{
	case MSG_EVENT_VALENTINE_2007_PACKAGE_OK : // ���ڷ� ������ ���� ����
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3987, "���ָӴϰ� ������� �������ϴ�. ���� ���ָӴϴ� �Ҹ������� ���� �� ���� �뵵�� ����� �� �����ϴ�.");
			else
				strMessage = _S(3192, "���ڷ��� ����Ǿ����ϴ�. ����� ���ڷ��� ���ڻ����� ���� �̿��� �뵵�� ����� �� �����ϴ�.");
		}
		break;
	case MSG_EVENT_VALENTINE_2007_PACKAGE_FULLINVEN : // �κ��� ���� ��
		{
			pUIManager->GetChattingUI()->AddSysMessage( _S(116, "�κ��丮 ������ �����Ͽ� ������ ���� �� �����ϴ�."), SYSMSG_ERROR );
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_OK : // ����� ���ڷ� ���� ����
		{
			(*istr) >> ulTemp;

			m_nBingo += ulTemp;
			m_pIconInsectItem[m_nTempItemArray]->setData(UBET_ITEM, m_pIconTemp->getIndex());

			FindBingo(m_nTempItemArray);

			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3988, "���� ���ָӴϰ� ���� �Ҹ����ڿ� �����Ǿ����ϴ�.");
			else
				strMessage = _S(3193, "���ڷ��� ���ڼ������ڿ� �����Ǿ����ϴ�.");

			pUIManager->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_ERROR);
			
			UpdateScore(m_nBingo);
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_NOTPACKAGE : // ����� ���ڷ��� �ƴ�
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3989, "���� ���ָӴϰ� �ƴմϴ�.");
			else
				strMessage = _S(3194, "����� ���ڷ��� �ƴմϴ�.");

			pUIManager->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_ERROR);
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_ARRANGE_ALREADY : // �̹� ����� �ڸ�
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3990, "�̹� ���� ���ָӴϰ� �����մϴ�.");
			else
				strMessage = _S(3195, "�̹� ����� ���ڷ��� �����մϴ�.");

			pUIManager->GetChattingUI()->AddSysMessage(strMessage, SYSMSG_ERROR);
			return;
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_GIFT_OK : // ��ǰ ���� ����
		{
			(*istr) >> ulTemp;

			if (IsVisible())
			{
				CloseUI();
			}

			strMessage.PrintF(_S(3196, "�����մϴ�. %d ���� ���� �Ǿ����ϴ�.(����:%d)"), ulTemp, m_nBingo);
			UpdateBingo();
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_GIFT_NOITEM : // ���� �ϳ��� ����
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3991, "�Ҹ����ڿ� ���� �ϳ��� �����ϴ�. Ȯ�� �� �ٽ� �����ϼ���");
			else
				strMessage = _S(3197, "���ڻ��ڿ� ���� �ϳ��� �����ϴ�. Ȯ�� �� �ٽ� �����ϼ���");
		}
		break;
	case MSG_EVENT_VALENTINE_2007_BINGO_ITEM_NOTFOUND : // ���ڻ��ڰ� ����
		{
			if(IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
				strMessage = _S(3992, "�Ҹ����ڰ� �������� �ʽ��ϴ�.");
			else
				strMessage = _S(3198, "���ڻ��ڰ� �������� �ʽ��ϴ�.");
		}
		break;
	}

	MsgBoxInfo.AddString(strMessage);
	pUIManager->CreateMessageBox(MsgBoxInfo);
}

WMSG_RESULT CBingoBoxUI::OnMouseMove( UINT16 x, UINT16 y, MSG* pMsg )
{
	int ndX = x - m_nOldX;
	int ndY = y - m_nOldY;

	if (m_bDrag)
	{
		m_nOldX = x; m_nOldY = y;
		Move(ndX, ndY);
		return WMSG_SUCCESS;
	}

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager::getSingleton()->SetMouseCursorInsideUIs();

	m_nSelectItem = -1;
	return WMSG_FAIL;
}

WMSG_RESULT CBingoBoxUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if ( IsInside(x, y) )
	{
		CUIManager* pUIManager = CUIManager::getSingleton();
		pUIManager->RearrangeOrder(UI_BINGOBOX, TRUE);

		m_nOldX = x;
		m_nOldY = y;

		if ( m_pTextTitle != NULL && m_pTextTitle->IsInside(x, y) == TRUE )
		{
			m_bDrag = true;
		}

		return WMSG_SUCCESS;
	}
	return WMSG_FAIL;
}

WMSG_RESULT CBingoBoxUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	m_bBtnDownInItem = false;

	if (pUIManager->GetDragIcon() == NULL)
	{
		m_bDrag = false;

		if ( IsFocused() == FALSE ) 
			return WMSG_FAIL;
	}
	else
	{
		for (int i = 0; i < 9; ++i )
		{
			if (m_pIconInsectItem[i]->IsInside(x, y) && m_pIconInsectItem[i]->IsEmpty())
			{
				if ( pUIManager->GetDragIcon()->getBtnType() == UBET_ITEM &&
					pUIManager->GetDragIcon()->GetWhichUI() == UI_INVENTORY )
				{
					m_pIconTemp->setData(UBET_ITEM, pUIManager->GetDragIcon()->getIndex());

					if (m_bSelectLock == FALSE)
					{
						m_nTempItemArray = m_pArrayData->HitTest(x, y);
					}

					m_nSelectItem = i;
					m_bSelectLock = TRUE;

					CItemData* pItemData = _pNetwork->GetItemData(m_pIconTemp->getIndex());

					pUIManager->CloseMessageBox(MSGCMD_SAVE_BINGOITEM);
					CUIMsgBox_Info MsgBoxInfo;

					MsgBoxInfo.SetMsgBoxInfo(_S( 191, "Ȯ��"), UMBS_OKCANCEL, UI_BINGOBOX,	MSGCMD_SAVE_BINGOITEM);

					if (IS_EVENT_ON(TEVENT_LUNARNEWYEAR_2008))
						MsgBoxInfo.AddString(_S(3986, "���ָӴϸ� �ѹ� ������ ��ġ�� �ٲܼ� �����ϴ�. ������ ���ָӴϸ� ���ڿ� �ְڽ��ϱ�?"));
					else
						MsgBoxInfo.AddString(_S(3191, "���ڷ��� �ѹ� ������ ��ġ�� �ٲܼ� �����ϴ�. ������ ���ڷ��� ���ڿ� �ְڽ��ϱ�?"));

					pUIManager->CreateMessageBox(MsgBoxInfo);

					pUIManager->ResetHoldBtn();
					return WMSG_SUCCESS;
				}
			}
		}
	}

	if (pUIManager->GetDragIcon() != NULL)
	{
		pUIManager->ResetHoldBtn();
		return WMSG_SUCCESS;
	}

	return WMSG_FAIL;
}

void CBingoBoxUI::UpdateBingo()
{
	__int64 tmTemp = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	for ( int i = 0; i < ctnBgNum; i++ )
	{
		if ( m_BingoBtnInfo[i].bHighlight && (tmTemp - m_BingoBtnInfo[i].tmBingoStart) > tmLimitHighlight )
		{
			m_BingoBtnInfo[i].bBingoEnable = TRUE;
			m_BingoBtnInfo[i].bHighlight = FALSE;
			m_BingoBtnInfo[i].tmBingoStart = 0;
		}
		else if ( m_BingoBtnInfo[i].bBingoEnable )
		{
			if (!m_BingoOfCase[BingoOfCase[i][0]].bMultiply)
				m_BingoOfCase[BingoOfCase[i][0]].bMultiply = TRUE;

			if (!m_BingoOfCase[BingoOfCase[i][1]].bMultiply)
				m_BingoOfCase[BingoOfCase[i][1]].bMultiply = TRUE;

			if (!m_BingoOfCase[BingoOfCase[i][2]].bMultiply)
				m_BingoOfCase[BingoOfCase[i][2]].bMultiply = TRUE;
		}
	}
}

void CBingoBoxUI::UpdateScore(int nScore)
{
	if (m_pTextScore != NULL)
	{
		CTString strTemp;
		strTemp.PrintF(_S(3190, "����:%d"), nScore);
		m_pTextScore->SetText(strTemp);
	}
}
