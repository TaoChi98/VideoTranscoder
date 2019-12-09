// SingleThread.cpp: implementation of the CSingleThread class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SingleThread.h"

#ifdef WIN32
DWORD WINAPI SingleThreadRun(void* lpParam)
#else
void *SingleThreadRun(void* lpParam)
#endif
{
	CSingleThread *pThread = (CSingleThread *)lpParam;
	pThread->Run();
	return 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSingleThread::CSingleThread():pFrmRead(NULL)
{
#ifdef WIN32
	m_hThread = NULL;
#endif
	m_bStop = TRUE;
	pFrmRead = (unsigned char*)malloc(1024*1024*2*sizeof(unsigned char));
	CreateLock();
	//	CreateSemp(1, 1);
}

CSingleThread::~CSingleThread()
{
	Stop();
	//	DeleteSemp();
	if (pFrmRead != NULL)
	{
		free(pFrmRead);
		pFrmRead = NULL;
	}
	DeleteLock();
}

int CSingleThread::Start()
{
	if (m_bStop)
	{
		m_bStop = FALSE;
#ifdef WIN32
		m_hThread = CreateThread(NULL, 0, SingleThreadRun, this, 0, NULL);
#else
		pthread_t thr_id;
		pthread_create(&thr_id, NULL, SingleThreadRun, this);
#endif
	}
	return 0;
}

void CSingleThread::Stop()
{
	if (!m_bStop)
	{
		WaitSemp();
		m_bStop = TRUE;
#ifdef WIN32
		/*if (WaitForSingleObject(m_hThread, 100) != 0)
		{
			TerminateThread(m_hThread, -1);
		}
		*/
	//	Sleep(1000);
		CloseHandle(m_hThread);
		m_hThread = NULL;
#endif
	}
}

void CSingleThread::Run()
{
	return ;
}

void CSingleThread::CreateLock()
{
#ifdef WIN32
	::InitializeCriticalSection(&m_cs);
#else // LINUX
	pthread_mutex_init(&m_mutex, NULL);
#endif
}

void CSingleThread::DeleteLock()
{
#ifdef WIN32
	::DeleteCriticalSection(&m_cs);
#else // LINUX
	pthread_mutex_destroy(&m_mutex);
#endif
}

void CSingleThread::Lock()
{
#ifdef WIN32
	::EnterCriticalSection(&m_cs);
#else // LINUX
	pthread_mutex_lock(&m_mutex);
#endif
}

void CSingleThread::Unlock()
{
#ifdef WIN32
	::LeaveCriticalSection(&m_cs);
#else // LINUX
	pthread_mutex_unlock(&m_mutex);
#endif
}

// 创建信号量
void CSingleThread::CreateSemp(int nInit, int nCnt)
{
#ifdef WIN32
	m_hSemp = CreateSemaphore(NULL, nInit, nCnt, NULL);
#else
	m_hSemp = (sem_t *)malloc(sizeof(sem_t));
	sem_init(m_hSemp, 0, nInit);
#endif
}

// 删除信号量
void CSingleThread::DeleteSemp(void)
{
#ifdef WIN32
	CloseHandle(m_hSemp);
	m_hSemp = NULL;
#else
	sem_destroy((sem_t *)m_hSemp);
	free(m_hSemp);
#endif
}

// 等待信号量
void CSingleThread::WaitSemp(void)
{
#ifdef WIN32
	WaitForSingleObject(m_hSemp, INFINITE);
#else
	sem_wait((sem_t *)m_hSemp);
#endif
}

// 释放信号量
void CSingleThread::ReleaseSemp(void)
{
#ifdef WIN32
	ReleaseSemaphore(m_hSemp, 1, NULL);
#else
	sem_post((sem_t *)m_hSemp);
#endif
}
