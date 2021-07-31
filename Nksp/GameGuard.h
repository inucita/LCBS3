// ----------------------------------------------------------------------------
//  File : GameGuard.h
//  Desc : Orignal Author Sam
// ----------------------------------------------------------------------------

#ifndef GAME_GUARD_H
#define GAME_GUARD_H

#ifndef NO_GAMEGUARD
	#include "NPGameLib.h"

const int CHECK_TERM = 5000;

void CREATE_GAMEGUARD_WRAPPER();
void DESTROY_GAMEGUARD_WRAPPER();

class CGameGuard 
{
public: 
	CGameGuard();
	virtual ~CGameGuard ();
	
	static CGameGuard * Get ();
	
	BOOL		Init ();
	void		Release ();
	// ���� ������ �ڵ� ���Ӱ��忡 ���� 
	void		SetMainHwnd ( HWND hWnd );								
	// GameMoe�� ���� ���̵� ������ 
	void		SendUserId ( char* pszUserID );							
	// ���� ���� ���� �۵������� Ȯ�� 
	void		CheckGameGuard ();										
	// ����̽� ��ŷ üũ 
	BOOL		CheckDevice ( LPVOID lpD3ddevice, LPSTR lpszDll );         
	
	// ���� �߻��� �����޽��� ���̱� 
	void		ShowErrMsg ();												
	
	CNPGameLib* GetLibInstance () { return m_pNpgl; }
	// 2~5�� ���� Ȯ���ϱ� ���� �ð� ����
	inline void	SetLastCheckTime ( DWORD dwTime );
	
	void		Auth( GG_AUTH_DATA* pData );
	
	//�ʱ�ȭ�ÿ� ���ӽ��� �� üũ�� ������ �߻� ���� �� ȣ�� 	
	void		GameGuardErrorProcess ( DWORD dwMsg , bool bGameUI );
	BOOL		CallBackErrCheck ( DWORD dwMsg, DWORD dwArg );

	static CGameGuard* m_spThis;
private:
	
	CNPGameLib *m_pNpgl;	
	DWORD		m_dwLastCheckTime;
	CTString	m_szGameGuardMsg;
	CTString	m_szErrorCaption;
	
	
	DWORD		m_dwMsg;
	HWND		m_hWnd;
	
	
};

#define GAME_GUARD() CGameGuard::Get()

#endif 
#endif