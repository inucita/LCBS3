#include "F_CSocket.h"

F_CSocket::F_CSocket(void)
{
	m_Info = NULL;
	m_Recv_Size = 0;
	m_Recv_Buffer = NULL;
}


F_CSocket::~F_CSocket(void)
{
	Free_Info();
}

//	���� ���� ����
bool	F_CSocket::Alloc_Info(int p_Type,int p_Recv_Buffer_Size)
{
	//	���� �޸� ����
	Free_Info();
	//	���� �޸� ����
	m_Recv_Size = p_Recv_Buffer_Size;
	//	���� �����
	m_Recv_Buffer = (char *)malloc(m_Recv_Size);
	memset(m_Recv_Buffer,0x00,m_Recv_Size);
	 if(m_Info != NULL)
		 return false;
	//	�޸� ���� Ȯ�� �� �ʱ�ȭ
	m_Info = (FS_Socket_INFO *)malloc(sizeof(FS_Socket_INFO));
	if(m_Info == NULL)
		return false;
	memset(m_Info,0x00,sizeof(FS_Socket_INFO));
	//	Ÿ�� ������ ����/Ŭ���̾�Ʈ �б�
	m_Info->m_Type = p_Type;
	return true;
}

//	���� ���� ����
bool	F_CSocket::Free_Info()
{
	if(m_Info != NULL)
	{
		free((void *)m_Info);
		m_Info = NULL;
	}
	m_Recv_Size = 0;
	if(m_Recv_Buffer != NULL)
	{
		free(m_Recv_Buffer);
		m_Recv_Buffer = NULL;
	}
	return true;
}

//	���� ���� �ݱ�
bool	F_CSocket::Close_Socket()
{
	if(m_Info != NULL)
	{
		// �Ұ� �ݱ�
		if(m_Info->m_Socket != NULL)
		{
			shutdown(m_Info->m_Socket,SD_BOTH);
			closesocket(m_Info->m_Socket);
			m_Info->m_Socket = NULL;
		}
	}
	// ���� DLL �ݱ�
	WSACleanup();
	return true;
}

//	���� ���� (p_Type [1:����, 0:Ŭ���̾�Ʈ],IP �ּ�, ��Ʈ ��ȣ)
bool	F_CSocket::Create_Socket(int p_Type,char *p_IP,int p_Port,int p_Recv_Buffer_Size)
{
	if(p_IP == NULL)
		return false;
	//	���� ���� ���� �ִ� ��� ����
	if(m_Info != NULL)
		return false;
	//	���� �Ѵ�.
	if(!Alloc_Info(p_Type,p_Recv_Buffer_Size))
		return false;

	// ���� DLL ����
	if(SOCKET_ERROR == WSAStartup(0x202,&m_Info->m_WsaData))
		return false;
	//	IP ���� �� ���� �� ����
	memcpy(m_Info->m_IP,p_IP,lstrlenA(p_IP));
	//	��Ʈ ����
	m_Info->m_Port = p_Port;

	// ���� �ּ� ����ü ����
	m_Info->m_Address.sin_family = AF_INET;
	m_Info->m_Address.sin_addr.S_un.S_addr = inet_addr(m_Info->m_IP);
	m_Info->m_Address.sin_port = htons((unsigned short)m_Info->m_Port);
	//	TCP ����  ����
	m_Info->m_Socket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	//	���� ���� ����
	if(m_Info->m_Socket == INVALID_SOCKET)
	{
		Free_Info();
		return false;
	}
	return true;
}

//	Ŭ���̾�Ʈ�� ��츸, ���� ���� ȣ��
bool	F_CSocket::Connect_Socket()
{
	if(m_Info == NULL)
		return false;
	if(m_Info->m_Type == 1)
		return false;
	//	���� ���� ����
	if(SOCKET_ERROR == connect(m_Info->m_Socket,(struct sockaddr*)&m_Info->m_Address,sizeof(sockaddr)))
	{
		if(WSAEWOULDBLOCK == WSAGetLastError())
		{
			// �񵿱���� �ΰ�� ó�� ���ð� �ʿ���
			return true;
		}
		Free_Info();
		return false;
	}
	return true;
}

