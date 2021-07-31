// ***************************************************************
//  UISocketSystem   version:  1.0   ? date: 04/12/2010
//  -------------------------------------------------------------
//  Author	: rumist
//  Desc	: ui windows class about socket system.
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#ifndef UISOCKETSYSTEM_H__
#define UISOCKETSYSTEM_H__
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIProgressBar.h>
#include <Engine/Interface/UIImageBox.h>

#define ITEM_MAX_COUNT			9
#define JEWEL_MAX_COUNT			7

#define DEF_TARGET_ITEM			0
#define DEF_PREVENT_ITEM_SLOT	8

#define JEWEL_COMPOS_SCROLL_ITEM_INDEX		9816
#define CHAOSJEWEL_COMPOS_SCROLL_ITEM_INDEX  9817

class CUIProgressBar;

class CUISocketSystem : public CUIWindow
{
	typedef	void (CUISocketSystem::*pReq)();
	typedef void (CUISocketSystem::*pRender)();
	typedef WMSG_RESULT	(CUISocketSystem::*pMsgProc)( MSG* pMsg );
public:
	enum _SOCKET_STAT
	{
		STAT_CREATE = 0,			// punched item.
		STAT_COMBINE,				// item combines with jewels.
		STAT_JEWEL_COMPOS,
		STAT_CHAOSJEWEL_COMPOS,
		STAT_EMPTY,					// empty socket slot. destroy jewels.
		STAT_ONCEEMPTY,
		STAT_END,
	};

	enum _SOCKET_ERROR_TYPE
	{
		SOCKET_ERROR_INEXACT = 0,	// button type is miss matched in socket slot.
		SOCKET_ERROR_EMPTY,			// equip button is empty.
		SOCKET_ERROR_EXCEED,		// slot have exceeded about socket count.
		SOCKET_ERROR_ITEM_LACK,		// lacking item count.
		SOCKET_ERROR_JEWEL_OVERLAP,	// check jewel type about equipment.
		SOCKET_ERROR_COMBINE_TERMS, // ���� ���ǿ� ���� ���� ���� �Դϴ�. 
		SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT,	// ���� ���Ž�, ���ŵ� ������ ���� �κ��丮 ������ ������� ���� ���.
	};
	enum _JEWEL_ERROR_TYPE
	{
		JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL,	// �Ϲ� ������ ��� �ϼ���.
		JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL,		// ī���� ������ ��� �ϼ���.
		JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL,	// ����� �� ���� ������ �Դϴ�.
		JEWEL_COMPOS_ERROR_NOT_INVENTORY,		// �ռ��ϴ� ������ �����ϴ�.
		JEWEL_COMPOS_ERROR_JEWEL_LACK,			// ������ �����մϴ�.
		JEWEL_COMPOS_ERROR_NAS_LACK,			// ������ �����մϴ�.
		JEWEL_COMPOS_ERROR_COMPOS_SCROLL,		// ���� �ռ���ũ���� �߸� �Ǿ����ϴ�.
		JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY, // �κ��丮 ������ �����մϴ�.
	};

	enum _JEWEL_ACTION_STATE
	{
		JEWEL_COMPOS_ACTION_IDLE = 0, 
		JEWEL_COMPOS_ACTION_NAS_LACK = 1, // ��������
		JEWEL_COMPOS_ACTION_READY = 2, // �غ�Ϸ�
		JEWEL_COMPOS_ACTION_DIRECTION = 3, // ����
	};

	enum eJEWEL_ITEMBTN_NAME
	{
		JEWEL_COMPOS_BTN_JEWEL_1 = 0,
		JEWEL_COMPOS_BTN_JEWEL_2 = 1,
		JEWEL_COMPOS_BTN_JEWEL_3 = 2,
		JEWEL_COMPOS_BTN_SCROLL = 3,
		JEWEL_COMPOS_BTN_JEWEL_RESULT = 4,
		JEWEL_COMPOS_BTN_JEWEL_MAX,
		JEWEL_COMPOS_JEWEL_MAX = JEWEL_COMPOS_BTN_SCROLL,
		JEWEL_COMPOS_BTN_RESET_MAX = JEWEL_COMPOS_BTN_JEWEL_RESULT,
	};

