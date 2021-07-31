/*
	FGC_Net_Process : Nksp���� ������ ��� ����� ��� ó�� Ŭ����.
	�ۼ��� : �迵ȯ 2011,12,05
	������ : �̾�ȣ 2012,04,13
	FGC : Forthen Game Client
	Net_Process : Network Modul Process

    ���ó�� ��ɸ� ó���Ѵ�.
*/
#include "Engine/Network/CPacket.h"
#include "include/F_Thread.h"
#include <LCCrypt.h>
#include <list>

#pragma once

using namespace std;

// ���� Ŭ������ ������ Interface
typedef int		(*SEND_FUNC) (char* p_Buffer, int iBuffer_Size);
typedef char*	(*RECV_FUNC) (int *p_Recv_Size);
typedef int		(*CONNECT_FUNC) (char *p_Ip, int p_Port);
typedef int		(*FUNC_INT_RETURN) ();
// �� interface �Լ��� ��ȣȭ�� ��Ŷ�� ����Ѵ�.

namespace net
{
	DWORD	WINAPI		SendThreadFunc(LPVOID pParam);
	DWORD	WINAPI		RecvThreadFunc(LPVOID pParam);

	// ����ü�� Ŭ������ Ȯ���� �ʿ伺�� ������ ����~
	struct LocalMsgBuffer 
	{
		UBYTE	ubType;
		UBYTE	msgBuf[CNM_TEMP_BUFFER_LENGTH];
		int		msgSize;

		void Clear()
		{
			memset(msgBuf, 0, CNM_TEMP_BUFFER_LENGTH);
		}
	};

	class lmbExtendQueue : public list<LocalMsgBuffer*>
	{
	private:
		void ReleaseElement(LocalMsgBuffer* lmbInput); // pop() ��Ҹ� Release�Ѵ�.

	public:
		void				AddNetworkMessage(CNetworkMessage& istr); // push_back�� LocalMsgBuffer�� ���
		bool				getFrontElement(CNetworkMessage& istr);
		void				ReleaseAll();
	};
} // net namespace end

// �ϴ� send, recv �����带 �ΰ� ���� ����. (���߿� ���ļ� �� ���� �ִ�.)
// ThreadState�� ��쿡 ���� ������ ��� �߰�����. (recv, send ���� �����̴�.)
namespace FGC_STATE
{
	typedef enum socketThreadState
	{
		SOC_DISCONNECTED,
		SOC_AUTH,
		SOC_TASK_WAIT,
		SOC_MOVING_ZONE,
		SOC_RESTART,
		SOC_CLOSE,
	}SOCKET_STATE;
}

// 1. singleton class�� ������
// 2. Naming�� ������ ������.

class ENGINE_API FGC_Net_Process
{
private:
	CTString				m_IpAddress;		// ���� Ip ����
	int						m_Port;				// ���� ��Ʈ ����

	ULONG					m_Sequence;			// Send Sequence index
	
	// [2013/01/09] sykim70
	CLCCRC32 m_crc;

	// ����� ������ ���� ����
	F_Thread*				m_pRecvEventThread;
	F_Thread*				m_pSendEventThread;

	HANDLE					m_hExitTask[2];	// send, recv ������ ���Ḧ ��ٸ��� �̺�Ʈ �ڵ�

	// ����� �޽��� ���� ����
	//list<LocalMsgBuffer*>	m_recvBuffer;
	//list<LocalMsgBuffer*>	m_sendBuffer;
	net::lmbExtendQueue		m_recvBuffer;
	net::lmbExtendQueue		m_sendBuffer;

	// ������ (Task) ���� ���� ����
	FGC_STATE::SOCKET_STATE	m_FGCState;

	// linked functions
	CONNECT_FUNC			connectSocket;		// ���� �� �սô�.
	FUNC_INT_RETURN			disconnectSocket;	// ���� ���ھ��.
	FUNC_INT_RETURN			isConnectSocket;	// ���� ���ΰ���?
	SEND_FUNC				sendToSocket;		// ��Ŷ�� ������.
	RECV_FUNC				receiveSocket;		// ��Ŷ�� �޾ƿ�.

	// static single instance ����
	static FGC_Net_Process*	g_FGC;

public:
	// Functions
	FGC_Net_Process();			// �⺻ ������
	FGC_Net_Process(
		CONNECT_FUNC pConnect,
		FUNC_INT_RETURN pDisConnect,
		FUNC_INT_RETURN pIsConnect,
		SEND_FUNC pSend,
		RECV_FUNC pReceive);	// ���� ������
	~FGC_Net_Process();			// �Ҹ���

	// Accessor function
	void			SetIpAddress(CTString strIp)		{ m_IpAddress = strIp; }
	const CTString	GetIpAddress()						{ return m_IpAddress; }

	void			SetPort(int port)					{ m_Port = port; }
	int				GetPort()							{ return m_Port; }

	void			IncSequence(void)					{ ++m_Sequence; }	// ������ �������� ���� ó��
	ULONG			GetSequence(void)					{ return m_Sequence; }

	F_Thread*		GetSendThread(void)					{ return m_pSendEventThread; }
	F_Thread*		GetRecvThread(void)					{ return m_pRecvEventThread; }

	const FGC_STATE::SOCKET_STATE&	GetFGCState(void)									{ return m_FGCState; }
	void							SetFGCState(const FGC_STATE::SOCKET_STATE& state)	{ m_FGCState = state; }

	// Member function
	CPacket*		MakePacketHeader(const void *pvReceive, int packetSize); // Packet Header ����
	BOOL			SendChannelAuth(void);//(int nChannel, int charIndex, CTString charName);
	BOOL			RecvPacket(void); // ��Ŷ �ޱ�
	BOOL			SendPacket(void); // ��Ŷ ������(Main)
	BOOL			SendPacket(void *pvSend, int packetSize); // ��Ŷ ������(sub)

	bool			MsgCryption(net::LocalMsgBuffer& lmbMsg);
	bool			MsgCryption(net::LocalMsgBuffer* lmbMsg);
	bool			MsgCryption(CNetworkMessage& iStr);
	bool			MsgDecryption(net::LocalMsgBuffer* lmbMsg);

	bool			AddSendBuffer(CNetworkMessage& iStr);
	bool			PopRecvBuffer(CNetworkMessage& iStr);

	bool			IsConnectSubHelper();
	bool			DisConnectSubHelper();
	bool			ConnectSubHelper();

	void			SubHelperRecvProcessing(void);

	void			SetExitEvent(int iValue) 
	{
		if (m_hExitTask[iValue])
			SetEvent(m_hExitTask[iValue]);
	}

public:
	// static functions
	static FGC_Net_Process* CreateFGC(
		CONNECT_FUNC pConn,
		FUNC_INT_RETURN pDisConn,
		FUNC_INT_RETURN pIsConn,
		SEND_FUNC pSend,
		RECV_FUNC pRecv)
	{
		if ( NULL == g_FGC)
		{
			g_FGC = new FGC_Net_Process(pConn, pDisConn, pIsConn, pSend, pRecv);
		}

		return g_FGC;
	}
	static void ReleaseFGC(void)
	{
		if (g_FGC)
		{
			delete g_FGC;
		}

		g_FGC = NULL;
	}
	static FGC_Net_Process* GetFGC(void)
	{
		return g_FGC;
	}
};

#define FGC		FGC_Net_Process::GetFGC()
