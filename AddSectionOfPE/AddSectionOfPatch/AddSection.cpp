//
//   生成的程序  原理上没什么问题   在Win7只能弹出Box XP无法执行   用PE Tool 执行Rebuild PE之后就可以正常运行  不懂诶(⊙_⊙)
//
//Wipe relocation...No relocation found！
//Rebuilding...Done!
//Validate PE Image...Done!
//
//
#include "StdAfx.h"
#include "AddSection.h"
#include "AddSectionOfPatchDlg.h"


extern CAddSectionOfPatchDlg* g_Dlg;




void Main()
{
	//读文件
	FILEDATA PatchFile = {0};
	FILEDATA DestFile = {0};
	BOOL bOk = GetFileMess(&PatchFile,L"Patch.exe");
	if(bOk==TRUE)
	{
		CString mz;
		mz.Format(L"补丁程序大小为:%d",PatchFile.ulFileSize);
		g_Dlg->AddListInfo(mz);
	}
	else
	{
		return;
	}
	bOk = GetFileMess(&DestFile,L"Notepad.exe");
	if(bOk==TRUE)
	{
		CString mz;
		mz.Format(L"目标程序大小为:%d",DestFile.ulFileSize);
		g_Dlg->AddListInfo(mz);
	}
	else
	{
		return;
	}


	//获取原文件PE头有PE长度
	DWORD	dwValidHeaderSize	=	GetValidHeadSize(DestFile.lpFileData);

	if (dwValidHeaderSize==0)
	{
		return;
	}

	//将最后文件头按文件对齐
	DWORD dwFileAlign	=	0;
	DWORD dwNewFileHeaderSize	=	0;
	dwFileAlign	=	GetFileAlign(DestFile.lpFileData);
	dwNewFileHeaderSize	=	((dwValidHeaderSize+2*sizeof(IMAGE_SECTION_HEADER)+dwFileAlign)/dwFileAlign)*dwFileAlign; //对齐以后的文件头大小

	CString str;
	str.Format(L"对齐以后的New文件头大小 ： 0x%p",dwNewFileHeaderSize);
	g_Dlg->AddListInfo(str);


	//获得目标补丁大小  196h   对齐后200h
	DWORD dwPatchCodeSize = GetCodeSize(PatchFile.lpFileData);
	//对齐后新节大小
	DWORD dwNewSectionSize
		=	((dwPatchCodeSize+dwFileAlign)/dwFileAlign)*dwFileAlign; 


	str.Format(L"对齐后PatchCodeSize ： 0x%p",dwNewSectionSize);
	g_Dlg->AddListInfo(str);



	//求新文件的大小
	DWORD	dwNewFileSize	=	0;
	DWORD	dwOff			=	0;
	PIMAGE_SECTION_HEADER DestSectionHeader;
	PIMAGE_DOS_HEADER	DestDosHeader	=	(PIMAGE_DOS_HEADER)DestFile.lpFileData;
	PIMAGE_NT_HEADERS	DestNtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DestDosHeader+(ULONG_PTR)DestDosHeader->e_lfanew);

	DestSectionHeader	=	(PIMAGE_SECTION_HEADER)((ULONG_PTR)DestNtHeader+sizeof(IMAGE_NT_HEADERS));
	//文件多出的部分 dwOff = 0
	dwOff	=		dwNewFileHeaderSize - DestSectionHeader[0].PointerToRawData;
	//新文件大小 = 原文件大小 + 添加节表对齐后的部分 + 新节对齐后的部分
	dwNewFileSize	=	DestFile.ulFileSize	+	dwOff + dwNewSectionSize;     //加了200
	
	str.Format(L"New文件大小 ： 0x%p",dwNewFileSize);
	g_Dlg->AddListInfo(str);


	//申请内存
	FILEDATA	NewFile	=	{0};
	wcscpy(NewFile.wzFilePath,L"NewFile.exe");

	NewFile.ulFileSize	=	dwNewFileSize;
	NewFile.lpFileData	=	(BYTE*)malloc(dwNewFileSize);

	if (NewFile.lpFileData==NULL)
	{
		return;
	}
	memset(NewFile.lpFileData,0,NewFile.ulFileSize);



	//开始拷贝
	DWORD  dwCopySize	=	0;
	memcpy(NewFile.lpFileData,DestFile.lpFileData,dwValidHeaderSize);  //拷贝原始Dos Nt 节表
	dwCopySize	+=	dwNewFileHeaderSize;   //加过对齐后文件头大小  越过文件头
	str.Format(L"将目标文件文件的DOS头部分复制到内存区域 Size： 0x%p",dwValidHeaderSize);
	g_Dlg->AddListInfo(str);

	//复制节的详细内容
	DWORD dwFirstSectionRaw	=	DestSectionHeader[0].PointerToRawData;  //从第一个节开始的之后所有东西拷贝
	DWORD dwNeedCopy			=	DestFile.ulFileSize - dwFirstSectionRaw;

	memcpy((PVOID)((DWORD)(NewFile.lpFileData)+dwNewFileHeaderSize),(PVOID)((DWORD)DestFile.lpFileData+dwFirstSectionRaw),dwNeedCopy);
	dwCopySize	+=	dwNeedCopy;
	str.Format(L"复制节的详细内容 Size： 0x%p",dwNeedCopy);
	g_Dlg->AddListInfo(str);





	DWORD dwOldEntryPoint = DestNtHeader->OptionalHeader.AddressOfEntryPoint;   //旧的EntryPoint
	DWORD OldSectionNum = DestNtHeader->FileHeader.NumberOfSections;

	PIMAGE_SECTION_HEADER FirstSection = (PIMAGE_SECTION_HEADER)((ULONG_PTR)DestNtHeader+sizeof(IMAGE_NT_HEADERS));
