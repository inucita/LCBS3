
#include "StdH.h"
#include <Engine/Interface/UIInternalClasses.h>
#include "UIBase.h"
#include <Engine/Interface/UIManager.h>

#pragma warning(disable: 4172)

stTooltipInfo CUIBase::temp_tooltip_data("", DEF_UI_COLOR_WHITE);

CUIBase::CUIBase()
    : m_pParent(NULL)
    , m_pTexData(NULL)
    , m_nPosX(0)
    , m_nPosY(0)
    , m_nOrigX(0)
    , m_nOrigY(0)
    , m_bHide(FALSE)
    , m_strPID("")
    , m_bInherit(true)
    , m_bEnter(false)
    , m_eAlign_h(eALIGN_H_LEFT)
    , m_eAlign_v(eALIGN_V_TOP)
    , m_nTooltipWidth(0)
	, m_func(NULL)
	, m_funcUp(NULL)
	, m_dwWndState( UWS_ENABLE | UWS_VISIBLE )
	, m_eChildItemState(eSTATE_IDLE)
	, m_nWidth(0)
	, m_nHeight(0)
#ifdef	UI_TOOL
    , m_bSelect(false)
    , m_nTooltipIndex(-1)
#endif	// UI_TOOL
{
    setType(eUI_CONTROL_BASE);
    CmdErase();
}

CUIBase::~CUIBase()
{
    Destroy();
}

void CUIBase::postInit()
{
    updatePosition();
}

void CUIBase::Destroy()
{
    STOCK_RELEASE(m_pTexData);

    deleteChildAll();

    SAFE_DELETE(m_pCmd);
    SAFE_DELETE(m_pCmdOn);
    SAFE_DELETE(m_pCmdUp);
    SAFE_DELETE(m_pCmdR);
    SAFE_DELETE(m_pCmdRUp);
    SAFE_DELETE(m_pCmdOnEnter);
    SAFE_DELETE(m_pCmdOnLeave);
    SAFE_DELETE(m_pCmdDBL);
    SAFE_DELETE(m_pCmdWheelUp);
    SAFE_DELETE(m_pCmdWheelDown);

    CmdErase();
	clearTooltipCont();
}

CUIBase* CUIBase::Clone()
{
	CUIBase* pUI = new CUIBase(*this);

	CloneChild(pUI);

    return pUI;
}

//------------------------------------------------------------------------

bool CUIBase::addChild(CUIBase* child)
{
    if (child == NULL)
        return false;

    m_VecChild.push_back(child);
    child->setParent(this);

    return true;
}

bool CUIBase::insertChild( CUIBase* child, int pos )
{
	if (child == NULL)
		return false;

	if (pos < 0 || pos >= m_VecChild.size())
		return false;

	vec_uinode_iter iter = m_VecChild.begin();

	m_VecChild.insert(iter + pos, child);
	child->setParent(this);

	return true;
}

bool CUIBase::deleteChild(const char* child_name)
{
    return true;
}

bool CUIBase::deleteChildAll()
{
    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        SAFE_DELETE((*iter));
    }

    m_VecChild.clear();

    return true;
}

bool CUIBase::deleteChild(int vPos)
{
    int iMax = m_VecChild.size();

    if (vPos >= iMax || vPos < 0)
        return false;

    vec_uinode_iter iter = m_VecChild.begin();

    CUIBase* pDel = (*(iter + vPos));
    m_VecChild.erase(iter + vPos);
    SAFE_DELETE(pDel);

    return true;
}

bool CUIBase::deleteChild(CUIBase* pChild, bool bDelete)
{
    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) == pChild)
        {
            m_VecChild.erase(iter);

            if (bDelete == true)
                SAFE_DELETE(pChild);

            return true;
        }
    }

    return false;
}

bool CUIBase::eraseChild(CUIBase* pChild)
{
    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) == pChild)
        {
            m_VecChild.erase(iter);
            break;
        }
    }

    return true;
}

void CUIBase::deleteChildList()
{
#ifdef	_DEBUG

    if (m_VecChild.size() <= 0)
        return;

#endif	// _DEBUG

    m_VecChild.clear();
}

