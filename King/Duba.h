/***************************************************************************************
* AUTHOR : MZ
* DATE   : 2016-1-15
* MODULE : Duba.H
*
* IOCTRL Sample Driver
*
* Description:
*		Demonstrates communications between USER and KERNEL.
*
****************************************************************************************
* Copyright (C) 2010 MZ.
****************************************************************************************/

#ifndef CXX_DUBA_H
#define CXX_DUBA_H


#include <ntifs.h>
#include <devioctl.h>






#include "GetVersion.h"
#include "GetModules.h"
#include "Other.h"
#include "Hook.h"
#include "HookKiFastCall.h"


typedef 
	NTSTATUS 
	(*pfnRtlGetVersion)(OUT PRTL_OSVERSIONINFOW lpVersionInformation);



extern
NTSTATUS
NTAPI
	ObReferenceObjectByName(
	__in PUNICODE_STRING ObjectName,
	__in ULONG Attributes,
	__in_opt PACCESS_STATE AccessState,
	__in_opt ACCESS_MASK DesiredAccess,
	__in POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__inout_opt PVOID ParseContext,
	__out PVOID *Object
	);





NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString);

VOID DriverUnload(IN PDRIVER_OBJECT pDriverObj);
#define BaseAddress1	((ULONG)KisDriver->DriverStart)

extern
	PDRIVER_OBJECT KisDriver;

typedef struct 
{
	ULONG FuncAddress;
	ULONG FuncName;
}StructFunc;

typedef struct KSERVICE_TABLE_DESCRIPTOR1 {
	PULONG_PTR Base;
	PULONG Count;
	ULONG Limit;
	PUCHAR Number;
} KSERVICE_TABLE_DESCRIPTOR1, *PKSERVICE_TABLE_DESCRIPTOR1;

#endif