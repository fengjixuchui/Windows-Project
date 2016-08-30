// Welcome.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Welcome.h"
#include "afxdialogex.h"
#include "ShowFlight.h"
#include "Return.h"
#include "Tickets.h"


// CWelcome 对话框

IMPLEMENT_DYNAMIC(CWelcome, CDialogEx)

CWelcome::CWelcome(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWelcome::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Welcome = _T("");
}

CWelcome::~CWelcome()
{
}

void CWelcome::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_WELCOME, m_Welcome);
	DDX_Control(pDX, IDC_STATIC_WELCOME, m_CWelcome);
}


BEGIN_MESSAGE_MAP(CWelcome, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Purse, &CWelcome::OnBnClickedButtonPurse)
	ON_BN_CLICKED(IDC_BUTTON_Refunf, &CWelcome::OnBnClickedButtonRefunf)
	ON_BN_CLICKED(IDC_BUTTON_MyMes, &CWelcome::OnBnClickedButtonMymes)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CWelcome 消息处理程序


BOOL CWelcome::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString  Welcome;
	 Welcome    +=L"     欢迎来到本航空预定系统！\r\n";
	 Welcome	+=L"       我们一定竭诚为您服务\r\n\r\n\r\n";
	Welcome  +=L"本系统由闵真，许心成，施媛媛制作，版权所有违者必究\r\n";
	m_Welcome = Welcome;
	UpdateData(FALSE);
	

	CDialogEx::SetBackgroundImage(IDB_BITMAP_Welcome);
	//m_CWelcome.ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CWelcome::OnBnClickedButtonPurse()
{
	// TODO: 在此添加控件通知处理程序代码
	CShowFlight showflight;
	showflight.DoModal();
}


void CWelcome::OnBnClickedButtonRefunf()
{
	// TODO: 在此添加控件通知处理程序代码
	CReturn RETURN;
	RETURN.DoModal();

}


void CWelcome::OnBnClickedButtonMymes()
{
	// TODO: 在此添加控件通知处理程序代码
	CTickets Ticket;
	Ticket.DoModal();
}


void CWelcome::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}



}


HBRUSH CWelcome::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔


	

	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC_WELCOME||id==IDC_BUTTON_MyMes||id==IDC_BUTTON_Purse||id==IDC_BUTTON_Refunf)// || id == IDC_CHECK1)
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
