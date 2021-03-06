
// AddSectionOfPatchDlg.h : 头文件
//

#pragma once
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"


// CAddSectionOfPatchDlg 对话框
class CAddSectionOfPatchDlg : public CDialogEx
{
// 构造
public:
	CAddSectionOfPatchDlg(CWnd* pParent = NULL);	// 标准构造函数
	void CAddSectionOfPatchDlg::AddListInfo(CString strData);
// 对话框数据

	enum { IDD = IDD_ADDSECTIONOFPATCH_DIALOG  };

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
	CListCtrl m_List;
};
