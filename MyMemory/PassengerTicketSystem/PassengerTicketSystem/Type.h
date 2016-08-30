#pragma once
#include "afxwin.h"


// CType 对话框

class CType : public CDialogEx
{
	DECLARE_DYNAMIC(CType)

public:
	CType(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CType();

// 对话框数据
	enum { IDD = IDD_DIALOG_Fix };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Type;
	CString m_Name;
	CString m_Old;
	CString m_New;
	BOOL bOk;
	afx_msg void OnBnClickedButtonSure();
	virtual BOOL OnInitDialog();
	CComboBox m_TYPE;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
