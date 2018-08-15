#pragma once
#include <windows.h>
#include <Dbghelp.h>
#pragma comment(lib,"Dbghelp.lib")
#include <atlstr.h>

#include  <map>
//#include "AbsDbg.h"

using namespace std;

typedef	struct FUNCTIONINFO
{
	CString	strFunctionName;
	CString	strModuleName;
}FUNCTIONINFO;


	void	InitSymbol();
	//void	Init(HANDLE	hProcess,HANDLE	hThread,HANDLE	hFile);

	SIZE_T	FindApiAddress(const char* pszName);
	BOOL	GetFunctionName(SIZE_T nAddress,CString& strName);

