#include "StdAfx.h"
#include "Flags.h"
#include "DispatchException.h"
#include "DebuggerMainDlg.h"


extern CDebugger1Dlg *MainDlg;

//显示寄存器值命令的处理函数。
//命令格式为 r
void OnShowRegisters() {

	CHECK_DEBUGGEE;

	CONTEXT context = {0};

	if (GetDebuggeeContext(&context) == FALSE) {
		MessageBox(NULL,L"Show registers failed.",L"MZ",1);
		return;
	}
	CString EAX,EBX,ECX,EDX,ESI,EBP,ESP,EIP,EDI;
	EAX.Format(L"EAX: %p",context.Eax);
	EBX.Format(L"EBX: %p",context.Ebx);
	ECX.Format(L"ECX: %p",context.Ecx);
	EDX.Format(L"EDX: %p",context.Edx);
	ESI.Format(L"ESI: %p",context.Esi);
	EBP.Format(L"EBP: %p",context.Ebp);
	ESP.Format(L"ESP: %p",context.Esp);
	EIP.Format(L"EIP: %p",context.Eip);
	EDI.Format(L"EDI: %p",context.Edi);

	MainDlg->m_Eax = EAX;
	MainDlg->m_Ebx = EBX;
	MainDlg->m_Ecx = ECX;
	MainDlg->m_Edx = EDX;
	MainDlg->m_Edi = EDI;
	MainDlg->m_Esi = ESI;
	MainDlg->m_Ebp = EBP;
	MainDlg->m_Esp = ESP;
	MainDlg->m_Eip = EIP;



	//CF 在第0位
	if ((context.EFlags & 0x1) != 0) {

		MainDlg->m_CF = L"CF  1";
	}
	else
	{
		MainDlg->m_CF = L"CF  0"; 
	}

	//PF 在第2位
	if ((context.EFlags & 0x4) != 0) {

		MainDlg->m_PF = L"PF  1";
	}
	else
	{
		MainDlg->m_PF = L"PF  0";
	}

	//AF 在第4位
	if ((context.EFlags & 0x10) != 0) {

		MainDlg->m_AF = L"AF  1";
	}
	else
	{
		MainDlg->m_AF = L"AF  0";
	}

	//ZF 在第6位
	if ((context.EFlags & 0x40) != 0) {

		MainDlg->m_ZF = L"ZF  1";
	}
	else
	{
		MainDlg->m_ZF = L"ZF  0";
	}

	//SF 在第7位
	if ((context.EFlags & 0x80) != 0) {

		MainDlg->m_SF =  L"SF  1";
	}
	else{
		MainDlg->m_SF =  L"SF  0";
	}

	//OF 在第11位
	if ((context.EFlags & 0x400) != 0) {

		MainDlg->m_OF  = L"OF  1";
	}
	else
	{
		MainDlg->m_OF  = L"OF  0";
	}

	//DF 在第10位
	if ((context.EFlags & 0x200) != 0) {

		MainDlg->m_DF = L"DF  1";
	}
	else
	{
		MainDlg->m_DF = L"DF  0";
	}
	MainDlg->UpdateData(FALSE);
}