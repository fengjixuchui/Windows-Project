#include "StdAfx.h"
#include "ShowDumpText.h"
#include "DebuggerMainDlg.h"
#include "DispatchException.h"
#include <iostream>
using namespace std;
extern CDebugger1Dlg *MainDlg;

void OnDump(ULONG Address) {

	CHECK_DEBUGGEE;

	unsigned int address = 0;
	unsigned int length = 128;

	if(Address==0)
	{
		CONTEXT context = {0};
		GetDebuggeeContext(&context);
		address = context.Eip;
		DumpHex(address, length);	
	}
	else
	{
		DumpHex(Address, length);	
	}	
}


//以十六进制的格式显示指定地址出的内存。每一行固定显示16个字节，输出的地址要对齐到16的倍数。
//显示的内容先输出到一个wostringstream对象中，
//最后再输出这个对象中的字符串。
void DumpHex(unsigned int address, unsigned int length) {
	MainDlg->m_Dump = L"";
	//存储每一行字节对应的ASCII字符。
	char content[17] = { 0 };

	//将起始输出的地址对齐到16的倍数。
	unsigned int blankLen = address % 16;
	unsigned int startAddress = address - blankLen;

	unsigned int lineLen = 0;
	unsigned int contentLen = 0;
	BYTE byte;
	//逐个字节读取被调试进程的内存，并输出。
	for (BYTE memLen = 0; memLen < length; ++memLen) {

		unsigned int curAddress = startAddress + memLen;

		//如果是每行的第一个字节，则先输出其地址。
		if (lineLen % 16 == 0) {
			CString CstringAddress ;
			CstringAddress.Format(L"%p  ",curAddress);
			MainDlg->m_Dump += CstringAddress;
			//startAddress += 16;

			lineLen = 0;
		}

		//读取内存，如果成功的话，则原样输出，并获取其对应的ASCII字符。
		if (ReadDebuggeeMemory(curAddress, 1, &byte) == TRUE) {
			BYTE Mz = byte;
			CString ByteString;
			if(Mz>=0x10)
			{
				ByteString.Format(L"%-2x ",Mz);
				MainDlg->m_Dump += ByteString;
			}
			else
			{
				DealSingleNum(Mz);
			}
			
			content[contentLen] = ByteToChar(byte);
		}
		//如果读取失败，则以 ?? 代替。
		else {

			MainDlg->m_Dump +=L"??";
			content[contentLen] = TEXT('.');
		}

		//如果一行满了16个字节，则输出换行符。由于从0开始计数，所以这里与15比较。
		if (contentLen == 15) {
			MainDlg->m_Dump += L"  ";
			content[contentLen] = 0;
			MainDlg->m_Dump += content;
			MainDlg->m_Dump += L"\r\n";
		}

		++contentLen;
		contentLen %= 16;
		++lineLen;

	}


	MainDlg->UpdateData(FALSE);
}



//将字节转换成字符。不能显示的字符分别以 . 和 ？ 表示。
char ByteToChar(BYTE byte) {

	if (byte >= 0x00 && byte <= 0x1F) {
		return '.';
	}

	if (byte >= 0x81 && byte <= 0xFF) {
		return '?';
	}

	return (char)byte;
}

VOID  DealSingleNum(BYTE Mz)
{
	if(Mz==1)
	{
		MainDlg->m_Dump += L"01 ";
	}
	if(Mz==2)
	{
		MainDlg->m_Dump += L"02 ";
	}
	if(Mz==3)
	{
		MainDlg->m_Dump += L"03 ";
	}
	if(Mz==4)
	{
		MainDlg->m_Dump += L"04 ";
	}
	if(Mz==5)
	{
		MainDlg->m_Dump += L"05 ";
	}
	if(Mz==6)
	{
		MainDlg->m_Dump += L"06 ";
	}
	if(Mz==7)
	{
		MainDlg->m_Dump += L"07 ";
	}
	if(Mz==8)
	{
		MainDlg->m_Dump += L"08 ";
	}
	if(Mz==9)
	{
		MainDlg->m_Dump += L"09 ";
	}
	if(Mz==0x0A)
	{
		MainDlg->m_Dump += L"0A ";
	}
	if(Mz==0x0B)
	{
		MainDlg->m_Dump += L"0B ";
	}
	if(Mz==0x0C)
	{
		MainDlg->m_Dump += L"0C ";
	}
	if(Mz==0x0D)
	{
		MainDlg->m_Dump += L"0D ";
	}
	if(Mz==0x0E)
	{
		MainDlg->m_Dump += L"0E ";
	}
	if(Mz==0)
	{
		MainDlg->m_Dump += L"00 ";
	}
}
