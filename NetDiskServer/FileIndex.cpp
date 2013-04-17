// FileIndex.cpp : implementation file
//

#include "stdafx.h"
#include "NetDiskServer.h"
#include "FileIndex.h"
IMPLEMENT_SERIAL( CFileIndex, CObject, 1 )

// CFileIndex

CFileIndex::CFileIndex()
{
	m_strName=_T("");
	m_strPath=_T("");
	m_strEditTime=_T("");
	m_strFileSize=_T("");
	m_strParentCatalogName=_T("");
	m_bIsShare=FALSE;
	m_strSharePath=_T("");
	m_Next=NULL;
}

CFileIndex::~CFileIndex()
{
	if(m_Next!=NULL)
		delete m_Next;
}

void CFileIndex::Serialize( CArchive& archive )
{
	CObject::Serialize(archive);

	if(archive.IsStoring())
		archive<<m_strName<<m_strPath<<m_strEditTime<<m_bIsShare<<m_strSharePath;
	else
		archive>>m_strName>>m_strPath>>m_strEditTime>>m_bIsShare>>m_strSharePath;

}
// CFileIndex member functions
