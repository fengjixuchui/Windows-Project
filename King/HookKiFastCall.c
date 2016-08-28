#include "HookKiFastCall.h"
#include "Duba.h"

extern ULONG dword_92CE0[21];
extern ULONG OffsetOfImageName_8D9E0[161];  //OffsetOfImageName_8D9E0[0] 174
extern ERESOURCE Resource_8DC80;    
extern ERESOURCE Resource_8DCC0;
extern ERESOURCE Resource_92C60;
extern ULONG VersionNumber;           //版本
extern StructFunc NtdllFunc_92820[56];           //函数名称
extern PVOID g_NtdllBaseAddress;

extern ULONG NtQueryInformationThread_Indexunk_4B7E4;
extern ULONG NtQueryInformationObject_Indexunk_4B7E8;
extern ULONG NtReadVirtualMemory_Indexunk_4B7EC;
extern ULONG NtWriteVirtualMemory_Indexunk_4B7F0;
extern ULONG NtProtectVirtualMemory_Indexunk_4B7F4;
extern ULONG NtReplyPort_Indexunk_4B7F8;
extern ULONG ZwQueryVirtualMemory_Indexunk_4B7FC;

extern PVOID   IoCreateFileSpecifyDeviceObjectHint_4B798; 
extern PVOID	IoCreateFileExd_4B79C; 
extern PVOID	FsRtlAllocateExtraCreateParameterList_4B7A0; 
extern PVOID   FsRtlFreeExtraCreateParameterList_4B7A4; 
extern PVOID	FsRtlAllocateExtraCreateParameter_4B7B0; 
extern PVOID	FsRtlInsertExtraCreateParameter_4B7A8; 
extern PVOID	FsRtlRemoveExtraCreateParameter_4B7AC; 
extern PVOID	FsRtlFreeExtraCreateParameter_4B7B4; 
extern PVOID	FsRtlGetEcpListFromIrp_4B7B8; 
extern PVOID	FsRtlFindExtraCreateParameter_4B7BC;
extern PVOID	PsGetProcessExitProcessCalled_4B7C0; 

extern ULONG dword_4C16C;
extern ULONG dword_8DAD4;//000004dc

extern UNICODE_STRING String;
extern ULONG FirstModuleSize;
extern PVOID FirstModuleImageBase;
extern PKSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;

extern ULONG KeServiceDescriptorTableBase_4C188;
extern ULONG KeServiceDescriptorTable_4C18C;
extern ULONG KeServiceDescriptorTable_4C190;
extern ULONG KeServiceDescriptorTable_4C194;

extern ULONG dword_641F8;
extern int (__stdcall *dword_4C1AC)(int);
extern int (__stdcall *FakeKiFastCallEntryRetAddr)(int,int);


extern  PVOID KiFastCallEntryRetAddress;//PVOID dword_4C1B0;

extern ULONG dword_8DB9C;

extern PULONG dword_79780;
extern PULONG dword_79788;
extern PULONG dword_7978C;
extern PULONG dword_79790;

extern WCHAR FirstModulePath[260];

extern ULONG dword_4C1BC;
extern ULONG dword_4C1C8;


//ULONG FakeKiFastCallEntryRetAddr;//  e9  +8    8053e62e 0f83a8010000    jae     nt!KiSystemCallExit2+0x9f (8053e7dc)
extern ULONG dword_4C1B8;
extern ULONG KiFastCallEntryEbxAddress; //给ebx赋值的地址
extern ULONG dword_4C1CC;
extern ULONG dword_4C1C0;
extern PVOID FirstModuleBase_4C1A8;
extern BYTE byte_641D8;



