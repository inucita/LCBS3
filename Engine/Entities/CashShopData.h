#ifndef _CASHSHOP_DATA_H
#define _CASHSHOP_DATA_H
#ifdef PRAGMA_ONCE
#pragma once
#endif
#include <Engine/Entities/Entity.h>

#define	MAX_CASHSHOP_CLASS					10		// wooss 050902 a kind of cash shop items

enum CASH_SHOP_KIND
{	
	CONSUME_ITEM = 0,
	TIME_LIMIT_ITEM,
	POTION_ITEM,
	AVATA_ITEM,
	WEAPON_ITEM,
	DEFEND_ITEM,
	SET_ITEM,
	PLATINUM_ITEM,
	CASH_SHOP_TOTAL,
};
// 2010.11 new defined Cash Category
// �̰��� �� ���� ǥ������ ��´�.(��ȹ������ ���� ��)
// 2010.11�� ��ȹ������ ������ Category���� ��� ��ǰ�� ���������� ���ú��� �ٸ��� ���� ���� ���� ���� ����.(������ ��ȹ �ǵ��� ��߳�)
// Cash type ������ 10000 ������ ���� �ȴ�.
// CASH_CATEGORY * 10000 = CC
// ���� subtype�鵵 ���� ���� Add�Ǿ� ���ȴ�.
// first subtype = 100 ����
// second subtype = 1����
// ��ǰ �ϳ��� ������ 
// (CASH_CATEGORY * 10000) + (first subtype * 100) + (second subtype) = goods type flag

enum CASH_CATEGORY
{
	CC_HOTandNew,	// Hot & New ��ǰ
	CC_Platinum,	// �÷�Ƽ�� ��ǰ
	CC_SpendGoods,	// �Һ� ��ǰ
	CC_EquipGoods,	// ��� ��ǰ
	CC_AVATA,		// �ƹ�Ÿ
	CC_ServiceGoods,// ���� ��ǰ
	CC_PackageGoods,// ��Ű�� ��ǰ
	CC_Total,
};
// �������� ����ϰ� �ִ� Flag ����.. Ŭ�󿡼��� �����ϰ� ������ ����.
#define		CATALOG_FLAG_NEW						(1 << 0)			// �Ż�ǰ
#define		CATALOG_FLAG_POPULAR					(1 << 1)			// �α��ǰ
#define		CATALOG_FLAG_DISCOUNT					(1 << 2)			// Ư����ǰ
#define		CATALOG_FLAG_RECOMM_1					(1 << 3)			// ��õ������
#define		CATALOG_FLAG_RECOMM_2					(1 << 4)			// ��õ������
#define		CATALOG_FLAG_RECOMM_3					(1 << 5)			// ��õ������
#define		CATALOG_FLAG_RECOMM_4					(1 << 6)			// ��õ������
#define		CATALOG_FLAG_RECOMM_5					(1 << 7)			// ��õ������
#define		CATALOG_FLAG_RESV1						(1 << 8)			// �����÷���
#define		CATALOG_FLAG_RESV2						(1 << 9)			// �����÷���
#define		CATALOG_FALG_TO_LIMIT(a, b)				(b = 0 | (a >> 10))	// īŻ�α�FLAG ���
#define		CATALOG_FALG_FROM_LIMIT(a, b)			(a = a | (b << 10))	// īŻ�α�FLAG �Է�
#define		CATALOG_FLAG_MAX_LIMIT					(2000000)			// īŻ�α��÷���LIMIT MAX

// subtype�� ���� �����Ӵ�. subtype������ ĳ�ü� ���½� ������ ���� �ް� �� ó�� �ȴ�.
// �⺻������ type�̶� ���� �츮 ���� ���� ��ǰ�� ���� ���������� ���� ��ȭ�� ���� ���̸�(���ο� ��ǰ�� �߰��� ��� ���� �� �ְ���),
// subtype�� �⺻������ �츮 �����۵鿡 �°� ��� ���ǵǾ� ����.

// ���������� ĳ�� ������ type���Ǹ� �״�� ����Ѵ�.
// 2010.11.17
// �ٽ� type �������� ������. ���������� ��� Ÿ���� �����ϰ�, ���� ����ϴ� Ÿ���� ������ �Ǿ� �ִ�.
// ��� type�� Ŭ���̾�Ʈ�� �˰� �־�� �Ѵ�.

