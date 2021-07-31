#include "stdh.h"

#include <Engine/Templates/StaticArray.cpp>
#include <Engine/Entities/OptionData.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Help/LoadLod.h>

//-----------------------------------------------------------------------------
// Purpose: ������ �����͸� ���Ϸκ��� �о���Դϴ�.
// Input  : &apOptionData - ������ ����� ����� �迭.
//			FileName - ���ϸ�.
// Output : 	static int
//-----------------------------------------------------------------------------
bool COptionData::loadEx(const char* FileName)
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

	stOption* pdata = new stOption[_nSize];
	fread(pdata, sizeof(stOption) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		COptionData* ptmp = new COptionData;
		memcpy(ptmp, &pdata[i], sizeof(stOption));
		if (_mapdata.insert(std::make_pair(ptmp->type, ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new COptionData; // ���̵���Ÿ ����
	memset(m_dummy, 0, sizeof(stOption));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}
