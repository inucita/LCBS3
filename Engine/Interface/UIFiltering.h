

#ifndef _UIFILTERING_H_
#define _UIFILTERING_H_

// Define ...
#define MAX_SPACE_ARRAY_NUM		80
#define MAX_STR_LENGTH          64

// Date : 2005-01-28,   By Lee Ki-hwan
// #include <string>
// #include <vector>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIListBox.h>
using namespace std;


//------------------------------------------------------------------------------
// _tagFILTER 
// Explain:  
// Date : 2005-01-12,Author: Lee Ki-hwan
// History: �ӵ��� ���� ���ڿ��� ���̵� �Բ� ����
//			050128 : ��Ʈ�� ����� std::string �� ����
//------------------------------------------------------------------------------
typedef struct _tagFILTER 
{
	string strString;
	string strReword;
	
	int		nString;
	int		nReword;

	//------------------------------------------------------------------------------
	// SFILTER
	// Explain: ������~
	// Date : 2005-01-28(���� 7:31:52) Lee Ki-hwan
	//------------------------------------------------------------------------------
	_tagFILTER ( const char* _szString = NULL, const char* _szReword = NULL )
	{
		strString = _szString;
		strReword = _szReword;

		nString = strString.length();
		nReword = strReword.length();
	}

} FILTER;

typedef vector<FILTER> FILTER_VECTOR;

//------------------------------------------------------------------------------
// CUIFiltering
// Explain:  
// Date : 2004-12-01,Author: Lee Ki-hwan
// History: 
//		Date : 2005-01-12,   By Lee Ki-hwan : c ������ �ҽ��� C++���·� �ٲ�
//------------------------------------------------------------------------------
class CUIFiltering
{
	FILTER_VECTOR	m_vList;	

	char	m_strRefineWord[MAX_STR_LENGTH];
	int		m_nListCount;
	char	m_strFileName[1024];
	bool	m_bCharacterfilter;

public :
	CUIFiltering()		{ Clear();		}	
	~CUIFiltering()		{ Destroy();	}

	void Clear();
	BOOL Create( char* file_name , bool charfilter = false);
	void Destroy();
	
	BOOL Filtering( char* syntax );
	BOOL Filtering( char* syntax, int* szReturn );
	
	void EnDecoding( char* szResult, const char* szData );

	void MySort_ForFilter(FILTER *ft, int n);

	char* GetString( int nIndex ) const;
	char* GetKey() const;
	char* GetRefineWord() const;
	
	void SetFileName( char* szData );
	char* GetFileName() const;
	int GetListCount()	{ return m_nListCount; }
};

extern CUIFiltering _UIFiltering;
extern CUIFiltering _UIFilteringCharacter;	// Date : 2005-02-16,   By Lee Ki-hwan : ĳ���� ������ ĳ���� ���͸�

//[ttos_2009_7_17]: CHARATER_CHAT_FILTER ĳ���� ä�� ����
//******************************************************
extern CUIFiltering _UICharacterChatFilter;	

//[ttos_2009_7_15]:�ɸ��� ä�� ����
class CUIChatFilter : public CUIWindow
{
protected:
	CTextureData* m_ptdButtonTexture;
	// surface.
	CUIRectSurface			m_rtsBase;
	// title rect.
	UIRect					m_rtTitle;
	
	CUIButton	m_btnAddChar;
	CUIButton	m_btnDelChar;
	CUIButton	m_btnClose;

	CTString	m_strSelName;
	int			m_nSelIndex;

	CUIListBox	m_lbCharName;

public:
	CUIChatFilter();
	~CUIChatFilter();
	
	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void OpenChatFilter();
	void CloseChatFilter();
	void Render();
	void ListUpdate();
	void SaveFile();
	void AddCharName(char* strCharName);
	void DelCharName();
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void Message( int nCommandCode, CTString strTitle, CTString strMessage, DWORD dwStyle );
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	void CharacterFilterInSimplePopup( CTString strCharName );
};
//*********************************************************
#endif