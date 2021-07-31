#include "stdh.h"

// ��� ����. [12/2/2009 rumist]
#include <vector>
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIRanking.h>
#include <algorithm>

#define RANKING_LIST_BOX_WIDTH			(280)
#define RANKING_LIST_BOX_HEIGHT			(88)
#define CHOICE_LIST_BOX_WIDTH			(280)
#define CHOICE_LIST_BOX_HEIGHT			(120)

// ----------------------------------------------------------------------------
// Name : CUIRanking()
// Desc : 
// ----------------------------------------------------------------------------
CUIRanking::CUIRanking()
{
}

// ----------------------------------------------------------------------------
// Name : ~CUIRanking()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIRanking::~CUIRanking()
{
	if(!m_vectorRankingList.empty())
	{
		m_vectorRankingList.clear();
	}

	if(!m_vectorPrizeList.empty())
	{
		m_vectorPrizeList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIRanking::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	// Region of each part
	m_rcTitle.SetRect( 0, 0, 311, 22 );	

	// Create skill learn texture
	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\MessageBox.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	// UV Coordinate of each part
	// Background
	m_rtBackTop.SetUV( 0, 0, 216, 26, fTexWidth, fTexHeight );
	m_rtBackMiddle1.SetUV( 0, 31, 216, 33, fTexWidth, fTexHeight );
	m_rtBackMiddle2.SetUV( 0, 35, 216, 37, fTexWidth, fTexHeight );
	m_rtBackManagerTop.SetUV( 36, 86, 252, 106, fTexWidth, fTexHeight );		// 20
	m_rtBackManagerMiddle.SetUV( 36, 106, 252, 112, fTexWidth, fTexHeight );	// 6
	m_rtBackManagerBottom.SetUV( 36, 112, 252, 116, fTexWidth, fTexHeight );	// 4
	m_rtBackBottom.SetUV( 0, 38, 216, 45, fTexWidth, fTexHeight );

	m_rtTab.SetUV( 43, 69, 44, 86, fTexWidth, fTexHeight );

	// Close button
	m_btnClose.Create( this, CTString( "" ), 279, 4, 14, 14 );
	m_btnClose.SetUV( UBS_IDLE, 219, 0, 233, 14, fTexWidth, fTexHeight );
	m_btnClose.SetUV( UBS_CLICK, 234, 0, 248, 14, fTexWidth, fTexHeight );
	m_btnClose.CopyUV( UBS_IDLE, UBS_ON );
	m_btnClose.CopyUV( UBS_IDLE, UBS_DISABLE );

	// List box of guild description
	m_lbRankingList.Create( this, 8, 46, RANKING_LIST_BOX_WIDTH, RANKING_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight(), 13, 3, 5, FALSE );
	m_lbRankingList.CreateScroll( TRUE, 0, 0, 9, RANKING_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );		
	m_lbRankingList.SetColumnPosX( 0, 10 );
	m_lbRankingList.SetColumnPosX( 1, 40 );
	m_lbRankingList.SetColumnPosX( 2, 122 );
	m_lbRankingList.SetColumnPosX( 3, 170 );
	m_lbRankingList.SetColumnPosX( 4, 210 );
	// Up button
	m_lbRankingList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbRankingList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbRankingList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbRankingList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbRankingList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbRankingList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbRankingList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbRankingList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbRankingList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbRankingList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbRankingList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// List box of guild description
	m_lbPrizeList.Create( this, 8, 46, RANKING_LIST_BOX_WIDTH, RANKING_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight(), 13, 3, 3, TRUE );
	m_lbPrizeList.CreateScroll( TRUE, 0, 0, 9, RANKING_LIST_BOX_HEIGHT, 9, 7, 0, 0, 10 );		
	m_lbPrizeList.SetSelBar( RANKING_LIST_BOX_WIDTH, _pUIFontTexMgr->GetLineHeight(), 187, 46, 204, 61, fTexWidth, fTexHeight );
	m_lbPrizeList.SetColumnPosX( 0, 10 );
	m_lbPrizeList.SetColumnPosX( 1, 40 );
	m_lbPrizeList.SetColumnPosX( 2, 134 );	
	// Up button
	m_lbPrizeList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbPrizeList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbPrizeList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbPrizeList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbPrizeList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbPrizeList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbPrizeList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbPrizeList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbPrizeList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbPrizeList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbPrizeList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

	// List box of guild description
	m_lbChoiceList.Create( this, 8, 150, CHOICE_LIST_BOX_WIDTH, CHOICE_LIST_BOX_HEIGHT, _pUIFontTexMgr->GetLineHeight(), 13, 3, 1, TRUE );	
	m_lbChoiceList.SetOverColor( 0xF8E1B5FF );
	m_lbChoiceList.SetSelectColor( 0xF8E1B5FF );	
	// Up button
	m_lbChoiceList.SetScrollUpUV( UBS_IDLE, 230, 16, 239, 23, fTexWidth, fTexHeight );
	m_lbChoiceList.SetScrollUpUV( UBS_CLICK, 240, 16, 249, 23, fTexWidth, fTexHeight );
	m_lbChoiceList.CopyScrollUpUV( UBS_IDLE, UBS_ON );
	m_lbChoiceList.CopyScrollUpUV( UBS_IDLE, UBS_DISABLE );
	// Down button
	m_lbChoiceList.SetScrollDownUV( UBS_IDLE, 230, 24, 239, 31, fTexWidth, fTexHeight );
	m_lbChoiceList.SetScrollDownUV( UBS_CLICK, 240, 24, 249, 31, fTexWidth, fTexHeight );
	m_lbChoiceList.CopyScrollDownUV( UBS_IDLE, UBS_ON );
	m_lbChoiceList.CopyScrollDownUV( UBS_IDLE, UBS_DISABLE );
	// Bar button
	m_lbChoiceList.SetScrollBarTopUV( 219, 16, 228, 26, fTexWidth, fTexHeight );
	m_lbChoiceList.SetScrollBarMiddleUV( 219, 27, 228, 29, fTexWidth, fTexHeight );
	m_lbChoiceList.SetScrollBarBottomUV( 219, 30, 228, 40, fTexWidth, fTexHeight );	

//	AddToRankingList( 0, 0, CTString("�׽�Ʈ��"), TRUE, 100, 40);
//	AddToRankingList( 1, 1, CTString("�׽�Ʈ2"), FALSE, 300, 80);
//	AddToRankingList( 2, 2, CTString("�׽�Ʈ3"), TRUE, 100, 90);

	//RefreshRankingList();
}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRanking::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUIRanking::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}

