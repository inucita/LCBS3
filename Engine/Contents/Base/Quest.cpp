#include "StdH.h"
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIInternalClasses.h>
#include "Quest.h"
#include <Engine/Contents/Base/UIQuestBookNew.h>
#include <Engine/Contents/Base/UIQuestView.h>
#include <Engine/Interface/UIPlayerInfo.h>

#if		(_MSC_VER < 1600)
bool Compare(Quest::stRestoreInfo& lhs, Quest::stRestoreInfo& rhs)
{
	bool bResult;
		
	bResult = lhs.iQuestLevel < rhs.iQuestLevel;
		
	if (lhs.iQuestLevel == rhs.iQuestLevel)
		bResult = lhs.iQuestIndex < rhs.iQuestIndex;
		
	return bResult;
}
#endif	// (_MSC_VER < 1600)

Quest::Quest(void)
{
	m_dQuestNoticeStartTime = 0;
	m_dRaidRemainTime		= 0;		// ���̵� ���� �ð� �ʱ�ȭ
	m_bIsRaidNow			= FALSE;	// ���̵� ������ �÷��� �ʱ�ȭ	
	m_dRaidRemainTimeRecvTime = 0;		// ���̵� ���� �ð� ���� �ð� �ʱ�ȭ
	m_nCurrViewPage = eVIEW_FIRST;

	for (int i = 0; i < eTREE_MAX; i++)
	{
		m_bListExtend[i] = TRUE;
	}
}

Quest::~Quest(void)
{
}

//////////////////////////////////////////////////////////////////////////
// network
void Quest::SendQuestReq( int nNpcIndex )
{
	ASSERT(nNpcIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_REQUEST, nNpcIndex );
}

void Quest::SendAcceptReq( int nQuestIndex, CItems* pQuestItem )
{
	if(nQuestIndex < 0) return;

	if( pQuestItem == NULL )
	{
		_pNetwork->SendQuestMessage( MSG_QUEST_START, nQuestIndex );
	}
	else
	{
		_pNetwork->SendItemUse(pQuestItem->Item_Tab, pQuestItem->InvenIndex, pQuestItem->Item_UniIndex, nQuestIndex);		
	}
}

void Quest::SendQuestPrize( int iQuestIndex, INDEX iNpcIndex, INDEX iOptionItemIndex, INDEX iOptionItemPlus )
{
	ASSERT(iQuestIndex != -1 && iNpcIndex != -1);
	_pNetwork->SendQuestPrizeMessage(MSG_QUEST_PRIZE, iQuestIndex, iNpcIndex, iOptionItemIndex, iOptionItemPlus);
}

void Quest::SendQuestCancel( int nQuestIndex )
{
	ASSERT(nQuestIndex != -1);
	_pNetwork->SendQuestMessage( MSG_QUEST_GIVEUP, nQuestIndex );
}
//////////////////////////////////////////////////////////////////////////
// ����, �Ϸ� â ����
INDEX Quest::GetProceedQuestIndex( INDEX index )
{
	stQuestInfo info = m_vectorProceedQuestList[index];
	return info.iQuestIndex;
}

INDEX Quest::GetCompleteQuestIndex( INDEX index )
{
	stQuestInfo info = m_vectorCompleteQuestList[index];
	return info.iQuestIndex;
}

void Quest::ClearQuest()
{
	m_vectorCompleteQuestList.clear();
	m_vectorProceedQuestList.clear();
}

BOOL Quest::AddQuest( int iQuestIndex, BOOL bComplete )
{
	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	stQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();
	TempQuest.iQuestType	= qdd.GetQuestCategory();
	TempQuest.iQuestScale	= qdd.GetQuestPartyScale();

	if( bComplete )
	{
		if(m_vectorCompleteQuestList.end()
			!= std::find_if(m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), stFindQuest(TempQuest)))
			return FALSE;

		m_vectorCompleteQuestList.push_back( TempQuest );
		return TRUE;
	}
	else
	{
		if(m_vectorProceedQuestList.end()
			!= std::find_if(m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), stFindQuest(TempQuest)))
			return FALSE;

		m_vectorProceedQuestList.push_back( TempQuest );
		return TRUE;
	}
	return FALSE;
}

