#include "StdAfx.h"
#include "PECheck.h"




BOOL
	LoadFileData(WCHAR* wzFilePath,CHAR** szFileData,ULONG* ulLow)
{

	HANDLE hFile;
	ULONG ulHigh = 0;
	ULONG ulReturn = 0;

	if (wzFilePath==NULL)
	{
		return FALSE;
	}

	hFile = CreateFileW(wzFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile==INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	*ulLow = GetFileSize(hFile,&ulHigh); 

	*szFileData = new char[*ulLow + 20];

	if (ReadFile(hFile,*szFileData,*ulLow,&ulReturn,NULL)==0)
	{
		CloseHandle(hFile);
		delete *szFileData;
		return FALSE;
	}


	return TRUE;
}

BOOL
	IsPEFile(CHAR* szFileData,PETYPE* PeType)
{
	PIMAGE_DOS_HEADER	DosHead =NULL;
	PIMAGE_NT_HEADERS	NtHead = NULL;
	IMAGE_FILE_HEADER	FileHead = {0};

	DosHead = (PIMAGE_DOS_HEADER)szFileData;
	if (DosHead->e_magic!=IMAGE_DOS_SIGNATURE) //MZ
	{
		return FALSE;
	}

	NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

	if (NtHead->Signature!=IMAGE_NT_SIGNATURE) //PE
	{
		return FALSE;
	}

	FileHead = NtHead->FileHeader;
	switch(FileHead.Machine)   //根据NT头中的FileHeader.Machine判断是哪种PE文件
	{
	case IMAGE_FILE_MACHINE_IA64:
	case IMAGE_FILE_MACHINE_AMD64:
		*PeType = PE64;
		break;

	case IMAGE_FILE_MACHINE_I386:
		*PeType = PE;
		break;

	default: *PeType = Unkonw;
	}

	return TRUE;


}


ULONG GetOEP(CHAR* szFileData)
{
	PIMAGE_DOS_HEADER				DosHead;  //ReadFile()返回的szFileData地址
	PIMAGE_NT_HEADERS				NtHead = NULL;
	IMAGE_FILE_HEADER				FileHead = {0};
	ULONG							FileKind = 0;
	PIMAGE_NT_HEADERS64				NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32				NtHeader32 = NULL;
	IMAGE_OPTIONAL_HEADER32			OptionHead32;
	//IMAGE_OPTIONAL_HEADER64		    OptionHead64;

	if (szFileData==NULL)
	{
		return 0;
	}
	DosHead = (PIMAGE_DOS_HEADER)szFileData;
	NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

	NtHeader32 = (PIMAGE_NT_HEADERS32)NtHead;
	OptionHead32 = NtHeader32->OptionalHeader;
	return OptionHead32.AddressOfEntryPoint;

}



ULONG GetBase(CHAR* szFileData)
{
	PIMAGE_DOS_HEADER				DosHead;  //ReadFile()返回的szFileData地址
	PIMAGE_NT_HEADERS				NtHead = NULL;
	IMAGE_FILE_HEADER				FileHead = {0};
	ULONG							FileKind = 0;
	PIMAGE_NT_HEADERS64				NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32				NtHeader32 = NULL;
	IMAGE_OPTIONAL_HEADER32			OptionHead32;
	IMAGE_OPTIONAL_HEADER64		    OptionHead64;

	if (szFileData==NULL)
	{
		return 0;
	}
	DosHead = (PIMAGE_DOS_HEADER)szFileData;
	NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

	NtHeader32 = (PIMAGE_NT_HEADERS32)NtHead;
	OptionHead32 = NtHeader32->OptionalHeader;
	return OptionHead32.ImageBase;

}