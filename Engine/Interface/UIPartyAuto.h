// ----------------------------------------------------------------------------
//  File : CUIPartyAuto.h
//  Desc : Created by eons
// ----------------------------------------------------------------------------

#ifndef UIPARTY_AUTO_H_ 
#define UIPARTY_AUTO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UICheckButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIComboBox.h>
// #include <vector>

#define P_AUTO_WIDTH	598
#define P_AUTO_HEIGHT	423

//	Job Flags
#define JF_TITAN		0x00000001
#define JF_KNIGHT		0x00000002
#define JF_HEALER		0x00000004
#define JF_MAGE			0x00000008
#define JF_ROGUE		0x00000010
#define JF_SORCERER		0x00000020
#define JF_NIGHTSHADOW	0x00000040

#ifdef CHAR_EX_ROGUE
	#define JF_EX_ROGUE		0x00000080	// [2012/08/27 : Sora] EX�α� �߰�
		#ifdef CHAR_EX_MAGE
			#define JF_EX_MAGE 0x00000100	//	2013/01/08 jeil EX������ �߰� �÷��� �����ؼ� ����� �߰��� �����ʿ� �ӽð� 
			#define JF_ALLCLASS		0x000001FF	
		#else
			#define JF_ALLCLASS		0x000000FF
	#endif
#else
	#define JF_ALLCLASS		0x0000007F
#endif

enum eAutoPartySelection
{
	PARTY_REGIST,
	PARTY_REGIST_LEADER,
	INVITE_LIST,
	PARTY_LIST,
	PARTY_DEL,
};

struct PartyListData
{
	CTString	strSubject;		// ��Ƽ ���� �� ĳ����
	DWORD		dwJobflag;		// ����
	int			nIndex;			// ĳ�� �ĺ��� �� ���� �ĺ���
	int			nLeveldiff;		// ���� ���� on/off
	int			nZone;			// ��Ƽ ��ġ
	int			nPartyType;		// ��Ƽ ����
	int			nLimitLv;		// ���� ����( ��� ���� )
	int			nPartyNum;		// ��Ƽ ���� ���� �ο�
};

// ----------------------------------------------------------------------------
// Name : CUIPartyAuto
// Desc : ��Ƽ���� ��Ī �ý���
// ----------------------------------------------------------------------------

class CUIPartyAuto : public CUIWindow
{
protected:
	// Region of each part
	// Join set
	UIRect		m_rcJoinTitle;				// Region of title
	BOOL		m_bShowTextScroll;
	BOOL		m_bTextDir;

	TIME		mtmTextScrollTime;

	int			m_nSelectState;				// selection state
	int			m_nLevelLimit;				// level limit num
	int			m_nCurrentPageNum;			// Current Page number
	int			m_nSelectLine;				// List select line
	int			m_nCharIndex;				// Allow and Reject CharIndex

	int			m_FrontChPos;				// string start pos;

protected:
	void		AdjustUIPos();
	BOOL		Is2ByteChar( int nCharIndex, int nFirstCheck, CTString strText );
public:
	CUIPartyAuto();
	~CUIPartyAuto();

	// Create
	void		Create(CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void		Clear();

	// Adjust position
	void		ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void		AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void		OpenPartyMatching();										// system select mode
	void		OpenAutoParty( int nType );			// Oepn AutoParty Menu
	void		CloseAutoParty();

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	// Command functions
	void		MsgBoxCommand( int nCommandCode, BOOL bOK, CTString &strInput );
	void		MsgBoxLCommand( int nCommandCode, int nResult );

	// Error Msg
	void		MatchRegMemberRep( int nErrorcode );
	void		MatchRegPartyRep( int nErrorcode );

	// text scroll
	void		TextScroll( CTString strText );

	// select line
	int			ListSelectLine( int nY );

};

#endif