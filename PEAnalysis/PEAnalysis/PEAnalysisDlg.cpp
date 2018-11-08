
// PEAnalysisDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PEAnalysis.h"
#include "PEAnalysisDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


int dpix;
int dpiy;

CHAR*	g_szFileData = NULL;
ULONG	g_SelectTab = 0;
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


// CPEAnalysisDlg 对话框




CPEAnalysisDlg::CPEAnalysisDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPEAnalysisDlg::IDD, pParent)
	, m_EditFilePath(_T(""))
	, m_EditFileType(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_szFileData = NULL;
}

void CPEAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DRAGFILE, m_EditFilePath);
	DDX_Text(pDX, IDC_EDIT_FILETYPE, m_EditFileType);
	DDX_Control(pDX, IDC_TAB_MAIN, m_TabMain);
}

BEGIN_MESSAGE_MAP(CPEAnalysisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CPEAnalysisDlg::OnTcnSelchangeTabMain)
END_MESSAGE_MAP()


// CPEAnalysisDlg 消息处理程序

BOOL CPEAnalysisDlg::OnInitDialog()
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

	//初始化Tab键
	m_TabMain.InsertItem(0, _T("PE头"));           
	m_TabMain.InsertItem(1, _T("目录表"));   
	m_TabMain.InsertItem(2,_T("区块表"));
	m_TabMain.InsertItem(3,_T("导入表"));
	m_TabMain.InsertItem(4,_T("导出表"));
	m_TabMain.InsertItem(5,_T("重定向表"));

	//与两个自对话框关联
	m_PeHeaderDlg.Create(IDD_DIALOG_PEHEADER,GetDlgItem(IDC_TAB_MAIN));
	m_PeTableDlg.Create(IDD_DIALOG_TABLE,GetDlgItem(IDC_TAB_MAIN));


	CRect tabRect;
	GetWindowRect(&tabRect);
	CPaintDC dc(this);
	dpix = GetDeviceCaps(dc.m_hDC,LOGPIXELSX);
	dpiy = GetDeviceCaps(dc.m_hDC,LOGPIXELSY);
	tabRect.bottom += (LONG)(1+21*(dpiy/96.0));
	MoveWindow(&tabRect);

	m_TabMain.GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;                  
	tabRect.right -= 1;   
	tabRect.top += 25;   
	tabRect.bottom -= 1;   
	// 根据调整好的tabRect放置m_jzmDlg子对话框，并设置为显示   
	m_PeHeaderDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_PeTableDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPEAnalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPEAnalysisDlg::OnPaint()
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
HCURSOR CPEAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPEAnalysisDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UpdateData(TRUE);

	CRect tabRect;   
	m_TabMain.GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;                  
	tabRect.right -= 1;   
	tabRect.top += 25;   
	tabRect.bottom -= 1;   



	DragQueryFile(hDropInfo,0,m_FilePath,MAX_PATH);//取得第一个文件的路径

	if(_wcsicmp(m_FilePath,m_EditFilePath.GetBuffer()) == 0)
	{
		goto END;
	}

	m_EditFilePath = m_FilePath;

	if(LoadFileData(m_FilePath,&m_szFileData,&m_ulLow)==FALSE)
	{
		free(m_szFileData);
		goto END;
	}

	g_szFileData = m_szFileData;

	if(IsPEFile(m_szFileData,&PeType)==FALSE)
	{
		MessageBox(L"不是PE文件",L"PECheck",0);
		goto END;
	}

	switch(PeType)
	{
	case PE:
		{
			m_EditFileType.Format(L"32位 MZ (0x%p)",(ULONG_PTR)m_szFileData);            
			break;
		}

	case PE64:
		{
			m_EditFileType.Format(L"64位 MZ (0x%p)",(ULONG_PTR)m_szFileData);	
			break;
		}
	case Unkonw:
		{
			m_EditFileType = L"未知";
			break;
		}
	default:
		break;
	}

	m_PeHeaderDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_PeHeaderDlg.CheckPEHeader();

	UpdateData(FALSE);

END:
	CDialog::OnDropFiles(hDropInfo);
}


void CPEAnalysisDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	ULONG		m_SelectTab = 0;

	m_SelectTab = m_TabMain.GetCurSel();
	g_SelectTab = m_SelectTab;

	CRect tabRect;   
	m_TabMain.GetClientRect(&tabRect);    // 获取标签控件客户区Rect   
	// 调整tabRect，使其覆盖范围适合放置标签页   
	tabRect.left += 1;                  
	tabRect.right -= 1;   
	tabRect.top += 25;   
	tabRect.bottom -= 1;   

	switch(m_SelectTab)
	{
	case 0:
		{

			m_PeHeaderDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
			m_PeTableDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
			m_PeHeaderDlg.CheckPEHeader();

			break;
		}

	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		{
			m_PeHeaderDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);
			m_PeTableDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);

			m_PeTableDlg.ShowPeTable(m_szFileData,PeType);
			break;
		}


	}


	*pResult = 0;
}
