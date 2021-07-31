// MerchantDialog.cpp : implementation file
//

#include "stdafx.h"
#include "worldeditor.h"
#include "MerchantDialog.h"
#include "MakeFileDlg.h"
#include "resource.h"
#include "dbcmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMerchantDialog dialog

enum eColumnType
{
	COLUMN_ITEM_NAME = 1,
	COLUMN_ITEM_TYPE,
	COLUMN_ITEM_SUB_TYPE,
	COLUMN_ITEM_LEVEL,
	COLUMN_ITEM_PRICE,
	COLUMN_ITEM_NATIONAL_START,
};
//////////////////////////////////////////////////////////////////////////
// [100106 sora] �ش� ���� �ش��ϴ� ������ shop������ �������� üũ
// shop������ ����ϴ� ���������� WorldEditor.h �� _aStrCountry�� ����
//////////////////////////////////////////////////////////////////////////
BOOL ValidCountry(int num)
{
	if(num == MALAYSIA_ENG || num == HONGKONG_ENG)
		return FALSE;

	return TRUE;
}

int GetColumnCount(int nCountry)
{
	int nUpCnt = 0;

	if(nCountry > HONGKONG_ENG)
		nUpCnt = 2; // ȫ�῵��, ������ ��� ����
	else if(nCountry > MALAYSIA_ENG)
		nUpCnt = 1; // ������ ��� ����

	return COLUMN_ITEM_NATIONAL_START + nCountry - nUpCnt;
}

CMerchantDialog::CMerchantDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMerchantDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMerchantDialog)
	m_strShopperName = _T("");
	m_bHealer = FALSE;
	m_bKnight = FALSE;
	m_bMage = FALSE;
	m_bRogue = FALSE;
	m_bTitan = FALSE;
	m_bSorcerer = FALSE;
	m_iBuyRate = 0;
	m_iSelectedShopper = -1;
	m_iSellRate = 0;
	//}}AFX_DATA_INIT
}


void CMerchantDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMerchantDialog)
	DDX_Control(pDX, IDC_FIND_BY_TYPE, m_ctlFindByType);
	DDX_Control(pDX, IDC_FIND_BY_SUBTYPE, m_ctlFindBySubType);
	DDX_Control(pDX, IDC_ITEM_LIST_SELL, m_ctlItemListOfNPC);
	DDX_Control(pDX, IDC_ITEM_LIST_TOTAL, m_ctlItemList);
	DDX_Text(pDX, IDC_SHOPPER_NAME, m_strShopperName);
	DDX_Check(pDX, IDC_HEALER, m_bHealer);
	DDX_Check(pDX, IDC_KNIGHT, m_bKnight);
	DDX_Check(pDX, IDC_MAGE, m_bMage);
	DDX_Check(pDX, IDC_ROGUE, m_bRogue);
	DDX_Check(pDX, IDC_TITAN, m_bTitan);
	DDX_Check(pDX, IDC_SORCERER, m_bSorcerer);
	DDX_Text(pDX, IDC_BUY_RATE, m_iBuyRate);	
	DDX_Text(pDX, IDC_SELL_RATE, m_iSellRate);	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMerchantDialog, CDialog)
	//{{AFX_MSG_MAP(CMerchantDialog)
	ON_BN_CLICKED(IDC_ITEM_TO_NPC, OnItemToNpc)
	ON_BN_CLICKED(IDC_ITEM_TO_NPC_ALL, OnItemToNpcAll)
	ON_BN_CLICKED(IDC_ITEM_FROM_NPC, OnItemFromNpc)
	ON_BN_CLICKED(IDC_ITEM_FROM_NPC_ALL, OnItemFromNpcAll)
	ON_CBN_SELCHANGE(IDC_FIND_BY_TYPE, OnSelchangeFindByType)
	ON_CBN_SELCHANGE(IDC_FIND_BY_SUBTYPE, OnSelchangeFindBySubtype)
	ON_CBN_SELCHANGE(IDC_FIND_BY_RACE, OnSelchangeFindByRace)
	ON_BN_CLICKED(IDC_MERCHANT_UPDATE, OnMerchantUpdate)
	ON_BN_CLICKED(IDC_MERCHANT_REFRESH, OnMerchantRefresh)
	ON_BN_CLICKED(IDC_MERCHANT_MAKEFILE, OnMerchantMakefile)
	ON_BN_CLICKED(IDC_TITAN, OnTitan)
	ON_BN_CLICKED(IDC_KNIGHT, OnKnight)
	ON_BN_CLICKED(IDC_HEALER, OnHealer)
	ON_BN_CLICKED(IDC_MAGE, OnMage)
	ON_BN_CLICKED(IDC_ROGUE, OnRogue)
	ON_BN_CLICKED(IDC_SORCERER, OnSorcerer)		
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ITEM_LIST_SELL, OnItemchangedItemListSell)	
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_CBN_SELCHANGE(IDC_COUNTRY, OnChangeCountry)
	ON_BN_CLICKED(IDC_BTN_COPY, OnCopyCountry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMerchantDialog message handlers

BOOL CMerchantDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CListCtrl* pWnd = (CListCtrl*)GetDlgItem(IDC_ITEM_LIST_SELL);	
	pWnd->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);

	pWnd			= (CListCtrl*)GetDlgItem(IDC_ITEM_LIST_TOTAL);
	pWnd->SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);	

	//--------------------------------------------------------
	CString aStrFindByType[] =
	{ _T("��ü"), _T("����"), _T("��"), _T("��ȸ��"), 
		_T("źȯ"), _T("��Ÿ"), _T("�Ǽ��縮"), _T("����") };

	int i;

	for (i = 0; i < 8; ++i)
		m_ctlFindByType.InsertString(i, aStrFindByType[i]);

	m_ctlFindByType.SetCurSel(0);

	//--------------------------------------------------------
	int aiColWidth[COLUMN_ITEM_NATIONAL_START + MAX_COUNTRY] = { 35, 80, 50, 50, 50, 60, };

	for(i=COLUMN_ITEM_NATIONAL_START; i<COLUMN_ITEM_NATIONAL_START + MAX_COUNTRY; ++i)
	{
		if(ValidCountry(i-COLUMN_ITEM_NATIONAL_START))
			aiColWidth[i] = 50;
		else
			aiColWidth[i] = 0;
	}
