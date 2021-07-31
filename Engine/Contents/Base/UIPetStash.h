#ifndef UI_PETSTASH_H_
#define UI_PETSTASH_H_

enum eMSG_PetStash_Type
{
	MSG_TYPE_INSTASH = 0,
	MSG_TYPE_OUTSTASH,
};

class CUIListItem;
class CUIArrayItem;
class CUIArray;
class CUITab;
class CUIText;
class CUICheckButton;
class CUIList;
class CUIIcon;

class CUIPetStash : public CUIWindow
{
public:
	CUIPetStash();
	~CUIPetStash();
	////// interface /////////////////
	void	OpenPetStash();
	void	ClosePetStash();

	void	UpdateUI();
	void	UpdateReuseTime();

	void	ClearUI();

	////// Command //////////////////
	// �븮���� ������ Ŭ��
	void	SetArrayItemClick();
	// ����Ʈ �� Ŭ��
	void	SetListItemClick();
	// �� Ŭ��
	void	ChangeTab();
	// �븮���� ��� Ŭ��
	void	SelectPet();
	// �ñ��, ã�� ��ư Ŭ����
	void	CreateMSGBox(eMSG_PetStash_Type type);

	//////////////////////////////////
	void	initialize();
		
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	
	BOOL	CloseWindowByEsc()	{ ClosePetStash(); return TRUE; }
		
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	// Msg Proc
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnMouseMove(UINT16 x, UINT16 y, MSG* pMsg);

private:
	//-------------------------------------------------------------------
	// ��������1 �� ����Ʈ ������ ����

	// �� ����Ʈ ������ ������Ʈ
	BOOL	_UpdatePetListStash();
	// �� ����Ʈ ������ ����
	BOOL	_SetPetStashList(CUIListItem* pListItem, int PetNum);

	// �� ����Ʈ ������ Ŭ����
	void	_ClearPetStashList(CUIListItem* pListItem);
	void	_ClearImage(CUIListItem* pListItem);
	
	//--------------------------------------------------------------------
	// ��������2 �븮���� ������ ����

	// �븮���� �� ����Ʈ ������Ʈ
	BOOL	_UpdateDeputyList();
	// �븮���� �� ����Ʈ ����
	BOOL	_SetDeputyList(CUIArrayItem* pArrayItem, int PetNum);

	// �븮���� �� ����Ʈ Ŭ����
	void	_ClearDeputyList(CUIArrayItem* pArrayItem);	
	void	_ClearSelectPet();

	// titleMove
	bool			m_bTitleClick;
	UIRect			m_rcOriginPos;
	bool			m_bDrag;
	int				m_nOriX, m_nOriY;

	//main
	CUITab*			m_pTab;
	CUIText*		m_pTxtTitle;
	CUIText*		m_pMoveTitle;
	CUIText*		m_pTxtReuseTime;
	CUIButton*		m_pbtnClose;
	CUICheckButton*	m_pbtnPetstash;
	CUICheckButton*	m_pbtnPetDeputy;

	//��������1 �� ����Ʈ
	CUIButton*		m_pbtnInStash;
	CUIButton*		m_pbtnOutStash;
	CUIText*		m_pTxtPetCount;
	CUIList*		m_pListPetInfo;
	CUIScrollBar*	m_pListScroll;

	//��������2 �븮����
	CUIButton*		m_pbtnSelect;
	CUIIcon*		m_pIconSelectPet;
	
	CUIText*		m_pTxtPetName;
	CUIText*		m_pTxtPetLevel;
	CUIText*		m_pTxtPetExp;
	CUIText*		m_pTxtPetLoyalty;
	CUIText*		m_pTxtPetHungry;
	CUIArray*		m_pPetArray;
};
#endif // UI_PETSTASH_H_