	enum eBTN_EMPTY_GUIDE
	{
		BTN_EMPTY_JEWEL1,
		BTN_EMPTY_JEWEL2,
		BTN_EMPTY_JEWEL3,
		BTN_EMPTY_SCROLL,
		BTN_EMPTY_RESULT,
		BTN_EMPTY_NASBAR,
		BTN_EMPTY_MAX,
	};
	
	enum eJEWEL_SOUND_TYPE
	{
		JEWEL_SOUND_DULL,
		JEWEL_SOUND_REFINEMENT,
		JEWEL_SOUND_SHINY,
	};

	enum eJEWEL_COMPOS_TYPE
	{
		GENARAL_JEWEL_COMPOS = 0,
		CHAOS_JEWEL_COMPOS = 1,
	};

	enum _SOCKET_CREAT_STAT
	{
		SOCKET_CREATE_IDLE = 0,
		SOCKET_CREATE_SUCCESS,
		SOCKET_CREATE_FAIL,
	};

	// definition socket system ui positions.
	enum __tagSocketSystem_UI
	{
		SOCKET_UI_FRAME_WIDTH				= 172,
		SOCKET_UI_FRAME_HEIGHT				= 283,
		SOCKET_UI_FRAME_TITLE				= 37,
		SOCKET_UI_FRAME_BORDER				= 6,
		SOCKET_UI_FRAME_INTERVAL			= 20,
		SOCKET_UI_FRAME_BOTTOM				= 15,
		SOCKET_UI_INNERFRAME_WIDTH			= 162,	// �ϴ� 
		SOCKET_UI_INNERFRAME_HEIGHT			= 120, /*108,*/
		SOCKET_UI_BTN_CONFIRM_WIDTH			= 92,
		SOCKET_UI_BTN_CONFIRM_HEIGHT		= 21,
		SOCKET_UI_BTN_CLOSE_SIZE			= 16,
		SOCKET_UI_BTN_CLOSE_GAP				= 4,
		SOCKET_UI_BTN_CLOSE_POS_X			= SOCKET_UI_FRAME_WIDTH - SOCKET_UI_BTN_CLOSE_GAP * 2 - SOCKET_UI_BTN_CLOSE_SIZE,
		SOCKET_UI_BTN_CLOSE_POS_Y			= SOCKET_UI_BTN_CLOSE_GAP,			
		SOCKET_UI_TEX_FRAME_WIDTH			= 172,
		SOCKET_UI_TEX_FRAME_HEIGHT			= 228,
		SOCKET_UI_TEX_CREATE_X				= 177,
		SOCKET_UI_TEX_CREATE_Y				= 0,
		SOCKET_UI_TEX_COMBINE_X				= 177,
		SOCKET_UI_TEX_COMBINE_Y				= 121,
		SOCKET_UI_TEX_WARNING_X				= 340,
		SOCKET_UI_TEX_WARNING_Y				= 121,
		SOCKET_UI_TEX_UPGRADE_X				= 340,
		SOCKET_UI_TEX_UPGRADE_Y				= 0,
		SOCKET_UI_TEX_COMPLITE_X			= 341,
		SOCKET_UI_TEX_COMPLITE_Y			= 173,
		SOCKET_UI_TEX_UNKNOWN_X				= 377,
		SOCKET_UI_TEX_UNKNOWN_Y				= 173,
		EX_SOCKET_UI_FRAME_WIDTH				= 173,
		EX_SOCKET_UI_FRAME_HEIGHT				= 397,
		EX_COMBINE_UI_FRAME_HEIGHT				= 424,
		EX_SOCKET_UI_FRAME_TITLE				= 35,
		EX_SOCKET_UI_FRAME_INTERVAL				= 12,
		EX_SOCKET_UI_FRAME_BOTTOM				= 12,
		EX_SOCKET_UI_INNERFRAME_WIDTH			= 161,	 
		EX_SOCKET_UI_INNERFRAME_HEIGHT			= 233, 
		EX_COMBINE_UI_INNERFRAME_HEIGHT			= 260, 
		EX_SOCKET_UI_TEX_FRAME_WIDTH			= 173,
		EX_SOCKET_UI_TEX_FRAME_HEIGHT			= 397,
		EX_COMBINE_UI_TEX_FRAME_HEIGHT			= 424,
		EX_SOCKET_UI_TEX_COMBINE_X				= 6,
		EX_SOCKET_UI_TEX_COMBINE_Y				= 34,
		JEWEL_COMPOS_UI_FRAME_WIDTH			= 193,
		JEWEL_COMPOS_UI_FRAME_HEIGHT		= 308,
		JEWEL_COMPOS_UI_FRAME_TITLE			= 34,
		JEWEL_COMPOS_UI_FRAME_INTERVAL_X	= 17,
		JEWEL_COMPOS_UI_FRAME_INTERVAL_Y	= 24,
		JEWEL_COMPOS_UI_FRAME_BOTTOM		= 10,
		JEWEL_COMPOS_UI_FRAME_BORDER_X		= 17,
		JEWEL_COMPOS_UI_FRAME_BORDER_Y		= 35,
		JEWEL_COMPOS_UI_INNERFRAME_WIDTH	= 159,	 
		JEWEL_COMPOS_UI_INNERFRAME_HEIGHT	= 173, 
		JEWEL_COMPOS_UI_TEX_INNERFRAME_X	= 17,
		JEWEL_COMPOS_UI_TEX_INNERFRAME_Y	= 35,
		CHAOSJEWEL_COMPOS_UI_TEX_INNERFRAME_X	= 0,
		CHAOSJEWEL_COMPOS_UI_TEX_INNERFRAME_Y	= 333,
		JEWEL_COMPOS_UI_TEX_FRAME_X			= 0,
		JEWEL_COMPOS_UI_TEX_FRAME_Y			= 0,
		JEWEL_COMPOS_UI_TEX_FRAME_WIDTH		= 193,
		JEWEL_COMPOS_UI_TEX_FRAME_HEIGHT	= 308,
		JEWEL_COMPOS_BTN_JEWEL1_POS_X		= JEWEL_COMPOS_UI_FRAME_BORDER_X + 16,
		JEWEL_COMPOS_BTN_JEWEL1_POS_Y		= JEWEL_COMPOS_UI_FRAME_BORDER_Y + 22,
		JEWEL_COMPOS_BTN_JEWEL2_POS_X		= JEWEL_COMPOS_UI_FRAME_BORDER_X + 63,
		JEWEL_COMPOS_BTN_JEWEL2_POS_Y		= JEWEL_COMPOS_UI_FRAME_BORDER_Y + 22,
		JEWEL_COMPOS_BTN_JEWEL3_POS_X		= JEWEL_COMPOS_UI_FRAME_BORDER_X + 110,
		JEWEL_COMPOS_BTN_JEWEL3_POS_Y		= JEWEL_COMPOS_UI_FRAME_BORDER_Y + 22,
		JEWEL_COMPOS_BTN_SCROLL_POS_X		= JEWEL_COMPOS_UI_FRAME_BORDER_X + 110,
		JEWEL_COMPOS_BTN_SCROLL_POS_Y		= JEWEL_COMPOS_UI_FRAME_BORDER_Y + 81,
		JEWEL_COMPOS_BTN_RESULT_POS_X		= JEWEL_COMPOS_UI_FRAME_BORDER_X + 63,
		JEWEL_COMPOS_BTN_RESULT_POS_Y		= JEWEL_COMPOS_UI_FRAME_BORDER_Y + 126,
		JEWEL_COMPOS_BTN_CONFIRM_POS_X		= JEWEL_COMPOS_UI_FRAME_WIDTH / 2 - SOCKET_UI_BTN_CONFIRM_WIDTH / 2,
		JEWEL_COMPOS_BTN_CONFIRM_POS_Y		= JEWEL_COMPOS_UI_FRAME_HEIGHT - SOCKET_UI_BTN_CONFIRM_HEIGHT - JEWEL_COMPOS_UI_FRAME_BOTTOM,
		JEWEL_COMPOS_BTN_CLOSE_POS_X		= JEWEL_COMPOS_UI_FRAME_WIDTH - SOCKET_UI_BTN_CLOSE_GAP * 2 - SOCKET_UI_BTN_CLOSE_SIZE,
		JEWEL_COMPOS_BTN_CLOSE_POS_Y		= SOCKET_UI_BTN_CLOSE_POS_Y,
		JEWEL_COMPOS_MSG_WIDTH				= 180,
		JEWEL_COMPOS_MSG_HEIGHT				= 30,
		JEWEL_COMPOS_MSG_POS_X				= (JEWEL_COMPOS_UI_FRAME_WIDTH - JEWEL_COMPOS_MSG_WIDTH) / 2,
		JEWEL_COMPOS_MSG_GAP				= 5,
		JEWEL_COMPOS_MSG_POS_Y				= JEWEL_COMPOS_UI_FRAME_BORDER_Y + JEWEL_COMPOS_UI_INNERFRAME_HEIGHT + JEWEL_COMPOS_MSG_GAP,
		JEWEL_COMPOS_NAS_TEX_FROM_RIGHT		= 22,
		JEWEL_COMPOS_NAS_TEX_FROM_BOTTOM	= 45,
		JEWEL_COMPOS_NAS_POS_X				= JEWEL_COMPOS_UI_FRAME_WIDTH - JEWEL_COMPOS_NAS_TEX_FROM_RIGHT,
		JEWEL_COMPOS_NAS_POS_Y				= JEWEL_COMPOS_UI_FRAME_HEIGHT - JEWEL_COMPOS_UI_FRAME_BOTTOM - JEWEL_COMPOS_NAS_TEX_FROM_BOTTOM,
		JEWEL_COMPOS_GAP_SCROLLPAY_X		= 3,
		JEWEL_COMPOS_GAP_SCROLLPAY_Y		= JEWEL_COMPOS_GAP_SCROLLPAY_X,
		JEWEL_COMPOS_NASBAR_WIDTH			= 160,
		JEWEL_COMPOS_NASBAR_HEIGHT			= 21,
		JEWEL_COMPOS_NASBAR_POS_X			= JEWEL_COMPOS_NAS_POS_X - JEWEL_COMPOS_NASBAR_WIDTH,
		JEWEL_COMPOS_NASBAR_POS_Y			= JEWEL_COMPOS_NAS_POS_Y,
		JEWEL_COMPOS_EMPTY_GUIDE_WIDTH		= 20,
		JEWEL_COMPOS_DIRECTION_TIME			= 2000,
		JEWEL_COMPOS_MAX_SCROLL_PAY			= 99,
		JEWEL_COMPOS_CHANGE_JEWEL_EFFECT_TIME = 50,
	};