//	{ 35, 80, 50, 50, 60, 50, 50, 50, 50, 50, 50, 50 };	

	CString aStrTitle[] =
	{	
		_T("IDX"), 
		_T("NAME"), 
		_T("TYPE"), 
		_T("S-TYPE"),
		_T("LEVEL"), 
		_T("PRICE"),
	};

	//--------------------------------------------------------
	// ��ü ����Ʈ ��� ���â.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;

	// �÷� ����� �߾ӿ� ���.
	lvcolumn.fmt = LVCFMT_CENTER;

	for(i = 0; i < COLUMN_ITEM_NATIONAL_START; ++i)
	{
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlItemList.InsertColumn(i, &lvcolumn);
	}

	// ������.
	for( i = COLUMN_ITEM_NATIONAL_START; i < COLUMN_ITEM_NATIONAL_START + MAX_COUNTRY; ++i )
	{
		if (!ValidCountry(i - COLUMN_ITEM_NATIONAL_START)) continue;
		lvcolumn.iSubItem	= i;
		lvcolumn.cx			= aiColWidth[i];
		lvcolumn.pszText	= (LPSTR)(LPCTSTR)_aStrCountry[i - COLUMN_ITEM_NATIONAL_START];
		m_ctlItemList.InsertColumn(i, &lvcolumn);
	}
	//--------------------------------------------------------
	// ���õ� ������ ��� ���â.
	//--------------------------------------------------------
	LV_COLUMN lvcolumn_;
	lvcolumn_.mask			= LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;

	// �÷� ����� �߾ӿ� ���.
	lvcolumn_.fmt			= LVCFMT_CENTER;

	for(i = 0; i < COLUMN_ITEM_NATIONAL_START; ++i)
	{
		lvcolumn_.iSubItem	= i;
		lvcolumn_.cx		= aiColWidth[i];
		lvcolumn_.pszText	= (LPSTR)(LPCTSTR)aStrTitle[i];
		m_ctlItemListOfNPC.InsertColumn(i, &lvcolumn_);
	}

	// ������.
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COUNTRY);
	CComboBox* pCBCopyFrom = (CComboBox*)GetDlgItem(IDC_COPYFROM);
	CComboBox* pCBCopyTo = (CComboBox*)GetDlgItem(IDC_COPYTO);
	
	for( i = COLUMN_ITEM_NATIONAL_START; i < COLUMN_ITEM_NATIONAL_START + MAX_COUNTRY; ++i )
	{
		// �޺��ڽ��� �����̸� �ֱ�
		pCB->AddString(_aStrCountry[i-COLUMN_ITEM_NATIONAL_START]);
		pCBCopyFrom->AddString(_aStrCountry[i-COLUMN_ITEM_NATIONAL_START]);
		pCBCopyTo->AddString(_aStrCountry[i-COLUMN_ITEM_NATIONAL_START]);

		if (!ValidCountry(i - COLUMN_ITEM_NATIONAL_START)) continue;
		lvcolumn_.iSubItem	= i;
		lvcolumn_.cx		= aiColWidth[i];
		lvcolumn_.pszText	= (LPSTR)(LPCTSTR)_aStrCountry[i-COLUMN_ITEM_NATIONAL_START];
		m_ctlItemListOfNPC.InsertColumn(i, &lvcolumn_);
	}

	pCB->AddString(_T("��ü"));

	//--------------------------------------------------------	
	m_ctlItemList.DeleteAllItems();	
	RefreshItemList(m_ctlItemList, TRUE);
	RefreshItemListOfNPC(m_iSelectedShopper);
	GetTradeRate();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Sell Rate�� Buy Rate�� ����ϴ�.
