#include "StdH.h"
#include <Engine/Base/FileName.h>
#include <Engine/Base/Stream.h>

#include "QuestSystem.h"
#include <Engine/Ska/Render.h>
#include <Engine/Base/ErrorReporting.h>
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/JobInfo.h>
#include <Engine/World/World.h>
#include <Engine/Interface/UIInventory.h>

//[sora] �̹��� ��Ʈ�� index ǥ��
void CQuestStaticData::SetNoTranslate()
{
	char buff[MAX_PATH];
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
	{
		sprintf( buff, "[%d] : quest name", index );
		m_szName = buff;
	}
	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
	{
		sprintf( buff, "[%d] : quest name", index );
		m_szDesc = buff;
	}
	if( pUIManager->IsNotTranslated( TRANS_DESC2, transFlag ) )
	{
		sprintf( buff, "[%d] : quest name", index );
		m_szDesc2 = buff;
	}
	if( pUIManager->IsNotTranslated( TRANS_DESC3, transFlag ) )
	{
		sprintf( buff, "[%d] : quest name", index );
		m_szDesc3 = buff;
	}
}

void CQuestStaticData::ClearNoTranslate()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if( pUIManager->IsNotTranslated( TRANS_NAME, transFlag ) )
		m_szName = "";
	if( pUIManager->IsNotTranslated( TRANS_DESC, transFlag ) )
		m_szDesc = "";
	if( pUIManager->IsNotTranslated( TRANS_DESC2, transFlag ) )
		m_szDesc2 = "";
	if( pUIManager->IsNotTranslated( TRANS_DESC3, transFlag ) )
		m_szDesc3 = "";
}

bool CQuestStaticData::loadEx( const char* fileName )
{
	FILE*	fp = NULL;

	fp = fopen(fileName, "rb");

	if (fp == NULL)
		return false;

	fread(&_nSize, sizeof(int), 1, fp);

	if (_nSize <= 0)
	{
		fclose(fp);
		return false;
	}

	stQuest* pdata = new stQuest[_nSize];
	fread(pdata, sizeof(stQuest) * _nSize, 1, fp);
	fclose(fp);

	for (int i = 0; i < _nSize; i++)
	{
		CQuestStaticData* ptmp = new CQuestStaticData;
		memcpy(ptmp, &pdata[i], sizeof(stQuest));
		if (_mapdata.insert(std::make_pair(ptmp->getindex(), ptmp)).second == false)
		{
			delete ptmp;
			ptmp = NULL;
		}
	}

	m_dummy = new CQuestStaticData; // ���̵���Ÿ ����
	memset(m_dummy, 0, sizeof(stQuest));

	if (pdata != NULL)
	{
		delete[] pdata;
		pdata = NULL;
	}

	return true;
}

// ------------- CQuestDynamicData ------------- 
CQuestDynamicData::CQuestDynamicData(CQuestStaticData* data)
: m_pStaticData(data)
, m_bQuestComplete( FALSE )
, m_iQuestIndex( data->index )
, m_ctTitleDesc( 0 )
, m_ctStatusDesc( 0 )
, m_ctPrizeDesc( 0 )
, m_ctOptionPrizeDesc( 0 )
, m_ctNeedDesc( 0 )
{
	INDEX	i;
	for( i = 0; i < QUEST_MAX_CONDITION; ++i )
	{
		//m_iConditionValue[i] = conditionValue[i];
		m_iCurrentConditionValueStatus[i] = 0;
	}
	
	m_bPrize = FALSE;
	for( i = 0; i < QUEST_MAX_PRIZE; ++i )
	{
		m_bPrize = m_bPrize || (m_pStaticData->prizeType[i] != -1);
		m_iPrizeType[i] = m_pStaticData->prizeType[i];
		m_iPrizeIndex[i] = FindItemForPrize(m_iPrizeType[i], m_pStaticData->prizeIndex[i]);
		m_iPrizeData[i] = m_pStaticData->prizeData[i];
	}

	// [090728: selo] �ɼ� ������ ���� ������ ������ (������ �޶�����)
	m_iOptionPrizeCount = QUEST_MAX_OPTPRIZE;

	for( i = 0; i < m_iOptionPrizeCount; ++i )
	{
		m_bOptionPrize = m_pStaticData->optionPrize;
		m_iOptionPrizeType[i] = m_pStaticData->optPrizeType[i];
		
		// [090727: selo] ����Ʈ���� ���� Only üũ�ڽ��� üũ�ϸ�
		// ������ ������� �ش� ������ �״�� �����Ѵ�.
		if( m_pStaticData->onlyOptPrize )
		{
			m_iOptionPrizeIndex[i] = m_pStaticData->optPrizeIndex[i];
		}
		else
		{
			m_iOptionPrizeIndex[i] = FindItemForPrize(m_iOptionPrizeType[i], m_pStaticData->optPrizeIndex[i]);
		}		
		m_iOptionPrizeData[i] = m_pStaticData->optPrizeData[i];
		m_iOptionPrizePlus[i] = m_pStaticData->optPrizePlus[i];
	}
	
	m_strIntroDesc = "";
	for( i = 0; i < QUEST_TITLE_DESC; i++ )
		m_strTitleDesc[i] = "";
	for( i = 0; i < QUEST_STATUS_DESC; i++ )
		m_strStatusDesc[i] = "";
	for( i = 0; i < QUEST_PRIZE_DESC; i++ )
		m_strPrizeDesc[i] = "";
	for( i = 0; i < QUEST_OPTION_PRIZE_DESC; i++ )
		m_strOptionPrizeDesc[i] = "";
	for( i = 0; i < QUEST_NEED_DESC; i++ )
		m_strNeedDesc[i] = "";
	
	if (m_pStaticData->index > 0)
		MakeQuestFirstDesc();
}

