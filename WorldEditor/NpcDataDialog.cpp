// NpcDataDialog.cpp : implementation file
//
// FIXME : ���� ���õ� �κ� ��� ����ĥ��.
// FIXME : DB�� �����ϴ� �κ��� �ʹ� �ߺ��Ǵ� �κ��� ����,
// FIXME : ����, Respawner, Mob, Npc��� �ϰ����� ������.

#include "stdafx.h"
#include "WorldEditor.h"
#include "resource.h"
#include "NpcDataDialog.h"
#include "WEDefinition.h"
#include "dbcmd.h"
#include <Engine/Entities/MobData.h>

#include <Engine/Entities/SmcParser.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNpcDataDialog dialog

CNpcDataDialog::CNpcDataDialog(CWnd* pParent /*=NULL*/)
: CDialog(CNpcDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNpcDataDialog)		
	m_bShowEnemyModel = FALSE;
	//}}AFX_DATA_INIT
}

void CNpcDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNpcDataDialog)
	DDX_Control(pDX, IDC_FIND_BY_YLAYER, m_ctlcbFindByYLayer);
	DDX_Control(pDX, IDC_FIND_BY_NAME, m_ctlcbFindByName);
	DDX_Control(pDX, IDC_FIND_BY_TYPE,  m_ctlcbFindByType);
	DDX_Control(pDX, IDC_NPC_GRID, m_ctlNpcGrid);
	DDX_Check(pDX, IDC_SHOW_ENEMY_MODEL, m_bShowEnemyModel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcDataDialog, CDialog)
//{{AFX_MSG_MAP(CNpcDataDialog)	
ON_BN_CLICKED(ID_NPC_DATA_DELETE, OnNpcDataDelete)		
ON_BN_CLICKED(IDC_ENEMY_SCAN, OnEnemyUpdate)
ON_BN_CLICKED(ID_NPC_GENERATE, OnNpcGenerate)
ON_CBN_SELCHANGE(IDC_FIND_BY_YLAYER, OnSelchangeFindByYlayer)
ON_CBN_SELCHANGE(IDC_FIND_BY_NAME, OnSelchangeFindByName)
ON_CBN_SELCHANGE(IDC_FIND_BY_TYPE, OnSelchangeFindByType)

ON_COMMAND(IDM_IMPORT_DB_DATA, OnImportDbData)
ON_COMMAND(IDM_EXPORT_DB_DATA, OnExportDbData)
ON_WM_NCDESTROY()
ON_BN_CLICKED(IDC_ENEMY_EDIT, OnEnemyEdit)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNpcDataDialog message handlers

BOOL CNpcDataDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CListCtrl* pWnd = (CListCtrl*)GetDlgItem(IDC_NPC_GRID);	
	pWnd->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	//-------------------------------------------------------------------
	// Grid ���� ����.
	//-------------------------------------------------------------------
	CString aStrTitle[] = 
	{ _T("IDX"), _T("NPC Type"), _T("X"), _T("Layer"), _T("Y"), _T("Z"), _T("R"), _T("Regen Sec"), _T("Total Num") };

	int aiColWidth[] =
	{ 50, 120, 80, 50, 50, 50, 80, 50, 50};

	//--------------------------------------------------------
	// ��ü ����Ʈ ��� ���â.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
	
	// �÷� ����� �߾ӿ� ���.
	lvcolumn.fmt = LVCFMT_CENTER;
	
	int i;
	for (i = 0; i < 9; ++i)
	{
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlNpcGrid.InsertColumn(i, &lvcolumn);
	}

	// NOTE : ������ �߻��ؼ� �ּ� ó����.
	//-------------------------------------------------------------------
	// ComboBox �ؽ�Ʈ ����.		- Mob 
	//-------------------------------------------------------------------		
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	m_ctlcbFindByName.InsertString(0, _T("��ü"));
	int j = 1;

	int nMax = CMobData::getsize();

	for (i = 0; i < nMax; ++i)
	{
		CMobData* MD = CMobData::getDataSeq(i);

		if(MD->GetMobIndex() <= 0)
		{
			TRACE(_T("Cannot find Mob Data!!!\n"));
			continue;
		}		
		
		m_ctlcbFindByName.InsertString(j, MD->GetName());
		m_ctlcbFindByName.SetItemData(j, MD->GetMobIndex());
		j++;
	}
	m_ctlcbFindByName.SetCurSel(0);

	//-------------------------------------------------------------------
	// ComboBox �ؽ�Ʈ ����.		- YLayer
	//-------------------------------------------------------------------	
	for(i = 0; i < TYPE_TOTAL; ++i)
	{
		m_ctlcbFindByType.InsertString(i, _aStrMobType[i]);
	}
	m_ctlcbFindByType.SetCurSel(0);

	//-------------------------------------------------------------------
	// ComboBox �ؽ�Ʈ ����.		- YLayer
	//-------------------------------------------------------------------	
	CString aStrYLayer[] = 
	{ _T("��ü"), _T("1��"), _T("2��"), _T("3��"), _T("4��"), _T("5��")};

	for(i = 0; i < 6; i++)
	{
		m_ctlcbFindByYLayer.InsertString(i, aStrYLayer[i]);
	}
	m_ctlcbFindByYLayer.SetCurSel(0);
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//-----------------------------------------------------------------------------
// Purpose: World ��ȣ�� �ش��ϴ� �����͸� ������.
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnNpcDataDelete() 
{
	// TODO: Add your control notification handler code here	
	if (AfxMessageBox(_T("Delete?"), MB_OKCANCEL) == IDCANCEL)
		return;	

	DeleteShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	DeleteNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	return;
}