// ���� �������� ����� Type values
// 1) Category
#define CATEGORY_HOTNNEW				10000	// HOT & NEW
#define CATEGORY_PLATINUM				20000	// �÷�Ƽ��
#define CATEGORY_DISPOSABLE				30000	// �Һ��ǰ
#define CATEGORY_COSTUME				40000	// ����ǰ
#define CATEGORY_PET					50000	// �ƹ�Ÿ
#define CATEGORY_SERVICE				60000	// ���� ��ǰ
#define CATEGORY_PACKAGE				70000	// ��Ű�� ��ǰ

// 2) Type ����(First subtype)
// none type���� ���� first subtype������ �׷� �˻��� �ȵȴ�.
#define TYPE_HOTNNEW_NONE				10100	// HOT & NEW None(none�̶�� ������ ��ǰ Ÿ���� �ִ�. ���� ��ü�� �ǹ����� �ʴ´�.)
#define TYPE_HOTNNEW_NEW				10200	// HOT & NEW �Ż�ǰ
#define TYPE_HOTNNEW_DISCOUNT			10300	// HOT & NEW Ư��
#define TYPE_PLATINUM_NONE				20100	// �÷�Ƽ�� NONE
#define TYPE_DISPOSABLE_NONE			30100	// �Һ� ��ǰ NONE
#define TYPE_DISPOSABLE_CHARACTER_GROW	30200	// �Һ� ��ǰ ĳ���� ����
#define TYPE_DISPOSABLE_ABILITY_BUILDUP	30300	// �Һ� ��ǰ �ɷ�ġ ��ȭ
#define TYPE_DISPOSABLE_POTION			30400	// �Һ� ��ǰ ����
#define TYPE_EQUIP_NONE					40100	// ��� ��ǰ NONE
#define TYPE_EQUIP_EQUIPMENT			40200	// ��� ��ǰ ���
#define TYPE_EQUIP_BUILDUP				40300	// ��� ��ǰ ��� ��ȭ
#define TYPE_AVATAR_NONE				50100	// �ƹ�Ÿ NONE
#define TYPE_AVATAR_PET					50200	// �ƹ�Ÿ �� ��ǰ
#define TYPE_AVATAR_MERCENARY			50300	// �ƹ�Ÿ �뺴
#define TYPE_SERVICE_NONE				60100	// ���� ��ǰ NONE
#define TYPE_SERVICE_CONENIENCE			60200	// ���� ��ǰ ���� ��ǰ
#define TYPE_SERVICE_ETC				60300	// ���� ��ǰ ��Ÿ
#define TYPE_PACKAGE_NONE				70100	// ��Ű�� ��ǰ NONE

