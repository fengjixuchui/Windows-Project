#include "GetModules.h"
#include "Other.h"
extern PVOID FirstModuleImageBase;
extern WCHAR FirstModulePath[260];
extern ULONG FirstModuleSize;

extern PVOID g_Win32kBaseAddress;
extern PVOID g_Win32kfullBaseAddress;
extern PVOID g_NtdllBaseAddress;
extern ULONG dword_4B7D8;


signed int _GetSystemModule()
{
	NTSTATUS Status,Status1;
	SIZE_T SystemInformationLength = 0;
	PRTL_PROCESS_MODULES SystemInformation = NULL;
	PRTL_PROCESS_MODULES Buffer = NULL;
	PVOID Address = NULL;
	ULONG ModuleNumber;
	PRTL_PROCESS_MODULE_INFORMATION PModuleInformation;
	char *v7=NULL;
	STRING DestinationString;
	UNICODE_STRING UnicodeString;
	int v8;
	int v9; // ebx@9
	char* NtdllBaseAddress; // edi@18
	int v12; // eax@23
	void *v13; // edi@28
	void *v14; // eax@29
	NTSTATUS v15; // eax@31
	signed int v16; // esi@31
	signed int result; // eax@33
	signed int v18; // ebx@37
	PSYSTEM_PROCESS_INFORMATION i; // esi@37
	char *v22; // [sp+18h] [bp-14h]@11
	SIZE_T Length = 1;
	RTL_PROCESS_MODULE_INFORMATION ModuleInformation;
	int m=0;
	SystemInformationLength = 1;

	while(1)
	{
		Buffer = (PRTL_PROCESS_MODULES)_AllocMemoryWithLength(Length);
		Address = Buffer;
		SystemInformation = Buffer;
		if(!Buffer)
		{
			return STATUS_NO_MEMORY;
		}
		Status = ZwQuerySystemInformation(SystemModuleInformation,Buffer,Length,0);
		Status1 = Status;
		if(Status!=STATUS_INFO_LENGTH_MISMATCH)
		{
			break;
		}
		_ReleaseMemoryWithLength(Address);
		SystemInformationLength = 2* Length;
		Length *= 2;
	}
	if(!NT_SUCCESS(Status))
	{
		_ReleaseMemoryWithLength(Address);
		result =Status1;
	}
	else
	{
		ModuleNumber = Buffer->NumberOfModules;
		PModuleInformation = Buffer->Modules;
		v7 = strrchr((char*)(PModuleInformation->FullPathName),92);//\\
		if( v7 )
		{
			RtlInitAnsiString(&DestinationString , v7 + 1);//查找到最后
			if( RtlAnsiStringToUnicodeString(&UnicodeString,&DestinationString,1u)>=0)
			{
			//	__asm int 3;
				Asm_19856((int)FirstModulePath,260,(int)(L"\\SystemRoot\\system32\\"));
				Asm_1C49E((int)FirstModulePath,260,(int)UnicodeString.Buffer,(int)(UnicodeString.Length>>1));
				RtlFreeUnicodeString(&UnicodeString);
			}
		}
		v8 = ModuleNumber - 1;
		FirstModuleImageBase = PModuleInformation->ImageBase;
		v9 = ModuleNumber - 1;
		FirstModuleSize = PModuleInformation->ImageSize;
		if ( ((ModuleNumber - 1) & 0x80000000) == 0 )
		{	
			for(m=0;m<Buffer->NumberOfModules;m++)
			{
				ModuleInformation = Buffer->Modules[m];

				if(strstr((char*)(ModuleInformation.FullPathName),"win32k.sys")!=NULL)
				{
					g_Win32kBaseAddress = ModuleInformation.ImageBase;
				}
				if(strstr((char*)(ModuleInformation.FullPathName),"win32kfull.sys")!=NULL)
				{
					g_Win32kfullBaseAddress = ModuleInformation.ImageBase;
				}
			}
		}
		if ( ModuleNumber >= 1 )
		{
			NtdllBaseAddress = (char*)(&PModuleInformation[ModuleNumber-1].ImageBase);
			if ( *(PVOID*)NtdllBaseAddress <= MmSystemRangeStart )
			{
				for(m=0;m<Buffer->NumberOfModules;m++)
				{
					ModuleInformation = Buffer->Modules[m];
					NtdllBaseAddress = (char*)ModuleInformation.ImageBase;
					if(strstr((char*)ModuleInformation.FullPathName,"ntdll.dll")!=NULL)
					{
						g_NtdllBaseAddress = NtdllBaseAddress;
						dword_4B7D8 = NtdllBaseAddress + 4;
					}
				}

			}
		}
		v12 = (int)g_NtdllBaseAddress;
		if ( !g_NtdllBaseAddress )
		{
			v12 = (int)_CreateFileMap(L"\\SystemRoot\\System32\\ntdll.dll", (int)g_NtdllBaseAddress, (int)g_NtdllBaseAddress);
			g_NtdllBaseAddress = (PVOID)v12;
		}
		if ( FirstModuleImageBase && g_Win32kBaseAddress && v12 )
		{
			while ( 1 )
			{
				v13 = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
				if(!SystemInformation)
				{
					v14 = (void*)_AllocMemoryWithLength(SystemInformationLength);
					if(!14)
					{
						return STATUS_NO_MEMORY;
					}
					v13 = (PSYSTEM_PROCESS_INFORMATION)v14;
				}
				v15 = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, v13, SystemInformationLength, 0);
				v16 = v15;
				if ( v15 != STATUS_INFO_LENGTH_MISMATCH )
					break;
				_ReleaseMemoryWithLength(v13);
				SystemInformation = 0;
				SystemInformationLength *= 2;
			}
			if ( v15 >= 0 )
			{
				v18 = STATUS_NOT_FOUND;
				//找到csrss
				for ( i = (PSYSTEM_PROCESS_INFORMATION)v13;
					i->ImageName.Length != 18 || _wcsnicmp(i->ImageName.Buffer, L"csrss.exe", 9u);
					i =(PSYSTEM_PROCESS_INFORMATION)((char *)i+i->NextEntryOffset) )
				{
					if ( !i->NextEntryOffset )
						goto LABEL_44;
				}
				if ( sub_2DF21((int)i->UniqueProcessId,i->SpareUl2 ))
					v18 = 0;

			    //ULONG retureSize=0;   
			    //Status = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, NULL, 0, &retureSize);   
			    //if ( Status == STATUS_INFO_LENGTH_MISMATCH )   
			    //{
				//v13 = _AllocMemoryWithLength(retureSize);
				//if(!v13)
				//{
				//	_ReleaseMemoryWithLength(v13);
				//	return STATUS_NO_MEMORY;
				//}
				//Status = ZwQuerySystemInformation(SystemProcessesAndThreadsInformation, v13, retureSize, 0);
				//if(!NT_SUCCESS(Status))
				//{
				//	_ReleaseMemoryWithLength(v13);
				//}
				//if ( Status >= 0 )
				//{
				//	v18 = STATUS_NOT_FOUND;
				//	for ( i = (PSYSTEM_PROCESS_INFORMATION)v13; i->ImageName.Length != 18 || _wcsnicmp(i->ImageName.Buffer, L"csrss.exe", 9u);
				//		i = (i + i->NextEntryOffset) )
				//	{
				//		if ( !i->NextEntryOffset )
				//			goto LABEL_44;
			    //
				//	}
				//	if ( sub_2DF21((int)i->UniqueProcessId,i->PeakWorkingSetSize) )
				//		v18 = 0;
LABEL_44:
					_ReleaseMemoryWithLength(v13);
					result = v18;
				}
				else
				{
					_ReleaseMemoryWithLength(v13);
					result = Status;
				}
		

		}
		else
		{
			_ReleaseMemoryWithLength(SystemInformation);
			result = STATUS_NOT_FOUND;
		}
	}
	return result;
}


