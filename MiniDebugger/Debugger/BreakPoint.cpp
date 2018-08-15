#include "StdAfx.h"
#include "BreakPoint.h"
#include "ShowDumpText.h"
#include "Flags.h"
#include "DeAsmEngine.h"

extern DWORD g_threadID;
extern HANDLE g_hThread;
extern int g_debuggeeStatus;
extern BOOL g_alwaysContinue;
extern CDebugger1Dlg *MainDlg;
std::list<PointInfo>  g_BpList; //软件中断
vector<HardPointInfo>	 m_vecHard; // 硬件中断表
BOOL g_isInitBpOccured = FALSE;

DWORD g_resetUserBpAddress;
HWBRK_TYPE SaveType;
HWBRK_SIZE SaveSize;
DWORD Saveaddress;


//返回用户设置断点的链表
const std::list<PointInfo>& GetBreakPoints() {

	return g_BpList;
}




//初始化断点组件
void InitializeBreakPointHelper() {

	g_BpList.clear();
	g_isInitBpOccured = FALSE;
	g_resetUserBpAddress = 0;
}


//获取断点的类型
int GetBreakPointType(DWORD address) {

	//如果初始断点未触发，则必定是初始断点
	if (g_isInitBpOccured == FALSE) {
		g_isInitBpOccured = TRUE;
		return BP_INIT;
	}

	//在用户设置的断点中寻找
	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == address) {
				return it->ptType;
			}
	}

	//以上条件都不满足，则是被调试进程中的断点
	return BP_CODE;
}



//在指定的地址设置断点
BOOL SetCCBreakPointAt(DWORD address,DWORD type) {

	//检查是否已存在该断点
	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == address) {

				//MessageBox(NULL,L"Break point has existed.",L"MZ",1);
				return FALSE;
			}
	}

	PointInfo newBp;
	newBp.lpPointAddr = address;
	newBp.chOldByte = SetBreakPointMemory(newBp.lpPointAddr);
	newBp.type = type;
	newBp.ptType = BP_CC;
	g_BpList.push_back(newBp);

	return TRUE;
}

//在指定地址设置断点，返回值为原指令的第一个字节  写入CC
BYTE SetBreakPointMemory(DWORD address) {

	BYTE byte;
	ReadDebuggeeMemory(address, 1, &byte);

	BYTE intInst = 0xCC;
	WriteDebuggeeMemory(address, 1, &intInst);

	return byte;
}


//取消指定地址的断点
BOOL CancelBreakPoint(DWORD address) {

	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == address) {

				RecoverBreakPoint(&*it);
				g_BpList.erase(it);

				return TRUE;
			}
	}

	MessageBox(NULL,L"Break point does not exist.",L"MZ",1);

	return FALSE;
}


//恢复断点所在指令第一个字节的内容
BOOL RecoverUserBreakPoint(DWORD address) {

	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == address) {

				RecoverBreakPoint(&*it);
				return TRUE;
			}
	}

	MessageBox(NULL,L"Break point does not exist.",L"MZ",1);

	return FALSE;
}



//恢复断点所在指令的第一个字节
void RecoverBreakPoint(const PointInfo* pBreakPoint) {

	WriteDebuggeeMemory(pBreakPoint->lpPointAddr, 1, &pBreakPoint->chOldByte);
}

//显示已设置的断点
void DisplayBreakPoints() {

	const std::list<PointInfo> bpList = GetBreakPoints();

	if (bpList.size() == 0) {
		MessageBox(NULL,L"No break point.",L"MZ",1);
		return;
	}

	for (std::list<PointInfo>::const_iterator it = bpList.cbegin();
		it != bpList.cend();
		++it) {

			//PrintHex(it->address, FALSE);
			//std::wcout << std::endl;
	}
}


//程序启动入口设置断点
VOID SetBreakPointAtEntryPoint(DWORD Address)
{
	if(Address!=0)
	{
		if (SetCCBreakPointAt(Address,SOFTTYPE_ONCE) == FALSE) {
		    MessageBox(NULL,L"Set break point at entry function failed.",L"MZ",1);	
		}
	}
}

//保存用于重新设置的用户断点地址  cc断点中  保存当前地址，与列表中地址比较是否要重新写入
void SaveResetUserBreakPoint(DWORD address) {

	g_resetUserBpAddress = address;
}