//------------------------------------------------------------------------------
// CUIRanking::SetFocus
// Explain:  
// Date : 2005-03-10(���� 1:02:39) Lee Ki-hwan
//------------------------------------------------------------------------------
void CUIRanking::SetFocus( BOOL bVisible )
{
	CUIWindow::SetFocus( bVisible );
}

// ----------------------------------------------------------------------------
// Name : OpenRankingList()
// Desc : 
// ----------------------------------------------------------------------------
void CUIRanking::OpenRankingList( )
{	
	if( IsVisible() )
	{		
		return;
	}
	//ResetRanking();

	m_lbRankingList.ResetAllStrings();
	m_lbChoiceList.ResetAllStrings();

	m_eRankingState		= RANKING_LIST;

	m_lbRankingList.SetEnable( m_eRankingState == RANKING_LIST );
	m_lbPrizeList.SetEnable( m_eRankingState != RANKING_LIST );

	RefreshRankingList( TRUE );	
	
	CTString strJobName;
	int i = 0; 
	for( ; i < (TOTAL_JOB) ; ++i )
	{
		strJobName.PrintF( "%s", CJobInfo::getSingleton()->GetName(i, 0) );
		m_lbChoiceList.AddString( 0, strJobName );
	}	
	strJobName.PrintF( _S( 1220, "����Ѵ�." ) );		
	m_lbChoiceList.AddString( 0, strJobName );

	CUIManager::getSingleton()->RearrangeOrder( UI_RANKING, TRUE );
}