int sub_1742B(int a1)
{
	dword_79780 = (PULONG)(BaseAddress1 + 0x69780);
	dword_79788 = (PULONG)(BaseAddress1 + 0x69788);
	dword_7978C = (PULONG)(BaseAddress1 + 0x6978C);
	dword_79790 = (PULONG)(BaseAddress1 + 0x69790);
	_InitializePagedLookasideList(); //sub_1E0EB();
	sub_1887F();//reset globle var dword_92CE0
	_InitializeNPagedLookasideList();
	_InitializResourceLite409BA();//
	_GetOffsetOfImageName();  //Get OffsetOfImageName
	_SetFunction(a1);
	//sub_41EA6();
	sub_3540A();
	sub_3781F(); //dword_8DAD4
	sub_3792C(); //dword_8DB9C
	//sub_37B06(&Resource_92C60);
	//sub_18BE6();//??未实现
	//sub_19BE8();
	//sub_19895();
	sub_3C54F();  //hookKiFastCallEntry
	//sub_2BE97();
	//sub_34B9B();
	//sub_2175B();
	//sub_1BF7F(1);
	//sub_22FD9();
	//sub_4010B();
	//sub_2D820();
	//sub_41FE5();
	//sub_24E26();
	//return sub_1E225();
}
void *sub_1887F()
{
	void *result; // eax@1

	result = memset(dword_92CE0, 0, 0x54u);//84
	dword_92CE0[0] = 1;
	return result;
}
int _GetOffsetOfImageName()
{
	int v0; // esi@1
	PEPROCESS v1; // edi@1
	int result; // eax@2

	v0 = 0;
	memset(OffsetOfImageName_8D9E0, 0, 0x284u);
	v1 = IoGetCurrentProcess();
	OffsetOfImageName_8D9E0[0] = 0;
	ExInitializeResourceLite(&Resource_8DC80);
	while ( 1 )
	{
		result = _strnicmp("System", (char*)v1 + v0, 6u);
		if ( !result )
			break;
		if ( ++v0 >= 12288 )
			return result;
	}
	OffsetOfImageName_8D9E0[0] = v0;
	return result;
}
PVOID _SetFunction(int a1)
{
	_GetFuncAndAddress(a1);
	return _SetOthersAndGetProcessExitProcessCallAddress();  //PsGetProcessExitProcessCalled
}
PVOID _SetOthersAndGetProcessExitProcessCallAddress()
{
	int v0; // edx@1
	PVOID result; // eax@15
	UNICODE_STRING DestinationString; // [sp+0h] [bp-8h]@15

	v0 = 0;
	if ( !NtQueryInformationThread_Indexunk_4B7E4 )
		NtQueryInformationThread_Indexunk_4B7E4 = _GetIndexByFuncAddress(NtdllFunc_92820[15].FuncAddress); 
	if ( NtQueryInformationObject_Indexunk_4B7E8 == v0 )
		NtQueryInformationObject_Indexunk_4B7E8 = _GetIndexByFuncAddress(NtdllFunc_92820[16].FuncAddress);
	if ( NtReadVirtualMemory_Indexunk_4B7EC == v0 )
		NtReadVirtualMemory_Indexunk_4B7EC = _GetIndexByFuncAddress(NtdllFunc_92820[17].FuncAddress);
	if ( NtWriteVirtualMemory_Indexunk_4B7F0 == v0 )
		NtWriteVirtualMemory_Indexunk_4B7F0 = _GetIndexByFuncAddress(NtdllFunc_92820[18].FuncAddress);
	if (  NtProtectVirtualMemory_Indexunk_4B7F4 == v0 )
		NtProtectVirtualMemory_Indexunk_4B7F4 = _GetIndexByFuncAddress(NtdllFunc_92820[4].FuncAddress);
	if ( NtReplyPort_Indexunk_4B7F8 == v0 )
		NtReplyPort_Indexunk_4B7F8 = _GetIndexByFuncAddress(NtdllFunc_92820[44].FuncAddress);
	if ( ZwQueryVirtualMemory_Indexunk_4B7FC == v0 )
		ZwQueryVirtualMemory_Indexunk_4B7FC = _GetIndexByFuncAddress(NtdllFunc_92820[54].FuncAddress);
	RtlInitUnicodeString(&DestinationString, L"IoCreateFileSpecifyDeviceObjectHint");
	IoCreateFileSpecifyDeviceObjectHint_4B798 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"IoCreateFileEx");
	IoCreateFileExd_4B79C = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlAllocateExtraCreateParameterList");
	FsRtlAllocateExtraCreateParameterList_4B7A0 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlFreeExtraCreateParameterList");
	FsRtlFreeExtraCreateParameterList_4B7A4 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlAllocateExtraCreateParameter");
	FsRtlAllocateExtraCreateParameter_4B7B0 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlInsertExtraCreateParameter");
	FsRtlInsertExtraCreateParameter_4B7A8 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlRemoveExtraCreateParameter");
	FsRtlRemoveExtraCreateParameter_4B7AC = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlFreeExtraCreateParameter");
	FsRtlFreeExtraCreateParameter_4B7B4 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlGetEcpListFromIrp");
	FsRtlGetEcpListFromIrp_4B7B8 = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"FsRtlFindExtraCreateParameter");
	FsRtlFindExtraCreateParameter_4B7BC = MmGetSystemRoutineAddress(&DestinationString);
	RtlInitUnicodeString(&DestinationString, L"PsGetProcessExitProcessCalled");
	result = MmGetSystemRoutineAddress(&DestinationString);
	PsGetProcessExitProcessCalled_4B7C0 = result;
	return result;
}
int __stdcall _GetIndexByFuncAddress(int a1)
{
	int result; // eax@2

	if ( a1 && *(int*)a1 == -72 )//FFFFFFB8
		result = (*(int*)(a1 + 1) & 0xFFFFC000) == 0 ? *(int*)(a1 + 1) : 0;
	else
		result = 0;
	return result;
}
int _GetFuncAndAddress(int a1)//sub_1C4E7()
{
	int v0; // esi@1
	int *v1; // eax@25

	v0 = 0;
	//NtdllFunc_92820   20  
	memset(NtdllFunc_92820, 0, 0x1C0u);//448
	NtdllFunc_92820[0].FuncName = (ULONG)("NtCreateThread");//dword_92824
	NtdllFunc_92820[2].FuncName = (ULONG)"NtSuspendThread";//dword_92834
	NtdllFunc_92820[3].FuncName = (ULONG)"NtSuspendProcess";//dword_9283C
	NtdllFunc_92820[4].FuncName = (ULONG)"NtProtectVirtualMemory";//dword_92844
	NtdllFunc_92820[5].FuncName = (ULONG)"NtShutdownSystem";//dword_9284C
	NtdllFunc_92820[6].FuncName = (ULONG)"NtTerminateThread";//dword_92854
	NtdllFunc_92820[7].FuncName = (ULONG)"NtSetContextThread";//dword_9285C
	NtdllFunc_92820[8].FuncName = (ULONG)"NtTerminateJobObject";//dword_92864
	NtdllFunc_92820[9].FuncName = (ULONG)"NtSystemDebugControl";//dword_9286C
	NtdllFunc_92820[11].FuncName = (ULONG)"NtDebugActiveProcess";//dword_9287C
	NtdllFunc_92820[12].FuncName = (ULONG)"NtSetSystemPowerState";//dword_92884
	NtdllFunc_92820[13].FuncName = (ULONG)"NtInitiatePowerAction";//dword_9288C
	NtdllFunc_92820[14].FuncName = (ULONG)"NtQueueApcThread";//dword_92894
	NtdllFunc_92820[15].FuncName = (ULONG)"NtQueryInformationThread";//dword_9289C
	NtdllFunc_92820[16].FuncName = (ULONG)"NtQueryInformationJobObject";//dword_928A4
	NtdllFunc_92820[17].FuncName = (ULONG)"NtReadVirtualMemory";//dword_928AC
	NtdllFunc_92820[18].FuncName = (ULONG)"NtWriteVirtualMemory";//dword_928B4
	NtdllFunc_92820[19].FuncName = (ULONG)"ZwTerminateProcess";//dword_928BC
	NtdllFunc_92820[20].FuncName = (ULONG)"ZwCreateSection";//dword_928C4
	NtdllFunc_92820[21].FuncName = (ULONG)"NtCreateProcessEx";//dword_928CC
	NtdllFunc_92820[22].FuncName = (ULONG)"NtCreatePagingFile";//dword_928D4
	NtdllFunc_92820[23].FuncName = (ULONG)"NtAssignProcessToJobObject";//dword_928DC
	if ( VersionNumber == 4
		|| VersionNumber == 5
		|| VersionNumber == 6
		|| VersionNumber == 7
		|| VersionNumber == 8
		|| VersionNumber == 9
		|| VersionNumber == 10 )
	{
		NtdllFunc_92820[1].FuncName = (ULONG)"NtCreateThreadEx"; //9282C
		NtdllFunc_92820[10].FuncName = (ULONG)"NtCreateUserProcess";//dword_92874
	}
	NtdllFunc_92820[24].FuncName =  (ULONG)"ZwOpenKey";//dword_928E4
	NtdllFunc_92820[25].FuncName =  (ULONG)"ZwCreateKey";//dword_928EC
	NtdllFunc_92820[26].FuncName =  (ULONG)"ZwDeleteKey";//dword_928F4
	NtdllFunc_92820[27].FuncName =  (ULONG)"ZwDeleteValueKey";//dword_928FC
	NtdllFunc_92820[28].FuncName =  (ULONG)"ZwSetValueKey";//dword_92904
	NtdllFunc_92820[29].FuncName =  (ULONG)"ZwQueryValueKey";//dword_9290C
	NtdllFunc_92820[30].FuncName = (ULONG) "ZwEnumerateValueKey";//dword_92914
	NtdllFunc_92820[31].FuncName =  (ULONG)"ZwEnumerateKey";//dword_9291C
	NtdllFunc_92820[32].FuncName =  (ULONG)"ZwQueryKey";//dword_92924
	NtdllFunc_92820[33].FuncName =  (ULONG)"ZwClose";//dword_9292C
	if ( VersionNumber == 4
		|| VersionNumber == 5
		|| VersionNumber == 6
		|| VersionNumber == 7
		|| VersionNumber == 8
		|| VersionNumber == 9
		|| VersionNumber == 10 )
	{
		NtdllFunc_92820[34].FuncName = (ULONG) "ZwOpenKeyEx";//dword_92934
	}
	NtdllFunc_92820[35].FuncName =  (ULONG)"ZwRenameKey";//dword_9293C
	NtdllFunc_92820[36].FuncName =  (ULONG)"ZwRestoreKey";//dword_92944
	NtdllFunc_92820[37].FuncName =  (ULONG)"ZwWriteFile";//dword_9294C
	NtdllFunc_92820[38].FuncName =  (ULONG)"ZwOpenSection";//dword_92954
	NtdllFunc_92820[39].FuncName = (ULONG) "ZwLoadDriver";//dword_9295C
	NtdllFunc_92820[40].FuncName = (ULONG) "ZwSetSystemInformation";//dword_92964
	NtdllFunc_92820[41].FuncName =  (ULONG)"ZwRequestWaitReplyPort";//dword_9296C
	NtdllFunc_92820[42].FuncName = (ULONG) "ZwSetSystemTime";//dword_92974
	NtdllFunc_92820[43].FuncName =  (ULONG)"ZwDeviceIoControlFile";//dword_9297C
	NtdllFunc_92820[44].FuncName = (ULONG) "NtReplyPort";//dword_92984
	NtdllFunc_92820[45].FuncName =  (ULONG)"ZwUnmapViewOfSection";//dword_9298C
	NtdllFunc_92820[46].FuncName = (ULONG) "ZwFreeVirtualMemory";//dword_92994
	if ( VersionNumber == 4
		|| VersionNumber == 5
		|| VersionNumber == 6
		|| VersionNumber == 7
		|| VersionNumber == 8
		|| VersionNumber == 9
		|| VersionNumber == 10 )
	{
		NtdllFunc_92820[47].FuncName= (ULONG) "NtAlpcSendWaitReceivePort";//dword_9299C
	}
	
	 NtdllFunc_92820[48].FuncName=  (ULONG)"NtRaiseHardError";//dword_929A4
	 NtdllFunc_92820[49].FuncName=  (ULONG)"NtFsControlFile";//dword_929AC
	 NtdllFunc_92820[50].FuncName=  (ULONG)"NtTestAlert";//dword_929B4
	 NtdllFunc_92820[51].FuncName = (ULONG)"NtOpenProcess";//dword_929BC
	 NtdllFunc_92820[52].FuncName = (ULONG)"NtSetInformationProcess";//dword_929C4
	 NtdllFunc_92820[53].FuncName=  (ULONG)"NtSetTimer";//dword_929CC
	 NtdllFunc_92820[54].FuncName=  (ULONG)"ZwQueryVirtualMemory";//dword_929D4
	 NtdllFunc_92820[55].FuncName=  (ULONG)"NtDisplayString";//dword_929DC
	v1 = (int*)&NtdllFunc_92820[0];
	do
	{
		if ( *v1 )
			++v0;
		v1 += 2;
	}
	while ( v1 < (int*)&NtdllFunc_92820[56] );//Ntkrnlpa.exe   unk_929E4
	return _FillFuncAddressFromNtdllExport(v0);
}
int  _FillFuncAddressFromNtdllExport(int a1)
{
	int v1; // edi@2
	int v2; // esi@3
	int v3; // ebx@3
	int v4; // eax@3
	int v5; // edx@3
	unsigned int v6; // ecx@3
	int v7; // ecx@7
	const char *v8; // ecx@8
	int result; // eax@15
	PIMAGE_EXPORT_DIRECTORY ExportTable; // esi@17
	PULONG AddressOfFunctions; // ebx@17
	PULONG AddressOfNames; // ecx@17
	PUSHORT  AddressOfNameOrdinals; // edx@17
	unsigned int v15; // eax@17
	int v16; // eax@21
	const char *v17; // eax@22
	int v18; // [sp-50h] [bp-50h]@0
	int v19; // [sp-4Ch] [bp-4Ch]@0
	int v20; // [sp-48h] [bp-48h]@0
	int v21; // [sp-44h] [bp-44h]@0
	int v22; // [sp-40h] [bp-40h]@0
	int v23; // [sp-3Ch] [bp-3Ch]@0
	int v24; // [sp-38h] [bp-38h]@0
	int v25; // [sp-34h] [bp-34h]@0
	int v26; // [sp-30h] [bp-30h]@0
	int v27; // [sp-2Ch] [bp-2Ch]@0
	int v28; // [sp-28h] [bp-28h]@0
	int v29; // [sp-24h] [bp-24h]@0
	int v30; // [sp-20h] [bp-20h]@0
	int v31; // [sp-1Ch] [bp-1Ch]@0
	int v32; // [sp-18h] [bp-18h]@0
	int v33; // [sp-14h] [bp-14h]@0
	int v34; // [sp-10h] [bp-10h]@0
	int v35; // [sp-Ch] [bp-Ch]@0
	int v36; // [sp-8h] [bp-8h]@2
	int v37; // [sp-8h] [bp-8h]@16
	int v38; // [sp-4h] [bp-4h]@16
	int i ,j;
	PIMAGE_DOS_HEADER DosHeader;
	PIMAGE_NT_HEADERS NtHeader;
	USHORT   index=0 ; 
	ULONG addr;
	PUCHAR pFuncName = NULL;



	if ( VersionNumber == 10 )
	{
		//_SEH_prolog4(dword_46CC8, 40);
		//*(a1 - 36) = 0;
		//*(a1 - 4) = 0;
		
		if ( g_NtdllBaseAddress )
		{
			DosHeader = (PIMAGE_DOS_HEADER)g_NtdllBaseAddress;
			NtHeader = (PIMAGE_NT_HEADERS)((BYTE*)g_NtdllBaseAddress+DosHeader->e_lfanew);
			ExportTable = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)g_NtdllBaseAddress+
			NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	
			AddressOfFunctions = (ULONG*)((BYTE*)g_NtdllBaseAddress + ExportTable->AddressOfFunctions);
			AddressOfNames = (ULONG*)((BYTE*)g_NtdllBaseAddress + ExportTable->AddressOfNames);
			//*(a1 - 40) = AddressOfFunctions;
			AddressOfNameOrdinals = (PUSHORT)((BYTE*)g_NtdllBaseAddress +ExportTable->AddressOfNameOrdinals);
			//*(a1 - 44) = AddressOfNameOrdinals;
			v15 = 0;
			for ( i = 0; i <ExportTable->NumberOfFunctions; i++ )
			{
				pFuncName = (BYTE*)g_NtdllBaseAddress + AddressOfNames[i];//*(a1 - 48)
				index = AddressOfNameOrdinals[i];
				addr =  (ULONG)((BYTE*)g_NtdllBaseAddress + AddressOfFunctions[index]);//*(a1 - 52)
				for (j = 0; j < 0x38u; ++j )
				{
					if ( !NtdllFunc_92820[j].FuncAddress )
					{
						v17 = (char*)NtdllFunc_92820[j].FuncName;
						if ( v17 )
						{
							if ( !_stricmp((char*)pFuncName, v17) )
							{
								NtdllFunc_92820[j].FuncAddress = addr;
								break;
							}
						}
					}
				}
			}
		}
		//*(a1 - 4) = -2;
		//result = _SEH_epilog4(v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v37, v38);
	}
	else
	{
		//_SEH_prolog4(dword_46CA8, 44);
		//*(a1 - 36) = 0;
		//*(a1 - 4) = 0;
		if ( g_NtdllBaseAddress )
		{
			DosHeader = (PIMAGE_DOS_HEADER)g_NtdllBaseAddress;
			NtHeader = (PIMAGE_NT_HEADERS)((BYTE*)g_NtdllBaseAddress+DosHeader->e_lfanew);
			ExportTable = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)g_NtdllBaseAddress+
			NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	
			AddressOfFunctions = (ULONG*)((BYTE*)g_NtdllBaseAddress + ExportTable->AddressOfFunctions);
			AddressOfNames = (ULONG*)((BYTE*)g_NtdllBaseAddress + ExportTable->AddressOfNames);
			//*(a1 - 40) = AddressOfFunctions;
			AddressOfNameOrdinals = (PUSHORT)((BYTE*)g_NtdllBaseAddress +ExportTable->AddressOfNameOrdinals);
			//*(a1 - 44) = AddressOfNameOrdinals;
			v15 = 0;
			for ( i = 0; i <ExportTable->NumberOfFunctions; i++ )
			{
				pFuncName = (BYTE*)g_NtdllBaseAddress + AddressOfNames[i];//*(a1 - 48)
				index = AddressOfNameOrdinals[i];
				addr =  (ULONG)((BYTE*)g_NtdllBaseAddress+ AddressOfFunctions[index]);//*(a1 - 52)
				for (j = 0; j < 0x38u; ++j )//
				{
					if ( !NtdllFunc_92820[j].FuncAddress )
					{
						v17 = (char*)NtdllFunc_92820[j].FuncName;
						if ( v17 )
						{
							if ( !_stricmp((char*)pFuncName, v17) )
							{
								NtdllFunc_92820[j].FuncAddress = addr;
								break;
							}
						}
					}
				}
			}
		}
				//if ( *(a1 - 36) >= *(a1 + 8) )
				//	break;
				//++*(a1 - 32);
				//v5 = *(a1 - 60);
			
	
	    //*(a1 - 4) = -2;
		//result = _SEH_epilog4(v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32, v33, v34, v35, v36);
	
	}
	
	return 1;//result
}
BOOLEAN sub_3540A()
{
	ExInitializeResourceLite(&Resource_8DCC0);
	dword_4C16C = sub_1EA40(2048);
	return dword_4C16C != 0;
}
int __stdcall sub_1EA40(int a1) //2048
{
	int v2; // esi@3
	int v3; // eax@4

	if ( !a1 )
		return 0;
	v2 = _AllocMemoryWithLength(0x48u);
	if ( !v2 )
		return 0;
	//*(v2 + 56) = a1;
	v3 = _AllocMemoryWithLength(4 * a1);
	//*(v2 + 68) = v3;
	if ( !v3 )
	{
		_ReleaseMemoryWithLength(v2);
		return 0;
	}
	ExInitializeResourceLite(v2);
	//*(v2 + 64) = 0;
	//*(v2 + 60) = 0;
	return v2;
}
void sub_3781F()
{
	_AcquireResourceExclusiveLiteByResource(&Resource_8DC80);
	memset(&dword_8DAD4, -1, 0xC8u);
	_ReleaseResourceLiteByEresource(&Resource_8DC80);
	
}
void sub_3792C()
{
	_AcquireResourceExclusiveLiteByResource(&Resource_8DC80);
	memset(&dword_8DB9C, -1, 0xC8u);
	_ReleaseResourceLiteByEresource(&Resource_8DC80);
}
int __stdcall sub_37B06(PERESOURCE Resource)
{
	PERESOURCE v1; // eax@1
	char *v2; // eax@1
	char *v3; // eax@1
	char *v4; // eax@1
	char *v5; // eax@1
	char *v6; // eax@1
	char *v7; // eax@1
	PERESOURCE v8; // eax@1
	char *v9; // eax@1

	ExInitializeResourceLite(Resource);
	v1 = Resource + 1;
	v1->SystemResourcesList.Blink = (LIST_ENTRY*)v1;
	v1->SystemResourcesList.Flink = (LIST_ENTRY*)v1;
	v2 = (char*)(&Resource[1].OwnerTable);
	*(v2 + 1) = (char)v2;
	*v2 = v2;
	v3 = &Resource[1].SharedWaiters;
	*(v3 + 1) = v3;
	*v3 = v3;
	v4 = &Resource[1].OwnerEntry; //OwnerThreads
	*(v4 + 1) = v4;
	*v4 = v4;
	v5 = &Resource[1].ActiveEntries;
	*(v5 + 1) = v5;
	*v5 = v5;
	v6 = &Resource[1].NumberOfSharedWaiters;
	*(v6 + 1) = v6;
	*v6 = v6;
	v7 = &Resource[1].Address;
	*(v7 + 1) = v7;
	*v7 = v7;
	v8 = Resource + 2;
	v8->SystemResourcesList.Blink = v8;
	v8->SystemResourcesList.Flink = v8;
	v9 = &Resource[2].OwnerTable;
	*(v9 + 1) = v9;
	*v9 = v9;
	return 1;
}
int sub_18BE6()
{
	//sub_38204(&Resource_92C60);
	//sub_18B5C();
	return 0;
}
int __stdcall sub_38204(PERESOURCE Resource)
{
	_AcquireResourceExclusiveLiteByResource(Resource);
	sub_37B60(&Resource[1]);
	sub_37B60(&Resource[1].OwnerTable);
	sub_37B60(&Resource[1].SharedWaiters);
	sub_37B60(&Resource[1].OwnerEntry);
	sub_37B60(&Resource[1].ActiveEntries);
	sub_37B60(&Resource[1].ContentionCount);
	sub_37B60(&Resource[1].Address);
	sub_37B60(&Resource[2]);
	sub_37B60(&Resource[2].OwnerTable);
	_ReleaseResourceLiteByEresource(Resource);
	return 1;
}
void __stdcall sub_37B60(int **a1)
{
	int *v1; // ecx@2
	int v2; // eax@2
	while ( *a1 != a1 )
	{
		v1 = *a1;
		v2 = **a1;
		*a1 = v2;
		//*(v2 + 4) = a1;
	   sub_38C2F(v1);
	}
}
void __stdcall sub_38C2F(PVOID P)
{
	sub_38514((PUCHAR)P + 12);
	sub_38514((PUCHAR)P + 20);
	sub_38514((PUCHAR)P + 28);
	sub_38514((PUCHAR)P + 36);
	sub_37B60((PUCHAR)P + 11);
	sub_37BB6((PUCHAR)P + 56);
	if ( *(PULONG)((PUCHAR)P + 19) )
		_ReleaseMemoryWithLength(*(PULONG)((PUCHAR)P + 19));
	_ReleaseMemoryWithLength(P);
}
void __stdcall sub_37BB6(int **a1)
{
	int *v1; // ecx@2
	int v2; // eax@2

	while ( *a1 != a1 )
	{
		v1 = *a1;
		v2 = **a1;
		*a1 = v2;
		//*(v2 + 4) = a1;
		_ReleaseMemoryWithLength(v1);
	}
}
void __stdcall sub_38514(int **a1)
{
	int *v1; // ecx@2
	int v2; // eax@2

	while ( *a1 != a1 )
	{
		v1 = *a1;
		v2 = **a1;
		*a1 = v2;
		//*(v2 + 4) = a1;
		sub_37BD4(v1);
	}
}
void __stdcall sub_37BD4(PVOID P)
{
	if ( *((PUCHAR)P + 12) )
		_ReleaseMemoryWithLength(*(PULONG)((PUCHAR)P + 12));
	if ( *((PUCHAR)P + 15) )
		_ReleaseMemoryWithLength(*(PULONG)((PUCHAR)P + 15));
	_ReleaseMemoryWithLength(P);
}


