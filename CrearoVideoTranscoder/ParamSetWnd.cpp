#include "StdAfx.h"
#include "ParamSetWnd.h"



CParamSetWnd::CParamSetWnd( LPCTSTR pszXMLPath ):
CXMLBaseWnd(pszXMLPath)
{
}

CParamSetWnd::~CParamSetWnd(void)
{
}

void CParamSetWnd::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName() == _T("RunTaskNum"))
		{
			CComboUI *pCombo =  static_cast< CComboUI *>(m_PaintManager.FindControl(_T("RunTaskNum")));
			if (pCombo)
			{
				int nCurSel = pCombo->GetCurSel();
				if(nCurSel == 0) m_szComTaskNum =1;
				else if(nCurSel == 1) m_szComTaskNum = 2;
				else if(nCurSel ==2) m_szComTaskNum = 3;
				else if(nCurSel == 3) m_szComTaskNum = 4;
				else if (nCurSel == 4) m_szComTaskNum = 5;
			}
		}
	}
	__super::Notify(msg);
}

void CParamSetWnd::OnClick( TNotifyUI& msg )
{
	if (msg.pSender->GetName() == _T("SetOpenFilePathBtn"))
	{
		TCHAR szBuffer[MAX_PATH] = { 0 };
		BROWSEINFO bi = { 0 };
		bi.hwndOwner = NULL;
		bi.pszDisplayName = szBuffer;
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (idl)
		{
			SHGetPathFromIDList(idl,szBuffer);
			m_szOutPath = szBuffer;
		}
		CEditUI *szOutFile = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("SetFilePath")));
		if (szOutFile)
		{
			szOutFile->SetText(m_szOutPath);
			szOutFile->SetToolTip(m_szOutPath);
		}
	}
	else if (msg.pSender->GetName() == _T("TaskDerBtn"))
	{
		CString dd ;
		dd.Format(_T("%d"),m_szComTaskNum);
		CEditUI *szOutFile = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("SetFilePath")));
		if (szOutFile)
		{
			::DragAcceptFiles(this->m_hWnd,true);
			CreateDirectory(szOutFile->GetText(),NULL);
			if (!PathIsDirectory(szOutFile->GetText()))
			{
				::CreateDirectory(szOutFile->GetText(),NULL);
			}
			::SendMessage(m_pHwnd,CM_SETOUTPATH,(WPARAM)(LPCTSTR)dd,(LPARAM)(LPCTSTR)(szOutFile->GetText()));
		}
		Close();
	}
	else if (msg.pSender->GetName() == _T("TaskCancleBtn"))
	{
		Close();
	}
	__super::OnClick(msg);
}

void CParamSetWnd::InitWindow()
{
	m_szComTaskNum = 2;
}

LRESULT CParamSetWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(uMsg)
	{
	case CM_MAINTOSETWND:
		{
			m_szOutPath = (CString)(LPCTSTR)lParam;
			CEditUI *szOutFile = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("SetFilePath")));
			if (szOutFile)
			{
				szOutFile->SetText(m_szOutPath);
				szOutFile->SetToolTip(m_szOutPath);
			}
			CComboUI *pCombo =  static_cast< CComboUI *>(m_PaintManager.FindControl(_T("RunTaskNum")));
			m_szComTaskNum=_ttoi((CString)(LPCTSTR)wParam);
			if (pCombo)
			{
				pCombo->SelectItem(m_szComTaskNum-1);
			}
			break;
		}
	}
	return  WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam,bHandled);
}

LRESULT CParamSetWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	__super::OnCreate(uMsg,wParam,lParam,bHandled);
	m_WndShadow.Create(GetHWND());
	m_WndShadow.SetSize(4);  
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetColor(RGB(0xA6, 0xA6, 0xA6));
	return 0;
}


