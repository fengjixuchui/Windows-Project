#include "Hook.h"

extern PVOID FirstModuleImageBase;
extern ULONG FirstModuleSize;
extern ULONG VersionNumber; 
extern ULONG _BuildNumber; 

extern ULONG IndexOfNtTerminateProcess;
extern WCHAR FirstModulePath[260];
extern PVOID BaseAddress;

extern PVOID AddressOfNtTerminateProcess; 
extern PVOID OrgObReferenceByHandle;   //dword_92808 = 0x805b1ab6 ;


extern ERESOURCE  Resource;//


ULONG dword_8E1E0 = 0xb1f211e0;

ULONG dword_4B810 = 0;
ULONG dword_8E7E8 = 0;


ULONG dword_4C164 = 0x00000001;
extern NPAGED_LOOKASIDE_LIST NoPagedLookasideList_stru_8DD00;



#define BUFFER_SIZE 0x4000
ULONG OutputBuffer[BUFFER_SIZE];



extern PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;
PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable1;
typedef int (__stdcall *PfnV17)(int, int, int, int, int, int); // [sp+20h] [bp-8h]@1
void _HookObferenceObjectByHandleInNtTerminateProcess()
{
	ULONG v0 = 0; // ebx@1
	char *ObReferenceObjectByHandleAddress; // ecx@3
	PVOID v2; // eax@4
	char *v3; // edx@5
	signed int v4; // ecx@6
	char *ObferenceObjectByHandleWithTagAddress; // ecx@7
	unsigned int v6; // esi@11
	signed int v7; // ecx@31
	signed int v8; // ecx@41
	char *v9; // esi@51
	int v10; // eax@54
	signed __int32 v11; // esi@60
	char *v12; // [sp+Ch] [bp-1Ch]@3
	signed int v13 = 0; // [sp+10h] [bp-18h]@1
	PMDL MemoryDescriptorList = 0; // [sp+14h] [bp-14h]@1
	PMDL v15 = 0; // [sp+18h] [bp-10h]@1
	char *v16 = NULL; // [sp+1Ch] [bp-Ch]@1
	int (__stdcall *v17)(int, int, int, int, int, int); // [sp+20h] [bp-8h]@1
	int v18 = 0; // [sp+24h] [bp-4h]@1
	UNICODE_STRING uniObferenceObjectByHandleWithTag ;
	UNICODE_STRING uniObReferenceObjectByHandle ;
	UNICODE_STRING uniSSDT;
	KIRQL Irql;
	ULONG mz=0;
	v17 = 0;

	RtlInitUnicodeString(&uniSSDT,L"KeServiceDescriptorTable");
	RtlInitUnicodeString(&uniObferenceObjectByHandleWithTag,L"ObReferenceObjectByHandleWithTag");
	RtlInitUnicodeString(&uniObReferenceObjectByHandle,L"ObReferenceObjectByHandle");
	KeServiceDescriptorTable1 = MmGetSystemRoutineAddress(&uniSSDT);  
	if ( !FirstModuleImageBase )
		goto LABEL_65;
	if ( !FirstModuleSize ) //001f8480
		goto LABEL_65;
	ObReferenceObjectByHandleAddress = (char*)MmGetSystemRoutineAddress(&uniObReferenceObjectByHandle);  //Function Address in First Module
	v12 = ObReferenceObjectByHandleAddress;
	if ( !ObReferenceObjectByHandleAddress )
		goto LABEL_65;
	v2 = FirstModuleImageBase;
	if ( ObReferenceObjectByHandleAddress < FirstModuleImageBase )
		goto LABEL_65;
	v3 = (char*)((ULONG)FirstModuleImageBase + FirstModuleSize);  //Second Module
	if ( ObReferenceObjectByHandleAddress > (char*)((ULONG)FirstModuleImageBase + FirstModuleSize) )
		goto LABEL_65;
	v4 = VersionNumber;
	if ( (VersionNumber - 5) <= 5 )
	{
		ObferenceObjectByHandleWithTagAddress = (char*)MmGetSystemRoutineAddress(&uniObferenceObjectByHandleWithTag);  //Func Address
		v16 = ObferenceObjectByHandleWithTagAddress;
		if ( !ObferenceObjectByHandleWithTagAddress )
			goto LABEL_65;
		v2 = FirstModuleImageBase;
		if ( ObferenceObjectByHandleWithTagAddress < FirstModuleImageBase )
			goto LABEL_65;
		v3 = (char*)((ULONG)FirstModuleImageBase + FirstModuleSize); //second module Address
		if ( ObferenceObjectByHandleWithTagAddress > v3)  
			goto LABEL_65;
		v4 = VersionNumber;
	}
	v6 = (int)AddressOfNtTerminateProcess;//AddressOfNtTerminateProcess;//805c9c2a   dword_8E25C   NtTerminateProcess
	if ( !v6 || v6 < (unsigned int)v2 || v6 > (unsigned int)v3 )
	{
		if ( GetStorageDeviceBusType(0x83002188, 0, 0, OutputBuffer, 0x4000u) < 0 )//获得存储设备总线类型
			memset(OutputBuffer, 0, 0x4000u);
		if ( !IndexOfNtTerminateProcess || (4 * IndexOfNtTerminateProcess) > 0x4000 ) //0x101
			goto LABEL_65;
		v6 = OutputBuffer[IndexOfNtTerminateProcess];
		if ( !v6 || v6 < (ULONG)FirstModuleImageBase || v6 > (ULONG)FirstModuleImageBase + FirstModuleSize )
		{
			if ( _ResumeSSDTFromFirstModule((unsigned int)KeServiceDescriptorTable) < 0 )  //
				goto LABEL_65;
			v6 = OutputBuffer[IndexOfNtTerminateProcess];
			if ( !MmIsAddressValid((PVOID)(OutputBuffer[IndexOfNtTerminateProcess])) 
				|| !v6 
				|| v6 < (ULONG)FirstModuleImageBase 
				|| v6 > (ULONG)FirstModuleImageBase + FirstModuleSize )
				goto LABEL_65;
		}
		v4 = VersionNumber;
	}
	if ( v4 >= 2 && v4 < 9 )
		goto LABEL_31;
	if ( v4 != 9 )
		goto LABEL_72;
	if ( _BuildNumber <= 0x2697 )
	{
LABEL_31:
		v7 = 90;
		do
		{
			//805c9e6f ff7508          push    dword ptr [ebp+8]
			//805c9e72 e83f7cfeff      call    nt!ObReferenceObjectByHandle (805b1ab6)
			//从NtTerminateProcess 往下搜索0x90字节，找到  FF7508  e8  的字节码
			if ( *(BYTE*)v6 == 0xFFu && *(BYTE*)(v6 + 1) == 0x75 && *(BYTE*)(v6 + 2) == 0x08 && *(BYTE*)(v6 + 3) == 0xE8u )
			{
				++v18;
				v0 = *(ULONG*)(v6 + 4);  //v0 = Address of ObReference
				MemoryDescriptorList = (PMDL)(v6 + 3);  //e8 Address 
			}
			++v6;
			--v7;
		}
		while ( v7 );
		goto LABEL_49;
	}
	if ( _BuildNumber != 9926 )
	{
LABEL_72:
		if ( v4 != 10 )
			goto LABEL_65;
	}
	v8 = 90;
	do
	{
		if ( *(BYTE*)v6 == -1 && *(BYTE*)(v6 + 1) == 53 && *(BYTE*)(v6 + 6) == 86 && *(BYTE*)(v6 + 7) == 81 && *(BYTE*)(v6 + 8) == -24 )
		{
			++v18;
			v0 = *(ULONG*)(v6 + 9);
			MemoryDescriptorList = (PMDL)(v6 + 8);
		}
		++v6;
		--v8;
	}
	while ( v8 );
LABEL_49:
	if ( !v18 )
		goto LABEL_65;
	if ( v18 > 1 )
		goto LABEL_65;
	v9 = (char*)&MemoryDescriptorList->Size + v0 + 1;  //NtTerminateProcess   fake函数
	if ( !MmIsAddressValid(v9) )
		goto LABEL_65;
	if ( v9 == v12 )  //相等  没hook
		goto LABEL_59;
	if ( v9 == v16 )
	{
LABEL_57:
		v17 = (PfnV17)Asm_1D302;  //Fake Func
		dword_8E7E8 = (ULONG)&MemoryDescriptorList->Size + v0 + 1; 
		goto LABEL_60;
	}
	v10 = _GetMappingOffsetOfObReferenceObjectByHandleFromFirstModuleBaseAddress((unsigned int)KeServiceDescriptorTable, 7);
	//v10 = 0xD9AB6;
	if ( !v10 )
		goto LABEL_65;
	if ( v16 - FirstModuleImageBase != v10 )//ObReferenceObjectByHandleWithTag - Base != ObReferenceObjectByHandle - Base
	{
		if ( v12 - FirstModuleImageBase != v10 ) 
			goto LABEL_65;
LABEL_59:
		OrgObReferenceByHandle = (PVOID)((char*)&MemoryDescriptorList->Size + v0 + 1);//ObReferenceObjectByHandle
		//没恢复就不能用
		//OrgObReferenceByHandle = v12;
		v17 = (PfnV17)Asm_1D355;   //MyFakeObReferenceObjectByHandle; //fake func
		goto LABEL_60;
	}
	if ( v16 )
		goto LABEL_57;
LABEL_60:
	v11 = (char*)v17 - (char*)MemoryDescriptorList - 5; //nDiff = (PUCHAR)pHook  - Address -5
	if ( VersionNumber < 9 )
	{
		v15 = (PMDL)CreateMdl(MemoryDescriptorList, 0x14u, (int)(&MemoryDescriptorList));//pMdpped
		if ( !v15 )//pMapped = NULL
			goto LABEL_65;
		v13 = 1;
	}
	else
	{
		v15 = MemoryDescriptorList;
	}
	//_disable();
	//__writecr0(__readcr0() & 0xFFFEFFFF);
	//_InterlockedExchange((&v15->Next + 1), v11);
	//__writecr0(__readcr0() | 0x10000);
	//_enable();
	Irql = SecCloseProtect();
	InterlockedExchange((volatile LONG*)((char*)&v15->Next + 1),v11);
	SecOpenProtect(Irql);



LABEL_65:
	if ( BaseAddress )
	{
		ZwUnmapViewOfSection((HANDLE)0xFFFFFFFF, BaseAddress);
		BaseAddress = 0;
	}
	if ( v13 )
	{
		if ( v15 )
		{
			MmUnlockPages(MemoryDescriptorList);
			IoFreeMdl(MemoryDescriptorList);
		}
	}
}
PVOID __stdcall CreateMdl(PVOID VirtualAddress, ULONG Length, int a3)
{
	PMDL v3; // eax@1
	PMDL v4; // esi@1
	PVOID result; // eax@3

	v3 = IoAllocateMdl(VirtualAddress, Length, 0, 1u, 0);
	v4 = v3;
	if ( v3 )
	{
		MmProbeAndLockPages(v3, 0, IoWriteAccess);
		if ( v4->MdlFlags & 5 )
			result = v4->MappedSystemVa;
		else
			result = MmMapLockedPagesSpecifyCache(v4, 0, MmCached, 0, 0, HighPagePriority);
		if ( result )
		{
			*(int*)a3 = (int)v4;
			return result;
		}
		MmUnlockPages(v4);
		IoFreeMdl(v4);
	}
	return 0;
}

