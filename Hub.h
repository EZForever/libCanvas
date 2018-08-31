//Hub.h: Some universal definitions.
#ifndef __HUB_H
#define __HUB_H

#include "libCanvas.h"
#include <gdiplus/gdiplus.h>

//Export marker for a function.
#define EXPORT __declspec(dllexport)

//Window class of canvas windows.
extern const char* CLSNAME;

//Internal procs.
DWORD WINAPI WndThread(LPVOID lpParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif //__HUB_H
