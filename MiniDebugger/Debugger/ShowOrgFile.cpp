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


	//获取EIP
	CONTEXT context = {0};
	GetDebuggeeContext(&context);

	//获取源文件以及行信息
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

				// 126 表示还没有通过SymLoadModule64加载模块信息
			case 126:
				{
					CString strStatusMsg;
					strStatusMsg.Format(L"Debug info in current module has not loaded.");
					MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
					return;
				}
			

				// 487 表示模块没有调试符号
			case 487:
				{
					CString strStatusMsg;
					strStatusMsg.Format(L"No debug info in current module.");
					MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
					return;
				}
			default:
				{
					CString strStatusMsg;
					strStatusMsg.Format(L"SymGetLineFromAddr64 failed:");
					MainDlg->m_wndStatusBar.SetPaneText(0,strStatusMsg);//在状态条上显示文字
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



//显示源文件中指定的行
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

	//计算从第几行开始输出
	int startLineNumber = lineNum - before;
	if (startLineNumber < 1) {
		startLineNumber = 1;
	}

	std::wstring line;

	//跳过不需要显示的行
	while (curLineNumber < startLineNumber) {

		std::getline(inputStream, line);
		++curLineNumber;
	}

	//输出开始行到当前行之间的行
	while (curLineNumber < lineNum) {

		std::getline(inputStream, line);
		DisplayLine(sourceFile, line, curLineNumber, FALSE);
		++curLineNumber;
	}

	//输出当前行
	getline(inputStream, line);
	DisplayLine(sourceFile, line, curLineNumber, TRUE);
	++curLineNumber;

	//输出当前行到最后行之间的行
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



//显示源文件中的一行。
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