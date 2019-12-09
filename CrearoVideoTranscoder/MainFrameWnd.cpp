#include "StdAfx.h"
#include <time.h>
#include <stdio.h>
#include <iostream>
#include "MainFrameWnd.h"
#include "ParamSetWnd.h"
#include "NewTranscodeWnd.h"
#include "DllManager.h"
#include <ctime>
#include "ExitHintWnd.h"

#define NS_PROGRESS 10000
#define NS_TIMERCHANGE 1000
using namespace std;

CMainFrameWnd::CMainFrameWnd( LPCTSTR pszXMLPath ):CXMLBaseWnd(pszXMLPath)
{
	CString szCurPath = CPaintManagerUI::GetCurrentPath();
	int n = szCurPath.GetLength(), nPos;
	nPos = szCurPath.ReverseFind('\\');
	szCurPath = szCurPath.Left(nPos);
	szCurPath += "\\VideoFiles";
	m_szOutPath = szCurPath;
	m_szFlag = FALSE;
	hintWnd = FALSE;
	m_nIndex = 0;
	m_szTempTaskNum = _T("");
}

CMainFrameWnd::~CMainFrameWnd(void)
{
	for (int i=0;i<m_szTranscodeFileInfoVct.size();i++)
	{
		if ( m_szTranscodeFileInfoVct[i].pVideoTrans != NULL)
		{
			m_szTranscodeFileInfoVct[i].pVideoTrans->isStartRun(TRUE);
			delete m_szTranscodeFileInfoVct[i].pVideoTrans;
			m_szTranscodeFileInfoVct[i].pVideoTrans = NULL;		
		}
	}
}

LPCTSTR CMainFrameWnd::GetItemText( CControlUI* pControl, int iIndex, int iSubItem )
{
	return 0;
}

void CMainFrameWnd::InitWindow()
{
	m_szTaskNum = 2;
	m_szTempTaskNum = _T("2");
	::DragAcceptFiles(this->m_hWnd,true);
	CreateDirectory(m_szOutPath,NULL);
	if (!PathIsDirectory(m_szOutPath))
	{
		::CreateDirectory(m_szOutPath,NULL);
	}

	CButtonUI* btnStart = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PlayBtn")));
	CButtonUI* btnStop = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PauseBtn")));
	CButtonUI* btnDelete = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("DeleteBtn")));
	if (btnStart)
	{
		btnStart->SetEnabled(false);
	}
	if (btnStop)
	{
		btnStop->SetEnabled(false);
	}
	if (btnDelete)
	{
		btnDelete->SetEnabled(false);
	}
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
	if(pList != NULL)
	{
		pList->OnSize += MakeDelegate(this,&CMainFrameWnd::OnListSize);
	}
	m_szBKImage1.Format(_T("file='bkImage.png' dest='340,100,740,500'"));
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	int nPosX = (cx - 400)/2;
	int nPosY = (cy - 400-220)/2;
	m_szBKImage2.Format(_T("file='bkImage.png' dest='%d,%d,%d,%d'"), nPosX, nPosY, nPosX+400, nPosY+400);
	pList->SetBkImage(m_szBKImage1);
	m_szInFileName.empty();
	m_szInFileName.resize(0);

	m_szTempFileName.empty();
	m_szTempFileName.resize(0);

	m_szCurrentFileName.empty();
	m_szCurrentFileName.resize(0);

	m_szOutFileName.empty();
	m_szOutFileName.resize(0);

	m_szStatus.empty();
	m_szStatus.resize(0);

	m_szProgress.empty();
	m_szProgress.resize(0);

	m_szUsedTime.empty();
	m_szUsedTime.resize(0);

	m_szOutType.empty();
	m_szOutType.resize(0);

	m_szOutFilePath.empty();
	m_szOutFilePath.resize(0);

	m_szInPathFileName.empty();
	m_szInPathFileName.resize(0);

	m_szTranscodeFileInfoVct.empty();
	m_szTranscodeFileInfoVct.resize(0);

	m_szCurrentTaskNumVct.empty();
	m_szCurrentTaskNumVct.resize(0);

	::SetTimer(GetHWND(),NS_PROGRESS,NS_TIMERCHANGE,NULL);
	m_nIndex = 1;
}

