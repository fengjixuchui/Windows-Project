
// PEAnalysisDlg.h : 头文件
//

#pragma once
#include "PeCheckFunc.h"
#include "afxcmn.h"
#include "PEHeaderDlg.h"
#include "PETableDlg.h"
// CPEAnalysisDlg 对话框
class CPEAnalysisDlg : public CDialogEx
{
// 构造
public:
	CPEAnalysisDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PEANALYSIS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	TCHAR				m_FilePath[1024];
	CString				m_strFilePath;
	CHAR*				m_szFileData;
	ULONG				m_ulLow;
	PETYPE				PeType;


	CPEHeaderDlg		m_PeHeaderDlg;
	CPETableDlg			m_PeTableDlg;

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
	CString m_EditFilePath;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_EditFileType;
	CTabCtrl m_TabMain;
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
};
