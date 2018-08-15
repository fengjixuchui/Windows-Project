
// DebuggerMainDlg.cpp : ʵ���ļ�
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

//״̬��ʹ�õ�����
static UINT indicators[] =
{
	IDS_MESSAGE,
};
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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
// CDebugger1Dlg �Ի���
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


// CDebugger1Dlg ��Ϣ�������

BOOL CDebugger1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


	MainDlg = this;
	CreatStatusBar();
	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDebugger1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDebugger1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDebugger1Dlg::OnDropFiles(HDROP hDropInfo)
{
	DragQueryFile(hDropInfo,0,m_FilePath,MAX_PATH);//ȡ�õ�һ���ļ���·��

	//�����ļ����ڴ���
	if(LoadFileData(m_FilePath,&m_szFileData,&m_ulLow)==FALSE)
	{
		free(m_szFileData);
		return ;
	}
	if(IsPEFile(m_szFileData,&PeType)==FALSE)
	{
		MessageBox(L"����PE�ļ�",L"PECheck",0);

		return;
	}
	if(PeType==PE)
	{
		MessageBox(L"32λ MZ",L"MZ");   
	//	ULONG OepRVA = GetOEP(m_szFileData);
	//	ULONG Base = GetBase(m_szFileData);
	//	OEP = (ULONG_PTR)OepRVA + (ULONG_PTR)Base;  //��ý���OEP ֮ǰ����OEP��ϣ�
	//���������������������������ڽ��̴����¼������ʱ�򣬶��ڽṹ���е�pInfo->lpStartAddress���öϵ�

		CreateDebuggerProcess(m_FilePath);
	}
	else if(PeType==PE64)
	{
		MessageBox(L"64λ MZ",L"MZ");  
		CDialog::OnDropFiles(hDropInfo);
	}
	else if(PeType==Unkonw)
	{
		MessageBox(L"δ֪",L"MZ");  
		CDialog::OnDropFiles(hDropInfo);
	}
}


void CDebugger1Dlg::OnBnClickedButtonClose()
{
	StopDebugSeesion();
}


void CDebugger1Dlg::OnEnChangeEditMain()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		sizeof(indicators)/sizeof(UINT)))                    //����״̬���������ַ���Դ��ID
	{
		return ;      
	}
	 CRect rc;
	 GetClientRect(rc);
	 m_wndStatusBar.MoveWindow(1,rc.bottom-20,rc.right,20);// ����״̬����λ�úʹ�С
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


//����
void CDebugger1Dlg::OnBnClickedButtonSinglein()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	SetTrapFlag();//����TFλ
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
