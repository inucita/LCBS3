#include "StdH.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Interface/UIInternalClasses.h>

#include <Engine/Interface/UISkillToolTip.h>
#include <Engine/Help/ItemHelp.h>
#include <Engine/Interface/UISkillLearn.h>
#include <Engine/Contents/Base/UICharacterInfoNew.h>
#include <Engine/Interface/UIInventory.h>
#include <Engine/Info/MyInfo.h>

extern INDEX g_iCountry;

#define SKILL_LEARNED_COLOR				0xF2F2F2FF
#define NOT_SATISFACTION_COL			0xFF0000FF
#define SATISFACTION_COL				0xFFFF00FF

#define SKILL_NOT_EXIST					-1

#define SKILLINFO_LINE_MAX				20

static int	_iMaxMsgStringChar = 0;

CUISkillToolTip::CUISkillToolTip()
	: m_bShowInfo(false)
{

}

void CUISkillToolTip::initialize()
{
	m_pTexData = CreateTexture( CTString( "Data\\Interface\\CommonBtn.tex" ) );

	FLOAT	fTexWidth  = m_pTexData->GetPixWidth();
	FLOAT	fTexHeight = m_pTexData->GetPixHeight();

	m_rtInfoUL.SetUV( 0, 137, 20, 157, fTexWidth, fTexHeight );
	m_rtInfoUM.SetUV( 20, 137, 120, 157, fTexWidth, fTexHeight );
	m_rtInfoUR.SetUV( 120, 137, 140, 157, fTexWidth, fTexHeight );
	m_rtInfoML.SetUV( 0, 157, 20, 177, fTexWidth, fTexHeight );
	m_rtInfoMM.SetUV( 20, 157, 120, 177, fTexWidth, fTexHeight );
	m_rtInfoMR.SetUV( 120, 157, 140, 177, fTexWidth, fTexHeight );
	m_rtInfoLL.SetUV( 0, 206, 20, 226, fTexWidth, fTexHeight );
	m_rtInfoLM.SetUV( 20, 206, 120, 226, fTexWidth, fTexHeight );
	m_rtInfoLR.SetUV( 120, 206, 140, 226, fTexWidth, fTexHeight );


	m_rsSkillName.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);
	m_rsSkillDesc.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 50);

	m_rsCurrentSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);
	m_rsNextSkillInfo.Create(NULL, 0, 0, SKILLINFO_MIN_WIDTH, 200);

	m_rcSkillInfo.SetRect(0, 0, SKILLINFO_MIN_WIDTH, 50);
}