//------------------------------------------------------------------------

void CUIBase::setID(const char* strID)
{
    if (strID != NULL)
        m_strID = strID;
}

CUIBase* CUIBase::findUI(const char* strID)
{
    if (m_strID.compare(strID) == 0)
        return this;

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    CUIBase*	find_node = NULL;

    for (; iter != eiter; ++iter)
    {
        find_node = (*iter)->findUI(strID);

        if (find_node)
            return find_node;
    }

    return NULL;
}

void CUIBase::setTexString(const char* strTex)
{
    m_strTex = strTex;

    if (m_strTex.empty() == false)
    {
        std::string strPath;

        if (FileExists(m_strTex.c_str()) == FALSE)
        {
            CUIManager* pUIManager = CUIManager::getSingleton();
            strPath = pUIManager->GetFullPath(m_strTex);
        }
        else
        {
            strPath = m_strTex;
        }

        ASSERT(FileExists(strPath.c_str()));

        m_pTexData = CreateTexture(strPath.c_str());
    }
}

void CUIBase::setParent(CUIBase* parent)
{
	m_pParent = parent;
}

CTString& CUIBase::getText(INDEX idx)
{
    CUIManager* pUIMgr = CUIManager::getSingleton();

    if (pUIMgr)
    {
        CTString& ret = pUIMgr->GetString(idx);

        if (_pNetwork->m_ubGMLevel > 1)
        {
            if (ret.IsEmpty() == TRUE)
                ret.PrintF("[%d]Client", idx);
        }

        return ret;
    }

    return CTString("");
}

void CUIBase::InitPos(int x, int y, int w, int h)
{
    m_nPosX = x;
    m_nOrigX = x;
    m_nPosY = y;
    m_nOrigY = y;
    m_nWidth = w;
    m_nHeight = h;
}

void CUIBase::SetPos(int nX, int nY)
{
    m_nPosX = nX;
    m_nOrigX = nX;
    m_nPosY = nY;
    m_nOrigY = nY;
}

void CUIBase::SetSize(int Width, int Height, bool bUpdateChild)
{
    m_nWidth = Width;
    m_nHeight = Height;

    updatePosition(bUpdateChild);
}

void CUIBase::updatePosition(bool bUpdateChild)
{
    int		nPX = 0, nPY = 0;
    int		nPW = 0, nPH = 0;

    OnUpdatePosition();

    CUIBase* pParent = getParent();

    if (pParent == NULL)
    {
        CUIManager* pUIMgr = CUIManager::getSingleton();
		nPW = pUIMgr->GetWidth();
		nPH = pUIMgr->GetHeight();
    }
    else
    {
        nPX = pParent->GetAbsPosX();
        nPY = pParent->GetAbsPosY();
        nPW = pParent->GetWidth();
        nPH = pParent->GetHeight();
    }

    if (m_eAlign_h == eALIGN_H_LEFT)
    {
        m_nPosX = m_nOrigX;
    }
    else if (m_eAlign_h == eALIGN_H_CENTER)
    {
        m_nPosX = (int)(nPW * 0.5f) - (int)(m_nWidth * 0.5f) + m_nOrigX;
    }
    else if (m_eAlign_h == eALIGN_H_RIGHT)
    {
        m_nPosX = nPW - m_nWidth + m_nOrigX;
    }

    if (m_eAlign_v == eALIGN_V_TOP)
    {
        m_nPosY = m_nOrigY;
    }
    else if (m_eAlign_v == eALIGN_V_CENTER)
    {
        m_nPosY = (int)(nPH * 0.5f) - (int)(m_nHeight * 0.5f) + m_nOrigY;
    }
    else if (m_eAlign_v == eALIGN_V_BOTTOM)
    {
        m_nPosY = nPH - m_nHeight + m_nOrigY;
    }

	// ��ġ ��� �Ŀ� ȣ���ϴ� �Լ�
	OnUpdatePositionPost();

    if (bUpdateChild == true)
        updatePositionChild(bUpdateChild);
}

