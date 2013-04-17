#pragma once
#include "resource.h"
// CCatalogShowDlg dialog

class CCatalogShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CCatalogShowDlg)

public:
	CCatalogShowDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCatalogShowDlg();

// Dialog Data
	enum { IDD = IDD_CATALOGSHOW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_CatalogShow;
	CRect m_rect;
	HTREEITEM	m_MyDisk;
	HTREEITEM	m_ShareDisk;
	CString		m_strCurrentPath;
	CString		m_strUserLoginName;
	CString		m_strIndexInfo;
	Client		m_Client;
	CNetdiskClientDlg* m_MainDlg;
public:
	void				UpdateCatalogShow(CString indexInfo);
	CString				GetCatalogName(CString srcStr); 
	CString				GetOneIndexInfo(CString srcStr,int &iPos);
	int					NumOfSubFile(CString srcStr);
	int					NumOfSubCatalog(CString srcStr);
	CString				getRootCatalogName(CString srcName);
	void				SetSelectByFileListClick(CString path);
	
	//bool				readCatalogIndex(CCatalogIndex* catalogHead,CString srcStr);
	//HTREEITEM			RecurInsertCatalogInfo(HTREEITEM parentRoot,CString catalogName);
	virtual BOOL		OnInitDialog();
	afx_msg void		OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchangedTreeCatalogshow(NMHDR *pNMHDR, LRESULT *pResult);
};
