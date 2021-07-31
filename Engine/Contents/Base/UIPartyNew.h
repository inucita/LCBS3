#ifndef	_UIPARTY_NEW_H_
#define	_UIPARTY_NEW_H_

#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include "CommunityDefines.h"

// Define size of option
#define	PARTY_WIDTH					161
#define	PARTY_HEIGHT				57
#define	PARTY_STRETCH_HEIGHT		58
#define	PARTY_BAR_WIDTH				99

enum eUI_TYPE
{
	eUI_PARTY = 0, 
	eUI_EXPEDITION, // ������
};

class CUIImage;
class CUIText;
class CUIButton;
class CUIList;
class CUIImageArray;
class CUIIcon;

// ������ �׷� â
class CUIExpGroup : public CUIWindow
{
public:
	CUIExpGroup();

	void	initialize();

	void	UpdateUI();
	void	openUI();
	void	closeUI();

	void	UpdateMemberInfo(int nPos);
	void	SetGroupNum(int nGroupNum) { m_nGroupNum = nGroupNum; }
	
	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nGroupNum;

	CUIImage*	m_pMoveArea;
	CUIText*	m_pstrTitle;
	CUIButton*	m_pBtnClose;
	CUIList*	m_pList;
};

class CUIParty : public CUIWindow
{
public:
	CUIParty();

	void	initialize();

	void	open();
	void	close();

	void	Init();
	void	InitExpedition();	// ������ �ʱ�ȭ
	
	void	OnPostRender(CDrawPort* pDraw);
	void	OnUpdatePosition();
	// Internal functions
	void	UpdateParty(); // ��Ƽ ������Ʈ
	void	UpdateExpedition(); // ������ ������Ʈ
	void	UpdateGroupMemberInfo(int nGroup, int nPos);
	void	UpdateMemberInfo( int nMemIndex, bool bExpedition = false, int nItemIdx = -1 ); // ��� HPMP ����
	void	UpdatePartyBuff( int nMemIndex, int nX, int nY ); // ���� ������Ʈ

	// Toggle visible
	void	ToggleVisible();
	// Hide party buff
	void	ShowPartyBuff() { m_bShowBuff = TRUE; }
	void	HidePartyBuff() { m_bShowBuff = FALSE; }
	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	// Set Expedition Info
	void	OpenExpeditionMemberWindow();	// ������� ����â OPEN
	void	OpenExpeditionManageWindow();	// ������� ����â OPEN
	void	OpenDivisionInfoWindow();		// �й� ����â�� ����

	COLOR	GetJobStringColor(SBYTE sbJob);									// ������ �ؽ�Ʈ ���� ��ȯ
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnRButtonDown(UINT16 x, UINT16 y);
protected:
	// Info	
	BOOL				m_bShowBuff;								// iF buff is shown or not	
	CUIExpGroup*		m_pUIExpGroup[EXPEDITION_GROUP_MAX];			// ������ ����UI
	CUIList*			m_pListParty;
	CTString			m_strDivisionType[3];
	CTString			m_strExpeditionType[4];

	bool				m_bDrag;
	int					m_nOriX, m_nOriY;
	CUIBase*			m_pDragArea;
};


#endif	//	_UIPARTY_NEW_H_