void CUISkillToolTip::ShowSkillLearnInfo(int nSkillIdx)
{
	CTString strTemp;
	CTString strNeedLevel, strNeedSP, strNeedSkill, strNeedItem, strNeedStr, strNeedDex, strNeedInt, strNeedCon;
	COLOR	strColor;
	COLOR	colNeed[12];
	INDEX	ColorCount = -1;
	BOOL bLearnedSkill;	// �̹� ��� ��ų�ΰ���?
	INDEX	LevelAccess;

	ResetSkillInfoString();

	m_bShowInfo = true;
	//	m_bSkillInfoVisible = FALSE;

	CUIManager* pUIManager = CUIManager::getSingleton();
	int nIndex = nSkillIdx;

	CSkill		&rSkill = _pNetwork->GetSkillData( nIndex );
	int nLevel = MY_INFO()->GetSkillLevel(nIndex); // ������ ������ 1����, ��ų������ ���� �迭�� 0 ����
	bLearnedSkill = (nLevel > 0) ? TRUE:FALSE;

	if (_pNetwork->m_ubGMLevel > 1)
	{
		CTString strIndex;
		strIndex.PrintF("Index [%d]", nIndex);

		m_rsSkillName.AddString(strIndex, DEF_UI_COLOR_WHITE);
	}

	strColor =  0x0077FFFF;
	strTemp.PrintF("%s(%d/%d)",rSkill.GetName(), nLevel, rSkill.GetMaxLevel());
	m_rsSkillName.AddString(strTemp, strColor);

	if (rSkill.GetType() == CSkill::ST_MELEE ||
		rSkill.GetType() == CSkill::ST_RANGE ||
		rSkill.GetType() == CSkill::ST_MAGIC ||
		rSkill.GetType() == CSkill::ST_SUMMON_TOTEM_SKILL)
	{
		m_rsSkillDesc.AddString( _S( 63, "��Ƽ�� ��ų" ), 0xCACACAFF );
	}
	else if ( rSkill.GetType() == CSkill::ST_PASSIVE )
		m_rsSkillDesc.AddString( _S( 67, "�нú� ��ų" ), 0xCACACAFF );

	strColor =  0x72D02EFF;
	strTemp = rSkill.GetDescription();
	m_rsSkillDesc.AddString(strTemp, strColor);

	if( rSkill.GetFlag() & SF_SINGLEMODE )
	{
		strTemp.PrintF(_S(499, "�۽��δ��� ���� ��ų"));
		m_rsSkillName.AddString(strTemp, strColor);
	}

	for(int n = 0; n < 2; n++, nLevel++)
	{
		if (!bLearnedSkill && nLevel > 0)
		{
			break;
		}

		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
		BOOL	bLearnSkill = FALSE;
		BOOL	bLearnItem = FALSE;

		LevelAccess = (bLearnedSkill) ? (nLevel - 1):0;
		
		if( LevelAccess >= rSkill.GetMaxLevel() )
			continue;

		for( int i = 0; i < 3; i++ )
		{
			nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( LevelAccess, i );
			sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( LevelAccess, i );
			nLearnItemIndex[i] = rSkill.GetLearnItemIndex( LevelAccess, i );
			nLearnItemCount[i] = rSkill.GetLearnItemCount( LevelAccess, i );

			if( nLearnSkillIndex[i] != -1 )
				bLearnSkill = TRUE;
			if( nLearnItemIndex[i] != -1 )
				bLearnItem = TRUE;
		}

		switch( rSkill.GetType() )
		{
		case CSkill::ST_MELEE:					// Active
		case CSkill::ST_RANGE:					// Active
		case CSkill::ST_MAGIC:					// Active
		case CSkill::ST_PASSIVE:
		case CSkill::ST_SUMMON_TOTEM_SKILL:
			{
				const int iLearnStr = rSkill.GetLearnStr( LevelAccess );
				const int iLearnDex = rSkill.GetLearnDex( LevelAccess );
				const int iLearnInt = rSkill.GetLearnInt( LevelAccess );
				const int iLearnCon = rSkill.GetLearnCon( LevelAccess );

				strNeedLevel.PrintF(_S(256, "�ʿ� ���� : %d"), rSkill.GetLearnLevel(LevelAccess));

				CTString strSp;
				strSp.PrintF("%d", rSkill.GetLearnSP(LevelAccess));
				pUIManager->InsertCommaToString(strSp);
				strNeedSP.PrintF(_S(257, "�ʿ� SP : %s"), strSp);

				strNeedStr.PrintF(_S(1391, "�ʿ� �� : %d"), iLearnStr);
				strNeedDex.PrintF(_S(1392, "�ʿ� ��ø : %d"), iLearnDex);
				strNeedInt.PrintF(_S(1393, "�ʿ� ���� : %d"), iLearnInt);
				strNeedCon.PrintF(_S(1394, "�ʿ� ü�� : %d"), iLearnCon);

				if(n == 1 || !bLearnedSkill)
				{
					int Satisfied = IsSatisfiedSkill(nSkillIdx, nLevel, FALSE);
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_LEVEL ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_SP ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_STR ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_DEX ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_INT ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_CON ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_SKILL_0 ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_SKILL_1 ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_SKILL_2 ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_ITEM_0 ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_ITEM_1 ? NOT_SATISFACTION_COL : SATISFACTION_COL;
					colNeed[++ColorCount] = Satisfied & NOT_SATISFIED_ITEM_2 ? NOT_SATISFACTION_COL : SATISFACTION_COL;
				}

				AddSkillInfoString(SKILLINFO_CURRENT + n, strNeedLevel, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_LEVEL]);
				AddSkillInfoString(SKILLINFO_CURRENT + n, strNeedSP, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_SP]);

				if (iLearnStr > 0)
				{
					AddSkillInfoString(SKILLINFO_CURRENT + n, strNeedStr, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_STR]);
				}

				if (iLearnDex > 0)
				{
					AddSkillInfoString(SKILLINFO_CURRENT + n, strNeedDex, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_DEX]);
				}

				if (iLearnInt > 0)
				{
					AddSkillInfoString(SKILLINFO_CURRENT + n, strNeedInt, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_INT]);
				}

				if (iLearnCon > 0)
				{
					AddSkillInfoString(SKILLINFO_CURRENT + n, strNeedCon, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_CON]);
				}

				// Need skill
				if( bLearnSkill )
				{
					for( int i = 0; i < 3; i++ )
					{
						if( nLearnSkillIndex[i] != -1 )
						{
							CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );

							strTemp.PrintF( " : %s %s.%d", rNeedSkill.GetName(), _S( 4414, "LV" ),sbLearnSkillLevel[i] );

							strTemp = _S( 258, "�ʿ� ��ų" ) + strTemp;

							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_SKILL_0 + i]);
						}
					}
				}

				// Need item
				if( bLearnItem )
				{
					CTString strCount;
					for( int i = 0; i < 3; i++ )
					{
						if( nLearnItemIndex[i] != -1 )
						{
							strCount = pUIManager->IntegerToCommaString(nLearnItemCount[i]);
							strTemp.PrintF( _S( 260, "  %s %s��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), strCount );
							strTemp = _S( 259, "�ʿ� ������" ) + strTemp;

							AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : colNeed[SKILL_CONDITION_ITEM_0 + i]);
						}
					}
				}

				int	nNeedMP = rSkill.GetNeedMP( LevelAccess );
				int	nNeedHP = rSkill.GetNeedHP( LevelAccess );

				if( nNeedHP == 0 )
				{
					if( nNeedMP != 0 )
					{
						strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
					}
				}
				else
				{
					if( nNeedMP == 0 )
					{
						strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );	
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
					}
					else
					{
						strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
						strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
					}
				}

				//�Ҹ� ������ ǥ�� �߰�
				{
					int		nNeed[2], nCnt[2];
					nNeed[0] = rSkill.GetNeedItemIndex1(LevelAccess);
					nNeed[1] = rSkill.GetNeedItemIndex2(LevelAccess);
					nCnt[0] = rSkill.GetNeedItemCount1(LevelAccess);
					nCnt[1] = rSkill.GetNeedItemCount2(LevelAccess);					
					bool bRet[2];
					SWORD swTab, swIdx;
					COLOR col = NOT_SATISFACTION_COL;

					for (int i = 0; i < 2; ++i)
					{
						bRet[i] = ItemHelp::HaveItem( nNeed[i], &swTab, &swIdx, nCnt[i] );

						if (bRet[i] == true)
							col = SATISFACTION_COL;
					}

					if (nNeed[0] > 0 || nNeed[1] > 0)
					{
						strTemp.PrintF(_S(4405,"�Ҹ� ������"));
						AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, col);

						for (int i = 0; i < 2; ++i)
						{
							if (nNeed[i] > 0)
							{
								const char	*pItemName = _pNetwork->GetItemName( nNeed[i] );
								strTemp.PrintF("- %s(%d)", pItemName, nCnt[i]);
								AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, bRet[i] ? SATISFACTION_COL : NOT_SATISFACTION_COL);
							}
						}
					}
				}

				if( rSkill.GetPower( LevelAccess ) > 0 )
				{
					strTemp.PrintF( _S( 65, "���� : %d" ), rSkill.GetPower( LevelAccess ) );
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
				}

				strTemp.PrintF( _S( 66, "��ȿ �Ÿ� : %.1f" ), rSkill.GetFireRange() );
				AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);

				if(rSkill.GetDurTime(LevelAccess) > 0)
				{
					strTemp.PrintF( _S( 4172, "���ӽð� : %d�� " ), rSkill.GetDurTime(LevelAccess) / 10);
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
				}

				if(rSkill.GetReUseTime() > 0)
				{
					strTemp.PrintF( _S( 4173, "���� �ð� : %d�� " ), rSkill.GetReUseTime() / 10);
					AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, (n==0 && bLearnedSkill) ? SKILL_LEARNED_COLOR : SATISFACTION_COL);
				}
			}
			break;
		}

		// �Ӽ� �ý��� ��ų ���� �Ӽ� ��Ʈ�� ��� [1/21/2013 Ranma]
		CTString strAtt;
		int nAtt = 0, nAttLv = 0;

		// ���� �Ӽ� [1/21/2013 Ranma]
		if (!bLearnedSkill)
			nAtt = int(rSkill.GetAttrAtt(nLevel));
		else 
			nAtt = int(rSkill.GetAttrAtt(nLevel - 1));

		// �Ӽ� �ý��� ��ų ���� �Ӽ� ��Ʈ�� ��� [1/21/2013 Ranma]
		if (nAtt > 0)
		{
			strAtt = UtilHelp::getSingleton()->GetAttributeString(nAtt);

			if (!bLearnedSkill)
				nAttLv = int(rSkill.GetAttrAttLv(nLevel));
			else 
				nAttLv = int(rSkill.GetAttrAttLv(nLevel - 1));

			strTemp.PrintF( _S(5845, "�Ӽ� ���� : [%s]�Ӽ� [%d]�ܰ�"), strAtt, nAttLv);

			AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, 0xFF0000FF);
		}

		// ��� �Ӽ� [1/21/2013 Ranma]
		if (!bLearnedSkill)
			nAtt = int(rSkill.GetAttrDef(nLevel));
		else 
			nAtt = int(rSkill.GetAttrDef(nLevel - 1));

		// �Ӽ� �ý��� ��ų ���� �Ӽ� ��Ʈ�� ��� [1/21/2013 Ranma]
		if (nAtt > 0)
		{
			strAtt = UtilHelp::getSingleton()->GetAttributeString(nAtt);

			if (!bLearnedSkill)
				nAttLv = int(rSkill.GetAttrDefLv(nLevel));
			else 
				nAttLv = int(rSkill.GetAttrDefLv(nLevel - 1));

			strTemp.PrintF( _S(5845, "�Ӽ� ���� : [%s]�Ӽ� [%d]�ܰ�"), strAtt, nAttLv);

			AddSkillInfoString(SKILLINFO_CURRENT + n, strTemp, 0x0000EEFF);
		}
	}