// ----------------------------------------------------------------------------
// Name : OpenRankingPrize()
// Desc : 
// ----------------------------------------------------------------------------
void CUIRanking::OpenRankingPrize( )
{	
	if( IsVisible() )
	{		
		return;
	}
	//ResetRanking();

	m_lbPrizeList.ResetAllStrings();
	m_lbChoiceList.ResetAllStrings();

	m_eRankingState		= RANKING_PRIZE;

	m_lbRankingList.SetEnable( m_eRankingState == RANKING_LIST );
	m_lbPrizeList.SetEnable( m_eRankingState != RANKING_LIST );

	RefreshPrizeList( TRUE );	

	CTString strChoice;		
	
	// NOTE : ������ ��������, ��ŷ�� �ش������� ���̵� �־�߸� �����ϴ�.
	strChoice.PrintF( _S( 1712, "������ �޴´�." ) );			
	m_lbChoiceList.AddString( 0, strChoice );
	
	strChoice.PrintF( _S( 1220, "����Ѵ�." ) );		
	m_lbChoiceList.AddString( 0, strChoice );	

	CUIManager::getSingleton()->RearrangeOrder( UI_RANKING, TRUE );
}

// ----------------------------------------------------------------------------
// Name : ResetRanking()
// Desc : 
// ----------------------------------------------------------------------------
void CUIRanking::ResetRanking()
{		
	m_lbRankingList.ResetAllStrings();
	m_lbPrizeList.ResetAllStrings();
	m_lbChoiceList.ResetAllStrings();

	// NOTE : ��带 Ż���ϰų� ��ü ���� �ʴ� �̻�, Ŭ���� �ϸ� �ȵɰ� ����.
	//ClearRankingList();

	ClearRankingList();
	ClearPrizeList();

	CUIManager::getSingleton()->RearrangeOrder( UI_RANKING, FALSE );
}

