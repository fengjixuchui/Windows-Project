
// Debugger1.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDebugger1App:
// �йش����ʵ�֣������ Debugger1.cpp
//

class CDebugger1App : public CWinApp
{
public:
	CDebugger1App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDebugger1App theApp;