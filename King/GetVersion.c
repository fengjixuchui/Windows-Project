#include "GetVersion.h"
#include "Duba.h"




extern ULONG _BuildNumber;
extern ULONG VersionNumber;
extern ULONG ServicePackNumber;

extern PVOID g_Win32kBaseAddress;

BOOLEAN _GetSystemVersion()
{
	BOOLEAN result; // al@1
	signed int v1; // edx@1
	ULONG BuildNumber = 0; // [sp+4h] [bp-Ch]@1
	ULONG MinorVersion = 0; // [sp+8h] [bp-8h]@1
	ULONG MajorVersion = 0; // [sp+Ch] [bp-4h]@1

	result = PsGetVersion(&MajorVersion, &MinorVersion, &BuildNumber, 0);
	v1 = 5;
	_BuildNumber = BuildNumber;//4b754
	if ( MajorVersion == 5 )
	{
		result = MinorVersion;
		if ( !MinorVersion )   //0
		{
			VersionNumber = 1;   //dword_4b750
			return result;
		}
		if ( MinorVersion == 1 )   //XP
		{
			VersionNumber = 2;
			return _GetServicePackNumber();
		}
		result = MinorVersion - 2;
		if ( MinorVersion == 2 )
		{
			VersionNumber = 3;
			return result;   //0
		}
		goto LABEL_25;
	}


	if ( MajorVersion != 6 )
	{
		v1 = 10;
		if ( MajorVersion != 10 )
			goto LABEL_25;
		result = 0;
		if ( BuildNumber < 0x2800 )
			goto LABEL_14;
		if ( BuildNumber != 10240 )
			return result;
LABEL_24:
		VersionNumber = v1;
		return result;
	}

	result = MinorVersion;
	if ( !MinorVersion )
	{///0
		VersionNumber = 4;
		return result;
	}
	result = MinorVersion - 1;
	if ( MinorVersion == 1 )
		goto LABEL_24;
	result = MinorVersion - 2;
	if ( MinorVersion == 2 )
	{
		VersionNumber = 6;
		return result;
	}
	result = MinorVersion - 3;
	if ( MinorVersion != 3 )
	{
		result = MinorVersion - 4;
		if ( MinorVersion == 4 )
		{
LABEL_14:
			VersionNumber = 9;
			return result;
		}
LABEL_25:
		VersionNumber = 0;
		return result;
	}
	if ( BuildNumber == 9431 )
		VersionNumber = 7;
	if ( BuildNumber == 9600 )
		VersionNumber = 8;
	return result;
}



NTSTATUS _GetServicePackNumber()   //获得Service Pack 3的版本
{
	NTSTATUS result; // eax@1
	NTSTATUS Status;
	wchar_t *v2 = NULL; // eax@6
	const WCHAR *i; // eax@7
	ULONG ResultLength; // [sp+10h] [bp-88h]@2
	OBJECT_ATTRIBUTES ObjectAttributes; // [sp+14h] [bp-84h]@1
	ULONG Value; // [sp+2Ch] [bp-6Ch]@12
	UNICODE_STRING DestinationString; // [sp+30h] [bp-68h]@1

	UNICODE_STRING ValueName; // [sp+38h] [bp-60h]@1
	HANDLE KeyHandle; // [sp+40h] [bp-58h]@1
	PKEY_VALUE_PARTIAL_INFORMATION  KeyValueInformation; // [sp+44h] [bp-54h]@2
	int v11 = 1; // [sp+48h] [bp-50h]@4
	int v12 = 1; // [sp+4Ch] [bp-4Ch]@5
	wchar_t v13[20] = {0}; // [sp+50h] [bp-48h]@6
	__int16 v14; // [sp+10Ah] [bp+72h]@4

	ValueName.Length = 20;
	ValueName.MaximumLength = 22;
	ValueName.Buffer = L"CSDVersion";
	RtlInitUnicodeString(&DestinationString, L"\\Registry\\Machine\\Software\\Microsoft\\Windows NT\\CurrentVersion");


	ObjectAttributes.ObjectName = &DestinationString;
	ObjectAttributes.Length = 24;
	ObjectAttributes.RootDirectory = 0;
	ObjectAttributes.Attributes = 576;
	ObjectAttributes.SecurityDescriptor = 0;
	ObjectAttributes.SecurityQualityOfService = 0;


	result = ZwOpenKey(&KeyHandle, 0xF003Fu, &ObjectAttributes);
	if ( result >= 0 )
	{

		if ((Status = ZwQueryValueKey(KeyHandle, &ValueName, KeyValuePartialInformation, NULL, 0, &ResultLength)) != STATUS_OBJECT_NAME_NOT_FOUND)
		{
			if ((Status != STATUS_SUCCESS) && 
				(Status != STATUS_BUFFER_OVERFLOW) && 
				(Status != STATUS_BUFFER_TOO_SMALL))
			{
				ZwClose(KeyHandle);
				return STATUS_UNSUCCESSFUL;
			}
			KeyValueInformation = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(NonPagedPool, ResultLength);
			if (KeyValueInformation)
			{
				memset(KeyValueInformation, 0, ResultLength);
				Status = ZwQueryValueKey(
					KeyHandle, 
					&ValueName, 
					KeyValuePartialInformation,
					KeyValueInformation,
					ResultLength, 
					&ResultLength
					);
				ZwClose(KeyHandle);
				if (NT_SUCCESS(Status))
				{
					if (MmIsAddressValid(KeyValueInformation->Data) &&
						KeyValueInformation->DataLength > 0)
					{
						memcpy((PVOID)v13,KeyValueInformation->Data,KeyValueInformation->DataLength);
					}

					v14 = 0;
					if ( v11 == 1 && v12 && (_wcslwr(v13), (v2 = wcsstr(v13, L"service pack")) != 0) )
					{
						for ( i = v2 + 12; *i == 32; ++i )
							;
						if ( *i )
						{
							RtlInitUnicodeString(&DestinationString, i);
							result = RtlUnicodeStringToInteger(&DestinationString, 0xAu, &Value);
							if ( result >= 0 )
								ServicePackNumber = Value;
						}
						else
						{
							result = 0;
						}
					}
					else
					{
						result = -1073741823;
					}


				}
				else
				{
					DbgPrint("%d\r\n",Status);
					result = Status;
				}

				ExFreePool(KeyValueInformation);
			}

		}
	}


	return result;
}


