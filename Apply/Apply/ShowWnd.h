#ifndef SHOWWND_H
#define SHOWWND_H
#pragma once
#include "basewnd.h"
#include "IeWnd.h"

class CShowWnd :
	public CBaseWnd
{
public:
	CShowWnd(void);
	~CShowWnd(void);


public:
	virtual void PaintFrame(CDC* pDC);
	virtual void MoveControl();
	virtual void OnInitCommand();
	virtual void OnCreateControl();
	virtual void CreatePlayWnd();
			void OnInitContro();

private:
	//number btn
	CSkinBtn m_btnClose;
	CSkinBtn m_btnMin;
	CSkinBtn m_btnTopMost;
	CSkinBtn m_btnNoTopMost;
	CSkinBtn m_btnFeedBack;

	CIeWnd m_wndIe;

public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedMin();
	afx_msg void OnBnClickedNoTopMost();
	afx_msg void OnBnClickedTopMost();
	afx_msg void OnBnClickedFeedBack();

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
	afx_msg void OnClose();
};

#endif