// CompanyOrder.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "CompanyOrder.h"
#include "afxdialogex.h"
#include "Common.h"

// CCompanyOrder 对话框

IMPLEMENT_DYNAMIC(CCompanyOrder, CDialogEx)

CCompanyOrder::CCompanyOrder(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCompanyOrder::IDD, pParent)
	, m_CompanyOrder(_T(""))
{

}

CCompanyOrder::~CCompanyOrder()
{
}

void CCompanyOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AAAA, m_CompanyOrder);
}


BEGIN_MESSAGE_MAP(CCompanyOrder, CDialogEx)
END_MESSAGE_MAP()


// CCompanyOrder 消息处理程序


BOOL CCompanyOrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CCommon Common;
	Common.readOrder();
	CString m_Temp = L"";
	for(int i =0;i<Common.OrderCount;i++)
	{
		
			m_Temp += L"订单号：";
			m_Temp +=Common.Order[i].OrderNum;
			m_Temp += L"     ";



			m_Temp +=L"航班号:  ";
			m_Temp +=Common.Order[i].FightNum;
			m_Temp +=L"     ";


			if(Common.Order[i].IsGet==FALSE)
			{
				m_Temp +=L"领票情况： 未领票";
				m_Temp +=L"\r\n";
			}
			else
			{
				m_Temp +=L"领票情况： 已领票";
				m_Temp +=L"\r\n";
			}
	}

	m_CompanyOrder = m_Temp;
	UpdateData(FALSE);

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