void CMerchantDialog::GetTradeRate()
{
	CString strQuery = _T("");
	UpdateData(TRUE);
	strQuery.Format(_T("SELECT a_buy_rate, a_sell_rate FROM t_shop WHERE a_keeper_idx = %d"), m_iSelectedShopper);

	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Open())
	{
		AfxMessageBox(_T("Cannot open table"));
		return;
	}
	if (!cmd.MoveFirst())		return;

	cmd.GetRec("a_buy_rate",	m_iBuyRate);
	cmd.GetRec("a_sell_rate",	m_iSellRate);
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : listctrl - 
//			iZoneFlag - 
//			iItemIdx - 
//			iType - 
//			iSubType - 
//			iRace - 
//-----------------------------------------------------------------------------
void CMerchantDialog::RefreshItemList(CListCtrl& listctrl, BOOL bTotal, int iZoneFlag, int iItemIdx, int iNational, int iType, int iSubType, int iRace)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");

	UpdateData(TRUE);

	// �̺�Ʈ �����۰� ����Ʈ �����۰� ���� ������ ���.
	strQuery.Format(_T("SELECT * FROM t_item WHERE"
		" NOT (a_type_idx=4 AND (a_subtype_idx=0 OR a_subtype_idx=1 OR a_subtype_idx=5))"
		" AND a_zone_flag = %d"		
		), iZoneFlag);

	//--------------------------------------------------------
	// Find ���� �߰� ����...
	//--------------------------------------------------------	
	if(iItemIdx != -1)		// �ش��ϴ� �ε�����...
	{
		CString strItemIndex = _T("");
		strItemIndex.Format(_T(" AND a_index = %d"), iItemIdx);
		strQuery += strItemIndex;
	}

	if(iType != -1)			// �ش��ϴ� Ÿ�Ը�...
	{
		CString strType = _T("");
		strType.Format(_T(" AND a_type_idx = %d"), iType);
		strQuery += strType;

		if(iSubType != -1)	// �ش��ϴ� ���� Ÿ�Ը�...
		{
			CString strSubType = _T("");
			strSubType.Format(_T(" AND a_subtype_idx= %d"), iSubType);
			strQuery += strSubType;
		}
	}

	//-------------------------------------
	DWORD dwRace	= 0;
	dwRace			|= m_bTitan	? MASK_TITAN : 0;
	dwRace			|= m_bKnight ? MASK_KNIGHT : 0;
	dwRace			|= m_bHealer ? MASK_HEALER : 0;
	dwRace			|= m_bMage ? MASK_MAGE : 0;
	dwRace			|= m_bRogue ? MASK_ROGUE : 0;
	dwRace			|= m_bSorcerer ? MASK_SOCERER : 0;
	//-------------------------------------

	if(dwRace != 0)			// �ش��ϴ� ������.
	{
		CString strRace = _T("");
		strRace.Format(_T(" AND a_job_flag & %ld"), dwRace);
		strQuery += strRace;
	}

	// ������ ���...
	//CString strOrder = _T(" ORDER BY a_job_flag, a_level, a_type_idx, a_subtype_idx, a_index");	
	CString strOrder = _T(" ORDER BY a_index, a_type_idx, a_subtype_idx");	
	strQuery += strOrder;	
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Open())
	{
		AfxMessageBox(_T("Cannot open table"));
		return;
	}
	if (!cmd.MoveFirst())		return;

	//--------------------------------------------------------
	// ������ �߰��ϱ�...
	//--------------------------------------------------------
	LV_ITEM lvitem;
	do
	{
		int		iIndex			= -1;
		int		iTypeIndex		= -1;
		int		iSubTypeIndex	= -1;
		int		iLevel	= -1;
		int		iPrice			= -1;
		CString strItemName		= _T("");
		CString	strTemp			= _T("");
		cmd.GetRec("a_index",		iIndex);
		cmd.GetRec("a_type_idx",	iTypeIndex);
		cmd.GetRec("a_subtype_idx",	iSubTypeIndex);
		cmd.GetRec("a_level",	iLevel);
		cmd.GetRec("a_price",		iPrice);
		strItemName.Format(_T("%s"), cmd.GetRec("a_name"));

		strTemp.Format(_T("%d"), iIndex);
		lvitem.iItem	= listctrl.GetItemCount();
		lvitem.mask		= LVIF_TEXT;
		lvitem.iSubItem = 0;
		lvitem.pszText	= (LPSTR)(LPCTSTR)strTemp;

		listctrl.InsertItem(&lvitem);
		listctrl.SetItemText(lvitem.iItem, COLUMN_ITEM_NAME, strItemName);		// Name

		switch(iTypeIndex)
		{
		case CItemData::ITEM_WEAPON:
			strTemp.Format(_T("[%d] ����"), iTypeIndex);
			break;
		case CItemData::ITEM_SHIELD:
			strTemp.Format(_T("[%d] ��"), iTypeIndex);
			break;
		case CItemData::ITEM_ONCEUSE:
			strTemp.Format(_T("[%d] ��ȸ��"), iTypeIndex);
			break;
		case CItemData::ITEM_BULLET:
			strTemp.Format(_T("[%d] ź��"), iTypeIndex);
			break;			
		case CItemData::ITEM_ETC:
			strTemp.Format(_T("[%d] ��Ÿ"), iTypeIndex);
			break;			
		case CItemData::ITEM_ACCESSORY:
			strTemp.Format(_T("[%d] �Ǽ��縮"), iTypeIndex);
			break;
		case CItemData::ITEM_POTION:
			strTemp.Format(_T("[%d] ����"), iTypeIndex);
			break;
		}		
		listctrl.SetItemText(lvitem.iItem, COLUMN_ITEM_TYPE, strTemp);			// Type
		strTemp.Format(_T("%d"), iSubTypeIndex);
		listctrl.SetItemText(lvitem.iItem, COLUMN_ITEM_SUB_TYPE, strTemp);			// Sub Type		
		strTemp.Format(_T("%d"), iLevel);
		listctrl.SetItemText(lvitem.iItem, COLUMN_ITEM_LEVEL, strTemp);			// Level		
		strTemp.Format(_T("%d"), iPrice);
		listctrl.SetItemText(lvitem.iItem, COLUMN_ITEM_PRICE, strTemp);			// Price

		for( int t = 0; t < MAX_COUNTRY; ++t )
		{
			if (!ValidCountry(t))
				continue;
			
			BOOL bTrue = iNational & ( 1 << t );
			strTemp.Format(_T("%s"), bTrue ? "�Ⱦ���" : "���" );
			listctrl.SetItemText(lvitem.iItem, GetColumnCount(t), strTemp);
		}


		if(!bTotal)
		{
			SellItems TempItem;
			TempItem.iIndex		= iItemIdx;
			TempItem.lNational	= iNational;
			m_vectorSellItems.push_back(TempItem);
		}
	}
	while(cmd.MoveNext());
}

