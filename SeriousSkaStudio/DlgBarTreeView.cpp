// DlgBarTreeView.cpp: implementation of the CDlgBarTreeView class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SeriousSkaStudio.h"
#include "DlgBarTreeView.h"
#include "resource.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "DlgTemplate.h"
#include <Engine/Base/Stream.h>
#include <Engine/Ska/Skeleton.h>
#include <Engine/Templates/Stock_CShader.h>

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//������ ���� ����	//(Open beta)(2004-12-31)
FLOAT g_fAnimStrength = 0.5f;
//������ ���� ��	//(Open beta)(2004-12-31)

// array of depth in recursion to selected item
CStaticStackArray<INDEX> _aSelectItem;
// array of selected mesh surfaces
CStaticStackArray<INDEX> _aiSelectedMeshSurfaces;

//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
CTag *_pSelectedTag = NULL;
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)

CStaticArray<class CTextureControl> _atxcTexControls;
CStaticArray<class CTexCoordControl> _atxccTexCoordControls;
CStaticArray<class CFloatControl> _afcFloatControls;
CStaticArray<class CFlagControl>  _afcFlagControls;
CStaticArray<class CColorControl> _accColors;
extern INDEX iCustomControlID;

BEGIN_MESSAGE_MAP(CDlgBarTreeView, CDlgTemplate)
//{{AFX_MSG_MAP(CDlgBarTreeView)
ON_NOTIFY(TCN_SELCHANGE, IDC_MODE_SELECT_TAB, OnSelchangeModeSelectTab)
ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDlgBarTreeView::CDlgBarTreeView()
{
	pdlgCurrent = NULL;
}
CDlgBarTreeView::~CDlgBarTreeView()
{
}
// returns info of item in tree view
NodeInfo &CDlgBarTreeView::GetNodeInfo(HTREEITEM hItem)
{
	ASSERT(hItem!=NULL);
	INDEX iNodeIndex = m_TreeCtrl.GetItemData(hItem);
	return theApp.aNodeInfo[iNodeIndex];
}

// returns infog of selected item in tree view
NodeInfo &CDlgBarTreeView::GetSelectedNodeInfo()
{
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	ASSERT(hSelected!=NULL);
	// aditional check
	INDEX iNodeIndex = 0;
	if(hSelected!=NULL) 
	{
		iNodeIndex = m_TreeCtrl.GetItemData(hSelected);
	}
	return theApp.aNodeInfo[iNodeIndex];
}


// chose control group to show
void CDlgBarTreeView::ShowControlGroup(INDEX iGroup)
{
	CDialog *pLastVisibleDlg = pdlgCurrent;
	switch(iGroup)
	{
	case GR_PARENT:
		pdlgCurrent = &m_dlgParent;
		break;
	case GR_ANIMSET:
		pdlgCurrent = &m_dlgAnimSet;
		break;
	case GR_COLISION:
		pdlgCurrent = &m_dlgColision;
		break;
	case GR_ALLFRAMES_BBOX:
		pdlgCurrent = &m_dlgAllFrames;
		break;
	case GR_LOD:
		pdlgCurrent = &m_dlgLod;
		break;
	case GR_BONE:
		pdlgCurrent = &m_dlgBone;
		break;
	case GR_TEXTURE:
		pdlgCurrent = &m_dlgTexture;
		break;
	case GR_LISTOPTIONS:
		pdlgCurrent = &m_dlgListOpt;
		break;
	case GR_SHADERS:
		pdlgCurrent = &m_dlgShader;
		break;
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	case GR_SKATAG:
		pdlgCurrent = &m_dlgSkaTag;
		break;
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	case GR_ANIMEFFECT:
		pdlgCurrent = &m_dlgAnimEffect;
		break;
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
	case GR_RIDINGPOSITION:
		pdlgCurrent = &m_dlgRideSet;
		break;
	default:
		pdlgCurrent = NULL;
		break;
	}
	if(pLastVisibleDlg!=pdlgCurrent) 
	{
		if(pLastVisibleDlg!=NULL) 
		{
			// hide current dialog
			pLastVisibleDlg->ShowWindow(SW_HIDE);
		}
		if(pdlgCurrent!=NULL) 
		{
			// show new dialog
			pdlgCurrent->ShowWindow(SW_SHOW);
		}
	}
}

// select surface in tree view
void CDlgBarTreeView::SelectMeshSurface(HTREEITEM hItem)
{
	INDEX iNodeIndex = m_TreeCtrl.GetItemData(hItem);
	INDEX iIcon = 12;
	NodeInfo &ni = theApp.aNodeInfo[iNodeIndex];
	ASSERT(ni.ni_iType == NT_MESHSURFACE);
	
	// count selected mesh surfaces
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surface in array
	for(INDEX isms=0;isms<ctsms;isms++)
	{
		INDEX &iIndex = _aiSelectedMeshSurfaces[isms];
		// check if surface is allready selected
		if(iIndex == iNodeIndex)
		{
			// surface is allready selected, return
			return;
		}
	}
	// add surface to selection array
	INDEX &iIndex = _aiSelectedMeshSurfaces.Push();
	iIndex = iNodeIndex;
	// set new icon
	m_TreeCtrl.SetItemImage(hItem,iIcon,iIcon);
}

// Seselect surface in tree view
void CDlgBarTreeView::DeselectMeshSurface(HTREEITEM hItem)
{
	INDEX iNodeIndex = m_TreeCtrl.GetItemData(hItem);
	INDEX iIcon = 11;
	NodeInfo &ni = theApp.aNodeInfo[iNodeIndex];
	ASSERT(ni.ni_iType == NT_MESHSURFACE);
	
	// count selected mesh surfaces
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surface in array
	for(INDEX isms=0; isms<ctsms; ++isms)
	{
		INDEX &iIndex = _aiSelectedMeshSurfaces[isms];
		// check if this is clicked item
		if(iIndex == iNodeIndex)
		{
			// remove one item
			INDEX iLastIndex = _aiSelectedMeshSurfaces.Pop();
			iIndex = iLastIndex;
			// set new icon
			m_TreeCtrl.SetItemImage(hItem,iIcon,iIcon);
			return;
		}
	}
}

// Togle surface selection in tree view
void CDlgBarTreeView::TogleSurfaceSelection(HTREEITEM hItem)
{
	INDEX iNodeIndex = m_TreeCtrl.GetItemData(hItem);
	NodeInfo &ni = theApp.aNodeInfo[iNodeIndex];
	ASSERT(ni.ni_iType == NT_MESHSURFACE);
	
	// count selected mesh surfaces
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surface in array
	for(INDEX isms=0;isms<ctsms;isms++)
	{
		INDEX &iIndex = _aiSelectedMeshSurfaces[isms];
		// check if this is clicked item
		if(iIndex == iNodeIndex)
		{
			// deselect surface
			DeselectMeshSurface(hItem);
			return;
		}
	}
	// if item was't previously selected, select it 
	SelectMeshSurface(hItem);
}

// Deselect all surfaces
void CDlgBarTreeView::DeSelectAllSurfaces(HTREEITEM hParent/*=NULL*/)
{
	INDEX iIcon=11;
	if(hParent==NULL)
	{
		hParent = m_TreeCtrl.GetRootItem();
		_aiSelectedMeshSurfaces.PopAll();
	}
	
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hParent);
	while(hChild!=NULL)
	{
		DeSelectAllSurfaces(hChild);
		NodeInfo &ni = GetNodeInfo(hChild);
		if(ni.ni_iType == NT_MESHSURFACE)
		{
			m_TreeCtrl.SetItemImage(hChild,iIcon,iIcon);
		}
		hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
	}
}

// select all surfaces if item is not selected, otherwise deselect all
void CDlgBarTreeView::SelectAllSurfaces(HTREEITEM hItem)
{
	INDEX iIcon = -1;
	INDEX iNodeIndex = m_TreeCtrl.GetItemData(hItem);
	NodeInfo &ni = theApp.aNodeInfo[iNodeIndex];
	ASSERT(ni.ni_iType == NT_MESHSURFACE);
	MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
	// clicked item is allready selected, deselect all surfaces
	BOOL bSelect = IsSurfaceSelected(*pmsrf);
	// get parent
	HTREEITEM hParent = m_TreeCtrl.GetParentItem(hItem);
	INDEX iParentIndex = m_TreeCtrl.GetItemData(hParent);
	NodeInfo &niParent = theApp.aNodeInfo[iParentIndex];
	ASSERT(niParent.ni_iType == NT_MESHLOD);
	MeshLOD *pmlod = (MeshLOD*)niParent.ni_pPtr;
	// get children of mesh lod
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hParent);
	while(hChild!=NULL)
	{
		NodeInfo &niChild = GetNodeInfo(hChild);
		if(niChild.ni_iType==NT_MESHSURFACE) 
		{
			// select surface
			if(!bSelect) 
			{
				SelectMeshSurface(hChild);
			}
			else 
			{
				DeselectMeshSurface(hChild);
			}
		}
		hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
	}
}

// check if surface is selected
BOOL CDlgBarTreeView::IsSurfaceSelected(MeshSurface &msrf)
{
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++)
	{
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		// check if this is selected surface
		if(pmsrf == &msrf) return TRUE;
	}
	return FALSE;
}

// change texture for all selected surfaces
void CDlgBarTreeView::ChangeTextureOnSelectedSurfaces(CTString strControlID,CTString strNewTexID)
{
	// get id of texture
	INDEX iTextureID = ska_GetIDFromStringTable(strNewTexID);
	// get selected surfaces count
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++)
	{
		// get pointer to mesh surface
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		if(pmsrf->msrf_pShader==NULL) continue;
		// get surface description and find witch param has same id as control that was modified
//������ ���� ����	//(For Performance)(0.1)
		ShaderDesc *pshDesc;
		pmsrf->msrf_pShader->GetShaderDesc(pshDesc);
		ShaderDesc &shDesc = *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
		INDEX cttx = shDesc.sd_astrTextureNames.Count();
		// for each texture in shader
		for(INDEX itx=0;itx<cttx;itx++)
		{
			// if this surface has shader param that has text same as control that changed value
			if(shDesc.sd_astrTextureNames[itx] == strControlID)
			{
				// change it
				pmsrf->msrf_ShadingParams.sp_aiTextureIDs[itx] = iTextureID;
			}
		}
		UpdateSurfaceSubSurfaces(ni);
	}
}

// change texture coord index for all selected surfaces
void CDlgBarTreeView::ChangeTextureCoordsOnSelectedSurfaces(CTString strControlID,INDEX iNewTexCoordsIndex)
{
	// get selected surfaces count
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++)
	{
		// get pointer to mesh surface
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		if(pmsrf->msrf_pShader==NULL) continue;
		// get surface description and find witch param has same id as control that was modified
//������ ���� ����	//(For Performance)(0.1)
		ShaderDesc *pshDesc;
		pmsrf->msrf_pShader->GetShaderDesc(pshDesc);
		ShaderDesc &shDesc = *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
		INDEX cttxc = shDesc.sd_astrTexCoordNames.Count();
		// for each texture in shader
		for(INDEX itxc=0;itxc<cttxc;itxc++)
		{
			// if this surface has shader param that has text same as control that changed value
			if(shDesc.sd_astrTexCoordNames[itxc] == strControlID)
			{
				// change it
				pmsrf->msrf_ShadingParams.sp_aiTexCoordsIndex[itxc] = iNewTexCoordsIndex;
			}
		}
		UpdateSurfaceSubSurfaces(ni);
	}
}

// change color on curently selected model instance
void CDlgBarTreeView::ChangeColorOnSelectedModel(COLOR colNewColor)
{
	ASSERT(pmiSelected!=NULL);
	pmiSelected->mi_colModelColor = colNewColor;
}

// change color on all selected mesh surfaces
void CDlgBarTreeView::ChangeColorOnSelectedSurfaces(CTString strControlID,COLOR colNewColor)
{
	// get selected surfaces count
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++)
	{
		// get pointer to mesh surface
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		if(pmsrf->msrf_pShader==NULL) continue;
		// get surface description and find witch param has same id as control that was modified
//������ ���� ����	//(For Performance)(0.1)
		ShaderDesc *pshDesc;
		pmsrf->msrf_pShader->GetShaderDesc(pshDesc);
		ShaderDesc &shDesc = *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
		INDEX ctcl = shDesc.sd_astrColorNames.Count();
		// for each color in shader
		for(INDEX icl=0;icl<ctcl;icl++)
		{
			// if this surface has shader param that has text same as control that changed value
			if(shDesc.sd_astrColorNames[icl] == strControlID)
			{
				// change it
				pmsrf->msrf_ShadingParams.sp_acolColors[icl] = colNewColor;
			}
		}
		UpdateSurfaceSubSurfaces(ni);
	}
}