void CUIBase::updatePositionChild(bool bUpdateChild)
{
    vec_uinode_iter iter = m_VecChild.begin();
    vec_uinode_iter eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        (*iter)->updatePosition(bUpdateChild);
    }
}

// ----------------------------------------------------

void CUIBase::Update(float fDeltaTime, ULONG ElapsedTime)
{
    OnUpdate(fDeltaTime, ElapsedTime);
    UpdateChild(fDeltaTime, ElapsedTime);
}


void CUIBase::UpdateChild(float fDeltaTime, ULONG ElapsedTime)
{
    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != NULL)
            (*iter)->Update(fDeltaTime, ElapsedTime);
    }
}

void CUIBase::Render(CDrawPort* pDraw)
{
    if (m_bHide == TRUE)
        return;

    OnRender(pDraw);
    RenderChild(pDraw);
    OnPostRender(pDraw);	// Child�� �׸���, �߰��� �׸��� ���� ���

#ifdef UI_TOOL
    RenderBorder(pDraw);
#endif // UI_TOOL
}

void CUIBase::RenderChild(CDrawPort* pDraw)
{
    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    CUIBase* pFocusUI = CUIFocus::getSingleton()->getUI();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pFocusUI)
            (*iter)->Render(pDraw);
    }
}

SLONG CUIBase::getTexWidth()
{
    if (m_pTexData == NULL)
        return 0;

    return m_pTexData->GetPixWidth();
}

SLONG CUIBase::getTexHeight()
{
    if (m_pTexData == NULL)
        return 0;

    return m_pTexData->GetPixHeight();
}

CUIBase* CUIBase::getChildAt(int idx)
{
    if (idx < m_VecChild.size() && idx >= 0)
    {
        return m_VecChild[idx];
    }

    return NULL;
}

#ifdef	UI_TOOL

void CUIBase::RenderBorder(CDrawPort* pDraw)
{
    if (m_bSelect == false)
        return;

    int		nX, nY;
    GetAbsPos(nX, nY);

    pDraw->DrawBorder((PIX)nX - 1, (PIX)nY - 1, (PIX)m_nWidth + 1, (PIX)m_nHeight + 1, 0xFF0000FF);
}

bool CUIBase::Swap(int v1, int v2)
{
    if (v1 >= m_VecChild.size() ||
            v2 >= m_VecChild.size())
        return false;

    std::swap(m_VecChild[v1], m_VecChild[v2]);

    return true;
}

bool CUIBase::VecMove(CUIBase* pOrigin, CUIBase* pTarget)
{
    if (pOrigin == NULL || pTarget == NULL)
        return false;

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if (pOrigin == (*iter))
        {
            m_VecChild.erase(iter);
            // orizin ��Ʈ�Ѱ� m_VecChild�� ��Ʈ���� ���ٸ� ������ �� target�� ���ϵ�� �̵��Ѵ�.
            pTarget->addChild(pOrigin);
            break;
        }
    }

    return true;
}

int CUIBase::GetChildIndex(CUIBase* pBase)
{
    //CUIBase* pParent = getChildAt(0);
    int		i, iMax = pBase->getParent()->getChildCount();

    for (i = 0; i < iMax; ++i)
    {
        if (m_VecChild[i] == pBase)
            return i;
    }

    return -1;
}

#endif	// UI_TOOL

//-----------------------------------------------------------------
// Msg

