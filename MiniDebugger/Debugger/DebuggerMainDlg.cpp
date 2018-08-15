
// DebuggerMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Debugger.h"
#include "DebuggerMainDlg.h"
#include "afxdialogex.h"
#include "PECheck.h"
#include "DispatchException.h"
#include "ShowDumpText.h"
#include "Flags.h"
#include "BreakPoint.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern HANDLE g_hProcess;
extern HANDLE g_hThread;
extern DWORD g_processID;
extern DWORD g_threadID;

CDebugger1Dlg *MainDlg;
ULONG OEP = 0;

//状态栏使用的数组
static UINT indicators[] =
{
	IDS_MESSAGE,
};
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
// CDebugger1Dlg 对话框
CDebugger1Dlg::CDebugger1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDebugger1Dlg::IDD, pParent)
	, m_Dump(_T(""))
	, m_Ebp(_T(""))
	, m_Esi(_T(""))
	, m_Edi(_T(""))
	, m_Eip(_T(""))
	, m_Esp(_T(""))
	, m_Ebx(_T(""))
	, m_Edx(_T(""))
	, m_Ecx(_T(""))
	, m_Eax(_T(""))
	, m_CF(_T(""))
	, m_AF(_T(""))
	, m_PF(_T(""))
	, m_ZF(_T(""))
	, m_SF(_T(""))
	, m_OF(_T(""))
	, m_DF(_T(""))
	, m_Text(_T(""))
	, m_CC(_T(""))
	, m_Dr(_T(""))
	, m_Mem(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CDebugger1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MAIN, m_MainText);
	DDX_Control(pDX, IDC_EDIT_DUMP, m_DumpText);
	DDX_Text(pDX, IDC_EDIT_DUMP, m_Dump);
	DDX_Text(pDX, IDC_EDIT_EBP, m_Ebp);
	DDX_Text(pDX, IDC_EDIT_ESI, m_Esi);
	DDX_Text(pDX, IDC_EDIT_EDI, m_Edi);
	DDX_Text(pDX, IDC_EDIT_EIP, m_Eip);
	DDX_Text(pDX, IDC_EDIT_ESP, m_Esp);
	DDX_Text(pDX, IDC_EDIT_EBX, m_Ebx);
	DDX_Text(pDX, IDC_EDIT_EDX, m_Edx);
	DDX_Text(pDX, IDC_EDIT_ECX, m_Ecx);
	DDX_Text(pDX, IDC_EDIT_EAX, m_Eax);
	DDX_Text(pDX, IDC_EDIT_CF, m_CF);
	DDX_Text(pDX, IDC_EDIT_AF, m_AF);
	DDX_Text(pDX, IDC_EDIT_PF, m_PF);
	DDX_Text(pDX, IDC_EDIT_ZF, m_ZF);
	DDX_Text(pDX, IDC_EDIT_SF, m_SF);
	DDX_Text(pDX, IDC_EDIT_OF, m_OF);
	DDX_Text(pDX, IDC_EDIT_DF, m_DF);
	DDX_Text(pDX, IDC_EDIT_MAIN, m_Text);
	DDX_Text(pDX, IDC_EDIT_CC, m_CC);
	DDX_Text(pDX, IDC_EDIT_DR, m_Dr);
	DDX_Text(pDX, IDC_EDIT_MEM, m_Mem);
}
BEGIN_MESSAGE_MAP(CDebugger1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CDebugger1Dlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_GoOn, &CDebugger1Dlg::OnBnClickedButtonGoon)
	ON_BN_CLICKED(IDC_BUTTON_CC, &CDebugger1Dlg::OnBnClickedButtonCc)
	ON_BN_CLICKED(IDC_BUTTON_Dr, &CDebugger1Dlg::OnBnClickedButtonDr)
	ON_BN_CLICKED(IDC_BUTTON_MEM, &CDebugger1Dlg::OnBnClickedButtonMem)
	ON_BN_CLICKED(IDC_BUTTON_SingleIn, &CDebugger1Dlg::OnBnClickedButtonSinglein)
	ON_BN_CLICKED(IDC_BUTTON_SingleOut, &CDebugger1Dlg::OnBnClickedButtonSingleout)
	ON_BN_CLICKED(IDC_BUTTON_SingleOver, &CDebugger1Dlg::OnBnClickedButtonSingleover)