	// ���� & ���Ž� �ʿ��� ���� ����.
	static const float					PRICE_CREATE;
	static const float					PRICE_CLEAR;

protected:
	_SOCKET_STAT				m_euSocketStat;
	CTextureData*				m_ptdButtonTexture;
	// SOCKET_SYSTEM_S2 ����, ���� ���� UI Tex �߰� [4/3/2013 Ranma]
	CTextureData*				m_ptdSocketCombineTexture;
	CTextureData*				m_ptdSocketOnceEmptyTexture;
	CTextureData*				m_ptdSocketJewelComposTexture;
	CUIRectSurface				m_rtsJewelCompos;
	CUIRectSurface				m_rtsChaosJewelCompos;
	CUIRectSurface				m_rtsResultJewel;
	CUIMultiLineText			m_msgJewelcomposIdle;		// �Ϲ� ���� �غ� ����
	CUIMultiLineText			m_msgChaosJewelcomposIdle;	// ī���� ���� �غ� ����
	CUIMultiLineText			m_msgJewelcomposReady;		// �غ� �Ϸ�
	CUIMultiLineText			m_msgJewelcomposNaslack;	// ���� ����
	CUIRectSurface				m_rtsCreate;
	CUIRectSurface				m_rtsNeedMoney;
	CUIRectSurface				m_rtsCombine;
	CUIRectSurface				m_rtsOnceEmpty;
	CUIRectSurface				m_rtsWarning;
	CUIRectSurface				m_rtsUpgrade;
	CUIRectSurface				m_rtsComplite;
	CUIRectSurface				m_rtsUnknown;

