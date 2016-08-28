#pragma once
#include "Duba.h"
#include "GetModules.h"
#include <ntimage.h>


BOOLEAN  _GetSystemVersion();
NTSTATUS _GetServicePackNumber();
signed int sub_423FC();
int  Asm_4211A(PVOID DataEntry,ULONG DataLength);
unsigned int Asm_42363();
//int __stdcall sub_4211A(PIMAGE_RESOURCE_DATA_ENTRY DataEntry, ULONG DataLength);

//signed int sub_42363();
//char *__stdcall _GetFuncAddressWithFuncNameFromExport(PVOID ImageBase, int a2);
//int __stdcall sub_42308(int a1);

NTSYSAPI
	PIMAGE_NT_HEADERS
	NTAPI
	RtlImageNtHeader(PVOID Base);