//最后一个节表
	PIMAGE_SECTION_HEADER LastSection = (PIMAGE_SECTION_HEADER)((ULONG_PTR)FirstSection+(OldSectionNum-1)*sizeof(IMAGE_SECTION_HEADER));

//越过最后一个节表所得的地址 10600
	DWORD LastSectionPointRaw = 
		((LastSection->PointerToRawData+LastSection->SizeOfRawData)/dwFileAlign)*dwFileAlign; //对齐以后的文件头大小
	               //8400                       8000

	//新增节
	DWORD dwPatchCodeSegStart	=	0;
	dwPatchCodeSegStart	=	GetPatchCodeSegStart(PatchFile.lpFileData);		//获得Code的起始地址

	memcpy((PVOID)(LastSectionPointRaw+(ULONG_PTR)NewFile.lpFileData),(PVOID)((DWORD)PatchFile.lpFileData+dwPatchCodeSegStart),dwPatchCodeSize);
	dwCopySize	+= dwPatchCodeSize;


	str.Format(L"复制补丁代码 Size： 0x%p",dwPatchCodeSize);
	g_Dlg->AddListInfo(str);

	//复制完毕，修改新加表头




	IMAGE_SECTION_HEADER SectionNew = {0};
	strcpy((char*)SectionNew.Name,".mz");             //节名称
	SectionNew.Misc.VirtualSize = dwPatchCodeSize;    //实际大小
	SectionNew.SizeOfRawData =dwNewSectionSize; // 0x19A;      //文件对齐后大小   dwNewSectionSize
	SectionNew.PointerToRawData = LastSectionPointRaw;//节的文件偏移
	SectionNew.Characteristics = (DWORD)0x60000020  ; //节页面的属性  可读可写可执行   
	SectionNew.VirtualAddress = DestNtHeader->OptionalHeader.SizeOfImage;   //节在内存中加载地址  
	
	

	
	
	PIMAGE_DOS_HEADER	NewDosHeader	=	(PIMAGE_DOS_HEADER)(NewFile.lpFileData);
	PIMAGE_NT_HEADERS	NewNtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)NewDosHeader+(ULONG_PTR)NewDosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER NewSection = (PIMAGE_SECTION_HEADER)((ULONG_PTR)NewNtHeader+sizeof(IMAGE_NT_HEADERS));


	//程序入口  RVA
	NewNtHeader->OptionalHeader.AddressOfEntryPoint = DestNtHeader->OptionalHeader.SizeOfImage;
	//节数量
	NewNtHeader->FileHeader.NumberOfSections++;
	//文件头大小
	NewNtHeader->OptionalHeader.SizeOfHeaders = dwNewFileHeaderSize;
	//文件在内存中大小
	NewNtHeader->OptionalHeader.SizeOfImage = DestNtHeader->OptionalHeader.SizeOfImage+ dwPatchCodeSize; //DestNtHeader->OptionalHeader.SectionAlignment;

	PIMAGE_SECTION_HEADER NewFirstSection = (PIMAGE_SECTION_HEADER)((ULONG_PTR)NewNtHeader+sizeof(IMAGE_NT_HEADERS));
	PIMAGE_SECTION_HEADER NewLastSection = (PIMAGE_SECTION_HEADER)((ULONG_PTR)NewFirstSection+(OldSectionNum)*sizeof(IMAGE_SECTION_HEADER));


	//将节头信息拷入
	memcpy(NewLastSection,&SectionNew,sizeof(IMAGE_SECTION_HEADER));


	//返回真正OEP
	DWORD dwJmpAddr	=	(DWORD)NewFile.lpFileData+LastSectionPointRaw+dwPatchCodeSize -5;
	DWORD dwJmpOff	=	dwOldEntryPoint	-	(DestNtHeader->OptionalHeader.SizeOfImage+dwPatchCodeSize -5 +4);
	*(DWORD*)dwJmpAddr		=	dwJmpOff;

	str.Format(L"修正补丁代码E9之后的操作数   ： 0x%p",dwJmpOff);
	g_Dlg->AddListInfo(str);


	
	RemoveIatDirectory((ULONG_PTR)NewFile.lpFileData);
	str.Format(L"移除导入表成功！");
	g_Dlg->AddListInfo(str);

	if (IcWriteFile(&NewFile))
	{
		str	=	L"生成文件 :  ";
		str	+=	NewFile.wzFilePath;
		g_Dlg->AddListInfo(str);
	}


	if (PatchFile.lpFileData!=NULL)
	{
		free(PatchFile.lpFileData);
		PatchFile.lpFileData	=	NULL;
	}
	if (DestFile.lpFileData!=NULL)
	{
		free(DestFile.lpFileData);
		DestFile.lpFileData	=	NULL;
	}
	if (NewFile.lpFileData!=NULL)
	{
		free(NewFile.lpFileData);
		NewFile.lpFileData	=	NULL;

	}

}
void RemoveIatDirectory(ULONG_PTR lpData)
{
		DWORD SearchAddress = 0;


		PIMAGE_DOS_HEADER DosHeader = (PIMAGE_DOS_HEADER)lpData;
		PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)((ULONG_PTR)lpData + DosHeader->e_lfanew);

		SearchAddress = NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;

		NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
		NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;

	//	NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress =0;
	//	NtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size = 0;
	
		PIMAGE_SECTION_HEADER SectionHeader = (PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader  + sizeof(IMAGE_NT_HEADERS)) ;
		DWORD	dwNumberOfSection				=	NtHeader->FileHeader.NumberOfSections;

	
	if (SearchAddress)
	{
		for (WORD i = 0; i < dwNumberOfSection; i++)
		{
			if (SectionHeader[i].VirtualAddress <= SearchAddress && (SectionHeader[i].VirtualAddress + SectionHeader[i].Misc.VirtualSize) > SearchAddress)
			{
				//section must be read and writable
				SectionHeader[i].Characteristics |= IMAGE_SCN_MEM_READ|IMAGE_SCN_MEM_WRITE;
			}
		}
	}
}