// 	int nNewPosX;
// 	int nNewPosY;
// 
// 	//m_btnSelectedSkill[nRow].GetAbsPos(nNewPosX, nNewPosY);
// 
// 	nNewPosX = nX;
// 	nNewPosY = nY;
// 
// 	if(nNewPosX + GetSkillInfoWidth() > pUIManager->GetDrawPort()->GetWidth())
// 	{
// 		nNewPosX -= GetSkillInfoWidth();
// 	}
// 
// 	if(nNewPosY + GetSkillInfoHeight() + 34 > pUIManager->GetDrawPort()->GetHeight())
// 	{
// 		nNewPosY -= GetSkillInfoHeight();
// 	}
// 	else
// 	{
// 		nNewPosY += 34;
// 	}
// 
// 	SetSkillInfoPos(nNewPosX, nNewPosY);
}

void CUISkillToolTip::RenderSkillInfoDesc()
{
	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();

	if( m_pTexData == NULL )
		return;

	pDrawPort->InitTextureData( m_pTexData );

	pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top,
		m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top + 20,
		m_rtInfoUL.U0, m_rtInfoUL.V0, m_rtInfoUL.U1, m_rtInfoUL.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top,
		m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top + 20,
		m_rtInfoUM.U0, m_rtInfoUM.V0, m_rtInfoUM.U1, m_rtInfoUM.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top,
		m_rcSkillInfo.Right, m_rcSkillInfo.Top + 20,
		m_rtInfoUR.U0, m_rtInfoUR.V0, m_rtInfoUR.U1, m_rtInfoUR.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Top + 20,
		m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom - 20,
		m_rtInfoML.U0, m_rtInfoML.V0, m_rtInfoML.U1, m_rtInfoML.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Top + 20,
		m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom - 20,
		m_rtInfoMM.U0, m_rtInfoMM.V0, m_rtInfoMM.U1, m_rtInfoMM.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Top + 20,
		m_rcSkillInfo.Right, m_rcSkillInfo.Bottom - 20,
		m_rtInfoMR.U0, m_rtInfoMR.V0, m_rtInfoMR.U1, m_rtInfoMR.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left, m_rcSkillInfo.Bottom - 20,
		m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom,
		m_rtInfoLL.U0, m_rtInfoLL.V0, m_rtInfoLL.U1, m_rtInfoLL.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Left + 20, m_rcSkillInfo.Bottom - 20,
		m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom,
		m_rtInfoLM.U0, m_rtInfoLM.V0, m_rtInfoLM.U1, m_rtInfoLM.V1,
		0xFFFFFFBB );
	pDrawPort->AddTexture( m_rcSkillInfo.Right - 20, m_rcSkillInfo.Bottom - 20,
		m_rcSkillInfo.Right, m_rcSkillInfo.Bottom,
		m_rtInfoLR.U0, m_rtInfoLR.V0, m_rtInfoLR.U1, m_rtInfoLR.V1,
		0xFFFFFFBB );


	m_rsSkillName.Render();
	m_rsSkillDesc.Render();

	m_rsCurrentSkillInfo.Render();
	m_rsNextSkillInfo.Render();

	// Render all elements
	pDrawPort->FlushRenderingQueue();
}

