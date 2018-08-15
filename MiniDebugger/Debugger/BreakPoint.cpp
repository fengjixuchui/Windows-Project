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
std::list<PointInfo>  g_BpList; //����ж�
vector<HardPointInfo>	 m_vecHard; // Ӳ���жϱ�
BOOL g_isInitBpOccured = FALSE;

DWORD g_resetUserBpAddress;
HWBRK_TYPE SaveType;
HWBRK_SIZE SaveSize;
DWORD Saveaddress;


//�����û����öϵ������
const std::list<PointInfo>& GetBreakPoints() {

	return g_BpList;
}




//��ʼ���ϵ����
void InitializeBreakPointHelper() {

	g_BpList.clear();
	g_isInitBpOccured = FALSE;
	g_resetUserBpAddress = 0;
}


//��ȡ�ϵ������
int GetBreakPointType(DWORD address) {

	//�����ʼ�ϵ�δ��������ض��ǳ�ʼ�ϵ�
	if (g_isInitBpOccured == FALSE) {
		g_isInitBpOccured = TRUE;
		return BP_INIT;
	}

	//���û����õĶϵ���Ѱ��
	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == address) {
				return it->ptType;
			}
	}

	//���������������㣬���Ǳ����Խ����еĶϵ�
	return BP_CODE;
}



//��ָ���ĵ�ַ���öϵ�
BOOL SetCCBreakPointAt(DWORD address,DWORD type) {

	//����Ƿ��Ѵ��ڸöϵ�
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

//��ָ����ַ���öϵ㣬����ֵΪԭָ��ĵ�һ���ֽ�  д��CC
BYTE SetBreakPointMemory(DWORD address) {

	BYTE byte;
	ReadDebuggeeMemory(address, 1, &byte);

	BYTE intInst = 0xCC;
	WriteDebuggeeMemory(address, 1, &intInst);

	return byte;
}


//ȡ��ָ����ַ�Ķϵ�
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


//�ָ��ϵ�����ָ���һ���ֽڵ�����
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



//�ָ��ϵ�����ָ��ĵ�һ���ֽ�
void RecoverBreakPoint(const PointInfo* pBreakPoint) {

	WriteDebuggeeMemory(pBreakPoint->lpPointAddr, 1, &pBreakPoint->chOldByte);
}

//��ʾ�����õĶϵ�
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


//��������������öϵ�
VOID SetBreakPointAtEntryPoint(DWORD Address)
{
	if(Address!=0)
	{
		if (SetCCBreakPointAt(Address,SOFTTYPE_ONCE) == FALSE) {
		    MessageBox(NULL,L"Set break point at entry function failed.",L"MZ",1);	
		}
	}
}

//���������������õ��û��ϵ��ַ  cc�ϵ���  ���浱ǰ��ַ�����б��е�ַ�Ƚ��Ƿ�Ҫ����д��
void SaveResetUserBreakPoint(DWORD address) {

	g_resetUserBpAddress = address;
}


//����ϵ��쳣
BOOL OnSoftBreakPoint(const EXCEPTION_DEBUG_INFO* pInfo) {

	//��һ�ν��յ��ϵ�
	if (pInfo->dwFirstChance == TRUE) {

		//��ȡ�ϵ�����
		int bpType = GetBreakPointType((DWORD)pInfo->ExceptionRecord.ExceptionAddress);

		if(bpType==BP_CC)
		{
	

			//�ڶ��ν��յ��ϵ㣬�ָ��ϵ�����ָ��ĵ�һ���ֽڡ���ֻ���û����õĶϵ���еڶ��ν��գ�
			RecoverUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);

			SaveResetUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);

			//����ϵ㣬�ָ��ĵط���eip+1�ĵط�������Ҫ�ضϵ�ĵط�����ִ��
			CONTEXT context = {0};
			GetDebuggeeContext(&context);
			--context.Eip;
			SetDebuggeeContext(&context);
			//����TF��־

			for (std::list<PointInfo>::iterator it = g_BpList.begin(); 
				it != g_BpList.end();
				++it) {

					if (it->lpPointAddr == (DWORD)pInfo->ExceptionRecord.ExceptionAddress) 
					{
						if(it->type!=SOFTTYPE_ONCE) //�����һ�ζϵ������������öϵ�
						{
							SetTrapFlag();
						}
						g_resetUserBpAddress = 0;
					}
			}

			//HandledException(TRUE);
			//return TRUE;//�쳣�Ѿ��ָ�
		}
		//CONTEXT context;
		//GetDebuggeeContext(&context);
		//������Ǵ����еĶϵ��򲻴����쳣�����������Խ������С�
		//if (bpType != BP_CODE) {
		//	HandledException(FALSE);
		//	return FALSE;
		//}
	}
	else {

		//�ڶ��ν��յ��ϵ㣬�ָ��ϵ�����ָ��ĵ�һ���ֽڡ���ֻ���û����õĶϵ���еڶ��ν��գ�
		//RecoverUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);
		//SaveResetUserBreakPoint((DWORD)pInfo->ExceptionRecord.ExceptionAddress);
		int a = 0;
		//�ڶ��ε�ʱ���Ѿ�-1��
		//CONTEXT context;
		//GetDebuggeeContext(&context);
		//--context.Eip;
		//SetDebuggeeContext(&context);
		
		//����TF��־,����ָ�
		//SetTrapFlag();
	}

	//����Ǵ����еĶϵ�����û����õĶϵ㣬����ͣ�����Խ��̵�ִ�У���֪ͨ�û���
	CString strStatusMsg;
	strStatusMsg.Format(L"A break point occured at %p",(DWORD)pInfo->ExceptionRecord.ExceptionAddress);
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
	
	OnDump(0);
	ShowAsm(20);
	OnShowRegisters();

	g_alwaysContinue = TRUE;  //�û���Go��ִ��ContinueDebugEvent������DEBUG_CONTINUE
	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE; //����������
}

