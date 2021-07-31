#include "stdh.h"
#include <Engine/Interface/UIMultiEditBox.h>
#include <Engine/Interface/UITextureManager.h>
#include <Engine/Network/Web.h>
#include <Engine/Help/Util_Help.h>

// External variables
extern HWND	_hwndMain;
extern INDEX g_iCountry;

#define WEB_NEXT_LINE	"\r\n"		// Use Web ( New Line Charactor )
#define MAX_MULTILINE	20


int	CUIMultiEditBox::s_nRefCount = 0;
extern CFontData *_pfdDefaultFont;

//------------------------------------------------------------------------------
// CUIMultiEditBox::CUIMultiEditBox
// Explain: Constructor 
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMultiEditBox::CUIMultiEditBox()
{
	// m_ebEditBox;				// Do Nothing 
	// m_sbScrollBar;			// Do Nothing 
	m_strTexts.clear();
	m_bNotEditBox		= FALSE;
	m_nCurrentLine		= 0;		
	m_nFontWidth		= 0;		
	m_nFontHeight		= 0;		
	m_nMaxChar			= 0;		
	m_nLineHeight		= 0;
	m_nBlankSpaceTop	= 0;
	m_nBlankSpaceLeft	= 0;
	m_nMaxLine			= MAX_MULTILINE;
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::~CUIMultiEditBox
// Explain: Destructor
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUIMultiEditBox::~CUIMultiEditBox()
{
	Destroy();
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::Destroy
// Explain: Destroy
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::Destroy() 
{
	// Do Nothing
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::Create
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int nLineHeight )
{
	// Basic Data Set
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_nFontHeight	= _pUIFontTexMgr->GetFontHeight() + _pUIFontTexMgr->GetLineSpacing();
	m_nFontWidth	= _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing();
	// [091104 sora]
	if (g_iCountry == RUSSIA)
		m_nMaxChar		= 255;	
	else
		m_nMaxChar		= ( nWidth / m_nFontWidth ) - 2;

	m_nLineHeight	= ( nHeight / m_nFontHeight );
	m_nMaxLine = nLineHeight;
	
	// Texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// Sctroll Bar
	m_sbScrollBar.Create( this, nWidth - 6, 0, 9, nHeight - 3 );
	m_sbScrollBar.CreateButtons( TRUE, 9, 7, 0, 0, 10 );
	m_sbScrollBar.SetScrollPos( 0 );
	m_sbScrollBar.SetScrollRange( m_nLineHeight );
	m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetItemsPerPage( m_nLineHeight );
	m_sbScrollBar.SetWheelRect ( -nWidth, -1, nWidth, nHeight );
	// UP Button
	m_sbScrollBar.SetUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_sbScrollBar.SetUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyUpUV( UBS_IDLE, UBS_DISABLE );
	// Down Button
	m_sbScrollBar.SetDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_sbScrollBar.SetDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_ON );
	m_sbScrollBar.CopyDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar Button
	m_sbScrollBar.SetBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_sbScrollBar.SetBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );

	// Edit Box
	m_ebEditBox.Create( this, m_nFontWidth / 2, 0, nWidth, 15, m_nMaxChar );
	m_ebEditBox.SetReadingWindowUV( 146, 46, 163, 62, fTexWidth, fTexHeight );
	m_ebEditBox.SetCandidateUV( 146, 46, 163, 62, fTexWidth, fTexHeight );

	// Blank Space
	m_nBlankSpaceTop	= ( m_ebEditBox.GetHeight() - _pUIFontTexMgr->GetFontHeight() ) / 2;	// ���ʿ� �ణ�� ������ 
	m_nBlankSpaceLeft	= m_nFontWidth / 2;														// ���� ������

	//!! Change AddString ( "" , 0 );
	/*for( int i =0; i < nLineHeight; i++ )
	{
		m_strTexts.push_back( CTString ( "" ) );
	}*/
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::Render
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::Render()
{
	int nX, nY;
	int nScrollBarPos	= m_sbScrollBar.GetScrollPos();	// ��ũ�� �� ��ġ 
	int nEndLine		= m_strTexts.size();

	GetAbsPos( nX, nY );
	
	// Blank Space 
	nX += m_nBlankSpaceLeft;
	nY += m_nBlankSpaceTop;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// ���ڿ� ���
	int	nShowLineLimit	= ( nEndLine < m_nLineHeight ) ? nEndLine : nScrollBarPos + m_nLineHeight;
	for( int i = nScrollBarPos; i < nShowLineLimit; i++ )
	{
		pDrawPort->PutTextEx( m_strTexts[i], nX, nY, 0xFFFFFFFF );
		nY += m_nFontHeight;
	}

	// Flush all render text queue
	pDrawPort->EndTextEx();

	// Render EditBox
	// EditBox�� ȭ�� �ȿ� ǥ�� �� �� ( m_nCurrentLine�� ��ġ�� �Ǵ� )
	if( nScrollBarPos <= m_nCurrentLine && nScrollBarPos + m_nLineHeight > m_nCurrentLine )
	{
//		int nMaxLine = nScrollBarPos + nEndLine;
//		if( nMaxLine >= m_nCurrentLine )
		{
			m_ebEditBox.Render ();
		}
	}
	
	// Set texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Render ScrollBar
	m_sbScrollBar.Render();
	
	// Reading window
	if( m_ebEditBox.DoesShowReadingWindow() )
	{
		// Reading window
		m_ebEditBox.RenderReadingWindow();
	}
	// Render all elements
	pDrawPort->FlushRenderingQueue();

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetFocus
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetFocus( BOOL bVisible )
{
	CUIBase::SetFocus( bVisible );
	m_ebEditBox.SetFocus( bVisible );
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::IsFocused
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
BOOL CUIMultiEditBox::IsFocused()
{
	BOOL bFocus = CUIBase::IsFocused();
	AutoRef( bFocus );
	return bFocus;
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::KeyMessage
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT	CUIMultiEditBox::KeyMessage( MSG *pMsg )
{
	switch( pMsg->wParam )
	{
	case VK_UP:
		MoveCursorUp();
		return WMSG_SUCCESS;
	
	case VK_DOWN:
		MoveCursorDown();
		return WMSG_SUCCESS;
	
	case VK_RETURN: // ������ ó���̳� �߰����� �Է��� ���� ������ ���������� �Է��� �� 
		{
			if ( ( (m_nCurrentLine + 1 ) < m_nMaxLine ) 
				&&  ( GetLineCount() < m_nMaxLine ) )		// ���μ��� ���ġ
			{
				if( m_strTexts[m_nCurrentLine].Length() <= m_ebEditBox.GetCursorIndex() )
				{
					m_nCurrentLine++;
					AddString( "", m_nCurrentLine );
					SetStringToEditBox();
					ResizeScrollBar();
					SetScrollBarPos();
				}
				else // ���� �߰� �̳� ó�� 
				{
					// ������ 2���� �и� �Ѵ�.
					SplitLine( m_ebEditBox.GetCursorIndex() );
					MoveCursorFirst();				
				}
			}
		}
		return WMSG_SUCCESS;

	case VK_BACK:
		if( m_ebEditBox.GetCursorIndex () == 0 )
		{
			if ( m_nCurrentLine == 0 ) return WMSG_SUCCESS;

			m_nCurrentLine--;

			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
			m_ebEditBox.SetCursorIndex ( m_strTexts[m_nCurrentLine].Length() );
			
			//WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			//m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();

			Cutting ();
			ResizeScrollBar ();
			SetScrollBarPos ();
			return WMSG_SUCCESS;

		}
		else
		{
			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			SetScrollBarPos ();
			return wmsgResult;
		}
		return WMSG_FAIL;

	case VK_DELETE :
		/*if ( m_strTexts[m_nCurrentLine].Length() <= 0 ) // ���� ������ ������ �ϳ��� ������
		{
			if ( m_nCurrentLine != 0 )
			{
				//������ ����
				m_strTexts.erase ( m_strTexts.begin() + m_nCurrentLine );
				
				m_nCurrentLine--;
								
				if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
				else
					m_ebEditBox.ResetString();

				m_ebEditBox.SetCursorIndex( 0 );
				
				ReSizeScrollBar ();
				SetScrollBarPos ();
			}
			else if ( m_nCurrentLine == 0 )
			{
				if ( m_strTexts.size () > 1 )
				{
					if ( m_strTexts[1].Length() > 0 ) 
						m_ebEditBox.SetString ( m_strTexts[1].str_String );
					else
						m_ebEditBox.ResetString();

					m_ebEditBox.SetCursorIndex( 0 );
					
					m_strTexts.erase ( m_strTexts.begin() );
										
					ReSizeScrollBar ();
					SetScrollBarPos ();

				}
				
			}
			return WMSG_SUCCESS;
		}
		else 
		*/if ( m_ebEditBox.GetCursorIndex() >= m_strTexts[m_nCurrentLine].Length() ) // ������ ��
		{
			if ( m_nCurrentLine >= m_strTexts.size() - 1 ) 	return WMSG_SUCCESS;
			
			if ( m_strTexts[m_nCurrentLine].Length() >= m_nMaxChar ) // ������ ���̶��
			{	
				// ���� ������ ó������ �ű�� 
				m_nCurrentLine++;
				
				if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
				else
					m_ebEditBox.ResetString();
				
				m_ebEditBox.SetCursorIndex ( 0 );

			//	WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			//	m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
				//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
				ResizeScrollBar ();
				SetScrollBarPos ();
				return WMSG_SUCCESS;
			}
			
			Cutting ();	// �ؿ��� ���� �÷��� �ڸ���.
			ResizeScrollBar ();
			SetScrollBarPos ();
			return WMSG_SUCCESS;

		}
		else // ������ �߰��̶��
		{
			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			
			return wmsgResult;
		}
		return WMSG_FAIL;

	case VK_LEFT :
		{
			if ( m_ebEditBox.GetCursorIndex() <= 0 )
			{
				if ( m_nCurrentLine <= 0 )
				{
					m_nCurrentLine = 0;
					WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
					m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
					SetScrollBarPos ();
					return wmsgResult;
				}
				else 
					m_nCurrentLine--;

				if ( !m_strTexts[m_nCurrentLine].Length() )
				{
					m_ebEditBox.ResetString ();
				}
				else
				{
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
					m_ebEditBox.SetCursorIndex ( m_strTexts[m_nCurrentLine].Length() );
				}

				if ( m_nCurrentLine < m_sbScrollBar.GetScrollPos() )
				{
					m_sbScrollBar.SetScrollPos ( m_nCurrentLine );
				}
				m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() ) * m_nFontHeight );
			
				return WMSG_SUCCESS;
			}

			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();

			if ( m_nCurrentLine < m_sbScrollBar.GetScrollPos() )
			{
				m_sbScrollBar.SetScrollPos ( m_nCurrentLine );
			}
			m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() ) * m_nFontHeight );
			return wmsgResult;
		}
	case VK_RIGHT : 
		{
			if ( m_ebEditBox.GetCursorIndex() >= m_strTexts[m_nCurrentLine].Length() ) // ������ ��
			{
				
				if ( m_nCurrentLine >= m_strTexts.size() - 1 )
				{
					m_nCurrentLine = m_strTexts.size() - 1;
					SetScrollBarPos ();
					return WMSG_SUCCESS;
				}
				else
				{
					m_nCurrentLine++;	
				}

				if ( !m_strTexts[m_nCurrentLine].Length() )
				{
					m_ebEditBox.ResetString ();
				}
				else
				{
					m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
					m_ebEditBox.SetCursorIndex ( 0 );
					
				}

				//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
				SetScrollBarPos ();
				return WMSG_SUCCESS;
			}

			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			SetScrollBarPos ();
			return wmsgResult;
		}
	default :
		{
			WMSG_RESULT wmsgResult = m_ebEditBox.KeyMessage ( pMsg );
			m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();
			SetScrollBarPos ();
			return wmsgResult;
		}
		
	}

	return WMSG_FAIL;	
}


