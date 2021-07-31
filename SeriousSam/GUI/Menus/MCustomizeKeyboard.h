#ifndef SE_INCL_GAME_MENU_CUSTOMIZEKEYBOARD_H
#define SE_INCL_GAME_MENU_CUSTOMIZEKEYBOARD_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "GameMenu.h"
#include "GUI/Components/MGArrow.h"
#include "GUI/Components/MGKeyDefinition.h"
#include "GUI/Components/MGTitle.h"


class CCustomizeKeyboardMenu : public CGameMenu {
public:
  CMGTitle gm_mgTitle;
  CMGKeyDefinition gm_mgKey[KEYS_ON_SCREEN];
  CMGArrow gm_mgArrowUp;
  CMGArrow gm_mgArrowDn;

  void Initialize_t(void);
  void StartMenu(void);
  void EndMenu(void);
  void FillListItems(void);
};

#endif  /* include-once check. */