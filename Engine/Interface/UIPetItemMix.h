// ---------------------------------------------------------------------
// File : UIPetItemMix.h
// Desc : �� ������ ����
// ---------------------------------------------------------------------
#ifndef UIPETITEMMIX_H_
#define UIPETITEMMIX_H_
#ifdef	PRAGMA_ONCE 
	#pragma once
#endif

#include <Engine/Entities/Itemdata.h>
#include <Engine/Interface/UIProcess.h> // â�� ũ�Ⱚ�� UIProcess�� define���� ���

enum DRAGON_WEAPON_TYPE
{
	T_DRAGON_SWORD = 0,		// �巡�� �ҵ�
	T_DUAL_DRAGON_SWORD,	// ��� �巡�� �ҵ�
	T_DRAGON_BLADE,		// �巡�� ���̵�
	T_DRAGON_AXE,			// �巡�� �׽�
	T_DRAGON_WAND,		// �巡�� �ϵ�
	T_DRAGON_BOW,			// �巡�� ����
	T_DRAGON_STICK,		// �巡�� ��ƽ
	T_DRAGON_STAFF,		// �巡�� ������
	T_DRAGON_DAGGER,		// �巡�� ���
	T_DRAGON_ARMLETBOW,	// �巡�� �ϸ� ����
	T_DRAGON_SIDE,		// �巡�� ���̵�
	T_DRAGON_LOAD,		// �巡�� �ε�
	T_DRAGON_MAX,
};

enum KNIGHT_AMOR_TYPE
{
	T_HELMET = 0,		// ����
	T_COAT,			// ����
	T_PANTS,			// ����
	T_BOOTS,			// ����
	T_GLOVE,			// �尩
	T_SHIELD,
	T_KNIGHT_MAX,		
};

enum UNIQUE_INDEX				// ����ũ ������ �ε���
{
	UNI_DRAGON_SWORD = 887,		// �巡�� �ҵ�
	UNI_DUAL_DRAGON_SWORD = 907,	// ��� �巡�� �ҵ�
	UNI_DRAGON_BLADE = 908,		// �巡�� ���̵�
	UNI_DRAGON_AXE = 909,			// �巡�� �׽�
	UNI_DRAGON_WAND = 910,		// �巡�� �ϵ�
	UNI_DRAGON_BOW = 911,			// �巡�� ����
	UNI_DRAGON_STICK = 912,		// �巡�� ��ƽ
	UNI_DRAGON_STAFF = 913,		// �巡�� ������
	UNI_DRAGON_DAGGER = 914,		// �巡�� ���
	UNI_DRAGON_ARMLETBOW = 915,	// �巡�� �ϸ� ����
	UNI_DRAGON_SIDE = 916,		// �巡�� ���̵�
	UNI_DRAGON_LOAD = 917,		// �巡�� �ε�
	UNI_TITAN_HELMET = 918,		// Ÿ��ź 
	UNI_TITAN_COAT = 919,
	UNI_TITAN_PANTS = 920,
	UNI_TITAN_BOOTS = 921,
	UNI_TITAN_GLOVE = 922,
	UNI_KNIGHT_HELMET = 923,	// ���
	UNI_KNIGHT_COAT = 924,
	UNI_KNIGHT_PANTS = 925,
	UNI_KNIGHT_BOOTS = 926,
	UNI_KNIGHT_GLOVE = 927,
	UNI_KNIGHT_DEFENDSHIELD = 1280,
	UNI_MAGE_CIRCLET = 928,		// ������
	UNI_MAGE_COAT = 929,
	UNI_MAGE_PANTS = 930,
	UNI_MAGE_BOOTS = 931,
	UNI_MAGE_GLOVE = 932,
	UNI_LOG_HOOD = 933,			// �α�
	UNI_LOG_COAT = 934,
	UNI_LOG_PANTS = 935,
	UNI_LOG_BOOTS = 936,
	UNI_LOG_GLOVE = 937,
	UNI_HEALER_HEALCIRCLET = 938,		// ����
	UNI_HEALER_COAT = 939,
	UNI_HEALER_PANTS = 940,
	UNI_HEALER_BOOTS = 941,
	UNI_HEALER_GLOVE = 942,
	UNI_SORCERER_CIRCLET = 943,			// �Ҽ���
	UNI_SORCERER_COAT = 944,
	UNI_SORCERER_PANTS = 945,
	UNI_SORCERER_BOOTS = 946,
	UNI_SORCERER_GLOVE = 947,
};

