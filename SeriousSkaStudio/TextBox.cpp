// TextBox.cpp : implementation file
//

#include "stdafx.h"
#include "seriousskastudio.h"
#include "TextBox.h"
#include <Engine/Templates/Stock_CMesh.h>
#include <Engine/Templates/Stock_CSkeleton.h>

#ifdef _DEBUG
#undef new
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CTextBox

CTextBox::CTextBox()
{
}

CTextBox::~CTextBox()
{
}


BEGIN_MESSAGE_MAP(CTextBox, CEdit)
	//{{AFX_MSG_MAP(CTextBox)
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// value changed in some text box
void CTextBox::ValueChanged() 
{
	CSeriousSkaStudioDoc *pDoc = theApp.GetDocument();

	char strText[32];
	// get value from text box
	GetWindowText(strText,31);
	CModelTreeCtrl &m_TreeCtrl = theApp.m_dlgBarTreeView.m_TreeCtrl;
	// get selected item in tree view
	HTREEITEM hSelected = m_TreeCtrl.GetSelectedItem();
	if(hSelected == NULL) return;
	if(pmiSelected == NULL) return;

	INDEX iSelected = m_TreeCtrl.GetItemData(hSelected);
	INDEX iParent;
	// get parent of selected item in tree view
	HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);
	// if no parent set iParent to = 1 
	if(hParent == NULL) iParent = -1;
	else iParent = m_TreeCtrl.GetItemData(hParent);

	NodeInfo &niSelected = theApp.aNodeInfo[iSelected];

	FLOATmatrix3D mat;
	FLOAT3D &vPos = pmiSelected->mi_qvOffset.vPos;
	ANGLE3D aRot;
	pmiSelected->mi_qvOffset.qRot.ToMatrix(mat);
	DecomposeRotationMatrix(aRot,mat);
	// colison
	ColisionBox *pcb = NULL;
	if(pmiSelected->mi_iCurentBBox >= 0 && pmiSelected->mi_iCurentBBox < pmiSelected->mi_cbAABox.Count()) {
		pcb = &pmiSelected->mi_cbAABox[pmiSelected->mi_iCurentBBox];
	}
	if(niSelected.ni_iType == NT_ALLFRAMESBBOX) {
		pcb = &pmiSelected->mi_cbAllFramesBBox;
	}


	FLOAT fOffset[6] = {vPos(1),vPos(2),vPos(3),aRot(1),aRot(2),aRot(3)};

	INDEX ictrlID = GetDlgCtrlID();
	switch(ictrlID)
	{
		case IDC_TB_OFFSET_POSX:
			fOffset[0] = atof(strText);
			// set offset of selected object
			pmiSelected->SetOffset(fOffset);
		break;
		case IDC_TB_OFFSET_POSY:
			fOffset[1] = atof(strText);
			// set offset of selected object
			pmiSelected->SetOffset(fOffset);
		break;
		case IDC_TB_OFFSET_POSZ:
			fOffset[2] = atof(strText);
			// set offset of selected object
			pmiSelected->SetOffset(fOffset);
		break;
		case IDC_TB_OFFSET_ROTH:
			fOffset[3] = atof(strText);
			// set offset of selected object
			pmiSelected->SetOffset(fOffset);
			// refresh all text boxes for colision 
			theApp.m_dlgBarTreeView.SelItemChanged(hSelected);
		break;
		case IDC_TB_OFFSET_ROTP:
			fOffset[4] = atof(strText);
			// set offset of selected object
			pmiSelected->SetOffset(fOffset);
			// refresh all text boxes for colision 
			theApp.m_dlgBarTreeView.SelItemChanged(hSelected);
		break;
		case IDC_TB_OFFSET_ROTB:
			fOffset[5] = atof(strText);
			// set offset of selected object
			pmiSelected->SetOffset(fOffset);
			// refresh all text boxes for colision 
			theApp.m_dlgBarTreeView.SelItemChanged(hSelected);
		break;
		case IDC_EB_DISTANCE:
		{
			// set lod distance
			if(niSelected.ni_iType == NT_SKELETONLOD)
			{
				// if no parent return
				if(iParent < 0) return;
				NodeInfo &niParent   = theApp.aNodeInfo[iParent];
				// get references to objects
				CSkeleton *psk = (CSkeleton*)niParent.ni_pPtr;
				SkeletonLOD *slod = (SkeletonLOD*)niSelected.ni_pPtr;
				// change slods max distance
				slod->slod_fMaxDistance = atof(strText);
				theApp.UpdateRootModelInstance();
			}
			else if(niSelected.ni_iType == NT_MESHLOD)
			{
				// if no parent return
				if(iParent < 0) return;
				NodeInfo &niParent = theApp.aNodeInfo[iParent];
				// get references to objects
				MeshInstance *pmshi = (MeshInstance*)niParent.ni_pPtr;
				MeshLOD *pmlod = (MeshLOD*)niSelected.ni_pPtr;
				CMesh *pmesh = pmshi->mi_pMesh;
				// change mlods max distance
				pmlod->mlod_fMaxDistance = atof(strText);
				theApp.UpdateRootModelInstance();
			}
		}
		break;
		case IDC_EB_TRESHOLD:
		{
			if(niSelected.ni_iType == NT_ANIMATION)
			{
				// if no parent return
				if(iParent < 0) return;
				NodeInfo &niParent = theApp.aNodeInfo[iParent];
				// get references to objects
				CAnimSet *pas = (CAnimSet*)niParent.ni_pPtr;
				Animation *pan = (Animation*)niSelected.ni_pPtr;
				// remember old treshold
				FLOAT fOldTreshold = pan->an_fThreshold;
				pan->an_fThreshold = atof(strText);
				// convert only one animation in animset
				if(!theApp.ConvertAnimationInAnimSet(pas,pan)) {
					pan->an_fThreshold = fOldTreshold;
				}
				// update model instance
				theApp.UpdateRootModelInstance();
			}
		}
		break;
		case IDC_TB_COLWIDTH:
		{
			// change width of colision box
			if(pcb!=NULL)
			{
				FLOAT fWidth = atof(strText);
//������ ���� ����	//(Easy Use Ska Studio)(0.1)
				fWidth = fabs(fWidth);
//������ ���� ��	//(Easy Use Ska Studio)(0.1)
				FLOAT fPosX = (pcb->Max()(1)+pcb->Min()(1)) / 2;
				pcb->Min()(1) = -(fWidth/2) + fPosX;
				pcb->Max()(1) = (fWidth/2) + fPosX;
			}
		}
		break;
		case IDC_TB_COLHEIGHT:
		{
			// change height of colision box
			if(pcb!=NULL)
			{
				FLOAT fHeight = atof(strText);
//������ ���� ����	//(Easy Use Ska Studio)(0.1)
				fHeight = fabs(fHeight);
//������ ���� ��	//(Easy Use Ska Studio)(0.1)
				FLOAT fPosY = pcb->Min()(2);
				pcb->Min()(2) = fPosY;
				pcb->Max()(2) = fHeight + fPosY;
			}
		}
		break;
		case IDC_TB_COLLENGTH:
		{                  
			// change length of colision box
			if(pcb!=NULL)
			{
				FLOAT fLength = atof(strText);
//������ ���� ����	//(Easy Use Ska Studio)(0.1)
				fLength = fabs(fLength);
//������ ���� ��	//(Easy Use Ska Studio)(0.1)
				FLOAT fPosZ = (pcb->Max()(3)+pcb->Min()(3)) / 2;
				pcb->Min()(3) = -(fLength/2) + fPosZ;
				pcb->Max()(3) = (fLength/2) + fPosZ;
			}
		}
		break;
		case IDC_TB_COLPOSX:
		{
			// change pos x of colision box
			if(pcb!=NULL)
			{
				FLOAT fPosX = atof(strText);
				FLOAT fWidth = (pcb->Max()(1)-pcb->Min()(1));
				pcb->Min()(1) = -(fWidth/2) + fPosX;
				pcb->Max()(1) = (fWidth/2) + fPosX;
			}
		}
		break;
		case IDC_TB_COLPOSY:
		{
			// change pos y of colision box
			if(pcb!=NULL)
			{
				FLOAT fPosY = atof(strText);
				FLOAT fHeight = (pcb->Max()(2)-pcb->Min()(2));
				fPosY += fHeight/2;
				pcb->Min()(2) = -(fHeight/2) + fPosY;
				pcb->Max()(2) = (fHeight/2) + fPosY;
			}
		}
		break;
		case IDC_TB_COLPOSZ:
		{
			// change pos z of colision box
			if(pcb!=NULL)
			{
				FLOAT fPosZ = atof(strText);
				FLOAT fLength = (pcb->Max()(3)-pcb->Min()(3));
				pcb->Min()(3) = -(fLength/2) + fPosZ;
				pcb->Max()(3) = (fLength/2) + fPosZ;
			}
		}
		break;
		case IDC_TB_COLNAME:
		{
			// change name of colision box
			if(pcb!=NULL) {
				pcb->SetName(strText);
				m_TreeCtrl.SetItemText(hSelected,(const char*)strText);
			}
		}
		break;
		// change animation speed
		case IDC_EB_SECPERFRAME:
		{
			// if no parent return
			if(iParent < 0) return;
			NodeInfo &niParent = theApp.aNodeInfo[iParent];
			Animation *pan = (Animation*)niSelected.ni_pPtr;
			CAnimSet *pas = (CAnimSet*)niParent.ni_pPtr;
			pan->an_fSecPerFrame = atof(strText);
			if(pan->an_fSecPerFrame == 0) pan->an_fSecPerFrame = 0.00001f;
			// update model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		// change animation walk speed 
		case IDC_TB_WALKSPEED:
		{
			pDoc->m_fSpeedZ = atof(strText);
			return; // do not mark doc as channged
		}
		break;
		// change animation walk loop
		case IDC_TB_WALKLOOP:
		{
			pDoc->m_fLoopSecends = atof(strText);
			return; // do not mark doc as channged
		}
		break;
		case IDC_EB_TEXTURENAME:
		{
			TextureInstance *pti = (TextureInstance*)niSelected.ni_pPtr;
			pti->SetName(CTString(strText));
			// update model instance
			theApp.UpdateRootModelInstance();
		}
		break;
		case IDC_TB_MI_NAME:
		{
			NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
			CModelInstance &mi = *(CModelInstance*)ni.ni_pPtr;
			mi.SetName(strText);
			// update model instance
			theApp.UpdateRootModelInstance();
		}
		break;
//������ ���� ����	//(Add & Modify SSSE Effect)(0.1)
		case IDC_SKATAG_NAME:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				MeshLOD &mlod = *skaTag.GetMeshLOD();
				CModelInstance &mi = *ni.pmi;
				skaTag.SetName(strText);
				mi.RefreshTagManager();
				theApp.UpdateRootModelInstance();
			} break;
		case IDC_SKATAG_OFFSETPOSX:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOAT3D pos = skaTag.GetOffsetPos();
				pos(1) = atof(strText);
				skaTag.SetOffsetPos(pos);
			} break;
		case IDC_SKATAG_OFFSETPOSY:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOAT3D pos = skaTag.GetOffsetPos();
				pos(2) = atof(strText);
				skaTag.SetOffsetPos(pos);
			} break;
		case IDC_SKATAG_OFFSETPOSZ:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOAT3D pos = skaTag.GetOffsetPos();
				pos(3) = atof(strText);
				skaTag.SetOffsetPos(pos);
			} break;
		case IDC_SKATAG_OFFSETROTAXISX:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOATquat3D quat = skaTag.GetOffsetRot();
				FLOAT3D axis; FLOAT angle;
				quat.ToAxisAngle(axis, angle);
				axis(1) = atof(strText);
				axis.Normalize();
				quat.FromAxisAngle(axis, angle);
				skaTag.SetOffsetRot(quat);
			} break;
		case IDC_SKATAG_OFFSETROTAXISY:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOATquat3D quat = skaTag.GetOffsetRot();
				FLOAT3D axis; FLOAT angle;
				quat.ToAxisAngle(axis, angle);
				axis(2) = atof(strText);
				axis.Normalize();
				quat.FromAxisAngle(axis, angle);
				skaTag.SetOffsetRot(quat);
			} break;
		case IDC_SKATAG_OFFSETROTAXISZ:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOATquat3D quat = skaTag.GetOffsetRot();
				FLOAT3D axis; FLOAT angle;
				quat.ToAxisAngle(axis, angle);
				axis(3) = atof(strText);
				axis.Normalize();
				quat.FromAxisAngle(axis, angle);
				skaTag.SetOffsetRot(quat);
			} break;
		case IDC_SKATAG_OFFSETROTANGLE:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				CSkaTag &skaTag = *(CSkaTag*)ni.ni_pPtr;
				FLOATquat3D quat = skaTag.GetOffsetRot();
				FLOAT3D axis; FLOAT angle;
				quat.ToAxisAngle(axis, angle);
				angle = RadAngle( atof(strText) );
				quat.FromAxisAngle(axis, angle);
				skaTag.SetOffsetRot(quat);
			} break;
