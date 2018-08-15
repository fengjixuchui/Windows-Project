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
// ������: GetCoodeLen
///��  ��: ��ȡָ���
// ��  ��: HANDLE g_hCurrentProcess
// ��  ��: const LPVOID lpAddress
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
int GetCoodeLen(const SIZE_T lpAddress)
{
	// 1. �����Գ�����ڴ�(OPCode)���Ƶ�����
	SIZE_T  dwRetSize = 0;
	LPVOID lpRemote_Buf = new BYTE[32];
	ZeroMemory(lpRemote_Buf,32);
	ReadProcessMemory(g_hProcess,(LPVOID)lpAddress,lpRemote_Buf,32,&dwRetSize);

	// 2. ��ʼ�����������
	DISASM objDiasm;
	objDiasm.EIP = (UIntPtr)lpRemote_Buf; // ��ʼ��ַ
	objDiasm.VirtualAddr = (UINT64)lpAddress;     // �����ڴ��ַ��������������ڼ����ַ��
	objDiasm.Archi = 0;                     // AI-X86
	objDiasm.Options = 0x000;                 // MASM

	// 3. ��������
	int nLen = Disasm(&objDiasm);
	delete[] lpRemote_Buf;
	return nLen;
}


//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
// ������: DeAsm
///��  ��: ��ȡopcode�ͻ��ָ��
// ��  ��: HANDLE g_hCurrentProcess
// ��  ��: const LPVOID lpAddress
// ��  ��: TCHAR * pszOpCode
// ��  ��: TCHAR * pszAsm
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
int DeAsm(const SIZE_T lpAddress,CString& strOpCode,CString& strAsm,CString & strCom)
{
	strOpCode.Empty( );
	strAsm.Empty( );
	strCom.Empty( );

	// 1. �����Գ�����ڴ�(OPCode)���Ƶ�����
	DWORD  dwRetSize = 0;
	LPVOID lpRemote_Buf = new BYTE[64];
	ZeroMemory(lpRemote_Buf,64);
	// 	if(!ReadProcMemory((SIZE_T)lpAddress,lpRemote_Buf,32))
	// 		return -1;
	SIZE_T nRead = 0;
	if(!ReadProcessMemory(g_hProcess,(LPVOID)lpAddress,lpRemote_Buf,64,&nRead))
	{
		CString strStatusMsg;
		strStatusMsg.Format(L"��ȡ�ڴ����");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
		return -1;
	}
	//��ʾ���������ʱ��ʾ  ԭ�������� 0xCC
	for (std::list<PointInfo>::const_iterator it = g_BpList.cbegin();
		it != g_BpList.cend();
		++it) {

			if (it->lpPointAddr == lpAddress) {
				*((LPBYTE)lpRemote_Buf) = it->chOldByte;
			}
	}

	// 2. ��ʼ�����������
	DISASM objDiasm;
	objDiasm.EIP = (UIntPtr)lpRemote_Buf; // ��ʼ��ַ
	objDiasm.VirtualAddr = (UINT64)lpAddress;// �����ڴ��ַ��������������ڼ����ַ��
	objDiasm.Archi = 0;                     // AI-X86
	objDiasm.Options = 0x000;                 // MASM

	// 3. ��������
	UINT unLen = Disasm(&objDiasm);
	if(-1 == unLen) 
		return unLen;
	// 4. ��������ת��Ϊ�ַ���
	strOpCode.GetBufferSetLength(MAX_OPCODESTRING);
	byteArr2HexStr((LPBYTE)lpRemote_Buf,unLen,(LPWSTR)(LPCTSTR)strOpCode,0);

	// 6. ���淴������ָ��
	strAsm.GetBufferSetLength(MAX_OPCODESTRING);
	MultiByteToWideChar(CP_ACP,0,objDiasm.CompleteInstr,-1,(LPWSTR)(LPCTSTR)strAsm,INSTRUCT_LENGTH);

	// �ж��Ƿ���callָ��.
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
// ������: DeAsm
///��  ��: ��ָ����ַ��opcode�����,���浽������vector��
// ��  ��: const LPVOID lpAddress
// ��  ��: vector<DEASM> & vecAsm
// ��  ��: DWORD dwLine
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
// ������: GetCodeType
///��  ��: ����ڴ��ַ�ϵ�ָ��(���ͱ�����CODETYPEö����)
// ��  ��: SIZE_T address
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
CODETYPE GetCodeType(SIZE_T address)
{
	// ��ȡԶ���ڴ�
	LPBYTE	lpCode = new BYTE[4];
	SIZE_T dwRead = 0;
	if(!ReadProcessMemory(g_hProcess,(LPVOID)address,lpCode,4,&dwRead))
	{	CString strStatusMsg;
		strStatusMsg.Format(L"��ȡ�ڴ����");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
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
	case 0xe8:// call offset+ָ���
		dwType = CODETYPE_CALLOFFSET;
		break;
	case 0xe9:// jmp offset+ָ���
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
// ������: GetCodeOperand
///��  ��: ��ȡָ����ַ�е�ָ��Ĳ�����
// ��  ��: SIZE_T address
// ��  ��: CODETYPE codeType
// ��  ��: PCODEFOMART pCodeOperand
//\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
SIZE_T GetCodeOperand(SIZE_T address,CODETYPE codeType,PCODEFOMART pCodeOperand)
{
	// ��ȡԶ���ڴ�
	LPBYTE	lpCode = new BYTE[MAX_OPCODESTRING];
	if(!ReadDebuggeeMemory(address,MAX_OPCODESTRING,lpCode))
	{
		CString strStatusMsg;
		strStatusMsg.Format(L"��ȡ�ڴ����");
		MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
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
				strStatusMsg.Format(L"��ȡ�ڴ����");
				MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
				return CODETYPE_ERROR;
			}	
			addr = (SIZE_T)*addressInMem;
			if(addressInMem!=NULL)
				delete[] addressInMem; 
			return addr;
			break;
		}
	case CODETYPE_JMPOFFSET: // jmp  : 0xe9 + offset+address+ָ���
	case CODETYPE_CALLOFFSET: // call : 0xE8 + offset+address+ָ���
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



