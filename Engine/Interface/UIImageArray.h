#ifndef _UI_IMAGE_ARRAY_H_
#define _UI_IMAGE_ARRAY_H_

class ENGINE_API CUIImageArray : public CUIBase
{
public:
	CUIImageArray();
	~CUIImageArray();

	CUIBase* Clone();
	void initialize();
	
	//------------------------------------>>
	void AddImage(UIRect rc, UIRectUV uv, int idx = -1, std::string desc = "");	// xml�� desc�� ���� �����ֱ� ����.
	void DataAt(int idx, UIRect& rc, UIRectUV& uv, std::string& desc);	// desc�� �������� ���.
	void SetImageWidth(int idx, int nWidth);

	void SetRenderIdx(int idx)	{ m_nRenderIdx = idx; }
	int  GetRenderIdx()			{ return m_nRenderIdx; }

	void SetColor(COLOR col)	{ m_color = col;	}
	COLOR GetColor()			{ return m_color;	}
	void SetDepth(FLOAT fDepth) { m_fDepth = fDepth;}
	void SetDepthMode(BOOL bMode) { m_bDepthMode = bMode; }

	int  GetImageCount()			{ return m_vecData.size();	}

	bool RemoveImage(int idx);
	void Clear()	{ m_vecData.clear();	}
	//------------------------------------<<

	void OnRender(CDrawPort* pDraw);

private:
	struct stData
	{
		UIRectUV uv;
		UIRect   rc;
#ifdef UI_TOOL
		std::string desc;
		stData::stData() { desc = ""; }
#endif // UI_TOOL
	};

	typedef std::vector<stData>	vecData;
	typedef vecData::iterator	vecIter;

	vecData	m_vecData;

	int		m_nRenderIdx;
	COLOR	m_color;
	FLOAT	m_fDepth;
	BOOL	m_bDepthMode;
};

#endif // _UI_IMAGE_ARRAY_H_