// MakeFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "MakeFileDlg.h"
#include <Engine/Entities/ShopData.h>
#include "dbcmd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMakeFileDlg dialog


CMakeFileDlg::CMakeFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMakeFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMakeFileDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMakeFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMakeFileDlg)
	DDX_Control(pDX, IDC_ZONENO, m_ctlZoneNo);
	DDX_Control(pDX, IDC_LANGUAGE, m_ctlLang);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMakeFileDlg, CDialog)
	//{{AFX_MSG_MAP(CMakeFileDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMakeFileDlg message handlers

BOOL CMakeFileDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctlLang.AddString( _T("��ü") );
	for( int i = 0; i < MAX_COUNTRY; ++i )
	{
		m_ctlLang.AddString(_aStrCountry[i]);
	}	
	m_ctlLang.SetCurSel(0);
	m_ctlZoneNo.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// FIXME : ���� �ȵ��...��.��
void CMakeFileDlg::OnOK() 
{
	CWorldEditorDoc *pDoc	= theApp.GetDocument();
	CString strMessage;		
	CString strQuery	= _T("");
	strQuery.Format(	
		"SELECT a_keeper_idx, a_name, a_sell_rate, a_buy_rate"
		" FROM t_shop"
		" ORDER by a_keeper_idx");
	
	CDBCmd cmd;
	cmd.Init(theApp.m_pDBConn);
	cmd.SetQuery(strQuery);
	if (!cmd.Open())
	{
		AfxMessageBox("Cannot open table");
		return ;
	}
	if (!cmd.MoveFirst())
	{
		strMessage.Format(_T("���� �����͸� ã�� �� �����ϴ�."));
		AfxMessageBox(strMessage);
		return ;
	}
	
	int iLanguage				= m_ctlLang.GetCurSel() - 1;		

	int			iShopCount		= cmd.m_nrecords;			// ������ ����.
	CShopData	*pShopData		= new CShopData[iShopCount];
	int			i				= 0;	
	int			iKeeperIdx		= -1;
	char		szShopName[50]	= {0};
	int			iSellRate		= 100;
	int			iBuyRate		= 100;
	do
	{
		cmd.GetRec("a_keeper_idx",		iKeeperIdx);
		cmd.GetRec("a_name",			szShopName);
		cmd.GetRec("a_sell_rate",		iSellRate);
		cmd.GetRec("a_buy_rate",		iBuyRate);

		pShopData[i].SetData(iKeeperIdx, szShopName, iSellRate, iBuyRate);
		i++;
	} while (cmd.MoveNext());
	//---------------------------------------------------------------------
	for(i = 0; i < iShopCount; i++)
	{
		const int iIndex			= pShopData[i].GetIndex();
		/*
		strQuery.Format("SELECT a_item_idx"
				" FROM t_shopitem WHERE a_keeper_idx = %d"
				" ORDER by a_item_idx", iIndex);
				*/

		strQuery.Format("SELECT t1.a_item_idx AS itemindex"
						" FROM t_shopitem t1, t_item t2 WHERE");

		if( iLanguage != -1 )
		{
			const ULONG lNational		= (1 << iLanguage);	
			CString strNational;
			strNational.Format(_T(" NOT (t1.a_national & %ld) AND"), lNational);
			strQuery += strNational;
		}

		CString strOrder;
		strOrder.Format(" t1.a_keeper_idx=%d AND t1.a_item_idx=t2.a_index"
			" ORDER BY t2.a_job_flag, t2.a_level, t2.a_type_idx, t2.a_subtype_idx, t2.a_index"
			, iIndex);		
		
		strQuery += strOrder;

		CDBCmd cmd;
		cmd.Init(theApp.m_pDBConn);
		cmd.SetQuery(strQuery);
		if (!cmd.Open())
		{
			AfxMessageBox(_T("Cannot open table"));
			continue;
		}
		if (!cmd.MoveFirst())
		{
			if(iIndex > 0)
			{
				CMobData* MD = CMobData::getData(iIndex);			
				strMessage.Format(_T("[%s] ������ �Ǹ��ϴ� ������ ����� �����ϴ�."), MD->GetName());
				AfxMessageBox(strMessage);
				continue;
			}
			else
			{
				strMessage.Format(_T("���� �ε����� �߸� �Ǿ��ų� �Ǹ��ϴ� �������� �����ϴ�. : %d"), iIndex);
				AfxMessageBox(strMessage);
				continue;
			}
		}

		pShopData[i].m_iNumOfItem		= cmd.m_nrecords;
		pShopData[i].m_vectorSellItems.resize(cmd.m_nrecords);

		int j = 0;
		do
		{
			cmd.GetRec("itemindex", pShopData[i].m_vectorSellItems[j]);
			j++;
		} while(cmd.MoveNext());
	}
	//---------------------------------------------------------------------
	CString strFilter	= _T("Lod Files(*.lod) | *.lod||");
	CString szFilename	= _T("");

	if((m_ctlZoneNo.GetCurSel() - 1) == -1)
	{
		szFilename.Format(_T("shopAll.lod"), m_ctlZoneNo.GetCurSel());
	}
	else
	{
		szFilename.Format(_T("shop%d.lod"), m_ctlZoneNo.GetCurSel());
	}

    CFileDialog fDlg(FALSE, NULL, szFilename,
			OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);
	
	const int iLastIndex = pShopData[iShopCount - 1].GetIndex();

	//---------------------------------------------------------------------
	// ���Ͽ� �����͸� ���� �κ�.
	//---------------------------------------------------------------------
	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		szFilename = fDlg.m_ofn.lpstrFile;

		if(!wFile.Open(szFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
			return;

		//wFile.Write(&iShopCount, sizeof(int));	// �Ѽ���
		wFile.Write(&iLastIndex, sizeof(int));		// ������ �ε���.
		
		for (int i = 0; i < iShopCount; i++)
		{
			int iIndex		= pShopData[i].GetIndex();
			int iSellRate	= pShopData[i].GetSellRate();
			int iBuyRate	= pShopData[i].GetBuyRate();
			int	len			= strlen(pShopData[i].GetShopName());
			int iNumOfItem	= pShopData[i].m_iNumOfItem;

			wFile.Write(&iIndex, sizeof(int));
			wFile.Write(&len, sizeof(int));
			wFile.Write(pShopData[i].GetShopName(), len);
			wFile.Write(&iSellRate, sizeof(int));
			wFile.Write(&iBuyRate, sizeof(int));			
			wFile.Write(&iNumOfItem, sizeof(int));
			for(int j = 0; j < iNumOfItem; j++)
			{
				int iItem	= pShopData[i].m_vectorSellItems[j];
				wFile.Write(&iItem, sizeof(int));
			}
		}

		wFile.Close();
    }

	if(pShopData)
	{
		delete [] pShopData;
		pShopData = NULL;
	}
	CDialog::OnOK();
}

void CMakeFileDlg::LoadShopData()
{
	/*
	CFileDialog fDlg(TRUE, NULL, szFilename,
			OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT, strFilter, NULL);
	
	const int iLastIndex = pShopData[iShopCount - 1].GetIndex();

	//---------------------------------------------------------------------
	// ���Ͽ� �����͸� ���� �κ�.
	//---------------------------------------------------------------------
	CFile wFile;
	if(fDlg.DoModal() == IDOK)
    {
		wFile.Close();
	}
	*/
}