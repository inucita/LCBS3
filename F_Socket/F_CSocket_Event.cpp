#include "F_CSocket_Event.h"


F_CSocket_Event::F_CSocket_Event(void)
{
	//	���� Ŭ����
	m_Socket = NULL;
	//	���� �̺�Ʈ ������ Ŭ����
	m_SThread = NULL;
	m_Connect_State = F_CSocket_Event::Init;
	m_Close_Func = NULL;
	m_Recv_Buffer = NULL;
}


F_CSocket_Event::~F_CSocket_Event(void)
{
	Release();
}

//	���� ���� ����
bool	F_CSocket_Event::Release()
{
	//	������ �ݱ�
	if(m_SThread != NULL)
	{
		delete m_SThread;
		m_SThread = NULL;
	}
	//	���� �ݱ�
	if(m_Socket != NULL)
	{
		delete m_Socket;
		m_Socket = NULL;
	}
	//	���� ���°� �ʱ�ȭ
	m_Connect_State = F_CSocket_Event::Init;
	//	���� ���� ����
	if(m_Recv_Buffer != NULL)
	{
		delete m_Recv_Buffer;
		m_Recv_Buffer = NULL;
	}
	return true;
}
	
//	���� ���� Ȯ��
int		F_CSocket_Event::Get_Connect_State()
{
	return m_Connect_State;
}
	
//	���� ���� ���� ����
int		F_CSocket_Event::Set_Connect_State(int p_Connect_State)
{
	m_Connect_State = p_Connect_State;
	return m_Connect_State;
}

//	���� Ŭ���� ���
F_CSocket	*F_CSocket_Event::Get_Socket()
{
	return m_Socket;
}

//	ó�������� ���
F_WSThread	*F_CSocket_Event::Get_Thread()
{
	return m_SThread;
}

//	���� ���� ���
F_Buffer_L	*F_CSocket_Event::Get_Recv_Buffer()
{
	return m_Recv_Buffer;
}

//	���� ���� �Լ�
F_CSocket_Event	*F_CSocket_Event::Connect_Socket(char *p_IP,int p_Port,SOCKET_CLOSE p_Close_Func,int p_Recv_Buffer_Size)
{
	if(p_IP == NULL)
		return NULL;
	//	���� �� ���� �����
	F_CSocket_Event *t_Return = NULL;
	t_Return = new F_CSocket_Event;
	if(t_Return == NULL)
		return NULL;
	//	���� ����
	t_Return->m_Socket = new F_CSocket;
	if(t_Return->m_Socket == NULL)
	{
		delete t_Return;
		return NULL;
	}
	//	��� �̺�Ʈ ������ ����
	t_Return->m_SThread = new F_WSThread;
	if(t_Return->m_SThread == NULL)
	{
		delete t_Return;
		return NULL;
	}
	//	���� ���� ����
	t_Return->m_Recv_Buffer = new F_Buffer_L;
	if(t_Return->m_Recv_Buffer == NULL)
	{
		delete t_Return;
		return NULL;
	}
	//	���� ���� ����
	t_Return->m_Recv_Buffer->Init(0);
	//	���� ���� �Լ� ������ ����
	t_Return->m_Close_Func = p_Close_Func;
	//	��� ���� ����
	if(!t_Return->m_Socket->Create_Socket(0,p_IP,p_Port,p_Recv_Buffer_Size))
	{
		delete t_Return;
		return NULL;
	}
	//	�̺�Ʈ�� ���� ����
	if(!t_Return->m_SThread->Create_Event(t_Return->m_Socket->Get_Socket(),FD_CONNECT | FD_READ | FD_CLOSE ))
	{
		delete t_Return;
		return NULL;
	}
	//	���� ó�� ������ ����
	if(!t_Return->m_SThread->Create(FF_Socket_Event_Thread,(void *)t_Return))
	{
		delete t_Return;
		return NULL;
	}
	//	���� ���� �õ�
	if(!t_Return->m_Socket->Connect_Socket())
	{
		//	�����尡 ������� �ִ� ���¿��� 
		//	���� �ʱ�ȭ �ϴ� ��쿡�� ������ ���ῡ�� ó���Ѵ�.
		t_Return->m_SThread->Set_Exit();
		return NULL;
	}
	//	���� ��� ����
	t_Return->Set_Connect_State(F_CSocket_Event::WaitConnect);
	return t_Return;
}

//	������ ���� ����
bool	F_CSocket_Event::Delete_Socket(F_CSocket_Event *p_Socket)
{
	if(p_Socket == NULL)
		return true;
	if(p_Socket->m_SThread != NULL)
	{
		//	�����尡 �����Ǿ� �ִ� ��� �����忡�� �ش� ������ �����ϵ��� �Ѵ�.
		p_Socket->m_SThread->Set_Exit();
	}
	else
	{
		//	�����尡 �������� ���� ��� ������ �����Ѵ�.
		delete p_Socket;
	}
	return true;
}

//	�޽��� ������
int		F_CSocket_Event::Send_Data(char *p_Buffer,int p_Size)
{
	if(p_Buffer == NULL)
		return -1;
	if(m_Socket == NULL)
		return 0;
	//	���� ���°� �ƴ�
	if(m_Connect_State != F_CSocket_Event::Connect)
		return -2;
	return m_Socket->Send_Socket(p_Buffer,p_Size);
}

