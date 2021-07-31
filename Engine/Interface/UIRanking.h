// ----------------------------------------------------------------------------
//  File : UIRanking.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIRANKING_H_
#define	UIRANKING_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <vector>

// Define text position
#define	RANKING_TITLE_TEXT_OFFSETX		25
#define	RANKING_TITLE_TEXT_OFFSETY		5

// Define size of Ranking
#define	RANKING_WIDTH					311
#define	RANKING_HEIGHT					280//260

// ----------------------------------------------------------------------------
// Name : CUIRanking
// Desc :
// ----------------------------------------------------------------------------
class CUIRanking : public CUIWindow
{
protected:
	// Ranking State
	enum eRankingState
	{
		RANKING_LIST,		// ��ŷ ����Ʈ.
		RANKING_PRIZE,		// ��ŷ�� ���� ����
	};

	// internal structure
	struct sUserRanking
	{
		sUserRanking()
		{
			bSuccess		= FALSE;
			lPoint			= 0;
			llTime			= 0;
		}

		~sUserRanking()
		{			
		}
		
		CTString		strName;			// �����?		
		BOOL			bSuccess;			// ���� ����.
		LONG			lPoint;				// ȹ�� ����Ʈ.
		__int64			llTime;				// �ҿ� �ð�.
	};

	struct sUserPrize
	{
		sUserPrize()
		{
			strName			= "";	
			iItemIndex		= -1;			// ������ �ε���
			iNumOfItem		= -1;			// ������ ����.
			bReward			= FALSE;
		}
		
		CTString		strName;			// �̸�
		INDEX			iItemIndex;			// ������ �ε���
		INDEX			iNumOfItem;			// ������ ����.		
		BOOL			bReward;			// ����ǰ ����
	};

	std::vector<sUserRanking>	m_vectorRankingList;		// �ܿ� ���
	std::vector<sUserPrize>		m_vectorPrizeList;			// ���� ���

	// Controls
	CUIButton				m_btnClose;						// Close button	

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar	

	// UV of each part
	UIRectUV				m_rtBackground;					// UV of background		
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackManagerTop;				//
	UIRectUV				m_rtBackManagerMiddle;			//
	UIRectUV				m_rtBackManagerBottom;			//
	UIRectUV				m_rtBackBottom;					// UV of bottom background

	UIRectUV				m_rtTab;						// UV of section tab
	
	CUIListBox				m_lbRankingList;				// List box
	CUIListBox				m_lbChoiceList;					// List box
	CUIListBox				m_lbPrizeList;					// List Box

	eRankingState			m_eRankingState;				// Ranking State
	BOOL					m_bSatisfyPrize;

protected:
	// Command
	void	PressOKBtn();	

public:
	CUIRanking();
	~CUIRanking();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild	
	void	OpenRankingList( );
	void	OpenRankingPrize( );
	void	ResetRanking();
	void	ClearRankingList();
	void	ClearPrizeList();
	
	// ���� ��� ����.
	void	AddToPrizeList( const CTString& strName, INDEX iItemIndex, INDEX iItemCount, BOOL bReward );	

	// ��� ��� ����
	void	AddToRankingList( const CTString& strName, BOOL bSuccess, LONG lPoint, __int64 llTime );		// ��� ��Ͽ� �߰�	

	// ��� ��� ����
	// NOTE : �������̽��� ������ �ݵ�� ���ŵǾ�� �ϴ� �κ��̹Ƿ� TRUE�� �����ϰ�,
	// NOTE : ���̴� �����϶��� �����ϰ��� �Ѵٸ� FALSE�� �����Ұ�.
	void	RefreshRankingList( BOOL bInit = FALSE );
	void	RefreshPrizeList( BOOL bInit = FALSE );	

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Set focus
	void	SetFocus( BOOL bVisible );	
	// esc closing support [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()				{ ResetRanking();			return TRUE;		}
};

#endif	// UIRANKING_H_