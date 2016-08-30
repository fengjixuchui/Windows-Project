// Type.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Type.h"
#include "afxdialogex.h"
#include "Login.h"


// CType 对话框

IMPLEMENT_DYNAMIC(CType, CDialogEx)

CType::CType(CWnd* pParent /*=NULL*/)
	: CDialogEx(CType::IDD, pParent)
	, m_Type(_T(""))
	, m_Name(_T(""))
	, m_Old(_T(""))
	, m_New(_T(""))
{
	bOk = FALSE;
}

CType::~CType()
{
}

void CType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_Type, m_Type);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_Old, m_Old);
	DDX_Text(pDX, IDC_EDIT_New, m_New);
	DDX_Control(pDX, IDC_COMBO_Type, m_TYPE);
}


BEGIN_MESSAGE_MAP(CType, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Sure, &CType::OnBnClickedButtonSure)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CType 消息处理程序


void CType::OnBnClickedButtonSure()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CLogin  Login;
	char Name[20]={0};
	char PassWordOld[20]={0};
	char PassWordNew[20] = {0};
	wcstombs(Name,m_Name,20);
	wcstombs(PassWordOld,m_Old,20);
	wcstombs(PassWordNew,m_New,20);
	int i =0;
	if(m_Name==L"")
	{
		MessageBox(L"请输入用户名",L"出错啦");
		return;
	}
	CString Temp;
	m_TYPE.GetWindowText(Temp);
	if(Temp==L"航空公司")
	{
		//	MessageBox(L"a",L"s");
		Login.ReadCompany();

		for(i=0;i<Login.CompanyNum;i++)
		{
			if(strcmp(Login.User[i].LoginName,Name)==0)
			{
			   bOk = TRUE;
			}
		}
		if(bOk==TRUE)
		{
			for(i=0;i<Login.CompanyNum;i++)
			{
				if(strcmp(Login.User[i].LoginName,Name)==0&&strcmp(Login.User[i].Password,PassWordOld)==0)
				{
					if(strcmp(PassWordNew,PassWordOld)==0)
					{
						MessageBox(L"新密码不能与旧密码一致",L"出错啦");
						return ;
					}

					strcpy(Login.User[i].Password,PassWordNew);

					Login.SaveCompany();
					MessageBox(L"修改成功",L"成功");

					
				}
			}
		}
		else
		{
			MessageBox(L"无此用户",L"出错啦");
			return;
		}
	
	}


	if(Temp==L"旅行社")
	{
		//	MessageBox(L"a",L"s");
		//Login.SaveTravel();
		Login.ReadTravel();

		for(i=0;i<Login.TravelNum;i++)
		{
			if(strcmp(Login.User[i].LoginName,Name)==0)
			{
				bOk = TRUE;
			}
		}
		if(bOk==TRUE)
		{
			for(i=0;i<Login.TravelNum;i++)
			{
				if(strcmp(Login.User[i].LoginName,Name)==0&&strcmp(Login.User[i].Password,PassWordOld)==0)
				{
					if(strcmp(PassWordNew,PassWordOld)==0)
					{
						MessageBox(L"新密码不能与旧密码一致",L"出错啦");
						return ;
					}

					strcpy(Login.User[i].Password,PassWordNew);

					Login.SaveTravel();
					MessageBox(L"修改成功",L"成功");


				}
			}
		}
		else
		{
			MessageBox(L"无此用户",L"出错啦");
			return;
		}

	}

	if(Temp==L"个人用户")
	{
		//	MessageBox(L"a",L"s");
		Login.ReadPerson();

		for(i=0;i<Login.UserNum;i++)
		{
			if(strcmp(Login.User[i].LoginName,Name)==0)
			{
				bOk = TRUE;
			}
		}
		if(bOk==TRUE)
		{
			for(i=0;i<Login.UserNum;i++)
			{
				if(strcmp(Login.User[i].LoginName,Name)==0&&strcmp(Login.User[i].Password,PassWordOld)==0)
				{
					if(strcmp(PassWordNew,PassWordOld)==0)
					{
						MessageBox(L"新密码不能与旧密码一致",L"出错啦");
						return ;
					}

					strcpy(Login.User[i].Password,PassWordNew);

					Login.SavePerson();
					MessageBox(L"修改成功",L"成功");


				}
			}
		}
		else
		{
			MessageBox(L"无此用户",L"出错啦");
			return;
		}

	}

	CDialogEx::OnOK();
}


BOOL CType::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_TYPE.AddString(L"航空公司");
	m_TYPE.AddString(L"旅行社");
	m_TYPE.AddString(L"个人用户");
	m_TYPE.SetWindowText(L"个人用户");


	CDialogEx::SetBackgroundImage(IDB_BITMAP_GRAND);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


HBRUSH CType::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔


	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC||id==IDC_COMBO_Type)// || id == IDC_CHECK1)
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
