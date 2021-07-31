//������ ���� ����	//(5th Closed beta)(0.2)
#include "stdH.h"

#include "ThreadWrapper.h"
#include <assert.h>
#include <process.h>

//cThreadWrapper::cThreadWrapper(LPTHREAD_START_ROUTINE pRunningFunction)
cThreadWrapper::cThreadWrapper(UINT (WINAPI *pRunningFunction)(LPVOID))
: m_hThreadHandle( 0 )
, m_uiThreadId( 0 )
, m_dwThreadSuspendCount( 0 )
{
	assert(pRunningFunction);
	m_pRunningFunction = pRunningFunction;
}

cThreadWrapper::~cThreadWrapper()
{
	if(m_hThreadHandle != NULL) CloseHandle(m_hThreadHandle);
}

DWORD cThreadWrapper::GetExitCode()
{
	if(m_hThreadHandle != NULL)
	{
		DWORD exitCode = 0;
		if(GetExitCodeThread(m_hThreadHandle, &exitCode))
		{
			return exitCode;
		}
	}
	return 0xFFFFFFFF;
}

BOOL cThreadWrapper::Start(void *parameter)
{
	if(m_hThreadHandle != NULL) CloseHandle(m_hThreadHandle);
	//m_hThreadHandle = ::CreateThread(NULL, 0, m_pRunningFunction, parameter, 0, &m_dwThreadId); C RunTime Func���� �޸� ������ �ִ��Ͽ� �ּ�
	m_hThreadHandle = (HANDLE)_beginthreadex(NULL, 0, m_pRunningFunction, parameter, 0, &m_uiThreadId);
	if(m_hThreadHandle == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL cThreadWrapper::Terminate(DWORD exitCode)
{
	BOOL bRet = TerminateThread(m_hThreadHandle, exitCode);

	// Thread�� ����ɶ� ���� ��ٸ���.(����� ���Ѵ�� ó��)
	WaitForSingleObject(m_hThreadHandle, INFINITE);

	CloseHandle(m_hThreadHandle);
	m_hThreadHandle = 0;
	return bRet;
}

void cThreadWrapper::Resume()
{
	m_dwThreadSuspendCount = ResumeThread(m_hThreadHandle);
}

void cThreadWrapper::Suspend()
{
	m_dwThreadSuspendCount = SuspendThread(m_hThreadHandle);
}
//������ ���� ��	//(5th Closed beta)(0.2)