WMSG_RESULT CUIBase::MouseMessage(MSG* pMsg)
{
    WMSG_RESULT ret = WMSG_FAIL;

	if (IsEnabled() == FALSE)
		return ret;

    UINT16 x = LOWORD(pMsg->lParam);
    UINT16 y = HIWORD(pMsg->lParam);

    // ��Ŀ���� �ִٸ� ���� ó������
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    switch (pMsg->message)
    {
    case WM_LBUTTONDOWN:
        {
            if (pUI)
                ret = pUI->LButtonDown(x, y);

            if (ret == WMSG_FAIL)
                ret = LButtonDown(x, y);
        }
        break;

    case WM_LBUTTONUP:
        {
            if (pUI)
                ret = pUI->LButtonUp(x, y);

            if (ret == WMSG_FAIL)
                ret = LButtonUp(x, y);
        }
        break;

    case WM_LBUTTONDBLCLK:
        {
            if (pUI)
                ret = pUI->LButtonDBLClick(x, y);

            if (ret == WMSG_FAIL)
                ret = LButtonDBLClick(x, y);
        }
        break;

    case WM_RBUTTONDOWN:
        {
            if (pUI)
                ret = pUI->RButtonDown(x, y);

            if (ret == WMSG_FAIL)
                ret = RButtonDown(x, y);
        }
        break;

    case WM_RBUTTONUP:
        {
            if (pUI)
                ret = pUI->RButtonUp(x, y);

            if (ret == WMSG_FAIL)
                ret = RButtonUp(x, y);
        }
        break;

    case WM_RBUTTONDBLCLK:
        {
            if (pUI)
                ret = pUI->RButtonDBLClick(x, y);

            if (ret == WMSG_FAIL)
                ret = RButtonDBLClick(x, y);
        }
        break;

    case WM_MOUSEMOVE:
        {
            if (pUI)
                ret = pUI->MouseMove(x, y, pMsg);

            if (ret == WMSG_FAIL)
                ret = MouseMove(x, y, pMsg);
        }
        break;

    case WM_MOUSEWHEEL:
        {
            int wheel = (short)HIWORD(pMsg->wParam);

            if (pUI)
                ret = pUI->MouseWheel(x, y, wheel);

            if (ret == WMSG_FAIL)
                ret = MouseWheel(x, y, wheel);
        }
        break;
    }

    return ret;
}

WMSG_RESULT CUIBase::LButtonDown(UINT16 x, UINT16 y)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnLButtonDown(x, y);

    if (ret == WMSG_FAIL)
        ret = LButtonDownChild(x, y);

    return ret;
}

WMSG_RESULT CUIBase::LButtonDownChild(UINT16 x, UINT16 y)
{
    WMSG_RESULT ret = WMSG_FAIL;

    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->LButtonDown(x, y);

		if (ret != WMSG_FAIL && ret != WMSG_OB)
			return ret;
	}

    // ����� ����������, �޼��� ������� ���� (�ٸ����� ���� ����)
    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::LButtonUp(UINT16 x, UINT16 y)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnLButtonUp(x, y);

    if (ret == WMSG_FAIL)
        ret = LButtonUpChild(x, y);

    return ret;
}

WMSG_RESULT CUIBase::LButtonUpChild(UINT16 x, UINT16 y)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->LButtonUp(x, y);

		if (ret != WMSG_FAIL && ret != WMSG_OB)
			return ret;
	}

    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::LButtonDBLClick(UINT16 x, UINT16 y)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnLButtonDBLClick(x, y);

    if (ret == WMSG_FAIL)
        ret = LButtonDBLClickChild(x, y);

    return ret;
}

WMSG_RESULT CUIBase::LButtonDBLClickChild(UINT16 x, UINT16 y)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->LButtonDBLClick(x, y);

		if (ret != WMSG_FAIL && ret != WMSG_OB)
			return ret;
	}

    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::RButtonDown(UINT16 x, UINT16 y)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnRButtonDown(x, y);

    if (ret == WMSG_FAIL)
        ret = RButtonDownChild(x, y);

    return ret;
}

WMSG_RESULT CUIBase::RButtonDownChild(UINT16 x, UINT16 y)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->RButtonDown(x, y);

		if (ret != WMSG_FAIL && ret != WMSG_OB)
			return ret;
	}

    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::RButtonUp(UINT16 x, UINT16 y)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnRButtonUp(x, y);

    if (ret == WMSG_FAIL)
        ret = RButtonUpChild(x, y);

    return ret;
}

WMSG_RESULT CUIBase::RButtonUpChild(UINT16 x, UINT16 y)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->RButtonUp(x, y);

		if (ret != WMSG_FAIL && ret != WMSG_OB)
			return ret;
	}

    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::RButtonDBLClick(UINT16 x, UINT16 y)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnRButtonDBLClick(x, y);

    if (ret == WMSG_FAIL)
        ret = RButtonDBLClickChild(x, y);

    return ret;
}

