#pragma once
/*
	�Ϲ� ������ Ŭ����
	�ۼ��� : �迵ȯ
*/

class F_Thread
{
	////	���� �� ����.
public:
	F_Thread(void);
	~F_Thread(void);

	////	Ŭ���� ���� ����
public:
	DWORD	m_ThreadID;		// ������ ������ ID
	HANDLE	m_Thread;		// ������ �ڵ�
	HANDLE	m_Event;		// ������ ���� �̺�Ʈ
	int		m_Exit_ID;		// ���� ID

	////	Ŭ���� ���� �Լ�
public:
	//	�����Լ�
	virtual bool	Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param = NULL);
	//	�����Լ�
	virtual bool	Release();
	//	�̺�Ʈ ��� �Լ�
	bool	Wait_Event(DWORD p_Wait_Time = INFINITE);
	//	�̺�Ʈ �߻�
	virtual bool	Set_Event();
	//	���� ���� �� ���
	bool	Get_Exit();
	//	���� ���� �� ����
	bool	Set_Exit();
};

/*
	������ ó�� �Լ� ���� ���
	
////	�׽�Ʈ ������ ó�� �Լ�
DWORD	WINAPI	������ν�����(LPVOID p_Param)
{
	//	���� ���� ����ȯ.
	F_Thread	*t_Thread = (F_Thread *)p_Param;

	//	���� �ݺ�.
	while(t_Thread->Get_Exit())
	{
		//	���
		t_Thread->Wait_Event(1000);
		
		//	������ ó�� �ڵ�.
		
	}

	//	������ ����.
	delete t_Thread;
	t_Thread = NULL;
	return 0;
}
*/