// �Ǹ� ������ �����մϴ�.
BOOL CMerchantDialog::SetTradeRate(int iSellRate, int iBuyRate)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CString strQuery = _T("");
	
	// �̺�Ʈ �����۰� ����Ʈ �����۰� ���� ������ ���.
	strQuery.Format(_T("UPDATE t_shop SET a_sell_rate = %d, a_buy_rate = %d WHERE a_keeper_idx = %d"), iSellRate, iBuyRate, m_iSelectedShopper);
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("Cannot Update!!!"));
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: NPC(����)�� �Ǹ��ϰ� �ִ� ������ ����� ������.
// Input  : iMerchantIdx - 
//-----------------------------------------------------------------------------
void CMerchantDialog::RefreshItemListOfNPC(int iMerchantIdx)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");
	strQuery.Format(_T("SELECT * FROM t_shopitem WHERE a_keeper_idx = %d ORDER BY a_item_idx"), iMerchantIdx);
	
	m_ctlItemListOfNPC.DeleteAllItems();
	m_vectorSellItems.clear();
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Open())
	{
		AfxMessageBox(_T("Cannot open table"));
		return;
	}
	if (!cmd.MoveFirst())		return;

	//--------------------------------------------------------
	// ������ �߰��ϱ�...
	//--------------------------------------------------------
	//	LV_ITEM lvitem;
	do
	{
		int		iIndex			= -1;		
		int		iNational		= 0;
		CString	strTemp			= _T("");
		cmd.GetRec("a_item_idx",	iIndex);
		cmd.GetRec("a_national",	iNational);
		
		//--------------------------------------------------------
		// �ε����� �ش��ϴ� ������ ������ �� ����ϱ�.
		//--------------------------------------------------------
		// �Ǹ��ϴ� ��Ͽ� �߰�.
		RefreshItemList(m_ctlItemListOfNPC, FALSE, ZONEALL, iIndex, iNational);
	}
	while(cmd.MoveNext());
}

//--------------------------------------------------------
// �������� �̵�...
// ��ü ��Ͽ��� -> NPC ������...
//--------------------------------------------------------
void CMerchantDialog::OnItemToNpc() 
{
	// TODO: Add your control notification handler code here	
	int iSelectedCount		= m_ctlItemList.GetSelectedCount();
	POSITION posSelected	= m_ctlItemList.GetFirstSelectedItemPosition();	
	if (posSelected == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (posSelected)
		{
			CString strTemp	= _T("");
			int nItem		= m_ctlItemList.GetNextSelectedItem(posSelected);
			strTemp			= m_ctlItemList.GetItemText(nItem, 0);
			{
				// �ߺ��Ǵ� ���� �߰����� ����.
				LVFINDINFO lvfindInfo;
				lvfindInfo.flags	= LVFI_PARTIAL|LVFI_STRING;
				lvfindInfo.psz		= strTemp;
				int iValue			= atoi(strTemp);

				// �Ǹ��ϴ� ��Ͽ� �߰�.
				SellItems TempItem;
				TempItem.iIndex		= iValue;
				if(std::find_if(m_vectorSellItems.begin(), m_vectorSellItems.end(), FindItems(TempItem)) == m_vectorSellItems.end())
				{
					RefreshItemList(m_ctlItemListOfNPC, FALSE, ZONEALL, iValue);
				}
			}
		}
	}
}