// change floats on all selected mesh surfaces
void CDlgBarTreeView::ChangeFloatOnSelectedSurfaces(CTString strControlID,FLOAT fNewFloat)
{
	// get selected surfaces count
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++) 
	{
		// get pointer to mesh surface
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		if(pmsrf->msrf_pShader==NULL) continue;
		// get surface description and find witch param has same id as control that was modified
//������ ���� ����	//(For Performance)(0.1)
		ShaderDesc *pshDesc;
		pmsrf->msrf_pShader->GetShaderDesc(pshDesc);
		ShaderDesc &shDesc = *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
		INDEX ctfl = shDesc.sd_astrFloatNames.Count();
		// for each float in shader
		for(INDEX ifl=0;ifl<ctfl;ifl++) 
		{
			// if this surface has shader param that has text same as control that changed value
			if(shDesc.sd_astrFloatNames[ifl] == strControlID) 
			{
				// change it
				pmsrf->msrf_ShadingParams.sp_afFloats[ifl] = fNewFloat;
			}
		}
		UpdateSurfaceSubSurfaces(ni);
	}
}

void CDlgBarTreeView::ChangeFlagOnSelectedSurfaces(CTString strControlID, BOOL bChecked, INDEX iFlagIndex)
{
	ULONG ulNewFlag = (1UL<<iFlagIndex);
	// get selected surfaces count
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++) 
	{
		// get pointer to mesh surface
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		if(pmsrf->msrf_pShader==NULL) continue;
		// get surface description and find witch param has same id as control that was modified
//������ ���� ����	//(For Performance)(0.1)
		ShaderDesc *pshDesc;
		pmsrf->msrf_pShader->GetShaderDesc(pshDesc);
		ShaderDesc &shDesc = *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
		INDEX ctfl = shDesc.sd_astrFlagNames.Count();
		// for each flag in shader
		for(INDEX ifl=0;ifl<ctfl;ifl++) 
		{
			// if this surface has shader param that has text same as control that changed value
			if(shDesc.sd_astrFlagNames[ifl] == strControlID) 
			{
				// if now checked 
				if(bChecked) 
				{
					// add this flag
					pmsrf->msrf_ShadingParams.sp_ulFlags |= ulNewFlag;
					// if unchecked
				}
				else 
				{
					// remove this flag
					pmsrf->msrf_ShadingParams.sp_ulFlags &= ~ulNewFlag;
				}
			}
		}
		UpdateSurfaceSubSurfaces(ni);
	}
}

static MeshLOD *FindMeshLodSurface(CModelInstance *pmi, const MeshSurface *pmsrfFind)
{
	ASSERT(pmi!=NULL);
	ASSERT(pmsrfFind!=NULL);
	// for each mesh instance
	const INDEX ctmshi = pmi->mi_aMeshInst.Count();
	for(INDEX imshi=0;imshi<ctmshi;imshi++) 
	{
		MeshInstance &mshi = pmi->mi_aMeshInst[imshi];
		if(mshi.mi_pMesh!=NULL) 
		{
			CMesh &msh = *mshi.mi_pMesh;
			const INDEX ctmlods = msh.msh_aMeshLODs.Count();
			for(INDEX imlod=0;imlod<ctmlods;imlod++) 
			{
				MeshLOD &mlod = msh.msh_aMeshLODs[imlod];
				const INDEX ctmsrf = mlod.mlod_aSurfaces.Count();
				for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
				{
					const MeshSurface *pmsrf = &mlod.mlod_aSurfaces[imsrf];
					if(pmsrf == pmsrfFind) 
					{
						return &mlod;
					}
				}
			}
		}
	}
	ASSERTALWAYS("Surface does not exists in mesh lod?!");
	return NULL;
}

void CDlgBarTreeView::UpdateSurfaceSubSurfaces(const NodeInfo &niSurface)
{
	ASSERT(niSurface.ni_iType==NT_MESHSURFACE);
	CModelInstance *pmi = niSurface.pmi;
	MeshSurface *pmsrf = (MeshSurface*)niSurface.ni_pPtr;
	MeshLOD *pmlod = FindMeshLodSurface(pmi,pmsrf);
	ASSERT(pmsrf!=NULL);
	ASSERT(pmlod!=NULL);
	ASSERT(!(pmsrf->msrf_ulFlags&MS_MESH_SUBSURFACE)); // this must not be subsurface
	if(pmlod!=NULL) 
	{
		const CTString &strSurface = ska_IDToString(pmsrf->msrf_iSurfaceID);
		const INDEX strSurfLen = strSurface.Length();
		INDEX ctmsrf = pmlod->mlod_aSurfaces.Count();
		for(INDEX imsrf=0;imsrf<ctmsrf;imsrf++) 
		{
			MeshSurface &msrf = pmlod->mlod_aSurfaces[imsrf];
			// if this surface is sub surface
			if(msrf.msrf_ulFlags&MS_MESH_SUBSURFACE) 
			{
				CTString strSubSurface = ska_IDToString(msrf.msrf_iSurfaceID);
				strSubSurface.TrimRight(strSurfLen);
				// if names match
				if(strSubSurface==strSurface) 
				{
					// this is our subsurface
					msrf.msrf_ShadingParams = pmsrf->msrf_ShadingParams;
					if(pmsrf->msrf_pShader!=NULL) 
					{
						const CTFileName &fnShader = pmsrf->msrf_pShader->ser_FileName;
						try 
						{
							msrf.msrf_pShader = _pShaderStock->Obtain_t(fnShader);
						}
						catch(char*) 
						{
							msrf.msrf_pShader = NULL;
						}
					}
				}
			}
		}
	}
}

// change shader for all surfaces that are in selection array
void CDlgBarTreeView::ChangeShaderOnSelectedSurfaces(CTString fnNewShader)
{
	INDEX ctsms = _aiSelectedMeshSurfaces.Count();
	// for each selected mesh surfaces
	for(INDEX isms=0;isms<ctsms;isms++) 
	{
		NodeInfo &ni = theApp.aNodeInfo[_aiSelectedMeshSurfaces[isms]];
		ASSERT(ni.ni_iType == NT_MESHSURFACE);
		MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
		ASSERT(pmsrf!=NULL);
		try 
		{
			// try to change surface shader
			ChangeSurfaceShader_t(*pmsrf,fnNewShader);
		}
		catch(char *strErr) 
		{
			theApp.ErrorMessage(strErr);
		}
		UpdateSurfaceSubSurfaces(ni);
	}
}

// notification that item was clicked
void CDlgBarTreeView::OnItemClick(HTREEITEM hItem,HTREEITEM hLastSelected)
{
	BOOL bControl = (GetKeyState( VK_CONTROL)&0x8000) != 0;
	BOOL bShift = (GetKeyState( VK_SHIFT)&0x8000) != 0;
	ASSERT(hItem != NULL);
	NodeInfo &ni = theApp.aNodeInfo[m_TreeCtrl.GetItemData(hItem)];
	if(ni.ni_iType == NT_MESHSURFACE) 
	{
		if (bShift) 
		{
			if(hLastSelected!=NULL) 
			{
				NodeInfo &niLastSelected = GetNodeInfo(hLastSelected);
				if(niLastSelected.ni_iType == NT_MESHSURFACE) 
				{
					HTREEITEM hParent = m_TreeCtrl.GetParentItem(hItem);
					HTREEITEM hLastParent = m_TreeCtrl.GetParentItem(hLastSelected);
					ASSERT(hParent!=NULL);
					ASSERT(hLastParent!=NULL);
					if(hParent==hLastParent)           
					{
						// deselect all items
						if(!bControl) DeSelectAllSurfaces();
						// get first child
						HTREEITEM hChild = m_TreeCtrl.GetChildItem(hParent);
						// find first item or last selected item
						while((hChild!=hItem)&&(hChild!=hLastSelected)) 
						{
							hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
							ASSERT(hChild!=NULL);
						}
						// select first item
						SelectMeshSurface(hChild);
						if(hItem!=hLastSelected) hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
						// select all items until item or last selected item
						while((hChild!=hItem)&&(hChild!=hLastSelected)) 
						{
							SelectMeshSurface(hChild);
							hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
							ASSERT(hChild!=NULL);
						}
						SelectMeshSurface(hChild);
					}
					return;
				}
			}
		}
		if(bControl) 
		{
			// select curent surface
			TogleSurfaceSelection(hItem);
			return;
		}
		// deselect all items
		DeSelectAllSurfaces();
		// select curent surface
		SelectMeshSurface(hItem);
	}
}

// notification that item icons was clicked
void CDlgBarTreeView::OnItemIconClick(HTREEITEM hItem)
{
	NodeInfo &ni = GetNodeInfo(hItem);
	if(ni.ni_iType == NT_MESHSURFACE) 
	{
		TogleSurfaceSelection(hItem);
	}
}

// show shader for surface
void CDlgBarTreeView::ShowSurfaceShader(MeshSurface *pmsrf,MeshLOD *pmlod,MeshInstance *pmshi)
{
	ASSERT(pmsrf!=NULL);
	ASSERT(pmlod!=NULL);
	ASSERT(pmshi!=NULL);
	
	// remember vertical scroll pos
	INDEX ivScroll = m_dlgShader.GetScrollPos(SB_VERT);
	// set scroll to 0
	m_dlgShader.SetScrollPos(SB_VERT,0);
	VScrollControls(&m_dlgShader);
	// clear shader dlg
	RemoveDialogControls(&m_dlgShader);
	CComboBox *cbShader = ((CComboBox*)m_dlgShader.GetDlgItem(IDC_CB_SHADER));
	
	_atxcTexControls.Clear();
	_atxccTexCoordControls.Clear();
	_accColors.Clear();
	_afcFloatControls.Clear();
	_afcFlagControls.Clear();
	iCustomControlID=FIRSTSHADEID;
	
//������ ���� ����	//(For Performance)(0.1)
	ShaderDesc *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
	
	INDEX ctTextures = 0;
	INDEX ctTexCoords = 0;
	INDEX ctColors = 0;
	INDEX ctFloats = 0;
	INDEX ctFlags  = 0;
	
	// if surface has shader
	if(pmsrf->msrf_pShader!=NULL) 
	{
//������ ���� ����	//(For Performance)(0.1)
		pmsrf->msrf_pShader->GetShaderDesc(pshDesc);
		ShaderDesc &shDesc = *pshDesc;
//������ ���� ��	//(For Performance)(0.1)
		ctTextures = shDesc.sd_astrTextureNames.Count();
		ctTexCoords = shDesc.sd_astrTexCoordNames.Count();
		ctColors = shDesc.sd_astrColorNames.Count();
		ctFloats = shDesc.sd_astrFloatNames.Count();
		ctFlags  = shDesc.sd_astrFlagNames.Count();
		// select shader in combo box
		CTFileName fnShader = pmsrf->msrf_pShader->GetName();
		CTString strShader = fnShader.FileName();
		ASSERT(cbShader!=NULL);
		// try to find shader in list if shaders
		INDEX iShaderIndex = cbShader->FindStringExact(-1,(const char*)strShader);
		// if shader does not exits
		if(iShaderIndex == CB_ERR) 
		{
			// error: shader is not found in list of shaders
			return;
			// else shader exists
		}
		else 
		{
			// select it
			cbShader->SetCurSel(iShaderIndex);
		}
		ShaderParams &mspParams = pmsrf->msrf_ShadingParams;
		
		//    ASSERT(pmsrf->msrf_ShadingParams.sp_acolColors.Count() == ctColors);
		
		_atxcTexControls.New(ctTextures);
		_atxccTexCoordControls.New(ctTexCoords);
		_accColors.New(ctColors);
		_afcFloatControls.New(ctFloats);
		_afcFlagControls.New(ctFlags);
		
		// add texture controls to shader dialog
		for(INDEX itx=0;itx<ctTextures;itx++) 
		{
			CTextureControl &txc = _atxcTexControls[itx];
			txc.AddControl(shDesc.sd_astrTextureNames[itx],&pmsrf->msrf_ShadingParams.sp_aiTextureIDs[itx]);
			// count texture instances
			INDEX ctti=pmshi->mi_tiTextures.Count();
			// for each texture instances
			for(INDEX iti=0;iti<ctti;iti++) 
			{
				TextureInstance &ti = pmshi->mi_tiTextures[iti];
				CTString strTexName = ska_GetStringFromTable(ti.GetID());
				txc.txc_Combo.AddString((const char*)strTexName);
			}
			INDEX iSelectTexID = mspParams.sp_aiTextureIDs[itx];
			// if texture id is valid
			if(iSelectTexID>=0) 
			{
				// Select texture string in shader textures combo box
				CTString strSelectTexID = ska_GetStringFromTable(iSelectTexID);
				int iItemIndex = txc.txc_Combo.FindStringExact(-1,(const char*)strSelectTexID);
				if(iItemIndex!=CB_ERR) 
				{
					// Select texture in combo box
					txc.txc_Combo.SetCurSel(iItemIndex);
				}
				else 
				{
					// Select nothing
					txc.txc_Combo.SetCurSel(-1);
				}
			}
			else 
			{
				// Select nothing
				txc.txc_Combo.SetCurSel(-1);
			}
		}
		// add texcoord controls to shader dialog
		for(INDEX itxc=0;itxc<ctTexCoords;itxc++) 
		{
			CTexCoordControl &txcc = _atxccTexCoordControls[itxc];
			txcc.AddControl(shDesc.sd_astrTexCoordNames[itxc],&pmsrf->msrf_ShadingParams.sp_aiTexCoordsIndex[itxc]);
			// count uvmaps
			INDEX ctuv = pmlod->mlod_aUVMaps.Count();
			// for each uvmap in lod
			for(INDEX iuv=0;iuv<ctuv;iuv++) 
			{
				// add uvmap name in combo box
				MeshUVMap &uvm = pmlod->mlod_aUVMaps[iuv];
				CTString strUVMapName = ska_GetStringFromTable(uvm.muv_iID);
				txcc.txcc_Combo.AddString((const char*)strUVMapName);
			}
			
			INDEX iSelectTexCoordsIndex = mspParams.sp_aiTexCoordsIndex[itxc];
			txcc.txcc_Combo.SetCurSel(iSelectTexCoordsIndex);
		}
		// add color controls to shader dialog
		for(INDEX icc=0;icc<ctColors;icc++) 
		{
			CColorControl &cc = _accColors[icc];
			cc.AddControl(shDesc.sd_astrColorNames[icc],&pmsrf->msrf_ShadingParams.sp_acolColors[icc]);
		}
		// add float controls to shader dialog

		INDEX ifc = 0;
		for( ifc = 0; ifc < ctFloats; ifc++) 
		{
			CFloatControl &fc = _afcFloatControls[ifc];
			fc.AddControl(shDesc.sd_astrFloatNames[ifc],&pmsrf->msrf_ShadingParams.sp_afFloats[ifc]);
		}
		// add flag controls to shader dialog
		for( ifc = 0; ifc < ctFlags; ifc++) 
		{
			CFlagControl &fc = _afcFlagControls[ifc];
			fc.AddControl(shDesc.sd_astrFlagNames[ifc],ifc,mspParams.sp_ulFlags);
		}
	}
	else 
{
		// clear text from combo box
		cbShader->SetCurSel(-1);
	}
	
	
	CRect rcDlg;
	m_dlgShader.GetWindowRect(rcDlg);
	
	INDEX iCtrlCount = iCustomControlID-FIRSTSHADEID;
	INDEX iDlgHeight = (rcDlg.bottom-rcDlg.top-15);
	
	if(iDlgHeight<YPOS) m_dlgShader.SetScrollRange(SB_VERT,1,YPOS-iDlgHeight);
	else m_dlgShader.SetScrollRange(SB_VERT,0,0);
	
	AddDialogControls(&m_dlgShader);
	// return old vertical scroll pos
	m_dlgShader.SetScrollPos(SB_VERT,ivScroll);
	VScrollControls(&m_dlgShader);
	
	//m_dlgShader.Invalidate(FALSE);
}

