

/*
 *  Last Chaos
 *  Copyright (c) 1998, CroTeam. All rights reserved.
 *
 */

#include "StdH.h"

#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <process.h>
#include <shlwapi.h>	// for PathFileExists()

#include <Engine/CurrentVersion.h>
#include <Engine/Templates/Stock_CEntityClass.h>
#include <Engine/Templates/Stock_CFontData.h>
#include <Engine/Interface/UIManager.h>				// yjpark
#include <Engine/Interface/UIOption.h>
#include <engine/Contents/function/SystemMenuUI.h>
#include <Engine/GameDataManager/GameDataManager.h>
#include <Engine/Interface/UIMouseCursor.h>
#include <Engine/GameState.h>
#include <Engine/GlobalDefinition.h>
#include <Engine/Contents/Login/UILoginNew.h>
#include <Engine/GameStageManager/StageMgr.h>

#include <crtdbg.h>


#include <Engine/Network/Web.h>
#include <Engine/Network/WebAddress.h>
#include <Engine/Network/MessageDispatcher.h>
#include <Engine/Contents/function/News_Web_UI.h>

#define DECL_DLL
#include <EntitiesMP/Global.h>
#include "resource.h"

//#include "SplashScreen.h"
#include "MainWindow.h"
#include "GlSettings.h"
//#include "LevelInfo.h"
//#include "LCDDrawing.h"
#include "CmdLine.h"
//#include "Credits.h"
#include <Engine/Base/Protection.h>
#if COPY_PROTECTION
	#include <Engine/Base/Protection.cpp>
#endif
#include <TlHelp32.h>
#include <float.h>
#include <Ext_ipc_event.h> // IPC
HWND g_parenthWnd = NULL;
ENGINE_API extern cWeb g_web;
extern ENGINE_API char *g_szExitError;

#ifndef _DEBUG	// bypass this code when debug mode build [3/29/2012 rumist]
#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    #include "xTrapWrapper.h"
#endif
#endif

#ifdef KALYDO
#include <Engine/Kalydo/PackageManager.h>
#include <Nksp/social/TLastChaosApplication.h>

#endif

//2013/04/15 jeil ��ó�� �ػ� ���� �߰�
#include <fstream>

using namespace std;


//#undef XTRAP_SECURE_CKBANG_2010_07_20

//CTString _fnmForCDCheck = "Bin\\SeriousSam.exe";

// application state variables by NSP
extern BOOL g_bTcp = TRUE;
// end of ..

extern CGame		*_pGame = NULL;

// application state variables
//extern BOOL _bRunning = TRUE;
//extern BOOL _bQuitScreen = TRUE;
//extern BOOL _bMenuActive = FALSE;
//extern BOOL _bMenuRendering = FALSE;

// Debug����϶� ���̺� ���� �ٲ� �ֱ� SharedMemoryDll [7/31/2012 Ranma]

#if		defined(_MSC_VER) && (_MSC_VER >= 1600)
#	ifdef _DEBUG
#		pragma comment(lib, "SharedMemoryD.lib")
#	else
#		pragma comment(lib, "SharedMemory.lib")
#	endif
#else	// _MSC_VER
#	ifdef _DEBUG
#		pragma comment(lib, "SharedMemoryDLL_d.lib")
#	else
#		pragma comment(lib, "SharedMemoryDLL.lib")
#	endif
#endif	// _MSC_VER

#pragma comment(lib, "version.lib")

static PIX _apixRes[][2] = {	 
	640, 480,	 
	800, 500,
	800, 600,	 
	1024, 768,
	1152, 864,	
	1280, 800,
	1280, 960,	
	1280, 1024,
	1600,1200,    	
};

extern void enableFPExceptions()
{
    int i = _controlfp (0,0);

    i &= ~ (EM_ZERODIVIDE);

    _controlfp (i, MCW_EM);
}

// helper for limiting window size when changing mode out of full screen
extern void FindMaxResolution( PIX &pixSizeI, PIX &pixSizeJ)
{
	const INDEX ctResolutions = ARRAYCOUNT(_apixRes);
	for( INDEX iRes=(ctResolutions-1); iRes>=0; iRes--) 
	{
		if( pixSizeI>_apixRes[iRes][0] && pixSizeJ>_apixRes[iRes][1]) 
		{
			pixSizeI = _apixRes[iRes][0];
			pixSizeJ = _apixRes[iRes][1];
			return;
		}
	} // should not end up here
	ASSERTALWAYS( "Desktop resolution too low!");
	pixSizeI = _apixRes[0][0];
	pixSizeJ = _apixRes[0][1];
}

//extern BOOL _bDefiningKey;		// ����.
extern BOOL _bDefiningKey = FALSE;
//static BOOL _bReconsiderInput = FALSE;
extern PIX  _pixDesktopWidth  = 0;
extern PIX  _pixDesktopHeight = 0;

static INDEX sam_iMaxFPSActive   = 500;
static INDEX sam_iMaxFPSInactive = 10;
static INDEX sam_bPauseOnMinimize = TRUE; // auto-pause when window has been minimized
extern ENGINE_API INDEX	sam_bWideScreen;		// yjpark
extern FLOAT sam_fPlayerOffset = 0.0f;

// Window handle
extern ENGINE_API HWND	_hwndMain;
// display mode settings
extern ENGINE_API BOOL	_bWindowChanging;				// yjpark |<--
extern ENGINE_API INDEX sam_bFullScreenActive;
extern ENGINE_API INDEX sam_iScreenSizeI;
extern ENGINE_API INDEX sam_iScreenSizeJ;
extern ENGINE_API INDEX sam_iDisplayDepth;
extern ENGINE_API INDEX sam_iDisplayAdapter;
extern ENGINE_API INDEX sam_iGfxAPI;	// 0==OpenGL	// yjpark     -->|
extern INDEX sam_bFirstStarted = FALSE;
extern FLOAT sam_tmDisplayModeReport = 5.0f;
//extern INDEX sam_bShowAllLevels = FALSE;
//extern INDEX sam_bMentalActivated = FALSE;

// network settings
extern CTString sam_strNetworkSettings = "";
// command line
extern CTString sam_strCommandLine = "";

// 0...app started for the first time
// 1...all ok
// 2...automatic fallback
static INDEX _iDisplayModeChangeFlag	= 0;
static TIME _tmDisplayModeChanged		= 100.0f; // when display mode was last changed

// rendering preferences for automatic settings
extern INDEX sam_iVideoSetup = 1;  // 0==speed, 1==normal, 2==quality, 3==custom
// automatic adjustment of audio quality
extern BOOL sam_bAutoAdjustAudio	= TRUE;

extern INDEX sam_bAutoPlayDemos		= TRUE;
static INDEX _bInAutoPlayLoop		= TRUE;

// menu calling
/*
extern INDEX sam_bMenuSave     = FALSE;
extern INDEX sam_bMenuLoad     = FALSE;
extern INDEX sam_bMenuControls = FALSE;
extern INDEX sam_bMenuHiScore  = FALSE;
*/
extern INDEX sam_bToggleConsole		= FALSE;
//extern INDEX sam_iStartCredits		= FALSE;

// for mod re-loading
extern CTFileName _fnmModToLoad		= CTString("");
extern CTString _strModServerJoin	= CTString("");
extern CTString _strURLToVisit		= CTString("");


// state variables fo addon execution
// 0 - nothing
// 1 - start (invoke console)
// 2 - console invoked, waiting for one redraw
extern INDEX _iAddonExecState		= 0;
extern CTFileName _fnmAddonToExec	= CTString("");

// logo textures
//static CTextureObject  _toLogoCT;
//static CTextureObject  _toLogoODI;
//static CTextureObject  _toLogoGSpy;
//static CTextureObject  _toLogoEAX;
//extern CTextureObject *_ptoLogoCT  = NULL;
//extern CTextureObject *_ptoLogoODI = NULL;
//extern CTextureObject *_ptoLogoGSpy = NULL;
//extern CTextureObject *_ptoLogoEAX = NULL;

extern CTString sam_strVersion = "0.5";
//extern CTString sam_strModName = TRANS("Prologue");
/*
#if _SE_DEMO
extern CTString sam_strFirstLevel = "Levels\\KarnakDemo.wld";
#else
extern CTString sam_strFirstLevel = "Levels\\01_Hatshepsut.wld";
#endif
*/
extern CTString sam_strIntroLevel = "Levels\\Intro.wld";
extern CTString sam_strGameName = "serioussam";

//extern CTString sam_strTechTestLevel = "Levels\\TechTest.wld";
//extern CTString sam_strTrainingLevel = "Levels\\KarnakDemo.wld";

ENGINE_API extern INDEX snd_iFormat;


// main window canvas
/*CDrawPort *pdp;
CDrawPort *pdpNormal;
CDrawPort *pdpWideScreen;
CViewPort *pvpViewPort;
HINSTANCE _hInstance;*/		// ypark

// main window canvas		// yjpark
extern ENGINE_API CDrawPort	*_pdpMain;
extern ENGINE_API CDrawPort	*_pdpNormalMain;
extern ENGINE_API CDrawPort	*_pdpWideScreenMain;
extern ENGINE_API CViewPort	*_pvpViewPortMain;
extern ENGINE_API HINSTANCE	_hInstanceMain;


ENGINE_API extern INDEX g_iCountry;

// WSS_NPROTECT 070402 -------------------------->>
#ifndef NO_GAMEGUARD
	#include <Nksp/GameGuard.h>
	extern ENGINE_API CTString g_szHackMsg;
#endif
// ----------------------------------------------<<

