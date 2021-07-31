// ----------------------------------------------------------------------------
//  File : UIAutoHelp.h
//  Desc : Created by cpp2angel
// ----------------------------------------------------------------------------

#ifndef	UIAUTOHELP_H_
#define	UIAUTOHELP_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#define AUTOHELO_SHOW_TIME			8000			// (m sec) �����̸��� ǥ�� �Ǵ� �ð� 
#define RND_HELP_TIME				20000

#define RND_HELP_LEVEL_LOW			1	
#define RND_HELP_LEVEL_HIGH			5
#define RND_HELP_LEVEL_MAX			100				// 070820_ttos: ����� ��û ���� ���� ����

// Define notice delay time
#ifdef	IMPROV1107_NOTICESYSTEM
#define	CHATMSG_NOTICE_DELAY		3000			// milliseconds
#define	CHATMSG_NOTICE_FADEOUT		2000
#else
#define	CHATMSG_NOTICE_DELAY		8000			// milliseconds
#define	CHATMSG_NOTICE_FADEOUT		7000
#endif
#define	CHATMSG_NOTICE_FADETIME		( CHATMSG_NOTICE_DELAY - CHATMSG_NOTICE_FADEOUT )
#define CLASSIFICATION_TIME			60*60*1000			//���ǥ�� �ð� 1�ð����� 


// ----------------------------------------------------------------------------
// Name : _SAutoHelpInfo
// Desc : struct
// ----------------------------------------------------------------------------
struct _SAutoHelpInfo
{
	CTString	m_strMessage;		// ��� �޼���
	int			m_iEndLevel;		// ���� ���� 
	int			m_iStartLevel;		// ���� ���� 
	int			m_Kindred;			// ���� ����
	BOOL		m_bJustOnce;		// �ѹ��� �ϴ� �̴ϴ�.
	BOOL		m_bActive;			// Ȱ��ȭ ���Դϴ�.


	void Clear ()
	{
		//SetInfo ( CTString (""), -1, -1, -1, FALSE );
		m_iEndLevel		= -1;			// ���� ���� 
		m_iStartLevel	= -1;
		m_Kindred		= -1;			// ���� ����
		m_bJustOnce		= FALSE;
		m_bActive		= FALSE;
		m_strMessage.Clear();
	}

	void SetInfo ( CTString strMessage, int iStartLevel = -1, 
					int iLevel = -1, int Kindred = -1, BOOL bJustOnce = FALSE )
	{
		m_strMessage	= strMessage;		// ��� �޼���
		m_iEndLevel		= iLevel;			// ���� ���� 
		m_iStartLevel	= iStartLevel;
		m_Kindred		= Kindred;			// ���� ����
		m_bJustOnce		= bJustOnce;
		m_bActive		= FALSE;
			
	}
	
	bool CheckInfo ();
};

typedef struct _SGMNotice
{
	CTString	strGMNotice;
	COLOR		colGMTextColor;
	__int64		i64GMNoticeTime;
	__int64		i64GMFadeTime;
	BOOL		bCompleteVisible;
	UIRect		rcBackground;

}SGMNotice;

// ----------------------------------------------------------------------------
// Name : AUTO_HELP_INDEX
// Desc : enum
// ----------------------------------------------------------------------------
enum AUTO_HELP_INDEX
{
	AU_MOB_KILL,			// ���͸� �׿��� ��
	AU_ITEM_DROP,			// �������� �������� ���
	AU_PICK_ITEM ,			// �ٴڿ� ������ �������� �־��� ��
	AU_GET_SKILL_POINT,		// ���µ��� ���� ��
	AU_MOVE_DRATAN,			// ���ź���� �̵�
	
	
	AU_LEVEL_UP,			// ������
	AU_LEVEL_UP_QUEST2,		// Quest ...
	AU_LEVEL_UP_QUEST8,
	AU_LEVEL_UP_QUEST9,
	AU_LEVEL_UP_QUEST10,
	AU_LEVEL_UP_QUEST16,
	AU_LEVEL_UP_QUEST24,
	AU_LEVEL_UP_STRUP,
	
	AU_RANDOM_HELP_START,						// 13
	AU_RANDOM_HELP = AU_RANDOM_HELP_START, 
	AU_RANDOM_HELP_END = 31,					// 29
	
