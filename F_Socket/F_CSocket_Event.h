#pragma once
/*
	�̺�Ʈ ��� ���� Ŭ���̾�Ʈ ���� ���
	�ۼ��� : �迵ȯ
*/
#include "F_CSocket.h"
#include "F_Critical.h"
#include "F_WSThread.h"
#include "F_Thread.h"
#include "F_Buffer_L.h"

//	���� ���� �Լ��� ������
typedef bool (*SOCKET_CLOSE)(void *p_Socket);

////	���� �̺�Ʈ ������ �Լ�
DWORD	WINAPI	FF_Socket_Event_Thread(void *p_Param);


//	�̺�Ʈ ��� ���� Ŭ���̾�Ʈ
class F_CSocket_Event
{
public:
	F_CSocket_Event(void);
	~F_CSocket_Event(void);
	//	���� ���� ��
	enum State
	{
		Init = 0,	// �ʱ�ȭ
		WaitConnect,// ���� ���
		DisConnect, // ���� ����
		Connect		// �����
	};

	//	Ŭ���� ���� ����
public:
	//	���� Ŭ����
	F_CSocket	*m_Socket;
	//	���� �̺�Ʈ ������ Ŭ����
	F_WSThread	*m_SThread;
	//	���� ������� Ȯ�� ����
	int		m_Connect_State;
	//	���� �Լ� ������
	SOCKET_CLOSE	m_Close_Func;

	//	���� ���� �� ó�� ������ �ʿ�.(���� ������ �κе� �߰� �ʿ�).

	//	���� �޽��� ���ۿ� �߰� �ʿ�.
	F_Buffer_L	*m_Recv_Buffer;
	

	//	Ŭ���� ���� �Լ�
public:
	//	���� ���� ����
	bool	Release();
	//	���� ���� ���� Ȯ��
	int		Get_Connect_State();
	//	���� ���� ���� ����
	int		Set_Connect_State(int p_State);

	//	���� Ŭ���� ���
	F_CSocket	*Get_Socket();
	//	ó�������� ���
	F_WSThread	*Get_Thread();
	//	���� ���� ���
	F_Buffer_L	*Get_Recv_Buffer();
	
	//	�޽��� ������
	int		Send_Data(char *p_Buffer,int p_Size);
	//	���� �޽��� ��� (���� �����Ͱ� �ִ� ��� p_Recv_Size ���� ũ�Ⱚ�� ����, �Լ��� ���� �����͸� �����Ѵ�.
	//	���� ���۸� ����� �Ŀ� �ݵ��� Free() �Լ��� �̿��Ͽ� �����Ѵ�.
	char	*Recv_Date(int *p_Recv_Size);

	//	Ŭ���� �ܺ� �Լ�
public:
	//	���� ���� �� ���� ���� �Լ� (������ ������ ��ȯ �Ѵ�)
	//	p_Close_Func �Լ� ���� �ϳ��� �ϳ��� �����ϴ� ���� ����. ���� �Լ��� ���� ���Ͽ� ����ϴ� ��� ũ��Ƽ�ü����� �������
	static F_CSocket_Event	*Connect_Socket(char *p_IP,int p_Port,SOCKET_CLOSE p_Close_Func,int p_Recv_Buffer_Size = 2048);
	//	������ ���� ����
	static bool	Delete_Socket(F_CSocket_Event *p_Socket);
};