CQuestDynamicData::~CQuestDynamicData()
{
}

INDEX CQuestDynamicData::FindItemForPrize(INDEX type, INDEX index)
{
	// ����Ʈ ���� �������� �������� ������
	if(type != QPRIZE_ITEM) return index;

	CItemData*	pItemData = _pNetwork->GetItemData( index );

	if (pItemData == NULL)
		return index;

	int max = CItemData::getsize();
	
	if (pItemData->GetType() == CItemData::ITEM_SHIELD &&
		pItemData->GetSubType() != CItemData::ITEM_SHIELD_SHIELD)
	{
		for (int i = 0; i < max; ++i)
		{
			CItemData*	ptempItem = _pNetwork->GetItemData(i);

			if (ptempItem == NULL)
				continue;

			if (ptempItem->GetType() == pItemData->GetType() &&
				ptempItem->GetSubType() == pItemData->GetSubType() &&
				ptempItem->GetLevel() == pItemData->GetLevel() &&
				ptempItem->CanUse(_pNetwork->MyCharacterInfo.job))
			{
				return i;
			}
		}
	}

	return index;
}

void CQuestDynamicData::SetCurrentConditionValueStatus(INDEX current[QUEST_MAX_CONDITION])
{
	for( int i = 0; i < QUEST_MAX_CONDITION; ++i )
		m_iCurrentConditionValueStatus[i] = current[i];
	
	MakeQuestStatusDesc();
}

void CQuestDynamicData::MakeQuestFirstDesc()
{
	MakeQuestTitleDesc();
	MakeQuestStatusDesc();
	MakeQuestPrizeDesc();
	MakeQuestNeedDesc();
	MakeQuestOptionPrizeDesc();
}

