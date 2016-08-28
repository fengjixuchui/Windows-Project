#include "Other.h"



extern ULONG dword_4C180;
extern ULONG PagedLookasideListd_word_4BE5C;
extern ULONG NoPagedLookasideListd_word_4C164;
extern ULONG NoPagedLookasideList_dword_4C168;

extern ERESOURCE  Resource;
extern PAGED_LOOKASIDE_LIST ListHead;
extern NPAGED_LOOKASIDE_LIST NoPagedLookasideList_stru_8DD00;
extern NPAGED_LOOKASIDE_LIST NoPagedLookasideList_stru_8DD60;
extern ERESOURCE Resource65290;
extern ULONG ResourceFlagd65290_65348;


/*
申请非分页内存空间
*/
int _AllocMemoryWithLength(SIZE_T NumberOfBytes)
{
	int result;
	PVOID BaseAddress = NULL;
	int Address;

	if(NumberOfBytes)
	{
		BaseAddress = ExAllocatePoolWithTag(NonPagedPool,NumberOfBytes,'KSMA');
		Address = (int)BaseAddress;
		if(BaseAddress)
		{
			memset(BaseAddress,0,NumberOfBytes);
		}
		result = Address;
	}
	else
	{
		result = 0;
	}
	return result;
}
void _ReleaseMemoryWithLength(PVOID p)
{
	if(p)
	{
		ExFreePoolWithTag(p,'KSMA');
	}
}



PDEVICE_OBJECT _OpenPhysicalDrive0()
{
	UNICODE_STRING DestinationString;
	PFILE_OBJECT FileObject = NULL;
	PDEVICE_OBJECT DeviceObject = NULL;
	RtlInitUnicodeString(&DestinationString,L"\\??\\PhysicalDrive0");
	if(IoGetDeviceObjectPointer(&DestinationString,0x80,&FileObject,&DeviceObject)>0)
	{
		DeviceObject = FileObject->DeviceObject;
		ObfDereferenceObject(FileObject);
	}
	return DeviceObject;
}

/*
资源锁
*/
signed int _InitializeResourceLite()
{
	signed int result;
	ExInitializeResourceLite(&Resource);
	Asm_3A1E7();
	result = 1;
	dword_4C180 = 1;
	return 1;
}
BOOLEAN __stdcall _AcquireResourceSharedLiteByEresource(PERESOURCE Resource)
{
	KeEnterCriticalRegion();
	return ExAcquireResourceSharedLite(Resource, 1u);
}
void __stdcall _ReleaseResourceLiteByEresource(PERESOURCE Resource)
{
	ExReleaseResourceLite(Resource);
	KeLeaveCriticalRegion();
}
int  Asm_3A1E7()
{
	PVOID Address;
	int Result;

	Address = (PVOID)(BaseAddress1 + 0x2A1E7);

	_asm
	{
		call Address;
		mov Result,eax;
	}

	return Result;
}


/*
In KiFastCallEntry
*/
NTSTATUS _InitializResourceLite409BA()
{
	ResourceFlagd65290_65348 = 0;
	return ExInitializeResourceLite(&Resource65290);
}
BOOLEAN __stdcall _AcquireResourceExclusiveLiteByResource(PERESOURCE Resource)//sub_1EF33
{
	KeEnterCriticalRegion();
	return ExAcquireResourceExclusiveLite(Resource, 1u);
}
/*
LookasideList
*/
signed int _InitializePagedLookasideList()
{
	signed int result; // eax@1

	ExInitializePagedLookasideList(&ListHead, 0, 0, 0, 0x102u, 0x6B726571u, 0);
	result = 1;
	PagedLookasideListd_word_4BE5C = 1;
	return result;
}
void   _InitializeNPagedLookasideList() //sub_34D86()
{
	if ( !NoPagedLookasideListd_word_4C164 )
	{
		ExInitializeNPagedLookasideList(&NoPagedLookasideList_stru_8DD00, 0, 0, 0, 0x240u, 0x53424B4Eu, 0);
		NoPagedLookasideListd_word_4C164 = 1;
	}
	if ( !NoPagedLookasideList_dword_4C168 )
	{
		ExInitializeNPagedLookasideList(&NoPagedLookasideList_stru_8DD60, 0, 0, 0, 0x10000u, 0x53424B4Eu, 0);
		NoPagedLookasideList_dword_4C168 = 1;
	}
}