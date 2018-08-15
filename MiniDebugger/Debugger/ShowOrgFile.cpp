#include "StdAfx.h"
#include "ShowOrgFile.h"
#include "DebuggerMainDlg.h"
#include "DispatchException.h"
#include <DbgHelp.h>
#pragma comment(lib,"dbghelp.lib")
#include <string>
#include <Windows.h>
#include <fstream>
#include <iomanip>


extern CDebugger1Dlg *MainDlg;

void OnShowSourceLines() {

	CHECK_DEBUGGEE;

	int afterLines = 10;
	int beforeLines = 10;


	//��ȡEIP
	CONTEXT context = {0};
	GetDebuggeeContext(&context);

	//��ȡԴ�ļ��Լ�����Ϣ
	IMAGEHLP_LINE64 lineInfo = { 0 };
	lineInfo.SizeOfStruct = sizeof(lineInfo);
	DWORD displacement = 0;

	if (SymGetLineFromAddr64(
		GetDebuggeeHandle(),
		context.Eip,
		&displacement,
		&lineInfo) == FALSE) {

			DWORD errorCode = GetLastError();

			switch (errorCode) {

				// 126 ��ʾ��û��ͨ��SymLoadModule64����ģ����Ϣ
			case 126:
				{
					CString strStatusMsg;
					strStatusMsg.Format(L"Debug info in current module has not loaded.");
					MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
					return;
				}
			

				// 487 ��ʾģ��û�е��Է���
			case 487:
				{
					CString strStatusMsg;
					strStatusMsg.Format(L"No debug info in current module.");
					MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
					return;
				}
			default:
				{
					CString strStatusMsg;
					strStatusMsg.Format(L"SymGetLineFromAddr64 failed:");
					MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//��״̬������ʾ����
					return;
				}
			}
	}

	DisplaySourceLines(
		lineInfo.FileName, 
		lineInfo.LineNumber,
		(unsigned int)lineInfo.Address,
		afterLines, 
		beforeLines);
}



//��ʾԴ�ļ���ָ������
void DisplaySourceLines(PCHAR sourceFile, int lineNum, unsigned int address, int after, int before) {

	std::wcout << std::endl;

	std::wifstream inputStream(sourceFile);

	if (inputStream.fail() == true) {

		std::wcout << TEXT("Open source file failed.") << std::endl 
			<< TEXT("Path: ") << sourceFile << std::endl;
		return;
	}

	inputStream.imbue(std::locale("chs", std::locale::ctype));

	int curLineNumber = 1;

	//����ӵڼ��п�ʼ���
	int startLineNumber = lineNum - before;
	if (startLineNumber < 1) {
		startLineNumber = 1;
	}

	std::wstring line;

	//��������Ҫ��ʾ����
	while (curLineNumber < startLineNumber) {

		std::getline(inputStream, line);
		++curLineNumber;
	}

	//�����ʼ�е���ǰ��֮�����
	while (curLineNumber < lineNum) {

		std::getline(inputStream, line);
		DisplayLine(sourceFile, line, curLineNumber, FALSE);
		++curLineNumber;
	}

	//�����ǰ��
	getline(inputStream, line);
	DisplayLine(sourceFile, line, curLineNumber, TRUE);
	++curLineNumber;

	//�����ǰ�е������֮�����
	int lastLineNumber = lineNum + after;
	while (curLineNumber <= lastLineNumber) {

		if (!getline(inputStream, line)) {
			break;
		}

		DisplayLine(sourceFile, line, curLineNumber, FALSE);
		++curLineNumber;
	}

	inputStream.close();
	MainDlg->UpdateData(FALSE);
}



//��ʾԴ�ļ��е�һ�С�
void DisplayLine(PCHAR sourceFile, const std::wstring& line, int lineNumber, BOOL isCurLine) {

	if (isCurLine == TRUE) {
		MainDlg->m_Text+=L"=>";
	}
	else {
		MainDlg->m_Text+=L"  ";
	}

	LONG displacement;
	IMAGEHLP_LINE64 lineInfo = { 0 };
	lineInfo.SizeOfStruct = sizeof(lineInfo);

	if (SymGetLineFromName64(
		GetDebuggeeHandle(),
		NULL,
		sourceFile,
		lineNumber, 
		&displacement,
		&lineInfo) == FALSE) {


			//std::wcout << TEXT("SymGetLineFromName64 failed: ") << GetLastError() << std::endl;
			return;
	}

	CString LineNumber;
	LineNumber.Format(L"  %d  ",lineNumber);
	MainDlg->m_Text+=LineNumber;
//	std::wcout << std::setw(4) << std::setfill(TEXT(' ')) << lineNumber << TEXT("  ");

	if (displacement == 0) {
		CString Address1;
		Address1.Format(L"  %p  ",lineInfo.Address);
		MainDlg->m_Text+=Address1;
	}
	else {

		CString Address1;
		Address1.Format(L"            ");
		MainDlg->m_Text+=Address1;
	}
	CString A ;
	A.Format(L"%s",line);
	MainDlg->m_Text += A;
	MainDlg->m_Text +="\r\n";
	//std::wcout << TEXT("  ") << line << std::endl;
}