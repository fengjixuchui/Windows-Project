
// PassengerTicketSystem.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "PassengerTicketSystem.h"
#include "PassengerTicketSystemDlg.h"
#include "Login.h"
#include "Flight.h"
#include "Person.h"
#include "Welcome.h"
#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPassengerTicketSystemApp

BEGIN_MESSAGE_MAP(CPassengerTicketSystemApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPassengerTicketSystemApp 构造

CPassengerTicketSystemApp::CPassengerTicketSystemApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPassengerTicketSystemApp 对象

CPassengerTicketSystemApp theApp;


// CPassengerTicketSystemApp 初始化

BOOL CPassengerTicketSystemApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	

	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));


	CLogin LoginDlg;
	if(LoginDlg.DoModal()==IDOK)
	{
		if(LoginDlg.iTypeUser=="航空公司")
		{
			CFlight Flight;
			Flight.DoModal();
		}
		if(LoginDlg.iTypeUser=="旅行社")
		{

			WCHAR Name[20] = {0};
			wsprintf(Name,L"%s",LoginDlg.m_Name);
			CCommon Common;
			Common.saveUser(Name);

		
			CWelcome Welcome;
			Welcome.DoModal();
		}
		if(LoginDlg.iTypeUser=="个人")
		{
			WCHAR Name[20] = {0};
			wsprintf(Name,L"%s",LoginDlg.m_Name);
			CCommon Common;
			Common.saveUser(Name);

			CWelcome Welcome;
			Welcome.DoModal();
		}
	}


	
	
	return FALSE;
}


/*
CPassengerTicketSystemDlg dlg;
m_pMainWnd = &dlg;
INT_PTR nResponse = dlg.DoModal();

if (nResponse == IDOK)
{
	// TODO: 在此放置处理何时用
	//  “确定”来关闭对话框的代码
}
else if (nResponse == IDCANCEL)
{
	// TODO: 在此放置处理何时用
	//  “取消”来关闭对话框的代码
}

// 删除上面创建的 shell 管理器。
if (pShellManager != NULL)
{
	delete pShellManager;
}

// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
//  而不是启动应用程序的消息泵。
	}
	*/