void CUISkillToolTip::AddSkillDescString( CTString &strDesc, COLOR colDesc /*= 0xF2F2F2FF */ )
{
	// Get length of string
	INDEX	nLength = strDesc.Length();
	if( nLength == 0 )
		return;

	int		iPos;
	// wooss 051002
#if defined G_THAI
	// Get length of string
	INDEX	nThaiLen = FindThaiLen(strDesc);
	INDEX	nChatMax= (_iMaxMsgStringChar-1)*(_pUIFontTexMgr->GetFontWidth()+_pUIFontTexMgr->GetFontSpacing());
	if( nLength == 0 )
		return;
	// If length of string is less than max char
	if( nThaiLen <= nChatMax )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbSkillDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddSkillDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if(nChatMax < FindThaiLen(strDesc,0,iPos))
				break;
		}
		nSplitPos = iPos;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddSkillDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddSkillDescString( strTemp, colDesc );
		}

	}
#else
	// If length of string is less than max char
	if( nLength <= _iMaxMsgStringChar )
	{
		// Check line character
		for( iPos = 0; iPos < nLength; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nLength )
		{
			m_lbSkillDesc.AddString( 0, strDesc, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddSkillDescString( strTemp, colDesc );
		}
	}
	// Need multi-line
	else
	{
		// Check splitting position for 2 byte characters
		int		nSplitPos = _iMaxMsgStringChar;
		BOOL	b2ByteChar = FALSE;
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] & 0x80 )
				b2ByteChar = !b2ByteChar;
			else
				b2ByteChar = FALSE;
		}

		if( b2ByteChar )
			nSplitPos--;

		// Check line character
		for( iPos = 0; iPos < nSplitPos; iPos++ )
		{
			if( strDesc[iPos] == '\n' || strDesc[iPos] == '\r' )
				break;
		}

		// Not exist
		if( iPos == nSplitPos )
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( nSplitPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim space
			if( strTemp[0] == ' ' )
			{
				int	nTempLength = strTemp.Length();
				for( iPos = 1; iPos < nTempLength; iPos++ )
				{
					if( strTemp[iPos] != ' ' )
						break;
				}

				strTemp.TrimLeft( strTemp.Length() - iPos );
			}

			AddSkillDescString( strTemp, colDesc );
		}
		else
		{
			// Split string
			CTString	strTemp, strTemp2;
			strDesc.Split( iPos, strTemp2, strTemp );
			m_lbSkillDesc.AddString( 0, strTemp2, colDesc );

			// Trim line character
			if( strTemp[0] == '\r' && strTemp[1] == '\n' )
				strTemp.TrimLeft( strTemp.Length() - 2 );
			else
				strTemp.TrimLeft( strTemp.Length() - 1 );

			AddSkillDescString( strTemp, colDesc );
		}
	}
#endif
}

void CUISkillToolTip::SetSkillInfoPos( int nPosX, int nPosY )
{
	int nNextPosy;

	m_rsSkillName.SetPos(nPosX + 10, nPosY + 10);
	nNextPosy = nPosY + m_rsSkillName.GetHeight() + 10;
	m_rsSkillDesc.SetPos(nPosX + 10, nNextPosy);

	nNextPosy += m_rsSkillDesc.GetHeight();
	m_rsCurrentSkillInfo.SetPos(nPosX + 10, nNextPosy);

	m_rsNextSkillInfo.SetPos(nPosX + m_rsCurrentSkillInfo.GetWidth() + 10, nNextPosy);

	m_rcSkillInfo.SetRect(nPosX, nPosY, nPosX+m_rcSkillInfo.GetWidth(), nPosY+m_rcSkillInfo.GetHeight());
}

