#ifndef SE_INCL_GAME_MENU_SINGLEPLAYERNEW_H
#define SE_INCL_GAME_MENU_SINGLEPLAYERNEW_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "GameMenu.h"
#include "GUI/Components/MGButton.h"
#include "GUI/Components/MGTitle.h"


class CSinglePlayerNewMenu : public CGameMenu {
public:
  CMGTitle gm_mgTitle;
  CMGButton gm_mgTourist;
  CMGButton gm_mgEasy;
  CMGButton gm_mgMedium;
  CMGButton gm_mgHard;
  CMGButton gm_mgSerious;
  CMGButton gm_mgMental;

  void Initialize_t(void);
  void StartMenu(void);
};

#endif  /* include-once check. */