#ifndef SE_INCL_GAME_MENU_SPLITSTART_H
#define SE_INCL_GAME_MENU_SPLITSTART_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "GameMenu.h"
#include "GUI/Components/MGButton.h"
#include "GUI/Components/MGTitle.h"
#include "GUI/Components/MGTrigger.h"


class CSplitStartMenu : public CGameMenu {
public:
  CMGTitle gm_mgTitle;
  CMGTrigger gm_mgGameType;
  CMGTrigger gm_mgDifficulty;
  CMGButton gm_mgLevel;
  CMGButton gm_mgOptions;
  CMGButton gm_mgStart;

  void Initialize_t(void);
  void StartMenu(void);
  void EndMenu(void);
};

#endif  /* include-once check. */