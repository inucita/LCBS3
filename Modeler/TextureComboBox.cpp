// TextureComboBox.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextureComboBox

CTextureComboBox::CTextureComboBox()
{
  m_ptdSelectedTexture = NULL;
}

CTextureComboBox::~CTextureComboBox()
{
}

BEGIN_MESSAGE_MAP(CTextureComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTextureComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropdown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextureComboBox message handlers

BOOL CTextureComboBox::OnIdle(LONG lCount)
{
  CModelerView *pModelerView = CModelerView::GetActiveMappingNormalView();

  // if document doesn't exist
  if( pModelerView == NULL)
  {
    // document closed but we had texture on last idle 
    if( m_ptdSelectedTexture != NULL)
    {
      ResetContent();
      AddString( "None available");
      SetCurSel( 0);
      m_ptdSelectedTexture = NULL;
    }
    return TRUE;
  }

  CModelerDoc *pDoc = (CModelerDoc *) pModelerView->GetDocument();
  INDEX ctTextures = pDoc->m_emEditModel.edm_WorkingSkins.Count();
  // if count of textures changed or model has diferent texture, reflect change
  if( (GetCount() != ctTextures) || (pModelerView->m_ptdiTextureDataInfo->tdi_TextureData != m_ptdSelectedTexture) )
  {
    if( ctTextures == 0)
    {
      ResetContent();
      AddString( "None available");
      SetCurSel( 0);
      m_ptdSelectedTexture = NULL;
      return TRUE;
    }
    // remove combo entries, add working textures and select current one
    m_ptdSelectedTexture = pModelerView->m_ptdiTextureDataInfo->tdi_TextureData;
    ResetContent();
    INDEX iTexture=0;
    FOREACHINLIST( CTextureDataInfo, tdi_ListNode, pDoc->m_emEditModel.edm_WorkingSkins, it)
    {
      int iAddedAs = AddString( it->tdi_FileName.FileName());
      SetItemDataPtr( iAddedAs, &it.Current());
      if( pModelerView->m_ptdiTextureDataInfo == &it.Current())
      {
        SetCurSel( iTexture);
      }
      iTexture++;
    }
  }
  return TRUE;
}

void CTextureComboBox::OnSelchange()
{
  CModelerView *pModelerView = CModelerView::GetActiveView();

  if(pModelerView != NULL)
  {
    // get curently selected combo member
    int iSelectedTexture = GetCurSel();
    // if there is valid member selected
    if( iSelectedTexture != CB_ERR)
    {
      // set new texture ptr for active view
      pModelerView->m_ptdiTextureDataInfo = (CTextureDataInfo *) GetItemDataPtr( iSelectedTexture);
      pModelerView->Invalidate( FALSE);
    }
  }
}

void CTextureComboBox::OnDropdown() 
{
  INDEX ctItems = GetCount();
  if( ctItems == CB_ERR) return;
  
  CRect rectCombo;
  GetWindowRect( &rectCombo);
  
  PIX pixScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
  PIX pixMaxHeight = pixScreenHeight - rectCombo.top;

  CWnd *pwndParent = GetParent();
  if( pwndParent == NULL) return;
  pwndParent->ScreenToClient( &rectCombo);
  PIX pixNewHeight = GetItemHeight(0)*(ctItems+2);
  rectCombo.bottom = rectCombo.top + ClampUp( pixNewHeight, pixMaxHeight);
  MoveWindow( rectCombo);
}
