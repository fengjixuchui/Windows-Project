#include "stdafx.h"
#include "DeAsmEngine.h"
#include <strsafe.h>
//#include "PEInfo.h"
#include "DispatchException.h"
#include "DebuggerMainDlg.h"
#include "Common.h"


#define BEA_ENGINE_STATIC
#define BEA_USE_STDCALL
#include "../BeaEngine_4.1/Win32/headers/BeaEngine.h"
#ifndef _WIN64
#pragma comment(lib, "../BeaEngine_4.1/Win32/Win32/Lib/BeaEngine.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt.lib\"")
#else
#pragma comment(lib, "../BeaEngine_4.1/Win64/Win64/Lib/BeaEngine64.lib")
#pragma comment(linker, "/NODEFAULTLIB:\"crt64.lib\"")
#endif

extern HANDLE g_hProcess;
extern CDebugger1Dlg *MainDlg;
extern std::list<PointInfo>  g_BpList;

#ifndef ALJLKASJDLKASJLAFLDKF

#endif // !1




//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// 函数名: GetCoodeLen
///功  能: 获取指令长度
// 形  参: HANDLE g_hCurrentProcess
// 形  参: const LPVOID lpAddress
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
int GetCoodeLen(const SIZE_T lpAddress)
{
	// 1. 将调试程序的内存(OPCode)复制到本地
	SIZE_T  dwRetSize = 0;
	LPVOID lpRemote_Buf = new BYTE[32];
	ZeroMemory(lpRemote_Buf,32);
	ReadProcessMemory(g_hProcess,(LPVOID)lpAddress,lpRemote_Buf,32,&dwRetSize);

	// 2. 初始化反汇编引擎
	DISASM objDiasm;
	objDiasm.EIP = (UIntPtr)lpRemote_Buf; // 起始地址
	objDiasm.VirtualAddr = (UINT64)lpAddress;     // 虚拟内存地址（反汇编引擎用于计算地址）
	objDiasm.Archi = 0;                     // AI-X86
	objDiasm.Options = 0x000;                 // MASM

	// 3. 反汇编代码
	int nLen = Disasm(&objDiasm);
	delete[] lpRemote_Buf;
	return nLen;
}


//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// 函数名: DeAsm
///功  能: 获取opcode和汇编指令
// 形  参: HANDLE g_hCurrentProcess
// 形  参: const LPVOID lpAddress
// 形  参: TCHAR * pszOpCode
// 形  参: TCHAR * pszAsm
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
int DeAsm(const SIZE_T lpAddress,CString& strOpCode,CString& strAsm,CString & strCom)
{
	strOpCode.Empty( );
	strAsm.Empty( );
	strCom.Empty( );

	// 1. 将调试程序的内存(OPCode)复制到本地
	DWORD  dwRetSize = 0;
	LPVOID lpRemote_Buf = new BYTE[64];
	ZeroMemory(lpRemote_Buf,64);
	// 	if(!ReadProcMemory((SIZE_T)lpAddress,lpRemote_Buf,32))
	// 		return -1;
	SIZE_T nRead = 0;
	if(!ReadProcessMemory(g_hProcess,(LPVOID)lpAddress,lpRemote_Buf,64,&nRead))
	{
		CString strStatusMsg;
		strStatusMsg.Format(L"读取内存出错");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
		return -1;
	}
	//显示反汇编内容时显示  原来的内容 0xCC
	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == lpAddress) {
				*((LPBYTE)lpRemote_Buf) = it->chOldByte;
			}
	}

	// 2. 初始化反汇编引擎
	DISASM objDiasm;
	objDiasm.EIP = (UIntPtr)lpRemote_Buf; // 起始地址
	objDiasm.VirtualAddr = (UINT64)lpAddress;// 虚拟内存地址（反汇编引擎用于计算地址）
	objDiasm.Archi = 0;                     // AI-X86
	objDiasm.Options = 0x000;                 // MASM

	// 3. 反汇编代码
	UINT unLen = Disasm(&objDiasm);
	if(-1 == unLen) 
		return unLen;
	// 4. 将机器码转码为字符串
	strOpCode.GetBufferSetLength(MAX_OPCODESTRING);
	byteArr2HexStr((LPBYTE)lpRemote_Buf,unLen,(LPWSTR)(LPCTSTR)strOpCode,0);

	// 6. 保存反汇编出的指令
	strAsm.GetBufferSetLength(MAX_OPCODESTRING);
	MultiByteToWideChar(CP_ACP,0,objDiasm.CompleteInstr,-1,(LPWSTR)(LPCTSTR)strAsm,INSTRUCT_LENGTH);

	// 判断是否是call指令.
	DWORD type = GetCodeType((SIZE_T)lpAddress);
	if(type == CODETYPE_CALLOFFSET
		|| type == CODETYPE_CALL_MEM
		|| type == CODETYPE_JMP_MEM
		|| type == CODETYPE_JMPOFFSET
		)
	{
		SIZE_T destAddress = GetCodeOperand((SIZE_T)lpAddress,(CODETYPE)type,NULL);
		if(destAddress != 0)
		{
			GetFunctionName((SIZE_T)lpAddress,strCom);
		}
	}
	return unLen;
}



