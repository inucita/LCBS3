#ifndef _ITEM_COLLECTION_DATA_H_
#define _ITEM_COLLECTION_DATA_H_

#include <common/header/def_item_collection.h>
#include <Engine/Help/LoadLod.h>
#include <vector>

class CItemCollectionData : public stItemCollection, public LodLoader<CItemCollectionData>
{
public:
	CItemCollectionData();
	static bool loadEx(const char* szFileName);
	static void init();
	static void Release();
	static void CheckInvalidItem();

	static int GetCategoryCount();
	// nCategory = ���� �ε����� ����.
	static int GetThemeCount(int nCategory);
	static int GetTotalThemeCount();
	static int GetThemeCompleteCount(int nCategory);
	static int GetTotalThemeCompleteCount();
	static CItemCollectionData* GetTheme(int nCategory, int nTheme);

	// �׸��ε����θ� ã��. �������� �׸��� ������� �ε����� �׸��ε����� �Է� ��.
	static CItemCollectionData* GetTheme(int nThemeIndex);

	// ��� ����Ǿ� �ִ� ī�װ� �ѹ��� �׸� �ѹ��� ����.
	static void	UpdateItemCount(int nCat, int nTheme, int* pCount);
	static void CompleteTheme(int nCat, int nTheme, int endTime);
	
	// getData(index)�ϱ� ���� ��� ����Ǿ� �ִ� ī�װ� ��ȣ�� �׸���ȣ�� �־��ش�.
	static int calcIndex(int nCat, int nTheme);

	static void ClearData();
	static void GetVectorIndex(CItemCollectionData* pItem, int &nCat, int &nTheme);

public:
	bool IsCompleteTheme()						{ return m_bCompleteTheme;				}
	int GetNeedItemCount()						{ return m_nNeedItemCount;				}
	int GetNeedItemCompleteCount()				{ return m_nNeedItemCompleteCount;		} 
	int GetNeedItemPossessionCount(int nNum)	{ return m_nNeedItemPossessionCnt[nNum];}
	int GetCompleteTime()						{ return m_nEndTime;					}

	inline void SetName(const char* name)	{ theme_name = name;	}
	inline void SetDesc(const char* desc)	{ desc_name = desc;		}
	inline const char*	GetName()			{ return theme_name.c_str();}
	inline const char*	GetDesc()			{ return desc_name.c_str(); }
	inline int& GetTransFlag()				{ return transFlag;			}

	//�̹��� ��Ʈ�� index ǥ��
	void SetNoTranslate();
	void ClearNoTranslate();
	
private:
	typedef std::vector< CItemCollectionData* >		vecTheme;
	typedef std::vector< vecTheme >					vecCategory;

	static vecCategory	m_cont;
	static vecTheme m_invalid_data;

	bool m_bCompleteTheme;
	int	m_nNeedItemCount;
	int	m_nNeedItemCompleteCount;
	int	m_nNeedItemPossessionCnt[DEF_NEED_ITEM_COUNT];
	int m_nEndTime;

	int transFlag;
	std::string theme_name;
	std::string desc_name;
};

#endif // _ITEM_COLLECTION_DATA_H_