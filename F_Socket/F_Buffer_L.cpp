#include <windows.h>
#include "F_Buffer_L.h"

F_Buffer_L::F_Buffer_L(void)
{
	//	���� ����Ʈ
	m_Buffer = NULL;
	//	�Է� ����Ʈ
	m_InPut = NULL;
	//	��� ����Ʈ
	m_OutPut = NULL;
	//	����
	m_Type = 1;
	m_Number = 0;
	//	�ʱ�ȭ ó��.
	Init();
}

F_Buffer_L::~F_Buffer_L(void)
{
	//	��� ���� ó��
	Delete();
}
	
//	�ʱ�ȭ
bool	F_Buffer_L::Init(int p_Type,int p_Number)
{
	//	���� ���� ����
	Delete();
	//	���� ����
	m_Type = p_Type;
	m_Number = p_Number;
	//	���� ����
	if(m_Type == 0)
	{
		//	���� ����Ʈ
		m_Buffer = BList_Alloc();
		//	�Է� ����Ʈ
		m_InPut = m_Buffer;
		//	��� ����Ʈ
		m_OutPut = m_Buffer;
	}
	//	��ȯ ����
	else
	{
		//	���� ����Ʈ�� ���� ��ŭ ������ �Ѵ�.
		m_Buffer = BList_Alloc(m_Number);
		//	��ȯ ���� ����
		m_Buffer[m_Number - 1].m_Next = &m_Buffer[0];
		for(int i = 0 ; i < (m_Number - 1);i++)
		{
			//	���� �ּ� ����.
			m_Buffer[i].m_Next = &m_Buffer[i+1];
		}
		//	�Է� ����Ʈ
		m_InPut = m_Buffer;
		//	��� ����Ʈ
		m_OutPut = m_Buffer;
	}
	return true;
}

//	����Ʈ ���� �����
FS_BList	*F_Buffer_L::BList_Alloc(int p_Number)
{
	if(p_Number == 0)
		return NULL;
	//	����Ʈ ���� 1�� ����
	FS_BList *t_List = (FS_BList *)malloc(p_Number * sizeof(FS_BList));
	memset(t_List,0x00,p_Number * sizeof(FS_BList));
	return t_List;
}

//	����Ʈ ���� ����
bool	F_Buffer_L::BList_Free(FS_BList *p_List)
{
	if(p_List == NULL)
		return false;
	//	����Ʈ ���� ����.
	free(p_List);
	return true;
}

//	��� ����
bool	F_Buffer_L::Delete()
{
	//	���� ����
	if(m_Type == 0)
	{
		//	���� ���� �ּ� ����.
		m_Buffer = m_OutPut;
		//	�ݺ� ���� ó��.
		FS_BList	*t_Buffer = m_Buffer;
		while(t_Buffer != NULL)
		{
			//	���� ������ ����.
			m_Buffer = t_Buffer->m_Next;
			//	t_Buffer ����
			BList_Free(t_Buffer);
			t_Buffer = NULL;
			//	�ݺ� ó���� ���ؼ�
			t_Buffer = m_Buffer;
		}
	}
	//	��ȯ ����
	else
	{
		//	����Ʈ ���� ����.
		BList_Free(m_Buffer);
		m_Buffer = NULL;
	}
	//	������ �ʱ�ȭ
	if(m_InPut != NULL)
		m_InPut = NULL;
	if(m_OutPut != NULL)
		m_OutPut = NULL;
	return true;
}

//	������ �Է� �Լ� 
bool	F_Buffer_L::In_Data_Buffer(char *p_In,int p_Size)
{
	//	���� �� Ȯ��.
	if(p_In == NULL)
		return false;
	if(m_InPut == NULL)
		return false;

	//	 ���� ����
	if(m_Type == 0)
	{
		//	������ �Է�
		if(m_InPut->m_Data != NULL)
		{
			//	�ش� �޸� ���� �˻簡 �ʿ��ϴ�.
			//	��ȿ�� �޸��� ��� ���� ������ �߰��� �ʿ� ����.
			return false;
		}
		m_InPut->m_Size = p_Size;
		//	������ �Է�.
		m_InPut->m_Data = p_In;
		//	���ο� ��ũ ���� ���� ���� ��ũ�� ����.
		m_InPut->m_Next = BList_Alloc();
		if(m_InPut->m_Next == NULL)
		{
			//	��å ����. malloc ����.
			return false;
		}
		//	���� �ּҷ� ����.
		m_InPut = m_InPut->m_Next;
	}
	//	��ȯ ����.
	else
	{
		//	�˻�
		if(m_InPut->m_Next == m_OutPut)
			return false;
		m_InPut->m_Size = p_Size;
		//	������ �Է�
		m_InPut->m_Data = p_In;
		//	���� ��ġ�� �̵�.
		m_InPut = m_InPut->m_Next;
	}
	return true;
}

//	������ �Է� �Լ� 
char	*F_Buffer_L::Out_Data_Buffer(int *p_Size_Out)
{
	//	���� ���� Ȯ��
	if(m_OutPut == NULL)
		return NULL;
	if(m_InPut == NULL)
		return NULL;

	char	*t_Return = NULL;
	//	 ���� ����
	if(m_Type == 0)
	{
		//	��� �����Ͱ� ���� ����
		if(m_OutPut != m_InPut)
		{
			//	��� ������ ����
			t_Return = m_OutPut->m_Data;
			if(p_Size_Out != NULL)
			{
				*p_Size_Out = m_OutPut->m_Size;
			}
			//	���� ����Ʈ ����.
			FS_BList	*d_List = m_OutPut;
			//	���� ��� ������ ����
			m_OutPut = m_OutPut->m_Next;
			//	��� �Ϸ� ���� ����.
			BList_Free(d_List);
			//	������ ���.
			return t_Return;
		}
	}
	//	��ȯ ����
	else
	{
		//	��� �����Ͱ� ���� ����
		if(m_OutPut != m_InPut)
		{
			//	��� ������ ����
			t_Return = m_OutPut->m_Data;
			if(p_Size_Out != NULL)
			{
				*p_Size_Out = m_OutPut->m_Size;
			}
			m_OutPut->m_Data = NULL;
			m_OutPut->m_Size = NULL;
			//	���� ��� ������ ����
			m_OutPut = m_OutPut->m_Next;
			//	������ ���.
			return t_Return;
		}
	}
	//	����� ������ ���� ���
	if(p_Size_Out != NULL)
	{
		*p_Size_Out = 0;
	}
	return NULL;
}
