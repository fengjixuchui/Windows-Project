// PETableDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PEAnalysis.h"
#include "PETableDlg.h"
#include "afxdialogex.h"
#include <strsafe.h>
#define MakePtr(a,b,c) ((a)((char*)b+c))

extern
	ULONG		g_SelectTab;
extern
	int dpix;

UINT g_Column_Count_DirTable  = 5;	
COLUMNSTRUCT g_Column_DirTableList[] =
{
	{ L"Name",			180 },	//ldr
	{ L"RVA",			150 },	//传递
	{ L"Size",			150 },	//
	{L"RAW",		150},
	{L"所属区块",	100}
};

UINT g_Column_Count_SectionTable  = 5;	
COLUMNSTRUCT g_Column_SectionTableList[] =
{
	{ L"Name",			180 },	//ldr
	{ L"RVA",			150 },	//传递
	{ L"内存区块大小(未对齐)",			150 },	//
	{L"文件偏移(offset)",			150},
	{L"文件大小",			150}
};

UINT g_Column_Count_ImportTable  = 4;	
COLUMNSTRUCT g_Column_ImportTableList[] =
{
	{ L"函数序号",			150 },	//ldr
	{ L"函数名",			180 },	//传递
	{ L"函数地址",			150 },	//
	{L"导入模块",150}
};

UINT g_Column_Count_ExportTable  = 5;	
COLUMNSTRUCT g_Column_ExportTableList[] =
{
	{ L"函数序号",			150 },	//ldr
	{ L"函数名",			180 },	//传递
	{ L"函数地址",			150 },	//
	{L"RVA",			150},
	{L"Offset",			150}
};

UINT g_Column_Count_RelocTable  = 7;	
COLUMNSTRUCT g_Column_RelocTableList[] =
{
	{ L"重定位表RVA",			150 },	//ldr
	{L"重定位表Offset",100},
	{ L"重定位结构大小",			180 },	//传递
	{ L"TypeOffset",			150 },
	{L"重定位项RVA",150},
	{L"重定位项Offset",100},
	{L"需要重定位的数据",150}
};


WCHAR wzDirTable[15][64] = {L"导出表地址",L"导入名称表",L"资源表",L"异常表",L"证书",L"重定向表",L"调试",L"版权",L"全局指针",L"LTS",L"加载配置",L"绑定导入表",L"导入地址表(IAT)",L"延迟加载导入表",L".NET 运行时"};

// CPETableDlg 对话框

IMPLEMENT_DYNAMIC(CPETableDlg, CDialog)

CPETableDlg::CPETableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPETableDlg::IDD, pParent)
	, m_GroupStatic(_T(""))
	, m_Table1(_T(""))
	, m_Table2(_T(""))
	, m_Table3(_T(""))
	, m_Table4(_T(""))
	, m_EditTable1(_T(""))
	, m_EditTable2(_T(""))
	, m_EditTable3(_T(""))
	, m_EditTable4(_T(""))
{

}

CPETableDlg::~CPETableDlg()
{
}

void CPETableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TABLE, m_ListTable);
	DDX_Text(pDX, IDC_STATIC_TABLE, m_GroupStatic);
	DDX_Text(pDX, IDC_STATIC_TABLE1, m_Table1);
	DDX_Text(pDX, IDC_STATIC_TABLE2, m_Table2);
	DDX_Text(pDX, IDC_STATIC_TABLE3, m_Table3);
	DDX_Text(pDX, IDC_STATIC_TABLE4, m_Table4);
	DDX_Text(pDX, IDC_EDIT_TABLE1, m_EditTable1);
	DDX_Text(pDX, IDC_EDIT_TABLE2, m_EditTable2);
	DDX_Text(pDX, IDC_EDIT_TABLE3, m_EditTable3);
	DDX_Text(pDX, IDC_EDIT_TABLE4, m_EditTable4);
}


BEGIN_MESSAGE_MAP(CPETableDlg, CDialog)
END_MESSAGE_MAP()


// CPETableDlg 消息处理程序

