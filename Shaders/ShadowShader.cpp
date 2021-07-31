//�������� ���� �߰��� ������.
//������ ���� ����	//(Add Shadow Util)(0.1)

#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 1; 
static const INDEX ctUVMaps   = 1;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 0;
static const INDEX iTransTexture	= 0;	// Base Texture
static const INDEX iBaseUVMap		= 0;
static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms = 1;

static const INDEX iShadowColor	= 0;
static const INDEX iShadowVP	= 0;
static const INDEX iShadowPP	= 0;

static void SetRenderingState()
{
	// Set depth write
	shaDisableDepthWrite();
	// Set blend mode
	shaDisableBlend();
	// Set alpha test
	shaEnableAlphaTest();
}

static void VertexShaderProcess()
{
	shaSetVertexProgram(iShadowVP);
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	shaSetVertexProgramConst_Multiply();		//c7
	shaSetVertexProgramConst_LightDir();		//c4
	shaSetVertexProgramConst_BoneMatrix();		//c21~
}

static void PixelShaderProcess(COLOR colShadowColor)
{
	//����� Texture����
	shaSetTexture(iTransTexture, 0);
	//����� UV��ǥ��
	shaSetUVMap(iBaseUVMap);
	//Shadow color setting
	GFXColor colShadow = colShadowColor;
	ShaderRegister srShadowColor(colShadow);
	shaSetPixelProgramConst( 0, &srShadowColor, 1);	// c0
//������ ���� ����		03.22
	/*
	GFXColor colPlane = 0xFFFFFF80;
	ShaderRegister srPlaneColor(colPlane);
	shaSetPixelProgramConst( 1, &srPlaneColor, 1);	// c1
	*/
//������ ���� ��		03.22

//������ ���� ����		03.27
	GFXColor colPlane = 0xFFFFFF00;
	ShaderRegister srPlaneColor(colPlane);
	shaSetPixelProgramConst( 1, &srPlaneColor, 1);	// c1

	GFXColor colSub = 0x0a0a0a75;
	ShaderRegister srSubColor(colSub);
	shaSetPixelProgramConst( 2, &srSubColor, 1);	// c2
//������ ���� ��		03.27
	//set pixel shader
	shaSetPixelProgram(iShadowPP);
}

static void Render()
{
	// if double sided flag is on
	shaCullFace(GFX_NONE);	// no face culling
	// Render surface
	shaRender();
}

//Texture�� �׷����� �׸��ڸ� ���� ���Ǵ� Shader��.
SHADER_MAIN(Shadow)
{
//������ ���� ����
	const COLOR colShadowColor = shaGetColor(iShadowColor); // ����		
//������ ���� ��
//������ ���� ����		03.30	
	if(((shaGetModelColor()&CT_AMASK)>>CT_ASHIFT) >= 64)
	{
//������ ���� ��		03.30
		const BOOL bHardwareShader = shaUseHWShaders();
		if(bHardwareShader)
		{
			SetRenderingState();
			VertexShaderProcess();
			PixelShaderProcess(colShadowColor);
			Render();
		}
		else	//USE VS(SOFT or HARD), NON-PS
		{
//������ ���� ���� ���� ������ �۾�	06.01
			COLOR crModel = 0x00000000f;
			shaSetModelColor(crModel);
			shaSetLightColor(crModel, crModel);
			SetRenderingState();

//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.27
			DWORD dwAlphaArg, dwColorArg;
			DWORD dwAlphaOP, dwColorOp;
			HRESULT hr;

			// Get Stage State
			hr = _pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAARG1, &dwAlphaArg);
			hr = _pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_ALPHAOP,	&dwAlphaOP);
			hr = _pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLORARG1, &dwColorArg);
			hr = _pGfx->gl_pd3dDevice->GetTextureStageState(0, D3DTSS_COLOROP,	&dwColorOp);

			hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(128, 15, 15, 15) );
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR );
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.27

			Render();