bool CMainFrameWnd::OnListSize(void *p)
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
	for (int i = 0 ;i <m_szTranscodeFileInfoVct.size();i++)
	{
		CListContainerElementUI* pListElement = (CListContainerElementUI*)pList->GetItemAt(i);
		if (pListElement)
		{
			CHorizontalLayoutUI *pHorizontalLayout = (CHorizontalLayoutUI*)pListElement->GetItemAt(0);
			if (pHorizontalLayout)
			{
				CListHeaderItemUI *pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("StauteItem")));
				int nWidth = pListHeader->GetWidth();
				CDuiString strPos;
				strPos.Format(_T("%d"),nWidth);
				CLabelUI *pLabel = (CLabelUI *)pHorizontalLayout->GetItemAt(0);
				CDuiString szText;
				szText.Format(_T("%s"),m_szTranscodeFileInfoVct[i].Status);
				pLabel->SetAttribute(_T("width"),strPos);
				pLabel->SetText(szText);

				pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("FileItem")));
				nWidth = pListHeader->GetWidth();
				strPos.Format(_T("%d"),nWidth);
				pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(1);
				szText.Format(_T("%s"),m_szTranscodeFileInfoVct[i].strShowFileName);
				pLabel->SetAttribute(_T("width"),strPos);
				pLabel->SetText(szText);

				pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("ProgressItem")));
				nWidth = pListHeader->GetWidth();
				strPos.Format(_T("%d"),nWidth);
				pLabel = (CLabelUI *)pHorizontalLayout->GetItemAt(2);
				szText.Format(_T("%d"),m_szTranscodeFileInfoVct[i].Progress);
				pLabel->SetAttribute(_T("width"),strPos);
				CString centent = _T("%");
				szText += centent;
				pLabel->SetText(szText);

				pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("TimeItem")));
				nWidth = pListHeader->GetWidth();
				strPos.Format(_T("%d"),nWidth);
				pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(3);
				pLabel->SetAttribute(_T("width"),strPos);
				int mill = m_szTranscodeFileInfoVct[i].timeShow;
				CString hour,minute,sec ,szTime;
				hour.Format(_T("%02d:"),mill/3600);
				minute.Format(_T("%02d:"),(mill%3600)/60);
				sec.Format(_T("%02d"),(mill%3600%60));
				szTime = hour+minute +sec;
				pLabel->SetText(szTime);

				pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("OutTypeItem")));
				nWidth = pListHeader->GetWidth();
				strPos.Format(_T("%d"),nWidth);
				pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(4);
				pLabel->SetAttribute(_T("width"),strPos);
				szText.Format(_T("%s"),m_szTranscodeFileInfoVct[i].outType);
				pLabel->SetText(szText);

				pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("OutFileItem")));
				nWidth = pListHeader->GetWidth();
				strPos.Format(_T("%d"),nWidth);
				pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(5);
				pLabel->SetAttribute(_T("width"),strPos);
				szText.Format(_T("%s"),m_szTranscodeFileInfoVct[i].strFileNameOut);
				pLabel->SetText(szText);
			}
		}
	}
	return true;
}

void CMainFrameWnd::Notify( TNotifyUI& msg )
{
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		CButtonUI* btnStart = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PlayBtn")));
		CButtonUI* btnStop = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PauseBtn")));
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		CButtonUI* btnDelete = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("DeleteBtn")));
		int nCulSel = pList->GetCurSel();
		if (m_szTranscodeFileInfoVct[nCulSel].Status.CompareNoCase(_T("暂停"))==0)
		{
			if (btnStart &&btnStop && btnDelete)
			{
				btnStart->SetEnabled(TRUE);
				btnStop->SetEnabled(FALSE);
				btnDelete->SetEnabled(TRUE);
			}
		}
		else if (m_szTranscodeFileInfoVct[nCulSel].Status.CompareNoCase(_T("等待"))==0 ||m_szTranscodeFileInfoVct[nCulSel].Status.CompareNoCase(_T("完成"))==0)
		{
			if (btnStart &&btnStop && btnDelete)
			{
				btnStart->SetEnabled(FALSE);
				btnStop->SetEnabled(FALSE);
				btnDelete->SetEnabled(TRUE);
			}
		}
		else if (m_szTranscodeFileInfoVct[nCulSel].Status.CompareNoCase(_T("正在进行"))==0)
		{
			if (btnStart &&btnStop && btnDelete)
			{
				btnStart->SetEnabled(FALSE);
				btnStop->SetEnabled(TRUE);
				btnDelete->SetEnabled(FALSE);
			}
		}
	}
	__super::Notify(msg);
}

LRESULT CMainFrameWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	__super::OnCreate(uMsg,wParam,lParam,bHandled);
	m_WndShadow.Create(GetHWND());
	m_WndShadow.SetSize(4);  
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetColor(RGB(0xA6, 0xA6, 0xA6));
	return 0;
}

