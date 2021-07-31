#include "StdH.h"
#include "attendance.h"
#include <Engine/Network/CNetwork.h>
#include <Engine/Network/Server.h>
#include <Engine/Interface/UIManager.h>
#include <Engine/Interface/UIMessageBox.h>
#include <Common/Packet/ptype_attendance.h>
#include <Engine/Contents/Base/ChattingUI.h>

Attendance::Attendance()
{
	m_nAccDate = 0;
}

void Attendance::RecvAttendanceMsg( CNetworkMessage* istr )
{
	CUIManager* pUIManager = CUIManager::getSingleton();
	pTypeThirdBase* pPacket = reinterpret_cast<pTypeThirdBase*>(istr->GetBuffer());
	CTString		strMessage;
	CUIMsgBox_Info	MsgBoxInfo;

	switch(pPacket->thirdType)
	{
	case MSG_SUB_ATTENDANCE_CHECK_RES: // ���� �⼮�ϼ� Ȯ�� (���� ���� Ȯ�� ����)
		{
			ResponseClient::AttendanceCheck* pRecv = reinterpret_cast<ResponseClient::AttendanceCheck*>(istr->GetBuffer());

			m_nAccDate = pRecv->acc_count;

			if( pUIManager->DoesMessageBoxLExist(MSGLCMD_ATTENDANCE_SYSTEM))
				pUIManager->CloseMessageBoxL(MSGLCMD_ATTENDANCE_SYSTEM);

			pUIManager->CreateMessageBoxL( _S(5017, "�⼮ ��Ȳ ����"), UI_QUEST, MSGLCMD_ATTENDANCE_SYSTEM );
			strMessage.PrintF( _S(5018, "[%s]���� �⼮��Ȳ�� �˷��帮�ڽ��ϴ�."), _pNetwork->MyCharacterInfo.name );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5019, "���� %d��° ���� �⼮���Դϴ�."), m_nAccDate );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage, -1, 0x6BD2FFFF );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _S(5020, "�����⼮ �ϼ��� ���� ����ǰ�� �ٸ��� ������ �⼮üũ�Ͻñ� �ٶ��ϴ�." ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _s(" " ) );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _S(1143, "���󳻿�" ) );
			strMessage.PrintF( _S(5021, "%d�Ͽ��� �⼮ : %d%% �߰� ����ġ"), 2, 3 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5021, "%d�Ͽ��� �⼮ : %d%% �߰� ����ġ"), 3, 4 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5021, "%d�Ͽ��� �⼮ : %d%% �߰� ����ġ"), 4, 5 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S(5021, "%d�Ͽ��� �⼮ : %d%% �߰� ����ġ"), 5, 10 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5583, "%d�Ͽ��� �⼮ : ȣĪ ������"), 50 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5584, "%d�Ͽ��� �⼮ : ��� ������"), 100 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );

			strMessage.PrintF( _S( 5584, "%d�Ͽ��� �⼮ : ��� ������"), 150 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5583, "%d�Ͽ��� �⼮ : ȣĪ ������"), 200 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );

			strMessage.PrintF( _S( 5584, "%d�Ͽ��� �⼮ : ��� ������"), 250 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			strMessage.PrintF( _S( 5584, "%d�Ͽ��� �⼮ : ��� ������"), 300 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );

			strMessage.PrintF( _S( 5583, "%d�Ͽ��� �⼮ : ȣĪ ������"), 365 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, strMessage );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, TRUE, _S( 5585, "(365�� ������ �����ø�, �����⼮�� 0�Ϸ� �ʱ�ȭ �˴ϴ�.)") );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, FALSE, _S(1839, "������ ����"), 0 );
			pUIManager->AddMessageBoxLString( MSGLCMD_ATTENDANCE_SYSTEM, FALSE, _S(191, "Ȯ��") );
		}
		break;
	case MSG_SUB_ATTENDANCE_REWARD_RES: // ���� �ϼ��� ����
		{
			ResponseClient::AttendanceReward* pRecv = reinterpret_cast<ResponseClient::AttendanceReward*>(istr->GetBuffer());

			switch(pRecv->error_code)
			{
			case ATTENDANCE_ERROR_SUCCESS: // ���� ����
				break;
			case ATTENDANCE_ERROR_NO_REWARD: // ���� ������ ����
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( _S( 674, "�̺�Ʈ �������� ���� �� �����ϴ�." ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			case ATTENDANCE_ERROR_INVEN_FULL: // �κ��丮�� ���� ��
				{
					MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
					MsgBoxInfo.AddString( _S( 675, "�κ��丮 ������ �����ؼ� �̺�Ʈ �������� ���� �� �����ϴ�." ) );
					pUIManager->CreateMessageBox( MsgBoxInfo );
				}
				break;
			}
		}
		break;

	case MSG_SUB_ATTENDANCE_FINISH: // �⼮ �Ϸ�
		{
			UpdateClient::AttendanceFinish* pRecv = reinterpret_cast<UpdateClient::AttendanceFinish*>(istr->GetBuffer());

			m_nAccDate = pRecv->acc_count;

			strMessage.PrintF(_S(5016, "�⼮üũ�� �Ǿ����ϴ�. ���� �⼮ %d��° �ϰ� ��ʴϴ�."), m_nAccDate );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
			pUIManager->GetChattingUI()->AddSysMessage( _S(5024, "������ ������ �������� �ϼž� ���Ӱ� �⼮�� �����˴ϴ�."), SYSMSG_NORMAL);
			strMessage.PrintF(_S(5025, "�⼮ �ϼ��� ���� +%d�� �߰� ����ġ�� �޽��ϴ�."), pRecv->up_exp );
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);

			if ( m_nAccDate == 100 || m_nAccDate == 200 )
			{
				MsgBoxInfo.SetMsgBoxInfo( _S( 191, "Ȯ��" ), UMBS_OK, UI_NONE, MSGCMD_NULL );
				MsgBoxInfo.AddString( _S( 5117, "���� �⼮ �ϼ��� ���� ������ ��� ���������� ���𿡰� �����ñ� �ٶ��ϴ�." ) );
				pUIManager->CreateMessageBox( MsgBoxInfo );
			}
		}
		break;
	case MSG_SUB_ATTENDANCE_INFO_UPD: // ���� ���� ��,
		{
			UpdateClient::AttendanceInfo* pRecv = reinterpret_cast<UpdateClient::AttendanceInfo*>(istr->GetBuffer());

			m_nAccDate = pRecv->acc_count;

			strMessage.PrintF(_S(5015, "���� �⼮ %d��° �ϰ� ��ʴϴ�."), m_nAccDate);
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
			pUIManager->GetChattingUI()->AddSysMessage( _S(5022, "���� �⼮ �ϼ��� ���� �߰� ����ġ�� ȹ���Ͻ� �� �ֽ��ϴ�."), SYSMSG_NORMAL);
			strMessage.PrintF(_S(5023, "�⼮ �ϼ��� ���� +%d%%�� �߰� ����ġ�� �޽��ϴ�."), pRecv->up_exp);
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL);
		}
		break;
	case MSG_SUB_ATTENDANCE_ASSURE_CANCEL_OK_UPD:
		{
			UpdateClient::AttendanceAssureOk* pRecv = reinterpret_cast<UpdateClient::AttendanceAssureOk*>(istr->GetBuffer());

			if ( pUIManager->DoesMessageBoxExist(MSGCMD_ATTENDANCE_ASSURE_OK_UPD) == TRUE )
				pUIManager->CloseMessageBox(MSGCMD_ATTENDANCE_ASSURE_OK_UPD);

			strMessage.PrintF(_S(6315, "���� �⼮ ���� ������ ���� ���Դϴ�. �⼮ üũ�� �Ͻ÷��� �⼮ ���� ������ �����Ͽ��� �մϴ�. ���� �⼮ ���� ������ ���� �Ͻðڽ��ϱ�?"));
			MsgBoxInfo.SetMsgBoxInfo(_S(191, "Ȯ��"), UMBS_YESNO, UI_NONE, MSGCMD_ATTENDANCE_ASSURE_OK_UPD);
			MsgBoxInfo.AddString(strMessage);
			pUIManager->CreateMessageBox(MsgBoxInfo);
		}
		break;

	case MSG_SUB_ATTENDANCE_SYS_MSG:
		{
			UpdateClient::AttendanceSysMsg* pRecv = reinterpret_cast<UpdateClient::AttendanceSysMsg*>(istr->GetBuffer());
			
			switch(pRecv->sysMsg)
			{
			case ATTENDANCE_SYS_MSG_END_OK:
				strMessage.PrintF(_S(6319, "�⼮ ���� ������ �����Ǿ����ϴ�. ���� �����⼮ %d ��°�̸�, ���� �⼮ üũ�� �Ϸ��ϼž� �����⼮�� �����˴ϴ�."), m_nAccDate );
				break;
			case ATTENDANCE_SYS_MSG_TIME_END:
				strMessage.PrintF(_S(6318, "�⼮ ���� ������ ��� �Ⱓ�� ����Ǿ����ϴ�."));
				break;
			case ATTENDANCE_SYS_MSG_TIME_END_INIT_ATTENDANCE:
				strMessage.PrintF(_S(6320, "�⼮ ���� ������ ����� �� ���� �⼮�� ���� �ʾ� �⼮ �� ���� �ʱ�ȭ �Ǿ����ϴ�."));
				break;
			}
			pUIManager->GetChattingUI()->AddSysMessage( strMessage, SYSMSG_NORMAL, 0xFF0000FF);
		}
		break;
	}
}