void GetNM(LPSTR lpCmdLine)
{
	
	// wooss 060209 NetMable_JP 
	extern BOOL g_bAutoLogin;
	extern CTString g_nmVER;
	extern CTString g_nmCID;
	extern CTString g_nmID;
	extern CTString g_nmPW;
	
	// wooss NetMable_JP Auto login 060209
	CTString strTemp((CHAR*)lpCmdLine);

//	MessageBox(NULL, strTemp, "CommandLine", MB_OK);

	for(INDEX ii = 0; ii < 4; ii++)
	{
		INDEX tempPos = strTemp.FindSubstr(" ");

		CTString strInsert = strTemp;

		strInsert.TrimRight(tempPos);

		switch(ii)
		{
			case 0 :
				g_nmVER.PrintF("%s", strInsert);
				break;
			case 1 :
				g_nmCID.PrintF("%s", strInsert);
				break;
			case 2 :
				g_nmID.PrintF("%s", strInsert);
				break;
			case 3 :
				g_nmPW.PrintF("%s", strTemp);
				break;
		}

		if(ii < 3)
			strTemp.DeleteChars(0, tempPos + 1);
	}

//	MessageBox(NULL, g_nmID, "loginID", MB_OK);
//	MessageBox(NULL, g_nmPW, "loginPW", MB_OK);

	if(g_nmID.Length()!=0 && g_nmPW.Length()!=0 ) {
			switch(g_iCountry)
			{
			case KOREA :
				//if(g_nmCID.IsEqualCaseSensitive("em") || g_nmCID.IsEqualCaseSensitive("nt"))
				//if(g_nmCID.IsEqualCaseSensitive("or"))
				if(g_nmCID.Length() > 0)
					g_bAutoLogin = TRUE;
				break;
			case THAILAND :
				break;
			case USA:
				break;
			case BRAZIL:	// [2012/07/19 : Sora]  ���� �ڵ� �α��� �߰�
				//g_bAutoLogin = TRUE;
				break;
			case MEXICO:	// [2012/07/19 : Sora]  ���� �ڵ� �α��� �߰�
				//g_bAutoLogin = TRUE;
				break;
			}
	}
}

/***********************************************************************************************
* IsRunning()
* �ߺ� ���� ���� �Լ�
* ���� ���� ���� ���μ����� �˻��Ͽ� Nksp.exe�� ����Ǿ� �ִ� ���� �ִ��� Ȯ���Ͽ� TRUE, FALSE��
* ���� �Ѵ�.
*************************************************************************************************/
BOOL IsRunning(void)
{
	PROCESSENTRY32 peNext;

	// ���� ���μ��� �ڵ��� �����Ѵ�.
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnapShot == INVALID_HANDLE_VALUE )
	{
		if (g_iCountry == RUSSIA)
			MessageBox(NULL, "������ ������ �������� LastChaos!", "LastChaos Run", MB_OK);
		else
			MessageBox(NULL, "Fail Execute LastChaos!", "LastChaos Run", MB_OK);

		return TRUE;
	}

	peNext.dwSize = sizeof(PROCESSENTRY32);

	int nFindNum = 0;
	int nSize = 0;
	DWORD dwTemp = GetCurrentProcessId();
	BOOL bCurrentProcess = FALSE;

	Process32First(hSnapShot, &peNext); // ù��° ���μ����� �����´�.

	while(Process32Next(hSnapShot, &peNext)) // ��� ���μ����� �˻��Ѵ�.
	{
		if (peNext.th32ProcessID == dwTemp)
		{
			bCurrentProcess = TRUE;
			continue;
		}

		nSize = strlen(peNext.szExeFile);

		if ( nSize > 8 ) { nSize -= 8; }
		else { nSize = 0; }
		
		if (stricmp(peNext.szExeFile+(nSize), "Nksp.exe") == 0)
		{
			nFindNum++;

			if (nFindNum > 0)
			{
				CloseHandle(hSnapShot);
				return TRUE;
			}
		}
	}

	CloseHandle(hSnapShot);

	if (!bCurrentProcess)
	{
		if (g_iCountry == RUSSIA)
			MessageBox(NULL, "������ ������ �������� LastChaos!", "LastChaos Run", MB_OK);	
		else
			MessageBox(NULL, "Fail Execute LastChaos!", "LastChaos Run", MB_OK);

		return TRUE;
	}

	return FALSE;
}


static void ApplyRenderingPreferences(void)
{
	ApplyGLSettings(TRUE);
}

/*
extern void ApplyVideoMode(void)
{
	StartNewMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
								(enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive) );
}
*/

static void BenchMark(void)
{
	_pGfx->Benchmark(_pvpViewPortMain, _pdpMain);
}


static void QuitGame(void)
{
	_pGameState->Running()		= FALSE;
	_pGameState->QuitScreen()	= FALSE;
}

// check if another app is already running
static HANDLE _hLock = NULL;
static CTFileName _fnmLock;
static void DirectoryLockOn(void)
{
	// create lock filename
	_fnmLock = _fnmApplicationPath+"nksp.loc";
	// try to open lock file
	_hLock = CreateFile(
		_fnmLock, 
		GENERIC_WRITE,
		0/*no sharing*/,
		NULL, // pointer to security attributes
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL|FILE_FLAG_DELETE_ON_CLOSE,  // file attributes
		NULL);
	// if failed
	if (_hLock==NULL || GetLastError()!=0) {
		// report warning
		CPrintF(TRANS("WARNING: Last Chaos didn't shut down properly last time!\n"));
	}
}
static void DirectoryLockOff(void)
{
	// if lock is open
	if (_hLock!=NULL) {
		// close it
		CloseHandle(_hLock);
	}
}

//������ ���� ���� �ý��� ���콺 �۾�	09.09
// NOTE : �ִϸ��̼� Ŀ���� ���ҽ��κ��� �о��.
static HCURSOR LoadAnimationCursor(HINSTANCE hInstance, UINT nID)
{
	HRSRC hRes		= FindResource(hInstance, MAKEINTRESOURCE(nID), "CURSORS");
	DWORD dwSize	= SizeofResource(hInstance, hRes);
	HGLOBAL hGlob	= LoadResource(hInstance, hRes);
	LPBYTE pBytes	= (LPBYTE)LockResource(hGlob);
	return (HCURSOR)CreateIconFromResource(pBytes, dwSize, FALSE, 0x00030000);	
}
//������ ���� �� �ý��� ���콺 �۾�		09.09

void End(void);





// automaticaly manage pause toggling
void UpdatePauseState(void)
{
	
}


// limit current frame rate if neeeded
void LimitFrameRate(void)
{
	// measure passed time for each loop
	static CTimerValue tvLast(-1.0f);
	CTimerValue tvNow   = _pTimer->GetHighPrecisionTimer();
	TIME tmCurrentDelta = (tvNow-tvLast).GetSeconds();

	// limit maximum frame rate
	sam_iMaxFPSActive   = ClampDn( (INDEX)sam_iMaxFPSActive,   1L);
	sam_iMaxFPSInactive = ClampDn( (INDEX)sam_iMaxFPSInactive, 1L);
	INDEX iMaxFPS = sam_iMaxFPSActive;
	if( IsIconic(_hwndMain)) iMaxFPS = sam_iMaxFPSInactive;

	
	TIME tmWantedDelta = 1.0f / iMaxFPS;
	if( tmCurrentDelta<tmWantedDelta) Sleep( (tmWantedDelta-tmCurrentDelta)*1000.0f);
	
	// remember new time
	tvLast = _pTimer->GetHighPrecisionTimer();
}

BOOL _bCDPathFound = FALSE;

BOOL FileExistsOnHD(const CTString &strFile)
{
	FILE *f = fopen(_fnmApplicationPath+strFile, "rb");
	if (f!=NULL) {
		fclose(f);
		return TRUE;
	} else {
		return FALSE;
	}
}

void TrimString(char *str)
{
	int i = strlen(str);
	if (str[i-1]=='\n' || str[i-1]=='\r') {
		str[i-1]=0;
	}
}

void LoadAndForceTexture(CTextureObject &to, CTextureObject *&pto, const CTFileName &fnm)
{
	try 
	{
		to.SetData_t(fnm);
		CTextureData *ptd = (CTextureData*)to.GetData();
		ptd->Force( TEX_CONSTANT);
		ptd = ptd->td_ptdBaseTexture;
		if( ptd!=NULL) ptd->Force( TEX_CONSTANT);
		pto = &to;
	} 
	catch( char *pchrError) 
	{
		(void*)pchrError;
		pto = NULL;
	}
}

//������ ���� ����	//(DevPartner Bug Fix)(2005-01-14)
HMODULE g_hGame = NULL;
//������ ���� ��	//(DevPartner Bug Fix)(2005-01-14)
void InitializeGame(void)
{
	try {
#ifndef NDEBUG 
//#	define GAMEDLL ("Bin\\Debug\\Game"+_strModExt+"D.dll")
#	if		defined(_MSC_VER) && (_MSC_VER >= 1600)
#		define GAMEDLL ("Bin\\Debug2010\\Game"+_strModExt+"D.dll")
#	else
#		define GAMEDLL ("Bin\\Debug\\Game"+_strModExt+"D.dll")
#	endif
#else
#	define GAMEDLL ("Bin\\Game"+_strModExt+".dll")
#endif
		CTFileName fnmExpanded;
		ExpandFilePath(EFP_READ, CTString(GAMEDLL), fnmExpanded);

		CPrintF(TRANS("Loading game library '%s'...\n"), (const char *)fnmExpanded);
//������ ���� ����	//(DevPartner Bug Fix)(2005-01-14)
		g_hGame = LoadLibrary(fnmExpanded);
		if (g_hGame==NULL) 
		{
			ThrowF_t("%s", GetWindowsError(GetLastError()));
		}
		CGame* (*GAME_Create)(void) = (CGame* (*)(void))GetProcAddress(g_hGame, "GAME_Create");
		if (GAME_Create==NULL) 
		{
			ThrowF_t("%s", GetWindowsError(GetLastError()));
		}
//������ ���� ��	//(DevPartner Bug Fix)(2005-01-14)
		_pGame = GAME_Create();		
	} 
	catch (char *strError) 
	{
		FatalError("%s", strError);
	}
	// init game - this will load persistent symbols
	// ������ init [3/2/2011 rumist]
	_pGame->Initialize(CTString("Data\\nksp.gms"), FALSE);
}

