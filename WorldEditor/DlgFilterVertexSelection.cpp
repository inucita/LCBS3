// DlgFilterVertexSelection.cpp : implementation file
//

#include "stdafx.h"
#include "WorldEditor.h"
#include "DlgFilterVertexSelection.h"

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFilterVertexSelection dialog


CDlgFilterVertexSelection::CDlgFilterVertexSelection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFilterVertexSelection::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFilterVertexSelection)
	m_fMaxY = 0.0f;
	m_fMaxX = 0.0f;
	m_fMaxZ = 0.0f;
	m_fMinX = 0.0f;
	m_fMinY = 0.0f;
	m_fMinZ = 0.0f;
	//}}AFX_DATA_INIT

  m_fMinX =-100000.0f;
	m_fMinY =-100000.0f;
	m_fMinZ =-100000.0f;
	m_fMaxX = 100000.0f;
	m_fMaxY = 100000.0f;
	m_fMaxZ = 100000.0f;
}


void CDlgFilterVertexSelection::DoDataExchange(CDataExchange* pDX)
{
  CWorldEditorDoc* pDoc = theApp.GetActiveDocument();
  // if dialog is recieving data
  if( pDX->m_bSaveAndValidate == FALSE)
  {
  }

	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFilterVertexSelection)
	DDX_Text(pDX, IDC_MAX_Y, m_fMaxY);
	DDX_Text(pDX, IDC_MAX_X, m_fMaxX);
	DDX_Text(pDX, IDC_MAX_Z, m_fMaxZ);
	DDX_Text(pDX, IDC_MIN_X, m_fMinX);
	DDX_Text(pDX, IDC_MIN_Y, m_fMinY);
	DDX_Text(pDX, IDC_MIN_Z, m_fMinZ);
	//}}AFX_DATA_MAP

  // if dialog is giving data
  if( pDX->m_bSaveAndValidate != FALSE)
  {
    CDynamicContainer<CBrushVertex> dcVertices;
    {FOREACHINDYNAMICCONTAINER( pDoc->m_selVertexSelection, CBrushVertex, itvtx)
    {
      dcVertices.Add( itvtx);
    }}

    // for each of the dynamic container
    {FOREACHINDYNAMICCONTAINER( dcVertices, CBrushVertex, itvtx)
    {
      BOOL bDeselect=FALSE;
      FLOAT3D vVtx=itvtx->bvx_vAbsolute;
      if(vVtx(1)<m_fMinX || vVtx(1)>m_fMaxX) bDeselect=TRUE;
      if(vVtx(2)<m_fMinY || vVtx(2)>m_fMaxY) bDeselect=TRUE;
      if(vVtx(3)<m_fMinZ || vVtx(3)>m_fMaxZ) bDeselect=TRUE;

      if( bDeselect)
      {
        pDoc->m_selVertexSelection.Deselect( *itvtx);
      }
    }}

    // refresh selection
    pDoc->m_chSelections.MarkChanged();
    pDoc->UpdateAllViews( NULL);
  }
}


BEGIN_MESSAGE_MAP(CDlgFilterVertexSelection, CDialog)
	//{{AFX_MSG_MAP(CDlgFilterVertexSelection)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFilterVertexSelection message handlers
