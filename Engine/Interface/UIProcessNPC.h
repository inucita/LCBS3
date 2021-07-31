
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

#define PROCESSNPC_SLOT_ROW			4
#define PROCESSNPC_SLOT_ROW_TOTAL		30


// ----------------------------------------------------------------------------
// Name : CUIProcessNPC
// Desc : ���� 
// ----------------------------------------------------------------------------
class CUIProcessNPC : public CUIWindow
{
protected:
	// Skill buttons
	typedef std::vector< CItems* >::iterator		vecItems_Iter;
	std::vector< CItems* >	m_vecItems;							// ���� ������
	int						m_nProcessItemCount;				// ������� �ϴ� ��.
		
	int						m_nSelectProcessNPCItem;			// ���� ���õ� ������
	
	BOOL					m_bSatisfied;						// ������ ����Ѱ�?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// �ʿ� ������ ����
	
	int						m_nNeedItemCount;					// �ʿ��� ������ ������ ��
	
	// Network ...
	BOOL					m_bWaitProcessNPCResult;				// Wait Message

	int						m_iResourceType;
	int						m_iResourceSubType;
	int						m_iLastResourceType;
	int						m_iLastResourceSubType;
	
public:
	CUIProcessNPC();
	~CUIProcessNPC();

	void initialize();
		
	// Open & close ProcessNPC
	void	Clear ();
	void	InitProcessNPC();
	
	ENGINE_API	void	OpenProcessNPC( );
	ENGINE_API	void	OpenProcessList( int iType, int iSubType );
	void				CloseProcessNPC();

	// SpinCtrl �� �Է� �������� �ʿ�
	BOOL	IsEditBoxFocused() { 
		if (m_nSelectProcessNPCItem >= 0)
			return TRUE;

		return FALSE;
	}
	
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

	void	pushback_string(CTString& strText, COLOR color);
	void	callback_select();

	void	show_spinctrl(int idx, BOOL bhide);

	void	callback_spin();

	CUIList*	m_pListItems;
	CUIList*	m_pListDesc;
	CUIButton*	m_pBtnOK;

	int			m_nNeedTextWidth;		// �ʿ� ������ �ؽ�Ʈ ����
};


#endif	// UIProcessNPC