void CQuestDynamicData::MakeQuestTitleDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctTitleDesc; i++ )
		m_strTitleDesc[i].Clear();
	m_ctTitleDesc = 0;
	
	// Introduction desc
	m_strIntroDesc.PrintF( "%s\n\n\n", GetDesc() );
	m_colIntroDesc = 0xA3A1A3FF;
	
	// Title desc
	m_strTitleDesc[m_ctTitleDesc].PrintF( "%s", GetName() );
	m_colTitleDesc[m_ctTitleDesc++] = 0xF2F2F2FF;
	
	CTString	strTemp;
	switch( GetQuestType1() )
	{
	case QTYPE_KIND_COLLECTION:		// ������ ����Ʈ
		{
			// Title desc
			INDEX	ctCond = 0;
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_ITEM
					||  GetConditionType( iCond ) == QCONDITION_ITEM_NORMAL);
				
				const char	*szItemName = _pNetwork->GetItemName( GetConditionIndex( iCond ) );
				if( ctCond == 0 )
				{
					m_strTitleDesc[m_ctTitleDesc].PrintF( _S2( 49, szItemName, ", %s<��> %d����" ),
						szItemName, GetConditionNum( iCond ) );
					ctCond++;
				}
				else
				{
					strTemp.PrintF( _S2( 50, szItemName, ", %s<��> %d��" ),
						szItemName, GetConditionNum( iCond ) );
					m_strTitleDesc[m_ctTitleDesc] += strTemp;
					ctCond++;
				}
			}
			
			if( ctCond > 0 )
			{
				m_strTitleDesc[m_ctTitleDesc] += _S( 51, " ��ƿͶ�\n\n" );
				m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			}
		}
		break;
		
	case QTYPE_KIND_DELIVERY:		// ����� ����Ʈ
		{
			// Title desc
			INDEX	ctCond = 0;
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_ITEM
					||  GetConditionType( iCond ) == QCONDITION_ITEM_NORMAL );
				
				const char	*szItemName = _pNetwork->GetItemName( GetConditionIndex( iCond ) );
				if( ctCond == 0 )
				{
					m_strTitleDesc[m_ctTitleDesc].PrintF( _S( 460, "%s %d�� " ),	
						szItemName, GetConditionNum( iCond ) );
					ctCond++;
				}
				else
				{
					strTemp.PrintF( _S( 461 , ", %s %d�� " ),
						szItemName, GetConditionNum( iCond ) );
					m_strTitleDesc[m_ctTitleDesc] += strTemp;
					ctCond++;
				}
			}
			
			if( ctCond > 0 && GetPrizeNPCIndex() != -1)
			{
				const char	*szMobName = CMobData::getData( GetPrizeNPCIndex() )->GetName();
				strTemp.PrintF( _S( 462, "�� %s���� �����϶�.\n" ), szMobName);			
				
				m_strTitleDesc[m_ctTitleDesc] += strTemp;
				m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			}
		}
		break;
		
	case QTYPE_KIND_REPEAT:			// �ݺ� ����Ʈ
		{
			// Desc
			INDEX	ctCond = 0;
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				const char	*szMobName = CMobData::getData( GetConditionIndex( iCond ) )->GetName();
				if( ctCond == 0 )
				{
					m_strTitleDesc[m_ctTitleDesc].PrintF( _S2( 52, szMobName, "%s<��> %d����" ),
						szMobName, GetConditionNum( iCond ) );
					ctCond++;
				}
				else
				{
					strTemp.PrintF( _S2( 53, szMobName, ", %s<��> %d����" ),
						szMobName, GetConditionNum( iCond ) );
					m_strTitleDesc[m_ctTitleDesc] += strTemp;
					ctCond++;
				}
			}
			
			if( ctCond > 0 )
			{
				m_strTitleDesc[m_ctTitleDesc] += _S( 54, " �����϶�\n\n" );
				m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
			}
		}
		break;
	case QTYPE_KIND_SAVE:		// ������ ����Ʈ ����.
		{
			m_strTitleDesc[m_ctTitleDesc] += _S( 463 , "���� ���ָ� �����϶�.\n" );		
			m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
		}
		break;
	case QTYPE_KIND_DEFEAT:		// �̱۴��� ����Ʈ ����.
		{
			INDEX	ctCond = 0;
			m_strTitleDesc[m_ctTitleDesc] += _S( 350, "�߷��� �����϶�\n\n" );
			m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
		}
		break;
	}
	
	// Need item
	BOOL	bNeedItem = FALSE;
	for( INDEX iNeed = 0; iNeed < MAX_MAX_NEED_ITEM; iNeed++ )
	{
		if( GetNeedItemIndex( iNeed ) != -1 )
			bNeedItem = TRUE;
	}
	
	if( bNeedItem )
	{
		m_strTitleDesc[m_ctTitleDesc] = _S( 351, "�ʿ� ������" );
		m_colTitleDesc[m_ctTitleDesc++] = 0xF2F2F2FF;
		
		INDEX	ctNeed = 0;
		for( INDEX iNeed = 0; iNeed < MAX_MAX_NEED_ITEM; iNeed++ )
		{
			if( GetNeedItemIndex( iNeed ) == -1 )
				continue;
			
			const char* szItemName = _pNetwork->GetItemName( GetNeedItemIndex( iNeed ) );
			if( ctNeed == 0 )
			{
				m_strTitleDesc[m_ctTitleDesc].PrintF( _S( 352, "%s %d��\n" ),
					szItemName,
					GetNeedItemCount( iNeed ) );
				ctNeed++;
			}
			else
			{
				strTemp.PrintF( _S( 352, "%s %d��\n" ),
					szItemName,
					GetNeedItemCount( iNeed ) );
				m_strTitleDesc[m_ctTitleDesc] += strTemp;
				ctNeed++;
			}
		}
		
		m_strTitleDesc[m_ctTitleDesc] += "\n";
		m_colTitleDesc[m_ctTitleDesc++] = 0x80CC17FF;
	}
}

