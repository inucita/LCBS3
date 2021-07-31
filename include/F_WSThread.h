#pragma once
#include "F_Thread.h"
/*
	���� ��� ������ Ŭ����
	�ۼ��� : �迵ȯ
*/

class F_WSThread : public F_Thread
{
	////	���� �� ����.
public:
	F_WSThread(void);
	~F_WSThread(void);

	////	Ŭ���� ���� �Լ�
public:
	//	���� �̺�Ʈ ���� �Լ�
	bool	Create_Event(SOCKET p_Socket,long p_Network_Event);
	//	�����Լ�
	virtual bool	Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param = NULL);
	//	�����Լ�
	virtual bool	Release();
	//	�̺�Ʈ ��� �Լ�
	bool	Wait_Event_WS(SOCKET p_Socket,LPWSANETWORKEVENTS p_Event_Type,DWORD p_Wait_Time = INFINITE);

	////	�̺�Ʈ �߻�
	virtual bool	Set_Event();
	//	�̺�Ʈ ������
	bool	Reset_Event();
};

/*
	������ ó�� �Լ� ���� ���
	
////	�׽�Ʈ ������ ó�� �Լ�
DWORD	WINAPI	������ν�����(LPVOID p_Param)
{
	//	���� ���� ����ȯ.
	F_WSThread	*t_Thread = (F_WSThread *)p_Param;

	//	���� �ݺ�.
	while(t_Thread->Get_Exit())
	{
		//	���
		t_Thread->Wait_Event(����,�̺�Ʈ Ÿ��);
		
		//	������ ó�� �ڵ�.
		
	}

	//	������ ����.
	delete t_Thread;
	t_Thread = NULL;
	return 0;
}
*/