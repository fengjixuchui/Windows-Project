
// PELookerDlg.h : header file
//

#pragma once
#include "afxcmn.h"

enum PETYPE
{
    Unkonw,
    PE64,
    PE,
    PEARM,
};
// CPELookerDlg dialog
class CPELookerDlg : public CDialog
{
// Construction
public:
	CPELookerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PELOOKER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


    BOOL CPELookerDlg::LoadFileData(WCHAR* wzFilePath,CHAR** szFileData,ULONG* ulLow);
    BOOL CPELookerDlg::IsPEFile(CHAR* szFileData,PETYPE* PeType);

    WCHAR m_FilePath[MAX_PATH];
    CHAR* m_szFileData;
    ULONG m_ulLow;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnDropFiles(HDROP hDropInfo);
    CListCtrl m_List;
    CString m_FilePathShow;
};