//	���� �޽��� ���
char	*F_CSocket_Event::Recv_Date(int *p_Recv_Size)
{
	if(p_Recv_Size == NULL)
		return NULL;
	if(m_Socket == NULL)
		return NULL;
	//	���� ���°� �ƴѰ��
	if(m_Connect_State != F_CSocket_Event::Connect)
		return NULL;
	if(m_Recv_Buffer == NULL)
		return NULL;
	//	���� �޽��� ���
	return m_Recv_Buffer->Out_Data_Buffer(p_Recv_Size);
}

////	���� �̺�Ʈ ������ �Լ�
DWORD	WINAPI	FF_Socket_Event_Thread(void *p_Param)
{
	if(p_Param == NULL)
		return 0;
	F_CSocket_Event	*t_CSocket = (F_CSocket_Event *)p_Param;
	//	������ ���� ���
	F_WSThread	*t_SThread = t_CSocket->Get_Thread();
	F_CSocket	*t_Socket = t_CSocket->Get_Socket();
	F_Buffer_L	*t_Recv = t_CSocket->Get_Recv_Buffer();
	if(t_SThread == NULL)
		return 0;	// ���޹��� Ŭ������ ���� ���� �ȵ� : Ŭ���� ���� ó�� ���� �ʿ�.
	if(t_Socket == NULL)
		return 0;	// ���޹��� Ŭ������ ���� ���� �ȵ� : Ŭ���� ���� ó�� ���� �ʿ�.

	//	������ ���� ����
	WSANETWORKEVENTS	t_GetEvent;
	int		t_Recv_Size = 0;
	char	*t_Recv_Buffer = NULL;

	//	���� ó���� ������ �ݺ�.
	while(t_SThread->Get_Exit())
	{
		//	�̺�Ʈ �޽��� ��ٸ�
		t_SThread->Wait_Event_WS(t_Socket->Get_Socket(),&t_GetEvent,WSA_INFINITE);
		//	��Ʈ��ũ �̺�Ʈ�� �߻��ϴ� ���
		if(!t_SThread->Get_Exit())
			break;	// ������ ���� ó�� ��.

		//	���� �޽����� ���� ���.
		if((t_GetEvent.lNetworkEvents & FD_CONNECT) == FD_CONNECT)
		{
			// ����� ���� Ȯ��
			if(t_GetEvent.iErrorCode[FD_CONNECT_BIT] == 0)
			{
				t_CSocket->Set_Connect_State(F_CSocket_Event::Connect);
			}
			else
			{
				//	10060 �̸� ���� ����
				t_CSocket->Set_Connect_State(F_CSocket_Event::DisConnect);
				//	���� ���� �̺�Ʈ �߻��� ���� �ݹ� ó��
				if(t_CSocket->m_Close_Func != NULL)
				{
					t_CSocket->m_Close_Func(t_CSocket);
				}
				else
				{
					//	���� ó���Լ��� ���� ��� �׳� ����
					break;	// ������ ���� ó�� ��.
				}
			}
		}
		//	���� �޽����� �°��
		if((t_GetEvent.lNetworkEvents & FD_READ) == FD_READ)
		{
			//	�޽��� �����´�.
			t_Recv_Buffer = t_Socket->Recv_Socket(&t_Recv_Size);
			if(t_Recv_Buffer != NULL)
			{
				if(t_Recv != NULL)
				{
					if(t_Recv_Size < 1)
					{
						//	�޽��� ũ�� ���� ����
						free(t_Recv_Buffer);
						t_Recv_Buffer = NULL;
						t_Recv_Size = 0;
					}
					else
					{
						//	���� ���ۿ� �ֱ�
						t_Recv->In_Data_Buffer(t_Recv_Buffer,t_Recv_Size);
						t_Recv_Buffer = NULL;
						t_Recv_Size = 0;
					}
				}
				else
				{
					//	���� ���� ����
					free(t_Recv_Buffer);
					t_Recv_Buffer = NULL;
					t_Recv_Size = 0;
				}
			}
			else
			{
				//	���� ���۰� ���� ����
				free(t_Recv_Buffer);
				t_Recv_Buffer = NULL;
				t_Recv_Size = 0;
			}
		}
		//	�ݱ� �޽����� ���� ���
		if((t_GetEvent.lNetworkEvents & FD_CLOSE) == FD_CLOSE)
		{
			//	���� ���� �̺�Ʈ �߻��� ���� �ݹ� ó��
			if(t_CSocket->m_Close_Func != NULL)
			{
				t_CSocket->m_Close_Func(t_CSocket);
			}
			else
			{
				//	���� ó���Լ��� ���� ��� �׳� ����
				break;	// ������ ���� ó�� ��.
			}
		}
	}

	//	������ ���� �Ǹ� ���� ���ϵ� ���� �Ѵ�.
	if(t_CSocket != NULL)
	{
		delete t_CSocket;
		t_CSocket = NULL;
	}
	return 0;
}