VOID
	CPETableDlg::ShowPeTable(CHAR* FileData,PETYPE PeType)
{
	UpdateData(TRUE);

	switch(g_SelectTab)
	{
	case 1:
		{
			m_GroupStatic.Format(L"目录表详细信息");
			m_Table1.Format(L"");
			m_Table2.Format(L"");
			m_Table3.Format(L"");
			m_Table4.Format(L"");

			ShowDataDirTable(FileData,PeType);
			break;
		}

	case 2:
		{
			m_GroupStatic.Format(L"区块表详细信息");
			m_Table1.Format(L"");
			m_Table2.Format(L"");
			m_Table3.Format(L"");
			m_Table4.Format(L"");

			ShowSectionTable(FileData,PeType);
			break;
		}

	case 3:
		{
			m_GroupStatic.Format(L"导入表详细信息");
		
			m_Table1.Format(L"输入表RVA");
			m_Table2.Format(L"");
			m_Table3.Format(L"");
			m_Table4.Format(L"");

			ShowImportTable(FileData,PeType);
			break;
		}

	case 4:
		{
			m_GroupStatic.Format(L"导出表详细信息");

			m_Table1.Format(L"模块真实名称");
			m_Table2.Format(L"函数地址数组RVA");
			m_Table3.Format(L"函数名字指针RVA");
			m_Table4.Format(L"导出序号数组RVA");

			m_EditTable1.Format(L"");

			ShowExportTable(FileData,PeType);
			break;
		}

	case 5:
		{
			ShowRelocTable(FileData,PeType);

			break;
		}

	default:
		break;
	}

	UpdateData(FALSE);
}


VOID
	CPETableDlg::ShowDataDirTable(CHAR* szFileData,PETYPE PeType)
{

	PIMAGE_NT_HEADERS32						NtHead = NULL;
	PIMAGE_NT_HEADERS64					NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32					NtHeader32 = NULL;
	IMAGE_OPTIONAL_HEADER32			OptionHead32;
	IMAGE_OPTIONAL_HEADER64			OptionHead64;
	PIMAGE_SECTION_HEADER				SectionHeader = NULL;

	ULONG													DataDirTable = 0;
	ULONG													DataSize = 0;
	ULONG_PTR											DataRaw = 0;

	CString														strRVA;
	CString														strSize;
	CString														strSection;
	CString														strRaw;

	while(m_ListTable.DeleteColumn(0));
	m_ListTable.DeleteAllItems();

	m_ListTable.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	UINT	i = 0;
	for (i = 0;i<g_Column_Count_DirTable;i++)
	{
		m_ListTable.InsertColumn(i, g_Column_DirTableList[i].szTitle,LVCFMT_LEFT,(int)(g_Column_DirTableList[i].nWidth*(dpix/96.0)));
	}

	if (szFileData==NULL)
	{
		return;
	}

	PIMAGE_DOS_HEADER DosHead = (PIMAGE_DOS_HEADER)szFileData;  //ReadFile()返回的szFileData地址
	
	switch(PeType)
	{
	case PE:
		{
			NtHeader32 = (PIMAGE_NT_HEADERS32)((ULONG)szFileData+DosHead->e_lfanew);
			 OptionHead32 = NtHeader32->OptionalHeader;

			 for (int i = 0;i<OptionHead32.NumberOfRvaAndSizes;i++)
			 {
				 DataDirTable = OptionHead32.DataDirectory[i].VirtualAddress;
				 DataSize = OptionHead32.DataDirectory[i].Size;

				 //得到数据块在文件中的偏移
				 DataRaw = RVATwoOffset(NtHeader32,DataDirTable);

				 //计算数据块属于哪个节中
				SectionHeader = GetSectionHeaderFromRva(DataDirTable,(PIMAGE_NT_HEADERS)NtHeader32);

				 strRVA.Format(L"0x%X",DataDirTable);
				 strSize.Format(L"0x%X",DataSize);
				 CHAR szSectionName[50] = {0};
                 HRESULT r = StringCchCopyA(szSectionName, IMAGE_SIZEOF_SHORT_NAME+1, (char*)SectionHeader->Name);

                 strSection = szSectionName;
				 strRaw.Format(L"0x%X",DataRaw);

				 int n = m_ListTable.GetItemCount();
				 int j = m_ListTable.InsertItem(n, wzDirTable[i]);
				 m_ListTable.SetItemText(j, 1, strRVA);
				 m_ListTable.SetItemText(j, 2, strSize);
				 m_ListTable.SetItemText(j,3,strRaw);
				 m_ListTable.SetItemText(j,4,strSection);

			 }
			 

			break;
		}

	case PE64:
		{
			NtHeader64 = (PIMAGE_NT_HEADERS64)((ULONG)szFileData+DosHead->e_lfanew);

			OptionHead64 = NtHeader64->OptionalHeader;

			for (int i = 0;i<OptionHead64.NumberOfRvaAndSizes;i++)
			{
				DataDirTable = OptionHead64.DataDirectory[i].VirtualAddress;
				DataSize = OptionHead64.DataDirectory[i].Size;

				DataRaw = RVATwoOffset64(NtHeader64,DataDirTable);

				SectionHeader = GetSectionHeaderFromRva(DataDirTable,(PIMAGE_NT_HEADERS)NtHeader64);

				strRVA.Format(L"0x%X",DataDirTable);
				strSize.Format(L"0x%X",DataSize);
				CHAR szSectionName[50] = {0};
                HRESULT r = StringCchCopyA(szSectionName, IMAGE_SIZEOF_SHORT_NAME+1, (char*)SectionHeader->Name);

                strSection = szSectionName;
				strRaw.Format(L"0x%X",DataRaw);

				int n = m_ListTable.GetItemCount();
				int j = m_ListTable.InsertItem(n, wzDirTable[i]);
				m_ListTable.SetItemText(j, 1, strRVA);
				m_ListTable.SetItemText(j, 2, strSize);
				m_ListTable.SetItemText(j,3,strRaw);
				m_ListTable.SetItemText(j,4,strSection);

			}

			break;
		}
	default:
		break;

	}
}


	PIMAGE_SECTION_HEADER
		CPETableDlg::GetSectionHeaderFromRva(ULONG RVA,PIMAGE_NT_HEADERS NtHeader)  //判断RVA是在那个节表当中
	{
		ULONG   i = 0;


		//Nt头得到节表
		PIMAGE_SECTION_HEADER  SectionHeader = IMAGE_FIRST_SECTION(NtHeader);

		//Nt头中的File头中有区块表的个数  区块表是若干SECTION_HEADER个数据结构,个数是在文件头的NumberofSection中
		for (i=0;i<NtHeader->FileHeader.NumberOfSections;i++,SectionHeader++)
		{

			if ((RVA>=SectionHeader->VirtualAddress)&&
				(RVA<(SectionHeader->VirtualAddress + SectionHeader->Misc.VirtualSize)))
			{
				return SectionHeader;
			}
		}

		return NULL;	
	}


