
// WSS_DRAWFIGURE 070417 ------------------------------------------->><<
// �ڽ�,��, �ٰ����� �׸� �� �ִ� Ŭ������ ��ġ ��Ų��.
// ����� �ڽ���...���� �ʿ信 ���� �߰��...

#ifndef	UIDRAWFIGURE_H_
#define	UIDRAWFIGURE_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

#include <Engine/Interface/UIWindow.h>

// ----------------------------------------------------------------------------
// Name : WRect
// Desc : Rect class <- UIRect�� struct ������ class ������ ������
// ----------------------------------------------------------------------------
class WRect
{
public:
	int		Left, Top, Right, Bottom;

	//Constructor
	WRect();
	WRect(int tLeft,int tTop,int tRight,int tBottom);

	WRect& operator=(const WRect ct);
	WRect& operator=(const UIRect ct);
	WRect& operator-=(int ct);
	WRect& operator+=(int ct);

	void	SetRect( int nLeft, int nTop, int nRight, int nBottom );
	void	CopyRect( const WRect &rcSrc );
	void	CopyRect( const UIRect &rcSrc );
	void	Scale( FLOAT fScale );
	void	Offset( int nX, int nY );

	UIRect	GetUIRect();
	
	inline	int		GetCenterX() { return ( Left + Right ) / 2; }
	inline	int		GetCenterY() { return ( Top + Bottom ) / 2; }
	inline	int		GetWidth() const { return Right - Left; }
	inline	int		GetHeight() const { return Bottom - Top; }

};


// ----------------------------------------------------------------------------
// Name : CUIDrawBox
// Desc : Draw Box 
// ----------------------------------------------------------------------------
class CUIDrawBox
{
public :
	// Texture 
	CTextureData	*m_pTexture;

	// Variables
	int			m_iThickX;					// UV ���ý� �ܰ� �β�
	int			m_iThickY;					// UV ���ý� �ܰ� �β�
	WRect		m_rcBoxUV;					// Box UV 
	WRect		m_rcBoxRT;					// Region of background
	
	// UV of each part
	UIRectUV	m_rtCandUL;					// UV of upper left background
	UIRectUV	m_rtCandUM;					// UV of upper middle background
	UIRectUV	m_rtCandUR;					// UV of upper right background
	UIRectUV	m_rtCandML;					// UV of middle left background
	UIRectUV	m_rtCandMM;					// UV of middle middle background
	UIRectUV	m_rtCandMR;					// UV of middle right background
	UIRectUV	m_rtCandLL;					// UV of lower left background
	UIRectUV	m_rtCandLM;					// UV of lower middle background
	UIRectUV	m_rtCandLR;					// UV of lower right background
	
	CUIDrawBox();
	~CUIDrawBox();

	// Set UV
	void	SetBoxUV(CTextureData* pTexture, int iThick,WRect rcBoxUV);
	void	SetBoxUV(CTextureData* pTexture, int iThickX,int iThickY,WRect rcBoxUV);
	// Set Position
	void	SetBoxPos(WRect tRectPos);
	void	SetBoxPos(UIRect tRectPos);
	// Copy UV
	void	CopyUV(CUIDrawBox* pDrawBox);
	// Render
	void	Render(int nX=0,int nY=0,const COLOR nCol =0xFFFFFFFF,FLOAT fDepth =0);

};


#endif	
