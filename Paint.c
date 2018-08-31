//Paint.c: Painting works.
#include "Hub.h"
#include <string.h> //For strlen().

//PRINT_NEWLINE: Height of default font.
#define PRINT_NEWLINE 16

//cvsRefresh: Force redraw.
EXPORT void cvsRefresh(PCVS pC) {
  RedrawWindow(pC->hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE); 
}

//cvsClear: Clear a canvas by covering color.
//BUG: Will leave a strip uncovered if canvas is too small(e.g. 128*128).
EXPORT void cvsClear(PCVS pC, COLORREF Color) {
  RECT CliArea;
  GetClientRect(pC->hWnd, &CliArea);
  SetRect(&CliArea, 0, 0, CliArea.right - CliArea.left, CliArea.bottom - CliArea.top);
  FillRect(pC->hDC, &CliArea, CreateSolidBrush(Color));
}

//cvsGetPx & cvsSetPx: Single pixel operation.
EXPORT COLORREF cvsGetPx(PCVS pC, int x, int y) {
  return GetPixel(pC->hDC, x, y);
}
//
EXPORT void cvsSetPx(PCVS pC, int x, int y, COLORREF Color) {
  SetPixel(pC->hDC, x, y, Color);
}

//cvsLine: Draw simple, 1px line.
EXPORT void cvsLine(PCVS pC, int x1, int y1, int x2, int y2, COLORREF Color) {
  HPEN hPen = CreatePen(PS_SOLID, 1, Color);
  HPEN hPenOrig = (HPEN)SelectObject(pC->hDC, hPen);
  MoveToEx(pC->hDC, x1, y1, NULL); LineTo(pC->hDC, x2, y2);
  SelectObject(pC->hDC, hPenOrig);
  DeleteObject(hPen);
}

//cvsRect: Draw simple rectangle, hollow or filled.
EXPORT void cvsRect(PCVS pC, int x, int y, int Width, int Height, COLORREF Color, unsigned char Fill) {
  RECT Area;
  SetRect(&Area, x, y, x + Width, y + Height);
  if(Fill) FillRect(pC->hDC, &Area, CreateSolidBrush(Color));
    else FrameRect(pC->hDC, &Area, CreateSolidBrush(Color));
}

//cvsPrint: Write text on canvas.
EXPORT void cvsPrint(PCVS pC, int x, int y, char* Text, COLORREF Color, COLORREF ColorBG, int LineNo) {
  SetTextColor(pC->hDC, Color);
  if(ColorBG == NOCOLOR) SetBkMode(pC->hDC, TRANSPARENT);
    else {SetBkColor(pC->hDC, ColorBG); SetBkMode(pC->hDC, OPAQUE);}
  TextOut(pC->hDC, x, y + LineNo * PRINT_NEWLINE, Text, strlen(Text));
}
