#ifndef STRINGPTRMAP_H
#define STRINGPTRMAP_H
#pragma once

typedef struct tagTFontInfo
{
    HFONT hFont;
    TEXTMETRIC tm;
} TFontInfo;

typedef struct tagTImageInfo
{
    HBITMAP hBitmap;
    int nX;
    int nY;
    bool alphaChannel;
} TImageInfo;

struct TITEM
{
    CString Key;
    LPVOID Data;
    struct TITEM* pNext;
};

class  CStringPtrMap
{
public:
    CStringPtrMap(int nSize = 83);
    ~CStringPtrMap();

    void Resize(int nSize = 83);
    LPVOID Find(LPCTSTR key) const;
    bool Insert(LPCTSTR key, LPVOID pData);
    LPVOID Set(LPCTSTR key, LPVOID pData);
    bool Remove(LPCTSTR key);
    int GetSize() const;
    LPCTSTR GetAt(int iIndex) const;
    LPCTSTR operator[] (int nIndex) const;

protected:
    TITEM** m_aT;
    int m_nBuckets;
};

class STRINGorID
{
public:
    STRINGorID(LPCTSTR lpString) : m_lpstr(lpString)
    { }
    STRINGorID(UINT nID) : m_lpstr(MAKEINTRESOURCE(nID))
    {}
    LPCTSTR m_lpstr;
};



/////////////////////////////////////////////////////////////////////////////
extern "C"
{
    extern unsigned char *stbi_load_from_memory(unsigned char const *buffer, int len, int *x, int *y, \
        int *comp, int req_comp);
};
//////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//
//
static COLORREF PixelAlpha(COLORREF clrSrc, double src_darken, COLORREF clrDest, double dest_darken)
{
    return RGB (GetRValue (clrSrc) * src_darken + GetRValue (clrDest) * dest_darken,		//GetRValue 该函数获取一个RGB颜色值中的红色强度值。
        GetGValue (clrSrc) * src_darken + GetGValue (clrDest) * dest_darken, 
        GetBValue (clrSrc) * src_darken + GetBValue (clrDest) * dest_darken);

}

static BOOL WINAPI AlphaBitBlt(HDC hDC, int nDestX, int nDestY, int dwWidth, int dwHeight, HDC hSrcDC, \
                        int nSrcX, int nSrcY, int wSrc, int hSrc, BLENDFUNCTION ftn)
{
    HDC hTempDC = ::CreateCompatibleDC(hDC);		//该函数创建一个与指定设备兼容的内存设备上下文环境（DC）。
    if (NULL == hTempDC)
        return FALSE;

    //Creates Source DIB  创建源的DIB
    LPBITMAPINFO lpbiSrc;  //指向bitmap结构信息的指针
    // Fill in the BITMAPINFOHEADER  填充BITMAPINFOHEADER结构
    lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
    lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    lpbiSrc->bmiHeader.biWidth = dwWidth;
    lpbiSrc->bmiHeader.biHeight = dwHeight;
    lpbiSrc->bmiHeader.biPlanes = 1;
    lpbiSrc->bmiHeader.biBitCount = 32;
    lpbiSrc->bmiHeader.biCompression = BI_RGB;
    lpbiSrc->bmiHeader.biSizeImage = dwWidth * dwHeight;
    lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
    lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
    lpbiSrc->bmiHeader.biClrUsed = 0;
    lpbiSrc->bmiHeader.biClrImportant = 0;

    COLORREF* pSrcBits = NULL;			//COLORREF值是用来指定一个RGB颜色。
    HBITMAP hSrcDib = CreateDIBSection (
        hSrcDC, lpbiSrc, DIB_RGB_COLORS, (void **)&pSrcBits,
        NULL, NULL);
	//CreateDIBSection 该功能为您提供了一个位图位值的位置的指针。
	//可以提供一个处理一个文件映射对象，该函数将使用它来创建位图，或者你可以让系统分配的内存位图。
	//pSrcBits 一个变量，它接收到的DIB位值的位置的指针的指针

    if ((NULL == hSrcDib) || (NULL == pSrcBits)) 
    {
        ::DeleteDC(hTempDC);
        return FALSE;
    }

    HBITMAP hOldTempBmp = (HBITMAP)::SelectObject (hTempDC, hSrcDib);
    ::StretchBlt(hTempDC, 0, 0, dwWidth, dwHeight, hSrcDC, nSrcX, nSrcY, wSrc, hSrc, SRCCOPY);
	//StretchBlt函数从源矩形的位图复制到目标矩形，拉伸或压缩位图，以适应目标矩形的尺寸，如果必要的。
    ::SelectObject (hTempDC, hOldTempBmp);

    //Creates Destination DIB  创建目的DIB
    LPBITMAPINFO lpbiDest;
    // Fill in the BITMAPINFOHEADER    //填充BITMAPINFOHEADER信息
    lpbiDest = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
    lpbiDest->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    lpbiDest->bmiHeader.biWidth = dwWidth;
    lpbiDest->bmiHeader.biHeight = dwHeight;
    lpbiDest->bmiHeader.biPlanes = 1;
    lpbiDest->bmiHeader.biBitCount = 32;
    lpbiDest->bmiHeader.biCompression = BI_RGB;
    lpbiDest->bmiHeader.biSizeImage = dwWidth * dwHeight;
    lpbiDest->bmiHeader.biXPelsPerMeter = 0;
    lpbiDest->bmiHeader.biYPelsPerMeter = 0;
    lpbiDest->bmiHeader.biClrUsed = 0;
    lpbiDest->bmiHeader.biClrImportant = 0;

    COLORREF* pDestBits = NULL;
    HBITMAP hDestDib = CreateDIBSection (
        hDC, lpbiDest, DIB_RGB_COLORS, (void **)&pDestBits,
        NULL, NULL);

    if ((NULL == hDestDib) || (NULL == pDestBits))
    {
        delete [] lpbiSrc;
        ::DeleteObject(hSrcDib);
        ::DeleteDC(hTempDC);
        return FALSE;
    }

    ::SelectObject (hTempDC, hDestDib);
    ::BitBlt (hTempDC, 0, 0, dwWidth, dwHeight, hDC, nDestX, nDestY, SRCCOPY);  //贴图
    ::SelectObject (hTempDC, hOldTempBmp);

    double src_darken;
    BYTE nAlpha;
					//像素PixelAlpha返回 RGB的值
    for (int pixel = 0; pixel < dwWidth * dwHeight; pixel++, pSrcBits++, pDestBits++)
    {
        nAlpha = LOBYTE(*pSrcBits >> 24);
        src_darken = (double) (nAlpha * ftn.SourceConstantAlpha) / 255.0 / 255.0;
        if( src_darken < 0.0 ) src_darken = 0.0;
        *pDestBits = PixelAlpha(*pSrcBits, src_darken, *pDestBits, 1.0 - src_darken);
    } //for

    ::SelectObject (hTempDC, hDestDib);
    ::BitBlt (hDC, nDestX, nDestY, dwWidth, dwHeight, hTempDC, 0, 0, SRCCOPY);
    ::SelectObject (hTempDC, hOldTempBmp);

    delete [] lpbiDest;
    ::DeleteObject(hDestDib);

    delete [] lpbiSrc;
    ::DeleteObject(hSrcDib);

    ::DeleteDC(hTempDC);
    return TRUE;
}

#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
