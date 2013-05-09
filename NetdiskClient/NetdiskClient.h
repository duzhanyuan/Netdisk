
// NetdiskClient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "Client.h"
//class Client;


// CNetdiskClientApp:
// See NetdiskClient.cpp for the implementation of this class
//

class CNetdiskClientApp : public CWinApp
{
public:
	CNetdiskClientApp();

// Overrides
public:
	virtual BOOL InitInstance();
public:
	SOCKET m_TmpClient;
	CString m_loginName;
	CString	m_strIndexInfo;
// Implementation

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CNetdiskClientApp theApp;