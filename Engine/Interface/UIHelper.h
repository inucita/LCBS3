// ----------------------------------------------------------------------------
//  File : UIHelper.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIHELPER_H_
#define	UIHELPER_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <vector>

// Define text position
#define	HELPER_TITLE_TEXT_OFFSETX		25
#define	HELPER_TITLE_TEXT_OFFSETY		5

// Define size of Helper
#define	HELPER_WIDTH					250
#define	HELPER_HEIGHT					335

// [8/31/2010 kiny8216] �İ��� �ý��� ����
#define NEW_HELPER_WIDTH				434
#define NEW_TAB_CLASS					50
#define NEW_TAB_NAME					114
#define NEW_TAB_TIME					114
#define NEW_TAB_COUNT					80
#define NEW_TAB_INFO					46

//toop-tip
#define HELPER_MAX_INFO_LINE 20

#define	HELPER_MAX_STUDENTS					(20)		// �ִ� ���� ��û��.

// ----------------------------------------------------------------------------
// Name : CUIHelper
// Desc :
// ----------------------------------------------------------------------------
class CUIHelper : public CUIWindow
{
protected:	
	// Helper State
	enum eHelperState
	{
		HELPER_MENU_REQ,
		HELPER_MENU_TEACHER,			// �߽��� ����
		HELPER_MENU_STUDENT,			// �İ��� ����
	};	

	// internal structure
	struct sHelperMember
	{
		sHelperMember()
		{
			lIndex			= -1;			
			iJob			= -1;
			iJob2			= -1;
			iLevel			= 0;
			strName			= CTString("");
		}

		~sHelperMember()
		{	
		}

		bool operator<(const sHelperMember &other) const
		{
			if( lIndex < other.lIndex )
				return true;
			return false;
		}
		bool operator>(const sHelperMember &other) const
		{			
			return other.operator < (*this);
		}

		SLONG			lIndex;
		CTString		strName;			// �����?		
		int				iLevel;
		int				iJob;
		int				iJob2;
	};

	// Function Object
	struct FindMember
	{
		SLONG lIndex;
		FindMember(sHelperMember& info) : lIndex(info.lIndex) {};
		bool operator() (const sHelperMember& info)
		{
			if(info.lIndex == lIndex)
			{
				return true;
			}
			else
				return false;
		}
	};

	struct sTeacherInfo
	{
		bool operator<(const sTeacherInfo &other) const
		{
			if( index < other.index )
				return true;
			return false;
		}
		bool operator>(const sTeacherInfo &other) const
		{			
			return other.operator < (*this);
		}
		LONG index;
		SLONG nFame;
		SLONG nCntTeachingStudent;
		SLONG nCntCompleteStudent;
		SLONG nCntFailStudent;
		SBYTE sbStartPlayTime;
		SBYTE sbEndPlayTime;
	};
	struct sStudentInfo
	{
		LONG index;
		char szFirstDate[50];
		char szFinalDate[50];
	};

	std::vector<sHelperMember>	m_vectorHelperList;			// �ܿ� ���
	std::vector<sTeacherInfo>	m_vectorTeacherInfoList;	// �������� ���� ���
	std::vector<sStudentInfo>	m_vectorStudentInfoList;	// �л����� ���� ���

	// Controls
	CUIButton				m_btnClose;						// Close button

	CUIButton				m_btnRefreshTeacher;			// �İ��� ��� ����
	CUIButton				m_btnRegisterTeacher;			// �İ��� ���
	CUIButton				m_btnBeMyTeacher;				// �߽���
	CUIButton				m_btnFireMyTeacher;				// �߽��� ����
	CUIButton				m_btnCancelRegister;			// �İ��� ��� ���

	CUIButton				m_btnExit;						// �ݱ�

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackManagerTop;				//
	UIRectUV				m_rtBackManagerMiddle;			//
	UIRectUV				m_rtBackManagerBottom;			//
	UIRectUV				m_rtBackBottom;					// UV of bottom background	

	UIRectUV				m_rtTab;						// UV of section tab
	UIRect					m_rcTab;

	CUIListBox				m_lbTeacherList;				// List box of teacher member list
	CUIListBox				m_lbStudentList;				// List box of student list
	
	// Tool Tip	
	UIRectUV				m_rtInfoUL;								// UV of upper left region of information
	UIRectUV				m_rtInfoUM;								// UV of upper middle region of information
	UIRectUV				m_rtInfoUR;								// UV of upper right region of information
	UIRectUV				m_rtInfoML;								// UV of middle left region of information
	UIRectUV				m_rtInfoMM;								// UV of middle middle region of information
	UIRectUV				m_rtInfoMR;								// UV of middle right region of information
	UIRectUV				m_rtInfoLL;								// UV of lower left region of information
	UIRectUV				m_rtInfoLM;								// UV of lower middle region of information
	UIRectUV				m_rtInfoLR;								// UV of lower right region of information

