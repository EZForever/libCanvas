//Window.c: Controls the canvas windows.
#include "Hub.h"
#include <stdlib.h> //malloc() & free()
#include <windowsx.h> //For GET_X_LPARAM & GET_Y_LPARAM

//Window class of canvas windows.
const char* CLSNAME = "libCanvas";

//cvsCreate: Create a new, hidden canvas.
EXPORT PCVS cvsCreate(int w, int h, char* title, int x, int y) {
  PCVS pC = (PCVS)malloc(sizeof(CVS));
  if(!pC) return NULL;
  memset(pC, 0, sizeof(CVS));
  pC->w = w; pC->h = h; //Fill necessary data in new canvas.
  pC->title = title;
  pC->x = x; pC->y = y;
  //pC->hWnd = 0; //memset() did this.
  CreateThread(NULL, 0, WndThread, pC, 0, &pC->hThread);
  while(pC->hWnd <= 0) { //Wait for CreateWindow() in new thread.
    if(pC->hWnd < 0) {free(pC); return NULL;} //CreateWindow failed.
      else Sleep(100);
  }
  return pC;
}

//cvsDestroy: Destroy a created canvas.
EXPORT void cvsDestroy(PCVS pC) {
  pC->hWnd = NULL; //This informs WndThread to do the rest things.
}

//cvsShow: Show or hide a canvas.
EXPORT void cvsShow(PCVS pC, unsigned char Show) {
  ShowWindow(pC->hWnd, Show ? SW_SHOW : SW_HIDE);
}

//WndThread: Proc running on every canvas thread.
DWORD WINAPI WndThread(LPVOID lpParam) {
  PCVS pC = (PCVS)lpParam;
  //Calculate actual window size.
  RECT CliArea; SetRect(&CliArea, 0, 0, pC->w, pC->h);
  AdjustWindowRect(&CliArea, WS_OVERLAPPED | WS_CAPTION, FALSE);
  //Create canvas window.
  pC->hWnd = CreateWindow(
    CLSNAME, pC->title ? pC->title : CLSNAME,
    WS_OVERLAPPED | WS_CAPTION,
    pC->x < 0 ? CW_USEDEFAULT : pC->x,
    pC->y < 0 ? CW_USEDEFAULT : pC->y,
    CliArea.right - CliArea.left,
    CliArea.bottom - CliArea.top,
    NULL, NULL, NULL, NULL
  );
  //If failed, inform cvsCreate() & end thread.
  if(!pC->hWnd) {pC->hWnd = (HWND)-1; return -1;}
  //Get hDC for drawing operations.
  HDC hDCWnd = GetDC(pC->hWnd);
  pC->hDC = CreateCompatibleDC(hDCWnd); 
  pC->hBitmap = CreateDiscardableBitmap(hDCWnd, pC->w, pC->h);
  SelectObject(pC->hDC, pC->hBitmap);
  //If failed, destroy canvas & end thread.
  if(!pC->hDC) {DestroyWindow(pC->hWnd); free(pC); return -1;}
  //Send canvas ptr to window.
  SetWindowLong(pC->hWnd, GWL_USERDATA, (LONG)pC);
  //Enter message loop.
  MSG msg;
  while(pC->hWnd && GetMessage(&msg, pC->hWnd, 0, 0) > 0) { //Wait for cvsDestory()
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  //Canvas Destruction.
  DeleteObject(pC->hBitmap);
  DeleteDC(pC->hDC);
  ReleaseDC(pC->hWnd, hDCWnd);
  DestroyWindow(pC->hWnd);
  free(pC);
  return 0;
}

//WndProc: Event processing.
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
  PCVS pC = (PCVS)GetWindowLong(hWnd, GWL_USERDATA); //Fetch canvas ptr from WndThread().
  switch(Msg) {
    case WM_PAINT: { //cvsRefresh() or system redraw.
      PAINTSTRUCT ps;
      HDC hDCWnd = BeginPaint(hWnd, &ps);
      BitBlt(hDCWnd, 0, 0, pC->w, pC->h, pC->hDC, 0, 0, SRCCOPY);
      EndPaint(hWnd, &ps);
      break;
    }
    case WM_LBUTTONDOWN: { //Click callback.
      if(pC->EnableEvents && pC->onClick) pC->onClick(pC, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;
    }
    case WM_LBUTTONDBLCLK: { //Double click callback.
      if(pC->EnableEvents && pC->onDblClick) pC->onDblClick(pC, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
      break;
    }
    default:
      return DefWindowProc(hWnd, Msg, wParam, lParam);
  }
  return 0;
}
