#include "BatchFilePatcher.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "windows.h"

DLL_API IPatcher* getBatchFilePatcher()
{
	//IPatcher* pPatcher = new CBatchFilePatcher;
	//return pPatcher;
	return CBatchFilePatcher::instance();
}

bool CBatchFilePatcher::patch(_LogDataList& pLogDataList, const SPatchConfig& Config)
{
	if (pLogDataList.empty())
		return false;

	// �ʿ��� ������, list ����
	SOrganizeData organize;

	// ���� ��� ������ ���� RootPath
	organize.strRootPath = Config.strSvnUrl.substr(Config.strSvnUrl.rfind('/') + 1);

	// ����
	_organizeData(pLogDataList, organize);

	std::ofstream fPatcher(Config.strFilePath, std::ios_base::trunc);

	// ���� ����
	_StringIterator iter = organize.listFolders.begin();

	while (iter != organize.listFolders.end())
	{
		fPatcher << "md " << "\"" << *iter << "\"" << std::endl;

		++iter;
	}

	// ���� ����
	_PatchDataIterator iter2 = organize.listPatchData.begin();

	while (iter2 != organize.listPatchData.end())
	{
		std::string path = _makeExportCommand(*iter2, Config);
		fPatcher << path << std::endl;

		++iter2;
	}

	fPatcher.close();

	organize.listPatchData.clear();
	organize.listFolders.clear();

	return true;
}

bool CBatchFilePatcher::_organizeData(_LogDataList& pLogDataList, SOrganizeData& Organize)
{
	std::string& strRootPath = Organize.strRootPath;
	_PatchDataList& listPatchData = Organize.listPatchData;
	_StringList& listFolders = Organize.listFolders;

	// ��� ����
	_LogDataIterator iterData = pLogDataList.begin();

	while (iterData != pLogDataList.end())
	{
		_PathIterator iterPath = iterData->paths.begin();
		int revision = iterData->revision;

		while (iterPath != iterData->paths.end())
		{
			std::string path = iterPath->text;

			// ���� ��Ʈ�������� ��� ����
			std::string parsed = path.substr(path.find(strRootPath) + strRootPath.length() + 1);

			if (iterPath->action != "D") // A, M
			{
				// �߰� �Ǿ����� Ȯ��
				_PatchDataIterator iter = std::find(listPatchData.begin(), listPatchData.end(), parsed);

				// �߰� ���� �ʾ��� ���
				if (iter == listPatchData.end())
					listPatchData.push_back(SPatchData(revision, parsed));
				else if (iter->revision < revision)
					iter->revision = revision;
			}
			else
			{
				// �����ؾ��� ���� �˻�
				_PatchDataIterator iter = std::find(listPatchData.begin(), listPatchData.end(), parsed);

				if (iter != listPatchData.end())
					listPatchData.erase(iter);
			}

			++iterPath;
		}

		++iterData;
	}

	// ���� ����
	_PatchDataIterator iter = listPatchData.begin();

	while (iter != listPatchData.end())
	{
		std::string path = iter->path;
		std::string::size_type pos = path.find('/');

		while (pos != std::string::npos)
		{
			path.replace(pos, 1, "\\");
			pos = path.find('/');
		}

		std::string front = path.substr(0, path.rfind('\\'));

		_StringIterator iter2 = std::find(listFolders.begin(), listFolders.end(), front);

		// �߰� �Ǿ����� Ȯ��
		if (iter2 == listFolders.end())
			listFolders.push_back(front);

		++iter;
	}

	return true;
}

std::string CBatchFilePatcher::_makeExportCommand(const SPatchData& PatchData, const SPatchConfig& Config) const
{
	std::string strExport = PatchData.path;

	// '/' -> "\\"
	std::string::size_type pos = strExport.find('/');

	while (pos != std::string::npos)
	{
		strExport.replace(pos, 1, "\\");
		pos = strExport.find('/');
	}

	std::stringstream command;
	command << "svn export ";
	command << "-r " << PatchData.revision << " ";
	command << "\"" << Config.strSvnUrl << "/" << PatchData.path << "\" ";
	command << "\"" << strExport << "\"";

	return command.str();
}
