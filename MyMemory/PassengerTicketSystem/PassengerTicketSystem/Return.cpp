// Return.cpp : 实现文件
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "Return.h"
#include "afxdialogex.h"
#include "Common.h"


// CReturn 对话框

IMPLEMENT_DYNAMIC(CReturn, CDialogEx)

CReturn::CReturn(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReturn::IDD, pParent)
{

}

CReturn::~CReturn()
{
}

void CReturn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CReturn, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CReturn::OnNMRClickList1)
	ON_COMMAND(ID_DELPerson, &CReturn::OnDelperson)
	ON_BN_CLICKED(IDC_BUTTON1, &CReturn::OnBnClickedButton1)
END_MESSAGE_MAP()


// CReturn 消息处理程序


BOOL CReturn::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	bOk = FALSE;

	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);   //（此句，点了list里面一项全蓝）
	m_List.InsertColumn(0,L"姓名",LVCFMT_LEFT,100);
	m_List.InsertColumn(1,L"性别",LVCFMT_LEFT,50);
	m_List.InsertColumn(2,L"身份证",LVCFMT_LEFT,150);
	m_List.InsertColumn(3,L"航班号",LVCFMT_LEFT,150);
	m_List.InsertColumn(4,L"票价",LVCFMT_LEFT,100);
	m_List.InsertColumn(5,L"座位号",LVCFMT_LEFT,50);
	m_List.InsertColumn(6,L"出发地",LVCFMT_LEFT,100);
	m_List.InsertColumn(7,L"目的地",LVCFMT_LEFT,100);
	m_List.InsertColumn(8,L"出发时间",LVCFMT_LEFT,100);
	m_List.InsertColumn(9,L"到达时间",LVCFMT_LEFT,100);


	m_List.DeleteAllItems();
	CCommon Common;
	WCHAR *szBuffer;
	WCHAR Temp[20] = {0};
	Common.readPerson();
	szBuffer = Common.readUser();
	memcpy(Temp,szBuffer,40);
	
	
	for(int i=0;i<Common.PersonCount;i++)
	{
		if(wcscmp(Temp,Common.Passenger[i].UserName)==0)
		{
			CString  Name;
			CString  Sex;
			CString  IDNum;
			CString  FlightNum;
			CString  Price;
			CString  Seat;
			CString  Start;
			CString  Arrive;
			CString  Time1;
			CString  Time2;

			Name = Common.Passenger[i].PassengerName;
			Sex = Common.Passenger[i].Sex;
			IDNum = Common.Passenger[i].IDCard;
			FlightNum =Common.Passenger[i].FightNum;
			Price =Common.Passenger[i].Price;
			Seat = Common.Passenger[i].Seat;
			Start = Common.Passenger[i].Start;
			Arrive= Common.Passenger[i].Arrive;
			Time1 =Common.Passenger[i].StartTime;
			Time2 = Common.Passenger[i].ArriveTime;
		
			
			m_List.InsertItem(i,Name);                                //默认为0行  这样所有插入的新列都在最上面
			m_List.SetItemText(i,1,Sex);      
			m_List.SetItemText(i,2,IDNum); 
			m_List.SetItemText(i,3,FlightNum); 
			m_List.SetItemText(i,4,Price);
			m_List.SetItemText(i,5,Seat);
			m_List.SetItemText(i,6,Start);
			m_List.SetItemText(i,7,Arrive);
			m_List.SetItemText(i,8,Time1);
			m_List.SetItemText(i,9,Time2);
			m_List.SetItemData(i,Common.Passenger[i].nDay);
			
		}
		

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CReturn::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码


	CMenu	popup;
	popup.LoadMenu(IDR_MENU_DELPERSON);               //加载菜单资源
	CMenu*	pM = popup.GetSubMenu(0);                 //获得菜单的子项
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();
	if (m_List.GetSelectedCount() == 0)         //如果没有选中
	{ 
		for (int i = 0;i<count;i++)
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          //菜单全部变灰
		}

	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);


	*pResult = 0;
}




