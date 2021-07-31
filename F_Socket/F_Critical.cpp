#include <windows.h>
#include "F_Critical.h"

F_Critical::F_Critical(void)
{
	InitializeCriticalSection(&m_CriticalSection);
}

F_Critical::~F_Critical(void)
{
	DeleteCriticalSection(&m_CriticalSection);
}

//	����
void	F_Critical::Enter()
{
	if(this != NULL)
		EnterCriticalSection(&m_CriticalSection);
}
//	����
void	F_Critical::Leave()
{
	if(this != NULL)
		LeaveCriticalSection(&m_CriticalSection);
}

//	����
F_Critical	*F_Critical::New()
{
	F_Critical	*t_Return = new F_Critical;
	return t_Return;
}

//	����
BOOL	F_Critical::Delete(F_Critical *p_Critical)
{
	if(p_Critical == NULL)
		return true;
	//	��������� Ȯ�� �ʿ�
	p_Critical->Enter();
	p_Critical->Leave();
	delete p_Critical;
	return true;
}