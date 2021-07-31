#ifndef		_PETSTASH_COMMAND_H_
#define		_PETSTASH_COMMAND_H_

class Command;
//--------------------------------------------------------------
// �� ��ȯ�� ���
class CmdChangeTab : public Command
{
public:
	CmdChangeTab() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute()	{
		if (pUI_)
			pUI_->ChangeTab();
	}
private:
	CUIPetStash*	pUI_;
};

//--------------------------------------------------------------
// �ݱ�
class CmdPetStashClose : public Command
{
public:
	CmdPetStashClose() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClosePetStash();
	}
private:
	CUIPetStash* pUI_;
};

//--------------------------------------------------------------
// �ñ��, ã�� ��ư ���� �� �޽��� �ڽ� ����
class CmdPetMsgStash : public Command
{
public:
	CmdPetMsgStash() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI, eMSG_PetStash_Type type)		
	{
		pUI_ = pUI; 
		m_type = type;
	}
	void execute() {
		if (pUI_)
			pUI_->CreateMSGBox(m_type);
	}
private:
	CUIPetStash* pUI_;
	eMSG_PetStash_Type m_type;
};

//--------------------------------------------------------------
// �븮���� ��Ϲ�ư Ŭ����
class CmdPetSelect : public Command
{
public:
	CmdPetSelect() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SelectPet();
	}
private:
	CUIPetStash* pUI_;
};

//--------------------------------------------------------------
// �� ����Ʈ ������ Ŭ����
class CmdPetListItemClick : public Command
{
public:
	CmdPetListItemClick() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SetListItemClick();
	}
private:
	CUIPetStash* pUI_;
};

//--------------------------------------------------------------
// �븮���� ��� ������ Ŭ����
class CmdPetArrayItemClick : public Command
{
public:
	CmdPetArrayItemClick() : pUI_(NULL) {}
	void setData(CUIPetStash* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->SetArrayItemClick();
	}
private:
	CUIPetStash* pUI_;
};

#endif		// _PETSTASH_COMMAND_H_