VOID
	CPETableDlg::ShowSectionTable(CHAR* szFileData,PETYPE PeType)
{
	PIMAGE_NT_HEADERS						NtHead = NULL;
	PIMAGE_NT_HEADERS64					NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32					NtHeader32 = NULL;

	IMAGE_FILE_HEADER						FileHeader;

	ULONG													DataSize = 0;
	PIMAGE_SECTION_HEADER				SectionHeader;

	CString														SectionName;
	CString														strRVA;
	CString														strRSize;

	CString													strOffset;
	CString													strVsize;

	while(m_ListTable.DeleteColumn(0));
	m_ListTable.DeleteAllItems();

	//CRect rect;
	//GetWindowRect(rect);
	////设置ListContrl在对话框中的位置
	//if (m_ListTable.m_hWnd != NULL)
	//{
	//	CRect rc;
	//	rc.left = 0;
	//	rc.top = 0;
	//	rc.right = rect.Width() - 10;
	//	rc.bottom = rect.Height() - 35;
	//	m_ListTable.MoveWindow(rc);
	//}

	m_ListTable.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	UINT	i = 0;
	for (i = 0;i<g_Column_Count_SectionTable;i++)
	{
		m_ListTable.InsertColumn(i, g_Column_SectionTableList[i].szTitle,LVCFMT_LEFT,(int)(g_Column_SectionTableList[i].nWidth*(dpix/96.0)));
	}

	if (szFileData==NULL)
	{
		return;
	}

	PIMAGE_DOS_HEADER DosHead = (PIMAGE_DOS_HEADER)szFileData;

	//获得节表  
	if (PeType==PE)
	{
		NtHeader32 = (PIMAGE_NT_HEADERS32)((ULONG)szFileData+DosHead->e_lfanew);
		FileHeader = NtHeader32->FileHeader;
		SectionHeader =	(PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader32 + sizeof(IMAGE_NT_HEADERS32));  
	}
	else
	{
		NtHeader64 = (PIMAGE_NT_HEADERS64)((ULONG)szFileData+DosHead->e_lfanew);
		FileHeader =  NtHeader64->FileHeader;

		SectionHeader = (PIMAGE_SECTION_HEADER)((ULONG_PTR)NtHeader64 + sizeof(IMAGE_NT_HEADERS64));  
	}
		

	for (int i = 0;i<FileHeader.NumberOfSections;i++,SectionHeader++)
	{
		CHAR szSectionName[50] = {0};
        HRESULT r = StringCchCopyA(szSectionName, IMAGE_SIZEOF_SHORT_NAME+1, (char*)SectionHeader->Name);

        SectionName = szSectionName;
		strRVA.Format(L"0x%x",SectionHeader->VirtualAddress);
		strRSize.Format(L"0x%x",SectionHeader->Misc.VirtualSize);

		strOffset.Format(L"0x%x",SectionHeader->PointerToRawData);
		strVsize.Format(L"0x%x",SectionHeader->SizeOfRawData);

		int n = m_ListTable.GetItemCount();
		int j = m_ListTable.InsertItem(n, SectionName);
		m_ListTable.SetItemText(j, 1, strRVA);
		m_ListTable.SetItemText(j, 2, strRSize);
		m_ListTable.SetItemText(j,3,strOffset);
		m_ListTable.SetItemText(j,4,strVsize);

	}


	UpdateData(FALSE);

}

