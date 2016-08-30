// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Login.h"
#include "afxdialogex.h"
#include "Register.h"
#include "Common.h"
#include "Type.h"

int CLogin::CompanyNum = 0;
int CLogin:: UserNum = 0;
int CLogin::TravelNum = 0;

// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLogin::IDD, pParent)
{

	m_Name = _T("");
	m_Password = _T("");
	User = new USER[MAXNUM];
	IsTrue = false;
}

CLogin::~CLogin()
{
	delete[] User;
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT_NAME, m_Name);
	//  DDX_Control(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);

	DDX_Check(pDX, IDC_CHECK_Company, m_Company);
	DDX_Check(pDX, IDC_CHECK_Person, m_Person);
	DDX_Check(pDX, IDC_CHECK_Travel, m_Travel);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_Person, &CLogin::OnBnClickedCheckPerson)
	ON_BN_CLICKED(IDOK, &CLogin::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_Register, &CLogin::OnBnClickedButtonRegister)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTONFix, &CLogin::OnBnClickedButtonfix)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLogin 消息处理程序


void CLogin::OnBnClickedCheckPerson()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL CLogin::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

//	CDialogEx::SetBackgroundImage(IDB_BITMAP_Enter);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLogin::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	int i =0;
	char Name[20]={0};
	char PassWord[20]={0};
	wcstombs(Name,m_Name,20);
	wcstombs(PassWord,m_Password,20);
	if(m_Name==L"")
	{
		MessageBox(L"请输入用户名",L"用户名为空");
		return ;
	}
	if(m_Password==L"")
	{
		MessageBox(L"请输入密码",L"密码为空");
		return ;
	}
	
	if(m_Company==FALSE&&m_Travel==FALSE&&m_Person==FALSE)
	{
		MessageBox(L"请选择用户登陆方式",L"出错啦");
		return ;
	}
	if((m_Company==TRUE&&m_Person==TRUE)||(m_Company==TRUE&&m_Travel==TRUE)||(m_Person==TRUE&&m_Travel==TRUE)
		||(m_Company==TRUE&&m_Person==TRUE&&m_Travel==TRUE))
	{
		MessageBox(L"不能同时选择多种用户类型登陆",L"出错啦");
		return ;
	}
	if(m_Company==TRUE)
	{
		ReadCompany();
		for(i=0;i<CompanyNum;i++)
		{
			if(strcmp(User[i].LoginName,Name)==0&&strcmp(User[i].Password,PassWord)==0)
			{
				iTypeUser = "航空公司";
				UserName = (WCHAR*)Name;
				CDialogEx::OnOK();
				IsTrue = true;
			}
		}
		if(IsTrue==false)
		{
			MessageBox(L"用户名或密码错误",L"出错啦");
		}
		
		return ;

	}

	if(m_Travel==TRUE)
	{
		ReadTravel();
		for(i=0;i<TravelNum;i++)
		{
			if(strcmp(User[i].LoginName,Name)==0&&strcmp(User[i].Password,PassWord)==0)
			{
				iTypeUser = "旅行社";
				UserName = (WCHAR*)Name;
				CDialogEx::OnOK();
				IsTrue = true;
			}
		}
		if(IsTrue==false)
		{
			MessageBox(L"用户名或密码错误",L"出错啦");
		}

		return ;

	}

	if(m_Person==TRUE)
	{
		ReadPerson();
		for(i=0;i<UserNum;i++)
		{
			if(strcmp(User[i].LoginName,Name)==0&&strcmp(User[i].Password,PassWord)==0)
			{
				iTypeUser = "个人";
				UserName = (WCHAR*)Name;
				CDialogEx::OnOK();
				IsTrue = true;
			}
		}
		if(IsTrue==false)
		{
			MessageBox(L"用户名或密码错误",L"出错啦");
		}

		return ;

	}
	

}

void CLogin::OnBnClickedButtonRegister()
{
	// TODO: 在此添加控件通知处理程序代码
	CRegister Register;
	Register.DoModal();
}