// ----------------------------------------------------------------------------
// Name : ClearRankingList()
// Desc : ��� ����� Ŭ�����մϴ�.
// ----------------------------------------------------------------------------
void CUIRanking::ClearRankingList()
{
	if(!m_vectorRankingList.empty())
	{
		m_vectorRankingList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : ClearPrizeList()
// Desc : ��� ����� Ŭ�����մϴ�.
// ----------------------------------------------------------------------------
void CUIRanking::ClearPrizeList()
{
	if(!m_vectorPrizeList.empty())
	{
		m_vectorPrizeList.clear();
	}
}

// ----------------------------------------------------------------------------
// Name : PressOKBtn()
// Desc : Ȯ�� ��ư�� ������ ���� ó��.
// ----------------------------------------------------------------------------
void CUIRanking::PressOKBtn()
{	
}

static LONG g_lChaIndex		= -1;		// ���� ��û�� ���õ� �κп��� ����.
static LONG g_lRankingIndex	= -1;
static LONG g_lMemberIndex	= -1;		// ����� ���õ� �κп��� ����.

// ----------------------------------------------------------------------------
// Name : AddToRankingList()
// Desc : ��� ��Ͽ� �߰�
// ----------------------------------------------------------------------------
void CUIRanking::AddToRankingList( const CTString& strName, BOOL bSuccess, LONG lPoint, __int64 llTime )
{	
	sUserRanking TempRankingUser;
	TempRankingUser.strName			= strName;
	TempRankingUser.bSuccess		= bSuccess;
	TempRankingUser.lPoint			= lPoint;
	TempRankingUser.llTime			= llTime;
	m_vectorRankingList.push_back(TempRankingUser);
}

// ----------------------------------------------------------------------------
// Name : AddToPrizeList()
// Desc : 
// ----------------------------------------------------------------------------
void CUIRanking::AddToPrizeList( const CTString& strName, INDEX iItemIndex, INDEX iItemCount, BOOL bReward )
{
	sUserPrize	TempPrize;
	TempPrize.strName		= strName;			// �̸�
	TempPrize.iItemIndex	= iItemIndex;		// ������ �ε���
	TempPrize.iNumOfItem	= iItemCount;		// ������ ����.		
	TempPrize.bReward		= bReward;			// ����ǰ ����
	m_vectorPrizeList.push_back(TempPrize);
}

// ----------------------------------------------------------------------------
// Name : RefreshRankingList()
// Desc : ����� �����մϴ�.
// ----------------------------------------------------------------------------
void CUIRanking::RefreshRankingList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbRankingList.ResetAllStrings();
	//std::sort(m_vectorRankingList.begin(), m_vectorRankingList.end());

	CTString	strTemp;	

	std::vector<sUserRanking>::const_iterator member_end = m_vectorRankingList.end();
	std::vector<sUserRanking>::iterator it = m_vectorRankingList.begin();
	int iNo	= 1;
	for(it = m_vectorRankingList.begin(); it != member_end; ++it, ++iNo )
	{
		strTemp.PrintF( "%d.", iNo );
		m_lbRankingList.AddString( 0, strTemp, 0xFFFFFFFF );	// ����
		m_lbRankingList.AddString( 1, (*it).strName, 0xFFFFFFFF );	// ���̵�

		strTemp = (*it).bSuccess ? _S( 1970, "����" ) : _S( 1971, "����" );		
		m_lbRankingList.AddString( 2, strTemp, 0xFFFFFFFF );	// ����

		strTemp.PrintF( "%ld", (*it).lPoint );
		m_lbRankingList.AddString( 3, strTemp, 0xFFFFFFFF );	// ����

		int iMin	= (*it).llTime / 60;
		int	iSecond	= (*it).llTime % 60;
		//strTemp.PrintF( "%I64d", (*it).llTime );
		strTemp.PrintF( _S( 1713, "%d�� %d��" ), iMin, iSecond );			
		m_lbRankingList.AddString( 4, strTemp, 0xFFFFFFFF );	// �ð�
	}
}

// ----------------------------------------------------------------------------
// Name : RefreshPrizeList()
// Desc :
// ----------------------------------------------------------------------------
void CUIRanking::RefreshPrizeList( BOOL bInit )
{
	if( !bInit && !IsVisible() )
		return;

	m_lbPrizeList.ResetAllStrings();
	//std::sort(m_vectorPrizeList.begin(), m_vectorPrizeList.end());

	CTString	strTemp;	
	int iSelPrize	= -1;
	m_bSatisfyPrize	= FALSE;

	std::vector<sUserPrize>::const_iterator prize_end = m_vectorPrizeList.end();
	std::vector<sUserPrize>::iterator it = m_vectorPrizeList.begin();
	int iNo	= 1;
	for(it = m_vectorPrizeList.begin(); it != prize_end; ++it, ++iNo )
	{
		strTemp.PrintF( "%d.", iNo );
		m_lbPrizeList.AddString( 0, strTemp, 0xFFFFFFFF );	// ����
		m_lbPrizeList.AddString( 1, (*it).strName, 0xFFFFFFFF );	// ���̵�
		
		CTString strItemName = _pNetwork->GetItemName( (*it).iItemIndex );
		strTemp.PrintF( _S( 61, "%s %d��" ), strItemName, (*it).iNumOfItem );
		m_lbPrizeList.AddString( 2, strTemp, 0xFFFFFFFF );
		
		if( (*it).bReward == FALSE && 
			_pNetwork->MyCharacterInfo.name == (*it).strName )
		{
			iSelPrize		= iNo - 1;
			m_bSatisfyPrize	= TRUE;
		}
	}

	if( iSelPrize != -1 )
		m_lbPrizeList.SetCurSel( iSelPrize );
}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIRanking::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	// Set skill learn texture
	pDrawPort->InitTextureData( m_ptdBaseTexture );

	// Add render regions
	int	nX, nY;

	// Background
	// Top
	nX = m_nPosX + m_nWidth;
	nY = m_nPosY + 26;
	pDrawPort->AddTexture( m_nPosX, m_nPosY, nX, nY,
										m_rtBackTop.U0, m_rtBackTop.V0,
										m_rtBackTop.U1, m_rtBackTop.V1,
										0xFFFFFFFF );
	
	{
		// Manager
		pDrawPort->AddTexture( m_nPosX, nY, nX, nY + 20,
											m_rtBackManagerTop.U0, m_rtBackManagerTop.V0,
											m_rtBackManagerTop.U1, m_rtBackManagerTop.V1,
											0xFFFFFFFF );

		nY += 20;
		
		pDrawPort->AddTexture( m_nPosX, nY, nX, nY + RANKING_LIST_BOX_HEIGHT + 2,
			m_rtBackManagerMiddle.U0, m_rtBackManagerMiddle.V0,
			m_rtBackManagerMiddle.U1, m_rtBackManagerMiddle.V1,
			0xFFFFFFFF );
		
		nY += RANKING_LIST_BOX_HEIGHT + 2;		

		pDrawPort->AddTexture( m_nPosX, nY, nX, nY + 4,
											m_rtBackManagerBottom.U0, m_rtBackManagerBottom.V0,
											m_rtBackManagerBottom.U1, m_rtBackManagerBottom.V1,
											0xFFFFFFFF );

		nY += 4;
	}

	// Middle 2
	pDrawPort->AddTexture( m_nPosX, m_nPosY + 26, nX, m_nPosY + m_nHeight - 7 ,
										m_rtBackMiddle2.U0, m_rtBackMiddle2.V0,
										m_rtBackMiddle2.U1, m_rtBackMiddle2.V1,
										0xFFFFFFFF );

	// Bottom
	nY = m_nPosY + m_nHeight - 7;
	pDrawPort->AddTexture( m_nPosX, nY, nX, m_nPosY + m_nHeight,
										m_rtBackBottom.U0, m_rtBackBottom.V0,
										m_rtBackBottom.U1, m_rtBackBottom.V1,
										0xFFFFFFFF );
	// ----------------------------------------------------------------------------

	nX = m_nPosX + 55;
	nY = m_nPosY + 26;
	int nX2 = nX + 1;
	int nY2 = nY + 16;
	pDrawPort->AddTexture( nX, nY, nX2, nY2,
												m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
												0xFFFFFFFF );	

	// Ranking List Tab
	if( m_eRankingState == RANKING_LIST )
	{
		nX = m_nPosX + 132;	
		nX2 = nX + 1;
		pDrawPort->AddTexture( nX, nY, nX2, nY2,
												m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
												0xFFFFFFFF );

		nX = m_nPosX + 174;
		nX2 = nX + 1;
		pDrawPort->AddTexture( nX, nY, nX2, nY2,
												m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
												0xFFFFFFFF );
		
		nX = m_nPosX + 225;
		nX2 = nX + 1;
		pDrawPort->AddTexture( nX, nY, nX2, nY2,
												m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
												0xFFFFFFFF );
	}
	else
	{
		nX = m_nPosX + 145;	
		nX2 = nX + 1;
		pDrawPort->AddTexture( nX, nY, nX2, nY2,
												m_rtTab.U0, m_rtTab.V0, m_rtTab.U1, m_rtTab.V1,
												0xFFFFFFFF );
	}
	
	// Close button
	m_btnClose.Render();	

	if( m_eRankingState == RANKING_LIST )
	{
		m_lbRankingList.Render();
	}
	else
	{
		m_lbPrizeList.Render();
	}
	m_lbChoiceList.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->PutTextEx( _S( 1714, "����" ), m_nPosX + 20,		
										m_nPosY + 29, 0xFFFFFFFF );

	pDrawPort->PutTextEx( _S( 1715, "���̵�" ), m_nPosX + 80,	
										m_nPosY + 29, 0xFFFFFFFF );

	if( m_eRankingState == RANKING_LIST )
	{
		pDrawPort->PutTextEx( _S( 1716, "����" ), m_nPosX + 142,		
			m_nPosY + 29, 0xFFFFFFFF );
		
		pDrawPort->PutTextEx( _S( 1717, "����" ), m_nPosX + 190,		
			m_nPosY + 29, 0xFFFFFFFF );
		
		pDrawPort->PutTextEx( _S( 1718, "�ð�" ), m_nPosX + 238,		
			m_nPosY + 29, 0xFFFFFFFF );
	}
	else
	{
		pDrawPort->PutTextEx( _S( 1719, "����ǰ" ), m_nPosX + 190,		
			m_nPosY + 29, 0xFFFFFFFF );
	}

	CTString strTitle;
	if( m_eRankingState == RANKING_LIST )
		strTitle = _S( 1720, "���� Ȯ��" );		
	else
		strTitle = _S( 1721, "���� �ޱ�" );		

	// Text in guild
	pDrawPort->PutTextEx( strTitle, m_nPosX + RANKING_TITLE_TEXT_OFFSETX,
										m_nPosY + RANKING_TITLE_TEXT_OFFSETY, 0xFFFFFFFF );

	// Flush all render text queue
	pDrawPort->EndTextEx();
	
}

// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT	CUIRanking::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;

	// Title bar
	static BOOL	bTitleBarClick = FALSE;

	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;

	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );
	
	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();

			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			// Move shop
			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}
			// Close Button
			else if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			else if( m_lbRankingList.MouseMessage( pMsg ) != WMSG_FAIL )			
				return WMSG_SUCCESS;
			else if( m_lbChoiceList.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
			//else if( m_lbPrizeList.MouseMessage( pMsg ) != WMSG_FAIL )			
			//	return WMSG_SUCCESS;			
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				// Close button
				if( m_btnClose.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}				
				else if( m_lbRankingList.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}
				//else if( m_lbPrizeList.MouseMessage( pMsg ) != WMSG_FAIL )
				//{			
				//}

				else if( m_lbChoiceList.MouseMessage( pMsg ) != WMSG_FAIL )
				{					
					int	iSelChoice = m_lbChoiceList.GetCurSel();
					if( iSelChoice != -1 )
					{
						// ���� ���� �޴�...
						if( m_eRankingState == RANKING_LIST )
						{
							// ����ϱ� ���ý�...
							if( iSelChoice == TOTAL_JOB )
							{
								ResetRanking();
							}
							else
							{								
								_pNetwork->Ranking_RequestList( iSelChoice );								
							}
						}
						// ���� �ޱ� �޴�...
						else
						{
							// ����ϱ� ���ý�.
							if( iSelChoice == 1 )
							{
								ResetRanking();
							}
							else
							{
								if( m_bSatisfyPrize	)
								{
									_pNetwork->Ranking_Prize();
								}
								else
								{
									CTString strMessage;
									strMessage = _S( 1722, "������ ������ �� �ִ� ������ �ƴմϴ�." );		
									_pNetwork->ClientSystemMessage( strMessage, SYSMSG_ERROR );
									ResetRanking();
								}
							}
						}
					}
					return WMSG_SUCCESS;
				}

				CUIManager::getSingleton()->RearrangeOrder( UI_RANKING, TRUE );
				return WMSG_SUCCESS;
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;

			// Close button
			if( (wmsgResult = m_btnClose.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					ResetRanking();
				}
				return WMSG_SUCCESS;
			}			
			// List box
			else if( m_lbRankingList.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}			
			else if( m_lbChoiceList.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			/*
			else if( m_lbPrizeList.MouseMessage( pMsg ) != WMSG_FAIL )
			{
				return WMSG_SUCCESS;
			}
			*/
		}
		break;

	case WM_MOUSEWHEEL:
		{
			if( IsInside( nX, nY ) )
			{	
				// List box of member list
				if( m_lbRankingList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
				else if( m_lbPrizeList.MouseMessage( pMsg ) != WMSG_FAIL )
					return WMSG_SUCCESS;
			}
		}
		break;
	}
	return WMSG_FAIL;
}

