#pragma once
#include "basewnd.h"

class CIeWnd :
	public CBaseWnd
{
public:
	CIeWnd(void);
	~CIeWnd(void);


public:
	virtual void PaintFrame(CDC* pDC);
	virtual void MoveControl();
	virtual void OnInitCommand();
	virtual void OnCreateControl();
			void OnInitContro();

//private:
	CHtmlCtrl m_htmlCtrl;
	CHtmlCtrl m_htmlAd;
	CHtmlCtrl m_htmlBuff;


public:
	afx_msg LRESULT OnViewCompleted(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnBufferExploere(WPARAM wParam,LPARAM lParam);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
};
