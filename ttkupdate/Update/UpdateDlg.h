// UpdateDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "IntroductionDlg.h"
#include "CheckDlg.h"
#include "DownLoadDlg.h"
#include "NewDlg.h"
#include "CompleteDlg.h"
#include "DownLoadControl.h"
#include <tlhelp32.h>
#include "FailDlg.h"
#include "vector"
using namespace std;


// CUpdateDlg 对话框
class CUpdateDlg : public CDialog
{
// 构造
public:
	CUpdateDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPDATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


public:
	afx_msg LRESULT OnExitDlg(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCheckDlgShow(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCheckNextDownLoad(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDownLoadNextComplete(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDownLoadComplete(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDownLoadAtOnce(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnDownLoadTryAgain(WPARAM wParam,LPARAM lParam);	
	afx_msg LRESULT OnDownLoadFail(WPARAM wParam,LPARAM lParam);	
	
	
// 实现
public:
	void SetTrans(int itrans);
	void OnInit();
	void OnMoveControl();
	BOOL GetFileExist(CString strFile);
	BOOL GetUpdateSection();
	BOOL MoveUpdateFile();
	BOOL CreateMulDirectory(CString strPath);
	BOOL DeleteBufferDirectory(CString strPath);
	CString GetExeVision(CString strExe);
	BOOL CompareVision(CString strUpdate,CString strExe);
	BOOL IsUpdateNow();   
	DWORD FindProcessID(CString szExe);
	BOOL CloseExe();
	BOOL LookUpProcessPriviege();


private:
	CBitmap m_bmpBk;
	CIntroductionDlg m_dlgIntroduction;
	//CCheckDlg m_dlgCheck;
	CNewDlg m_dlgNew;
	CCompleteDlg m_dlgComplete;
	CString m_strFilePath,m_strFileName;
	int m_downIndex;
	CString m_strExePath;

public:
	CStringArray m_strSectionName;
	CDownLoadDlg m_dlgDownLoad;
	CFailDlg m_dlgFail;
	CString m_strFile,m_strFileBuffer,m_strCompletedFileIni;
	CString m_strUrl;
	vector<CString> m_vectorFileName;
	CWinThread* pDownFileThread;

protected:
	HICON m_hIcon;

public:
	DECLARE_MESSAGE_MAP()
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
