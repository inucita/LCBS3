#include "stdh.h"
#include <Engine/Interface/UITrackPopup.h>
#include <Engine/Interface/UITextureManager.h>

extern INDEX g_iCountry;

// ����Ʈ �ڽ� ũ�⿡ �°� �ڽ��� �׸��� ��,�Ʒ�/ ��,�� ���� 
#define SPACE_UPDONW	14//27		
#define SPACE_LEFTRIGT	10//19

// russia support. [8/31/2010 rumist]

//------------------------------------------------------------------------------
// CUITrackPopup::CUITrackPopup
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUITrackPopup::CUITrackPopup()
{
	m_nMaxCharCount = 0;	// �ִ� ���� �� 
	m_strLongestStringInMenu = "";
	
}


//------------------------------------------------------------------------------
// CUITrackPopup::~CUITrackPopup
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
CUITrackPopup::~CUITrackPopup()
{
	m_vecCommandList.clear(); // [sora] command ����
}


void CUITrackPopup::Render()
{
	if( !IsVisible() ) return;
	if( m_vecString.size() <= 0 ) return;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if (g_iCountry == RUSSIA)
	{
		// [2011/05/17 : Sora] �˾�â���� ��Ʈ���� ���� ���� ��� ���� ���̰� 0�̵Ǿ� �˾�â ����� �پ���
		if( m_strLongestStringInMenu.Length() > 0 )
		{
			Resize( pDrawPort->GetTextWidth( m_strLongestStringInMenu ) );		
		}
	}	

	// ��� ����
	int	nX, nY;
	UIRect m_rcBack;

	GetAbsPos( nX, nY );
	m_rcBack.SetRect( nX, nY, nX + m_nWidth, nY + m_nHeight );

	// Item Back region
	pDrawPort->AddTexture( m_rcBack.Left, m_rcBack.Top,
										m_rcBack.Left + 7, m_rcBack.Top + 7,
										m_rtBackUL.U0, m_rtBackUL.V0, m_rtBackUL.U1, m_rtBackUL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Left + 7, m_rcBack.Top,
										m_rcBack.Right - 7, m_rcBack.Top + 7,
										m_rtBackUM.U0, m_rtBackUM.V0, m_rtBackUM.U1, m_rtBackUM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Right - 7, m_rcBack.Top,
										m_rcBack.Right, m_rcBack.Top + 7,
										m_rtBackUR.U0, m_rtBackUR.V0, m_rtBackUR.U1, m_rtBackUR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Left, m_rcBack.Top + 7,
										m_rcBack.Left + 7, m_rcBack.Bottom - 7,
										m_rtBackML.U0, m_rtBackML.V0, m_rtBackML.U1, m_rtBackML.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Left + 7, m_rcBack.Top + 7,
										m_rcBack.Right - 7, m_rcBack.Bottom - 7,
										m_rtBackMM.U0, m_rtBackMM.V0, m_rtBackMM.U1, m_rtBackMM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Right - 7, m_rcBack.Top + 7,
										m_rcBack.Right, m_rcBack.Bottom - 7,
										m_rtBackMR.U0, m_rtBackMR.V0, m_rtBackMR.U1, m_rtBackMR.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Left, m_rcBack.Bottom - 7,
										m_rcBack.Left + 7, m_rcBack.Bottom,
										m_rtBackLL.U0, m_rtBackLL.V0, m_rtBackLL.U1, m_rtBackLL.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Left + 7, m_rcBack.Bottom - 7,
										m_rcBack.Right - 7, m_rcBack.Bottom,
										m_rtBackLM.U0, m_rtBackLM.V0, m_rtBackLM.U1, m_rtBackLM.V1,
										0xFFFFFFFF );
	pDrawPort->AddTexture( m_rcBack.Right - 7, m_rcBack.Bottom - 7,
										m_rcBack.Right, m_rcBack.Bottom,
										m_rtBackLR.U0, m_rtBackLR.V0, m_rtBackLR.U1, m_rtBackLR.V1,
										0xFFFFFFFF );

	// ���� ����Ʈ �ڽ��� ���� ���� ����
	CUIListBox::Render();
	
}

//------------------------------------------------------------------------------
// UITrackPopup::AddMenuList
// Explain:  �޷� �׸� �߰� 
//			���ο� �׸��� �߰� �� ���� ListBox�� ũ�Ⱑ �����ȴ�.
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITrackPopup::AddMenuList( CTString strText, COLOR colText, int nCommandNum)
{
	AddString( 0, strText, colText );

	m_vecCommandList.push_back(nCommandNum); // [sora] command�� ����

	if (CUIListBox::GetScrollBar() == FALSE)
		CUIListBox::ChangeLineCount(m_vecString[0].vecString.size());
		
	if( m_nMaxCharCount < strText.Length() ) // �ִ� ���ڼ��� ������ ���´�~
	{
		m_nMaxCharCount = strText.Length();
	}
	
	if (g_iCountry == RUSSIA)
	{
		if( strText.Length() > m_strLongestStringInMenu.Length() )	
		{
			m_strLongestStringInMenu = strText;
		}
	}
	else
	{
		Resize();
	}
	
	// �ϴ��� ���� ���� 
	Hide();
}


//------------------------------------------------------------------------------
// CUITrackPopup::ResetAllStrings
// Explain:  
// Date : 2005-05-17,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
void CUITrackPopup::ResetAllStrings()
{
	m_nMaxCharCount = 0;	// �ִ� ���� �� 
	SetCurSel( -1 );

	CUIListBox::ResetAllStrings();
	m_vecCommandList.clear(); // [sora] command ����
}