//处理断点异常
BOOL OnSoftBreakPoint(const EXCEPTION_DEBUG_INFO* pInfo) {

	//第一次接收到断点
	if (pInfo->dwFirstChance == TRUE) {

		//获取断点类型
		int bpType = GetBreakPointType((DWORD)pInfo->ExceptionRecord.ExceptionAddress);

		if(bpType==BP_CC)
		{
	

			//第二次接收到断点，恢复断点所在指令的第一个字节。（只有用户设置的断点才有第二次接收）
			RecoverUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);

			SaveResetUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);

			//软件断点，恢复的地方在eip+1的地方，所以要重断点的地方重新执行
			CONTEXT context = {0};
			GetDebuggeeContext(&context);
			--context.Eip;
			SetDebuggeeContext(&context);
			//设置TF标志

			for (std::list<PointInfo>::iterator it = g_BpList.begin(); 
				it != g_BpList.end();
				++it) {

					if (it->lpPointAddr == (DWORD)pInfo->ExceptionRecord.ExceptionAddress) 
					{
						if(it->type!=SOFTTYPE_ONCE) //如果是一次断点则不用重新设置断点
						{
							SetTrapFlag();
						}
						g_resetUserBpAddress = 0;
					}
			}

			//HandledException(TRUE);
			//return TRUE;//异常已经恢复
		}
		//CONTEXT context;
		//GetDebuggeeContext(&context);
		//如果不是代码中的断点则不处理异常，继续被调试进程运行。
		//if (bpType != BP_CODE) {
		//	HandledException(FALSE);
		//	return FALSE;
		//}
	}
	else {

		//第二次接收到断点，恢复断点所在指令的第一个字节。（只有用户设置的断点才有第二次接收）
		//RecoverUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);
		//SaveResetUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);
		int a = 0;
		//第二次的时候已经-1了
		//CONTEXT context;
		//GetDebuggeeContext(&context);
		//--context.Eip;
		//SetDebuggeeContext(&context);
		
		//设置TF标志,方便恢复
		//SetTrapFlag();
	}

	//如果是代码中的断点或者用户设置的断点，则暂停被调试进程的执行，并通知用户。
	CString strStatusMsg;
	strStatusMsg.Format(L"A break point occured at %p",(DWORD)pInfo->ExceptionRecord.ExceptionAddress);
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
	
	OnDump(0);
	ShowAsm(20);
	OnShowRegisters();

	g_alwaysContinue = TRUE;  //用户按Go会执行ContinueDebugEvent，传入DEBUG_CONTINUE
	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE; //程序会断下来
}

//单步执行异常的处理函数。
BOOL OnSingleStep(const EXCEPTION_DEBUG_INFO* pInfo) {
	//MessageBox(NULL,L"2",L"22",1);
	if(g_resetUserBpAddress != 0)
	{
		for (std::list<PointInfo>::iterator it = g_BpList.begin(); 
		it != g_BpList.end();
		++it) {

			if (it->lpPointAddr == g_resetUserBpAddress) 
			{
				if(it->type!=SOFTTYPE_ONCE) //如果是一次断点则不用重新设置断点
				{
					SetBreakPointMemory(it->lpPointAddr);	
				}
				g_resetUserBpAddress = 0;
			}
		}
	}
	CONTEXT Context = {0};
	GetDebuggeeContext(&Context);
	for(DWORD i = 0; i < m_vecHard.size( ); ++i)
	{
		if(m_vecHard[i].lpPointAddr ==(DWORD)pInfo->ExceptionRecord.ExceptionAddress)
		{
			CString strStatusMsg;
			strStatusMsg.Format(L"A Hard break point occured at %p",(DWORD)pInfo->ExceptionRecord.ExceptionAddress);
			MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
			
			HWBRK_TYPE Type = m_vecHard[i].TypeREADWRITECODE;
			HWBRK_SIZE Size = m_vecHard[i].HardPointSize;
			UnSetHardwareBreakpoint(m_vecHard[i].TypeREADWRITECODE,m_vecHard[i].HardPointSize,(PVOID)m_vecHard[i].lpPointAddr);
			SaveResetUserHardBreakPoint(Type,Size,(DWORD)pInfo->ExceptionRecord.ExceptionAddress);

			//SetTrapFlag();

			OnDump(0);
			ShowAsm(20);
			OnShowRegisters();
			g_alwaysContinue = TRUE;  //用户按Go会执行ContinueDebugEvent，传入DEBUG_CONTINUE
			g_debuggeeStatus = STATUS_INTERRUPTED;
			return FALSE;
			//m_vecHard.erase(m_vecHard.begin( ) + i);
		}
	}
	if(Saveaddress!=0)
	{
		//SetHardwareBreakpoint(NULL,SaveType,SaveSize,(PVOID)Saveaddress);
		Saveaddress =0;
		return TRUE;
	}

	OnDump(0);
	ShowAsm(20);
	OnShowRegisters();

	CString strStatusMsg;
	strStatusMsg.Format(L"A break point occured at %p",(DWORD)pInfo->ExceptionRecord.ExceptionAddress);
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
	g_alwaysContinue = TRUE;  //用户按Go会执行ContinueDebugEvent，传入DEBUG_CONTINUE
	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE; //程序会断下来
	
}

