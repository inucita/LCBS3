#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Entities/InternalClasses.h>
#include <Engine/Interface/UIProcessNPC.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Interface/UIProduct.h>


// Date : 2005-03-07,   By Lee Ki-hwan
static int	_iMaxMsgStringChar = 0;

enum eSelProcessNPC
{
	SEL_MINERAL = 0,	// ���� ����
	SEL_CROP,			// �Ĺ� ����
	SEL_ENERGY,			// ���� ����
};

enum eSelMineral
{
	SEL_MINERAL_1 = 0,	// ���� ��
	SEL_MINERAL_2,		// ���� ����
	SEL_MINERAL_3,		// ���� ����
	SEL_MINERAL_4,		// ���� ��Ʈ
	SEL_MINERAL_5,		// ���� ������
};

enum eSelEnergy
{
	SEL_ENERGY_1 = 0,	// E
	SEL_ENERGY_2,		// D
	SEL_ENERGY_3,		// C
	SEL_ENERGY_4,		// B
	SEL_ENERGY_5,		// A
};

enum eSelCrop
{
	SEL_CROP_1 = 0,		// �����
	SEL_CROP_2,			// �ٱ�
	SEL_CROP_3,			// �Ķ���
	SEL_CROP_4,			// ����
	SEL_CROP_5,			// ��
};