BOOL IcWriteFile(PFILEDATA FileData)
{
	HANDLE		hFile		=	NULL;

	hFile = CreateFile(FileData->wzFilePath,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile==INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}


	if (WriteFile(hFile,FileData->lpFileData,FileData->ulFileSize,&FileData->ulFileSize,NULL))
	{
		CloseHandle(hFile);


		return TRUE;

	}
	return FALSE;

}
//获得文件FileAlign
DWORD GetFileAlign(PVOID lpData)
{
	DWORD dwRet	=	0;
	PIMAGE_DOS_HEADER	DosHeader	=	(PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS	NtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader+(ULONG_PTR)DosHeader->e_lfanew);


	dwRet	=	NtHeader->OptionalHeader.FileAlignment;



	return dwRet;
}

//获得patchcodestart地址
DWORD GetPatchCodeSegStart(PVOID lpData)
{
	DWORD dwRet	=	0;

	PIMAGE_DOS_HEADER	DosHeader	=	(PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS	NtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader+(ULONG_PTR)DosHeader->e_lfanew);


	dwRet	=	NtHeader->OptionalHeader.AddressOfEntryPoint;
	dwRet	=	GetRVASectionStart(lpData,dwRet);




	return dwRet;
}
//获取RVA所在节的文件起始地址
DWORD GetRVASectionStart(PVOID lpData,ULONG_PTR ulAddressofEntryPoint)
{
	DWORD dwStart =	0;
	DWORD i		 =	0;

	PIMAGE_DOS_HEADER	DosHeader	=	(PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS	NtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader+(ULONG_PTR)DosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER	SectionHeader	=	(PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader+sizeof(IMAGE_NT_HEADERS));
	DWORD	dwNumberOfSection				=	NtHeader->FileHeader.NumberOfSections;


	//遍历节表 


	for (i=0;i<dwNumberOfSection;i++)
	{

		if (ulAddressofEntryPoint>=SectionHeader[i].VirtualAddress
			&&ulAddressofEntryPoint<SectionHeader[i].VirtualAddress+SectionHeader[i].SizeOfRawData)
		{
			//Found
			dwStart	=	SectionHeader[i].PointerToRawData;

			break;

		}


	}

	return dwStart;
}

WORD GetCodeSize(PVOID BaseAddress)
{
	DWORD dwSize	=	0;


	PIMAGE_DOS_HEADER	DosHeader	=	(PIMAGE_DOS_HEADER)BaseAddress;
	PIMAGE_NT_HEADERS	NtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader+(ULONG_PTR)DosHeader->e_lfanew);


	ULONG_PTR	ulAddressOfEntryPoint	=	NtHeader->OptionalHeader.AddressOfEntryPoint;


	dwSize	=	GetRVASectionSize(BaseAddress,ulAddressOfEntryPoint);

	return	dwSize;
}
//获取RVA所在节的原始大小
DWORD GetRVASectionSize(PVOID BaseAddress,ULONG_PTR ulAddressofEntryPoint)
{
	DWORD dwSize =	0;
	DWORD i		 =	0;

	PIMAGE_DOS_HEADER	DosHeader	=	(PIMAGE_DOS_HEADER)BaseAddress;
	PIMAGE_NT_HEADERS	NtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader+(ULONG_PTR)DosHeader->e_lfanew);
	PIMAGE_SECTION_HEADER	SectionHeader	=	(PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader+sizeof(IMAGE_NT_HEADERS));
	DWORD	dwNumberOfSection				=	NtHeader->FileHeader.NumberOfSections;


	//遍历节表 


	for (i=0;i<dwNumberOfSection;i++)
	{

		if (ulAddressofEntryPoint>=SectionHeader[i].VirtualAddress
			&&ulAddressofEntryPoint<SectionHeader[i].VirtualAddress+SectionHeader[i].SizeOfRawData)
		{
			//Found
			dwSize	=	SectionHeader[i].Misc.VirtualSize;

			break;

		}


	}

	return dwSize;
}