/*
Hook KiFastCallEntry
*/
int sub_3C54F()
{
	sub_3C4D3();
	_KisInstallKiFastCallEntryHook();
	//sub_3C43B();
	return 0;
}

int sub_3C4D3()
{
	int result; // eax@1

	result = VersionNumber - 1;
	switch ( VersionNumber )
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 6:
		dword_641F8 = 84;
		break;
	case 5:
		dword_641F8 = 82;
		break;
	case 7:
		dword_641F8 = 87;
		break;
	case 8:
	case 9:
		dword_641F8 = 88;
		break;
	case 10:
		dword_641F8 = 91;
		break;
	default:
		dword_641F8 = 0;
		break;
	}
	return result;
}

signed int _KisInstallKiFastCallEntryHook()
{
	char *v0; // esi@6
	int v1; // esi@8
	signed int result; // eax@14

	if ( FirstModuleImageBase
		&& FirstModuleSize)
	{
		    KeServiceDescriptorTableBase_4C188 = KeServiceDescriptorTable->Base;
			KeServiceDescriptorTable_4C18C = KeServiceDescriptorTable->Count;//最大访问次数
			KeServiceDescriptorTable_4C190 = KeServiceDescriptorTable->Limit;//SSDT函数个数
			KeServiceDescriptorTable_4C194 = KeServiceDescriptorTable->Number; //4C194
			if(_HookZwDisplayString() >= 0)
			{
				if(
					(ULONG)KiFastCallEntryRetAddress >= (ULONG)FirstModuleImageBase
					&& (ULONG)KiFastCallEntryRetAddress <= (ULONG)FirstModuleImageBase + FirstModuleSize
					&& (v0 = (PUCHAR)KiFastCallEntryRetAddress - 2, MmIsAddressValid((PUCHAR)KiFastCallEntryRetAddress - 2))
					&& *(PUSHORT)v0 == 0xd3ff 
					)
				{
					__asm int 3;
					v1 = _KisHookKiFastCallEntry();
					if ( v1 < 0 )
					{



						__asm int 3;
						KdPrint(("第一个HOOK点被占用\n"));
						
						v1 = sub_3BFF3();
						//这里用windbg手动换那个地方，手动hook
						//ZwDisplayString(&String);//调用这个函数，进入我们的fake函数
						
						//ZwOpenProcess(NULL,FILE_SHARE_READ,NULL,NULL);
					}
					if ( FirstModuleBase_4C1A8 )
					{
					//	ZwUnmapViewOfSection((HANDLE)0xFFFFFFFF, FirstModuleBase_4C1A8);
						FirstModuleBase_4C1A8 = 0;
					}
					if ( v1 >= 0 )
						byte_641D8 = 1;
					result = v1;
				}
				else
				{
					result = STATUS_UNSUCCESSFUL;
				}
			}
			else
			{
				result = STATUS_UNSUCCESSFUL;
			}
	}
	else
	{
		result = STATUS_UNSUCCESSFUL;
	}
	return result;
}

