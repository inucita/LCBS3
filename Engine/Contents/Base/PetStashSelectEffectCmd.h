#ifndef		_PETSTASH_SELECTEFEECT_COMMAND_H_
#define		_PETSTASH_SELECTEFEECT_COMMAND_H_

class Command;

//--------------------------------------------------------------
// �� ����Ʈ UI �ݱ�
class CmdPetSelectEffectClose : public Command
{
public:
	CmdPetSelectEffectClose() : pUI_(NULL) {}
	void setData(CUIPetStashSelectEffect* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->ClosePetStashSelectEffect();
	}
private:
	CUIPetStashSelectEffect* pUI_;
};

//--------------------------------------------------------------
// �� ����Ʈ ���� (����� UI�� ���½������� �ٷ� ���۵Ǳ⿡ ������� ���� Test �����θ� ���)
class CmdPetSelectEffectOk : public Command
{
public:
	CmdPetSelectEffectOk() : pUI_(NULL) {}
	void setData(CUIPetStashSelectEffect* pUI)	{ pUI_ = pUI; }
	void execute() {
		if (pUI_)
			pUI_->StartAni();
	}
private:
	CUIPetStashSelectEffect* pUI_;
};

#endif		// _PETSTASH_SELECTEFEECT_COMMAND_H_