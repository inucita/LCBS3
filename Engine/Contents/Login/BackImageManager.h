#ifndef _BACKGROUND_IMAGE_MANAGER_H_
#define _BACKGROUND_IMAGE_MANAGER_H_

#define DEF_CLIENT_SIZE_COUNT (12)

class CBackImageManager : public CSingletonBase<CBackImageManager>
{
public:
	CBackImageManager();
	~CBackImageManager();

	enum eTEX_TYPE
	{
		eTEX_NONE = -1,
		eTEX_LOGIN_BACK = 0,
	};

	enum eCLIENT_SIZE
	{
		eSIZE_800_600  = 0,
		eSIZE_1024_768,
		eSIZE_1152_864,
		eSIZE_1280_720, 
		eSIZE_1280_800, 
		eSIZE_1280_960, 
		eSIZE_1280_1024, 	
		eSIZE_1440_900,  
		eSIZE_1600_900,  
		eSIZE_1600_1200,   
		eSIZE_1680_1050,
		eSIZE_1920_1200,

		eSIZE_MAX
	};

	enum eSETTING_TYPE
	{
		eSET_CREATE_DIS = 0, // ĳ���� ���� �ػ󵵺� ���� �Ÿ�
		eSET_SELECT_DIS, // ĳ���� ���� �ػ󵵺� ���� �Ÿ�
		eSET_CREATE_ZOOM_DIS_MIN, // ĳ���� ���� �ػ󵵺� �ּ� �� �Ÿ�
		eSET_CREATE_ZOOM_DIS_MAX, // ĳ���� ���� �ػ󵵺� �ִ� �� �Ÿ�
		eSET_CREATE_ZOOM_HEIGHT_MIN, // ĳ���� ���� �ػ󵵺� �ּ� �� ����
		eSET_CREATE_ZOOM_HEIGHT_MAX, // ĳ���� ���� �ػ󵵺� �ִ� �� ����
		eSET_MAX
	};

	enum eRATIO
	{
		eRATIO_4_3 = 0,
		eRATIO_5_4,
		eRATIO_16_9,
		eRATIO_16_10,
	};

	struct stSize
	{
		int nWidth;	// ������ ũ��.
		int nHeight;// ������ ũ��.
		int	nOffsetX; // �ػ󵵺� offset
		int nOffsetY;
		float fDisCreate;
		float fDisSelect;
		float fDisZoom;
		float fDisZoomMax;
		float fHeightZoom;
		float fHeightZoomMax;

		stSize::stSize()
		{
			nWidth = 0;
			nHeight = 0;
			nOffsetX = 0;
			nOffsetY = 0;
			fDisCreate = 0.f;
			fDisSelect = 0.f;
			fDisZoom = 0.f;
			fDisZoomMax = 0.f;
			fHeightZoom = 0.f;
			fHeightZoomMax = 0.f;
		}
	};
	void InitData();
	void ResetData();

	void Render(CDrawPort* pdp);
	void SetRenderTex(eTEX_TYPE eType);
	eTEX_TYPE GetRenderTex()			{ return m_nCurrentType; }

	void GetArrangeOffset(int &x, int &y, int nResolusionIdx = -1);
	void SetArrangeOffset(int x, int y, int nResolusionIdx);

	float GetCharDistance(bool bSelectUI);
	float GetZoomDisValue(bool bMax);
	float GetZoomHeightValue(bool bMax);
	int	 GetResolutionIndex();


	eRATIO GetRatio(int nWidth, int nHeight);
	float  GetRatioCamera(float fFov, int nWidth, int nHeight);

	void InIRead();

	bool IsInit()	{ return m_vecTexData.size() > 0 ? true : false; }
private:
	typedef std::vector<CTextureData*>	vecTexData;
	typedef vecTexData::iterator		vecTexDataIter;

	vecTexData	m_vecTexData;
	CTextureData* TexDataAt(int idx);

	CTextureData* m_pRenderTex;

	FLOAT m_fTexU0;
	FLOAT m_fTexV0;
	FLOAT m_fTexU1;
	FLOAT m_fTexV1;

	int m_nWidth;
	int m_nHeight;

	eTEX_TYPE m_nCurrentType;

	stSize m_stResolution[eSIZE_MAX];
};

#endif // _BACKGROUND_IMAGE_MANAGER_H_