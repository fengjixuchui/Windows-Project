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
	CString strCom; // ע��
}DEASM;
#endif

enum CODETYPE
{
	CODETYPE_ERROR,
	CODETYPE_CALL_MEM,		// call : 0xff15 + [address]
	CODETYPE_JMP_MEM,		// jmp  : 0xff25 + [address]
	CODETYPE_CALLOFFSET,	// call : 0xE8 + offset+ָ���
	CODETYPE_JMPOFFSET,		// jmp  : 0xe9 + offset+ָ���
	CODE_JMP,				// jmp  : 0xea + address

};

typedef	struct CODEFOMART
{
	SIZE_T	operand0;
	SIZE_T	operand1;
	SIZE_T	operand2;
}CODEFOMART,*PCODEFOMART;



	//virtual void Init(HANDLE hProcess,HANDLE hThread,HANDLE hFile);
	// ��ȡԶ�̽���ָ����ַ��ָ��ĳ���
	int GetCoodeLen(const SIZE_T lpAddress);
	// ������Զ�̽��̵�ָ����ַ����һ�����ָ��
	int DeAsm(const SIZE_T lpAddress,CString& strOpCode,CString& strAsm,CString & strCom);
	int DeAsm(const SIZE_T lpAddress,vector<DEASM>& vecAsm,DWORD dwLine);



	// �ж��ڵ�ǰEIPָ�����ָ���Ƿ���callָ��
	//CODETYPE GetCodeType();
	// ����ڴ��ַ�ϵ�ָ��(���ͱ�����CODETYPEö����)
	CODETYPE GetCodeType(SIZE_T address);

	// ��ȡָ����ַ�е�ָ��Ĳ�����
	SIZE_T	GetCodeOperand(SIZE_T address,CODETYPE codeType,PCODEFOMART pCodeOperand);