CEntityClass *g_pPlayer, *g_pWorldBase;
ENGINE_API void CheckEngineVersion();

BOOL Init( HINSTANCE hInstance, int nCmdShow, CTString strCmdLine)
{
	_hInstanceMain = hInstance;
	// remember desktop dimensions
	_pixDesktopWidth  = ::GetSystemMetrics(SM_CXSCREEN);
	_pixDesktopHeight = ::GetSystemMetrics(SM_CYSCREEN);

#if	!defined(VER_TEST) && !defined(G_CHINA) //&& !defined(G_USA)
	if(strCmdLine.IsEqualCaseSensitive(CTString("fkzktlfgod!")))
#elif	defined(VER_TEST) || defined(G_CHINA) //|| defined(G_USA)
	if (true)
#else
	if(strCmdLine.IsEqualCaseSensitive(CTString("6574")))
#endif	
	{
		
	}
	else
	{
		// WSS_NPROTECT 070405 ------------------>><<	
		// ȫ���� ��� nProtect�� �ߺ� ����
//#ifdef NO_GAMEGUARD
#ifndef MULTI_CLIENT
		if(FindWindow(APPLICATION_NAME,NULL))
		{
			return FALSE;
		}

		if (IsRunning())
		{
			if (g_iCountry == RUSSIA)
				MessageBox(NULL, "������ LastChaos ��?������?", "LastChaos", MB_OK);
			else
				MessageBox(NULL, "LastChaos is already running on your system", "LastChaos Run", MB_OK);

			return FALSE;
		}		
#endif // MULTI_CLIENT
//#endif
	}
	// prepare main window
	MainWindow_Init();

#ifndef _DEBUG
#ifdef XTRAP_SECURE_CKBANG_2010_07_20

    if (XTRAP())
        XTRAP()->EngineConfirm();

#endif
#endif

	OpenMainWindowInvisible();

	// parse command line before initializing engine
	ParseCommandLine(strCmdLine);

	// [2013/01/16] sykim70
	if (!SE_CheckEngine())
		return FALSE;

	// initialize engine
	SE_InitEngine(sam_strGameName);

	extern BOOL g_bNasTrans;
	g_bNasTrans = FALSE;
	switch(g_iCountry)
	{
		case BRAZIL:
			SetClassLong( _hwndMain, GCL_HICON, (LONG)LoadIcon( _hInstanceMain, (LPCTSTR)IDI_GUNSOFT ) );
			g_bNasTrans = TRUE;
			break;
	}

#if COPY_PROTECTION

	// get full path to the exe
	char strExePath[MAX_PATH] = "";
	GetModuleFileName( NULL, strExePath, sizeof(strExePath)-1);
	char strDrive[] = "C:\\";
	strDrive[0] = strExePath[0];
	DWORD dwSerial;
	GetVolumeInformation(strDrive, NULL, 0, &dwSerial, NULL, NULL, NULL, 0);

	CTFileName fnmKey = CTString("TSE.key");
	INDEX iSerial = dwSerial;

	// init encryption table two times
	BLOWFISH_CTX ctx1;
	BLOWFISH_CTX ctx2;
	CTString strEnKey = _strEngineBuild;
	strEnKey.TrimLeft(16);
	char aubKey1[16];
	char aubKey2[16];
	for(INDEX i=0; i<16; i++) {
		aubKey1[i] = strEnKey[i];
		aubKey2[i] = strEnKey[16-1-i];
	}
	Blowfish_Init (&ctx1, (unsigned char*)aubKey1, 16);
	Blowfish_Init (&ctx2, (unsigned char*)aubKey2, 16);

	// if no key
	if (!FileExists(fnmKey)) {
		// just bail out
		FatalError(TRANS("No registration key!\nPlease request registration key for code:\n\n%08X"), iSerial);
		return FALSE;
	// if key is here
	} else {
		// load it
		CTString strKey;
		LoadStringVar(fnmKey, strKey);
		__int64 llCodeRead = 0;
		strKey.ScanF("%I64x", &llCodeRead);
		__int64 llCode1 = llCodeRead;
		__int64 llCode2 = llCodeRead;

		// decrypt the code
		Blowfish_Decrypt(&ctx1, (ULONG*)&llCode1+1, ((ULONG*)&llCode1));
		Blowfish_Decrypt(&ctx2, (ULONG*)&llCode2+1, ((ULONG*)&llCode2));
		// if not valid
		if (llCode1!=iSerial && llCode2!=iSerial) {
			// bail out
			FatalError(TRANS("Registration key is invalid!"));
			return FALSE;
		}
	}
#endif

	SE_LoadDefaultFonts();
	// now print the output of command line parsing
	// deleted by seo

	// lock the directory
	DirectoryLockOn();

	// load all translation tables
	InitTranslation();
	try {
		AddTranslationTablesDir_t(CTString("Data\\Translations\\"), CTString("*.txt"));
		FinishTranslationTable();
	} catch (char *strError) {
		FatalError("%s", strError);
	}

	// always disable all warnings when in serious sam
	_pShell->Execute( "con_bNoWarnings=1;");

	// declare shell symbols
	_pShell->DeclareSymbol("persistent INDEX sam_bFullScreen;",   &sam_bFullScreenActive);
	
	GetWindowSizeFromTxt();

	_pShell->DeclareSymbol("persistent INDEX sam_iDisplayDepth;", &sam_iDisplayDepth);
	_pShell->DeclareSymbol("persistent INDEX sam_iDisplayAdapter;", &sam_iDisplayAdapter);
	_pShell->DeclareSymbol("persistent INDEX sam_iGfxAPI;",         &sam_iGfxAPI);
	_pShell->DeclareSymbol("persistent INDEX sam_bAutoAdjustAudio;", &sam_bAutoAdjustAudio);
	_pShell->DeclareSymbol("persistent user INDEX sam_bWideScreen;", &sam_bWideScreen);
	_pShell->DeclareSymbol("persistent user FLOAT sam_fPlayerOffset;",  &sam_fPlayerOffset);
	_pShell->DeclareSymbol("persistent user INDEX sam_bAutoPlayDemos;", &sam_bAutoPlayDemos);
	_pShell->DeclareSymbol("persistent user INDEX sam_iMaxFPSActive;",    &sam_iMaxFPSActive);
	_pShell->DeclareSymbol("persistent user INDEX sam_iMaxFPSInactive;",  &sam_iMaxFPSInactive);
	_pShell->DeclareSymbol("persistent user INDEX sam_bPauseOnMinimize;", &sam_bPauseOnMinimize);
	_pShell->DeclareSymbol("persistent user FLOAT sam_tmDisplayModeReport;",   &sam_tmDisplayModeReport);
	_pShell->DeclareSymbol("persistent user CTString sam_strNetworkSettings;", &sam_strNetworkSettings);
	_pShell->DeclareSymbol("persistent user CTString sam_strIntroLevel;",      &sam_strIntroLevel);
	_pShell->DeclareSymbol("persistent user CTString sam_strGameName;",      &sam_strGameName);
	_pShell->DeclareSymbol("user CTString sam_strVersion;",    &sam_strVersion);

	_pShell->DeclareSymbol("user void Quit(void);", &QuitGame);

	_pShell->DeclareSymbol("persistent user INDEX sam_iVideoSetup;",     &sam_iVideoSetup);
	_pShell->DeclareSymbol("user void ApplyRenderingPreferences(void);", &ApplyRenderingPreferences);
	_pShell->DeclareSymbol("user void Benchmark(void);", &BenchMark);

	_pShell->DeclareSymbol("user INDEX sam_bToggleConsole;",&sam_bToggleConsole);

	// Obtain player and world base class without releasing it (this will never be released from stock)
	g_pPlayer = _pEntityClassStock->Obtain_t(CTString("Classes\\Player.ecl"));  // this must not be a dependency!
	g_pWorldBase = _pEntityClassStock->Obtain_t(CTString("Classes\\WorldBase.ecl"));  // this must not be a dependency!

	//	�迵ȯ : Ŭ���̾�Ʈ�� ����DLL ���
	_bClientApp = TRUE;
	// if kalydo options.
#ifdef KALYDO
	RECT rcKalydoClient;
	GetClientRect( application->getKalydoWindow(), &rcKalydoClient );
	sam_iScreenSizeI = rcKalydoClient.right - rcKalydoClient.left;
	sam_iScreenSizeJ = rcKalydoClient.bottom - rcKalydoClient.top;
#endif
	InitializeGame();
	_pNetwork->md_strGameID = sam_strGameName;

	// initialize sound library
	snd_iFormat = Clamp( snd_iFormat, (INDEX)CSoundLibrary::SF_NONE, (INDEX)CSoundLibrary::SF_44100_16);
	_pSound->SetFormat( (enum CSoundLibrary::SoundFormat)snd_iFormat);

	if (sam_bAutoAdjustAudio) 
	{
		_pShell->Execute("include \"Scripts\\Addons\\SFX-AutoAdjust.ini\"");
	}

	CPrintF(TRANS("Nksp version: %s\n"), sam_strVersion);

#ifdef KALYDO
	sam_bFullScreenActive = FALSE;
#endif
	// apply application mode
	StartNewMode( GAT_D3D, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
								(enum DisplayDepth)sam_iDisplayDepth, IsFullScreen( sam_bFullScreenActive) );

	GameDataManager* pGameDataManager = SE_Get_GameDataManagerPtr();
	pGameDataManager->Create();

	StageMgr* pStageMgr = StageMgr::getSingleton();
	pStageMgr->Create();
	pStageMgr->SetNextStage( eSTAGE_INTRO, eSTAGE_LOGIN);

	CUIManager* pUIManager = SE_Get_UIManagerPtr();

	pUIManager->Create();

	pUIManager->GetOption()->SetDesktopSize( _pixDesktopWidth, _pixDesktopHeight );
	pUIManager->SetGameHandle(_pGame);
	pUIManager->ResetUIPos( _pdpMain );

//������ ���� ���� �ý��� ���콺 �۾�	09.09
//#define ANIMATION_CURSOR
#ifdef ANIMATION_CURSOR
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_NORMAL, LoadAnimationCursor( _hInstanceMain, IDC_NORMAL_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_NORMAL, LoadAnimationCursor( _hInstanceMain, IDC_ATTACK_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_SKILL, LoadAnimationCursor( _hInstanceMain, IDC_SKILL_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_MAGIC, LoadAnimationCursor( _hInstanceMain, IDC_MAGIC_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_BOW, LoadAnimationCursor( _hInstanceMain, IDC_BOW_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_PICK, LoadAnimationCursor( _hInstanceMain, IDC_PICK_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_TALK, LoadAnimationCursor( _hInstanceMain, IDC_TALK_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_PRODUCE, LoadAnimationCursor( _hInstanceMain, IDC_PRODUCE_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_SIZE, LoadAnimationCursor( _hInstanceMain, IDC_SIZE_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ZOOMIN, LoadAnimationCursor( _hInstanceMain, IDC_ZOOMIN_CURSOR) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_SIGNAL, LoadAnimationCursor( _hInstanceMain, IDC_SIGNAL_CURSOR) );
#else
	// FIXME : Engine�� Resource�� *.cur ȭ���� �־�������, �ش� Ŀ���� ������ ���ؼ� �̷������� ó����.	
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_NORMAL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_NORMAL_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_NORMAL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_ATTACK_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_SKILL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_SKILL_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_MAGIC, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_MAGIC_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ATT_BOW, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_BOW_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_PICK, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_PICK_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_TALK, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_TALK_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_PRODUCE, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_PRODUCE_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_SIZE, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_SIZE_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_ZOOMIN, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_ZOOMIN_CURSOR) ) );
	pUIManager->GetMouseCursor()->SetCursorHandle( UMCT_SIGNAL, LoadCursor( _hInstanceMain, MAKEINTRESOURCE(IDC_SIGNAL_CURSOR) ) );
	
#endif

	SE_Get_WebAddressPtr()->initialize();
	g_web.Begin();

	// 051121 nksp.exe version check on/off  
	if(strCmdLine.IsEqualCaseSensitive(CTString("6574"))) ;
	else { 
		CheckEngineVersion();
		CheckEntityVersion();
	}

	return TRUE;
}


void DestroyNetworkClasses()
{
	SE_Destroy_WebAddressPtr();
	g_web.End();
}


void End(void)
{
	SaveWindowSizeToTxt();	// ���� �Ҷ� Ŭ���̾�Ʈ ������ ����.
	DestroyNetworkClasses();

	ENTITY_STOCK_RELEASE(g_pPlayer);
	ENTITY_STOCK_RELEASE(g_pWorldBase);
	
	// destroy the main window and its canvas
	if (_pvpViewPortMain!=NULL) 
	{
 		_pGfx->DestroyWindowCanvas( _pvpViewPortMain);
		_pvpViewPortMain = NULL;
		_pdpNormalMain   = NULL;
	}
	CloseMainWindow(true);
	MainWindow_End();
	_pGame->End();

	// unlock the directory
	DirectoryLockOff();
	SE_EndEngine();
	
	void (*GAME_Destroy)(void) = (void (*)(void))GetProcAddress(g_hGame, "GAME_Destroy");
	if (GAME_Destroy==NULL) 
	{
		ThrowF_t("%s", GetWindowsError(GetLastError()));
	}
	GAME_Destroy();
//������ ���� ����	//(DevPartner Bug Fix)(2005-01-14)
	if(g_hGame) FreeLibrary(g_hGame);
//������ ���� ��	//(DevPartner Bug Fix)(2005-01-14)
}


// print display mode info if needed
void PrintDisplayModeInfo(void)
{
	// skip if timed out
	if( _pTimer->GetRealTimeTick() > (_tmDisplayModeChanged+sam_tmDisplayModeReport)) return;

	// cache some general vars
	SLONG slDPWidth  = _pdpMain->GetWidth();
	SLONG slDPHeight = _pdpMain->GetHeight();
	if( _pdpMain->IsDualHead())   slDPWidth/=2;
	if( _pdpMain->IsTripleHead()) slDPWidth/=3;

	CDisplayMode dm;
	dm.dm_pixSizeI = slDPWidth;
	dm.dm_pixSizeJ = slDPHeight;
	// determine proper text scale for statistics display
	FLOAT fTextScale = (FLOAT)slDPHeight/480.0f/2;

	// get resolution
	CTString strRes;
	extern CTString _strPreferencesDescription;
	strRes.PrintF( "%dx%dx%s", slDPWidth, slDPHeight, _pGfx->gl_dmCurrentDisplayMode.DepthString());
	if( dm.IsDualHead())   strRes += TRANS(" DualMonitor");
	if( dm.IsTripleHead()) strRes += TRANS(" TripleMonitor");
	if( dm.IsWideScreen()) strRes += TRANS(" WideScreen");
			 if( _pGfx->gl_eCurrentAPI==GAT_OGL) strRes += " (OpenGL)";
	else 
		if( _pGfx->gl_eCurrentAPI==GAT_D3D) strRes += " (Direct3D)";

	CTString strDescr;
	strDescr.PrintF("\n%s (%s)\n", _strPreferencesDescription, RenderingPreferencesDescription(sam_iVideoSetup));
	strRes += strDescr;
	// tell if application is started for the first time, or failed to set mode
		if( _iDisplayModeChangeFlag==0) 
		{
		strRes += TRANS("Display mode set by default!");
		} 
		else if( _iDisplayModeChangeFlag==2) 
		{
		strRes += TRANS("Last mode set failed!");
	}

	// print it all
	_pdpMain->SetFont( _pfdDisplayFont);
	_pdpMain->SetTextScaling( fTextScale);
	_pdpMain->SetTextShadow( +2);
	_pdpMain->SetTextAspect( 1.0f);
	const FLOAT fPosMul = _pdpMain->IsTripleHead() ? 1.05f : 0.05f;
	//pdp->PutText( strRes, slDPWidth*fPosMul, slDPHeight*0.85f, LCDGetColor(C_GREEN|255, "display mode"));		// ����.
	_pdpMain->PutText( strRes, slDPWidth*fPosMul, slDPHeight*0.85f);
}

// do the main game loop and render screen
void DoGame(void)
{
	StageMgr* pStageMgr = StageMgr::getSingleton();
	// set flag if not in game
	if( !_pGame->gm_bGameOn){ 
		//_gmRunningGameMode = GM_NONE;		// ����.		
		_pGameState->GetGameMode() = CGameState::GM_NONE;
	}
	else if(_pGameState->GetGameMode() == CGameState::GM_RESTART)
	{
		if (_pNetwork->bMoveCharacterSelectUI == FALSE)
			pStageMgr->SetNextStage( eSTAGE_INTRO, eSTAGE_LOGIN);
		else
			pStageMgr->SetNextStage(eSTAGE_INTRO, eSTAGE_SELCHAR);
	}
	
	{
		pStageMgr->Run();
	}

	if( _pGameState->GetGameMode() != CGameState::GM_NONE)		// ����.
	{
		_pGame->GameMainLoop();
	// if game is not started
	} 
	else 
	{
		// just handle broadcast messages
		_pNetwork->GameInactive();
	}

	// redraw the view
	if( !IsIconic(_hwndMain) && _pdpMain!=NULL && _pdpMain->Lock())
	{
		int iState = _pGameState->GetGameMode();

		if( _pGameState->GetGameMode() != CGameState::GM_NONE)		// ����.
		{
			// handle pretouching of textures and shadowmaps
			_pdpMain->Unlock();

			// �ᱹ�� RenderView()�� ȣ����.
			_pGame->GameRedrawView( _pdpMain, (_pGame->gm_csConsoleState!=CS_OFF)?0:GRV_SHOWEXTRAS);

			_pdpMain->Lock();			
		} 
		else 
		{
			_pdpMain->Fill(C_dGREEN|CT_OPAQUE);
		}

		//0609 kwon
		_pGame->GameRedrawCursor( _pdpMain, (_pGame->gm_csConsoleState!=CS_OFF)?0:GRV_SHOWEXTRAS);

		// render console
		// �ܼ�â ������.
		_pGame->ConsoleRender(_pdpMain);

		// done with all
		_pdpMain->Unlock();

		// show
		_pvpViewPortMain->SwapBuffers();
	}
}

CTextureObject _toStarField;
static FLOAT _fLastVolume = 1.0f;
void RenderStarfield(CDrawPort *pdp, FLOAT fStrength)
{
	CTextureData *ptd = (CTextureData *)_toStarField.GetData();
	// skip if no texture
	if(ptd==NULL) return;

	PIX pixSizeI = pdp->GetWidth();
	PIX pixSizeJ = pdp->GetHeight();
	FLOAT fStretch = pixSizeJ / 480.0f;
	fStretch *= FLOAT(ptd->GetPixWidth()) / ptd->GetWidth();

	PIXaabbox2D boxScreen(PIX2D(0,0), PIX2D(pixSizeI, pixSizeJ));
	MEXaabbox2D boxTexture(MEX2D(0, 0), MEX2D(pixSizeI/fStretch, pixSizeJ/fStretch));
	pdp->PutTexture(&_toStarField, boxScreen, boxTexture, LerpColor(C_BLACK, C_WHITE, fStrength)|CT_OPAQUE);
}


FLOAT RenderQuitScreen(CDrawPort *pdp, CViewPort *pvp)
{
	CDrawPort dpQuit(pdp, 0);
	CDrawPort dpWide;
	dpQuit.MakeWideScreen(&dpWide);
	// redraw the view
	if (!dpWide.Lock()) {
		return 0;
	}

	dpWide.Fill(C_BLACK|CT_OPAQUE);
	RenderStarfield(&dpWide, _fLastVolume);
	
//	FLOAT fVolume = Credits_Render(&dpWide);
//	_fLastVolume = fVolume;
	_fLastVolume	= 1.0f;

	dpWide.Unlock();
	pvp->SwapBuffers();

	return _fLastVolume;
}

// ����� Credit �����ִ� �κ�.
void QuitScreenLoop(void)
{
//	Credits_On(3);
	CSoundObject soMusic;
	try 
	{
		_toStarField.SetData_t(CTFILENAME("Textures\\Background\\Night01\\Stars01.tex"));
		//0603 kwon ����.
		//		soMusic.Play_t(CTFILENAME("Music\\Credits.mp3"), SOF_NONGAME|SOF_MUSIC|SOF_LOOP);
	} 
	catch (char *strError) 
	{
		CPrintF("%s\n", strError);
	}
	// while it is still running
	FOREVER 
	{
		FLOAT fVolume = RenderQuitScreen(_pdpMain, _pvpViewPortMain);
		if (fVolume<=0) 
		{
			return;
		}
		// assure we can listen to non-3d sounds
		soMusic.SetVolume(fVolume, fVolume);
		_pSound->UpdateSounds();
		// while there are any messages in the message queue
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// if it is not a keyboard or mouse message
			if(msg.message==WM_LBUTTONDOWN||
				 msg.message==WM_RBUTTONDOWN||
				 msg.message==WM_KEYDOWN) 
			{
				return;
			}
		}
		//Sleep(5);
	}
}

// reverses string
void StrRev( char *str) {
	char ctmp;
	char *pch0 = str;
	char *pch1 = str+strlen(str)-1;
	while( pch1>pch0) {
		ctmp  = *pch0;
		*pch0 = *pch1;
		*pch1 = ctmp;
		pch0++;
		pch1--;
	}
}

void GetLCModulePathName(char* Path)
{
	char strTmpPath[MAX_PATH] = "";
	GetModuleFileName(NULL, strTmpPath, MAX_PATH);

	StrRev(strTmpPath);

	char *pstr = strchr( strTmpPath, '\\');
	if( strnicmp( pstr, "\\gubed", 6)==0) pstr += 6;
	if( pstr[0] = '\\') pstr++;
	char *pstrFin = strchr( pstr, '\\');
	if( pstrFin==NULL) {
		strcpy( pstr, "\\");
		pstrFin = pstr;
	}
	StrRev(pstrFin);
	strncpy(strTmpPath, pstrFin, MAX_PATH-1);
	strcpy(Path, strTmpPath);
}

int SubMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	(void)hPrevInstance;

#if		!defined(KALYDO) && !defined(G_CHINA) && !defined(VER_TEST)
	CTString strparam = CTString(lpCmdLine);
	IPCEventInfo EventInfo;
	memset(EventInfo.ulParameter2, 0, sizeof(EventInfo.ulParameter2));
	
	if (!strparam.IsEqualCaseSensitive(CTString("fkzktlfgod!")))
	{
		// IPC //////////////////////////////////////////////////////////////////////////
		XExtIPCManager<IPCEventInfo> IPCMgr;

		int Ret = IPCMgr.XExtIPCEventCreate(1, FALSE);

		if (Ret < 0)
		{
			MessageBox(NULL, "This program could not be run itself", "LastChaos", MB_OK);
			IPCMgr.XExtIPCEventRelease(FALSE);
			return TRUE;
		}

		int nLoopCount = 0;
		do 
		{
			Ret = IPCMgr.XExtIPCEventQueryIPCEvent(&EventInfo, NULL);
			++nLoopCount;

			if (Ret > 0)
			{
				IPCEventInfo SendEventInfo;
				
				if (EventInfo.EventID != 100)
				{
					SendEventInfo.EventID = -1;
					SendEventInfo.DestProcessID = 0;
					SendEventInfo.ulParameter1 = 0;
					memset(SendEventInfo.ulParameter2, 0, sizeof(SendEventInfo.ulParameter2));
					IPCMgr.XExtIPCEventPost(&SendEventInfo);
					MessageBox(NULL, "This program could not be run itself", "LastChaos", MB_OK);
					IPCMgr.XExtIPCEventRelease(FALSE);
					return TRUE;
				}
#ifndef	MULTI_CLIENT
				if (SE_GetEngineDllRefCnt() > 1  || IsRunning() )// ��Ƽ Ŭ���̾�Ʈ ����. [2/4/2010 rumist]
				{
					SendEventInfo.EventID = -1;
					SendEventInfo.DestProcessID = 0;
					SendEventInfo.ulParameter1 = 0;
					memset(SendEventInfo.ulParameter2, 0, sizeof(SendEventInfo.ulParameter2));
					IPCMgr.XExtIPCEventPost(&SendEventInfo);
					MessageBox(NULL, "Already Executive Game.", "LastChaos", MB_OK);
					IPCMgr.XExtIPCEventRelease(FALSE);
					return TRUE;
				}
				else
#endif	//	MULTI_CLIENT
				{
					SendEventInfo.EventID = 100;
					SendEventInfo.DestProcessID = 0;
					SendEventInfo.ulParameter1 = 0;
					memset(SendEventInfo.ulParameter2, 0, sizeof(SendEventInfo.ulParameter2));
					IPCMgr.XExtIPCEventPost(&SendEventInfo);
					IPCMgr.XExtIPCEventRelease(FALSE);
					//MessageBox(NULL, "Send Event message", "LastChaos", MB_OK);
				}

				break;
			}

		} while( nLoopCount != 1000 );
	}

	GetNM(EventInfo.ulParameter2);

#endif	// !defined(KALYDO) && !defined(G_CHINA) && !defined(VER_TEST)
	
	ShowCursor(FALSE);
	
	if( !Init( hInstance, nCmdShow, lpCmdLine )) return FALSE;

#ifndef		KALYDO
	{
		// ���� ����� ���α׷��� ��θ� ������ �����̴�.
		char temp_path[MAX_PATH] = {0,};

		// ���� ����� ���α׷��� ��θ� ��´�.
		GetModuleFileName(NULL, temp_path, sizeof(temp_path));


		// ���� ������ ��� ���� ����� �ڵ鰪�� �����ϴ� �����̴�.
		DWORD h_version_handle;
		// ���������� �׸��� ����ڰ� �߰�/���� �Ҽ� �ֱ� ������ ������ ũ�Ⱑ �ƴϴ�.
		// ���� ���� ���α׷��� ���������� ���� ũ�⸦ �� �� ũ�⿡ �´� �޸𸮸� �Ҵ��ϰ� �۾��ؾ��Ѵ�.
		DWORD version_info_size = GetFileVersionInfoSize(temp_path, &h_version_handle);

		// ���������� �����ϱ� ���� �ý��� �޸𸮸� �����Ѵ�. ( �ڵ� �������� ���� )
		HANDLE h_memory = GlobalAlloc(GMEM_MOVEABLE, version_info_size); 
		// �ڵ� ������ �޸𸮸� ����ϱ� ���ؼ� �ش� �ڵ鿡 �����Ҽ� �ִ� �ּҸ� ��´�.
		LPVOID p_info_memory = GlobalLock(h_memory);

		// ���� ���α׷��� ���� ������ �����´�.
		GetFileVersionInfo(temp_path, h_version_handle, version_info_size, p_info_memory);


		// ���� ������ ���Ե� �� �׸� ���� ��ġ�� ������ �����̴�. �� �����Ϳ� ���޵� �ּҴ�
		// p_info_memory �� ���� ��ġ�̱� ������ �����ϸ� �ȵ˴ϴ�.
		// ( p_info_memory �� �����ϴ� ������ ������ �Դϴ�. )
		char *p_data = NULL;
		// ������ ���� ������ ũ�⸦ ������ �����̴�.
		UINT data_size = 0;

		// ���������� ���Ե� FileVersion ������ �� ����Ѵ�.
		if(VerQueryValue(p_info_memory, "\\StringFileInfo\\000004b0\\FileVersion", (void **)&p_data, &data_size)){ 
			LOG_DEBUG("Version %s", p_data);
			SE_Get_UIManagerPtr()->GetLogin()->setVersion(p_data);
		}


		// ���� ������ �����ϱ� ���� ����ߴ� �޸𸮸� �����Ѵ�.
		GlobalUnlock(h_memory); 
		GlobalFree(h_memory);
	}
#endif	// KALYDO

	BOOL bInit = TRUE;
	// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
//	CPrintF("[ ---->> GameGuard Init...Start <<---- ] - %lu\n",timeGetTime());
	//bInit = _pNetwork->Init_nProtect();
	CREATE_GAMEGUARD_WRAPPER();
	bInit = GAME_GUARD()->Init();		
	_pGameState->Running() = bInit;
//	CPrintF("[ ---->> GameGuard Init...End <<---- ] - %lu\n",timeGetTime());
#endif
	// ---------------------------------------------------<<
	
	// initialy, application is running and active, console and menu are off
	//_bRunning    = TRUE;
	//_bQuitScreen = TRUE;
	if(g_szExitError == NULL && bInit)
	{
		_pGameState->Running()	= TRUE;
		_pGameState->QuitScreen() = TRUE;
	}
	_pGame->gm_csConsoleState  = CS_OFF;
	//_pGame->gm_csComputerState = CS_OFF;
	//  _pGameState->m_bMenuActive    = FALSE;
	//_pGameState->m_bMenuRendering = FALSE;
	// while it is still running
	
#if _DEBUG
//	enableFPExceptions();
#endif	
	//_pGame->QuickTest(LOGIN_WORLD, pdp, pvpViewPort);
//#define AUTO_LOGIN
	
	ShowCursor(TRUE);
	
	// �̱�ȯ ���� ���� (11. 15) : IME ���� â ���� 
	SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
	// �̱�ȯ ���� �� 


//������ ���� ����	//(5th Closed beta)(0.2)
	FLOAT tmPrev = 0;
	FLOAT tmNow = 0;
	INDEX cntFrame = 0;
	FLOAT tmLastSndMute = 0;
//������ ���� ��	//(5th Closed beta)(0.2)

	// WSS_NPROTECT 070402 ------------------------------->>
	// ������ �ڵ� ���� ���� ��� �׷��� UI���� �ʱ�ȭ�� ���� �� SetHwnd() ȣ��
	// �� �Լ��� ���� CallBack �Լ��� ȣ�� �Ǳ� ������...
#ifndef NO_GAMEGUARD
//	CPrintF("[ ---->> GameGuard SetHwnd...Start <<---- ] - %lu\n",timeGetTime());
	//_pNetwork->SetHwnd_nProtect(_hwndMain);
	GAME_GUARD()->SetMainHwnd( _hwndMain );
//	CPrintF("[ ---->> GameGuard SetHwnd...End <<---- ] - %lu\n",timeGetTime());
#endif
	// ---------------------------------------------------<<

	//while( _bRunning && _fnmModToLoad=="")
	while( _pGameState->Running() && _fnmModToLoad=="")
	{
		// while there are any messages in the message queue
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE)) 
		{
			// if it is not a mouse message
			if( !(msg.message>=WM_MOUSEFIRST && msg.message<=WM_MOUSELAST) || g_web.IsWebHandle() ) 
			{
				// if not system key messages
				if( !(msg.message==WM_KEYDOWN&& msg.wParam==VK_F10
					||msg.message==WM_SYSKEYDOWN) ) 
				{
					{
						// dispatch it
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}
			}
			
			// system commands (also send by the application itself)
			if( msg.message==WM_SYSCOMMAND)
			{
				switch( msg.wParam & ~0x0F) 
				{
					// if should minimize
				case SC_MINIMIZE:
					if( _bWindowChanging) break;
					_bWindowChanging  = TRUE;
					// just minimize the window
					ShowWindow(_hwndMain, SW_MINIMIZE);
					break;
					// if should restore
				case SC_RESTORE:
					if( _bWindowChanging) break;
					_bWindowChanging  = TRUE;
					// restore window
					ShowWindow(_hwndMain, SW_SHOWNORMAL);
					break;
					// if should maximize
				case SC_MAXIMIZE:
					if( _bWindowChanging) break;
					_bWindowChanging  = TRUE;
					//_bReconsiderInput = TRUE;
					// go to full screen
					StartNewMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, sam_iScreenSizeI, sam_iScreenSizeJ,
						(enum DisplayDepth)sam_iDisplayDepth, TRUE);
					ShowWindow( _hwndMain, SW_SHOWNORMAL);

					#ifdef USE_GAMEGUARD
					//DESTROY_GAMEGUARD_WRAPPER();
					//CREATE_GAMEGUARD_WRAPPER();
					//bInit = GAME_GUARD()->Init();		
						GAME_GUARD()->SetMainHwnd( _hwndMain );
					#endif

					break;
				}
			}
			
			// if application should stop
			if( msg.message==WM_QUIT /*|| msg.message==WM_CLOSE*/) 
			{				
				QuitGame();
			}
			
			// if application is deactivated or minimized
			if( (msg.message==WM_ACTIVATE && (LOWORD(msg.wParam)==WA_INACTIVE || HIWORD(msg.wParam)))
				||  msg.message==WM_CANCELMODE
				||  msg.message==WM_KILLFOCUS
				|| (msg.message==WM_ACTIVATEAPP && !msg.wParam)) 
			{
				// �̱�ȯ ���� ���� (11. 15) : IME ���� â ���� 
				SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
				// �̱�ȯ ���� �� 
			}
			// if application is activated or minimized
			else if( (msg.message==WM_ACTIVATE && (LOWORD(msg.wParam)==WA_ACTIVE || LOWORD(msg.wParam)==WA_CLICKACTIVE))
				||  msg.message==WM_SETFOCUS
				|| (msg.message==WM_ACTIVATEAPP && msg.wParam)) 
			{
				if (msg.message == WM_SETFOCUS)
					msg = msg;
				// �̱�ȯ ���� ���� (11. 15) : IME ���� â ���� 
				SendMessage ( _hwndMain, WM_IME_NOTIFY, IMN_CLOSESTATUSWINDOW, NULL );
				// �̱�ȯ ���� �� 
			}
			// UI message
			SE_Get_UIManagerPtr()->MsgProc( &msg, &_bIMEProc );	// yjpark // �̱�ȯ ���� 11.12 : IME ó�� �߰�
			
			BOOL bMenuForced = 
				//(_gmRunningGameMode==GM_NONE &&		// ����.
				(_pGameState->GetGameMode() == CGameState::GM_NONE &&
				(_pGame->gm_csConsoleState==CS_OFF || 
				_pGame->gm_csConsoleState==CS_TURNINGOFF));
			
			// interpret console key presses
			// �ܼ�â�� Ű�Է¹޴� �κ�.

#if		defined(VER_TEST)
			if (_iAddonExecState==0) 
			{				
				if (msg.message==WM_KEYDOWN) 
				{
					_pGame->ConsoleKeyDown(msg);
				}
				else if (msg.message==WM_CHAR) 
				{
					_pGame->ConsoleChar(msg);
				}
			}
			
			// if toggling console
			// �ܼ� Ű�� ��������~!!!
			BOOL bConsoleKey = sam_bToggleConsole || msg.message==WM_KEYDOWN && _pNetwork->m_ubGMLevel > 0 &&
				( MapVirtualKey( msg.wParam, 0 ) == 41 /* scan code for '~' */ );
			if(bConsoleKey && !_bDefiningKey)
			{
				sam_bToggleConsole = FALSE;
				if( _iAddonExecState==3) _iAddonExecState = 0;
				// if it is up, or pulling up
				if( _pGame->gm_csConsoleState==CS_OFF || _pGame->gm_csConsoleState==CS_TURNINGOFF) 
				{
					// start it moving down and disable menu
					_pGame->gm_csConsoleState = CS_TURNINGON;
					// stop all IFeel effects
					IFeel_StopEffect(NULL);
				}
				
				// �ܼ�â�� �������Ҷ�...
				// ��, �ܼ�â�� �������¿���....
				else if( _pGame->gm_csConsoleState==CS_ON || _pGame->gm_csConsoleState==CS_TURNINGON) 
				{
					// start it moving up
					_pGame->gm_csConsoleState = CS_TURNINGOFF;
				}
			}
#endif		// VER_TEST
		} // loop while there are messages
		
		// when all messages are removed, window has surely changed
		_bWindowChanging = FALSE;
			
#if		defined(VER_TEST)
		// if addon is to be executed
		if (_iAddonExecState==1) {
			// print header and start console
			CPrintF(TRANS("---- Executing addon: '%s'\n"), (const char*)_fnmAddonToExec);
			sam_bToggleConsole = TRUE;
			_iAddonExecState = 2;
			// if addon is ready for execution
		} else if (_iAddonExecState==2 && _pGame->gm_csConsoleState == CS_ON) {
			// execute it
			CTString strCmd;
			strCmd.PrintF("include \"%s\"", (const char*)_fnmAddonToExec);
			_pShell->Execute(strCmd);
			CPrintF(TRANS("Addon done, press Escape to close console\n"));
			_iAddonExecState = 3;
		}
#endif	// VER_TEST
		
		UpdatePauseState();
#ifdef KALYDO
		// update resource queue.
		SE_UpdateStreamingData();
#endif		
		// do the main game loop and render screen
		DoGame();
		
//������ ���� ����	//(5th Closed beta)(0.2)
		extern ENGINE_API FLOAT g_fFramePerSecond;
		extern ENGINE_API BOOL g_bNoPlaySnd;
		++cntFrame;
		tmNow = (FLOAT)_pTimer->GetHighPrecisionTimer().GetSeconds();
		static bool bForDebug = FALSE;
		if(!bForDebug)
		{
			if(g_fFramePerSecond == FLT_MAX || (tmNow - tmPrev) >= 1.0f)	//1�ʿ� �ѹ� ó��.
			{
				g_fFramePerSecond = FLOAT( cntFrame / (tmNow - tmPrev) );
				if(g_fFramePerSecond < 6.1f)
				{
					tmLastSndMute = tmNow;
					g_bNoPlaySnd = TRUE;
				}
				else if( tmLastSndMute != 0 && (tmNow - tmLastSndMute) > 1.f )
				{
					tmLastSndMute = 0;
					g_bNoPlaySnd = FALSE;
				}
				cntFrame = 0;
				tmPrev = tmNow;

				// WSS_NPROTECT 070402 ------------------------------->>
#ifndef NO_GAMEGUARD
				//_pNetwork->Check_nProtect();
				GAME_GUARD()->CheckGameGuard();
#endif
				// ---------------------------------------------------<<
			}
		}
//������ ���� ��	//(5th Closed beta)(0.2)
	} // end of main application loop
	
	//_pInput->DisableInput();
	_pGame->StopGame();

#ifndef NO_GAMEGUARD
	GAME_GUARD()->ShowErrMsg();
	DESTROY_GAMEGUARD_WRAPPER();
#endif
	
	End();
	
	return TRUE;
}

void CheckModReload(void)
{
	if (_fnmModToLoad!="") {
#ifndef NDEBUG
		//CTString strDebug = "Debug\\";
#	if		defined(_MSC_VER) && (_MSC_VER >= 1600)
		CTString strDebug = "Debug2010\\";
#	else
		CTString strDebug = "Debug\\";
#	endif
#else
		CTString strDebug = "";
#endif
		CTString strCommand = _fnmApplicationPath+"Bin\\"+strDebug+"SeriousSam.exe";
		//+mod "+_fnmModToLoad.FileName()+"\"";
		CTString strMod = _fnmModToLoad.FileName();
		const char *argv[7];
		argv[0] = strCommand;
		argv[1] = "+game";
		argv[2] = strMod;
		argv[3] = NULL;
		if (_strModServerJoin!="") {
			argv[3] = "+connect";
			argv[4] = _strModServerJoin;
			argv[5] = "+quickjoin";
			argv[6] = NULL;
		}
		_execv(strCommand, argv);
	}
}

// 2012-07-19 sykim70
void ReleaseIPC()
{
	XExtIPCManager<IPCEventInfo> IPCMgr;
	for (int retry = 3; retry > 0; retry--)
	{
		int Ret = IPCMgr.XExtIPCEventCreate(1, FALSE);
		if (Ret == 0)
		{
			IPCEventInfo EventInfo;
			EventInfo.EventID = -1;
			IPCMgr.XExtIPCEventPost(&EventInfo);
			IPCMgr.XExtIPCEventRelease(FALSE);
			break;
		}
		Sleep(500);
	}
}

// 2012-07-19 sykim70
BOOL UpdateNewLauncher()
{
	char szDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, szDir);
	strcat(szDir, "\\");
	
	CTString strLauncher = CTString(szDir) + "LC.exe";
	CTString strNewLauncher = CTString(szDir) + "LCNew.exe";

	if (g_iCountry == BRAZIL)
	{
		strLauncher = CTString(szDir) + "LW.exe";
		strNewLauncher = CTString(szDir) + "LWNew.exe";
	}
	else if (g_iCountry == THAILAND)
	{
		strLauncher = CTString(szDir) + "LastChaosThai.exe";
	}
	
	// update lccnct.dta
	CTString fnmLccnctNew = CTString(szDir) + "lccnctNew.dta";
	if (PathFileExists(fnmLccnctNew.str_String))
	{
		CTString fnmLccnct = CTString(szDir) + "lccnct.dta";
		while (TRUE)
		{
			BOOL ret = DeleteFile(fnmLccnct.str_String);
			if (ret)
				break;
			Sleep(500);
		}
		MoveFile(fnmLccnctNew.str_String, fnmLccnct.str_String);
	}

	// check new launcher exists
	if (!PathFileExists(strNewLauncher.str_String))
		return FALSE;
	
	// release IPC (shared mem) if exists
	ReleaseIPC();
	
	// delete old launcher
	while (TRUE)
	{
		BOOL ret = DeleteFile(strLauncher.str_String);
		if (ret)
			break;
		Sleep(500);
	}
	
	// rename new launcher
	MoveFile(strNewLauncher.str_String, strLauncher.str_String);
	return TRUE;
}