int __stdcall Asm_1D302(int a1, int a2, int a3, int a4, int a5, PVOID *a6, int a7)
{
	int result; // eax@1
	PVOID BaseAddress; // edi@1
	
	BaseAddress = (PVOID)(BaseAddress1 + 0xD302);

	__asm
	{
		push a7
		push a6
		push a5
		push a4
		push a3
		push a2
		push a1
		call BaseAddress
		mov result,eax
	}
	return result;
}

unsigned int __stdcall Asm_1CA90(unsigned int a1)
{
	int result;
	PVOID BaseAddress;
	BaseAddress = (PVOID)(BaseAddress1 + 0xCA90);
	__asm
	{
		push a1
		call BaseAddress
		mov result,eax
	}
	return result;
}

int __stdcall _GetMappingOffsetOfObReferenceObjectByHandleFromFirstModuleBaseAddress(unsigned int a1, int a2)
{
	PVOID v2; // eax@2
	//unsigned int v3; // edi@4
	PIMAGE_NT_HEADERS v4; // ebx@7
	PIMAGE_NT_HEADERS v5; // eax@7
	char *v6; // ecx@11
	unsigned int v7; // edx@11
	unsigned int v8; // eax@13
	int result; // eax@14
	int v10; // esi@19
	int v11; // esi@37
	int v12; // esi@46
	int v13; // esi@72
	unsigned int v14; // esi@75
	ULONG v15; // [sp+20h] [bp-28h]@1
	ULONG v16; // [sp+24h] [bp-24h]@1
	unsigned int v17; // [sp+28h] [bp-20h]@1
	unsigned int l; // [sp+2Ch] [bp-1Ch]@20
	unsigned int k; // [sp+2Ch] [bp-1Ch]@29
	unsigned int m; // [sp+2Ch] [bp-1Ch]@38
	unsigned int j; // [sp+2Ch] [bp-1Ch]@50
	unsigned int i; // [sp+2Ch] [bp-1Ch]@61
	int SSDT =*(int*)a1 - (ULONG)FirstModuleImageBase;

	v17 = 0;
	v15 = 0;
	v16 = 0;
	if ( !a1 )
		return 0;
	v2 = FirstModuleImageBase;
	if ( a1 < (ULONG)FirstModuleImageBase || a1 > (ULONG)FirstModuleImageBase + FirstModuleSize )
		return 0;
	//v3 = a1 - (ULONG)FirstModuleImageBase;
	if ( !BaseAddress )
	{
		if ( _ResumeSSDTFromFirstModule(a1) < 0 )   //Map之后恢复系统中的SSDT在全局OutPutBuffer中
			return 0;
		v2 = FirstModuleImageBase;
	}
	v4 = RtlImageNtHeader(v2);
	v5 = RtlImageNtHeader(BaseAddress);
	if ( !v4
		|| !v5
		|| v4->OptionalHeader.CheckSum != v5->OptionalHeader.CheckSum
		|| v4->OptionalHeader.ImageBase != v5->OptionalHeader.ImageBase )
	{
		return 0;
	}
	v6 = (char*)BaseAddress + SSDT;  //Map之后的KeServiceDescriptorTable
	v7 = KeServiceDescriptorTable->Limit;//(&KeServiceDescriptorTable + 2);  Number of SSDT Func
	if ( v7 >= 0x1000 )
		v7 = 4096;
	//v8 = dword_8E1E0[4 * a2];
	v8 = 0x101; //Index of NtTerminateProcess
	if ( v7 <= v8 )
		return 0;
	if ( a2 == 7 )
	{
		v12 = *(ULONG*)((char*)v6+4 * v8); //SSDT里面函数地址  NtTerminateProcess
		ProbeForRead((PVOID)(*(ULONG*)((char*)v6+4 * v8)), 0x5Au, 1u);
		if ( VersionNumber < 2 || VersionNumber >= 9 ) //不走
		{
			if ( VersionNumber != 9 )
				goto LABEL_80;
			if ( _BuildNumber <= 0x2697 )
				goto LABEL_50;
			if ( VersionNumber != 9 || _BuildNumber != 9926 )
			{
LABEL_80:
				if ( VersionNumber != 10 )
					goto LABEL_70;
			}
			for ( i = 0; i < 0x5A; ++i )
			{
				if ( *(BYTE*)v12 == 0xFF && *(BYTE*)(v12 + 1) == 0x35 && *(BYTE*)(v12 + 6) == 0x56 && *(BYTE*)(v12 + 7) == 0x51 && *(BYTE*)(v12 + 8) == 0xE8 )
				{
					++v17;
					v15 = v12 + 8;
					v16 = *(int*)(v12 + 9);
				}
				++v12;
			}
			goto LABEL_70;  //去70
		}
LABEL_50:
		for ( j = 0; j < 0x5A; ++j ) //NtTerminateProcess
		{
			if(*(ULONG*)(v12+4)==0x22)
			{

			}
			if ( *(BYTE*)v12 == 0xFF && *(BYTE*)(v12 + 1) == 0x75 && *(BYTE*)(v12 + 2) == 0x08 && *(BYTE*)(v12 + 3) == 0xE8 )
			{
				++v17;
				v16 = *(ULONG*)(v12 + 4); //XX XX XX XX
				v15 = v12 + 3;  //e8 XX XX XX XX
				
			}
			++v12;
		}
		goto LABEL_70;
	}
	if ( a2 != 13 )
	{
		if ( a2 == 19 )
		{
			v10 = *&v6[4 * v8];
			ProbeForRead((PVOID)v6[4 * v8], 0x82u, 1u);
			if ( (VersionNumber - 2) > 6 )
			{
				if ( VersionNumber >= 9 )
				{
					for ( k = 0; k < 0x82; ++k )
					{
						if ( *(BYTE*)v10 == 0xFF && *(BYTE*)(v10 + 1) == 0x75 && *(BYTE*)(v10 + 2) == 0x08 && *(BYTE*)(v10 + 6) == 0xE8 )
						{
							++v17;
							v15 = v10 + 6;
							v16 = *(int*)(v10 + 7);
						}
						++v10;
					}
				}
			}
			else
			{
				for ( l = 0; l < 0x82; ++l )
				{
					if ( *(BYTE*)v10 == 0xFF && *(BYTE*)(v10 + 1) == 0x75 && *(BYTE*)(v10 + 2) == 0x0C && *(BYTE*)(v10 + 3) == 0xE8 )
					{
						++v17;
						v15 = v10 + 3;
						v16 = *(int*)(v10 + 4);
					}
					++v10;
				}
			}
			goto LABEL_70;
		}
		return 0;
	}
	v11 = *&v6[4 * v8];
	ProbeForRead((PVOID)v6[4 * v8], 0x5Au, 1u);
	if ( (VersionNumber - 2) <= 8 )
	{
		for ( m = 0; m < 0x5A; ++m )
		{
			if ( *(BYTE*)v11 == -1 && *(BYTE*)(v11 + 1) == 117 && *(BYTE*)(v11 + 2) == 8 && *(BYTE*)(v11 + 3) == -24 )
			{
				++v17;
				v15 = v11 + 3;
				v16 = *(int*)(v11 + 4);
			}
			++v11;
		}
	}
LABEL_70:
	if ( v17 && v17 <= 1 ) //有值得1
		v13 = v16; //偏移
	else
		v13 = 0;
	if ( v13 )
	{
		v14 = v13 + v15 + 5; //偏移 + 首地址 +5 = 目标地址 
		result = Asm_1CA90(v14);  
		if ( result )
			return result;
		v13 = v14 - (ULONG)BaseAddress;  //正确的目标函数的偏移  ObReferenceObjectByHandle   
	}
	return v13;
}



