#include "stdh.h"

// header ����. [12/1/2009 rumist]
#include <Engine/Interface/UIInternalClasses.h>
#include <Engine/Interface/UIAutoHelp.h>
#include <Engine/GameStageManager/StageMgr.h>
#include <Engine/Entities/LevelupGuide.h>
#include <Engine/Entities/SkillTree.h>
#include <Engine/Entities/Skill.h>
#include <Engine/Help/Util_Help.h>

#ifdef	IMPROV1107_NOTICESYSTEM
#define	CHATMSG_NOTICE_MOVEPITCHTIME	300
#define	CHATMSG_NOTICE_VIEWCOUNT		3
#endif

CUIAutoHelp*	_UIAutoHelp = NULL;
extern INDEX g_iCountry;
bool _SAutoHelpInfo::CheckInfo()
{
	// �ѹ��� �ϴ� �ſ� .. ~
	if ( m_bJustOnce ) 
	{
		if ( m_bActive )
		{
			return false;
		}
	}

	if ( m_iEndLevel == -1 && m_iStartLevel == -1 )
	{
	
	}
	else
	{
		// FIXME : ��� ������ ��Ʈ��ũ�� ����ϰ� ����.
		SWORD Level = _pNetwork->MyCharacterInfo.level;
					
		// ���� üũ 
		if ( Level < m_iStartLevel || Level > m_iEndLevel )
		{
			return false;
		}
	}
	// ���� üũ ( ���� �� )

	
	m_bActive = true; 
	return true;
}

// ----------------------------------------------------------------------------
// Name : CUIAutoHelp()
// Desc : Constructor
// ----------------------------------------------------------------------------
CUIAutoHelp::CUIAutoHelp()
{
	m_ptdBaseTexture = NULL;
	m_nUpStat = 0;
	Clear ();

	if (IsBila(g_iCountry) == FALSE)
		m_nInterval_time = 15000;
	else
		m_nInterval_time = 30000;
}

// ----------------------------------------------------------------------------
// Name : ~CUIAutoHelp()
// Desc : Destructor
// ----------------------------------------------------------------------------
CUIAutoHelp::~CUIAutoHelp()
{
	Clear ();
}

// ----------------------------------------------------------------------------
// Name : Clear()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Clear()
{
	m_bStop				= FALSE;		// ���� �����ΰ�?
	m_fShowTime			= AUTOHELO_SHOW_TIME;	// ���� ���� �ִ� �ð�
	m_tmStartTime		= 0;			// ���̱� ���� �� �ð�
	m_bVisible			= FALSE;		// ȭ�鿡 ���� �ְ� �ִ°�?
	m_nActiveIndex		= -1;			// ���� ���� �ְ� �ִ� ���� Index
	m_tmEndTime			= 0;
	m_tmClassification  = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_tmCurTime			= 0;
	m_nKillMobIndex		= -1;
	m_RndHelp			= FALSE;

	for ( int i = 0; i < AUTOHELP_COUNT; i++ )
	{
		m_AutoHelpInfo[i].Clear();
	}
		
	memset ( m_bCondition, FALSE, sizeof ( BOOL ) * AUTOHELP_COUNT );
#if	defined(IMPROV1107_NOTICESYSTEM)
	m_vGMNotice.clear();
	m_bmovGMNotice		= FALSE;
#endif

	STOCK_RELEASE(m_ptdBaseTexture);
}

