// ----------------------------------------------------------------------------
//  File : UIQuestBook.h
//  Desc : Created by Dongmin
// ----------------------------------------------------------------------------

#ifndef	UIQUESTBOOK_H_
#define	UIQUESTBOOK_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Define text position
#define	QUESTBOOK_TITLE_TEXT_OFFSETX		25 
#define	QUESTBOOK_TITLE_TEXT_OFFSETY		5

// Define size of message box
#define	QUESTBOOK_TOP_HEIGHT				26
#define	QUESTBOOK_DESC_HEIGHT				164
#define	QUESTBOOK_BOTTOM_HEIGHT				7
#define QUESTBOOK_WIDTH						311
#define	QUESTBOOK_HEIGHT					290

class CItems;

// ----------------------------------------------------------------------------
// Name : CUIQuestBook
// Desc :
// ----------------------------------------------------------------------------
class CUIQuestBook : public CUIWindow
{
public:
	struct sQuestInfo
	{
		sQuestInfo()
		{
			iQuestIndex		= -1;
			strQuestTitle	= CTString("");
			iQuestType		= -1;
			iQuestScale		= -1;
			dOldTime		= -1;			
			dRemainTime		= -1;
		}

		// [090708: selo] �� ������ �����ε�
		inline const bool operator== (const sQuestInfo& rhs)
		{
			return iQuestIndex == rhs.iQuestIndex;
		}		
		// [090709: selo] �������� ������ �����ε�
		inline const bool operator< (const sQuestInfo& rhs)
		{
			return iQuestIndex < rhs.iQuestIndex;
		}

		CTString	strQuestTitle;
		int			iQuestIndex;
		int			iQuestType;			// [090601: selo] ����Ʈ ���� Ÿ��(�Ϲ�, �ó�����, ����, ����Ʈ ������)
		int			iQuestScale;		// [090603: selo] ����Ʈ �Ը�(�Ϲ�, ��Ƽ, ������)
		DOUBLE		dOldTime;			// [090609: selo] �ð����� ����Ʈ �� ������ ���� �ð�		
		DOUBLE		dRemainTime;		// [090609: selo] �ð����� ����Ʈ ���� �ð�(��)
	};

	struct FindQuest
	{
		int iQuestIndex;
		FindQuest(sQuestInfo& info) : iQuestIndex(info.iQuestIndex) {};
		bool operator() (const sQuestInfo& info)
		{
			if(info.iQuestIndex == iQuestIndex)
			{
				return true;
			}
			else
				return false;
		}
	};	

	const int				m_nUIIndex;

	int						m_nSelectedQuestIndex;					// ���õ� ����Ʈ �ε���.
	CUIButton				m_btnClose;								// Close button
	CUIButton				m_btnOK;								// Ȯ��
	CUIButton				m_btnCancel;							// �ݱ�
	CUIButton				m_btnGiveUp;							// ����Ʈ ����
	CUIButton				m_btnAccept;							// ����
	CUIButton				m_btnReserve;							// ����	
	CUIButton				m_btnDeny;								// ����
	CUIButton				m_btnProceedPop;						// ���ι�ư
	CUIButton				m_btnCompletePop;						// ���ι�ư
	CUIButton				m_btnPrev;								// [090526: selo] Ȯ���� ��ȹ�� ���� NPC �޴� ����Ʈ�� ���ư��� ��ư
	CUIComboBox				m_cmbSort;								// [090601: selo] ����Ʈ ����Ʈ ������ ���� �޺� ��ư
	CUIListBoxEx			m_lbDescription;						// List box of description	
	CUIListBoxEx			m_lbQuestList;							// List box of Quest
	CTString				m_strTitle;								// Title of message box	
	BOOL					m_bTitleBarClick;						// If title bar is clicked or not

	// Region of each part
	UIRect					m_rcTitle;								// Region of title bar

	// UV of each part
	UIRectUV				m_rtTopL;								// UV of top background
	UIRectUV				m_rtTopM;								// UV of top background
	UIRectUV				m_rtTopR;								// UV of top background
	UIRectUV				m_rtMiddleScrollL;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleScrollM;						// UV of middle background with scroll bar
	UIRectUV				m_rtMiddleScrollR;						// UV of middle background with scroll bar	
	UIRectUV				m_rtMiddleGapL;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapM;							// UV of middle gap background
	UIRectUV				m_rtMiddleGapR;							// UV of middle gap background
	UIRectUV				m_rtBottomL;							// UV of bottom background
	UIRectUV				m_rtBottomM;							// UV of bottom background
	UIRectUV				m_rtBottomR;							// UV of bottom background
	UIRectUV				m_rtPopingExtended;
	UIRectUV				m_rtPopingClosed;

	std::vector<CUIButton>	m_vectorBtnDynamicPrize;				// ���� ����	
	std::vector<sQuestInfo>	m_vectorProceedQuestList;
	std::vector<sQuestInfo>	m_vectorCompleteQuestList;
	std::vector<sQuestInfo>	m_vectorCategoryQuestList[4];			// [090603: selo] ī�װ� ���� ����Ʈ�� ���� ����Ʈ
	std::vector<sQuestInfo> m_vectorRaidMessageList;				// [090708: selo] ���̵� ����Ʈ ����Ʈ

