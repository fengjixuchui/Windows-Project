#include "StdAfx.h"
#include "DispatchException.h"
#include "Common.h"
#include "DealDebugEvent.h"
#include "BreakPoint.h"
#include "DebuggerMainDlg.h"
#include "DeAsmEngine.h"
#include "Symbol.h"


extern CDebugger1Dlg *MainDlg;
HANDLE g_hProcess = NULL;
HANDLE g_hThread = NULL;
DWORD g_processID = 0;
DWORD g_threadID = 0;

HANDLE hThread = NULL;

typedef map<DWORD,HANDLE>::iterator CurrentContextItoa;
map<DWORD,HANDLE>	m_mapTHandleAndTID;// �߳�tid���߳̾����

int g_debuggeeStatus = STATUS_NONE;
DWORD g_continueStatus = DBG_EXCEPTION_NOT_HANDLED;

//�����ֵΪTRUE��������g_continueStatus��ֵ��ʲô��������DBG_CONTINUE
//����ContineDebugEvent����ֵ���ڶϵ㹦�ܡ�
BOOL g_alwaysContinue = FALSE;


VOID CreateDebuggerProcess(WCHAR *szFilePath)
{

	if (g_debuggeeStatus != STATUS_NONE) {
		MessageBox(NULL,L"Debuggee is running.",L"MZ",1);
		return;
	}
	HANDLE Handle=NULL;

	STARTUPINFO si = { 0 };
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = { 0 };
	if (CreateProcess(
		szFilePath,
		NULL,
		NULL,
		NULL,
		FALSE,
		DEBUG_ONLY_THIS_PROCESS | CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi) == FALSE) {
			MessageBox(NULL,L"CreateProcess failed",L"MZ",1);
			return;
	}

	g_hProcess = pi.hProcess;
	g_hThread = pi.hThread;
	g_processID = pi.dwProcessId;
	g_threadID = pi.dwThreadId;

// 	DuplicateHandle( g_hProcess, g_hThread,	GetCurrentProcess(), &hThread, 0, FALSE, DUPLICATE_SAME_ACCESS );
// 	int c= GetLastError();
 	g_debuggeeStatus = STATUS_SUSPENDED;
	ContinueDebugSession();
}

void ContinueDebugSession() {

	if (g_debuggeeStatus == STATUS_NONE) {
		MessageBox(NULL,L"Debuggee is not started yet.",L"MZ",1);
		return;
	}

	if (g_debuggeeStatus == STATUS_SUSPENDED) {

// 		int a = ResumeThread(hThread);  //���سɹ�������ǰһ���̹߳������
// 		int b =0;
// 		b= GetLastError();
	
	}
	else {
		//Ĭ����δ�����쳣
		ContinueDebugEvent(g_processID, g_threadID, g_alwaysContinue == TRUE ? DBG_CONTINUE : g_continueStatus);
		g_alwaysContinue = FALSE;
	}
	DEBUG_EVENT debugEvent;

	while (WaitForDebugEvent(&debugEvent, INFINITE) == TRUE) {
		
		
	//	UpdataCurrentContextInfo(0,debugEvent.dwThreadId);
		if (DispatchDebugEvent(&debugEvent) == TRUE) {

			ContinueDebugEvent(g_processID, g_threadID, g_continueStatus);
		}
		else {
			break;
		}
	}

	//unsigned int unTid = 0;
	//HANDLE hThread = (HANDLE)_beginthreadex(0,0,WaitForEvent,NULL,0,&unTid);

	
}

unsigned int __stdcall WaitForEvent(void* lpParam)
{
	
	return 0;
}