// ----------------------------------------------------------------------------
// Name : CharMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUIMultiEditBox::CharMessage( MSG *pMsg )	
{ 

	// [091104 sora]
	if (g_iCountry == RUSSIA)
	{
		if ( UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, m_ebEditBox.GetString()) >= (m_nWidth-20) )
		{
			m_nCurrentLine++;
			m_strTexts.insert ( m_strTexts.begin() + m_nCurrentLine, "" );
			
			m_ebEditBox.ResetString ();
			SetScrollBarPos ();
		}
		else 
		{
			PasteNextLine( m_nCurrentLine );	
			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
		}
	}
	else
	{
		if ( m_ebEditBox.GetCursorIndex() >= m_nMaxChar && (m_nCurrentLine+1) < m_nMaxLine)
		{
			m_nCurrentLine++;
			m_strTexts.insert ( m_strTexts.begin() + m_nCurrentLine, "" );
		
			m_ebEditBox.ResetString ();
			SetScrollBarPos ();
		}
		else
		{
			PasteNextLine( m_nCurrentLine );	
			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
		}
	}

	WMSG_RESULT wmsgResult = m_ebEditBox.CharMessage ( pMsg );
	m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString();
	
	ResizeScrollBar();

	return wmsgResult;
		
}

// ----------------------------------------------------------------------------
// Name : IMEMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIMultiEditBox::IMEMessage( MSG *pMsg )	
{ 
	// IME Message������ �ٹٲ� ó���� ���� �ʾƵ� �ɰ� ����. Char Message���� ó��
	if ( m_ebEditBox.GetCursorIndex() >= m_nMaxChar -1 && (m_nCurrentLine+1) < m_nMaxLine) // ������ ��ġ 
	{
		m_nCurrentLine++;
		m_strTexts.insert ( m_strTexts.begin() + m_nCurrentLine, "" );
		//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
		m_ebEditBox.ResetString ();
		SetScrollBarPos ();
	}
	else if ( m_ebEditBox.GetCursorIndex() < m_nMaxChar - 3 )
	{
		PasteNextLineKor ( m_nCurrentLine );
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
	}

	WMSG_RESULT wmsgResult = m_ebEditBox.IMEMessage ( pMsg );
	m_strTexts[m_nCurrentLine] = m_ebEditBox.GetString ();

	if (wmsgResult != WMSG_FAIL && GetAllStringLength() >= 254)
	{
		m_ebEditBox.StopComposition();
	}

	//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
	SetScrollBarPos ();
	
	ResizeScrollBar();
	
	return wmsgResult;
}


