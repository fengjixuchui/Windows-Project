// ADDFLIGHT.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "ADDFLIGHT.h"
#include "afxdialogex.h"
#include "Common.h"


// CADDFLIGHT 对话框

IMPLEMENT_DYNAMIC(CADDFLIGHT, CDialogEx)

CADDFLIGHT::CADDFLIGHT(CWnd* pParent /*=NULL*/)
	: CDialogEx(CADDFLIGHT::IDD, pParent)
	, m_FlightNum(_T(""))
	, m_Start(_T(""))
	, m_Arrive(_T(""))
	, m_StartDate(COleDateTime::GetCurrentTime())
	, m_StartTime(COleDateTime::GetCurrentTime())
	, m_ArriveDate(COleDateTime::GetCurrentTime())
	, m_ArriveTime(COleDateTime::GetCurrentTime())
	, m_PersonNum(_T(""))
	, m_Price(_T(""))
{

}

CADDFLIGHT::~CADDFLIGHT()
{
}

void CADDFLIGHT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FlightNum, m_FlightNum);
	DDX_Text(pDX, IDC_EDIT_Start, m_Start);
	DDX_Text(pDX, IDC_EDIT_Arrive, m_Arrive);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER1, m_StartDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER2, m_StartTime);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER3, m_ArriveDate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER4, m_ArriveTime);
	DDX_Text(pDX, IDC_EDIT_PersonNum, m_PersonNum);
	DDX_Text(pDX, IDC_EDIT_Price, m_Price);
}


BEGIN_MESSAGE_MAP(CADDFLIGHT, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CADDFLIGHT::OnBnClickedButtonAdd)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CADDFLIGHT 消息处理程序




void CADDFLIGHT::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	if(m_FlightNum==L"")
	{
		MessageBox(L"请输入航班号",L"");
		return;
	}
	else if (m_FlightNum.GetLength()!=6)
	{
		MessageBox(L"请输入六位航班号");
		return;
	}
	else if(m_Start==L"")
		{
			MessageBox(L"请输入出发地",L"");
		}
		else if(m_Arrive==L"")
		{
			MessageBox(L"请输入目的地",L"");
			return;
		}
		else if(m_Price==L"")
		{
			MessageBox(L"请输入价格",L"");
			return;
		}
		else if(m_PersonNum==L"")
		{
			MessageBox(L"请输入满员人数",L"");
			return;
		}
	CCommon Common;
	int j = 0;
	int nDay = 0;
	Common.read();
	wsprintf(Common.Flight[Common.FlightCount].FightNum,L"%s",m_FlightNum);
	wsprintf(Common.Flight[Common.FlightCount].Start,L"%s",m_Start);
	wsprintf(Common.Flight[Common.FlightCount].Arrive,L"%s",m_Arrive);
	nDay = m_StartDate.GetDay();
	for(j=0;j<Common.FlightCount;j++)   
		while(wcscmp(Common.Flight[j].FightNum,Common.Flight[Common.FlightCount].FightNum)==0&&
			wcscmp(Common.Flight[j].Start,Common.Flight[Common.FlightCount].Start)==0&&
			wcscmp(Common.Flight[j].Arrive,Common.Flight[Common.FlightCount].Arrive)==0&&
			nDay == Common.Flight[j].nDay)   
		{   
			MessageBox(L"\n此航班已经存在!!!请核实后重新输入...",L"出错啦");
			//Common.Flight[FlightCount-1].FightNum;     //重新读取航班
			return;
		}  

	CDialogEx::OnOK();
}


HBRUSH CADDFLIGHT::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔


	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC||id==IDC_BUTTON_ADD)// || id == IDC_CHECK1)
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


BOOL CADDFLIGHT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CDialogEx::SetBackgroundImage(IDB_BITMAP_Flight);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