VOID
	CPETableDlg::ShowImportTable(CHAR*	szFileData,PETYPE PeType)
{
	PIMAGE_NT_HEADERS									NtHead = NULL;

	PIMAGE_THUNK_DATA32									ImportOriFirstThunk32      = NULL;
	PIMAGE_THUNK_DATA64									ImportOriFirstThunk64      = NULL;

	PIMAGE_IMPORT_BY_NAME						OrdinalName              = NULL;
	PIMAGE_NT_HEADERS64					NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32					NtHeader32 = NULL;

	IMAGE_OPTIONAL_HEADER32						OptionHead32;
	IMAGE_OPTIONAL_HEADER64						OptionHead64;
	IMAGE_DATA_DIRECTORY							ImportDirectory = {0};
	PIMAGE_IMPORT_DESCRIPTOR					ImportTable = NULL;
	ULONG_PTR														ImportSize  = 0;
	ULONG_PTR														ImportDiff  = 0;
	ULONG																ulIndex = 0;

	ULONG_PTR														ulFuncRVA = 0;


	CString																	strFuncName,strDllName;
	CString																	strIndex = 0;
	CString																	strFuncRVA;

	while(m_ListTable.DeleteColumn(0));
	m_ListTable.DeleteAllItems();

	m_ListTable.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	UINT	i = 0;
	for (i = 0;i<g_Column_Count_ImportTable;i++)
	{
		m_ListTable.InsertColumn(i, g_Column_ImportTableList[i].szTitle,LVCFMT_LEFT,(int)(g_Column_ImportTableList[i].nWidth*(dpix/96.0)));
	}

	if (szFileData==NULL)
	{
		return;
	}

	PIMAGE_DOS_HEADER DosHead = (PIMAGE_DOS_HEADER)szFileData;

	NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

	switch(PeType)
	{
	case PE:
		{
			NtHeader32 = (PIMAGE_NT_HEADERS32)NtHead;
			OptionHead32 = NtHeader32->OptionalHeader;

			ImportDirectory = OptionHead32.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT]; 

			m_EditTable1.Format(L"0x%p",ImportDirectory);

			if (ImportDirectory.Size==0)
			{
				return;
			}

			ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG_PTR)szFileData+ RVATwoOffset(NtHeader32,ImportDirectory.VirtualAddress));
		
			while (ImportTable->Name)
			{
				char* DllName = (char*)((ULONG_PTR)szFileData+ RVATwoOffset(NtHeader32,ImportTable->Name));
				strDllName = DllName;

				ImportOriFirstThunk32 = (PIMAGE_THUNK_DATA32)((ULONG_PTR)szFileData + RVATwoOffset(NtHeader32,ImportTable->OriginalFirstThunk));

					while(ImportOriFirstThunk32->u1.Function)
					{

						int n = m_ListTable.GetItemCount();
						strIndex.Format(L"%d",ulIndex);
						int j = m_ListTable.InsertItem(n, strIndex);

						//判断是以序数导入还是以Name导入		
						//最高位为1说明以序数导入 序号值为低31位
						int Temp = ImportOriFirstThunk32->u1.Ordinal&0x80000000;

						if (Temp)
						{
							Temp =  ImportOriFirstThunk32->u1.Ordinal&0x7fffffff;
							strIndex.Format(L"%d",Temp);
							m_ListTable.SetItemText(j,1,strIndex);
						}
						else
						{
							OrdinalName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)szFileData + RVATwoOffset(NtHeader32,ImportOriFirstThunk32->u1.AddressOfData));
							char* FuncName = (char*)OrdinalName->Name;
							strFuncName = FuncName;						

							ulFuncRVA = (ULONG_PTR)((ULONG_PTR)szFileData + RVATwoOffset(NtHeader32,ImportOriFirstThunk32->u1.ForwarderString));
							strFuncRVA.Format(L"0x%x",ulFuncRVA);
							m_ListTable.SetItemText(j,1,strFuncName);			
							m_ListTable.SetItemText(j,2,strFuncRVA);
						}

						m_ListTable.SetItemText(j,3,strDllName);

						ulIndex++;
						ImportOriFirstThunk32++;

					}

					ImportTable++;
			}


			break;
		}
	case PE64:
		{
			NtHeader64 = (PIMAGE_NT_HEADERS64)NtHead;
			OptionHead64 = NtHeader64->OptionalHeader;

			ImportDirectory = OptionHead64.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
			if (ImportDirectory.Size==0)
			{
				return;
			}

			ImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG_PTR)szFileData+ RVATwoOffset64(NtHeader64,ImportDirectory.VirtualAddress));


			while (ImportTable->Name)
			{
				char* DllName = (char*)((ULONG_PTR)szFileData+ RVATwoOffset64(NtHeader64,ImportTable->Name));
				strDllName = DllName;

				ImportOriFirstThunk64 = (PIMAGE_THUNK_DATA64)((ULONG_PTR)szFileData + RVATwoOffset64(NtHeader64,ImportTable->OriginalFirstThunk));

				while(ImportOriFirstThunk64->u1.Function)
				{

					int n = m_ListTable.GetItemCount();
					strIndex.Format(L"%d",ulIndex);
					int j = m_ListTable.InsertItem(n, strIndex);

					INT64 Temp = ImportOriFirstThunk64->u1.Ordinal&0x8000000000000000;

					if (Temp)
					{
						Temp =  ImportOriFirstThunk64->u1.Ordinal&0x7fffffffffffffff;
						strIndex.Format(L"%d",Temp);
						m_ListTable.SetItemText(j,1,strIndex);
					}
					else
					{
						OrdinalName = (PIMAGE_IMPORT_BY_NAME)((ULONG_PTR)szFileData + RVATwoOffset64(NtHeader64,ImportOriFirstThunk64->u1.AddressOfData));
						char* FuncName = (char*)OrdinalName->Name;
						strFuncName = FuncName;						

						ulFuncRVA = (ULONG_PTR)((ULONG_PTR)szFileData + RVATwoOffset64(NtHeader64,ImportOriFirstThunk64->u1.ForwarderString));
						strFuncRVA.Format(L"0x%x",ulFuncRVA);
						m_ListTable.SetItemText(j,1,strFuncName);			
						m_ListTable.SetItemText(j,2,strFuncRVA);

						m_ListTable.SetItemText(j,1,strFuncName);						
					}

					m_ListTable.SetItemText(j,3,strDllName);

					ulIndex++;
					ImportOriFirstThunk64++;

				}

				ImportTable++;
			}


			break;
		}

	default:
		break;

	}