//������ ���� ���� �׽�Ʈ Ŭ���̾�Ʈ �۾�	06.27
			// Restore Stage State
			hr = _pGfx->gl_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, dwAlphaArg);
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,	dwAlphaOP);
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, dwColorArg);
			hr = _pGfx->gl_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP,	dwColorOp);
//������ ���� �� �׽�Ʈ Ŭ���̾�Ʈ �۾�		06.27
//������ ���� �� ���� ������ �۾�	06.01
		}
	}
}

//������ ���� ����	//(For Performance)(0.1)
SHADER_DESC(Shadow, ShaderDesc *&pshDesc)
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_astrTextureNames.New(ctTextures);
		shDescMe.sd_astrTexCoordNames.New(ctUVMaps);
		shDescMe.sd_astrColorNames.New(ctColors);
		shDescMe.sd_astrFloatNames.New(ctFloats);
		shDescMe.sd_astrFlagNames.New(ctFlags);

		shDescMe.sd_astrTextureNames[0] = "Trans. map";
		shDescMe.sd_astrTexCoordNames[0] = "Base uvmap";
		shDescMe.sd_astrColorNames[0] = "Mat.Shadow";
		shDescMe.sd_strShaderInfo = "Shadow shader";

		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM | GFX_TEXCOORD0;
	}
	pshDesc = &shDescMe;
//������ ���� ��	//(For Performance)(0.1)
}

SHADER_VCODE(Shadow, CTString &strVPCode, INDEX iVertexProgram)
{
	//������ c0 ~ c20 ��밡��, c0 ~ c7�� �ܺο��� �Էµ�.
	//--------------- Input ---------------------//
	//--- v5     - tex coord                  ---//
	//--- r0     - vertex position            ---//
	//--- c0     - mProjection*mView          ---//
	//--- c7     - (0.0f, 1.0f, 2.0f, 0.5f)   ---//��������� �ܺο��� ����
	//--- c4.w   - 765.01f                    ---//
	//-------------- Output ---------------------//
	//--- oPos   - Position Output            ---//
	//--- oT0    - Tex Coord Output           ---//

	ASSERT(iVertexProgram == iShadowVP);
	if(iVertexProgram == iShadowVP)
	{
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
					"mov  oT0.xy, v5.xy                         \n"
					;
	}
}

SHADER_PCODE(Shadow, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	//������ c0�� ����(PS�� ���� ī����� ȣȯ���� ����� ���)
	//--------------- Input ---------------------//
	//--- v0     - diffuse color              ---//
	//--- t0     - trans. tex use only alpha  ---//
	//--- c0     - Shadow color               ---//
	//-------------- Output ---------------------//
	//--- r0     - Output Pixel COLOR & ALPHA ---//

	ASSERT(iPixelProgram == iShadowPP);
	//�Ȱ� ������� ����.
	if(iPixelProgram == iShadowPP)
	{
/*		strPPCode = "tex		t0									\n" 
					"mov        r0.rgb,  c0                         \n"	//paint shadow color
					"+mov       r0.a,    t0.a						\n"	//alpha output
//������ ���� ����		03.22
//					"mul		r0, r0,  c1							\n"
//������ ���� ��		03.22
//������ ���� ����		03.27
					"add		r0.a,	r0.a, c2.a					\n"
					// r0.a�� 0.5���� Ŭ ���� ������ c0.a, c1.a�� ������.
					"cnd		r0.a, r0.a, c0.a, c1.a				\n"
//������ ���� ��		03.27
					;*/
		strPPCode = "texld		r0,		t0							\n" 
					"mov        r0.rgb,  c0                         \n"	//paint shadow color
//					"+mov       r0.a,    t0.a						\n"	//alpha output
//������ ���� ����		03.22
//					"mul		r0, r0,  c1							\n"
//������ ���� ��		03.22
//������ ���� ����		03.27
					"add		r0.a,	r0.a, c2.a					\n"
					// r0.a�� 0.5���� Ŭ ���� ������ c0.a, c1.a�� ������.
					"cnd		r0.a, r0.a, c0.a, c1.a				\n"
//������ ���� ��		03.27
					;
	}
	return;
}
//������ ���� ��	//(Add Shadow Util)(0.1)
