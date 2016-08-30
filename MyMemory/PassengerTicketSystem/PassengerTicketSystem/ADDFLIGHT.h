#pragma once
#include "atlcomtime.h"


// CADDFLIGHT 对话框

class CADDFLIGHT : public CDialogEx
{
	DECLARE_DYNAMIC(CADDFLIGHT)

public:
	CADDFLIGHT(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CADDFLIGHT();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADDFLIGHT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_FlightNum;
	CString m_Start;
	CString m_Arrive;
	COleDateTime m_StartDate;
	COleDateTime m_StartTime;
	COleDateTime m_ArriveDate;
	COleDateTime m_ArriveTime;
	CString m_PersonNum;
	CString m_Price;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};
