#include "StdAfx.h"
#include "ExitHintWnd.h"
#include "DllManager.h"

CExitHintWnd::CExitHintWnd(LPCTSTR pszXMLPath):
CXMLBaseWnd(pszXMLPath)
{
}

CExitHintWnd::~CExitHintWnd(void)
{
}

void CExitHintWnd::Notify( TNotifyUI& msg )
{
	__super::Notify(msg);
}

void CExitHintWnd::OnClick( TNotifyUI& msg )
{
	if (msg.pSender->GetName() == _T("DerBtn"))
	{
		for (int i = 0 ;i < m_szTransFileInfoVct.size() ;i++)
		{
			if (m_szTransFileInfoVct[i].m_Reader != NULL)
			{
				m_szTransFileInfoVct[i].pVideoTrans->CloseFile();
				m_szTransFileInfoVct[i].m_Reader  = NULL;
			}
		}
		Sleep(1000);
	//	::SendMessage(m_pHwnd,CM_EXITTOMAIN,0,(LPARAM)&m_szTransFileInfoVct);
		PostQuitMessage(0L);
	}
	else if (msg.pSender->GetName() == _T("CloseBtn"))
	{
		Close();
	}
	__super::OnClick(msg);
}

void CExitHintWnd::InitWindow()
{
	m_szTransFileInfoVct.empty();
	m_szTransFileInfoVct.clear();
}

LRESULT CExitHintWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	__super::OnCreate(uMsg,wParam,lParam,bHandled);
	m_WndShadow.Create(GetHWND());
	m_WndShadow.SetSize(4);  
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetColor(RGB(0xA6, 0xA6, 0xA6));
	return 0;
}

LRESULT CExitHintWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	bHandled = FALSE;
	switch(uMsg)
	{
	case  CM_MAINTOEXIT:
		{
			vector <StreamHandleInfo> *NewFileName;
			NewFileName = (vector<StreamHandleInfo> *)lParam;
			m_szTransFileInfoVct = static_cast<vector <StreamHandleInfo>>(*NewFileName);
			break;
		}
	}
	return  WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam,bHandled);
}
