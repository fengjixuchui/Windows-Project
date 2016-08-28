#pragma once
#include "Duba.h"





typedef enum _SYSTEM_INFORMATION_CLASS   
{   
	SystemBasicInformation,                 // 0 Y N   
	SystemProcessorInformation,             // 1 Y N   
	SystemPerformanceInformation,           // 2 Y N   
	SystemTimeOfDayInformation,             // 3 Y N   
	SystemNotImplemented1,                  // 4 Y N   
	SystemProcessesAndThreadsInformation,   // 5 Y N   
	SystemCallCounts,                       // 6 Y N   
	SystemConfigurationInformation,         // 7 Y N   
	SystemProcessorTimes,                   // 8 Y N   
	SystemGlobalFlag,                       // 9 Y Y   
	SystemNotImplemented2,                  // 10 Y N   
	SystemModuleInformation,                // 11 Y N   
	SystemLockInformation,                  // 12 Y N   
	SystemNotImplemented3,                  // 13 Y N   
	SystemNotImplemented4,                  // 14 Y N   
	SystemNotImplemented5,                  // 15 Y N   
	SystemHandleInformation,                // 16 Y N   
	SystemObjectInformation,                // 17 Y N   
	SystemPagefileInformation,              // 18 Y N   
	SystemInstructionEmulationCounts,       // 19 Y N   
	SystemInvalidInfoClass1,                // 20   
	SystemCacheInformation,                 // 21 Y Y   
	SystemPoolTagInformation,               // 22 Y N   
	SystemProcessorStatistics,              // 23 Y N   
	SystemDpcInformation,                   // 24 Y Y   
	SystemNotImplemented6,                  // 25 Y N   
	SystemLoadImage,                        // 26 N Y   
	SystemUnloadImage,                      // 27 N Y   
	SystemTimeAdjustment,                   // 28 Y Y   
	SystemNotImplemented7,                  // 29 Y N   
	SystemNotImplemented8,                  // 30 Y N   
	SystemNotImplemented9,                  // 31 Y N   
	SystemCrashDumpInformation,             // 32 Y N   
	SystemExceptionInformation,             // 33 Y N   
	SystemCrashDumpStateInformation,        // 34 Y Y/N   
	SystemKernelDebuggerInformation,        // 35 Y N   
	SystemContextSwitchInformation,         // 36 Y N   
	SystemRegistryQuotaInformation,         // 37 Y Y   
	SystemLoadAndCallImage,                 // 38 N Y   
	SystemPrioritySeparation,               // 39 N Y   
	SystemNotImplemented10,                 // 40 Y N   
	SystemNotImplemented11,                 // 41 Y N   
	SystemInvalidInfoClass2,                // 42   
	SystemInvalidInfoClass3,                // 43   
	SystemTimeZoneInformation,              // 44 Y N   
	SystemLookasideInformation,             // 45 Y N   
	SystemSetTimeSlipEvent,                 // 46 N Y   
	SystemCreateSession,                    // 47 N Y   
	SystemDeleteSession,                    // 48 N Y   
	SystemInvalidInfoClass4,                // 49   
	SystemRangeStartInformation,            // 50 Y N   
	SystemVerifierInformation,              // 51 Y Y   
	SystemAddVerifier,                      // 52 N Y   
	SystemSessionProcessesInformation       // 53 Y N   
} SYSTEM_INFORMATION_CLASS;   




typedef struct _RTL_PROCESS_MODULE_INFORMATION {
	HANDLE Section;                
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;
typedef struct _RTL_PROCESS_MODULES {
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;



typedef enum _SECTION_INFORMATION_CLASS {
	SectionBasicInformation,
	SectionImageInformation,
	MaxSectionInfoClass  // MaxSectionInfoClass should always be the last enum
} SECTION_INFORMATION_CLASS;


typedef struct _SECTION_IMAGE_INFORMATION {
	PVOID TransferAddress;
	ULONG ZeroBits;
	SIZE_T MaximumStackSize;
	SIZE_T CommittedStackSize;
	ULONG SubSystemType;
	union {
		struct {
			USHORT SubSystemMinorVersion;
			USHORT SubSystemMajorVersion;
		};
		ULONG SubSystemVersion;
	};
	ULONG GpValue;
	USHORT ImageCharacteristics;
	USHORT DllCharacteristics;
	USHORT Machine;
	BOOLEAN ImageContainsCode;
	BOOLEAN Spare1;
	ULONG LoaderFlags;
	ULONG ImageFileSize;
	ULONG Reserved[ 1 ];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;


extern
	NTSTATUS
	NTAPI
	ZwQuerySection (
	__in HANDLE SectionHandle,
	__in SECTION_INFORMATION_CLASS SectionInformationClass,
	__out_bcount(SectionInformationLength) PVOID SectionInformation,
	__in SIZE_T SectionInformationLength,
	__out_opt PSIZE_T ReturnLength
	);



typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SpareUl2;
	ULONG SpareUl3;
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG WorkingSetSize;
	ULONG QuotaPeakPagedPoolUsage;
	ULONG QuotaPagedPoolUsage;
	ULONG QuotaPeakNonPagedPoolUsage;
	ULONG QuotaNonPagedPoolUsage;
	ULONG PagefileUsage;
	ULONG PeakPagefileUsage;
	ULONG PrivatePageCount;
} SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;



extern
	NTSTATUS        
	ZwQuerySystemInformation(IN SYSTEM_INFORMATION_CLASS SystemInformationClass,    
	OUT PVOID SystemInformation,    
	IN ULONG SystemInformationLength,    
	OUT PULONG ReturnLength OPTIONAL);



signed int _GetSystemModule();
NTSTATUS  
	Asm_1C49E(IN int a1,
	unsigned int  a2,
	int  a3,
	unsigned int a4);


NTSTATUS  
	Asm_19856(IN int a1,
	int a2,
	int  a3);


PVOID __stdcall _CreateFileMap(PCWSTR SourceString, int a2, int a3);
signed int __stdcall sub_2DF21(int a1, unsigned int a2);