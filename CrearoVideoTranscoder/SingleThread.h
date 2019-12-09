/*
 *	Copyright (c) 2005, 安徽创世科技有限公司
 *	All rights reserved.
 *	
 *	文件名称：SingleThread.h
 *	文件标识：
 *	摘    要：单线程类基类的头文件
 *	
 *	当前版本：1.0
 *	作	  者：于继梁
 *	创建时间：2005年5月25日
 *	备    注：
 *
 * 
 *	历史记录：
 *	版    本：
 *	作    者：
 *	完成日期：
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

	//创建线程
	virtual int Start(void);
	//线程停止
	virtual void Stop(void);

	//线程函数运行的主函数
	virtual void Run(void);

	// 创建线程锁
	void CreateLock(void);
	// 删除线程锁
	void DeleteLock(void);
	
	// 加锁
	void Lock(void);
	// 解锁
	void Unlock(void);

	// 创建信号量
	void CreateSemp(int nInit, int nCnt);
	// 删除信号量
	void DeleteSemp(void);
	// 等待信号量
	void WaitSemp(void);
	// 释放信号量
	void ReleaseSemp(void);

protected:
	bool				m_bStop;			// 线程控制符

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