//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// 函数名: DeAsm
///功  能: 将指定地址的opcode反汇编,保存到传进的vector中
// 形  参: const LPVOID lpAddress
// 形  参: vector<DEASM> & vecAsm
// 形  参: DWORD dwLine
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
int  DeAsm(const SIZE_T lpAddress,vector<DEASM>& vecAsm,DWORD dwLine)
{
	TCHAR szOpCode[MAX_OPCODESTRING] = {0};
	TCHAR szAsm[MAX_OPCODESTRING] = {0};
	DEASM	stcDeAsm = {0};

	SIZE_T	address = (SIZE_T)lpAddress;

	for(DWORD i = 0; i < dwLine; ++i)
	{

		stcDeAsm.dwCodeLen = DeAsm(address,
			stcDeAsm.strOpCode,
			stcDeAsm.strAsm,
			stcDeAsm.strCom
			);
		stcDeAsm.address = address;
		if(stcDeAsm.dwCodeLen == -1)
		{
			return stcDeAsm.dwCodeLen;
		}
		// 
		vecAsm.push_back(stcDeAsm);
		address += stcDeAsm.dwCodeLen;
	}
	return TRUE;
}



// void CDeAsmEngine::Init(HANDLE hProcess,HANDLE hThread,HANDLE hFile)
// {
// 	g_hCurrentProcess = hProcess;
// 	g_hCurrentThread = hThread;
// 	g_hFile = hFile;
// }


//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// 函数名: GetCodeType
///功  能: 获得内存地址上的指令(类型保存在CODETYPE枚举中)
// 形  参: SIZE_T address
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
CODETYPE GetCodeType(SIZE_T address)
{
	// 读取远程内存
	LPBYTE	lpCode = new BYTE[4];
	SIZE_T dwRead = 0;
	if(!ReadProcessMemory(g_hProcess,(LPVOID)address,lpCode,4,&dwRead))
	{	CString strStatusMsg;
		strStatusMsg.Format(L"读取内存出错");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
		return CODETYPE_ERROR;
	}
	DWORD	dwType = 0;
	switch(lpCode[0])
	{
	case 0xff:
		{
			;
			if(lpCode[1] == 0x15) //call [address]
				dwType = CODETYPE_CALL_MEM;
			if (lpCode[1]==0x25) // jmp [address]
				dwType = CODETYPE_JMP_MEM;
			break;
		}
	case 0xe8:// call offset+指令长度
		dwType = CODETYPE_CALLOFFSET;
		break;
	case 0xe9:// jmp offset+指令长度
		{
			;
			dwType= CODETYPE_JMPOFFSET;
			break;
		}
	case 0xea: // jmp address
		{
			;
			dwType = CODE_JMP;
			break;
		}
	default:
		dwType = CODETYPE_ERROR;
		break;
	}
	delete[] lpCode;
	lpCode = nullptr;
	return (CODETYPE)dwType;
}


//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// 函数名: GetCodeOperand
///功  能: 获取指定地址中的指令的操作数
// 形  参: SIZE_T address
// 形  参: CODETYPE codeType
// 形  参: PCODEFOMART pCodeOperand
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
SIZE_T GetCodeOperand(SIZE_T address,CODETYPE codeType,PCODEFOMART pCodeOperand)
{
	// 读取远程内存
	LPBYTE	lpCode = new BYTE[MAX_OPCODESTRING];
	if(!ReadDebuggeeMemory(address,MAX_OPCODESTRING,lpCode))
	{
		CString strStatusMsg;
		strStatusMsg.Format(L"读取内存出错");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
		return CODETYPE_ERROR;
	}

	switch(codeType)
	{
	case CODETYPE_ERROR:return CODETYPE_ERROR;
		break;
	case CODETYPE_JMP_MEM: // jmp  : 0xff25 + [address]
	case CODETYPE_CALL_MEM: // call : 0xff15 + [address]
		{
			;
			SIZE_T	addr = *((SIZE_T*)(lpCode + 2));//address
			LPBYTE	addressInMem = new BYTE[8];
			if(!ReadDebuggeeMemory(addr,8,addressInMem)) // [address]
			{
				CString strStatusMsg;
				strStatusMsg.Format(L"读取内存出错");
				MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
				return CODETYPE_ERROR;
			}	
			addr = (SIZE_T)*addressInMem;
			if(addressInMem!=NULL)
				delete[] addressInMem; 
			return addr;
			break;
		}
	case CODETYPE_JMPOFFSET: // jmp  : 0xe9 + offset+address+指令长度
	case CODETYPE_CALLOFFSET: // call : 0xE8 + offset+address+指令长度
		{
			;
			return	*((SIZE_T*)(lpCode + 1)) + GetCoodeLen(address)+address;
			break;
		}
	case CODE_JMP: // jmp  : 0xea + address
		return	*((SIZE_T*)(lpCode + 1));
		break;
	default:
		break;
	}

	delete[] lpCode;
	lpCode = nullptr;
	return 0;
}



