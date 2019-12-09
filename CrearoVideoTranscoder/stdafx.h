
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"
//#include<afxdhtml.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ

#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��





#include "../Include/Duilib/UIlib.h"
#include "CR_VA_Stream_Info.h"
#define CM_BASE	            (WM_APP+0x1200)
#define WM_ADDLISTITEM WM_USER + 50
#define WM_TRANSCODEEND WM_USER + 100

#define  CM_SETOUTPATH  	(CM_BASE+0x0001)
#define  CM_MAINTONEWTRANCFILENAME (CM_BASE+0x0002)
#define  CM_NEWTRANCTOMAINFILENAME (CM_BASE+0x0003)
#define  CM_MAINTOSETWND (CM_BASE+0x0004)
#define  CM_MAINTOEXIT (CM_BASE+0x0005)
#define  CM_EXITTOMAIN (CM_BASE+0x0006)

//#define  NS_PROGRESS (CM_BASE + 0x0005)
using namespace DuiLib;

class DllManager;

typedef struct Node
{
	Node()
	{
		Status =_T("");
		strFileNameOut =_T("");
		outType = _T("");
		strFileNameIn= _T("");

		pVideoTrans = NULL;
		m_StreamVR = NULL;
		m_StreamAR = NULL;
		m_StreamVW = NULL;
		m_StreamAW = NULL;
		m_Writer = NULL;
		m_Reader = NULL;
		strShowFileName  =_T("");
	}

	DllManager *pVideoTrans;
	CString Status;
	CString strFileNameOut ;
	float uiReadLen;
	float uiFileLen ;
	int sampleA;
	int sampleV;

	CString strShowFileName;
	CString strFileNameIn;
	int Progress;
	int timeShow;
	CString outType;

	CAVIREADER_STREAMHANDLE m_StreamVR;
	CAVIREADER_STREAMHANDLE m_StreamAR;

	CAVIWRITER_STREAMHANDLE m_StreamVW;
	CAVIWRITER_STREAMHANDLE m_StreamAW;

	CAVIREADER_HANDLE m_Writer;
	CAVIREADER_HANDLE m_Reader;

}StreamHandleInfo;


#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Include\\Duilib\\Lib\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\Include\\Duilib\\Lib\\DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Include\\Duilib\\Lib\\DuiLib_u.lib")
#   else
#      pragma comment(lib, "..\\Include\\Duilib\\Lib\\DuiLib.lib")
#   endif
#endif

class CXMLBaseWnd : public WindowImplBase
{
public:
	CXMLBaseWnd(void) {}
	explicit CXMLBaseWnd(LPCTSTR pszXMLPath) : m_strXMLPath(pszXMLPath)
	{

	}
	virtual ~CXMLBaseWnd(void) {}
	virtual LPCTSTR GetWindowClassName() const
	{
		return _T("XMLWnd");
	}

	virtual CDuiString GetSkinFile()
	{
		return m_strXMLPath;
	}

	virtual CDuiString GetSkinFolder()
	{
		return _T("Skin");
	}

protected:
	CDuiString m_strXMLPath;
//	int  m_szComTaskNum;

};



#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


