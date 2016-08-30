#pragma once


// COrder 对话框

class COrder : public CDialogEx
{
	DECLARE_DYNAMIC(COrder)

public:
	COrder(CWnd* pParent = NULL,WCHAR* Buffer=NULL);   // 标准构造函数
	virtual ~COrder();

// 对话框数据
	enum { IDD = IDD_DIALOG_ORDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Order;
	WCHAR* szBuffer;
	CString m_Temp;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