WMSG_RESULT CUIBase::RButtonDBLClickChild(UINT16 x, UINT16 y)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->RButtonDBLClick(x, y);

		if (ret != WMSG_FAIL && ret != WMSG_OB)
			return ret;
	}

    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::MouseMove(UINT16 x, UINT16 y, MSG* pMsg)
{
    if (m_bHide || IsEnabled() == FALSE)
    {
        if (m_bEnter == true)
            OnLeave(x, y);

        return WMSG_FAIL;
    }

    // â�� ����� �ֵ� ������ ������~
    if (IsInside(x, y) == TRUE)
    {
        if (m_bEnter == false)
            OnEnter(x, y);
    }
    else
    {
        if (m_bEnter == true)
            OnLeave(x, y);
    }

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnMouseMove(x, y, pMsg);

    if (ret == WMSG_FAIL)
        ret = MouseMoveChild(x, y, pMsg);

    return ret;
}

WMSG_RESULT CUIBase::MouseMoveChild(UINT16 x, UINT16 y, MSG* pMsg)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->MouseMove(x, y, pMsg);

        if (ret != WMSG_FAIL)
            return ret;
    }

    if (m_bEnter == true && m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::MouseWheel(UINT16 x, UINT16 y, int wheel)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    if (IsInside(x, y) == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    if (wheel > 0)
        OnMouseWheelUp();
    else if (wheel < 0)
        OnMouseWheelDown();

    ret = OnMouseWheel(x, y, wheel);

    if (ret == WMSG_FAIL)
        ret = MouseWheelChild(x, y, wheel);

    return ret;
}

WMSG_RESULT CUIBase::MouseWheelChild(UINT16 x, UINT16 y, int wheel)
{
    WMSG_RESULT ret = WMSG_FAIL;
    CUIBase* pUI = CUIFocus::getSingleton()->getUI();

    vec_uinode_iter		iter = m_VecChild.begin();
    vec_uinode_iter		eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        if ((*iter) != pUI)
            ret = (*iter)->MouseWheel(x, y, wheel);

        if (ret != WMSG_FAIL)
            return ret;
    }

    if (m_bInherit == false)
        return WMSG_SUCCESS;

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::KeyMessageProc(MSG* pMsg)
{
	if (m_bHide || IsEnabled() == FALSE)
		return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnKeyMessage(pMsg);

    if (ret == WMSG_FAIL)
        ret = KeyMessageChild(pMsg);

    return ret;
}

WMSG_RESULT CUIBase::KeyMessageChild(MSG* pMsg)
{
    WMSG_RESULT ret = WMSG_FAIL;

    vec_uinode_iter iter = m_VecChild.begin();
    vec_uinode_iter eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        ret = (*iter)->KeyMessageProc(pMsg);

        if (ret != WMSG_FAIL)
            return ret;
    }

    return ret;
}

WMSG_RESULT CUIBase::CharMessageProc(MSG* pMsg)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnCharMessage(pMsg);

    if (ret == WMSG_FAIL)
        ret = CharMessageChild(pMsg);

    return ret;
}

WMSG_RESULT CUIBase::CharMessageChild(MSG* pMsg)
{
    WMSG_RESULT ret = WMSG_FAIL;

    vec_uinode_iter iter = m_VecChild.begin();
    vec_uinode_iter eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        ret = (*iter)->CharMessageProc(pMsg);

        if (ret != WMSG_FAIL)
            return ret;
    }

    return ret;
}

WMSG_RESULT CUIBase::IMEMessageProc(MSG* pMsg)
{
    if (m_bHide || IsEnabled() == FALSE)
        return WMSG_FAIL;

    WMSG_RESULT ret = WMSG_FAIL;

    ret = OnIMEMessage(pMsg);

    if (ret == WMSG_FAIL)
        ret = IMEMessageChild(pMsg);

    return ret;
}