void CMainFrameWnd::OnClick( TNotifyUI& msg )
{
	if (msg.pSender->GetName() == _T("SetBtn"))
	{
		CParamSetWnd *paramSet = new CParamSetWnd(_T("SettingWnd.xml"));
		paramSet->Create(m_hWnd,_T("ParamSetWnd"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE);
		paramSet->GetParaentWndHwnd(this->m_hWnd);
		::SendMessage(paramSet->SetCurrentWndHwnd(),CM_MAINTOSETWND,(WPARAM)(LPCTSTR)m_szTempTaskNum,(LPARAM)(LPCTSTR)m_szOutPath);
		paramSet->CenterWindow();
		paramSet->ShowModal();	
		delete paramSet;
		paramSet = NULL;
	}
	else if (msg.pSender->GetName() == _T("closeBtn"))
	{
		bool f =false;
		if(m_szTranscodeFileInfoVct.empty()) 
		{
			PostQuitMessage(0L);
		}
		for (int i=0;i<m_szTranscodeFileInfoVct.size();i++)
		{
			if (m_szTranscodeFileInfoVct[i].pVideoTrans->m_phReader != NULL)
			{	
				f  = TRUE;
				CExitHintWnd *pExitHint = new CExitHintWnd(_T("QuitHintWnd.xml"));
				pExitHint ->Create(m_hWnd,_T("QuitHintWnd"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE);
				if (!m_szTranscodeFileInfoVct.empty())
				{
					pExitHint->GetParaentWndHwnd(m_hWnd);
					//::SendMessage(pExitHint->SetWndHwnd(),CM_MAINTOEXIT,0,(LPARAM)&m_szTranscodeFileInfoVct);
				}
				pExitHint->CenterWindow();
				pExitHint->ShowModal();
				delete pExitHint;
				pExitHint = NULL;
				break;
			}
		}
		if (!f)
		{
			PostQuitMessage(0L);
		}
	}
	else if (msg.pSender->GetName() == _T("AddBtn"))
	{
		size_t index;
		CString cstrsucstring;
		CFileDialog filedlg(TRUE, NULL ,NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT,_T("Source Files(*.avi)|*.avi||"));
		TCHAR *pBuffer = new TCHAR[MAX_PATH*20];
		filedlg.m_ofn.lpstrFile = pBuffer;
		filedlg.m_ofn.nMaxFile = MAX_PATH * 20;
		filedlg.m_ofn.lpstrFile[0] = '\0';
		if (filedlg.DoModal() == IDOK)
		{
			CString cstrfilpath = _T("");
			POSITION pos = filedlg.GetStartPosition();
			while (pos != NULL)
			{
				cstrfilpath = filedlg.GetNextPathName(pos);
				m_szInFileName.push_back((cstrfilpath));
			}
		    CNewTranscodeWnd *newTranscode = new CNewTranscodeWnd(_T("NewTranscodeWnd.xml"));
			newTranscode ->Create(m_hWnd,_T("NewTranscodeWnd"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE);
			m_NewTranscodeHwnd = newTranscode->GetCurrentwndHwnd();
			newTranscode->SetParentHwnd(this->m_hWnd);

			NewTaskAndFilePath taskAndFilePath;
			taskAndFilePath.szOut = m_szOutPath;
			taskAndFilePath.nNum = m_szTempTaskNum;
			::SendMessage(m_NewTranscodeHwnd,CM_MAINTONEWTRANCFILENAME,(WPARAM)&taskAndFilePath,(LPARAM)&m_szInFileName);
			m_szInFileName.clear();
			m_szInFileName.resize(0);
			newTranscode->CenterWindow();
			newTranscode->ShowModal();
			delete newTranscode;
			newTranscode = NULL;
		}
		delete []pBuffer;
	}
	else if (msg.pSender->GetName() == _T("PlayBtn"))
	{
		CButtonUI* btnStart = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PlayBtn")));
		CButtonUI* btnStop = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PauseBtn")));
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		CButtonUI* btnDelete = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("DeleteBtn")));
		if (btnStart)
		{
			btnStart->SetEnabled(FALSE);
		}
		if (btnStop)
		{
			btnStop->SetEnabled(TRUE);
		}
		if (btnDelete)
		{
			btnDelete->SetEnabled(FALSE);
		}
		int nCulSel = pList->GetCurSel();
		if (m_szCurrentTaskNumVct.size() >= m_szTaskNum)
		{
			std::vector<StreamHandleInfo>::iterator it = std::find_if(m_szTranscodeFileInfoVct.begin(),m_szTranscodeFileInfoVct.end(),std::bind2nd(NodeInfo(),m_szCurrentTaskNumVct[m_szTaskNum-1]));
			int index;
			if (m_szTranscodeFileInfoVct.end() != it)
			{
				index = it-m_szTranscodeFileInfoVct.begin();
				m_szTranscodeFileInfoVct[index].pVideoTrans->isStartRun(FALSE);
				m_szTranscodeFileInfoVct[index].Status =_T("等待");
				m_szTranscodeFileInfoVct[index].uiFileLen=m_szTranscodeFileInfoVct[index].pVideoTrans->m_uiFileLen;
				m_szTranscodeFileInfoVct[index].uiReadLen=m_szTranscodeFileInfoVct[index].pVideoTrans->m_uiReadLen;
				m_szTranscodeFileInfoVct[index].sampleA=m_szTranscodeFileInfoVct[index].pVideoTrans->m_SampleNOA;
				m_szTranscodeFileInfoVct[index].sampleV=m_szTranscodeFileInfoVct[index].pVideoTrans->m_SampleNOV;
				m_szCurrentTaskNumVct.erase(m_szCurrentTaskNumVct.begin() + m_szTaskNum-1);
			}
			CListContainerElementUI* pListElement = (CListContainerElementUI*)pList->GetItemAt(index);
			if (pListElement)
			{
				CHorizontalLayoutUI *pHorizontalLayout = (CHorizontalLayoutUI*)pListElement->GetItemAt(0);
				if (pHorizontalLayout)
				{
					CListHeaderItemUI *pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("StauteItem")));
					int nWidth = pListHeader->GetWidth();
					CLabelUI *pLabel = (CLabelUI *)pHorizontalLayout->GetItemAt(0);
					CDuiString szText;
					szText.Format(_T("%s"),m_szTranscodeFileInfoVct[index].Status);
					pLabel->SetText(szText);
				}
			}
		}
		m_szCurrentTaskNumVct.push_back(m_szTranscodeFileInfoVct[nCulSel]);
		m_szTranscodeFileInfoVct[nCulSel].Status = _T("正在进行");
		m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_uiReadLen = m_szTranscodeFileInfoVct[nCulSel].uiReadLen;
		m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_uiFileLen = m_szTranscodeFileInfoVct[nCulSel].uiFileLen;
		m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_SampleNOA = m_szTranscodeFileInfoVct[nCulSel].sampleA;
		m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_SampleNOV = m_szTranscodeFileInfoVct[nCulSel].sampleV;
		m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->isStartRun(TRUE);
	}
	else if(msg.pSender->GetName() == _T("PauseBtn"))
	{
		CButtonUI* btnStart = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PlayBtn")));
		CButtonUI* btnStop = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PauseBtn")));
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		CButtonUI* btnDelete = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("DeleteBtn")));
		if (btnStart)
		{
			btnStart->SetEnabled(TRUE);
		}
		if (btnStop)
		{
			btnStop->SetEnabled(FALSE);
		}
		if (btnDelete)
		{
			btnDelete->SetEnabled(TRUE);
		}
		int nCulSel = pList->GetCurSel();
		m_szTranscodeFileInfoVct[nCulSel].Status =_T("暂停");
		m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->isStartRun(FALSE);
		m_szTranscodeFileInfoVct[nCulSel].uiReadLen = m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_uiReadLen;
		m_szTranscodeFileInfoVct[nCulSel].uiFileLen = m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_uiFileLen;
		m_szTranscodeFileInfoVct[nCulSel].sampleA = m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_SampleNOA;
		m_szTranscodeFileInfoVct[nCulSel].sampleV = m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->m_SampleNOV;
		CListContainerElementUI* pListElement = (CListContainerElementUI*)pList->GetItemAt(nCulSel);
		if (pListElement)
		{
			CHorizontalLayoutUI *pHorizontalLayout = (CHorizontalLayoutUI*)pListElement->GetItemAt(0);
			if (pHorizontalLayout)
			{
				CListHeaderItemUI *pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("StauteItem")));
				int nWidth = pListHeader->GetWidth();
				CLabelUI *pLabel = (CLabelUI *)pHorizontalLayout->GetItemAt(0);
				CDuiString szText;
				szText.Format(_T("%s"),m_szTranscodeFileInfoVct[nCulSel].Status);
				pLabel->SetText(szText);
			}
		}
		for (int i=0;i<m_szCurrentTaskNumVct.size();i++)
		{
			if (m_szCurrentTaskNumVct[i].strFileNameOut.CompareNoCase(m_szTranscodeFileInfoVct[nCulSel].strFileNameOut)==0)
			{
				m_szCurrentTaskNumVct.erase(m_szCurrentTaskNumVct.begin() + i);
				break;
			}
		}
	}
	else if (msg.pSender->GetName() == _T("DeleteBtn"))
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		int nCulSel = pList->GetCurSel();
		if (nCulSel < 0) return;
		pList->RemoveAt(nCulSel);
		if ( m_szTranscodeFileInfoVct[nCulSel].pVideoTrans != NULL)
		{
			m_szTranscodeFileInfoVct[nCulSel].pVideoTrans->isStartRun(TRUE);
			delete m_szTranscodeFileInfoVct[nCulSel].pVideoTrans;
			m_szTranscodeFileInfoVct[nCulSel].pVideoTrans = NULL;		
		}
		m_szTranscodeFileInfoVct.erase(m_szTranscodeFileInfoVct.begin() + nCulSel);
	
	}
	else if (msg.pSender->GetName() == _T("ListBtn"))
	{
		ShellExecute(m_hWnd,_T("open"),_T(""),_T("nil"),m_szOutPath,1);
	}
	else if (msg.pSender->GetName() == _T("maxbtn"))
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		CButtonUI *btn = static_cast<CButtonUI *>(m_PaintManager.FindControl(_T("maxbtn")));
		if (pList)
		{
			pList->SetBkImage(m_szBKImage2);
			hintWnd = TRUE;
		}
	}
	else if (msg.pSender->GetName() == _T("restorebtn"))
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		if (pList)
		{
			pList->SetBkImage(m_szBKImage1);
			hintWnd = FALSE;
		}
	}
	__super::OnClick(msg);
}