// ----------------------------------------------------------------------------
// Name : !!MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIMultiEditBox::MouseMessage( MSG *pMsg )	
{ 
//	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL bTitleBarClick = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			
			if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
				return WMSG_SUCCESS;
			}
			else if (m_ebEditBox.MouseMessage ( pMsg ) != WMSG_FAIL ) 
				return WMSG_SUCCESS;
			
		}
		break;

	case WM_LBUTTONDOWN:
		{	
			if( IsInside( nX, nY ) )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
				}

				m_ebEditBox.MouseMessage ( pMsg );
				
				if( m_strTexts.size() > 0 )
				{
					ConvertToWindow ( nX, nY );
					m_ebEditBox.SetFocus ( TRUE );
					SetFocus( TRUE );

					m_nCurrentLine = nY / m_nFontHeight   + m_sbScrollBar.GetScrollPos();					
					 
					if ( m_nCurrentLine > m_strTexts.size() - 1 )
					{	
						m_nCurrentLine  = m_strTexts.size() - 1;
					}
					
					//Add: Su-won
					if( m_nCurrentLine > m_sbScrollBar.GetScrollPos() +m_sbScrollBar.GetItemsPerPage() )
					{
						m_ebEditBox.SetFocus ( FALSE);
						SetFocus( FALSE );
						break;
					}
					
					if ( m_strTexts[m_nCurrentLine].Length() > 0 ) 
						m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
					else
						m_ebEditBox.ResetString();
					
					m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );							

					int pos = nX / m_nFontWidth;

					if ( pos >= m_strTexts[m_nCurrentLine].Length() )
						m_ebEditBox.SetCursorIndex ( m_strTexts[m_nCurrentLine].Length() );
					
				}

				return WMSG_SUCCESS;
			}
			m_ebEditBox.SetFocus ( FALSE );
			SetFocus( FALSE );
		}
		break;
	case WM_LBUTTONUP:
		{
			if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
				{
//					if( m_nCurrentLine - m_sbScrollBar.GetScrollPos() > ( m_nLineHeight - 1 ) )
//					{
//						m_nCurrentLine	= m_sbScrollBar.GetScrollPos() + m_nLineHeight - 1;
//						SetStringToEditBox();
//					}
					m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
					return WMSG_SUCCESS;
				}
			}
		}
		break;
	}	
	return WMSG_FAIL;	
}