struct  CNeedStuff {
	int		nNeedA[3];		// �ʿ���� 
	int		nNeedB[3];
};

// ��Ʈ��ũ �޼����� ���� ������ ����ü ( UIProces�� CNeedItems �̿� )

class CUIPetItemMix : public CUIWindow
{
protected:
	
// Controls...
	
	// Button
	CUIButton				m_btnClose;							// �ݱ� ��ư 
	CUIButton				m_btnOK[3];							// ���� ��ư( KNIGHT [1 -> A, 2 -> B] , 0 -> DRAGON ) 
	CUIButton				m_btnCancel;						// ��� ��ư 
	
	// Skill buttons
	std::vector< CUIIcon* > m_vecIcons;							// ���� ������


	// Etc ...
	CUIScrollBar			m_sbProcessItem;					// ���� ������ â ��ũ�� ��
	CUIListBox				m_lbPreconditionDesc;				// �ʿ� ���� ���� ����Ʈ �ڽ�
	
	
//	Process Item Info

//	int						m_nProcessText;						// ���� ���� ������ �ε���
	SBYTE					m_nRow;
	SBYTE					m_nCol;

	int						m_nSelectProcessItem;				// ���� ���õ� ������
	int						m_nProcessItemCount;				// ���������� �´� ����ǰ ����
	
	BOOL					m_bSatisfiedA;						// ������ ����Ѱ�?
	BOOL					m_bSatisfiedB;						// ������ ����Ѱ�?
	
	CNeedItems				m_NeedItems[MAX_MAKE_ITEM_MATERIAL];// �ʿ� ������ ����

	int						m_nNeedItemCount;					// �ʿ��� ������ ������ ��
	int						m_nJobIndex[TOTAL_JOB];				// ���� �� ���� ����ũ ������ �ε��� ���� ��

	CNeedStuff				m_NStuffKnight[T_KNIGHT_MAX];		// ����Ʈ �޾� �ʿ� ��� ����

// Region of each part
	UIRect					m_rcTitle;							// Region of title bar
	UIRect					m_rcIcons;							// Region of icons
	UIRect					m_rcTab;							// Region of tab
	UIRect					m_rcItem;
	UIRect					m_rcDesc;
	
// UV of each part
	UIRectUV				m_rtBackgroundTop;					// UV of background UP
	UIRectUV				m_rtBackgroundBtm;					// UV of background DOWN
	UIRectUV				m_rtSelOutline;						// UV of outline of selected button

// Network ...
	BOOL					m_bWaitProcessResult;				// Wait Message
	
	int						m_nPetType;

	CTString				m_strItemInfo[T_DRAGON_MAX];		// Item information string
	COLOR					m_colItemInfo[T_DRAGON_MAX];		// Color of item information string
	BOOL					m_bDetailItemInfo;

	void	ClearProcessItems();

public:
	CUIPetItemMix();
	~CUIPetItemMix();

	
// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	
// Render
	void	Render();

// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	
// Open & close Process
	void	Clear ();
	void	InitPetItemMix( int nType );

	ENGINE_API	void	OpenPetItemMix( int nType );
	void				ClosePetItemMix();

// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

// Network message functions ( receive )
	void	PetItemMixRep( SBYTE sbResult );
	
// Send ...
	void	SendPetItemMixReq( int nbtn, int nType );

// etc ...
	void	SelectItem ( int nIndex = -1 );
	void	AddString ( CTString& strText, COLOR colText = 0xffffffff );
	void	InitNeedItem( void );
};

#endif