void CUISkillToolTip::GetSkillDesc( int nIndex, int nLevel /*= 0*/, BOOL bSpecial /*= FALSE */ )
{
	m_lbSkillDesc.ResetAllStrings();

	// If skill is not exist
	if( nIndex == -1 )
		return;

	// Make description of skill
	CTString strTemp;
	CTString strSp;

	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nIndex );

		strTemp.PrintF( "%s", rSkill.GetName() );
		AddSkillDescString( strTemp, 0xFFC672FF );

		if( rSkill.GetFlag() & SF_SINGLEMODE )
			AddSkillDescString( _S( 499, "�۽��δ��� ���� ��ų" ), 0xCACACAFF );		

		AddSkillDescString( CTString( " " ) );

		const char	*pDesc = rSkill.GetDescription();
		if( pDesc != NULL )
		{
			strTemp.PrintF( "%s\n\n", pDesc );
			AddSkillDescString( strTemp, 0xC5C5C5FF );
		}

		--nLevel;

		// Get learning condition
		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
		BOOL	bLearnSkill = FALSE;
		BOOL	bLearnItem = FALSE;

		for( int i = 0; i < 3; i++ )
		{
			nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
			sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
			nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
			nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );

			if( nLearnSkillIndex[i] != -1 )
				bLearnSkill = TRUE;
			if( nLearnItemIndex[i] != -1 )
				bLearnItem = TRUE;
		}

		switch( rSkill.GetType() )
		{
		case CSkill::ST_MELEE:					// Active
		case CSkill::ST_RANGE:					// Active
		case CSkill::ST_MAGIC:					// Active
		case CSkill::ST_SUMMON_TOTEM_SKILL:
			{
				strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), rSkill.GetLearnLevel( nLevel ) );
				AddSkillDescString( strTemp, 0xBDA99FFF );

				strSp.PrintF("%d", rSkill.GetLearnSP( nLevel ));
				UIMGR()->InsertCommaToString(strSp);
				strTemp.PrintF( _S( 257, "�ʿ� SP : %s" ), strSp ); // ���õ�
				AddSkillDescString( strTemp, 0xBDA99FFF );				

				const int iLearnStr = rSkill.GetLearnStr( nLevel );
				const int iLearnDex = rSkill.GetLearnDex( nLevel );
				const int iLearnInt = rSkill.GetLearnInt( nLevel );
				const int iLearnCon = rSkill.GetLearnCon( nLevel );

				if( iLearnStr > 0 ) 
				{
					strTemp.PrintF( _S( 1391, "�ʿ� �� : %d" ), iLearnStr );		// �ʿ� �� 
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnDex > 0 ) 
				{
					strTemp.PrintF( _S( 1392, "�ʿ� ��ø : %d" ), iLearnDex );	// �ʿ� ��ø
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnInt > 0 ) 
				{
					strTemp.PrintF( _S( 1393, "�ʿ� ���� : %d" ), iLearnInt );	// �ʿ� ����
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnCon > 0 )
				{
					strTemp.PrintF( _S( 1394, "�ʿ� ü�� : %d" ), iLearnCon );	// �ʿ� ü��
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				// Need skill
				if( bLearnSkill )
				{
					AddSkillDescString( _S( 258, "�ʿ� ��ų" ), 0xBDA99FFF );
					for( int i = 0; i < 3; i++ )
					{
						if( nLearnSkillIndex[i] != -1 )
						{
							CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );

							strTemp.PrintF( "  %s %s.%d", rNeedSkill.GetName(), _S( 4414, "LV" ),sbLearnSkillLevel[i] );

							AddSkillDescString( strTemp, 0xBDA99FFF );
						}
					}
				}

				// Need item
				if( bLearnItem )
				{
					AddSkillDescString( _S( 259, "�ʿ� ������" ), 0xBDA99FFF );
					CTString strCount;
					for( int i = 0; i < 3; i++ )
					{
						if( nLearnItemIndex[i] != -1 )
						{
							strCount = UIMGR()->IntegerToCommaString(nLearnItemCount[i]);
							strTemp.PrintF( _S( 260, "  %s %s��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), strCount );
							AddSkillDescString( strTemp, 0xBDA99FFF );
						}
					}
				}

				int	nNeedMP = rSkill.GetNeedMP( nLevel );
				int	nNeedHP = rSkill.GetNeedHP( nLevel );
				if( nNeedHP == 0 )
				{
					if( nNeedMP != 0 )
					{
						strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}
				else
				{
					if( nNeedMP == 0 )
					{
						strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );		
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
					else
					{
						strTemp.PrintF( _S( 64, "�Ҹ� MP : %d" ), nNeedMP );
						AddSkillDescString( strTemp, 0xBDA99FFF );
						strTemp.PrintF( _S( 500, "�Ҹ� HP : %d" ), nNeedHP );		
						AddSkillDescString( strTemp, 0xBDA99FFF );
					}
				}

				//�Ҹ� ������ ǥ�� �߰�
				if (rSkill.GetNeedItemIndex1(nLevel) > 0 || rSkill.GetNeedItemIndex2(nLevel) > 0)
				{
					AddSkillDescString(_S(4405,"�Ҹ� ������"), 0xBDA99FFF);
					if (rSkill.GetNeedItemIndex1(nLevel) > 0)
					{
						const char	*pItemName = _pNetwork->GetItemName( rSkill.GetNeedItemIndex1(nLevel) );
						strTemp.PrintF("- %s(%d)", pItemName, rSkill.GetNeedItemCount1(nLevel));
						AddSkillDescString(strTemp, 0xBDA99FFF);
					}

					if (rSkill.GetNeedItemIndex2(nLevel) > 0)
					{
						const char	*pItemName = _pNetwork->GetItemName( rSkill.GetNeedItemIndex2(nLevel) );
						strTemp.PrintF("- %s(%d)", pItemName, rSkill.GetNeedItemCount2(nLevel));
						AddSkillDescString(strTemp, 0xBDA99FFF);
					}
				}

				if( rSkill.GetPower( nLevel ) > 0 )
				{
					strTemp.PrintF( _S( 65, "���� : %d" ), rSkill.GetPower( nLevel ) );
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				strTemp.PrintF( _S( 66, "��ȿ �Ÿ� : %.1f" ), rSkill.GetFireRange() );
				AddSkillDescString( strTemp, 0xBDA99FFF );
				strTemp.PrintF( _S( 261, "�ִ뽺ų ���� : %d" ), rSkill.GetMaxLevel() );
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			break;

		case CSkill::ST_PASSIVE:				// Passive
			{
				strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), rSkill.GetLearnLevel( nLevel ) );
				AddSkillDescString( strTemp, 0xBDA99FFF );

				strSp.PrintF("%d", rSkill.GetLearnSP( nLevel ));
				UIMGR()->InsertCommaToString(strSp);
				strTemp.PrintF( _S( 257, "�ʿ� SP : %s" ), strSp ); // ���õ�
				AddSkillDescString( strTemp, 0xBDA99FFF );

				const int iLearnStr = rSkill.GetLearnStr( nLevel );
				const int iLearnDex = rSkill.GetLearnDex( nLevel );
				const int iLearnInt = rSkill.GetLearnInt( nLevel );
				const int iLearnCon = rSkill.GetLearnCon( nLevel );

				if( iLearnStr > 0 ) 
				{
					strTemp.PrintF( _S( 1391, "�ʿ� �� : %d" ), iLearnStr );		// �ʿ� ��
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnDex > 0 ) 
				{
					strTemp.PrintF( _S( 1392, "�ʿ� ��ø : %d" ), iLearnDex );	// �ʿ� ��ø
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnInt > 0 ) 
				{
					strTemp.PrintF( _S( 1393, "�ʿ� ���� : %d" ), iLearnInt );	// �ʿ� ����
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				if( iLearnCon > 0 )
				{
					strTemp.PrintF( _S( 1394, "�ʿ� ü�� : %d" ), iLearnCon );	// �ʿ� ü��
					AddSkillDescString( strTemp, 0xBDA99FFF );
				}

				// Need skill
				if( bLearnSkill )
				{
					AddSkillDescString( _S( 258, "�ʿ� ��ų" ), 0xBDA99FFF );
					for( int i = 0; i < 3; i++ )
					{
						if( nLearnSkillIndex[i] != -1 )
						{
							CSkill	&rNeedSkill = _pNetwork->GetSkillData( nLearnSkillIndex[i] );

							strTemp.PrintF( "  %s %s.%d", rNeedSkill.GetName(), _S( 4414, "LV" ),sbLearnSkillLevel[i] );

							AddSkillDescString( strTemp, 0xBDA99FFF );
						}
					}
				}

				// Need item
				if( bLearnItem )
				{
					AddSkillDescString( _S( 259, "�ʿ� ������" ), 0xBDA99FFF );
					CTString strCount;
					for( int i = 0; i < 3; i++ )
					{
						if( nLearnItemIndex[i] != -1 )
						{
							strCount = UIMGR()->IntegerToCommaString(nLearnItemCount[i]);
							strTemp.PrintF( _S( 260, "  %s %s��" ), _pNetwork->GetItemName( nLearnItemIndex[i] ), strCount );
							AddSkillDescString( strTemp, 0xBDA99FFF );
						}
					}
				}

				strTemp.PrintF( _S( 261, "�ִ뽺ų ���� : %d" ), rSkill.GetMaxLevel() );
				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
			break;
		}
	}
	else
	{
		CSpecialSkill* pSSkill = CSpecialSkill::getData(nIndex);

		if (pSSkill == NULL)
			return;

		strTemp.PrintF( "%s\n\n", pSSkill->GetName() );
		AddSkillDescString( strTemp, 0xFFC672FF );

		const char *pDesc = pSSkill->GetDescription();
		if( pDesc != NULL )
		{
			strTemp.PrintF( "%s\n\n", pDesc );
			AddSkillDescString( strTemp, 0xC5C5C5FF );
		}

		--nLevel;

		// Get learning condition
		int		nLearnSkillIndex;
		SBYTE	sbLearnSkillLevel;
		BOOL	bLearnSkill	= FALSE;
		BOOL	bLearnItem	= FALSE;		

		nLearnSkillIndex	= pSSkill->GetLearnSkillIndex();
		sbLearnSkillLevel	= pSSkill->GetLearnSkillLevel();

		if( nLearnSkillIndex != -1 )
			bLearnSkill = TRUE;

		strTemp.PrintF( _S( 256, "�ʿ� ���� : %d" ), pSSkill->GetLearnLevel( nLevel ) );
		AddSkillDescString( strTemp, 0xBDA99FFF );

		strSp.PrintF("%d", pSSkill->GetLearnSP( nLevel ));
		UIMGR()->InsertCommaToString(strSp);
		strTemp.PrintF( _S( 257, "�ʿ� SP : %s" ), strSp ); // ���õ�
		AddSkillDescString( strTemp, 0xBDA99FFF );

		// Need skill
		if( bLearnSkill )
		{
			AddSkillDescString( _S( 258, "�ʿ� ��ų" ), 0xBDA99FFF );
			if( nLearnSkillIndex != -1 )
			{
				CSpecialSkill* pNeedSSkill = CSpecialSkill::getData(nLearnSkillIndex);
					
				if (pNeedSSkill == NULL)
					return;

				strTemp.PrintF( "  %s %s.%d", pNeedSSkill->GetName(), _S( 4414, "LV" ),sbLearnSkillLevel );

				AddSkillDescString( strTemp, 0xBDA99FFF );
			}
		}
	}
}

void CUISkillToolTip::ResetSkillInfoString()
{
	m_rsSkillName.ClearString();
	m_rsSkillDesc.ClearString();

	m_rsCurrentSkillInfo.ClearString();
	m_rsNextSkillInfo.ClearString();
	m_bShowInfo = false;
}

void CUISkillToolTip::Render(CDrawPort* pDraw)
{
	if( m_bShowInfo == true )
		RenderSkillInfoDesc();
}

int CUISkillToolTip::IsSatisfiedSkill(int nSkillIndex, int nLevel, BOOL bSpecial, BOOL bReinforcementSkill /*= FALSE*/ )
{
	int nSatisfied = 0;

	if( nSkillIndex < 0 )
		return SKILL_NOT_EXIST;

	if( nLevel > 0 )
		--nLevel; // ������ ������ 1����, ��ų������ ���� �迭�� 0 ����

	if( !bSpecial )
	{
		CSkill		&rSkill = _pNetwork->GetSkillData( nSkillIndex );

		int		nLearnSkillIndex[3];
		SBYTE	sbLearnSkillLevel[3];
		int		nLearnItemIndex[3];
		SQUAD	nLearnItemCount[3];
		BOOL	bLearnSkill = FALSE;
		BOOL	bLearnItem = FALSE;

		for( int i = 0; i < 3; i++ )
		{
			nLearnSkillIndex[i] = rSkill.GetLearnSkillIndex( nLevel, i );
			sbLearnSkillLevel[i] = rSkill.GetLearnSkillLevel( nLevel, i );
			nLearnItemIndex[i] = rSkill.GetLearnItemIndex( nLevel, i );
			nLearnItemCount[i] = rSkill.GetLearnItemCount( nLevel, i );

			if( bReinforcementSkill != TRUE && nLearnSkillIndex[i] != -1 )
				bLearnSkill = TRUE;
			if( nLearnItemIndex[i] != -1 )
				bLearnItem = TRUE;
		}

		// �ʿ� ���� üũ
		if(_pNetwork->MyCharacterInfo.level < rSkill.GetLearnLevel( nLevel ))
			nSatisfied |= NOT_SATISFIED_LEVEL;
		// �ʿ� ���õ� üũ
		if((_pNetwork->MyCharacterInfo.sp / 10000) < rSkill.GetLearnSP( nLevel ))
			nSatisfied |= NOT_SATISFIED_SP;
		// �ʿ� �ɷ�ġ üũ

		const int iLearnStr = rSkill.GetLearnStr( nLevel );
		const int iLearnDex = rSkill.GetLearnDex( nLevel );
		const int iLearnInt = rSkill.GetLearnInt( nLevel );
		const int iLearnCon = rSkill.GetLearnCon( nLevel );

		if( iLearnStr > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.str - _pNetwork->MyCharacterInfo.opt_str  < iLearnStr )
				nSatisfied |= NOT_SATISFIED_STR;
		}

		if( iLearnDex > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.dex - _pNetwork->MyCharacterInfo.opt_dex < iLearnDex )
				nSatisfied |= NOT_SATISFIED_DEX;
		}

		if( iLearnInt > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.intel - _pNetwork->MyCharacterInfo.opt_intel < iLearnInt )
				nSatisfied |= NOT_SATISFIED_INT;
		}

		if( iLearnCon > 0 ) 
		{
			if( _pNetwork->MyCharacterInfo.con - _pNetwork->MyCharacterInfo.opt_con < iLearnCon )
				nSatisfied |= NOT_SATISFIED_CON;
		}

		CUIManager* pUIManager = CUIManager::getSingleton();

		// �ʿ� ��ų üũ
		if( bLearnSkill )
		{
			for( int i = 0; i < 3; i++ )
			{
				if (nLearnSkillIndex[i] != -1)
				{
					//�� ��ų �������� ����� ã��.
					// ĳ���� �������ΰ�?
					int nCurrentSkillLevel = MY_INFO()->GetSkillLevel(nLearnSkillIndex[i]);
					if(nCurrentSkillLevel < sbLearnSkillLevel[i])
					{
						nSatisfied |= (NOT_SATISFIED_SKILL_0 << i);
					}
				}
			}
		}

		// �ʿ� ������ üũ
		if( bLearnItem )
		{
			for( int i = 0; i < 3; i++ )
			{
				//2013/04/10 jeil
				if( nLearnItemIndex[i] != -1 )
				{
					if(nLearnItemIndex[i] == 19)
					{
						if(_pNetwork->MyCharacterInfo.money < nLearnItemCount[i])
						{
							nSatisfied |= (NOT_SATISFIED_ITEM_0 << i);
						}
					}
					else
					{
						SQUAD nCurrentItemCount = pUIManager->GetInventory()->GetItemCount(nLearnItemIndex[i]);
						if(nCurrentItemCount < nLearnItemCount[i])
						{
							nSatisfied |= (NOT_SATISFIED_ITEM_0 << i);
						}
					}
				}
			}
		}
	}
	else
	{
		CSpecialSkill* pSSkill = CSpecialSkill::getData(nSkillIndex);

		if (pSSkill == NULL)
			return SKILL_NOT_EXIST;

		// Get learning condition
		int		nLearnSkillIndex;
		SBYTE	sbLearnSkillLevel;

		nLearnSkillIndex	= pSSkill->GetLearnSkillIndex();
		sbLearnSkillLevel	= pSSkill->GetLearnSkillLevel();

		// �ʿ� ���� üũ
		if(_pNetwork->MyCharacterInfo.level < pSSkill->GetLearnLevel( nLevel ))
			nSatisfied |= NOT_SATISFIED_LEVEL;
		// �ʿ� ���õ� üũ
		if((_pNetwork->MyCharacterInfo.sp / 10000) < pSSkill->GetLearnSP( nLevel ))
			nSatisfied |= NOT_SATISFIED_SP;

		if( nLearnSkillIndex != -1 )
		{
			int nCurrentSkillLevel = MY_INFO()->GetSkillLevel(nLearnSkillIndex, true);
			if(nCurrentSkillLevel < sbLearnSkillLevel)
			{
				nSatisfied |= NOT_SATISFIED_SKILL_0;
			}
		}
	}

	return nSatisfied;
}