void CLogin::SaveCompany()//保存模块程序   
{   int i = 0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("航空公司用户.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(L"创建文件失败!\n\n",L"出错啦");//打印出错提示   
	getchar();   
	return;   
}   
if((fp1=fopen("航空公司用户数.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(L"创建文件失败!\n\n",L"出错啦");//打印出错提示  
	getchar();   
	return;   
}   
for(i=0;i<CompanyNum;i++)   
	if(fwrite(&User[i],sizeof(_USER_),1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(L"向文件输入数据失败!\n\n",L"出错啦");   
fprintf(fp1,"%d",CompanyNum);   
fclose(fp);//关闭文件   
fclose(fp1);//关闭文件   
}   
/*
void CLogin::InputFlight()
{
	int j =0;
	CompanyNum++;
	strcpy(Flight[CompanyNum-1].FightNum,"FASS");
	for(j=0;j<CompanyNum-1;j++)   
		while(Flight[j].FightNum==Flight[CompanyNum-1].FightNum)   
		{   
			MessageBox(L"\n此航班已经存在!!!请核实后重新输入...",L"出错啦");
			Flight[FlightCount-1].FightNum;     //重新读取航班
		}   
		strcpy(Flight[CompanyNum-1].Start,"西安");
		strcpy(Flight[CompanyNum-1].Arrive,"福州");

		strcpy(Flight[CompanyNum-1].Time,"5015.5.21");
		Flight[CompanyNum-1].SeatNum=200;

		SaveCompany();//保存

}*/



void CLogin::ReadCompany()//从文件读取信息模块   
{   

	int i =0;
	FILE *fp,*fp1;//定义文件指针   
	if((fp=fopen("航空公司用户.doc","rb"))==NULL)//打开文件，并判断是否出错   
	{   
	//	MessageBox(L"没有此用户文件,请注册",L"出错啦");//打印出错提示   
	//	getchar();   
		return ;
	}   
	if((fp1=fopen("航空公司用户数.doc","rb"))==NULL)//打开文件并判断是否出错     
	{   
	//	MessageBox(L"没有此用户文件,请注册!\n\n",L"出错啦");//打印出错提示   
	//	getchar();   
		return;   
	}   
	fscanf(fp1,"%d",&CompanyNum);   
	fclose(fp1);//关闭文件   

	for(i=0;i<CompanyNum;i++)    
	{   
		fread(&User[i],sizeof(_USER_),1,fp);//从文件中读取信息   

	}   
	fclose(fp);//关闭文件   
}   




void CLogin::ReadTravel()//从文件读取信息模块   
{   

	int i =0;
	FILE *fp,*fp1;//定义文件指针   
	if((fp=fopen("旅行社用户.doc","rb"))==NULL)//打开文件，并判断是否出错   
	{   
		//	MessageBox(L"没有此用户文件,请注册",L"出错啦");//打印出错提示   
		//	getchar();   
		return ;
	}   
	if((fp1=fopen("旅行社用户数.doc","rb"))==NULL)//打开文件并判断是否出错     
	{   
		//	MessageBox(L"没有此用户文件,请注册!\n\n",L"出错啦");//打印出错提示   
		//	getchar();   
		return;   
	}   
	fscanf(fp1,"%d",&TravelNum);   
	fclose(fp1);//关闭文件   

	for(i=0;i<TravelNum;i++)    
	{   
		fread(&User[i],sizeof(_USER_),1,fp);//从文件中读取信息   

	}   
	fclose(fp);//关闭文件   
}   



void CLogin::SaveTravel()//保存模块程序   
{   int i = 0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("旅行社用户.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(L"创建文件失败!\n\n",L"出错啦");//打印出错提示   
	getchar();   
	return;   
}   
if((fp1=fopen("旅行社用户数.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(L"创建文件失败!\n\n",L"出错啦");//打印出错提示  
	getchar();   
	return;   
}   
for(i=0;i<TravelNum;i++)   
	if(fwrite(&User[i],sizeof(_USER_),1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(L"向文件输入数据失败!\n\n",L"出错啦");   
fprintf(fp1,"%d",TravelNum);   
fclose(fp);//关闭文件   
fclose(fp1);//关闭文件   
}   




void CLogin::ReadPerson()//从文件读取信息模块   
{   

	int i =0;
	FILE *fp,*fp1;//定义文件指针   
	if((fp=fopen("个人用户.doc","rb"))==NULL)//打开文件，并判断是否出错   
	{   
		//	MessageBox(L"没有此用户文件,请注册",L"出错啦");//打印出错提示   
		//	getchar();   
		return ;
	}   
	if((fp1=fopen("个人用户数.doc","rb"))==NULL)//打开文件并判断是否出错     
	{   
		//	MessageBox(L"没有此用户文件,请注册!\n\n",L"出错啦");//打印出错提示   
		//	getchar();   
		return;   
	}   
	fscanf(fp1,"%d",&UserNum);   
	fclose(fp1);//关闭文件   

	for(i=0;i<UserNum;i++)    
	{   
		fread(&User[i],sizeof(_USER_),1,fp);//从文件中读取信息   

	}   
	fclose(fp);//关闭文件   
}   



void CLogin::SavePerson()//保存模块程序   
{   int i = 0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("个人用户.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(L"创建文件失败!\n\n",L"出错啦");//打印出错提示   
	getchar();   
	return;   
}   
if((fp1=fopen("个人用户数.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(L"创建文件失败!\n\n",L"出错啦");//打印出错提示  
	getchar();   
	return;   
}   
for(i=0;i<UserNum;i++)   
	if(fwrite(&User[i],sizeof(_USER_),1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(L"向文件输入数据失败!\n\n",L"出错啦");   
fprintf(fp1,"%d",UserNum);   
fclose(fp);//关闭文件   
fclose(fp1);//关闭文件   
}   



void CLogin::OnPaint()
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
 
 	 wtj_bitmap.LoadBitmap(IDB_BITMAP_Enter);
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


void CLogin::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CLogin::OnBnClickedButtonfix()
{
	// TODO: 在此添加控件通知处理程序代码
	CType Type;
	Type.DoModal();
}


HBRUSH CLogin::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔



	UINT id = pWnd->GetDlgCtrlID();
	if(id == IDC_STATIC||id ==IDC_CHECK_Company||id==IDC_CHECK_Person||id==IDC_CHECK_Travel||id==IDC_BUTTON_Register
		||id==IDOK||id==IDC_BUTTONFix)// || id == IDC_CHECK1)
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