int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			LPSTR lpCmdLine, int nCmdShow)
{
#ifdef	_DEBUG
//	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
//	_CrtSetBreakAlloc(106262);
#endif	// _DEBUG

	CMessageDispatcher::InitSEEDEncrypt();	

	// 2012-07-19 sykim70
	if (UpdateNewLauncher())
	{
		if( GetSystemDefaultLangID() == 0x0804 ) // �߱� ������ ��� ����
		{
			MessageBox(NULL, "����ȷ���������������ꡣ", "����", MB_OK); 
		}
		else 
		{
			// ITS #0005892 : russia ��û����. [1/13/2012 rumist]
			if (g_iCountry == RUSSIA)
				MessageBoxW(NULL, L"���֬�֬٬Ѭ����ڬ�� �ڬԬ��.", L"���Ҭ߬�Ӭݬ֬߬ڬ� �٬ѬӬ֬��֬߬�", MB_OK);
			else
				MessageBox(NULL, "Update is completed.\nPlease, restart game.", "Update completed", MB_OK);
		}
		return 1;
	}

#ifndef _DEBUG	// bypass this code when debug mode build [3/29/2012 rumist]
#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    CREATE_XTRAP_WRAPPER("");
#endif
#endif

#ifdef CKBANG_UI_TWITTER

    LUA_OPEN();

#endif

	int iResult;
	CTSTREAM_BEGIN {
		iResult = SubMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	} CTSTREAM_END;
	CTStream::ClearStreamHandling();

	CheckModReload();


#ifndef _DEBUG	// bypass this code when debug mode build [3/29/2012 rumist]
#ifdef XTRAP_SECURE_CKBANG_2010_07_20
    DESTROY_XTRAP_WRAPPER();
#endif
#endif

#ifdef CKBANG_UI_TWITTER
    
    LUA_CLOSE();
    
#endif

/*
	CloseHandle(hMutex);
*/
	if(g_szExitError != NULL)
	{
		::MessageBox(NULL, g_szExitError, "Error", MB_OK);
		delete[] g_szExitError;
	}

	return iResult;
}


