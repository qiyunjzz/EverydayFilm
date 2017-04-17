#ifndef SKINBTN_H
#define SKINBTN_H

#pragma once
#include "afxwin.h"
#include "RenderEngine.h"

class CSkinBtn :
	public CButton
{
public:
	CSkinBtn(void);
	~CSkinBtn(void);

//////// Message
	LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	LRESULT OnMouseHover(WPARAM wParam,LPARAM lParam);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	void DrawButton(CDC *pDC,int nIndex);
   
	LPCTSTR GetName();
    void SetName(LPCTSTR pStrImage);

	void SetTextColor(COLORREF TextColour);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

public:
	LPCTSTR GetToolTip();
    void SetToolTip(LPCTSTR pStrImage);

	LPCTSTR GetNormalImage();
    void SetNormalImage(LPCTSTR pStrImage);
    LPCTSTR GetHotImage();
    void SetHotImage(LPCTSTR pStrImage);
    LPCTSTR GetPushedImage();
    void SetPushedImage(LPCTSTR pStrImage);
    LPCTSTR GetFocusedImage();
    void SetFocusedImage(LPCTSTR pStrImage);
    LPCTSTR GetDisabledImage();
    void SetDisabledImage(LPCTSTR pStrImage);

	void SetShowText(BOOL bShow);	
	void PaintBk(CDC* pDC);
    void DrawText(CDC* hDC,int index=0);	
	void MoveButton(int x,int y,int cx,int cy,BOOL lRepaint=1);
    
private:
	CString m_sControlName;

	CString m_sNormalImage;
    CString m_sHotImage;
    CString m_sPushedImage;
    CString m_sFocusedImage;
    CString m_sDisabledImage; 
	CString m_sToolTip;

	CToolTipCtrl m_ToolTip;                         // The tooltip

	RECT m_rcItem;
    RECT m_rcPadding;
	RECT m_rcPaint;

	BOOL m_Tracking;
	BOOL m_bFirst,m_bFirstToolTip;
	BOOL m_bShowText;
	int m_nIndex;

	COLORREF m_strTextColour;                       // Hover colour
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#endif