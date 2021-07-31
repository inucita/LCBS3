

// �̱�ȯ ���� �ۼ� ( 2004. 11. 11 ) 

#ifndef _UIIME_H_
#define _UIIME_H_

// #include <dimm.h>

#pragma comment ( lib, "imm32" )

// define ...
#define MAX_CANDLIST	10

// class Candidate
struct CCandList
{
    char	szCandidate[MAX_CANDLIST][256];
	int		nMaxLength;							// Maximum length of candidate list string
    char	*HoriCand;							// Candidate list string (for horizontal candidate window)
    int		nFirstSelected;						// First character position of the selected string in HoriCand
    int		nHoriSelectedLen;					// Length of the selected string in HoriCand
    DWORD	dwCount;							// Number of valid entries in the candidate list
    DWORD	dwSelection;						// Currently selected candidate entry relative to page top
    DWORD	dwPageSize;
	int		nReadingError;						// Index of the error character
    bool	bShowWindow;						// Whether the candidate list window is visible
    RECT	rcCandidate;						// Candidate rectangle computed and filled each time before rendered
	bool	bVerticalCand;						// ���� ������ Candidate			

	CCandList ()
	{
		memset ( szCandidate, 0, sizeof ( szCandidate ) );

		nMaxLength			= 0;
		HoriCand			= NULL;
		nFirstSelected		= 0;
		nHoriSelectedLen	= 0;
		dwCount				= 0;
		dwSelection			= 0;
		dwPageSize			= 0;
		nReadingError		= 0;
		bShowWindow			= false;
		bVerticalCand		= false;

		SetRectEmpty ( &rcCandidate );
	}

	~CCandList ()
	{
		
	}
};


// extern 
extern HKL			_hKL;				// keyboard Layout
extern DWORD		_dwLocalLanguage;	// ���� ��� ( �����Ͻ� �������� �ϵ� �ڵ� )
extern BOOL			_bIMEProc;


// ��� ���� �� ���� ����
#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)

inline DWORD GetPrimaryLang ()	{ return PRIMARYLANGID( LOWORD ( _hKL ) ); }
inline DWORD GetSubLang ()		{ return SUBLANGID( LOWORD( _hKL ) ); }
inline DWORD GetLanguage ()		{ return LOWORD( _hKL ); }

void SetEngMode ( HWND hWnd );

char*	GetNationName ( DWORD dwNations );

// IME ��� �� ���� �� ����
#define LANG_CHT            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL)
#define LANG_CHS            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED)
#define _CHT_HKL            ( (HKL)(INT_PTR)0xE0080404 ) // New Phonetic
#define _CHT_HKL2           ( (HKL)(INT_PTR)0xE0090404 ) // New Chang Jie
#define _CHS_HKL            ( (HKL)(INT_PTR)0xE00E0804 ) // MSPY
#define MAKEIMEVERSION( major, minor ) \
    ( (DWORD)( ( (BYTE)( major ) << 24 ) | ( (BYTE)( minor ) << 16 ) ) )


#endif // _UIIME_H_

// �̱�ȯ �ۼ� �� ( 2004. 11. 11 )