// try to start a new display mode
BOOL TryToSetDisplayMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
													enum DisplayDepth eColorDepth, BOOL bFullScreenMode,bool bCenter)
{
	// remember new settings
	sam_bFullScreenActive = (bFullScreenMode==FULLSCREEN_MODE)?FULLSCREEN_MODE:OLD_WINDOW_MODE;

	CDisplayMode dmTmp;
	dmTmp.dm_ddDepth = eColorDepth;
	CPrintF( TRANS("  Starting display mode: %dx%dx%s (%s)\n"),
					 pixSizeI, pixSizeJ, dmTmp.DepthString(),
					 bFullScreenMode ? TRANS("fullscreen") : TRANS("window"));

	// mark to start ignoring window size/position messages until settled down
	_bWindowChanging = TRUE;
	
	// destroy canvas if existing
	if( _pvpViewPortMain!=NULL) {
		_pGfx->DestroyWindowCanvas( _pvpViewPortMain);
		_pvpViewPortMain = NULL;
		_pdpNormalMain = NULL;
	}

	// close the application window
	CloseMainWindow();

	// try to set new display mode
	BOOL bSuccess;
	//	�迵ȯ : �Լ� �Ķ���� ����.
	if( eGfxAPI==GAT_D3D) OpenMainWindowNormal( pixSizeI, pixSizeJ,bCenter);
	bSuccess = _pGfx->ResetDisplayMode( eGfxAPI);
	if( bSuccess && eGfxAPI==GAT_OGL) OpenMainWindowNormal( pixSizeI, pixSizeJ);
	//	�迵ȯ : ���� OpenMainWindowNormal() ó����.

