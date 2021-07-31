//------------------------------------------------------------------
//
// LCE System
// ������ ��ü�ϴ� �����κ��� ���� �޴� ��� �������� �������� ����ȴ�.
#ifndef UI_LCE_SYSTEM_H_
#define UI_LCE_SYSTEM_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

#define LCE_UI_INFO_START_X		0
#define	LCE_UI_INFO_START_Y		0
#define	LCE_UI_INFO_WIDTH		750
#define	LCE_UI_INFO_HEIGHT		491
#define MAX_ITEM_LIST			10

enum eMsgType
{
	MSG_TYPE_RECV = 0,
	MSG_TYPE_RECVALL = 1,
	MSG_TYPE_DELETE = 2,
	MSG_TYPE_DELETEALL = 3
};

class ExpressSystem;
class CUIExpressItemList;

class ENGINE_API CUIExpressSystem : public CUIWindow
{
public:
	CUIExpressSystem(void);
	virtual ~CUIExpressSystem(void);

	//------------------------Interface--------------------------------
	// �ͽ������� ������ ����Ÿ�� ����
	bool		SetLinkData();
	
	void		ClearItemList();
	void		ClearExpress();
	
	// ������ ����Ʈ ������Ʈ
	void		UpdateExpressItemList();

	void		SetItemCount(UBYTE nCount)				{ m_nItemCount = nCount; }
	
	void		OpenExpressSystem();
	void		CloseExpressSystem();
	//------------------------------------------------------------------
	// overriding func. (virtual func.)
	void		Create(CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight);

	void		Render();

	void		ResetPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);
	void		AdjustPosition(PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ);

	// Message pump.
	WMSG_RESULT MouseMessage(MSG* pMsg);
	BOOL		CloseWindowByEsc();
	// Message Handler.
	void		MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
protected:
	// UI ��ġ ����
	int		_GetBtnMiddleSize(CTString strText, int nCurrentSize);
	// UI ��ư ����
	void	_SetBtnRect(int _nMiddleWidth);
	void	_SetPage();

	//-----------------------------------------------------
	// �ͽ������� ������ ����Ʈ
	bool	_SetItemList();
	bool	_SetItemListString(int nIndex, bool bItem);

	CTString _GetSenderType( short SendType );
	
	// �ͽ������� ����Ʈ ����
	void	_ExpressRender();

	//-----------------------------------------------------
	// ���� üũ
	void	_FailReset();
	// �޽��� �ڽ� üũ
	void	_CheckExistMsgBox();
	// �޽��� �ڽ� ����
	void	_CreateMsgBox(eMsgType msgType);

private:
	UIRect						m_rcTitle;
	UIRect						m_rcArea;
	UIRect						m_rcBottom;
	UIRect						m_rcLeft, m_rcMiddel, m_rcRight;
	UIRectUV3					m_rtIdleBtn, m_rtClickBtn, m_rtDisableBtn;

	CUIRectSurface				m_rtsFrame;			// Base
	CUIRectSurface				m_rtsSelectFrame;	// Select Bar
	
	CUIButton					m_btnRecv;			// �ޱ� ��ư
	CUIButton					m_btnAllRecv;		// ��� �ޱ� ��ư
	CUIButton					m_btnDelete;		// �ޱ� ��ư
	CUIButton					m_btnAllDelete;		// ��� �ޱ� ��ư
	CUIButton					m_btnClose;			// �ݱ� ��ư
	CUIButton					m_btnNextPage;		// ���� ������
	CUIButton					m_btnPrevPage;		// ���� ������

	CTextureData*				m_ptdLCETexture;
	ExpressSystem*				m_pExPressData;		// �ͽ������� ����Ÿ ����
	bool						m_bItemList;		// ������ ����Ʈ �߰� ���� ����	
	bool						m_bPrevPage;		// ���� �������� �ִ� ����
			
	UBYTE						m_nItemCount;		// ������ ����
	int							m_SelectItem_index; // ���õ� ������ �ε���
	CTString					m_strCurPage;		// ���� ������

	CUIExpressItemList*			m_pExpressItemList[MAX_ITEM_LIST];

	// UI ����Ʈ �÷� �޴�
	CTString					m_strTitle;
	CTString					m_strCulumName;
	CTString					m_strCulumQuantity;
	CTString					m_strCulumAmount;
	CTString					m_strCulumSender;
	CTString					m_strCulumType;
	CTString					m_strCulumTime;
};
#	endif	// UI_LCE_SYSTEM_H_
