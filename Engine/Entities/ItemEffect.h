#ifndef __CITEMEFFECT_H__
#define __CITEMEFFECT_H__

#include <Engine/Base/Types.h>
#include <Engine/Base/Memory.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Entities/ItemData.h>
#include <Engine/Effect/CEffect.h>

class CEffectGroup;
class CTagManager;
class ENGINE_API CItemEffect
{
public:

	typedef struct  _ITEMEFFECTGROUP
	{
		char szEffectName[64];
		CEffectGroup* m_pEffectGroup;

		_ITEMEFFECTGROUP() : m_pEffectGroup(NULL)
		{
			memset(szEffectName, 0, sizeof(szEffectName));
		}

		void operator = (const _ITEMEFFECTGROUP& Temp)
		{ // Only EffectName copy
			strcpy(szEffectName, Temp.szEffectName);
			m_pEffectGroup = NULL;
		}

	}ItemEffectGroup;

	CItemEffect();
	~CItemEffect();

	void Refresh(CTagManager *pTM, SBYTE sbOption);
	void RefreshAccessoryEffect(CTagManager *pTM);
	void RefreshRelicEffect(CTagManager *pTM);
	void RefreshPetStashEffect(CTagManager *pTM);
	void DeleteEffect(const int wearPos, BOOL bClear = FALSE);
	void Clear();
	void Change(const int job, CItemData *ItemData, const int wearPosNew, const int itemPlusNew, CTagManager *pTM, SBYTE sbOption, int nItemSubType );
	void DeleteNickEffect();  // ȣĪ ����Ʈ ����
	void DeletePetStashEffect();  // ȣĪ ����Ʈ ����
	void DeleteLoginJinEffect();
	void DeleteRelicEffect(); // ���� ����Ʈ ����

	BOOL AddAccessoryEffect(const char* strEffectName, CTagManager *pTM);
	BOOL AddWearEffect(const char* strEffectName, const int wearPos, CTagManager *pTM);
	BOOL DelWearEffect(const int wearPos);
	BOOL DelAccessoryEffect(void);
	BOOL AddNickEffect( const char* strEffectFile, CTagManager *pTM  ); // ȣĪ ����Ʈ �߰�

	BOOL AddPetStashEffect( int nEffectType, CTagManager *pTM ); // �� â�� ����Ʈ �߰�
	void AddHolyWaterEffect(int nEffecttype, CTagManager *pTM);
	BOOL AddRelicEffect(int nEffectType, CTagManager *pTM); // ���� ������ ����Ʈ �߰�

	void AddLoginEffect(CTagManager *pTM); // �α��� ����Ʈ �߰�

	int GetSubTypeWeapon ( int nSubType );

	CItemEffect(const CItemEffect &);
	CItemEffect &operator=(const CItemEffect &);

	void SetItemERType(EFF_RENDER_TYPE erType) { m_ItemERType = erType; }
	void SetItemERSubType(EFF_RENDER_SUB_TYPE erSubType) { m_ItemERSubType = erSubType; }
	ItemEffectGroup* GetPlusEffectGroup(int nType) { return &m_PlusEffectGourp[nType]; }
	ItemEffectGroup* GetItemEffectGroup(void) { return m_ItemEffectGroup; }
	ItemEffectGroup* GetNickEffectGroup(void) { return &m_NickEffectGroup; }
	ItemEffectGroup* GetPetStashEffectGroup(void) { return &m_PetStashEffectGroup; }
	ItemEffectGroup* GetRelicEffectGroup(void) { return &m_RelicEffectGroup; }
	CEffectGroup*	GetAccessoryEffect(void) { return m_pAccessroyEffect; }

protected:
	ItemEffectGroup m_PlusEffectGourp[2]; // 0 : ����, 1: ��
	ItemEffectGroup	m_ItemEffectGroup[WEAR_TOTAL];
	ItemEffectGroup	m_NickEffectGroup;	// ȣĪ
	ItemEffectGroup	m_PetStashEffectGroup;	// �� ���� ����Ʈ
	ItemEffectGroup	m_RelicEffectGroup;	// ���� ������ ����Ʈ
	CEffectGroup* m_pHolyWaterEffectGroup[2];
	
	char m_szAccessoryEffectName[64];
	CEffectGroup *m_pAccessroyEffect;
	
	int m_aItemPlus[WEAR_TOTAL];
	EFF_RENDER_TYPE	m_ItemERType;
	EFF_RENDER_SUB_TYPE m_ItemERSubType;
	int	m_nHolyWaterEffectType;
	int m_nCurRelicEffectType;
};

#endif //__CITEMEFFECT_H__