void CDlgBarTreeView::VScrollControls(CDialog *pDlg)
{
	//pDlg->SetRedraw(FALSE);
	int iPosition = pDlg->GetScrollPos(SB_VERT);
	// GetScrollPos returns values from 1 to n
	if(iPosition>0) iPosition--;
	INDEX ctctrl = dlg_aControls.Count();
	// for each control in array of contrls
	for(INDEX ictrl=0;ictrl<ctctrl;ictrl++) 
	{
		Control &ctrl = dlg_aControls[ictrl];
		// check if its dialog is being scrolled
		if(ctrl.ct_pParentDlg == pDlg) 
		{
			// move control
			CRect rcCtrl;
			pDlg->GetDlgItem(ctrl.ct_iID)->GetWindowRect(rcCtrl);
			pDlg->ScreenToClient(rcCtrl);
			rcCtrl.top = ctrl.ct_iTop - iPosition;
			rcCtrl.bottom = ctrl.ct_iBottom - iPosition;
			pDlg->GetDlgItem(ctrl.ct_iID)->MoveWindow(rcCtrl);
		}
	}
	//  pDlg->SetRedraw(TRUE);
	//  pDlg->Invalidate(TRUE);
}

// notification that selected item has changed
void CDlgBarTreeView::SelItemChanged(HTREEITEM hSelected)
{
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	_pSelectedTag = NULL;
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();
	BOOL bCtrl  = (GetKeyState( VK_CONTROL)&0x8000) != 0;
	BOOL bShift = (GetKeyState( VK_SHIFT)&0x8000) != 0;
	BOOL bAlt   = (GetKeyState( VK_MENU)&0x8000) != 0;
	
	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	// reset selected bone in treeview
	theApp.iSelectedItemID = -1;
	// if no item selected
	if(hSelected == NULL) 
	{
		pmiSelected = NULL;
		ResetControls();
	}
	// get index in node array of selected item
	INDEX iSelIndex = m_TreeCtrl.GetItemData(hSelected);
	// get node info of selected item
	NodeInfo &ni = theApp.aNodeInfo[iSelIndex];
	// get model instance that holds selected item
	pmiSelected = ni.pmi;
	// if selected item is model instance set it to be selected model instance
	if(ni.ni_iType == NT_MODELINSTANCE) pmiSelected = (CModelInstance*)ni.ni_pPtr;
	
	if(pmiSelected!=NULL) 
	{
		CTString strStretch = CTString(0,"%g",pmiSelected->mi_vStretch(1));
		pMainFrame->m_ctrlMIStretch.SetWindowText((const char*)strStretch);
		
//������ ���� ����	//(Open beta)(2004-11-30)
		CTString strAlpha = CTString(0,"%d",pmiSelected->mi_colModelColor & 0x000000FF);
		pMainFrame->m_ctrlMIAlpha.SetWindowText((const char*)strAlpha);		
//������ ���� ��	//(Open beta)(2004-11-30)
	}
	// get parent of selected item
	HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);
	INDEX iParent=-1;
	// if parent exists get ist index
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	NodeInfo *pNIParent = NULL;
	if(hParent != NULL)
	{
		iParent = m_TreeCtrl.GetItemData(hParent);
		pNIParent = &(theApp.aNodeInfo[iParent]);
	}
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
	// get parent model isntance of selected model instance
	CModelInstance *pmiParent = pmiSelected->GetParent(pDoc->m_ModelInstance);
	// if parent exisits fill combo box with parent bones
	if(pmiParent != NULL) 
	{
		// fill combo box with parent bones
		if(pmiParent->mi_psklSkeleton != NULL) FillBonesToComboBox(m_dlgParent, pmiParent->mi_psklSkeleton,0,IDC_CB_PARENTBONE);
		// clear combo box for parent bones
		else ((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTBONE))->ResetContent();
	}
	else 
	{
		// clear combo box for parent bones
		((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTBONE))->ResetContent();
	}
	// if parent bone is valid
	if(pmiSelected->mi_iParentBoneID>=0) 
	{
		// get name of parent bone
		CTString strBoneName = ska_IDToString(pmiSelected->mi_iParentBoneID);
		// select parent bone in combo box
		((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTBONE))->SelectString(-1,(const char*)strBoneName);
	}
	
	if (pmiSelected->mi_iRideParentBoneID >= 0)
	{
		CTString strBoneName = ska_IDToString(pmiSelected->mi_iRideParentBoneID);
		((CComboBox*)m_dlgRideSet.GetDlgItem(IDC_CB_RIDE_PARENTBONE))->ResetContent();
		((CComboBox*)m_dlgRideSet.GetDlgItem(IDC_CB_RIDE_PARENTBONE))->AddString((const char*)strBoneName);
		((CComboBox*)m_dlgRideSet.GetDlgItem(IDC_CB_RIDE_PARENTBONE))->SelectString(-1, (const char*)strBoneName);

		FLOAT3D vPos = pmiSelected->GetRideOffsetPos();
		ANGLE3D aRot = pmiSelected->GetRideOffsetRot();

		((CEdit*)m_dlgRideSet.GetDlgItem(IDC_TB_RIDE_OFFSET_POSX))->SetWindowText((const char*)CTString(0,"%g",vPos(1)));
		((CEdit*)m_dlgRideSet.GetDlgItem(IDC_TB_RIDE_OFFSET_POSY))->SetWindowText((const char*)CTString(0,"%g",vPos(2)));
		((CEdit*)m_dlgRideSet.GetDlgItem(IDC_TB_RIDE_OFFSET_POSZ))->SetWindowText((const char*)CTString(0,"%g",vPos(3)));

		((CEdit*)m_dlgRideSet.GetDlgItem(IDC_TB_RIDE_OFFSET_ROTH))->SetWindowText((const char*)CTString(0,"%g",aRot(1)));
		((CEdit*)m_dlgRideSet.GetDlgItem(IDC_TB_RIDE_OFFSET_ROTP))->SetWindowText((const char*)CTString(0,"%g",aRot(2)));
		((CEdit*)m_dlgRideSet.GetDlgItem(IDC_TB_RIDE_OFFSET_ROTB))->SetWindowText((const char*)CTString(0,"%g",aRot(3)));
	}
	
	// select parent model instance in combo box
	CComboBox *cbParentList = ((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTMODEL));
	// count items in combo box
	INDEX ctpl = cbParentList->GetCount();
	// for each item in combo box
	for(INDEX ipl=0;ipl<ctpl;ipl++) 
	{
		CModelInstance *pmiParentInList = (CModelInstance*)cbParentList->GetItemDataPtr(ipl);
		// chech if this item is parent in current selected mi
		if(pmiParentInList == pmiParent) 
		{
			cbParentList->SetCurSel(ipl);
			break;
		}
		cbParentList->SetCurSel(-1);
	}
	
	// switch type of selected item
	switch(ni.ni_iType)
	{ 
	case NT_MODELINSTANCE:
		{
			ShowControlGroup(GR_PARENT);
			CModelInstance *pmi = (CModelInstance*)ni.ni_pPtr;
			m_tbMiName.SetWindowText(pmi->GetName());
		}
		break;
	case NT_ANIMSET:
	case NT_MESHLODLIST:
	case NT_SKELETONLODLIST:
		ShowControlGroup(GR_LISTOPTIONS);
		break;
	case NT_TEXINSTANCE:
		{
			TextureInstance *pti = (TextureInstance*)ni.ni_pPtr;
			CTString strTexName = ska_GetStringFromTable(pti->GetID());
			m_tbTextureName.SetWindowText((const char*)strTexName);
			m_tvTexView.ChangeTexture(pti->ti_toTexture.GetName());
			ShowControlGroup(GR_TEXTURE);
		}
		break;
	case NT_MESHLOD:
		{
			MeshLOD *pmlod = (MeshLOD*)ni.ni_pPtr;
			CTString strName;
			strName.PrintF("%g",pmlod->mlod_fMaxDistance);
			m_tbDistance.SetWindowText((const char*)strName);
			// set this mesh lod as forced lod
			pDoc->m_fCustomLodDist=pmlod->mlod_fMaxDistance-0.01f;
			ShowControlGroup(GR_LOD);
			SetCustomTabText("Mesh lod");
		}
		break;
	case NT_SKELETONLOD:
		{
			SkeletonLOD *pslod = (SkeletonLOD*)ni.ni_pPtr;
			CTString strName = CTString(0,"%g",pslod->slod_fMaxDistance);
			m_tbDistance.SetWindowText((const char*)strName);
			// set this skleleton lod as forced lod
			pDoc->m_fCustomLodDist=pslod->slod_fMaxDistance-0.01f;
			ShowControlGroup(GR_LOD);
			SetCustomTabText("Skeleton lod");
		}
		break;
	case NT_BONE:
		{
			SkeletonBone *pbone = (SkeletonBone*)ni.ni_pPtr;
			theApp.iSelectedItemID = pbone->sb_iID;
			ShowControlGroup(GR_BONE);
		}
		break;
	case NT_WEIGHT_MAP:
		{
			MeshWeightMap *pmwm = (MeshWeightMap*)ni.ni_pPtr;
			theApp.iSelectedItemID = pmwm->mwm_iID;
			ShowControlGroup(-1);
		}
		break;
	case NT_MORPH_MAP:
		{
			MeshMorphMap *pmmm = (MeshMorphMap*)ni.ni_pPtr;
			theApp.iSelectedItemID = pmmm->mmp_iID;
			ShowControlGroup(-1);
		}
		break;
	case NT_ANIMATION:
		{
			Animation *pan = (Animation*)ni.ni_pPtr;
			FLOAT fAnimStrenght = 1.0f;
			// start playing selected animation
			ULONG ulAnimFlags = 0;// | AN_NORESTART;
			
			if(pDoc->m_bFreezeOnBlend) 
			{
				ulAnimFlags |= AN_FREEZE_ON_BLEND;
			}
			
			// if looping animations
			if(pDoc->m_bAnimLoop) 
			{
				// if shift is not presed 
				if(!bShift) 
				{
					// loop animations
					ulAnimFlags |= AN_LOOPING;
				}
				else 
				{
					// ulAnimFlags |= AN_REMOVE_ON_END;
				} 
				// if not looping and shift pressed
			}
			else if(bShift) 
			{
				// loop anims
				ulAnimFlags |= AN_LOOPING;
			}
			
			if(bCtrl && bAlt) 
			{
				// Add animation
				pmiSelected->RemovePassedAnimsFromQueue();
				pmiSelected->AddAnimation(pan->an_iID,(ulAnimFlags&(~AN_LOOPING))|AN_REMOVE_ON_END,g_fAnimStrength,0);
				// if alt is presed 
			}
			// if control is presed
			else if(bCtrl) 
			{
				// Add animation
				pmiSelected->NewClonedState(pDoc->m_fFadeTime);
				pmiSelected->AddAnimation(pan->an_iID,ulAnimFlags,fAnimStrenght,0);
				// if alt is presed 
			}
			else if(bAlt) 
			{
				// Add animation
				pmiSelected->NewClonedState(pDoc->m_fFadeTime);
				pmiSelected->RemAnimation(pan->an_iID);
				// else
			}
			else 
			{
				// Add animation
				pmiSelected->NewClearState(pDoc->m_fFadeTime);
				pmiSelected->AddAnimation(pan->an_iID,ulAnimFlags,fAnimStrenght,0);
			}
			
			// Set timer for current document
			pDoc->SetTimerForDocument();
			CTString strTreshold;
			CTString strSecPerFrame;
			CTString strZSpeed;
			CTString strLoopSec;
			strTreshold.PrintF("%g",pan->an_fThreshold);
			strSecPerFrame.PrintF("%g",pan->an_fSecPerFrame);
			strZSpeed.PrintF("%g",pDoc->m_fSpeedZ);
			strLoopSec.PrintF("%g",pDoc->m_fLoopSecends);
			m_tbTreshold.SetWindowText((const char*)strTreshold);
			m_tbAnimSpeed.SetWindowText((const char*)strSecPerFrame);
			m_tbWalkSpeed.SetWindowText((const char*)strZSpeed);
			m_tbWalkLoopSec.SetWindowText((const char*)strLoopSec);
			// check compresion 
			((CButton*)m_dlgAnimSet.GetDlgItem(IDC_CB_COMPRESION))->SetCheck(pan->an_bCompresed);
			CheckSecPerFrameCtrl(pan->an_bCustomSpeed);
			ShowControlGroup(GR_ANIMSET);
			SetCustomTabText("Animation");
		}
		break;
	case NT_MESHSURFACE:
		{
			// get mesh instance
			HTREEITEM hParentsParent = m_TreeCtrl.GetParentItem(hParent);
			NodeInfo &niParent = GetNodeInfo(hParent);
			NodeInfo &niParentsParent = GetNodeInfo(hParentsParent);
			
			MeshSurface *pmsrf = (MeshSurface*)ni.ni_pPtr;
			MeshLOD *pmlod = (MeshLOD*)niParent.ni_pPtr;
			MeshInstance *pmshi = (MeshInstance*)niParentsParent.ni_pPtr;
			
			ShowControlGroup(GR_SHADERS);
			ShowSurfaceShader(pmsrf,pmlod,pmshi);
			theApp.iSelectedItemID = pmsrf->msrf_iSurfaceID;
		}
		break;
	case NT_COLISIONBOX:
		{
			ColisionBox *pcb = (ColisionBox*)ni.ni_pPtr;
			INDEX iIndex = -1;
			// get colision box count
			INDEX ctcb = pmiSelected->mi_cbAABox.Count();
			// for each colision box in array
			for(INDEX icb=0;icb<ctcb;icb++) 
			{
				ColisionBox *pcb2 = &pmiSelected->mi_cbAABox[icb];
				if(pcb == pcb2) 
				{
					iIndex = icb;
					break;
				}
			}
			if(iIndex<0) return;
			// set it to be curent colision box
			pmiSelected->mi_iCurentBBox = iIndex;
			ShowControlGroup(GR_COLISION);
			SetCustomTabText("Colision");
		}
		break;
	case NT_ALLFRAMESBBOX:
		{
			ShowControlGroup(GR_ALLFRAMES_BBOX);
			SetCustomTabText("All frames");
		}
		break;
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	case NT_SKATAG:
		{
			//_pSelectedTag = (CTag *)ni.ni_pPtr;
			CTag *pTag = (CTag *)ni.ni_pPtr;
			_pSelectedTag = ni.pmi->m_tmSkaTagManager.Find(pTag->GetName());
			ShowControlGroup(GR_SKATAG);
			SetCustomTabText("SkaTag");
		}
		break;
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	case NT_ANIMEFFECT:
		{
			//_pSelectedTag = (CTag *)ni.ni_pPtr;
			ShowControlGroup(-1);
			SetCustomTabText("AnimEffect");
			CAnimEffect &ae = *(CAnimEffect*)ni.ni_pPtr;

			FLOAT fAnimStrenght = 1.0f;
			// start playing selected animation
			ULONG ulAnimFlags = 0;// | AN_NORESTART;
			
			if(pDoc->m_bFreezeOnBlend) 
			{
				ulAnimFlags |= AN_FREEZE_ON_BLEND;
			}
			
			// if looping animations
			if(pDoc->m_bAnimLoop) 
			{
				if(!bShift) ulAnimFlags |= AN_LOOPING;
			}
			else
			{
				if(bShift) ulAnimFlags |= AN_LOOPING;
			}
			
			if(bCtrl && bAlt) 
			{
				// Add animation
				pmiSelected->RemovePassedAnimsFromQueue();
				pmiSelected->AddAnimation(ae.GetAnimID(),(ulAnimFlags&(~AN_LOOPING))|AN_REMOVE_ON_END,g_fAnimStrength,0);
				// if alt is presed 
			}
			// if control is presed
			else if(bCtrl) 
			{
				// Add animation
				pmiSelected->NewClonedState(pDoc->m_fFadeTime);
				pmiSelected->AddAnimation(ae.GetAnimID(),ulAnimFlags,fAnimStrenght,0);
				// if alt is presed 
			}
			else if(bAlt) 
			{
				// Add animation
				pmiSelected->NewClonedState(pDoc->m_fFadeTime);
				pmiSelected->RemAnimation(ae.GetAnimID());
				// else
			}
			else 
			{
				// Add animation
				pmiSelected->NewClearState(pDoc->m_fFadeTime);
				pmiSelected->AddAnimation(ae.GetAnimID(),ulAnimFlags,fAnimStrenght,0);
			}
		}
		break;
	case NT_ANIMEFFECT_REG:
		{
			//_pSelectedTag = (CTag *)ni.ni_pPtr;
			ShowControlGroup(GR_ANIMEFFECT);
			SetCustomTabText("AnimEffect REG");
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
			CAnimEffect &ae = *(CAnimEffect*)pNIParent->ni_pPtr;
			INDEX iREG = (INDEX)ni.ni_pPtr;
			CAnimEffect::CReservedEffectGroup *pREG = ae.GetReservedEG(iREG);
			if(pREG != NULL)
			{
				m_tbAEAnimName.SetWindowText( (const char*)CTString(0,"%s", ae.GetAnimName()) );
				m_tbAEStartTime.SetWindowText( (const char*)CTString(0,"%1.6f", pREG->GetStartTime()) );
				m_tbAEEffectGroupName.SetWindowText( (const char*)CTString(0,"%s", pREG->GetEffectGroupName()) );
				CListBox &lb = *((CListBox*)m_dlgAnimEffect.GetDlgItem(IDC_ANIMEFFECT_FLAGS));
				lb.SetSel(-1, FALSE);
				for(INDEX iFlag=0; iFlag<32; ++iFlag)
				{
					if(pREG->GetFlags() & (1UL << iFlag)) lb.SetSel(iFlag);
				}
			}
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
		}
		break;
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
	case NT_RIDINGPOSITION:
		{
			ShowControlGroup(GR_RIDINGPOSITION);
			SetCustomTabText("Riding Position");
		}
		break;
	default:
		// no custom group
		//iShowCustomGroup = -1;
		ShowControlGroup(-1);
		SetCustomTabText("Custom");
		break;
	}
	
	// show name of selected model instance
	GetDlgItem(IDC_SELECTEDMI)->SetWindowText(pmiSelected->GetName());
	
	// show offset of model instance 
	FLOATmatrix3D mat;
	FLOAT3D vPos = pmiSelected->mi_qvOffset.vPos;
	ANGLE3D aRot;
	pmiSelected->mi_qvOffset.qRot.ToMatrix(mat);
	DecomposeRotationMatrix(aRot,mat);
	
	m_tbOffPosX.SetWindowText((const char*)CTString(0,"%g",vPos(1)));
	m_tbOffPosY.SetWindowText((const char*)CTString(0,"%g",vPos(2)));
	m_tbOffPosZ.SetWindowText((const char*)CTString(0,"%g",vPos(3)));
	m_tbOffRotH.SetWindowText((const char*)CTString(0,"%g",aRot(1)));
	m_tbOffRotP.SetWindowText((const char*)CTString(0,"%g",aRot(2)));
	m_tbOffRotB.SetWindowText((const char*)CTString(0,"%g",aRot(3)));
	
	// show colision box values
	if((pmiSelected->mi_iCurentBBox >= 0) && (pmiSelected->mi_iCurentBBox < pmiSelected->mi_cbAABox.Count()))
	{
		ColisionBox &cb = pmiSelected->mi_cbAABox[pmiSelected->mi_iCurentBBox];
		FLOAT fWidth = (cb.Max()(1)-cb.Min()(1));
		FLOAT fHeight = (cb.Max()(2)-cb.Min()(2));
		FLOAT fLength = (cb.Max()(3)-cb.Min()(3));
		FLOAT fPosX = (cb.Max()(1)+cb.Min()(1)) / 2;
		FLOAT fPosY = (cb.Max()(2)+cb.Min()(2)) / 2;
		FLOAT fPosZ = (cb.Max()(3)+cb.Min()(3)) / 2;
		fPosY -= fHeight/2;
		m_tbColName.SetWindowText((const char*)cb.GetName());
		m_tbColWidth.SetWindowText((const char*)CTString(0,"%g",fWidth));
		m_tbColHeight.SetWindowText((const char*)CTString(0,"%g",fHeight));
		m_tbColLength.SetWindowText((const char*)CTString(0,"%g",fLength));
		m_tbColPosX.SetWindowText((const char*)CTString(0,"%g",fPosX));
		m_tbColPosY.SetWindowText((const char*)CTString(0,"%g",fPosY));
		m_tbColPosZ.SetWindowText((const char*)CTString(0,"%g",fPosZ));
	}
	// show all frames bbox
	ColisionBox &cb = pmiSelected->mi_cbAllFramesBBox;
	FLOAT fWidth = (cb.Max()(1)-cb.Min()(1));
	FLOAT fHeight = (cb.Max()(2)-cb.Min()(2));
	FLOAT fLength = (cb.Max()(3)-cb.Min()(3));
	FLOAT fPosX = (cb.Max()(1)+cb.Min()(1)) / 2;
	FLOAT fPosY = (cb.Max()(2)+cb.Min()(2)) / 2;
	FLOAT fPosZ = (cb.Max()(3)+cb.Min()(3)) / 2;
	fPosY -= fHeight/2;
	m_tbAFBBWidth.SetWindowText((const char*)CTString(0,"%g",fWidth));
	m_tbAFBBHeight.SetWindowText((const char*)CTString(0,"%g",fHeight));
	m_tbAFBBLength.SetWindowText((const char*)CTString(0,"%g",fLength));
	m_tbAFBBPosX.SetWindowText((const char*)CTString(0,"%g",fPosX));
	m_tbAFBBPosY.SetWindowText((const char*)CTString(0,"%g",fPosY));
	m_tbAFBBPosZ.SetWindowText((const char*)CTString(0,"%g",fPosZ));
	// change custom controls visibility
	//  ShowControlGroup(iVisibleGroup);
	
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	//��Ÿ �ൿ, �ʵ带 ä�� �ִ� ���� ���� �Ѵ�.
	if(_pSelectedTag == NULL)
	{
		m_tbSTName.SetWindowText("");
		m_tbSTBlendFactor1.SetWindowText("");
		m_tbSTBlendFactor2.SetWindowText("");
		m_tbSTBlendFactor3.SetWindowText("");
		m_tbSTBlendFactor4.SetWindowText("");
		m_tbSTOffsetPosX.SetWindowText("");
		m_tbSTOffsetPosY.SetWindowText("");
		m_tbSTOffsetPosZ.SetWindowText("");
		m_tbSTOffsetRotAxisX.SetWindowText("");
		m_tbSTOffsetRotAxisY.SetWindowText("");
		m_tbSTOffsetRotAxisZ.SetWindowText("");
		m_tbSTOffsetRotAngle.SetWindowText("");
		if(pmiSelected->mi_psklSkeleton != NULL)
		{
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE1);
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE2);
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE3);
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE4);
		}
		((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE1))->EnableWindow(FALSE);
		((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE2))->EnableWindow(FALSE);
		((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE3))->EnableWindow(FALSE);
		((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE4))->EnableWindow(FALSE);
	}
	else if(_pSelectedTag->GetType() == TT_SKA)
	{
		CSkaTag *pSkaTag = (CSkaTag*)_pSelectedTag;
		m_tbSTName.SetWindowText( pSkaTag->GetName().c_str() );
		m_tbSTBlendFactor1.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetBoneWeight(0)) );
		m_tbSTBlendFactor2.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetBoneWeight(1)) );
		m_tbSTBlendFactor3.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetBoneWeight(2)) );
		m_tbSTBlendFactor4.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetBoneWeight(3)) );
		m_tbSTOffsetPosX.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetOffsetPos()(1)) );
		m_tbSTOffsetPosY.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetOffsetPos()(2)) );
		m_tbSTOffsetPosZ.SetWindowText( (const char*)CTString(0,"%g",pSkaTag->GetOffsetPos()(3)) );
		FLOAT3D axis;
		FLOAT angle;
		pSkaTag->GetOffsetRot().ToAxisAngle(axis, angle);
		m_tbSTOffsetRotAxisX.SetWindowText( (const char*)CTString(0,"%g",axis(1)) );
		m_tbSTOffsetRotAxisY.SetWindowText( (const char*)CTString(0,"%g",axis(2)) );
		m_tbSTOffsetRotAxisZ.SetWindowText( (const char*)CTString(0,"%g",axis(3)) );
		m_tbSTOffsetRotAngle.SetWindowText( (const char*)CTString(0,"%g",AngleRad(angle)) );
		if(pmiSelected->mi_psklSkeleton != NULL)
		{
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE1);
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE2);
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE3);
			FillBonesToComboBox(m_dlgSkaTag, pmiSelected->mi_psklSkeleton,0,IDC_SKATAG_PARENTBONE4);
		}

		MeshLOD &mlod = *pSkaTag->GetMeshLOD();
		if(pmiSelected->mi_psklSkeleton != NULL && mlod.mlod_aWeightMaps.Count() > pSkaTag->GetBoneID(0))
		{
			// get name of bone
			CTString strBoneName = ska_IDToString(mlod.mlod_aWeightMaps[pSkaTag->GetBoneID(0)].mwm_iID);
			// select parent bone in combo box
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE1))->SelectString(-1,(const char*)strBoneName);
			if(pSkaTag->GetBoneWeight(0) > 0.0f) ((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE1))->EnableWindow();
			strBoneName = ska_IDToString(mlod.mlod_aWeightMaps[pSkaTag->GetBoneID(1)].mwm_iID);
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE2))->SelectString(-1,(const char*)strBoneName);
			if(pSkaTag->GetBoneWeight(1) > 0.0f) ((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE2))->EnableWindow();
			strBoneName = ska_IDToString(mlod.mlod_aWeightMaps[pSkaTag->GetBoneID(2)].mwm_iID);
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE3))->SelectString(-1,(const char*)strBoneName);
			if(pSkaTag->GetBoneWeight(2) > 0.0f) ((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE3))->EnableWindow();
			strBoneName = ska_IDToString(mlod.mlod_aWeightMaps[pSkaTag->GetBoneID(3)].mwm_iID);
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE4))->SelectString(-1,(const char*)strBoneName);
			if(pSkaTag->GetBoneWeight(3) > 0.0f) ((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE4))->EnableWindow();
		}
		else
		{
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE1))->EnableWindow(FALSE);
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE2))->EnableWindow(FALSE);
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE3))->EnableWindow(FALSE);
			((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE4))->EnableWindow(FALSE);
		}
	}
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
}

