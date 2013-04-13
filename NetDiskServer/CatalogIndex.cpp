// CatalogIndex.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "CatalogIndex.h"

IMPLEMENT_SERIAL( CCatalogIndex, CObject, 1 )
// CCatalogIndex

CCatalogIndex::CCatalogIndex()
{
	m_strName=_T("");
	m_strPath=_T("");
	m_strEditTime=_T("");
	m_bIsShare=FALSE;
	m_strSharePath=_T("");
	m_NextCatalog=NULL;
	m_subCatalog=NULL;
	m_subFile=NULL;
}

CCatalogIndex::~CCatalogIndex()
{
	if(m_subCatalog!=NULL)
		delete m_subCatalog;
	if(m_subFile!=NULL)
		delete m_subFile;
	if(m_NextCatalog!=NULL)
		delete m_NextCatalog;
}

void CCatalogIndex::Serialize( CArchive& archive )
{
	CObject::Serialize(archive);

	if(archive.IsStoring())
		archive<<m_strName<<m_strPath<<m_strEditTime<<m_bIsShare<<m_strSharePath;
	else
		archive>>m_strName>>m_strPath>>m_strEditTime>>m_bIsShare>>m_strSharePath;

}
// CCatalogIndex member functions