//设置硬件断点
HANDLE SetHardwareBreakpoint(HANDLE hThread,HWBRK_TYPE Type,HWBRK_SIZE Size,void* s)
{	

	if(m_vecHard.size( ) > 3) 
		return FALSE;

	HardPointInfo bkpt;
	bkpt.lpPointAddr = (DWORD)s;
	bkpt.ptType = DR_POINT;
	bkpt.TypeREADWRITECODE = Type;
	bkpt.HardPointSize = Size;


	//	bkpt.type = dwType;

	int j = 0;
	int y = 0;
	CONTEXT ct = {0};
	int iReg = 0;

	

	j =SuspendThread(g_hThread);
	y = GetLastError();

	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS|CONTEXT_FULL;
	if(!GetThreadContext(g_hThread,&ct))
	{
		y = GetLastError();
		MessageBox(NULL,L"Fail",L"1",1);
	}
	int FlagBit = 0;

	bool Dr0Busy = false;
	bool Dr1Busy = false;
	bool Dr2Busy = false;
	bool Dr3Busy = false;
	if (ct.Dr7 & 1) //0位  0 local
		Dr0Busy = true;
	if (ct.Dr7 & 4) //2位  1 local
		Dr1Busy = true;
	if (ct.Dr7 & 16)//4位  2 local
		Dr2Busy = true;
	if (ct.Dr7 & 64)//6位  3 local
		Dr3Busy = true;
	
	
		if (!Dr0Busy)
		{  bkpt.Number = 0;
			iReg = 0;
			ct.Dr0 = (DWORD_PTR)s;  //地址
			Dr0Busy = true; 
		}
		else
			if (!Dr1Busy)
			{
				 bkpt.Number = 1;
				iReg = 1;
				ct.Dr1 = (DWORD_PTR)s;
				Dr1Busy = true;
			}
			else
				if (!Dr2Busy)
				{ bkpt.Number= 2;
					iReg = 2;
					ct.Dr2 = (DWORD_PTR)s;
					Dr2Busy = true;
				}
				else
					if (!Dr3Busy)
					{
						bkpt.Number = 3;
						iReg = 3;
						ct.Dr3 = (DWORD_PTR)s;
						Dr3Busy = true;
					}
					else
					{
						//h->SUCC = false;
						j = ResumeThread(g_hThread);
						y = GetLastError();
						return 0;
					}
					ct.Dr6 = 0;
					int st = 0;
					if (Type == HWBRK_TYPE_CODE)
						st = 0;
					if (Type == HWBRK_TYPE_READWRITE)
						st = 3;
					if (Type == HWBRK_TYPE_WRITE)
						st = 1;
					int le = 0;
					if (Size == HWBRK_SIZE_1)
						le = 0;
					if (Size == HWBRK_SIZE_2)
						le = 1;
					if (Size == HWBRK_SIZE_4)
						le = 3;
					if (Size == HWBRK_SIZE_8)
						le = 2;

					SetBits(ct.Dr7, 16 + iReg*4, 2, st);
					SetBits(ct.Dr7, 18 + iReg*4, 2, le);
					SetBits(ct.Dr7, iReg*2,1,1);
	



	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if(!SetThreadContext(g_hThread,&ct))
	{
		y = GetLastError();
		MessageBox(NULL,L"Fail",L"1",1);
	}
	CString strStatusMsg;
	strStatusMsg.Format(L"Hard Break Point has Set;");
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字


	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	if(!GetThreadContext(g_hThread,&ct))
	{
		y = GetLastError();
		MessageBox(NULL,L"Fail",L"1",1);
	}

	j = ResumeThread(g_hThread);

	y = GetLastError();
	


	//h->SUCC = true;

	//SetEvent(h->hEv);
	m_vecHard.push_back(bkpt);
	return 0;
}
void SaveResetUserHardBreakPoint(HWBRK_TYPE Type,HWBRK_SIZE Size,DWORD address) {
	Saveaddress  = address;
	SaveType = Type;
	SaveSize = SaveSize;
}


