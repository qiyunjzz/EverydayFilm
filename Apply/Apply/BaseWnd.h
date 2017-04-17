#ifndef BASEWND_H
#define BASEWND_H
#pragma once

#include "Header.h"

class CBaseWnd :
	public CWnd
{
public:
	CBaseWnd(void);
	~CBaseWnd(void);

	virtual void CreatePlayWnd();

	void SetBkGnd(CDC* pDC);
	CDC* GetBkGnd();
	virtual void PaintFrame(CDC* pDC);
	virtual void MoveControl();
	virtual void OnInitCommand();

	void SetupRegion(int border_offset[], int nSize);
	void SetTrans(int itrans);
public:
	//Member
	HICON m_hIcon;
	CFont m_fontTitle;

	vector<int> border_Newoffset;

	CDC m_dcBk;
    CBitmap bmp, *pOldBitmap;

	BOOL m_bMoveFirst;

public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

#endif