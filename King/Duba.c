
#ifndef CXX_DUBA_H
#	include "Duba.h"
#endif




//////////////////////////////Driver Entry
extern ULONG InitSafeBootMode;       //安全模式
extern POBJECT_TYPE IoDriverObjectType;//对象类型  ObReferenceObjectByName
PDRIVER_OBJECT KisDriver;
RTL_OSVERSIONINFOEXW osverInfo = {sizeof(osverInfo)}; //RtlGetVersion
 ////////////////////////////Version
ULONG _BuildNumber =0 ;         //4B754
ULONG VersionNumber = 0;                 
ULONG ServicePackNumber = 0;
//////////////////////////Other
ERESOURCE  Resource;
ULONG dword_4C180 = 0;
PDEVICE_OBJECT Device_Object;    //物理磁盘设备对象
ULONG PagedLookasideListd_word_4BE5C  = 0;
ULONG NoPagedLookasideListd_word_4C164  = 0;
ULONG NoPagedLookasideList_dword_4C168  = 0;
ULONG ResourceFlagd65290_65348 = 0;
/////////////////////////////Modules
WCHAR FirstModulePath[260];
ULONG FirstModuleSize = 0;    //dword_92BE8
PVOID g_Win32kBaseAddress;
PVOID g_Win32kfullBaseAddress;
PVOID g_NtdllBaseAddress;
ULONG dword_4B7D8;
PVOID FirstModuleImageBase;
//////////////////////////////Hook
ULONG IndexOfNtTerminateProcess = 0x00000101;  //dword_8E250
PVOID BaseAddress;
PVOID AddressOfNtTerminateProcess = 0;   //dword_8E25C
PVOID OrgObReferenceByHandle;   //dword_92808 = 0x805b1ab6 ;
//////////////////////////////HookKiFastCall
PAGED_LOOKASIDE_LIST ListHead;
NPAGED_LOOKASIDE_LIST NoPagedLookasideList_stru_8DD00;
NPAGED_LOOKASIDE_LIST NoPagedLookasideList_stru_8DD60;
ERESOURCE Resource65290;
ERESOURCE Resource_8DC80;
ERESOURCE Resource_8DCC0;
ERESOURCE Resource_92C60;
ULONG dword_92CE0[21] = {0};
ULONG OffsetOfImageName_8D9E0[161] = {0};
ULONG dword_4C16C = 0;
ULONG dword_8DAD4 = 0;//000004dc
//Index  FuncAddress
StructFunc NtdllFunc_92820[57] = {0};
ULONG NtQueryInformationThread_Indexunk_4B7E4 = 0;
ULONG NtQueryInformationObject_Indexunk_4B7E8 =0;
ULONG NtReadVirtualMemory_Indexunk_4B7EC =0;
ULONG NtWriteVirtualMemory_Indexunk_4B7F0 =0;
ULONG NtProtectVirtualMemory_Indexunk_4B7F4 =0;
ULONG NtReplyPort_Indexunk_4B7F8 =0;
ULONG ZwQueryVirtualMemory_Indexunk_4B7FC =0;
//Func
PVOID   IoCreateFileSpecifyDeviceObjectHint_4B798 = 0; 
PVOID	IoCreateFileExd_4B79C =0; 
PVOID	FsRtlAllocateExtraCreateParameterList_4B7A0=0; 
PVOID   FsRtlFreeExtraCreateParameterList_4B7A4 =0; 
PVOID	FsRtlAllocateExtraCreateParameter_4B7B0 =0; 
PVOID	FsRtlInsertExtraCreateParameter_4B7A8 =0; 
PVOID	FsRtlRemoveExtraCreateParameter_4B7AC =0; 
PVOID	FsRtlFreeExtraCreateParameter_4B7B4 =0; 
PVOID	FsRtlGetEcpListFromIrp_4B7B8 =0; 
PVOID	FsRtlFindExtraCreateParameter_4B7BC =0;
PVOID	PsGetProcessExitProcessCalled_4B7C0 =0; 
//SSDT
PKSERVICE_TABLE_DESCRIPTOR1 KeServiceDescriptorTable_4C188;
ULONG dword_8DB9C;
ULONG dword_641F8;
UNICODE_STRING String;
int (__stdcall *dword_4C1AC)(int);
int (__stdcall *dword_4C1B4)(int,int);
PVOID KiFastCallEntryRetAddress;//KiFastCallEntry 中call ebx 的返回地址

ULONG KeServiceDescriptorTableBase_4C188;
ULONG KeServiceDescriptorTable_4C18C;
ULONG KeServiceDescriptorTable_4C190;
ULONG KeServiceDescriptorTable_4C194;


PULONG dword_79780;
PULONG dword_79788;
PULONG dword_7978C;
PULONG dword_79790;

ULONG dword_4C1BC;
ULONG dword_4C1C8;


//ULONG dword_4C1B4;//  e9  +8    8053e62e 0f83a8010000    jae     nt!KiSystemCallExit2+0x9f (8053e7dc)
ULONG dword_4C1B8;
ULONG KiFastCallEntryEbxAddress; //给ebx赋值的地址
ULONG dword_4C1CC;
ULONG dword_4C1C0;
PVOID FirstModuleBase_4C1A8;
BYTE byte_641D8;


NTSTATUS
DriverEntry(IN PDRIVER_OBJECT pDriverObj, IN PUNICODE_STRING pRegistryString)
{
	
	NTSTATUS		status = STATUS_SUCCESS;
	UNICODE_STRING DriverName;
	UNICODE_STRING uniFunction;
	int v2 = 0;
	pfnRtlGetVersion RtlGetVersionAddress = NULL;
//	if(InitSafeBootMode)//是否处于安全模式
//	{
//		return STATUS_ACCESS_DENIED;
//	}
	__asm int 3;

	RtlInitUnicodeString(&DriverName,L"\\Driver\\kisknl");
	status = ObReferenceObjectByName(&DriverName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		FILE_ALL_ACCESS,
		IoDriverObjectType,
		KernelMode,
		NULL,
		&KisDriver);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	
	//_AllocateMemoryWithLength()
	RtlInitUnicodeString(&uniFunction,L"RtlGetVersion");
	
	RtlGetVersionAddress = (pfnRtlGetVersion)MmGetSystemRoutineAddress(&uniFunction);
	if(RtlGetVersionAddress)
	{
		RtlGetVersionAddress((PRTL_OSVERSIONINFOW)&osverInfo);
	}
	_GetSystemVersion();
	//if(!VersionNumber || _InitializeResourceLite() ||  ||_GetSystemModule())

	_InitializeResourceLite();
	Device_Object=_OpenPhysicalDrive0();
	_GetSystemModule();




	sub_1742B(v2);

	_HookObferenceObjectByHandleInNtTerminateProcess();

	pDriverObj->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}













VOID
DriverUnload(IN PDRIVER_OBJECT pDriverObj)
{	
	return;
}