signed int sub_423FC()
{
	BOOLEAN v1;
	char v2;
	ULONG DataLength = 0;
	if(VersionNumber!=7 && VersionNumber !=8 && VersionNumber!=9 && VersionNumber!=10)
	{
		return 1;
	}
	if(!Asm_4211A((ULONG)&v2,(ULONG)&DataLength))
		return 0;

	if(VersionNumber == 9)
	{
		if(_BuildNumber==9879)
		{
			return 1;
		}
		v1 = _BuildNumber == 9926;
	}
	else
	{
		if(VersionNumber != 10)
		{
			if(DataLength > 0x4287
				&& DataLength != 17090
				&& DataLength != 17114
				&& DataLength != 17200
				&& DataLength != 17250
				&& (DataLength<=0x4362 || (Asm_42363() & 0x80000000)!=0))
			{
				return 0;
			}
			return 1;
		}
		v1 = _BuildNumber == 10240;
	}
	if(!v1)
	{
		return Asm_42363()>0;
	}
	return 1;

}


int  
	Asm_4211A(PVOID DataEntry,ULONG DataLength)
{
	PVOID Address;
	int Result;

	Address = (PVOID)(BaseAddress1 + 0x3211A);

	_asm
	{
		push DataEntry
		push DataLength
		call Address;
		mov Result,eax;
	}

	return Result;
}

