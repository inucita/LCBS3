#ifndef __CHANGE_WEAPON__
#define __CHANGE_WEAPON__

#include <map>
#include <vector>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>

#define DEF_TRADE_CONDITION_INDEX	0	// ��ȯ ���� ��� ���� �ε���
#define DEF_TRADE_WEAPON_MAX		13	// ��ȯ ������ �Ѱ��� 0 == ��ȯ���� ��� ��
#define DEF_TRADE_ARMOR_MAX			9 // ��ȯ ���� �Ƹӷ� �Ѱ��� 0 == ��ȯ���� ��� ��
// ����Ʈ ������� ��ȯ ��� ���� TOTAL_JOB - (1 ����Ʈ������) + ( 1 ��ȯ ��) 

struct stChangeWeapon
{
	int index[DEF_TRADE_WEAPON_MAX];
};

struct stChangeArmor
{
	int index[DEF_TRADE_ARMOR_MAX];
};

const int nWeaponSelType[DEF_TRADE_WEAPON_MAX - 1] = {
	CItemData::ITEM_WEAPON_KNIFE,	
	CItemData::ITEM_WEAPON_CROSSBOW,
	CItemData::ITEM_WEAPON_STAFF,	
	CItemData::ITEM_WEAPON_BIGSWORD,
	CItemData::ITEM_WEAPON_AXE,		
	CItemData::ITEM_WEAPON_SSTAFF,
	CItemData::ITEM_WEAPON_BOW,		
	CItemData::ITEM_WEAPON_DAGGER,
	CItemData::ITEM_WEAPON_TWOSWORD,
	CItemData::ITEM_WEAPON_WAND,		
	CItemData::ITEM_WEAPON_SCYTHE,
	CItemData::ITEM_WEAPON_POLEARM,			
};

const int nClassSelType[TOTAL_JOB - 1] = {
	TITAN,
	KNIGHT,
	HEALER,
	MAGE,
	ROGUE,
	SORCERER,
#ifdef CHAR_EX_ROGUE
	EX_ROGUE,
#endif
#ifdef CHAR_EX_MAGE
	EX_MAGE,
#endif
};

class ChangeWeapon
{
public:
	ChangeWeapon(void);
	virtual ~ChangeWeapon(void);

	void Clear();

	void SetInvenTab(SWORD nTab)		{ m_nTab = nTab;					}
	void SetInvenIdx(SWORD nIdx)		{ m_nInvenIdx = nIdx;				}
	void SetVirtualIndex(int nIdx)		{ m_nItemVirtualIdx = nIdx;			}
	void SetSelectIdx(int nSelect)		{ m_nSelectIdx = nSelect;			}
	void SetTradeItemIndex(int nIdx)	{ m_nTradeItemDBIdx = nIdx;			}
	void SetGoldTokenVirIndex(int nIdx)	{ m_nGoldTokenVirtualIdx = nIdx;	}
	void SetGoldTokenCount(int nCnt)	{ m_nGoldTokenCount = nCnt;			}
	// network
	void SendChangeEquipment(BOOL bCash);

private:
	SWORD m_nTab;
	SWORD m_nInvenIdx;
	int m_nTradeItemDBIdx;
	int m_nGoldTokenVirtualIdx;
	int m_nGoldTokenCount;
	int m_nItemVirtualIdx;
	int m_nSelectIdx;
};

typedef std::vector<int>			vec_equip;
typedef std::map<int, vec_equip>	mapChangeEquip;
typedef mapChangeEquip::iterator	mapChangeEquip_iter;

class CChangeEquipment
{
public:
	~CChangeEquipment();
	
	static bool loadEx(const char* FileName);

	static int getTradeWeaponIndex(int nItemIdx, int nSelectIdx);
	static int getTradeArmorIndex(int nItemIdx, int nSelectIdx);

	static bool IsValidWeapon(int nItemIdx);
	static bool IsValidArmor(int nItemIdx);

	static mapChangeEquip m_mapChangeWeapon; // ����
	static mapChangeEquip m_mapChangeArmor; // ��
};

#endif	//	__CHANGE_WEAPON__