LRESULT CMainFrameWnd::OnDropFile( HDROP hDropInfo )
{
	HDROP m_hdrop = hDropInfo;
	//std::queue<TCHAR *> ll;
	//查询拖入文件个数
	int number = 0;
	number = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	//查询每个文件的名称
	TCHAR szpathName[MAX_PATH];
	CString  pathName;
	CString  fileName;
	DWORD    dwSize;
	std::vector<CString> szInFileName;

	for (int i = 0; i < number; i++)
	{
		dwSize = DragQueryFile(hDropInfo, i, NULL, 0);
		DragQueryFile(hDropInfo, i, szpathName, dwSize + 1);
		pathName = szpathName;
		szInFileName.push_back((pathName));
	}
	int nNumber = 0;
	int nCnt = szInFileName.size();
	while (nCnt > nNumber)
	{
		CString ll = szInFileName[nNumber].Right(4);
		CString ll2 = _T(".avi") ,ll3=_T(".mp4");
		if (ll.CompareNoCase(ll2) == 0)	
		{
			m_szInFileName.push_back(szInFileName[nNumber]);
		}
		nNumber++;
	}
	//完成拖入文件操作，让系统释放缓冲区
	DragFinish(hDropInfo);

	CNewTranscodeWnd *newTranscode = new CNewTranscodeWnd(_T("NewTranscodeWnd.xml"));
	newTranscode ->Create(m_hWnd,_T("NewTranscodeWnd"),UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE);
	m_NewTranscodeHwnd = newTranscode->GetCurrentwndHwnd();
	newTranscode->SetParentHwnd(this->m_hWnd);

	NewTaskAndFilePath taskAndFilePath;
	taskAndFilePath.szOut = m_szOutPath;
	taskAndFilePath.nNum = m_szTempTaskNum;
	::SendMessage(m_NewTranscodeHwnd,CM_MAINTONEWTRANCFILENAME,(WPARAM)&taskAndFilePath,(LPARAM)&m_szInFileName);
	m_szInFileName.clear();
	m_szInFileName.resize(0);
	newTranscode->CenterWindow();
	newTranscode->ShowModal();
	delete newTranscode;
	newTranscode = NULL;
	return 0;
}

