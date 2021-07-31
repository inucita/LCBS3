#ifndef		_CHARACTER_CREATE_COMMAND_H_
#define		_CHARACTER_CREATE_COMMAND_H_

class Command;

//--------------------------------------------------------------
// ���ư��� ĳ���� ����â����
class CmdCharCreateClose : public Command
{
public:
	CmdCharCreateClose() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ReturnUI();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// ����
class CmdCharCreateOk : public Command
{
public:
	CmdCharCreateOk() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->PressOk();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// Ŭ���� ����
class CmdCharSelect : public Command
{
public:
	CmdCharSelect() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->SelJob(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};

//--------------------------------------------------------------
// �Ҽ� ����
class CmdSocailSelect : public Command
{
public:
	CmdSocailSelect() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->SelSocial(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};

//--------------------------------------------------------------
// zoom
class CmdCharZoom : public Command
{
public:
	CmdCharZoom() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->Zoom(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};
//--------------------------------------------------------------
// Rotation
class CmdCharRotation : public Command
{
public:
	CmdCharRotation() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI, eCreateBtnType eType)	{ pUI_ = pUI; eType_ = eType; }
	void execute() {
		if (pUI_)
			pUI_->Rotation(eType_);
	}
private:
	CUICharacterCreateNew* pUI_;
	eCreateBtnType eType_;
};

//--------------------------------------------------------------
// face select
class CmdSelectFace : public Command
{
public:
	CmdSelectFace() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectFace();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// hair select
class CmdSelectHair : public Command
{
public:
	CmdSelectHair() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectHair();
	}
private:
	CUICharacterCreateNew* pUI_;
};

//--------------------------------------------------------------
// Armor select
class CmdSelectArmor : public Command
{
public:
	CmdSelectArmor() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectArmor();
	}
private:
	CUICharacterCreateNew* pUI_;
};

class CmdSelectSocial : public Command
{
public:
	CmdSelectSocial() : pUI_(NULL) {}
	void setData(CUICharacterCreateNew* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectSocial();
	}
private:
	CUICharacterCreateNew* pUI_;
};


#endif		// _CHARACTER_CREATE_COMMAND_H_