WMSG_RESULT CUIBase::IMEMessageChild(MSG* pMsg)
{
    WMSG_RESULT ret = WMSG_FAIL;

    vec_uinode_iter iter = m_VecChild.begin();
    vec_uinode_iter eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        ret = (*iter)->IMEMessageProc(pMsg);

        if (ret != WMSG_FAIL)
            return ret;
    }

    return ret;
}

void CUIBase::OnEnter(UINT16 x, UINT16 y)
{
    m_bEnter = true;

	if (m_vec_tooltip.empty() == false || m_funcSetTooltip)
        CUITooltipMgr::getSingleton()->setData(this);

    if (m_pCmdOnEnter != NULL)
        m_pCmdOnEnter->execute();

	if (m_funcOnEnter)
		m_funcOnEnter(this);
}

void CUIBase::OnLeave(UINT16 x, UINT16 y)
{
	m_bEnter = false;
	
	if (m_vec_tooltip.empty() == false || m_funcSetTooltip)
		CUITooltipMgr::getSingleton()->hideUI(this);

    if (m_pCmdOnLeave != NULL)
        m_pCmdOnLeave->execute();

	if (m_funcOnLeave)
		m_funcOnLeave(this);
}

void CUIBase::OnMouseWheelUp()
{
    if (m_pCmdWheelUp != NULL)
        m_pCmdWheelUp->execute();

	if (m_funcWheelUp)
		m_funcWheelUp(this);
}

void CUIBase::OnMouseWheelDown()
{
    if (m_pCmdWheelDown != NULL)
        m_pCmdWheelDown->execute();

	if (m_funcWheelDown)
		m_funcWheelDown(this);
}


WMSG_RESULT CUIBase::KeyMessage(MSG* pMsg)
{
    return KeyMessageProc(pMsg);
}

WMSG_RESULT CUIBase::CharMessage(MSG* pMsg)
{
    return CharMessageProc(pMsg);
}

WMSG_RESULT CUIBase::IMEMessage(MSG* pMsg)
{
    return IMEMessageProc(pMsg);
}

void CUIBase::CloneChild(CUIBase* pUI)
{
    if (pUI != NULL)
    {
        pUI->CmdErase();

        if (m_VecChild.empty())
            return;

        pUI->deleteChildList();

        vec_uinode_iter iter = m_VecChild.begin();

        while (iter != m_VecChild.end())
        {
            pUI->addChild((*iter)->Clone());
            ++iter;
        }
    }
}

WMSG_RESULT CUIBase::CloseProc()
{
    OnCloseProc();
    CloneProcChild();

    return WMSG_FAIL;
}

WMSG_RESULT CUIBase::CloneProcChild()
{
    WMSG_RESULT ret = WMSG_FAIL;

    vec_uinode_iter iter = m_VecChild.begin();
    vec_uinode_iter eiter = m_VecChild.end();

    for (; iter != eiter; ++iter)
    {
        ret = (*iter)->CloseProc();

        if (ret != WMSG_FAIL)
            return ret;
    }

    return ret;
}

void CUIBase::CmdErase()
{
    // Clone �� Ư���� ��쿡�� �޸� ���� ���� �����.
    // �Ϲ������� ������!!!!!!!!!!!
    m_pCmd			= NULL;
    m_pCmdOn		= NULL;
    m_pCmdUp		= NULL;
    m_pCmdR			= NULL;
    m_pCmdRUp		= NULL;
    m_pCmdOnEnter	= NULL;
    m_pCmdOnLeave	= NULL;
    m_pCmdDBL		= NULL;
    m_pCmdWheelUp	= NULL;
    m_pCmdWheelDown	= NULL;

	m_func			= NULL;
	m_funcOn		= NULL;
	m_funcUp		= NULL;
	m_funcR			= NULL;
	m_funcRUp		= NULL;
	m_funcOnEnter	= NULL;
	m_funcOnLeave	= NULL;
	m_funcDBL		= NULL;
	m_funcWheelUp	= NULL;
	m_funcWheelDown = NULL;

	m_funcUpdateTooltipText = NULL;
}