	CUIRectSurface				m_rtsCombineSocket; // ���� ���Ծ����� ���� ���, ��Ȱ��, ����, ��� [4/4/2013 Ranma]

	UIRect						m_rcTitle;
	UIRect						m_rcArea;
	UIRect						m_rcTop;
	UIRect						m_rcMiddle;
	UIRect						m_rcBottom;
	// texture uv pos
	UIRectUV					m_rtTop;
	UIRectUV					m_rtMiddle;
	UIRectUV					m_rtBottom;
	UIRectUV					m_rtItemSlot;
	UIRectUV					m_rtSocketBase;
	CUIButton					m_btnConfirm;
	CUIButton					m_btnClose;

	CUIIcon*					m_pIconsItem[ITEM_MAX_COUNT];
	CUIDrawBox					m_bxToolTip;
	CTString					m_strToolTip;
	SQUAD						m_sqNeedMoney;
	CUIMultiLineText			m_msgCombineWarning;
	CUIIcon*					m_pIconEmpty;
	SBYTE						m_nEmptyJewelPos;
	CUIMultiLineText			m_msgCombineSuccess;
	CUIMultiLineText			m_msgCombineFail;
	CUIMultiLineText			m_msgCombineFail2;
	BOOL						m_bDefendScroll;

	CUIImageBox					m_CombineEmptyBtnGuide;						// �󽽷� ���̵� ��ư
	BOOL						m_bCombineEmptyBtnGuide;					// �󽽷� ���̵��� ���� ����
	//Popup
	void						_CreateEmptyGuideCombinePopup();			// �󽽷� ���̵� ����
	void						_CheckEmptyGuideCombine();					// �󽽷� ���̵� ���� ���� üũ
	void						_RenderEmptyGuideCombine();					// �󽽷� ���̵� ����
	void						_ResetEmptyGuideCombine();					// �󽽷� ���̵� ����

