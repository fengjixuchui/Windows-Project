
// PassengerTicketSystemDlg.h : 头文件
//

#pragma once

typedef struct _USER_
{
	char LoginName[20];
	char Password[20];
}USER,*PUSER;

// CPassengerTicketSystemDlg 对话框
class CPassengerTicketSystemDlg : public CDialogEx
{
// 构造
public:
	CPassengerTicketSystemDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CPassengerTicketSystemDlg::read();
	void CPassengerTicketSystemDlg::save();
	void CPassengerTicketSystemDlg::SearchFromFlightNum();
	CHAR* iTypeUser;
//	CTrueColorToolBar m_ToolBar; 
	
	
//	void CPassengerTicketSystemDlg::CreateToolBar(void);
// 对话框数据
	enum { IDD = IDD_PASSENGERTICKETSYSTEM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnBnClickedButtonCompany();
};
