
// DebuggerMainDlg.h : 头文件
//

#pragma once
#include "Common.h"
#include "afxwin.h"
#include "resource.h"


// CDebugger1Dlg 对话框
class CDebugger1Dlg : public CDialogEx
{
// 构造
public:
	CDebugger1Dlg(CWnd* pParent = NULL);	// 标准构造函数

	WCHAR m_FilePath[MAX_PATH];
	CHAR*				m_szFileData;
	ULONG				m_ulLow;
	PETYPE				PeType;
	CStatusBar  m_wndStatusBar;  
	void CDebugger1Dlg::CreatStatusBar(void);
// 对话框数据
	enum { IDD = IDD_DEBUGGER1_DIALOG };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	

// 实现
protected:
	HICON m_hIcon;
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CEdit m_MainText;
	CEdit m_DumpText;
	afx_msg void OnBnClickedButtonClose();
	CString m_Dump;
	afx_msg void OnEnChangeEditMain();
	afx_msg void OnBnClickedButtonGoon();
	afx_msg void OnEnChangeEditEbp();
	CString m_Ebp;
	CString m_Esi;
	CString m_Edi;
	CString m_Eip;
	CString m_Esp;
	CString m_Ebx;
	CString m_Edx;
	CString m_Ecx;
	CString m_Eax;
	CString m_CF;
	CString m_AF;
	CString m_PF;
	CString m_ZF;
	CString m_SF;
	CString m_OF;
	CString m_DF;
	CString m_Text;
	afx_msg void OnBnClickedButtonCc();
	CString m_CC;
	CString m_Dr;
	CString m_Mem;
	afx_msg void OnBnClickedButtonDr();
	afx_msg void OnBnClickedButtonMem();
	afx_msg void OnBnClickedButtonSinglein();
	afx_msg void OnBnClickedButtonSingleout();
	afx_msg void OnBnClickedButtonSingleover();
};
VOID ShowAsm(ULONG_PTR Address);