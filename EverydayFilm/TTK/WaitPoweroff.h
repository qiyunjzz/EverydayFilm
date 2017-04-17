#pragma once
#include "uibase.h"

class CWaitPoweroff : public CWindowWnd, public INotifyUI
{
public:
	CWaitPoweroff(void);
	~CWaitPoweroff(void);
public:
	CPaintManagerUI* m_pm;
	CStdString skn;
	LPCTSTR GetWindowClassName() const { return _T("WaitPoweroff"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init();
	//显示之前
	void OnPrepare()
	{

	}
	void Notify(TNotifyUI& msg);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);;
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	CButtonUI*	m_btnClose,*m_btnPoweroffNow,*m_btnNoPoweroff;
	CLabelUI* m_timeLabel;
	int seconds;//几秒钟后关机
	void PowerOff(BOOL bPoweroff=TRUE);
	void PowerOffStartup(int SecondsLater);
	void ShutDownPoweroff(void);
	void ShotdownNow(void);
};
