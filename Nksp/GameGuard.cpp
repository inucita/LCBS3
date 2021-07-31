// ----------------------------------------------------------------------------
//  File : GameGuard.cpp
//  Desc : Orignal Author Sam
// ----------------------------------------------------------------------------

#include "StdH.h"
#include "GameGuard.h"
#include "Engine//GameState.h"
#include "Engine/Network/SessionState.h"
#include "Engine/Network/MessageDefine.h"
#include "Engine/Interface/UIManager.h"

#ifndef NO_GAMEGUARD
	#include "Engine/GameGuardInterface.h"
	#pragma comment(lib, "NPGameLib.lib" )


ENGINE_API extern CGameState *_pGameState;
ENGINE_API extern CNetworkLibrary *_pNetwork;


CGameGuard* CGameGuard::m_spThis = NULL;

CGameGuard* g_pGameGuard = NULL;	

void GameGuard_Auth_Recv (GG_AUTH_DATA AuthData)
{
	g_pGameGuard->Auth (&AuthData);
}

void GameGuard_Send_ID (char* pszUserID)
{
	g_pGameGuard->SendUserId( pszUserID );
}

void CREATE_GAMEGUARD_WRAPPER()
{
	g_pGameGuard			= new CGameGuard();   
	
	g_pGameGuardRecvAuth	= GameGuard_Auth_Recv;
	g_pGameGuardSendUserID  = GameGuard_Send_ID;
}

void DESTROY_GAMEGUARD_WRAPPER()
{
	if (g_pGameGuard)
    {
        delete g_pGameGuard;
        g_pGameGuard = NULL;
    }
}


CGameGuard::CGameGuard()
{
	m_spThis			= this;
	m_pNpgl				= NULL;	
	m_dwLastCheckTime	= 0;	
	m_dwMsg				= 0;
	m_szGameGuardMsg.Clear();
}

CGameGuard::~CGameGuard ()
{
	m_spThis = NULL;
	Release();
}

CGameGuard* CGameGuard::Get()
{
    ASSERT(m_spThis);
    return m_spThis;
}

BOOL CGameGuard::Init ()
{
	m_pNpgl = new CNPGameLib("ShadowTW");

	if ( !m_pNpgl )
	{
		m_szGameGuardMsg = _S(3326,"���Ӱ��� ���࿡ �����߽��ϴ�. ���Ӱ��� �¾� ������ �ٽ� ��ġ�� ���ñ� �ٶ��ϴ�.");
		::MessageBox(NULL, m_szGameGuardMsg , "Error", MB_OK );
		return FALSE;
	}

	DWORD dwErrCode = m_pNpgl->Init();

	if ( NPGAMEMON_SUCCESS != dwErrCode )
	{		
		GameGuardErrorProcess( dwErrCode, false );
		//Release();
		return FALSE;
	}
	SetLastCheckTime( GetTickCount() );
	
	return TRUE;
}

void CGameGuard::Release ()
{
	if ( m_pNpgl )
	{
		delete m_pNpgl;
		m_pNpgl = NULL;
	}
	//if ( m_spThis )
	//	m_spThis = NULL;
}

inline void	CGameGuard::SetLastCheckTime ( DWORD dwTime )
{
	m_dwLastCheckTime = dwTime;
}
	
void CGameGuard::SetMainHwnd ( HWND hWnd )
{
	m_hWnd = hWnd;
	if ( m_pNpgl )
		m_pNpgl->SetHwnd( hWnd );
}

void CGameGuard::SendUserId ( char* pszUserID )
{
	if ( m_pNpgl )
	{	
		if ( m_pNpgl->Send( pszUserID ) )
		{
			//������ ����
		}
		else
		{
			//������ ���� �� ó���� �ʿ��Ѱ�?
		}
	}
}

void  CGameGuard::CheckGameGuard ()
{
	if ( m_pNpgl && ( GetTickCount() - m_dwLastCheckTime ) >= CHECK_TERM )
	{
		DWORD dwResult  = m_pNpgl->Check();
		m_dwLastCheckTime = GetTickCount();
			
	
		if( dwResult  != NPGAMEMON_SUCCESS )
		{
			GameGuardErrorProcess( dwResult, true );
			_pGameState->Running()		= FALSE;
			_pGameState->QuitScreen()	= FALSE;
		}
	}
	
}

BOOL CGameGuard::CheckDevice ( LPVOID lpD3ddevice, LPSTR lpszDll )
{
	return TRUE;
}