// ----------------------------------------------------------------------------
// Name : CUIProcessNPC()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIProcessNPC::CUIProcessNPC()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : ~CUIProcessNPC()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIProcessNPC::~CUIProcessNPC()
{
	Destroy();
	clearVecBtnEX();
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight)
{
    int diff = SKILLLEARN_HEIGHT - PROCESSNPC_HEIGHT;

    SetPos(nX, nY);
    SetSize(nWidth, nHeight);

    _iMaxMsgStringChar = PROCESSNPC_STRING_CHAR_WIDTH / (_pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing());

    // Region of each part
    m_rcTitle.SetRect(0, 0, 216, 22);
    m_rcIcons.SetRect(17, 55, 51, 200);
    //m_rcTab.SetRect( 21, 31, 204, 50 );

    m_rcItem.SetRect(13, 33, 218, 203);
    m_rcDesc.SetRect(13, 203, 218, 331);

    // ProcessNPC Texture ���� - SkillLearn Texture ���
    m_ptdBaseTexture = CreateTexture(CTString("Data\\Interface\\SkillLearn.tex"));
    FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
    FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

    // UV Coordinate of each part
    m_rtBackgroundTop.SetUV(0, 0, 216, PROCESSNPC_TOP_HEIGHT, fTexWidth, fTexHeight);
    m_rtBackgroundBtm.SetUV(0, PROCESSNPC_TOP_HEIGHT + diff, 216, SKILLLEARN_HEIGHT, fTexWidth, fTexHeight);

    m_rtSelOutline.SetUV(218, 80, 250, 112, fTexWidth, fTexHeight);
    m_rtInput.SetUV(218, 136, 248, 149, fTexWidth, fTexHeight);

    // Close button
    m_btnClose.Create(this, CTString(""), 184, 4, 14, 14);
    m_btnClose.SetUV(UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight);
    m_btnClose.SetUV(UBS_CLICK, 219, 15, 233, 29, fTexWidth, fTexHeight);
    m_btnClose.CopyUV(UBS_IDLE, UBS_ON);
    m_btnClose.CopyUV(UBS_IDLE, UBS_DISABLE);

    // ProcessNPC button
    m_btnOK.Create(this, _S(191, "Ȯ��"), 70, 372 - diff, 63, 21);
    m_btnOK.SetUV(UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight);
    m_btnOK.SetUV(UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight);
    m_btnOK.CopyUV(UBS_IDLE, UBS_ON);
    m_btnOK.CopyUV(UBS_IDLE, UBS_DISABLE);

    // Cancel button
    m_btnCancel.Create(this, _S(139, "���"), 141, 372 - diff, 63, 21);
    m_btnCancel.SetUV(UBS_IDLE, 0, 403, 63, 424, fTexWidth, fTexHeight);
    m_btnCancel.SetUV(UBS_CLICK, 66, 403, 129, 424, fTexWidth, fTexHeight);
    m_btnCancel.CopyUV(UBS_IDLE, UBS_ON);
    m_btnCancel.CopyUV(UBS_IDLE, UBS_DISABLE);

    // Up button
    m_btnUP.Create(this, CTString(""), 141, 372 - diff, 7, 7);
    m_btnUP.SetUV(UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight);
    m_btnUP.SetUV(UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight);
    m_btnUP.CopyUV(UBS_IDLE, UBS_ON);
    m_btnUP.CopyUV(UBS_IDLE, UBS_DISABLE);
    m_btnUP.SetEnable(FALSE);

    // Down button
    m_btnDown.Create(this, CTString(""), 141, 372 - diff, 7, 7);
    m_btnDown.SetUV(UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight);
    m_btnDown.SetUV(UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight);
    m_btnDown.CopyUV(UBS_IDLE, UBS_ON);
    m_btnDown.CopyUV(UBS_IDLE, UBS_DISABLE);
    m_btnDown.SetEnable(FALSE);

    // Scroll bar of ProcessNPC Item
    m_sbProcessNPCItem.Create(this, 193, 51, 9, 153);
    m_sbProcessNPCItem.CreateButtons(TRUE, 9, 7, 0, 0, 10);
    m_sbProcessNPCItem.SetScrollPos(0);
    m_sbProcessNPCItem.SetScrollRange(PROCESSNPC_SLOT_ROW_TOTAL);
    m_sbProcessNPCItem.SetCurItemCount(0);
    m_sbProcessNPCItem.SetItemsPerPage(PROCESSNPC_SLOT_ROW);
    // Up button
    m_sbProcessNPCItem.SetUpUV(UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetUpUV(UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.CopyUpUV(UBS_IDLE, UBS_ON);
    m_sbProcessNPCItem.CopyUpUV(UBS_IDLE, UBS_DISABLE);
    // Bar button
    m_sbProcessNPCItem.SetBarTopUV(219, 32, 228, 42, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetBarMiddleUV(219, 43, 228, 45, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetBarBottomUV(219, 46, 228, 56, fTexWidth, fTexHeight);
    // Down button
    m_sbProcessNPCItem.SetDownUV(UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.SetDownUV(UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight);
    m_sbProcessNPCItem.CopyDownUV(UBS_IDLE, UBS_ON);
    m_sbProcessNPCItem.CopyDownUV(UBS_IDLE, UBS_DISABLE);
    // Wheel region
    m_sbProcessNPCItem.SetWheelRect(-181, -1, 180, 154);


    // List box of Precondition description
    m_lbPreconditionDesc.Create(this, 13, 207, 180, 124, _pUIFontTexMgr->GetLineHeight(), 7, 6, 1, FALSE);
    m_lbPreconditionDesc.CreateScroll(TRUE, 0, 0, 9, 124, 9, 7, 0, 0, 10);

    // Up button
    m_lbPreconditionDesc.SetScrollUpUV(UBS_IDLE, 219, 62, 228, 69, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollUpUV(UBS_CLICK, 230, 62, 239, 69, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.CopyScrollUpUV(UBS_IDLE, UBS_ON);
    m_lbPreconditionDesc.CopyScrollUpUV(UBS_IDLE, UBS_DISABLE);
    // Bar button
    m_lbPreconditionDesc.SetScrollBarTopUV(219, 32, 228, 42, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollBarMiddleUV(219, 43, 228, 45, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollBarBottomUV(219, 46, 228, 56, fTexWidth, fTexHeight);
    // Down button
    m_lbPreconditionDesc.SetScrollDownUV(UBS_IDLE, 219, 71, 228, 78, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.SetScrollDownUV(UBS_CLICK, 230, 71, 239, 78, fTexWidth, fTexHeight);
    m_lbPreconditionDesc.CopyScrollDownUV(UBS_IDLE, UBS_ON);
    m_lbPreconditionDesc.CopyScrollDownUV(UBS_IDLE, UBS_DISABLE);

    m_bSatisfied = FALSE;
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
    SetPos((pixMaxI + pixMinI - GetWidth()) / 2, (pixMaxJ + pixMinJ - GetHeight()) / 2);
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ)
{
    if (m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
        m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ)
        ResetPosition(pixMinI, pixMinJ, pixMaxI, pixMaxJ);
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Clear()
{
    m_iResourceType			= -1;
    m_iResourceSubType		= -1;
    m_nProcessItemCount		= 1;

    m_nSelectProcessNPCItem	= -1;
    m_bWaitProcessNPCResult	= TRUE;
    m_bSatisfied			= FALSE;

    m_nNeedItemCount		= 0;

    memset(m_NeedItems, 0, sizeof(CNeedItems) * MAX_MAKE_ITEM_MATERIAL);

    //m_StrProcessNPCType.Clear ();

    m_bWaitProcessNPCResult = FALSE;
    m_lbPreconditionDesc.ResetAllStrings();

    clearVecBtnEX();
}

// ----------------------------------------------------------------------------
// Name : InitProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::InitProcessNPC()
{
    clearVecBtnEX();

    // �ӽ� ó��
    int nProcessType = m_iResourceType;
    int nProcessSubType = m_iResourceSubType;

    // ����  Type ����
    //m_StrProcessNPCType = szItemName;

    CUIIcon* pIcon;
    int bit = 0x00000001;

    //!! ���� ������ ���� �Ҽ� �ִ� ������ �˻�
    CItemData::_map::iterator	iter = CItemData::_mapdata.begin();
    CItemData::_map::iterator	eiter = CItemData::_mapdata.end();

    for (; iter != eiter; ++iter)
    {
        CItemData* pItemData = (*iter).second;

        if (pItemData == NULL)
            continue;

        if (pItemData->GetFlag() & ITEM_FLAG_MADE)
        {
            if (pItemData->GetType() == CItemData::ITEM_ETC &&
                pItemData->GetSubType() == CItemData::ITEM_ETC_PROCESS &&  // ����ǰ �̶�� ���
                pItemData->GetProcessType() == nProcessType
                && pItemData->GetProcessSubType() == nProcessSubType
               )
            {
                pIcon = new CUIIcon;
                pIcon->Create(this, 0, 0, BTN_SIZE, BTN_SIZE, UI_PROCESSNPC, UBET_ITEM);
                pIcon->setData(UBET_ITEM, pItemData->GetItemIndex());
                m_vecIcons.push_back(pIcon);
            }
        }
    }

    AddString(_S(561, "����ǰ�� ������ �ֽʽÿ�."), COLOR_TEXT);

    m_sbProcessNPCItem.SetScrollPos(0);
    m_sbProcessNPCItem.SetCurItemCount(m_vecIcons.size());

    m_btnOK.SetEnable(m_bSatisfied);
}

// ----------------------------------------------------------------------------
// Name : OpenProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessNPC()
{
    CUIManager* pUIManager = CUIManager::getSingleton();

    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_REQ);

    pUIManager->CreateMessageBoxL(_S(1786, "���� NPC") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_REQ);

    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1787, "������ �����Ϸ� �Դ°�?"), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1788, "� ����ǰ�̵� �ðܸ� �ָ� ���� �Ϻ��ϰ� ������ �ְڳ�. ���� ���� �԰� ��ƾ� �ϴ� ����δ� �����."), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, TRUE, _S(1789, "�����Ḧ �ִ� ���� �ȴٸ� ��¿ �� ������ �� ��ó���� ����ŭ �پ ��������� ������ �ִ� ����� ���� ���߾�.  �� ������ ����."), -1, 0xA3A1A3FF);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1790, "���� ����"), SEL_MINERAL);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1791, "�Ĺ� ����"), SEL_CROP);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1792, "���� ����"), SEL_ENERGY);
    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_REQ, FALSE, _S(1220, "����Ѵ�."));
}

