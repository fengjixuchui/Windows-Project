#pragma once
#define MAXNUM 10000   //设定最大10000人的信息
typedef struct _FLIGHT_
{
	WCHAR FightNum[8];  //航班号
	WCHAR Start[20];     //起始站
	WCHAR Arrive[20];    //终点站
	WCHAR TimeStart[20];     //出发时间
	WCHAR TimeArrive[20];    //到达时间
	int nYear;
	int nMonth;
	int nDay;
	int    Price;        //价格
	int     SeatLeft;    //余票数
	int     SeatNum;     //机票数
}FLIGHT,*PFLIGHT;



typedef struct _PASSENGER_
{
	WCHAR OrderNum[28];
	WCHAR UserName[20];    //用户名
	WCHAR PassengerName[20];//姓名
	WCHAR Sex[4];           //性别
	WCHAR WorkPlace[20];    //工作单位
	WCHAR IDCard[20];       //身份证号码
	WCHAR StartTime[20];             //旅行时间
	WCHAR ArriveTime[20];
	WCHAR Start[20];		//系统查询旅客的始发站 分配航班
	WCHAR Arrive[20];
	WCHAR Price[10];
	WCHAR Seat[20];         //航班舱位
	WCHAR FightNum[8];     //由系统分配的航班号   
	BOOL IsGet;            //是否取票
	int nDay;
}PASSENGER,*PPASSENGER;

typedef struct _ORDER_
{
	WCHAR FightNum[8];     //由系统分配的航班号 
	WCHAR OrderNum[28];
	BOOL IsGet;            //是否取票
}ORDER,*PORDER;


class CCommon
{
public:
	CCommon(void);
	~CCommon(void);
	void CCommon::read();
	void CCommon::save();
	PFLIGHT Flight;
	PPASSENGER Passenger;
	PORDER Order;
	static int FlightCount;
	static int PersonCount;
	static int OrderCount;
	long CCommon::StringToInt( LPCTSTR str);

	void CCommon::savePerson();
	void CCommon::readPerson();

	WCHAR* CCommon::readUser();
	void CCommon::saveUser(WCHAR *Name);


	void CCommon::saveOrder();
	void CCommon::readOrder();
};







