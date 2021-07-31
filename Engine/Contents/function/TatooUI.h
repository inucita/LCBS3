#ifndef	TATOO_UI_H_
#define	TATOO_UI_H_

#include <Engine/Interface/UIWindow.h>

class CUIImage;
class CUIButton;
class CUIArray;

class CTatooUI : public CUIWindow
{
public:
	CTatooUI();
	void OpenTatoo();
	void Close();		// ����

	void initialize();

	WMSG_RESULT OnLButtonDown(UINT16 x, UINT16 y);
	WMSG_RESULT OnLButtonUp(UINT16 x, UINT16 y);

	// Send
	void SendItemUse();	// ������ ���� �� ������ ������
private:
	void CheckShowShape();			// ���� ������ ���󿡼� ������ ���� �� �� �ִ��� üũ	
		 
	int	 GetColorAndShapeIndex();	// ������ ���� ���� �ε����� ��´�.
	void MakeMessageBox();			// �޽��� �ڽ��� �����

	int _getSelectIndex(int nArrayType);
	void _setUI();
	enum
	{
		eBTN_CLOSE = 0,
		eBTN_OK,
		eBTN_CANCEL,
		eBTN_MAX
	};

	CUIButton* m_pBtn[eBTN_MAX];

	enum
	{
		eARRAY_COLOR = 0,	// ���� ����
		eARRAY_PATTERN,		// ���� ����
		eARRAY_MAX
	};

	CUIArray* m_pArray[eARRAY_MAX];

	enum
	{
		eIMG_HEART = 0,	// ��Ʈ����
		eIMG_CLOVER,	// Ŭ�ι�����
		eIMG_DIA,		// ���̾�����
		eIMG_SPADE,		// �����̵�����
		eIMG_SKULL,		// �ذ�����
		eIMG_COLORTEMP, // ����÷�������
		eIMG_MAX
	};

	CUIImage* m_pImgTemp[eIMG_MAX];
	
	// drag
	CUIImage* m_pMoveArea;
};

#endif	// TATOO_UI_H_

