// Person.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Person.h"
#include "afxdialogex.h"
#include "Common.h"

// CPerson 对话框

IMPLEMENT_DYNAMIC(CPerson, CDialogEx)

CPerson::CPerson(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPerson::IDD, pParent)
{

	m_Seat = _T("");
	m_Arrive = _T("");
	m_ID = _T("");
	m_Name = _T("");
	m_Start = _T("");
	m_Time = _T("");
	m_WorkPlace = _T("");
	m_Rate = COleDateTime::GetCurrentTime();
}

CPerson::~CPerson()
{
}

void CPerson::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_FlightNum, m_FlightNum);
	DDX_Control(pDX, IDC_COMBO_Sex, m_Sex);
	//  DDX_Control(pDX, IDC_EDIT_Arrive, m_Arrive);
	//  DDX_Control(pDX, IDC_EDIT_ID, m_ID);
	//  DDX_Control(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_Seat, m_Seat);
	DDX_Text(pDX, IDC_EDIT_Arrive, m_Arrive);
	DDX_Text(pDX, IDC_EDIT_ID, m_ID);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_Start, m_Start);
	//DDX_Text(pDX, IDC_EDIT_Time, m_Time);
	DDX_Text(pDX, IDC_EDIT_WorkPlace, m_WorkPlace);
	//  DDX_Control(pDX, IDC_DATETIMEPICKER_Rate, m_Rate);
	DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_Rate, m_Rate);
	DDX_Control(pDX, IDC_EDIT_Seat, m_SEAT);
}


BEGIN_MESSAGE_MAP(CPerson, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BUY, &CPerson::OnBnClickedButtonBuy)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPerson 消息处理程序


BOOL CPerson::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_Sex.AddString(L"男");
	m_Sex.AddString(L"女");
	m_Sex.SetWindowText(L"男");

	m_FlightNum.AddString(L"MF8484"); //武汉-福州
	m_FlightNum.AddString(L"CA8211"); //武汉-西安
	m_FlightNum.AddString(L"MF8235");
	m_FlightNum.SetWindowText(L"MF8484");//西安-福州

	m_FlightNum.ShowWindow(FALSE);
	m_SEAT.ShowWindow(FALSE);

	bOk = FALSE;

	//CDialogEx::SetBackgroundImage(IDB_BITMAP_USER1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPerson::OnBnClickedButtonBuy()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	bOk = FALSE;
	if(m_Name==L"")
	{
		MessageBox(L"请输入姓名",L"出错啦");
		return;
	}
	if(m_WorkPlace==L"")
	{

		MessageBox(L"请输入工作单位",L"出错啦");
		return;
	}

	if(m_ID.GetLength()!=18)
	{
		MessageBox(L"请输入正确的身份证号码",L"出错啦");
		return ;
	}
	
	if(m_Start==L"")
	{
		MessageBox(L"请输入出发地",L"出错啦");
		return;
	}
	if(m_Arrive==L"")
	{

		MessageBox(L"请输入目的地",L"出错啦");
		return;
	}
	m_Sex.GetWindowText(Temp);
	
	
	CCommon Common;
	int j = 0;
	int nDay = 0;
	Common.read();
	Common.readPerson();
	wsprintf(Common.Flight[Common.FlightCount].Start,L"%s",m_Start);
	wsprintf(Common.Flight[Common.FlightCount].Arrive,L"%s",m_Arrive);
	wsprintf(Common.Passenger[Common.PersonCount].Start,L"%s",m_Start);
	wsprintf(Common.Passenger[Common.PersonCount].Arrive,L"%s",m_Arrive);
	wsprintf(Common.Passenger[Common.PersonCount].PassengerName,L"%s",m_Name);
	nDay = m_Rate.GetDay();
	for(j=0;j<Common.PersonCount;j++)
	{
		if(wcscmp(Common.Passenger[j].Start,Common.Passenger[Common.PersonCount].Start)==0&&
			wcscmp(Common.Passenger[j].Arrive,Common.Passenger[Common.PersonCount].Arrive)==0&&
			nDay == Common.Passenger[j].nDay&&wcscmp(Common.Passenger[j].PassengerName,Common.Passenger[Common.PersonCount].PassengerName)==0)
		{
			MessageBox(L"对不起，一个航班每个人不能同时订两张票",L"");
			return;
		}
	}
	for(j=0;j<Common.FlightCount;j++)   
		if(wcscmp(Common.Flight[j].Start,Common.Flight[Common.FlightCount].Start)==0&&
			wcscmp(Common.Flight[j].Arrive,Common.Flight[Common.FlightCount].Arrive)==0&&
			nDay == Common.Flight[j].nDay)   
		{   
			//MessageBox(L"\n此航班已经存在!!!请核实后重新输入...",L"出错啦");
			//Common.Flight[FlightCount-1].FightNum;     //重新读取航班
			bOk = TRUE;
		}  


	
	if(bOk==FALSE)
	{
		MessageBox(L"没有相应时间的航班，请查询后在订票",L"");
		return;
	}
	
	CDialogEx::OnOK();
	
	
}


HBRUSH CPerson::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔


	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC||id==IDC_BUTTON_BUY)// || id == IDC_CHECK1)
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
