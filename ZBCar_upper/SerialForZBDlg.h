// SerialForZBDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"
//}}AFX_INCLUDES

#if !defined(AFX_SERIALFORZBDLG_H__A6DA03BF_FA00_4248_96A7_F7BCF3ECBFFC__INCLUDED_)
#define AFX_SERIALFORZBDLG_H__A6DA03BF_FA00_4248_96A7_F7BCF3ECBFFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialForZBDlg dialog

class CSerialForZBDlg : public CDialog
{
// Construction
public:
	CSerialForZBDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSerialForZBDlg)
	enum { IDD = IDD_SERIALFORZB_DIALOG };
	CButton	m_Send;
	CButton	m_Open;
	CString	m_RDZB1;
	CString	m_RDZB2;
	CString	m_RDZB3;
	CString	m_WTZB;
	CMSComm	m_Comm;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialForZBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialForZBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOnCommMscomm();
	afx_msg void OnOpenSerial();
	afx_msg void OnSendData();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALFORZBDLG_H__A6DA03BF_FA00_4248_96A7_F7BCF3ECBFFC__INCLUDED_)
