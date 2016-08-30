#pragma once


// CTickets 对话框

class CTickets : public CDialogEx
{
	DECLARE_DYNAMIC(CTickets)

public:
	CTickets(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTickets();

// 对话框数据
	enum { IDD = IDD_DIALOG_TICKETS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_Read;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