void CQuestDynamicData::MakeQuestStatusDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctStatusDesc; i++ )
		m_strStatusDesc[i].Clear();
	m_ctStatusDesc = 0;
	
	// Status desc
	switch( GetQuestType1() )
	{
	case QTYPE_KIND_COLLECTION:		// ������ ����Ʈ
	case QTYPE_KIND_DELIVERY:		// ����� ����Ʈ
		{
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;
				
				ASSERT( GetConditionType( iCond ) == QCONDITION_ITEM
					||  GetConditionType( iCond ) == QCONDITION_ITEM_NORMAL );
				const char *szItemName = _pNetwork->GetItemName( GetConditionIndex( iCond ) );
				
				m_strStatusDesc[m_ctStatusDesc].PrintF( _S( 58, "%s ����" ),
					szItemName );
				m_colStatusDesc[m_ctStatusDesc++] = 0xC0C0C0FF;
				m_strStatusDesc[m_ctStatusDesc].PrintF( "%d / %d", GetCurrentConditionValueStatus( iCond ),
					GetConditionNum( iCond ) );
				m_colStatusDesc[m_ctStatusDesc++] = 0x80CC17FF;
			}
			
			if( m_ctStatusDesc > 0 )
				m_strStatusDesc[m_ctStatusDesc - 1] += "\n\n";
		}
		break;
		
	case QTYPE_KIND_REPEAT:			// �ݺ� ����Ʈ
		{
			for( INDEX iCond = 0; iCond < QUEST_MAX_CONDITION; iCond++ )
			{
				if( GetConditionType( iCond ) == -1 )
					continue;

				m_strStatusDesc[m_ctStatusDesc].PrintF( _S( 59, "%s ���� ��" ),

				CMobData::getData(GetConditionIndex( iCond ) )->GetName());
				m_colStatusDesc[m_ctStatusDesc++] = 0xC0C0C0FF;

				m_strStatusDesc[m_ctStatusDesc].PrintF( "%d / %d", GetCurrentConditionValueStatus( iCond ),
					GetConditionNum( iCond ) );
				m_colStatusDesc[m_ctStatusDesc++] = 0x80CC17FF;
			}
			
			if( m_ctStatusDesc > 0 )
				m_strStatusDesc[m_ctStatusDesc - 1] += "\n\n";
		}
		break;
	case QTYPE_KIND_SAVE:			// ���� ����Ʈ
		{
		}
		break;
	case QTYPE_KIND_DEFEAT:			// ���� ����Ʈ
		{
		}
		break;
	}
}

