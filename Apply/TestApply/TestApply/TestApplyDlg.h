// TestApplyDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CTestApplyDlg 对话框
class CTestApplyDlg : public CDialog
{
// 构造
public:
	CTestApplyDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTAPPLY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    CString GetExeModulePath(HINSTANCE hInstance=NULL);

private:
	TCHAR strTest[10][MAX_PATH];
	CString strVal;
	CString strPath;
	CEdit m_str;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();

	afx_msg void OnBnClickedButton9();
	afx_msg void OnStnClickedStatic1();

};