	// if new mode was set
	if( bSuccess) {
		// create canvas
		ASSERT( _pvpViewPortMain==NULL);
		ASSERT( _pdpNormalMain==NULL);
		if(_pdpNormalMain) delete _pdpNormalMain;
 	  _pGfx->CreateWindowCanvas( _hwndMain, &_pvpViewPortMain, &_pdpNormalMain);

	    // ������ ��Ŀ�� ����
	    SetFocus(_hwndMain);

		// erase context of both buffers (for the sake of wide-screen)
		_pdpMain = _pdpNormalMain;
		if( _pdpMain!=NULL && _pdpMain->Lock()) {
			_pdpMain->Fill(C_BLACK|CT_OPAQUE);
			_pdpMain->Unlock();
			_pvpViewPortMain->SwapBuffers();
			_pdpMain->Lock();
			_pdpMain->Fill(C_BLACK|CT_OPAQUE);
			_pdpMain->Unlock();
			_pvpViewPortMain->SwapBuffers();
		}

		// lets try some wide screen screaming :)
		const PIX pixYBegAdj = _pdpMain->GetHeight() * 21/24;
		const PIX pixYEndAdj = _pdpMain->GetHeight() * 3/24;
		const PIX pixXEnd    = _pdpMain->GetWidth();
		
		// initial screen fill and swap, just to get context running
		BOOL bSuccess = FALSE;
		if( _pdpMain!=NULL && _pdpMain->Lock()) 
		{
			//pdp->Fill( LCDGetColor( C_dGREEN|CT_OPAQUE, "bcg fill"));		// ����.
			_pdpMain->Fill( C_BLACK|CT_OPAQUE);				// maybe ����
			_pdpMain->Unlock();
			_pvpViewPortMain->SwapBuffers();
			bSuccess = TRUE;
		}
		// if the mode is not working, or is not accelerated
		if( !bSuccess || !_pGfx->IsCurrentModeAccelerated())
		{ // report error
			CPrintF( TRANS("This mode does not support hardware acceleration.\n"));
			// destroy canvas if existing
			if( _pvpViewPortMain!=NULL) {
 				_pGfx->DestroyWindowCanvas( _pvpViewPortMain);
				_pvpViewPortMain = NULL;
				_pdpNormalMain = NULL;
			}
			// close the application window
			CloseMainWindow(true);
			// report failure
			return FALSE;
		}

		// Compressed texture
		if( eGfxAPI == GAT_D3D )
		{
			HRESULT	hrDXT1 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
									                            0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT1 );
			HRESULT	hrDXT3 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
									                            0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT3 );
			HRESULT	hrDXT5 = _pGfx->gl_pD3D->CheckDeviceFormat( iAdapter,
																D3DDEVTYPE_HAL,
																_pGfx->gl_d3dColorFormat,
									                            0,
																D3DRTYPE_TEXTURE,
																D3DFMT_DXT5 );

