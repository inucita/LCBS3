#ifndef	_UIEXPEDITION_NEW_H_
#define	_UIEXPEDITION_NEW_H_

#ifdef	PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Interface/UITrackPopup.h>

#define VIEW_DETAIL_BTN_MAX 14

class CUIImage;
class CUIText;
class CUIButton;
class CUIList;
class CUIImageArray;
class CUIIcon;
class CItems;

// ���캸�� â(��û ���� ����� ������ ������( �ǽð� ����X ))
class CUIViewDetail : public CUIWindow
{
public:
	CUIViewDetail();

	void	initialize();

	void	openUI();
	void	closeUI();

	void	SetCharInfo( CTString strCharInfo, SBYTE sbJob);
	void	SetPetInfo( CNetworkLibrary::sPetInfo petInfo )		{ m_sPetInfo = petInfo;			}
	void	SetWildPetInfo( sPetItem_Info wildPetInfo )			{ m_sWildPetInfo = wildPetInfo; }
	void	SetViewItem(int nWearPos, CItems* pItem);
	void	ClearViewDetail();

	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

private:
	CNetworkLibrary::sPetInfo		m_sPetInfo;		// ���캸�� �� ����
	sPetItem_Info					m_sWildPetInfo;	// ���캸�� ������ �� ����

	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	
	CUIImageArray*	m_pIaBack;
	CUIButton*		m_pbtnClose;
	CUIText*		m_pstrName;
	CUIImage*		m_pMoveArea;
	CUIIcon*		m_pIcon[WEAR_TOTAL];
};

//������ ���� â
class CUIExpManage : public CUIWindow
{
protected:

	//���� â POPUP COMMAND
	enum eManageCommandList
	{
		MANAGE_CONNAMD_CHANGE_LEADER = 0,
		MANAGE_CONNAMD_SET_SUBLEADER,
		MANAGE_CONNAMD_RESET_SUBLEADER,
		MANAGE_CONNAMD_KICK_MEMBER,
		MANAGE_CONNAMD_VIEW_MEMBER,
	};

	CTextureData	*m_ptdMessageBoxTexture;
	
	BOOL SetCommand(SLONG slPosition, BOOL bIsMyGroup = FALSE, BOOL bIsExpeditionLeader = FALSE);
	void RunCommand(INDEX iCommand);

public:
	CUIExpManage();
	~CUIExpManage();
	
	void	initialize();
	void	UpdateUI();
	void	openUI();
	void	closeUI();

	void	ClearExpManage();
	void	UpdateMemberInfo(int nGroup, int nPos);

	void	PressAddBtn();
	void	ShowManageMenu(int nGroup, int nPos);

	void	SetSelectIndex(int nIndex) { m_slDestIndex = nIndex; }
	int		GetSelectIndex()	{ return m_slDestIndex; }
	int		calcHpBar(int nGroup, int nPos, int nWidth);
	void	OnPostRender(CDrawPort* pDraw);

	void	SetSelectBtnGroup(int nIndex) { m_slSeletedBtnGroup = nIndex; }
	void	SetSelectBtnPos(int nIndex) { m_slSeletedBtnPos = nIndex; }
	void	SetBtnMove(bool bMove)	{ m_bCanUseBtn = bMove;	}
	int		GetSelectBtnGroup()	{ return m_slSeletedBtnGroup;	}
	int		GetSelectBtnPos()	{ return m_slSeletedBtnPos;		}
	bool	GetBtnMove()		{ return m_bCanUseBtn;			}
	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	WMSG_RESULT MouseMessage( MSG *pMsg );

private:
	enum eBtnType
	{
		eBTN_CLOSE = 0, // �ݱ� ��ư
		eBTN_ADD,		// ����߰� ��ư
		eBTN_CANCEL,	// ��� ��ư
		eBTN_MAX
	};

	enum eGroupType
	{
		eGROUP_1 = 0,
		eGROUP_2,
		eGROUP_3,
		eGROUP_4,
		eGROUP_MAX
	};

	bool		m_bDrag;
	int			m_nOriX, m_nOriY;
	int			m_nMouseX, m_nMouseY;

	CUIImage*	m_pMoveArea;
	CUIText*	m_pstrTitle;
	CUIText*	m_pStrText[eGROUP_MAX];
	CUIButton*	m_pBtn[eBTN_MAX];
	CUIList*	m_pList[eGROUP_MAX];

	SLONG		m_slDestIndex;			// ���õ� ��� index
	SLONG		m_slSeletedBtnGroup;
	SLONG		m_slSeletedBtnPos;
	CTString	m_strDestName;
	bool		m_bCanUseBtn;
	CUITrackPopup	m_tpList;			// �޴�
};

#endif	//	_UIEXPEDITION_NEW_H_