signed int _HookZwDisplayString()
{
	signed int v0; // edi@1
	PVOID v1; // eax@1
	int v2; // ecx@3
	int v3; // esi@7
	UNICODE_STRING SystemRoutineName; // [sp+4h] [bp-8h]@1
	KIRQL Irql;
	SystemRoutineName.Length = 30;
	SystemRoutineName.MaximumLength = 32;
	SystemRoutineName.Buffer = L"ZwDisplayString";
	v0 = STATUS_UNSUCCESSFUL;
	RtlInitUnicodeString(&String,L"HelloWorld");
	v1 = MmGetSystemRoutineAddress(&SystemRoutineName);   //0x804ff2d4
	if ( !v1 )
		return STATUS_NOT_FOUND;
	if ( *(PUCHAR)v1 == 0xB8 )//b8  43000000
	{//ntoskrnl.exe
		v2 = *(PULONG)((PUCHAR)v1 + 1); //0x43
	}
	else
	{//ntdll
		if ( !NtdllFunc_92820[55].FuncAddress )  //NtDisplayString的地址  dword_929D8
			return STATUS_NOT_FOUND;
		//通过函数地址   获得索引
		v2 = _GetIndexByFuncAddress(NtdllFunc_92820[55].FuncAddress);//dword_929D8
	}
	if ( v2 & 0xFFFFC000 )
		return STATUS_NOT_FOUND;
	//_disable();
	//__writecr0(__readcr0() & 0xFFFEFFFF);


	Irql = SecCloseProtect();

	v3 = 4 * (v2 & 0x3FFF); //0x10c                        //NtDisplayString  8060a0ce
	//Ori                                                                               hook
	dword_4C1AC = InterlockedExchange(((BYTE*)(KeServiceDescriptorTable->Base) + v3), _FakeZwDisplayString); //换了ZwDisplayString
	//__writecr0(__readcr0() | 0x10000);
	//_enable();
	SecOpenProtect(Irql);
	                                           //KiFastCallEntryRetAddress 是call ebx 的下一条指令
	if ( ZwDisplayString(&String) == 0xEEEEE007 && KiFastCallEntryRetAddress ) //调用ZwDisplayString  初始化KiFastCallEntryRetAddress
	{
		if ( MmIsAddressValid(KiFastCallEntryRetAddress) )
			v0 = 0;
	}
	//_disable();
	//__writecr0(__readcr0() & 0xFFFEFFFF);
	Irql = SecCloseProtect(); 
	             //InterlockedExchange                       //NtDisplayString
	InterlockedExchange(((BYTE*)(KeServiceDescriptorTable->Base) + v3), dword_4C1AC);
	//__writecr0(__readcr0() | 0x10000);
	//_enable();
	SecOpenProtect(Irql);
	return v0;
}
//Hook函数
int __stdcall _FakeZwDisplayString(PCUNICODE_STRING String1)  //hook
{
	int result; // eax@6
	int savedregs; // [sp+8h] [bp+0h]@4

	if ( !ExGetPreviousMode() && String1 && RtlEqualUnicodeString(String1, &String, 0) )
	{

		/************************************************************************/
		/*
							lower

			nEbp			f8af1bb0  <--	esp	
			Status			f8af1bb4	
			org_ebp			f8af1bb8  <--	ebp	
			ret				f8af1bbc  --->	call ebx 的下一条指令				
			DisplayString	f8af1bc0	
			
							High
		*/
		/************************************************************************/

		savedregs = (PULONG)((PUCHAR)&savedregs + 4 + 4);
		if ( _GetKiFastCallEntryRetuanAddress((PULONG)savedregs) //将全局变量KiFastCallEntryRetAddress赋值为call ebx(NtDisplayString)  的返回地址
			|| sub_3BCCA() )  //判断一个啥子
			result = 0xEEEEE007;
		else
			result = STATUS_NOT_FOUND;
	}
	else
	{
		result = dword_4C1AC(String1); //调用原始的ZwDisplayString
	}
	return result;
}
int __stdcall _GetKiFastCallEntryRetuanAddress(PVOID VirtualAddress)
{
	PULONG pSaveRet = NULL;  // edi@5
	PUCHAR pRetAddress = NULL;// edi@7
	PUCHAR pCallAddress = NULL;// ebx@9
	signed int Count = 0; // [sp+14h] [bp-1Ch]@1

	/*
	8053e636 ffd3            call    ebx
	8053e638 8be5            mov     esp,ebp
	8053e63a 8b0d24f1dfff    mov     ecx,dword ptr ds:[0FFDFF124h]
	8053e640 8b553c          mov     edx,dword ptr [ebp+3Ch]
	8053e643 899134010000    mov     dword ptr [ecx+134h],edx
	*/

	while ( Count < 2 )
	{
		if ( !MmIsAddressValid(VirtualAddress) )
			break;
		if ( (ULONG)VirtualAddress < MmUserProbeAddress )
			break;
		pSaveRet = (PULONG)((PUCHAR)VirtualAddress + 4);  //Result Addr
		if ( !MmIsAddressValid(pSaveRet) )
			break;
		if ( (ULONG)pSaveRet < MmUserProbeAddress )
			break;
		pRetAddress = (PUCHAR)(*pSaveRet);//RetAddress
		if ( !MmIsAddressValid(pRetAddress) || (ULONG)pRetAddress < MmUserProbeAddress )
			break;
		pCallAddress = (PUCHAR)(pRetAddress - 2);//CallAddress
		if ( MmIsAddressValid(pCallAddress)
			&& *(PUSHORT)pCallAddress == 0xd3ff  //FFFFD3FF
			&& pCallAddress > (PUCHAR)FirstModuleImageBase
			&& pCallAddress < (PUCHAR)FirstModuleImageBase + FirstModuleSize )
		{
			KiFastCallEntryRetAddress = pRetAddress;  //call next  
			DbgPrint("RetAddress = %x",KiFastCallEntryRetAddress);
			return 1;
		}
		VirtualAddress = (PVOID)(*(ULONG*)VirtualAddress);//如果已经被hook，得到上一层函数帧，一般只做两次
		++Count;

		
	}
	return 0;
}
signed int sub_3BCCA()
{
	signed int v0; // esi@1

	v0 = 1;
	if ( GetStorageDeviceBusType(0x830021B0, 0, 0, &KiFastCallEntryRetAddress, 4u) < 0 || !MmIsAddressValid(KiFastCallEntryRetAddress) )
	{
		v0 = 0;
		KiFastCallEntryRetAddress = 0;
	}
	return v0;
}

