#include "stdafx.h"
#include "LCDDrawing.h"
#include <Engine/Interface/UITextureManager.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/GameStageManager/StageMgr.h>
#ifdef KALYDO
#include <Kalydo/KRFReadLib/Include/KRFReadLib.h>
#endif

// Define
#define	ROUND					0.005f
#define FIRST_INTO_WORLD		MAX_ZONE_COUNT - 1

//Text Pos
#define PIX_TEXT				18
#define PIY_TEXT				18

// static Variable
static CTextureObject	g_CTexCurLoading[2];
static CTextureObject	ptoProgressBar;
static CTextureObject	ptoLoadingBar;
static CTextureObject	toText;
static CTextureObject	toClassification;	//���ӵ��ǥ��
static CTextureData*	ptdProgressBar = NULL;
static CTextureData*	ptdLoadingBar = NULL;
static UIRectUV			rtProgressBar[3];
static UIRectUV			rtLoadingBar[3];
static UIRectUV			rtClassification;

static int nLoadingStep = 0;
float g_fCurLoadingStep = 0.f;

// ----------------------------------------------------------------------------
// Name : ObtainLoadingData()
// Desc : written by seo 40815
// ----------------------------------------------------------------------------
BOOL ObtainLoadingData( void )
{
	nLoadingStep = 0;

	try 
	{
		if (g_slZone >= 0)
		{
			CTString strLoadingTex1;
			CTString strLoadingTex2;
			float	 fStep;
			CZoneInfo* pZoneInfo = CZoneInfo::getSingleton();
			
			pZoneInfo->GetLoadingInfo(g_slZone, fStep, strLoadingTex1, strLoadingTex2);
			
			g_CTexCurLoading[0].SetData_t(strLoadingTex1);
			g_CTexCurLoading[1].SetData_t(strLoadingTex2);
			g_fCurLoadingStep = fStep;
		}
		else
		{
			g_CTexCurLoading[0].SetData_t(CTFILENAME( "Data\\Interface\\Loading\\Loading00_1.tex" ));
			g_CTexCurLoading[1].SetData_t(CTFILENAME( "Data\\Interface\\Loading\\Loading00_2.tex" ));
			g_fCurLoadingStep = 16.f;
		}
		
		toClassification.SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Classification_15.tex" ) );
		rtClassification.SetUV(0,0,64,64,toClassification.GetWidth(),toClassification.GetHeight());

		toText.SetData_t( CTFILENAME( "Data\\Interface\\Loading\\Text.tex" ) );

		// Load Progress Bar
		ptoProgressBar.SetData_t( CTFILENAME ( "Data\\Interface\\Loading\\ProgressBar.tex" ) );
		ptoLoadingBar.SetData_t( CTFILENAME ( "Data\\Interface\\Loading\\LoadingBar.tex" ) );

		ptdProgressBar = (CTextureData*)ptoProgressBar.GetData();
		ptdLoadingBar = (CTextureData*)ptoLoadingBar.GetData();

		FLOAT	fTexWidth	= ptdProgressBar->GetPixWidth();
		FLOAT	fTexHeight	= ptdProgressBar->GetPixHeight();

		// UV Coordinate of Progress Bar Icon
 		rtProgressBar[0].SetUV( 0, 0, 13, 17, fTexWidth, fTexHeight );
		rtProgressBar[1].SetUV( 13, 0, 36, 17, fTexWidth, fTexHeight );
		rtProgressBar[2].SetUV( 49, 0, 62, 17, fTexWidth, fTexHeight );

		fTexWidth	= ptdLoadingBar->GetPixWidth();
		fTexHeight	= ptdLoadingBar->GetPixHeight();

		// UV Coordinate of Loading Bar Icon
 		rtLoadingBar[0].SetUV( 0, 0, 4, 11, fTexWidth, fTexHeight );
		rtLoadingBar[1].SetUV( 4, 0, 7, 11, fTexWidth, fTexHeight );
		rtLoadingBar[2].SetUV( 7, 0, 11, 11, fTexWidth, fTexHeight );
				
	} 
	catch( char *strError ) 
	{
		CPrintF("%s\n", strError);
		return FALSE;
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Name : ReleaseLoadingData()
// Desc : 
// Last Edit : �̱�ȯ ( 12. 10 )
// ----------------------------------------------------------------------------
void ReleaseLoadingData( void )
{
	g_CTexCurLoading[0].SetData(NULL);
	g_CTexCurLoading[1].SetData(NULL);

	ptoProgressBar.SetData ( NULL );
	ptdProgressBar = NULL;
	ptoLoadingBar.SetData ( NULL );
	ptdLoadingBar = NULL;

	toText.SetData ( NULL );

	toClassification.SetData ( NULL );
	//toClassification = NULL;

}

// ----------------------------------------------------------------------------
// Name : RenderLoading()
// Desc : 
// Last Edit : �̱�ȯ ( 12. 10 )
// ----------------------------------------------------------------------------
void RenderLoading( CDrawPort *pdp, ULONG ulLevelMask, CProgressHookInfo *pphi)
{
	static float fMaxStep = 0.0f;

	// ���� ����
	if( pphi->phi_phsStatus == PHS_INIT )	
	{
		if( !ObtainLoadingData() )
		{
			ReleaseLoadingData();
			return;
		}
		
		fMaxStep = g_fCurLoadingStep;
	}
	// Render Back Image ==============================================================================
	
  	PIX pixdpw = pdp->GetWidth();
	PIX pixdph = pdp->GetHeight();

	FLOAT fPointStretch	= 0.0f;;	// ��� �� �Ʒ� �� �׸��� ��µ� ��ġ
	FLOAT fStretch		= 1.0f;	// ���δ� ���� ��� ���� ( 800/600, 1024/768 ... )
			
	PIX pixOriginalX	= 1024;		// ���� �̹����� ũ��
	PIX pixOriginalY	= 768;

	PIX ImageSizeY512	= 512;		// Ŀ�� �� �̹��� ������
	PIX ImageSizeY256	= 256;

	PIX pixSX			= 0;		// �̹��� ��� ��ġ
	PIX pixSY			= 0;

	float fPersent		= 1.0f;
	
	if( pixdpw == 1024 && pixdph == 768 )	// 1024 x 768 : �׳� �ѷ� ����
	{
 		pdp->PutTexture( &g_CTexCurLoading[0], PIXaabbox2D( PIX2D(pixSX,pixSY), PIX2D(pixOriginalX,ImageSizeY512)), C_WHITE|255 );
 		pdp->PutTexture( &g_CTexCurLoading[1], PIXaabbox2D( PIX2D(pixSX,ImageSizeY512), PIX2D(pixOriginalX,pixOriginalY)), C_WHITE|255 );
	}
	else if ( pixdph < 768 ) // 1024 x 768 ���� ���� �� ( 800 x 600 ) : ���� ũ�⸦ �������� ��� �ѵ� ��� ����
	{
		fStretch		= 1.3333f;
		
		fPersent = (float)pixdph / (float)pixOriginalY;
		
		// ���
		fPointStretch	= (FLOAT)ImageSizeY512 / (FLOAT)pixOriginalY;
		PIX	pixPointY	= ( pixdph * fPointStretch );

		// ������ ������ ���� ��ġ ( �Ҽ��� 3° �ڸ����� �ݿø� )
		pixOriginalX	= pixdph *  fStretch + ROUND;
		
		// ���� ��� ����
		pixSX			= ( pixdpw - pixOriginalX ) / 2;
		
 		pdp->PutTexture( &g_CTexCurLoading[0], PIXaabbox2D( PIX2D(pixSX,pixSY), PIX2D(pixdpw-pixSX,pixPointY)), C_WHITE|255);
 		pdp->PutTexture( &g_CTexCurLoading[1], PIXaabbox2D( PIX2D(pixSX,pixPointY), PIX2D(pixdpw-pixSX,pixdph)), C_WHITE|255);

	}
	else // 1024 x 768 ���� Ŭ ��	// ������ �����
	{
		// ��� ��ġ�� ����� ����
		pixSX = ( pixdpw - pixOriginalX ) / 2;
		pixSY = ( pixdph - pixOriginalY ) / 2;
		
 		pdp->PutTexture( &g_CTexCurLoading[0], PIXaabbox2D( PIX2D(pixSX, pixSY), PIX2D(pixSX+pixOriginalX,pixSY+ImageSizeY512)), C_WHITE|255);
 		pdp->PutTexture( &g_CTexCurLoading[1], PIXaabbox2D( PIX2D(pixSX, pixSY+ImageSizeY512), PIX2D(pixSX+pixOriginalX, pixSY+pixOriginalY)), C_WHITE|255);
	}

	// Render Text
	if ( g_slZone == -1 ) // ó�� ����� �� �� ȭ��
	{
		float fWidth = toText.GetWidth();	// * fPersent;	// ���ǥ�ô� ��� ����
		float fHeight = toText.GetHeight();	// * fPersent;		
			
		// ��� �̹����� ���� ���
		PIX	pixX = pixSX + PIX_TEXT;
		PIX	pixY = pixSY + PIY_TEXT;
				
		pdp->PutTexture( &toText, PIXaabbox2D( PIX2D(pixX, pixY), PIX2D(pixX+fWidth,pixY+fHeight)), C_WHITE|255);	
	}

	if ( g_slZone == -1 || g_slZone == 14 ) // �ӽ� 14:OX��,�������� �⺻ �ε��̹�����...
	{
		
		// Render Progress Bar ==============================================================================
		float m_pX				= pixSX + 49 * fPersent;		// �̹��� ��� ��ġ
		float m_pY				= pixSY + 736 * fPersent - ROUND;
		
		float fUniStepWidth		= 0.0f;							// �ܰ� �ϳ��� ���� �ϴ� �̹��� ����
		
		float fSideImageWidth	= 12.0f * fPersent;				// ���� �̹����� ũ��
		float fSideImageHeight	= 16.0f * fPersent; 
		
		float fProgressLength	= 902.0f / fMaxStep * fPersent;	// �Ѵܰ��� �ε��� �̵��ؾ� �ϴ� ����
		
		float m_eX = 0.0f;
		
		// Render Start ...
 		pdp->InitTextureData( ptdProgressBar );
 		
// 		// Render Start Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtProgressBar[0].U0, rtProgressBar[0].V0, rtProgressBar[0].U1, rtProgressBar[0].V1,
			0xFFFFFFFF );
		
		// Progress Start Position 
		m_pX += fSideImageWidth;
		
		if( ( pphi->phi_fCompleted >= 1 ) && ( pphi->phi_phsStatus == PHS_WORKING ) )
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				nLoadingStep++;
			}
		}
		else 
		{
			fUniStepWidth = fProgressLength * ( ( pphi->phi_phsStatus == PHS_WORKING)?pphi->phi_fCompleted:0 );
		}
		
		// Progress End Position
		m_eX  = m_pX + ( nLoadingStep * fProgressLength ) + fUniStepWidth;
		
		
		// Render Middle Image
		pdp->AddTexture(  m_pX, m_pY, m_eX, m_pY + fSideImageHeight,
			rtProgressBar[1].U0, rtProgressBar[1].V0, rtProgressBar[1].U1, rtProgressBar[1].V1,
			0xFFFFFFFF );
		m_pX = m_eX;
		
		// Render End Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtProgressBar[2].U0, rtProgressBar[2].V0, rtProgressBar[2].U1, rtProgressBar[2].V1,
			0xFFFFFFFF );
		
		// ... Render Start 
		pdp->FlushRenderingQueue();
		
		// Render Text ( Description ) =========================================================================
		float fFontSize = fPersent / 2 - 0.05;
