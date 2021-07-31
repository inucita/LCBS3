#ifndef SE_INCL_GAME_MENU_NETWORKJOIN_H
#define SE_INCL_GAME_MENU_NETWORKJOIN_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "GameMenu.h"
#include "GUI/Components/MGButton.h"
#include "GUI/Components/MGTitle.h"


class CNetworkJoinMenu : public CGameMenu {
public:
  CMGTitle  gm_mgTitle;
  CMGButton gm_mgLAN;
  CMGButton gm_mgNET;
  CMGButton gm_mgOpen;

  void Initialize_t(void);
};

#endif  /* include-once check. */