// calculate size of tree view
CSize CDlgBarTreeView::CalcLayout(int nLength, DWORD nMode)
{
	CSize csResult;
	// Return default if it is being docked or floated
	if(nMode & LM_VERTDOCK)
	{
		csResult = m_Size;
		CRect rc;
		// get main frm
		CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
		// get his child
		CMDIClientWnd *pMDIClient = &pMainFrame->m_wndMDIClient;
		pMDIClient->GetWindowRect(rc);
		csResult.cy = rc.bottom - rc.top;
	}
	else if((nMode & LM_VERTDOCK) || (nMode & LM_HORZDOCK))
	{
		if (nMode & LM_STRETCH) // if not docked stretch to fit
		{
			csResult = CSize((nMode & LM_HORZ) ? 32767 : m_Size.cx,
				(nMode & LM_HORZ) ? m_Size.cy : 32767);
		}
		else
		{
			csResult = m_Size;
		}
	}
	else if (nMode & LM_MRUWIDTH)
	{
		csResult = m_Size;
	}
	// In all other cases, accept the dynamic length
	else
	{
		if (nMode & LM_LENGTHY)
		{
			// Note that we don't change m_Size.cy because we disabled vertical sizing
			csResult = CSize( m_Size.cx, m_Size.cy = nLength);
		}
		else
		{
			csResult = CSize( m_Size.cx = nLength, m_Size.cy);
		}
	}
	//  csResult = CSize(300,300);
	return csResult;
}

