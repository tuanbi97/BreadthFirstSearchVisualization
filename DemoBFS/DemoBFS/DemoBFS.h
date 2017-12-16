
// DemoBFS.h : main header file for the DemoBFS application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDemoBFSApp:
// See DemoBFS.cpp for the implementation of this class
//

class CDemoBFSApp : public CWinApp
{
public:
	CDemoBFSApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDemoBFSApp theApp;
