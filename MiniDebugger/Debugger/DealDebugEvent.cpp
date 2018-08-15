#include "StdAfx.h"
#include "DealDebugEvent.h"
#include "Common.h"
#include "DispatchException.h"
#include "Flags.h"
#include "ShowDumpText.h"
#include "DebuggerMainDlg.h"
#include "BreakPoint.h"
#include "ShowOrgFile.h"
#include "Symbol.h"
#include <DbgHelp.h>
#pragma comment(lib,"dbghelp.lib")


extern CDebugger1Dlg *MainDlg;
extern ULONG OEP;
extern HANDLE g_hProcess;
extern HANDLE g_hThread;
extern DWORD g_processID;
extern DWORD g_threadID;

extern DWORD g_continueStatus ;
extern int g_debuggeeStatus;

BOOL m_bIs32bitProcess;

VOID AA();
//c
BOOL OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO* pInfo) {

//	Init(pInfo->hProcess,pInfo->hThread,pInfo->hFile); //初始化当前进程的句柄、线程句柄、文件句柄
	
	InitializeBreakPointHelper();

	//初始化符号处理器
	//注意，这里不能使用pInfo->hProcess，因为g_hProcess和pInfo->hProcess
	//的值并不相同，而其它DbgHelp函数使用的是g_hProcess。
	InitSymbol();
	//HANDLE Thread = __beginthread
	char c1[100] = {0};
	//加载模块的调试信息
	DWORD64 moduleAddress = SymLoadModule64(
		g_hProcess,
		pInfo->hFile, 
		NULL,
		NULL,
		(DWORD64)pInfo->lpBaseOfImage,
		0);

	if (moduleAddress != 0) {

	//	SetBreakPointAtEntryPoint();
	}
	else {
	//	std::wcout << TEXT("SymLoadModule64 failed: ") << GetLastError() << std::endl;
	}
	//在程序入口设置断点
  //  SetHardwareBreakpoint(GetCurrentThread(),HWBRK_TYPE_CODE,HWBRK_SIZE_4,AA);


	//SetHardwareBreakpoint(g_hThread,HWBRK_TYPE_CODE,HWBRK_SIZE_4,(void*)pInfo->lpStartAddress);
	//Sleep(1000);
	__try
	{
	//	MessageBox(NULL,L"Success",L"1",1);
		//volatile char a1 = c1[2];
	//	AA();
	}
	__except(GetExceptionCode() == STATUS_SINGLE_STEP)
	{
		MessageBox(NULL,L"Success",L"1",1);
	}


	//调试器调试的时候才会断下来，直接运行不能断下来
    SetCCBreakPointAt((SIZE_T)pInfo->lpStartAddress,SOFTTYPE_ONCE);
//	IsWow64Process(g_hProcess,&m_bIs32bitProcess);
	CloseHandle(pInfo->hFile);
	return TRUE;
}
VOID AA()
{
	
	//MessageBox(NULL,L"1",L"3",1);
}
BOOL OnThreadCreated(const CREATE_THREAD_DEBUG_INFO* pInfo) {
	//多线程的时候插入线程维护列表，暂时还没实现
	//InsertNewThread(stcDbgEvt.dwThreadId,stcDbgEvt.u.CreateThread.hThread);
	return 0;
}
//发生异常的时候应该通知用户，交由用户来处理，所以应返回FALSE。
BOOL OnException(const EXCEPTION_DEBUG_INFO* pInfo) {

	switch (pInfo->ExceptionRecord.ExceptionCode) {

	case EXCEPTION_ACCESS_VIOLATION:      // 非法访问异常
		{
			break;
		}	
	case EXCEPTION_DATATYPE_MISALIGNMENT: // 内存对齐异常
		{	
			break;
		}
	case EXCEPTION_ILLEGAL_INSTRUCTION:   // 无效指令
		{	
			break;
		}
	case EXCEPTION_INT_DIVIDE_BY_ZERO:    // 除0错误
		{	
			break;
		}
	case EXCEPTION_PRIV_INSTRUCTION:      // 指令不支持当前模式
		{	
			break;
		}
	case EXCEPTION_BREAKPOINT:
		{
			/*OnShowSourceLines();*/
			
			return OnSoftBreakPoint(pInfo);   //return false 则中断停下来
		}

	case EXCEPTION_SINGLE_STEP:  //TF断点
		{
			/*OnShowSourceLines();*/
			
			return OnSingleStep(pInfo);
		}
	}

	CString strStatusMsg;
	strStatusMsg.Format(L"First Chance: Exception at %p",(DWORD)pInfo->ExceptionRecord.ExceptionAddress);
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE;
}
//被调试进程已退出，不应再继续执行，所以返回FALSE。
BOOL OnProcessExited(const EXIT_PROCESS_DEBUG_INFO* pInfo) {
	//清理符号信息
	SymCleanup(g_hProcess);


	CString strStatusMsg;
	strStatusMsg.Format(L"Debuggee was terminated.");
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字

	//由于在处理这个事件的时候Debuggee还未真正结束，
	//所以要调用一次ContinueDebugEvent，使它顺利结束。
	ContinueDebugEvent(g_processID, g_threadID, DBG_CONTINUE);

	CloseHandle(g_hThread);
	CloseHandle(g_hProcess);

	g_hProcess = NULL;
	g_hThread = NULL;
	g_processID = 0;
	g_threadID = 0;
	g_debuggeeStatus = STATUS_NONE;
	g_continueStatus = DBG_EXCEPTION_NOT_HANDLED;

	return FALSE;
}
BOOL OnThreadExited(const EXIT_THREAD_DEBUG_INFO* pInfo) {

	return TRUE;
}
//被调试进程输出调试信息，应通知用户并由用户来处理，返回FALSE。
BOOL OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO* pInfo) {

	BYTE* pBuffer = (BYTE*)malloc(pInfo->nDebugStringLength);

	SIZE_T bytesRead;

	ReadProcessMemory(
		g_hProcess,
		pInfo->lpDebugStringData,
		pBuffer, 
		pInfo->nDebugStringLength,
		&bytesRead);

	int requireLen = MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		(LPCSTR)pBuffer,
		pInfo->nDebugStringLength,
		NULL,
		0);

	TCHAR* pWideStr = (TCHAR*)malloc(requireLen * sizeof(TCHAR));

	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		(LPCSTR)pBuffer,
		pInfo->nDebugStringLength,
		pWideStr,
		requireLen);
	CString strStatusMsg = pWideStr;
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
	free(pWideStr);
	free(pBuffer);

	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE;
}
BOOL OnRipEvent(const RIP_INFO* pInfo) {

//std::wcout << TEXT("A RIP_EVENT occured.") << std::endl;

	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE;
}
BOOL OnDllLoaded(const LOAD_DLL_DEBUG_INFO* pInfo) {

	//加载模块的调试信息
	DWORD64 moduleAddress = SymLoadModule64(
		g_hProcess,
		pInfo->hFile, 
		NULL,
		NULL,
		(DWORD64)pInfo->lpBaseOfDll,
		0);

	if (moduleAddress == 0) {

		//std::wcout << TEXT("SymLoadModule64 failed: ") << GetLastError() << std::endl;
	}

	CloseHandle(pInfo->hFile);

	return TRUE;
}
BOOL OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO* pInfo) {
	SymUnloadModule64(g_hProcess, (DWORD64)pInfo->lpBaseOfDll);
	return TRUE;
}

