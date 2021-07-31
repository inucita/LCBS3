#ifndef INCL_SHOPDATA_H
#define INCL_SHOPDATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>
#include <vector>

// Shop�� ������ �����ϴ� Ŭ����.
class ENGINE_API CShopData
{
private:
	typedef struct _tagShopData
	{
		int		iIndex;
		char	szShopName[50];
		int		iSellRate;
		int		iBuyRate;		
	}TShopData, *PTShopData;
public:
	CShopData();
	~CShopData();

	inline	int		GetIndex()			const	{	return	m_ShopData.iIndex;		};
	inline	int		GetSellRate()		const	{	return	m_ShopData.iSellRate;	};
	inline	int		GetBuyRate()		const	{	return	m_ShopData.iBuyRate;	};

	// ������� �κ����� MobName�� ��µɰ���.
	inline	const char*	GetShopName()	const	{	return	m_ShopData.szShopName;	};

	int					m_iNumOfItem;			// �Ǹ��ϴ� �������� ����.
	std::vector<int>	m_vectorSellItems;
private:
	TShopData			m_ShopData;				// Shop�� ���� ������ ���� �ִ� ����ü.
public:
	void		SetData(int iKeeperIdx, const char* pszShopName, int iSellRate, int iBuyRate);
	static int	LoadShopDataFromFile(CStaticArray<CShopData> &apShopData, const char* FileName);
};
#endif