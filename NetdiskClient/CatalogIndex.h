#pragma once
#include "FileIndex.h"
// CCatalogIndex command target

class CCatalogIndex : public CObject
{
public:
	DECLARE_SERIAL(CCatalogIndex)
	CCatalogIndex();
	virtual ~CCatalogIndex();
	void Serialize( CArchive& archive );

public:
	CString		m_strName;			//Ŀ¼��������
	CString		m_strPath;			//Ŀ¼����·��
	CString		m_strEditTime;		//Ŀ¼����ʱ��
	BOOL		m_bIsShare;			//�Ƿ���
	CString		m_strSharePath;		//����·��
	CFileIndex* m_subFile;			//���ļ�����
	CCatalogIndex* m_NextCatalog;	//ͬ��Ŀ¼����
	CCatalogIndex* m_subCatalog;	//��Ŀ¼����
};


