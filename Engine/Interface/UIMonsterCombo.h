#ifndef	UIMonsterCombo_H_
#define	UIMonsterCombo_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define UI_MONSTERCOMBO_WIDTH	600
#define UI_MONSTERCOMBO_HEIGHT	400

#define READ_MAX_CHAR 32
#define MAX_COMBO_COUNT 20
#define CASE_SLOT_COUNT	5

// Define button size
#define	COMBO_BTN_SIZE	52

enum TexType
{
	SYS_STAGE = 0,
	SYS_MISSION,
	SYS_CLEAR,
	SYS_FAIL,
	SYS_TOTAL, 
};

enum CountType
{
	COMBO_TYPE = 0,
	CUBE_TYPE,
};

class CUIMonsterCombo : public CUIWindow
{
protected:

	CTextureData	*m_ptdAddTexture;
	CUIDrawBox	m_bxTitle;
	CUIDrawBox	m_bxBackground;
	CUIDrawBox	m_bxContentsBox;
	CUIButton	m_btnClose;
	CUIDrawBox	m_bxMissionSlot;
	CUIDrawBox	m_bxHelpSlot;
	CUIButton	m_btnDel;			// �����
	CUIButton	m_btnMakeView;		// �޺� ����
	CUIButton	m_btnAdmission;		// ����
	UIRect		m_rcCombo;			// �޺� ����
	UIRect		m_rcCase;			// ���̽� ����
	UIRectUV	m_rtSysView[SYS_TOTAL];
	UIRectUV	m_rtStagenum[2];
	CUIListBox*	m_plbComboHelp;

	BOOL		m_bCaseView;		// �̼� ���̽� ���� (�����̳� �̱� �÷��̾�)
	BOOL		m_bComboComplete;	// �̼� ���̽� ���� �Ϸ�
	BOOL		m_bShowSysImage;
	BOOL		m_bActionChack;			// �׼ǹ�ư���� �����°��
	CHAR		m_bStageClear;
	INDEX		m_nSysType;
	INDEX		m_nCombo[MAX_COMBO_COUNT];		// ���� �޺� Index ����
	INDEX		m_nCase[CASE_SLOT_COUNT];		// ���� �޺� Index ����
	INDEX		m_nComboCount;					// ���� �޺� ī��Ʈ
	INDEX		m_nCurComboNum;				// ���� �޺� ��ũ�� ī��Ʈ
	INDEX		m_nCurMissionNum;			// �̼� ���̽� ��ũ�� ī��Ʈ
	INDEX		m_nCurItem;					// ���õ� ������

	// Set detail
	CUIIcon*		m_pIconsSetCase[CASE_SLOT_COUNT];		// set Mission case arrayz
	CUIIcon*		m_pIconsSetCombo[MAX_COMBO_COUNT];	// set Mission Combo arrayz
	
	CUIScrollBar	m_sbMissionCaseList;	//�̼� ���̽� ����Ʈ 
	CUIScrollBar	m_sbComboList;			//�޺� ����Ʈ (�̼� ���̽� ����Ʈ���� ������ �޾ƿ��� ��)

	INDEX		m_nBossIdx;					// �̵� �޽��� ���� �� �ʿ�
	SQUAD		m_nAdmissionPay;
	INDEX		m_nComboPoint;
	INDEX		m_nStageCount;				// �������� �ѹ� ����
	__int64		m_tmStartTime;
	__int64		m_tmStageview;
	FLOAT		m_fNpcX, m_fNpcZ;
	int			m_nNpcVirIdx;
	INDEX		m_nCountType;
	

public:
	CUIMonsterCombo();
	~CUIMonsterCombo();

	void Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void Render();
	void RenderComboList(int nCurScrollNum,int nX, int nY);
	void MissionCaseRender(int iX, int iY);
	void RenderMissionList(int nCurScrollNum,int nX, int nY);
	void OpenMonsterCombo(BOOL bChack,FLOAT fX, FLOAT fZ, int nNpcVirIdx = -1);
	void CloseMonsterCombo();
	void MissionCaseEnable(BOOL bEnable) {m_bCaseView = bEnable;}
	void AddCombo(int nIndex,int nslot);
	void SendComboMessage(LONG comboMessage,BOOL bYesNo = FALSE, int nNpcVirIdx = -1);
	void RecComboErrorMessage(UBYTE comboMessage);
	void SetComboList(int num, INDEX nIndex); 
	void SetBossIndex(int nIndex) {m_nBossIdx = nIndex;}
	void StageComplete(int nStagenum, UBYTE bComplete);
	void SysStateRender();
	void StageCount(int stagenum);
	void SetComboCount(int nsum) {m_nComboCount = nsum;}
	int	 GetComboStage() {return m_nStageCount;}
	BOOL GetComboClear() {return m_bStageClear;}
	void SetStageNum(int stagenum) {m_nStageCount = stagenum;}
	void SetSysImage(int systype, BOOL bView = FALSE, int counttype = COMBO_TYPE);
	void SetActionChack(BOOL chack) {m_bActionChack = chack;}
	void ComboListRefresh();

private:
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void UpdateSetCase();
};

#endif