void CGameGuard::ShowErrMsg ()
{			
	if( m_szGameGuardMsg.Length() > 0)
	{
		if ( 0 == m_dwMsg)
			m_szErrorCaption.PrintF(_S(3342,"���Ӱ��� ����"));
		else
			m_szErrorCaption.PrintF(_S(3342,"���Ӱ��� ���� : %lu"),m_dwMsg);
		::MessageBox(NULL, m_szGameGuardMsg.str_String,m_szErrorCaption.str_String,MB_OK);
	}
}


void CGameGuard::Auth( GG_AUTH_DATA* pData )
{
	if ( m_pNpgl )
		m_pNpgl->Auth2( pData );
}

void CGameGuard::GameGuardErrorProcess ( DWORD dwMsg, bool bGameUI )
{
	m_dwMsg = dwMsg;	
	switch (dwMsg)
	{		
	//Error Code
	case NPGAMEMON_ERROR_EXIST:
		m_szGameGuardMsg = _S(3321,"���Ӱ��尡 ���� ���Դϴ�. ��� �ĳ� ����� �Ŀ� �ٽ� ������ ���ñ� �ٶ��ϴ�.");
		break;
		
		/*
	case NPGAMEMON_ERROR_CREATE :
		m_szGameGuardMsg =  "GameGuard Directory Create Error";
		break;
		/**/

	case NPGAMEMON_ERROR_NPSCAN:
		m_szGameGuardMsg = _S(3335,"���̷��� �� ��ŷ�� �˻� ��� �ε��� ���� �߽��ϴ�. �޸� �����̰ų� ���̷����� ���� ������ �� �ֽ��ϴ�.");
		break;

		/*
	case NPGAMEMON_ERROR_THREAD :
		strErrorMessage =  "CreateThread Error";
		break;
		/**/
		
	case NPGAMEMON_ERROR_INIT:
		m_szGameGuardMsg = _S(3323,"���Ӱ��� �ʱ�ȭ �����Դϴ�. ����� �� �ٽ� ������ ���ų� �浹�� �� �ִ� �ٸ� ���α׷����� ������ �� ������ ���ñ� �ٶ��ϴ�.");
		break;

	case NPGAMEMON_ERROR_GAME_EXIST:
		m_szGameGuardMsg = _S(3322,"������ �ߺ� ����Ǿ��ų� ���Ӱ��尡 �̹� ���� �� �Դϴ�. ���� ���� �� �ٽ� ������ ���ñ� �ٶ��ϴ�,");
		break;
		
	/*	
	case NPGAMEMON_ERROR_AUTH_NPGMUP:
		strErrorMessage =  "npgmup.des Authentication Fail";
		break;
		/**/
		
	case NPGAMEMON_ERROR_AUTH_GAMEGUARD:
	case NPGAMEMON_ERROR_AUTH_INI:
	//case NPGAMEMON_ERROR_AUTH_DLL:
	//case NPGAMEMON_ERROR_DECRYPT:
	case NPGAMEMON_ERROR_NFOUND_GG:	
	case NPGAMEMON_ERROR_NFOUND_INI:
		m_szGameGuardMsg = _S(3324,"���Ӱ��� ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�غ��ñ� �ٶ��ϴ�.");
		break;

	case NPGAMEMON_ERROR_CRYPTOAPI:
		m_szGameGuardMsg = _S(3325,"�������� �Ϻ� �ý��� ������ �ջ�Ǿ����ϴ�. ���ͳ� �ͽ��÷η�(IE)�� �ٽ� ��ġ�� ���ñ� �ٶ��ϴ�.");
		break;

	case NPGAMEMON_ERROR_EXECUTE:
		m_szGameGuardMsg = _S(3326,"���Ӱ��� ���࿡ �����߽��ϴ�. ���Ӱ��� �¾� ������ �ٽ� ��ġ�� ���ñ� �ٶ��ϴ�.");
		break;

	case NPGAMEMON_ERROR_ILLEGAL_PRG:
		m_szGameGuardMsg = _S(3327,"�ҹ� ���α׷��� �߰ߵǾ����ϴ�. ���ʿ��� ���α׷��� ������ �� �ٽ� ������ ���ñ� �ٶ��ϴ�.");
		break;

	case NPGMUP_ERROR_ABORT:
		m_szGameGuardMsg = _S(3328,"���Ӱ��� ������Ʈ�� ����ϼ̽��ϴ�. ������ ��ӵ��� ���� ��� ���ͳ� �� ���� ��ȭ�� ������ ������ ���ñ� �ٶ��ϴ�.");
		break;

	case NPGMUP_ERROR_CONNECT:
		m_szGameGuardMsg = _S(3329,"���̷����� �����̿���� ���� ��ŷ�� �����Ͽ����ϴ�. �ֽŹ���� ������ �� ��ǻ�� ��ü�˻縦 �� �ֽʽÿ�");
		break;


		//���� ���������� �������� 
 	//case HOOK_TIMEOUT:
	//	m_szGameGuardMsg = _S(3329,"���̷����� �����̿���� ���� ��ŷ�� �����Ͽ����ϴ�. �ֽŹ���� ������ �� ��ǻ�� ��ü�˻縦 �� �ֽʽÿ�");
 	//	break;

	case NPGAMEMON_ERROR_GAMEGUARD:
		m_szGameGuardMsg = _S(3330,"���Ӱ��� �ʱ�ȭ ���� �Ǵ� �������� ���Ӱ��� �����Դϴ�. ���Ӱ��� �¾� ������ �ٽ� ��ġ�ϰ� ������ ������ �ֽʽÿ�");
		break;

			//Callback Message
		/*
	case NPGAMEMON_UNDEFINED:
		{
			break;
		}
		/**/
	case NPGAMEMON_COMM_ERROR:
		{
			//GameMon���� ��� ä���� ���������ϴ�. ���� GameMon�� ��
			//���������� ����Ǿ��� ����̹Ƿ� ���ӵ� �������ݴϴ�.
			break;
		}
		
	case NPGAMEMON_COMM_CLOSE:
		{
			// GameMon���� ���� ���ӵ� �����Ű�� ���� 
			break;
		}
		
		
		//�߰� ���� �޽��� 
	case NPGMUP_ERROR_PARAM:
		m_szGameGuardMsg = _S(3331,"ini ������ ���ų� �����Ǿ����ϴ�. ���Ӱ��� �¾� ������ ��ġ�ϸ� �ذ��� �� �ֽ��ϴ�.");
		break;

	case NPGMUP_ERROR_INIT:
		m_szGameGuardMsg = _S(3332,"npgmup.des �ʱ�ȭ �����Դϴ�. ���Ӱ��� ������ ������ �ٽ� ���� ������ �ֽʽÿ�.");
		break;

	case NPGMUP_ERROR_DOWNCFG:
		m_szGameGuardMsg = _S(3333,"���Ӱ��� ������Ʈ ���� ���ӿ� �����Ͽ����ϴ�. ��� �� ��õ� �غ��ų�, ���� ��ȭ���� �ִٸ� ������ ������ ���ñ� �ٶ��ϴ�.");
		break;

	case NPGMUP_ERROR_AUTH:
		m_szGameGuardMsg = _S(3334,"���Ӱ��� ������Ʈ�� �Ϸ����� �� �߽��ϴ�. ���̷��� ����� �Ͻ� ������Ų �� ��õ� �غ��ðų�, PC���� ���α׷��� ����Ͻø� ������ ������ ���ñ� �ٶ��ϴ�.");
		break;

	case NPGG_ERROR_COLLISION:
//		strErrorMessage =  "���Ӱ���� �浹 ���α׷��� �߰ߵǾ����ϴ�.";
		break;
		
	default:		
		m_szGameGuardMsg = _S(3336,"���Ӱ��� ���� �� ������ �߻��Ͽ����ϴ�. ���� ���� ���� GameGuard������ �ִ� *erl ������ ���׷����ý� ÷���Ͽ� �ֽñ� �ٶ��ϴ�.");
		break;
	}	
	
}

 CGameGuard::CallBackErrCheck ( DWORD dwMsg, DWORD dwArg )
{
	BOOL bQuitApplication = FALSE;

	switch( dwMsg )
	{
	case NPGAMEMON_SUCCESS :
		break;

	case NPGAMEMON_COMM_ERROR:
		bQuitApplication = TRUE;
		break;
	case NPGAMEMON_COMM_CLOSE:
		bQuitApplication = TRUE;
		break;
	case NPGAMEMON_INIT_ERROR:
		{
			// GameMon ���� �����Դϴ�. ���� ���� ó���� �����ϰ� ���� ����
			//�� �����ڵ��� dwArg ���� �Բ� ������ �޽����� ����� �ݴϴ�.
			m_szGameGuardMsg.PrintF(_S(3337,"���Ӱ��� �ʱ�ȭ ���� : %lu"), dwArg);			
			bQuitApplication = TRUE;
			break;
		}
	case NPGAMEMON_SPEEDHACK:
		{
			//���ǵ����� �����Ǿ����ϴ�. ���� ���� ó���� �����ϰ� ���� ����
			//�� ������ �޽����� ������ݴϴ�
			m_szGameGuardMsg .PrintF(_S(3338,"���ǵ����� �����Ǿ����ϴ�."));
			bQuitApplication = TRUE;
			break;
		}
	case NPGAMEMON_GAMEHACK_KILLED :
		// �������� ����Ǿ����� ���������� ���� ������׽��ϴ�.
		// ������ ��� �����ص� ���������� �����ϱ⸦ �����մϴ�.
		m_szGameGuardMsg.PrintF(_S(3339,"�������� �߰ߵǾ����ϴ�.\r\n%s"),m_pNpgl->GetInfo());
		bQuitApplication = TRUE;
		break;

	case NPGAMEMON_GAMEHACK_DETECT :
		{
			//�������� �߰ߵǾ����ϴ�. ���� ���ᰡ �������� ���� ��
			//���̹Ƿ� ���� ���� ó���� �����ϰ� ���� ������ ������ �޽��� ������ݴϴ�.
			m_szGameGuardMsg.PrintF(_S(3340,"�������� �߰ߵǾ����ϴ�.\r\n%s"),m_pNpgl->GetInfo());
			if ( m_pNpgl->GetInfo() )
			{

			}				
			else
			{
				; // �ҹ� ���� �α� �����? ������ ������ ó�� �ʿ� 
			}
			bQuitApplication = TRUE;
		}		
		break;	
		
	case NPGAMEMON_GAMEHACK_DOUBT :
		{
			//���������� �ǽɵǴ� ���α׷��� ���� �� �Դϴ�. Ȥ�� ��
			//���̳� ���Ӱ��尡 �����Ǿ����ϴ�. ���� ���� ó���� �����ϰ� ���� ������ ���ʿ��� ���α׷���
			//�����ϰ� �ٽ� ������ �غ���� �޽����� ������ݴϴ�.
			m_szGameGuardMsg.PrintF(_S(3341,"�����̳� ���Ӱ��尡 �����Ǿ����ϴ�.\r\n%s"),m_pNpgl->GetInfo());
			bQuitApplication = TRUE;
			break;
		}

	case NPGAMEMON_GAMEHACK_REPORT:
		{
			DWORD dwHackInfoSize	= 0;
			LPBYTE pHackInfo		= NULL;
			pHackInfo = m_pNpgl->GetHackInfo(&dwHackInfoSize);
			
			if (pHackInfo && dwHackInfoSize > 0)
			{
				//�������� �߰ߵǾ����� ���� �����մϴ�. ����Ŭ���̾�Ʈ
				//�� ���޹��� �����͸� ������ �����ϰ� ������ �մϴ�.
				// �Ʒ� �Լ��� ���Ӱ��忡�� �����ϴ� �Լ��� �ƴմϴ�.
				//SendToHackLog(pHackInfo, dwHackInfoSize); // ������ ������ ����.
			}			
			break;
		}


	case NPGAMEMON_CHECK_CSAUTH2 :
		{
			GG_AUTH_DATA m_AuthData;
			memcpy( &m_AuthData, (PVOID)dwArg, sizeof( GG_AUTH_DATA ) );			
			CNetworkMessage nmItem(MSG_EXTEND);
			nmItem << (ULONG)MSG_EX_NPROTECT;
			nmItem << m_AuthData.dwIndex;
			nmItem << m_AuthData.dwValue1;
			nmItem << m_AuthData.dwValue2;
			nmItem << m_AuthData.dwValue3;			
			_pNetwork->SendToServerNew(nmItem);
		}
		break;		
	}
	

	return bQuitApplication;
}
BOOL CALLBACK NPGameMonCallback(DWORD dwMsg, DWORD dwArg)
{
	BOOL tAppExit =FALSE;
	
	tAppExit = GAME_GUARD()->CallBackErrCheck ( dwMsg, dwArg );
	
	if(tAppExit)
	{
		// ���� ����
		_pGameState->Running()		= FALSE;
		_pGameState->QuitScreen()	= FALSE;
		return FALSE;
	}
	return TRUE;
}



#endif