void CMerchantDialog::OnItemToNpcAll() 
{
	// TODO: Add your control notification handler code here
	m_ctlItemListOfNPC.DeleteAllItems();
	m_vectorSellItems.clear();
	const int iCount		= m_ctlItemList.GetItemCount();
	for(int i = 0; i < iCount; ++i)
	{
		CString strTemp		= _T("");
		strTemp				= m_ctlItemList.GetItemText(i, 0);
		// �ߺ��Ǵ� ���� �߰����� ����.
		LVFINDINFO lvfindInfo;
		lvfindInfo.flags	= LVFI_PARTIAL|LVFI_STRING;
		lvfindInfo.psz		= strTemp;
		int iValue			= atoi(strTemp);

		// �Ǹ��ϴ� ��Ͽ� �߰�
		SellItems TempItem;
		TempItem.iIndex	= iValue;
		if(std::find_if(m_vectorSellItems.begin(), m_vectorSellItems.end(), FindItems(TempItem)) == m_vectorSellItems.end())
		{
			RefreshItemList(m_ctlItemListOfNPC, FALSE, ZONEALL, iValue);
		}
	}	
}

//--------------------------------------------------------
// �������� ����...
// NPC ��Ͽ��� �������� ������.
//--------------------------------------------------------
void CMerchantDialog::OnItemFromNpc() 
{
	// TODO: Add your control notification handler code here	
	int iSelectedCount		= m_ctlItemListOfNPC.GetSelectedCount();
	POSITION posSelected	= m_ctlItemListOfNPC.GetFirstSelectedItemPosition();	
	if (posSelected == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (posSelected)
		{
			CString strTemp	= _T("");
			int nItem		= m_ctlItemListOfNPC.GetNextSelectedItem(posSelected);
			strTemp			= m_ctlItemListOfNPC.GetItemText(nItem, 0);
			int iValue		= atoi(strTemp);
			m_ctlItemListOfNPC.DeleteItem(nItem);
			int iSize = m_vectorSellItems.size();
			SellItems TempItem;
			TempItem.iIndex	= iValue;
			std::vector<SellItems>::iterator iter = std::find_if(m_vectorSellItems.begin(), m_vectorSellItems.end(), FindItems(TempItem));			
			if(iter != m_vectorSellItems.end())
			{
				m_vectorSellItems.erase(iter);
			}
			iSize = m_vectorSellItems.size();
			m_ctlItemListOfNPC.RedrawItems(nItem, nItem);
		}
	}
}

void CMerchantDialog::OnItemFromNpcAll() 
{
	// TODO: Add your control notification handler code here
	m_ctlItemListOfNPC.DeleteAllItems();
	m_vectorSellItems.clear();
}

void CMerchantDialog::OnSelchangeFindByType() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();
}

void CMerchantDialog::OnSelchangeFindBySubtype() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();
}

void CMerchantDialog::OnSelchangeFindByRace() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();
}

//-----------------------------------------------------------------------------
// Purpose: ����Ʈ ��Ʈ�ѿ� �ִ� ������� ��� DB�� �߰���.
// Input  : listctrl - 
//-----------------------------------------------------------------------------
BOOL CMerchantDialog::UpdateItemListOfNPC( int iMerchantIdx, int iItemIdx, int iNational )
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();	

	CString strQuery = _T("");
	strQuery.Format(
		"INSERT INTO t_shopitem"
		"(a_keeper_idx, a_item_idx, a_national)"
		" VALUES ("
		"%d, %d, %ld)"
		, iMerchantIdx
		, iItemIdx
		, iNational
		);
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("Cannot Insert!!!"));
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// Purpose: ������ �Ǹ��ϰ� 
// Input  : iMerchantIdx - ���� NPC Index
// Output : Returns TRUE on success, FALSE on failure.
//-----------------------------------------------------------------------------
BOOL CMerchantDialog::DeleteItemListOfNPC(int iMerchantIdx)
{
	ASSERT(theApp.m_pDBConn != NULL && "Invalid DB Connection");
	CWorldEditorDoc *pDoc			= theApp.GetDocument();
	CString strQuery = _T("");
	strQuery.Format(
		"DELETE FROM t_shopitem"
		" WHERE a_keeper_idx=%d",
		iMerchantIdx);

	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Update())
	{
		AfxMessageBox(_T("Cannot Delete"));
		return FALSE;
	}
	return TRUE;
}

