
// UITool.h : UITool ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

#include <queue>
#include <Engine/Interface/UIManager.h>


// CUIToolApp:
// �� Ŭ������ ������ ���ؼ��� UITool.cpp�� �����Ͻʽÿ�.
//

class CUIToolApp : public CWinAppEx
{
public:
	CUIToolApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	BOOL SubInitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();
	virtual int Run();

	void initDraw(HWND hWnd);

	void addQueue(Command* pCmd);

	std::string getLoadFileName()	{ return m_strLoadFileName;	}
	void		setSaveFileName(std::string strPath)	{ m_strLoadFileName = strPath;	}
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
	CFontData *m_pfdDefaultFont;

private:
	typedef std::queue<Command*>	cmd_queue;

	void Update();
	void Draw();

	CDrawPort*	m_pDrawPort;
	CViewPort*	m_pViewPort;

	CUIManager*	m_pUIMgr;

	cmd_queue	m_queue;
	ULONG		m_oldTime;

	std::string m_strLoadFileName;
	std::string m_strLoadFilePath;
	CTCriticalSection m_csLock;
		
public:
	afx_msg void OnFileOpen();
	afx_msg void OnRefresh();
	afx_msg void OnFileSave();
};

extern CUIToolApp theApp;
