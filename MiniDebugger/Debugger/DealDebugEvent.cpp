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

//	Init(pInfo->hProcess,pInfo->hThread,pInfo->hFile); //��ʼ����ǰ���̵ľ�����߳̾�����ļ����
	
	InitializeBreakPointHelper();

	//��ʼ�����Ŵ�����
	//ע�⣬���ﲻ��ʹ��pInfo->hProcess����Ϊg_hProcess��pInfo->hProcess
	//��ֵ������ͬ��������DbgHelp����ʹ�õ���g_hProcess��
	InitSymbol();
	//HANDLE Thread = __beginthread
	char c1[100] = {0};
	//����ģ��ĵ�����Ϣ
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
	//�ڳ���������öϵ�
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


	//���������Ե�ʱ��Ż��������ֱ�����в��ܶ�����
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
	//���̵߳�ʱ������߳�ά���б���ʱ��ûʵ��
	//InsertNewThread(stcDbgEvt.dwThreadId,stcDbgEvt.u.CreateThread.hThread);
	return 0;
}
//�����쳣��ʱ��Ӧ��֪ͨ�û��������û�����������Ӧ����FALSE��
BOOL OnException(const EXCEPTION_DEBUG_INFO* pInfo) {

	switch (pInfo->ExceptionRecord.ExceptionCode) {

	case EXCEPTION_ACCESS_VIOLATION:      // �Ƿ������쳣
		{
			break;
		}	
	case EXCEPTION_DATATYPE_MISALIGNMENT: // �ڴ�����쳣
		{	
			break;
		}
	case EXCEPTION_ILLEGAL_INSTRUCTION:   // ��Чָ��
		{	
			break;
		}
	case EXCEPTION_INT_DIVIDE_BY_ZERO:    // ��0����
		{	
			break;
		}
	case EXCEPTION_PRIV_INSTRUCTION:      // ָ�֧�ֵ�ǰģʽ
		{	
			break;
		}
	case EXCEPTION_BREAKPOINT:
		{
			/*OnShowSourceLines();*/
			
			return OnSoftBreakPoint(pInfo);   //return false ���ж�ͣ����
		}

	case EXCEPTION_SINGLE_STEP:  //TF�ϵ�
		{
			/*OnShowSourceLines();*/
			
			return OnSingleStep(pInfo);
		}
	}

	CString strStatusMsg;
	strStatusMsg.Format(L"First Chance: Exception at %p",(DWORD)pInfo->ExceptionRecord.ExceptionAddress);
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE;
}
//�����Խ������˳�����Ӧ�ټ���ִ�У����Է���FALSE��
BOOL OnProcessExited(const EXIT_PROCESS_DEBUG_INFO* pInfo) {
	//���������Ϣ
	SymCleanup(g_hProcess);


	CString strStatusMsg;
	strStatusMsg.Format(L"Debuggee was terminated.");
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����

	//�����ڴ�������¼���ʱ��Debuggee��δ����������
	//����Ҫ����һ��ContinueDebugEvent��ʹ��˳��������
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
//�����Խ������������Ϣ��Ӧ֪ͨ�û������û�����������FALSE��
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
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
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

	//����ģ��ĵ�����Ϣ
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