DWORD GetValidHeadSize(PVOID BaseAddress)
{
	DWORD	dwRet	=	0;
	PIMAGE_DOS_HEADER	DosHeader	=	(PIMAGE_DOS_HEADER)BaseAddress;
	PIMAGE_NT_HEADERS	NtHeader	=	(PIMAGE_NT_HEADERS)((ULONG_PTR)DosHeader+(ULONG_PTR)DosHeader->e_lfanew);

	PIMAGE_SECTION_HEADER	SectionHeader	=	(PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader+sizeof(IMAGE_NT_HEADERS));

	DWORD	dwFirstSectionRaw	=	SectionHeader[0].PointerToRawData;

	DWORD	dwTemp	=dwFirstSectionRaw	-	1;                                                                                                                                                             
	DWORD	i	=	0;


	for(i=0;i<dwFirstSectionRaw;i++)
	{
		if (((PBYTE)BaseAddress)[dwTemp-i]!=0)
		{

			dwRet	=	dwFirstSectionRaw - i + 2;		//为有效数字保留两个字符零
			return dwRet;	
		}

	}
	return 0;


}

BOOL GetFileMess(PFILEDATA FileData,CString FileName)
{
	//获得文件句柄
	HANDLE hFile =
	CreateFile(FileName,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_WRITE|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(hFile==INVALID_HANDLE_VALUE)
	{
		g_Dlg->AddListInfo(L"打开文件初始化失败...");
		return FALSE;
	}

	//获得文件大小
	DWORD dwHight  =0;
	DWORD   dwLow = GetFileSize(hFile,&dwHight);
	if(dwLow==0)
	{
		CloseHandle(hFile);
		g_Dlg->AddListInfo(L"获取文件大小失败...");
		return FALSE;
	}


	wsprintf(FileData->wzFilePath,L"s",FileName);
	FileData->ulFileSize	=	dwLow;
	//读取文件
	FileData->lpFileData	=	(BYTE*)malloc(dwLow);
	if (FileData->lpFileData==NULL)
	{
		return FALSE;

	}
	memset(FileData->lpFileData,0,dwLow);

	DWORD dwReadReturn = 0;
	BOOL bOk = ReadFile(hFile,FileData->lpFileData,dwLow,&dwLow,NULL);

	if (bOk==FALSE)
	{
		CloseHandle(hFile);
		return FALSE;
	}


	return TRUE;
	
}

ULONG_PTR RVAToOffset(PIMAGE_NT_HEADERS NTHeader, ULONG_PTR ulRVA)   
{
    PIMAGE_SECTION_HEADER SectionHeader =
        (PIMAGE_SECTION_HEADER)((ULONG_PTR)NTHeader + sizeof(IMAGE_NT_HEADERS));  //获得节表  
    
    for(int i = 0; i < NTHeader->FileHeader.NumberOfSections; i++)
    {
        //查询导出表是属于哪个节的 
        /***********************************************************************
            SectionHeader[i].VirtualAddress    节起始的RVA  0x1000
            SectionHeader[i].SizeOfRawData     节在文件上的大小
            SectionHeader[i].PointerToRawData  这是节基于文件的偏移量，PE 装载器通过本域值找到节数据在文件中的位置
            假如导出表在.txt节中
            SectionHeader[i].PointerToRawData == 0x200       SectionHeader[i].VirtualAddress == 0x1000    
                                                             ulRVA = 0x1030
                                                             
            那么导出表在文件中的偏移就是0x230  返回
        ***********************************************************************/
        if(ulRVA >= SectionHeader[i].VirtualAddress && ulRVA < 
            (SectionHeader[i].VirtualAddress 
            + SectionHeader[i].SizeOfRawData))
        {
            //文件偏移
            return SectionHeader[i].PointerToRawData + 
                (ulRVA - SectionHeader[i].VirtualAddress);
        }
    }
    
    return 0;
}
