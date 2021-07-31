#ifndef __COMMAND_CUSTOM_TITLE_H__
#define __COMMAND_CUSTOM_TITLE_H__

class Command;

//--------------------------------------------------------------
// �ݱ�
class CmdCustomTitleClose : public Command
{
public:
	CmdCustomTitleClose() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->closeUI();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// ���۹�ư Ŭ��
class CmdCustomPressMakeBtn : public Command
{
public:
	CmdCustomPressMakeBtn() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressMakeBtn();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// �����ϱ�
class CmdCustomTitleMake : public Command
{
public:
	CmdCustomTitleMake() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->MakeTitle();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// ���� ����ϱ�
class CmdCustomTitleMakeCancel : public Command
{
public:
	CmdCustomTitleMakeCancel() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->CancelMakeTitle();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// �޴� ������ �ٲٱ�
class CmdCustomTitleChangeMenu : public Command
{
public:
	CmdCustomTitleChangeMenu() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI, int menuType, eChangeType eType)
	{ 
		pUI_ = pUI; 
		_MenuType = menuType;
		_ChangeType = eType;
	}
	void execute() {
		if (pUI_)
			pUI_->ChangeMenu(_MenuType, _ChangeType);
	}
private:
	CCustomTitleUI* pUI_;
	int _MenuType;
	eChangeType _ChangeType;
};

//--------------------------------------------------------------
// ����Ʈ ����
class CmdCustomTitleSelectEffect : public Command
{
public:
	CmdCustomTitleSelectEffect() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectEffect();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// ȣĪ���ڻ� ����
class CmdCustomTitleSelectFrontColor : public Command
{
public:
	CmdCustomTitleSelectFrontColor() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectFrontColor();
	}
private:
	CCustomTitleUI* pUI_;
};

//--------------------------------------------------------------
// ȣĪ���� ����
class CmdCustomTitleSelectBackColor : public Command
{
public:
	CmdCustomTitleSelectBackColor() : pUI_(NULL) {}
	void setData(CCustomTitleUI* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectBackColor();
	}
private:
	CCustomTitleUI* pUI_;
};

#endif	//	__COMMAND_CUSTOM_TITLE_H__