			_pGfx->gl_bCompressedTexture = SUCCEEDED( hrDXT1 ) && SUCCEEDED( hrDXT3 ) && SUCCEEDED( hrDXT5 );
		}

		sam_iScreenSizeI = pixSizeI;
		sam_iScreenSizeJ = pixSizeJ;
		sam_iDisplayDepth = eColorDepth;
		sam_iDisplayAdapter = iAdapter;
		sam_iGfxAPI = eGfxAPI;

		SE_Get_UIManagerPtr()->DestroyRenderTarget();
//#ifdef KALYDO
		SE_Get_UIManagerPtr()->InitRenderTarget();
//#endif
				
		// report success
		return TRUE;
	}

	// if couldn't set new mode
	else {
		// close the application window
		CloseMainWindow(true);
		// report failure
		return FALSE;
	}
}


// list of possible display modes for recovery 
const INDEX aDefaultModes[][3] =
{ // color, API, adapter
	{ DISPD_DEFAULT, GAT_OGL, 0},
	{ DISPD_16BIT,   GAT_OGL, 0},
	{ DISPD_16BIT,   GAT_OGL, 1}, // 3dfx Voodoo2
	{ DISPD_DEFAULT, GAT_D3D, 0},
	{ DISPD_16BIT,   GAT_D3D, 0},
	{ DISPD_16BIT,   GAT_D3D, 1},
};
const INDEX ctDefaultModes = ARRAYCOUNT(aDefaultModes);

