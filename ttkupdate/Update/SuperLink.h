#ifndef __CSuperLink__H__
#define __CSuperLink__H__

#pragma once
#include "afxwin.h"

class CSuperLink :
	public CStatic
{
public:
    CSuperLink();
    virtual ~CSuperLink();
public:
	void OnInitCtrl();

	void SetLink(BOOL bGo);
	void SetTextStaticColor(COLORREF color);

	void SetBkGnd(CDC* pDC);
	void PaintBk(CDC* pDC);

    void SetURL(CString strURL);
    CString GetURL() const;
    void SetColours(COLORREF crLinkColour, COLORREF crVisitedColour, 
                    COLORREF crHoverColour = -1);
    COLORREF GetLinkColour() const;
    COLORREF GetVisitedColour() const;
    COLORREF GetHoverColour() const;

    void SetVisited(BOOL bVisited = TRUE);
    BOOL GetVisited() const;

    void SetLinkCursor(HCURSOR hCursor);
    HCURSOR GetLinkCursor() const;

    void SetUnderline(BOOL bUnderline = TRUE);
    BOOL GetUnderline() const;

    void SetAutoSize(BOOL bAutoSize = TRUE);
    BOOL GetAutoSize() const;

    HINSTANCE GotoURL(LPCTSTR url, int showcmd);

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
    virtual void PreSubclassWindow();

protected:
    void ReportError(int nError);
    LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);
    void PositionWindow();
    void SetDefaultCursor();


protected:
    COLORREF m_crLinkColour, m_crVisitedColour,m_strTextColor;     // Hyperlink colours
    COLORREF m_crHoverColour;                       // Hover colour
    BOOL     m_bOverControl;                        // cursor over control?
    BOOL     m_bVisited;                            // Has it been visited?
    BOOL     m_bUnderline;                          // underline hyperlink?
    BOOL     m_bAdjustToFit;                        // Adjust window size to fit text?
    CString  m_strURL;                              // hyperlink URL
    CFont    m_Font;                                // Underline font if necessary
    HCURSOR  m_hLinkCursor;                         // Cursor for hyperlink
    CToolTipCtrl m_ToolTip;                         // The tooltip

	CBitmap bmp, *pOldBitmap;
	CBitmap m_Bmp;
	CDC m_dcBk;

	BOOL m_bIsUrl;
	BOOL m_bOver,m_Tracking;

protected:
    //{{AFX_MSG(CSuperLink)
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    //}}AFX_MSG
    afx_msg void OnClicked();
    DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};

#endif