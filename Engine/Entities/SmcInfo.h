#pragma once

#ifndef __SMCINFO_H
#define __SMCINFO_H

#pragma warning(disable:4786)

#include <windows.h>
#include <vector>
#include <list>
#include <assert.h>

class CMeshInfo
{
public:
	typedef struct _TEXTURES
	{
		char*	_sTexName;	// �ؽ�ó �̸�
		char*	_sTexTFNM;	// �ؽ�ó�� ���� ���

		_TEXTURES()
		{
			_sTexName = NULL;	_sTexTFNM = NULL;
		}

		_TEXTURES(const _TEXTURES& Tex)
		{
			(*this) = Tex;
		}

		~_TEXTURES()
		{
			if (_sTexName != NULL) { delete [] _sTexName; }
			if (_sTexTFNM != NULL) { delete [] _sTexTFNM; }
		}

		void operator = (const _TEXTURES& Tex)
		{
			if (Tex._sTexName)
			{
				_sTexName = new char[strlen(Tex._sTexName)+1];
				strcpy(_sTexName, Tex._sTexName);
			}

			if (Tex._sTexTFNM)
			{
				_sTexTFNM = new char[strlen(Tex._sTexTFNM)+1];
				strcpy(_sTexTFNM, Tex._sTexTFNM);
			}
		}
	}TEXTUREINFO;

	CMeshInfo() { Clear(); }
	CMeshInfo(const CMeshInfo& Mesh);
	virtual ~CMeshInfo() { Destroy(); }

	void operator = (const CMeshInfo& Mesh);

	void Clear();		// ���� �ʱ�ȭ
	void Destroy();		// �Ҵ�� ���� �ʱ�ȭ

	inline void SetMeshNumber(int num)
	{
		m_MeshNumber = num;
	}

	inline bool CreateMeshTFNM(const char* strTFNM)
	{
		m_MeshTFNM = new char[strlen(strTFNM)+1];
		if (!m_MeshTFNM)
			return false;
		strcpy(m_MeshTFNM, strTFNM);
		return true;
	}

	inline int GetMeshNumber() { return m_MeshNumber; }
	inline int GetTexInfoSize() { return m_vecTextures.size(); }
	inline const char* GetMeshTFNM() { return (const char*)m_MeshTFNM; }
	inline const char* GetTexInfoName(int iIdx) { return (const char*)m_vecTextures[iIdx]._sTexName; }
	inline const char* GetTexInfoTFNM(int iIdx) { return (const char*)m_vecTextures[iIdx]._sTexTFNM; }
	inline void AddTextureInfo(TEXTUREINFO TexInfo) { m_vecTextures.push_back(TexInfo); }

	bool AddTextureInfo(const char* strTexName, const char* strTexTFNM);
	
protected:
	int		m_MeshNumber; // ���� smc������ �޽� ��ȣ
	char*	m_MeshTFNM;	// �޽��� bm���� ���
	
	std::vector<TEXTUREINFO>	m_vecTextures;	// �ؽ�ó ���� ���
};

typedef std::vector<CMeshInfo>::iterator	vecMeshInfoItor;
typedef std::vector<CMeshInfo>	vecMeshInfo;

class CSmcInfo
{
public:
	CSmcInfo() { Clear(); }
	CSmcInfo(const CSmcInfo& SmcInfo);
	virtual ~CSmcInfo() { Destroy(); }

	void operator = (const CSmcInfo& SmcInfo);

	void Clear();
	void Destroy();

	inline bool SetSmcFileName(const char* strName)
	{
		m_SmcFileName = new char[strlen(strName)+1];
		if (!m_SmcFileName)
			return false;
		strcpy(m_SmcFileName, strName);
		return true;
	}

	inline const char* GetSmcFileName() { return (const char*)m_SmcFileName; }
	inline void AddMeshInfo(CMeshInfo Mesh) { m_MeshInfoList.push_back(Mesh); }
	inline int GetMeshInfoListSize() { return m_MeshInfoList.size(); }
	inline CMeshInfo GetMeshInfo(int num) { return m_MeshInfoList[num]; }

	bool SaveSmcInfoText(std::string sFilename);	// Text���Ϸ� ����

protected:
	char*	m_SmcFileName;		// smc ���� �̸�
	vecMeshInfo	m_MeshInfoList; // smc���� ���� �޽� ��������Ʈ
};

#endif