// resize dialog
CSize CDlgBarTreeView::CalcDynamicLayout(int nLength, DWORD nMode)
{
	// return CDlgTemplate::CalcDynamicLayout(nLength, nMode);
	CSize csResult = CalcLayout(nLength,nMode);
	if(theApp.IsErrorDlgVisible()) 
	{
		csResult.cy+=theApp.GetLogDlgSize().cy;
	}
	
#define H_BORDER  20
#define V_BORDER  10
#define LINE1     15
#define LINE2     35
#define LINE3     55
#define LINE1U     5
#define LINE2U    25
#define LINE3U    45
#define LINE4U    65
	
//������ ���� ����	//(Ska Shader Configue Panel Size Fix)(0.1)
#define YRESIZE_BY_ANT	160
//������ ���� ��	//(Ska Shader Configue Panel Size Fix)(0.1)

#define WIT (csResult.cx-2*H_BORDER)/4
#define YOFFSET (csResult.cy - V_BORDER - 140)

	// move tab control
//������ ���� ����	//(Ska Shader Configue Panel Size Fix)(0.1)
	GetDlgItem(IDC_MODE_SELECT_TAB)->MoveWindow(CRect(H_BORDER/2, YOFFSET-30-YRESIZE_BY_ANT, csResult.cx-H_BORDER/2, YOFFSET+140));
//������ ���� ��	//(Ska Shader Configue Panel Size Fix)(0.1)
	// move dialogs
//������ ���� ����	//(Ska Shader Configue Panel Size Fix)(0.1)
	CRect rcDlg = CRect(2, 6, csResult.cx-H_BORDER/2-13, 165+YRESIZE_BY_ANT); // !!! fix
//������ ���� ��	//(Ska Shader Configue Panel Size Fix)(0.1)
	ResizeDlgWithChildren(&m_dlgParent,rcDlg);
	ResizeDlgWithChildren(&m_dlgAnimSet,rcDlg);
	ResizeDlgWithChildren(&m_dlgColision,rcDlg);
	ResizeDlgWithChildren(&m_dlgAllFrames,rcDlg);
	ResizeDlgWithChildren(&m_dlgLod,rcDlg);
	ResizeDlgWithChildren(&m_dlgBone,rcDlg);
	ResizeDlgWithChildren(&m_dlgTexture,rcDlg);
	ResizeDlgWithChildren(&m_dlgListOpt,rcDlg);
	ResizeDlgWithChildren(&m_dlgShader,rcDlg);
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	ResizeDlgWithChildren(&m_dlgSkaTag,rcDlg);
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
	ResizeDlgWithChildren(&m_dlgRideSet,rcDlg);
	
	AdjustSplitter();
	
	/*
	if(iDockSide == AFX_IDW_DOCKBAR_LEFT)
	{
	GetDlgItem(IDC_SPLITER_FRAME)->MoveWindow(CRect(csResult.cx-SPLITTER_WITDH,0,csResult.cx,csResult.cy));
	GetDlgItem(IDC_SPLITER_FRAME)->ShowWindow(SW_SHOW);
	}
	else if(iDockSide == AFX_IDW_DOCKBAR_RIGHT)
	{
	GetDlgItem(IDC_SPLITER_FRAME)->MoveWindow(CRect(0,0,SPLITTER_WITDH,csResult.cy));
	GetDlgItem(IDC_SPLITER_FRAME)->ShowWindow(SW_SHOW);
	}
	else
	{
	GetDlgItem(IDC_SPLITER_FRAME)->ShowWindow(SW_HIDE);
	}
	*/
	
	// move label that display curent selected mi                 
	GetDlgItem(IDC_SELECTEDMI)->MoveWindow(CRect(H_BORDER, 3, csResult.cx-H_BORDER, V_BORDER + 15));
	// move tree ctrl
	CRect NewTreePos;
//������ ���� ����	//(Ska Shader Configue Panel Size Fix)(0.1)
	NewTreePos = CRect(H_BORDER/2, V_BORDER + 15, csResult.cx - H_BORDER/2, csResult.cy - V_BORDER - 180 - YRESIZE_BY_ANT);
//������ ���� ��	//(Ska Shader Configue Panel Size Fix)(0.1)
	CWnd *pwndTree = GetDlgItem(IDC_TREE1);
	pwndTree->MoveWindow(NewTreePos);
	pwndTree->UpdateWindow();
	UpdateWindow();
	return csResult;
}

void CDlgBarTreeView::ResizeDlgWithChildren(CDialog *pDlg,CRect rcDlg)
{
	INDEX iDlgWidth = rcDlg.right - rcDlg.left;
	INDEX ctctrl = dlg_aControls.Count();
	// for each control in array of contrls
	for(INDEX ictrl=0;ictrl<ctctrl;ictrl++)
	{
		Control &ctrl = dlg_aControls[ictrl];
		// check if its dialog is being resized
		if(ctrl.ct_pParentDlg == pDlg)
		{
			// resize control
			CRect rcCtrl;
			rcCtrl.left = iDlgWidth * ctrl.ct_fLeft;
			rcCtrl.right = iDlgWidth * ctrl.ct_fRight;
			rcCtrl.top = ctrl.ct_iTop;
			rcCtrl.bottom = ctrl.ct_iBottom;
			pDlg->GetDlgItem(ctrl.ct_iID)->MoveWindow(rcCtrl);
		}
	}
	int l,r,t,b;
	l = rcDlg.left;
	r = rcDlg.right;
	t = rcDlg.top;
	b = rcDlg.bottom;
	// resize dialog
	pDlg->MoveWindow(rcDlg);
}

// remove control form array 
void CDlgBarTreeView::RemoveControlFromArray(CWnd *pChild, CDialog *pDlg)
{
	// get control ID
	INDEX ictrlID = pChild->GetDlgCtrlID();
	// count controls in array
	INDEX ctctrl = dlg_aControls.Count();
	// for each control in array
	for(INDEX ictrl=0;ictrl<ctctrl;ictrl++) 
	{
		Control &ctrl = dlg_aControls[ictrl];
		// check if this is control to remove
		if((ctrl.ct_iID == ictrlID) && (ctrl.ct_pParentDlg == pDlg)) 
		{
			// get last ctrl
			Control &ctrlLast = dlg_aControls[ctctrl-1];
			// copy last control insted of one that has to be removed
			ctrl = ctrlLast;
			// remove last control from array
			dlg_aControls.Pop();
			return;
		}
	}
}

// add control to array witch stores all controls that need to be dynamicly resized
void CDlgBarTreeView::AddControlToArray(CWnd *pChild, CDialog *pDlg)
{
	CRect rcParent;
	CRect rcChild;
	
	pDlg->GetWindowRect(rcParent);
	pChild->GetWindowRect(rcChild);
	
	INDEX iParentWidth = rcParent.right - rcParent.left;
	// convert to parent coords
	pDlg->ScreenToClient(rcChild);
	
	Control &ctrl = dlg_aControls.Push();
	ctrl.ct_iID = pChild->GetDlgCtrlID();
	ctrl.ct_pParentDlg = pDlg;
	ctrl.ct_iTop = rcChild.top;
	ctrl.ct_iBottom = rcChild.bottom;
	ctrl.ct_fLeft = ((FLOAT)rcChild.left/(FLOAT)iParentWidth);
	ctrl.ct_fRight = ((FLOAT)rcChild.right/(FLOAT)iParentWidth);
}

