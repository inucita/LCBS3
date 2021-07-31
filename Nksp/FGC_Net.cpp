
/*
	FGC_Net : ���� ��� ���� ó�� F_CSocket_Event Ŭ������ �̿��Ͽ� ������ �޽��� ó���Ѵ�.
	�ۼ��� : �迵ȯ 2011,12,05
	FGC : Forthen Game Client
	Net : Network Modul.

    �ش� �ҽ��� Nksp.dll �޸�� ���� ������ �����ϰ� Engine.dll���� ��� ó���� �����ϵ��� �Ѵ�.
	���� Engine.dll���� ó���� ��� �κ��� �Լ��� �����͸� �����Ѵ�.

	���� �ڵ�� ������ C ������� �ۼ��Ѵ�.

    FF_ : Forthen Function �Դϴ�.*/

#define POINTER_64

#include <F_CSocket_Event.h>
#include "FGC_Net.h"


/*
	���� ���� ����
*/
//	��� ���� : �ٸ� �ҽ����� �ش� Ŭ���� ���¸� ���� ��. ���� FGC_Net.cpp ������ ó��.

F_CSocket_Event	*g_Sub_Socket = NULL;

//	��� ���� ���� �Լ� : F_CSocket_Event�� ������ ���� ���� ó�� �Լ� �̹Ƿ� �ܺ� ���� ���� �ʴ´�.
bool	FF_Close_Socket(void *p_Socket)
{
	if(p_Socket == NULL)
		return true;

	if(g_Sub_Socket == p_Socket)
	{
		F_CSocket_Event::Delete_Socket(g_Sub_Socket);
		g_Sub_Socket = NULL;
		//	���� ���� ���� ���� ó���� ���׵� ó���Ѵ�.
		//	������ ����..
	}
	else
	{
		return false;
	}
	return true;
}


//	��� ���� �Լ�
int	FF_Connet_Server(char *p_IP,int p_Port)
{
	if(p_IP == NULL)
		return -10;	// ��Ĺ �ּ� ����
	if(p_Port < 1)
		return -11;	// ��Ʈ ��ȣ ����
	if(g_Sub_Socket != NULL)
	{
		//	�̹� ������
		return 2;
	}
	//	���� ���� : ���� �� �޴� ���۴� �ִ� 2048����Ѵ�. 
	g_Sub_Socket = F_CSocket_Event::Connect_Socket(p_IP,p_Port,FF_Close_Socket,2048);
	if(g_Sub_Socket == NULL)
	{
		//	���� : ���� ���� ����.
		return 0;
	}
	//	���� : ���� ���� ����
	return 1;
}

//	��� ���� ���� �Լ�
int	FF_DisConnet_Server()
{
	//	���� �ȵ�
	if(g_Sub_Socket == NULL)
		return 0;
	//	���� ���� ó��
	FF_Close_Socket(g_Sub_Socket);
	return 1;
}

//	���� ���� Ȯ�� �Լ�
int	FF_IS_Connect()
{
	if(g_Sub_Socket == NULL)
		return 0;
	//	��Ÿ ���°��� ����� ���� ��� ����.
	return g_Sub_Socket->Get_Connect_State();
}

/*
	Engine.Dll ���� ó�ϴ� �Լ�
*/
//	���� �޽��� ó�� : ���� �޽��� ���ۿ� ���� ũ�⸦ �����Ѵ�. �ش� ���۴� ��ȣȭ�� ���·� ������.
int	FF_Send_Message(char *p_Buffer,int p_Buffer_Size)
{
	//	���� ���۰� ����.
	if(p_Buffer == NULL)
		return -12;
	//	������ ������� �ʾҴ�.
	if(g_Sub_Socket == NULL)
		return -1;
	if(g_Sub_Socket->Send_Data(p_Buffer,p_Buffer_Size) < 1)
	{
		//	������ ������ ó������ ���� ����.
		//	�޽��� ���� ����.
		return 0;
	}
	//	�޽��� ���۵�.
	return 1;
}

//	���� �޽��� �������� : �������� ���� �޽����� �����´�. �ش� ���۴� ��ȣȭ�� ������ �����̴�.
//	���� ���۸� ����� �Ŀ� �ݵ��� Free() �Լ��� �̿��Ͽ� �����Ѵ�.
char	*FF_Recv_Message(int *p_Recv_Size)
{
	//	������ ������� ���� ����.
	if(g_Sub_Socket == NULL)
	{
		p_Recv_Size = 0;
		return NULL;
	}
	//	�޽��� ��������.
	return g_Sub_Socket->Recv_Date(p_Recv_Size);
}