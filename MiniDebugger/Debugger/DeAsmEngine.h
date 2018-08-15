#pragma once
#include <windows.h>
#include <vector>
#include <atlstr.h>
#include <Dbghelp.h>
#include "Symbol.h"
#include "BreakPoint.h"
//#include "hightLight.h"

#pragma comment(lib,"Dbghelp.lib")
using std::vector;

#define	MAX_OPCODESTRING	64

#ifndef DEASM_STRUCT
#define DEASM_STRUCT 
typedef	struct DEASM
{
	SIZE_T	address;
	DWORD	dwCodeLen;

	CString	strOpCode;
	CString	strAsm;
	CString strCom; // 注释
}DEASM;
#endif

enum CODETYPE
{
	CODETYPE_ERROR,
	CODETYPE_CALL_MEM,		// call : 0xff15 + [address]
	CODETYPE_JMP_MEM,		// jmp  : 0xff25 + [address]
	CODETYPE_CALLOFFSET,	// call : 0xE8 + offset+指令长度
	CODETYPE_JMPOFFSET,		// jmp  : 0xe9 + offset+指令长度
	CODE_JMP,				// jmp  : 0xea + address

};

typedef	struct CODEFOMART
{
	SIZE_T	operand0;
	SIZE_T	operand1;
	SIZE_T	operand2;
}CODEFOMART,*PCODEFOMART;



	//virtual void Init(HANDLE hProcess,HANDLE hThread,HANDLE hFile);
	// 获取远程进程指定地址的指令的长度
	int GetCoodeLen(const SIZE_T lpAddress);
	// 解析出远程进程的指定地址出的一条汇编指令
	int DeAsm(const SIZE_T lpAddress,CString& strOpCode,CString& strAsm,CString & strCom);
	int DeAsm(const SIZE_T lpAddress,vector<DEASM>& vecAsm,DWORD dwLine);



	// 判断在当前EIP指向出的指令是否是call指令
	//CODETYPE GetCodeType();
	// 获得内存地址上的指令(类型保存在CODETYPE枚举中)
	CODETYPE GetCodeType(SIZE_T address);

	// 获取指定地址中的指令的操作数
	SIZE_T	GetCodeOperand(SIZE_T address,CODETYPE codeType,PCODEFOMART pCodeOperand);

