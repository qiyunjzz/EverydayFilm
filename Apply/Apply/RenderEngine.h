#ifndef RENDERENGINE_H
#define RENDERENGINE_H
#pragma once
#include "StringPtrMap.h"


class CRenderEngine
{
public:
	CRenderEngine(void);
	~CRenderEngine(void);


public:

	TImageInfo* GetImage(LPCTSTR bitmap);
    TImageInfo* GetImageEx(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    TImageInfo* AddImage(LPCTSTR bitmap, LPCTSTR type = NULL, DWORD mask = 0);
    bool RemoveImage(LPCTSTR bitmap);
    void RemoveAllImages();

    bool DrawImageString(HDC hDC,const RECT& rcItem, const RECT& rcPaint,LPCTSTR pStrImage, LPCTSTR pStrModify = NULL);
    TImageInfo* LoadImage(STRINGorID bitmap, STRINGorID skn,STRINGorID type = (UINT) 0, DWORD mask = 0);  //º”‘ÿÕº∆¨µΩπ˛œ£±Ì
    void DrawImage(HDC hDC, HBITMAP hBitmap, const RECT& rc, const RECT& rcPaint, \
        const RECT& rcBmpPart, const RECT& rcCorners, bool alphaChannel, BYTE uFade = 255, bool hole = false);	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
	void DrawBorder(CDC* pDC,CRect rcPaint,int bSize=2,COLORREF color=RGB(255,255,255));
	void DrawBorder2(CDC* pDC,CRect rcPaint,int bSize=2,COLORREF color=RGB(255,255,255));
	void DrawIcon(CDC* pDC,UINT uId,CRect pDest);
	void DrawIcon(CDC* pDC,HICON* icon,CRect pDest);
	void DrawFillRect(CDC* pDC,CRect rect,COLORREF color);
	void DrawWindowText(CDC* pDC,CString str,CRect pDest,COLORREF color=RGB(255,255,255),CFont* mFont=NULL);
	void DrawLine(CDC* pDC,CPoint start,CPoint end,COLORREF color=RGB(0,0,0),int bordersize=1);

	void SetSkin(CString str=NULL);
public:
	CStringPtrMap m_mImageHash;
	CString m_skn;

private:
	CFont m_Font;
	int m_iFontSize;

};
extern CRenderEngine m_renderEngine;
#endif