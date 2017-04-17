#pragma once
#include <vector>
#include <stack>
#include "uibase.h"
#define WM_MYMESSAGE WM_USER+1234

class CFileManager : public CWindowWnd, public INotifyUI
{
public:
	CFileManager(void);
	~CFileManager(void);
public:
	CPaintManagerUI* m_pm;
	CStdString skn;
	CStdString m_qvodIni;
	CStdString m_cfgPath;
	LPCTSTR GetWindowClassName() const { return _T("FileManager"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init();
	//œ‘ æ÷Æ«∞
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
	void ErgodicDir(CStdString path,__int64& FullSize,int& FileNum);
	LRESULT OnCheckOver(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);



	CButtonUI*	m_btnClose,*m_btnOpenpath,*m_btnNomore,*m_btnCancel;
	CLabelUI* m_Label;
};
