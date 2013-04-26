// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// ("Fluent UI")，该示例仅作为参考资料提供， 
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有 (C) Microsoft Corporation
// 保留所有权利。

// NetDiskClientDevView.h : CNetDiskClientDevView 类的接口
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
	
public: // 仅从序列化创建
	CNetDiskClientDevView();
	DECLARE_DYNCREATE(CNetDiskClientDevView)

// 属性
public:
	CNetDiskClientDevDoc* GetDocument() const;

// 操作
public:
	CButton m_b;
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CNetDiskClientDevView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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
	CString m_StrFileStoreName;//保存mainframe传过来的路径
	LPCTSTR m_StrFileStore; //将路径传给视图类中的listctrl
	int nItem;//记录插入的数据数
	UpLoadData* m_pUploadDataStore;//保存上传的数据信息
	CString m_StrFileULDate;
	int m_iFileSize;
	CString m_lstViewFindPath;//用于查找的路径
	CString m_StrFolderName;//保存双击的文件夹名
	CString m_StrFolderTime;//保存双击的文件夹访问时间
	CEdit m_edit;
	int   m_nItem;
	int   m_nSubItem;
	CString m_NewFolderName;
	CString m_NewFolderPath;
	CString* m_PathFromViewToMainFrm;//将路径传个主窗口，用于上传文件
	CCatalogView *pCalogView;
	BOOL m_checked;//判断文件夹是否选中
	CMainFrame* m_pMainFrm;
	CListShowOperator* m_pLsShow;
	CString m_StrPath;//保存文件路径
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

#ifndef _DEBUG  // NetDiskClientDevView.cpp 中的调试版本
inline CNetDiskClientDevDoc* CNetDiskClientDevView::GetDocument() const
   { return reinterpret_cast<CNetDiskClientDevDoc*>(m_pDocument); }
#endif

