#pragma once
#include "afxwin.h"


// CWelcome 对话框

class CWelcome : public CDialogEx
{
	DECLARE_DYNAMIC(CWelcome)

public:
	CWelcome(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWelcome();
	HICON m_hIcon;
// 对话框数据
	enum { IDD = IDD_DIALOG_WELCOME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Welcome;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPurse();
	afx_msg void OnBnClickedButtonRefunf();
	afx_msg void OnBnClickedButtonMymes();
	afx_msg void OnPaint();
	CStatic m_CWelcome;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