unsigned int Asm_42363()
{
	PVOID Address;
	unsigned int Result;

	Address = (PVOID)(BaseAddress1 + 0x32363);

	_asm
	{
		call Address;
		mov Result,eax;
	}

	return Result;
}
/*

int __stdcall sub_4211A(PIMAGE_RESOURCE_DATA_ENTRY DataEntry, ULONG DataLength)
{
	PIMAGE_RESOURCE_DATA_ENTRY v2 = DataEntry; // edi@1
	ULONG *v3; // esi@2
	int v4; // eax@4
	int v5; // ST0C_4@4
	ULONG *v6; // eax@8
	char v8; // [sp+8h] [bp-30h]@5
	int v9 = 16; // [sp+20h] [bp-18h]@1
	int v10 = 1; // [sp+24h] [bp-14h]@1
	int v11 = 0; // [sp+28h] [bp-10h]@1
	PVOID Data; // [sp+2Ch] [bp-Ch]@6
	int v13 =0; // [sp+30h] [bp-8h]@1
	PVOID Object = NULL; // [sp+34h] [bp-4h]@1


	if ( DataEntry )
	{
		v3 = DataLength;
		if ( DataLength )
		{
			*(ULONG*)DataLength = 0;
			v2->OffsetToData = 0;
			if ( g_Win32kBaseAddress )
			{
				v4 = sub_2DF43(&Object);
				if ( PsLookupProcessByProcessId(v4, v5) >= 0 )
				{
					KeStackAttachProcess(Object, &v8);
					if ( LdrFindResource_U(g_Win32kBaseAddress, &v9, 3, &DataEntry) >= 0
						&& LdrAccessResource(g_Win32kBaseAddress, DataEntry, &Data, &DataLength) >= 0
						&& DataLength >= 0x5C )
					{
						v6 = Data;
						if ( *Data )
						{
							if ( *(Data + 1) && *(Data + 10) == -17890115 )
							{
								v2->OffsetToData = *(Data + 27);
								*v3 = v6[26];
								v13 = 1;
							}
						}
					}
					KeUnstackDetachProcess(&v8);
				}
				if ( Object )
					ObfDereferenceObject(Object);
			}
		}
	}
	return v13;
}

signed int sub_42363()
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL; // ebx@1
	PVOID MapAddress; // edi@1
	char **i; // esi@5
	unsigned int v3; // eax@9
	int v5 = 0; // [sp+8h] [bp-8h]@1
	int v6 = 0; // [sp+Ch] [bp-4h]@1

	MapAddress = _CreateFileMap(L"\\SystemRoot\\System32\\user32.dll", &v6, &v5);
	if ( MapAddress )
	{
		if ( VersionNumber == 7 || VersionNumber == 8 )
		{
LABEL_14:
			//for ( i = &off_497C8; *i; i += 2 )
			//{
			//	v3 = _GetFuncAddressWithFuncNameFromExport(MapAddress, *i);
				if ( !v3 || v3 < MapAddress || v3 > MapAddress + v6 || i[1] != sub_42308(v3) )
					goto LABEL_16;
			//}
			Status = 0;
		}
		else
		{
			if ( VersionNumber == 9 )
			{
				i = &off_49888;
				goto LABEL_14;
			}
			if ( VersionNumber == 10 )
			{
				i = &off_49948;
				goto LABEL_14;
			}
		}
LABEL_16:
		ZwUnmapViewOfSection(0xFFFFFFFF, v1);
	}
	return v0;
}

char *__stdcall _GetFuncAddressWithFuncNameFromExport(PVOID ImageBase, int a2)
{   
	int v2 =0; // esi@1
	PIMAGE_NT_HEADERS HtHeaders; // eax@1
	ULONG ExportVirtualAddress; // edi@2
	ULONG ExportSize; // eax@2
	PIMAGE_EXPORT_DIRECTORY ExportTable; // edi@4
	ULONG* OrdinalsOfArrayRVA ; // eax@5
	int v8; // ecx@5
	int v9; // eax@7
	unsigned int v10; // eax@14
	char *result; // eax@15
	int v12 = 0; // [sp+1Ch] [bp-24h]@1
	 ULONG* NameOfArray;
	 ULONG* FuncAddressOfArray;


	HtHeaders = RtlImageNtHeader(ImageBase);
	if ( !HtHeaders )
		goto LABEL_19;
	ExportVirtualAddress = HtHeaders->OptionalHeader.DataDirectory[0].VirtualAddress;
	ExportSize = HtHeaders->OptionalHeader.DataDirectory[0].Size;
	if ( !ExportVirtualAddress )
		goto LABEL_19;
	if ( !ExportSize )
		goto LABEL_19;
	ExportTable = (PIMAGE_EXPORT_DIRECTORY)((ULONG)ImageBase + ExportVirtualAddress);
	if ( !ExportTable )
		goto LABEL_19;
	OrdinalsOfArrayRVA  = (ULONG*)((ULONG)ImageBase + ExportTable->AddressOfNameOrdinals);
	v8 = ExportTable->NumberOfNames - 1;
	NameOfArray = (ULONG*)((ULONG)ImageBase + ExportTable->AddressOfNames);
	while ( v8 >= v12 )
	{
		v2 = (v8 + v12) >> 1;
		v9 = strcmp((char*)a2, (char*)((ULONG)ImageBase+NameOfArray[v2]));
		if ( v9 >= 0 )
		{
			if ( v9 <= 0 )
			{
				OrdinalsOfArrayRVA  = (ULONG*)((ULONG)ImageBase + ExportTable->AddressOfNameOrdinals);
				break;
			}
			v12 = v2 + 1;
		}
		else
		{
			v8 = v2 - 1;
		}
		OrdinalsOfArrayRVA  = (ULONG*)((ULONG)ImageBase + ExportTable->AddressOfNameOrdinals);
	}
	if ( v8 < v12 )
		goto LABEL_19;
	v10 = OrdinalsOfArrayRVA[2 * v2];
	FuncAddressOfArray = (ULONG*)((ULONG)ImageBase + ExportTable->AddressOfFunctions);
	if ( v10 <= ExportTable->NumberOfFunctions )
		result = (char*)((ULONG)ImageBase + FuncAddressOfArray[v10]);
	else
LABEL_19:
	result = 0;
	return result;
}

int __stdcall sub_42308(int a1)
{
	int v1; // eax@3
	int result; // eax@4

	if ( a1 && *a1 == -72 && (v1 = *(a1 + 1), v1 & 0x1000) )
		result = *(a1 + 1) & 0xFFF;
	else
		result = 0;
	return result;
}

*/