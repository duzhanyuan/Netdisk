
// NetDiskServer.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNetDiskServerApp:
// See NetDiskServer.cpp for the implementation of this class
//

class CNetDiskServerApp : public CWinApp
{
public:
	CNetDiskServerApp();

// Overrides
public:
	virtual BOOL InitInstance();
public:
	CString m_strCompyName;
// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNetDiskServerApp theApp;