//	������ ��츸, �ּ� ����Ʈ
bool	F_CSocket::BindListen_Socket()
{
	if(m_Info == NULL)
		return false;
	if(m_Info->m_Type == 0)
		return false;	
	// ���� ���ϰ� �ּҸ� �����ش�.
	if(SOCKET_ERROR == bind(m_Info->m_Socket,(struct sockaddr*)&m_Info->m_Address,sizeof(SOCKADDR_IN)))
	{
		Free_Info();
		return false;
	}
	// ������� ��� ����
	if(listen(m_Info->m_Socket,SOMAXCONN) == SOCKET_ERROR)
	{
		Free_Info();
		return false;
	}
	return true;
}

//	�� ���� ���� ���
SOCKET	F_CSocket::Get_Socket()
{
	if(m_Info == NULL)
		return 0;
	return m_Info->m_Socket;
}

/*
	���� �޽��� ���� 
	���۵� ũ��� ������ ũ�Ⱑ �ٸ� ��� �Լ� �ܺο��� ���� ó��. ���� �����Ϳ� ���� ��ȣ�ʹ� �ܺο��� ó���ؼ� �ѱ��.
*/
int		F_CSocket::Send_Socket(char *p_Buffer,int p_Size)
{
	//	�������� Ȯ��
	if(p_Buffer == NULL)
		return -4;
	if(p_Size < 2)
		return -3;
	//	���� ���� Ȯ��
	if(m_Info == NULL)
		return -2;
	if(m_Info->m_Socket == 0)
		return -1;

	//	���� ������ Ȯ�� ����
	int	t_Send_Size = 0;
	//	���ۿ� �޽��� ������
	t_Send_Size = send(m_Info->m_Socket,p_Buffer,p_Size,0);
	return t_Send_Size;
}

/*
	���� �޽��� �б�
	���� �޽����� ��ȣȭ �� ��Ÿ ó���� �ܺο��� ó���� ��
*/
char	*F_CSocket::Recv_Socket(int *p_Size)
{
	//	�������� Ȯ��
	if(p_Size == NULL)
		return NULL;
	//	���� ���� Ȯ��
	if(m_Info == NULL)
		return NULL;
	if(m_Info->m_Socket == 0)
		return NULL;
	if(m_Recv_Size == 0)
		return NULL;
	if(m_Recv_Buffer == NULL)
		return NULL;
	//	�޽��� ���� ũ�� ����
	int	t_Get_Size1 = 0;
	int	t_Get_Size2 = 0;
	*p_Size = 0;
	
	//	���� ���� ����
	char	*t_Return1 = NULL;
	char	*t_Return2 = NULL;

	//	�޽����� �޴´�.
	memset(m_Recv_Buffer,0x00,m_Recv_Size);
	t_Get_Size1 = recv(m_Info->m_Socket,m_Recv_Buffer,m_Recv_Size,0 );
	if(t_Get_Size1 < 1)
		return NULL;
	//	�޸� ���� �� ������ ����.
	t_Return1 = (char *)malloc(t_Get_Size1);
	memcpy(t_Return1,m_Recv_Buffer,t_Get_Size1);
	t_Get_Size2 = t_Get_Size1;
	//	���ۿ� ������ �°�� �߰� �޽����� �����Ѵ�.
	while(t_Get_Size1 >= m_Recv_Size)
	{
		//	���� �̻��� �޽����� ���� ���
		memset(m_Recv_Buffer,0x00,m_Recv_Size);
		//	������ �޽����� �޴´�.
		t_Get_Size1 = recv(m_Info->m_Socket,m_Recv_Buffer,m_Recv_Size,0);
		//	���� ���۸� �����Ѵ�.
		//	���۸� ����
		t_Return2 = (char *)malloc(t_Get_Size2 + t_Get_Size1);
		//	���۳��� ����
		memcpy(t_Return2,t_Return1,t_Get_Size2);
		//	������ ���� ����
		memcpy(&t_Return2[t_Get_Size2],m_Recv_Buffer,t_Get_Size1);
		//	���� ũ�� ����
		t_Get_Size2 = t_Get_Size2 + t_Get_Size1;
		//	���� ���� ����
		free(t_Return1);
		//	���� ������ ����.
		t_Return1 = t_Return2;
		t_Return2 = NULL;
	}
	//	��ȯ ũ�� ����
	*p_Size = t_Get_Size2;
	//	�ش� �޸� ���۴� �ܺο��� ó���ϰ� �����Ѵ�.
	return t_Return1;
}