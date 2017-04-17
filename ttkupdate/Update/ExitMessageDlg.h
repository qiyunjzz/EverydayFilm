#pragma once


// CExitMessageDlg 对话框

class CExitMessageDlg : public CDialog
{
	DECLARE_DYNAMIC(CExitMessageDlg)

public:
	CExitMessageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExitMessageDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EXITMESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnCancel();
};
