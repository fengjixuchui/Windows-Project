#pragma once
#include "afxcmn.h"


// CReturn 对话框

class CReturn : public CDialogEx
{
	DECLARE_DYNAMIC(CReturn)

public:
	CReturn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReturn();

// 对话框数据
	enum { IDD = IDD_DIALOG_RETURN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDelperson();
	afx_msg void OnBnClickedButton1();
	BOOL bOk;
};