// remove controls from array
void CDlgBarTreeView::RemoveDialogControls(CDialog *pDlg)
{
	CWnd *pChild = pDlg->GetWindow(GW_CHILD);
	while(pChild!=NULL) 
	{
		RemoveControlFromArray(pChild,pDlg);
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
}

// add all controls from dialog to array
void CDlgBarTreeView::AddDialogControls(CDialog *pDlg)
{
	CWnd *pChild = pDlg->GetWindow(GW_CHILD);
	while(pChild!=NULL) 
	{
		AddControlToArray(pChild,pDlg);
		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}
}

// create dialog 
BOOL CDlgBarTreeView::Create( CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	CRect rectDummy(0,0,0,0);
	if(!CDlgTemplate::Create(pParentWnd,nIDTemplate,nStyle,nID)) return FALSE;
	m_Size = m_sizeDefault;  
	SetSplitterControlID(IDC_SPLITTER_TREE);
	// spliter
	// m_wndSpliterFrame.SubclassDlgItem(IDC_SPLITTER_TREE,this);
	// m_wndSpliterFrame.SetDockingSide(AFX_IDW_DOCKBAR_FLOAT);
	
	// tree ctrl
	m_IconsImageList.Create( IDB_BITMAP1, 16, 32, CLR_NONE);
	m_TreeCtrl.SubclassDlgItem(IDC_TREE1, this);
	m_TreeCtrl.SetImageList( &m_IconsImageList, TVSIL_NORMAL);
	// add tab control buttons
	CTabCtrl *pTabCtrl = (CTabCtrl*)GetDlgItem(IDC_MODE_SELECT_TAB);
	
	// create dialogs
	m_dlgParent.Create(IDD_PARENT,pTabCtrl);
	m_dlgAnimSet.Create(IDD_ANIMSET,pTabCtrl);
	m_dlgColision.Create(IDD_COLISION,pTabCtrl);
	m_dlgAllFrames.Create(IDD_ALL_FRAMES_BBOX,pTabCtrl);
	m_dlgLod.Create(IDD_LOD,pTabCtrl);
	m_dlgBone.Create(IDD_BONE,pTabCtrl);
	m_dlgTexture.Create(IDD_TEXTURE,pTabCtrl);
	m_dlgListOpt.Create(IDD_LIST_OPTIONS,pTabCtrl);
	m_dlgShader.Create(IDD_SHADER,pTabCtrl);
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	m_dlgSkaTag.Create(IDD_SKATAG,pTabCtrl);
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	m_dlgAnimEffect.Create(IDD_ANIMEFFECT,pTabCtrl);
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
	m_dlgRideSet.Create(IDD_RIDESET, pTabCtrl);
	
	
	CRect rcDummy = CRect(0,0,100,20);
	
	AddDialogControls(&m_dlgTexture);
	AddDialogControls(&m_dlgListOpt);
	AddDialogControls(&m_dlgParent);
	AddDialogControls(&m_dlgAnimSet);
	AddDialogControls(&m_dlgColision);
	AddDialogControls(&m_dlgAllFrames);
	AddDialogControls(&m_dlgLod);
	AddDialogControls(&m_dlgBone);
	AddDialogControls(&m_dlgShader);
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	AddDialogControls(&m_dlgSkaTag);
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	AddDialogControls(&m_dlgAnimEffect);
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
	AddDialogControls(&m_dlgRideSet);
	
	// subclass controls in parent dialog
	m_tbOffPosX.SubclassDlgItem(IDC_TB_OFFSET_POSX, &m_dlgParent);
	m_tbOffPosY.SubclassDlgItem(IDC_TB_OFFSET_POSY, &m_dlgParent);
	m_tbOffPosZ.SubclassDlgItem(IDC_TB_OFFSET_POSZ, &m_dlgParent);
	m_tbOffRotH.SubclassDlgItem(IDC_TB_OFFSET_ROTH, &m_dlgParent);
	m_tbOffRotP.SubclassDlgItem(IDC_TB_OFFSET_ROTP, &m_dlgParent);
	m_tbOffRotB.SubclassDlgItem(IDC_TB_OFFSET_ROTB, &m_dlgParent);
	m_tbMiName.SubclassDlgItem(IDC_TB_MI_NAME, &m_dlgParent); 
	
	// subclass controls in animset dialog
	m_tbTreshold.SubclassDlgItem(IDC_EB_TRESHOLD    ,&m_dlgAnimSet);
	m_tbAnimSpeed.SubclassDlgItem(IDC_EB_SECPERFRAME,&m_dlgAnimSet);
	m_tbWalkSpeed.SubclassDlgItem(IDC_TB_ZTRANSSPEED,&m_dlgAnimSet);
	m_tbWalkLoopSec.SubclassDlgItem(IDC_TB_ZTRANSLOOP,&m_dlgAnimSet);
	// subclass controls in colision dialog
	m_tbColName.SubclassDlgItem(IDC_TB_COLNAME,   &m_dlgColision);
	m_tbColWidth.SubclassDlgItem(IDC_TB_COLWIDTH, &m_dlgColision);
	m_tbColHeight.SubclassDlgItem(IDC_TB_COLHEIGHT,&m_dlgColision);
	m_tbColLength.SubclassDlgItem(IDC_TB_COLLENGTH,&m_dlgColision);
	m_tbColPosX.SubclassDlgItem(IDC_TB_COLPOSX,   &m_dlgColision);
	m_tbColPosY.SubclassDlgItem(IDC_TB_COLPOSY,   &m_dlgColision);
	m_tbColPosZ.SubclassDlgItem(IDC_TB_COLPOSZ,   &m_dlgColision);
	
	// subclass controls in colision dialog
	m_tbAFBBWidth.SubclassDlgItem(IDC_TB_COLWIDTH, &m_dlgAllFrames);
	m_tbAFBBHeight.SubclassDlgItem(IDC_TB_COLHEIGHT,&m_dlgAllFrames);
	m_tbAFBBLength.SubclassDlgItem(IDC_TB_COLLENGTH,&m_dlgAllFrames);
	m_tbAFBBPosX.SubclassDlgItem(IDC_TB_COLPOSX,   &m_dlgAllFrames);
	m_tbAFBBPosY.SubclassDlgItem(IDC_TB_COLPOSY,   &m_dlgAllFrames);
	m_tbAFBBPosZ.SubclassDlgItem(IDC_TB_COLPOSZ,   &m_dlgAllFrames);
	
	m_tbDistance.SubclassDlgItem(IDC_EB_DISTANCE, &m_dlgLod);
	
	m_tbTextureName.SubclassDlgItem(IDC_EB_TEXTURENAME, &m_dlgTexture); 
	
	CWnd *pTexViewFrame = m_dlgTexture.GetDlgItem(IDC_TEXTURE_VIEW);
	// CRect rc;
	// pTexViewFrame->GetClientRect(&rc);
	CRect rc = CRect(0,0,200,100);
	m_tvTexView.Create( NULL, NULL, WS_BORDER|WS_VISIBLE, rc, pTexViewFrame,IDC_TEXTURE_VIEW);
	m_tvTexView.SubclassDlgItem(IDC_TEXTURE_VIEW, &m_dlgTexture);
	
	// set width of shader combo box
	((CComboBox*)m_dlgShader.GetDlgItem(IDC_CB_SHADER))->SetDroppedWidth(200);
	
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
	//���� m_dlgSkaTag�� ���θ� ä���.
	m_tbSTName.SubclassDlgItem(IDC_SKATAG_NAME, &m_dlgSkaTag);
	m_tbSTBlendFactor1.SubclassDlgItem(IDC_SKATAG_BLENDFACTOR1, &m_dlgSkaTag);
	m_tbSTBlendFactor2.SubclassDlgItem(IDC_SKATAG_BLENDFACTOR2, &m_dlgSkaTag);
	m_tbSTBlendFactor3.SubclassDlgItem(IDC_SKATAG_BLENDFACTOR3, &m_dlgSkaTag);
	m_tbSTBlendFactor4.SubclassDlgItem(IDC_SKATAG_BLENDFACTOR4, &m_dlgSkaTag);
	m_tbSTOffsetPosX.SubclassDlgItem(IDC_SKATAG_OFFSETPOSX, &m_dlgSkaTag);
	m_tbSTOffsetPosY.SubclassDlgItem(IDC_SKATAG_OFFSETPOSY, &m_dlgSkaTag);
	m_tbSTOffsetPosZ.SubclassDlgItem(IDC_SKATAG_OFFSETPOSZ, &m_dlgSkaTag);
	m_tbSTOffsetRotAxisX.SubclassDlgItem(IDC_SKATAG_OFFSETROTAXISX, &m_dlgSkaTag);
	m_tbSTOffsetRotAxisY.SubclassDlgItem(IDC_SKATAG_OFFSETROTAXISY, &m_dlgSkaTag);
	m_tbSTOffsetRotAxisZ.SubclassDlgItem(IDC_SKATAG_OFFSETROTAXISZ, &m_dlgSkaTag);
	m_tbSTOffsetRotAngle.SubclassDlgItem(IDC_SKATAG_OFFSETROTANGLE, &m_dlgSkaTag);
	((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE1))->SetDroppedWidth(200);
	((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE2))->SetDroppedWidth(200);
	((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE3))->SetDroppedWidth(200);
	((CComboBox*)m_dlgSkaTag.GetDlgItem(IDC_SKATAG_PARENTBONE4))->SetDroppedWidth(200);
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	//���� m_dlgAnimEffect�� ���θ� ä���.
	m_tbAEAnimName.SubclassDlgItem(IDC_ANIMEFFECT_ANIMNAME, &m_dlgAnimEffect);
	m_tbAEStartTime.SubclassDlgItem(IDC_ANIMEFFECT_STARTTIME, &m_dlgAnimEffect);
	m_tbAEEffectGroupName.SubclassDlgItem(IDC_ANIMEFFECT_EFFECTGROUPNAME, &m_dlgAnimEffect);
	((CListBox*)m_dlgAnimEffect.GetDlgItem(IDC_ANIMEFFECT_FLAGS))->ShowWindow(SW_SHOW);
	((CListBox*)m_dlgAnimEffect.GetDlgItem(IDC_ANIMEFFECT_FLAGS))->AddString("Sync Anim Length");
	((CListBox*)m_dlgAnimEffect.GetDlgItem(IDC_ANIMEFFECT_FLAGS))->AddString("Sync Anim Loop");
	((CListBox*)m_dlgAnimEffect.GetDlgItem(IDC_ANIMEFFECT_FLAGS))->AddString("Sync Anim Speed");
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)

	CMainFrame* pMainFrame = STATIC_DOWNCAST(CMainFrame, AfxGetMainWnd());
	pMainFrame->m_ctrlMIStretch.SetFont(m_dlgShader.GetFont(),FALSE);
//������ ���� ����	//(Open beta)(2004-11-30)
	pMainFrame->m_ctrlMIAlpha.SetFont(m_dlgShader.GetFont(),FALSE);
//������ ���� ��	//(Open beta)(2004-11-30)
	return TRUE;
}

// add node from tree view to nodeinfo array
INDEX AddNode(INDEX iType,void *ni_pPtr,CModelInstance *pmi)
{
	INDEX ctni = theApp.aNodeInfo.Count();
	NodeInfo &ni = theApp.aNodeInfo.Push();
	ni.ni_iType = iType;
	ni.pmi = pmi;
	ni.ni_pPtr = ni_pPtr;
	ni.ni_bSelected = FALSE;
	return ctni;
}

// add model instance to tree view
HTREEITEM CDlgBarTreeView::AddModelInst(CModelInstance &mi, CModelInstance *pmiParent, HTREEITEM hParent)
{
	// insert model instance item
	HTREEITEM hItem;
	// expand only root model
	if(hParent == TVI_ROOT) 
	{
		// add parent model instance
		hItem = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE , "", 0, 0, TVIS_EXPANDED, TVIS_EXPANDED, 0, hParent, 0 );
		m_TreeCtrl.SetItemText(hItem,(const char*)mi.GetName());
	}
	else 
	{
		int iIcon = 0;
		if(mi.mi_fnSourceFile != pmiParent->mi_fnSourceFile) iIcon = 8;
		
		// add child model instance
		hItem = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE , "", iIcon, iIcon, TVIS_EXPANDED, TVIS_EXPANDED, 0, hParent, 0 );
		// get parent bone name
		CTString strParentBoneName;
		if(mi.mi_iParentBoneID!=(-1)) 
		{
			strParentBoneName = ska_GetStringFromTable(mi.mi_iParentBoneID);
		}
		CTString strText = mi.GetName() + " [" + strParentBoneName + "]";
		m_TreeCtrl.SetItemText(hItem,(const char*)strText);
	}
	m_TreeCtrl.SetItemData(hItem,AddNode(NT_MODELINSTANCE,&mi,pmiParent));
	
	// add its mesh instances
	AddMeshInstances(mi,hItem);
	// if skeleton exists 
	if(mi.mi_psklSkeleton != NULL) 
	{
		// add skeleton
		AddSkeleton(mi,hItem);
	}
	// add animsets  
	AddAnimSet(mi,hItem);
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
	AddAnimEffects(mi,hItem);
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
	// add colision boxes
	AddColisionBoxes(mi,hItem);
	// add all frames colision box
	AddAllFramesBBox(mi,hItem);
	// add model instance children
	INDEX ctmi = mi.mi_cmiChildren.Count();
	// for each child in model isntance
	for(INDEX imi=0;imi<ctmi;imi++) 
	{
		// add child 
		AddModelInst(mi.mi_cmiChildren[imi],&mi,hItem);
	}

	AddRideSetting(mi, hItem);
	return hItem;
}

// add skeleton to tree view
void CDlgBarTreeView::AddSkeleton(CModelInstance &mi, HTREEITEM hParent)
{
	if(mi.mi_psklSkeleton == NULL) return;
	CSkeleton &sk = *mi.mi_psklSkeleton;
	// insert skeleton
	HTREEITEM hItem = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE  , "", 5, 5, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0 );
	CTString strSkeletonName;
	strSkeletonName.PrintF("%s [%d]",(const char*)sk.GetName().FileName(),sk.skl_aSkeletonLODs.Count());
	
	m_TreeCtrl.SetItemText(hItem,strSkeletonName);
	m_TreeCtrl.SetItemData(hItem,AddNode(NT_SKELETONLODLIST,&sk,&mi));
	
	INDEX ctslod = sk.skl_aSkeletonLODs.Count();
	for(INDEX islod=0;islod<ctslod;islod++) 
	{
		SkeletonLOD &slod = sk.skl_aSkeletonLODs[islod];
		// insert skeleton lod
		HTREEITEM hSlod = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "", 5, 5, TVIS_SELECTED, TVIF_STATE, 0, hItem, 0 );
		// count bones
		INDEX ctb = slod.slod_aBones.Count();
		
		CTString strText;
		CTFileName fnSlodSource = sk.skl_aSkeletonLODs[islod].slod_fnSourceFile;
		fnSlodSource = fnSlodSource.FileName();
		strText.PrintF("%s [D-%g,B-%d]",(const char*)fnSlodSource.NoExt(),sk.skl_aSkeletonLODs[islod].slod_fMaxDistance,ctb);
		m_TreeCtrl.SetItemText(hSlod,strText);
		m_TreeCtrl.SetItemData(hSlod,AddNode(NT_SKELETONLOD,&slod,&mi));
		
		for(INDEX ib=0;ib<ctb;ib++) 
		{
			SkeletonBone &sb = sk.skl_aSkeletonLODs[islod].slod_aBones[ib];
			// insert bone
			HTREEITEM hBone = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "", 7, 7, TVIS_SELECTED, TVIF_STATE, 0, hSlod , 0 );
			m_TreeCtrl.SetItemText(hBone,ska_GetStringFromTable(sb.sb_iID));
			m_TreeCtrl.SetItemData(hBone,AddNode(NT_BONE,&sb,&mi));
		}
	}
}

