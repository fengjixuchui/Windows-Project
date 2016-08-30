#include "StdAfx.h"
#include "Common.h"

int CCommon:: FlightCount = 0;  //总的航班数  由航班公司修改
int CCommon:: PersonCount = 0;
int CCommon:: OrderCount = 0;
CCommon::CCommon(void)
{
	Flight = new FLIGHT[MAXNUM];
	memset(Flight,0,sizeof(_FLIGHT_)*MAXNUM);
	Passenger = new PASSENGER[MAXNUM];
	memset(Passenger,0,sizeof(_PASSENGER_)*MAXNUM);
	Order = new ORDER[MAXNUM];
	memset(Order,0,sizeof(_ORDER_)*MAXNUM);
}


CCommon::~CCommon(void)
{
	delete[] Flight;
	delete[] Passenger;
	delete[] Order;
}





void CCommon::read()//从文件读取信息模块   
{   int i =0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("航班信息.doc","rb"))==NULL)//打开文件，并判断是否出错   
{   
	MessageBox(NULL,L"出错，请检查文件是否存在，按任意键返回主菜单",L"错误提示",1);//打印出错提示   
	getchar();   
}   
if((fp1=fopen("航班数.doc","rb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"错误提示",1);//打印出错提示   
	getchar();   
	return;   
}   
fscanf(fp1,"%d",&FlightCount);   
fclose(fp1);//关闭文件   

for(i=0;i<FlightCount;i++)    
{   
	fread(&Flight[i],sizeof(_FLIGHT_),1,fp);//从文件中读取信息   

}   
fclose(fp);//关闭文件   
}   





void CCommon::save()//保存模块程序   
{   int i = 0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("航班信息.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示   
	getchar();   
	return;   
}   
if((fp1=fopen("航班数.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示  
	getchar();   
	return;   
}   
for(i=0;i<FlightCount;i++)   
	if(fwrite(&Flight[i],sizeof(_FLIGHT_),1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(NULL,L"向文件输入数据失败!\n\n",L"出错啦",1);   
fprintf(fp1,"%d",FlightCount);   
fclose(fp);//关闭文件   
fclose(fp1);//关闭文件   
}   



long CCommon::StringToInt( LPCTSTR str)
{
	TCHAR szBuff[ 20 ], *p;
	p = szBuff;

#ifdef _UNICODE
	return wcstol(  str, &p, 10 );   
#else
	return strtol(  str, &p, 10  );   
#endif
}






void CCommon::readPerson()//从文件读取信息模块   
{   int i =0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("用户信息.doc","rb"))==NULL)//打开文件，并判断是否出错   
{   
	MessageBox(NULL,L"出错，请检查文件是否存在，按任意键返回主菜单",L"错误提示",1);//打印出错提示   
	getchar();   
}   
if((fp1=fopen("用户数.doc","rb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"错误提示",1);//打印出错提示   
	getchar();   
	return;   
}   
fscanf(fp1,"%d",&PersonCount);   
fclose(fp1);//关闭文件   

for(i=0;i<PersonCount;i++)    
{   
	fread(&Passenger[i],sizeof(_PASSENGER_),1,fp);//从文件中读取信息   

}   
fclose(fp);//关闭文件   
}   





void CCommon::savePerson()//保存模块程序   
{   int i = 0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("用户信息.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示   
	getchar();   
	return;   
}   
if((fp1=fopen("用户数.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示  
	getchar();   
	return;   
}   
for(i=0;i<PersonCount;i++)   
	if(fwrite(&Passenger[i],sizeof(_PASSENGER_),1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(NULL,L"向文件输入数据失败!\n\n",L"出错啦",1);   
fprintf(fp1,"%d",PersonCount);   
fclose(fp);//关闭文件   
fclose(fp1);//关闭文件   
}   




void CCommon::saveUser(WCHAR *Name)//保存模块程序   
{   int i = 0;


FILE *fp;//定义文件指针   
if((fp=fopen("登陆姓名.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示   
	getchar();   
	return;   
}   
	if(fwrite(Name,20,1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(NULL,L"向文件输入数据失败!\n\n",L"出错啦",1);   
  
fclose(fp);//关闭文件   

}   



WCHAR* CCommon::readUser()//从文件读取信息模块   
{   int i =0;
FILE *fp;//定义文件指针   
if((fp=fopen("登陆姓名.doc","rb"))==NULL)//打开文件，并判断是否出错   
{   
	MessageBox(NULL,L"出错，请检查文件是否存在，按任意键返回主菜单",L"错误提示",1);//打印出错提示   
	getchar();   
}   



WCHAR Name[20] = {0};
 
	fread(Name,20,1,fp);//从文件中读取信息   


   
fclose(fp);//关闭文件   
return Name;
}   






void CCommon::readOrder()//从文件读取信息模块   
{   int i =0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("订单信息.doc","rb"))==NULL)//打开文件，并判断是否出错   
{   
	MessageBox(NULL,L"出错，请检查文件是否存在，按任意键返回主菜单",L"错误提示",1);//打印出错提示   
	getchar();   
}   
if((fp1=fopen("订单数.doc","rb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"错误提示",1);//打印出错提示   
	getchar();   
	return;   
}   
fscanf(fp1,"%d",&OrderCount);   
fclose(fp1);//关闭文件   

for(i=0;i<OrderCount;i++)    
{   
	fread(&Order[i],sizeof(_ORDER_),1,fp);//从文件中读取信息   

}   
fclose(fp);//关闭文件   
}   





void CCommon::saveOrder()//保存模块程序   
{   int i = 0;
FILE *fp,*fp1;//定义文件指针   
if((fp=fopen("订单信息.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示   
	getchar();   
	return;   
}   
if((fp1=fopen("订单数.doc","wb"))==NULL)//打开文件并判断是否出错     
{   
	MessageBox(NULL,L"创建文件失败!\n\n",L"出错啦",1);//打印出错提示  
	getchar();   
	return;   
}   
for(i=0;i<OrderCount;i++)   
	if(fwrite(&Order[i],sizeof(_ORDER_),1,fp)==0)//向文件写入数据，并判断是否出错   
		MessageBox(NULL,L"向文件输入数据失败!\n\n",L"出错啦",1);   
fprintf(fp1,"%d",OrderCount);   
fclose(fp);//关闭文件   
fclose(fp1);//关闭文件   
}   

