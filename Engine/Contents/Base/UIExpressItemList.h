#ifndef UI_LCE_SYSTEM_ITEM_LIST_H_
#define UI_LCE_SYSTEM_ITEM_LIST_H_
#ifdef	PRAGMA_ONCE
#pragma once
#endif

enum eLCE_ITEM_STRING
{
	LCE_STRING_NAME = 0,		// ������ �̸�
	LCE_STRING_NUM,				// �������϶� ����
	LCE_STRING_AMOUNT,			// �����϶� ����
	LCE_STRING_SENDER,			// ���� ���
	LCE_STRING_TYPE,			// ��� Ÿ��
	LCE_STRING_DATE_START,		// ��� �ð�
	LCE_STRING_DATE_END,		// ��� ���� �ð�
	LCE_STRING_MAX,
};

enum eLCE_ITEM_STRING_COLOR
{
	LCE_COLOR_NAME = 0,			// ������ �̸�
	LCE_COLOR_NUM,				// �������϶� ����
	LCE_COLOR_AMOUNT,			// �����϶� ����
	LCE_COLOR_SENDER,			// ���� ���
	LCE_COLOR_TYPE,				// ��� Ÿ��
	LCE_COLOR_DATE,			// ��� �ð�
	LCE_COLOR_MAX,				
};

enum eLCE_Item_List_UI_Info
{
	COLUM_X_GAP = 3,
	COLUM_START_NAME_GAP	= BTN_SIZE + COLUM_X_GAP,
	CULUM_X_WIDTH_ITME_NAME = 258,
	CULUM_X_WIDTH_QUANTITY	= 51,
	CULUM_X_WIDTH_AMOUNT	= 90,
	CULUM_X_WIDTH_SENDER	= 100,
	CULUM_X_WIDTH_TYPE		= 79,
	CULUM_X_WIDTH_TIME		= 121,
	LCE_UI_ITEMLIST_COLUM_ITEM_NAME		= 0,
	LCE_UI_ITEMLIST_COLUM_QUANTITY		= LCE_UI_ITEMLIST_COLUM_ITEM_NAME + COLUM_X_GAP + CULUM_X_WIDTH_ITME_NAME,
	LCE_UI_ITEMLIST_COLUM_AMOUNT		= LCE_UI_ITEMLIST_COLUM_QUANTITY + COLUM_X_GAP + CULUM_X_WIDTH_QUANTITY,
	LCE_UI_ITEMLIST_COLUM_SENDER		= LCE_UI_ITEMLIST_COLUM_AMOUNT + COLUM_X_GAP + CULUM_X_WIDTH_AMOUNT,
	LCE_UI_ITEMLIST_COLUM_TYPE			= LCE_UI_ITEMLIST_COLUM_SENDER + COLUM_X_GAP + CULUM_X_WIDTH_SENDER,
	LCE_UI_ITEMLIST_COLUM_TIME			= LCE_UI_ITEMLIST_COLUM_TYPE + COLUM_X_GAP + CULUM_X_WIDTH_TYPE,
};

class CUIExpressItemList : public CUIWindow
{
public:
	CUIExpressItemList();
	~CUIExpressItemList();
	//----------------------------------------------------------------
	void		SetString( eLCE_ITEM_STRING type, CTString strString );
	void		SetStringColor( eLCE_ITEM_STRING_COLOR type, COLOR color );

	int			GetBtnPosX()	{ return m_pIcon->GetPosX(); }
	int			GetBtnPosY()	{ return m_pIcon->GetPosY(); }
	CUIIcon*	GetBtn()		{ return m_pIcon; }
	// ���� ���������� ����
	void		SetNasItem(bool bNas) { m_bNas = bNas; }
		
	// ������ ����Ʈ ����
	void		InitItemList();

	//-----------------------------------------------------------------
	void		Create( CUIWindow* pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void		Render();

	// Msg Proc
	WMSG_RESULT MouseMessage(MSG* pMsg);

private:
	// ��Ʈ�� ����
	void			_ListStringInit();
	// ��Ʈ�� �÷� ����
	void			_ListColorInit();
	// ��Ʈ�� ��ġ ����
	void			_StringPosInit();

	CUIIcon*		m_pIcon;		// �ͽ������� ������
	CTString		m_strString[LCE_STRING_MAX];
	COLOR			m_color[LCE_COLOR_MAX];

	//------------------------------------------------------------------
	// ��Ʈ�� ��ġ
	int				m_nItemNamePosX;
	int				m_nItemNamePosY;
	int				m_nItemNumPosX;
	int				m_nItemNumPosY;
	int				m_nAmountPosX;
	int				m_nAmountPosY;
	int				m_nSenderPosX;
	int				m_nSenderPosY;
	int				m_nTypePosX;
	int				m_nTypePosY;
	int				m_nStartTimePosX;
	int				m_nStartTimePosY;
	int				m_nEndTimePosX;
	int				m_nEndTimePosY;

	int				m_nNasAmountPosX;
	bool			m_bNas;		// �������� ������ ����
};

#	endif	// UI_LCE_SYSTEM_ITEM_LIST_H_