// add mesh surfaces to tree view
void CDlgBarTreeView::AddSurfaces(CModelInstance &mi,MeshLOD &mlod,HTREEITEM hParent)
{
	INDEX ctsrf = mlod.mlod_aSurfaces.Count();
	for(INDEX isrf=0;isrf<ctsrf;isrf++) 
	{
		MeshSurface &msrf = mlod.mlod_aSurfaces[isrf];
		// show surface only if its not subsurface
		if(!(msrf.msrf_ulFlags&MS_MESH_SUBSURFACE) || theApp.m_bDisplaySubSurfaces) 
		{
			HTREEITEM hItem = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  11, 11, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
			CTString strSurfName;
			strSurfName.PrintF("%s [%d]",(const char*)ska_GetStringFromTable(msrf.msrf_iSurfaceID), msrf.msrf_auwTriangles.Count());
			if(theApp.m_bDisplaySubSurfaces && msrf.msrf_ulFlags&MS_DYNAMIC_SURFACE) 
			{
				strSurfName += " [D]";
			}
			m_TreeCtrl.SetItemText(hItem,(const char*)strSurfName);
			m_TreeCtrl.SetItemData(hItem,AddNode(NT_MESHSURFACE,&msrf,&mi));
		}
	}
}

// add weight maps to tree view
void CDlgBarTreeView::AddWeightMaps(CModelInstance &mi,MeshLOD &mlod,HTREEITEM hParent)
{
	const INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
	if(ctmwm==0) 
	{
		return;
	}
	HTREEITEM hWMaps = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  0, 0, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
	CTString strWMapsCount = CTString(0,"Weight maps %d", ctmwm);
	m_TreeCtrl.SetItemText(hWMaps,(const char*)strWMapsCount);
	m_TreeCtrl.SetItemData(hWMaps,AddNode(NT_WMAPSCOUNT,NULL,&mi));
	
	// for each weight map
	for(INDEX imwm=0;imwm<ctmwm;imwm++) 
	{
		MeshWeightMap &mwm = mlod.mlod_aWeightMaps[imwm];
		HTREEITEM hmwm = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "", 16,16, TVIS_SELECTED, TVIF_STATE, 0, hWMaps, 0);
		CTString strWMap = CTString(0,"%s [V-%d]", (const char*)ska_IDToString(mwm.mwm_iID), mwm.mwm_aVertexWeight.Count());
		m_TreeCtrl.SetItemText(hmwm,(const char*)strWMap);
		m_TreeCtrl.SetItemData(hmwm,AddNode(NT_WEIGHT_MAP,&mwm,&mi));
	}
}

//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
void CDlgBarTreeView::AddSkaTags(CModelInstance &mi,MeshLOD &mlod,HTREEITEM hParent)
{
	const INDEX cnt = mlod.mlod_aSkaTags.Count();
	HTREEITEM hSkaTag = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  0, 0, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
	CTString strSkaTagTitle = CTString("Ska Tags");
	m_TreeCtrl.SetItemText(hSkaTag,(const char*)strSkaTagTitle);
	m_TreeCtrl.SetItemData(hSkaTag,AddNode(NT_SKATAG_GROUP,NULL,&mi));
	
	// for each weight map
	for(INDEX iTag=0; iTag<cnt; ++iTag) 
	{
//������ ���� ����	//(Remake Effect)(0.1)
		CTag *pTag = mlod.mlod_aSkaTags[iTag].GetNative();
//������ ���� ��	//(Remake Effect)(0.1)
		HTREEITEM hItem = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "", 19,19, TVIS_SELECTED, TVIF_STATE, 0, hSkaTag, 0);
		m_TreeCtrl.SetItemText(hItem,pTag->GetName().c_str());
		m_TreeCtrl.SetItemData(hItem,AddNode(NT_SKATAG, pTag, &mi));
	}
}
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)


// add morph maps to tree view
void CDlgBarTreeView::AddMorphMaps(CModelInstance &mi,MeshLOD &mlod,HTREEITEM hParent)
{
	const INDEX ctmmm = mlod.mlod_aMorphMaps.Count();
	if(ctmmm==0) 
	{
		return;
	}
	HTREEITEM hMMaps = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  0, 0, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
	CTString strMMapsCount = CTString(0,"Morph maps %d", ctmmm);
	m_TreeCtrl.SetItemText(hMMaps,(const char*)strMMapsCount);
	m_TreeCtrl.SetItemData(hMMaps,AddNode(NT_MMAPSCOUNT,NULL,&mi));
	
	// for each morph map
	for(INDEX immm=0;immm<ctmmm;immm++) 
	{
		MeshMorphMap &mmm = mlod.mlod_aMorphMaps[immm];
		HTREEITEM hmmm = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "", 16,16, TVIS_SELECTED, TVIF_STATE, 0, hMMaps, 0);
		CTString strWMap = CTString(0,"%s [V-%d]", (const char*)ska_IDToString(mmm.mmp_iID), mmm.mmp_aMorphMap.Count());
		m_TreeCtrl.SetItemText(hmmm,(const char*)strWMap);
		m_TreeCtrl.SetItemData(hmmm,AddNode(NT_MORPH_MAP,&mmm,&mi));
	}
}

// add mesh instance to tree view
void CDlgBarTreeView::AddMeshInstances(CModelInstance &mi,HTREEITEM hParent)
{
	INDEX ctmsh = mi.mi_aMeshInst.Count();
	for(INDEX imsh=0;imsh<ctmsh;imsh++) 
	{
		MeshInstance &mshi = mi.mi_aMeshInst[imsh];
		HTREEITEM hItem = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  1, 1, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
		CTString strMeshName;
		strMeshName.PrintF("%s [%d]",(const char*)mshi.mi_pMesh->GetName().FileName(),mshi.mi_pMesh->msh_aMeshLODs.Count());
		m_TreeCtrl.SetItemText(hItem,(const char*)strMeshName);
		m_TreeCtrl.SetItemData(hItem,AddNode(NT_MESHLODLIST,&mshi,&mi));
		
		// add mesh lods
		INDEX ctmlod = mshi.mi_pMesh->msh_aMeshLODs.Count();
		for(INDEX imlod=0;imlod<ctmlod;imlod++) 
		{
			MeshLOD &mlod = mshi.mi_pMesh->msh_aMeshLODs[imlod];
			HTREEITEM hMlod = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  3, 3, TVIS_SELECTED, TVIF_STATE, 0, hItem, 0);
			
			CTString strMeshLod;
			CTFileName fnMlodSource = mlod.mlod_fnSourceFile;
			fnMlodSource = fnMlodSource.FileName();
			
			INDEX ctTris = 0;
			INDEX ctsrf = mlod.mlod_aSurfaces.Count();
			// for each surface in this mesh lod
			for(INDEX isrf=0;isrf<ctsrf;isrf++) 
			{
				// Count number of tris in this surface
				MeshSurface &msrf = mlod.mlod_aSurfaces[isrf];
				ctTris += msrf.msrf_auwTriangles.Count();
			}
			
			const INDEX ctmwm = mlod.mlod_aWeightMaps.Count();
			const INDEX ctmmm = mlod.mlod_aMorphMaps.Count();
			const FLOAT fDistance = mlod.mlod_fMaxDistance;
			strMeshLod = fnMlodSource.NoExt() + CTString(0," [T-%d,D-%g",ctTris,fDistance);
			if(ctmwm>0) 
			{
				strMeshLod+=CTString(0,",W-%d",ctmwm);
			}
			if(ctmmm>0) 
			{
				strMeshLod+=CTString(0,",M-%d",ctmmm);
			}
			strMeshLod+="]";
			
			m_TreeCtrl.SetItemText(hMlod,strMeshLod);
			m_TreeCtrl.SetItemData(hMlod,AddNode(NT_MESHLOD,&mlod,&mi));
			AddSurfaces(mi,mlod,hMlod);
			AddWeightMaps(mi,mlod,hMlod);
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
			AddSkaTags(mi,mlod,hMlod);
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
			AddMorphMaps(mi,mlod,hMlod);
		}
		// add textures for this mesh
		INDEX cttex = mshi.mi_tiTextures.Count();
		for(INDEX itex=0;itex<cttex;itex++) 
		{
			TextureInstance &ti = mshi.mi_tiTextures[itex];
			CTString strTextName = ska_GetStringFromTable(ti.GetID());
			HTREEITEM hTexture = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  4, 4, TVIS_SELECTED, TVIF_STATE, 0, hItem, 0 );
			m_TreeCtrl.SetItemText(hTexture,strTextName);
			m_TreeCtrl.SetItemData(hTexture,AddNode(NT_TEXINSTANCE,&ti,&mi));
		}
	}
}
// add colision boxes to tree view
void CDlgBarTreeView::AddColisionBoxes(CModelInstance &mi,HTREEITEM hParent)
{
	INDEX ctcb = mi.mi_cbAABox.Count();
	// for each colision box
	for(INDEX icb=0;icb<ctcb;icb++)
	{
		// add colision box
		ColisionBox &cb = mi.mi_cbAABox[icb];
		HTREEITEM hColisionBox = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  2, 2, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
		m_TreeCtrl.SetItemText(hColisionBox,cb.GetName());
		m_TreeCtrl.SetItemData(hColisionBox,AddNode(NT_COLISIONBOX,&cb,&mi));
	}
}

void CDlgBarTreeView::AddAllFramesBBox(CModelInstance &mi,HTREEITEM hParent)
{
#pragma message(">> Remove AddAllFramesBBox")
	// add all frames bounding box
	ColisionBox &cb = mi.mi_cbAllFramesBBox;
	HTREEITEM hAllFramesBBox = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  6, 6, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
	m_TreeCtrl.SetItemText(hAllFramesBBox,"All frames BBox");
	m_TreeCtrl.SetItemData(hAllFramesBBox,AddNode(NT_ALLFRAMESBBOX,&cb,&mi));
}

// add anim set to tree view
HTREEITEM CDlgBarTreeView::AddAnimSet(CModelInstance &mi,HTREEITEM hParent)
{
	INDEX ctas = mi.mi_aAnimSet.Count();
	// for each animset
	for(INDEX ias=0;ias<ctas;ias++)
	{
		CAnimSet &as = mi.mi_aAnimSet[ias];
		INDEX ctan = as.as_Anims.Count();
		CTString strAnimSetName;
		strAnimSetName.PrintF("%s [%d]",(const char*)(as.GetName()).FileName(),ctan);
		HTREEITEM hAnimSet = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  9, 9, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
		m_TreeCtrl.SetItemText(hAnimSet,strAnimSetName);
		m_TreeCtrl.SetItemData(hAnimSet,AddNode(NT_ANIMSET,&as,&mi));
		// for each anim
		for(INDEX ian=0;ian<ctan;ian++)
		{
			Animation &an = as.as_Anims[ian];
			CTString strAnimName;
			//strAnimName.PrintF("%s [%d]",(const char*)ska_GetStringFromTable(an.an_iID),an.an_iFrames);
			strAnimName.PrintF("%s [%d]",(const char*)ska_GetStringFromTable(an.an_iID),an.an_iFrames);
			HTREEITEM hAnim = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  9, 9, TVIS_SELECTED, TVIF_STATE, 0, hAnimSet, 0);
			m_TreeCtrl.SetItemText(hAnim,strAnimName);
			m_TreeCtrl.SetItemData(hAnim,AddNode(NT_ANIMATION,&an,&mi));
			INDEX ctbe=an.an_abeBones.Count();
			for(INDEX ibe=0;ibe<ctbe;ibe++)
			{
				BoneEnvelope &be = an.an_abeBones[ibe];
				CTString strBoneEnvName;
				CTString strBoneName = ska_GetStringFromTable(be.be_iBoneID);
				
				INDEX ctr = be.be_arRot.Count();
				if(an.an_bCompresed) ctr = be.be_arRotOpt.Count();
				
				strBoneEnvName.PrintF("%s [%d]-[%d]",(const char*)strBoneName,ctr,be.be_apPos.Count());
				HTREEITEM hBoneEnv = m_TreeCtrl.InsertItem(TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  9, 9, TVIS_SELECTED, TVIF_STATE, 0, hAnim, 0);
				m_TreeCtrl.SetItemText(hBoneEnv,strBoneEnvName);
				m_TreeCtrl.SetItemData(hBoneEnv,AddNode(NT_ANIM_BONEENV,&be,&mi));
			}
		}
	}
	return 0;
}
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
void CDlgBarTreeView::AddAnimEffects(CModelInstance &mi,HTREEITEM hParent)
{
	CTString strAnimEffects;
	if(mi.m_fnmAnimEffectFile == "") strAnimEffects = mi.GetName();
	else strAnimEffects = mi.m_fnmAnimEffectFile.FileName();
	
	HTREEITEM hAnimEffects = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  32, 32, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
	m_TreeCtrl.SetItemText(hAnimEffects, strAnimEffects);
	m_TreeCtrl.SetItemData(hAnimEffects, AddNode(NT_ANIMEFFECT_GROUP, &mi, &mi));
	
	INDEX ctae = mi.m_vectorAnimEffect.size();
	// for each anim effect
	for(INDEX iae=0; iae<ctae; ++iae)
	{
		CAnimEffect &ae = mi.m_vectorAnimEffect[iae];

		CTString strAnimEffectName;
		strAnimEffectName.PrintF("%s [%d]",(const char*)ska_GetStringFromTable(ae.GetAnimID()), ae.GetReservedEGCount());
		HTREEITEM hAnimEffect = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  9, 9, TVIS_SELECTED, TVIF_STATE, 0, hAnimEffects, 0);
		m_TreeCtrl.SetItemText(hAnimEffect, strAnimEffectName);
		m_TreeCtrl.SetItemData(hAnimEffect, AddNode(NT_ANIMEFFECT, &ae, &mi));
		
		INDEX ctreg = ae.GetReservedEGCount();
		for(INDEX ireg=0; ireg<ctreg; ++ireg)
		{
			CAnimEffect::CReservedEffectGroup *pREG = ae.GetReservedEG(ireg);
			if(pREG == NULL) continue;

			CTString strREG;
			strREG.PrintF("%1.6f/%s", pREG->GetStartTime(), pREG->GetEffectGroupName());
			HTREEITEM hREG = m_TreeCtrl.InsertItem(TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  32, 32, TVIS_SELECTED, TVIF_STATE, 0, hAnimEffect, 0);
			m_TreeCtrl.SetItemText(hREG, strREG);
			m_TreeCtrl.SetItemData(hREG, AddNode(NT_ANIMEFFECT_REG, (void *)ireg, &mi));
		}
	}
	return;
}

