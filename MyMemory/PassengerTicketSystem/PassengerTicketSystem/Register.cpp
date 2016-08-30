// Register.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Register.h"
#include "afxdialogex.h"


// CRegister 对话框

IMPLEMENT_DYNAMIC(CRegister, CDialogEx)

CRegister::CRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegister::IDD, pParent)
{

	m_Name = _T("");
	m_Password = _T("");
}

CRegister::~CRegister()
{
}

void CRegister::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_USER, m_Name);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDX_Control(pDX, IDC_COMBO_USER, m_ComBoUser);
}


BEGIN_MESSAGE_MAP(CRegister, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRegister::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRegister 消息处理程序


void CRegister::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	char Name[20]={0};
	char PassWord[20]={0};
	wcstombs(Name,m_Name,20);
	wcstombs(PassWord,m_Password,20);
	int i =0;
	if(m_Name==L""||m_Password==L"")
	{
		MessageBox(L"请输入完整信息",L"出错啦");
		return;
	}
	CString Temp;
	m_ComBoUser.GetWindowText(Temp);
	if(Temp==L"航空公司注册")
	{
	//	MessageBox(L"a",L"s");
		Login.ReadCompany();
		for(i=0;i<Login.CompanyNum;i++)
		{
			if(strcmp(Login.User[i].LoginName,Name)==0)
			{
				MessageBox(L"该用户已注册",L"出错啦");
				return;
			}
		}
		Login.CompanyNum++;
	
		strcpy(Login.User[Login.CompanyNum-1].LoginName,Name);
		strcpy(Login.User[Login.CompanyNum-1].Password,PassWord);
		Login.SaveCompany();
	}
	

	if(Temp==L"旅行社注册")
	{
		//	MessageBox(L"a",L"s");
		//Login.SaveTravel();
		Login.ReadTravel();
		for(i=0;i<Login.TravelNum;i++)
		{
			if(strcmp(Login.User[i].LoginName,Name)==0)
			{
				MessageBox(L"该用户已注册",L"出错啦");
				return;
			}
		}
		Login.TravelNum++;

		strcpy(Login.User[Login.TravelNum-1].LoginName,Name);
		strcpy(Login.User[Login.TravelNum-1].Password,PassWord);
		Login.SaveTravel();
	}

	if(Temp==L"个人用户注册")
	{
		//	MessageBox(L"a",L"s");
		Login.ReadPerson();
		for(i=0;i<Login.UserNum;i++)
		{
			if(strcmp(Login.User[i].LoginName,Name)==0)
			{
				MessageBox(L"该用户已注册",L"出错啦");
				return;
			}
		}
		Login.UserNum++;

		strcpy(Login.User[Login.UserNum-1].LoginName,Name);
		strcpy(Login.User[Login.UserNum-1].Password,PassWord);
		Login.SavePerson();
	}
	CDialogEx::OnOK();
}


BOOL CRegister::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_ComBoUser.AddString(L"航空公司注册");
	m_ComBoUser.AddString(L"旅行社注册");
	m_ComBoUser.AddString(L"个人用户注册");
	m_ComBoUser.SetWindowText(L"个人用户注册");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CRegister::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	CDC *pDC;
  	pDC=GetDC();
  	CRect rect;
  	GetClientRect(&rect);
 	 CBitmap wtj_bitmap;
 	 CDC dcMemory;
 
 	 wtj_bitmap.LoadBitmap(IDB_BITMAP_Reg);
 	 BITMAP bmInfo;
  	wtj_bitmap.GetObject(sizeof(bmInfo),&bmInfo);
  	dcMemory.CreateCompatibleDC(pDC);
   	dcMemory.FillSolidRect(&rect,RGB(255,0,0));
  	dcMemory.SelectObject(&wtj_bitmap);
 

  	pDC->StretchBlt(0,0,
  	 rect.Width(),rect.Height(),
  	 &dcMemory,
 	  0,0,
  	 bmInfo.bmWidth,bmInfo.bmHeight,
  	 SRCCOPY);

 	 ReleaseDC(pDC);

}


HBRUSH CRegister::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC)// || id == IDC_CHECK1)
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
