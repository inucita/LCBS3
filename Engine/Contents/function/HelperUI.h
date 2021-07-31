#ifndef __HELPER_UI_H__
#define __HELPER_UI_H__

#define	HELPER_MAX_STUDENTS					(20)		// �ִ� ���� ��û��.

class CUIList;
class CHelperUI : public CUIWindow
{
public:
	CHelperUI();
	~CHelperUI();
	void initialize();
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Open guild
	void	OpenUI( );
	void	CloseUI();	

	void	TeacherAccept( LONG lIndex, const CTString& strStudentName );		// �л��� �İ����� �Ǿ�޶�� ��û�߾��.

	// �İ������� ��ϵǾ� �ִ��� ���θ� ������.
	void	SetRegistredTeacher( BOOL bRegistered );

	// ��� ��� ����	
	void	RefreshStudentList( BOOL bInit = FALSE);

	// Command functions
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput, CTString &strSEInput );
	void	MsgBoxLCommand( int nCommandCode, int nResult );
	// Set focus
	void	SetFocus( BOOL bVisible );
	void	SetMyTeachInfo( SLONG CurTeachCnt, SLONG CompleteTeachCnt, SLONG FailTeachCnt );
	void	ChangeOverColor(int idx);
	void	ChangeLeaveColor(int idx);

private:
	// Command
	void	RegisterTeacher( );					// �İ��� ���
	void	CancelRegisterTeacher( );			// �İ��� ��� ���
	void	BeMyTeacher( );						// �߽��� ��û
	void	FireMyStudent();
	void	AmendReq();

	CUIButton*	m_pBtnRegistTeacher;
	CUIList*	m_pListDisciple;
	CUIList*	m_pListInfo;

	int			m_nOldMouseOverIdx;
};
#endif // __HELPER_UI_H__