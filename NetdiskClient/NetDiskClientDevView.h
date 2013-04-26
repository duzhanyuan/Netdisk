// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// ("Fluent UI")����ʾ������Ϊ�ο������ṩ�� 
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ���� (C) Microsoft Corporation
// ��������Ȩ����

// NetDiskClientDevView.h : CNetDiskClientDevView ��Ľӿ�
//


#pragma once
#include "stdafx.h"
#include "CheckHeadCtrl.h"
#include "CheckListCtrl.h"
#include "UpLoadData.h"
#include "SysIcon.h"

class CMainFrame;
class CCatalogView;
class CListShowOperator;
typedef struct _ServerFileSizeInfo *PSERVERFILESIZEINFO;

class CNetDiskClientDevView : public CListView
{
	
public: // �������л�����
	CNetDiskClientDevView();
	DECLARE_DYNCREATE(CNetDiskClientDevView)

// ����
public:
	CNetDiskClientDevDoc* GetDocument() const;

// ����
public:
	CButton m_b;
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CNetDiskClientDevView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void CatalogContentShow(CString path);
public:
	CCheckHeadCtrl	m_checkHeadCtrl;
	CImageList	m_checkImgList;
	CImageList  m_lstctrlImgList;
	CString m_StrFileStoreName;//����mainframe��������·��
	LPCTSTR m_StrFileStore; //��·��������ͼ���е�listctrl
	int nItem;//��¼�����������
	UpLoadData* m_pUploadDataStore;//�����ϴ���������Ϣ
	CString m_StrFileULDate;
	int m_iFileSize;
	CString m_lstViewFindPath;//���ڲ��ҵ�·��
	CString m_StrFolderName;//����˫�����ļ�����
	CString m_StrFolderTime;//����˫�����ļ��з���ʱ��
	CEdit m_edit;
	int   m_nItem;
	int   m_nSubItem;
	CString m_NewFolderName;
	CString m_NewFolderPath;
	CString* m_PathFromViewToMainFrm;//��·�����������ڣ������ϴ��ļ�
	CCatalogView *pCalogView;
	BOOL m_checked;//�ж��ļ����Ƿ�ѡ��
	CMainFrame* m_pMainFrm;
	CListShowOperator* m_pLsShow;
	CString m_StrPath;//�����ļ�·��
	CImageList* m_pImageListBig;
	CSysIcon m_cSysIcon;
	CRect   rcCtrl;
	CMenu* m_pCMenuPopup;
	//CButton m_btnSearch;

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void TravelCatalogByName(CString path,CString name,CString time,int flag);
	afx_msg void ShowEdit(BOOL   bShow,int   nItem,int   nIndex,CRect   rcCtrl) ;
	afx_msg void OnEditEnd(NMHDR* pNMHDR,LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void EditNewFolderName();
	afx_msg void AddNewFolder(CString name);
	afx_msg void TravelDiskForNewFolder(CString path,CString name,CString time,CString NewFolderName);
	afx_msg void GetCheckedState();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void InitListViewByListStyle();
	afx_msg void CatalogShowOnList(CString path);
	afx_msg void ShowListViewByIconStyle();
	afx_msg BOOL IsFloderExisted(CString name);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void IfClickCheckbox();
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // NetDiskClientDevView.cpp �еĵ��԰汾
inline CNetDiskClientDevDoc* CNetDiskClientDevView::GetDocument() const
   { return reinterpret_cast<CNetDiskClientDevDoc*>(m_pDocument); }
#endif