// ----------------------------------------------------------------------------
// Name : IsDBCS
// Desc : ���� ����Ʈ�� 2Byte ������ ù ���� ���� Ȯ��
// ----------------------------------------------------------------------------
BOOL CUIMultiEditBox::IsDBCS ( char* strBuf, int nPos )
{
    BOOL bRet = FALSE;
    for(int i = 0; i <= nPos; i++)
	{
        if(isascii(strBuf[i])==0)
        {
            if(nPos == i+1)
                bRet = TRUE;

            i++;
        }
    }
    return bRet;
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SplitLine
// Explain: ������ �ΰ��� �и��Ѵ�.
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SplitLine( int nIndex )
{
	CTString strTemp = m_strTexts[m_nCurrentLine];
	CTString strTemp2;
	
	int		nSplitPos;
	BOOL	b2ByteChar = FALSE;	

	if( nIndex == -1 ) // MaxCharactor���� �ڸ���.
	{
		// [091104 sora]
		if (g_iCountry == RUSSIA)
			nSplitPos = UTIL_HELP()->CheckNoFixedLength(_pfdDefaultFont, strTemp.str_String, m_nWidth-20);
		else
			nSplitPos = m_nMaxChar;
	}
	else
	{
		nSplitPos = nIndex;
	}

	// Check splitting position for 2 byte characters
	for( int iPos = 0; iPos < nSplitPos; iPos++ )
	{
		if( strTemp[iPos] & 0x80 )	
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		nSplitPos--;

	strTemp.Split( nSplitPos, strTemp, strTemp2 );
	
	//SetString ( strTemp.str_String, m_nCurrentLine );			--->
	m_strTexts[m_nCurrentLine]	= strTemp.str_String;
	//m_nCurrentLine++;
	if( nIndex ==-1)
		AddString ( strTemp2.str_String, m_nCurrentLine+1);
	else
		AddString ( strTemp2.str_String, ++m_nCurrentLine);
	
	SetStringToEditBox();
		
	ResizeScrollBar();
	SetScrollBarPos();
	
}


// ----------------------------------------------------------------------------
// Name : Cutting
// Desc : ������ ���� �÷��� max �Ʒ����� �ڸ���.
// ----------------------------------------------------------------------------
void  CUIMultiEditBox::Cutting ()
{
	int temp = m_ebEditBox.GetCursorIndex();

	if ( m_strTexts.size() > 1 + m_nCurrentLine )
	{
		// �� �������� ��ģ��.
		m_strTexts[m_nCurrentLine] += m_strTexts[m_nCurrentLine+1];
	
		// �Ʒ������� ����
		m_strTexts.erase ( m_strTexts.begin() + m_nCurrentLine + 1 );
	}
	// [091104 sora]
	if (g_iCountry == RUSSIA)
	{
		if ( UTIL_HELP()->GetNoFixedWidth(_pfdDefaultFont, m_strTexts[m_nCurrentLine].str_String) >= (m_nWidth-20) )
		{
			SplitLine ();	
			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
			m_ebEditBox.SetCursorIndex ( temp );
		}
		else
		{
			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
			m_ebEditBox.SetCursorIndex ( temp );
		}

	}
	else
	{
		if ( m_strTexts[m_nCurrentLine].Length() > m_nMaxChar )
		{
			SplitLine ();	
			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
			m_ebEditBox.SetCursorIndex ( temp );
		}
		else
		{
			m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
			m_ebEditBox.SetCursorIndex ( temp );
		}

	}
	
	SetScrollBarPos ();
//	m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
	//m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), m_nCurrentLine * m_nFontHeight );
}


// ----------------------------------------------------------------------------
// Name : PasteNextLine
// Desc : ���� ������ ���̸� �Ѵ� ���ڸ� ���� ������ �տ� ���δ�.
// ----------------------------------------------------------------------------	
void CUIMultiEditBox::PasteNextLine ( int nCurrentLine, BOOL bHangle )	
{
	CTString strString = m_strTexts[nCurrentLine].str_String;
	int		nLength		= strString.Length(); //���� ������ ����

	int		nMaxChar = m_nMaxChar; // MAX�� ������ ¦�� ���̿��� �Ѵ�.

	if ( nCurrentLine != m_nCurrentLine ) // ó�� ������ �ƴ϶�� 
	{
		if( nLength <= ++nMaxChar ) return;
	}
	else 
	{
		if( nLength <= nMaxChar ) return;
	}
	
	// Need multi-line
		
	// Check splitting position for 2 byte characters
	int	nSplitPos = nMaxChar-1;
 	
	if ( bHangle )
		nSplitPos--;
	

	BOOL	b2ByteChar	= FALSE;	

	for( int iPos = 0; iPos < nSplitPos; iPos++ )
	{
		if( strString[iPos] & 0x80 )
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		nSplitPos--;

	// Split string
	CTString	strTemp;
	
	strString.Split( nSplitPos,strString, strTemp );
	m_strTexts[nCurrentLine] = strString;

	/*****
	if ( m_strTexts.size() <= 1 + nCurrentLine ) return;

	m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];

	if ( nCurrentLine >= m_strTexts.size() - 1 ) // ������ �����̶��
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine, "" );
		SetScrollBarPos ();
	}
	*****/
	if ( nCurrentLine >= m_strTexts.size() - 1 ) // ������ �����̶��
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine+1, "" );
		m_strTexts[nCurrentLine+1] = strTemp;
		SetScrollBarPos ();
		return;
	}
	else
		m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];

	PasteNextLine ( nCurrentLine + 1 );
}