	BOOL					m_bShowInfo;

	UIRect					m_rcInfo;
	int						m_nCurInfoLines;

	CTString				m_strInfo[HELPER_MAX_INFO_LINE];		// information string
	COLOR					m_colInfo[HELPER_MAX_INFO_LINE];		// Color of information string

	//info icon
	UIRectUV				m_rtInfoMarkUV;
	UIRect					m_rcInfoMark[HELPER_MAX_STUDENTS];

	// [8/31/2010 kiny8216] �İ��� �ý��� ����
	BOOL					m_bShowClass;
	SLONG					m_nMyCurTeachCnt;						// ���� ���� �缺 ��
	SLONG					m_nMyCompleteTeachCnt;					// ���� �Ϸ� �缺 ��
	SLONG					m_nMyFailTeachCnt;						// ���� ���� �缺 ��
	CTString				m_strClassName;							// Popup Class Name

	CUIButton				m_btnGift;								// Button : ���� �ޱ�
	CUIButton				m_btnFireMyStudent;						// Button : �İ��� ����

	UIRect					m_rtNewBack[9];							// New BackGround Rect
	UIRect					m_rcClass[HELPER_MAX_STUDENTS];			// Job Image Rect
	UIRect					m_rtStudentBack[6];						// Sub BackGround Rect : Student
	UIRect					m_rtTeacherBack[6];						// Sub BackGround Rect : Teacher
	UIRect					m_rtStudentInfo[6];						// Info BackGround Rect : Student
	UIRect					m_rtTeacherInfo[6];						// Info BackGround Rect : Teacher
	UIRect					m_rtTabLine;							// ListBox Line Rect
	UIRectUV				m_rtClass[TOTAL_JOB];					// Class Image Rect UV
	UIRectUV				m_rcTabLineUV;							// ListBox Line Rect UV
	CTextureData			*m_ptdSubTexture;						// New_Interface.tex
	CTextureData			*m_ptdBtnTexture;						// CommonBtn.tex
	CUIRectSurface			m_NewBack;								// New BackGround
	CUIRectSurface			m_NewSubBack;							// Sub BackGround
	CUIRectSurface			m_NewInfoBack;							// Info BackGround
protected:
	eHelperState			m_eHelperMenuState;
	BOOL					m_bRegistredTeacher;
	int						m_iLimitLevel;					// �ִ� ����
public:
	CUIHelper();
	~CUIHelper();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();
	void	RenderInfo();

	//tool-tip
	void	ShowInfo(int line, BOOL bRenew);

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild
	void	OpenHelper( );
	void	ResetHelper();	

	// ���� & �л� ��� ����	
	void	AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2);		// ��� ��Ͽ� �߰�
	void	AddToTeacherInfoList(LONG lIndex, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail, SBYTE StartPlayTime = -1, SBYTE EndPlayTime = -1 );
	void	AddToStudentInfoList(LONG lIndex, const char *szFirstDate, const char *szFinalDate);
	void	DelFromHelperList( LONG lIndex );
	void	ClearHelperList();	
	void	ChangeHelperLevel( LONG lIndex, int iLevel );		

	// Command
	void	RegisterTeacher( );					// �İ��� ���
	void	CancelRegisterTeacher( );			// �İ��� ��� ���
	void	BeMyTeacher( );						// �߽��� ��û
	void	FireMyTeacher();					// �߽��� ����	
	void	TeacherAccept( LONG lIndex, const CTString& strStudentName );		// �л��� �İ����� �Ǿ�޶�� ��û�߾��.

	// �İ������� ��ϵǾ� �ִ��� ���θ� ������.
	void	SetRegistredTeacher( BOOL bRegistered );

	// ��� ��� ����	
	void	RefreshTeacherList( BOOL bInit = FALSE );
	void	RefreshStudentList( BOOL bInit = FALSE);

	int		GetLimitLevel();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput, CTString &strSEInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );

	// Set focus
	void	SetFocus( BOOL bVisible );

	// [8/31/2010 kiny8216] �İ��� �ý��� ����
	void	RenderNew();			// �İ��� ���� Render �Լ�
	void	RenderStudent();		// �߽��� UI Render
	void	RenderTeacher();		// �İ��� UI Render
	void	PrepareOpen();			// UI ����
	void	ShowClass(int line);	// Ŭ���� �̸� �˾�
	void	RenderPopUpClass();
	void	FireMyStudent();
	void	SetMyTeachInfo( SLONG CurTeachCnt, SLONG CompleteTeachCnt, SLONG FailTeachCnt );

	void	Clear();
};

#endif	// UIHELPER_H_