	_JEWEL_ACTION_STATE			m_eJewelActionState;		// ���� ���̵� ���� ����
	COLOR						m_colorJewelcomposNasStr;	// ���� �÷�
	int							m_nOneMoreComposScrollpay;	// �ѹ��� �ռ��� ���Ǵ� ��ũ�� ����
	CUIImageBox					m_ImageJewelComposEmptyBtnGuide[BTN_EMPTY_MAX]; // �󽽷� ���̵� ��ư
	bool						m_bJewelComposEmptyBtnGuide[BTN_EMPTY_MAX]; // �󽽷� ���̵��� ���� ���� 
	BOOL						m_bStartDirection;			// �ռ����� ����
	BOOL						m_bBtnTranslucency;			// ��ư ������ȭ
	DWORD						m_dwStartTime;				// �ռ����� ���� �ð�
	DWORD						m_dwChanegeTime;			// �ռ����⿡�� ���� ���� �ð�
	_vecJewelInfo				m_DirectionInfo;			// �ռ����⿡ ���Ǵ� ���� ����
	_vecJewelInfo::iterator		m_iterChanegeInfo;			// �ռ����⿡�� ����� ���� ������ ����Ű�� iterator
	UBYTE						m_DirectionType;			// ���� UIType
	CUIMultiLineText			m_msgUpgradeText;
	CUIMultiLineText			m_msgEmptyText;
	CUIMultiLineText			m_msgOnceEmptyText;
	CUIProgressBar				m_pbBar;
	
