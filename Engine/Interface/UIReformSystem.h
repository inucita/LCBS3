// UIReformSystem.h: interface for the CUIReformSystem class.
//
//////////////////////////////////////////////////////////////////////

#ifndef UIREFORMSYSTEM_H__
#define UIREFORMSYSTEM_H__
#ifdef PRAGMA_ONCE
#pragma once
#endif

#include <Engine/Interface/UIProgressBar.h>

class CUIReformSystem : public CUIWindow
{
	typedef	void (CUIReformSystem::*pReq)();
public:
	enum _tagReformSystem_UI
	{
			REFORM_UI_FRAME_WIDTH				= 172,
			REFORM_UI_FRAME_HEIGHT				= 313,
			REFORM_UI_FRAME_TITLE				= 37,
			REFORM_UI_FRAME_BORDER				= 6,
			REFORM_UI_FRAME_INTERVAL			= 20,
			REFORM_UI_FRAME_BOTTOM				= 15,
			REFORM_UI_INNERFRAME_WIDTH			= 162,	// �ϴ� 
			REFORM_UI_INNERFRAME_HEIGHT			= 120, /*108,*/
			REFORM_UI_TEX_FRAME_WIDTH			= 172,
			REFORM_UI_TEX_FRAME_HEIGHT			= 228,
			REFORM_UI_TEX_CREATE_X				= 177,
			REFORM_UI_TEX_CREATE_Y				= 0,
			REFORM_UI_TOOLTIPSIZE				= 110,
			REFORM_UI_TOOLTIPGAP				= 6,
	};

	enum eICON_TYPE
	{
		eSLOT_REFORMER,
		eSLOT_MAGNIFIER,
		eSLOT_ITEM,
		eSLOT_MAX
	};

public:
	CUIReformSystem();
	virtual ~CUIReformSystem();

	ENGINE_API void OpenReformSystem(int iMobIndex, FLOAT fX, FLOAT fZ);
	void Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight);
	void Render();
	void Close();
	void SetItemButton();
	void ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	BOOL CloseWindowByEsc() { Close(); return TRUE; } ///esc Ű�� ���� [��Ŀ���� ���� ��쿡]
	// server to client message procedure.
	void RecvReformSystemResult(CNetworkMessage* istr);
	// Reform ������ ������ ��ϵ� ������ ������ �κ��丮���� ���� �� ��츦 check�Ͽ� ���������� ������ ����ش�.
	void SendReformItemBtnInitUniIndex(int uniindex);
	// ���α׷��� �� �߰� [9/12/2012 Ranma]
// 	void ClearProcessing(){};
 	void CompleteProgress()	{ m_pbBar.CompleteProgress();	}
	void MagnifierPutOnCallBack();
	void MagnifierPutOnCancelCallBack();
private:
	int		m_nOrgWidth;
	int		m_nOrgHeight;
	int		m_nHeightGap;
	int		m_nMiddleGap;

	int		m_nItemCount;
protected:
	virtual void	MsgBoxLCommand( int nCommandCode, int nResult );
	void			SetUIRect(const int nHeight = 0);
	void			MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
 	void			_calcCmdTimer(); // ���α׷��� �� �߰� [9/12/2012 Ranma]
 	void			CommandTimer( pReq req, UINT _second = 1 );
	void			SendReformReq();
	void			BtnSetItemCheckforPos(); // ��Ʈ���������� �ƴ����� ���� ��ġ�� ����ִ� �Լ�
 	WMSG_RESULT		MouseMessage(MSG *pMsg);

private:
	void setPrevData(CItems* pItems);

protected:
	CTextureData*				m_ptdButtonTexture;
	int							m_nNPCIndex;
	FLOAT						m_fNPCPosX;
	FLOAT						m_fNPCPosZ;
	int							m_nNPCVIdx;

	BOOL						m_bPickTitle;
	UBYTE						m_nReformType; // 1 =  High reform, 2 = Low reform

	CUIRectSurface				m_rtsReform;
	UIRectUV					m_rtTop;
	UIRectUV					m_rtMiddle;
	UIRectUV					m_rtBottom;
	UIRectUV					m_rtItemSlot;

	UIRect						m_rcTitle;		// UI title pos
	UIRect						m_rcTop;
	UIRect						m_rcMiddle;
	UIRect						m_rcBottom;
	UIRect						m_rcReformSystemRect;
	
	CUIButton					m_btnClose;		//UI close button
	CUIButton					m_btnConfirm;	//--����-- button create

	CUIIcon*					m_pIconsItem[eSLOT_MAX];
	CItems*						m_pItemsPrev;
	
 	CUIProgressBar				m_pbBar;		// ���α׷��� �� �߰� [9/12/2012 Ranma]

	CUITooltip*					m_pTooltip[2];

	BOOL						m_bUseTimer;
	__int64						m_uStartTime;
	__int64						m_uEndTime;
	pReq						m_fnReq;
};

#endif // !defined(AFX_UIREFORMSYSTEM_H__534C3BCC_23EA_40B5_B79E_628ACBF06FA1__INCLUDED_)