int __stdcall MyFakeObReferenceObjectByHandle(int a1, int a2, int a3, int a4, PVOID *a5, int a6)
{
	int result; // eax@1
	signed int v7; // edi@1
	pfnObReferenceObjectByHandle ObReferenceObjectByHandle1 = (pfnObReferenceObjectByHandle)OrgObReferenceByHandle;
	DbgPrint("HelloWorld");
	
	//不恢复就是fake的函数                                                  
	result = ObReferenceObjectByHandle1((HANDLE)a1, a2, (POBJECT_TYPE)a3, a4, a5, (POBJECT_HANDLE_INFORMATION)a6);   //恢复SSDT之后 调用的OrgObReferenceByHandle
	v7 = result;
	if ( result >= 0 )
	{
		if ( a5 && *a5 )
		{
		/*	if ( sub_1D15B(*(int*)a5) )
			{
				v7 = -1073741790;
				ObfDereferenceObject(*a5);   //EPROCESS 
				*a5 = 0;
			}*/
		}
		result = v7;
	}
	return result;
}
/*
int __stdcall sub_1D15B(PEPROCESS EProcess)  //*EPROCESS    PEPROCESS  EProcess = NULL;
{
	HANDLE v2; // ebx@3
	int v3; // eax@3
	void *v4; // eax@19
	__int16 v5; // [sp+10h] [bp-1Ch]@1
	int v6; // [sp+12h] [bp-1Ah]@1
	__int16 v7; // [sp+16h] [bp-16h]@1
	__int16 v8; // [sp+18h] [bp-14h]@1
	int v9; // [sp+1Ah] [bp-12h]@1
	__int16 v10; // [sp+1Eh] [bp-Eh]@1
	int Data; // [sp+20h] [bp-Ch]@1
	int v12; // [sp+24h] [bp-8h]@1
	int v13; // [sp+28h] [bp-4h]@1

	PsGetCurrentProcessId();
	v8 = 0;
	v9 = 0;
	v10 = 0;
	v5 = 0;
	v6 = 0;
	v7 = 0;
	v13 = 0;
	v12 = 0;
	Data = 1;
	if ( !dword_496B4 )//00000001
		return 0;
	v2 = PsGetCurrentProcessId();
	v3 = PsGetProcessId(EProcess);//*EPROCESS
	if ( v3 != v2 )
	{                  //ProcessId  struct  int
		if ( !sub_36677(v3, &v5, &v13) || v13 == 1 || !sub_3A10A(&v5) || !Event )
		{
LABEL_22:
			sub_34E43(&v8, 1);
			sub_34E43(&v5, 1);
			return v12;
		}
		if ( sub_36677(v2, &v8, &v13) && v13 != 1 )
		{
			wcslwr(*(&v9 + 2));
			if ( wcsstr(*(&v9 + 2), L"360")
				|| wcsstr(*(&v9 + 2), L"fastlife.exe")
				|| wcsstr(*(&v9 + 2), L"qqpc")
				|| wcsstr(*(&v9 + 2), L"baidu")
				|| wcsstr(*(&v9 + 2), L"qmgamespeedup") )
			{
				v12 = 1;
			}
			if ( !ExGetPreviousMode() && v2 == 4 )
				v12 = 1;
			v4 = sub_1CE63(*(&v9 + 2));
			if ( v4 )
			{
				sub_1CD90(L"\\Registry\\Machine\\SOFTWARE\\kingsoft\\AntiVirus", L"tproname", 1u, v4, 2 * wcslen(v4) + 2);
				sub_1CD90(L"\\Registry\\Machine\\SOFTWARE\\kingsoft\\AntiVirus", L"tflag", 4u, &Data, 4u);
			}
			KeSetEvent(Event, 0, 0);
			goto LABEL_22;
		}
	}
	return 0;
}
                          //Id     struct   int
void *__stdcall sub_36677(int a1, int a2, int a3)
{
	void *result; // eax@1
	void *v4; // esi@4
	int v5; // edi@5
	int v6; // [sp+0h] [bp-4h]@1

	v6 = 0;
	result = sub_3723B(a1, a2, &v6);
	if ( v6 )
	{
		if ( result != 1 )
		{
			result = sub_18D83(a1, 1024);
			v4 = result;
			if ( result )
			{
				v5 = sub_35FA8(result, a2, a3);
				ZwClose(v4);
				result = v5;
			}
		}
	}
	else
	{
		result = 0;
	}
	return result;
}

signed int __stdcall sub_3723B(int a1, int a2, ULONG *a3)
{
	PSINGLE_LIST_ENTRY v4; // eax@4

	*a3 = 1;
	if ( a1 != 4 && a1 != 8 )
		return 0;
	v4 = sub_34DDD();  //_SINGLE_LIST_ENTRY
	*(a2 + 4) = v4;
	if ( !v4 )
	{
		*a3 = 0;
		return 0;
	}
	*a2 = 14;
	*(a2 + 2) = 14;
	v4->Next = *L"System";
	v4[1].Next = *L"\u7900\u7300\u7400\u6500\u6d00";
	v4[2].Next = *L"ystem";
	LOWORD(v4[3].Next) = aSystem_0[6];
	return 1;
}

struct _SINGLE_LIST_ENTRY *sub_34DDD()
{
	struct _SINGLE_LIST_ENTRY *result; // eax@2
	PSINGLE_LIST_ENTRY v1; // eax@4
	struct _SINGLE_LIST_ENTRY *v2; // esi@4

	if ( dword_4C164 && KeGetCurrentIrql() <= 2u )//0x00000001
	{//_NPAGED_LOOKASIDE_LIST  //
		v1 = sub_1E0C4(&NoPagedLookasideList_stru_8DD00.L.ListHead);
		v2 = v1;//_SINGLE_LIST_ENTRY
		if ( v1 )
			memset(v1, 0, 0x240u);
		result = v2;
	}
	else
	{
		result = 0;
	}
	return result;
}


PSINGLE_LIST_ENTRY __stdcall sub_1E0C4(PSLIST_HEADER ListHead)
{
	PSINGLE_LIST_ENTRY result; // eax@1
	_SINGLE_LIST_ENTRY *v2; // ST08_4@2

	++*&ListHead[1].Depth;  
	result = InterlockedPopEntrySList(ListHead);  //从List头取出一个元素
	if ( !result ) //没有则返回NULL
	{
		v2 = ListHead[4].Next.Next;
		++ListHead[2].Next.Next;
		result = (ListHead[5].Next.Next)(*&ListHead[3].Depth, *&ListHead[4].Depth, v2);
	}
	return result;
}

signed int __stdcall sub_3A10A(signed int a1)
{
	signed int v1; // edi@1
	USHORT v3; // dx@7
	WCHAR *v4; // edx@11
	signed int v5; // esi@13
	UNICODE_STRING String1; // [sp+4h] [bp-10h]@11
	UNICODE_STRING String2; // [sp+Ch] [bp-8h]@10

	v1 = a1;
	if ( !a1 )
		return 0;
	_AcquireResourceSharedLiteByEresource(&Resource);
	if ( !Buffer )
	{
		_ReleaseResourceLiteByEresource(&Resource);
		return 0;
	}
	if ( *(Buffer + (dword_4C178 >> 1) - 1) == 92 )
		a1 = 2;
	v3 = *v1;
	if ( *v1 < dword_4C178 - a1 )
	{
		_ReleaseResourceLiteByEresource(&Resource);
		return 0;
	}
	String2.Buffer = Buffer;
	if ( v3 <= dword_4C178 )
	{
		*&String1.Length = *v1;
		String1.Buffer = *(v1 + 4);
		String2.Length = v3;
		String2.MaximumLength = v3;
	}
	else
	{
		String1.MaximumLength = v3;
		v4 = *(v1 + 4);
		String1.Length = dword_4C178;
		String1.Buffer = v4;
		*&String2.Length = dword_4C178;
	}
	v5 = 1;
	if ( RtlCompareUnicodeString(&String1, &String2, 1u) )
		v5 = 0;
	_ReleaseResourceLiteByEresource(&Resource);
	return v5;
}






*/