	BOOL						m_bShowToolTip;
	BOOL						m_bPickTitle;
	BOOL						m_bDrawStatusText;
	BOOL						m_bActionStatus;
	BOOL						m_bButtonLock[ITEM_MAX_COUNT]; // ������ �̹� ������ �־� ���Ǵ� ���
	BOOL						m_bItemButtonLock[ITEM_MAX_COUNT]; // �ٸ� ������ ������ ���� �ϰ� �־� �� ��ư�� ���� ��󿡼� ���� �Ҷ� ����ư
	int							m_nHeightGap;
	int							m_nOrgWidth;
	int							m_nOrgHeight;
	int							m_nSelSlotItemID;

	int							m_nSelCleanItemID;

	int							m_nNPCIndex;
	FLOAT						m_fNPCPosX;
	FLOAT						m_fNPCPosZ;
	int							m_nNPCVIdx;

	BOOL						m_bUseTimer;
	__int64						m_uStartTime;
	__int64						m_uEndTime;
	pReq						m_fnReq;
	pRender						m_fnRender;
	pMsgProc					m_fnMsgProc;
	
	// SOCKET_SYSTEM_S2 ��Ĺ ���� Į�� [4/1/2013 Ranma]
	COLOR						m_ColorStr[JEWEL_MAX_COUNT];
protected:
	void						_toggleVisible( const _SOCKET_STAT _status = STAT_CREATE );

	void						_close();
	void						_reset();

	void						_setUIRect(const _SOCKET_STAT _status, const int _nHeight = 0 );
	void						_setUIUVRect(const _SOCKET_STAT _status);

	void						_setCreateInfo();
	void						_setCombineInfo();
	void						_setEmptyInfo();
	// ����
	void						_showErrorSystemMessage( const _JEWEL_ERROR_TYPE type); // �����޽��� ���
	void						_PlaySound(eJEWEL_SOUND_TYPE playSound);				// ���� �÷���
	void						_setJewelcomposBtn();									// ��ưUI ��ġ �¾�
	void						_setActionCheck();										// ���̵� ���� üũ
	void						_SetJewelcomposNas();									// �Ҹ� ���� ����
	BOOL						_CheckErrorCompos(eJEWEL_COMPOS_TYPE composType);		// �ռ��� ���� üũ
	BOOL						_CheckOneMoreComposScrollpay();							// �ռ� ��ũ�� �Ҹ� ���� ǥ�� �� üũ
	void						_ResetJewelCompos();									// ���� ����
	void						_CheckUiItemslot();										// ���� ������Ʈ
	// �Ϲ� ���� �ռ�
	void						_setJewelcomposInfo();  
	void						_JewelRender();
	WMSG_RESULT					_JewelMouseMessage( MSG *pMsg );
	// ī���� ���� �ռ�
	void						_setChaosJewelcomposInfo();
	void						_ChaosJewelRender();
	WMSG_RESULT					_ChaosJewelMouseMessage( MSG *pMsg );	
	//Popup
	void						_CreateEmptyGuidePopup(eJEWEL_COMPOS_TYPE composType);// �󽽷� ���̵� ����
	void						_CheckEmptyGuide();								// �󽽷� ���̵� ���� ���� üũ
	void						_RenderEmptyGuidePopup();						// �󽽷� ���̵� ����
	void						_ResetEmptyGuide();								// �󽽷� ���̵� ����
	//�ռ�����
	void						_JewelComposDirectionStart(UBYTE StartType);	// �ռ����� ����
	void						_JewelComposDirectionEnd();						// �ռ����� ����
	void						_JewelComposDirection();						// main
	BOOL						_CheckComposDirectionTime();					// �ð� üũ
	BOOL						_JewelComposDirectionGradeInfo(eJEWEL_COMPOS_TYPE composType); // ��� �� ���� ���� ���
	void						_JewelRenderBtnTranslucency();					// ���� ���� ������ȭ
	void						_RenderResultJewel();							// ���� ��� �׵θ� �׸���
	void						_ChangeDirectionEfect();						// �ռ����� ���� ����Ʈ ����
	void						_setEmptyOnceInfo();
	void						_OnceemptyRender();
	WMSG_RESULT					_OnceemptyMouseMessage( MSG *pMsg );
	void						_OnceEmptyMessageBox();
	void						_SetButtonLock(BOOL bOnOff, SBYTE OpnePos = -1);

