/*
 *	Copyright (c) 2005, ���մ����Ƽ����޹�˾
 *	All rights reserved.
 *	
 *	�ļ����ƣ�SingleThread.h
 *	�ļ���ʶ��
 *	ժ    Ҫ�����߳�������ͷ�ļ�
 *	
 *	��ǰ�汾��1.0
 *	��	  �ߣ��ڼ���
 *	����ʱ�䣺2005��5��25��
 *	��    ע��
 *
 * 
 *	��ʷ��¼��
 *	��    ����
 *	��    �ߣ�
 *	������ڣ�
 */
// SingleThread.h: interface for the CSingleThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLETHREAD_H__E45F8DC6_C9C1_4339_A0CD_4667E52BAE4C__INCLUDED_)
#define AFX_SINGLETHREAD_H__E45F8DC6_C9C1_4339_A0CD_4667E52BAE4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define THREAD_STATUS_RUN		0
#define THREAD_STATUS_STOP		1

#include "stdafx.h"
#include "CR_VA_Stream_Info.h"

class CSingleThread
{
public:
	CSingleThread();
	virtual ~CSingleThread();

	//�����߳�
	virtual int Start(void);
	//�߳�ֹͣ
	virtual void Stop(void);

	//�̺߳������е�������
	virtual void Run(void);

	// �����߳���
	void CreateLock(void);
	// ɾ���߳���
	void DeleteLock(void);
	
	// ����
	void Lock(void);
	// ����
	void Unlock(void);

	// �����ź���
	void CreateSemp(int nInit, int nCnt);
	// ɾ���ź���
	void DeleteSemp(void);
	// �ȴ��ź���
	void WaitSemp(void);
	// �ͷ��ź���
	void ReleaseSemp(void);

protected:
	bool				m_bStop;			// �߳̿��Ʒ�

#ifdef WIN32
	CRITICAL_SECTION	m_cs;
	HANDLE				m_hThread;
	HANDLE				m_hSemp;
	unsigned char *pFrmRead ;

#else // LINUX
	pthread_mutex_t 	m_mutex;
	sem_t*				m_hSemp;
#endif
};

#endif // !defined(AFX_SINGLETHREAD_H__E45F8DC6_C9C1_4339_A0CD_4667E52BAE4C__INCLUDED_)