	std::list<int>			m_listSelectedQuestIndex;				// [090602: selo] ���õǾ� �ִ� ����Ʈ���� �ε��� ����
	
	CItems*					m_pQuestItem;
	INDEX					m_nTargetIndex;
	INDEX					m_nTargetVirIndex;
	SBYTE					m_nTargetUIType;
	FLOAT					m_fNpcX, m_fNpcZ;
	int						m_nSelectPrizeFirstIndex;
	int						m_nSelectPrizeCount;
	BOOL					m_bProceedListExtended;
	BOOL					m_bCompleteListExtended;
	BOOL					m_bCategoryListExtended[QCATEGORY_SUPPLEMENT];	// [090603: selo] ī�װ��� ����Ʈ���� ������ �ִ��� Ȯ��
	BOOL					m_bRaidMessageListExtended;				// [090708: selo] ���̵� ����Ʈ ����Ʈ�� ������ �ִ��� Ȯ��
	int						m_iNextQuestIndex;
		
	// 2009. 05. 27 ������
	// Ȯ���� ���� NPC �޴��� ���ư��� ���� NPC Index ����
	INDEX					m_nPrevNPCIndex;
	SBYTE				    m_nPrevNPCUIType;

	// 2009. 06. 01 ������
	// ����Ʈ �� ���� Ÿ�� (����Ʈ ����Ʈ ������ ����Ѵ�.)
	SBYTE					m_nSortType;

	// 2009. 06. 01 ������
	// ����Ʈ�� �����ϴ� ���� �޽��� ó���� �� �ϰ� �ϴ� �÷���
	BOOL					m_bLockQuestList;
	
	// [090818: selo]
	// ����Ʈ ���� ���� �ð�
	DOUBLE					m_dQuestNoticeStartTime;
	
	// [090820: selo]
	// ����Ʈ ���� �ι� �ȳ����� �ϱ� ���� 
	// �̹� �߻��� ����Ʈ ������ ������ �д�	
	CStaticArray<int>		m_aAlreadyShowQuestNotice;
	
	// [090907: selo]
	// ���̵� ����, �� ����
	DOUBLE					m_dRaidRemainTime;	// ���̵� ���� �ð�
	BOOL					m_bIsRaidNow;		// ���̵� �� ���� �Ǵ� �÷���
	DOUBLE					m_dRaidRemainTimeRecvTime;	// ���̵� ���� �ð��� ���� �ð�
	
