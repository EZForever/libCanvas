//libCanvas.h: TODO: Description
#ifndef __LIBCANVAS_H
#define __LIBCANVAS_H
#include <windows.h>

/*  NOCOLOR
  Indicate a transparent background color for cvsPrint().
*/
#define NOCOLOR ((COLORREF)-1)

/*  PMOUSECB
  Function prototype for mouse-related callbacks.
*/
typedef void (*PMOUSECB)(void* pC, int x, int y);

/*  CVS, PCVS
  Describe a canvas created by cvsCreate().
*/
typedef struct _CVS {
  //These are from user when calling cvsCreate().
  int w; int h;
  char* title;
  int x; int y;
  //These are filled by cvsCreate().
  DWORD hThread;
  HWND hWnd;
  HDC hDC;
  HBITMAP hBitmap;
  //These are optional; leave NULL if not need these.
  //Prototype: void ClickCB(PCVS pC, int x, int y) {}
  unsigned char EnableEvents;
  PMOUSECB onClick;
  PMOUSECB onDblClick;
} CVS, *PCVS;

/*  QBColor
  QBasic console colors.
  Use like the same function in Visual Basic.
*/
COLORREF QBColor(unsigned int Code);

/*  cvsCreate
  Create a new, hidden canvas.
  Title can be NULL. If so, "libCanvas" will be used.
  If x or y < 0, the window position will be determined by system.
  Returns NULL on error.
*/
PCVS cvsCreate(int w, int h, char* title, int x, int y);

/*  canvasDestroy
  Destroy a created canvas.
*/
void cvsDestroy(PCVS pC);

/*  cvsShow
  Show or hide a canvas.
*/
void cvsShow(PCVS pC, unsigned char Show);

/*  cvsRefresh
  Force redraw.
*/
void cvsRefresh(PCVS pC);

/*  cvsClear
  Clear a canvas by covering color.
*/
void cvsClear(PCVS pC, COLORREF Color);

/*  cvsGetPx & cvsSetPx
  Single pixel operation.
*/
COLORREF cvsGetPx(PCVS pC, int x, int y);
void cvsSetPx(PCVS pC, int x, int y, COLORREF Color);

/*  cvsLine
  Draw simple, 1px line.
*/
void cvsLine(PCVS pC, int x1, int y1, int x2, int y2, COLORREF Color);

/*  cvsRect
  Draw simple rectangle, hollow or filled.
*/
void cvsRect(PCVS pC, int x, int y, int Width, int Height, COLORREF Color, unsigned char Fill);

/*  cvsPrint
  Write text on canvas.
  Use NOCOLOR on ColorBG for transparent background.
  LineNo usage: cvsPrint(pC, 0, 0, "Line 1", 0, NOCOLOR, 0); cvsPrint(pC, 0, 0, "Line 2", 0, NOCOLOR, 1);
*/
void cvsPrint(PCVS pC, int x, int y, char* Text, COLORREF Color, COLORREF ColorBG, int LineNo);

/*  cvsIOInit & cvsIOUninit
  Get prepared for I/O operations.
*/
void cvsIOInit(void);
void cvsIOUninit(void);

/*  cvsSave & cvsLoad
  Save/Load current canvas image to/from a local file.
  According to MSDN, these formats are supported:
     image/bmp, image/jpeg, image/gif, image/tiff, image/png
*/
void cvsSave(PCVS pC, wchar_t* SaveTo, wchar_t* MIMEType);
void cvsLoad(PCVS pC, wchar_t* LoadFrom);

/*  cvsGet & cvsPut
  Save/load current canvas image to/from a COLORREF array.
  Use free() when the dumped array is not needed.
*/
COLORREF* cvsGet(PCVS pC);
void cvsPut(PCVS pC, COLORREF* From);


#endif //__LIBCANVAS_H