void CQuestDynamicData::MakeQuestPrizeDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctPrizeDesc; i++ )
		m_strPrizeDesc[i].Clear();
	m_ctPrizeDesc = 0;
	
	m_strPrizeDesc[m_ctPrizeDesc] = _S( 60, "����Ʈ ����" );
	m_colPrizeDesc[m_ctPrizeDesc++] = 0xF2F2F2FF;
	
	for( INDEX iPrize = 0; iPrize < QUEST_MAX_PRIZE; iPrize++ )
	{
		switch( GetPrizeType( iPrize ) )
		{
		case QPRIZE_ITEM:
			{
				INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( _S( 61, "%s %d��" ),
					_pNetwork->GetItemName( iPrizeIndex ), GetPrizeData( iPrize ) );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		case QPRIZE_MONEY:
			{
				{
					m_strPrizeDesc[m_ctPrizeDesc].PrintF( _S( 836, "%d ����" ), GetPrizeData( iPrize ) );
				}
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
			
		case QPRIZE_EXP:
			{
				CTString strTemp;
				strTemp.PrintF( "%s %I64u", _S( 89, "����ġ"), GetPrizeData( iPrize ) );
				m_strPrizeDesc[m_ctPrizeDesc] = strTemp;
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			} break;
			
		case QPRIZE_SP:
			{
#if defined (G_GERMAN)
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s %d", _S( 90, "���õ�" ), GetPrizeData( iPrize ) );
#else	// else about japan, german, europe3, europe2, netherlands.
				// support russia string [9/7/2010 rumist]
#if defined (G_RUSSIA)
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s %d", _S( 4415, "SP" ), GetPrizeData( iPrize ) );
#else	// else about russia
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "SP %d", GetPrizeData( iPrize ) );
#endif	// end russia
#endif	//end japan, german, europe3, europe2, netherlands.
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			} break;
		case QPRIZE_SKILL:
			{
				INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s", _pNetwork->GetSkillData( iPrizeIndex ).GetName() );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		case QPRIZE_SSKILL:
			{
				INDEX		iPrizeIndex = GetPrizeIndex( iPrize );
				CSpecialSkill* pData = CSpecialSkill::getData(iPrizeIndex);

				if (pData == NULL)
					break;

				m_strPrizeDesc[m_ctPrizeDesc].PrintF( "%s", pData->GetName() );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		case QPRIZE_RVR_POINT:
			{
				m_strPrizeDesc[m_ctPrizeDesc].PrintF( _S(6263, "���� ����Ʈ %d"), GetPrizeData( iPrize ) );
				m_colPrizeDesc[m_ctPrizeDesc++] = 0xFFB54DFF;
			}
		default:
			break;
		}
	}
}

void CQuestDynamicData::MakeQuestOptionPrizeDesc()
{
	// Reset strings
	for( INDEX i = 0; i < m_ctOptionPrizeDesc; i++ )
		m_strOptionPrizeDesc[i].Clear();
	m_ctOptionPrizeDesc = 0;
	
	m_strOptionPrizeDesc[m_ctOptionPrizeDesc] = _S( 1654, "����Ʈ ���� ���� (������ 1)" );
	m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xF2F2F2FF;
	
	// [090728: selo] �ɼ� ���� ���� ��ŭ ���� ����
	m_iOptionPrizeCount = QUEST_MAX_OPTPRIZE;

	for( INDEX iPrize = 0; iPrize < m_iOptionPrizeCount; iPrize++ )
	{
		switch( GetOptionPrizeType( iPrize ) )
		{
		case QPRIZE_ITEM:
			{
				INDEX		iPrizeItemIndex = GetOptionPrizeIndex( iPrize );
				if(GetOptionPrizePlus(iPrize) > 0)
				{
					m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 1655, "%s +%d  %d��" ),
						_pNetwork->GetItemName( iPrizeItemIndex ), GetOptionPrizePlus(iPrize), GetOptionPrizeData( iPrize ) );
				}
				else
				{
					if(iPrizeItemIndex == 19)//������ ���(�÷����� ������ �������� �ʴ´�)
					{
						m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 836, "%d ����" ),
							GetOptionPrizeData( iPrize ) );
					}
					else
					{
						m_strOptionPrizeDesc[m_ctOptionPrizeDesc].PrintF( _S( 61, "%s %d��" ),
							_pNetwork->GetItemName( iPrizeItemIndex ), GetOptionPrizeData( iPrize ) );
					}
				}
				m_colOptionPrizeDesc[m_ctOptionPrizeDesc++] = 0xFFB54DFF;
			}
			break;
		default:
			break;
		}
	}
}

void CQuestDynamicData::MakeQuestNeedDesc()
{
	CJobInfo* pInfo = CJobInfo::getSingleton();

	if (pInfo == NULL)
		return;

	// Reset strings
	for( INDEX i = 0; i < m_ctNeedDesc; i++ )
		m_strNeedDesc[i].Clear();
	m_ctNeedDesc = 0;
	
	m_strNeedDesc[m_ctNeedDesc] = _S( 464, "�ʿ� ����" );
	m_colNeedDesc[m_ctNeedDesc++] = 0xF2F2F2FF;
	if (GetNeedMaxLevel() == 999)
		m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 5667, "���� : %d ~ MAX" ), GetNeedMinLevel());
	else
		m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 465, "���� : %d ~ %d" ), GetNeedMinLevel(), GetNeedMaxLevel() );
	m_colNeedDesc[m_ctNeedDesc++] = 0xFFB54DFF;
	const int iNeedJob = GetNeedJob();
	if(iNeedJob != -1)
	{
		m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 466, "���� : %s" ), pInfo->GetName(iNeedJob, 0));
		m_colNeedDesc[m_ctNeedDesc++] = 0xFFB54DFF;
	}
	
	INDEX	ctNeed = 0;
	for( int iNeed = 0; iNeed < MAX_MAX_NEED_ITEM; iNeed++ )
	{
		if( GetNeedItemIndex( iNeed ) == -1 )
			continue;
		
		const char* szItemName = _pNetwork->GetItemName( GetNeedItemIndex( iNeed ) );
		if( ctNeed == 0 )
		{
			m_strNeedDesc[m_ctNeedDesc].PrintF( _S( 352, "%s %d��\n" ),
				szItemName,
				GetNeedItemCount( iNeed ) );
			ctNeed++;
		}
		else
		{
			CTString strTemp;
			strTemp.PrintF( _S( 352, "%s %d��\n" ),
				szItemName,
				GetNeedItemCount( iNeed ) );
			m_strNeedDesc[m_ctNeedDesc] += strTemp;
			ctNeed++;
		}
	}
	
	m_strNeedDesc[m_ctNeedDesc] += "\n";
	m_colNeedDesc[m_ctNeedDesc++] = 0xFFB54DFF;
}

