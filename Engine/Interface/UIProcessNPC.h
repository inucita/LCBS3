
// ----------------------------------------------------------------------------
//  File : UIProcessNPC.h
//  Desc : Created by �̱�ȯ
// ----------------------------------------------------------------------------

#ifndef	UIProcessNPC_H_
#define	UIProcessNPC_H_
#ifdef	PRAGMA_ONCE 
#pragma once
#endif

#include <Engine/Entities/ItemData.h>
#include <Engine/Interface/UIProcess.h>

// Define max char and line of strings
#define	MAX_PROCESSNPC_STRING			4
#define	PROCESSNPC_STRING_CHAR_WIDTH	170

// Define text position
#define	PROCESSNPC_TITLE_TEXT_OFFSETX	25
#define	PROCESSNPC_TITLE_TEXT_OFFSETY	5
#define	PROCESSNPC_DESC_TEXT_SX		18
#define	PROCESSNPC_DESC_TEXT_SY		33

// Define size of ProcessNPC
#define	PROCESSNPC_TOP_HEIGHT			339
#define	PROCESSNPC_BOTTOM_HEIGHT		41

#define	PROCESSNPC_WIDTH				216
#define	PROCESSNPC_HEIGHT				380

#define PROCESSNPC_SLOT_ROW			4
#define PROCESSNPC_SLOT_ROW_TOTAL		30


// ----------------------------------------------------------------------------
// Name : CUIProcessNPC
// Desc : ���� 
// ----------------------------------------------------------------------------
class CUIProcessNPC : public CUIWindow
{
protected:
	
	// Controls...
	
	// Button
	CUIButton				m_btnClose;							// �ݱ� ��ư 
	CUIButton				m_btnOK;							// ���� ��ư 
	CUIButton				m_btnCancel;						// ��� ��ư 
	CUIButton				m_btnUP;							// ���� ����
	CUIButton				m_btnDown;							// ���� ����.
	
	// Skill buttons
	typedef std::vector< CUIIcon* >::iterator		vecBtnEX_Iter;
	std::vector< CUIIcon* > m_vecIcons;							// ���� ������
	int						m_nProcessItemCount;				// ������� �ϴ� ��.
	
	// Etc ...
	CUIScrollBar			m_sbProcessNPCItem;					// ���� ������ â ��ũ�� ��
	CUIListBox				m_lbPreconditionDesc;				// �ʿ� ���� ���� ����Ʈ �ڽ�
	
	
	//	ProcessNPC Item Info	
	//int						m_nProcessNPCText;					// ���� ���� ������ �ε���	
	
	int						m_nSelectProcessNPCItem;			// ���� ���õ� ������
	CTString				m_StrProcessNPCType;				// ���� Ÿ��
	int						m_nProcessNPCItemCount;				// ���������� �´� ����ǰ ����
	
	BOOL					m_bSatisfied;						// ������ ����Ѱ�?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// �ʿ� ������ ����
	
	int						m_nNeedItemCount;					// �ʿ��� ������ ������ ��
	
	// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	//UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;
	
	// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button
	UIRectUV				m_rtInput;
	
	// Network ...
	BOOL					m_bWaitProcessNPCResult;				// Wait Message

	int						m_iResourceType;
	int						m_iResourceSubType;
	int						m_iLastResourceType;
	int						m_iLastResourceSubType;
	
public:
	CUIProcessNPC();
	~CUIProcessNPC();
	
	
	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
	// Render
	void	Render();
	
	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	// Open & close ProcessNPC
	void	Clear ();
	void	InitProcessNPC();
	
	ENGINE_API	void	OpenProcessNPC( );
	ENGINE_API	void	OpenProcessList( int iType, int iSubType );
	void				CloseProcessNPC();
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	// Network message functions ( receive )
	void	ProcessNPCRep( LONG processItemDBIndex, LONG count, LONG result );
	
	// Send ...
	void	SendProcessNPCReq();
	
	// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );

	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

private:
	void	clearVecBtnEX();
};


#endif	// UIProcessNPC