HANDLE UnSetHardwareBreakpoint(HWBRK_TYPE Type,HWBRK_SIZE Size,void* s)
{
	int j = 0;
	int y = 0;
	CONTEXT ct = {0};
	int iReg = 0;

	j = SuspendThread(g_hThread);
	y = GetLastError();

	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	j = GetThreadContext(g_hThread,&ct);
	y = GetLastError();
	int FlagBit = 0;

	bool Dr0Busy = false;
	bool Dr1Busy = false;
	bool Dr2Busy = false;
	bool Dr3Busy = false;
	if (ct.Dr7 & 1) //0位  0 local
		Dr0Busy = true;
	if (ct.Dr7 & 4) //2位  1 local
		Dr1Busy = true;
	if (ct.Dr7 & 16)//4位  2 local
		Dr2Busy = true;
	if (ct.Dr7 & 64)//6位  3 local
		Dr3Busy = true;

	//SetEvent(h->hEv);
	for(DWORD i = 0; i < m_vecHard.size( ); ++i)
	{
		if(m_vecHard[i].lpPointAddr == (DWORD)s)
		{

			// Remove
			if (m_vecHard[i].Number == 0)
			{
				FlagBit = 0;
				ct.Dr0 = 0;
				Dr0Busy = false;
			}
			if (m_vecHard[i].Number == 1)
			{
				FlagBit = 2;
				ct.Dr1 = 0;
				Dr1Busy = false;
			}
			if (m_vecHard[i].Number == 2)
			{
				FlagBit = 4;
				ct.Dr2 = 0;
				Dr2Busy = false;
			}
			if (m_vecHard[i].Number == 3)
			{
				FlagBit = 6;
				ct.Dr3 = 0;
				Dr3Busy = false;
			}

			ct.Dr7 &= ~(1 << FlagBit);


			m_vecHard.erase(m_vecHard.begin( ) + i);



		}
	}


	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	j = SetThreadContext(g_hThread,&ct);
	y = GetLastError();

	ct.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	j = GetThreadContext(g_hThread,&ct);
	y = GetLastError();

	j = ResumeThread(g_hThread);
	y = GetLastError();

	return 0;
}

void SetBits(DWORD_PTR& dw, int lowBit, int bits, int newValue)
{
	DWORD_PTR mask = (1 << bits) - 1; 
	dw = (dw & ~(mask << lowBit)) | (newValue << lowBit);
}



//设置TF标志位
void SetTrapFlag() {

	CONTEXT context = {0};

	GetDebuggeeContext(&context);

	context.EFlags |= 0x100;

	SetDebuggeeContext(&context);


}


BOOL MoveOut()
{
	// 获取ebp
	CONTEXT	Context = {0};
	GetDebuggeeContext(&Context);
	// 获取ebp+4处保存的返回地址
	SIZE_T	addr = 0;
	if(!ReadDebuggeeMemory(Context.Ebp + 4,sizeof(addr),(LPVOID)&addr))
	{
		return FALSE;
	}
	// 设置一次性断点
	SetCCBreakPointAt(addr,SOFTTYPE_ONCE);
	return TRUE;
}


BOOL MoveOver()
{
	CONTEXT	Context = {0};
	GetDebuggeeContext(&Context);
	SIZE_T addr = GetCoodeLen(Context.Eip) + Context.Eip;
	SetCCBreakPointAt(addr,SOFTTYPE_ONCE);
	return TRUE;
}