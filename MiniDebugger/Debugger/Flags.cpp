#include "StdAfx.h"
#include "Flags.h"
#include "DispatchException.h"
#include "DebuggerMainDlg.h"


extern CDebugger1Dlg *MainDlg;

//��ʾ�Ĵ���ֵ����Ĵ�������
//�����ʽΪ r
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



	//CF �ڵ�0λ
	if ((context.EFlags & 0x1) != 0) {

		MainDlg->m_CF = L"CF  1";
	}
	else
	{
		MainDlg->m_CF = L"CF  0"; 
	}

	//PF �ڵ�2λ
	if ((context.EFlags & 0x4) != 0) {

		MainDlg->m_PF = L"PF  1";
	}
	else
	{
		MainDlg->m_PF = L"PF  0";
	}

	//AF �ڵ�4λ
	if ((context.EFlags & 0x10) != 0) {

		MainDlg->m_AF = L"AF  1";
	}
	else
	{
		MainDlg->m_AF = L"AF  0";
	}

	//ZF �ڵ�6λ
	if ((context.EFlags & 0x40) != 0) {

		MainDlg->m_ZF = L"ZF  1";
	}
	else
	{
		MainDlg->m_ZF = L"ZF  0";
	}

	//SF �ڵ�7λ
	if ((context.EFlags & 0x80) != 0) {

		MainDlg->m_SF =  L"SF  1";
	}
	else{
		MainDlg->m_SF =  L"SF  0";
	}

	//OF �ڵ�11λ
	if ((context.EFlags & 0x400) != 0) {

		MainDlg->m_OF  = L"OF  1";
	}
	else
	{
		MainDlg->m_OF  = L"OF  0";
	}

	//DF �ڵ�10λ
	if ((context.EFlags & 0x200) != 0) {

		MainDlg->m_DF = L"DF  1";
	}
	else
	{
		MainDlg->m_DF = L"DF  0";
	}
	MainDlg->UpdateData(FALSE);
}