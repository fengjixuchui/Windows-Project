#pragma once


// CCompanyOrder 对话框

class CCompanyOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CCompanyOrder)

public:
	CCompanyOrder(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCompanyOrder();

// 对话框数据
	enum { IDD = IDD_DIALOG_CompanyOrder };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_CompanyOrder;
	virtual BOOL OnInitDialog();
};
