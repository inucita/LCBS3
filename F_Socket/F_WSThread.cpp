#include <WinSock2.h>
#include "F_WSThread.h"

F_WSThread::F_WSThread(void)
{
	m_ThreadID = 0;
	m_Thread = NULL;
	m_Event = NULL;
	m_Exit_ID = 0;
}

F_WSThread::~F_WSThread(void)
{
	Release();
}

//	���� �̺�Ʈ ���� �Լ�
bool	F_WSThread::Create_Event(SOCKET p_Socket,long p_Network_Event)
{
	if(m_Event != NULL)
		return false;
	if(p_Socket == 0)
		return false;
	//	���� �̺�Ʈ �ڵ� ����
	m_Event = WSACreateEvent();
	if(m_Event == NULL)
		return false;
	//	�̺�Ʈ�� ��Ʈ��ũ �̺�Ʈ ����
	WSAEventSelect(p_Socket, m_Event, p_Network_Event); 
	return true;
}

//	�����Լ�
bool	F_WSThread::Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param)
{
	//	���� ���� �� ���� ���� Ȯ��.
	if(m_Thread != NULL)
		return false;
	if(p_Thread_Fuc == NULL)
		return false;
	//	���ᰪ ����.
	m_Exit_ID = 1;
	//	������ �ڵ� ����
	m_Thread = CreateThread(NULL,0,p_Thread_Fuc,p_Param,0,(DWORD *)&m_ThreadID);
	if(m_Thread == NULL)
	{
		m_Exit_ID = 0;
		return false;
	}
	return true;
}

//	�����Լ�
bool	F_WSThread::Release()
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
		WSACloseEvent(m_Event);
		m_Event = NULL;
	}
	m_ThreadID = 0;
	m_Exit_ID = 0;
	return true;
}

//	�̺�Ʈ ��� �Լ�
bool	F_WSThread::Wait_Event_WS(SOCKET p_Socket,LPWSANETWORKEVENTS p_Event_Type,DWORD p_Wait_Time)
{
	//	�������� Ȯ��
	if(p_Socket == 0)
		return false;
	if(p_Event_Type == NULL)
		return false;
	//	���� ���� Ȯ��.
	if(m_Event == NULL)
		return false;
	//	 �̺�Ʈ ��ȣ ��� (m_Event �ϳ��� �����ϰ�, �̺�Ʈ ��ȣ�� ���� Ȱ��ȭ �Ѵ�)
	if(WSA_WAIT_FAILED == WSAWaitForMultipleEvents(1, &m_Event, FALSE, p_Wait_Time, FALSE))
		return false;
	//	�߻��� �̺�Ʈ ���� ���
	WSAEnumNetworkEvents(p_Socket, m_Event, p_Event_Type);
	return true;
}

//	�̺�Ʈ �߻�
bool	F_WSThread::Set_Event()
{
	if(m_Event == NULL)
		return false;
	WSASetEvent(m_Event);
	return true;
}
	
//	�̺�Ʈ ������
bool	F_WSThread::Reset_Event()
{
	if(m_Event == NULL)
		return false;
	WSAResetEvent(m_Event);
	return true;
}