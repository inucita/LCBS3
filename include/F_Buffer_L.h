#pragma once
/*
	��ũ ����� ������ ������ ����. ���ο� ���� ������ ���۴� �������� �ʴ´�.
	���� ���� ������ �����͸� �ٸ� ������ �����ϴ� ����̴�.
	FIFO ����� �Դϴ�.

	�ۼ��� : �迵ȯ
*/

//	���� ��ũ ����Ʈ
struct	FS_BList
{
	int		m_Size;		// ������ ũ��
	char	*m_Data;	// ���� ����Ʈ
	FS_BList	*m_Next;
};

////	��ũ ���� ť Ŭ����
class F_Buffer_L
{
	////	Ŭ���� ���� ����
public:
	F_Buffer_L(void);
	~F_Buffer_L(void);

	////	Ŭ���� ���� ����
public:
	//	���� ��� ����. m_Type = 0 (���� ����), 1 (��ȯ ����)
	int		m_Type;
	int		m_Number;
	//	���� ����Ʈ
	FS_BList	*m_Buffer;
	//	�Է� ����Ʈ
	FS_BList	*m_InPut;
	//	��� ����Ʈ
	FS_BList	*m_OutPut;

	////	Ŭ���� ���� �Լ�
public:
	//	�ʱ�ȭ
	bool	Init(int p_Type = 1,int p_Number = 1000);
	//	��� ����
	bool	Delete();

	//	����Ʈ ���� �����
	static FS_BList	*BList_Alloc(int p_Number = 1);
	//	����Ʈ ���� ����
	static bool		BList_Free(FS_BList *p_List);


	//	������ �Է� �Լ� 
	bool	In_Data_Buffer(char *p_In,int p_Size);

	//	������ ��� �Լ� 
	char	*Out_Data_Buffer(int *p_Size_Out);
};