/* !
 * Child ���� RideSetting�� ���� Offset�������� ��ȯ�ϸ鼭, Tree�޴��� �߰��Ѵ�.
 * ��(or �Ϲ� �𵨿�ƼƼ)�� ž���� ���� ó���� child���� Offset������ ���
 * ž���� ���ؼ��� Child�𵨵����͸� �Բ� ������ �ʿ䰡 ����.
*/
void CDlgBarTreeView::AddRideSetting(CModelInstance &mi,HTREEITEM hParent)
{
	/// Offset �� Parent BONE ������ ���ϰ�, Child���� �����Ѵ�.
	// add all frames bounding box
	if (mi.mi_iRideParentBoneID >= 0)
	{
		HTREEITEM hRideSet = m_TreeCtrl.InsertItem( TVIF_IMAGE | TVIF_SELECTEDIMAGE , "",  2, 2, TVIS_SELECTED, TVIF_STATE, 0, hParent, 0);
		m_TreeCtrl.SetItemText(hRideSet,"Riding Position");
		m_TreeCtrl.SetItemData(hRideSet,AddNode(NT_RIDINGPOSITION,NULL,&mi));
	}
}

//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
// add all model instances to combo box
void CDlgBarTreeView::FillParentDropDown(CModelInstance *pmi)
{
	if(pmi == NULL) return;
	CComboBox *cbParentList = ((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTMODEL));
	INDEX iItem = cbParentList->AddString((const char*)pmi->GetName());
	cbParentList->SetItemDataPtr(iItem,pmi);
	// add all children to combo box
	INDEX ctmi = pmi->mi_cmiChildren.Count();
	for(INDEX imi=0;imi<ctmi;imi++)
	{
		FillParentDropDown(&pmi->mi_cmiChildren[imi]);
	}
}
// find out how many children from root to selected item
BOOL CDlgBarTreeView::RememberSelectedItem(HTREEITEM hParent,HTREEITEM hSelected)
{
	if(hParent==NULL) return FALSE;
	HTREEITEM hChild = m_TreeCtrl.GetChildItem(hParent);
	INDEX ctLoops = 0;
	while(hChild != NULL)
	{
		ctLoops++;
		if(m_TreeCtrl.ItemHasChildren(hChild))
		{
			if(RememberSelectedItem(hChild,hSelected))
			{
				INDEX &iCur = _aSelectItem.Push();
				iCur = ctLoops;
				return TRUE;
			}
		}
		if(hChild == hSelected)
		{
			INDEX &iCur = _aSelectItem.Push();
			iCur = ctLoops;
			return TRUE;
		}
		hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
	}
	return FALSE;
}
// use previously filed array of child depth to reach selected item
BOOL CDlgBarTreeView::ReselectItem(HTREEITEM hParent)
{
	INDEX ctrec=_aSelectItem.Count();
	// from last to first
	HTREEITEM hChild = hParent;
	// if child is NULL select his parent and return
	// loop filled array of recursion dephth for selected item
	for(INDEX irec=ctrec-1;irec>=0;irec--)
	{
		HTREEITEM hRet = m_TreeCtrl.GetChildItem(hChild);
		if(hRet == NULL)
		{
			m_TreeCtrl.SelectItem(hChild);
			return FALSE;
		}
		else hChild = hRet;
		
		INDEX cti=_aSelectItem[irec];
		for(INDEX i=0;i<cti-1;i++)
		{
			HTREEITEM hRet = m_TreeCtrl.GetNextSiblingItem(hChild);
			if(hRet == NULL)
			{
				m_TreeCtrl.SelectItem(hChild);
				return FALSE;
			}
			else hChild = hRet;
		}
	}
	if(hChild != NULL)
	{
		m_TreeCtrl.SelectItem(hChild);
	}
	return TRUE;
}
// update tree view containing whole hierarchy of model instance
void CDlgBarTreeView::UpdateModelInstInfo(CModelInstance *pmi)
{
	m_TreeCtrl.SetRedraw(FALSE);
	//  ShowControlGroup(-1);
	HTREEITEM htSelectedItem = m_TreeCtrl.GetSelectedItem();
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	
	CTString strRoot;
	// if root item exists
	if(hRoot!=NULL) 
	{
		// remember its name
		strRoot = m_TreeCtrl.GetItemText(hRoot);
	}
	
	// clear array for selected item
	_aSelectItem.PopAll();
	_aiSelectedMeshSurfaces.PopAll();
	// get curent selected item and fill array of depthts how to reach it
	RememberSelectedItem(hRoot,htSelectedItem);
	theApp.iSelectedItemID = -1;
	
	INDEX iSelIndex=0;
	NodeInfo niSelected;
	if(htSelectedItem != NULL)
	{
		iSelIndex = m_TreeCtrl.GetItemData(htSelectedItem);
		niSelected = theApp.aNodeInfo[iSelIndex];
	}
	
	m_TreeCtrl.DeleteAllItems();
	theApp.aNodeInfo.PopAll();
	
	// reset combo box
	((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTMODEL))->ResetContent();
	
	if(pmi == NULL) 
	{
		m_TreeCtrl.SetRedraw(TRUE);
		return;
	}
	
	// fill combo with all parents
	FillParentDropDown(pmi);
	// fill tree ctrl with all hierarchy
	HTREEITEM hParent = AddModelInst(*pmi,NULL,TVI_ROOT);
	
	// get name of root item
	CTString strNewRoot = m_TreeCtrl.GetItemText(hParent);
	// if root item name is different then old root item name clear selection
	if(strRoot != strNewRoot && strRoot.Length() > 0) _aSelectItem.PopAll();
	if(_aSelectItem.Count() > 0)
	{
		// try to select item that was selected before reloading
		ReselectItem(hParent);
	}
	else
	{
		// select hParent
		m_TreeCtrl.SelectItem(hParent);
	}
	m_TreeCtrl.SetRedraw(TRUE);
}
// chech custom seconds per frame check box
void CDlgBarTreeView::CheckSecPerFrameCtrl(BOOL bCheck)
{
	((CButton*)m_dlgAnimSet.GetDlgItem(IDC_CB_SECPERFRAME))->SetCheck(bCheck);
	m_dlgAnimSet.GetDlgItem(IDC_EB_SECPERFRAME)->EnableWindow(bCheck);
}
// changet tab in tab control
void CDlgBarTreeView::OnSelchangeModeSelectTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//  CTabCtrl* pTab = (CTabCtrl*)GetDlgItem(IDC_MODE_SELECT_TAB);
	//  ShowControlGroup(pTab->GetCurSel());
	*pResult = 0;
}
// expand all model instances in tree view
void CDlgBarTreeView::ExpandAllModelInstances(HTREEITEM hItem)
{
	INDEX iIndex = m_TreeCtrl.GetItemData(hItem);
	NodeInfo &ni = theApp.aNodeInfo[iIndex];
	// it this item is model instance
	if(ni.ni_iType == NT_MODELINSTANCE)
	{
		// expand it
		m_TreeCtrl.Expand(hItem,TVE_EXPAND);
	}
	
	// check if current item has children
	if(m_TreeCtrl.ItemHasChildren(hItem))
	{
		HTREEITEM hChild = m_TreeCtrl.GetChildItem(hItem);
		while(TRUE)
		{
			// expand all model isntance in this child
			ExpandAllModelInstances(hChild);
			// get next item
			hChild = m_TreeCtrl.GetNextSiblingItem(hChild);
			if(hChild==NULL)
			{
				break;
			}
		}
	}
}

// put all bones of selected skeleton in combo box
void CDlgBarTreeView::FillBonesToComboBox(CDlgClient &dlgClient, CSkeleton *pskl,INDEX iSelectedIndex, SINT iComboBoxContorlID)
{
	// delete all bones from combo box
	((CComboBox*)dlgClient.GetDlgItem(iComboBoxContorlID))->ResetContent();
	// if skeleton does not exist return
	if(pskl == NULL) return;
	// count skeleton lods
	INDEX ctslod = pskl->skl_aSkeletonLODs.Count();
	if(ctslod<1) return;
	
	SkeletonLOD *pslod = &pskl->skl_aSkeletonLODs[0];
	// if lod doesnt exist return;
	if(pslod == NULL) return;
	// count bones in skeleton lod
	INDEX ctsb = pslod->slod_aBones.Count();
	// for each bone in skeleton lod
	for(INDEX isb=0;isb<ctsb;isb++)
	{
		SkeletonBone &sb = pslod->slod_aBones[isb];
		// add bone to combo box
		((CComboBox*)dlgClient.GetDlgItem(iComboBoxContorlID))->AddString((const char*)ska_GetStringFromTable(sb.sb_iID));
	}
	((CComboBox*)dlgClient.GetDlgItem(iComboBoxContorlID))->SetCurSel(iSelectedIndex);
}

// set text for 'custom' tab in tab control
void CDlgBarTreeView::SetCustomTabText(char *strText)
{
	// fill tab control item
	TCITEM tcitem;
	memset(&tcitem,0,sizeof(tcitem));
	tcitem.mask = TCIF_TEXT;
	tcitem.cchTextMax = 256;
	tcitem.pszText = strText;
	((CTabCtrl*)GetDlgItem(IDC_MODE_SELECT_TAB))->SetItem(2,&tcitem);
}
// reset all controls on dialog
void CDlgBarTreeView::ResetControls()
{
	m_TreeCtrl.DeleteAllItems();
	m_TreeCtrl.hLastSelected = NULL;
	GetDlgItem(IDC_SELECTEDMI)->SetWindowText("(none)");
	((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTBONE))->ResetContent();
	((CComboBox*)m_dlgParent.GetDlgItem(IDC_CB_PARENTMODEL))->ResetContent();
	((CButton*)m_dlgAnimSet.GetDlgItem(IDC_CB_COMPRESION))->SetCheck(FALSE);
	CheckSecPerFrameCtrl(FALSE);
	ShowControlGroup(-1);
	
	
	m_tbOffPosX.SetWindowText("");
	m_tbOffPosY.SetWindowText("");
	m_tbOffPosZ.SetWindowText("");
	m_tbOffRotH.SetWindowText("");
	m_tbOffRotP.SetWindowText("");
	m_tbOffRotB.SetWindowText("");
	
	m_tbTreshold.SetWindowText("");
	m_tbAnimSpeed.SetWindowText("");
	
	m_tbColName.SetWindowText("");
	m_tbColWidth.SetWindowText("");
	m_tbColHeight.SetWindowText("");
	m_tbColLength.SetWindowText("");
	m_tbColPosX.SetWindowText("");
	m_tbColPosY.SetWindowText("");
	m_tbColPosZ.SetWindowText("");
	
	m_tbDistance.SetWindowText("");
	//GET_CTRL(IDC_CB_TEXNAME)->SetWindowText("");
}

void CDlgBarTreeView::OnSize(UINT nType, int cx, int cy) 
{
/*
// if app has initialized
if(theApp.bAppInitialized) 
{
// adjust spliter
AdjustSplitter();
}
	*/
	CDlgTemplate::OnSize(nType, cx, cy);
}