// ----------------------------------------------------------------------------
// Name : Create()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Create()
{
	Clear ();
	
	// initialize valiables ---------------------wooss 060328-->>
	m_tmCheckTime	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_bChecked		= FALSE;
	m_iNoticeTime	= 3;
	// --------------------------------------------------------<<

	// Create refine texture
	m_ptdBaseTexture = _pTextureStock->Obtain_t ( CTString( "Data\\Interface\\Notice.tex" ) );

	FLOAT	fTexWidth	= m_ptdBaseTexture->GetPixWidth();
	FLOAT	fTexHeight	= m_ptdBaseTexture->GetPixHeight();

	m_rcNotice.SetRect( 0, 119, 0, 138 );
	m_rcGMNotice.SetRect( 0, 140, 0, 159 );
	
	
	// Notice
	m_rtNoticeL.SetUV( 365, 131, 397, 150, fTexWidth, fTexHeight );
	m_rtNoticeC.SetUV( 404, 131, 411, 150, fTexWidth, fTexHeight );
	m_rtNoticeR.SetUV( 419, 131, 451, 150, fTexWidth, fTexHeight );

	
	m_AutoHelpInfo[0].SetInfo ( _S( 467, "�ɾ� ������ ü���� �� ���� ȸ�� �˴ϴ�." ), 
								1, 2 );

	m_AutoHelpInfo[1].SetInfo ( _S( 468,  "�ٴڿ� ������ �������� ���콺�� Ŭ���ϸ� ���� �� �ֽ��ϴ�." ), 
								1, 2, -1, TRUE );

	m_AutoHelpInfo[2].SetInfo (  _S( 469, "�ݱ� ��ư�� �̿��ϸ� �� ���ϰ� �������� ���� �� �ֽ��ϴ�." ),
								1, 2, -1, TRUE );

	m_AutoHelpInfo[3].SetInfo (  _S( 470, "��ų�����͸� ã�ư���, ������ ��ų������ �ø��ų� ���ο� ��ų�� ��� �� �ֽ��ϴ�." ) ,
								1, 3 );

	m_AutoHelpInfo[4].SetInfo (  _S( 471, "���ź���� ���������� �ֽ��ϴ�. ������ ���� ������." ),
								1, 4, -1, TRUE );

	m_AutoHelpInfo[5].SetInfo ( _S( 472, "5������ �ǽø�, �۽��� ������ �� �� �ֽ��ϴ�." ), 
								2, 4 );
	// Quest ...			
	m_AutoHelpInfo[6].SetInfo ( _S( 473, "���� �������� [������ ���븰]���� ���� ����Ʈ�� ������ �� �ֽ��ϴ�." ), 
								1, 1, -1, TRUE );
	m_AutoHelpInfo[7].SetInfo ( _S( 474, "���� �������� [������ ����]���� ���� ���� ��ų ����Ʈ�� ������ �� �ֽ��ϴ�." ), 
								7, 7,-1, TRUE );
	m_AutoHelpInfo[8].SetInfo ( _S( 475, "���� �������� Ʈ��ź�� [���� ������ ��]���� ���� ���� ��ų ����Ʈ�� ������ �� �ֽ��ϴ�." ), 
								8, 8, -1,TRUE );
	m_AutoHelpInfo[9].SetInfo ( _S( 476, "���� �������� [��ȭ���� �Է���]���� ���� ����Ʈ�� ������ �� �ֽ��ϴ�." ), 
								9, 9, -1,TRUE );
	m_AutoHelpInfo[10].SetInfo ( _S( 477, "���� �������� [������ ����]���� ���� ����Ʈ�� ������ �� �ֽ��ϴ�." ), 
								15, 15, -1, TRUE );
	m_AutoHelpInfo[11].SetInfo ( _S( 478, "���� �������� [������ �ο�]���� ���� ����Ʈ�� ������ �� �ֽ��ϴ�." ), 
								23, 23, -1,TRUE );
	m_AutoHelpInfo[12].SetInfo ( _s( "" ), 
								1, 9, -1 );
	// Random Help ...
	m_AutoHelpInfo[13].SetInfo ( _S( 480, "������� �ʴ� ����� ���� ���ɹ̽�Ʈ���� �������ø� ���ü����� ��ȯ�� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[14].SetInfo ( _S( 481, "ALT+B, BŰ�� ������ �������� �� �Խ����� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[15].SetInfo ( _S( 482, "ALT+W, WŰ�� ������ ������ ���ؼ� ���� �� NPC ��ġ�� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[16].SetInfo ( _S( 483, "ALT+T, TŰ�� ������ ���ݷ� ���� ���� ĳ���� ������ Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[17].SetInfo ( _S( 484, "ALT+A, AŰ�� ������ ����/�ɱ�/�ݱ�/��ȯ �������� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[18].SetInfo ( _S( 485, "ALT+S, SŰ�� ������ �Ϲ�, ��ȭ, Ư����ų�� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[19].SetInfo ( _S( 486, "ALT+Q, QŰ�� ������ �������� ����Ʈ�� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[20].SetInfo ( _S( 487, "ALT+C, CŰ�� ������ �Ҽ�â�� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[21].SetInfo ( _S( 488, "ALT+Z, ZŰ�� ������ �ɼǼ����� �ý���â�� Ȯ���� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[22].SetInfo ( _S( 489, "TAB, ALT+E, EŰ�� �̿��Ͽ� ĳ���� �κ��丮â�� �� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[23].SetInfo ( _S( 490, "�ӼӸ��� !ĳ���͸� �ϰ���� ���� ���ؼ� �̿��� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[24].SetInfo ( _S( 491, "����� �ɱ⸦ �ϸ� ������ ������ ������ HP�� MP�� ȸ���˴ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[25].SetInfo ( _S( 492, "�κ��丮â�� ���� �������� ���� Ŭ���ϸ� ���� �� Ż���� �� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );
	
	m_AutoHelpInfo[26].SetInfo ( _S( 493, "������ ���׷��̵�� ���ü��� �̿��Ͽ� ���׷��̵��� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[27].SetInfo ( _S( 494, "���ü��� ����� ���ؼ� ȹ���ϰų� �������� ���ɹ̽�Ʈ�� ���ؼ� ��ȯ�� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[28].SetInfo ( _S( 495, "����Ű(ȭ��ǥ�� �̿��ϸ� ȭ���̵� �� ������ȯ�� �� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[29].SetInfo ( _S( 496, "�ٸ� ������ �̵��ϱ� ���ؼ��� �������� NPC�� ���ؼ� �̵��� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[30].SetInfo ( _S( 497, "������ �����ڸ��� ���콺�� �����̸�, ������ ũ�⸦ Ȯ�� ����� �� �ֽ��ϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	m_AutoHelpInfo[31].SetInfo ( _S( 498, "������ �� ���¿��� ������ Ŭ���ϸ� ���� �������� ���ɴϴ�." ), 
								RND_HELP_LEVEL_LOW, RND_HELP_LEVEL_HIGH );

	
// 070820_ttos: ������� �����, ���� ���� ���� ��� 

	int nLocalLevel = RND_HELP_LEVEL_HIGH;

	if (IsBila(g_iCountry) == TRUE)
	{
		nLocalLevel = RND_HELP_LEVEL_MAX;
	}

	m_AutoHelpInfo[AU_NO_TOXICOSIS_S].SetInfo ( _S( 2547,"��ð��� ������ ���� ������ ����� �մϴ�." ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
	m_AutoHelpInfo[AU_NO_TOXICOSIS_S+1].SetInfo ( _S( 2548,"��ð��� ������ ���� ������ ����� �մϴ�." ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
	m_AutoHelpInfo[AU_NO_TOXICOSIS_S+2].SetInfo ( _S( 2549,"��ð��� ������ ���� ������ ����� �մϴ�." ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
	m_AutoHelpInfo[AU_NO_TOXICOSIS_S+3].SetInfo ( _S( 2550,"��ð��� ������ ���� ������ ����� �մϴ�." ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
		
	m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S].SetInfo ( _S( 2551,"����...�ǿܷ� ª��ϴ�...^^;" ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
	m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+1].SetInfo ( _S( 2552,"����...�ǿܷ� ª��ϴ�...^^;" ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
	m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+2].SetInfo ( _S( 2553,"����...�ǿܷ� ª��ϴ�...^^;" ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
	m_AutoHelpInfo[AU_NO_TOXICOSIS_VACATION_S+3].SetInfo ( _S( 2554,"����...�ǿܷ� ª��ϴ�...^^;" ), 
										RND_HELP_LEVEL_LOW, nLocalLevel );
			
	
	m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	m_RndHelp			= FALSE;
	
}

// ----------------------------------------------------------------------------
// Name : SetInfo()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::SetInfo ( DWORD dwAutoHelpInfo )
{
	if ( dwAutoHelpInfo < 0 || dwAutoHelpInfo >= AUTOHELP_COUNT ) 
	{
		return;
	}

	TIME NowTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if ( NowTime - m_tmEndTime < m_nInterval_time ) return;

	if( AU_NO_TOXICOSIS_S <= dwAutoHelpInfo && AU_NO_TOXICOSIS_VACATION_E >= dwAutoHelpInfo)
	{
		ShowAutoHelp ( dwAutoHelpInfo );
		return;
	}
	
	switch ( dwAutoHelpInfo )
	{

	case AU_MOB_KILL :
		{
			if ( m_bVisible )
			{
				if ( m_nActiveIndex != AU_GET_SKILL_POINT )
					return;

				m_bVisible			= FALSE;
				m_nActiveIndex		= -1;
			}
	
			float CharHp	= _pNetwork->MyCharacterInfo.hp;
			float CharMaxHp = _pNetwork->MyCharacterInfo.maxHP;

			float hp = ( CharHp / CharMaxHp ) * 100.0f;

			if ( hp <= 40 ) // ü���� 40% �����̸�...
			{
				if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
				{
					ShowAutoHelp ( dwAutoHelpInfo );
				}
				break;
			}
		}
		break;

	case AU_ITEM_DROP :
		{
			if ( m_bVisible )
			{
				if ( m_nActiveIndex != AU_GET_SKILL_POINT )
					return;
	
				// �ʱ�ȭ
				m_bVisible			= FALSE;
				m_nActiveIndex		= -1;
		
			}

			if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
			{
				ShowAutoHelp ( dwAutoHelpInfo );
			}

			m_bCondition[dwAutoHelpInfo] = TRUE;


		}
		break;
	case AU_GET_SKILL_POINT :
		{
			if ( m_bVisible ) return;
			if ( !m_AutoHelpInfo[dwAutoHelpInfo].m_bActive )
			{
				m_AutoHelpInfo[dwAutoHelpInfo].m_bActive = TRUE;
				return ;
			}
		}
		break;	
	// �� �ۿ� ��Ȳ ������ �� ��ġ�� �͵� ...
	default :
		{
			if ( m_bVisible && !m_RndHelp )
			{
				return;	
			}
			if ( m_AutoHelpInfo[dwAutoHelpInfo].CheckInfo() )
			{
				ShowAutoHelp ( dwAutoHelpInfo );
			}

			//m_bCondition[dwAutoHelpInfo] = TRUE;
		}
		break;
	}
}

// ----------------------------------------------------------------------------
// Name : ShowAutoHelp()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::ShowAutoHelp ( int nIndex )
{
	if ( nIndex < 0 || nIndex >= AUTOHELP_COUNT ) 
	{
		return;
	}

	// ������ �ʱ�ȭ
	m_bVisible		= TRUE;
	m_nActiveIndex	= nIndex;
	m_fShowTime		= AUTOHELO_SHOW_TIME;

	int			nWidth = ( m_AutoHelpInfo[m_nActiveIndex].m_strMessage.Length() + 6 ) *
							( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int			nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	// wooss 051019
	// for Thai
	int tv_num = 0;

#if defined(G_THAI)
		nWidth=FindThaiLen(m_AutoHelpInfo[m_nActiveIndex].m_strMessage);
#endif

	m_rcNotice.Left = nCX - nWidth / 2;
	m_rcNotice.Right = m_rcNotice.Left + nWidth;

	

	m_tmStartTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

}

// ----------------------------------------------------------------------------
// Name : Render()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::Render ()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	CDrawPort* pDrawPort = pUIManager->GetDrawPort();
	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

	if( STAGEMGR()->GetCurStage() != eSTAGE_GAMEPLAY ) return;


	if ( !m_bVisible )
	{
		static int interval = m_nInterval_time + RND_HELP_TIME;
		static bool bFirst = false;
		static bool bTemp = false;

		TIME NowTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();

		
#if defined(G_THAI)
			
			int tv_rnd;
			__int64 tv_time = llCurTime - m_tmCheckTime;
			INDEX tv_tmp ;
			time((time_t*)&tv_tmp);
			tm* tv_tm = localtime((time_t*)&tv_tmp);
			if(tv_tm->tm_hour == 3 && tv_tm->tm_min == 55 && tv_tm->tm_sec == 0) {
				tv_rnd = ( rand()% (AU_NO_TOXICOSIS_VACATION_E - AU_NO_TOXICOSIS_VACATION_S + 1) ) + AU_NO_TOXICOSIS_VACATION_S;
				SetInfo ( tv_rnd );
			} else {
				if(tv_time/(60*60*1000) != 0){
					if((tv_time/(60*60*1000))%(m_iNoticeTime) == 0 ) {
					m_iNoticeTime +=3;
					tv_rnd = ( rand()% (AU_NO_TOXICOSIS_E - AU_NO_TOXICOSIS_S + 1) ) + AU_NO_TOXICOSIS_S;
					SetInfo ( tv_rnd );
					}
				}
			}
		 
#endif

		if (IsBila(g_iCountry) == TRUE)
		{
			int tv_rnd;
			tv_rnd = (rand()%(AU_NO_TOXICOSIS_VACATION_E - AU_NO_TOXICOSIS_S + 1) ) + AU_NO_TOXICOSIS_S;
			SetInfo ( tv_rnd );
		}

		if ( NowTime - m_tmEndTime > interval )
		{
			if ( bFirst == false )
			{
				m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
				bFirst = true;
				return;
			}


			m_RndHelp = TRUE;
			int rnd;
			
			{
				rnd = ( rand()% (AU_RANDOM_HELP_END - AU_RANDOM_HELP_START - 3 ) ) + AU_RANDOM_HELP_START;
			}

			SetInfo ( rnd ); // 1�� �̻� �϶� �������ּ���..^^
			
			m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
			m_RndHelp = FALSE;
		}
		return;
	}
		

	__int64	llCurDelay;

	llCurDelay = llCurTime - m_tmStartTime;

	if( llCurDelay < AUTOHELO_SHOW_TIME )
	{
		COLOR	colBackground = 0x000000FF;
		// tutorial renewal : ���� �ý��� �÷� ����. [8/3/2010 rumist]
		COLOR	colText = 0xFFFF00FF;
		
		if( llCurDelay > CHATMSG_NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( CHATMSG_NOTICE_DELAY - llCurDelay ) / (FLOAT)CHATMSG_NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;

			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}
	
	
		// Set texture
		pDrawPort->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		// Background
		pDrawPort->AddTexture( m_rcNotice.Left - 32, m_rcNotice.Top,
											m_rcNotice.Left, m_rcNotice.Bottom,
											m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
											colBackground );

		pDrawPort->AddTexture( m_rcNotice.Left, m_rcNotice.Top,
											m_rcNotice.Right, m_rcNotice.Bottom,
											m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
											colBackground );

		pDrawPort->AddTexture( m_rcNotice.Right, m_rcNotice.Top,
											m_rcNotice.Right + 32, m_rcNotice.Bottom,
											m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
											colBackground );

		// Render all elements
		pDrawPort->FlushRenderingQueue();

		// ���� �յڷ� ǥ���ϴ� ����� ���� ���� �ٸ��� ���� �սô�~

		CTString	strDecoration = "";
		
		CTString strTemp = strDecoration + " ";
		strTemp += m_AutoHelpInfo[m_nActiveIndex].m_strMessage + " ";
		strTemp += strDecoration;
		
		// Text in notice region
		pDrawPort->PutTextEx( strTemp, m_rcNotice.Left, m_rcNotice.Top + 4, colText );

		// Flush all render text queue
		pDrawPort->EndTextEx();
	}
	else
	{
		m_bVisible			= FALSE;
		m_nActiveIndex		= -1;

		m_tmEndTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	
	}
}

// ----------------------------------------------------------------------------
// Name : SetGMNotice()
// Desc :
// ----------------------------------------------------------------------------
void CUIAutoHelp::SetGMNotice ( CTString strGMNotice, COLOR colText )
{
	m_strGMNotice = strGMNotice;
	m_colGMTextColor = colText;

	// ������ �ʱ�ȭ
	m_bShowGMNotice	= TRUE;
	
	int			nWidth;
	
#if defined(G_THAI)
	
	nWidth = FindThaiLen(m_strGMNotice);

#else

	nWidth = m_strGMNotice.Length() *
			( _pUIFontTexMgr->GetFontWidth() + _pUIFontTexMgr->GetFontSpacing() ) - 1;	
#endif

	CDrawPort* pDrawPort = CUIManager::getSingleton()->GetDrawPort();
	int			nCX = pDrawPort->dp_MinI + ( pDrawPort->dp_MaxI - pDrawPort->dp_MinI ) / 2;

	m_rcGMNotice.Left = nCX - nWidth / 2;
	m_rcGMNotice.Right = m_rcGMNotice.Left + nWidth;

	m_tmGMNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds ();

#ifdef	IMPROV1107_NOTICESYSTEM
	SGMNotice		sNotice;

	sNotice.strGMNotice			= strGMNotice;
	sNotice.colGMTextColor		= colText;
	sNotice.i64GMFadeTime = sNotice.i64GMNoticeTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	sNotice.bCompleteVisible	= FALSE;
	sNotice.rcBackground.Left	= m_rcGMNotice.Left;
	sNotice.rcBackground.Right	= m_rcGMNotice.Right;

	m_vGMNotice.push_back(sNotice);

	if(m_vGMNotice.size() > 1)
	{
		m_bmovGMNotice		= TRUE;
		m_i64movGMNotice	= _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	}
#endif
}

// ----------------------------------------------------------------------------
// Name : RenderGMNotice()
// Desc : ��� ���� ǥ�� ����
// ----------------------------------------------------------------------------
void CUIAutoHelp::ClearGMNNotice()
{
#ifdef	IMPROV1107_NOTICESYSTEM
	m_vGMNotice.clear();
	m_bmovGMNotice		= FALSE;
#endif
}

// ----------------------------------------------------------------------------
// Name : RenderGMNotice()
// Desc : ��� ����
// ----------------------------------------------------------------------------
void CUIAutoHelp::RenderGMNotice ()
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	UtilHelp* pUtil = UtilHelp::getSingleton();

	if( m_bShowGMNotice == FALSE )
		return;

	__int64	llCurTime = _pTimer->GetHighPrecisionTimer().GetMilliseconds();
	__int64	llCurDelay = llCurTime - m_tmGMNoticeTime;
#ifdef	IMPROV1107_NOTICESYSTEM

	if(m_vGMNotice.size() > 0)
	{
		float	fposYPitchNotice	= 0.0f;

		if(m_bmovGMNotice)
		{
			__int64 llCurMoveDelady	= llCurTime - m_i64movGMNotice;

			if(llCurMoveDelady >= CHATMSG_NOTICE_MOVEPITCHTIME)
			{
				m_bmovGMNotice	= FALSE;
			}

			float	fMoveRatio	= 1.0f - ((float)llCurMoveDelady / (float)CHATMSG_NOTICE_MOVEPITCHTIME);
			if(fMoveRatio < 0.0f)
				fMoveRatio		= 0.0f;
			fposYPitchNotice	= (float)(m_rcGMNotice.Bottom - m_rcGMNotice.Top) * fMoveRatio;
		}

		// ���� �׸���
		int		iNoticeSize	= m_vGMNotice.size();
		std::vector<SGMNotice>::iterator	itNotice	= m_vGMNotice.end() - 1;
		for(int i = 0; i < iNoticeSize; i++)
		{
			SGMNotice&	sNotice			= (*itNotice);
			int			iFadeType		= 0;		// -1 : FadeIN(��Ÿ��) 0 : ���̵���� 1 : FadeOUT(�����)
			float		fAlphaRatio		= 1.0f;		// 1 = ALPHA 100%, 0 = 0% (FADE)
			COLOR		colBackground	= 0xFFFFFFFF;
			COLOR		colText			= sNotice.colGMTextColor;
			int			iNoticeHeight	= m_rcGMNotice.Bottom - m_rcGMNotice.Top;
			float		fYNotice		= m_rcGMNotice.Top;

			if(i > CHATMSG_NOTICE_VIEWCOUNT)
			{
				if(m_vGMNotice.end() != itNotice)
					itNotice = m_vGMNotice.erase(itNotice);

				continue;
			}
			else if(i == CHATMSG_NOTICE_VIEWCOUNT)
			{	// ��� ������ �Ѿ����Ƿ� ���� FADEOUT��Ű�� ����
				sNotice.i64GMNoticeTime		-= CHATMSG_NOTICE_DELAY;
			}

			if(i > 0)
			{	// ù��° ���� (���� �ϴ�) �� �ƴ� ��� fposYPitchNotice ���� ����޴´�. (���� �̵��Ѵ�)
				fYNotice	= m_rcGMNotice.Top - (i * iNoticeHeight);
				fYNotice	+= fposYPitchNotice;
			}
			
			if(llCurTime - sNotice.i64GMNoticeTime > CHATMSG_NOTICE_DELAY)
			{	// ����� �ð��� �ٵǾ���.
				if(sNotice.bCompleteVisible)
				{
					sNotice.i64GMFadeTime		= llCurTime;		// FadeTime �缳��
					sNotice.bCompleteVisible	= FALSE;
				}

				iFadeType		= 1;
			}
			else
			{
				if(!sNotice.bCompleteVisible)		// ���� �������� ���� ���´� ������ FadeIN ����
				{
					iFadeType		= -1;
				}
			}

			if(iFadeType == -1)
			{	// FadeIN (��Ÿ���� ȿ��)
				fAlphaRatio			= (float)(llCurTime - sNotice.i64GMFadeTime) / (float)CHATMSG_NOTICE_FADETIME;
				if(fAlphaRatio >= 1.0f)
				{
					fAlphaRatio					= 1.0f;
					sNotice.bCompleteVisible	= TRUE;
				}
			}
			else if(iFadeType == 1)
			{
				//FadeOUT (������� ȿ��)
				fAlphaRatio			-= (float)(llCurTime - sNotice.i64GMFadeTime) / (float)CHATMSG_NOTICE_FADETIME;
				if(fAlphaRatio <= 0.0f)
				{
					fAlphaRatio		= 0.0f;
					itNotice = m_vGMNotice.erase(itNotice);

					if(itNotice == m_vGMNotice.begin())
						break;
				}
			}

			if(fAlphaRatio > 0.0f)
			{
				// TEXT (���İ� ������ �ʾ� ��ä�� ��, ���� ���İ��� �������� �ؽ�Ʈ�� �˾Ƽ� ����.)
				if (pUtil != NULL)
				{
					colText			= pUtil->GetColorAlpha(fAlphaRatio, colText);
					colText			= pUtil->GetColorContrast(fAlphaRatio, colText);
					// BACKGROUND
					colBackground	= pUtil->GetColorAlpha(fAlphaRatio, colBackground);
				}
				// Set texture
				pUIManager->GetDrawPort()->InitTextureData( m_ptdBaseTexture );
				// Background
				pUIManager->GetDrawPort()->AddTexture( sNotice.rcBackground.Left - 32, fYNotice,
													sNotice.rcBackground.Left, fYNotice + iNoticeHeight,
													m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
													colBackground );

				pUIManager->GetDrawPort()->AddTexture( sNotice.rcBackground.Left, fYNotice,
													sNotice.rcBackground.Right, fYNotice + iNoticeHeight,
													m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
													colBackground );

				pUIManager->GetDrawPort()->AddTexture( sNotice.rcBackground.Right, fYNotice,
													sNotice.rcBackground.Right + 32, fYNotice + iNoticeHeight,
													m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
													colBackground );

				// Render all elements
				pUIManager->GetDrawPort()->FlushRenderingQueue();

				// Text in notice region
				pUIManager->GetDrawPort()->PutTextEx( sNotice.strGMNotice, sNotice.rcBackground.Left, fYNotice + 4, colText );

				// Flush all render text queue
				pUIManager->GetDrawPort()->EndTextEx();
			}

			if(itNotice == m_vGMNotice.begin())
				break;

			itNotice--;
		}
	}
	else if(m_vGMNotice.size() <= 0)
		m_bShowGMNotice = FALSE;

#else	// #ifdef	IMPROV1107_NOTICESYSTEM
	if( llCurDelay < CHATMSG_NOTICE_DELAY )
	{
		COLOR	colBackground = 0xFFFFFFFF;
		COLOR	colText = m_colGMTextColor;
		if( llCurDelay > CHATMSG_NOTICE_FADEOUT )
		{
			FLOAT	fFadeRatio = (FLOAT)( CHATMSG_NOTICE_DELAY - llCurDelay ) / (FLOAT)CHATMSG_NOTICE_FADETIME;
			COLOR	colBlend = 0xFF * fFadeRatio;

			colBackground &= 0xFFFFFF00;
			colBackground |= colBlend;
			colText &= 0xFFFFFF00;
			colText |= colBlend;
		}

		// Set texture
		pUIManager->GetDrawPort()->InitTextureData( m_ptdBaseTexture );

		// Add render regions
		// Background
		pUIManager->GetDrawPort()->AddTexture( m_rcGMNotice.Left - 32, m_rcGMNotice.Top,
											m_rcGMNotice.Left, m_rcGMNotice.Bottom,
											m_rtNoticeL.U0, m_rtNoticeL.V0, m_rtNoticeL.U1, m_rtNoticeL.V1,
											colBackground );

		pUIManager->GetDrawPort()->AddTexture( m_rcGMNotice.Left, m_rcGMNotice.Top,
											m_rcGMNotice.Right, m_rcGMNotice.Bottom,
											m_rtNoticeC.U0, m_rtNoticeC.V0, m_rtNoticeC.U1, m_rtNoticeC.V1,
											colBackground );

		pUIManager->GetDrawPort()->AddTexture( m_rcGMNotice.Right, m_rcGMNotice.Top,
											m_rcGMNotice.Right + 32, m_rcGMNotice.Bottom,
											m_rtNoticeR.U0, m_rtNoticeR.V0, m_rtNoticeR.U1, m_rtNoticeR.V1,
											colBackground );

		// Render all elements
		pUIManager->GetDrawPort()->FlushRenderingQueue();

		// Text in notice region
		pUIManager->GetDrawPort()->PutTextEx( m_strGMNotice, m_rcGMNotice.Left, m_rcGMNotice.Top + 4, colText );

		// Flush all render text queue
		pUIManager->GetDrawPort()->EndTextEx();
	}
	else
		m_bShowGMNotice = FALSE;
#endif	// #ifdef	IMPROV1107_NOTICESYSTEM

}

void CUIAutoHelp::initialize()
{
// 	int			i;
// 	CUIImage*	pImg = NULL;
// 
// 	const char* str_notice[] = {
// 		"notice_l",
// 		"notice_c",
// 		"notice_r",
// 	};
// 
// 	for( i = 0; i < 3; ++i )
// 	{
// 		pImg = (CUIImage*)findUI( str_notice[i] );
// 
// 		if( pImg )
// 		{
// 			addChild( pImg );
// 		}
// 	}

}

void CUIAutoHelp::GetLvGuidString( int lv )
{
	if (CLevelUpGuide::getData(lv) == NULL)
		return;

	CTString strTmp = CUIBase::getText(CLevelUpGuide::getData(lv)->strIndex);

	if (strTmp.IsEmpty() == TRUE)
		return;

	CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strTmp, SYSMSG_USER);
}

void CUIAutoHelp::LearnTheSkill( int lv )
{
	int nCnt = 0;
	
	int nJob = _pNetwork->MyCharacterInfo.job;
	int _2ndJob = _pNetwork->MyCharacterInfo.job2;
	int _tmp2ndJob = 0;
	int nIndex = 0;

	nCnt = CSkillTree::m_nSkillCount[nJob][0];

	if (_2ndJob > 0)
		nCnt += CSkillTree::m_nSkillCount[nJob][_2ndJob];

	for (int i = 0; i < nCnt; ++i)
	{
		if (i > CSkillTree::m_nSkillCount[nJob][0])
			_tmp2ndJob = _2ndJob;

		nIndex = CSkillTree::m_SkillTree[nJob][_tmp2ndJob][i/8].index[i%8];

		CSkill& rSkill = _pNetwork->GetSkillData(nIndex);

		for (int j = 0; j < rSkill.GetMaxLevel(); ++j)
		{
			if (rSkill.GetLearnLevel(j) == lv)
			{
				CTString strTmp;
				strTmp.PrintF(_S(6213, "���ο� ��ų [%s]�� %d������ ��� �� �ֽ��ϴ�."), rSkill.GetName(), j + 1);

				CUIManager::getSingleton()->GetChattingUI()->AddSysMessage( strTmp, SYSMSG_USER);
				break;
			}
		}
	}
}

void CUIAutoHelp::SetStatpoint( int old, int cur )
{
	m_nUpStat = 0;

	if (old >= cur)
		return;

	m_nUpStat = cur - old;
}
