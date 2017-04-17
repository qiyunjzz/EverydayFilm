#pragma once


// CCheckDlg 对话框

class CCheckDlg : public CDialog
{
	DECLARE_DYNAMIC(CCheckDlg)

public:
	CCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCheckDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CHECK };


public:
	void ShowWindowEx(BOOL m_bShow=TRUE);
	void ShowProcessCtrl();


public:
	CWinThread* pCheckThread;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCancel();
	virtual BOOL OnInitDialog();
};
