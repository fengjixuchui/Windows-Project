#include "stdafx.h"
#include "Symbol.h"
#include <atlstr.h>
#include "DispatchException.h"

extern HANDLE g_hProcess;




typedef	map<SIZE_T,FUNCTIONINFO>::iterator functionSymbolItoa;



map<SIZE_T,FUNCTIONINFO>	m_mapFunctionSymbol; // ���뺯����Ϣ:������,������ַ,����ģ����
map<SIZE_T,FUNCTIONINFO>	m_mapAPIInfo; // API��Ϣ:���õ�ַ�͵��õ�API����




BOOL	GetImportFuntion(LPBYTE lpFile,map<SIZE_T,FUNCTIONINFO>& mapFunctionSymbol)
{
	return TRUE;
}

void InitSymbol()
{
	DWORD Options = SymGetOptions( );
	Options |= SYMOPT_DEBUG;
	::SymSetOptions(Options);

	::SymInitialize(
		g_hProcess,  
		0,
		TRUE
		);


	return;
}


SIZE_T FindApiAddress(const char* pszName)
{
	DWORD64  dwDisplacement = 0;
	char buffer[sizeof(SYMBOL_INFO)+MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;

	if(!SymFromName(g_hProcess,pszName,pSymbol))
	{
		return 0;
	}
	return (SIZE_T)pSymbol->Address;
}

BOOL GetFunctionName(SIZE_T nAddress,CString& strName)
{
	DWORD64  dwDisplacement = 0;
	char buffer[sizeof(SYMBOL_INFO)+MAX_SYM_NAME * sizeof(TCHAR)];
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
	pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
	pSymbol->MaxNameLen = MAX_SYM_NAME;

	if(!SymFromAddr(g_hProcess,nAddress,&dwDisplacement,pSymbol))
	{
		return FALSE;
	}
	strName.GetBufferSetLength(pSymbol->NameLen + 1);
	MultiByteToWideChar(CP_ACP,
		0,
		pSymbol->Name,
		pSymbol->NameLen + 1,
		(LPWSTR)(LPCTSTR)strName,
		pSymbol->NameLen + 1
		);
	return TRUE;
}
