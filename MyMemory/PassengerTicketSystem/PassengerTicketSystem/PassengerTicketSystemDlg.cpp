
// PassengerTicketSystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "PassengerTicketSystemDlg.h"
#include "afxdialogex.h"
#include "Login.h"
#include "Flight.h"
#include "Common.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPassengerTicketSystemDlg 对话框




CPassengerTicketSystemDlg::CPassengerTicketSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPassengerTicketSystemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPassengerTicketSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPassengerTicketSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_COMPANY, &CPassengerTicketSystemDlg::OnBnClickedButtonCompany)
END_MESSAGE_MAP()


// CPassengerTicketSystemDlg 消息处理程序

BOOL CPassengerTicketSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	
	// TODO: 在此添加额外的初始化代码

//	CreateToolBar();

/*
	CButton* Temp; 
	HBITMAP  hBitmap;
	Temp =(CButton *)GetDlgItem(IDC_BUTTON_COMPANY);
	hBitmap =::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_COMPANY));
	Temp->SetBitmap(hBitmap);
*/
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPassengerTicketSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPassengerTicketSystemDlg::OnPaint()
{
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPassengerTicketSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
void CPassengerTicketSystemDlg::CreateToolBar(void)
{
	if (!m_ToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_ToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))  //创建一个工具条  加载资源
	{

		return;     
	}
	m_ToolBar.ModifyStyle(0, TBSTYLE_FLAT);    //Fix for WinXP
	m_ToolBar.LoadTrueColorToolBar
		(
		48,    //加载真彩工具条
		IDB_BITMAP_MAIN,
		IDB_BITMAP_MAIN,
		IDB_BITMAP_MAIN
		);  //和我们的位图资源相关联
	RECT rt,rtMain;
	GetWindowRect(&rtMain);   //得到整个窗口的大小
	rt.left=0;
	rt.top=0;
	rt.bottom=80;
	rt.right=rtMain.right-rtMain.left+10;
	m_ToolBar.MoveWindow(&rt,TRUE);

	m_ToolBar.SetButtonText(0,L"航空公司");     //在位图的下面添加文件
	m_ToolBar.SetButtonText(1,L"旅行社购票"); 
	m_ToolBar.SetButtonText(2,L"用户付款"); 
	m_ToolBar.SetButtonText(3,L"帮助"); 
	//m_ToolBar.SetButtonText(4,"文件管理"); 
	//m_ToolBar.SetButtonText(5,"语音管理"); 
	//m_ToolBar.SetButtonText(6,"视频管理"); 
	//m_ToolBar.SetButtonText(7,"服务管理"); 
	//m_ToolBar.SetButtonText(8,"注册表管理"); 
	//m_ToolBar.SetButtonText(9,"参数设置"); 
	//m_ToolBar.SetButtonText(10,"生成服务端"); 
	//m_ToolBar.SetButtonText(11,"帮助"); 
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);  //显示
}
*/




/*
void CPassengerTicketSystemDlg::SearchFromFlightNum()
{
	CHAR* p = "GT00";
	int i = 0;
	read();
	for(i=0;i<FlightCount;i++)
	{
		if(strcmp(Flight[i].FightNum,p)==0)
		{
			MessageBox(L"查找航班信息成功",L"MADK");
			//对话框显示航班信息
		
		}
	}

}

*/



void CPassengerTicketSystemDlg::OnBnClickedButtonCompany()
{
	// TODO: 在此添加控件通知处理程序代码
	CFlight Flight;
	Flight.DoModal();
}