UpdateData(FALSE);

}

VOID
	CPETableDlg::ShowExportTable(CHAR*	szFileData,PETYPE PeType)
{
	PIMAGE_NT_HEADERS						NtHead = NULL;
	PIMAGE_NT_HEADERS64					NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32					NtHeader32 = NULL;
	IMAGE_OPTIONAL_HEADER32			OptionHead32;
	IMAGE_OPTIONAL_HEADER64			OptionHead64;
	IMAGE_DATA_DIRECTORY				ExportDirectory = {0};
	PIMAGE_EXPORT_DIRECTORY			ExportTable = NULL;

	ULONG_PTR											NameOfArrayRVA = NULL;
	ULONG*													NameOfArray = NULL;
	ULONG_PTR											OrdinalsOfArrayRVA = NULL;
	WORD*													OrdinalsOfArray   = NULL;
	ULONG_PTR											FuncAddressOfArrayRVA = NULL;
	ULONG*													FuncAddressOfArray   = NULL;
	ULONG_PTR											FuncAdress = NULL;

	CString														strIndex, strFuncName,strFuncAddr,strRVA,strOffset;

	while(m_ListTable.DeleteColumn(0));
	m_ListTable.DeleteAllItems();

	m_ListTable.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	UINT	i = 0;
	for (i = 0;i<g_Column_Count_ExportTable;i++)
	{
		m_ListTable.InsertColumn(i, g_Column_ExportTableList[i].szTitle,LVCFMT_LEFT,(int)(g_Column_ExportTableList[i].nWidth*(dpix/96.0)));
	}

	if (szFileData==NULL)
	{
		return;
	}


	PIMAGE_DOS_HEADER DosHead = (PIMAGE_DOS_HEADER)szFileData;
	NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

	switch(PeType)
	{
	case PE:
		{
			NtHeader32 = (PIMAGE_NT_HEADERS32)NtHead;
			OptionHead32 = NtHeader32->OptionalHeader;

			 ExportDirectory = OptionHead32.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]; 
			 if (ExportDirectory.Size==0)
			 {
				 return;
			 }

			ExportTable = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)szFileData+ RVATwoOffset(NtHeader32,ExportDirectory.VirtualAddress));

			CHAR*	DllName = (CHAR*)((ULONG_PTR)szFileData + RVATwoOffset(NtHeader32,ExportTable->Name));

			m_EditTable1.Format(L"%S",DllName);

			//
			NameOfArrayRVA = (ULONG_PTR)(ExportTable->AddressOfNames);
			m_EditTable3.Format(L"0x%p",NameOfArrayRVA);
			NameOfArrayRVA = RVATwoOffset(NtHeader32,NameOfArrayRVA);
			NameOfArray = (ULONG*)((ULONG_PTR)szFileData + NameOfArrayRVA);
			CHAR* wzFuncName = NULL;

			//
			OrdinalsOfArrayRVA = (ULONG_PTR)ExportTable->AddressOfNameOrdinals;
			m_EditTable4.Format(L"0x%p",OrdinalsOfArrayRVA);
			OrdinalsOfArrayRVA = RVATwoOffset(NtHeader32,OrdinalsOfArrayRVA);
			OrdinalsOfArray =  (WORD*)((ULONG_PTR)szFileData + OrdinalsOfArrayRVA);

			//
			FuncAddressOfArrayRVA = (ULONG_PTR)ExportTable->AddressOfFunctions;
			m_EditTable2.Format(L"0x%p",FuncAddressOfArrayRVA);
			FuncAddressOfArrayRVA = RVATwoOffset(NtHeader32,FuncAddressOfArrayRVA);
			FuncAddressOfArray = (ULONG*)((ULONG_PTR)szFileData + FuncAddressOfArrayRVA);


			for (int i=0;i<ExportTable->NumberOfNames;i++)
			{
				wzFuncName = (CHAR*)((ULONG_PTR)szFileData+RVATwoOffset(NtHeader32,NameOfArray[i]));
				FuncAdress  =(ULONG_PTR)szFileData + RVATwoOffset(NtHeader32,FuncAddressOfArray[OrdinalsOfArray[i]]);

				strIndex.Format(L"%d",OrdinalsOfArray[i]);
				strFuncName.Format(L"%S",wzFuncName);
				strFuncAddr.Format(L"0x%x",FuncAdress);
				strRVA.Format(L"0x%x",FuncAddressOfArray[OrdinalsOfArray[i]]);
				strOffset.Format(L"0x%x",RVATwoOffset(NtHeader32,FuncAddressOfArray[OrdinalsOfArray[i]]));


				int n = m_ListTable.GetItemCount();
				int j = m_ListTable.InsertItem(n, strIndex);
				m_ListTable.SetItemText(j, 1, strFuncName);
				m_ListTable.SetItemText(j, 2, strFuncAddr);
				m_ListTable.SetItemText(j,3,strRVA);
				m_ListTable.SetItemText(j,4,strOffset);

			}


			break;
		}

	case PE64:
		{
			NtHeader64 = (PIMAGE_NT_HEADERS64)NtHead;
			OptionHead64 = NtHeader64->OptionalHeader;

			ExportDirectory = OptionHead64.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]; 
			if (ExportDirectory.Size==0)
			{
				return;
			}

			ExportTable = (PIMAGE_EXPORT_DIRECTORY)((ULONG_PTR)szFileData+ RVATwoOffset64(NtHeader64,ExportDirectory.VirtualAddress));


			NameOfArrayRVA = (ULONG_PTR)(ExportTable->AddressOfNames);
			m_EditTable3.Format(L"0x%p",NameOfArrayRVA);
			NameOfArrayRVA = RVATwoOffset64(NtHeader64,NameOfArrayRVA);
			NameOfArray = (ULONG*)((ULONG_PTR)szFileData + NameOfArrayRVA);
			CHAR* wzFuncName = NULL;


			OrdinalsOfArrayRVA = (ULONG_PTR)ExportTable->AddressOfNameOrdinals;
			m_EditTable4.Format(L"0x%p",OrdinalsOfArrayRVA);
			OrdinalsOfArrayRVA = RVATwoOffset64(NtHeader64,OrdinalsOfArrayRVA);
			OrdinalsOfArray =  (WORD*)((ULONG_PTR)szFileData + OrdinalsOfArrayRVA);


			FuncAddressOfArrayRVA = (ULONG_PTR)ExportTable->AddressOfFunctions;
			m_EditTable2.Format(L"0x%p",FuncAddressOfArrayRVA);
			FuncAddressOfArrayRVA = RVATwoOffset64(NtHeader64,FuncAddressOfArrayRVA);
			FuncAddressOfArray = (ULONG*)((ULONG_PTR)szFileData + FuncAddressOfArrayRVA);


			for (int i=0;i<ExportTable->NumberOfNames;i++)
			{
				wzFuncName = (CHAR*)((ULONG_PTR)szFileData+RVATwoOffset64(NtHeader64,NameOfArray[i]));
				FuncAdress  =(ULONG_PTR)szFileData + RVATwoOffset64(NtHeader64,FuncAddressOfArray[OrdinalsOfArray[i]]);

				strIndex.Format(L"%d",OrdinalsOfArray[i]);
				strFuncName.Format(L"%S",wzFuncName);
				strFuncAddr.Format(L"0x%x",FuncAdress);
				strRVA.Format(L"0x%x",FuncAddressOfArray[OrdinalsOfArray[i]]);
				strOffset.Format(L"0x%x",RVATwoOffset64(NtHeader64,FuncAddressOfArray[OrdinalsOfArray[i]]));


				int n = m_ListTable.GetItemCount();
				int j = m_ListTable.InsertItem(n, strIndex);
				m_ListTable.SetItemText(j, 1, strFuncName);
				m_ListTable.SetItemText(j, 2, strFuncAddr);
				m_ListTable.SetItemText(j,3,strRVA);
				m_ListTable.SetItemText(j,4,strOffset);

			}


			break;
		}
	default:
		break;

	}

	UpdateData(FALSE);
}