//--------------------------------------------------------
// DB ������ �����ϴ� �κ�.
//--------------------------------------------------------
void CMerchantDialog::OnMerchantUpdate() 
{
	// TODO: Add your control notification handler code here
	DeleteItemListOfNPC(m_iSelectedShopper);

	UpdateData(TRUE);
	SetTradeRate(m_iSellRate, m_iBuyRate);

	std::vector<SellItems>::iterator it;
	std::vector<SellItems>::const_iterator end = m_vectorSellItems.end();
	for( it = m_vectorSellItems.begin(); it != end; ++it )
	{
		// ����Ʈ ��Ʈ�ѿ� �ִ� ������ ����� DB�� ������Ʈ ��.
		UpdateItemListOfNPC( m_iSelectedShopper, (*it).iIndex, (*it).lNational );
	}
}

void CMerchantDialog::OnMerchantRefresh() 
{
	// TODO: Add your control notification handler code here
	// ���� ���� ���� �޴�.
	m_ctlItemList.DeleteAllItems();
	RefreshItemList(m_ctlItemList, TRUE, ZONEALL, -1, 0, 
	m_ctlFindByType.GetCurSel() - 1, -1, -1);
}

//-----------------------------------------------------------------------------
// Purpose: SHOP�� ���� ������ ȭ�Ϸ� �ۼ��մϴ�.
//-----------------------------------------------------------------------------
void CMerchantDialog::OnMerchantMakefile() 
{
	// TODO: Add your control notification handler code here
	CMakeFileDlg	Dlg;
	Dlg.DoModal();	
}
void CMerchantDialog::OnTitan() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();
}

void CMerchantDialog::OnKnight() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();	
}

void CMerchantDialog::OnHealer() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();	
}

void CMerchantDialog::OnMage() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();	
}

void CMerchantDialog::OnRogue() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();	
}

void CMerchantDialog::OnSorcerer() 
{
	// TODO: Add your control notification handler code here
	OnMerchantRefresh();	
}

void CMerchantDialog::OnItemchangedItemListSell(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	GetNationalFlag();
}

// �÷��� ���� �����մϴ�.
void CMerchantDialog::SetNationalFlag()
{
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COUNTRY);
	CButton* pBtn		= static_cast<CButton*>(GetDlgItem(IDC_CHECK2));
	if(pCB->GetCurSel() < 0)
	{
		AfxMessageBox(_T("Select Country"));
		pBtn->SetCheck(!pBtn->GetCheck());
		return;
	}

	int iSelectedCount		= m_ctlItemListOfNPC.GetSelectedCount();
	POSITION posSelected	= m_ctlItemListOfNPC.GetFirstSelectedItemPosition();	
	if (posSelected == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (posSelected)
		{
			CString strTemp	= _T("");
			int nItem		= m_ctlItemListOfNPC.GetNextSelectedItem(posSelected);
			strTemp			= m_ctlItemListOfNPC.GetItemText(nItem, 0);
			int iValue		= atoi(strTemp);			
			SellItems TempItem;
			TempItem.iIndex	= iValue;
			std::vector<SellItems>::iterator iter = std::find_if(m_vectorSellItems.begin(), m_vectorSellItems.end(), FindItems(TempItem));
			if(iter != m_vectorSellItems.end())
			{
				const BOOL bChecked	= pBtn->GetCheck();

				strTemp.Format(_T("%s"), bChecked ? "�Ⱦ���" : "���" );

				if(pCB->GetCurSel() == MAX_COUNTRY)
				{
					for(int i=0; i<MAX_COUNTRY; i++)
					{
						m_ctlItemListOfNPC.SetItemText(nItem, GetColumnCount(i), strTemp);

						if(bChecked)
							(*iter).lNational |= 1 << i;
						else
							(*iter).lNational ^= 1 << i;
					}
				}
				else if(ValidCountry(pCB->GetCurSel()))
				{	
					m_ctlItemListOfNPC.SetItemText(nItem, GetColumnCount(pCB->GetCurSel()), strTemp);

					if(bChecked)
						(*iter).lNational |= (1 << pCB->GetCurSel());
					else
						(*iter).lNational ^= (1 << pCB->GetCurSel());
				}
			}
		}
	}
}