// ------------- CQuestSystem ------------- 
CQuestSystem CQuestSystem::m_instance;

static int test_cnt = 0;
CQuestSystem::CQuestSystem()
: m_pCurrentRequest( NULL )
{
	++test_cnt;
}

CQuestSystem::~CQuestSystem()
{
	--test_cnt;
	delete m_pCurrentRequest;
	for(int i=0; i<m_vectorCurrentUserQuest.size(); ++i)
	{
		delete m_vectorCurrentUserQuest[i];
	}
	
}

void CQuestSystem::ClearAllDynamicData()
{
	for( int i = 0; i < m_vectorCurrentUserQuest.size(); ++i )
	{
		delete m_vectorCurrentUserQuest[i];
	}
	m_vectorCurrentUserQuest.clear();
}

void CQuestSystem::ClearAllAllowList()
{
	for(int i=0; i<m_vectorQuestAllowList.size(); ++i)
	{
		m_vectorQuestAllowList[i] = QAT_ALLOW;
	}
}

CQuestDynamicData *CQuestSystem::GetDynamicDataByQuestIndex(INDEX idxQuest)
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && m_vectorCurrentUserQuest[i]->GetQuestIndex() == idxQuest)
		{
			return m_vectorCurrentUserQuest[i];
		}
	}
	return NULL;
}

INDEX CQuestSystem::GetVectorIndexByGiveNPCIndex( INDEX idxNPC )
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && 
			m_vectorCurrentUserQuest[i]->GetStartType() == QSTART_NPC &&
			m_vectorCurrentUserQuest[i]->GetStartData() == idxNPC)
		{
			return i;
		}
	}
	return -1;
}

INDEX CQuestSystem::GetVectorIndexByPrizeNPCIndex( INDEX idxNPC )
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && m_vectorCurrentUserQuest[i]->GetPrizeNPCIndex() == idxNPC )
		{
			return i;
		}
	}
	return -1;
}

CQuestDynamicData *CQuestSystem::Create(INDEX idxQuest)
{
	if(m_pCurrentRequest) DeleteCurrentRequest();
	m_pCurrentRequest = new CQuestDynamicData(GetStaticData(idxQuest));
	return m_pCurrentRequest;
}

INDEX CQuestSystem::AddCurrentRequest()
{
	m_vectorCurrentUserQuest.push_back(m_pCurrentRequest);
	m_pCurrentRequest = NULL;
	return m_vectorCurrentUserQuest[m_vectorCurrentUserQuest.size() - 1]->GetQuestIndex();
}

void CQuestSystem::DeleteCurrentRequest()
{
	delete m_pCurrentRequest;
	m_pCurrentRequest = NULL;
}

BOOL CQuestSystem::Remove(INDEX idxQuest)
{
	INDEX cnt = m_vectorCurrentUserQuest.size();
	for(INDEX i=0; i<cnt; ++i)
	{
		if(m_vectorCurrentUserQuest[i] != NULL && m_vectorCurrentUserQuest[i]->GetQuestIndex() == idxQuest)
		{
			delete *(m_vectorCurrentUserQuest.begin() + i);
			m_vectorCurrentUserQuest.erase(m_vectorCurrentUserQuest.begin() + i);
			
			return TRUE;
		}
	}
	return FALSE;
}

bool CQuestSystem::Load()
{
	int lastIndex = 0;

	CQuestStaticData::_map::iterator	iter = CQuestStaticData::_mapdata.begin();
	CQuestStaticData::_map::iterator	eiter = CQuestStaticData::_mapdata.end();

	for (;iter != eiter; ++iter)
	{
		CQuestStaticData* pQuest = (*iter).second;

		if (pQuest == NULL)
			continue;

		if(QSTART_NPC == pQuest->startType)
		{
			mmap::value_type insertPairValue(pQuest->startData, pQuest->index);
			m_mmapNPCToQuest.insert(insertPairValue);

			if (pQuest->preQuestNo > 0)
			{
				mmap::value_type insertPairReverseValue(pQuest->preQuestNo, pQuest->index);
				m_mmapPreQuestToCurQuest.insert(insertPairReverseValue);
			}
		}

		lastIndex = pQuest->getindex();
	}
	
	m_vectorQuestAllowList.resize(lastIndex + 1);
	memset(&m_vectorQuestAllowList[0], QAT_ALLOW, sizeof(BOOL)*(lastIndex+1));

	return true;
}

