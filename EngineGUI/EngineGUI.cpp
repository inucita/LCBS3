#include "StdH.h"
#include <Engine/Templates/Stock_CTextureData.h>

// global engine gui handling object
CEngineGUI _EngineGUI;

/*
 We cannot use dllmain if using MFC.
 See MSDN article "Regular DLLs Dynamically Linked to MFC" if initialization is needed.

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}
*/



void CEngineGUI::SelectMode( CDisplayMode &dm, GfxAPIType &gat)
{
  // stupid way to change resources, but it must be done
  HANDLE hOldResource = AfxGetResourceHandle();

  // call select mode dialog
  CDlgSelectMode dlgSelectMode( dm, gat);

  // activate CTGfx resources
  AfxSetResourceHandle( GetModuleHandle(ENGINEGUI_DLL_NAME));
  INDEX iDialogResult = dlgSelectMode.DoModal();

  // restore resources
  AfxSetResourceHandle( (HINSTANCE)hOldResource);

  // if mode dialog ended succesefully
  if( iDialogResult == IDOK) SetFullScreenModeToRegistry( "Display modes", dm, gat);
}


CTFileName CEngineGUI::CreateTexture(CTFileName fnTexFileToRecreate/*=CTString("")*/,
                         CDynamicArray<CTFileName> *pafnCreatedTextures/*=NULL*/)
{
  CTFileName fnResult;
  // stupid way to change resources, but it must be done
  HANDLE hOldResource = AfxGetResourceHandle();
  // activate CTGfx resources
  AfxSetResourceHandle( GetModuleHandle(ENGINEGUI_DLL_NAME) );

  // if create texture is called with a wish to recreate texture
  if( fnTexFileToRecreate != "")
  {
    fnResult = CTFILENAME("");
    CTextureData *ptdTextureToRecreate;
    CTFileName fnToRecreateNoExt = fnTexFileToRecreate.NoExt();
    // try to
    try
    {
      // obtain texture to recreate
      ptdTextureToRecreate = _pTextureStock->Obtain_t(fnTexFileToRecreate);
      ptdTextureToRecreate->Reload();
      // if texture is of effect type, call create effect texture dialog
      if( ptdTextureToRecreate->td_ptegEffect != NULL) {
        // call create effect texture dialog with .tex name
        CDlgCreateEffectTexture dlgCreateEffectTexture(fnTexFileToRecreate);
        const INDEX iResult = dlgCreateEffectTexture.DoModal();
        if( iResult==IDOK) fnResult = fnTexFileToRecreate;
      }
      // else this texture was created from script or from single picture
      else if( FileExists( fnToRecreateNoExt+".tga")) {
        // call create normal texture dialog with tga picture name
        CDlgCreateNormalTexture dlgCreateNormalTexture( fnToRecreateNoExt+".tga");
        if( dlgCreateNormalTexture.m_bSourcePictureValid) {
          const INDEX iResult = dlgCreateNormalTexture.DoModal();
          if( iResult==IDOK) fnResult = dlgCreateNormalTexture.m_fnCreatedFileName;
        }
      } // else if script exists
      else if( FileExists( fnToRecreateNoExt+".scr")) {
        CDynamicArray<CTFileName> afnScript;
        CTFileName *pfnScript = afnScript.New();
        *pfnScript = fnToRecreateNoExt+".scr";
        // call create animated texture dialog with script name
        CDlgCreateAnimatedTexture dlgCreateAnimatedTexture(afnScript);
        const INDEX iResult = dlgCreateAnimatedTexture.DoModal();
        if( iResult==IDOK) fnResult = dlgCreateAnimatedTexture.m_fnCreatedFileName;
      }
      else {
        // not found!
        WarningMessage( "Cannot find source for recreating texture: \"%s\"", (CTString&)fnTexFileToRecreate);
      }
      // reload the texture
      ptdTextureToRecreate->Reload();
      // release obtained texture
      _pTextureStock->Release(ptdTextureToRecreate);
    }
    catch( char *err_str)
    {
      // not loaded!
      WarningMessage(err_str);
    }
  }

  // create, not recreate
  else
  {
    // call choose texture type dialog
    CDlgChooseTextureType dlgChooseTextureType;
    int iDlgResult = dlgChooseTextureType.DoModal();
    // if user choosed a texture type
    if( (iDlgResult != -1) && (iDlgResult >= 0) && (iDlgResult <= 2) )
    {
      // if result is 2 we want to create effect texture
      if( iDlgResult == 2)
      {
        // no file requester, just call dialog
        CDlgCreateEffectTexture dlgCreateEffectTexture;
        if( dlgCreateEffectTexture.DoModal() == IDOK)
        {
          fnResult = dlgCreateEffectTexture.m_fnCreatedTextureName;
          if( pafnCreatedTextures != NULL)
          {
            CTFileName *pfnCreatedTexture = pafnCreatedTextures->New();
            *pfnCreatedTexture = fnResult;
          }
        }
      }
      // both normal and animated textures need to call open file requester
      else
      {
        // different filters for different requests
        char *pFilters = "All files (*.*)\0*.*\0\0";

        // if dialog result is 0 we want to create normal texture
        if( iDlgResult == 0)
        {
          pFilters = "Pictures (*.pcx;*.tga)\0*.pcx;*.tga\0"
                               "PCX files (*.pcx)\0*.pcx\0"
                               "TGA files (*.tga)\0*.tga\0\0";
        }
        // if dialog result is 1 we want to create animated texture
        else if( iDlgResult == 1)
        {
          pFilters = "Picture or script files (*.pcx;*.tga;*.scr)\0*.pcx;*.tga;*.scr\0"
                     "PCX files (*.pcx)\0*.pcx\0"
                     "TGA files (*.tga)\0*.tga\0"
                     "Script files (*.scr)\0;*.scr\0\0";
        }
        // call file requester for opening textures
        CDynamicArray<CTFileName> afnCreateTexture;
        FileRequester( "Create texture", pFilters, KEY_NAME_CREATE_TEXTURE_DIR,
                       "Textures\\", "", &afnCreateTexture);
        if( afnCreateTexture.Count() == 0)
        {
          // restore resources
          AfxSetResourceHandle( (HINSTANCE) hOldResource);
          return CTString("");
        }

        // if requested texture type is 0 we want to create normal texture
        if( iDlgResult == 0)
        {
          // create textures
          FOREACHINDYNAMICARRAY( afnCreateTexture, CTFileName, itPicture)
          {
            CTFileName fnSource = itPicture.Current();
            if( (fnSource.FileExt() == ".pcx") || (fnSource.FileExt() == ".tga") )
            {
              // call create normal texture dialog
              CDlgCreateNormalTexture dlgCreateNormalTexture( fnSource);
              if( dlgCreateNormalTexture.m_bSourcePictureValid)
              {
                if( dlgCreateNormalTexture.DoModal() == IDOK)
                {
                  fnResult = dlgCreateNormalTexture.m_fnCreatedFileName;
                  if( pafnCreatedTextures != NULL)
                  {
                    CTFileName *pfnCreatedTexture = pafnCreatedTextures->New();
                    *pfnCreatedTexture = fnResult;
                  }
                }
              }
            }
          }
        }
        // if requested texture type is 1 we want to create animated texture
        else if( iDlgResult == 1)
        {
          // call create animated texture dialog
          CDlgCreateAnimatedTexture dlgCreateAnimatedTexture( afnCreateTexture);
          INDEX iDlgResult = dlgCreateAnimatedTexture.DoModal();
          if( iDlgResult == IDOK)
          {
            fnResult = dlgCreateAnimatedTexture.m_fnCreatedFileName;
            if( pafnCreatedTextures != NULL)
            {
              CTFileName *pfnCreatedTexture = pafnCreatedTextures->New();
              *pfnCreatedTexture = fnResult;
            }
          }
        }
      }
    }
  }
  // restore resources
  AfxSetResourceHandle( (HINSTANCE) hOldResource);
  return fnResult;
}