unsigned __stdcall CMainFrameWnd::updateTaskListThread( LPVOID lpParam )
{
	if(lpParam != NULL)
	{
		CMainFrameWnd *pMainWnd = (CMainFrameWnd *) lpParam;
		while(1)
		{
			pMainWnd->updateTaskList();
			Sleep(100);
		}	
		delete pMainWnd;
	}
	return 0;
}

void CMainFrameWnd::updateTaskList()
{
	if (m_szCurrentTaskNumVct.size() >= m_szTaskNum)
	{
		return ;
	}
	for (int i = 0; i < m_szTranscodeFileInfoVct.size();i++)
	{
		if ((m_szTranscodeFileInfoVct[i].Status.CompareNoCase(_T("等待")) ==0 && m_szTranscodeFileInfoVct[i].m_Reader!= NULL) &&((m_szTranscodeFileInfoVct[i].Status.CompareNoCase(_T("等待")) ==0 && m_szTranscodeFileInfoVct[i].m_StreamVR != NULL)||(m_szTranscodeFileInfoVct[i].Status.CompareNoCase(_T("等待")) ==0 && m_szTranscodeFileInfoVct[i].m_StreamAR != NULL)) )
		{
			if(m_szTranscodeFileInfoVct[i].pVideoTrans != NULL)
			{

				m_szTranscodeFileInfoVct[i].Status = _T("正在进行");
				m_szTranscodeFileInfoVct[i].pVideoTrans->m_uiReadLen = m_szTranscodeFileInfoVct[i].uiReadLen;
				m_szTranscodeFileInfoVct[i].pVideoTrans->m_uiFileLen = m_szTranscodeFileInfoVct[i].uiFileLen;
				m_szTranscodeFileInfoVct[i].pVideoTrans->m_SampleNOA = m_szTranscodeFileInfoVct[i].sampleA;
				m_szTranscodeFileInfoVct[i].pVideoTrans->m_SampleNOV = m_szTranscodeFileInfoVct[i].sampleV;

				m_szTranscodeFileInfoVct[i].pVideoTrans->isStartRun(TRUE);
				m_szCurrentTaskNumVct.push_back(m_szTranscodeFileInfoVct[i]);
			}
		
			break;
		}
	}
}

