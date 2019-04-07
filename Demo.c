//gcc -I. -L./Release/ Demo.c -llibCanvas -o ./Release/Demo.exe
//Demo.c: libCanvas test.
//Remember to re-compile after major changes!
#include <stdio.h>
#include "libCanvas.h"

void ClickCB(PCVS pC, int x, int y) {
  printf("%d %d\n", x, y);
}

void DblClickCB(PCVS pC, int x, int y) {
  cvsPrint(pC, x, y, "XXX", 0, QBColor(0xF), 0);
  cvsRefresh(pC);
}

int main(void) {
  PCVS pC = cvsCreate(256, 256, "Hello libCanvas!", 64, 64);
  cvsClear(pC, QBColor(0xA));
  cvsSetPx(pC, 128, 128, RGB(0, 0, 255));
  cvsLine(pC, 0, 64, 64, 0, QBColor(9));
  cvsRect(pC, 96, 96, 16, 16, QBColor(8), 0);
  cvsRect(pC, 96, 128, 16, 16, QBColor(8), 1);
  cvsPrint(pC, 0, 0, "Line 1", 0, NOCOLOR, 0);
  cvsPrint(pC, 0, 0, "Line 2", 0, QBColor(0xF), 1);
  pC->onClick = (PMOUSECB)ClickCB;
  pC->onDblClick = (PMOUSECB)DblClickCB;
  pC->EnableEvents = 1;
  cvsRefresh(pC);
  cvsShow(pC, 1);
  getchar();
  cvsIOInit();
  cvsSave(pC, L".\\Hello.png", L"image/png");
  cvsClear(pC, 0);
  cvsRefresh(pC);
  getchar();
  cvsLoad(pC, L".\\Hello.png");
  cvsRefresh(pC);
  getchar();
  cvsIOUninit();
  cvsShow(pC, 0);
  cvsDestroy(pC);
  return 0;
}
