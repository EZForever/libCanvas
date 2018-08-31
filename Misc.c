//Misc.c: Some misc functions.
#include "Hub.h"

//Color table for QBColor().
const COLORREF QBColors[16] = {
  0x000000, 0xAA0000, 0x00AA00, 0xAAAA00,
  0x0000AA, 0xAA00AA, 0x00AAFF, 0xAAAAAA,
  0x555555, 0xFF5555, 0x55FF55, 0xFFFF55,
  0x5555FF, 0xFF55FF, 0x55FFFF, 0xFFFFFF
};

//QBColor: QBasic console colors.
EXPORT COLORREF QBColor(unsigned int Code) {
  return QBColors[Code % 16];
}

//DllMain: Called when loading / unloading libCanvas.
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
  if(fdwReason == DLL_PROCESS_ATTACH) {
    //Register canvas class.
    WNDCLASS WndCls;
    memset(&WndCls, 0, sizeof(WndCls));
    WndCls.hInstance     = hInstDLL;
    WndCls.lpszClassName = CLSNAME;
    WndCls.lpfnWndProc   = (WNDPROC)WndProc;
    WndCls.style         = CS_SAVEBITS | CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
    WndCls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WndCls.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    WndCls.hCursor       = LoadCursor(NULL, IDC_CROSS /*IDC_ARROW*/);
    return RegisterClass(&WndCls);
  }
  return TRUE;
}
