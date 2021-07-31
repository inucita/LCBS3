#pragma once
/*
	ũ��Ƽ�� ���� Ŭ����
	�ۼ��� : �迵ȯ
*/

////	ũ��Ƽ�� ���� Ŭ����
class F_Critical
{
	////	Ŭ���� ���� ���� �Լ�
public:
	F_Critical(void);
	~F_Critical(void);

	////	Ŭ���� ���� ����
protected:
	CRITICAL_SECTION    m_CriticalSection;
	
	////	Ŭ���� ���� �Լ�
public:
	//	����
	void	Enter();
	//	����
	void	Leave();

	//	���� ���� ���� �Լ�
	static	F_Critical	*New();
	static	BOOL	Delete(F_Critical *p_Critical);
};
