#include "stdh.h"
#include "TatooUI.h"

#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>

#define DEF_MAX_COLOR	9
#define DEF_MAX_PATTERN 5

// ������ �����ִ� ���ÿ����� extra ��
const int ct_iColorShapeIndex[DEF_MAX_COLOR][DEF_MAX_PATTERN] = { 
	  {26, 27, 28, 29, 30} 
	, {31, 32, 33, 34, 35}
	, {13, 22, 23, 24, 25} 
	, {36, 37, 38, 39, 40}
	, {41, 42, 43, 44, 45} 
	, {46, 47, 48, 49, 50}	
	, {51, 52, 53, 54, 55} 
	, {11, 14, 15, 16, 17}
	, {12, 18, 19, 20, 21} 
};

// ���� �ڵ�
const COLOR SelectColorList[DEF_MAX_COLOR] = {
	  0xFF0000FF // ����
	, 0xFFA500FF // ��Ȳ
	, 0xFFFF00FF // ���
	, 0x00FF00FF // �ʷ�
	, 0x0000FFFF // �Ķ�
	, 0x000080FF // ����
	, 0x800080FF // ����
	, 0x000000FF // ����
	, 0xFFFFFFFF // �Ͼ�
};

CTatooUI::CTatooUI()
	: m_pMoveArea(NULL)
{
	int i;
	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = NULL;

	for (i = 0; i < eIMG_MAX; ++i)
		m_pImgTemp[i] = NULL;

	for (i = 0; i < eARRAY_MAX; ++i)
		m_pArray[i] = NULL;

	setInherit(false);
}

void CTatooUI::OpenTatoo()
{
	if (GetHide() == FALSE)
		return;

	Hide(FALSE);
	SetVisible(TRUE);

	CheckShowShape();
	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_TATOO, TRUE );
}

void CTatooUI::Close()
{
	Hide(TRUE);
	SetVisible(FALSE);

	CUIManager* pUIMgr = UIMGR();
	pUIMgr->RearrangeOrder( UI_TATOO, FALSE );
}

void CTatooUI::initialize()
{
#ifndef		WORLD_EDITOR

	int i;
	// drag
	m_pMoveArea = (CUIImage*)findUI("move_area");

	if (m_pMoveArea != NULL)
	{
		int l, t;
		l = m_pMoveArea->GetPosX();
		t = m_pMoveArea->GetPosY();
		setTitleRect(l, t, l + m_pMoveArea->GetWidth(), t + m_pMoveArea->GetHeight());
	}

	// Button
	std::string strBtnID[eBTN_MAX] = { "btn_close", "btn_ok", "btn_cancel" };

	for (i = 0; i < eBTN_MAX; ++i)
		m_pBtn[i] = (CUIButton*)findUI(strBtnID[i].c_str());

	if (m_pBtn[eBTN_CLOSE] != NULL)
		m_pBtn[eBTN_CLOSE]->SetCommandFUp(boost::bind(&CTatooUI::Close, this));

	if (m_pBtn[eBTN_CANCEL] != NULL)
		m_pBtn[eBTN_CANCEL]->SetCommandFUp(boost::bind(&CTatooUI::Close, this));

	if (m_pBtn[eBTN_OK] != NULL)
		m_pBtn[eBTN_OK]->SetCommandFUp(boost::bind(&CTatooUI::MakeMessageBox, this));

	// Array
	std::string strArrayID[eARRAY_MAX] = { "array_color", "array_pattern" };

	for (i = 0; i < eARRAY_MAX; ++i)
		m_pArray[i] = (CUIArray*)findUI(strArrayID[i].c_str());

	// Image
	std::string strImgID[eIMG_MAX] = { "img_heart", "img_clover", "img_dia",
		"img_spade", "img_skull", "img_colorTemp" };

	for (i = 0; i < eIMG_MAX; ++i)
		m_pImgTemp[i] = (CUIImage*)findUI(strImgID[i].c_str());

	_setUI();

#endif		// WORLD_EDITOR
}


