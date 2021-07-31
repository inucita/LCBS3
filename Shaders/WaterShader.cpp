#include "StdH.h"
//������ ���� ���� Water ����		04.13
#include <Shaders/Common.h>

static const INDEX ctTextures		= 3; 
//������ ���� ���� Water ����		04.21
//static const INDEX ctTextures		= 3; 
//������ ���� �� Water ����			04.21
static const INDEX ctUVMaps			= 1;
static const INDEX ctColors			= 1;
static const INDEX ctFloats			= 0;
static const INDEX ctFlags			= 0;
static const INDEX iBumpTexture		= 0;		// Bump Texture
//������ ���� ���� Water ����		04.21
//static const INDEX iBumpTexture2		= 1;	// Bump Texture2
static const INDEX iBaseTexture		= 1;		// Base Texture
static const INDEX iWaterTexture	= 2;		// Water Texture
static const INDEX iBaseUVMap		= 0;

static const INDEX ctVertexPrograms = 1;
static const INDEX ctPixelPrograms	= 1;

static const INDEX iWaterHolderVP		= 0;
static const INDEX iWaterHolderPP		= 0;

static void SetRenderingState()
{
	// Set depth write
	//shaDisableDepthWrite();
	//shaEnableDepthTest();
	// Set blend mode
	shaDisableBlend();
	// Set alpha test
	shaDisableAlphaTest();
//������ ���� ���� Water ����		04.21
	shaSetTextureWrapping( GFX_REPEAT, GFX_REPEAT, 0);			// ����.	
	shaSetTextureWrapping( GFX_CLAMP, GFX_CLAMP, 1);
//������ ���� �� Water ����			04.21
	
//������ ���� ���� Water ����		04.13
	//CTimerValue tvNow   = _pTimer->GetHighPrecisionTimer();
	//FLOAT r = 0.01f;

//������ ���� �� Water ����			04.13

//������ ���� ���� Water ����		04.26
	// ����.
	//_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT4|D3DTTFF_PROJECTED );
	_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_PROJECTED );
//������ ���� �� Water ����			04.26
}

static void InitShaderProcess()
{
	// NOTE : �ִϸ��̼� �ؽ��İ� ����ɼ� �ֵ��� �ؾ��Ѵ�.
	// NOTE : ������ �� �ؽ��ĸ� ���� ��츦 ����ؼ�...
	//����� Texture����
	shaSetTexture(iBumpTexture, 0);		// Bump Texture1
	//shaSetTexture(iBumpTexture2, 1);	// Bump Texture2
	shaSetTexture(iBaseTexture, 1);		// Reflection Texture
//������ ���� ���� ���� ������ �۾�	05.31
	//	shaSetTexture(iWaterTexture, 2);	// Water Texture
//������ ���� �� ���� ������ �۾�	05.31
	//����� UV��ǥ��
	shaSetUVMap(iBaseUVMap);
}

static void VertexShaderProcess(INDEX iVertexProgram)
{
	shaSetVertexProgram(iVertexProgram);

	// ������ ��ġ�� ī�޶��� ��ġ�� �ܺο��� �������ٰ�...	
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	shaSetVertexProgramConst_ProjectionMatrix();//c20~c23
	shaSetVertexProgramConst_Multiply();		//c7

//������ ���� ���� Water ����		04.20
	// ���������� ���� ����.
	shaSetEMBM();
//������ ���� �� Water ����			04.20
	shaSetVertexProgramConst_WaterInformation();	
}

static void PixelShaderProcess(INDEX iPixelProgram)
{
	shaSetPixelProgramConst_WaterColor();
	shaSetPixelProgram(iPixelProgram);
}

static void Render()
{	 
	//_pGfx->gl_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_BUMPENVMAP );
	// Render surface
	shaRender();
}

SHADER_MAIN(WaterHolder)
{
	BOOL bHardwareShader = shaUseHWShaders();
	if(bHardwareShader)
	{
		SetRenderingState();
		InitShaderProcess();
		
		// NOTE : �����κп� Fog�� Haze�� ����Ǿ� ������, ����� �����ϰ� ������ ����.
//������ ���� ���� ���� ������ �۾�	05.23
		shaEnableBlend();
//������ ���� �� ���� ������ �۾�	05.23
		shaEnableDepthWrite();
		shaBlendFunc(
			GFX_SRC_ALPHA,
			GFX_INV_SRC_ALPHA);			
		// if double sided flag is on
		//shaCullFace(GFX_NONE);	// no face culling
		VertexShaderProcess(iWaterHolderVP);
		PixelShaderProcess(iWaterHolderPP);
		Render();
		_pGfx->gl_pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, 0);		
		shaDisableBlend();
	}
	else	//USE VS(SOFT or HARD), NON-PS
	{
		// �Ʒ� �ڵ� Ȯ�� ���غ���.
//������ ���� ���� ���� ������ �۾�	05.31
		SetRenderingState();
		//����� Texture����				
		shaSetTexture(iWaterTexture, 0);	// Water Texture
		//����� UV��ǥ��
		shaSetUVMap(iBaseUVMap);		
//������ ���� �� ���� ������ �۾�	05.31
		Render();
	}
}