int __stdcall Asm_1D355(int a1, int a2, int a3, int a4, PVOID *a5, int a6)
{
	int result; // eax@1
	PVOID BaseAddress;
	DbgPrint("HelloWorld");

	BaseAddress = (PVOID)(BaseAddress1 + 0xD355);
	__asm
	{
		push a6
		push a5
		push a4
		push a3
		push a2
		push a1
		call BaseAddress
		mov result , eax
	}
	return result;
}



signed int __stdcall _ResumeSSDTFromFirstModule(unsigned int a1)
{
	int v1; // ebx@4
	PIMAGE_NT_HEADERS v2; // edi@5
	PIMAGE_NT_HEADERS v3; // eax@5
	char *v4; // esi@9
	unsigned int v5; // ecx@9
	signed int result; // eax@14
	unsigned int i; // [sp+18h] [bp-1Ch]@11
	PIMAGE_DOS_HEADER DosHead;
	ULONG FuncAddress;
	int SSDT =*(int*)a1 - (ULONG)FirstModuleImageBase;
	if ( a1
		&& a1 >= (ULONG)FirstModuleImageBase
		&& a1 <= (ULONG)FirstModuleImageBase + FirstModuleSize
		&& (v1 = a1 - (ULONG)FirstModuleImageBase, (BaseAddress = _CreateFileMap(FirstModulePath, (int)(&dword_4B810), 0)) != 0)
		&& (v2 = RtlImageNtHeader(FirstModuleImageBase), v3 = RtlImageNtHeader(BaseAddress), v2)
		&& v3
		&& v2->OptionalHeader.CheckSum == v3->OptionalHeader.CheckSum
		&& v2->OptionalHeader.ImageBase == v3->OptionalHeader.ImageBase )
	{

		v4 = (char*)((ULONG)BaseAddress + SSDT);
		DosHead = (PIMAGE_DOS_HEADER)v4;
		if(DosHead->e_magic!=IMAGE_DOS_SIGNATURE)
			DbgPrint("aa");
		v5 = 4096;
		if ( (ULONG)(KeServiceDescriptorTable->Limit) < 0x1000 )
			v5 = (int)(KeServiceDescriptorTable ->Limit);
		
		for ( i = 0; i < v5; ++i )
		{
			FuncAddress = *(ULONG*)((char*)v4+4 * i);//将SSDT中函数拷贝到OutputBuffer中
			OutputBuffer[i] = ((ULONG)FirstModuleImageBase + FuncAddress - v3->OptionalHeader.ImageBase);
		}
			result = 0;
	}
	else
	{
		result = -1073741823;
	}
	return result;
}


