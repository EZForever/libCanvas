//IO.c: Canvas I/O with picture files.
#include "Hub.h"
#include <stdlib.h>

//GDI+ startup things.
GdiplusStartupInput GDIpInput;
ULONG_PTR GDIpToken;

//cvsIOInit & cvsIOUninit: Get prepared for I/O operations.
EXPORT void cvsIOInit(void) {
  GDIpInput.GdiplusVersion = 1;
  GdiplusStartup(&GDIpToken, &GDIpInput, NULL);
}
//
EXPORT void cvsIOUninit(void) {
  GdiplusShutdown(GDIpToken);
}

//GetEncoderClsid: Convert MIME type to encoder CLSID.
//From: https://docs.microsoft.com/zh-cn/windows/desktop/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
  UINT num = 0;  // number of image encoders
  UINT size = 0; // size of the image encoder array in bytes
  ImageCodecInfo* pImageCodecInfo = NULL;

  GetImageEncodersSize(&num, &size);
  if(size == 0) return -1;

  pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
  if(pImageCodecInfo == NULL) return -1;

  GetImageEncoders(num, size, pImageCodecInfo);
  for(UINT j = 0; j < num; ++j) {
    if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0) {
      *pClsid = pImageCodecInfo[j].Clsid;
      free(pImageCodecInfo);
      return j;
    }    
  }

  free(pImageCodecInfo);
  return -1;
}

//cvsSave & cvsLoad: Save/load current canvas image to/from a local file.
EXPORT void cvsSave(PCVS pC, wchar_t* SaveTo, wchar_t* MIMEType) {
  GpBitmap* pGDIBitmap = NULL;
  CLSID Encoder;
  GdipCreateBitmapFromHBITMAP(pC->hBitmap, 0, &pGDIBitmap);
  GetEncoderClsid(MIMEType, &Encoder);
  GdipSaveImageToFile(pGDIBitmap, SaveTo, &Encoder, NULL);
  GdipDisposeImage(pGDIBitmap);
}
//
EXPORT void cvsLoad(PCVS pC, wchar_t* LoadFrom) {
  GpBitmap* pGDIBitmap = NULL;
  GdipLoadImageFromFile(LoadFrom, &pGDIBitmap);

  GpGraphics* pGDIGraphics = NULL;
  GdipCreateFromHDC(pC->hDC, &pGDIGraphics);
  GdipDrawImageRectI(pGDIGraphics, pGDIBitmap, 0, 0, pC->w, pC->h);

  GdipDeleteGraphics(pGDIGraphics);
  GdipDisposeImage(pGDIBitmap);
}

//cvsGet & cvsPut: Save/load current canvas image to/from a COLORREF array.
EXPORT COLORREF* cvsGet(PCVS pC) {
  COLORREF* pA = (COLORREF*)malloc(pC->w * pC->h * sizeof(COLORREF));
  for(int i = 0; i < pC->h; i++)
    for(int j = 0; j < pC->w; j++)
      *(pA + i * pC->w + j) = GetPixel(pC->hDC, j, i);
}
//
EXPORT void cvsPut(PCVS pC, COLORREF* From) {
  for(int i = 0; i < pC->h; i++)
    for(int j = 0; j < pC->w; j++)
      SetPixel(pC->hDC, j, i, *(From + i * pC->w + j));
}

