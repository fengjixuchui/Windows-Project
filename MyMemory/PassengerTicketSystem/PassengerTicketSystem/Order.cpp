// Order.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Order.h"
#include "afxdialogex.h"
#include "Common.h"


// COrder 对话框

IMPLEMENT_DYNAMIC(COrder, CDialogEx)

COrder::COrder(CWnd* pParent /*=NULL*/,WCHAR* Buffer)
	: CDialogEx(COrder::IDD, pParent)
	, m_Order(_T(""))
{
	szBuffer = Buffer;
	m_Temp = L"";
}

COrder::~COrder()
{
}

void COrder::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ORDER, m_Order);
}


BEGIN_MESSAGE_MAP(COrder, CDialogEx)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// COrder 消息处理程序


BOOL COrder::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CCommon Common;
	Common.readPerson();
	
	for(int i =0;i<Common.PersonCount;i++)
	{
		if(wcscmp(szBuffer,Common.Passenger[i].OrderNum)==0)
		{
			m_Temp +=L"             ******TICKET*****";
			m_Temp += L"\r\n";

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
		}
	}

	
	SetTimer(0,0,NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COrder::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_Order = m_Temp;
	UpdateData(FALSE);
	KillTimer(0);

	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH COrder::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔



	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_EDIT_ORDER)// || id == IDC_CHECK1)
	{
		pDC->SetBkMode(TRANSPARENT);

		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CDC* dc = GetDC();

		pDC->BitBlt(0,0,rc.Width(),rc.Height(),dc,rc.left,rc.top,SRCCOPY);	//把父窗口背景图片先画到按钮上

		ReleaseDC(dc);

		hbr = (HBRUSH) ::GetStockObject(NULL_BRUSH);
	}	
	return hbr;
}
