// Tickets.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Tickets.h"
#include "afxdialogex.h"
#include "Common.h"

// CTickets 对话框
IMPLEMENT_DYNAMIC(CTickets, CDialogEx)

CTickets::CTickets(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTickets::IDD, pParent)
	, m_Read(_T(""))
{

}

CTickets::~CTickets()
{
}

void CTickets::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_READ, m_Read);
}


BEGIN_MESSAGE_MAP(CTickets, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTickets 消息处理程序


BOOL CTickets::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString m_Temp = L"";
	WCHAR *szBuffer;
	WCHAR Temp[20] = {0};
	CCommon Common;
	Common.readPerson();
	szBuffer = Common.readUser();
	memcpy(Temp,szBuffer,40);
	for(int i=0;i<Common.PersonCount;i++)
	{
		if(wcscmp(Temp,Common.Passenger[i].UserName)==0)
		{
			m_Temp += L"订单号：";
			m_Temp +=Common.Passenger[i].OrderNum;
			m_Temp += L"\r\n";

			m_Temp += L"姓名:  ";
			m_Temp += Common.Passenger[i].PassengerName;
			m_Temp +=L"\r\n";

			m_Temp +=L"性别：  ";
			m_Temp +=Common.Passenger[i].Sex;
			m_Temp +=L"\r\n";

			m_Temp +=L"身份证号：  ";
			m_Temp +=Common.Passenger[i].IDCard;
			m_Temp +=L"\r\n";

			m_Temp +=L"航班号:  ";
			m_Temp +=Common.Passenger[i].FightNum;
			m_Temp +=L"\r\n";

			m_Temp +=L"票价:  ";
			m_Temp +=Common.Passenger[i].Price;
			m_Temp +=L"\r\n";

			m_Temp +=L"座位号:  ";
			m_Temp +=Common.Passenger[i].Seat;
			m_Temp +=L"\r\n";

			m_Temp +=L"出发地：";
			m_Temp +=Common.Passenger[i].Start;
			m_Temp +=L"        目的地:  ";
			m_Temp +=Common.Passenger[i].Arrive;
			m_Temp +=L"\r\n";

			m_Temp +=L"出发时间：  ";
			m_Temp +=Common.Passenger[i].StartTime;
			m_Temp +=L"    到达时间:  ";
			m_Temp +=Common.Passenger[i].ArriveTime;
			m_Temp +=L"\r\n";

			if(Common.Passenger[i].IsGet==FALSE)
			{
				m_Temp +=L"机票领取：未领取";
			}
			else
			{
				m_Temp +=L"机票领取：已领取";
			}
			m_Temp +=L"\r\n";
			m_Temp +=L"\r\n";
			m_Temp +=L"\r\n";


		}
		
	}
	//m_Tickets = m_Temp;
	if(m_Temp==L"")
	{
		m_Temp = L"无订单信息";
	}
	m_Read = m_Temp;
	UpdateData(FALSE);

	//CDialogEx::SetBackgroundImage(IDB_BITMAP_GRAND);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CTickets::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	
	return hbr;
}