//------------------------------------------------------------------------------
// CUITrackPopup::MouseMessage
// Explain:  ���� ���� 
//			1. ������ ó�� 
//			2. LButton Up ó��
// Date : 2005-05-16,Author: Lee Ki-hwan
//------------------------------------------------------------------------------
WMSG_RESULT CUITrackPopup::MouseMessage( MSG *pMsg )
{
	// If list box is disabled
	if( !IsEnabled() )
		return WMSG_FAIL;

	// Mouse point
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( m_bSelectList )
			{
				m_nOverList = -1;

				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					// Over �� Selection �ǰ� ����
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 && nRealSel < m_sbScrollBar.GetCurItemCount() )
					{
						m_nSelectList = nRealSel;
					}

					SetFocus(true);

					return WMSG_SUCCESS;
				}
			}

			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			if( m_bSelectList )
			{
				// UP�ÿ� ���� 
				if( IsInside( nX, nY ) )
				{
					return WMSG_SUCCESS;
				}
			}
			
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		
		}
		break;

	case WM_LBUTTONUP:
		{
			if( m_bSelectList )
			{
				// UP�ÿ� ���� 
				if( IsInside( nX, nY ) )
				{
					int	nAbsY = GetAbsPosY();
					m_nOverList = ( nY - nAbsY - m_nTextSY + m_nSelectOffsetY ) / m_nLineHeight;

					int	nRealSel = m_nOverList + m_sbScrollBar.GetScrollPos();
					if( nRealSel >= 0 )//&& nRealSel < m_sbScrollBar.GetCurItemCount() )
					{
						if (nRealSel >= m_vecCommandList.size())
							return WMSG_FAIL;

						m_nSelectList = nRealSel;
						return WMSG_COMMAND;
					}

					return WMSG_SUCCESS;
				}
			}
			
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_LBUTTONDBLCLK:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( m_bScrollBar )
			{
				if( m_sbScrollBar.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}

	return WMSG_FAIL;
}

void CUITrackPopup::Resize(int nStrLength /* = 0  */)
{
	int nWidth = 0;
	int nHeight = 0;
	// ���� �Էµ� ��Ʈ���� ���� ListBox�� ũ�� ���� 
	// Width = ���� �� ��Ʈ�� ���� ����

	if (g_iCountry == RUSSIA)
	{
		m_nWidth = SPACE_UPDONW - 10 + nStrLength + 10;

		// Height = �޴� �� 
		//m_nHeight = SPACE_LEFTRIGT - _pUIFontTexMgr->GetLineSpacing() + m_vecString[0].vecString.size() * _pUIFontTexMgr->GetLineHeight();
		m_nHeight = SPACE_LEFTRIGT - _pUIFontTexMgr->GetLineSpacing() + m_vecString[0].vecString.size() * m_nLineHeight;

		// Selection Bar�� ���� ũ�� ���� 
		nWidth = m_nWidth - 5;
		nHeight = _pUIFontTexMgr->GetLineHeight()+2;
	}
	else
	{
		m_nWidth = SPACE_UPDONW - _pUIFontTexMgr->GetFontSpacing() + m_nMaxCharCount *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() );

		// Height = �޴� �� 
		//m_nHeight = SPACE_LEFTRIGT - _pUIFontTexMgr->GetLineSpacing() + m_vecString[0].vecString.size() * _pUIFontTexMgr->GetLineHeight();
		m_nHeight = SPACE_LEFTRIGT - _pUIFontTexMgr->GetLineSpacing() + m_vecString[0].vecString.size() * m_nLineHeight;

		// Selection Bar�� ���� ũ�� ���� 
		nWidth = m_nWidth - 5;
		nHeight = _pUIFontTexMgr->GetLineHeight()+2;
	}

	// Selection Width 
	if( nWidth > m_nWidth || nWidth == -1 )
		nWidth = m_nWidth;
	int	nHalfWidth = nWidth / 2;
	int	nLeft = m_nWidth / 2 - nHalfWidth;

	if( nHeight < m_nLineHeight || nHeight == -1 )
		nHeight = m_nLineHeight;

	nLeft+=1;  // �¿� �ణ�� ���� ����
	nWidth-=4;

	m_rcSelectOver.SetRect( nLeft, -m_nSelectOffsetY, nLeft + nWidth, nHeight - m_nSelectOffsetY );
}	

void CUITrackPopup::DeleteMenuList( int nCol, int nIndex )
{
	CTString strMenu =GetString(nCol, nIndex);

	RemoveString(nIndex, nCol);	

	// �ش� ������ command ����
	std::vector<int>::iterator iter = m_vecCommandList.begin();
	iter += nIndex;
	m_vecCommandList.erase(iter);

	int nMaxLengthIdx = 0;

	if( strMenu.Length() == m_nMaxCharCount )
	{
		m_nMaxCharCount =0;
		for(int i=0; i<m_vecString[nCol].vecString.size(); ++i)
		{
			if( m_vecString[nCol].vecString[i].Length() > m_nMaxCharCount )
			{
				m_nMaxCharCount =m_vecString[nCol].vecString[i].Length();
				nMaxLengthIdx = i;
			}
		}
	}

	if (g_iCountry == RUSSIA)
		m_strLongestStringInMenu = m_vecString[nCol].vecString[nMaxLengthIdx];
	else
		Resize();
}

// [sora] ������ �޴��� commandnum���� ��ȯ
int CUITrackPopup::GetCommandNum(int nIndex)
{
	if(nIndex > m_vecCommandList.size() || nIndex < 0)
	{
		return -1;
	}

	return m_vecCommandList[nIndex];
}