#if defined G_RUSSIA
		pdp->SetFont( _pfdDefaultFont );
		pdp->SetTextScaling( fFontSize * 2.5 );
#else
		pdp->SetFont( _pfdDisplayFont );
		pdp->SetTextScaling( fFontSize );
#endif
		pdp->SetTextAspect( 1.0f );
		pdp->SetTextShadow( +2 );
		
		m_pX = pixSX + pixOriginalX / 2;
		pdp->PutTextC ( pphi->phi_strDescription, m_pX, m_pY, 0xD88842FF );
		
	}
	else
	{			
		// Render Progress Bar ==============================================================================
		float m_pX				= pixSX + 149 * fPersent;		// �̹��� ��� ��ġ
		float m_pY				= pixSY + 612 * fPersent - ROUND;
		
		float fUniStepWidth		= 0.0f;							// �ܰ� �ϳ��� ���� �ϴ� �̹��� ����
		
		float fSideImageWidth	= 11.0f * fPersent;				// ���� �̹����� ũ��
		float fSideImageHeight	= 11.0f * fPersent; 
		
		// [2012/01/19 : Sora] �ε� �� ���̰� 727�ε� 11(���� �̹���) + 716(�߰� �̹���) + 11(��)���� ����ؼ� ����� �־���(716 -> 704�� ����)
		float fLoadingLength	= 704.0f / fMaxStep * fPersent;	// �Ѵܰ��� �ε��� �̵��ؾ� �ϴ� ����
		
		float m_eX = 0.0f;
		
		// Render Start ...
		pdp->InitTextureData( ptdLoadingBar );
		
		// Render Start Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtLoadingBar[0].U0, rtLoadingBar[0].V0, rtLoadingBar[0].U1, rtLoadingBar[0].V1,
			0xFFFFFFFF );
		
		// Loading Start Position 
		m_pX += fSideImageWidth;
		
		if( ( pphi->phi_fCompleted >= 1 ) && ( pphi->phi_phsStatus == PHS_WORKING ) )
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				nLoadingStep++;
			}
		}