// ----------------------------------------------------------------------------
// Name : OpenProcessList()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::OpenProcessList(int iType, int iSubType)
{
    // If this is already exist
    if (IsVisible() == TRUE)
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();

    // If inventory is already locked
    if (pUIManager->GetInventory()->IsLocked())
    {
        pUIManager->GetInventory()->ShowLockErrorMessage();
        return;
    }

    Clear();

    if (!pUIManager->GetInventory()->IsVisible())
        pUIManager->GetInventory()->ToggleVisible();

    //m_nProcessNPCText	= nItemIndex;
    m_iResourceType = iType;
    m_iResourceSubType = iSubType;

    pUIManager->RearrangeOrder(UI_PROCESSNPC, TRUE);

    InitProcessNPC();
}

// ----------------------------------------------------------------------------
// Name : CloseProcessNPC()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::CloseProcessNPC()
{
    // Close refine
    CUIManager::getSingleton()->RearrangeOrder(UI_PROCESSNPC, FALSE);

    Clear();
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::Render()
{
    CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

    // Set skill learn texture
    pDrawPort->InitTextureData(m_ptdBaseTexture);

    // Add render regions -----------------------------------------------------------------------------------------------
    // Background up
    pDrawPort->AddTexture(m_nPosX, m_nPosY, m_nPosX + m_nWidth, m_nPosY + PROCESSNPC_TOP_HEIGHT,
                          m_rtBackgroundTop.U0, m_rtBackgroundTop.V0, m_rtBackgroundTop.U1, m_rtBackgroundTop.V1,
                          0xFFFFFFFF);

    // Background down
    pDrawPort->AddTexture(m_nPosX, m_nPosY + PROCESSNPC_TOP_HEIGHT, m_nPosX + m_nWidth, m_nPosY + m_nHeight,
                          m_rtBackgroundBtm.U0, m_rtBackgroundBtm.V0, m_rtBackgroundBtm.U1, m_rtBackgroundBtm.V1,
                          0xFFFFFFFF);

    // Render Title Text
    pDrawPort->PutTextEx(_S(560, "����") , m_nPosX + PROCESSNPC_TITLE_TEXT_OFFSETX,
                         m_nPosY + PROCESSNPC_TITLE_TEXT_OFFSETY);

    // Render ProcessNPC Type
    //pDrawPort->PutTextExCX( m_StrProcessNPCType, m_nPosX + 106, m_nPosY + 35, COLOR_TITLE );


    // ProcessNPC Item Scroll bar
    m_sbProcessNPCItem.Render();

    // List box of Need Item Desc
    m_lbPreconditionDesc.Render();

    // Close button
    m_btnClose.Render();

    // ProcessNPC button
    m_btnOK.Render();

    // Cancel button
    m_btnCancel.Render();

    pDrawPort->FlushRenderingQueue();


    // Render ProcessNPC Item
    CTString m_strShortDesc;
    int	nX = SLEARN_SLOT_SX;
    int	nY = SLEARN_SLOT_SY;

    int	iEnd = m_sbProcessNPCItem.GetScrollPos() + PRODUCT_SLOT_ROW;

    if (iEnd > m_vecIcons.size())
        iEnd = m_vecIcons.size();

    for (int i = m_sbProcessNPCItem.GetScrollPos(); i < iEnd; i++)
    {
        if (m_vecIcons[i] != NULL && !m_vecIcons[i]->IsEmpty())
        {
            // render Item
            m_vecIcons[i]->SetPos(nX, nY);
            m_vecIcons[i]->Render(pDrawPort);

            // render Item desc
            CItemData* pItemData = _pNetwork->GetItemData(m_vecIcons[i]->getIndex());
            const char* szItemName = _pNetwork->GetItemName(m_vecIcons[i]->getIndex());
            CTString strOutput = szItemName;

            if (i == m_nSelectProcessNPCItem) //���� ���õǾ� �ִٸ�
            {
                int index = m_nSelectProcessNPCItem - m_sbProcessNPCItem.GetScrollPos();
                char buf[32];
                sprintf(buf, _S(766, "%d ��"), m_nProcessItemCount);
                strOutput += buf;
            }

            pDrawPort->PutTextExCX(strOutput, m_nPosX + 114, m_nPosY + nY + 8, 0xC3C3C3ff);

            nY += SLEARN_SLOT_OFFSETY;
        }
    }

    // Flush all render text queue
    pDrawPort->EndTextEx();

    pDrawPort->InitTextureData(m_ptdBaseTexture);

    nX = SLEARN_SLOT_SX;
    nY = SLEARN_SLOT_SY;

    m_btnDown.SetEnable(FALSE);
    m_btnUP.SetEnable(FALSE);

    // render sel outline
    if (m_nSelectProcessNPCItem != -1)
    {
        int BoxX, BoxY;

        int nOffset = m_nSelectProcessNPCItem - m_sbProcessNPCItem.GetScrollPos();

        if (nOffset >= 0 && nOffset < PROCESSNPC_SLOT_ROW)
        {
            BoxX = m_nPosX + nX - 1 ;
            BoxY = m_nPosY + nY + (SLEARN_SLOT_OFFSETY *  nOffset) - 1;

            pDrawPort->AddTexture(BoxX, BoxY,	BoxX + 34, BoxY + 34,
                                  m_rtSelOutline.U0, m_rtSelOutline.V0, m_rtSelOutline.U1, m_rtSelOutline.V1,
                                  0xffffffff);

            // Input
            pDrawPort->AddTexture(m_nPosX + 156, BoxY + 19, m_nPosX + 156 + 31, BoxY + 19 + 14,
                                  m_rtInput.U0, m_rtInput.V0, m_rtInput.U1, m_rtInput.V1, 0xFFFFFFFF);

            m_btnUP.SetPos(180, nY + (SLEARN_SLOT_OFFSETY *  nOffset) + 19 + 13 - 14);
            m_btnDown.SetPos(180, nY + (SLEARN_SLOT_OFFSETY *  nOffset) + 19 + 13 - 7);

            m_btnUP.SetEnable(TRUE);
            m_btnDown.SetEnable(TRUE);
            m_btnUP.Render();
            m_btnDown.Render();
        }
    }

    pDrawPort->FlushRenderingQueue();
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIProcessNPC::MouseMessage(MSG* pMsg)
{
    WMSG_RESULT	wmsgResult;

    // Title bar
    static BOOL bTitleBarClick = FALSE;

    // Mouse point
    static int	nOldX, nOldY;
    int	nX = LOWORD(pMsg->lParam);
    int	nY = HIWORD(pMsg->lParam);

    // Mouse message
    switch (pMsg->message)
    {
    case WM_MOUSEMOVE:
        {
            if (IsInside(nX, nY))
            {
                CUIManager::getSingleton()->SetMouseCursorInsideUIs();

                int	iRowS = m_sbProcessNPCItem.GetScrollPos();
                int	iRowE = iRowS + PROCESSNPC_SLOT_ROW;

                if (m_vecIcons.size() < iRowE)
                {
                    iRowE = m_vecIcons.size();
                }

                if (IsInsideRect(nX, nY, m_rcItem))
                {
                    for (int iRow = iRowS; iRow < iRowE; iRow++)
                    {
                        m_vecIcons[iRow]->MouseMessage(pMsg);
                    }
                }
            }


            // Move refine
            if (bTitleBarClick && (pMsg->wParam & MK_LBUTTON))
            {
                int	ndX = nX - nOldX;
                int	ndY = nY - nOldY;
                nOldX = nX;
                nOldY = nY;

                Move(ndX, ndY);

                return WMSG_SUCCESS;
            }
            // Close button
            else if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            // OK button
            else if (m_btnOK.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            // Cancel button
            else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            // List box of skill desc
            else if (m_sbProcessNPCItem.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            else if (m_lbPreconditionDesc.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            else if (m_btnUP.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
            else if (m_btnDown.MouseMessage(pMsg) != WMSG_FAIL)
                return WMSG_SUCCESS;
        }

        break;

    case WM_LBUTTONDOWN:
        {
            if (IsInside(nX, nY))
            {
                nOldX = nX;
                nOldY = nY;

                // Close button
                if (m_btnClose.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    // Nothing
                }
                // Title bar
                else if (IsInsideRect(nX, nY, m_rcTitle))
                {
                    bTitleBarClick = TRUE;
                }
                // OK button
                else if (m_btnOK.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    // Nothing
                }
                // Cancel button
                else if (m_btnCancel.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    // Nothing
                }
                else if (m_sbProcessNPCItem.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    return WMSG_SUCCESS;
                }
                else if (m_btnUP.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    ++m_nProcessItemCount;

                    if (m_nProcessItemCount > 10) m_nProcessItemCount = 10;

                    SelectItem(m_nSelectProcessNPCItem);
                    return WMSG_SUCCESS;
                }
                else if (m_btnDown.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    --m_nProcessItemCount;

                    if (m_nProcessItemCount < 1) m_nProcessItemCount = 1;

                    SelectItem(m_nSelectProcessNPCItem);
                    return WMSG_SUCCESS;
                }

                CUIManager::getSingleton()->RearrangeOrder(UI_PROCESSNPC, TRUE);
                return WMSG_SUCCESS;
            }
        }
        break;

    case WM_LBUTTONUP:
        {
            // If holding button doesn't exist
			if (UIMGR()->GetDragIcon() == NULL)
            {
                // Title bar
                bTitleBarClick = FALSE;

                // If refine isn't focused
                if (!IsFocused())
                    return WMSG_FAIL;

                // Close button
                if ((wmsgResult = m_btnClose.MouseMessage(pMsg)) != WMSG_FAIL)
                {
                    if (wmsgResult == WMSG_COMMAND)
                        CloseProcessNPC();

                    return WMSG_SUCCESS;
                }
                // OK button
                else if ((wmsgResult = m_btnOK.MouseMessage(pMsg)) != WMSG_FAIL)
                {
                    if (wmsgResult == WMSG_COMMAND)
                        SendProcessNPCReq();

                    return WMSG_SUCCESS;
                }
                // Cancel button
                else if ((wmsgResult = m_btnCancel.MouseMessage(pMsg)) != WMSG_FAIL)
                {
                    if (wmsgResult == WMSG_COMMAND)
                        CloseProcessNPC();

                    return WMSG_SUCCESS;
                }
                else if (m_sbProcessNPCItem.MouseMessage(pMsg) != WMSG_FAIL)
                {
                    return WMSG_SUCCESS;
                }
                else if (m_btnUP.MouseMessage(pMsg) != WMSG_FAIL)
                    return WMSG_SUCCESS;
                else if (m_btnDown.MouseMessage(pMsg) != WMSG_FAIL)
                    return WMSG_SUCCESS;
                else if (IsInsideRect(nX, nY, m_rcIcons))
                {
                    int	iRowS = m_sbProcessNPCItem.GetScrollPos();
                    int	iRowE = iRowS + PROCESSNPC_SLOT_ROW;

                    if (m_vecIcons.size() < iRowE)
                    {
                        iRowE = m_vecIcons.size();
                    }

                    for (int iRow = iRowS; iRow < iRowE; iRow++)
                    {
                        if (m_vecIcons[iRow]->MouseMessage(pMsg) != WMSG_FAIL)
                        {
                            if (m_nSelectProcessNPCItem != iRow) m_nProcessItemCount = 1;

                            m_nSelectProcessNPCItem = iRow;

                            SelectItem(m_nSelectProcessNPCItem);
                            return WMSG_SUCCESS;
                        }
                    }
                }

            }
        }
        break;

    case WM_LBUTTONDBLCLK:
        {
            if (IsInside(nX, nY))
                return WMSG_SUCCESS;
        }

        break;

    case WM_MOUSEWHEEL:
        {
            if (IsInside(nX, nY))
            {
                if (IsInsideRect(nX, nY, m_rcItem))
                {
                    return m_sbProcessNPCItem.MouseMessage(pMsg);
                }
                else if (IsInsideRect(nX, nY, m_rcDesc))
                {
                    return m_lbPreconditionDesc.MouseMessage(pMsg);
                }

                return WMSG_SUCCESS;
            }
        }
        break;
    }

    return WMSG_FAIL;
}

// ----------------------------------------------------------------------------
// Name : SelectItem()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SelectItem(int _nIndex)
{
    if (_nIndex == -1)
    {
        _nIndex = m_nSelectProcessNPCItem;

        if (_nIndex == -1) return;
    }

    if (_nIndex >= m_vecIcons.size())
        return;

    BOOL bNeedItem			= FALSE;
    BOOL bNeedItemCount		= FALSE;

    m_lbPreconditionDesc.ResetAllStrings();

    // ���� �ϰ��� �ϴ� ������ ���
    if (m_vecIcons[_nIndex]->getIndex() == -1) return;

    CItemData*		pProcessNPCItemData	= _pNetwork-> GetItemData(m_vecIcons[_nIndex]->getIndex());

    // �ʿ� ������
    int nIndexTemp = 0;

    // �ʿ� ������ ������ ���� ���
    nIndexTemp = pProcessNPCItemData->GetNeedItemIndex(0);

    if (nIndexTemp == -1) return;

    m_NeedItems[0].ItemData	= _pNetwork->GetItemData(nIndexTemp);
    m_NeedItems[0].llCount	= pProcessNPCItemData->GetNeedItemCount(0);
    __int64 myMoney = 0;
    myMoney = _pNetwork->MyCharacterInfo.money;
    // �κ����� �ʿ� ������ �˻� ( nUniIndex ó�� ������ )

    SWORD nTab, nIndex;

    if (ItemHelp::HaveItem(m_NeedItems[0].ItemData->GetItemIndex(), &nTab, &nIndex, m_NeedItems[0].llCount))
    {
        bNeedItem = TRUE;
        bNeedItemCount = TRUE;

        m_NeedItems[0].MatTab = nTab;
        m_NeedItems[0].inven_idx = nIndex;
    }

    // ���� ���
    CTString strTitle;
    CTString strSpace = "";

    // �ʿ� ��ų ���
    //AddString ( _S( 562, "�ʿ� ��ų" ), COLOR_SUB_TITLE );
    //AddString ( strSpace + NeedSSkill.GetName(), bNeedSSkill? COLOR_TEXT : COLOR_NONE );
    //m_bSatisfied = bNeedSSkill;

    //AddString ( CTString ( " " ) );

    // �ʿ� ��ǰ
    switch (m_iResourceType)
    {
    case CItemData::PROCESS_DOC_STONE :
        strTitle = _S(563, "�ʿ� ����");
        break;

    case CItemData::PROCESS_DOC_PLANT :
        strTitle = _S(564, "�ʿ� ũ��");
        break;

    case CItemData::PROCESS_DOC_ELEMENT :
        strTitle = _S(565, "�ʿ� ����");
        break;
    }

    AddString(strTitle, COLOR_SUB_TITLE);


    bNeedItem = bNeedItem && bNeedItemCount;
    const char* szItemName = NULL;

    if (m_NeedItems[0].ItemData != NULL)
    {
        szItemName = _pNetwork->GetItemName(m_NeedItems[0].ItemData->GetItemIndex());
        strTitle.PrintF(_S(576, "%s : %d�� "), strSpace + szItemName
                        , m_nProcessItemCount * m_NeedItems[0].llCount);
    }

    AddString(strTitle, bNeedItem ? COLOR_TEXT : COLOR_NONE);
    m_bSatisfied = bNeedItem;

    AddString(CTString(""), COLOR_TEXT);
    AddString(_S(1793, "������"), COLOR_SUB_TITLE);
    CTString strMoney;
    static int s_iMoney[3][5] =
    {
        {10, 21, 40, 63, 94}
        , {10, 21, 40, 63, 94}
        , {10, 21, 40, 63, 94}
    };
    __int64 needMoney = s_iMoney[m_iResourceType][m_iResourceSubType] * m_nProcessItemCount * m_NeedItems[0].llCount;
    strMoney.PrintF("%I64d %s", needMoney, _S(1762, "����"));
    AddString(strMoney, needMoney <= myMoney ? COLOR_TEXT : COLOR_NONE);

    m_bSatisfied = m_bSatisfied && needMoney <= myMoney;
    m_btnOK.SetEnable(m_bSatisfied);
}


// ----------------------------------------------------------------------------
// Name : AddString()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::AddString(CTString& strText, COLOR colText)
{
    int nLength =  strText.Length();

    if (nLength >= _iMaxMsgStringChar)
    {
        nLength -= nLength - _iMaxMsgStringChar;

        do
        {
            if (strText[nLength] == ' ')
            {
                break;
            }

        }
        while (nLength--);

        CTString strTemp2, strTemp;

        strText.Split(nLength, strTemp2, strTemp);
        m_lbPreconditionDesc.AddString(0, strTemp2, colText);

        strTemp2.PrintF("%s", strTemp);

        m_lbPreconditionDesc.AddString(0, strTemp2, colText);

    }
    else
    {
        m_lbPreconditionDesc.AddString(0, strText, colText);
    }

}

// ========================================================================= //
//                           Send message functions                          //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : SendProcessNPCReq()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::SendProcessNPCReq()
{
    if (m_bWaitProcessNPCResult == TRUE)
        return;

    if (m_nSelectProcessNPCItem >= m_vecIcons.size())
        return;

    CUIManager* pUIManager = CUIManager::getSingleton();

    if (pUIManager->IsCSFlagOn(CSF_TELEPORT))
    {
        pUIManager->GetChattingUI()->AddSysMessage(_S(942, "���� �̵��߿��� ������ �� �� �����ϴ�.") , SYSMSG_ERROR);
        return;
    }

    _pNetwork->SendNPCProcessMessage(m_vecIcons[m_nSelectProcessNPCItem]->getIndex(), m_nProcessItemCount);

    m_bWaitProcessNPCResult = TRUE;

    m_btnOK.SetEnable(!m_bWaitProcessNPCResult);
}

// ========================================================================= //
//                         Receive message functions                         //
// ========================================================================= //

// ----------------------------------------------------------------------------
// Name : ProcessNPCRep()
// Desc :
// ----------------------------------------------------------------------------
void CUIProcessNPC::ProcessNPCRep(LONG processItemDBIndex, LONG count, LONG result)
{
    CUIManager* pUIManager = CUIManager::getSingleton();

    m_bWaitProcessNPCResult = FALSE;

    // Close message box
    pUIManager->CloseMessageBox(MSGCMD_PROCESS_REP);

    // Show result
    CTString	strMessage;
    bool bRetProcessNPCList = false;
    CTString strTitle;
    CTString strContent;

    int commandCode = -1;

    strTitle = _S(1794, "���� ����");

    switch (result)
    {
    case MSG_ITEM_PROCESS_NPC_ERROR_OK:		// ����
        {
            strTitle = _S(1795, "���� ����");
            strContent = _S(1796, "��ǰ ������ �����Ͽ����ϴ�. �����Ḧ �����մϴ�.");
            commandCode = MSGCMD_PROCESS_REP;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_FAIL:	// ����
        {
            strContent = _S(1797, "��ǰ ������ �����Ͽ����ϴ�. ������� �������� �ʽ��ϴ�.");
            commandCode = MSGCMD_PROCESS_REP;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_COUNT:	// ī��Ʈ ����
        {
            strContent = _S(1798, "������ ����� ������ �Է��Ͽ� �ֽʽÿ�.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_NOHAVE:	// ��� ����
        {
            strContent = _S(1799, "��ᰡ �����մϴ�. Ȯ���ϰ� �ٽ� �õ��Ͽ� �ֽʽÿ�.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;

    case MSG_ITEM_PROCESS_NPC_ERROR_MONEY:	// ���� ����
        {
            strContent = _S(1800, "�����ᰡ �����մϴ�. Ȯ���ϰ� �ٽ� �õ��Ͽ� �ֽʽÿ�.");
            commandCode = MSGCMD_PROCESS_NOTIFY;
        }
        break;
    }

    m_iLastResourceType = m_iResourceType;
    m_iLastResourceSubType = m_iResourceSubType;
    CloseProcessNPC();

    pUIManager->CloseMessageBox(MSGCMD_PROCESS_NOTIFY);
    CUIMsgBox_Info	MsgBoxInfo;
    MsgBoxInfo.SetMsgBoxInfo(strTitle, UMBS_OK, UI_PROCESSNPC, commandCode);
    MsgBoxInfo.AddString(strContent);
    pUIManager->CreateMessageBox(MsgBoxInfo);

    m_bWaitProcessNPCResult = FALSE;
}

void CUIProcessNPC::MsgBoxCommand(int nCommandCode, BOOL bOK, CTString& strInput)
{
    switch (nCommandCode)
    {
    case MSGCMD_PROCESS_NOTIFY:
        {
        } break;

    case MSGCMD_PROCESS_REP:
        {
            OpenProcessList(m_iLastResourceType, m_iLastResourceSubType);
        }
        break;
    }
}

void CUIProcessNPC::MsgBoxLCommand(int nCommandCode, int nResult)
{
    switch (nCommandCode)
    {
    case MSGLCMD_PROCESSNPC_REQ:
        {
            CUIManager* pUIManager = CUIManager::getSingleton();

            switch (nResult)
            {
            case SEL_MINERAL:		// ���� ����
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_MINERAL_REQ);

                    pUIManager->CreateMessageBoxL(_S(1790, "���� ����") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_MINERAL_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1801, "�׷� �� �����߳�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1802, "�� ���� � ������ ������ ������ �ѹ� ��󺸶�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, TRUE, _S(1803, "�� ����� ������ ���ÿ� �����ϴ� ���� �ƴϳ�. ���� ���и� �ϱ⵵ ����. ���� �����Ѵٸ� ������� ���� �ʳ�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1804, "���� �� ����"), SEL_MINERAL_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1805, "���� ���� ����"), SEL_MINERAL_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1806, "���� ���� ����"), SEL_MINERAL_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1807, "���� ��Ʈ ����" ), SEL_MINERAL_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S( 1808, "���� ������ ����" ), SEL_MINERAL_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_MINERAL_REQ, FALSE, _S(1220, "����Ѵ�."));
                }
                break;

            case SEL_ENERGY:		// ���� ����
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_ENERGY_REQ);

                    pUIManager->CreateMessageBoxL(_S(1792, "���� ����") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_ENERGY_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1801, "�׷� �� �����߳�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1809, "�� ���� � ���Ҹ� ������ ������ �ѹ� ��󺸶�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, TRUE, _S(1810, "�� ����� ������ ������ �����ϴ� ���� �ƴϳ�. ���� ���и� �ϱ⵵ ����. ���� �����Ѵٸ� ������� ���� �ʳ�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1811, "E��� ���� ����"), SEL_ENERGY_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1812, "D��� ���� ����"), SEL_ENERGY_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1813, "C��� ���� ����"), SEL_ENERGY_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1814, "B��� ���� ����" ), SEL_ENERGY_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S( 1815, "A��� ���� ����" ), SEL_ENERGY_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_ENERGY_REQ, FALSE, _S(1220, "����Ѵ�."));
                }
                break;

            case SEL_CROP:			// �Ĺ� ����.
                {
                    pUIManager->CloseMessageBoxL(MSGLCMD_PROCESSNPC_CROP_REQ);

                    pUIManager->CreateMessageBoxL(_S(1791, "�Ĺ� ����") , UI_PROCESSNPC, MSGLCMD_PROCESSNPC_CROP_REQ);

                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1801, "�׷� �� �����߳�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1816, "�� ���� ũ���� � �κ��� ������ ������ �ѹ� ��󺸶�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, TRUE, _S(1817, "�� ����� ������ ������ �����ϴ� ���� �ƴϳ�. ���� ���и� �ϱ⵵ ����. ���� �����Ѵٸ� ������� ���� �ʳ�."), -1, 0xA3A1A3FF);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1818, "ũ���� ����� ����"), SEL_CROP_1);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1819, "ũ���� �ٱ� ����"), SEL_CROP_2);
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1820, "ũ���� �Ķ��� ����"), SEL_CROP_3);
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1821, "ũ���� ���� ����" ), SEL_CROP_4 );
                    //pUIManager->AddMessageBoxLString( MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S( 1822, "ũ���� �� ����" ), SEL_CROP_5 );
                    pUIManager->AddMessageBoxLString(MSGLCMD_PROCESSNPC_CROP_REQ, FALSE, _S(1220, "����Ѵ�."));
                }
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_MINERAL_REQ:
        {
            switch (nResult)
            {
            case SEL_MINERAL_1:		// ���� ����
            case SEL_MINERAL_2:		// ���� ����
            case SEL_MINERAL_3:		// ���� ����
            case SEL_MINERAL_4:		// ���� ����
            case SEL_MINERAL_5:		// ���� ����
                OpenProcessList(SEL_MINERAL, nResult);
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_ENERGY_REQ:
        {
            switch (nResult)
            {
            case SEL_ENERGY_1:		// ���� ����
            case SEL_ENERGY_2:		// ���� ����
            case SEL_ENERGY_3:		// ���� ����
            case SEL_ENERGY_4:		// ���� ����
            case SEL_ENERGY_5:		// ���� ����
                {
                    OpenProcessList(SEL_ENERGY, nResult);
                }
                break;
            }
        }
        break;

    case MSGLCMD_PROCESSNPC_CROP_REQ:
        {
            switch (nResult)
            {
            case SEL_CROP_1:		// ũ�� ����
            case SEL_CROP_2:		// ũ�� ����
            case SEL_CROP_3:		// ũ�� ����
            case SEL_CROP_4:		// ũ�� ����
            case SEL_CROP_5:		// ũ�� ����
                {
                    OpenProcessList(SEL_CROP, nResult);
                }
                break;
            }
        }
        break;
    }
}

//------------------------------------------------------------------------

void CUIProcessNPC::clearVecBtnEX()
{
    vecBtnEX_Iter iter = m_vecIcons.begin();
    vecBtnEX_Iter eiter = m_vecIcons.end();

    for (; iter != eiter; ++iter)
    {
        SAFE_DELETE(*iter);
    }

    m_vecIcons.clear();
}