WMSG_RESULT CUIBase::OnLButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_OB;

	if (IsInside(x, y) == FALSE)
		return WMSG_OB;

	if (m_pCmd)
		m_pCmd->execute();

	if (m_func)
		m_func(this);

	// Child �� �����ϱ� ���� Fail ����.
	return WMSG_FAIL;
}

WMSG_RESULT CUIBase::OnLButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_OB;

	if (IsInside(x, y) == FALSE)
		return WMSG_OB;

	if (m_pCmdUp)
		m_pCmdUp->execute();

	if (m_funcUp)
		m_funcUp(this);

	return WMSG_FAIL;
}

WMSG_RESULT CUIBase::OnLButtonDBLClick( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_OB;

	if (IsInside(x, y) == FALSE)
		return WMSG_OB;

	if (m_pCmdDBL)
		m_pCmdDBL->execute();

	if (m_funcDBL)
		m_funcDBL(this);

	return WMSG_FAIL;
}

WMSG_RESULT CUIBase::OnRButtonDown( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_OB;

	if (IsInside(x, y) == FALSE)
		return WMSG_OB;

	if (m_pCmdR)
		m_pCmdR->execute();

	if (m_funcR)
		m_funcR(this);

	return WMSG_FAIL;
}

WMSG_RESULT CUIBase::OnRButtonUp( UINT16 x, UINT16 y )
{
	if (m_bHide == TRUE)
		return WMSG_OB;

	if (IsInside(x, y) == FALSE)
		return WMSG_OB;

	if (m_pCmdRUp)
		m_pCmdRUp->execute();

	if (m_funcRUp)
		m_funcRUp(this);

	return WMSG_FAIL;
}

void CUIBase::setTooltip( const char* strTooltip, COLOR col /*= DEF_UI_COLOR_WHITE*/, bool bclear /*= true*/ )
{
	stTooltipInfo info((char*)strTooltip, col);
	
	if (bclear == true)
		clearTooltipCont();

	m_vec_tooltip.push_back(info);
}

stTooltipInfo CUIBase::getTooltip( int idx /*= 0*/ )
{
	if (idx < 0 || idx >= m_vec_tooltip.size())
		return temp_tooltip_data;
	return m_vec_tooltip[idx];
}

void CUIBase::ChildItemSelect()
{
	m_eChildItemState = eSTATE_SELECT;

	vec_uinode_iter		iter = m_VecChild.begin();
	vec_uinode_iter		eiter = m_VecChild.end();

	for(; iter != eiter; ++iter )
	{
		if ((*iter) != NULL)
			(*iter)->ChildItemSelect();
	}
}

void CUIBase::ChildItemEnter()
{
	m_eChildItemState = eSTATE_ENTER;

	vec_uinode_iter		iter = m_VecChild.begin();
	vec_uinode_iter		eiter = m_VecChild.end();

	for(; iter != eiter; ++iter )
	{
		if ((*iter) != NULL)
			(*iter)->ChildItemEnter();
	}
}

void CUIBase::ChildItemLeave()
{
	m_eChildItemState = eSTATE_IDLE;

	vec_uinode_iter		iter = m_VecChild.begin();
	vec_uinode_iter		eiter = m_VecChild.end();

	for(; iter != eiter; ++iter )
	{
		if ((*iter) != NULL)
			(*iter)->ChildItemLeave();
	}
}

void CUIBase::ChildItemIdle()
{
	m_eChildItemState = eSTATE_IDLE;

	vec_uinode_iter		iter = m_VecChild.begin();
	vec_uinode_iter		eiter = m_VecChild.end();

	for(; iter != eiter; ++iter )
	{
		if ((*iter) != NULL)
			(*iter)->ChildItemIdle();
	}
}

void CUIBase::procSetTooltipFunc()
{
	if (m_funcSetTooltip)
		m_funcSetTooltip(this);
}

void CUIBase::updateTooltipTextF()
{
	if (m_funcUpdateTooltipText)
		m_funcUpdateTooltipText(this);
}
