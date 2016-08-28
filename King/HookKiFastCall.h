#pragma once
#include "Duba.h";
#include "Other.h";
#include "GetModules.h";
#include "Hook.h"

#define  LOBYTE(w) ((BYTE)(SHORT)(w))
#define  HIBYTE(w) ((BYTE)((SHORT)(w)>>8))
#define HIWORD(w) ((SHORT)((ULONG)(w))>>16)
#define HIDWORD(w) ((ULONG)(((LONGLONG)(w)>>32)&0xFFFFFFFF))
//#define HIDWORD(w) (*((DWORD*)&(w)+1))
//
int sub_1742B(int a1);
   void *sub_1887F();
   int _GetOffsetOfImageName(); //sub_3773F();
   PVOID _SetFunction(int a1);  //sub_1CA86();
      int _GetFuncAndAddress(int a1);//sub_1C4E7()
          int _FillFuncAddressFromNtdllExport(int a1);// sub_1BC2F(int a1);
	  PVOID _SetOthersAndGetProcessExitProcessCallAddress(); //sub_1C2B3
	      int __stdcall _GetIndexByFuncAddress(int a1);//sub_1B8A5(int a1); 
	BOOLEAN sub_3540A();
	  int __stdcall sub_1EA40(int a1);
	void sub_3781F();
	void sub_3792C();
	int __stdcall sub_37B06(PERESOURCE Resource);
	int sub_18BE6();
	   int __stdcall sub_38204(PERESOURCE Resource);
	      void __stdcall sub_37B60(int **a1);
		     void __stdcall sub_38C2F(PVOID P);
			    void __stdcall sub_38514(int **a1);
				void __stdcall sub_37BB6(int **a1);
				   void __stdcall sub_37BD4(PVOID P);
	int sub_3C54F();
	   int sub_3C4D3();
	   signed int _KisInstallKiFastCallEntryHook();//sub_3C25F();
	      signed int _HookZwDisplayString();//sub_3BE77
		  int __stdcall _FakeZwDisplayString(PCUNICODE_STRING String1);  //hook sub_3BE1D
		      int __stdcall _GetKiFastCallEntryRetuanAddress(PVOID VirtualAddress); //sub_3BBDA(PVOID VirtualAddress);
			  signed int sub_3BCCA();
		  signed int _KisHookKiFastCallEntry();//sub_3BF6A();
		      signed int __stdcall _KisGetKiFastCallEntryEbxAddress(int a1, int a2, ULONG *a3);//3BB01
			  int __stdcall _KisGetKiFastCallEntryHookAddress(int a1, int a2, ULONG *a3);//3BB57
			  signed int __stdcall _KisHookKiFastCallEntryPoint(int a1);//3BD03
			     int __stdcall sub_3BCA6(signed __int64 *a1, signed __int64 a2);
				 int sub_3BAAB(unsigned int a1, int a2, int a3, int a4, int a5, int a6);
				   VOID _KifakeFastCallEntry();//3BA7B
				    int __stdcall _KisSyscallFilter(int a1, int a2, unsigned int a3, int a4, int a5);//3B9D2
					ULONG
						KisSyscallFilter(ULONG pEbx,ULONG pEdx,ULONG pEax,ULONG pEcx,ULONG pEdi);
		  signed int sub_3BFF3();//被钩过  重新钩
		    int sub_3BAD9(unsigned int a1, int a2, int a3, int a4, int a5);
			      signed int __stdcall sub_3BD9F(int a1);



extern NTSYSAPI
	NTSTATUS
	NTAPI
	ZwCreateProcessEx (
	__out PHANDLE ProcessHandle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_ATTRIBUTES ObjectAttributes,
	__in HANDLE ParentProcess,
	__in ULONG Flags,
	__in_opt HANDLE SectionHandle,
	__in_opt HANDLE DebugPort,
	__in_opt HANDLE ExceptionPort,
	__in ULONG JobMemberLevel
	);

