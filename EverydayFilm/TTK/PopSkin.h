#pragma once
#include <atlstr.h>

class CPopSkin : 
	public CWindowWnd,
	public INotifyUI
{
public:
    CPopSkin();

    LPCTSTR GetWindowClassName() const;

    UINT GetClassStyle() const;

    void OnFinalMessage(HWND /*hWnd*/);

    void Init();

	void SaveFile();

	void ReadFile();

    void Notify(TNotifyUI& msg);

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	BOOL MoveWindow(LPRECT rect,BOOL bRepaint = TRUE);

	BOOL MoveWidow(int x,int y,int width,int height,BOOL bRepaint = TRUE);

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    CPaintManagerUI m_pm;
	CHorizontalLayoutUI* m_pHLay;
	COptionUI* m_pOpt;
	COptionUI* m_pBlack,* m_pBlue,* m_pPurple;
	CStdString skn;
	CString m_sknSet;
	DWORD m_iSkin;
};