LRESULT CMainFrameWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	LRESULT lRes = 0;
	bHandled = FALSE;
	switch(uMsg)
	{
	case WM_DROPFILES: 
		{
			lRes = OnDropFile((HDROP)wParam);
			break;
		}
	case CM_SETOUTPATH:
		{
			m_szTempTaskNum = (CString)(LPCTSTR)wParam;
			m_szOutPath = (CString)(LPCTSTR)lParam;
			break;
		}
	case CM_NEWTRANCTOMAINFILENAME:
		{
			pNewTranscodeInfo pNewTranscode ;
			pNewTranscode = (pNewTranscodeInfo)lParam;
			m_szOutPath = pNewTranscode->szNewFilePath;
			m_szTempFileName = pNewTranscode->szNewFileNameVec;
			m_szTranscodeType = pNewTranscode->szNewTranscodeType;
			m_szTaskNum = _ttoi(pNewTranscode->szTaskNum);
			if (m_szTempFileName.size() !=0 )
			{
				AddMainFileNameToList();
			}
			break;
		}
	case CM_EXITTOMAIN:
		{
			vector <StreamHandleInfo> *NewFileName;
			NewFileName = (vector<StreamHandleInfo> *)lParam;
			//m_szTranscodeFileInfoVct = static_cast<vector <StreamHandleInfo>>(*NewFileName);
			break;
		}
	case  WM_NCLBUTTONDBLCLK:
		{
			if (HTCAPTION == wParam)
			{
				CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
				CButtonUI *maxBtn = static_cast<CButtonUI *>(m_PaintManager.FindControl(_T("maxbtn")));
				CButtonUI *restoreBtn = static_cast<CButtonUI *>(m_PaintManager.FindControl(_T("restorebtn")));
				if (pList&&!hintWnd)
				{
					pList->SetBkImage(m_szBKImage2);
					hintWnd = TRUE;
				}
				else if (pList&&hintWnd)
				{
					pList->SetBkImage(m_szBKImage1);
					hintWnd = FALSE;
				}	
			}
			break;
		}
	case WM_TIMER:
		{
			if (wParam == NS_PROGRESS)
			{
				ChangeProgress();
			}
			break;
		}
	default:
		break;
	}
	return  WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam,bHandled);
}

