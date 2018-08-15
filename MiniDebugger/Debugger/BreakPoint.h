#pragma once
#include <list>
#include <vector>
#include "DispatchException.h"
#include "DebuggerMainDlg.h"
// #define BP_INIT 1  //初始断点
// #define BP_CODE 2  //代码中的断点
// #define BP_USER 3  //用户设置的断点

#define	MODE_HARD		1 // 硬件断点:DR0~DR3
#define	MODE_SOFT		2 // 软件断点:0xcc
#define	MODE_TF			3 // TF  断点:TF=1

#define	SOFTTYPE_ONCE	1 // 一次性断点
#define SOFTTYPE_NORM	2 // 正常断点
#define	SOFTTYPE_COND	3 // 条件断点

#define CONDITIONTYPE_ADDRE 0 // 条件断点的地址类型
#define	CONDITIONTYPE_TIMES 1 // 条件断点的次数类型

#define	TFTYPE_NO		0 // 没有断点
#define TFTYPE_TF		1 // 正常的TF断点
#define	TFTYPE_RELOAD	2 // 重新安装软件断点
#define	TFTYPE_TIMES	3 // 次数断点

#define	UPDOWN_EQU		1 // 相等
#define	UPDOWN_UP		2 // 升序
#define	UPDOWN_DOWN		3 // 降序
enum HWBRK_TYPE
{
	HWBRK_TYPE_CODE,
	HWBRK_TYPE_READWRITE,
	HWBRK_TYPE_WRITE,
};

enum HWBRK_SIZE
{
	HWBRK_SIZE_1,
	HWBRK_SIZE_2,
	HWBRK_SIZE_4,
	HWBRK_SIZE_8,
};

enum PointType
{
	BP_INIT,  //初始断点
	BP_CODE,  //代码中的断点
	BP_CC,    //软件断点
	DR_POINT, //硬件断点
	BP_MEMORY //内存断点
};
typedef struct _PointInfo_
{
	PointType ptType; //硬件断点，软件断点断点类型  ,不同类型就有不同的结构体

	DWORD lpPointAddr; //断点地址
	char chOldByte;   //原先的字节（针对INT3断点）

	DWORD	type;		//条件断点，还是一次性断点，

	DWORD	conditionType : 4;// 条件断点使用哪种条件: 地址,次数
	DWORD	upDown		  : 4; // >,==,< 
	SIZE_T	condition;  // 条件:地址或者次数
}PointInfo,*PPointInfo;

typedef struct _HardPointInfo_
{
	PointType ptType; //硬件断点类型  ,  DR_POINT

	int Number ;       //硬件断点序号
	DWORD lpPointAddr; //断点地址

	HWBRK_TYPE   TypeREADWRITECODE;
	HWBRK_SIZE   HardPointSize;
	DWORD	conditionType : 4;// 条件断点使用哪种条件: 地址,次数
	DWORD	upDown		  : 4; // >,==,< 
	SIZE_T	condition;  // 条件:地址或者次数
}HardPointInfo,*PHardPointInfo;

const std::list<PointInfo>& GetBreakPoints();
//初始化断点组件
void InitializeBreakPointHelper();
int GetBreakPointType(DWORD address);
BOOL SetCCBreakPointAt(DWORD address,DWORD type);
BYTE SetBreakPointMemory(DWORD address);
void SaveResetUserBreakPoint(DWORD address) ;

//取消指定地址的断点
BOOL CancelBreakPoint(DWORD address);
void RecoverBreakPoint(const PointInfo* pBreakPoint) ;
void SetTrapFlag();


VOID SetBreakPointAtEntryPoint(DWORD Address);
void SaveResetUserBreakPoint(DWORD address);


BOOL OnSoftBreakPoint(const EXCEPTION_DEBUG_INFO* pInfo);
BOOL OnSingleStep(const EXCEPTION_DEBUG_INFO* pInfo);

BOOL SetHardPoint(DWORD dwType,SIZE_T address);

typedef struct _DBG_REG7
{
	/*
	// 局部断点(L0~3)与全局断点(G0~3)的标记位
	*/
	unsigned L0 : 1;  // 对Dr0保存的地址启用 局部断点
	unsigned G0 : 1;  // 对Dr0保存的地址启用 全局断点
	unsigned L1 : 1;  // 对Dr1保存的地址启用 局部断点
	unsigned G1 : 1;  // 对Dr1保存的地址启用 全局断点
	unsigned L2 : 1;  // 对Dr2保存的地址启用 局部断点
	unsigned G2 : 1;  // 对Dr2保存的地址启用 全局断点
	unsigned L3 : 1;  // 对Dr3保存的地址启用 局部断点
	unsigned G3 : 1;  // 对Dr3保存的地址启用 全局断点
	/*
	// 【以弃用】用于降低CPU频率，以方便准确检测断点异常
	*/
	unsigned LE : 1;
	unsigned GE : 1;
	/*
	// 保留字段
	*/
	unsigned Reserve1 : 3;
	/*
	// 保护调试寄存器标志位，如果此位为1，则有指令修改条是寄存器时会触发异常
	*/
	unsigned GD : 1;
	/*
	// 保留字段
	*/
	unsigned Reserve2 : 2;
	/*
	// 保存Dr0~Dr3地址所指向位置的断点类型(RW0~3)与断点长度(LEN0~3)，状态描述如下：
	*/
	unsigned RW0 : 2;  // 设定Dr0指向地址的断点类型
	unsigned LEN0 : 2;  // 设定Dr0指向地址的断点长度
	unsigned RW1 : 2;  // 设定Dr1指向地址的断点类型
	unsigned LEN1 : 2;  // 设定Dr1指向地址的断点长度
	unsigned RW2 : 2;  // 设定Dr2指向地址的断点类型
	unsigned LEN2 : 2;  // 设定Dr2指向地址的断点长度
	unsigned RW3 : 2;  // 设定Dr3指向地址的断点类型
	unsigned LEN3 : 2;  // 设定Dr3指向地址的断点长度
}DBG_REG7,*PDBG_REG7;




void SetBits(DWORD_PTR& dw, int lowBit, int bits, int newValue);
HANDLE SetHardwareBreakpoint(HANDLE hThread,HWBRK_TYPE Type,HWBRK_SIZE Size,void* s);
void SaveResetUserHardBreakPoint(HWBRK_TYPE Type,HWBRK_SIZE Size,DWORD address) ;
HANDLE UnSetHardwareBreakpoint(HWBRK_TYPE Type,HWBRK_SIZE Size,void* s);
BOOL MoveOut();
BOOL MoveOver();