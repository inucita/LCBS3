#pragma once
#include "F_Thread.h"
/*
	윈속 기반 스레드 클래스
	작성자 : 김영환
*/

class F_WSThread : public F_Thread
{
	////	생성 및 삭제.
public:
	F_WSThread(void);
	~F_WSThread(void);

	////	클래스 내부 함수
public:
	//	윈속 이벤트 생성 함수
	bool	Create_Event(SOCKET p_Socket,long p_Network_Event);
	//	생성함수
	virtual bool	Create(LPTHREAD_START_ROUTINE p_Thread_Fuc,void *p_Param = NULL);
	//	종료함수
	virtual bool	Release();
	//	이벤트 대기 함수
	bool	Wait_Event_WS(SOCKET p_Socket,LPWSANETWORKEVENTS p_Event_Type,DWORD p_Wait_Time = INFINITE);

	////	이벤트 발생
	virtual bool	Set_Event();
	//	이벤트 제설정
	bool	Reset_Event();
};

/*
	스레드 처리 함수 설정 방법
	
////	테스트 스레드 처리 함수
DWORD	WINAPI	내맘대로스레드(LPVOID p_Param)
{
	//	전달 값을 형변환.
	F_WSThread	*t_Thread = (F_WSThread *)p_Param;

	//	무한 반복.
	while(t_Thread->Get_Exit())
	{
		//	대기
		t_Thread->Wait_Event(소켓,이벤트 타입);
		
		//	스레드 처리 코딩.
		
	}

	//	스레드 삭제.
	delete t_Thread;
	t_Thread = NULL;
	return 0;
}
*/