//-----------------------------------------------------------------------------
// Purpose: DeleteShopData
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iYLayer - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::DeleteShopData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");
	
	//---------------------------------------------------------------------------------------
	strQuery.Format(
		_T("DELETE FROM t_shop"
		" WHERE a_zone_num=%d"),
		iWorldNum);
	
	if(iNpcIndex != 0)
	{
		CMobData* MD = CMobData::getData(iNpcIndex);
		if(MD->IsShopper())
		{
			CString strNpcIndex = _T("");
			strNpcIndex.Format(_T(" AND a_keeper_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
			strQuery += strNpcIndex;
		}
	}
	
	if(iYLayer != -1)
	{
		CString strYLayer = _T("");
		strYLayer.Format(_T(" AND a_y_layer = %d"), iYLayer);
		strQuery += strYLayer;
	}
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("���� �����͸� �����Ҽ� �����ϴ�."));
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: DeleteNpcData
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iYLayer - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::DeleteNpcData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");
	strQuery.Format(
		_T("DELETE t_npc_regen FROM t_npc, t_npc_regen"
		" WHERE t_npc.a_index=t_npc_regen.a_npc_idx AND t_npc_regen.a_zone_num=%d"),
		iWorldNum);	

	if(iNpcType != TYPE_WHOLE)
	{			
		CString strNpcType	= _T("");
		// FiXME : ���� ���� �ʴ� �κ�.
		switch(iNpcType)
		{
		case TYPE_ENEMY:		// ����			
			strNpcType.Format(_T(" AND ((t_npc.a_flag & (%ld | %ld | %ld | %ld | %ld)))"), 
						NPC_FIRSTATTACK, NPC_ATTACK, NPC_MOVING, NPC_MBOSS, NPC_BOSS );
			break;
		case TYPE_ZONEMOVING:	// ���̵�
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_ZONEMOVER);
			break;
		case TYPE_GUILD:		// ���
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_GUILD);
			break;
		case TYPE_REFINER:		// ���ݼ���
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_REFINER);
			break;
		case TYPE_QUEST:		// ����Ʈ
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_QUEST);
			break;
		case TYPE_PRODUCE:		// ����ǰ
			strNpcType.Format(_T(" AND ((t_npc.a_flag & (%ld | %ld | %ld | %ld)))"), NPC_MINERAL, NPC_CROPS, NPC_ENERGY, NPC_ETERNAL );
			break;
		case TYPE_GUARD:		// ���.
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_GUARD);
			break;
		case TYPE_EVENT:		// �̺�Ʈ
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_EVENT);
			break;
		case TYPE_WAREHOUSE:	// â��
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_KEEPER);
			break;
		case TYPE_SHOPPER:		// ����
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_SHOPPER);
			break;
		case TYPE_RESETSTAT:	// ���� �ʱ�ȭ
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_RESETSTAT);
			break;				
		case TYPE_REMISSION:	// ���˺�
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_REMISSION);
			break;
		case TYPE_WARCASTLE:	// ����
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_WARCASTLE);
			break;
		case TYPE_CHANGEWEAPON:	// ���� ��ȯ
			strNpcType.Format(_T(" AND t_npc.a_flag & %ld"), NPC_CHANGEWEAPON);
			break;

		default:
			break;
			
		}			
		strQuery += strNpcType;
	}

	if(iNpcIndex != 0)
	{		
		CString strNpcIndex = _T("");
		strNpcIndex.Format(_T(" AND t_npc_regen.a_npc_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
		strQuery += strNpcIndex;
	}

	if(iYLayer != -1)
	{
		CString strYLayer = _T("");
		strYLayer.Format(_T(" AND t_npc_regen.a_y_layer = %d"), iYLayer);
		strQuery += strYLayer;
	}

	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("���� ������ �����Ҽ� �����ϴ�."));
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: �о���� NPC ������ Grid�� �����.
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iYLayer - 
//-----------------------------------------------------------------------------
void CNpcDataDialog::RefreshNpcData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	// TODO: Add your control notification handler code here
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");

	try
	{
		//m_ctlNpcGrid.SetRows(1);
		// ���������� �߰�.
		m_ctlNpcGrid.DeleteAllItems();
		
		CWorldEditorDoc *pDoc			= theApp.GetDocument();
		CEntity			*penEntity		= NULL;
		CPlacement3D	plPlacement;
		
		m_paNPCWithEntity.PopAll();
		
		CString strQuery = _T("");
		if(iNpcType == 0)		// ��ü
		{
			strQuery.Format(_T("SELECT a_index npcindex, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_z, a_pos_r, a_regen_sec, a_total_num FROM t_npc_regen WHERE a_zone_num = %d"), iWorldNum);
			
			if(iNpcIndex != 0)
			{
				CString strNpcIndex = _T("");
				strNpcIndex.Format(_T(" AND a_npc_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
				strQuery += strNpcIndex;
			}
			
			if(iYLayer != -1)
			{
				int iFindLayer = iYLayer * 2;
				CString strYLayer = _T("");
				strYLayer.Format(_T(" AND a_y_layer = %d"), iFindLayer);
				strQuery += strYLayer;
			}
			
			strQuery += CString(" ORDER BY a_npc_idx, npcindex");
		}
		else
		{	
			strQuery.Format(_T("SELECT reg.a_index npcindex, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_z, a_pos_r, a_regen_sec, a_total_num, reg.* FROM t_npc npc, t_npc_regen reg WHERE npc.a_index=reg.a_npc_idx AND reg.a_zone_num = %d"), iWorldNum);
			if(iNpcType != TYPE_WHOLE)
			{			
				CString strNpcType	= _T("");
				// FiXME : ���� ���� �ʴ� �κ�.
				switch(iNpcType)
				{
				case TYPE_ENEMY:		// ����
										/*
										strNpcType.Format(_T(" AND !((npc.a_flag & (%ld | %ld | %ld | %ld | %ld | %ld | %ld | %ld | %ld | %ld)))"), 
										NPC_ZONEMOVER, NPC_REFINER, NPC_QUEST, NPC_REMISSION, NPC_GUARD, 
										NPC_KEEPER, NPC_MINERAL, NPC_CROPS, NPC_ENERGY, NPC_ETERNAL, 
										NPC_SHOPPER, NPC_RESETSTAT, NPC_REMISSION, NPC_WARCASTLE, NPC_CHANGEWEAPON );
					*/
					strNpcType.Format(_T(" AND ((npc.a_flag & (%ld | %ld | %ld | %ld | %ld)))"), 
						NPC_FIRSTATTACK, NPC_ATTACK, NPC_MOVING, NPC_MBOSS, NPC_BOSS );
					break;
				case TYPE_ZONEMOVING:	// ���̵�
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_ZONEMOVER);
					break;
				case TYPE_GUILD:		// ���
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_GUILD);
					break;
				case TYPE_REFINER:		// ���ݼ���
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_REFINER);
					break;
				case TYPE_QUEST:		// ����Ʈ
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_QUEST);
					break;
				case TYPE_PRODUCE:		// ����ǰ
					strNpcType.Format(_T(" AND ((npc.a_flag & (%ld | %ld | %ld | %ld)))"), NPC_MINERAL, NPC_CROPS, NPC_ENERGY, NPC_ETERNAL );
					break;
				case TYPE_GUARD:		// ���.
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_GUARD);
					break;
				case TYPE_EVENT:		// �̺�Ʈ
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_EVENT);
					break;
				case TYPE_WAREHOUSE:	// â��
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_KEEPER);
					break;
				case TYPE_SHOPPER:		// ����
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_SHOPPER);
					break;
					/*
					case TYPE_SKILLMASTER:	// ��ų ������
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_SKILLMASTER);
					break;
					case TYPE_SSKILLMASTER:	// Ư�� ��ų������
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_SSKILLMASTER);
					break;
					*/
				case TYPE_RESETSTAT:	// ���� �ʱ�ȭ
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_RESETSTAT);
					break;				
				case TYPE_REMISSION:	// ���˺�
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_REMISSION);
					break;
				case TYPE_WARCASTLE:	// ����
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_WARCASTLE);
					break;
				case TYPE_CHANGEWEAPON:	// ���� ��ȯ
					strNpcType.Format(_T(" AND npc.a_flag & %ld"), NPC_CHANGEWEAPON);
					break;
					
				default:
					break;
					
				}			
				strQuery += strNpcType;
			}
			
			if(iNpcIndex != 0)
			{
				CString strNpcIndex = _T("");
				strNpcIndex.Format(_T(" AND a_npc_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
				strQuery += strNpcIndex;
			}
			
			if(iYLayer != -1)
			{
				int iFindLayer = iYLayer * 2;
				CString strYLayer = _T("");
				strYLayer.Format(_T(" AND reg.a_y_layer = %d"), iFindLayer);
				strQuery += strYLayer;
			}
			
			strQuery += CString(" ORDER BY a_npc_idx, npcindex");
		}
		
		CDBCmd cmd;
		cmd.Init(theApp.m_pDBConn);
		cmd.SetQuery(strQuery);
		if (!cmd.Open())
		{
			AfxMessageBox(_T("Cannot open table"));
			return;
		}
		if (!cmd.MoveFirst())		return;	
		
		//---------------------------------------------------------------------------
		// FIXME : CEntity�� en_ulID�� DB�� �ö� �����͸� 1:1 ��Ī ���Ѽ�,
		// FIXME : DB�� �����ͷ� ��ƼƼ�� �����Ҽ� �ֵ��� �Ұ�.
		// FIXME : ���⿡ NPCWithEntity ����ü�� �ۼ��� �Ǿ����.
		//---------------------------------------------------------------------------
		//m_ctlNpcGrid.SetRows(cmd.m_nrecords + 1);
		LV_ITEM lvitem;
		do
		{
			int		iIndex				= -1;				// DB ���� �ε���.
			int		iNpcIndex			= -1;				// NPC�� �ε���.
			float	fPosX				= 0.0f;				// NPC�� ��ġ��.
			int		iPosY				= -1;
			float	fPosZ				= 0.0f;
			float	fPosY				= 0.0f;
			float	fRot				= 0.0f;
			int		iRegenTime			= -1;				// NPC�� ���� Ÿ��
			int		iTotalNum			= -1;				// NPC�� �ִ� ���� ����.
			cmd.GetRec("npcindex",		iIndex);
			cmd.GetRec("a_npc_idx",		iNpcIndex);
			cmd.GetRec("a_pos_x",		fPosX);
			cmd.GetRec("a_y_layer",		iPosY);
			cmd.GetRec("a_pos_h",		fPosY);
			cmd.GetRec("a_pos_z",		fPosZ);		
			cmd.GetRec("a_pos_r",		fRot);
			cmd.GetRec("a_regen_sec",	iRegenTime);
			cmd.GetRec("a_total_num",	iTotalNum);
			
			if(iNpcIndex > 0)
			{			
				//-------------------------------------------------------------------
				// Grid�� �� ����.
				//-------------------------------------------------------------------
				CString strGridText = _T("");		
				
				strGridText.Format(_T("%d"), iIndex);		
				lvitem.iItem	= m_ctlNpcGrid.GetItemCount();
				lvitem.mask		= LVIF_TEXT;
				lvitem.iSubItem = 0;
				lvitem.pszText	= (LPSTR)(LPCTSTR)strGridText;
				m_ctlNpcGrid.InsertItem(&lvitem);
				
				CMobData* MD = CMobData::getData(iNpcIndex);
				strGridText.Format(_T("[%d]%s"), iNpcIndex, MD->GetName());
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 1, strGridText);			
				
				NPCWithEntity &npcEntity = m_paNPCWithEntity.Push();
				npcEntity.iNpcIndex = iNpcIndex;
				npcEntity.pMobData	= MD;
				npcEntity.fPosX		= fPosX;
				npcEntity.fPosZ		= fPosZ;
				npcEntity.fPosY		= fPosY;
				npcEntity.fAngle	= fRot;
				npcEntity.iPosY		= iPosY;	
				npcEntity.iRegenTime= iRegenTime;
				npcEntity.iTotalNum	= iTotalNum;
				
				strGridText.Format(_T("%.3f"), fPosX);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 2, strGridText);
				
				int iFloor = -1;
				if(iPosY % 2 == 0)
				{
					iFloor = iPosY / 2 + 1;
				}
				
				strGridText.Format(_T("%d��"), iFloor);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 3, strGridText);		
				
				strGridText.Format(_T("%.3f"), fPosY);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 4, strGridText);
				
				strGridText.Format(_T("%.3f"), fPosZ);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 5, strGridText);
				
				strGridText.Format(_T("%.3f"), fRot);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 6, strGridText);
				
				strGridText.Format(_T("%d"), iRegenTime);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 7, strGridText);
				
				strGridText.Format(_T("%d"), iTotalNum);
				m_ctlNpcGrid.SetItemText(lvitem.iItem, 8, strGridText);
			}		
		}
		while(cmd.MoveNext());	
	}
	catch(char *strError)
	{
		AfxMessageBox(strError);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iWorldNum - 
//			iNpcIndex - 
//			iNpcType - 
//			iYLayer - 
//-----------------------------------------------------------------------------
void CNpcDataDialog::RefreshShopData(int iWorldNum, int iNpcIndex, int iNpcType, int iYLayer)
{
	// TODO: Add your control notification handler code here
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	try
	{
		
		CWorldEditorDoc *pDoc			= theApp.GetDocument();
		CEntity			*penEntity		= NULL;
		CPlacement3D	plPlacement;
		
		CString strQuery = _T("");
		if(iNpcType == TYPE_WHOLE || iNpcType == TYPE_SHOPPER)		// ��ü
		{
			strQuery.Format(_T("SELECT * FROM t_shop WHERE a_zone_num = %d"), iWorldNum);
			
			if(iNpcIndex != 0)
			{
				CString strNpcIndex = _T("");
				strNpcIndex.Format(_T(" AND a_keeper_idx = %d"), m_ctlcbFindByName.GetItemData(iNpcIndex));
				strQuery += strNpcIndex;
			}
			
			if(iYLayer != -1)
			{
				CString strYLayer = _T("");
				strYLayer.Format(_T(" AND a_y_layer = %d"), iYLayer);
				strQuery += strYLayer;
			}	
			CDBCmd cmd;
			cmd.Init(theApp.m_pDBConn);
			cmd.SetQuery(strQuery);
			if (!cmd.Open())
			{
				AfxMessageBox(_T("Cannot open shop table"));
				return;
			}
			if (!cmd.MoveFirst())		return;	
			
			//---------------------------------------------------------------------------
			// FIXME : CEntity�� en_ulID�� DB�� �ö� �����͸� 1:1 ��Ī ���Ѽ�,
			// FIXME : DB�� �����ͷ� ��ƼƼ�� �����Ҽ� �ֵ��� �Ұ�.
			// FIXME : ���⿡ NPCWithEntity ����ü�� �ۼ��� �Ǿ����.
			//---------------------------------------------------------------------------
			//m_ctlNpcGrid.SetRows(cmd.m_nrecords + 1);
			LV_ITEM lvitem;
			do
			{
				int		iNpcIndex			= -1;				// NPC�� �ε���.
				float	fPosX				= 0.0f;				// NPC�� ��ġ��.
				int		iPosY				= -1;
				float	fPosZ				= 0.0f;
				float	fPosY				= 0.0f;
				float	fAngle				= 0.0f;
				cmd.GetRec("a_keeper_idx",	iNpcIndex);
				cmd.GetRec("a_pos_x",		fPosX);
				cmd.GetRec("a_y_layer",		iPosY);
				cmd.GetRec("a_pos_h",		fPosY);
				cmd.GetRec("a_pos_z",		fPosZ);
				cmd.GetRec("a_pos_r",		fAngle);
				
				if(iNpcIndex > 0)
				{				
					CString strGridText = _T("");		
					
					strGridText.Format(_T("����"));
					lvitem.iItem	= m_ctlNpcGrid.GetItemCount();
					lvitem.mask		= LVIF_TEXT;
					lvitem.iSubItem = 0;
					lvitem.pszText	= (LPSTR)(LPCTSTR)strGridText;
					m_ctlNpcGrid.InsertItem(&lvitem);
					
					NPCWithEntity &npcEntity = m_paNPCWithEntity.Push();
					CMobData* MD		= CMobData::getData(iNpcIndex);
					npcEntity.iNpcIndex = iNpcIndex;
					npcEntity.pMobData	= MD;
					npcEntity.fPosX		= fPosX;
					npcEntity.fPosZ		= fPosZ;
					npcEntity.fPosY		= fPosY;
					npcEntity.iPosY		= iPosY;
					npcEntity.fAngle	= fAngle;
					
					strGridText.Format(_T("[%d]%s"), iNpcIndex, MD->GetName());
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 1, strGridText);
					
					strGridText.Format(_T("%.3f"), fPosX);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 2, strGridText);
					
					int iFloor = -1;
					if(iPosY % 2 == 0)
					{
						iFloor = iPosY / 2 + 1;
					}
					
					strGridText.Format(_T("%d��"), iPosY);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 3, strGridText);		
					
					strGridText.Format(_T("%.3f"), fPosY);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 4, strGridText);
					
					strGridText.Format(_T("%.3f"), fPosZ);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 5, strGridText);
					
					strGridText.Format(_T("%.3f"), fAngle);
					m_ctlNpcGrid.SetItemText(lvitem.iItem, 6, strGridText);
				}
			}
			while(cmd.MoveNext());	
		}
	}
	catch(char* strError)
	{
		AfxMessageBox(strError);
	}
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}

