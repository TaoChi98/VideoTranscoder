
// CrearoVideoTranscoder.cpp : ����Ӧ�ó��������Ϊ��
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


// CCrearoVideoTranscoderApp ����

CCrearoVideoTranscoderApp::CCrearoVideoTranscoderApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CCrearoVideoTranscoderApp ����

CCrearoVideoTranscoderApp theApp;


// CCrearoVideoTranscoderApp ��ʼ��

BOOL CCrearoVideoTranscoderApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	//_CrtSetBreakAlloc(619);
	/*_CrtDumpMemoryLeaks();  
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); 
	*/
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	CString szCurPath = CPaintManagerUI::GetCurrentPath();
	int n = szCurPath.GetLength(), nPos;
	nPos = szCurPath.ReverseFind('\\');
	szCurPath = szCurPath.Left(nPos);
	szCurPath += "\\Skin\\";
	CDuiString szResourePath;
	CWndShadow::Initialize(AfxGetInstanceHandle());//duilib������Ӱ
	szResourePath.Format(_T("%s"), szCurPath);
	CPaintManagerUI::SetInstance(AfxGetInstanceHandle());
	CPaintManagerUI::SetResourcePath(szResourePath);
	CMainFrameWnd *pMainWnd = new CMainFrameWnd(_T("MainWnd.xml"));
	pMainWnd->Create(NULL, _T("��Ƶת��"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pMainWnd->CenterWindow();
	pMainWnd->ShowModal();
	delete pMainWnd;
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	// _CrtDumpMemoryLeaks();
	return FALSE;
}
