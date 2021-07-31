#ifndef	UITATOO_H_
#define	UITATOO_H_
#ifdef	PRAGMA_ONCE
	#pragma once
#endif

// Include Files
// #include <Engine/Interface/UIWindow.h>
// #include <Engine/Interface/UIButton.h>
// #include <Engine/Interface/UIListBox.h>
// #include <Engine/Interface/UIEditBox.h>

// Define size of Tatoo
// �ؽ��� ���� �� (254, 609)
#define	TATOO_WIDTH						256
#define	TATOO_HEIGHT					352

#define TATOO_TOTAL_ROW					6
#define TATOO_TOTAL_COL					2

//------------------------------------------------------------------------------
// CUISingleBattle
// Explain:  ��� ���� 
//------------------------------------------------------------------------------
class ENGINE_API CUITatoo : public CUIWindow
{
public:
	struct sTatooButton
	{
		CUIButton	btnButton;
		COLOR		color;
		bool		exist;

		sTatooButton() : color(0xFFFFFFFF), exist(false)
		{
			
		}

		void Render()
		{
			btnButton.Render(color);
		}
	};

protected:
	// Region of each part
	UIRect				m_rcTitle;

	// UV
	UIRectUV			m_rtMain;
	
	// Buttons
	CUIButton			m_btnClose;							
	CUIButton			m_btnOK;							
	CUIButton			m_btnCancel;	
	
	CTextureData*		m_ptdCommonBtnTexture;
	CTextureData*		m_ptdNewInventoryTexture;
	
	sTatooButton		m_btnColor[TATOO_TOTAL_ROW][TATOO_TOTAL_COL];
	sTatooButton		m_btnShape[TATOO_TOTAL_ROW][TATOO_TOTAL_COL];

	// Properties
	int					m_iSelectedColorIndex;		// ���õ� ���� �ε���
	int					m_iSelectedShapeIndex;		// ���õ� ���� �ε���

public:

	CUITatoo();
	~CUITatoo();

	// Create
	void	Create( CUIWindow *pParentWnd, int nX, int nY, int nWidth, int nHeight );
	void	Clear();	//
	void	Close();	// ����

	// Adjust position
	void	ResetPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );
	void	AdjustPosition( PIX pixMinI, PIX pixMinJ, PIX pixMaxI, PIX pixMaxJ );

	void	SetFocus( BOOL bVisible );
	void	OpenTatoo();
	void	SendItemUse();				// ������ ���� �� ������ ������

	// Messages
	WMSG_RESULT	MouseMessage( MSG *pMsg );	

	// Render
	void	Render();					// Render	

	void	RenderColorButtons();
	void	RenderShapeButtons();

	void	RenderSelectedColorOutline();
	void	RenderSelectedShapeOutline();

private:
	void	AddColorButton(int iRow, int iCol, COLOR color);				// ���� ��ư �߰�
	void	AddShapeButton(int iRow, int iCol, int texLeft, int texTop);	// ��� ��ư �߰�

	void	CheckShowShape();			// ���� ������ ���󿡼� ������ ���� �� �� �ִ��� üũ	
	
	int		GetColorAndShapeIndex();	// ������ ���� ���� �ε����� ��´�.
	void	MakeMessageBox();			// �޽��� �ڽ��� �����
};

#endif	// UIGB_H_