// ----------------------------------------------------------------------------
// Name : PasteNextLineKor
// Desc : ���� ������ ���̸� �Ѵ� ���ڸ� ���� ������ �տ� ���δ�.
// ----------------------------------------------------------------------------
void CUIMultiEditBox::PasteNextLineKor ( int nCurrentLine )	
{
	CTString strString	= m_strTexts[nCurrentLine].str_String;
	int		nLength		= strString.Length(); //���� ������ ����
	int		nMaxChar	= m_nMaxChar;

	if ( nCurrentLine != m_nCurrentLine ) // ó�� ������ �ƴ϶�� 
	{
		if( nLength <= ++nMaxChar -1 ) return;
	}
	else 
	{
		if( nLength <= nMaxChar - 2 ) return;
	}
	
	// Check splitting position for 2 byte characters
	int	nSplitPos = nMaxChar-2;
 	
	BOOL	b2ByteChar	= FALSE;	

	for( int iPos = 0; iPos < nSplitPos; iPos++ )
	{
		if( strString[iPos] & 0x80 )
			b2ByteChar = !b2ByteChar;
		else
			b2ByteChar = FALSE;
	}

	if( b2ByteChar )
		nSplitPos--;

	// Split string
	CTString	strTemp;
	
	strString.Split( nSplitPos,strString, strTemp );
	m_strTexts[nCurrentLine] = strString;
	
	/*****
	if ( m_strTexts.size() <= 1 + nCurrentLine ) return;
	
	m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];

	if ( nCurrentLine >= m_strTexts.size() - 1 ) // ������ �����̶��
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine, "" );
		SetScrollBarPos ();
		
	}
	*****/
	if ( nCurrentLine >= m_strTexts.size() - 1 ) // ������ �����̶��
	{
		m_strTexts.insert ( m_strTexts.begin() + nCurrentLine+1, "" );
		m_strTexts[nCurrentLine+1] = strTemp;
		SetScrollBarPos ();
		return;
	}
	else
		m_strTexts[nCurrentLine+1] = strTemp + m_strTexts[nCurrentLine+1];


	PasteNextLine ( nCurrentLine + 1 );
}


