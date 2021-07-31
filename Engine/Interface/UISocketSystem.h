// ***************************************************************
//  UISocketSystem   version:  1.0   ? date: 04/12/2010
//  -------------------------------------------------------------
//  Author	: rumist
//  Desc	: ui windows class about socket system.
//  -------------------------------------------------------------
//  Copyright (C) 2010 - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************


#ifndef UISOCKETSYSTEM_H__
#define UISOCKETSYSTEM_H__
#ifdef PRAGMA_ONCE
	#pragma once
#endif

#define JEWEL_MAX_COUNT 7

class CUISocketSystem : public CUIWindow
{
public:
	struct stPACKET_ITEM
	{
		SWORD Item_Tab;
		SWORD InvenIdx;
		LONG ItemVirIdx;
		int SlotIdx;

		stPACKET_ITEM()
		{
			Item_Tab = -1;
			InvenIdx = -1;
			ItemVirIdx = -1;
			SlotIdx = -1;
		}
	};

	enum _SOCKET_STAT
	{
		STAT_CREATE = 0,			// punched item.
		STAT_COMBINE,				// item combines with jewels.
		STAT_JEWEL_COMPOS,
		STAT_CHAOSJEWEL_COMPOS,
		STAT_EMPTY,					// empty socket slot. destroy jewels.
		STAT_ONCEEMPTY,
		STAT_END,
	};

	enum _SOCKET_ERROR_TYPE
	{
		SOCKET_ERROR_INEXACT = 0,	// button type is miss matched in socket slot.
		SOCKET_ERROR_EMPTY,			// equip button is empty.
		SOCKET_ERROR_EXCEED,		// slot have exceeded about socket count.
		SOCKET_ERROR_ITEM_LACK,		// lacking item count.
		SOCKET_ERROR_JEWEL_OVERLAP,	// check jewel type about equipment.
		SOCKET_ERROR_COMBINE_TERMS, // ���� ���ǿ� ���� ���� ���� �Դϴ�. 
		SOCKET_ERROR_NOT_ENOUGH_INVEN_SLOT,	// ���� ���Ž�, ���ŵ� ������ ���� �κ��丮 ������ ������� ���� ���.
	};
	enum _JEWEL_ERROR_TYPE
	{
		JEWEL_COMPOS_ERROR_NOT_GENERAL_JEWEL,	// �Ϲ� ������ ��� �ϼ���.
		JEWEL_COMPOS_ERROR_NOT_CHAOS_JEWEL,		// ī���� ������ ��� �ϼ���.
		JEWEL_COMPOS_ERROR_NOT_COMPOS_SCROLL,	// ����� �� ���� ������ �Դϴ�.
		JEWEL_COMPOS_ERROR_NOT_INVENTORY,		// �ռ��ϴ� ������ �����ϴ�.
		JEWEL_COMPOS_ERROR_JEWEL_LACK,			// ������ �����մϴ�.
		JEWEL_COMPOS_ERROR_NAS_LACK,			// ������ �����մϴ�.
		JEWEL_COMPOS_ERROR_COMPOS_SCROLL,		// ���� �ռ���ũ���� �߸� �Ǿ����ϴ�.
		JEWEL_COMPOS_ERROR_NOT_INVENTORY_EMPTY, // �κ��丮 ������ �����մϴ�.
	};
protected:
	// ����
	_SOCKET_STAT				m_euSocketStat;		
	int							m_nNPCIndex;
	int							m_nNPCVIdx;
	FLOAT						m_fNPCPosX;
	FLOAT						m_fNPCPosZ;

	void						_toggleVisible( const _SOCKET_STAT _status = STAT_CREATE );
	void						_close();

	// ���� ���� ����
	CUIIcon*					m_pIconEmpty;	
	SBYTE						m_nEmptyJewelPos;			

public:
	CUISocketSystem();
	~CUISocketSystem();

	//////////////////////////////////////////////////////////////////////////
	// Overriding functions.
	// create
	void						Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight )	;
	void						MsgBoxLCommand(int nCommandCode, int nResult );
	
	void						ToggleVisible( const _SOCKET_STAT _status = STAT_CREATE );
	const BOOL					IsOpenedUI()	const;
	void						CloseAllUI();
	void						absCloseAllUI();

	const FLOAT					GetNPCPosX()	const									{ return m_fNPCPosX;			}
	const FLOAT					GetNPCPosZ()	const									{ return m_fNPCPosZ;			}
	void						GetNPCPos( FLOAT &fNPCPosX, FLOAT &fNPCPosZ )	const	{ fNPCPosX = m_fNPCPosX; fNPCPosZ = m_fNPCPosZ; }
	
	//////////////////////////////////////////////////////////////////////////
	// User creation functions.
	ENGINE_API void				OpenSocketSystem( const int iMobIndex, const BOOL bHasQuest, const FLOAT fX, const FLOAT fZ );
	ENGINE_API void				CloseSocketSystem()										{ _close();						}
		
	//////////////////////////////////////////////////////////////////////////
	// user network message functions.
	void						SendCreateReq(SWORD item_Tab1, SWORD invenIdx1, LONG itemVirIdx1, SWORD item_Tab2, SWORD invenIdx2, LONG itemVirIdx2);
	void						SendCombineReq(stPACKET_ITEM Item, stPACKET_ITEM Juwel, stPACKET_ITEM Scroll);
	void						SendUpgradeReq(bool bChaosJewel, stPACKET_ITEM Jewel1, stPACKET_ITEM Jewel2, stPACKET_ITEM Jewel3, stPACKET_ITEM Scroll, int nScrollCnt);
	void						SendCleanAllJewelReq(SWORD item_Tab, SWORD invenIdx, LONG itemVirIdx);
	void						SendCleanOneJewelReq(SWORD tab, SWORD inven_idx, SLONG index);

	void						_showErrorSystemMessage( const _JEWEL_ERROR_TYPE type); // �����޽��� ���
	void						showErrorMessage( const _SOCKET_ERROR_TYPE type = SOCKET_ERROR_INEXACT );
	int							IsEmptyInvenSlotCount();	// �κ��丮 ���� �� ������ ī��Ʈ.
	
	// ���� ��Ĺ ���� Send Data Set
	void						SetRemoveSingleItem(CUIIcon* pItem);
	void						SetRemoveSingleJewelPos(int nJewelPos);
};


#endif
