// yjpark |<--
#include "stdh.h"
#include <Engine/Graphics/Shader.h>
#include <Engine/Terrain/Terrain.hpp>

static const INDEX	ctVertexPrograms = 1;
static const INDEX	ctPixelPrograms = 1;
static const INDEX	iBasePP = 0;
static const INDEX	iBaseVP = 0;

SHADER_MAIN( TRShader_Shadow )
{
//������ ���� ����	//(Add Tagent-space Normal Map)(0.1)
	shaSetDefaultConstantRegisters();
//������ ���� ��	//(Add Tagent-space Normal Map)(0.1)
	shaRender();
}

//������ ���� ����	//(For Performance)(0.1)
SHADER_DESC( TRShader_Shadow, ShaderDesc *&pshDesc )
{
	static bool bInit = false;
	static ShaderDesc shDescMe;
	if(!bInit)
	{
		bInit = true;
		shDescMe.sd_ctVertexPrograms = ctVertexPrograms;
		shDescMe.sd_ctPixelPrograms = ctPixelPrograms;

		shDescMe.sd_ulStreamFlags.New( ctVertexPrograms );
		shDescMe.sd_ulStreamFlags[0] = SHA_POSITION_STREAM | SHA_NAKED_CODE;
	}
	pshDesc = &shDescMe;
//������ ���� ��	//(For Performance)(0.1)
}

SHADER_VCODE( TRShader_Shadow, CTString &strVPCode, INDEX iVertexProgram )
{
	ASSERT( iVertexProgram == iBaseVP );

	strVPCode = "vs.1.1                        \n"
		        "m4x4 oPos,   v0,   c0         \n"
				"mov  r0.xy,  v0.xz            \n"
				"mov  r0.zw,  c7.xy            \n"
#if TER_SHADER_OPT
				"dp4  oT0.x,  r0,   c21        \n"
				"dp4  oT0.y,  r0,   c22        \n"
				"dp4  oT1.x,  r0,   c24        \n"
				"dp4  oT1.y,  r0,   c25        \n"
#else
				"m4x3 r1,     r0,   c21        \n"
				"m4x3 r2,     r0,   c24        \n"
				"mov  oT0.xy, r1               \n"
				"mov  oT1.xy, r2               \n"
#endif
              ;
}

SHADER_PCODE( TRShader_Shadow, CTString &strPPCode, INDEX iPixelProgram, FOGTYPE eFogType )
{
	ASSERT( iPixelProgram == iBasePP );
	ASSERT( eFogType == FT_NONE );

//������ ���� ����	//(Add Sun Moon Entity and etc)(0.2)
	//c7 : ������ shadow map�� blend�ϱ� ���� ���
	//c6 : �� ���� �Ͻ����� ���ȭ�� ������ ������ ��� ������ ���� ���.
/*	strPPCode = "tex    t0                      \n"
		        "tex    t1                      \n"
				
				"lrp    r0,   c7,   t0,   t1    \n"
				"mul	r0,   r0,   c6          \n"//shader code�� ���⸸ ����, Ant
              ;*/
	strPPCode = "texld	r0,    t0                      \n"
		        "texld	r1,    t1                      \n"
				
				"lrp    r0,   c7,   r0,   r1    \n"
				"mul	r0,   r0,   c6          \n"//shader code�� ���⸸ ����, Ant
              ;
//������ ���� ��	//(Add Sun Moon Entity and etc)(0.2)
}
// yjpark     -->|