//-----------------------------------------------------------------------------
// Purpose: ��ƼƼ ����� DB�� �����մϴ�.
// Input  : iNpcIndex - 
//			iWorldNum -
//			pEnt - 
//			YLayer - 
//			iRegenTime - 
//			iTotalNum - 
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::InsertEntityList(int iNpcIndex, int iWorldNum, CEntity* pEnt, int YLayer, int iRegenTime, int iTotalNum)
{
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	ASSERT(pEnt != NULL && "Invalid Entity Pointer");
	if(iNpcIndex < 0)
	{		
		AfxMessageBox(_T("Invalid Mob Index"));
		return FALSE;
	}	
	try
	{	
		// �ش� ������ NPC �����͸� ������.
		FLOAT3D vPosition	= pEnt->GetLerpedPlacement().pl_PositionVector;
		ANGLE3D vAngle		= pEnt->GetLerpedPlacement().pl_OrientationAngle;
		
		CMobData* MD = CMobData::getData(iNpcIndex);
		CString strQuery = _T("");
		CDBCmd cmd;
		cmd.Init(theApp.m_pDBConn);
		CString strError;
		
		if(MD->IsShopper())
		{
			strQuery.Format(
				"SELECT a_zone_num FROM t_shop"
				" WHERE a_keeper_idx = %d"
				,iNpcIndex
				);		
			
			cmd.SetQuery(strQuery);
			if (!cmd.Open())
			{
				AfxMessageBox(_T("t_shop ���̺� ������ �� �����ϴ�."));
				return FALSE;
			}
			if(cmd.MoveFirst())
			{
				int			a_zone_num;
				cmd.GetRec("a_zone_num", a_zone_num);
				
				strError.Format("[%s]�� [%2d]�� ���� �̹� �����ϴ� �����Դϴ�.", MD->GetName(), a_zone_num);
				AfxMessageBox(strError);
				return FALSE;
			}
			else
			{
				// �������� ������, ���� �߰�����.
				strQuery.Format(
					"INSERT INTO t_shop"
					"(a_keeper_idx, a_zone_num, "
					"a_name, a_sell_rate, a_buy_rate, "
					"a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer)"
					" VALUES ("
					"%d, %d, \"%s\", %d, %d, %f, %f, %f, %f, %d)"
					, iNpcIndex
					, iWorldNum
					, MD->GetName()
					, 40
					, 100
					, vPosition(1)
					, vPosition(3)
					, vPosition(2) + 0.5f
					, vAngle(1)
					, YLayer
					);
			}
		}
		else
		{	
			strQuery.Format(
				"INSERT INTO t_npc_regen"
				" (a_zone_num, a_npc_idx, a_pos_x, a_y_layer, a_pos_h, a_pos_r, a_pos_z,"
				"a_regen_sec, a_total_num)"
				
				" VALUES ("
				"%d,%d,%f,%d,%f,%f,%f,"
				"%d,%d)"
				
				,iWorldNum
				,iNpcIndex
				,vPosition(1)
				,YLayer
				,vPosition(2) + 0.5f
				,vAngle(1)
				,vPosition(3)
				,iRegenTime
				,iTotalNum
				);
		}
		
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			strError.Format("[%s]�� �����͸� �Է��� �� �����ϴ�.", MD->GetName());
			AfxMessageBox(strError);
			return FALSE;
		}
	}
	catch(char* strError)
	{
		AfxMessageBox(strError);
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: ���忡�� ENEMY ��ƼƼ���� ����� ����ϴ�.  �׸��� �� ����� DB�� ����˴ϴ�.
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CNpcDataDialog::ScanEntities()
{
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	INDEX	iCount			= pDoc->m_woWorld.wo_cenAllEntities.Count();
	pDoc->m_woWorld.wo_cenEnemyHolders.Clear();

	// ���� ������ ENF_ENEMY �Ӽ��� ���� �ִ� ��ƼƼ�� ã�Ƽ� �迭�� �ֽ��ϴ�.
	FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenAllEntities, CEntity, iten) 
	{
		if(iten->IsEnemy())
		{
			/*
			if(iNpcIndex != -1)
			{
				CDLLEntityClass *pdecDLLClass	= iten->GetClass()->ec_pdecDLLClass;
				CEntityProperty &epProperty		= pdecDLLClass->dec_aepProperties[2];		// NPC Type
				INDEX iEnemyType = ENTITYPROPERTY( &*iten, epProperty.ep_slOffset, INDEX);
				if(iEnemyType != iNpcIndex)		continue;
			}
			*/
			// �߰��� ��ƼƼ�� �迭�� �߰��մϴ�.
			pDoc->m_woWorld.wo_cenEnemyHolders.Add(iten);
		}		
	}

	CString strTitle = _T("");
	strTitle.Format(_T("NPC REGEN LIST - [%d/%d]"), pDoc->m_woWorld.wo_cenEnemyHolders.Count(), iCount);
	this->SetWindowText((LPCTSTR)strTitle);
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: ��ƼƼ ������ DB�� �߰���...
// Input  : pEnt - 
//-----------------------------------------------------------------------------
void CNpcDataDialog::InsertNpcData(CEntity* pEnt, int iWorldNum)
{
	ASSERT(pEnt != NULL && "Invalid Entity Pointer");
	INDEX tmpID = pEnt->en_ulID;
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	// ������Ƽ ������ �о����.
	CDLLEntityClass *pdecDLLClass	= pEnt->GetClass()->ec_pdecDLLClass;	
	CEntityProperty &epProperty		= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 91);		// NPC Type
	if(epProperty.ep_strName!=NULL && CTString(epProperty.ep_strName) != "" )
	{
		// NOTE : ������Ƽ�� ���� �ִ� �κ��Դϴ�.  
		// NOTE : ĳ������ �ִϸ��̼� �������� �̷������� ó���� �� ������...
		// NOTE : Enemy Holder�� ������ �Ŀ�, ������ Ÿ���� �����ϴ� ��???
		// NOTE : �Ʒ� EnemyType�� 1���� �����ؾ� �մϴ�.
		INDEX iEnemyType = ENTITYPROPERTY( &*pEnt, epProperty.ep_slOffset, INDEX);
		if(iEnemyType == 0)
			return;
		
		CEntityProperty &epRegenTime	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 21);		// Regen Time
		INDEX iRegenTime = ENTITYPROPERTY( &*pEnt, epRegenTime.ep_slOffset, INDEX);
		
		CEntityProperty &epTotalCount	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 150);		// Total Count
		INDEX iTotalCount= ENTITYPROPERTY( &*pEnt, epTotalCount.ep_slOffset, INDEX);

		// FallDownToFloor�� �Ϻ� �ڵ�.
		CPlacement3D plPlacement = pEnt->GetPlacement();
		plPlacement.pl_PositionVector(2) += 0.2f;
		FLOAT3D vRay[4];	
		FLOATaabbox3D box;
		pEnt->GetModelInstance()->GetCurrentColisionBox(box);	
		
		FLOAT3D vCenterUp	= box.Center();
		vCenterUp(2)		= box.Max()(2);
		vRay[0]				= vCenterUp;
		vRay[1]				= vCenterUp;
		vRay[2]				= vCenterUp;
		vRay[3]				= vCenterUp;
		
		FLOAT fMaxY			= -9999999.0f;
		CEntity* penHit		= NULL;
		BOOL bFloorHitted	= FALSE;
		for( INDEX iRay=0; iRay<4; iRay++)
		{
			FLOAT3D vSource = plPlacement.pl_PositionVector+vRay[iRay];
			FLOAT3D vTarget = vSource;
			vTarget(2)		-= (1000.0f - (iRay * 200)); // Ray���̸� ���� ª��
			CCastRay crRay( pEnt, vSource, vTarget);
			crRay.cr_ttHitModels			= CCastRay::TT_NONE; // CCastRay::TT_FULLSEETHROUGH;
			crRay.cr_bHitTranslucentPortals = TRUE;
			crRay.cr_bPhysical				= TRUE;
			pDoc->m_woWorld.CastRay(crRay);
			if( (crRay.cr_penHit != NULL) && (crRay.cr_vHit(2) > fMaxY)) 
			{
				penHit			= crRay.cr_penHit;
				fMaxY			= crRay.cr_vHit(2);
				bFloorHitted	= TRUE;
			}
		}
		if(bFloorHitted)
		{
			if(penHit->en_RenderType == CEntity::RT_TERRAIN)
			{
				InsertEntityList(iEnemyType, iWorldNum, pEnt, 0, iRegenTime, iTotalCount);
			}
			else
			{
				FLOAT3D			vPoint;
				FLOATplane3D	plPlane;
				FLOAT			fDistanceToEdge;
				CBrushPolygon	*pBrushPolygon = NULL;
				pBrushPolygon	= pEnt->GetNearestPolygon(vPoint, plPlane, fDistanceToEdge);

				// WSS_DRATAN_SEIGEWARFARE 2007/08/30
				// �ʵ� �̱� ������ ���� �ʿ䰡 ����...
/*				if(pBrushPolygon == NULL || 
					theApp.m_iWorldType == ZONE_FIELD || 
					theApp.m_iWorldType == ZONE_SDUNGEON)
				{
					InsertEntityList(iEnemyType, iWorldNum, pEnt, 0, iRegenTime, iTotalCount);
				}
				else*/
				{
					UBYTE ubAttribute = pBrushPolygon->bpo_ubPolygonAttribute;

					// Unwalkable
					if((ubAttribute >= BPOA_UNWALKABLE1F && ubAttribute <= BPOA_UNWALKABLE5F) ||
						(ubAttribute >= BPOA_STAIRWALL1F2F && ubAttribute <= BPOA_STAIRWALL4F5F) ||
						(ubAttribute >= BPOA_STAIR1F2F && ubAttribute <= BPOA_STAIR4F5F) )
						
					{
						ULONG ulID = pEnt->en_ulID;
						CString strMsg = _T("");
						strMsg.Format(_T("[%ld] �� �ش��ϴ� ��ƼƼ�� �ùٸ��� �ʴ� �̵��Ҽ� ���� ���� ��ġ�ϰ� �ֽ��ϴ�."), ulID);
						//AfxMessageBox(strMsg, MB_OKCANCEL);
						AfxMessageBox(strMsg);
					}
					// Walkable
					else
					{
						// on the floor
						if((ubAttribute >= BPOA_WALKABLE1F && ubAttribute <= BPOA_WALKABLE5F))
						{
							InsertEntityList(iEnemyType, iWorldNum, pEnt, ((ubAttribute-1) % 10) * 2, iRegenTime, iTotalCount);
						}
						// on the terrain?
						else
						{
							InsertEntityList(iEnemyType, iWorldNum, pEnt, 0, iRegenTime, iTotalCount);
						}
					}
				}
			}
		}		
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnEnemyUpdate()
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	if (AfxMessageBox(_T("������ ���� ������ ��� ����� ���� ���ŵ˴ϴ�. �����Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDCANCEL)
		return;	
	ScanEntities();
	
	if(pDoc->m_woWorld.wo_cenEnemyHolders.Count() > 0)
	{
		DeleteShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
		DeleteNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
		FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenEnemyHolders, CEntity, itent) 
		{
			InsertNpcData(itent, theApp.m_iWorldNum);
		}
		RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
		RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	}
	else
	{
		AfxMessageBox(_T("����� NPC�� �Ѹ����� �����ϴ�.  Ȯ�� �� ��õ� �Ͻʽÿ�."), MB_OK);
	}	
}

//-----------------------------------------------------------------------------
// Purpose: 
// ��ƼƼ�� ��ġ���� ���� ������ �ִ� �κ�.
// ���߿� �������� ���Ѱ�.
// Input  : MD - 
//			XPos - 
//			YLayer - 
//			ZPos - 
// Output : CEntity*
//-----------------------------------------------------------------------------
CEntity* CNpcDataDialog::CreateEntityByYLayer(CMobData& MD, float XPos, int YLayer, float YPos, float ZPos, float fAngle, int iRegenTime, int iTotalNum)
{
	// ������ ���̿��� Ray�� ����...
	// ������ ���̰��� �������� �����ݴϴ�.
	// �������� ���� ���� �󸶸� ���ؼ� ó���ؾ� �ϴ����� ����...
	// �� ��쿡, ���� ������
	// FallDownToFloor()
	//CCastRay crRay;
	//crRay.Init();

	CWorldEditorDoc *pDoc			= theApp.GetDocument();

	CPlacement3D	plPosition;
	//plPosition.pl_PositionVector	= FLOAT3D(XPos, YLayer, ZPos);		// ����.
	plPosition.pl_PositionVector	= FLOAT3D(XPos, YPos, ZPos);		// ����.
	plPosition.pl_OrientationAngle	= ANGLE3D(fAngle, 0.0f, 0.0f);

	UpdateData(TRUE);
	CEntity			*penEntity		= NULL;
	penEntity						= pDoc->m_woWorld.CreateEntity_t(plPosition, CLASS_ENEMY);

	CMobData::SetMobDataToNPC( penEntity, &MD, MD.GetName() );

	if(m_bShowEnemyModel && MD.GetMobIndex() != -1)
	{
		//CMobData::SetMobDataToNPC(penEntity, MD);

		//-----------------------------------------------------------------------------
		penEntity->InitAsSkaModel();
		penEntity->SetSkaModel(MD.GetMobSmcFileName());
	}

	// Set Regen Time
	CDLLEntityClass *pdecDLLClass	= penEntity->GetClass()->ec_pdecDLLClass;	
	CEntityProperty &epRegenTime	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 21);		// Regen Time
	ENTITYPROPERTY( &*penEntity, epRegenTime.ep_slOffset, INDEX) = iRegenTime;	

	CEntityProperty &epTotalCount	= *pdecDLLClass->PropertyForTypeAndID(CEntityProperty::EPT_INDEX, 150);		// Total Count
	ENTITYPROPERTY( &*penEntity, epTotalCount.ep_slOffset, INDEX) = iTotalNum;		
	return penEntity;
}

