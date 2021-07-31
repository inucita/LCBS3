//�������� ���� �߰��� ������.
//������ ���� ����	//(Add Shadow Util)(0.1)

#include "StdH.h"
#include <Shaders/Common.h>

static const INDEX ctTextures = 0; 
static const INDEX ctUVMaps   = 0;
static const INDEX ctColors   = 1;
static const INDEX ctFloats   = 0;
static const INDEX ctFlags    = 0;
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
	//shaEnableAlphaTest();
}

static void VertexShaderProcess()
{
	shaSetVertexProgramConst_TransformMatrix();	//c0~c3
	shaSetVertexProgramConst_Multiply();		//c7
	shaSetVertexProgramConst_LightDir();		//c4
	shaSetVertexProgramConst_BoneMatrix();		//c21~
	shaSetVertexProgram(iShadowVP);
}

static void PixelShaderProcess(COLOR colShadowColor)
{
	//Shadow color setting
	GFXColor colShadow = colShadowColor;
	ShaderRegister srShadowColor(colShadow);
	shaSetPixelProgramConst( 0, &srShadowColor, 1);
//������ ���� ����		03.22
	GFXColor colPlane = 0xFFFFFF80;
	ShaderRegister srPlaneColor(colPlane);
	shaSetPixelProgramConst( 1, &srPlaneColor, 1);	// c1
//������ ���� ��		03.22
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

// �ؽ��� ���� ���� ĥ��.
SHADER_MAIN(NoShadow)
{
	const COLOR colShadowColor = shaGetColor(iShadowColor);
	const BOOL bHardwareShader = shaUseHWShaders();
//������ ���� ����		03.30	
	if(((shaGetModelColor()&CT_AMASK)>>CT_ASHIFT) >= 64)
	{
//������ ���� ��		03.30
		
		if(bHardwareShader)
		{
			SetRenderingState();
			VertexShaderProcess();
			//������ ���� ����
			PixelShaderProcess(colShadowColor);
			//������ ���� ��
			Render();
		}
		else	//USE VS(SOFT or HARD), NON-PS
		{
			//TODO : ������
		}
	}
}

//������ ���� ����	//(For Performance)(0.1)
SHADER_DESC(NoShadow, ShaderDesc *&pshDesc)
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

		shDescMe.sd_astrColorNames[0] = "Mat.Shadow";

		shDescMe.sd_strShaderInfo = "NoShadow shader";
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;
		shDescMe.sd_ulStreamFlags.New(ctVertexPrograms);
		shDescMe.sd_ulStreamFlags[0] = GFX_POSITION_STREAM;
	}
	pshDesc = &shDescMe;
//������ ���� ��	//(For Performance)(0.1)
}

SHADER_VCODE(NoShadow, CTString &strVPCode, INDEX iVertexProgram)
{
	//������ c0 ~ c20 ��밡��, c0 ~ c7�� �ܺο��� �Էµ�.
	//--------------- Input ---------------------//
	//--- r0     - vertex position            ---//
	//--- c0     - mProjection*mView          ---//
	//--- c7     - (0.0f, 1.0f, 2.0f, 0.5f)   ---//��������� �ܺο��� ����
	//--- c4.w   - 765.01f                    ---//
	//-------------- Output ---------------------//
	//--- oPos   - Position Output            ---//

	ASSERT(iVertexProgram == iShadowVP);
	if(iVertexProgram == iShadowVP)
	{
		strVPCode = "m4x4 oPos,   r0,       c0                  \n"
					;
	}
}

SHADER_PCODE(NoShadow, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType)
{
	//������ c0�� ����(PS�� ���� ī����� ȣȯ���� ����� ���)
	//--------------- Input ---------------------//
	//--- v0     - diffuse color              ---//
	//--- c0     - Shadow color              ---//
	//-------------- Output ---------------------//
	//--- r0     - Output Pixel COLOR & Alpha ---//

	ASSERT(iPixelProgram == iShadowPP);
	//�Ȱ� ������� ����.
	if(iPixelProgram == iShadowPP)
	{
		strPPCode = "mov        r0,      c0                        \n"	//paint shadow color
//������ ���� ����		03.22
					"mul		r0, r0,  c1							\n"
//������ ���� ��		03.22
					;
	}
	return;
}
//������ ���� ��	//(Add Shadow Util)(0.1)