void CMainFrameWnd::AddMainFileNameToList()
{
	int szSize = m_szCurrentFileName.size();
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
	startTime = clock();
	for (int i=0;i<m_szTempFileName.size();i++)
	{
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		CString ss;
		ss.Format(_T("_%4d%d%d%d%d%d%d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds);

		CString szCurPath ,szInFileName;
		int n = m_szTempFileName[i].GetLength(), nPos;
		nPos = m_szTempFileName[i].ReverseFind('\\');
		szCurPath = m_szTempFileName[i].Right(n-nPos-1);
		int len = szCurPath.GetLength(),pos;
		pos = szCurPath.ReverseFind('.');
		szInFileName = szCurPath.Left(pos);
		szInFileName = szInFileName +ss +m_szTranscodeType;
		m_szInPathFileName.push_back(m_szTempFileName[i]);//输入文件的完整路径名

		CString strFileNameOut;
		if (m_szOutPath.CompareNoCase(_T("C:\\"))==0 || m_szOutPath.CompareNoCase(_T("D:\\"))==0  || m_szOutPath.CompareNoCase(_T("E:\\"))==0 ||m_szOutPath.CompareNoCase(_T("F:\\"))==0)
			strFileNameOut = m_szOutPath + szInFileName;
		else
			strFileNameOut = (m_szOutPath +_T("\\") + szInFileName);
		CListContainerElementUI *pListElement = new CListContainerElementUI;
		pListElement->SetAttribute(_T("height"),_T("30"));
		CHorizontalLayoutUI *pHorizontalLayout = new CHorizontalLayoutUI;
		CLabelUI *pLabel = NULL;
		CDuiString strPos;
		CDuiString strText;
		CListHeaderItemUI *pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("StauteItem")));
		if (pListHeader)
		{
			strText = _T("等待");
			int iWidth = pListHeader->GetWidth();
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("center"));
			pLabel->SetText(strText);
			pHorizontalLayout->Add(pLabel);
		}

		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("FileItem")));
		if (pListHeader)
		{
			strText.Format(_T("%s"),szCurPath);
			int iWidth = pListHeader->GetWidth();
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("left"));
			pLabel->SetAttribute(_T("itemendellipsis"),_T("true"));
			pLabel->SetText(strText);
			pLabel->SetToolTip(strText);
			pHorizontalLayout->Add(pLabel);
		}

		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("ProgressItem")));
		if (pListHeader)
		{
			strText = _T("0%");
			int iWidth = pListHeader->GetWidth();
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("center"));
			pLabel->SetText(strText);
			pHorizontalLayout->Add(pLabel);
		}
		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("TimeItem")));
		if (pListHeader)
		{
			strText = _T("00:00:00");
			int iWidth = pListHeader->GetWidth();
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("center"));
			pLabel->SetText(strText);
			pHorizontalLayout->Add(pLabel);
		}
		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("OutTypeItem")));
		if (pListHeader)
		{
			strText.Format(_T("%s"),m_szTranscodeType);
			int iWidth = pListHeader->GetWidth();
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("center"));

			pLabel->SetText(strText);
			pLabel->SetToolTip(strText);
			pHorizontalLayout->Add(pLabel);
		}

		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("OutFileItem")));
		if (pListHeader)
		{
			strText.Format(_T("%s"),strFileNameOut);
			int iWidth = pListHeader->GetWidth();
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("left"));
			pLabel->SetText(strText);
			pLabel->SetToolTip(strText);
			pHorizontalLayout->Add(pLabel);
		}

		pListElement->Add(pHorizontalLayout);
		pListElement->SetTag(szSize + i);
		if (pListElement != NULL)
		{
			pList->Add(pListElement);
		}

		USES_CONVERSION;
		std::string szTranInFileName(W2A(m_szTempFileName[i]));
		std::string szTranOutFileName(W2A(strFileNameOut));

		StreamHandleInfo szTranscodeInfo;
		szTranscodeInfo.pVideoTrans = new DllManager();
		szTranscodeInfo.pVideoTrans->CAVI2AVI(szTranInFileName.c_str(),szTranOutFileName.c_str(),0.0,0,0,m_hWnd);
		szTranscodeInfo.Status = _T("等待");
		szTranscodeInfo.strFileNameOut=strFileNameOut;
		szTranscodeInfo.uiReadLen = 0.0;
		szTranscodeInfo.uiFileLen = szTranscodeInfo.pVideoTrans->m_uiFileLen;
		szTranscodeInfo.sampleA = szTranscodeInfo.pVideoTrans->m_SampleNOA;
		szTranscodeInfo.sampleV = szTranscodeInfo.pVideoTrans->m_SampleNOV;
		szTranscodeInfo.m_StreamVR = szTranscodeInfo.pVideoTrans->m_phStreamVR;
		szTranscodeInfo.m_StreamAR = szTranscodeInfo.pVideoTrans->m_phStreamAR;
		szTranscodeInfo.m_StreamVW = szTranscodeInfo.pVideoTrans->m_phStreamVW;
		szTranscodeInfo.m_StreamAW = szTranscodeInfo.pVideoTrans->m_phStreamAW;
		szTranscodeInfo.m_Reader = szTranscodeInfo.pVideoTrans->m_phReader;
		szTranscodeInfo.m_Writer = szTranscodeInfo.pVideoTrans->m_phWriter; 
		szTranscodeInfo.strFileNameIn  =  m_szTempFileName[i];
		szTranscodeInfo.Progress = 0;
		szTranscodeInfo.timeShow = 0;
		szTranscodeInfo.strShowFileName = szCurPath;
		szTranscodeInfo.outType = m_szTranscodeType;
		m_szTranscodeFileInfoVct.push_back(szTranscodeInfo);

		m_szStatus.push_back(_T(" 等待"));
		m_szProgress.push_back(0);
		m_szUsedTime.push_back(0);
		m_szOutType.push_back(m_szTranscodeType);
		m_szOutFilePath.push_back(m_szOutPath);
		Sleep(1);
	}
	endTime = clock();
	//double ss = (double)(endTime - startTime) / CLOCKS_PER_SEC ;
	m_szTempFileName.clear();
	m_szTempFileName.resize(0);

	if (!m_szFlag)
	{
		m_szFlag = TRUE;
		if (m_szTranscodeFileInfoVct.empty())
		{
			return;
		}
		else 
		{
			for (int i= 0;i<m_szTaskNum &&i<m_szTranscodeFileInfoVct.size();i++)
			{
				StreamHandleInfo szTranscodeInfo;
				m_szTranscodeFileInfoVct[i].Status =_T("正在进行"); 
				szTranscodeInfo = m_szTranscodeFileInfoVct[i];
				m_szCurrentTaskNumVct.push_back(szTranscodeInfo);
				m_szTranscodeFileInfoVct[i].pVideoTrans->isStartRun(TRUE);
			}
			HANDLE ss = (HANDLE)_beginthreadex(NULL, 0, updateTaskListThread, this, 0, NULL);
			CloseHandle(ss);
		}	
	}	
}

