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


//��ʮ�����Ƶĸ�ʽ��ʾָ����ַ�����ڴ档ÿһ�й̶���ʾ16���ֽڣ�����ĵ�ַҪ���뵽16�ı�����
//��ʾ�������������һ��wostringstream�����У�
//����������������е��ַ�����
void DumpHex(unsigned int address, unsigned int length) {
	MainDlg->m_Dump = L"";
	//�洢ÿһ���ֽڶ�Ӧ��ASCII�ַ���
	char content[17] = { 0 };

	//����ʼ����ĵ�ַ���뵽16�ı�����
	unsigned int blankLen = address % 16;
	unsigned int startAddress = address - blankLen;

	unsigned int lineLen = 0;
	unsigned int contentLen = 0;
	BYTE byte;
	//����ֽڶ�ȡ�����Խ��̵��ڴ棬�������
	for (BYTE memLen = 0; memLen < length; ++memLen) {

		unsigned int curAddress = startAddress + memLen;

		//�����ÿ�еĵ�һ���ֽڣ�����������ַ��
		if (lineLen % 16 == 0) {
			CString CstringAddress ;
			CstringAddress.Format(L"%p  ",curAddress);
			MainDlg->m_Dump += CstringAddress;
			//startAddress += 16;

			lineLen = 0;
		}

		//��ȡ�ڴ棬����ɹ��Ļ�����ԭ�����������ȡ���Ӧ��ASCII�ַ���
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
		//�����ȡʧ�ܣ����� ?? ���档
		else {

			MainDlg->m_Dump +=L"??";
			content[contentLen] = TEXT('.');
		}

		//���һ������16���ֽڣ���������з������ڴ�0��ʼ����������������15�Ƚϡ�
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



//���ֽ�ת�����ַ���������ʾ���ַ��ֱ��� . �� �� ��ʾ��
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
