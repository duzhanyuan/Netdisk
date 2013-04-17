#pragma once

// CFileIndex command target

class CFileIndex : public CObject
{
public:
	DECLARE_SERIAL(CFileIndex)
	CFileIndex();
	virtual ~CFileIndex();
	void Serialize( CArchive& archive );
public:
	CString		m_strName;		//�ļ�����
	CString		m_strPath;		//�ļ�·��
	CString		m_strEditTime;	//�ļ�����ʱ��
	CString		m_strParentCatalogName;//��Ŀ¼����	
	CString		m_strFileSize;	//�ļ���С
	BOOL		m_bIsShare;		//�Ƿ���
	CString		m_strSharePath;	//����·��
	CFileIndex* m_Next;	//ͬ���ļ�����
};


