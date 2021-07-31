#ifndef __CITEMCOMPOSE_UI_H__
#define __CITEMCOMPOSE_UI_H__

enum eCOMPOSE_POPUP_TYPE
{
	eCOMPOSE_POPUP_DONT_IN_DUNGEON = 0,
	eCOMPOSE_POPUP_NOT_ENOUGH_NAS,
	eCOMPOSE_POPUP_NOT_ENOUGH_MAT,
	eCOMPOSE_POPUP_HAVE_NOT_RELIC,
	eCOMPOSE_POPUP_NO_COMPOSE_ITEM,
	eCOMPOSE_POPUP_EQUIP_ITEM,
	eCOMPOSE_POPUP_COMPOSE_FAIL,
	eCOMPOSE_POPUP_MAX
};

class CItemComposeUI : public CUIWindow
{
public:
	CItemComposeUI(void);

	enum eSTATE
	{
		eNONE = 0,
		eREGIST,
		ePROGRESSING,
		eCOMPLETE,
		eSTATE_MAX
	};

	enum eBTN_TYPE
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	enum eICON_TYPE
	{
		eICON_MAT1 = 0,
		eICON_MAT2,
		eICON_MAT3,
		eICON_MAT4,
		eICON_MAT5,
		eICON_RESULT,
		eICON_MAX
	};

	enum eCOMPOSE_GUIDE_TYPE
	{
		eGUIDE_BASE = 0,
		eGUIDE_ALL_READY,
		eGUIDE_NOT_ENOUGH_NAS,
		eGUIDE_COMPOSE_OK,
		eGUIDE_COMPOSE_FAIL,
		eGUIDE_MAX
	};

	void openUI();
	void closeUI();
		 
	void PressOK();
	void UpdateResultIcon(int ItemIdx = -1);
	void ShowErrorPopup(int nErrorIdx);
	void ShowGuideString(int nIdx = -1);
	void ClearIcon();
		 
	void initialize();
	void OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	
	// Reset position
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Msg Proc
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnRButtonDown(UINT16 x, UINT16 y);
private:
	void			StartProgress();
	void			SendEvent(); // ���α׷��� �Ϸ�� ������ �̺�Ʈ
	LONGLONG		CalcConsumNas(); // �Һ� ���� ���
	void			CalcProgressTimer();

	void			SlotRegistCheck(); // �󽽷Կ� �������� ��� üũ ù���� ���Ժ��� ü����
	void			UpdateIconBg(); // ������ ��Ͻ� ���� BackGround ������Ʈ
	void			UpdateNasString();
	void			CheckOkBtn(); // Ok��ư enable üũ
	bool			IsComposeItem(int nIdx); //��ϰ����� ���������� üũ 
	bool			IsCheckIcon(int nIndex); // ������ ������� �˻�
	bool			IsRegist(int nSlotIdx, int nItemIdx);
	bool			IsAlreadyRegist(int nVirIndex); // �̹� ��ϵ� ���������� üũ
	
	bool			SetBtnState(eBTN_TYPE eType, BOOL bEnable); // ��ư ���� ����
	void			SetState(eSTATE eState); // UI ���� ����
	
	eSTATE			m_eState; // ���̵� ����
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;
	UIRect			m_rcOriginPos;

	BOOL			m_bProgress; // ���α׷��� ����
	__int64			m_uStartTime;
	__int64			m_uEndTime;
	
	LONGLONG		m_llNas; // �Һ� ���� ǥ�ÿ�
	
	CUIText*		m_pStrTitle;
	CUITextBox*		m_pTbStateGuide;
	CUIImage*		m_pDrag;
	CUIProgressBar* m_pPgBar;
	CUIButton*		m_pBtn[eBTN_MAX];
	CUIIcon*		m_pIcon[eICON_MAX];
	CUIImage*		m_pIconBg[eICON_MAX];
};

#endif	//	__CITEMCOMPOSE_UI_H__
