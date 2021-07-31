#include "StdH.h"
#include <Engine/Base/KeyNames.h>
#include <Engine/CurrentVersion.h>
#include <GameMP/LCDDrawing.h>
#include "MGTitle.h"


void CMGTitle::Render(CDrawPort *pdp)
{
  SetFontTitle(pdp);

  PIXaabbox2D box = FloatBoxToPixBox(pdp, mg_boxOnScreen);
  PIX pixI = box.Center()(1);
  PIX pixJ = box.Min()(2);

  pdp->PutTextC(mg_strText, pixI, pixJ, LCDGetColor(C_WHITE | CT_OPAQUE, "title"));
}