CQuestSystem::eNpcQuestType CQuestSystem::TestNPCForQuest(INDEX iNPCIndex)
{
	///���� �Ϸ��(������ �ȹ���) ��� quest�� ���� npc index�� ���Ѵ�.
	///������ �� �� �ִٸ� NQT_CAN_PRIZE�� ����.

	CMobData* MD = CMobData::getData(iNPCIndex);

	if (MD->IsChannelFlag(_pNetwork->m_iServerCh) == false)
	{
		return NQT_NONE;
	}

	if (_pNetwork->IsRvrZone() && MD->GetSyndicateType() != _pNetwork->MyCharacterInfo.iSyndicateType)
		return NQT_NONE;

	for(INDEX i=0; i<m_vectorCurrentUserQuest.size(); ++i)
	{
		if(m_vectorCurrentUserQuest[i]->IsQuestComplete()
		&& (m_vectorCurrentUserQuest[i]->GetPrizeNPCZoneIndex() == -1
			|| m_vectorCurrentUserQuest[i]->GetPrizeNPCZoneIndex() == g_slZone
			)
		&& m_vectorCurrentUserQuest[i]->GetPrizeNPCIndex() == iNPCIndex)
		{
			return NQT_CAN_PRIZE;
		}
	}

	///�� npc�� �� �� �ִ� ��� quest�� ã�´�.
	mmap::iterator iterLower = m_mmapNPCToQuest.lower_bound(iNPCIndex); 
	mmap::iterator iterUpper = m_mmapNPCToQuest.upper_bound(iNPCIndex);
	if(iterLower == m_mmapNPCToQuest.end()) return NQT_NONE;
	
	///���� ���� ������ �˻��ؼ� ���డ���ϸ� NQT_HAVE_QUEST�� ����.
	//if(iterUpper != m_mmapNPCToQuest.end()) ++iterUpper;
	for(mmap::iterator iter=iterLower; iter!=iterUpper; ++iter)
	{
		INDEX iQuestIndex = (*iter).second;
		///���� ���� ���� ����Ʈ���� Ȯ���Ѵ�.
		///m_vectorQuestAllowList���� �̹� �����ϰų� ������ ����Ʈ���� �˻��Ѵ�.

		// RVR�� ��������Ʈ. ��������Ʈ Ÿ���� ���� ������ �ӽ÷� �ϵ��ڵ�.
		// �� ����Ʈ�� �ϳ��� ���డ�� ���°� �ƴ϶�� NPC���� Q��ũ�� �ѷ����� �ʴ´�.
		if (iQuestIndex == 682 || iQuestIndex == 683)
		{
			if (m_vectorQuestAllowList[ 682 ] != QAT_ALLOW ||
				m_vectorQuestAllowList[ 683 ] != QAT_ALLOW)
				return NQT_NONE;
		}

		if (m_vectorQuestAllowList[ iQuestIndex ] == QAT_ALLOW)
		{
			ASSERT(CQuestStaticData::getData(iQuestIndex)->startType == QSTART_NPC);
			//start quest npc zone �˻�
			if (CQuestStaticData::getData(iQuestIndex)->startNpcZoneNo == -1 || 
				CQuestStaticData::getData(iQuestIndex)->startNpcZoneNo == g_slZone)
			{
				if(CanIDoQuest(iQuestIndex))
				{
					return NQT_HAVE_QUEST;
				}
			}
		}
	}

	///NQT_NONE�� ����
	return NQT_NONE;
}

