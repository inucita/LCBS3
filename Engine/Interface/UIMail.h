// ----------------------------------------------------------------------------
//  File : UIWareHouse.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	UIMAIL_H_
#define	UIMAIL_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Entities/Mail.h>
class CItems;


#define PAGEMAILMAX 8 //���������� �����ִ� ������ ����
#define MAXPAGE 5 //�ִ� ǥ�� ������ ���� 0~5, ������ 48��

#define ITEMBUTTONMAX 1		// ÷�� ������ ������ ������ ����
#define ITEMBUTTONLINEMAX 1 // ���ٿ� � ���� ���� 2�� 5�� �����̳� ���� �������� �ϴ� 1���� �Ѵ�.

#define MAINBACKGROUNDWIDTHTEX 395 //textuere���� ����� ����
#define MAINBACKGROUNDHEIGHTTEX 108 //textuere���� ����� ����

#define MAINBACKGROUNDWIDTH 395 //UI���� ����� ����
#define MAINBACKGROUNDHEIGHT 428 //UI���� ����� ����

#define OPENBACKGROUNDWIDTHTEX 283
#define OPENBACKGROUNDHEIGHTTEX 70

#define OPENBACKGROUNDWIDTH 395
#define OPENBACKGROUNDHEIGHT 428

#define OPENTIME	(84400*15)
#define NOOPENTIME	(84400*15)

#define DAYTIME		84400
#define HOURTIME	3600
#define HALFTIME	1800


namespace TABSTATUS
{
	typedef enum tabName
	{
		BEGIN,
		LIST,
		SEND,
		END
	}TABSTATUS;
}


// ----------------------------------------------------------------------------
// Name : CUIMail
// Desc :
// ----------------------------------------------------------------------------
class CUIMail : public CUIWindow
{
public:
	class CUIOpenMail : public CUIWindow
	{
	protected:
		
		UIRect				m_rcMainTitle;
		
		UIRect				m_rcOpenMailReciver;
		UIRect				m_rcOpenMailTitle;
		UIRect				m_rcOpenMailText;

		UIRect				m_rcOpenMailBackground;
		UIRect				m_rcOpenMailItemBackground;

		UIRect				m_rcOpenMailItemEmpty[ITEMBUTTONMAX];

		UIRect				m_rcOpenMailMoneyIcon;
		UIRect				m_rcOpenMailMoney;



//���� ��׶���
		UIRectUV			m_rtMainBackgroundUL;
		UIRectUV			m_rtMainBackgroundUM;
		UIRectUV			m_rtMainBackgroundUR;
		
		UIRectUV			m_rtMainBackgroundML;
		UIRectUV			m_rtMainBackgroundMM;
		UIRectUV			m_rtMainBackgroundMR;
		
		UIRectUV			m_rtMainBackgroundLL;
		UIRectUV			m_rtMainBackgroundLM;
		UIRectUV			m_rtMainBackgroundLR;
		
		UIRectUV			m_rtBrownGradationRectL;
		UIRectUV			m_rtBrownGradationRectM;
		UIRectUV			m_rtBrownGradationRectR;
//���� ��׶���
		UIRectUV			m_rtBrownBackgroundU;
		
		UIRectUV			m_rtBrownBackgroundML;
		UIRectUV			m_rtBrownBackgroundMM;
		UIRectUV			m_rtBrownBackgroundMR;
		
		UIRectUV			m_rtBrownBackgroundL;
//��� ��׶���
		UIRectUV			m_rtGreenBackguoundU;
		
		UIRectUV			m_rtGreenBackguoundML;
		UIRectUV			m_rtGreenBackguoundMM;
		UIRectUV			m_rtGreenBackguoundMR;
		
		UIRectUV			m_rtGreenBackguoundL;
//���� �׸��� ĭ	
		UIRectUV			m_rtBrownRectR;
		UIRectUV			m_rtBrownRectM;
		UIRectUV			m_rtBrownRectL;

//�� ������ ������
		UIRectUV			m_rtItemCapIcon;
		UIRectUV			m_rtEmptyItemIcon;

