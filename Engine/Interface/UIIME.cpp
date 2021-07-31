#include "stdh.h"
#include <Engine/Interface/UIIME.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>

HKL			_hKL		= NULL;
BOOL		_bIMEProc	= false;

// ���� ���� ( �ϵ� �ڵ� )
DWORD _dwLocalLanguage	 =   LANG_KOREAN;		// �ѱ� 
							// LANG_CHT;		// �븸 
							// LANG_CHS;		// �߱� (PRC)
							// LANG_JAPANESE;	// �Ϻ�

// ----------------------------------------------------------------------------
// Name : GetNationName()
// Desc :
// ----------------------------------------------------------------------------
char* GetNationName ( DWORD dwNations )
{
	switch ( PRIMARYLANGID ( dwNations ) )
	{
	case LANG_CHINESE :
		switch ( SUBLANGID ( dwNations ) )
		{
		case SUBLANG_CHINESE_SIMPLIFIED :
			return "�߱�(PRC)";
							
		case SUBLANG_CHINESE_TRADITIONAL :
			return "�븸";
		}
		return "�߱�";

	case LANG_KOREAN :
		return "�ѱ�";

	case LANG_JAPANESE :
		return "�Ϻ�";
	}
	return "Unknow";
}


//------------------------------------------------------------------------------
// SetEngMode
// Explain: Ű���� �Է��� ����� ��ȯ �ϴ� �Լ�
// Date : 2005-01-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void SetEngMode ( HWND hWnd )
{
	HIMC hImc = ImmGetContext ( hWnd );
	DWORD dwConv, dwSent;
	DWORD dwTemp;

	ImmGetConversionStatus ( hImc, &dwConv, &dwSent );

	dwTemp = dwConv & ~IME_CMODE_LANGUAGE;
	dwTemp |= IME_CMODE_ALPHANUMERIC;
	
	dwConv = dwTemp;

	ImmSetConversionStatus ( hImc, dwConv, dwSent );
	ImmReleaseContext ( hWnd, hImc );
}