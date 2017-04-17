#pragma once
#include "c:\documents and settings\administrator\桌面\天天看\newfilmeveryday\directui\uilib_lib\uibase.h"

class CMyMenu :	public CWindowWnd,public INotifyUI
{
public:

	CMyMenu(void);
	~CMyMenu(void);
	LPCTSTR GetWindowClassName() const { return _T("UIMenuSkin"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	//显示之前
	void OnPrepare()
	{
	}
	void Init();
	void Notify(TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMoveWindow(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CPaintManagerUI* m_pm;
	CVerticalLayoutUI *m_pMenuIE,*m_pMenuMain,*m_pMenuList,*m_pTray,*m_pMenuPlayer;
	CStdString skn;
	CButtonUI	*go,
	*del,		*set,
	*one,		*brk,
	*back,		*set2,
	*sort,		*open,
	*copy,		*play,
	*rand,		*paste,
	*about,		*reflash,
	*about2,	*breakall,
	*offsite,	*findfile,
	*openurl,	*autopoweroff,
	*onecycle,  *picset,
	*connect,	*sortcycle,
	*connall,	*showmain,
	*offsite2,  *quit,*quit2,
	*offsite3,  *ar,*noar,*ar43,*ar169,
	//sub menus
	*playpause,*stop,
	*left,*right,
	*full,*list,
	*playup,*set3;
	HWND hParent;
	void CheckButton(CButtonUI* pButton, bool check);
};