//���ݵ����¼������͵��ò�ͬ�Ĵ�������
BOOL DispatchDebugEvent(const DEBUG_EVENT* pDebugEvent) {

	switch (pDebugEvent->dwDebugEventCode) {

	case CREATE_PROCESS_DEBUG_EVENT:
		{
			InsertNewThread(pDebugEvent->dwThreadId,pDebugEvent->u.CreateProcessInfo.hThread);
			return OnProcessCreated(&pDebugEvent->u.CreateProcessInfo);
		}
	
	case CREATE_THREAD_DEBUG_EVENT:
		{
			InsertNewThread(pDebugEvent->dwThreadId,pDebugEvent->u.CreateThread.hThread);
			return OnThreadCreated(&pDebugEvent->u.CreateThread);
		}
	case EXCEPTION_DEBUG_EVENT:
		{
			return OnException	(&pDebugEvent->u.Exception);
		}
		
	case EXIT_PROCESS_DEBUG_EVENT:
		return OnProcessExited(&pDebugEvent->u.ExitProcess);

	case EXIT_THREAD_DEBUG_EVENT:
		return OnThreadExited(&pDebugEvent->u.ExitThread);

	case LOAD_DLL_DEBUG_EVENT:
		return OnDllLoaded(&pDebugEvent->u.LoadDll);

	case OUTPUT_DEBUG_STRING_EVENT:
		return OnOutputDebugString(&pDebugEvent->u.DebugString);

	case RIP_EVENT:
		return OnRipEvent(&pDebugEvent->u.RipInfo);

	case UNLOAD_DLL_DEBUG_EVENT:
		return OnDllUnloaded(&pDebugEvent->u.UnloadDll);

	default:
		return FALSE;
	}
}


/*
pSegs->ds = cont.SegDs;
pSegs->cs = cont.SegCs;
pSegs->es = cont.SegEs;
pSegs->fs = cont.SegFs;
pSegs->gs = cont.SegGs;
pSegs->ss = cont.SegSs;
*/
//��ȡ�����Խ��̵����̵߳������Ļ�����
BOOL GetDebuggeeContext(CONTEXT* pContext) {

	SuspendThread(g_hThread);
	pContext->ContextFlags = CONTEXT_FULL;

	if (GetThreadContext(g_hThread, pContext) == FALSE) {
		CString strStatusMsg;
		strStatusMsg.Format(L"GetThreadContext failed");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
		return FALSE;
	}
	ResumeThread(g_hThread);
	return TRUE;
}

//���ñ����Խ��̵����̵߳������Ļ���
BOOL SetDebuggeeContext(CONTEXT* pContext) {

	if (SetThreadContext(g_hThread, pContext) == FALSE) {

	//	std::wcout << TEXT("SetThreadContext failed: ") << GetLastError() << std::endl;
	}

	return TRUE;
}




//��ȡ�����Խ��̵��ڴ档
BOOL ReadDebuggeeMemory(unsigned int address, unsigned int length, void* pData) {

	SIZE_T bytesRead;

	return ReadProcessMemory(g_hProcess, (LPCVOID)address, pData, length, &bytesRead);
}

/*

	��ָ�븳ֵ����
	0x00000000~0x0000FFFF

	�û�ģʽ����
	0x00010000~0x7FFEFFFF

	64KB�������
	0x7FFF0000~0x7FFFFFFF

	�ں�ģʽ����
	0x80000000~0xFFFFFFFF

	*/
//��ָ��������д�������Խ��̵��ڴ�
BOOL WriteDebuggeeMemory(unsigned int address, unsigned int length, const void* pData) {

	SIZE_T byteWriten;

	return WriteProcessMemory(g_hProcess, (LPVOID)address, pData, length, &byteWriten);
}

//ֹͣ�����Խ��̵�ִ�С�����Ҫ����һ��ContinueDebugSession��
//�õ���������EXIT_PROCESS_DEBUG_EVENT�¼���
void StopDebugSeesion() {

	if (TerminateProcess(g_hProcess, -1) == TRUE) {

		ContinueDebugSession();
	}
	else {

//		std::wcout << TEXT("TerminateProcess failed: ") << GetLastError() << std::endl;
	}
}


int GetDebuggeeStatus() {
	return g_debuggeeStatus;
}