	void						_UpdateBtn( int nBtnPos );
//	void						_setCardInfo();

	void						_createRender();
	void						_combineRender();
	void						_upgradeRender();
	void						_emptyRender();
//	void						_cardRender();

	void						_showErrorMessage( const _SOCKET_ERROR_TYPE type = SOCKET_ERROR_INEXACT );
	WMSG_RESULT					_createMouseMessage( MSG *pMsg );
	WMSG_RESULT					_combineMouseMessage( MSG *pMsg );
	WMSG_RESULT					_upgradeMouseMessage( MSG *pMsg );
	WMSG_RESULT					_emptyMouseMessage( MSG *pMsg );
//	WMSG_RESULT					_cardMouseMessage( MSG *pMsg );

	void						_calcCmdTimer();
protected:
	
public:
	CUISocketSystem();
	~CUISocketSystem();

	//////////////////////////////////////////////////////////////////////////
	// Overriding functions.
	// create
	void						Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )	;
	// render
	void						Render();
	// Adjust position
	void						ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void						AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	BOOL						CloseWindowByEsc() { CloseAllUI(); return TRUE; } ///esc Ű�� ���� [��Ŀ���� ���� ��쿡]
	// Message Procedure.
	WMSG_RESULT					MouseMessage(MSG *pMsg );

	void						MsgBoxLCommand(int nCommandCode, int nResult );
	void						MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void						ToggleVisible( const _SOCKET_STAT _status = STAT_CREATE );
	const BOOL					IsOpenedUI()	const;
	void						CloseAllUI();
	void						absCloseAllUI();

	const FLOAT					GetNPCPosX()	const									{ return m_fNPCPosX;			}
	const FLOAT					GetNPCPosZ()	const									{ return m_fNPCPosZ;			}
	void						GetNPCPos( FLOAT &fNPCPosX, FLOAT &fNPCPosZ )	const	{ fNPCPosX = m_fNPCPosX; fNPCPosZ = m_fNPCPosZ; }

	void						SetNeedMoneyForCreate( const SQUAD _money = 0 );
	//////////////////////////////////////////////////////////////////////////
	// User creation functions.
	ENGINE_API void				OpenSocketSystem( const int iMobIndex, const BOOL bHasQuest, const FLOAT fX, const FLOAT fZ );
	ENGINE_API void				CloseSocketSystem()										{ _close();						}

	void						UpdateCreateUI( const BOOL bSuccess = TRUE );
	void						UpdateCombineUI(const BOOL bSuccess = TRUE, bool bResult = false);
	void						UpdateUpgradeUI( const int nUniqueIdx );
	void						UpdateUI( const SBYTE sbPos, const int nUniqueIdx );
	void						UpdateJewelComposUI( eJEWEL_COMPOS_TYPE UpdateType, const int nUniqueIdx );
	void						ClearProcessing();
	void						CompleteProgress()										{ m_pbBar.CompleteProgress();	}

	//////////////////////////////////////////////////////////////////////////
	// user network message functions.
	void						SendCreateReq();
	void						SendCombineReq();
	void						SendUpgradeReq();
	void						SendInfoReq();
	void						SendCleanAllJewelReq();
	void						SendCleanOneJewelReq(SWORD tab, SWORD inven_idx, SLONG index);

	void						CommandTimer( pReq req, UINT _second = 2 );

private:
	int							IsEmptyInvenSlotCount();	// �κ��丮 ���� �� ������ ī��Ʈ.

};


#endif
