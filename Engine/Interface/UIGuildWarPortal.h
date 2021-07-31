
// ----------------------------------------------------------------------------
//  File : UISelectResource.h
//  Desc : Created by dongmin
// ----------------------------------------------------------------------------

#ifndef	GUILDWARPORTAL_H_
#define	GUILDWARPORTAL_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIEditBox.h>
// #include <Engine/Interface/UIListBox.h>
// #include <map>
// #include <Engine/Effect/CEffectGroupManager.h>
// #include <Engine/Effect/CEffectGroup.h>

// Define text position
#define	GUILDWARPORTAL_TITLE_TEXT_OFFSETX		25
#define	GUILDWARPORTAL_TITLE_TEXT_OFFSETY		5

// Define size of skill learn
#define	GUILDWARPORTAL_WIDTH		171
#define	GUILDWARPORTAL_HEIGHT		214

// ----------------------------------------------------------------------------
// Name : CUIGuildWarPortal
// Desc :
// ----------------------------------------------------------------------------
class CUIGuildWarPortal : public CUIWindow
{
public:

protected:
	CUIButton				m_btnClose;						// Close button

	// Region of each part
	UIRect					m_rcTitle;						// Region of title bar

	// UV of each part
	UIRectUV				m_rtBackTop;					// UV of top background
	UIRectUV				m_rtBackMiddle1;				// UV of middle background
	UIRectUV				m_rtBackMiddle2;				// UV of middle background
	UIRectUV				m_rtBackBottom;					// UV of bottom background	

	int						m_iClientNPCIndex;
	
	std::vector<CUIButton>	m_vectorResourceList;
	int						m_bPrePortal;					// �̵����� ���� 
	int						m_nPortal;						// 

	LONG					m_lLeftTime;				// ���� ���� �ð� 
	TIME					m_tmLeftTime;				// ���� ���� �ð� 

	int m_nNpcType;
	std::map<SLONG, CEffectGroup *> m_mapEG;

	// WSS_DRATAN_SEIGEWARFARE 2007/08/07
	int						m_btNpcAvailable;			// ���ź ���� ��� ���� Ÿ��(bit flag)
	int						m_nSelPotalIdx;
		
public:
	CUIGuildWarPortal();
	~CUIGuildWarPortal();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );

	// Render
	void	Render();

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	PortalCancel(  BOOL bMessage = TRUE );	// ���� ���� ���¸� ����Ѵ�.
	void	PrePortal( int nPotal );
	void	ResetGuildWarPortal();
	
	// Open
	ENGINE_API void	OpenGuildWarPortal( int npcIndex );	
	
	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// WSS_DRATAN_SEIGEWARFARE 2007/08/07
	ENGINE_API void	OpenGuildWarPortalDratan();
	void SetDratanPortalFlag(int npcIndex );
	ENGINE_API inline void SetPortalIdx(int npcIdx) { m_nSelPotalIdx = npcIdx; }
};


#endif	// GUILDWARPORTAL_H_
