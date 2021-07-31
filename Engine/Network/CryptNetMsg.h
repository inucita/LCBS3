#ifndef __CRYPT_NET_MSG_H__
#define __CRYPT_NET_MSG_H__

// ************************************************************************************
#ifdef	CRYPT_NET_MSG_LEVEL2


#define CRYPT_INIT_CHAR				128

static unsigned char	g_ubKey[]	= { 0x77, 0x6C, 0x71, 0x64, 0x70, 0x72, 0x6B, 0x72, 0x68, 0x74, 0x6C, 0x76, 0x65, 0x6B };

/*
#if defined (LC_KOR) || defined (LC_CHN)
#define CRYPT_XOR_STRING			"dkwk"
#else
#define CRYPT_XOR_STRING			"nako"
#endif*/

#define CRYPT_XOR_STRING_LENGTH		4

inline unsigned char* CryptMem(unsigned char* buf, int size, unsigned char arrKeyToken[], int nKeyToken)
{
	char ch = (char)CRYPT_INIT_CHAR;
//	const char* strxor = CRYPT_XOR_STRING;
	const unsigned char* strxor = arrKeyToken;
	int i;
	for (i = 0; i < size; i++)
	{
		buf[i] += ch;
		ch = buf[i];
		buf[i] ^= strxor[i % nKeyToken];
	}
	return buf;
}

inline unsigned char* DecryptMem(unsigned char* buf, int size, unsigned char arrKeyToken[], int nKeyToken)
{
	char ch = (char)CRYPT_INIT_CHAR;
	const unsigned char* strxor = arrKeyToken;
	int i;
	for (i = 0; i < size; i++)
	{
		buf[i] ^= strxor[i % nKeyToken];
		buf[i] -= ch;
		ch = buf[i] + ch;
	}
	return buf;
}

#else	// CRYPT_NET_MSG_LEVEL2
// ************************************************************************************

// 4����Ʈ �������� �� ����Ʈ ���� unsigned char�� ����, ���� ����Ʈ 0 ~ �ֻ��� ����Ʈ 3
#define CNM_BYTE_0(n)		((unsigned char)((n      ) & 0x000000ff))
#define CNM_BYTE_1(n)		((unsigned char)((n >>  8) & 0x000000ff))
#define CNM_BYTE_2(n)		((unsigned char)((n >> 16) & 0x000000ff))
#define CNM_BYTE_3(n)		((unsigned char)((n >> 24) & 0x000000ff))

#define CNM_MAKEWORD(low, high)			((unsigned short)(((high << 8) & 0xff00) | (low & 0x00ff)))
#define CNM_MAKELONG(b0, b1, b2, b3)	((unsigned int)((b0 & 0x000000ff) | ((b1 << 8) & 0x0000ff00) | ((b2 << 16) & 0x00ff0000) | ((b3 << 24) & 0xff000000)))

#define CNM_ROTATE_RIGHT(n)	((unsigned char)((((n << 7) & 0x80) | ((n >> 1) & 0x7f)) & 0xff))
#define CNM_ROTATE_LEFT(n)	((unsigned char)(((n >> 7) & 0x01) | ((n << 1) & 0xfe) & 0xff))

#define CNM_INIT_KEY		((unsigned int)(1852535664))	// �ʱ� Ű �� : nksp

#define CNM_DUMMY_SIZE		(1)								// ���� ������ : ���̴� �޽��� Ÿ�� ��ſ� CNetMsg���� ���
#define CNM_BEGIN_SIZE		(1)								// ���� ��ũ ũ��
#define CNM_END_SIZE		(1)								// �� ��ũ ũ��
#define CNM_CHECKSUM_SIZE	(2)								// üũ�� ũ��
#define CNM_DUMMY_TYPE		((unsigned char)1)				// ����
#define CNM_BEGIN_MARK		((unsigned char)('l'))			// ���� ��ũ : l
#define CNM_END_MARK		((unsigned char)('c'))			// ���� ��ũ : c

