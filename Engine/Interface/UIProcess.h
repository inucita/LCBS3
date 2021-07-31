
// ----------------------------------------------------------------------------
//  File : UIProcess.h
//  Desc : Created by �̱�ȯ
// ----------------------------------------------------------------------------

#ifndef	UIProcess_H
#define	UIProcess_H
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Interface/UIListBox.h>

// Define max char and line of strings
#define	MAX_PROCESS_STRING			4
#define	PROCESS_STRING_CHAR_WIDTH	170

// Define text position
#define	PROCESS_TITLE_TEXT_OFFSETX	25
#define	PROCESS_TITLE_TEXT_OFFSETY	5
#define	PROCESS_DESC_TEXT_SX		18
#define	PROCESS_DESC_TEXT_SY		33

// Define size of Process
#define	PROCESS_TOP_HEIGHT			339
#define	PROCESS_BOTTOM_HEIGHT		41

#define	PROCESS_WIDTH				216
#define	PROCESS_HEIGHT				380

#define PROCESS_SLOT_ROW			4
#define PROCESS_SLOT_ROW_TOTAL		30


// ��Ʈ��ũ �޼����� ���� ������ ����ü 
struct CNeedItems 
{	
	CItemData*	ItemData;
	SWORD		MatTab;
	SWORD		inven_idx;
	__int64		llCount;

	CNeedItems() : ItemData(NULL) {}
};



// ----------------------------------------------------------------------------
// Name : CUIProcess
// Desc : ���� 
// ----------------------------------------------------------------------------
class CUIProcess : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// �ݱ� ��ư 
	CUIButton				m_btnOK;							// ���� ��ư 
	CUIButton				m_btnCancel;						// ��� ��ư 
	
	// Skill buttons
	std::vector< CUIIcon* > m_vecIcons;							// ���� ������

	// Etc ...
	CUIScrollBar			m_sbProcessItem;					// ���� ������ â ��ũ�� ��
	CUIListBox				m_lbPreconditionDesc;				// �ʿ� ���� ���� ����Ʈ �ڽ�
	
	
//	Process Item Info

	int						m_nProcessText;						// ���� ���� ������ �ε���
	SWORD					m_nTab;
	SLONG					m_nInvenIdx;

	int						m_nSelectProcessItem;				// ���� ���õ� ������
	CTString				m_StrProcessType;					// ���� Ÿ��
	int						m_nProcessItemCount;				// ���������� �´� ����ǰ ����
	
	BOOL					m_bSatisfied;						// ������ ����Ѱ�?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// �ʿ� ������ ����

	int						m_nNeedItemCount;					// �ʿ��� ������ ������ ��
		
// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;
	
// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitProcessResult;				// Wait Message

public:
	CUIProcess();
	~CUIProcess();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close Process
	void	Clear ();
	void	InitProcess();

	ENGINE_API	void	OpenProcess( int nItemIndex, int nTab, int inven_idx );
	void				CloseProcess();

// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

// Network message functions ( receive )
	void	ProcessRep( SBYTE sbResult );
	
// Send ...
	void	SendProcessReq();

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );

private:
	
	void	clearIcon();
};


#endif	// UIProcess

