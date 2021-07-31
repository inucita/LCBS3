
// ----------------------------------------------------------------------------
//  File : UIMultiEditBox.h
//  Desc : Created by yjpark
//	Last Editor : // Date : 2005-01-11,   By Lee Ki-hwan	
// ----------------------------------------------------------------------------

#ifndef	UIMULTIEDITBOX_H_
#define	UIMULTIEDITBOX_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIIME.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIScrollBar.h>
#include <vector>

// ----------------------------------------------------------------------------
// Name : CUIMultiEditBox
// Desc : Multi EditBox Control
//      - �̹� ������ EditBox�� �̿��ؼ� MultiEditBox ����
//		- ���� ���� Ȯ�� ( Use Vector, Scroll Bar )
//      - ���� �ִ� ���μ��� �Ѿ� ���� NewLine.
//		- �Խ��ǿ� �۾�������� �ʱ� �ۼ� �Ǿ���.
// ----------------------------------------------------------------------------
class CUIMultiEditBox : public CUIWindow
{
protected:
	// controls
	CUIEditBox				m_ebEditBox;	// EditBox
	CUIScrollBar			m_sbScrollBar;	// Scroll Bar	

	// Properties
	std::vector<CTString>	m_strTexts;		// ȭ�鿡 ��µǴ� �ؽ�Ʈ
	int						m_nCurrentLine;	// Current Line
	int						m_nMaxLine;
	int						m_nFontWidth;	// Font Width	( Use Set m_nMaxChar and Render )
	int						m_nFontHeight;	// Font Height  ( Use Set m_nLineHeight and Render )
	int						m_nMaxChar;		// Max Charactor Count
	int						m_nLineHeight;	// Max Line Height
	int						m_nBlankSpaceLeft;
	int						m_nBlankSpaceTop;
	BOOL					m_bNotEditBox;
	ENGINE_API static int	s_nRefCount;

protected:
	void	IncRef()			{ s_nRefCount = 1;	}
	void	DecRef()			{ s_nRefCount = 0;	}
	void	AutoRef( BOOL ref )	{ ref ? IncRef() : DecRef();	}
public:
	CUIMultiEditBox();
	virtual ~CUIMultiEditBox();

	// Create & destroy
	virtual void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight = 10 );
	virtual void Destroy();
	
	virtual void Render();
		
	// Messages
	WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT CharMessage( MSG *pMsg );
	WMSG_RESULT	IMEMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Window 
	virtual void SetFocus( BOOL bVisible );
	BOOL IsFocused();
	
	// String
	CTString GetString();
	CTString GetString( unsigned int nIndex );
	int GetAllStringLength();
	void ResetString();
	void SetString( char* strString, char* strHead = NULL );
	void SetString( char* strString, char* Writer, char* strHead = NULL ); // ���� ���ÿ�
		
	// String Control
	void SetString ( char* strString, int nPos );
	void AddString( char* strString, int nPos = -1 );
	void DeleteString( int nPos = -1 );
	BOOL AddEOLTokenString( CTString& strString, char* pstrToken );
	BOOL AddEOLTokenString( char* pstrToken );
	BOOL RemoveEOLTokenString( CTString& strString, char* pstrToken );
	BOOL RemoveEOLTokenString( char *pstrToken );

	void SplitLine( int nIndex  = -1 );
	void Cutting();
	void PasteNextLine( int, BOOL t = false );
	void PasteNextLineKor( int nCurrentLine );	
	
	BOOL IsDBCS( char* strBuf, int nPos );
	
	// ScrollBar Control
	void ResizeScrollBar();
	void SetScrollBarPos();

	int GetLineCount()	{	 return m_strTexts.size();	}

	// Cursor Control
	void MoveCursorUp();
	void MoveCursorDown();
	void MoveCursorFirst();
	void MoveCursorEnd();
	
	// ���� ���� EditBox�� ��Ʈ�� ����
	void SetStringToEditBox();

	BOOL	IsFocusedInAll() { return s_nRefCount;	}
	void	SetEnableInput(BOOL benable) { m_bNotEditBox = benable; }
};


#endif	// UIMULTIEDITBOX_H_

