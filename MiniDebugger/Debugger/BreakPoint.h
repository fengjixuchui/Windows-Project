#pragma once
#include <list>
#include <vector>
#include "DispatchException.h"
#include "DebuggerMainDlg.h"
// #define BP_INIT 1  //��ʼ�ϵ�
// #define BP_CODE 2  //�����еĶϵ�
// #define BP_USER 3  //�û����õĶϵ�

#define	MODE_HARD		1 // Ӳ���ϵ�:DR0~DR3
#define	MODE_SOFT		2 // ����ϵ�:0xcc
#define	MODE_TF			3 // TF  �ϵ�:TF=1

#define	SOFTTYPE_ONCE	1 // һ���Զϵ�
#define SOFTTYPE_NORM	2 // �����ϵ�
#define	SOFTTYPE_COND	3 // �����ϵ�

#define CONDITIONTYPE_ADDRE 0 // �����ϵ�ĵ�ַ����
#define	CONDITIONTYPE_TIMES 1 // �����ϵ�Ĵ�������

#define	TFTYPE_NO		0 // û�жϵ�
#define TFTYPE_TF		1 // ������TF�ϵ�
#define	TFTYPE_RELOAD	2 // ���°�װ����ϵ�
#define	TFTYPE_TIMES	3 // �����ϵ�

#define	UPDOWN_EQU		1 // ���
#define	UPDOWN_UP		2 // ����
#define	UPDOWN_DOWN		3 // ����
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
	BP_INIT,  //��ʼ�ϵ�
	BP_CODE,  //�����еĶϵ�
	BP_CC,    //����ϵ�
	DR_POINT, //Ӳ���ϵ�
	BP_MEMORY //�ڴ�ϵ�
};
typedef struct _PointInfo_
{
	PointType ptType; //Ӳ���ϵ㣬����ϵ�ϵ�����  ,��ͬ���;��в�ͬ�Ľṹ��

	DWORD lpPointAddr; //�ϵ��ַ
	char chOldByte;   //ԭ�ȵ��ֽڣ����INT3�ϵ㣩

	DWORD	type;		//�����ϵ㣬����һ���Զϵ㣬

	DWORD	conditionType : 4;// �����ϵ�ʹ����������: ��ַ,����
	DWORD	upDown		  : 4; // >,==,< 
	SIZE_T	condition;  // ����:��ַ���ߴ���
}PointInfo,*PPointInfo;

typedef struct _HardPointInfo_
{
	PointType ptType; //Ӳ���ϵ�����  ,  DR_POINT

	int Number ;       //Ӳ���ϵ����
	DWORD lpPointAddr; //�ϵ��ַ

	HWBRK_TYPE   TypeREADWRITECODE;
	HWBRK_SIZE   HardPointSize;
	DWORD	conditionType : 4;// �����ϵ�ʹ����������: ��ַ,����
	DWORD	upDown		  : 4; // >,==,< 
	SIZE_T	condition;  // ����:��ַ���ߴ���
}HardPointInfo,*PHardPointInfo;

const std::list<PointInfo>& GetBreakPoints();
//��ʼ���ϵ����
void InitializeBreakPointHelper();
int GetBreakPointType(DWORD address);
BOOL SetCCBreakPointAt(DWORD address,DWORD type);
BYTE SetBreakPointMemory(DWORD address);
void SaveResetUserBreakPoint(DWORD address) ;

//ȡ��ָ����ַ�Ķϵ�
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
	// �ֲ��ϵ�(L0~3)��ȫ�ֶϵ�(G0~3)�ı��λ
	*/
	unsigned L0 : 1;  // ��Dr0����ĵ�ַ���� �ֲ��ϵ�
	unsigned G0 : 1;  // ��Dr0����ĵ�ַ���� ȫ�ֶϵ�
	unsigned L1 : 1;  // ��Dr1����ĵ�ַ���� �ֲ��ϵ�
	unsigned G1 : 1;  // ��Dr1����ĵ�ַ���� ȫ�ֶϵ�
	unsigned L2 : 1;  // ��Dr2����ĵ�ַ���� �ֲ��ϵ�
	unsigned G2 : 1;  // ��Dr2����ĵ�ַ���� ȫ�ֶϵ�
	unsigned L3 : 1;  // ��Dr3����ĵ�ַ���� �ֲ��ϵ�
	unsigned G3 : 1;  // ��Dr3����ĵ�ַ���� ȫ�ֶϵ�
	/*
	// �������á����ڽ���CPUƵ�ʣ��Է���׼ȷ���ϵ��쳣
	*/
	unsigned LE : 1;
	unsigned GE : 1;
	/*
	// �����ֶ�
	*/
	unsigned Reserve1 : 3;
	/*
	// �������ԼĴ�����־λ�������λΪ1������ָ���޸����ǼĴ���ʱ�ᴥ���쳣
	*/
	unsigned GD : 1;
	/*
	// �����ֶ�
	*/
	unsigned Reserve2 : 2;
	/*
	// ����Dr0~Dr3��ַ��ָ��λ�õĶϵ�����(RW0~3)��ϵ㳤��(LEN0~3)��״̬�������£�
	*/
	unsigned RW0 : 2;  // �趨Dr0ָ���ַ�Ķϵ�����
	unsigned LEN0 : 2;  // �趨Dr0ָ���ַ�Ķϵ㳤��
	unsigned RW1 : 2;  // �趨Dr1ָ���ַ�Ķϵ�����
	unsigned LEN1 : 2;  // �趨Dr1ָ���ַ�Ķϵ㳤��
	unsigned RW2 : 2;  // �趨Dr2ָ���ַ�Ķϵ�����
	unsigned LEN2 : 2;  // �趨Dr2ָ���ַ�Ķϵ㳤��
	unsigned RW3 : 2;  // �趨Dr3ָ���ַ�Ķϵ�����
	unsigned LEN3 : 2;  // �趨Dr3ָ���ַ�Ķϵ㳤��
}DBG_REG7,*PDBG_REG7;




void SetBits(DWORD_PTR& dw, int lowBit, int bits, int newValue);
HANDLE SetHardwareBreakpoint(HANDLE hThread,HWBRK_TYPE Type,HWBRK_SIZE Size,void* s);
void SaveResetUserHardBreakPoint(HWBRK_TYPE Type,HWBRK_SIZE Size,DWORD address) ;
HANDLE UnSetHardwareBreakpoint(HWBRK_TYPE Type,HWBRK_SIZE Size,void* s);
BOOL MoveOut();
BOOL MoveOver();