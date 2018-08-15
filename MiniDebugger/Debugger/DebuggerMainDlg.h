
// DebuggerMainDlg.h : ͷ�ļ�
//

#pragma once
#include "Common.h"
#include "afxwin.h"
#include "resource.h"


// CDebugger1Dlg �Ի���
class CDebugger1Dlg : public CDialogEx
{
// ����
public:
	CDebugger1Dlg(CWnd* pParent = NULL);	// ��׼���캯��

	WCHAR m_FilePath[MAX_PATH];
	CHAR*				m_szFileData;
	ULONG				m_ulLow;
	PETYPE				PeType;
	CStatusBar  m_wndStatusBar;  
	void CDebugger1Dlg::CreatStatusBar(void);
// �Ի�������
	enum { IDD = IDD_DEBUGGER1_DIALOG };
	
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	

// ʵ��
protected:
	HICON m_hIcon;
	
	// ���ɵ���Ϣӳ�亯��
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