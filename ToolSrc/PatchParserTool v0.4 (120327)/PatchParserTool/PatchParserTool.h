
// PatchParserTool.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CPatchParserToolApp:
// �� Ŭ������ ������ ���ؼ��� PatchParserTool.cpp�� �����Ͻʽÿ�.
//

class CPatchParserToolApp : public CWinApp
{
public:
	CPatchParserToolApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CPatchParserToolApp theApp;