//����ִ���쳣�Ĵ�������
BOOL OnSingleStep(const EXCEPTION_DEBUG_INFO* pInfo) {
	//MessageBox(NULL,L"2",L"22",1);
	if(g_resetUserBpAddress != 0)
	{
		for (std::list<PointInfo>::iterator it = g_BpList.begin(); 
		it != g_BpList.end();
		++it) {

			if (it->lpPointAddr == g_resetUserBpAddress) 
			{
				if(it->type!=SOFTTYPE_ONCE) //�����һ�ζϵ������������öϵ�
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
			MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
			
			HWBRK_TYPE Type = m_vecHard[i].TypeREADWRITECODE;
			HWBRK_SIZE Size = m_vecHard[i].HardPointSize;
			UnSetHardwareBreakpoint(m_vecHard[i].TypeREADWRITECODE,m_vecHard[i].HardPointSize,(PVOID)m_vecHard[i].lpPointAddr);
			SaveResetUserHardBreakPoint(Type,Size,(DWORD)pInfo->ExceptionRecord.ExceptionAddress);

			//SetTrapFlag();

			OnDump(0);
			ShowAsm(20);
			OnShowRegisters();
			g_alwaysContinue = TRUE;  //�û���Go��ִ��ContinueDebugEvent������DEBUG_CONTINUE
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
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
	g_alwaysContinue = TRUE;  //�û���Go��ִ��ContinueDebugEvent������DEBUG_CONTINUE
	g_debuggeeStatus = STATUS_INTERRUPTED;
	return FALSE; //����������
	
}

//����Ӳ���ϵ�
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
	if (ct.Dr7 & 1) //0λ  0 local
		Dr0Busy = true;
	if (ct.Dr7 & 4) //2λ  1 local
		Dr1Busy = true;
	if (ct.Dr7 & 16)//4λ  2 local
		Dr2Busy = true;
	if (ct.Dr7 & 64)//6λ  3 local
		Dr3Busy = true;
	
	
		if (!Dr0Busy)
		{  bkpt.Number = 0;
			iReg = 0;
			ct.Dr0 = (DWORD_PTR)s;  //��ַ
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
	MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����


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
	if (ct.Dr7 & 1) //0λ  0 local
		Dr0Busy = true;
	if (ct.Dr7 & 4) //2λ  1 local
		Dr1Busy = true;
	if (ct.Dr7 & 16)//4λ  2 local
		Dr2Busy = true;
	if (ct.Dr7 & 64)//6λ  3 local
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



//����TF��־λ
void SetTrapFlag() {

	CONTEXT context = {0};

	GetDebuggeeContext(&context);

	context.EFlags |= 0x100;

	SetDebuggeeContext(&context);


}


BOOL MoveOut()
{
	// ��ȡebp
	CONTEXT	Context = {0};
	GetDebuggeeContext(&Context);
	// ��ȡebp+4������ķ��ص�ַ
	SIZE_T	addr = 0;
	if(!ReadDebuggeeMemory(Context.Ebp + 4,sizeof(addr),(LPVOID)&addr))
	{
		return FALSE;
	}
	// ����һ���Զϵ�
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