//[ttos_2010_8_9]: ť�� ��ũ UI
//------------------------------------------------------------//
// CUICubeRank
//------------------------------------------------------------//
CUICubeRank::CUICubeRank()
{
	m_lMypoint = 0;
	m_bGuild = FALSE;
}
CUICubeRank::~CUICubeRank()
{

}
// ----------------------------------------------------------------------------
// Name : CubeRankInit()
// Desc :
// ----------------------------------------------------------------------------
void CUICubeRank::CubeRankInit()
{
	m_lMypoint = 0;
	m_bGuild = FALSE;

	for (int i = 0; i < 2; i++ )
	{
		for (int j = 0; j < 5; j++)
		{
			m_stCubeRank[i][j].Init();
		}
	}
}
// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUICubeRank::Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )
{
	CUIWindow::Create(pParentWnd, nX, nY, nWidth, nHeight);

	m_ptdBaseTexture = CreateTexture( CTString( "Data\\Interface\\new_Interface.tex" ) );
	FLOAT	fTexWidth = m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight = m_ptdBaseTexture->GetPixHeight();

	m_bxBackground[0].SetBoxUV(m_ptdBaseTexture, 120, 36, WRect(0,98,272,253));
	m_bxBackground[0].SetBoxPos(WRect(0,0,nWidth,nHeight));

	m_bxBackground[1].SetBoxUV(m_ptdBaseTexture, 20, WRect(0,304,221,346));
	m_bxBackground[1].SetBoxPos(WRect(20, 100, 380, 195));

	m_bxBackground[2].SetBoxUV(m_ptdBaseTexture, 20, WRect(0,304,221,346));
	m_bxBackground[2].SetBoxPos(WRect(20, 220, 380, 315));

	m_bxBackground[3].SetBoxUV(m_ptdBaseTexture, 3, WRect(339,464,368,466));
	m_bxBackground[3].SetBoxPos(WRect(20, 70, 380, 73));


	m_btnOK.Create(this, CTString(_S(191, "Ȯ��")), 150, 320, 100, 20);
	m_btnOK.SetUV(UBS_IDLE,282,244,351,265, fTexWidth, fTexHeight );
	m_btnOK.SetUV(UBS_CLICK, 364,244,433,265, fTexWidth, fTexHeight);
	m_btnOK.CopyUV(UBS_IDLE, UBS_ON);
	m_btnOK.CopyUV(UBS_IDLE, UBS_DISABLE);

	m_rcTitle.SetRect(0, 0, nWidth, 30);


}