signed int _KisHookKiFastCallEntry()
{
	signed int v0; // edi@1
	void *v1; // eax@3
	unsigned int v2; // esi@3
	signed int result; // eax@8
	int v4; // [sp+8h] [bp-4h]@1

	v4 = 0;
	v0 = 0;
	/*
	8053e619 8a0c18          mov     cl,byte ptr [eax+ebx]
	8053e61c 8b3f            mov     edi,dword ptr [edi]
	8053e61e 8b1c87          mov     ebx,dword ptr [edi+eax*4]          =====>v4
	8053e621 2be1            sub     esp,ecx
	8053e623 c1e902          shr     ecx,2
	8053e626 e95034a431      jmp     b1f81a7b
	8053e62b 90              nop
	8053e62c 90              nop
	8053e62d 90              nop
	8053e62e 0f83a8010000    jae     nt!KiSystemCallExit2+0x9f (8053e7dc)
	8053e634 f3a5            rep movs dword ptr es:[edi],dword ptr [esi]
	8053e636 ffd3            call    ebx
	8053e638 8be5            mov     esp,ebp
	*/
	//dword_4C1B8 = 1
	dword_4C1B8 = _KisGetKiFastCallEntryEbxAddress((ULONG)KiFastCallEntryRetAddress - 100, 100, (ULONG*)&v4);//v4 = 8053e61e
	if ( dword_4C1B8 )
	{
		KiFastCallEntryEbxAddress = v4; //0x8053e61e 8b1c87  mov     ebx,dword ptr [edi+eax*4]  
		v0 = 1;
	}
	//nt!KiFastCallEntry+0xe6:
	//8053e626 8bfc            mov     edi,esp
	//v1 = 8053e626    hook之后v1 = 0
	v1 = (void*)_KisGetKiFastCallEntryHookAddress((ULONG)KiFastCallEntryRetAddress - 100, 100, (ULONG*)MmUserProbeAddress);
	v2 = (ULONG)v1;
	if ( v1
		&& MmIsAddressValid(v1)
		&& v2 >= (ULONG)FirstModuleImageBase
		&& v2 <= (ULONG)FirstModuleImageBase + FirstModuleSize
		&& (dword_4C1C0 = v2, v0) )
	{
		result = _KisHookKiFastCallEntryPoint(v2);
	}
	else
	{
		result = STATUS_UNSUCCESSFUL;
	}
	return result;
}
                            //Ret Address - 100  , 100 ,  