void CMainFrameWnd::ChangeProgress()
{
	int nProgress;
	if (m_szCurrentTaskNumVct.empty())
	{
		return ;
	}
	CButtonUI* btnStop = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("PauseBtn")));
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
	for (int i=0;i<m_szCurrentTaskNumVct.size();i++)
	{
		std::vector<StreamHandleInfo>::iterator it = std::find_if(m_szTranscodeFileInfoVct.begin(),m_szTranscodeFileInfoVct.end(),std::bind2nd(NodeInfo(),m_szCurrentTaskNumVct[i]));
		int index;
		if (m_szTranscodeFileInfoVct.end() != it)
		{
			index = it-m_szTranscodeFileInfoVct.begin();
			m_szTranscodeFileInfoVct[index].pVideoTrans->GetProgress(&nProgress);
			m_szTranscodeFileInfoVct[index].Progress = nProgress;
			if (nProgress >=100)
			{
				m_szTranscodeFileInfoVct[index].Status =_T("完成");
				m_szCurrentTaskNumVct.erase(m_szCurrentTaskNumVct.begin() + i);
				if (btnStop)
				{
					btnStop->SetEnabled(false);
				}
			}
			else
			{
				m_szTranscodeFileInfoVct[index].Status =_T("正在进行");
				m_szTranscodeFileInfoVct[index].timeShow ++;
			}

			CListContainerElementUI* pListElement = (CListContainerElementUI*)pList->GetItemAt(index);
			if (pListElement)
			{
				CHorizontalLayoutUI *pHorizontalLayout = (CHorizontalLayoutUI*)pListElement->GetItemAt(0);
				if (pHorizontalLayout)
				{
					CLabelUI *pLabel = (CLabelUI *)pHorizontalLayout->GetItemAt(0);
					CDuiString szText;
					szText.Format(_T("%s"),m_szTranscodeFileInfoVct[index].Status);
					pLabel->SetText(szText);

					pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(1);
					szText.Format(_T("%s"),m_szTranscodeFileInfoVct[index].strShowFileName);
					pLabel->SetText(szText);

					pLabel = (CLabelUI *)pHorizontalLayout->GetItemAt(2);
					szText.Format(_T("%d"),m_szTranscodeFileInfoVct[index].Progress);
					CString centent = _T("%");
					szText += centent;
					pLabel->SetText(szText);

					pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(3);
					int mill = m_szTranscodeFileInfoVct[index].timeShow;
					CString hour,minute,sec ,szTime;
					hour.Format(_T("%02d:"),mill/3600);
					minute.Format(_T("%02d:"),(mill%3600)/60);
					sec.Format(_T("%02d"),(mill%3600%60));
					szTime = hour+minute +sec;
					pLabel->SetText(szTime);

					pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(4);
					szText.Format(_T("%s"),m_szTranscodeFileInfoVct[index].outType);
					pLabel->SetText(szText);

					pLabel =(CLabelUI *) pHorizontalLayout->GetItemAt(5);
					szText.Format(_T("%s"),m_szTranscodeFileInfoVct[index].strFileNameOut);
					pLabel->SetText(szText);
				}
			}
		}
	}
}