//ָʾ�Ƿ������쳣�����handledΪTRUE����ʾ�������쳣��
//����Ϊû�д����쳣��
void HandledException(BOOL handled) {

	g_continueStatus = (handled == TRUE) ? DBG_CONTINUE : DBG_EXCEPTION_NOT_HANDLED;
}
//��ȡ�����Խ��̵ľ��
HANDLE GetDebuggeeHandle() {
	return g_hProcess;
}

BOOL OnSoftBreakPoint(DWORD Address,DWORD type)
{
	//CHECK_DEBUGGEE;
	//DisplayBreakPoints();
	if(SetCCBreakPointAt(Address,type) == TRUE)
	{
		CString strStatusMsg;
		strStatusMsg.Format(L"Break Point has Set;");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����

	}
	else
	{
		if (CancelBreakPoint(Address) == TRUE)
		{
			CString strStatusMsg;
			strStatusMsg.Format(L"Break Point has Cancelled;");
			MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
			
		}
	}
	return TRUE;
}



//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// ������: ShowAsm
///��  ��: ���ָ����ַ�ķ����ָ�����̨,��ָ���������
// ��  ��: const LPVOID lpAddress
// ��  ��: DWORD dwLine
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
void  ShowAsm(const LPVOID lpAddress,DWORD dwLine)
{
	MainDlg->m_Text = L"";
	vector<DEASM> vecDeAsm;  //�����ṹ������
	if(-1 == DeAsm((SIZE_T)lpAddress,vecDeAsm,dwLine))
		return;

	for(DWORD i = 0; i < vecDeAsm.size( ); ++i)
	{
		CString Address,Code;
			//	Address.Format(L"%-10p %-40s  %-20s  %-10s",vecDeAsm[i].address,vecDeAsm[i].strOpCode,vecDeAsm[i].strAsm,vecDeAsm[i].strCom);
		Address.Format(L"%p\t\t%s\t\t%s",vecDeAsm[i].address,vecDeAsm[i].strOpCode,vecDeAsm[i].strAsm);
		
		MainDlg->m_Text +=Address;
		MainDlg->m_Text +=vecDeAsm[i].strOpCode;
		MainDlg->m_Text +=L"  ";
		MainDlg->m_Text +=vecDeAsm[i].strAsm;
		MainDlg->m_Text +=L"\t\t";
		MainDlg->m_Text +=vecDeAsm[i].strCom;
		MainDlg->m_Text +=L"\r\n";
// 		HighLight(vecDeAsm[i].address,
// 			(LPWSTR)(LPCTSTR)vecDeAsm[i].strOpCode,
// 			(LPWSTR)(LPCTSTR)vecDeAsm[i].strAsm,
// 			(LPWSTR)(LPCTSTR)vecDeAsm[i].strCom);
	}
	MainDlg->UpdateData(FALSE);
}


void ShowAsm(DWORD dwLine)
{
	CONTEXT	context = {0};
	context.ContextFlags = CONTEXT_CONTROL;   //���EIP��ESP��
	if(!GetThreadContext(g_hThread,(PCONTEXT)&context))
	{
		CString strStatusMsg;
		strStatusMsg.Format(L"��ȡ�̻߳���ʧ��");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
		return;
	}

	ShowAsm((LPVOID)context.Eip,dwLine);
}


void UpdataCurrentContextInfo(DWORD dwPid,DWORD dwTid)
{
	CurrentContextItoa	itoa = m_mapTHandleAndTID.find(dwTid);
	if(itoa != m_mapTHandleAndTID.end( ))
	{
		g_hThread = itoa->second; // ���µ����߳̾��
	}
}

void InsertNewThread(DWORD dwTid,HANDLE hThread)
{
	m_mapTHandleAndTID.insert(pair<DWORD,HANDLE>(dwTid,hThread));

}

void RemoveThread(DWORD dwTid)
{
	m_mapTHandleAndTID.erase(m_mapTHandleAndTID.find(dwTid));
}


void Init(HANDLE hProcess,HANDLE hThread,HANDLE hFile)
{
	g_hProcess = hProcess;
	g_hThread = hThread;
	//g_hFile = hFile;
}