		CUIButton			m_btnRetun;
		CUIButton			m_btnReply;
		CUIButton			m_btnDelete;
		CUIButton			m_btnClose;
		CUIButton			m_btnMoney;

		CUIButtonEx			m_btnOpenMailItem[ITEMBUTTONMAX];

		CUIMultiEditBox		m_mebOpenMailText;
		
	public:
		CUIOpenMail();
		~CUIOpenMail();
		
		// Create
		void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
		
		// Render
		void	Render();
		
		// Adjust position
		void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
		
		WMSG_RESULT	MouseMessage( MSG *pMsg );
		WMSG_RESULT	KeyMessage(MSG *pMsg );
		WMSG_RESULT CharMessage(MSG *pMsg );
		WMSG_RESULT	IMEMessage(MSG *pMsg );
		
		BOOL IsEditBoxFocused();
		void KillFocusEditBox();

		void OpenOpenMail();
		void CloseMail();

		CUIButtonEx* GetOpenMailItemButtonEX();
		void CopyiteminfoFromMail(RecvItemDataArray::iterator itr, int i);
		void CopyiteminfoFromMail(ReceiveItemData& ItemData, int i);

		
	private:
	};
protected:

// ���� ����
	UIRect				m_rcMainTitle;

	UIRect				m_rcListTab;
	UIRect				m_rcSendTab;

	CUIButton			m_btnConfirm;
	CUIButton			m_btnCancel;
	CUIButton			m_btnClose;
//���� ��


// ���� ����Ʈ ���� ����
//�׷��� ��Ʈ ��ư ����
	UIRect				m_rcListMailIcon[PAGEMAILMAX];
	UIRect				m_rcListMailInfo[PAGEMAILMAX];

	UIRect				m_rcListMailPage;
	UIRect				m_rcListMailBackground;

// ��ư	
	CUIButton			m_btnFirst;
	CUIButton			m_btnLeft;
	CUIButton			m_btnRight;
	CUIButton			m_btnLast;

	CUIButtonEx			m_btnListMailItem[PAGEMAILMAX];
// ���� ����Ʈ ���� ��

// ���� ���� ���� ����
	
	UIRect				m_rcSendMailTextBackground;
	
	CUIEditBox			m_ebSendMailReciver;
	CUIEditBox			m_ebSendMailTitle;
	CUIEditBox			m_ebSendMailMoney;

	CUIMultiEditBox		m_mebSendMailText;

	CUIButton			m_btnMoney;	
	CUIButtonEx			m_btnSendMailItem[ITEMBUTTONMAX];

	UIRect				m_rcSendMailReciver;
	UIRect				m_rcSendMailTitle;
	UIRect				m_rcSendMailMoney;
	UIRect				m_rcSendMailText;

	UIRect				m_rcSendMailCheckClaim;
	UIRect				m_rcSendMailCheckSend;
	UIRect				m_rcSendMailMoneyIcon;

	UIRect				m_rcSendMailItemEmpty[ITEMBUTTONMAX];
	UIRect				m_rcSendMailItemBackground;

// ���� ���� ���� ��
// TEXTURE ���� ����
	UIRectUV			m_rtEmptyItemIcon;
	UIRectUV			m_rtEmptyMailIcon;

//���� ����â	
	UIRectUV			m_rtBrownRectR;
	UIRectUV			m_rtBrownRectM;
	UIRectUV			m_rtBrownRectL;


	UIRectUV			m_rtTabSelect;
	UIRectUV			m_rtTab;
//���� ��׶���	
	UIRectUV			m_rtMainBackgroundUL;
	UIRectUV			m_rtMainBackgroundUM;
	UIRectUV			m_rtMainBackgroundUR;
	
	UIRectUV			m_rtMainBackgroundML;
	UIRectUV			m_rtMainBackgroundMM;
	UIRectUV			m_rtMainBackgroundMR;
	