signed int __stdcall _KisGetKiFastCallEntryEbxAddress(int a1, int a2, ULONG *a3)
{
	int v3; // edi@1
	int v4; // esi@1
	signed int result; // eax@1
	BYTE* v6; // ecx@2
	char v7; // bl@4

	v3 = a2;
	v4 = 0;
	result = 0;
	if ( !a2 )
		return 0;
	v6 = (a1 + 2);  //0x8053e5d6   开始搜索8b 1c 87
	do           //8053e61e 8b1c87          mov     ebx,dword ptr [edi+eax*4]  给ebx赋值的语句
	{ //87 1C 8B
		if ( *(v6 - 2) != 0x8B ) //-117
			goto LABEL_11;
		v7 = *(v6 - 1);
		/*
		8053e619 8a0c18          mov     cl,byte ptr [eax+ebx]
		8053e61c 8b3f            mov     edi,dword ptr [edi]
		8053e61e 8b1c87          mov     ebx,dword ptr [edi+eax*4]
		*/
		if ( v7 == 0x1C && *v6 == 0x87 )//28  -121
		{
			result = 1;
LABEL_10:
			*a3 = v6 - 2;//*a3 = 0x8053e61e
			++v4;
			goto LABEL_11;
		}
		if ( v7 == 0x14 && *v6 == 0x87 ) //20  -121
		{
			result = 2;
			goto LABEL_10;
		}
LABEL_11:
		++v6;
		--v3;
	}
	while ( v3 );
	if ( v4 != 1 )
		return 0;
	return result;
}
int __stdcall _KisGetKiFastCallEntryHookAddress(int a1, int a2, ULONG *a3)
{
	int v3; // edi@1
	BYTE* v4; // ecx@2
	int v6; // [sp+4h] [bp-4h]@1

	v3 = 0;
	v6 = 0;
	dword_4C1CC = 0;
	if ( !a2 )
		return 0;
	v4 = a1;
	/*
	Hook前
    kd> u 8053e626
	nt!KiFastCallEntry+0xe6:
	8053e626 8bfc            mov     edi,esp
	8053e628 3b35d4995580    cmp     esi,dword ptr [nt!MmUserProbeAddress (805599d4)]
	8053e62e 0f83a8010000    jae     nt!KiSystemCallExit2+0x9f (8053e7dc)
	8053e634 f3a5            rep movs dword ptr es:[edi],dword ptr [esi]
	8053e636 ffd3            call    ebx
	8053e638 8be5            mov     esp,ebp
	
	Hook后
	nt!KiFastCallEntry+0xe6:
	8053e626 e950e4c431      jmp     b218ca7b
	8053e62b 90              nop
	8053e62c 90              nop
	8053e62d 90              nop
	8053e62e 0f83a8010000    jae     nt!KiSystemCallExit2+0x9f (8053e7dc)
	8053e634 f3a5            rep movs dword ptr es:[edi],dword ptr [esi]
	8053e636 ffd3            call    ebx
	8053e638 8be5            mov     esp,ebp

	*/
	do
	{
		if ( *(BYTE*)v4 == 0x8B && *(BYTE*)((BYTE*)v4 + 1) == 0xFC )//-4
		{                      //59                 //53
			if ( *(v4 + 2) != 0x3B || *(v4 + 3) != 0x35)//(*(ULONG*)(v4 + 4)) != *(ULONG*)a3
			{
				if ( *(ULONG*)(v4 + 2) != 0x27245F6 )//41043446
					goto LABEL_12;
				dword_4C1CC = 2;
			}
			else
			{
				dword_4C1CC = 1;
			}
			++v3;
			v6 = v4;
		}
LABEL_12:
		++v4;
		--a2;
	}
	while ( a2 );
	if ( v3 == 1 )
		return v6;
	return 0;
}
signed int __stdcall _KisHookKiFastCallEntryPoint(int a1)
{
	int v1; // ecx@2
	int (*v2)(); // eax@2
	signed int v4; // [sp+0h] [bp-10h]@2
	ULONG64 JmpCode; // [sp+4h] [bp-Ch]@1
	KIRQL Irql;
	LOBYTE(JmpCode) = 0xE9;//-23
	//HIDWORD(JmpCode) = 0x90909090;  //-1869574000
	if ( dword_4C1CC == 1 )
	{
		//8053e626 8bfc            mov     edi,esp
		v1 = a1;//v1 = 8053e626 hook点
		v2 = (PUCHAR)_KifakeFastCallEntry;//sub_3BA7B;
		v4 = 8;
LABEL_5:
		*(ULONG*)(((PUCHAR)&JmpCode + 1)) = (PUCHAR)v2 - (PUCHAR)v1 - 5;
		FakeKiFastCallEntryRetAddr = v1 + v4;//FastCallEntryExit的调用地址 dword_4C1B4
		//_disable();
		//__writecr0(__readcr0() & 0xFFFEFFFF);
		Irql = SecCloseProtect();
		sub_3BCA6((signed __int64*)v1, JmpCode, HIDWORD(JmpCode));
		SecOpenProtect(Irql);
		//__writecr0(__readcr0() | 0x10000);
		//_enable();
		return 0;
	}
	if ( dword_4C1CC == 2 )
	{
		//v1 = a1;
		//BYTE6(v5) = *(a1 + 6);
		//v2 = sub_3BAAB;
		//v4 = 6;
		//BYTE7(v5) = *(a1 + 7);
		//goto LABEL_5;
	}
	return STATUS_UNSUCCESSFUL;
}
int __stdcall sub_3BCA6(signed __int64 *a1, signed __int64 a2)
{
	return _InterlockedCompareExchange64(a1, a2, *a1);
}
int sub_3BAAB(unsigned int a1, int a2, int a3, int a4, int a5, int a6)
{
/*	unsigned int v6; // et0@1
	int v7; // eax@1
	bool v8; // zf@3
	int v10; // [sp-Ch] [bp-20h]@1
	int v11; // [sp-8h] [bp-1Ch]@1
	int v12; // [sp+4h] [bp-10h]@1
	unsigned int v13; // [sp+10h] [bp-4h]@1

	v6 = __getcallerseflags();
	v13 = v6;
	v12 = a5;
	v11 = a3;
	v10 = a2;
	v7 = sub_3B9D2(a4, a2, a1, a3, a6);
	if ( dword_4C1B8 != 1 )
		v12 = v7;
	__writeeflags(v13);
	v8 = (*(v12 + 114) & 2) == 0;
	return FakeKiFastCallEntryRetAddr(v11, v10);*/
}
//fake func
VOID _KifakeFastCallEntry()
{
	/*
	unsigned int v5; // et0@1
	int v6; // ST18_4@1
	int v7; // ST1C_4@1
	unsigned int v9; // [sp+10h] [bp-4h]@1

	v5 = __getcallerseflags();
	v9 = v5;
	v7 = a3;
	v6 = a2;
	sub_3B9D2(a4, a2, a1, a3, a5);
	__writeeflags(v9);
	return FakeKiFastCallEntryRetAddr(v7, v6);
	*/
	__asm int 3;
	__asm
	{
		pushfd;
		pushad;
		push edi;    //SSDT Base
		push ecx;    //Argc
		push eax;    //Index 
		push edx;    //
		push ebx;    //Func Address
		call _KisSyscallFilter;
		cmp dword_4C1B8,1;
		jnz Lable1;
		mov dword ptr [esp+10h],eax;
		jmp Lable2;
Lable1:
		mov dword ptr [esp+14h],eax
Lable2:
		popad;
		popfd;
		//	这2条指令就是原先hook点的指令
		mov edi, esp;
		cmp esi, ds:MmUserProbeAddress;
		push FakeKiFastCallEntryRetAddr;	//	注意这里，压地址，然后返回先前保存的下一条指令开始执行
		retn;
	}
}
VOID __stdcall _KisSyscallFilter(int a1, int a2, unsigned int a3, int a4, int a5)
{
	//ULONG pEbx,ULONG pEdx,ULONG pEax,ULONG pEcx,ULONG pEdi
	//       a1         a2         a3        a4         a5
	int result; // eax@1
	int v6; // ecx@7
	int v7; // edx@8
	int v8; // ecx@10
	int v9; // ecx@14
	int v10; // edx@15

	//ULONG gServiceTableBase = *(PULONG*)(BaseAddress1 + 0x3C188);
	//ULONG gNumberOfServices = *(PULONG)(BaseAddress1 + 0x3C190);
	result = a1;
	if ( dword_4C1B8 != 1 )//找到给ebx赋值的语句  返回1
		result = a2;
	if ( result )
	{//KeServiceDescriptorTableBase_4C188
		//KeServiceDescriptorTable_4C190
		if ( KeServiceDescriptorTableBase_4C188 &&  KeServiceDescriptorTableBase_4C188== a5 && KeServiceDescriptorTable_4C190 > a3 )
		{
			v6 = 5 * a3;
			if ( dword_79780[5 * a3] )
			{
				v7 = dword_79790[v6];
				if ( v7 )
				{
					if ( v7 == a3 )
					{
						dword_7978C[v6] = result;
						v8 = dword_79788[v6];
						if(v8)
						{
							result = v8;
						}
						KdPrint(("index: %d -- address -- %x\n",a3,result));
						return result;
						__asm int 3;
						//goto LABEL_18;
					}
				}
			}
		}
		else
		{
			return  a1;
		}
		//else if ( dword_4C198 )
		//{
		//	if ( dword_4C198 == a5 && dword_4C1A0 > a3 )
		//	{
		//		v9 = 5 * a3;
		//		if ( dword_65780[5 * a3] )
		//		{
		//			v10 = dword_65790[v9];
		//			if ( v10 )
		//			{
		//				if ( v10 == a3 )
		//				{
		//					dword_6578C[v9] = result;
		//					v8 = dword_65788[v9];
//LABEL_18:
		//					if ( v8 )
		//						result = v8;
		//					return result;
		//				}
		//			}
		//		}
		//	}
		//}
	}
	return result;
}
ULONG
	KisSyscallFilter(ULONG pEbx,ULONG pEdx,ULONG pEax,ULONG pEcx,ULONG pEdi)  //sub_3B9D2
{
	/*KdPrint(("KisSyscallFilter\n"));
	int result; // eax@1
	int v6; // ecx@7
	int v7; // edx@8
	int v8; // ecx@10
	int v9; // ecx@14
	int v10; // edx@15

	result = a1;
	if ( dword_4C1B8 != 1 )
		result = a2;
	if ( result )
	{
		if ( dword_4C188 && dword_4C188 == a5 && dword_4C190 > a3 )
		{
			v6 = 5 * a3;
			if ( dword_79780[5 * a3] )
			{
				v7 = dword_79790[v6];
				if ( v7 )
				{
					if ( v7 == a3 )
					{
						dword_7978C[v6] = result;
						v8 = dword_79788[v6];
						goto LABEL_18;
					}
				}
			}
		}
		else if ( dword_4C198 )
		{
			if ( dword_4C198 == a5 && dword_4C1A0 > a3 )
			{
				v9 = 5 * a3;
				if ( dword_65780[5 * a3] )
				{
					v10 = dword_65790[v9];
					if ( v10 )
					{
						if ( v10 == a3 )
						{
							dword_6578C[v9] = result;
							v8 = dword_65788[v9];
LABEL_18:
							if ( v8 )
								result = v8;
							return result;
						}
					}
				}
			}
		}
	}
	return result;

	*/
	return pEbx;
}


