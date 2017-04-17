#pragma once


// CFailDlg 对话框

class CFailDlg : public CDialog
{
	DECLARE_DYNAMIC(CFailDlg)

public:
	CFailDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFailDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_DOWNLOAD_FAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

private:
	CSuperLink m_linkSite;
	CFont    m_Font; 

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
