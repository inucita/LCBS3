
// UIToolDoc.cpp : CUIToolDoc Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
#ifndef SHARED_HANDLERS
#include "UITool.h"
#endif

#include "UIToolDoc.h"
#include "MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CUIToolDoc

IMPLEMENT_DYNCREATE(CUIToolDoc, CDocument)

BEGIN_MESSAGE_MAP(CUIToolDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CUIToolDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CUIToolDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CUIToolDoc ����/�Ҹ�

CUIToolDoc::CUIToolDoc()
{
	// TODO: ���⿡ ��ȸ�� ���� �ڵ带 �߰��մϴ�.

}

CUIToolDoc::~CUIToolDoc()
{
}

BOOL CUIToolDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���⿡ ���ʱ�ȭ �ڵ带 �߰��մϴ�.
	// SDI ������ �� ������ �ٽ� ����մϴ�.

	return TRUE;
}

// CUIToolDoc serialization

void CUIToolDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	}
	else
	{
		// TODO: ���⿡ �ε� �ڵ带 �߰��մϴ�.
	}
}

#ifdef SHARED_HANDLERS

// ����� �׸��� �����մϴ�.
void CUIToolDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// ������ �����͸� �׸����� �� �ڵ带 �����Ͻʽÿ�.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// �˻� ó���⸦ �����մϴ�.
void CUIToolDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ������ �����Ϳ��� �˻� �������� �����մϴ�.
	// ������ �κ��� ";"�� ���еǾ�� �մϴ�.

	// ��: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CUIToolDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CUIToolDoc ����

#ifdef _DEBUG
void CUIToolDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUIToolDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CUIToolDoc ���


void CUIToolDoc::OnFileSave()
{
	// TODO: ���⿡ ��� ó���� �ڵ带 �߰��մϴ�.

	if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
	{
		if (pMain->GetPath().empty() == false)
		{
			pMain->OnSaveFile( (LPSTR)(LPCSTR)pMain->GetPath().c_str() );
			return;
		}
	}

	CFileDialog dlgFile(FALSE, NULL, (LPCTSTR)theApp.getLoadFileName().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString strFilter;
	strFilter += "UI Files(*.xml)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += "*.xml";
	strFilter += (TCHAR)'\0';   // last string

	dlgFile.m_ofn.lpstrTitle = "UI ���� ����";
	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrInitialDir = "";

	if( dlgFile.DoModal() == IDOK )
	{
		if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
		{
			CString strPath = dlgFile.GetPathName();
			
			//char szfilePath[_MAX_EXT];
			//_splitpath_s(szfilePath, 0, NULL, szfilePath, sizeof(szfilePath));
			pMain->OnSaveFile( (LPSTR)(LPCSTR)strPath );

			if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
			{
				std::string strTemp = strPath;
				std::size_t pos = strTemp.rfind(".xml");

				if (pos == std::string::npos)
					strTemp += ".xml";

				pMain->SetPath(strTemp);
			}

			theApp.setSaveFileName(dlgFile.GetFileTitle().GetString());	// ���� �̸� ����.
		}			
	}	
}


void CUIToolDoc::OnFileSaveAs()
{
	CFileDialog dlgFile(FALSE, NULL, (LPCTSTR)theApp.getLoadFileName().c_str(), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL, 0);

	CString strFilter;
	strFilter += "UI Files(*.xml)";
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += "*.xml";
	strFilter += (TCHAR)'\0';   // last string

	dlgFile.m_ofn.lpstrTitle = "UI ���� ����";
	dlgFile.m_ofn.lpstrFilter = strFilter;
	//dlgFile.m_ofn.lpstrInitialDir = "";

	if( dlgFile.DoModal() == IDOK )
	{
		if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
		{
			CString strPath = dlgFile.GetPathName();

			pMain->OnSaveFile( (LPSTR)(LPCSTR)strPath );
			
			if( CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd() )
			{
				std::string strTemp = strPath;
				std::size_t pos = strTemp.rfind(".xml");

				if (pos == std::string::npos)
					strTemp += ".xml";

				pMain->SetPath(strTemp);
			}

			theApp.setSaveFileName(dlgFile.GetFileTitle().GetString());	// ���� �̸� ����.
		}			
	}
}