void CReturn::OnDelperson()
{
	// TODO: 在此添加命令处理程序代码
	//MessageBox(L"a",L"s");
	CCommon Common;
	Common.readPerson();
	Common.readOrder();
	Common.read();

	CString  Name;
	CString  IDCard;
	CString  FlightNum;
	CString  Start;
	CString  Arrive;
	WCHAR NAME[20] = {0};
	WCHAR IDCARD[20] = {0};
	WCHAR FightNum1[8] = {0};  //航班号
	WCHAR Start1[20] = {0};     //起始站
	WCHAR Arrive1[20] = {0};    //终点站
	CString FlightMessage = L"此航班已取消";
	WCHAR FlightMessage1[8] = {0};
	int Day ;
	int iSelect = m_List.GetSelectionMark( );                   //获得选择项的索引
	Name = m_List.GetItemText(iSelect,0);          //通过选项中的索引获得数据0项的IP
	IDCard = m_List.GetItemText(iSelect,2);
	FlightNum = m_List.GetItemText(iSelect,3);
	Start = m_List.GetItemText(iSelect,6);
	Arrive =  m_List.GetItemText(iSelect,7);
	Day = m_List.GetItemData(iSelect);
	wsprintf(NAME,L"%s",Name);
	wsprintf(IDCARD,L"%s",IDCard);
	wsprintf(FightNum1,L"%s",FlightNum);
	wsprintf(Start1,L"%s",Start);
	wsprintf(Arrive1,L"%s",Arrive);
	wsprintf(FlightMessage1,L"%s",FlightMessage);


	for(int i =0;i<Common.PersonCount;i++)
	{
		if(wcscmp(Common.Passenger[i].FightNum,FlightMessage1)==0)
		{
			Common.Passenger[i] = Common.Passenger[Common.PersonCount-1];
			
			if(wcscmp(Common.Passenger[i].PassengerName,NAME)==0&&wcscmp(Common.Passenger[i].Start,Start1)==0&&
				wcscmp(Common.Passenger[i].Arrive,Arrive1)==0&&Common.Passenger[i].nDay==Day&&
				wcscmp(Common.Passenger[i].IDCard,IDCARD)==0&&wcscmp(Common.Passenger[i].FightNum,FightNum1)==0)
			{

				for(int m=0;m<Common.OrderCount;m++)
				{
					if(wcscmp(Common.Order[m].OrderNum,Common.Passenger[i].OrderNum)==0)
					{
						Common.Order[m] = Common.Order[Common.OrderCount-1];
						bOk = TRUE;
					}
	
				}
			}
		}
		else
		{
			if(wcscmp(Common.Passenger[i].PassengerName,NAME)==0&&wcscmp(Common.Passenger[i].Start,Start1)==0&&
				wcscmp(Common.Passenger[i].Arrive,Arrive1)==0&&Common.Passenger[i].nDay==Day&&
				wcscmp(Common.Passenger[i].IDCard,IDCARD)==0&&wcscmp(Common.Passenger[i].FightNum,FightNum1)==0)
			{
				Common.Passenger[i] = Common.Passenger[Common.PersonCount-1];

				for(int m=0;m<Common.OrderCount;m++)
				{
					if(wcscmp(Common.Order[m].OrderNum,Common.Passenger[i].OrderNum)==0)
					{
						Common.Order[m] = Common.Order[Common.OrderCount-1];
						bOk = TRUE;
					}
				}
			}
		}
		


		}
		for(int i =0;i<Common.FlightCount;i++)
		{
			if(wcscmp(Common.Flight[i].FightNum,FightNum1)&&wcscmp(Common.Flight[i].Start,Start1)&&
				wcscmp(Common.Flight[i].Arrive,Arrive1)&&
				Common.Flight[i].nDay==Day)
			{
				Common.Flight[i].SeatLeft++;
			}


		}
		if(bOk==TRUE)
		{
				Common.OrderCount--;
		}
	Common.PersonCount--;


	Common.savePerson();
	Common.save();
	Common.saveOrder();


	m_List.DeleteItem(iSelect);                                 //删除该项
	MessageBox(L"退票成功",L"成功");
}


void CReturn::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	MessageBox(L"请您右键选择退票",L"提示");
}
