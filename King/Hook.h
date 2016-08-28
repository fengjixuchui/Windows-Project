#pragma once

#include "Duba.h"
#include "GetModules.h"
#include "Other.h"



NTSTATUS __stdcall GetStorageDeviceBusType(ULONG IoControlCode, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength);
typedef struct _KSERVICE_TABLE_DESCRIPTOR {
	PULONG_PTR Base;
	PULONG Count;
	ULONG Limit;
	PUCHAR Number;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;
void _HookObferenceObjectByHandleInNtTerminateProcess();  //1D85A
signed int __stdcall _ResumeSSDTFromFirstModule(unsigned int a1);  //1CBB7
int __stdcall Asm_1D302(int a1, int a2, int a3, int a4, int a5, PVOID *a6, int a7);
unsigned int __stdcall Asm_1CA90(unsigned int a1);
int __stdcall _GetMappingOffsetOfObReferenceObjectByHandleFromFirstModuleBaseAddress(unsigned int a1, int a2);//1D553



int __stdcall Asm_1D355(int a1, int a2, int a3, int a4, PVOID *a5, int a6);
int __stdcall MyFakeObReferenceObjectByHandle(int a1, int a2, int a3, int a4, PVOID *a5, int a6);
int __stdcall sub_1D15B(int a1); //Deal
   void *__stdcall sub_36677(int a1, int a2, int a3);//yi
     signed int __stdcall sub_3723B(int a1, int a2, ULONG *a3);
	   struct _SINGLE_LIST_ENTRY *sub_34DDD();
   signed int __stdcall sub_3A10A(signed int a1);





PVOID __stdcall CreateMdl(PVOID VirtualAddress, ULONG Length, int a3);
typedef unsigned char  BYTE, *PBYTE;

KIRQL 
	SecCloseProtect();

VOID 
	SecOpenProtect(KIRQL Irql);

typedef
NTKERNELAPI                                                     // ntddk wdm nthal ntifs
	NTSTATUS                                                        // ntddk wdm nthal ntifs
	(*pfnObReferenceObjectByHandle)(                                      // ntddk wdm nthal ntifs
	IN HANDLE Handle,                                           // ntddk wdm nthal ntifs
	IN ACCESS_MASK DesiredAccess,                               // ntddk wdm nthal ntifs
	IN POBJECT_TYPE ObjectType OPTIONAL,                        // ntddk wdm nthal ntifs
	IN KPROCESSOR_MODE AccessMode,                              // ntddk wdm nthal ntifs
	OUT PVOID *Object,                                          // ntddk wdm nthal ntifs
	OUT POBJECT_HANDLE_INFORMATION HandleInformation OPTIONAL   // ntddk wdm nthal ntifs
	);      