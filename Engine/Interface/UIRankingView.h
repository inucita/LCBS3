#ifndef UIRANKING_VIEW_H
#define UIRANKING_VIEW_H

#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIListBoxMultiList.h>

struct stRankingViewSubTitle
{
	stRankingViewSubTitle()
	{
		nRow		= -1;
		nStringCode = -1;
	}
	int nRow;
	int nStringCode;
	CTString cszString;
};

class CUIRankingView: public CUIWindow
{
public:
	explicit CUIRankingView( int nClassID );
	~CUIRankingView ();
	
	int					m_iImageIdx;						// view Image Index
	int					m_bVisible;							// Right View 
	int					m_bVisibleImg;						// Right View Image		
	
	// Controls
	CUIButton			m_btnClose;							// Close button
	CUIButton			m_btnCancel;						// cancel button;
	//CUIButton			m_btnReward;						// �����ư 
	CUIButton			m_btnMore;							// ���� ������ ���� ��ư
	
	// List 
	CUIListBoxMultiCol	m_lbListContent;					// Ȯ���� �ʿ� �� �� 
	
	// Region of each part
	UIRect				m_rcTitle;							// Region of title		
	
	// UV of each part
	UIRectUV			m_rtTitleUL;						// UV of upper left background
	UIRectUV			m_rtTitleUM;						// UV of upper middle background
	UIRectUV			m_rtTitleUR;						// UV of upper right background
	
	UIRectUV			m_rtBackUL;							// UV of upper left background
	UIRectUV			m_rtBackUM;							// UV of upper middle background
	UIRectUV			m_rtBackUR;							// UV of upper right background
	UIRectUV			m_rtBackML;							// UV of middle left background
	UIRectUV			m_rtBackMM;							// UV of middle middle background
	UIRectUV			m_rtBackMR;							// UV of middle right background
	UIRectUV			m_rtBackLL;							// UV of lower left background
	UIRectUV			m_rtBackLM;							// UV of lower middle background
	UIRectUV			m_rtBackLR;							// UV of lower right background	
		
	UIRectUV			m_rtHorizonLine;
	UIRectUV			m_rtVerticalLine;
	int					m_iColCount;						// ����Ʈ�� Į�� �� 		
	

	//���� �簢�� ���� �׸���. 	
	UIRectUV			m_rtInnerRectML;
	UIRectUV			m_rtInnerRectMM;
	UIRectUV			m_rtInnerRectMR;	

	
	//�� ������ ����
	int					m_nGap1;
	int					m_nGap2;
	int					m_nGap3;
	int					m_nGap4;

	bool				m_bShow;

	CTString			m_cszTitle;
	int					m_nTitleStringCode;

	int					m_iClassID; 
	int					m_iPageIndex;	
	
	typedef std::map<int, stRankingViewSubTitle> MAP_SUBTITLE;
	typedef MAP_SUBTITLE::iterator MAP_SUBTITLE_IT;

	MAP_SUBTITLE		m_mapSubTitle;

	int					m_iListCount;
	int					m_nListHight;
	int					m_nTitleHeight;
	int					m_nSubTitleHeight;
	int					m_nBtnSpace;			//��ư ũ��� �Ʒ����� ��
	int					m_nTitleSubTitleGap;
	int					m_nBoarderListGap;

	COLOR				m_RankColor[5];
public:	
	
	// Create
	virtual void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, int iID );	
	
	// Į���� �� ���� 
	virtual void	SetColNum ( int iCount );

	// �ڽ��� id������ 
	virtual void SetClassID ( int nID )
	{
		m_iClassID = nID;
	}
	
	// ����
	virtual void SetTitleName ( CTString cszTitle, int iStringCode );
	//�� Į���� ����
	virtual void SetEachRowTitleName ( int iRow, CTString cszTitle, int iStringCode );
	// �� ���� ũ��
	virtual void	SetEachRowWidth ( int iRow, int iCharCount );
	int				GetEachRowWIdth ( int nRow );
	// Render
	void	Render();
	void	RenderList();		
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Messages
	//WMSG_RESULT	KeyMessage( MSG *pMsg );
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Toggle visible of web board
	void	ToggleVisible();

	// �з� â ���̱� 
	void	Open();
	// �з� â �ݱ�
	void	Close();
	
	// ����Ʈ�� ��� �߰� �ϱ�
	virtual void AddListContents ( int icol, int iRow,  CTString cstrName, COLOR cColor = 0xffff00ff );	

	// ���������� ��û
	void	SendNextPage ();

	void	SetEnableNextPageBtn ( bool bEnable )
	{
		if ( !bEnable )
		{
			m_iPageIndex = -1;
			m_btnMore.SetEnable( FALSE );
		}
		else
		{
			m_iPageIndex = 0;
			m_btnMore.SetEnable( TRUE );
		}
	}

	const int GetColumnCount()
	{
		return m_lbListContent.GetColumnCount();
	}

	void	AddListCount ( int nCount )
	{
		//m_lbListContent.AddReserveColumn(nCount);
		m_iListCount += nCount;
	}
	const int &GetListCount () 
	{
		return m_iListCount;
	}

	const COLOR& GetRankColor ( int nRank)
	{
		int nTempRank = max ( nRank - 1, 0 );

		if ( nTempRank < 5 )
			return m_RankColor[nTempRank];
		return m_RankColor[4];
	}
	void SetRowAlign ( int nROW , TEXT_ALIGN align );	

	// esc closing support [5/30/2011 rumist]
	BOOL		CloseWindowByEsc()			{ Close();	return TRUE;				}
};

#endif
