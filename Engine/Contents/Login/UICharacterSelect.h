#ifndef _UI_CHARACTER_SELECT_H_
#define _UI_CHARACTER_SELECT_H_

class CUIListItem;
class CUICharacterSelect : public CUIWindow
{
public:
	CUICharacterSelect();

	void	initialize();
	void	Lock(BOOL bLock);
	void	OpenUI();
	void	CloseUI();

	void	UpdateCharacterList();
	void	DeleteCharacterMsgBox(int nDelIdx);
	void	SetCharacterModel(int nSelIdx);

	void	LogOut();
	void	ChangeServer();
	void	PressOkBtn();

	void	Reset();
	void	DeleteModel();
	//----------------------------------------------------------------------------->>
	void OnUpdatePosition();
	void MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonDBLClick(UINT16 x, UINT16 y);
	void		OnUpdate( float fDeltaTime, ULONG ElapsedTime );
	WMSG_RESULT OnKeyMessage(MSG* pMsg);
	//-----------------------------------------------------------------------------<<

	void	SetPostCommand(Command* pCmd)	{ m_pCmd = pCmd; }

private:
	void	SetTextData(CUIListItem* pItem, int idx);
	void	SetImageData(CUIListItem* pItem, int idx);
	void	SetButtonData(CUIListItem* pItem, int idx);
	void	SetTooltipData(CUIListItem* pItem, int idx);

	void	UpdateSlotInfo();
	void	UpdateServerInfo();

	void	DeleteCharacter(CTString strSecurity);
	void	StartGame(int nSelectIdx);
	void	CreateCharacter();
	void	EquipWear();
	int		GetMotionIdx(bool bDelete);
	CTString GetJobName(int nJob, int n2ndJob = 0);

	enum	eCHAR_SLOT_LIST 
	{
		eSLOT_LIST_LEFT = 0,
		eSLOT_LIST_RIGHT,

		eSLOT_LIST_MAX
	};

	enum	eCHAR_SLOT_STATE
	{
		eSLOT_STATE_NONE = 0,	// ĳ���Ͱ� �ִ� ����.
		eSLOT_STATE_EMPTY,		// ĳ���Ͱ� ���� ����. ( ADD�̹��� )
		eSLOT_STATE_LOCK,		// ������ ��� �ִ� ���� ( �ڹ��� �̹��� )

		eSLOT_STATE_MAX
	};

	enum eCHAR_MOVE_STATE
	{
		eMOVE_STATE_NORMAL = 1,
		eMOVE_STATE_WAIT,
		eMOVE_STATE_ROLLBACK,
	};
	
	CUIList*	m_pListCharacterSlot[eSLOT_LIST_MAX];
	CUIBase*	m_pUIBase[2];
	CUIText*	m_pTextSlotInfoTitle;
	CUIButton*	m_pBtnPressOk;
	CUIButton*	m_pBtnServerChange;
	CUIButton*	m_pBtnLogout;
	CUITextBox*	m_pTbSlotInfoDesc;

	int			m_nDeleteCharIdx;
	int			m_nRenderModelOldIdx;
	ULONG		m_ulExpansoinTime;
	int			m_nPlayAnimId;

	bool		m_IsWear;

	// 
	Command*	m_pCmd;
};

class CmdPostUpdateCharacter : public Command
{
public:
	CmdPostUpdateCharacter() : _pUI(NULL) {}
	void setData(CUICharacterSelect* pUI)	{ _pUI = pUI; }
	void execute()
	{
		if (_pUI == NULL)
			return;

		_pUI->UpdateCharacterList();
		_pUI->Lock(FALSE);
	}

private:
	CUICharacterSelect* _pUI;
};

#endif // _UI_CHARACTER_SELECT_H_