#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/NpcHelp.h>
//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.09
#include <Engine/Entities/EntityClass.h>
#include <Engine/Entities/EntityProperties.h>
#include <Engine/Ska/StringTable.h>
#include <Engine/World/World.h>
#include <Engine/Interface/UIManager.h>
//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.09
//-----------------------------------------------------------------------------
// Purpose: 
// Input  : &apShopData - 
//			FileName - 
// Output : int
//-----------------------------------------------------------------------------
bool CNpcHelp::loadEx(const char* FileName)
{	
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stNpcHelp* pdata = new stNpcHelp[_nSize];
	fread(pdata, sizeof(stNpcHelp) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CNpcHelp* ptmp = new CNpcHelp;
		memcpy(ptmp, &pdata[i], sizeof(stNpcHelp));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CNpcHelp; // ���̵���Ÿ ����
	memset(m_dummy, 0, sizeof(stNpcHelp));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

bool CMobHelp::loadEx( const char* FileName )
{
	FILE*	fp = NULL;

	fp = fopen(FileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stNpcHelp* pdata = new stNpcHelp[_nSize];
	fread(pdata, sizeof(stNpcHelp) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CMobHelp* ptmp = new CMobHelp;
		memcpy(ptmp, &pdata[i], sizeof(stNpcHelp));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CMobHelp; // ���̵���Ÿ ����
	memset(m_dummy, 0, sizeof(stNpcHelp));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}
