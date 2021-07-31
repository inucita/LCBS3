// ----------------------------------------------------------------------------
//  File : UINickName.h
//  Desc : NickName interface
//  code : 
// ----------------------------------------------------------------------------

#ifndef	UINICKNAME_H_
#define	UINICKNAME_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>
#include <Engine/Interface/UIButton.h>
#include <Engine/Interface/UIListBox.h>
#include <Engine/Interface/UIEditBox.h>
#include <Engine/Interface/UIScrollBar.h>
#include <Engine/Interface/UIRectString.h>
#include <Engine/Contents/function/TitleData.h>

#define NICKNAME_WIDTH		251
#define NICKNAME_HEIGHT		285
#define NICK_NAME_MAX		50

class CUINickName : public CUIWindow  
{
protected:
	struct NickListData 
	{
		INDEX		NickAccessor;	// ȣĪ �ε���(ȣĪ ������ ������)
		LONG		lDate;			// ���� ��¥
		INDEX		NickItemIndex;	// ȣĪ ������ �ε���
		INDEX		CustomIndex;
		CUIIcon*	pIconNick;		// ȣĪ ��ư

		NickListData() : NickAccessor(0), lDate(0), NickItemIndex(0), CustomIndex(-1) {}

		NickListData(INDEX l, LONG r, INDEX i, INDEX ic) : NickAccessor(l), lDate(r), NickItemIndex(i), CustomIndex(ic){}
		INDEX GetNickAccessor(void) { return NickAccessor;	}
		INDEX GetCustomIndex(void)	{ return CustomIndex;	}
	};

	UIRectUV			m_rtBackGround;				// BackGround UV
	UIRectUV			m_rtSelectedBackGround;		// Select Box UV

	UIRect				m_rtCloseBtn;				// X Button Rect
	UIRect				m_rtSelectBtn;				// Select Button Rect
	UIRect				m_rtCancelBtn;				// Cancel Button Rect
	UIRect				m_rtDeleteBtn;				// Delete Button Rect
	UIRect				m_rcTitle;					// Title Rect
	UIRect				m_rtButtonAll;				// NickName Button Rect (5���� ȣĪ ��ư�� �׷����� ��ü ����)
	UIRect				m_rtTextAll;				// Text(NickName, EndDate) Rect (5���� ȣĪ �̸�, ����Ⱓ�� �׷����� ��ü ����)
	UIRect				m_rtBtn[5];					// NickName Button Rect (5���� ȣĪ ��ư ������ ����)
	UIRect				m_rtText[5];				// Text Rect (5���� ȣĪ �̸�, ����Ⱓ�� �׷����� ������ ����)
		
	CTextureData		*m_ptdButtonTexture;		// Texture
	
	CUIButton			m_btnClose;					// X Button 
	CUIButton			m_btnDelete;				// Delete Button
	CUIButton			m_btnCancel;				// Cancel Button
	CUIButton			m_btnSelect;				// Select Button
	
	CUIScrollBar		m_sbNickNameListBox;		// Scroll Bar

	BOOL				m_bTitleBarClick;			// Title Bar
	
	SLONG				m_sbNickNameListBoxNumber;	// ���õ� ����Ʈ �ڽ� �ε���
	INDEX				m_nNickNameListSize;		// ����Ʈ�� �� ����
	
	int					m_nScrollBarPos;			// Scroll Bar Position
	int					m_nCurInfoLines;			// ��Ƽ ����
	
	CTString			m_strItemInfo[2];							// Item information string
	COLOR				m_colItemInfo[2];							// Color of item information string
	
	std::vector<NickListData> m_NickList;			// NickName Data�� �����ϱ� ���� list

	
	
public:
	CUINickName();
	~CUINickName();
	void		Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	void		Clear();							// ȣĪâ �ʱ�ȭ
	
	void		AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void		ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void		CloseNickName();

	void		Render();				// Render
	void		RenderListButton();		// Button Render
	void		RenderText();			// Text (NickName, EndDate) Render
	void		RenderSelectedTexture();	// Select Box Render

	WMSG_RESULT MouseMessage( MSG *pMsg );	// Mouse Message

	void		MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );	// Message Box Command

	void		Open();							// Open

	CTString	CulEndDate( INDEX iIndex );		// EndDate

	void		AddNickNameList( INDEX iIndex, LONG lDate, int nCustomIndex = -1 );		// Add NickName
	void		DelNickNameList( INDEX iIndex );	// Delete NickName
	void		SortNickNameData();

	void		SetBtnPos();									// Set Button Position

	void		SetCancelButtonState( BOOL b) {	m_btnCancel.SetEnable(b); }	// Cancel Button State(select button, delete button, cancel button) State
	
	void		AddNickNameText( CTString &strItemInfo, COLOR colItemInfo ,int maxLine=2, int maxChars=20);	// ȣĪ ���� â �ؽ�Ʈ �߰�

	static bool SortCompare(const NickListData& pNickData1, const NickListData& pNickData2)
	{
		int nLIndex = TitleStaticData::getData(pNickData1.NickAccessor)->GetItemIndex();
		int nRIndex = TitleStaticData::getData(pNickData2.NickAccessor)->GetItemIndex();

		std::string strL = CItemData::getData(nLIndex)->GetName();
		std::string strR = CItemData::getData(nRIndex)->GetName();

		return strL < strR ? true : false; 
	}
	
	//  [3/25/2010 kiny8216] ȣĪ �̸��� ������ lod���� �ҷ������� ����
	CTString	GetName(INDEX iIndex);
	CTString	GetDesc(INDEX iIndex);
	// ��ŷ�ý��ۿ��� ȣĪ�� �޾Ƽ� ȣĪ ���� �ð��� ���� �� �� �ִ�.
	void		ChangeTitleTime ( INDEX iIndex, LONG iDate );
	int			GetTitleIndex(int nItemIndex);
};

#endif // !defined(AFX_UINICKNAME_H__379A2562_EA30_4320_8038_73C1035B9C78__INCLUDED_)
