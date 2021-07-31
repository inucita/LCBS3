#include <windows.h>
#include "F_Thread.h"

F_Thread::F_Thread(void)
{
	m_ThreadID = 0;
	m_Thread = NULL;
	m_Event = NULL;
	m_Exit_ID = 0;
}

F_Thread::~F_Thread(void)
{
	Release();
}

//	�����Լ�
bool	F_Thread::Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param)
{
	//	���� ���� �� ���� ���� Ȯ��.
	if(m_Thread != NULL)
		return false;
	if(p_Thread_Fuc == NULL)
		return false;
	if(m_Event != NULL)
		return false;
	//	�̺�Ʈ �ڵ� ����
	m_Event = CreateEvent(NULL,false,false,NULL);
	if(m_Event == NULL)
		return false;
	//	���ᰪ ����.
	m_Exit_ID = 1;
	//	������ �ڵ� ����
	m_Thread = CreateThread(NULL,0,p_Thread_Fuc,p_Param,0,&m_ThreadID);
	if(m_Thread == NULL)
		return false;
	return true;
}

//	���� ���� �� ���
bool	F_Thread::Get_Exit()
{
	if(m_Exit_ID == 1)
	{
		//	�ݺ� ó��
		return true;
	}
	else if(m_Exit_ID == 2)
	{
		//	���� ó��.
		m_Exit_ID = 0;
	}
	return false;
}

//	���� ���� �� ����
bool	F_Thread::Set_Exit()
{
	if(m_Exit_ID == 1)
	{
		m_Exit_ID = 2;
		Set_Event();
	}
	else if(m_Exit_ID == 0)
	{
		//	�����尡 �������� �ʾҰŰ� �̹� ���� ó�� �Ȱ��
		return false;
	}
	return true;
}

//	�����Լ�
bool	F_Thread::Release()
{
	//	������ �ڵ� �ݱ�
	if(m_Thread != NULL)
	{
		CloseHandle(m_Thread);
		m_Thread = NULL;
	}
	//	�̺�Ʈ �ڵ� �ݱ�.
	if(m_Event != NULL)
	{
		CloseHandle(m_Event);
		m_Event = NULL;
	}
	m_ThreadID = 0;
	m_Exit_ID = 0;
	return true;
}

//	�̺�Ʈ ��� �Լ�
bool	F_Thread::Wait_Event(DWORD p_Wait_Time)
{
	if(m_Event == NULL)
		return false;
	//	 �̺�Ʈ ��ȣ ���
	WaitForSingleObject(m_Event,p_Wait_Time);
	return true;
}

//	�̺�Ʈ �߻�
bool	F_Thread::Set_Event()
{
	if(m_Event == NULL)
		return false;
	SetEvent(m_Event);
	return true;
}