WMSG_RESULT CTatooUI::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->DoesMessageBoxExist(MSGCMD_PET_TATOOCHANGE_USE) )
		return WMSG_SUCCESS;

	pUIManager->RearrangeOrder( UI_TATOO, TRUE );

	return CUIWindow::OnLButtonDown(x, y);
}

WMSG_RESULT CTatooUI::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide)
		return WMSG_FAIL;

	if (IsInside(x, y) == FALSE)
		return WMSG_FAIL;

	CUIManager* pUIManager = CUIManager::getSingleton();
	pUIManager->ResetHoldBtn();
	if( pUIManager->DoesMessageBoxExist(MSGCMD_PET_TATOOCHANGE_USE) )
		return WMSG_SUCCESS;

	return CUIWindow::OnLButtonUp(x, y);
}

void CTatooUI::SendItemUse()
{
	int nColorAndPatternIndex = GetColorAndShapeIndex();

	if (nColorAndPatternIndex <= 0)
	{
		Close();
		return;
	}

	int tv_tab, tv_idx;
	CUIManager::getSingleton()->GetInventory()->GetUseItemSlotInfo(tv_tab,tv_idx);
	CItems& Items = _pNetwork->MySlotItem[tv_tab][tv_idx];
	int iItemIndex = Items.Item_UniIndex;
	
	_pNetwork->SendItemUse(tv_tab, tv_idx, iItemIndex, nColorAndPatternIndex);
}

void CTatooUI::CheckShowShape()
{
	BOOL bHide = TRUE;

	ObjInfo* pInfo = ObjInfo::getSingleton();
	CPetTargetInfom* pPetInfo = pInfo->GetMyPetInfo();

	int iAge = pPetInfo->iAge;

	// [090831: selo] Ÿ�� �� �ƴϸ� ���� ���� ���ϰ� ����
	if( iAge < 2 )		// Ÿ�°� �ƴ� ��
		bHide = TRUE;
	else if( iAge >= 2 ) // Ÿ�°� �� ��
		bHide = FALSE;

	if(m_pArray[eARRAY_PATTERN] != NULL)
	{
		m_pArray[eARRAY_PATTERN]->Hide(bHide);
		m_pArray[eARRAY_PATTERN]->SetSelectIdx(-1);
	}
}

int CTatooUI::GetColorAndShapeIndex()
{
	int nErrorIndex = 0;

	if (m_pArray[eARRAY_COLOR] == NULL)
		return nErrorIndex;

	int nSelColorIndex = _getSelectIndex(eARRAY_COLOR);
	int nOnlyColorIndex = nSelColorIndex + 1;

	if (nSelColorIndex < 0 || nSelColorIndex >= DEF_MAX_COLOR)
		return nErrorIndex;

	if (m_pArray[eARRAY_PATTERN] == NULL)
		return nOnlyColorIndex;

	int nSelPatternIdx = _getSelectIndex(eARRAY_PATTERN);

	if (nSelPatternIdx < 0 || nSelPatternIdx >= DEF_MAX_PATTERN)
		return nOnlyColorIndex;

	return ct_iColorShapeIndex[nSelColorIndex][nSelPatternIdx];
}

