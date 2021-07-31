
#ifndef	UISPINBUTTON_H_
#define	UISPINBUTTON_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// #include <Engine/Interface/UIWindow.h>
// #include <vector>
// #include <Engine/Interface/UIButton.h>
//------------------------------------------------------------------------------
// CUISpinButton
// Explain:  
// Date : 2005-06-23,Author: Lee Ki-hwan
// History: 
//------------------------------------------------------------------------------
enum eBTN_STATE{ eBTN_UP,
				 eBTN_DOWN, 
				 eBTN_MAX	
				};
class ENGINE_API CUISpinButton : public CUIWindow
{
protected:
	std::vector<CTString>	m_vecData;			// ����Ǵ� ������ 
	BOOL					m_bDataBack;		// �������� ����� ��µǴ°�?
	int						m_nDataBackWidth;	// �������� ����� �ʺ�(���̴� UI�� ���̸� ����)
	UIRectUV3				m_rt3Data;			// ������ ����� UV

	CTString				m_strTitle;			// ����
	CUIButton				m_btnUp;			// ���� ��ư 
	CUIButton				m_btnDown;			// �Ʒ��� ��ư 
	
	int						m_nSelectIndex;		// ���� ���õ� Data Index ( AddData�ϸ� 0���� ���õ� )
	int						m_nStrWidth;		// Title �� ����
	BOOL					m_bVertical;		// SpinButton�� �������� ��������
	UIRect					m_rcWheel;
	

public:

	CUISpinButton();
	
	CUIBase* Clone();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight, CTString strTitle, int nDataBackWidth );
	void	CreateButtons( int nBtnWidth, int nBtnHeight, int nButtonGap = 0,  BOOL bVertical = TRUE );

	void	Render();

	// Message
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void		AddData( CTString strData );		// ��Ʈ�� ���·� ���������� ���ڰ� �ɼ��� �ִ�.
	int			GetDataToNumber( int nIndex = -1 );		// �����͸� �������� ��´�.
	CTString	GetDataToString( int nIndex = -1 );		// �����͸� ��Ʈ�� ���·� ��´�		
	inline int	GetSelectPos()	{return m_nSelectIndex;} // ������ ��ġ // WSS_DRATAN_SEIGEWARFARE 2007/08/06
	inline void SetSelectPos(int tPos)	{ m_nSelectIndex = tPos;} // ������ ��ġ // WSS_DRATAN_SEIGEWARFARE 2007/08/06
	void		MoveDataIndex( int nIndexDelta );
	void		ResetAllDatas()
	{
		m_vecData.clear();
	}
	// ���� UI�� PosX, PosY�� �������� nLeft, nTop�� ��ƾ���.
	void	SetWheelRect( int nLeft, int nTop, int nWidth, int nHeight )
	{
		m_rcWheel.SetRect( nLeft, nTop, nLeft + nWidth, nTop + nHeight );
	}

	// InLine : Set/Cppy UV
	void	SetDataBackUV3( FLOAT fLU0, FLOAT fLV0, FLOAT fLU1, FLOAT fLV1,
							FLOAT fMU0, FLOAT fMV0, FLOAT fMU1, FLOAT fMV1,
							FLOAT fRU0, FLOAT fRV0, FLOAT fRU1, FLOAT fRV1,
							FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_bDataBack = TRUE;
		m_rt3Data.SetUV3( fLU0, fLV0, fLU1, fLV1, fMU0, fMV0, fMU1, fMV1, fRU0, fRV0, fRU1, fRV1,
						fTexWidth, fTexHeight );
	}

	void	SetUpUV( UIBtnState bsState, FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnUp.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyUpUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnUp.CopyUV( bsSrcState, bsDstState );
	}
	void	SetDownUV( UIBtnState bsState, FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_btnDown.SetUV( bsState, fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	void	CopyDownUV( UIBtnState bsSrcState, UIBtnState bsDstState )
	{
		m_btnDown.CopyUV( bsSrcState, bsDstState );
	}
	
	void RenderRectUV3( int nUnitSize, int nX, int nY, int nWidth, int nHeight, UIRectUV3 rtRectUV3 );	

	void SetTitle( CTString strTitle );

public:
	WMSG_RESULT	OnMouseWheel(UINT16 x, UINT16 y, int wheel);

	void		setUVDataBack( UIRectUV uv );
	void		setDataBackWidth(int nWidth)	{ m_nDataBackWidth = nWidth;	}
	void		setSpinButton( int nW, int nH, int nGap )
	{ 
		m_nSpinBtnWidth = nW; m_nSpinBtnHeight = nH; m_nSpinBtnGap = nGap;
#ifdef UI_TOOL
		setBtnAutoInfo(m_bBtnAutoInfo);
#endif // UI_TOOL
	}

	int			getDataBackWidth()				{ return m_nDataBackWidth;		}
	const char* getTitleText()					{ return m_strTitle.str_String; }
	void		getSpinButton( int& nW, int& nH, int& nGap )
	{
		nW = m_nSpinBtnWidth; nH = m_nSpinBtnHeight; nGap = m_nSpinBtnGap;
	}
	int			getAdjust()						{ return m_nAdjust;				}

#ifdef UI_TOOL
	UIRectUV	getUVDataBack()					{ return m_uvDataBack;			}
	bool		getBtnAutoInfo()				{ return m_bBtnAutoInfo;		}
	UIRect		getWheelRect()					{ return m_rcWheel;				}
	// ������ ������� ��ư�� Ŭ���� �ɹ��� ������� �ֱ� ���� ���.
	void		SetSpinButton( int type, CUIButton* pButton );
	void		setBtnAutoInfo(bool bAuto);

private :
	UIRectUV	m_uvDataBack;
	bool		m_bBtnAutoInfo;
#endif // UI_TOOL

protected:
	void		initialize();
	void		OnRender(CDrawPort* pDraw);

private:
	CUIButton*	m_pSpinBtn[eBTN_MAX];
	int			m_nSpinBtnWidth;
	int			m_nSpinBtnHeight;
	int			m_nSpinBtnGap;
	int			m_nAdjust;
};

#endif	// UISPINBUTTON_H_

