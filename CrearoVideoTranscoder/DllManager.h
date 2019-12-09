
#if !defined(AFX_DLLMANAGER_H__5D996117_BDDE_4747_8E37_1A9B9D5DAFA2__INCLUDED_)
#define AFX_DLLMANAGER_H__5D996117_BDDE_4747_8E37_1A9B9D5DAFA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SingleThread.h"
#include "CR_VA_Stream_Info.h"

/*#ifdef _DEBUG
#define new DEBUG_NEW
#endif
*/
class DllManager : public CSingleThread
{
public:
	DllManager();
	virtual ~DllManager();

	// 得到全局实例，如果该实例还没有创建，则创建之
	static DllManager* open_singleton(void)
	{
		if (instance() == 0)
		{
			instance() = new DllManager();
		}
		return instance();
	}

	// 如果用户创建了全局实例，必须调用这个函数
	static void close_singleton(void)
	{
		if (instance() != NULL)
		{
			delete instance();
			instance() = 0;
		}
	}

	static DllManager* & instance(void)
	{
		static DllManager* s_inst = 0;
		return s_inst;
	}

	int CAVI2AVI(const char *pCAVIPath, const char *pAVIPath ,float ReadLen ,int SampleV,int SampleA,HWND hwnd);
	int GetProgress(int *pnProgress);
	void SetFinishSample(CAVIREADER_HANDLE phReader,CAVIREADER_HANDLE phWriter, CAVIREADER_STREAMHANDLE phStreamVR,CAVIREADER_STREAMHANDLE phStreamAR, CAVIWRITER_STREAMHANDLE phStreamVW,CAVIWRITER_STREAMHANDLE phStreamAW,float FileLen, float ReadLen,int SampleV,int SampleA)
	{m_phReader = phReader,m_phWriter = phWriter,m_phStreamVR = phStreamVR,m_phStreamAR = phStreamAR,m_phStreamVW = phStreamVW,m_phStreamAW = phStreamAW,  m_uiReadLen = ReadLen,m_SampleNOV = SampleV,m_SampleNOA = SampleA,m_uiFileLen = FileLen;}
	//线程函数运行的主函数
	virtual void Run();
	void isStartRun(BOOL flag){m_szFlag = flag;}

public:
	void CloseFile();

	STORAGE_READ_OBJ *m_pObjRead;
	STORAGE_WRITE_OBJ *m_pObjWrite;
	AENC_HANDLE m_hAENC;

	CAVIREADER_HANDLE m_phReader;
	CAVIREADER_STREAMHANDLE m_phStreamVR;
	CAVIREADER_STREAMHANDLE m_phStreamAR;

	//	CAVIWRITER_HANDLE m_phWriter;
	CAVIREADER_HANDLE m_phWriter;

	CAVIWRITER_STREAMHANDLE m_phStreamVW;
	CAVIWRITER_STREAMHANDLE m_phStreamAW;

	int m_SampleNOV;
	int m_SampleNOA;
	BOOL m_szFlag;
	BOOL m_bRun;
	HWND m_szHwnd;

	VARENDER_HANDLE m_phVARender;
	UINT m_hCRAudioDec;

	float m_uiFileLen;
	float m_uiReadLen;
};
#endif 