void CTatooUI::MakeMessageBox()
{
	CTString strColor ="";
	CTString strShape =_S(4448, "��");

	int nSelColorIndex = _getSelectIndex(eARRAY_COLOR);
	int nSelPatternIdx = _getSelectIndex(eARRAY_PATTERN);

	switch(nSelColorIndex)
	{
	case 0:
		strColor = _S(4434, "����");
		break;
	case 1:
		strColor = _S(4435, "��Ȳ");
		break;
	case 2:
		strColor = _S(4436, "���");
		break;
	case 3:
		strColor = _S(4437, "�ʷ�");
		break;
	case 4:
		strColor = _S(4438, "�Ķ�");
		break;
	case 5:
		strColor = _S(4439, "����");
		break;
	case 6:
		strColor = _S(4440, "����");
		break;
	case 7:
		strColor = _S(4441, "����");
		break;
	case 8:
		strColor = _S(4442, "�Ͼ�");
		break;
	}
	switch(nSelPatternIdx)
	{
	case 0:
		strShape = _S(4443, "��Ʈ");		
		break;
	case 1:
		strShape = _S(4445, "Ŭ�ι�");				
		break;
	case 2:		
		strShape = _S(4444, "���̾Ƹ��");		
		break;
	case 3:
		strShape = _S(4446, "�����̵�");		
		break;
	case 4:		
		strShape = _S(4447, "�ذ�");
		break;
	}
	
	CUIMsgBox_Info	MsgBoxInfo;
	CTString		strMessage;
	strMessage.PrintF(_S(4433, "%s���� %s������ �����ϼ̽��ϴ�. �̴�� �����Ͻðڽ��ϱ�?"), strColor, strShape);
	MsgBoxInfo.SetMsgBoxInfo(_S(4430, "���� ����"), UMBS_YESNO, UI_NONE, MSGCMD_PET_TATOOCHANGE_USE);
	MsgBoxInfo.AddString(strMessage);
	CUIManager::getSingleton()->CreateMessageBox(MsgBoxInfo);
}

int CTatooUI::_getSelectIndex( int nArrayType )
{
	if (nArrayType < 0 || nArrayType >= eARRAY_MAX)
		return -1;

	if (m_pArray[nArrayType] == NULL)
		return -1;

	return m_pArray[nArrayType]->GetSelectIdx();
}

void CTatooUI::_setUI()
{
	CUIArrayItem* pItem = NULL;
	CUIImage* pImg = NULL;
	
	int nCount = 0;
	int i;

	if (m_pArray[eARRAY_COLOR] != NULL)
	{
		nCount = m_pArray[eARRAY_COLOR]->GetArrayChildCount();

		if (nCount >= DEF_MAX_COLOR)
		{
			for (i = 0; i < DEF_MAX_COLOR; ++i)
			{
				pItem = m_pArray[eARRAY_COLOR]->GetArrayItem(i);

				if (pItem == NULL)
					continue;

				pItem->deleteChildAll();

				if (m_pImgTemp[eIMG_COLORTEMP] == NULL)
					continue;

				pImg = (CUIImage*)m_pImgTemp[eIMG_COLORTEMP]->Clone();

				if (pImg != NULL)
				{
					pImg->Hide(FALSE);
					pImg->SetColor(SelectColorList[i]);
#ifndef WORLD_EDITOR
					pImg->SetCommandFUp(boost::bind(&CTatooUI::CheckShowShape, this));
#endif	// WORLD_EDITOR	
					pItem->addChild((CUIBase*)pImg);
				}
			}

			m_pArray[eARRAY_COLOR]->UpdateItem();
		}		
	}

	if (m_pArray[eARRAY_PATTERN] != NULL)
	{
		nCount = m_pArray[eARRAY_PATTERN]->GetArrayChildCount();
		
		if (nCount >= DEF_MAX_PATTERN)
		{
			for (i = 0; i < DEF_MAX_PATTERN; ++i)
			{
				pItem = m_pArray[eARRAY_PATTERN]->GetArrayItem(i);

				if (pItem == NULL)
					continue;

				pItem->deleteChildAll();

				if (m_pImgTemp[i] == NULL)
					continue;

				pImg = (CUIImage*)m_pImgTemp[i]->Clone();

				if (pImg != NULL)
				{
					pImg->Hide(FALSE);
					pItem->addChild((CUIBase*)pImg);
				}
			}

			m_pArray[eARRAY_PATTERN]->UpdateItem();
		}		
	}
}
