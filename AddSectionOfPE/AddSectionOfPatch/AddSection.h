#pragma once

typedef struct _FILEDATA_
{
	WCHAR wzFilePath[MAX_PATH];
	ULONG ulFileSize;
	BYTE* lpFileData;

}FILEDATA,*PFILEDATA;

void Main();
BOOL GetFileMess(PFILEDATA FileData,CString FileName);
DWORD GetValidHeadSize(PVOID BaseAddress);
WORD GetCodeSize(PVOID BaseAddress);
DWORD GetRVASectionSize(PVOID BaseAddress,ULONG_PTR ulAddressofEntryPoint);
DWORD GetFileAlign(PVOID lpData);
DWORD GetPatchCodeSegStart(PVOID lpData);
DWORD GetRVASectionStart(PVOID lpData,ULONG_PTR ulAddressofEntryPoint);
BOOL IcWriteFile(PFILEDATA FileData);
void RemoveIatDirectory(ULONG_PTR lpData);
ULONG_PTR RVAToOffset(PIMAGE_NT_HEADERS NTHeader, ULONG_PTR ulRVA) ;