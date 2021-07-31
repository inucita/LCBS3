#include "stdh.h"
#include <Engine/Interface/UIWindow.h>
#include "HelperManager.h"

#include <Engine/Interface/UIManager.h>
#include <Engine/Network/CNetwork.h>
#include "HelperUI.h"
#include "Helper_Student.h"
#include "Helper_GuardianInfoUI.h"

CHelperManager::CHelperManager()
{

}

CHelperManager::~CHelperManager()
{
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
	}
	if ( !m_vectorTeacherInfoList.empty() )
	{
		m_vectorTeacherInfoList.clear();
	}
	if ( !m_vectorStudentInfoList.empty() )
	{
		m_vectorStudentInfoList.clear();
	}
}

void CHelperManager::OpenDialog()
{
	CUIManager* pUIManager = CUIManager::getSingleton();

	if (pUIManager->DoesMessageBoxLExist( MSGLCMD_HELPER_REQ ) == TRUE)
		return;	

	const int iLevel	= _pNetwork->MyCharacterInfo.level;

	if (iLevel >= 50)
	{
		if (pUIManager->GetHelper_GuardianInfo()->GetHide() == FALSE)
			return;
	}
	else
	{
		if (pUIManager->GetHelper_Student()->GetHide() == FALSE)
			return;
	}

	//ResetHelper();

	pUIManager->CloseMessageBox( MSGCMD_HELPER_REGISTER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_CANCEL_REGISTER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_BEMYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_NOTIFY );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYTEACHER );
	pUIManager->CloseMessageBox( MSGCMD_HELPER_FIRE_MYSTUDENT );

	CTString strTitle;
	CTString strMessage;

	

	if( iLevel >=  50 )
	{
		strTitle = _S( 1140, "�߽��� ����" );			
		// Create message box
		pUIManager->CreateMessageBoxL( strTitle, UI_HELPER, MSGLCMD_HELPER_REQ );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );

		strMessage.PrintF( _S( 1141, "������ �����ϴ� �������� �����ֱ� ���ؼ� �İ����� ����� �� �ֽ��ϴ�." \
			"�İ��� ����� �ϸ�, �ű� ������ �İ��� ����Ʈ�� �� �� [%s]���� �̸��� ���̰� �˴ϴ�." ), _pNetwork->MyCharacterInfo.name );		
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		strMessage.PrintF( _S( 5045, "�߽����� 50������ �Ǵ� ����, �پ��� ������ ���� �� �ֽ��ϴ�." ), LIMIT_GUARDIAN_APP_LEVEL );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );		

		strMessage.PrintF( "" );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );

		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "Ȯ���ϱ�." ), SEL_HELPER_TEACHER );		
	}
	else
	{
		strTitle = _S( 1147, "�İ��� ����" );			
		// Create message box

		// �İ����� ���� ��� �İ��� ����â�� ����.
		if (_pNetwork->MyCharacterInfo.lTeacherIndex < 0)
			pUIManager->CreateMessageBoxL( strTitle, UI_HELPER_STUDENT, MSGLCMD_HELPER_REQ );
		else	// �İ����� ���ٸ� �İ��� ����Ʈ ui�� ����.
			pUIManager->CreateMessageBoxL( strTitle, UI_HELPER_GUARDIAN_INFO, MSGLCMD_HELPER_REQ );

		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strTitle, -1, 0xE18600FF );

		strMessage.PrintF( _S( 1148, "�İ����� [%s]���� %d�������� �����ϴµ� ������ �ִ� ����� �̾߱��մϴ�." \
			"�İ��� ����Ʈ���� [%s]�Բ� ������ �� �� �ִ� ����� ã�ƺ�����." )
			, _pNetwork->MyCharacterInfo.name, LIMIT_GUARDIAN_APP_LEVEL, _pNetwork->MyCharacterInfo.name );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		strMessage.PrintF( _S( 5044, "���� �������� �� 3���� ������ ���� ������, 30������ ������ �İ�����"\
			"���� �Ҽ��� ������ �����Ͻñ� �ٶ��ϴ�." ), _pNetwork->MyCharacterInfo.name );
		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, TRUE, strMessage, -1, 0xA3A1A3FF );			

		pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE,  _S( 1146, "Ȯ���ϱ�." ), SEL_HELPER_STUDENT );		
	}

	pUIManager->AddMessageBoxLString( MSGLCMD_HELPER_REQ, FALSE, _S( 880, "����ϱ�." ) );	
}

