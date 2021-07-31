
#ifndef		_INVENTORY_DATA_H_
#define		_INVENTORY_DATA_H_

#include <Engine/DesignClasses/Singleton/SingletonBase.h>
#include <vector>

class CItems;

class ENGINE_API CInvenData : public CSingletonBase< CInvenData >
{
public:
	CInvenData();
	virtual ~CInvenData();

	void	Update();

	void	ResetRenderCount()		{ m_nRenderTickCount = 0; }
	void	AddRenderTickCount()	{ m_nRenderTickCount++; }

	void	SetEffectSpeed(int nSpeed)		{ m_nEffectSpeed = nSpeed; }
	void	SetPlayCount(int nCount)		{ m_nEffectPlayCount = nCount; }
	void	SetShowEffectFlag(BOOL bShow)	{ m_bRander = bShow; }	
	int		GetEffectSpeed()		{ return m_nEffectSpeed; }
	int		GetPlayCount()			{ return m_nEffectPlayCount; }
	unsigned int GetRenderCount()	{ return m_nRenderTickCount; }
		
	BOOL	IsNewItemEffectRender() { return m_bRander; }

	void	InitItemData(CItems* pItems);
	CItems* GetItemInfo(int tab, int invenIndex);

private:
	int	m_nEffectSpeed;		// ������ ����Ʈ ���� �ӵ�
	int	m_nEffectPlayCount;	// ������ ����Ʈ ���� Ƚ�� (-1�� ��� �ݺ�)
	unsigned int m_nRenderTickCount; // ������ ����Ʈ�� ���� ���� TICK

	BOOL m_bRander;
	
	typedef std::vector<CItems*>	vecItemData;
	typedef vecItemData::iterator	vecItemDataIter;
	vecItemData	m_vecItemData;

	int		GetCalcIndex(int tab, int invenIndex);
};

#endif		// _INVENTORY_DATA_H_