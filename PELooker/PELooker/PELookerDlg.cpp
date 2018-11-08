
// PELookerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PELooker.h"
#include "PELookerDlg.h"
#include <strsafe.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int dpix;
int dpiy;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPELookerDlg dialog




CPELookerDlg::CPELookerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPELookerDlg::IDD, pParent)
    , m_FilePathShow(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPELookerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST, m_List);
    DDX_Text(pDX, IDC_EDIT1, m_FilePathShow);
}

BEGIN_MESSAGE_MAP(CPELookerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CPELookerDlg message handlers

BOOL CPELookerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

    CRect tabRect;
    GetWindowRect(&tabRect);
    CPaintDC dc(this);
    dpix = GetDeviceCaps(dc.m_hDC,LOGPIXELSX);
    dpiy = GetDeviceCaps(dc.m_hDC,LOGPIXELSY);
    tabRect.bottom += (LONG)(1+21*(dpiy/96.0));
    MoveWindow(&tabRect);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPELookerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPELookerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPELookerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CPELookerDlg::LoadFileData(WCHAR* wzFilePath,CHAR** szFileData,ULONG* ulLow)
{
    HANDLE hFile;
    ULONG ulHigh = 0;
    ULONG ulReturn = 0;

    if (wzFilePath==NULL)
    {
        return FALSE;
    }

    hFile = CreateFileW(wzFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

    if (hFile==INVALID_HANDLE_VALUE)
    {
        return -1;
    }

    *ulLow = GetFileSize(hFile,&ulHigh); 

    *szFileData = new char[*ulLow + 20];

    if (ReadFile(hFile,*szFileData,*ulLow,&ulReturn,NULL)==0)
    {
        CloseHandle(hFile);
        delete *szFileData;
        return FALSE;
    }

    CloseHandle(hFile);
    return TRUE;
}

BOOL CPELookerDlg::IsPEFile(CHAR* szFileData,PETYPE* PeType)
{
    PIMAGE_DOS_HEADER     DosHead =NULL;
    PIMAGE_NT_HEADERS     NtHead = NULL;
    IMAGE_FILE_HEADER     FileHead = {0};

    DosHead = (PIMAGE_DOS_HEADER)szFileData;
    if (DosHead->e_magic!=IMAGE_DOS_SIGNATURE) //MZ
    {
        return FALSE;
    }

    NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

    if (NtHead->Signature!=IMAGE_NT_SIGNATURE) //PE
    {
        return FALSE;
    }

    FileHead = NtHead->FileHeader;
    switch(FileHead.Machine)   //根据NT头中的FileHeader.Machine判断是哪种PE文件
    {
    case IMAGE_FILE_MACHINE_IA64:
    case IMAGE_FILE_MACHINE_AMD64:
        *PeType = PE64;
        break;

    case IMAGE_FILE_MACHINE_I386:
        *PeType = PE;
        break;

    default: *PeType = Unkonw;
    }

    return TRUE;
}

typedef struct _COLUMNSTRUCT
{
    WCHAR*        szTitle;
    UINT                nWidth;
}COLUMNSTRUCT;
UINT g_Column_Count_SectionTable  = 6;    
COLUMNSTRUCT g_Column_SectionTableList[] =
{
    { L"Name",            80 },    //ldr
    { L"RVA",            120 },    //传递
    { L"内存区块大小(未对齐)",            150 },    //
    {L"共享段", 150}
};


void CPELookerDlg::OnDropFiles(HDROP hDropInfo)
{
    // TODO: Add your message handler code here and/or call default

    DragQueryFile(hDropInfo,0,m_FilePath,MAX_PATH);//取得第一个文件的路径

    UpdateData(TRUE);

    PETYPE PeType = Unkonw;
    //拷贝文件到内存中
    if(LoadFileData(m_FilePath,&m_szFileData,&m_ulLow)==FALSE)
    {
        free(m_szFileData);
        CDialog::OnDropFiles(hDropInfo);
        return ;
    }
    if(IsPEFile(m_szFileData,&PeType)==FALSE)
    {
        MessageBox(L"不是PE文件",L"PECheck",0);
        CDialog::OnDropFiles(hDropInfo);
        return;
    }
    

    PIMAGE_NT_HEADERS                        NtHead = NULL;
    PIMAGE_NT_HEADERS64                    NtHeader64 = NULL;
    PIMAGE_NT_HEADERS32                    NtHeader32 = NULL;
    IMAGE_FILE_HEADER                        FileHeader;
    ULONG                                                    DataSize = 0;
    PIMAGE_SECTION_HEADER                SectionHeader;

    CString                                                        SectionName;
    CString                                                        strRVA;
    CString                                                        strRSize;

    CString                                                    strOffset;
    CString                                                    strVsize;

    while(m_List.DeleteColumn(0));
    m_List.DeleteAllItems();

    m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

    UINT    i = 0;
    for (i = 0;i<g_Column_Count_SectionTable;i++)
    {
        m_List.InsertColumn(i, g_Column_SectionTableList[i].szTitle,LVCFMT_LEFT,(int)(g_Column_SectionTableList[i].nWidth*(dpix/96.0)));
    }

    if (m_szFileData == NULL)
    {
        CDialog::OnDropFiles(hDropInfo);
        return;
    }

    PIMAGE_DOS_HEADER DosHead = (PIMAGE_DOS_HEADER)m_szFileData;

    //获得节表  
    if (PeType==PE)
    {
        NtHeader32 = (PIMAGE_NT_HEADERS32)((ULONG)m_szFileData + DosHead->e_lfanew);
        FileHeader = NtHeader32->FileHeader;
        SectionHeader =    (PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader32 + sizeof(IMAGE_NT_HEADERS32));  
    }
    else
    {
        NtHeader64 = (PIMAGE_NT_HEADERS64)((ULONG)m_szFileData + DosHead->e_lfanew);
        FileHeader =  NtHeader64->FileHeader;

        SectionHeader = (PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader64 + sizeof(IMAGE_NT_HEADERS64));  
    }


    for (int i = 0;i<FileHeader.NumberOfSections;i++,SectionHeader++)
    {
        CHAR szSectionName[50] = {0};
        HRESULT r = StringCchCopyA(szSectionName, IMAGE_SIZEOF_SHORT_NAME+1, (char*)SectionHeader->Name);

        SectionName = szSectionName;
        strRVA.Format(L"0x%x",SectionHeader->VirtualAddress);
        strRSize.Format(L"0x%x",SectionHeader->Misc.VirtualSize);

        BOOL isShare = 0;

        isShare = SectionHeader->Characteristics & IMAGE_SCN_MEM_SHARED;

        int n = m_List.GetItemCount();
        int j = m_List.InsertItem(n, SectionName);
        m_List.SetItemText(j, 1, strRVA);
        m_List.SetItemText(j, 2, strRSize);

        if(isShare)
        {
            m_List.SetItemText(j, 3, L"有");
        }
        else
        {
            m_List.SetItemText(j, 3, L"-");
        }

    }

    m_FilePathShow = m_FilePath;

    UpdateData(FALSE);

    CDialog::OnDropFiles(hDropInfo);
}