// �÷��� ���� ����ϴ�.
void CMerchantDialog::GetNationalFlag()
{
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_COUNTRY);
	CButton* pBtn		= static_cast<CButton*>(GetDlgItem(IDC_CHECK2));
	if(pCB->GetCurSel() < 0)
	{
		pBtn->SetCheck(FALSE);
		return;
	}

	if(!ValidCountry(pCB->GetCurSel()))
	{
		pBtn->SetCheck(FALSE);
		pBtn->EnableWindow(FALSE);
		return;
	}
	else
	{
		pBtn->EnableWindow(TRUE);
	}

	int iSelectedCount		= m_ctlItemListOfNPC.GetSelectedCount();
	POSITION posSelected	= m_ctlItemListOfNPC.GetFirstSelectedItemPosition();	
	if (posSelected == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (posSelected)
		{
			CString strTemp	= _T("");
			int nItem		= m_ctlItemListOfNPC.GetNextSelectedItem(posSelected);
			strTemp			= m_ctlItemListOfNPC.GetItemText(nItem, 0);
			int iValue		= atoi(strTemp);			
			SellItems TempItem;
			TempItem.iIndex	= iValue;
			std::vector<SellItems>::iterator iter = std::find_if(m_vectorSellItems.begin(), m_vectorSellItems.end(), FindItems(TempItem));
			if(iter != m_vectorSellItems.end())
			{
				const BOOL bChecked	= (*iter).lNational & (1 << pCB->GetCurSel());
				pBtn->SetCheck(bChecked);
			}
		}
	}
}

void CMerchantDialog::OnCheck2() 
{
	// TODO: Add your control notification handler code here
	SetNationalFlag();	
}

void CMerchantDialog::OnChangeCountry()
{
	// TODO: Add your control notification handler code here
	GetNationalFlag();
}

void CMerchantDialog::OnCopyCountry()
{
	CComboBox* pCBCopyFrom = (CComboBox*)GetDlgItem(IDC_COPYFROM);
	CComboBox* pCBCopyTo = (CComboBox*)GetDlgItem(IDC_COPYTO);

	if(pCBCopyFrom->GetCurSel() < 0 || pCBCopyTo->GetCurSel() < 0)
	{
		AfxMessageBox(_T("Select Country!"));
		return;
	}

	if(!ValidCountry(pCBCopyFrom->GetCurSel()) || !ValidCountry(pCBCopyTo->GetCurSel()))
	{
		AfxMessageBox(_T("Invalid Country!"));
		return;
	}

	std::vector<SellItems>::iterator it;
	BOOL bChecked;
	CString strTemp	= _T("");
	int nCnt = 0;

	for( nCnt = 0, it = m_vectorSellItems.begin(); it != m_vectorSellItems.end(); ++it, ++nCnt )
	{

		bChecked = (*it).lNational & (1 << pCBCopyFrom->GetCurSel());
		strTemp.Format(_T("%s"), bChecked ? "�Ⱦ���" : "���" );

		m_ctlItemListOfNPC.SetItemText(nCnt, GetColumnCount(pCBCopyTo->GetCurSel()), strTemp);

		if( ((*it).lNational & (1 << pCBCopyFrom->GetCurSel())) != ((*it).lNational & (1 << pCBCopyTo->GetCurSel())) )
		{
			if(bChecked)
				(*it).lNational |= (1 << pCBCopyTo->GetCurSel());
			else
				(*it).lNational ^= (1 << pCBCopyTo->GetCurSel());

		}
	}
}