//被钩过重新钩
signed int sub_3BFF3()
{
	
	signed int result; // eax@2
	PIMAGE_NT_HEADERS v1; // edi@4
	PIMAGE_NT_HEADERS v2; // eax@4
	PIMAGE_NT_HEADERS v3; // ebx@4
	char *v4; // eax@11
	char *v5; // esi@13
	char v6; // cl@19
	void *v7; // esi@21
	int v8; // eax@23
	unsigned int v9; // edi@24
	char v10; // cl@31
	char v11; // al@34
	void *v12; // esi@36
	char v13; // al@41
	int v14; // [sp+18h] [bp-28h]@12
	int v15; // [sp+1Ch] [bp-24h]@12
	int v16; // [sp+20h] [bp-20h]@1
	int v17; // [sp+24h] [bp-1Ch]@1
	//CPPEH_RECORD ms_exc; // [sp+28h] [bp-18h]@4
	ULONG *v18;
	v16 = 0;
	v17 = 0;
	FirstModuleBase_4C1A8 = _CreateFileMap(&FirstModulePath, &v16, &v17);
	if ( !FirstModuleBase_4C1A8 || v17 < ((ULONG)KiFastCallEntryRetAddress - (ULONG)FirstModuleImageBase + 4096) )
		return STATUS_UNSUCCESSFUL;
	//ms_exc.registration.TryLevel = 0;
	v1 = RtlImageNtHeader(FirstModuleImageBase);
	v2 = RtlImageNtHeader(FirstModuleBase_4C1A8);
	v3 = v2;
	if ( !v1
		|| !v2
		|| v1->OptionalHeader.CheckSum != v2->OptionalHeader.CheckSum
		|| v1->OptionalHeader.ImageBase != v2->OptionalHeader.ImageBase
		|| v1->OptionalHeader.SizeOfImage != v2->OptionalHeader.SizeOfImage
		|| v1->OptionalHeader.BaseOfCode != v2->OptionalHeader.BaseOfCode
		|| v1->OptionalHeader.SizeOfCode != v2->OptionalHeader.SizeOfCode
		 )
	{
LABEL_15:
		//ms_exc.registration.TryLevel = -2;
		return STATUS_UNSUCCESSFUL;
	}
	if((v4 = (ULONG)FirstModuleBase_4C1A8 + (ULONG)KiFastCallEntryRetAddress - (ULONG)FirstModuleImageBase, *(PUSHORT)((BYTE*)v4 - 2) != 0xd3ff)
		)
	{
		return STATUS_UNSUCCESSFUL;
	}
	if((v14 = (v4 - 100), (dword_4C1B8 = _KisGetKiFastCallEntryEbxAddress((v4 - 100), 100, &v15)) == 0))
	{
		return STATUS_UNSUCCESSFUL;
	}
	v5 = FirstModuleImageBase;
	dword_4C1BC = (ULONG)FirstModuleImageBase + v15 - (ULONG)FirstModuleBase_4C1A8;  //ebx address
	if ( dword_4C1BC )
	{
		if ( v15 - (ULONG)FirstModuleBase_4C1A8 != v15 - (ULONG)FirstModuleBase_4C1A8 )
			goto LABEL_15;
	}
	else
	{
		if ( !MmIsAddressValid(0) )
			goto LABEL_15;
		if ( dword_4C1BC < FirstModuleImageBase )
			goto LABEL_15;
		if ( dword_4C1BC > (ULONG)FirstModuleImageBase + FirstModuleSize )
			goto LABEL_15;
		v6 = *(ULONG*)(dword_4C1BC - 2);
		if ( v6 != -23 && v6 != -24 )
			goto LABEL_15;
		v7 = (*(ULONG*)(dword_4C1BC - 1) + dword_4C1BC + 3);
		if ( !MmIsAddressValid(v7) )
			goto LABEL_15;
		dword_4C1C8 = v7;
		v5 = FirstModuleImageBase;
	}
	v18 = (ULONG*)(MmUserProbeAddress +v3->OptionalHeader.ImageBase -(ULONG)v5);
	//v8  hook Address
	v8 = _KisGetKiFastCallEntryHookAddress(v14, 100, v18);
	if ( !v8 )
		goto LABEL_15;
	v9 = &v5[v8 - (ULONG)FirstModuleBase_4C1A8];  //hook Address
	if ( dword_4C1C0 )
	{
		if ( v8 - (ULONG)FirstModuleBase_4C1A8 != (ULONG)dword_4C1C0 - (ULONG)v5 )
			goto LABEL_15;
	}
	else
	{
		if ( !MmIsAddressValid(v9)
			|| v9 < (ULONG)FirstModuleImageBase
			|| v9 > (ULONG)FirstModuleImageBase + FirstModuleSize
			|| v9 > (ULONG)KiFastCallEntryRetAddress )
		{
			goto LABEL_15;
		}
		v10 = *(ULONG*)v9;
		if ( *(BYTE*)v9 == 0xE9 || v10 == 0xE8 )
		{
			v12 = (*((BYTE*)v9 + 1) + (BYTE*)v9 + 5);
		}
		else
		{
			if ( v10 != -112 )
				goto LABEL_15;
			v11 = *((BYTE*)v9 + 3);
			if ( v11 != 0xE9 && v11 != 0xE8 )
			{
				v13 = *((BYTE*)v9 + 1);
				if ( v13 != 0xE9 && v13 !=0xE8 )
					goto LABEL_15;
				v12 = (*((BYTE*)v9 + 2) +(BYTE*)v9 + 6);
			}
			else
			{
				v12 = (*((BYTE*)v9 + 4) + (BYTE*)v9 + 8);
			}
		}
		if ( !MmIsAddressValid(v12) )
			goto LABEL_15;
		//dword_4C1C4 = v12;
		dword_4C1C0 = v9;
	}
	//ms_exc.registration.TryLevel = -2;
	if ( sub_3BD9F(v9) < 0 )
		result = _KisHookKiFastCallEntryPoint(v9);
	else
		result = 0;
	return result;

	
}