// ----------------------------------------------------------------------------
// Name : !!GetString()
// Desc :
// ----------------------------------------------------------------------------

CTString CUIMultiEditBox::GetString( unsigned int nIndex )
{
	CTString	strLine;

	if( nIndex < m_strTexts.size() )
		strLine	= m_strTexts[ nIndex ];

	if( nIndex < m_strTexts.size() - 1 )	// ������ ���� �ƴ϶�� WEB_NEXT_LINE ����
		AddEOLTokenString( strLine, WEB_NEXT_LINE );

	return strLine;
}

CTString CUIMultiEditBox::GetString ()
{
	CTString String;

	for ( int i = 0; i < m_strTexts.size(); i++ )
	{
		String	+= GetString( i );
	}

	return String;
}


// ----------------------------------------------------------------------------
// Name : !!ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMultiEditBox::ResetString ()
{
	m_nCurrentLine = 0;
	m_ebEditBox.ResetString();
	m_sbScrollBar.SetScrollPos ( 0 );
	m_sbScrollBar.SetCurItemCount ( 0 );
	m_strTexts.clear ();
	m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), 0 );
	m_strTexts.push_back ( CTString ( "" ) );
}


//------------------------------------------------------------------------------
// ResizeScrollBar
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::ResizeScrollBar()
{
	int nLineCount =  m_strTexts.size ();
	
	if( nLineCount > m_nLineHeight )
	{
		int nOverCount = nLineCount - m_nLineHeight; 
		
		if( m_sbScrollBar.GetScrollPos() > nOverCount )
		{
			m_sbScrollBar.SetScrollPos( nOverCount );
		}
		
		nOverCount++; // Index�� Count�� 1����
	}

	//m_sbScrollBar.SetCurItemCount( 0 );
	m_sbScrollBar.SetCurItemCount( nLineCount );
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetScrollBarPos
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetScrollBarPos()
{
	int nScrollPos = m_sbScrollBar.GetScrollPos();
	
	// ���� ����� ������ ��ũ�ѹ� ��ġ ���� ������ 
	if ( m_nCurrentLine < nScrollPos  ) // ����� ���� ���� ��ũ�� �� �̵� 
	{
		m_sbScrollBar.SetScrollPos ( m_nCurrentLine );
	}
	else if ( m_nCurrentLine >= m_nLineHeight + m_sbScrollBar.GetScrollPos() ) // ��ũ�� ���� ��ġ�� ���� �� ��. (�Ʒ��� �̵� )
	{
		int nDist = ( m_nCurrentLine + 1 ) - m_nLineHeight;
		m_sbScrollBar.SetScrollPos ( nDist );
	}
	else 
	{
		// ��, �Ʒ� ���������� ��ũ�ѹ��� ��ġ�� ������ �ʿ䰡 ����
	}
	
	m_ebEditBox.SetPos ( m_ebEditBox.GetPosX(), ( m_nCurrentLine - m_sbScrollBar.GetScrollPos() )* m_nFontHeight );
}


// ----------------------------------------------------------------------------
// Name : AddEOLTokenString()
// Desc : ��Ʈ�� ������ ��ġ�� ��ū�� �˻��ϰ� ���ٸ� ��ū�� �̾������.
// ----------------------------------------------------------------------------

BOOL CUIMultiEditBox::AddEOLTokenString( CTString& strString, char* pstrToken )
{
	if( pstrToken == NULL ) return FALSE;

	int		lenToken	= strlen( pstrToken );
	char*	pString		= strString.str_String + ( strString.Length() - lenToken );

	if( strncmp( pString, pstrToken, lenToken ) != 0 )
	{
		strString	+= pstrToken;
		return TRUE;
	}

	return FALSE;
}

BOOL CUIMultiEditBox::AddEOLTokenString( char* pstrToken )
{
	if( m_strTexts.size() <= 0 ) return FALSE;
	if( pstrToken == NULL ) return FALSE;

	BOOL	bAdded	= FALSE;

	for( int i = 0; i < m_strTexts.size() - 1; i++ )	// ������ ���� ����
	{
		bAdded	|= AddEOLTokenString( m_strTexts[i], pstrToken );
	}

	return bAdded;
}

// ----------------------------------------------------------------------------
// Name : RemoveEOLCRLFString()
// Desc : ��Ʈ�� ������ ��ġ�� ��ū�� �˻��ϰ� ��ū�� �ִٸ� �����Ѵ�.
// ----------------------------------------------------------------------------

BOOL CUIMultiEditBox::RemoveEOLTokenString( CTString& strString, char* pstrToken )
{
	if( pstrToken == NULL ) return FALSE;

	int		lenToken	= strlen( pstrToken );
	int		lenString	= strString.Length() - lenToken;
	char*	pString		= strString.str_String + lenString;

	if( strncmp( pString, pstrToken, lenToken ) == 0 )
	{
		strString.DeleteChar( lenString );
		return TRUE;
	}

	return FALSE;
}

BOOL CUIMultiEditBox::RemoveEOLTokenString( char *pstrToken )
{
	if( m_strTexts.size() <= 0 ) return FALSE;

	BOOL	bAdded	= FALSE;

	for( int i = 0; i < m_strTexts.size(); i++ )
	{
		bAdded	|= RemoveEOLTokenString( m_strTexts[i], pstrToken );
	}

	return bAdded;
}

// ----------------------------------------------------------------------------
// Name : !!ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMultiEditBox::SetString ( char* strString, char* strHead )
{
	if ( strString == NULL ) return;

	int			lenWebNextLine	= strlen( WEB_NEXT_LINE );
	char*		pString			= strString;
	CTString	tmpString;

	ResetString();
	m_strTexts.clear();		// ResetString()���� ""�� �߰��س��� �ִ�. �ٽ� ����

	if( strHead != NULL )
		tmpString		+= strHead;

	for( ; pString != NULL; pString++ )
	{
		BOOL	bCopy	= FALSE;
		BOOL	bEOS	= FALSE;

		if( strncmp( pString, WEB_NEXT_LINE, lenWebNextLine ) == 0 )
		{
			bCopy		= TRUE;
			AddEOLTokenString( tmpString, WEB_NEXT_LINE );
			pString++;
		}
		else if( *pString == '\0' )
		{
			bCopy		= TRUE;
			bEOS		= TRUE;
		}

		if( bCopy == TRUE )
		{
			int nMaxChar = m_nMaxChar;
			if ( tmpString.Length() > nMaxChar && tmpString.str_String[nMaxChar] & 0x80 )
				nMaxChar--;

			while( tmpString.Length() > nMaxChar )
			{
				CTString	splitString;
				CTString	srcString;

				tmpString.Split( nMaxChar, srcString, splitString );
				
				m_strTexts.push_back( srcString );
				tmpString	= splitString;
			}
			
			m_strTexts.push_back( tmpString );

			tmpString.Clear();
			if( strHead != NULL )
				tmpString	+= strHead;

			if( bEOS == TRUE )
				break;

			continue;
		}

		tmpString.InsertChar( tmpString.Length(), *pString );
	}
	
	ResizeScrollBar();
	SetScrollBarPos();
}


// ----------------------------------------------------------------------------
// Name : !!ResetString()
// Desc :
// ----------------------------------------------------------------------------
void CUIMultiEditBox::SetString ( char* strString, char* Writer, char* strHead )
{
	if ( strString == NULL ) return;

	int			lenWebNextLine	= strlen( WEB_NEXT_LINE );
	char*		pString			= strString;
	CTString	tmpString;

	ResetString();
	AddEOLTokenString( m_strTexts[ 0 ], WEB_NEXT_LINE );

	CTString strWriter = "----";
	strWriter += Writer;
	strWriter +=_S( 1939,  "���� ���� ���Դϴ� -----" );	
		
	m_strTexts.push_back ( strWriter );

	if( strHead != NULL )
		tmpString		+= strHead;

	for( ; pString != NULL; pString++ )
	{
		BOOL	bCopy	= FALSE;
		BOOL	bEOS	= FALSE;

		if( strncmp( pString, WEB_NEXT_LINE, lenWebNextLine ) == 0 )
		{
			bCopy		= TRUE;
			AddEOLTokenString( tmpString, WEB_NEXT_LINE );
			pString++;
		}
		else if( *pString == '\0' )
		{
			bCopy		= TRUE;
			bEOS		= TRUE;
		}

		if( bCopy == TRUE )
		{
			int nMaxChar = m_nMaxChar;
			if ( tmpString.Length() > nMaxChar && tmpString.str_String[nMaxChar] & 0x80 )
				nMaxChar--;

			while( tmpString.Length() > nMaxChar )
			{
				CTString	splitString;
				CTString	srcString;

				tmpString.Split( nMaxChar, srcString, splitString );
				
				m_strTexts.push_back( srcString );
				tmpString	= splitString;
			}
			
			m_strTexts.push_back( tmpString );

			tmpString.Clear();
			if( strHead != NULL )
				tmpString	+= strHead;

			if( bEOS == TRUE )
				break;

			continue;
		}

		tmpString.InsertChar( tmpString.Length(), *pString );
	}
	
	ResizeScrollBar();
	SetScrollBarPos();

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorUp
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorUp()
{
	int nCursorIndex = m_ebEditBox.GetCursorIndex();

	if( m_nCurrentLine <= 0)
	{
		m_nCurrentLine = 0;
		return;
	}
	
	m_nCurrentLine--;
	SetStringToEditBox ();
	SetScrollBarPos ();		// ��ũ�� ���� ��ġ ������
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorDown
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorDown()
{
	if ( m_nCurrentLine >= m_strTexts.size() - 1 )
	{
		m_nCurrentLine = m_strTexts.size() - 1;
		return;
	}

	m_nCurrentLine++;
	SetStringToEditBox ();
	SetScrollBarPos (); // ��ũ�� ���� ��ġ ������
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorFirst
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorFirst()
{
	m_ebEditBox.SetCursorIndex( 0 );	
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::MoveCursorEnd
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::MoveCursorEnd()
{
	m_ebEditBox.SetCursorIndex( m_strTexts[m_nCurrentLine].Length() );
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetStringToEditBox
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetStringToEditBox()
{
	if( m_nCurrentLine >= m_strTexts.size() )
	{
		m_nCurrentLine = m_strTexts.size() - 1;
		return;
	}

	int nCursorIndex = m_ebEditBox.GetCursorIndex();

	// ���� ���ڿ��� NULL�̸� EditBox�� ���� �� �� �������� �� �������� 
	if( 0 == m_strTexts[m_nCurrentLine].Length() ) 
	{
		m_ebEditBox.ResetString ();
	}
	else
	{
		m_ebEditBox.SetString ( m_strTexts[m_nCurrentLine].str_String );
		
		// ���� Ŀ���� ��ġ�� ���ڿ��� ���� ���� ũ�� Ŀ���� ���ڿ��� ���������� �ű�
		if ( m_strTexts[m_nCurrentLine].Length() <= m_ebEditBox.GetCursorIndex() )
		{
			MoveCursorEnd ();
		}
		else 
		{
			// �ѱ� 2Byte���� ���̿� Ŀ���� ��ġ ���� ��
			// �ѱ� ���ڿ� �� �� ù ����Ʈ�� Ȯ���ؼ� Double byte�̸� Ŀ���� ��ġ ���� ����
			if( IsDBCS( m_strTexts[m_nCurrentLine].str_String, nCursorIndex ) )
			{
				++nCursorIndex;
				m_ebEditBox.SetCursorIndex ( nCursorIndex );
			}
		}
	}
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::SetString
// Explain: 
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::SetString( char* strString, int nPos )
{
	

}


//------------------------------------------------------------------------------
// CUIMultiEditBox::AddString
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::AddString( char* strString, int nPos )
{
	if ( -1 == nPos ) // Max Line�� �߰� 
	{
		nPos = m_strTexts.size() - 1;
	}
	else if ( nPos > m_strTexts.size() )
	{
		return;
	}
	
	//m_strTexts.insert ( m_strTexts.begin() + nPos, CTString ( m_ebEditBox.GetString () ) );
	//m_strTexts[m_nCurrentLine] = strString;
	m_strTexts.insert ( m_strTexts.begin() + nPos, CTString ( strString) );
	if (!m_bNotEditBox)
	{
		m_ebEditBox.SetString( strString );
	}
		
	SetScrollBarPos();
}


//------------------------------------------------------------------------------
// CUIMultiEditBox::DeleteString
// Explain:  
// Date : 2005-01-11,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIMultiEditBox::DeleteString( int nPos )
{


}



//------------------------------------------------------------------------------
// CUIMultiEditBox::GetAllStringLength
// Explain: ���� �ڽ��� �ִ� ��� ���ڿ��� ���� ����
// Date : 2006-06-23,Author: Su-won
//------------------------------------------------------------------------------
int CUIMultiEditBox::GetAllStringLength()
{
	int nLength=0;
	for(int i=0; i<m_strTexts.size(); ++i)
	{
		nLength += m_strTexts[i].Length();
	}

	/*if (m_strTexts.size() > 1)
	{
		nLength += (strlen(WEB_NEXT_LINE) * (m_strTexts.size()-1));
	}*/
	
	return nLength;
}

