// SerialForZB.h : main header file for the SERIALFORZB application
//

#if !defined(AFX_SERIALFORZB_H__908E964E_A002_4F28_926F_223781243587__INCLUDED_)
#define AFX_SERIALFORZB_H__908E964E_A002_4F28_926F_223781243587__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSerialForZBApp:
// See SerialForZB.cpp for the implementation of this class
//

class CSerialForZBApp : public CWinApp
{
public:
	CSerialForZBApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialForZBApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSerialForZBApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALFORZB_H__908E964E_A002_4F28_926F_223781243587__INCLUDED_)