//������ ���� ��	//(Add & Modify SSSE Effect)(0.1)
//������ ���� ����	//(Effect Add & Modify for Close Beta)(0.1)
		case IDC_ANIMEFFECT_STARTTIME:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				HTREEITEM hParent = theApp.m_dlgBarTreeView.m_TreeCtrl.GetParentItem( theApp.m_dlgBarTreeView.m_TreeCtrl.GetSelectedItem() );
				if(hParent == NULL) break;
				NodeInfo &niParent = theApp.m_dlgBarTreeView.GetNodeInfo(hParent);
				CAnimEffect &ae = *(CAnimEffect*)niParent.ni_pPtr;
				ae.GetReservedEG((INDEX)ni.ni_pPtr)->SetStartTime( atof(strText) );
				theApp.UpdateRootModelInstance();
			} break;
		case IDC_ANIMEFFECT_EFFECTGROUPNAME:
			{
				NodeInfo &ni = theApp.m_dlgBarTreeView.GetSelectedNodeInfo();
				HTREEITEM hParent = theApp.m_dlgBarTreeView.m_TreeCtrl.GetParentItem( theApp.m_dlgBarTreeView.m_TreeCtrl.GetSelectedItem() );
				if(hParent == NULL) break;
				NodeInfo &niParent = theApp.m_dlgBarTreeView.GetNodeInfo(hParent);
				CAnimEffect &ae = *(CAnimEffect*)niParent.ni_pPtr;
				ae.GetReservedEG((INDEX)ni.ni_pPtr)->SetEffectGroupName( CTString(strText) );
				theApp.UpdateRootModelInstance();
			} break;
//������ ���� ��	//(Effect Add & Modify for Close Beta)(0.1)
		default:
		{
			// check if this text box is in shader dialog
			if((ictrlID>FIRSTSHADEID) && (ictrlID<LASTSHADEID))
			{
				// control is in shader dialog
				theApp.m_dlgBarTreeView.ChangeFloatOnSelectedSurfaces(m_strID,atof(strText));
			}
		}
		break;
	}
	pDoc->MarkAsChanged();
}

BOOL CTextBox::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->message==WM_KEYDOWN) && ((int)pMsg->wParam==VK_RETURN))
	{
		ValueChanged();
		//SetSel(0,-1);
		return TRUE;
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CTextBox::SetDataPtr(FLOAT *pFloat)
{
	CTString strText = CTString(0,"%g",*pFloat);
	SetWindowText((const char*)strText);
}

void CTextBox::OnChange() 
{
	#pragma message(">> Fix textbox editing")
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
