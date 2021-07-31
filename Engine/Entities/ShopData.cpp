#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/ShopData.h>
//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.09
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.09

/*
 *  Constructor.
 */
CShopData::CShopData(void)
: m_iNumOfItem(0)
{
	memset(&m_ShopData, 0, sizeof(TShopData));
	m_ShopData.iIndex	= -1;
}

/*
 *  Destructor.
 */
CShopData::~CShopData(void) 
{
	m_vectorSellItems.clear();
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : iKeeperIdx - 
//			pszShopName - 
//			iSellRate - 
//			iBuyRate - 
//-----------------------------------------------------------------------------
void CShopData::SetData(int iKeeperIdx, const char* pszShopName, int iSellRate, int iBuyRate)
{
	m_ShopData.iIndex	= iKeeperIdx;
	strcpy(m_ShopData.szShopName, pszShopName);
	m_ShopData.iSellRate= iSellRate;
	m_ShopData.iBuyRate	= iBuyRate;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apShopData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
int CShopData::LoadShopDataFromFile(CStaticArray<CShopData> &apShopData, const char* FileName)
{	
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}
	int iNumOfShop	= 0;
	int	iLength		= 0;
	int	iReadBytes	= 0;
	int iLastIndex	= 0;

	//iReadBytes = fread(&iNumOfShop, sizeof(int), 1, fp);		// SHOP �������� ����.
	iReadBytes = fread(&iLastIndex, sizeof(int), 1, fp);		// SHOP�� ������ �ε���.
	apShopData.New(iLastIndex);
	ASSERT(apShopData.Count() > 0 && "Invalid SHOP Data");		
	ASSERT(iLastIndex > 0 && "Invalid SHOP Data");

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);
#define LOADSHORT(d)		iReadBytes = fread(&d, sizeof(short), 1, fp);
#define LOADCHAR(d)			iReadBytes = fread(&d, sizeof(char), 1, fp);
#define LOADFLOAT(d)		iReadBytes = fread(&d, sizeof(float), 1, fp);
#define LOADSTR(d)			{ int iLen; LOADINT(iLen); iReadBytes = fread(&d, iLen, 1, fp); }
	//////////////////////////////////////////////////////////////////////////	

	for(int i = 0; i < iLastIndex; ++i)
	{
		int iIndex = -1;
		LOADINT(iIndex);
		if(iReadBytes <= 0)		break;

		CShopData& SD		= apShopData[iIndex];
		TShopData& ShopData	= SD.m_ShopData;		
		ShopData.iIndex		= iIndex;
		int	iItemCount		= 0;

		LOADSTR(ShopData.szShopName);
		LOADINT(ShopData.iSellRate);
		LOADINT(ShopData.iBuyRate);
		LOADINT(iItemCount);

		SD.m_iNumOfItem		= iItemCount;
		ASSERT(iItemCount > 0 && "Invalid Item Count!!!");
		SD.m_vectorSellItems.resize(iItemCount);

		iReadBytes = fread(&SD.m_vectorSellItems[0],	sizeof(int), iItemCount, fp);	// SHOP�� �Ǹ��ϴ� �������� ����.

		if(iReadBytes < 0)
		{
			MessageBox(NULL, "SHOP ������ ȭ���� �ùٸ��� �ʽ��ϴ�.", "Error!", MB_OK);
			fclose(fp);
			return -1;
		}
	}
	fclose(fp);
	
//////////////////////////////////////////////////////////////////////////	
#undef LOADINT
#undef LOADCHAR
#undef LOADFLOAT
#undef LOADSTR

	return iLastIndex;
}