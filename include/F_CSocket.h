/*
	�⺻ ���� Ŭ���� : Ŭ���̾�Ʈ ��
	�ۼ��� : �迵ȯ
	Ver 1.00 : 2011�� 9�� �Ϲ� ���ϰ��� ��
*/
#pragma once
#include <WinSock2.h>

//	1����Ʈ ���� ����ü ���
#pragma	pack(push, 1)

//	���� Ŭ���� ���� ����ü
struct	FS_Socket_INFO
{
	//	�⺻ ����
	int			m_Type;		// 1:����, 0:Ŭ���̾�Ʈ
	char		m_IP[30];	// IP �ּ�
	int			m_Port;		// ��Ʈ ��ȣ
	//	���� �� ���� ����
	SOCKET		m_Socket;	// ��� ����
	WSADATA		m_WsaData;	// ���� ������ ����ü
	SOCKADDR_IN m_Address;	// ���� �ּ� ����ü
};

#pragma pack(pop)

////	���� Ŭ���̾�Ʈ ���� Ŭ����
class F_CSocket
{
	////	Ŭ���� ���� �� ���� �Լ�
public:
	F_CSocket(void);
	~F_CSocket(void);

	////	Ŭ���� ���� ����
public:
	//	���� ����
	FS_Socket_INFO		*m_Info;
	//	�޴� ���� �ִ� ũ��
	int		m_Recv_Size;
	char	*m_Recv_Buffer;

	////	Ŭ���� ���� �Լ�
public:
	//	���� ���� ���� ( p_Type 1:����, 0:Ŭ���̾�Ʈ)
	bool	Alloc_Info(int p_Type,int p_Recv_Buffer_Size);
	//	���� ���� ����
	bool	Free_Info();

	//	���� ���� �ݱ�
	bool	Close_Socket();
	//	���� ����(p_Type [1:����, 0:Ŭ���̾�Ʈ],IP �ּ�, ��Ʈ ��ȣ)
	bool	Create_Socket(int p_Type,char *p_IP,int p_Port,int p_Recv_Buffer_Size);
	//	Ŭ���̾�Ʈ�� ��츸, ���� ���� ȣ��
	bool	Connect_Socket();
	//	������ ��츸, �ּ� ���´�.
	bool	BindListen_Socket();

	//	�� ���� ���� ���
	SOCKET	Get_Socket();

/*
	���� �޽��� ���� 
	���۵� ũ��� ������ ũ�Ⱑ �ٸ� ��� �Լ� �ܺο��� ���� ó��. ���� �����Ϳ� ���� ��ȣ�ʹ� �ܺο��� ó���ؼ� �ѱ��.
*/
	int		Send_Socket(char *p_Buffer,int p_Size);
/*
	���� �޽��� �б�
	���� �޽����� ��ȣȭ �� ��Ÿ ó���� �ܺο��� ó���� ��
*/
	char	*Recv_Socket(int *p_Size);
};