NTSTATUS  
	Asm_19856(IN int a1,
	int a2,
	int  a3)
{
	PVOID Address;
	NTSTATUS Result;

	Address = (PVOID)(BaseAddress1 + 0x9856);

	_asm
	{
		push a3
			push a2;
		push a1;

		call Address;
		mov Result,eax;
	}

	return Result;
}




NTSTATUS  
	Asm_1C49E(IN int a1,
	unsigned int  a2,
	int  a3,
	unsigned int a4)
{
	PVOID Address;
	NTSTATUS Result;

	Address = (PVOID)(BaseAddress1 + 0xC49E);

	_asm
	{
		push a4
		push a3;
		push a2;
		push a1;

		call Address;
		mov Result,eax;
	}

	return Result;
}



signed int __stdcall sub_2DF21(int a1, unsigned int a2)
{
	signed int result; // eax@3

	if ( a1 && a2 < 0xA )
	{
		//	dword_8E164[2 * a2] = a1;
		result = 1;
	}
	else
	{
		result = 0;
	}
	return result;
}

PVOID __stdcall _CreateFileMap(PCWSTR SourceString, int a2, int a3)
{
	SECTION_IMAGE_INFORMATION SectionInformation; // [sp+8h] [bp-68h]@3
	OBJECT_ATTRIBUTES ObjectAttributes; // [sp+38h] [bp-38h]@1
	UNICODE_STRING DestinationString; // [sp+50h] [bp-20h]@1
	struct _IO_STATUS_BLOCK IoStatusBlock; // [sp+58h] [bp-18h]@1
	PVOID BaseAddress; // [sp+60h] [bp-10h]@1
	ULONG_PTR ViewSize; // [sp+64h] [bp-Ch]@1
	HANDLE FileHandle; // [sp+68h] [bp-8h]@1
	HANDLE SectionHandle; // [sp+6Ch] [bp-4h]@1

	IoStatusBlock.Status = 0;
	IoStatusBlock.Information = 0;
	BaseAddress = 0;
	FileHandle = 0;
	SectionHandle = 0;
	ViewSize = 0;
	RtlInitUnicodeString(&DestinationString, SourceString);
	ObjectAttributes.ObjectName = &DestinationString;
	ObjectAttributes.Length = 24;
	ObjectAttributes.RootDirectory = 0;
	ObjectAttributes.Attributes = 576;
	ObjectAttributes.SecurityDescriptor = 0;
	ObjectAttributes.SecurityQualityOfService = 0;
	if ( ZwOpenFile(&FileHandle, FILE_SYNCHRONOUS_IO_NONALERT|FILE_RESERVE_OPFILTER|MEM_4MB_PAGES, &ObjectAttributes, 
		&IoStatusBlock, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
		FILE_ATTRIBUTE_DEVICE|FILE_ATTRIBUTE_ARCHIVE) >= 0 )
	{//0x80100020    0x7  0x60
		ObjectAttributes.ObjectName = 0;
		//0xD 2 IMAGE_SECMENT = 0x100 0000
		if ( ZwCreateSection(&SectionHandle, SECTION_MAP_EXECUTE|SECTION_MAP_READ|SECTION_MAP_WRITE, &ObjectAttributes,
			0, PAGE_READONLY, 0x1000000u, FileHandle) >= 0
			&& ZwQuerySection(SectionHandle, SectionImageInformation, &SectionInformation, 0x30u, 0) >= 0 )
		{
			if ( a2 )
				*(int*)a2 = SectionInformation.ImageFileSize;
			ZwMapViewOfSection(SectionHandle, (HANDLE)0xFFFFFFFF, &BaseAddress, 0, 0, 0, &ViewSize, ViewShare, 0, 0x20u);
			if ( a3 )
				*(int*)a3 = ViewSize;
		}
	}
	if ( FileHandle )
		ZwClose(FileHandle);
	if ( SectionHandle )
		ZwClose(SectionHandle);
	return BaseAddress;
}