void CUISkillToolTip::AddSkillInfoString( int nSkillInfoList, CTString strSkillInfo, COLOR strColor /*= 0xF2F2F2FF*/ )
{
	if (g_iCountry == RUSSIA)
	{
		INDEX iStrSub = strSkillInfo.FindSubstr("\n");
		if(iStrSub != -1)
		{
			CTString	strTemp, strTemp2;
			strTemp = strSkillInfo;
			strTemp.str_String[iStrSub] = ' ';


			strTemp.Split( iStrSub+1, strTemp, strTemp2 );

			AddSkillInfoString(nSkillInfoList, strTemp, strColor );
			AddSkillInfoString(nSkillInfoList, strTemp2, strColor );
			return;
		}
	}

	if(nSkillInfoList == SKILLINFO_CURRENT)
	{
		m_rsCurrentSkillInfo.AddString(strSkillInfo, strColor);
	}
	else if(nSkillInfoList == SKILLINFO_NEXT)
	{
		m_rsNextSkillInfo.AddString(strSkillInfo, strColor);
	}

	int nHeight = 0;
	int nWidth =  0;

	if(m_rsNextSkillInfo.GetMaxStrWidth() == 0)
	{

		m_rsSkillName.SetWidth(SKILLINFO_MIN_WIDTH);
		nHeight = m_rsSkillName.GetMaxStrHeight();
		m_rsSkillName.SetHeight(nHeight);

		m_rsSkillDesc.SetWidth(SKILLINFO_MIN_WIDTH);
		nHeight = m_rsSkillDesc.GetMaxStrHeight();
		m_rsSkillDesc.SetHeight(nHeight);

		nWidth  = SKILLINFO_MIN_WIDTH;
	}
	else
	{
		m_rsSkillName.SetWidth(SKILLINFO_MAX_WIDTH);
		nHeight = m_rsSkillName.GetMaxStrHeight();
		m_rsSkillName.SetHeight(nHeight);

		m_rsSkillDesc.SetWidth(SKILLINFO_MAX_WIDTH);
		nHeight = m_rsSkillDesc.GetMaxStrHeight();
		m_rsSkillDesc.SetHeight(nHeight);

		nWidth  = SKILLINFO_MAX_WIDTH;
	}

	nHeight = m_rsSkillName.GetMaxStrHeight() + m_rsSkillDesc.GetMaxStrHeight();

	if( m_rsCurrentSkillInfo.GetMaxStrHeight() > m_rsNextSkillInfo.GetMaxStrHeight() )
	{
		nHeight += m_rsCurrentSkillInfo.GetMaxStrHeight();
	}
	else
	{
		nHeight += m_rsNextSkillInfo.GetMaxStrHeight();
	}

	m_rcSkillInfo.SetRect(m_rcSkillInfo.Left, m_rcSkillInfo.Top, m_rcSkillInfo.Left + nWidth + 20, m_rcSkillInfo.Top + nHeight + 20);
}
