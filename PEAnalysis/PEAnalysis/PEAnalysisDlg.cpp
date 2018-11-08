
// PEAnalysisDlg.cpp : ʵ���ļ�
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


// CPEAnalysisDlg �Ի���




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


// CPEAnalysisDlg ��Ϣ�������

BOOL CPEAnalysisDlg::OnInitDialog()
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

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��ʼ��Tab��
	m_TabMain.InsertItem(0, _T("PEͷ"));           
	m_TabMain.InsertItem(1, _T("Ŀ¼��"));   
	m_TabMain.InsertItem(2,_T("�����"));
	m_TabMain.InsertItem(3,_T("�����"));
	m_TabMain.InsertItem(4,_T("������"));
	m_TabMain.InsertItem(5,_T("�ض����"));

	//�������ԶԻ������
	m_PeHeaderDlg.Create(IDD_DIALOG_PEHEADER,GetDlgItem(IDC_TAB_MAIN));
	m_PeTableDlg.Create(IDD_DIALOG_TABLE,GetDlgItem(IDC_TAB_MAIN));


	CRect tabRect;
	GetWindowRect(&tabRect);
	CPaintDC dc(this);
	dpix = GetDeviceCaps(dc.m_hDC,LOGPIXELSX);
	dpiy = GetDeviceCaps(dc.m_hDC,LOGPIXELSY);
	tabRect.bottom += (LONG)(1+21*(dpiy/96.0));
	MoveWindow(&tabRect);

	m_TabMain.GetClientRect(&tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect   
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ   
	tabRect.left += 1;                  
	tabRect.right -= 1;   
	tabRect.top += 25;   
	tabRect.bottom -= 1;   
	// ���ݵ����õ�tabRect����m_jzmDlg�ӶԻ��򣬲�����Ϊ��ʾ   
	m_PeHeaderDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_SHOWWINDOW);
	m_PeTableDlg.SetWindowPos(NULL, tabRect.left, tabRect.top, tabRect.Width(), tabRect.Height(), SWP_HIDEWINDOW);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPEAnalysisDlg::OnPaint()
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
HCURSOR CPEAnalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPEAnalysisDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	UpdateData(TRUE);

	CRect tabRect;   
	m_TabMain.GetClientRect(&tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect   
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ   
	tabRect.left += 1;                  
	tabRect.right -= 1;   
	tabRect.top += 25;   
	tabRect.bottom -= 1;   



	DragQueryFile(hDropInfo,0,m_FilePath,MAX_PATH);//ȡ�õ�һ���ļ���·��

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
		MessageBox(L"����PE�ļ�",L"PECheck",0);
		goto END;
	}

	switch(PeType)
	{
	case PE:
		{
			m_EditFileType.Format(L"32λ MZ (0x%p)",(ULONG_PTR)m_szFileData);            
			break;
		}

	case PE64:
		{
			m_EditFileType.Format(L"64λ MZ (0x%p)",(ULONG_PTR)m_szFileData);	
			break;
		}
	case Unkonw:
		{
			m_EditFileType = L"δ֪";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ULONG		m_SelectTab = 0;

	m_SelectTab = m_TabMain.GetCurSel();
	g_SelectTab = m_SelectTab;

	CRect tabRect;   
	m_TabMain.GetClientRect(&tabRect);    // ��ȡ��ǩ�ؼ��ͻ���Rect   
	// ����tabRect��ʹ�串�Ƿ�Χ�ʺϷ��ñ�ǩҳ   
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
