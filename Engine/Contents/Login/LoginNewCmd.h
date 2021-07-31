#ifndef		_LOGIN_COMMAND_H_
#define		_LOGIN_COMMAND_H_

class Command;

//--------------------------------------------------------------
// �ݱ�
class CmdLoginClose : public Command
{
public:
	CmdLoginClose() : pUI_(NULL) {}
	void setData(CUILoginNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressCloseBtn();
	}
private:
	CUILoginNew* pUI_;
};

//--------------------------------------------------------------
// �α��� ��ư Ŭ��
class CmdLoginOk : public Command
{
public:
	CmdLoginOk() : pUI_(NULL) {}
	void setData(CUILoginNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressLoginBtn();
	}
private:
	CUILoginNew* pUI_;
};

//--------------------------------------------------------------
// ���̵� ���� ��ư Ŭ��
class CmdLoginSaveId : public Command
{
public:
	CmdLoginSaveId() : pUI_(NULL) {}
	void setData(CUILoginNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressCheckSave();
	}
private:
	CUILoginNew* pUI_;
};


#endif		// _LOGIN_COMMAND_H_