// ----------------------------------------------------------------------------
// Name : ResetPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICubeRank::ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	SetPos( ( pixMaxI + pixMinI - GetWidth() ) / 2, ( pixMaxJ + pixMinJ - GetHeight() ) / 2 );
}

// ----------------------------------------------------------------------------
// Name : AdjustPosition()
// Desc :
// ----------------------------------------------------------------------------
void CUICubeRank::AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ )
{
	if( m_nPosX < pixMinI || m_nPosX + GetWidth() > pixMaxI ||
		m_nPosY < pixMinJ || m_nPosY + GetHeight() > pixMaxJ )
		ResetPosition( pixMinI, pixMinJ, pixMaxI, pixMaxJ );
}
// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUICubeRank::Render()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	pDrawPort->InitTextureData( m_ptdBaseTexture );

	int		i;
	for( i = 0; i < 4; i++ )
	{
		m_bxBackground[i].Render(m_nPosX,m_nPosY);
	}
	m_btnOK.Render();

	CTString strMsg,strMsgEx;
	CTString strGuildName, strChaName; // ��� �̸�, ��� ������ �� ĳ���� �̸�

	if (m_bGuild)
	{
		strMsg = _S(4388, "��� ť�� ����Ʈ ��Ȳ");
		strMsgEx = _S(4389, "�ڽ��� �Ҽӵ� ��� ť�� ����Ʈ : %d");
		strGuildName = _S(3451,"����");
		strChaName = _S(4344, "��� ������");
	}
	else
	{
		strMsg = _S(4390, "���� ť�� ����Ʈ ��Ȳ");
		strMsgEx = _S(4391, "�ڽ��� ���� ť�� ����Ʈ : %d");
		strGuildName = _S(4392, "�Ҽ� ���");
		strChaName = _S(4393, "ĳ���� �̸�");
	}

	pDrawPort->PutTextExCX(strMsg, m_nPosX+200, m_nPosY+18);
	strMsg.PrintF(strMsgEx, m_lMypoint);
	pDrawPort->PutTextEx(strMsg, m_nPosX + 20, m_nPosY + 50);
	
	CTString strtem;

	for( i = 0; i < 2; i++)
	{		
		if (i == 0)
		{
			strtem = _S(5097, "���� ť�� ����Ʈ ���� ��Ȳ");
		}
		else
		{
			strtem = _S(5098, "������ ť�� ����Ʈ ���� ��Ȳ");
		}
		pDrawPort->PutTextEx(strtem, m_nPosX + 25, m_nPosY + 80+(i*120));
		pDrawPort->PutTextEx(_S(1714, "����"), m_nPosX + 25, m_nPosY + 105+(i*120));
		pDrawPort->PutTextExCX(strGuildName, m_nPosX + 100, m_nPosY + 105+(i*120));
		pDrawPort->PutTextExCX(strChaName, m_nPosX + 210, m_nPosY + 105+(i*120));
		pDrawPort->PutTextExCX( _S(4345, "ť������Ʈ"), m_nPosX + 330, m_nPosY + 105+(i*120));

		for (int nList = 0; nList < (m_bGuild?5:3); nList++)
		{
			strtem.PrintF("%d",m_stCubeRank[i][nList].nRank);
			pDrawPort->PutTextEx(strtem, m_nPosX + 30, m_nPosY + 120+(nList*14)+(i*120));
			pDrawPort->PutTextExCX(m_stCubeRank[i][nList].strGuildName, m_nPosX + 100, m_nPosY + 120+(nList*14)+(i*120));
			pDrawPort->PutTextExCX(m_stCubeRank[i][nList].strChaName, m_nPosX + 210, m_nPosY + 120+(nList*14)+(i*120));
			strtem.PrintF("%d",m_stCubeRank[i][nList].lPoint);
			pDrawPort->PutTextExCX(strtem , m_nPosX + 330, m_nPosY + 120+(nList*14)+(i*120));
		}
	}

	pDrawPort->FlushRenderingQueue();
	
	pDrawPort->EndTextEx();

}
// ----------------------------------------------------------------------------
// Name : CubeRankClose()
// Desc :
// ----------------------------------------------------------------------------
void CUICubeRank::CubeRankClose()
{
	CubeRankInit();
	CUIManager::getSingleton()->RearrangeOrder( UI_CUBERANK, FALSE );
}
// ----------------------------------------------------------------------------
// Name : MouseMessage()
// Desc :
// ----------------------------------------------------------------------------
WMSG_RESULT CUICubeRank::MouseMessage( MSG *pMsg )
{
	WMSG_RESULT	wmsgResult;
	
	// Title bar
	static BOOL	bTitleBarClick = FALSE;
	
	// Item clicked
	static BOOL	bLButtonDownInItem = FALSE;
	
	// Mouse point
	static int	nOldX, nOldY;
	int	nX = LOWORD( pMsg->lParam );
	int	nY = HIWORD( pMsg->lParam );

	// Mouse message
	switch( pMsg->message )
	{
	case WM_MOUSEMOVE:
		{
			if( IsInside( nX, nY ) )
				CUIManager::getSingleton()->SetMouseCursorInsideUIs();
			
			int	ndX = nX - nOldX;
			int	ndY = nY - nOldY;

			if( bTitleBarClick && ( pMsg->wParam & MK_LBUTTON ) )
			{
				nOldX = nX;	nOldY = nY;
				
				Move( ndX, ndY );
				
				return WMSG_SUCCESS;
			}

			else if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				return WMSG_SUCCESS;
		}break;
	case WM_LBUTTONDOWN:
		{
			if( IsInside( nX, nY ) )
			{
				nOldX = nX;		nOldY = nY;

				
				if( m_btnOK.MouseMessage( pMsg ) != WMSG_FAIL )
				{
					// Nothing
				}// Title bar
				else if( IsInsideRect( nX, nY, m_rcTitle ) )
				{
					bTitleBarClick = TRUE;
				}
				return WMSG_SUCCESS;
			}

		}break;
	case WM_LBUTTONUP:
		{
			bTitleBarClick = FALSE;
			
			// Close button
			if( (wmsgResult = m_btnOK.MouseMessage( pMsg ) ) != WMSG_FAIL )
			{
				if(wmsgResult == WMSG_COMMAND)
				{
					CubeRankClose();
					return WMSG_SUCCESS;
				}				
			}

		}break;
	}
		

	return WMSG_FAIL;
}