// 3) SubType ����(second subtype)
#define SUBTYPE_HOTNNEW_NONE_NONE					10101	// HOT & NEW NONE, NONE
#define SUBTYPE_HOTNNEW_NEW_NONE					10201	// HOT & NEW �Ż�ǰ NONE
#define SUBTYPE_HOTNNEW_DISCOUNT_NONE				10301	// HOT & NEW Ư�� NONE
#define SUBTYPE_PLATINUM_NONE_NONE					20101	// �÷�Ƽ�� NONE NONE
#define SUBTYPE_DISPOSABLE_NONE_NONE				30101	// �Һ� ��ǰ NONE NONE
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_NONE		30201 //�Һ��ǰ_ĳ���ͼ���_NONE
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_EXP		30202 // �Һ��ǰ_ĳ���ͼ���_����ġ
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_SP		30203 // �Һ��ǰ_ĳ���ͼ���_���õ�
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_DROP		30204 // �Һ��ǰ_ĳ���ͼ���_�����
#define SUBTYPE_DISPOSABLE_CHARACTER_GROW_MIX		30205 // �Һ��ǰ_ĳ���ͼ���_ȥ��
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_NONE		30301 // �Һ��ǰ_�ɷ�ġ��ȭ_NONE
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_HPEXT	30302 // �Һ��ǰ_�ɷ�ġ��ȭ_HPȮ��
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_MPEXT	30303 // �Һ��ǰ_�ɷ�ġ��ȭ_MPȮ�� 
#define SUBTYPE_DISPOSABLE_ABILITY_BUILDUP_VOL		30304	// VOL = Velocity of Light �Һ��ǰ_�ɷ�ġ��ȭ_����
#define SUBTYPE_DISPOSABLE_POTION_NONE				30401 // �Һ��ǰ_����_NONE
#define SUBTYPE_DISPOSABLE_POTION_HPRECOVER			30402 // �Һ��ǰ_����_HPȸ��
#define SUBTYPE_DISPOSABLE_POTION_MPRECOVER			30403 // �Һ��ǰ_����_MPȸ��
#define SUBTYPE_DISPOSABLE_POTION_DUALRECOVER		30404 // �Һ��ǰ_����_���ȸ�� 
#define SUBTYPE_EQUIP_NONE_NONE						40101 // ����ǰ_NONE_NONE
#define SUBTYPE_EQUIP_EQUIPMENT_NONE				40201 // ����ǰ_���_NONE
#define SUBTYPE_EQUIP_EQUIPMENT_WEAPON				40202 // ����ǰ_���_����
#define SUBTYPE_EQUIP_EQUIPMENT_ARMOR				40203 // ����ǰ_���_����
#define SUBTYPE_EQUIP_EQUIPMENT_COSTUME				40204 // ����ǰ_���_�ڽ�Ƭ
#define SUBTYPE_EQUIP_BUILDUP_NONE					40301 // ����ǰ_���ȭ_NONE
#define SUBTYPE_EQUIP_BUILDUP_MELDING				40302 // ����ǰ_���ȭ_�����ֹ���

// max line of item informaion 
#define	MAX_CASH_ITEMINFO_LINE				20

class ENGINE_API CCashShopData
{
public :

	typedef struct _ItemData
	{
		INDEX		m_itemIndex;
		INDEX		m_flag;
		INDEX		m_plus;
		INDEX		m_option;
		INDEX		m_itemCnt;
	} ITEM_DATA;


	// inner data class
	class CASH_SHOP_DATA
	{
	public :
		INDEX		m_shopItemIndex;
		INDEX		m_type;	// old type
		INDEX		m_cash;
		INDEX		m_mileage;

		INDEX		m_texID;
		INDEX		m_texRow;
		INDEX		m_texCol;
		INDEX		m_flag;
		CHAR		m_enable;

		CTString	m_itemName;
		CTString	m_itemDesc;
		INDEX		m_itemListCnt;
		INDEX		m_typeIndex;		// �� ��ǰ �迭������ �ε��� 

		INDEX		m_limitCnt;			// �÷�Ƽ�� ������ ���� ����

		std::vector<ITEM_DATA> m_vItemDataArray;
		
		CASH_SHOP_DATA() :	m_shopItemIndex(-1) ,
							m_type(0),
							m_cash(0),
							m_mileage(0),
							m_itemListCnt(0) ,
							m_limitCnt(0),
							m_texID(0), m_texRow(0), m_texCol(0) {};
		~CASH_SHOP_DATA() { m_vItemDataArray.clear(); }

		BOOL IsNew() { return static_cast<BOOL>(m_flag & CATALOG_FLAG_NEW); }
		BOOL IsPopular() { return static_cast<BOOL>(m_flag & CATALOG_FLAG_POPULAR); }
		BOOL IsDiscount() { return static_cast<BOOL>(m_flag & CATALOG_FLAG_DISCOUNT); }
		BOOL IsPackage() { return (m_type > CATEGORY_PACKAGE && m_type < (CATEGORY_PACKAGE+10000)); }
		BOOL IsPackageEx() { return (m_itemListCnt > 1); }

		CASH_SHOP_DATA(const CASH_SHOP_DATA& csData)
		{
			(*this) = csData;
		}

		void operator = (const CASH_SHOP_DATA& csData);
		bool operator () ( const CASH_SHOP_DATA& _other )	{ return ((this->m_shopItemIndex == _other.m_shopItemIndex)?true:false); }
	};

	~CCashShopData();

	std::vector<CASH_SHOP_DATA> m_vShopItemArray;

	CASH_SHOP_DATA* GetCashShopData(INDEX shopItemIndex);

	static int	LoadShopDataFromFile(CCashShopData &apShopData, const char* FileName);
};

#endif