/* Functions used by application for getting and setting registry keys concerning modes */

void CEngineGUI::GetFullScreenModeFromRegistry( CTString strSectionName, CDisplayMode &dm, GfxAPIType &gat)
{
  // prepare full screen mode as default
  dm.dm_pixSizeI = 640;
  dm.dm_pixSizeJ = 480;
  dm.dm_ddDepth  = DISPD_DEFAULT;
  // read FS parameters from registry
  CTString strResult = AfxGetApp()->GetProfileString( strSectionName, "Full screen mode", "640 x 480 x 0");
  strResult.ScanF( "%d x %d x %d", &dm.dm_pixSizeI, &dm.dm_pixSizeJ, &dm.dm_ddDepth);
  if( dm.dm_ddDepth<DISPD_DEFAULT || dm.dm_ddDepth>DISPD_32BIT) dm.dm_ddDepth = DISPD_DEFAULT;
  strResult = AfxGetApp()->GetProfileString( strSectionName, "Full screen API", "OpenGL");
  gat = (strResult=="Direct3D") ? GAT_D3D : GAT_OGL;
}


void CEngineGUI::SetFullScreenModeToRegistry( CTString strSectionName, CDisplayMode dm, GfxAPIType gat)
{
  CTString strDM( 0, "%d x %d x %d", dm.dm_pixSizeI, dm.dm_pixSizeJ, dm.dm_ddDepth);
  CTString strGAT = (gat==GAT_D3D) ? "Direct3D" : "OpenGL";
  AfxGetApp()->WriteProfileString( strSectionName, "Full screen mode", strDM);
  AfxGetApp()->WriteProfileString( strSectionName, "Full screen API",  strGAT);
}