//������ ���� ����	//(For Performance)(0.1)
SHADER_DESC(WaterHolder, ShaderDesc *&pshDesc)
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
		shDescMe.sd_astrColorNames[0] = "Mat.Water";
		shDescMe.sd_strShaderInfo = "Water shader";

		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM | GFX_TEXCOORD0;
		shDescMe.PixelShaderVersion = D3DPS_VERSION(1,1); /// WaterHolder�� 1.4���� ����� �� ���� texbem������� ���� 1.1�� ���.
			// ���� �� �ؽ�ó�� ��� ����� �ٲٴ°� ��Ʊ� ������, 1.1���
			// ���� Water ������ ���� ������ �ʿ��� ������ ����.
	}
	pshDesc = &shDescMe;
//������ ���� ��	//(For Performance)(0.1)
}

SHADER_VCODE(WaterHolder, CTString &strVPCode, INDEX iVertexProgram)
{
	if(iVertexProgram == iWaterHolderVP)
	{		
		strVPCode =
			// ������ ��ȯ�ؼ� ����ó�� ��鸮���� �ϴ� �κ�.
/*
			"mul r0, c15, v5.x							\n"	// �ؽ��� ��ǥ���� �ֱ��� �������� �Է����� ���.
			"mad r0, c16, v5.y, r0						\n"	// �ؽ��� ��ǥ���� �ֱ��� �������� �Է����� ���.
			
			"mov r1, c17.x								\n"	// �ð�.
			"mad r0, r1, c14, r0						\n"	// ��ʵ� �ð��� ���ؼ� ���ļ��� ���� ������� �̵�.
			"add r0, r0, c13							\n"
			
			"frc r0.xy, r0								\n"	// �� ���е��� �Ҽ��ε��� ���Ѵ�.
			"frc r1.xy, r0.zwzw							\n"
			"mov r0.zw, r1.xyxy							\n"

			"mul r0, r0, c11.x							\n"	// ���� ����� ���Ѵ�.
			"sub r0, r0, c7.w							\n"	// 0.5�� ����.
			"mul r0, r0, c20.w							\n"	// �ؽ��� ��ǥ�鿡 2*PI�� ���Ѵ�.
															// �̷��� �ؽ��� ��ǥ���� ������ (-PI���� PI)�� �ȴ�.

			"mul r5, r0, r0								\n"	// ���ΰ� �ڻ����� ���Ϸ� �޼� ������ ����Ѵ�.
			"mul r1, r5, r0								\n"
			"mul r6, r1, r0								\n"
			"mul r2, r6, r0								\n"
			"mul r7, r2, r0								\n"
			"mul r3, r7, r0								\n"
			"mul r8, r3, r0								\n"

			"mad r4, r1, c8.y, r0						\n"		// Sine
			"mad r4, r2, c8.z, r4						\n"
			"mad r4, r3, c8.w, r4						\n"

			"mov r0, c7.y								\n"		// 1
			"mad r5, r5, c9.x, r0						\n"		// Cosine
			"mad r5, r6, c9.y, r5						\n"
			"mad r5, r7, c9.z, r5						\n"
			"mad r5, r8, c9.w, r5						\n"

			"dp4 r0, r4, c12							\n"		// �ĵ鿡 �İ� ������� ���Ѵ�.

			"mul r0.xyz, c7.xxyx, r0					\n"		// �� ���������� �� ũ�⿡ ������ ���Ѵ�.
			"add r0.xyz, r0, v0							\n"		// ��ġ�� ���Ѵ�.
			"mov r0.w, c7.y								\n"		// ������
			"m4x4 oPos,   r0,       c0                  \n"		// ���� ��ġ = ��ü ���� ��ġ * ����:��:���� ���
			*/
			
			//------------------------------------------------
			// �� ���� �������� �������� �帣�� �ϴ� �κ�.
			// c17 -> �ð�
			// c18 -> �ؽ��� �帣�� �ϴ� ����.
			// v5 -> �ؽ��� ��ǥ.
			// c0 -> ���� ��ȯ ���.
			// c21 -> View * Proj * Tex Matrix
			/*"mov r0, c17.x								\n"
			"mul r0, r0, c18.xyxy						\n"
			"frc r0.xy, r0								\n"
			"add r0, v5, r0								\n"
			"mov oT0, r0								\n"		// ������ ��ǥ 1
			*/
			"mov r0, v5									\n"
			"add r0, r0, c17.yz							\n"
			"mov oT0, r0								\n"		// ������ ��ǥ 1

/*
			"mov r0, c17.x								\n"
			"mul r0, r0, c18.zwzw						\n"
			"frc r0.xy, r0								\n"
			"add r0, v5, r0								\n"
			"mov oT1, r0.yxzw							\n"		// ������ ��ǥ 2
			*/
			
			"m4x4 oPos,   v0,       c0                  \n"		// ���� ��ġ = ��ü ���� ��ġ * ����:��:���� ���			
			
			// �Ʒ����� �������� 4���� ���� 2�������� �����ؾ� ��.
			// 
			"m4x4 oT1, v0, c21							\n"
								;
	}
}

SHADER_PCODE(WaterHolder, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{  // pixel shader 1.4��� ����
	if(iPixelProgram == iWaterHolderPP)
	{
		// c2 -> WaterColor
		strPPCode =	"tex				t0		\n"
					"texbem t1,			t0		\n"		// ���� ����.
					"mov	r0.rgb,			t1	\n"
					"+mov   r0.a,    c3.a		\n"
					//"mov	r0,			t1		\n"
					//"add r0, r0, c3				\n"
					// calculate ocean color
					//lrp r1, v0, c[ CP_OCEAN_COLOR_DARK ], c[ CP_OCEAN_COLOR_LIGHT ]

					// combine ocean and env color
					//add r0, r0, r1
								;
	}
}
//������ ���� �� Water ����		04.13