DWORD RVATwoOffset(PIMAGE_NT_HEADERS32 NTHeader, ULONG ulRVA)   
{
    PIMAGE_SECTION_HEADER SectionHeader =
		(PIMAGE_SECTION_HEADER)((ULONG)NTHeader + sizeof(IMAGE_NT_HEADERS32));  //获得节表  
	
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

ULONG_PTR RVATwoOffset64(PIMAGE_NT_HEADERS64 NTHeader, ULONG_PTR ulRVA)   
{
    PIMAGE_SECTION_HEADER SectionHeader =
		(PIMAGE_SECTION_HEADER)((ULONG_PTR)NTHeader + sizeof(IMAGE_NT_HEADERS64));  //获得节表  
	
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



VOID
	CPETableDlg::ShowRelocTable(CHAR*	szFileData,PETYPE PeType)
{
	PIMAGE_NT_HEADERS						NtHead = NULL;
	PIMAGE_NT_HEADERS64					NtHeader64 = NULL;
	PIMAGE_NT_HEADERS32					NtHeader32 = NULL;
	IMAGE_OPTIONAL_HEADER32			OptionHead32;
	IMAGE_OPTIONAL_HEADER64			OptionHead64;
	IMAGE_DATA_DIRECTORY				RelocDirectory = {0};

	PIMAGE_BASE_RELOCATION		RelocTable = NULL;
	ULONG_PTR										RelocRVA = 0;
	ULONG_PTR										BlocSize = 0;
	ULONG_PTR										RelocDataOffset = 0;

	CString													strRelocRVA,strRelocOffset,strRelocDataRVA,strRelcoDataOffset,strRelocDataOffset,strRelocData,strRelocSize;
	CString													strTypeOffset,strSizeOfBlock;

	while(m_ListTable.DeleteColumn(0));
	m_ListTable.DeleteAllItems();

	m_ListTable.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_HEADERDRAGDROP);

	UINT	i = 0;
	for (i = 0;i<g_Column_Count_RelocTable;i++)
	{
		m_ListTable.InsertColumn(i, g_Column_RelocTableList[i].szTitle,LVCFMT_LEFT,(int)(g_Column_RelocTableList[i].nWidth*(dpix/96.0)));
	}

	if (szFileData==NULL)
	{
		return;
	}

	PIMAGE_DOS_HEADER DosHead = (PIMAGE_DOS_HEADER)szFileData;
	NtHead = (PIMAGE_NT_HEADERS)((ULONG)szFileData+DosHead->e_lfanew);

	switch (PeType)
	{
	case PE:
		{
			NtHeader32 = (PIMAGE_NT_HEADERS32)NtHead;
			OptionHead32 = NtHeader32->OptionalHeader;

			RelocDirectory = OptionHead32.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]; 

			if (RelocDirectory.Size==0)
			{
				return;
			}


			RelocTable = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)szFileData+ RVATwoOffset(NtHeader32,RelocDirectory.VirtualAddress));
		
			do 
			{
				ULONG_PTR		NumOfReloc=(RelocTable->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/2;

				SHORT				MiniOffset = 0;

				PSHORT 	RelocData =(PSHORT)((ULONG_PTR)RelocTable+sizeof(IMAGE_BASE_RELOCATION));

				for (i=0; i<NumOfReloc; i++) 
				{
					PULONG_PTR RelocAddress = 0;//需要重定位的地址

					if (((*RelocData)>>12)==IMAGE_REL_BASED_DIR64||((*RelocData)>>12)==IMAGE_REL_BASED_HIGHLOW)//判断重定位类型是否为IMAGE_REL_BASED_HIGHLOW[32]或IMAGE_REL_BASED_DIR64[64]
					{
						MiniOffset = (*RelocData)&0xfff;


						RelocDataOffset = (ULONG_PTR)RVATwoOffset(NtHeader32,MiniOffset+RelocTable->VirtualAddress);

						strSizeOfBlock.Format(L"0x%x",RelocTable->SizeOfBlock);
						strTypeOffset.Format(L"0x%x",(*RelocData));

						strRelocRVA.Format(L"0x%x",RelocTable->VirtualAddress);
						strRelocOffset.Format(L"0x%x",RVATwoOffset(NtHeader32,RelocTable->VirtualAddress));

						strRelocDataRVA.Format(L"0x%x",MiniOffset+RelocTable->VirtualAddress);
						strRelocDataOffset.Format(L"0x%x",RelocDataOffset);

						strRelocData.Format(L"0x%x",*(PULONG_PTR)((ULONG_PTR)szFileData+ RelocDataOffset));

						int n = m_ListTable.GetItemCount();
						int j = m_ListTable.InsertItem(n, strRelocRVA);
						m_ListTable.SetItemText(j, 1, strRelocOffset);
						m_ListTable.SetItemText(j, 2, strSizeOfBlock);
						m_ListTable.SetItemText(j,3,strTypeOffset);
						m_ListTable.SetItemText(j,4,strRelocDataRVA);
						m_ListTable.SetItemText(j,5,strRelocDataOffset);
						m_ListTable.SetItemText(j,6,strRelocData);


					}

					RelocData++;
				}

				RelocTable=(PIMAGE_BASE_RELOCATION)((ULONG_PTR)RelocTable+RelocTable->SizeOfBlock);

			} while (RelocTable->VirtualAddress);


			break;
		}

	case PE64:
		{
			NtHeader64 = (PIMAGE_NT_HEADERS64)NtHead;
			OptionHead64 = NtHeader64->OptionalHeader;

			RelocDirectory = OptionHead64.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC]; 

			if (RelocDirectory.Size==0)
			{
				return;
			}


			RelocTable = (PIMAGE_BASE_RELOCATION)((ULONG_PTR)szFileData+ RVATwoOffset64(NtHeader64,RelocDirectory.VirtualAddress));

			do 
			{
				ULONG_PTR		NumOfReloc=(RelocTable->SizeOfBlock-sizeof(IMAGE_BASE_RELOCATION))/2;

				SHORT				MiniOffset = 0;
				PUSHORT 			RelocData =(PUSHORT)((ULONG_PTR)RelocTable+sizeof(IMAGE_BASE_RELOCATION));

				for (i=0; i<NumOfReloc; i++) 
				{
					PULONG_PTR RelocAddress = 0;//需要重定位的地址

					if (((*RelocData)>>12)==IMAGE_REL_BASED_DIR64)//判断重定位类型是否为IMAGE_REL_BASED_HIGHLOW[32]或IMAGE_REL_BASED_DIR64[64]
					{
						MiniOffset = (*RelocData)&0xfff;


						RelocDataOffset = (ULONG_PTR)RVATwoOffset64(NtHeader64,MiniOffset+RelocTable->VirtualAddress);

						strSizeOfBlock.Format(L"0x%x",RelocTable->SizeOfBlock);
						strTypeOffset.Format(L"0x%x",(*RelocData));

						strRelocRVA.Format(L"0x%x",RelocTable->VirtualAddress);
						strRelocOffset.Format(L"0x%x",RVATwoOffset64(NtHeader64,RelocTable->VirtualAddress));

						strRelocDataRVA.Format(L"0x%x",MiniOffset+RelocTable->VirtualAddress);
						strRelocDataOffset.Format(L"0x%x",RVATwoOffset64(NtHeader64,MiniOffset+RelocTable->VirtualAddress));

						strRelocData.Format(L"0x%x",*(PULONG_PTR)((ULONG_PTR)szFileData+ RelocDataOffset));

						int n = m_ListTable.GetItemCount();
						int j = m_ListTable.InsertItem(n, strRelocRVA);
						m_ListTable.SetItemText(j, 1, strRelocOffset);
						m_ListTable.SetItemText(j, 2, strSizeOfBlock);
						m_ListTable.SetItemText(j,3,strTypeOffset);
						m_ListTable.SetItemText(j,4,strRelocDataRVA);
						m_ListTable.SetItemText(j,5,strRelocDataOffset);
						m_ListTable.SetItemText(j,6,strRelocData);


					}

					RelocData++;
				}

				RelocTable=(PIMAGE_BASE_RELOCATION)((ULONG_PTR)RelocTable+RelocTable->SizeOfBlock);

			} while (RelocTable->VirtualAddress);


			break;
		}

	default:
		break;
	}

}