// start new display mode
void StartNewMode( enum GfxAPIType eGfxAPI, INDEX iAdapter, PIX pixSizeI, PIX pixSizeJ,
									 enum DisplayDepth eColorDepth, BOOL bFullScreenMode)
{
	CPrintF( TRANS("\n* START NEW DISPLAY MODE ...\n"));

	// clamp window size to screen size if in window mode
	PIX pixMaxSizeI = _pixDesktopWidth  - 1; // need 1st lower resolution than desktop
	PIX pixMaxSizeJ = _pixDesktopHeight - 1;
	if( pixSizeI>pixMaxSizeI || pixSizeJ>pixMaxSizeJ) {
		extern void FindMaxResolution( PIX &pixSizeI, PIX &pixSizeJ);
		FindMaxResolution( pixMaxSizeI, pixMaxSizeJ);
		pixSizeI = pixMaxSizeI;
		pixSizeJ = pixMaxSizeJ;
	}

	// try to set the mode
	BOOL bSuccess = TryToSetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth, bFullScreenMode);

	// if failed
	if( !bSuccess)
	{
		// report failure and reset to default resolution
		_iDisplayModeChangeFlag = 2;  // failure
		CPrintF( TRANS("Requested display mode could not be set!\n"));
		pixSizeI = 640;
		pixSizeJ = 480;
		bFullScreenMode = TRUE;
		// try to revert to one of recovery modes
		for( INDEX iMode=0; iMode<ctDefaultModes; iMode++) {
			eColorDepth = (DisplayDepth)aDefaultModes[iMode][0];
			eGfxAPI     = (GfxAPIType)  aDefaultModes[iMode][1];
			iAdapter    =               aDefaultModes[iMode][2];
			CPrintF(TRANS("\nTrying recovery mode %d...\n"), iMode);
			bSuccess = TryToSetDisplayMode( eGfxAPI, iAdapter, pixSizeI, pixSizeJ, eColorDepth, bFullScreenMode);
			if( bSuccess) break;
		}
		// if all failed
		if( !bSuccess) {
			FatalError(TRANS(
				"Cannot set display mode!\n"
				"Last Chaos was unable to find display mode with hardware acceleration.\n"
				"Make sure you install proper drivers for your video card as recommended\n"
				"in documentation and set your desktop to 16 bit (65536 colors).\n"
				"Please see ReadMe file for troubleshooting information.\n"));
		}

	// if succeeded
	} else {
		_iDisplayModeChangeFlag = 1;  // all ok
	}

	SE_Get_UIManagerPtr()->InitPos(0, 0, pixSizeI, pixSizeJ);

	// apply 3D-acc settings
	ApplyGLSettings(FALSE);

	// remember time of mode setting
	_tmDisplayModeChanged = _pTimer->GetRealTimeTick();
}

//	�迵ȯ : ������ ũ�� ����
bool	Set_Window_Size()
{
	if(sam_bFullScreenActive != 1)
	{
		RECT	t_Rect;
		GetClientRect(_hwndMain,&t_Rect);

		long t_Width  = t_Rect.right  - t_Rect.left;
		long t_Height = t_Rect.bottom - t_Rect.top;
		TryToSetDisplayMode( (GfxAPIType)sam_iGfxAPI, sam_iDisplayAdapter, t_Width, t_Height, (enum DisplayDepth)sam_iDisplayDepth, sam_bFullScreenActive,false);
		 SE_Get_UIManagerPtr()->AdjustUIPos( _pdpNormalMain );
		 SE_Get_UIManagerPtr()->SetTitleName( sam_bFullScreenActive, sam_iScreenSizeI, sam_iScreenSizeJ );
		ShowWindow( _hwndMain, SW_SHOWNORMAL);
	}
	return true;
}

void GetWindowSizeFromTxt()
{
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTFileName fileName = CTString(strFullPath+"Data\\etc\\DisplaySize.txt");

	fstream fs;

	fs.open(fileName, ios::in | ios::out);

	if(fs.is_open())
	{
		INDEX data1 =0;
		INDEX data2 =0;
		fs >> data1 >> data2;
		sam_iScreenSizeI = data1;
		sam_iScreenSizeJ = data2;

		fs.close();
	}
	else
	{
		sam_iScreenSizeI = 1024;
		sam_iScreenSizeJ = 768;
	}
}

void SaveWindowSizeToTxt()
{
	CTString strFullPath = _fnmApplicationPath.FileDir();
	CTFileName fileName = CTString(strFullPath+"Data\\etc\\DisplaySize.txt");
	
	ofstream fs;

	fs.open(fileName);
	
	if(fs.is_open())
	{
		INDEX data1 =0;
		INDEX data2 =0;
		data1 = sam_iScreenSizeI;
		data2 = sam_iScreenSizeJ;
		fs << data1<<" "<<data2<<"  ";
		fs.close();
	}
}

#ifdef KALYDO

////////////////////////////////////////////////////////////////////
// kalydo functions.
CTFileName fnmPersistentSymbols = CTString("ps.dat");
CTFileName fnmDefaultPersistentSymbols = CTString("Data\\etc\\ps.dat");

void KalydoUserData()
{
	// test existing ps.dat file.
	FILE* fp = fopen( fnmPersistentSymbols, "rb" );
	// if not exist ps.dat, copy from krf packages.
	if( NULL == fp )
	{
		// not supporting Windows File I/O APIs.
		//BOOL bSuccess = CopyFile( fnmDefaultPersistentSymbols, fnmPersistentSymbols, TRUE );
		FILE* fr = fopen( fnmDefaultPersistentSymbols, "rb" );
		FILE* fw = fopen( fnmPersistentSymbols, "wb" );

		if( fr == NULL || fw == NULL )
			return;

		while ( !feof( fr ) )
		{
			char data;
			fread( &data, sizeof(char), 1, fr );
			fwrite( &data, sizeof(char), 1, fw );
		}

		fclose( fr );
		fclose( fw );
	}
	else
	{
		fclose( fp );
	}	
}

void KalydoSetUserAccount(std::string& sID, std::string& sPWD )
{
	extern BOOL g_bAutoLogin;
	if (sID == "" && sPWD == "")
	{
		g_bAutoLogin = FALSE;
	}
	else
	{
		extern CTString g_nmID;
		extern CTString g_nmPW;
		g_nmID.PrintF( "%s", sID.c_str() );
		g_nmPW.PrintF( "%s", sPWD.c_str() );
		g_bAutoLogin = TRUE;

		_pNetwork->m_strUserID = g_nmID;
		_pNetwork->m_strUserPW = g_nmPW;
	}
}

bool KalydoInit( HWND Parent )
{
	return true;
}

void KalydoStart()
{
}

bool KalydoUpdate()
{
	return false;
}

void KalydoEnd()
{
}

HWND getGameWindow()
{
	return _hwndMain;
}
#endif
