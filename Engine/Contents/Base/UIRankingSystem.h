#ifndef UI_RANKING_SYSTEM_H_
#define UI_RANKING_SYSTEM_H_

#include "RankingList.h"

class CUIRankingSystem : public CUIWindow
{
public:
	CUIRankingSystem();
	~CUIRankingSystem();

	typedef enum eRankingMouseEventType
	{
		RANKING_MOUSE_NONE = 0,
		RANKING_MOUSE_OVER,
	} MOUSE_EVENT_TYPE;

	typedef enum eRankingListType
	{
		RANKING_LIST_TOP = 0,
		RANKING_LIST_BOTTOM,
	} RANK_LIST_TYPE;
	////// interface ////////////////////////////////////

	//// Get ////
	CRankingList*	GetRankingList()	{ return &m_oRankingList; }
	// ���� Ÿ���� ��Ʈ������ ���
	CTString		GetJobStringByType(UBYTE ubClass);
	/// ��ŷ Ÿ���� ��Ʈ������ ���
	CTString		GetRankStringByType(ERANKINGEX_SUB_TYPE eSubType);
	// 0 : ���������ͷ�ŷ, 1: ������ŷ
	UCHAR			GetRankingNetworkType();							
	UCHAR			GetRankingNetworkSubType();							// 0:(����) , 1:(0 : Ÿ��ź����, 1 : ����Ʈ���� ...)

	//// Update ////
	void		UpdateBottomRanking();
	void		UpdateTopRanking();

	//// Open , close ////
	void		OpenUI();
	void		OpenUI(ERANKINGEX_SUB_TYPE eSubType);
	void		CloseUI();
	void		ToggleVisible();
	//// command ////
	void		SearchName(CTString cstrName);
	void		ChangeViewType(ERANKINGEX_SUB_TYPE eSubType);
	void		ToggleReset();
	void		SetRankingListUnSelect( RANK_LIST_TYPE ListType, COLOR col );
	void		SetRankingListSelect( RANK_LIST_TYPE ListType, COLOR col );
	void		SetRankingListMouseEvent( RANK_LIST_TYPE ListType, CUIListItem* pListItem, COLOR col, MOUSE_EVENT_TYPE EventType );
	void		SetRankingListStringCol( CUIListItem* pListItem, COLOR col );
	int			GetListCurSel( RANK_LIST_TYPE ListType );
	//// reset	////
	void		ClearRankingList();

	//////////////////////////////////////////////////////
	void	initialize();

	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	BOOL	CloseWindowByEsc()			{ CloseUI();	return TRUE; }

	virtual BOOL	IsEditBoxFocused()			
	{ 
		if ( m_pEbSearch )
		{
			return m_pEbSearch->IsFocused();
		}

		return FALSE;
	}
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT	OnKeyMessage(MSG* pMsg);
	WMSG_RESULT OnCharMessage(MSG* pMsg);
	WMSG_RESULT	OnIMEMessage(MSG* pMsg);
	////// network ////////////////////
	void	RecvRankingList(CNetworkMessage& nmMessage);
	BOOL	CheckRecvRankingListAndReq();
private:
	void			SetTitle(CTString cstrTitle);
	void			ChangeComboType();
	void			SetListString(SRankingPointData* pRankData, CUIListItem* pListItem, COLOR color);
	void			SetTopListErrorMsgString(CTString strString = "", BOOL bHide = TRUE);
	void			SetBottomListErrorMsgString(CTString strString = "", BOOL bHide = TRUE);
	
	bool			m_bTitleClick;
	UIRect			m_rcOriginPos;
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	int				m_nLastCurSel;
	INT				m_nRecvRankTotalCount;			// ���۹��� �� �������� ���� (��Ŷ ���� �� ��ŷ �� ��)
	INT				m_nRecvRankCount;				// ���۹��� �������� ���� (��Ŷ ���� ��ŷ �� ��)
	BOOL			m_bRecvRank;					// ���۹޴� ��
	ERANKINGEX_SUB_TYPE		m_eSubType;				// ���������ִ� �з�
	CRankingList			m_oRankingList;
	CTString		m_cstrSearch;					// �˻��� ���̵�
	CTString		m_cstrTopRankingMsg;			// ���κ� ��ŷ����Ʈ �ڽ� �� ǥ�õǴ� �޼���
	CTString		m_cstrBottomRankingMsg;			// �Ʒ��κ� ��ŷ����Ʈ �ڽ� �� ǥ�õǴ� �޼���
protected:
	CUIButton*		m_pbtnClose;
	CUIButton*		m_pbtnSearch;
	CUIButton*		m_pbtnReset;
	CUIList*		m_pTopRankList;
	CUIList*		m_pBottomRankList;
	CUIScrollBar*	m_pTopRankListScroll;
	CUIScrollBar*	m_pBottomRankListScroll;
	CUIImage*		m_pProgressBox;
	CUIProgressBar* m_pProgressBar;
	CUIComboBox*	m_pLankType;
	CUIEditBox*		m_pEbSearch;
	CUIText*		m_pTitle;
	CUIText*		m_pMoveTitle;
	CUIText*		m_pTopRankMsg;
	CUIText*		m_pBottomRankMsg;
};
#endif // UI_RANKING_SYSTEM_H_