#define CNM_CRC				(0x8003)						// CRC-16

// Ű ���� ����
inline unsigned int CNM_KeyGen()
{
	return CNM_MAKELONG((unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand(), (unsigned char)rand());
}

// 16��Ʈ CRC ����
inline unsigned short CNM_CalcCRC(unsigned char* pData, int nLen)
{
	unsigned short crc = 0;
	int i;

	while (nLen-- > 0)
	{
		crc = crc ^ ((int)*pData++ << 8);
		i = 8;
		while (i-- > 0)
		{
			if (crc & 0x8000)
				crc = (crc << 1) ^ CNM_CRC;
			else
				crc <<= 1;
		}
	}

	return crc;
}


////////////////////
// Function name	: CNM_Crypt
// Description	    : �޽��� ��ȣȭ
// Return type		: int 
//                  : ��ȣȭ�� ����, ������ -1
// Argument         : const unsigned char* pSrc
//                  : ���� ������ ����
// Argument         : int nLenSrc
//                  : ���� ����
// Argument         : unsigned int nKey
//                  : ���Ǵ� Ű��
// Argument         : unsigned char** pDest
//                  : ��ȣ ������ ����, new�� �Ҵ���, ȣ���� ������ delete [] �ʿ�
inline int CNM_Crypt(const unsigned char* pSrc, int nLenSrc, unsigned int nKey, unsigned char** pDest)
{
	// ��ȯ ���̺�
	#include "CryptNetMsg_TransTable"

	int nLenDest = nLenSrc + (CNM_BEGIN_SIZE + CNM_END_SIZE + CNM_CHECKSUM_SIZE + CNM_DUMMY_SIZE);
	unsigned short nCheckSum = 0;	// üũ��
	int nIndexSrc = 0;				// pSrc ���� �ε���
	int nIndexDest = 0;				// pDest ���� �ε���
	unsigned char btTrans;			// ��ȯ ���̺��
	unsigned char btKey[4];			// Ű�� ����Ʈ�� �и�

	(*pDest) = NULL;

	if (nLenSrc < 0)
		return -1;

	btKey[0] = CNM_BYTE_3(nKey);
	btKey[1] = CNM_BYTE_2(nKey);
	btKey[2] = CNM_BYTE_1(nKey);
	btKey[3] = CNM_BYTE_0(nKey);

	(*pDest) = new unsigned char[nLenDest];
	memset((*pDest), 0, sizeof(unsigned char) * nLenDest);

	// ���� ����
	(*pDest)[nIndexDest] = CNM_DUMMY_TYPE;
	nIndexDest++;

	// ���۸�ũ ����
	(*pDest)[nIndexDest] = CNM_BEGIN_MARK;
	nIndexDest++;

	// ������ ����
	while (nIndexSrc < nLenSrc)
	{
		(*pDest)[nIndexDest] = pSrc[nIndexSrc];
		nIndexDest++;
		nIndexSrc++;
	}

	// �� ��ũ ����
	(*pDest)[nIndexDest] = CNM_END_MARK;
	nIndexDest++;

	// üũ�� ���� : ���� ����
	nCheckSum = CNM_CalcCRC((*pDest) + CNM_DUMMY_SIZE, nIndexDest - CNM_DUMMY_SIZE);

	// üũ�� ����
	(*pDest)[nIndexDest] = CNM_BYTE_1(nCheckSum);
	nIndexDest++;
	(*pDest)[nIndexDest] = CNM_BYTE_0(nCheckSum);;
	nIndexDest++;

	// ������ ��ȣȭ : ���� ����
	nIndexDest = CNM_DUMMY_SIZE;
	while (nIndexDest < nLenDest)
	{
		// ġȯ
		btTrans = CNM_TransTable[(*pDest)[nIndexDest]];

		// Ű�� XOR
		btTrans ^= btKey[nIndexDest % 4];

		// ���������� Rotate
		btTrans = CNM_ROTATE_RIGHT(btTrans);

		// Ű ����
		btKey[nIndexDest % 4] = btTrans;

		// �� ����
		(*pDest)[nIndexDest] = btTrans;

		nIndexDest++;
	}

	return nLenDest;
}


////////////////////
// Function name	: CNM_Decrypt
// Description	    : �޽��� ��ȣȭ
// Return type		: int 
//                  : ��ȣȭ�� ����, ������ -1
// Argument         : const unsigned char* pSrc
//                  : ��ȣ ������ ����
// Argument         : int nLenSrc
//                  : ��ȣ ������ ����
// Argument         : unsigned int nKey
//                  : ���Ǵ� Ű��
// Argument         : unsigned char** pDest
//                  : ��ȣ ������ ����, new�� �Ҵ���, ȣ���� ������ delete [] �ʿ�
inline int CNM_Decrypt(const unsigned char* pSrc, int nLenSrc, unsigned int nKey, unsigned char** pDest)
{
	// ��ȯ ���̺�
	#include "CryptNetMsg_TransTable"

	int nLenDest = nLenSrc - (CNM_BEGIN_SIZE + CNM_END_SIZE + CNM_CHECKSUM_SIZE + CNM_DUMMY_SIZE);
	unsigned short nCheckSum = 0;	// üũ��
	unsigned char btKey[4];			// Ű�� ����Ʈ�� �и�
	int nIndexSrc = 0;				// pSrc ���� �ε���
	int nIndexDest = 0;				// pDest ���� �ε���
	unsigned char btTrans;			// ��ȯ ���̺��
	unsigned char* pTmpBuf;			// �ӽÿ�

	(*pDest) = NULL;

	if (nLenDest < 0)
		return -1;

	btKey[0] = CNM_BYTE_3(nKey);
	btKey[1] = CNM_BYTE_2(nKey);
	btKey[2] = CNM_BYTE_1(nKey);
	btKey[3] = CNM_BYTE_0(nKey);

	pTmpBuf = new unsigned char[nLenSrc];

	// ������ ��ȣȭ : ���� ����
	nIndexSrc = CNM_DUMMY_SIZE;
	while (nIndexSrc < nLenSrc)
	{
		// ���� ��ȣ���� ����
		btTrans = pSrc[nIndexSrc];

		// �������� Rotate
		btTrans = CNM_ROTATE_LEFT(btTrans);

		// Ű�� XOR
		btTrans ^= btKey[nIndexSrc % 4];

		// ġȯ
		btTrans = CNM_RTransTable[btTrans];

		// Ű����
		btKey[nIndexSrc % 4] = pSrc[nIndexSrc];

		// �� ����
		pTmpBuf[nIndexSrc] = btTrans;

		nIndexSrc++;
	}

	// üũ�� �˻� : ���� ����
	nCheckSum = CNM_MAKEWORD(pTmpBuf[nLenSrc - 1], pTmpBuf[nLenSrc - 2]);
	if (nCheckSum != CNM_CalcCRC(pTmpBuf + CNM_DUMMY_SIZE, nLenSrc - CNM_CHECKSUM_SIZE - CNM_DUMMY_SIZE))
	{
		delete [] pTmpBuf;
		return -1;
	}

	// ���۸�ũ �˻�
	if (pTmpBuf[CNM_DUMMY_SIZE] != CNM_BEGIN_MARK)
	{
		delete [] pTmpBuf;
		return -1;
	}

	// ����ũ �˻�
	if (pTmpBuf[nLenSrc - CNM_CHECKSUM_SIZE - 1] != CNM_END_MARK)
	{
		delete [] pTmpBuf;
		return -1;
	}

	(*pDest) = new unsigned char[nLenDest];
	memcpy((*pDest), pTmpBuf + CNM_BEGIN_SIZE + CNM_DUMMY_SIZE, sizeof(unsigned char) * nLenDest);

	delete [] pTmpBuf;

	return nLenDest;
}

#endif	// CRYPT_NET_MSG_LEVEL2

#endif // __CRYPT_NET_MSG_H__