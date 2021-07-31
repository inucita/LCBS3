// ***************************************************************
//  Lacarette   version:  1.0   ? date: 03/03/2010
//  -------------------------------------------------------------
//	Author 	: sora
//  Desc	: ��ī�� ������
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#ifndef	LACARETTE_H_
#define LACARETTE_H_
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#include <vector>
#include <algorithm>

class  CLacarette
{
	enum eLacarette
	{
		LACARETTE_RETTE_MIN = 1,
		LACARETTE_RETTE_MAX = 24,
	};

	INDEX	courseIndex;		// �ڽ� ��ȣ	

	struct stCoinData 
	{
		INDEX	itemIndex;		// ���� ������ �ε���
		SLONG	startCount;		// �⺻ ��� ����
		BYTE	addCount;		// ���ȸ���� ���� �������� ����

		stCoinData()
		{
			itemIndex	= -1;
			startCount	= 0;
			addCount	= 0;
		}
	};

	struct stGiftData
	{
		INDEX	itemIndex;		// ��ǰ ������ �ε���
		SQUAD	itemCount;
		std::vector<BYTE> vecRetteNum;	// ��ǰ�� �ش��ϴ� �귿 ��ȣ����Ʈ

		stGiftData()
		{
			itemIndex = -1;
			itemCount = 0;
		}

		 const int GetRettCount() { return vecRetteNum.size(); }
		 const BYTE GetRettNum( int num ) { return( num < vecRetteNum.size() && num >= 0  ? vecRetteNum[num] : 0 ); }
	};

	std::vector<stCoinData> vecCoinData;
	std::vector<stGiftData>	vecGiftData;
	int		retteData[LACARETTE_RETTE_MAX];

	static std::vector< INDEX > vecCoinIndex;
	
	std::string m_strName;

public:
	enum eGiftData
	{
		LACARETTE_GIFTDATA_TOTAL = 10,
	};

	CLacarette(void);
	~CLacarette(void);

	const INDEX GetCourseIndex()			{ return courseIndex; }
	const char* GetCourseName()				{ return m_strName.c_str(); }
	void		SetName(const char* name)	{ m_strName = name; }

	const int	GetCoinDataCount()			{ return vecCoinData.size(); }
	const INDEX GetCoinDataItemIndex( int num ) { return( num < vecCoinData.size() && num >= 0  ? vecCoinData[num].itemIndex : -1 ); }
	const SLONG GetCoinDataStartCount( int num ) { return( num < vecCoinData.size() && num >= 0  ? vecCoinData[num].startCount : 0 ); }
	const BYTE	GetCoinDataAddCount( int num ) { return( num < vecCoinData.size() && num >= 0  ? vecCoinData[num].addCount : 0 ); }

	const int	GetGiftDataNumByRetteNum( int rettnum ) { return( rettnum <= LACARETTE_RETTE_MAX && rettnum >= LACARETTE_RETTE_MIN ? retteData[rettnum - 1] : -1 ); }
	const INDEX GetGiftDataItemIndex( int num ) { return( num < vecGiftData.size() && num >= 0  ? vecGiftData[num].itemIndex : -1 ); }
	const SQUAD GetGiftDataItemCount( int num ) { return( num < vecGiftData.size() && num >= 0  ? vecGiftData[num].itemCount : 0 ); }
	const SQUAD GetGiftDataRettCount( int num ) { return( num < vecGiftData.size() && num >= 0  ? vecGiftData[num].GetRettCount() : 0 ); }
	const BYTE	GetGiftDataRettNum( int num, int rettnum ) { return( num < vecGiftData.size() && num >= 0  ? vecGiftData[num].GetRettNum( rettnum ) : 0 ); }

	static void LoadLacaretteDataFromFile( CStaticArray<CLacarette> &apLacaretteData, const char *fileName );

	static BOOL IsCoinItem( INDEX itemIndex ) { return( std::find(vecCoinIndex.begin(), vecCoinIndex.end(), itemIndex ) != vecCoinIndex.end() ); }
};


#endif	//ACTION_H_