END_MESSAGE_MAP()


// CDebugger1Dlg 消息处理程序

BOOL CDebugger1Dlg::OnInitDialog()
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


	MainDlg = this;
	CreatStatusBar();
	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CDebugger1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDebugger1Dlg::OnPaint()
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
HCURSOR CDebugger1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDebugger1Dlg::OnDropFiles(HDROP hDropInfo)
{
	DragQueryFile(hDropInfo,0,m_FilePath,MAX_PATH);//取得第一个文件的路径

	//拷贝文件到内存中
	if(LoadFileData(m_FilePath,&m_szFileData,&m_ulLow)==FALSE)
	{
		free(m_szFileData);
		return ;
	}
	if(IsPEFile(m_szFileData,&PeType)==FALSE)
	{
		MessageBox(L"不是PE文件",L"PECheck",0);

		return;
	}
	if(PeType==PE)
	{
		MessageBox(L"32位 MZ",L"MZ");   
	//	ULONG OepRVA = GetOEP(m_szFileData);
	//	ULONG Base = GetBase(m_szFileData);
	//	OEP = (ULONG_PTR)OepRVA + (ULONG_PTR)Base;  //获得进程OEP 之前想在OEP设断，
	//结果根本不会再这段下来，所以在进程创建事件到达的时候，对于结构体中的pInfo->lpStartAddress设置断点

		CreateDebuggerProcess(m_FilePath);
	}
	else if(PeType==PE64)
	{
		MessageBox(L"64位 MZ",L"MZ");  
		CDialog::OnDropFiles(hDropInfo);
	}
	else if(PeType==Unkonw)
	{
		MessageBox(L"未知",L"MZ");  
		CDialog::OnDropFiles(hDropInfo);
	}
}


void CDebugger1Dlg::OnBnClickedButtonClose()
{
	StopDebugSeesion();
}


void CDebugger1Dlg::OnEnChangeEditMain()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CDebugger1Dlg::OnBnClickedButtonGoon()
{
	ContinueDebugSession();
}


void CDebugger1Dlg::OnBnClickedButtonCc()
{
	UpdateData(TRUE);
	ULONG Address = 0;
	swscanf(m_CC.GetBuffer(),L"%p",&Address);  
	OnSoftBreakPoint(Address,SOFTTYPE_ONCE);
}


void CDebugger1Dlg::CreatStatusBar(void)
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))                    //创建状态条并设置字符资源的ID
	{
		return ;      
	}
	 CRect rc;
	 GetClientRect(rc);
	 m_wndStatusBar.MoveWindow(1,rc.bottom-20,rc.right,20);// 调整状态栏的位置和大小
	 m_wndStatusBar.SetPaneInfo(0,IDS_MESSAGE,SBPS_NORMAL,500); 
	//::GetWindowRect(m_wndStatusBar.m_hWnd,rc);             
	//m_wndStatusBar.MoveWindow(rc);   
}


void CDebugger1Dlg::OnBnClickedButtonDr()
{
	UpdateData(TRUE);
	//PointInfo* App = NULL;
	ULONG Address = 0;
	swscanf(m_Dr.GetBuffer(),L"%p",&Address);  
	SetHardwareBreakpoint(g_hThread,HWBRK_TYPE_CODE,HWBRK_SIZE_1,(void*)Address);
}


void CDebugger1Dlg::OnBnClickedButtonMem()
{
	// TODO: 在此添加控件通知处理程序代码
}


//单步
void CDebugger1Dlg::OnBnClickedButtonSinglein()
{
	// TODO: 在此添加控件通知处理程序代码

	SetTrapFlag();//设置TF位
	ContinueDebugSession();

}


void CDebugger1Dlg::OnBnClickedButtonSingleout()
{
	MoveOut();
	ContinueDebugSession();
}


void CDebugger1Dlg::OnBnClickedButtonSingleover()
{
	MoveOver();
	ContinueDebugSession();
}