NTSTATUS __stdcall GetStorageDeviceBusType(ULONG IoControlCode, PVOID InputBuffer, ULONG InputBufferLength, PVOID OutputBuffer, ULONG OutputBufferLength)
{
	NTSTATUS result; // eax@1
	struct _IRP *NewIrp; // eax@2
	signed int v7; // esi@3
	NTSTATUS Status; // eax@6
	struct _KEVENT Event; // [sp+4h] [bp-28h]@2
	struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+14h] [bp-18h]@2
	UNICODE_STRING DestinationString; // [sp+1Ch] [bp-10h]@1
	PFILE_OBJECT FileObject; // [sp+24h] [bp-8h]@1
	PDEVICE_OBJECT DeviceObject; // [sp+28h] [bp-4h]@1

	RtlInitUnicodeString(&DestinationString, L"\\Device\\KAVBootC");
	result = IoGetDeviceObjectPointer(&DestinationString, 0x80u, &FileObject, &DeviceObject);  //通过设备名获得设备对象指针
	if ( result >= 0 )
	{
		KeInitializeEvent(&Event, NotificationEvent, FALSE);
		NewIrp = IoBuildDeviceIoControlRequest(//获取到存储设备所在的总线类型
			IoControlCode,
			DeviceObject,
			InputBuffer,    //0
			InputBufferLength,  //0
			OutputBuffer,
			OutputBufferLength, //0x4000
			1u,        //TRUE
			&Event,
			&IoStatusBlock);
		if ( NewIrp )
		{
			Status = IofCallDriver(DeviceObject, NewIrp);
			if ( Status == STATUS_PENDING )//259
			{
				KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
				Status = IoStatusBlock.Status;
			}
			v7 = Status;
		}
		else
		{
			v7 = STATUS_INSUFFICIENT_RESOURCES;
		}
		ObfDereferenceObject(FileObject);
		result = v7;
	}
	return result;
}



KIRQL 
	SecCloseProtect()
{
	KIRQL Irql;

	Irql = KeRaiseIrqlToDpcLevel();

	_asm
	{
		CLI;					//	禁止中断
		MOV EAX,CR0;  
		AND EAX,NOT 10000H;		// CR0 16 BIT = 0
		MOV CR0, EAX;        
	}	

	return Irql;
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID 
	SecOpenProtect(KIRQL Irql)
{
	_asm 
	{
		MOV EAX,CR0;     
		OR EAX,10000H;            
		MOV CR0,EAX;           
		STI;					//	开启中断
	}

	KeLowerIrql(Irql);
}