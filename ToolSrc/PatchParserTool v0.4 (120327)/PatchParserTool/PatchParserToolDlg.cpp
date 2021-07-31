
// PatchParserToolDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "PatchParserTool.h"
#include "PatchParserToolDlg.h"
#include "afxdialogex.h"

#include <string>
#include <fstream>

#include "../PatchParser2/SLogData.h"
#include "../PatchParser2/IParser.h"
#include "../PatchParser2/IPatcher.h"
#include "../SvnController/ISvnController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_RECENT 32


// CPatchParserToolDlg ��ȭ ����




CPatchParserToolDlg::CPatchParserToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPatchParserToolDlg::IDD, pParent)
	, urlMap()
	, url("")
	, filename("patch")
	, limit(10)
	, bMakeXml(false)
	, selectedRev()
	, loadedRev()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	urlMap["DEV"] = "svn://10.1.90.3/LastChaos/LastChaosClient";
	urlMap["KOR"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosKOR";
	urlMap["HKG"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosHK_EP2";
	urlMap["USA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosUSA";
	urlMap["BRZ"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosBRZ(EP2)";
	urlMap["THA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosThai";
	urlMap["GER"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosGER";
	urlMap["ESP"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosEspanol(Ep2)";
	urlMap["POL"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosPoland(Ep2)";
	urlMap["RUS"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosRUS";
	urlMap["TUR"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosTUR(EP2)";
	urlMap["FRA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosFRA(Ep2)";
	urlMap["MEX"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosMEX(EP2)";
	urlMap["ITA"] = "svn://10.1.90.3/LastChaos/LastChaosLocalClient/LastChaosITA(EP2)";
}

void CPatchParserToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, limit);
	DDX_Check(pDX, IDC_CHECK1, bMakeXml);
	DDX_Control(pDX, IDC_LIST1, NationList);
	DDX_Control(pDX, IDC_LIST2, RevisionList);
	DDX_Control(pDX, IDC_EDIT5, LogBox);
}

BOOL CPatchParserToolDlg::PreTranslateMessage(MSG* pMsg)
{
	/// limit editbox�� ��Ŀ���� ����� ���
	if (GetDlgItem(IDC_EDIT3) == GetFocus())
	{
		/// EnterKey �Է½�
		if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		{
			// refresh
			OnBnClickedButton2();
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CPatchParserToolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPatchParserToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPatchParserToolDlg::OnBnClickedButton2)
	ON_LBN_SELCHANGE(IDC_LIST1, &CPatchParserToolDlg::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST2, &CPatchParserToolDlg::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CPatchParserToolDlg �޽��� ó����

BOOL CPatchParserToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	/// map���� Nation�о�ͼ� AddString
	std::map<CString, CString>::const_iterator iter = urlMap.begin();
	while (iter != urlMap.end())
	{
		NationList.AddString(iter->first.GetString());
		++iter;
	}

	// recent ���� �ε�
	loadRecent();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CPatchParserToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CPatchParserToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPatchParserToolDlg::OnBnClickedButton1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	/// ���õ��� �ʾ������
	if (RevisionList.GetSelCount() <= 0)
		return;

	CFileDialog save(false, "*.bat", filename.GetBuffer(), OFN_OVERWRITEPROMPT, "bat Files(*.bat)|*.bat|");
	save.m_ofn.lpstrInitialDir = ".";

	if (save.DoModal() == IDOK)
	{
		UpdateData(true);

		/// Svn Command
		ISvnController* svn = getSvnController();
		std::string log = svn->log(url.GetBuffer(), true, true, selectedRev);
		std::string xml = svn->dispatchToString(log);

		/// log.Xml Parsing
		_LogDataList list;
		IParser* parser = getXmlParser();
		parser->parser(xml, list);

		/// Config
		SPatchConfig config;
		config.strSvnUrl = url;
		config.strFilePath = save.GetPathName();

		/// patch.bat Patching
		IPatcher* patcher = getBatchFilePatcher();
		patcher->patch(list, config);

		/// xml file creating
		if (bMakeXml)
		{
			std::string path = save.GetPathName();
			std::string ext = save.GetFileExt();

			path = path.substr(0, path.rfind(ext));
			path += "xml";

			std::ofstream f(path, std::ios_base::trunc);
			if (f.is_open())
			{
				f << xml;
				f.close();
			}
		}

		/// recent creating
		std::ofstream f("recent", std::ios_base::trunc);
		if (f.is_open())
		{
			filename = save.GetFileTitle();
			int size = selectedRev.size();

			f << NationList.GetCurSel() << " ";
			f << filename.GetBuffer() << " ";
			f << limit << " ";
			f << bMakeXml << " ";
			f << size << " ";

			for (int i = 0; i < size; ++i)
				f << selectedRev[i] << " ";

			f.close();
		}
	}
}

void CPatchParserToolDlg::OnBnClickedButton2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	refreshLog();
}

void CPatchParserToolDlg::OnLbnSelchangeList1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int curr = NationList.GetCurSel();

	CString nation;
	NationList.GetText(curr, nation);

	/// ���õǾ��ִ� url ���� �� ��
	CString prevcheck(url);
	url = urlMap[nation];

	/// refresh
	if (url != prevcheck)		// ������ ���� �����̸� skip
		refreshLog();
}

void CPatchParserToolDlg::OnLbnSelchangeList2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	// �ʱ�ȭ
	selectedRev.clear();

	// ���õ� revision count �о����
	int count = RevisionList.GetSelCount();

	/// nothing selected
	if (count <= 0)
		return;

	CString temp;
	int rev = -1;

	/// ���õ� revision list index �о����
	CArray<int> list;
	list.SetSize(count);
	RevisionList.GetSelItems(count, list.GetData());

	/// index�� ���� Ž���� list���� �о����
	selectedRev.clear();
	for (int i = 0; i < count; ++i)
	{
		int index = list[i];
		selectedRev.push_back(loadedRev[index]);
	}

	/// svn log
	ISvnController* svn = getSvnController();
	std::string log = svn->log(url.GetBuffer(), false, false, selectedRev);
	std::string xml = svn->dispatchToString(log);

	/// ���õ� revision log ǥ��
	CString logedit(xml.c_str());
	LogBox.SetWindowText(logedit.GetBuffer());
}

void CPatchParserToolDlg::loadRecent()
{
	std::ifstream f("recent", std::ios_base::in);

	if (!f.is_open())
		return;

	int nation = -1;
	int recentRevSize = -1;
	std::string name;
	std::vector<int> rev;

	f >> nation >> name >> limit >> bMakeXml;
	f >> recentRevSize;

	for (int i = 0; i < recentRevSize; ++i)
	{
		int r = -1;
		f >> r;
		rev.push_back(r);
	}

	f.close();

	UpdateData(false);

	/// nation list ���� -> revision list ���ΰ�ħ
	NationList.SetCurSel(nation);
	OnLbnSelchangeList1();

	// filename
	filename.SetString(name.c_str());

	/// ���õǾ��� revision�� �ִٸ�
	if (recentRevSize > 0)
	{
		// ���õ� revision list ����
		selectedRev = rev;

		/// ���õǾ��� revision�� ���� revision list���� ���� revision Ž��
		int loadRevSize = loadedRev.size();
		for (int i = 0; i < loadRevSize; ++i)
		{
			for (int j = 0; j < recentRevSize; ++j)
			{
				if (loadedRev[i] == selectedRev[j])
					RevisionList.SetSel(i);			// RevisionList ����
			}
		}

		// revision list ���� -> log box ���ΰ�ħ
		OnLbnSelchangeList2();
	}
}

void CPatchParserToolDlg::refreshLog()
{
	UpdateData(true);

	/// �ʱ�ȭ
	selectedRev.clear();			// ���õ� revision �ʱ�ȭ
	RevisionList.ResetContent();	// ǥ�õ� revision list �ʱ�ȭ
	LogBox.SetWindowText("");		// log box �ʱ�ȭ
	loadedRev.clear();			// Ž���� revision list �ʱ�ȭ

	/// ���ϴ� limit ��ŭ �о����
	ISvnController* svn = getSvnController();
	std::string log = svn->log(url.GetBuffer(), limit, true);
	std::string xml = svn->dispatchToString(log);

	/// revision list parsing
	_LogDataList list;
	IParser* parser = getXmlParser();
	parser->parser(CString(xml.c_str()).GetBuffer(), list);
	
	_LogDataIterator iter = list.begin();

	while (iter != list.end())
	{
		int rev = iter->revision;
		std::string msg = iter->msg;

		// Ž���� ���� revision list ���� ����
		loadedRev.push_back(rev);

		/// revision list ǥ��
		CString add;
		add.Format("%d %s", rev, msg.c_str());
		RevisionList.AddString(add);

		++iter;
	}
}