signed int __stdcall sub_3BD9F(int a1)
{
	int v1; // esi@1
	unsigned int v2; // ecx@5
	int v4; // eax@10
	int (__fastcall *v5)(ULONG, ULONG); // edi@10
	signed int v6; // [sp-4h] [bp-10h]@2
	KIRQL Irql;
	v1 = a1;
	if ( dword_4C1CC == 1 )
	{
		v6 = 8;
	}
	else
	{
		if ( dword_4C1CC != 2 )
			return STATUS_UNSUCCESSFUL;
		v6 = 6;
	}
	v2 = 0;
	while ( *(BYTE*)v1 != 0xE9 )
	{
		++v2;
		++v1;
		if ( v2 > v6 - 5 )
			return STATUS_UNSUCCESSFUL;
	}
	v4 = *((BYTE*)v1 + 1);
	//v5 = (v1 + v4 + 5);
	v5 =(BYTE*)v1+8;
	if ( v1 + v4 == 0xFB || !MmIsAddressValid((v5)) )//v1 + v4 + 5
		return STATUS_UNSUCCESSFUL;
	FakeKiFastCallEntryRetAddr = (int (__stdcall *)(int,int))v5;
	//_disable();
	//__writecr0(__readcr0() & 0xFFFEFFFF);
	Irql = SecCloseProtect(); 
	_InterlockedExchange(((BYTE*)v1 + 1), (BYTE*)sub_3BAD9 -(BYTE*)v1 - 5);
	//__writecr0(__readcr0() | 0x10000);
	//_enable();
	SecOpenProtect(Irql);
	return 0;
}


_declspec(naked) int sub_3BAD9(unsigned int a1, int a2, int a3, int a4, int a5)
{

///	unsigned int v5; // et0@1
//	int v6; // ST18_4@1
//	int v7; // ST1C_4@1
//	unsigned int v9; // [sp+10h] [bp-4h]@1
/*
	v5 = __getcallerseflags();
	v9 = v5;
	v7 = a3;
	v6 = a2;
	_KisSyscallFilter(a4, a2, a1, a3, a5);
	__writeeflags(v9);
	return FakeKiFastCallEntryRetAddr(v7, v6);*/
	
	__asm
	{
		pushfd;
		pushad;
		push edi;
		push ecx;
		push eax;
		push edx;
		push ebx;
		call _KisSyscallFilter;
		cmp dword_4C1B8,1;
		jnz Lable1;
		mov dword ptr [esp+10h],eax;
		jmp Lable2;
Lable1:
		mov dword ptr [esp+14h],eax
Lable2:
		popad;
		popfd;
		//	这2条指令就是原先hook点的指令
		mov edi, esp;
		cmp esi, ds:MmUserProbeAddress;
		push FakeKiFastCallEntryRetAddr;	//	注意这里，压地址，然后返回先前保存的下一条指令开始执行
		ret;
	}
}