BOOL CQuestSystem::CanIDoQuest(INDEX iQuestIndex)
{
	if(iQuestIndex == -1) return FALSE;
	CQuestStaticData* Quest = CQuestStaticData::getData(iQuestIndex);

	if (Quest == NULL)
		return FALSE;

	///���� ����Ʈ ���� �˻�
	if(Quest->preQuestNo != 0
	&& m_vectorQuestAllowList[ Quest->preQuestNo ] != QAT_ALREADY_DONE)
	{
		return FALSE;
	}
// 	///����ġ �˻�
// 	if(Quest->m_iNeedMinLevel == _pNetwork->MyCharacterInfo.level
// 	&& Quest->m_iNeedEXP > _pNetwork->MyCharacterInfo.curExp)
// 	{
// 		return FALSE;
// 	}
	///���� �˻�
	if(Quest->needMinLevel > _pNetwork->MyCharacterInfo.level
	|| Quest->needMaxLevel < _pNetwork->MyCharacterInfo.level)
	{
		return FALSE;
	}
	///���� �˻�
	if(Quest->needJob != -1/*ALL*/ && _pNetwork->MyCharacterInfo.job != Quest->needJob)
	{
		return FALSE;
	}
	if ((Quest->needMinPenalty != 0 && Quest->needMaxPenalty != 0)) // min, max penalty�� ��� 0�̸� �˻� ���� �ʴ´�.
	{	// pk ���Ƽ ���� �˻�
		if (_pNetwork->MyCharacterInfo.pkpenalty < Quest->needMinPenalty ||
			_pNetwork->MyCharacterInfo.pkpenalty > Quest->needMaxPenalty)
		{
			return FALSE;
		}
	}

	// ���� Ÿ�� �˻� OR ���� �˻�.
	if (Quest->rvr_type > 0 && Quest->rvr_type > 0)
	{
		if (Quest->rvr_type != _pNetwork->MyCharacterInfo.iSyndicateType || 
			Quest->rvr_grade > _pNetwork->MyCharacterInfo.iSyndicateGrade)
			return FALSE;
	}

	CUIManager* pUIManager = CUIManager::getSingleton();

	///�ʿ� ������ �˻�
	for(int i=0; i<MAX_MAX_NEED_ITEM; ++i)
	{
		if(Quest->needItemIndex[i] != -1
		&& pUIManager->GetInventory()->GetItemCount(Quest->needItemIndex[i]) < Quest->needItemCount[i])
		{
			return FALSE;
		}
	}
	return TRUE;
}

CTString CQuestSystem::MakeInfoForCondition(int iQuestIndex, int iConditionType, int iConditionIndex, int iCurrentCnt, int iConditionCnt)
{
	CTString strRet;
	if(iConditionType == QCONDITION_NPC)
	{
		CMobData* mob = CMobData::getData(iConditionIndex);
		strRet.PrintF( _S( 1656, "%s ���� %d/%d" ), mob->GetName(), iCurrentCnt, iConditionCnt);
	}
	else if (iConditionType == QCONDITION_PC)
	{// PK ����Ʈ
		strRet.PrintF( _S(3976, "PK �¸� %d/%d"), iCurrentCnt, iConditionCnt);
	}
	else if(iConditionType == QCONDITION_ITEM || iConditionType == QCONDITION_ITEM_NORMAL)
	{
		CItemData* pItem = _pNetwork->GetItemData(iConditionIndex);
		strRet.PrintF( _S( 1657, "%s ���� %d/%d" ), pItem->GetName(), iCurrentCnt, iConditionCnt);
	}
	// [090616: selo] Ȯ���� ����Ʈ �ű� ����
	else if(iConditionType == QCONDITION_AREA)
	{
		strRet.PrintF( CTString("%d/%d"), iCurrentCnt, iConditionCnt);
	}
	else if(iConditionType == QCONDITION_ITEMUSE)
	{
		CItemData* pItem = _pNetwork->GetItemData(iConditionIndex);
		strRet.PrintF( _S( 1657, "%s ���� %d/%d" ), pItem->GetName(), iCurrentCnt, iConditionCnt);
	}

	return strRet;
}

INDEX CQuestSystem::SearchContactQuestIndex(INDEX iQuestIndex)
{
	INDEX iContactNpcIndex = 0;
	INDEX iContactQuest = 0;
	CQuestStaticData pContactData;

	mmap::iterator ItrQuestToNPC = m_mmapPreQuestToCurQuest.find(iQuestIndex);

	if (ItrQuestToNPC != m_mmapPreQuestToCurQuest.end())
	{
		iContactQuest = (*ItrQuestToNPC).second;
		iContactNpcIndex = CQuestStaticData::getData(iContactQuest)->prizeNPC;
	}

	return iContactNpcIndex;
}

//[sora] �̹��� ��Ʈ�� index ǥ��
void CQuestSystem::SetNoTranslate( std::vector<INDEX> transList )
{
	std::vector<INDEX>::iterator it = transList.begin();

	for ( ; it != transList.end(); ++it )
	{
		CQuestStaticData::getData((*it))->SetNoTranslate();
	}
}

void CQuestSystem::ClearNoTranslate( std::vector<INDEX> transList )
{
	std::vector<INDEX>::iterator it = transList.begin();

	for ( ; it != transList.end(); ++it )
	{
		CQuestStaticData::getData((*it))->ClearNoTranslate();
	}
}