void CNpcDataDialog::OnOK() 
{
	// TODO: Add extra validation here	
	//CDialog::OnOK();
}

//-----------------------------------------------------------------------------
// Purpose: ��� ENEMY ��ƼƼ���� ������.
//-----------------------------------------------------------------------------
void CNpcDataDialog::DestroyEntities()
{
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	
	// ���õ� ���� ����.
	//int iStartRow			= m_ctlNpcGrid.GetRow();
	//int iEndRow			= m_ctlNpcGrid.GetRowSel();

	pDoc->m_selEntitySelection.Clear();
	FOREACHINDYNAMICCONTAINER(pDoc->m_woWorld.wo_cenEnemyHolders, CEntity, iten)
	{
		iten->Destroy();
	}
	pDoc->UpdateAllViews( NULL);
}

void CNpcDataDialog::OnNpcGenerate() 
{
	// TODO: Add your control notification handler code here
	CWorldEditorDoc *pDoc	= theApp.GetDocument();	
	if (AfxMessageBox(_T("���� ���� ��� ENEMY ��ƼƼ�� ������ �Ŀ� �ٽ� �����˴ϴ�.  �����Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDCANCEL)	
		return;	
	
	// ������� ��� ENEMY�� ��ĵ�մϴ�.
	ScanEntities();
	DestroyEntities();

	// NOTE : ������ ����� ���� ���� �ּ��� �����غ���.
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	
	// �ش� ��ƼƼ�� ������.
	for(INDEX iEntity = 0; iEntity < m_paNPCWithEntity.Count(); iEntity++)
	{		
		NPCWithEntity &npcEntity	= m_paNPCWithEntity[iEntity];
		if(npcEntity.pMobData != NULL)
		{
			CEntity			*pEntity = NULL;
			if(theApp.m_iWorldType == ZONE_FIELD)
			{
				pEntity = CreateEntityByYLayer(*npcEntity.pMobData, npcEntity.fPosX, npcEntity.iPosY, 200.0f, npcEntity.fPosZ, npcEntity.fAngle, npcEntity.iRegenTime, npcEntity.iTotalNum);
			}			
			else
			{
				pEntity = CreateEntityByYLayer(*npcEntity.pMobData, npcEntity.fPosX, npcEntity.iPosY, npcEntity.fPosY, npcEntity.fPosZ, npcEntity.fAngle, npcEntity.iRegenTime, npcEntity.iTotalNum);
			}
			// �Ʒ� �Լ��� �ܺη� ���Ѱ�...
			//CEntity			*pEntity	= CreateEntityByYLayer(*npcEntity.pMobData, npcEntity.fPosX, npcEntity.iPosY, npcEntity.fPosY, npcEntity.fPosZ);
			//((CEnemy*)pEntity)->GetName();
			pEntity->Initialize();
			pEntity->FallDownToFloor();
		}
	}
}

void CNpcDataDialog::OnSelchangeFindByYlayer() 
{
	// TODO: Add your control notification handler code here
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
}

void CNpcDataDialog::OnSelchangeFindByName() 
{
	// TODO: Add your control notification handler code here
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
}

void CNpcDataDialog::OnSelchangeFindByType()
{
	// TODO: Add your control notification handler code here
	RefreshNpcData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
	RefreshShopData(theApp.m_iWorldNum, m_ctlcbFindByName.GetCurSel(), m_ctlcbFindByType.GetCurSel(), m_ctlcbFindByYLayer.GetCurSel() - 1);
}

void CNpcDataDialog::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu menu;
	
	// �˾� �޴�~!!!
	if( menu.LoadMenu(IDR_DB_DATA_POPUP))
	{
		CMenu* pPopup = menu.GetSubMenu(0);
		pPopup->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_LEFTALIGN,
			point.x, point.y, this);
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnExportDbData() 
{
	if (AfxMessageBox(_T("������ �Ǹ� ������ ������ ������� �ʽ��ϴ�.  ��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDCANCEL)
		return;	

	CString strQuery	= _T("");
	strQuery.Format(
			"SELECT a_npc_idx, a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer, a_regen_sec, a_total_num"
			" FROM t_npc_regen WHERE a_zone_num = %d"
			" ORDER by a_npc_idx", theApp.m_iWorldNum);
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);

	if (!cmd.Open())
	{
		AfxMessageBox("Cannot open regen table");
		return;
	}
	if (!cmd.MoveFirst())
	{
		AfxMessageBox("Not found!!!");
		return;
	}

	//ByteQueueData	tmpQueBuffer;
	CString strFilter	= _T("DB Data Files(*.sav) | *.sav||");
	CString szFileName	= _T("");
	szFileName.Format(_T("NpcRegen%d.sav"), theApp.m_iWorldNum);
	CFileDialog fDlg(FALSE, NULL, szFileName,
			OFN_EXPLORER | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);

	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		szFileName = fDlg.m_ofn.lpstrFile;
		
		if(!wFile.Open(szFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			return;
		
		int iMobCount			= cmd.m_nrecords;			// Mob�� ����.
		
		int i					= 0;
		wFile.Write(&theApp.m_iWorldNum, sizeof(int));		// ���� ��ȣ.
		wFile.Write(&iMobCount, sizeof(int));				// Mob�� ����.

/*		CString tmpflush;
		tmpflush.Format("World:%d Mob Count:%d",theApp.m_iWorldNum, iMobCount);
		tmpQueBuffer.WriteString(tmpflush.GetBuffer(0));
		tmpflush.Format("npcIndex,Posx,Posz,Posh,Rotation,yLayer,regen_time,totalNum");
		tmpQueBuffer.WriteString(tmpflush.GetBuffer(0));*/
		do
		{
			int			a_npc_idx;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h;
			float		a_pos_r;
			int			a_y_layer;
			int			a_regen_sec; 
			int			a_total_num;

			cmd.GetRec("a_npc_idx",		a_npc_idx);
			cmd.GetRec("a_pos_x",		a_pos_x);
			cmd.GetRec("a_pos_z",		a_pos_z);
			cmd.GetRec("a_pos_h",		a_pos_h);
			cmd.GetRec("a_pos_r",		a_pos_r);
			cmd.GetRec("a_y_layer",		a_y_layer);
			cmd.GetRec("a_regen_sec",	a_regen_sec);
			cmd.GetRec("a_total_num",	a_total_num);

			wFile.Write(&a_npc_idx,		sizeof(int));
			wFile.Write(&a_pos_x,		sizeof(float));
			wFile.Write(&a_pos_z,		sizeof(float));
			wFile.Write(&a_pos_h,		sizeof(float));
			wFile.Write(&a_pos_r,		sizeof(float));
			wFile.Write(&a_y_layer,		sizeof(int));
			wFile.Write(&a_regen_sec,	sizeof(int));
			wFile.Write(&a_total_num,	sizeof(int));
			
			/*CString strFlush;
			strFlush.Format(_T("%n,%d,%s,%f,%f,%f,%d\n"),*/

			i++;
		} while (cmd.MoveNext());

		//----------------------------------------------------------------------
		// ���� ���� ȭ�Ͽ� ����.

		int iNumOfShopper = 0;
		strQuery.Format(
			"SELECT a_keeper_idx, a_name, a_sell_rate, a_buy_rate, a_pos_x, a_pos_z, a_pos_h, a_pos_r, a_y_layer"
			" FROM t_shop WHERE a_zone_num = %d"
			" ORDER by a_keeper_idx", theApp.m_iWorldNum);
		
		cmd.SetQuery(strQuery);
		
		if (!cmd.Open())
		{
			AfxMessageBox("Cannot open shop table");
			wFile.Close();
			return;
		}

		iNumOfShopper		= cmd.m_nrecords;			// Shopper�� ����.
		wFile.Write(&iNumOfShopper,		sizeof(int));

		if (!cmd.MoveFirst())
		{			
			wFile.Close();
			return;
		}

		do
		{
			int			a_keeper_idx;
			CString		strShopName;
			float		a_sell_rate;
			float		a_buy_rate;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h; 
			float		a_pos_r;
			int			a_y_layer;
			int			len;

			cmd.GetRec("a_keeper_idx",	a_keeper_idx);			
			strShopName.Format(_T("%s"), cmd.GetRec("a_name"));
			cmd.GetRec("a_sell_rate",	a_sell_rate);
			cmd.GetRec("a_buy_rate",	a_buy_rate);
			cmd.GetRec("a_pos_x",		a_pos_x);
			cmd.GetRec("a_pos_z",		a_pos_z);
			cmd.GetRec("a_pos_h",		a_pos_h);
			cmd.GetRec("a_pos_r",		a_pos_r);
			cmd.GetRec("a_y_layer",		a_y_layer);

			len = strShopName.GetLength();
			wFile.Write(&a_keeper_idx,	sizeof(int));	
			wFile.Write(&len,			sizeof(int));
			wFile.Write(strShopName,	len);
			wFile.Write(&a_sell_rate,	sizeof(float));
			wFile.Write(&a_buy_rate,	sizeof(float));
			wFile.Write(&a_pos_x,		sizeof(float));
			wFile.Write(&a_pos_z,		sizeof(float));
			wFile.Write(&a_pos_h,		sizeof(float));
			wFile.Write(&a_pos_r,		sizeof(float));
			wFile.Write(&a_y_layer,		sizeof(int));
		} while (cmd.MoveNext());
		wFile.Close();
	}
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNpcDataDialog::OnImportDbData()
{
	if (AfxMessageBox(_T("�����͸� ������ ��� ������ �����Ϳ� �߰��˴ϴ�.  ���� �����͸� ����ð� �����Ͻʽÿ�.  ��� �Ͻðڽ��ϱ�?"), MB_OKCANCEL) == IDCANCEL)
		return;	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	CString strQuery	= _T("");
	CString strFilter	= _T("DB Data Files(*.sav) | *.sav||");
	CString szFileName	= _T("");
	szFileName.Format(_T("NpcRegen%d.sav"), theApp.m_iWorldNum);
	CFileDialog fDlg(TRUE, NULL, szFileName,
			OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);
	
	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		szFileName = fDlg.m_ofn.lpstrFile;
		
		if(!wFile.Open(szFileName, CFile::modeRead | CFile::typeBinary))
			return;
		
		// DB�� ����� �ٽ� ����.(npc regen ����)
		strQuery.Format("delete from t_npc_regen where a_zone_num = %d", theApp.m_iWorldNum);
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			AfxMessageBox(_T("Cannot npc_regen delete"));
			wFile.Close();
			return;
		}

		int iMobCount		= 0;
		int iWorldNum		= 0;

		wFile.Read(&iWorldNum, sizeof(int));			// ���� ��ȣ.
		wFile.Read(&iMobCount, sizeof(int));			// Mob�� ����.

		int i;

		for (i = 0; i < iMobCount; ++i)
		{
			int			a_npc_idx;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h;
			float		a_pos_r;
			int			a_y_layer;
			int			a_regen_sec; 
			int			a_total_num;

			wFile.Read(&a_npc_idx,		sizeof(int));
			wFile.Read(&a_pos_x,		sizeof(float));
			wFile.Read(&a_pos_z,		sizeof(float));
			wFile.Read(&a_pos_h,		sizeof(float));
			wFile.Read(&a_pos_r,		sizeof(float));
			wFile.Read(&a_y_layer,		sizeof(int));
			wFile.Read(&a_regen_sec,	sizeof(int));
			wFile.Read(&a_total_num,	sizeof(int));
			
			if(a_npc_idx < 0)	continue;
			
			strQuery.Format(
				"INSERT INTO t_npc_regen"
				" (a_zone_num, a_npc_idx, a_pos_x, a_pos_z, a_pos_r, a_pos_h, a_y_layer,"
				"a_regen_sec, a_total_num)"
				
				" VALUES ("
				"%d,%d,%f,%f,%f,%f,%d,"
				"%d,%d)"
				
				,theApp.m_iWorldNum
				,a_npc_idx
				,a_pos_x
				,a_pos_z
				,a_pos_r
				,a_pos_h
				,a_y_layer
				,a_regen_sec
				,a_total_num
				);

			cmd.SetQuery(strQuery);
			if (!cmd.Update())
			{
				AfxMessageBox(_T("Cannot Insert"));
				wFile.Close();
				return;
			}
		}
		
		// DB�� ����� �ٽ� ����.(shop ����)
		strQuery.Format("delete from t_shop where a_zone_num = %d", theApp.m_iWorldNum);
		cmd.SetQuery(strQuery);
		if (!cmd.Update())
		{
			AfxMessageBox(_T("Cannot t_shop delete"));
			wFile.Close();
			return;
		}

		int iNumOfShopper;
		wFile.Read(&iNumOfShopper,		sizeof(int));
		for (i = 0; i < iNumOfShopper; ++i)
		{
			int			a_keeper_idx;
			char		a_name[50] = {0};
			float		a_sell_rate;
			float		a_buy_rate;
			float		a_pos_x;
			float		a_pos_z;
			float		a_pos_h; 
			float		a_pos_r;
			int			a_y_layer;
			int			len;

			wFile.Read(&a_keeper_idx,	sizeof(int));
			wFile.Read(&len,			sizeof(int));
			wFile.Read(&a_name,			len);
			wFile.Read(&a_sell_rate,	sizeof(float));
			wFile.Read(&a_buy_rate,		sizeof(float));
			wFile.Read(&a_pos_x,		sizeof(float));
			wFile.Read(&a_pos_z,		sizeof(float));
			wFile.Read(&a_pos_h,		sizeof(float));
			wFile.Read(&a_pos_r,		sizeof(float));
			wFile.Read(&a_y_layer,		sizeof(int));

			if(a_keeper_idx < 0)	continue;

			strQuery.Format(
				"INSERT INTO t_shop"
				" (a_keeper_idx, a_name, a_sell_rate, a_buy_rate, a_pos_x, a_pos_z, a_pos_r, a_pos_h, a_y_layer,"
				"a_zone_num)"
				
				" VALUES ("
				"%d,\"%s\",%f,%f,%f,%f,%f,%f,%d,"
				"%d)"				
				
				,a_keeper_idx
				,a_name
				,a_sell_rate
				,a_buy_rate
				,a_pos_x
				,a_pos_z
				,a_pos_r
				,a_pos_h
				,a_y_layer
				,theApp.m_iWorldNum
				);
			cmd.SetQuery(strQuery);
			if (!cmd.Update())
			{
				AfxMessageBox(_T("Cannot Insert"));
				wFile.Close();
				return;
			}
	}
	wFile.Close();
	}
}

void CNpcDataDialog::BackupData()
{
	OnImportDbData();
}

void CNpcDataDialog::RestoreData()
{
	OnExportDbData();
}

void CNpcDataDialog::OnEnemyEdit() 
{
	// TODO: Add your control notification handler code here
	
}

void CNpcDataDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_paNPCWithEntity.PopAll();
	
	CDialog::PostNcDestroy();
}
