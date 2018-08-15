#pragma once



BOOL OnProcessCreated(const CREATE_PROCESS_DEBUG_INFO* pInfo);
BOOL OnThreadCreated(const CREATE_THREAD_DEBUG_INFO*);
BOOL OnException(const EXCEPTION_DEBUG_INFO*);
BOOL OnProcessExited(const EXIT_PROCESS_DEBUG_INFO*);
BOOL OnThreadExited(const EXIT_THREAD_DEBUG_INFO*);
BOOL OnOutputDebugString(const OUTPUT_DEBUG_STRING_INFO*);
BOOL OnDllLoaded(const LOAD_DLL_DEBUG_INFO*);
BOOL OnDllUnloaded(const UNLOAD_DLL_DEBUG_INFO*);
BOOL OnRipEvent(const RIP_INFO*);