// [2012/01/10 : Sora] ���� ���� �ʿ� ������;; �ϴ� ���þƸ� �о� ����
#if !defined(G_RUSSIA)
		else 
		{
			if ( nLoadingStep < fMaxStep ) 
			{
				fUniStepWidth = fLoadingLength * ( ( pphi->phi_phsStatus == PHS_WORKING)?pphi->phi_fCompleted:0 );
			}
		}
#endif
		
		// Loading End Position
		m_eX  = m_pX + ( nLoadingStep * fLoadingLength ) + fUniStepWidth;
		
		// Render Middle Image
		pdp->AddTexture(  m_pX, m_pY, m_eX, m_pY + fSideImageHeight,
			rtLoadingBar[1].U0, rtLoadingBar[1].V0, rtLoadingBar[1].U1, rtLoadingBar[1].V1,
			0xFFFFFFFF );
		m_pX = m_eX;
		
		// Render End Image
		pdp->AddTexture( m_pX, m_pY, m_pX + fSideImageWidth, m_pY + fSideImageHeight,
			rtLoadingBar[2].U0, rtLoadingBar[2].V0, rtLoadingBar[2].U1, rtLoadingBar[2].V1,
			0xFFFFFFFF );
		
		// ... Render Start 
		pdp->FlushRenderingQueue();
		
		// Render Text ( Description ) =========================================================================
		float fFontSize = fPersent / 2 - 0.05;
#if defined G_RUSSIA
		pdp->SetFont( _pfdDefaultFont );
		pdp->SetTextScaling( fFontSize * 2.5 );
#else
		pdp->SetFont( _pfdDisplayFont );
		pdp->SetTextScaling( fFontSize );
#endif
		pdp->SetTextAspect( 1.0f );
		pdp->SetTextShadow( +2 );
		
		m_pX = pixSX + pixOriginalX / 2;
		pdp->PutTextC ( pphi->phi_strDescription, m_pX, m_pY, 0xD88842FF );
	}
	
	// free textures used in map rendering
	if( pphi->phi_phsStatus == PHS_END )
	{
		ReleaseLoadingData();

		// �ε��� �Ϸ�Ǹ�, Stage ��ȯ
		if (STAGEMGR()->GetCurStage() == eSTAGE_ZONELOADING)
		{
			_pNetwork->SendGameStart();
		}
	}
}