	CTextureData	*m_ptdCommonBtnTexture;

protected:
	void	AcceptQuest();				// ����Ʈ ����
	void	ReserveQuest();				// ����Ʈ ����
	void	DenyQuest();				// ����Ʈ ����
	void	GiveUpQuest();				// ����Ʈ ����
	void	PrizeQuest();				// ����Ʈ ����
	// 2009. 05. 26 ������
	// Ȯ���� ���� NPC �޴� ����Ʈ�� ���ƿ���
	

public:
	CUIQuestBook( int nUIIndex );
	~CUIQuestBook();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );	
	bool	AddDescString( CTString &strMessage, const COLOR colDesc, BOOL bSelectable = FALSE, BOOL bPrizeDesc = FALSE );	// [090812: selo] ���� ���� ó�� �߰�
	void	InitQuestBook();
	void	CloseQuestBook();
	void	OpenQuestBook( int iQuestIndex = -1, CItems* pQuestItem = NULL );	
	void	ToggleVisible();

	// ����Ʈ ��Ͽ� �߰�
	BOOL	AddToQuestList( int iQuestIndex, BOOL bComplete );

	// ����Ʈ ��Ͽ��� ����
	BOOL	DelFromQuestList( int iQuestIndex, BOOL bComplete );

	// ����Ʈ ����� �����մϴ�.
	void	RefreshQuestList();

	// ����Ʈ ����� ��� ����ϴ�.
	void	ClearQuestList();

	// ����Ʈ ������ �����մϴ�.
	void	RefreshQuestContent(INDEX questIndex);
	
	INDEX	GetProceedQuestCount()		{ return m_vectorProceedQuestList.size();	}
	INDEX	GetCompleteQuestCount()		{ return m_vectorCompleteQuestList.size();	}
	INDEX	GetCurrentQuest()			{ return m_nSelectedQuestIndex;				}

	// 2009. 05. 27 ������
	// ����Ʈ Index ���� ��������
	INDEX	GetProceedQuestIndex(INDEX index);
	INDEX	GetCompleteQuestIndex(INDEX index);	

	// 2009. 05. 28 ������
	// NPC �޴��� ���ư��� ���� ���� ����
	void	SetPrevNPCIndex(INDEX index){ m_nPrevNPCIndex = index;					}
	INDEX	GetPrevNPCIndex()			{ return m_nPrevNPCIndex;					}
	void	SetPrevNPCUIType(SBYTE type){ m_nPrevNPCUIType = type;					}
	SBYTE	GetPrevNPCUIType()			{ return m_nPrevNPCUIType;					}


	//npc index ����
	void	SetTargetIndex(INDEX index)	{ m_nTargetIndex = index;					}
	INDEX	GetTargetIndex()			{ return m_nTargetIndex;					}

	//quest ���� ����
	void	SetNextQuestIndex(int index)	{ m_iNextQuestIndex = index;			}
	int		GetNextQuestIndex()				{ return m_iNextQuestIndex;				}

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	ResetSavePosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );
	void	MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	// add large message box. [8/2/2010 rumist]
	void	MsgBoxLCommand(int nCommandCode, int nResult );
	// add open tutorial window [8/2/2010 rumist]
	void	OpenTutorialWindow();
		
	void	OpenWindow( BOOL bHasQuestList );


	//notice ����
	void	RenderNotice();
	void	CreateNotice();
	void	SetNoticeInfo(const char *szString);
	
	ENGINE_API void	RequestQuest( int nNpcIndex, int nNpcVirIndex, SBYTE sbUIType, FLOAT fX, FLOAT fZ );
	void	SendQuestReq( int nNpcIndex );
	void	SendQuestCancel( int nQuestIndex );
	void	SendQuestStart( int nQuestIndex );
	void	SendQuestPrize( int nQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus );

	// [090810: selo] 
	// ������ ������� ... ���� �����Ͽ� �ش�
	static CTString MakeTitleString(CTString strTitle, int iSplitPos);

	// [090708: selo] 
	// ���̵� ����Ʈ ���� �Լ���
	void	AddRaidMessage(const int& iQuestIndex);
	void	RemoveRaidMessage(const int& iQuestIndex);
	void	RemoveRaidMessageAll(void);
	void	RefreshQuestContentByRaid(const int& iQuestIndex);
	bool	isRaidMessage(const int& iQuestIndex);

	// 2009. 05. 29 ������
	// Ȯ���� ����Ʈ Ȯ�� ���� �Լ��� �߰�		
	void	CreateQuestListNew( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	RefreshQuestListNew();
	void	RenderQuestListNew();
	WMSG_RESULT	MsgProcQuestListNew( MSG *pMsg );

	// 2009. 06. 02 ������
	// ��üȭ�鿡 ǥ���ϱ� ���� üũ�ص� ����Ʈ ���� �Լ�
	void	AddSelectedQuest(const int& iQuestIndex);
	void	RemoveSelectedQuest(const int& iQuestIndex);
	bool	IsSelectedQuest(const int& iQuestIndex);
	void	ClearSelectedQuestList();			// [090803 sora] ���õ� ����Ʈ ����Ʈ clear
	const std::list<int>& GetSelectedQuestList() { return m_listSelectedQuestIndex; }

	// 2009. 06. 03 ������
	// ����Ʈ ����Ʈ ��Ʈ
	void	SortQuestListByCondition();
	void	SortQuestListByType();
	
	// 2009. 06. 03 ������
	// ����Ʈ�� �Ϸ� ��Ȳ���� �Ǵ�
	bool	IsCompleteQuest(int nQuestIndex);
	
	// 2009 .06. 08 ������
	// Ÿ�Ӿ��� ����Ʈ �� ���� ���� ���� ���� ����Ʈ�� �ʽð��� ��ȯ�Ѵ�
	DOUBLE	GetTimeAttackRemainTime();

	// 2009. 06. 09 ������
	// Ÿ�Ӿ��� ����Ʈ�� ���� �ð��� �����Ѵ�.
	void	SetQuestRemainTime(int iQuestIndex, int iRemainTime);

	// [090818: selo]
	// ����Ʈ ������ ��� �� �帥 �ð��� �˷��ش�.
	DOUBLE	GetQuestNoticeElapsedTime();

	// [090907: selo]
	// ���̵� ���۰� �� ó��	
	void	RaidEnd(void);
	void	SetRaidRemainTime(INDEX iRemainTime);
	DOUBLE	GetRaidReminTime(void);
	BOOL	IsRaidNow(void);

	static void TalkWithNPC();

	// 2009. 05. 27 ������
	// �޽��� �ڽ��� ����Ʈ ����Ʈ�� �߰��ϱ�
	static void	AddQuestListToMessageBoxL(const int& iMessageBoxType);

	// 2009. 05. 27 ������
	// ����Ʈ�� �����Ͽ� ���� ����� �´� ����Ʈ ���� ���� �Ѵ�.
	static void SelectQuestFromMessageBox(const int& iClassifierIndex);
	// ����Ʈ type ���� ��������
	static SBYTE GetQuestType(const INDEX& iQuestIndex);

	// esc closing support [5/30/2011 rumist]
	BOOL	CloseWindowByEsc()						{ CloseQuestBook();	return TRUE;		}
};

#endif	// UIQUESTBOOK_H_