	// wooss 060328 add thai_auto_help
	AU_NO_TOXICOSIS_S = 32,							// 3�ð����� �ѹ��� �޽��� ���� ���� ����
	AU_NO_TOXICOSIS_E = 35,
	AU_NO_TOXICOSIS_VACATION_S = 36,					// ������ �Ϸ����� ���Ӹ� �ϴ� �������� �����ϴ� ����
	AU_NO_TOXICOSIS_VACATION_E = 39,

	AUTOHELP_COUNT,			
};

#define		RANDON_STRING_COUNT		1

// ----------------------------------------------------------------------------
// Name : CUIAutoHelp
// Desc : class
// ----------------------------------------------------------------------------
class ENGINE_API CUIAutoHelp
{
protected:
	CTextureData	*m_ptdBaseTexture;					// Texture of window
	CTextureData	*m_ptdClassification;				// ���� ��� ǥ��

	BOOL			m_bStop;							// ���� �����ΰ�?
	float			m_fShowTime;						// ���� ���� �ִ� �ð�
	__int64			m_tmStartTime;						// ���̱� ���� �� �ð�
	
	__int64			m_tmEndTime;						// ���̰� �� �� ���� �� �ð� 
	__int64			m_tmClassification;					// ���ǥ�� ����ð�
	__int64			m_tmCurTime;						// ���ǥ�� ���� ���� �ð�

	// Additional autohelp for thai - wooss 060328 ------------------------------->>
	__int64			m_tmCheckTime;						// time check for thai
	__int64			m_bChecked;							// time check for thai
	int				m_iNoticeTime;
	// ---------------------------------------------------------------------------<<

	BOOL			m_bVisible;							// ȭ�鿡 ���� �ְ� �ִ°�?
	int				m_nActiveIndex;						// ���� ���� �ְ� �ִ� ���� Index

	_SAutoHelpInfo	m_AutoHelpInfo[AUTOHELP_COUNT];
	BOOL			m_bCondition[AUTOHELP_COUNT];

	UIRect			m_rcNotice;							// Region of notice
	UIRect			m_rcGMNotice;						// Region of notice
	TIME			m_tmGMNoticeTime;					// ���̱� ���� �� �ð�

#if	defined(IMPROV1107_NOTICESYSTEM)
	std::vector<SGMNotice>	m_vGMNotice;
	BOOL					m_bmovGMNotice;
	__int64					m_i64movGMNotice;
#endif
	CTString		m_strGMNotice;
	COLOR			m_colGMTextColor;
	BOOL			m_bShowGMNotice;
	BOOL			m_bShowClassification;				//���ǥ��
	UIRect			m_rcClassification;
	UIRectUV		m_rtClassification;					//���ǥ��
	
	int				m_nKillMobIndex;
	BOOL			m_RndHelp;

#if		!defined(_UI_LIB_RENEWAL_)
	UIRectUV		m_rtNoticeL;						// UV of notice left region
	UIRectUV		m_rtNoticeC;						// UV of notice center region
	UIRectUV		m_rtNoticeR;						// UV of notice right region
#else	// _UI_LIB_RENEWAL_
	CUIImage*		m_pImgNotice[3];					// left, center, right
#endif	// _UI_LIB_RENEWAL_

public:

	CUIAutoHelp();
	~CUIAutoHelp();

	void Clear();
	
	// Create
	void Create();

	// Render
	void Render();
	void RenderGMNotice ();
	void ClassificationShowRender();

	void ShowAutoHelp ( int nIndex );
	void SetInfo ( DWORD dwAutoHelpInfo );
	
	void SetGMNotice ( CTString strGMNotice, COLOR colText = 0xFFBA13FF );
	COLOR GetGMNoticeColor(){ return m_colGMTextColor;	}
	BOOL IsShowGMNotice()	{ return m_bShowGMNotice;	}
	void ClearGMNNotice();

	void SetKillMonIndex ( int nIndex )
	{
		m_nKillMobIndex = nIndex;
	}

	int	GetKillMonIndex () const 
	{
		return m_nKillMobIndex;
	}

	void GetLvGuidString(int lv);
	void LearnTheSkill(int lv);
	void SetStatpoint(int old, int cur);
	int  GetUpStatpoint()	{ return m_nUpStat;	}
	void initialize();

private:
	int m_nUpStat;
	int m_nInterval_time;
};

ENGINE_API extern CUIAutoHelp*	_UIAutoHelp;

#endif	// UISINGBOARD_H_

