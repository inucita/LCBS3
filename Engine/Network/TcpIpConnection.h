#ifndef _TCPIPCONNECTION_H_
#define _TCPIPCONNECTION_H_


#include "winsock.h"


#define MAX_RECV_BUF_LEN	1024
#define MAX_SEND_BUF_LEN	4096

#define DEFAULT_FRAME		0x76453201

class CTxtQ;


class CTcpIpConnection
{

public:
	
	SOCKET Socket;	

	CTxtQ			 *m_pTxtQueue;
	
	int m_nSendingLen;		
	int m_nSecureMethod;
	int m_nSecureNum;
	
	BOOL m_bConnect;//connect�� �Ǿ�����...
	
	char m_Inbuf[1024]; //ť�� ���� ����
	char m_strSendingBuf[MAX_SEND_BUF_LEN+200];		

public:
	CTcpIpConnection();
	virtual ~CTcpIpConnection();
	
	virtual BOOL InitConnection(SOCKET socket);
	virtual void ResetConnection();	
	
	//sockets		
	SOCKET GetInSocket(){return Socket;};
	SOCKET GetOutSocket(){return Socket;};
	
	//client side of the connection
	virtual void* GetConnectionMessage(DWORD portNo, DWORD& length);
	virtual BOOL ConnectToServer(char* serverName, int port);	
	BOOL	IsConnected(SOCKET sock);
	
	BOOL SendNetMessage(void* msg, BOOL bForce = FALSE, BOOL bSecure = TRUE); //TCP/IP wrapper
	BOOL FlushSendingBuffer();
	void ResetSendingBuffer();	
	
	// ��ȣȭ�� ���� �Լ�
	int ExpandData (char *des, char *src, int size);
	int CompressData (char *des, char * src, int size);	
	
	//���� �ʱ�ȭ
	BOOL InitSocket();		
	
	//���پ� receive �ϱ�.
	int PerformSocketRead(SOCKET desc, char *read_point, int space_left);
	int ProcessInput(); 
	
	int ReadMessage(char *buf, int buf_len, BOOL bSecure = TRUE);

	//����üũ
	int WhatErr();
	
};

extern ENGINE_API CTcpIpConnection _tcpip;

#endif