BOOL Quest::DelQuest( int iQuestIndex, BOOL bComplete )
{
	stQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	std::vector<stQuestInfo>::iterator it = std::find_if( m_vectorCompleteQuestList.begin(), m_vectorCompleteQuestList.end(), stFindQuest( TempQuest ) );
	if( it != m_vectorCompleteQuestList.end() )
	{
		m_vectorCompleteQuestList.erase( it );
		return TRUE;
	}

	it = std::find_if( m_vectorProceedQuestList.begin(), m_vectorProceedQuestList.end(), stFindQuest( TempQuest ) );
	if( it != m_vectorProceedQuestList.end() )
	{
		m_vectorProceedQuestList.erase( it );
		return TRUE;
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// ����Ʈ �� ����

bool Quest::IsCompleteQuest( int nQuestIndex )
{
	stQuestInfo TempQuest;
	TempQuest.iQuestIndex	= nQuestIndex;

	if(m_vectorListComplete.end()
		!= std::find_if(m_vectorListComplete.begin(), m_vectorListComplete.end(), stFindQuest(TempQuest)))
		return true;

	return false;
}

INDEX Quest::GetListProceedIndex( INDEX index )
{
	stQuestInfo info = m_vectorListProceed[index];
	return info.iQuestIndex;
}

INDEX Quest::GetListCompleteIndex( INDEX index )
{
	stQuestInfo info = m_vectorListComplete[index];
	return info.iQuestIndex;
}

INDEX Quest::GetListRaidIndex( INDEX index )
{
	stQuestInfo info = m_vectorListRaid[index];
	return info.iQuestIndex;
}

void Quest::SetQuestRemainTime(int iQuestIndex, int iRemainTime)
{
	DOUBLE dCurTime = _pTimer->GetHighPrecisionTimer().GetSeconds();

	stQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	std::vector<stQuestInfo>::iterator iter = std::find_if(m_vectorListProceed.begin(), m_vectorListProceed.end(), stFindQuest(TempQuest));
	if( iter != m_vectorListProceed.end() )
	{
		(*iter).dRemainTime = (DOUBLE)iRemainTime;
		(*iter).dOldTime = dCurTime;
	}	
}

BOOL Quest::AddQuestList( int iQuestIndex, BOOL bComplete )
{
	std::vector<stQuestInfo>* pvecQuest = NULL;

	if (bComplete)
		pvecQuest = &m_vectorListComplete;
	else
		pvecQuest = &m_vectorListProceed;

	if( pvecQuest == NULL)
		return FALSE;

	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	stQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();
	TempQuest.iQuestType	= qdd.GetQuestCategory();
	TempQuest.iQuestScale	= qdd.GetQuestPartyScale();

	if(pvecQuest->end()
		!= std::find_if(pvecQuest->begin(), pvecQuest->end(), stFindQuest(TempQuest)))
		return FALSE;

	if( m_vectorListProceed.size() + m_vectorListComplete.size() >= DEF_MAX_QUEST )
	{	
		// [090527: selo] �޽��� �ڽ� �߰�
		CUIMsgBox_Info	MsgBoxInfo;		
		MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_NULL );				
		MsgBoxInfo.AddString( _S(4417, "����Ʈ �ִ� ������ �ʰ��Ͽ� ������ �� �����ϴ�.") );
		CUIManager::getSingleton()->CreateMessageBox( MsgBoxInfo );

		return FALSE;
	}

	pvecQuest->push_back(TempQuest);
	return TRUE;
}

BOOL Quest::DelQuestList( int iQuestIndex, BOOL bComplete )
{
	std::vector<stQuestInfo>* pvecQuest = NULL;

	if (bComplete)
		pvecQuest = &m_vectorListComplete;
	else
		pvecQuest = &m_vectorListProceed;

	if( pvecQuest == NULL)
		return FALSE;

	stQuestInfo TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	std::vector<stQuestInfo>::iterator it = std::find_if( pvecQuest->begin(), pvecQuest->end(), stFindQuest( TempQuest ) );

	if( it != pvecQuest->end() )
	{
		pvecQuest->erase( it );
		RemoveSelectedQuest(iQuestIndex);
		return TRUE;
	}

	return FALSE;
}

void Quest::ClearQuestList()
{
	m_vectorListRaid.clear();		//���̵� ����Ʈ ����Ʈ
	m_vectorListProceed.clear();	//������ ����Ʈ ����Ʈ
	m_vectorListComplete.clear();	//�Ϸ� ����Ʈ ����Ʈ

	for (int i = 0; i < eTREE_MAX; i++)
	{
		m_bListExtend[i] = TRUE;
	}
}

//////////////////////////////////////////////////////////////////////////
// ���̵� ����
void Quest::AddRaidMessage( const int& iQuestIndex )
{
	CQuestDynamicData qdd(CQuestSystem::Instance().GetStaticData(iQuestIndex));
	stQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;
	TempQuest.strQuestTitle	= qdd.GetName();	

	if(m_vectorListRaid.end()
		== std::find_if(m_vectorListRaid.begin(), m_vectorListRaid.end(), stFindQuest(TempQuest) ) )
	{
		m_vectorListRaid.push_back(TempQuest);
	}

	// �ε��� ������ ������ ���� �����Ѵ�.
	std::sort(m_vectorListRaid.begin(), m_vectorListRaid.end());

	// �̹� �߻��� ����Ʈ �������� üũ�ϰ� �Ѵ�.
	bool bExist = false;
	for( int i = 0; i < m_aAlreadyShowQuestNotice.size(); ++i )
	{
		if( iQuestIndex == m_aAlreadyShowQuestNotice[i] )
		{
			bExist = true;
			break;		
		}		
	}
	if( !bExist )
	{
		for( int index = 0; index < m_aAlreadyShowQuestNotice.size(); ++index )
		{
			if( 0 == m_aAlreadyShowQuestNotice[index] )
			{
				m_aAlreadyShowQuestNotice[index] = iQuestIndex;

				// ����Ʈ ������ ������ �Ѵ�.
				CUIManager::getSingleton()->GetPlayerInfo()->UpdateQuestNotice(iQuestIndex);

				// ����Ʈ ���� ���۵� �ð��� ����Ѵ�.
				m_dQuestNoticeStartTime = _pTimer->GetHighPrecisionTimer().GetSeconds();

				break;
			}
		}
	}	
}

void Quest::RemoveRaidMessageAll( void )
{
	std::vector<stQuestInfo>::iterator iter = m_vectorListRaid.begin();
	CUIManager::getSingleton()->GetQuestView()->UpdateContent();
	m_vectorListRaid.clear();
	RaidEnd();
}

bool Quest::isRaidMessage( const int& iQuestIndex )
{
	stQuestInfo	TempQuest;
	TempQuest.iQuestIndex	= iQuestIndex;

	if( std::count(m_vectorListRaid.begin(), m_vectorListRaid.end(), TempQuest) )
		return true;	

	return false;	
}

void Quest::RaidEnd( void )
{
	m_bIsRaidNow = false;
	m_dRaidRemainTime = -1;	
}

//����Ʈ ������ ��� �� �帥 �ð��� �˷��ش�.
DOUBLE Quest::GetQuestNoticeElapsedTime()
{
	return _pTimer->GetHighPrecisionTimer().GetSeconds() - m_dQuestNoticeStartTime;	
}

void Quest::SetRaidRemainTime(INDEX iRemainTime)
{
	m_bIsRaidNow = true;
	m_dRaidRemainTime = iRemainTime;
	m_dRaidRemainTimeRecvTime = _pTimer->GetHighPrecisionTimer().GetSeconds();
}

// ���̵� ��ǥ �ð� ���
DOUBLE Quest::GetRaidReminTime(void)
{	
	DOUBLE dOffset = _pTimer->GetHighPrecisionTimer().GetSeconds() - m_dRaidRemainTimeRecvTime;
	return m_dRaidRemainTime - dOffset;
}

BOOL Quest::IsRaidNow(void)
{
	return (CUIManager::getSingleton()->IsPlayInZone() && m_bIsRaidNow);
}

//////////////////////////////////////////////////////////////////////////
// �̸�����
void Quest::AddSelectedQuest(const int& iQuestIndex)
{
	// �̹� ����Ʈ�� ������ �׳� ����
	if( std::count(m_vectorViewList.begin(), m_vectorViewList.end(), iQuestIndex) )
		return;

	if (GetSelectedCount() >= 6)
		return;

	m_vectorViewList.push_back(iQuestIndex);

	CUIManager::getSingleton()->GetQuestView()->open();
}

void Quest::RemoveSelectedQuest(const int& iQuestIndex)
{
	if (IsSelectedQuest(iQuestIndex) == false)
		return;

	std::vector<int>::iterator iter = m_vectorViewList.begin();
	std::vector<int>::iterator iter_end = m_vectorViewList.end();

	for(; iter != iter_end; ++iter)
	{
		if(*iter == iQuestIndex)
		{
			m_vectorViewList.erase( iter );
			break;
		}
	}

	if (m_vectorViewList.size() - (DEF_PAGE_VIEW_COUNT * m_nCurrViewPage) <= 0)
	{
		PrevQuestViewPage();
	}

	CUIManager* pUIMgr = CUIManager::getSingleton();

	if (pUIMgr->GetQuestBookList()->IsVisible() != FALSE)
		pUIMgr->GetQuestBookList()->RefreshQuestListNew();

	if(m_vectorViewList.empty())
		pUIMgr->GetQuestView()->close();
	else
		pUIMgr->GetQuestView()->UpdateContent();
}

bool Quest::IsSelectedQuest(const int& iQuestIndex)
{
	if( std::count(m_vectorViewList.begin(), m_vectorViewList.end(), iQuestIndex) )
		return true;	

	return false;
}

void Quest::ClearSelectedQuestList()
{
	if(!m_vectorViewList.empty())
	{
		m_vectorViewList.clear();
		m_nCurrViewPage = 0;
	}

	CUIManager* pUIMgr = CUIManager::getSingleton();
	
	if (pUIMgr->GetQuestBookList()->IsVisible() != FALSE)
		pUIMgr->GetQuestBookList()->RefreshQuestListNew();
}

bool Quest::NextQuestViewPage()
{
	if (m_nCurrViewPage + 1 == eVIEW_MAX)
		return false;

	int nSize = m_vectorViewList.size();
	int nNextSize = DEF_PAGE_VIEW_COUNT * (m_nCurrViewPage + 1);
	if ( nSize <= nNextSize)
		return false;

	m_nCurrViewPage++;
	CUIManager* pUIMgr = CUIManager::getSingleton();
	pUIMgr->GetQuestView()->UpdateContent();
	return true;
}

bool Quest::PrevQuestViewPage()
{
	if (m_nCurrViewPage - 1 < eVIEW_FIRST)
		return false;

	m_nCurrViewPage--;
	CUIManager* pUIMgr = CUIManager::getSingleton();
	pUIMgr->GetQuestView()->UpdateContent();
	return true;
}

int Quest::GetSelectedQuestIndex( int Idx )
{
	if (Idx < 0 || Idx >= m_vectorViewList.size())
		return -1;

	return m_vectorViewList[Idx];
}

// ����Ʈ ����

void Quest::AddRestoreQuestList( const int& iQuestIndex, int iQuestLevel )
{
	stRestoreInfo resQuest;
	resQuest.iQuestIndex = iQuestIndex;
	resQuest.iQuestLevel = iQuestLevel;

	m_vectorRestoreList.push_back(resQuest);
}

void Quest::ClearAllRestoreList()
{
	m_vectorRestoreList.clear();
}

void Quest::SortRestoreQuest()
{
#if		(_MSC_VER >= 1600)
	std::sort(m_vectorRestoreList.begin(), m_vectorRestoreList.end(), Compare());
#else	// (_MSC_VER >= 1600)
	std::sort(m_vectorRestoreList.begin(), m_vectorRestoreList.end(), Compare);
#endif	// (_MSC_VER >= 1600)
}


