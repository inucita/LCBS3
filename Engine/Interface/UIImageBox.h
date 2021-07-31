#ifndef __UIIMAGEBOX_H__
#define __UIIMAGEBOX_H__

#include <Engine/Interface/UIDrawFigure.h>

class ENGINE_API CUIImageBox : public CUIWindow
{
public:
	enum eImageType
	{
		IT_GENERAL	= -1,
		IT_ITEM		= 0,
		IT_MONEY	= 1,
		IT_EXP		= 2,
		IT_SP		= 3,
		IT_SKILL	= 4,
		IT_SSKILL	= 5,
		IT_ACTION	= 6,
		IT_RVR_POINT,
		IT_CORPS,
		IT_AFFINITY, // ģȭ�� ����2 [2/6/2013 Ranma]
		IT_AFFINITY_MONSTER,
	};
	enum eCorpsType
	{
		CT_BOSS				= 0,		// �����
		CT_VICE_BOSS		= 1,		// ������
		CT_MEMBER			= 2,		// �Ϲ� ����
		CT_RUSH_CAPTAIN		= 3,		// ���ݴ���
		CT_SUPPORT_CAPTAIN	= 4,		// ��������
		CT_RECON_CAPTAIN	= 5,		// ��������
		CT_RUSH				= 6,		// ���ݴ��
		CT_SUPPORT			= 7,		// �������
		CT_RECON			= 8,		// �������
		CT_TOTAL			= 9,		// TOTAL
	};

	CUIImageBox();
	virtual ~CUIImageBox();

	virtual void Render();
	void PopupRender();

	void	SetUV( FLOAT fTx0, FLOAT fTy0, FLOAT fTx1, FLOAT fTy1, FLOAT fTexWidth, FLOAT fTexHeight )
	{
		m_rtUV.SetUV( fTx0, fTy0, fTx1, fTy1, fTexWidth, fTexHeight );
	}
	UIRectUV	GetUV();
	void	SetRenderRegion(int left, int top, int right, int bottom)
	{
		m_rtRenderRegion.SetRect(left, top, right, bottom);
	}
	UIRect	GetRenderRegion()	{ return m_rtRenderRegion;	}

	void SetImageByType(eImageType type, int index, int nSyndiType = 0);

	//IT_GENERAL is not effected. 
	void SetHighlight(BOOL enable, COLOR colHighlight);
	void SetImageForCorps(eImageType type, int index);
	WMSG_RESULT	MouseMessage( MSG *pMsg );

	void SetPopupInfo( CTString strInfo, BOOL bPopup, COLOR nColor = 0xFFFFFFFF, int nPopWidth = 200 );	
	void SetEnablePopup( BOOL bEnable )		{	m_bEnablePopup = bEnable;	}

	void InitCorpsImageBox();
	void InitPopupImageBox();

	void ResetImageBox();
	// ���� �ڸ��� [2/21/2013 Ranma]
	void StringSplit(std::vector<CTString>& vecOutput, CTString strInput, ULONG ulColumnWidth);
	int GetTextureID()	{ return m_nTextureID; }
	int GetBtnType()	{ return (int)m_eUBType; }
private:
	eImageType	m_eImageType;
	UIBtnExType m_eUBType;
	int			m_nTextureID;
	UIRectUV	m_rtUV;
	UIRect		m_rtRenderRegion;
	BOOL		m_bHighlight;
	COLOR		m_colHighlight;
	BOOL		m_bShowPopup;
	// Popup Info Data
	CUIDrawBox	   m_bxPopupInfo;
	CTextureData*  m_ptdPopup;				// Popup Texture
	CTString	   m_strInfo;				// Popup Info String
	BOOL	       m_bEnablePopup;			// Popup Enable
	int			   m_nPopWidth;
	// Corps Data
	CTextureData* m_ptdCorps;				// ��� ���� ��ũ Texture
	UIRectUV	m_rtCorpsMark[CT_TOTAL];	// ��� ���� ��ũ UV


	// String
 	std::vector<CTString>	m_vecString;			// Strings of list box
	COLOR					m_colText;
};

#endif //__UIIMAGEBOX_H__