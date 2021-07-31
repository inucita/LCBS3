#ifndef SE_INCL_GAME_MENU_DISABLED_H
#define SE_INCL_GAME_MENU_DISABLED_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "GameMenu.h"
#include "GUI/Components/MGButton.h"
#include "GUI/Components/MGTitle.h"


class CDisabledMenu : public CGameMenu {
public:
  CMGTitle gm_mgTitle;
  CMGButton gm_mgButton;

  void Initialize_t(void);
};

#endif  /* include-once check. */