void CUICubeRank::CreateCubeState(CNetworkMessage *istr, BOOL bGuild)
{
	LONG lMyPoint = 0, lInfoCubePoint = 0; // �ڽ��� �Ҽӵ� ��� �� ���� ����Ʈ , �� ����Ʈ�� ť�� ����Ʈ
	BYTE InfoCount = 0, InfoRank = 0; // ����Ʈ ����, ��ũ ����
	CTString strGuildName, strChaName; // ��� �̸�, ��� ������ �� ĳ���� �̸�
	int		i;

	(*istr) >> lMyPoint;
	(*istr) >> InfoCount;

	m_lMypoint = lMyPoint;
	m_bGuild = bGuild;
	// �̹��� ��ŷ
	for( i = 0; i<InfoCount; ++i )
	{
		(*istr) >> InfoRank >> strGuildName >> strChaName >> lInfoCubePoint;

		m_stCubeRank[0][i].nRank = InfoRank;
		m_stCubeRank[0][i].strGuildName = strGuildName;
		m_stCubeRank[0][i].strChaName = strChaName;
		m_stCubeRank[0][i].lPoint = lInfoCubePoint;
	}

	(*istr) >> InfoCount;
	// ������ ��ŷ
	for( i = 0; i < InfoCount; ++i )
	{		
		(*istr) >> InfoRank >> strGuildName >> strChaName >> lInfoCubePoint;
		
		m_stCubeRank[1][i].nRank = InfoRank;
		m_stCubeRank[1][i].strGuildName = strGuildName;
		m_stCubeRank[1][i].strChaName = strChaName;
		m_stCubeRank[1][i].lPoint = lInfoCubePoint;
	}

	CUIManager::getSingleton()->RearrangeOrder( UI_CUBERANK, TRUE );
}