void CHelperManager::PrepareOpen()
{
	CUIManager* pUIManager = UIMGR();

	if (pUIManager->GetHelper()->GetHide() == TRUE &&
		pUIManager->GetHelper_Student()->GetHide() == TRUE &&
		pUIManager->GetHelper_GuardianInfo()->GetHide() == TRUE)
		return;

	const int iLevel	= _pNetwork->MyCharacterInfo.level;

	pUIManager->GetHelper()->CloseUI();
	pUIManager->GetHelper_Student()->CloseUI();
	pUIManager->GetHelper_GuardianInfo()->CloseUI();

	// �İ���
	if( iLevel >=  50 )	
	{
		pUIManager->GetHelper()->OpenUI();
	}
	// �߽���
	else	
	{
		// �߽����ε� �İ����� �ִ� ���
		if( _pNetwork->MyCharacterInfo.lTeacherIndex > 0 )
		{
			pUIManager->GetHelper_GuardianInfo()->OpenUI();
		}
		// ���� ���.
		else
		{
			pUIManager->GetHelper_Student()->OpenUI();
		}
	}
}

// ----------------------------------------------------------------------------
// Name : AddToHelperList()
// Desc : ��� ��Ͽ� �߰�
// ----------------------------------------------------------------------------
void CHelperManager::AddToHelperList( LONG lIndex, const CTString& strName, int iLevel, int iJob, int iJob2 )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter == m_vectorHelperList.end() )
	{
		if( m_vectorHelperList.size() <= HELPER_MAX_STUDENTS )
		{
			sHelperMember TempHelperMember;
			TempHelperMember.lIndex			= lIndex;
			TempHelperMember.strName		= strName;
			TempHelperMember.iLevel			= iLevel;
			TempHelperMember.iJob			= iJob;
			TempHelperMember.iJob2			= iJob2;
			m_vectorHelperList.push_back(TempHelperMember);
		}
	}
}

void CHelperManager::AddToStudentInfoList(LONG index, const char *szFirstDate, const char *szFinalDate)
{
	if(m_vectorHelperList.size() == m_vectorStudentInfoList.size() + 1)
	{
		sStudentInfo info;
		info.index = index;
		strcpy(info.szFirstDate, szFirstDate);
		strcpy(info.szFinalDate, szFinalDate);
		m_vectorStudentInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorStudentInfoList.size());
}

// ----------------------------------------------------------------------------
// Name : DelFromHelperList()
// Desc : ��� ��Ͽ��� ����
// ----------------------------------------------------------------------------
void CHelperManager::DelFromHelperList( LONG lIndex )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		int dist = std::distance(m_vectorHelperList.begin(), iter);
		m_vectorHelperList.erase( iter );
		if(m_vectorTeacherInfoList.size() > 0)
		{
			m_vectorTeacherInfoList.erase(m_vectorTeacherInfoList.begin() + dist);
		}
		else if(m_vectorStudentInfoList.size() > 0)
		{
			m_vectorStudentInfoList.erase(m_vectorStudentInfoList.begin() + dist);
		}
	}
}

// ----------------------------------------------------------------------------
// Name : ChangeHelperLevel()
// Desc : ����� ������ ������.
// ----------------------------------------------------------------------------
void CHelperManager::ChangeHelperLevel( LONG lIndex, int iLevel )
{
	sHelperMember TempHelperMember;
	TempHelperMember.lIndex = lIndex;
	std::vector<sHelperMember>::iterator iter = 
		std::find_if(m_vectorHelperList.begin(), m_vectorHelperList.end(), FindMember( TempHelperMember ) );

	if( iter != m_vectorHelperList.end() )
	{
		(*iter).iLevel = iLevel;
	}
}

void CHelperManager::ClearHelperList()
{
	// NOTE : ��带 Ż���ϰų� ��ü ���� �ʴ� �̻�, Ŭ���� �ϸ� �ȵɰ� ����.
	if( !m_vectorHelperList.empty() )
	{
		m_vectorHelperList.clear();
		m_vectorTeacherInfoList.clear();
		m_vectorStudentInfoList.clear();
	}
}


void CHelperManager::AddToTeacherInfoList(LONG index, SLONG fame, SLONG cntTeaching, SLONG cntComplete, SLONG cntFail, SBYTE StartPlayTime, SBYTE EndPlayTime)
{
	if(m_vectorHelperList.size() == m_vectorTeacherInfoList.size() + 1)
	{
		sTeacherInfo info;
		info.index = index;
		info.nFame = fame;
		info.nCntTeachingStudent = cntTeaching;
		info.nCntCompleteStudent = cntComplete;
		info.nCntFailStudent = cntFail;
		info.sbStartPlayTime= StartPlayTime;
		info.sbEndPlayTime	= EndPlayTime;
		m_vectorTeacherInfoList.push_back(info);
	}
	ASSERT(m_vectorHelperList.size() == m_vectorTeacherInfoList.size());
}
