
// CrearoVideoTranscoder.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MainFrameWnd.h"
#include "CrearoVideoTranscoder.h"
#include "CrearoVideoTranscoderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCrearoVideoTranscoderApp

BEGIN_MESSAGE_MAP(CCrearoVideoTranscoderApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CCrearoVideoTranscoderApp 构造

CCrearoVideoTranscoderApp::CCrearoVideoTranscoderApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CCrearoVideoTranscoderApp 对象

CCrearoVideoTranscoderApp theApp;


// CCrearoVideoTranscoderApp 初始化

BOOL CCrearoVideoTranscoderApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	//_CrtSetBreakAlloc(619);
	/*_CrtDumpMemoryLeaks();  
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
	*/
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	CString szCurPath = CPaintManagerUI::GetCurrentPath();
	int n = szCurPath.GetLength(), nPos;
	nPos = szCurPath.ReverseFind('\\');
	szCurPath = szCurPath.Left(nPos);
	szCurPath += "\\Skin\\";
	CDuiString szResourePath;
	CWndShadow::Initialize(AfxGetInstanceHandle());//duilib窗口阴影
	szResourePath.Format(_T("%s"), szCurPath);
	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	CPaintManagerUI::SetResourcePath(szResourePath);
	CMainFrameWnd *pMainWnd = new CMainFrameWnd(_T("MainWnd.xml"));
	pMainWnd->Create(NULL, _T("视频转码"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pMainWnd->CenterWindow();
	pMainWnd->ShowModal();
	delete pMainWnd;
	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	// _CrtDumpMemoryLeaks();
	return FALSE;
}