	UIRectUV			m_rtMainBackgroundLL;
	UIRectUV			m_rtMainBackgroundLM;
	UIRectUV			m_rtMainBackgroundLR;

	UIRectUV			m_rtBrownGradationRectL;
	UIRectUV			m_rtBrownGradationRectM;
	UIRectUV			m_rtBrownGradationRectR;

	UIRectUV			m_rtItemCapIcon;
//���� ��׶���
	UIRectUV			m_rtBrownBackgroundU;

	UIRectUV			m_rtBrownBackgroundML;
	UIRectUV			m_rtBrownBackgroundMM;
	UIRectUV			m_rtBrownBackgroundMR;

	UIRectUV			m_rtBrownBackgroundL;

	UIRectUV			m_rtMoneyIcon;
	UIRectUV			m_rtCheckboxOn;
	UIRectUV			m_rtCheckboxOff;

	UIRectUV			m_rtGreenBackguoundU;
	
	UIRectUV			m_rtGreenBackguoundML;
	UIRectUV			m_rtGreenBackguoundMM;
	UIRectUV			m_rtGreenBackguoundMR;

	UIRectUV			m_rtGreenBackguoundL;
	//���� �׵θ�
	UIRectUV			m_rtSelectBorderUL;
	UIRectUV			m_rtSelectBorderUM;
	UIRectUV			m_rtSelectBorderUR;
	
	UIRectUV			m_rtSelectBorderML;
	UIRectUV			m_rtSelectBorderMM;
	UIRectUV			m_rtSelectBorderMR;
	
	UIRectUV			m_rtSelectBorderLL;
	UIRectUV			m_rtSelectBorderLM;
	UIRectUV			m_rtSelectBorderLR;

	



// TEXTURE ���� ��



private:
//���� UI
	CUIOpenMail				m_UIOpenMail;

//���� �� ���� ��Ʈ�� ����
	TABSTATUS::TABSTATUS	m_TabStatus;

	int						m_Page;
	int						m_ItemCount;
	int						m_SendMailItemOnSlot;
	int						m_SelectedIndex;

	BOOL					m_bCheckClaim;
	BOOL					m_bMoneyOver;

	SQUAD					m_Money;
	LONG					m_NpcUID;

	FLOAT					m_fNPCPosX;
	FLOAT					m_fNPCPosZ;

	BOOL					m_bSending;

public:
	CUIMail();
	~CUIMail();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	
	WMSG_RESULT	KeyMessage(MSG *pMsg );
	WMSG_RESULT CharMessage(MSG *pMsg );
	WMSG_RESULT	IMEMessage(MSG *pMsg );
	
	BOOL IsEditBoxFocused();
	void KillFocusEditBox();

	ENGINE_API void OpenMailListReq(LONG npcuid , FLOAT npcposx, FLOAT npcposz, BOOL bRequest);
	void OpenMailListRep();
	void CloseMailList();
	void init();

	void SetTab(TABSTATUS::TABSTATUS tabstatus);

	BOOL CheckSendMail();

	void initstring();
	void SetSendMailString(BOOL mode);
	void initMoney();

	CUIOpenMail* GetOpenMail();
	void ReplySet();

	void SendMailItemBtnInit();
	void SendMailItemBtnInitIndex(int index);
	void SendMailItemBtnInitUniIndex(int uniindex);

	BOOL CheckClaim();
	int	 GetItemCount();
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	BOOL CheckSendMailItemAdd();
	CUIButtonEx* GetMailListItemIcon();

	BOOL CheckDeletItem();

	void SetMoneyOver(BOOL bOver);
	BOOL GetMoneyOver();

	LONG GetNPCUID();

	const FLOAT GetNPCPosX();
	const FLOAT GetNPCPosZ();

	void SetFocusReciver(BOOL mode);

	void MailStatusCommand();


	//m_btnConfirm �� SetEnable���� ������ ��ư ���� ������ ������
	void SetSending(BOOL mode);
	BOOL GetSending();
};




#endif // UIMAIL_H_

