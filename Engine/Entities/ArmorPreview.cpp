#include "StdH.h"

#include <Engine/Network/CNetwork.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ArmorPreview.h>
#include <Engine/Util.h>

stArmorInfo* CArmorPreview::m_ArmorPreview[TOTAL_JOB];
stDefaultWearInfo* CArmorPreview::m_DefaultWearInfo[TOTAL_JOB];

void CArmorPreview::Destroy()
{
	int i;
	for( i = 0; i < TOTAL_JOB; ++i )
	{
		SAFE_ARRAY_DELETE(m_ArmorPreview[i]);
		SAFE_ARRAY_DELETE(m_DefaultWearInfo[i]);
	}
}

int CArmorPreview::LoadPreviewArmorDataFromFile(const char* FileName)
{
	FILE *fp		= NULL;
	if ((fp = fopen(FileName, "rb")) == NULL) 
	{
		MessageBox(NULL, "File is not Exist.", "error!", MB_OK);
		return -1;
	}

	fflush(fp);

	int	i;
	int iReadBytes	= 0;

	//////////////////////////////////////////////////////////////////////////	
	// MACRO DEFINITION
	//////////////////////////////////////////////////////////////////////////	
#define LOADINT(d)			iReadBytes = fread(&d, sizeof(int), 1, fp);

	int count = 0;
	int Job2nd = 3;
	// �ʱ� ���� Ŭ���� ����
	LOADINT(count);

	// �ʱ� ���� ������ �⺻ ��� �ε���
	for( i = 0; i < count; ++i )
	{
		m_DefaultWearInfo[i] = new stDefaultWearInfo;
		
		iReadBytes = fread(m_DefaultWearInfo[i], sizeof(stDefaultWearInfo), 1, fp);

		if( iReadBytes <= 0 )
		{
			fclose(fp);
			return iReadBytes;
		}		
	}

	for ( i = 0; i < TOTAL_JOB; ++i)
	{
		// �� ���� ī��Ʈ�� ��ϵǱ⿡ �׸�ŭ ����~!
		LOADINT(count);

		m_ArmorPreview[i] = new stArmorInfo;
		// ������ ������ 5 * 8
		iReadBytes = fread(m_ArmorPreview[i], sizeof(stArmorInfo) , 1, fp);

		if( iReadBytes <= 0 )
		{
			fclose(fp);
			return iReadBytes;
		}
	}

#undef LOADINT

	fclose(fp);

	return iReadBytes;
}
