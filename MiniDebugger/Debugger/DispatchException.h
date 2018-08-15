#pragma once

int GetDebuggeeStatus();
#define CHECK_DEBUGGEE \
	do { \
	if (GetDebuggeeStatus() == STATUS_NONE) {	\
	MessageBox(NULL,L"Debuggee has not started yet.",L"MZ",1); \
	return; \
	} \
	} \
	while (0)

void	UpdataCurrentContextInfo(DWORD dwPid,DWORD dwTid);
void	InsertNewThread(DWORD	dwTid,HANDLE hThread);
void	RemoveThread(DWORD dwTid);
void Init(HANDLE hProcess,HANDLE hThread,HANDLE hFile);

VOID CreateDebuggerProcess(WCHAR *szFilePath);
void ContinueDebugSession();
BOOL DispatchDebugEvent(const DEBUG_EVENT* pDebugEvent);

BOOL GetDebuggeeContext(CONTEXT* pContext);
BOOL SetDebuggeeContext(CONTEXT* pContext);


BOOL ReadDebuggeeMemory(unsigned int address, unsigned int length, void* pData);
BOOL WriteDebuggeeMemory(unsigned int address, unsigned int length, const void* pData);



void StopDebugSeesion();
void HandledException(BOOL handled);
HANDLE GetDebuggeeHandle();
BOOL OnSoftBreakPoint(DWORD Address,DWORD type);

unsigned int __stdcall WaitForEvent(void* lpParam);
void ShowAsm(DWORD dwLine);
void  ShowAsm(const LPVOID lpAddress,DWORD dwLine);