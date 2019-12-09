#pragma  once
#include "StdAfx.h"
#include <iostream>
#include "DllManager.h"
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include "NewTranscodeWnd.h"
using namespace std;

CNewTranscodeWnd::CNewTranscodeWnd(LPCTSTR pszXMLPath)
:CXMLBaseWnd(pszXMLPath)
{
	m_szNewFileName.clear();
	m_szNewFileName.resize(0);
	m_szTempNewFileName.clear();
	m_szTempNewFileName.resize(0);
	m_szIsTrancodeSucc.clear();
	m_szIsTrancodeSucc.resize(0);
	m_szInFileName.clear();
	m_szInFileName.resize(0);
	m_szOutType = _T(".avi");
	m_phReader = NULL;
    m_phStreamAR = NULL;
	m_phStreamVR = NULL;
}

CNewTranscodeWnd::~CNewTranscodeWnd(void)
{
}

void CNewTranscodeWnd::Notify( TNotifyUI& msg )
{
	if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName() == _T("TranscodeType"))
		{
			CComboUI *pCombo =  static_cast< CComboUI *>(m_PaintManager.FindControl(_T("TranscodeType")));
			if (pCombo)
			{
				int nCurSel = pCombo->GetCurSel();
				if(nCurSel == 0) m_szOutType  = _T(".avi");
				else m_szOutType = _T(".mp4");
			}	
		}
	}
	__super::Notify(msg);
}

void CNewTranscodeWnd::OnClick( TNotifyUI& msg )
{
	if (msg.pSender->GetName() == _T("NewCancleBtn"))
	{
		Close();
		m_szNewFileName.clear();
		m_szTempNewFileName.clear();
		m_szIsTrancodeSucc.clear();
		m_szInFileName.clear();
	}
	else if(msg.pSender->GetName() == _T("NewDerBtn"))
	{
		CEditUI *szOutFile = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("FilePath")));
		if (szOutFile)
		{
			::DragAcceptFiles(this->m_hWnd,true);
			CreateDirectory(szOutFile->GetText(),NULL);
			if (!PathIsDirectory(szOutFile->GetText()))
			{
				::CreateDirectory(szOutFile->GetText(),NULL);
			}
		}
		NewTranscodeInfo newTranscodeSendInfo;
		newTranscodeSendInfo.szNewFileNameVec=m_szInFileName;
		newTranscodeSendInfo.szNewFilePath = szOutFile->GetText();
		newTranscodeSendInfo.szNewTranscodeType = m_szOutType;
		newTranscodeSendInfo.szTaskNum = m_szTask;
		::SendMessage(m_hParentHwnd,CM_NEWTRANCTOMAINFILENAME,0,(LPARAM)&newTranscodeSendInfo);
		Close();
		m_szNewFileName.clear();
		m_szTempNewFileName.clear();
		m_szIsTrancodeSucc.clear();
		m_szInFileName.clear();
	}
	else if(msg.pSender->GetName() == _T("NewDeleteBtn"))
	{
		CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("newdomainlist")));
		int nCulSel = pList->GetCurSel();
		if (nCulSel < 0) return;
		pList->RemoveAt(nCulSel);
		if (m_szIsTrancodeSucc[nCulSel].CompareNoCase(_T("能"))==0)
		{
			int uiStartTimeIndex;
			std::vector<CString>::iterator itStart =std::find(m_szInFileName.begin(),m_szInFileName.end(),m_szNewFileName[nCulSel]);
			if (m_szInFileName.end() != itStart)
			{
				uiStartTimeIndex = itStart - m_szInFileName.begin();
			}
			m_szInFileName.erase(m_szInFileName.begin()+uiStartTimeIndex);
		}
		m_szNewFileName.erase(m_szNewFileName.begin() + nCulSel);
		m_szTempNewFileName.erase(m_szTempNewFileName.begin() + nCulSel);
		m_szIsTrancodeSucc.erase(m_szIsTrancodeSucc.begin()+nCulSel);
	}
	else if (msg.pSender->GetName() == _T("NewAddBtn"))
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
				m_szNewFileName.push_back((cstrfilpath));
			}
		}
		AddFileNameToList();
		delete []pBuffer;
	}
	else if(msg.pSender->GetName() == _T("openFilePathBtn"))
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
			m_szNewOutPath = szBuffer;
		}
		CLabelUI * sz_pDiskSize= static_cast<CLabelUI *>(m_PaintManager.FindControl(_T("DiskSize")));
		CEditUI *szOutFile = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("FilePath")));
		if (szOutFile)
		{
			szOutFile->SetText(m_szNewOutPath);
			szOutFile->SetToolTip(m_szNewOutPath);
		}
		double ss = GetDiskSize(m_szNewOutPath);
		CString szDiskSize;
		szDiskSize.Format(_T("可用空间:%.2fG"),ss);
		if (sz_pDiskSize)
		{
			sz_pDiskSize->SetText(szDiskSize);
		}
	}
	__super::OnClick(msg);
}

LRESULT CNewTranscodeWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	switch(uMsg)
	{
	case CM_MAINTONEWTRANCFILENAME:
		{
			vector <CString> *NewFileName;
			NewFileName = (vector<CString> *)lParam;
			m_szNewFileName = static_cast<vector <CString>>(*NewFileName);
			pNewTaskAndFilePath taskAndFilePath ;
			taskAndFilePath = (pNewTaskAndFilePath)wParam;
			m_szNewOutPath = taskAndFilePath->szOut;
			m_szTask = taskAndFilePath->nNum;
			CEditUI * szFilePath = static_cast<CEditUI *>(m_PaintManager.FindControl(_T("FilePath")));
			if (szFilePath)
			{
				szFilePath->SetText(m_szNewOutPath);
				szFilePath->SetToolTip(m_szNewOutPath);
			}
			CLabelUI * sz_pDiskSize= static_cast<CLabelUI *>(m_PaintManager.FindControl(_T("DiskSize")));
			double ss = GetDiskSize(m_szNewOutPath);
			CString szDiskSize;
			szDiskSize.Format(_T("可用空间:%.2fG"),ss);
			if (sz_pDiskSize)
			{
				sz_pDiskSize->SetText(szDiskSize);
			}
			AddFileNameToList();
			break;
		}
	}
	return  WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam,bHandled);
}

void CNewTranscodeWnd::InitWindow()
{
	
}

LPCTSTR CNewTranscodeWnd::GetItemText( CControlUI* pControl, int iIndex, int iSubItem )
{
	/*TCHAR szBuf[MAX_PATH] = {0};
	switch(iSubItem)
	{
	case 0:
		{
			_stprintf_s(szBuf,m_szNewFileName[iIndex]);
			break;
		}
	case 1:
		{
			CString ss=_T("") ;
			ss.Format(_T("            %s"),m_szIsTrancodeSucc[iIndex]);
			_stprintf_s(szBuf,ss);
			break;
		}
	case 2:
		{
			_stprintf_s(szBuf,_T("            无"));
			break;	
		}	
	}
	pControl->SetUserData(szBuf);
	return pControl->GetUserData();
	*/
	return 0;
}

LRESULT CNewTranscodeWnd::OnCreate( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	__super::OnCreate(uMsg,wParam,lParam,bHandled);
	m_WndShadow.Create(GetHWND());
	m_WndShadow.SetSize(4);  
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetColor(RGB(0xA6, 0xA6, 0xA6));
	return 0;
}

double CNewTranscodeWnd::GetDiskSize( CString szFileName )
{
	CString firstchar = szFileName.Mid(0,1);
	CString strRootPath = firstchar + L":\\";
	DWORD dwSectorsPerCluster=0;
	DWORD dwBytesPerSector = 0;
	DWORD dwFreeClusters = 0;
	DWORD dwTotalClusters = 0;
	if (GetDiskFreeSpace(strRootPath,&dwSectorsPerCluster,
		&dwBytesPerSector,&dwFreeClusters,&dwTotalClusters))
	{
		double dd = dwSectorsPerCluster *dwBytesPerSector/(1024.*1024.);
		dd = dd / 1024;
		double m_dFree = dwFreeClusters *dd;
		return m_dFree;
	}
	return 0.0;
}

void CNewTranscodeWnd::AddFileNameToList()
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("newdomainlist")));
	for (int i = m_szTempNewFileName.size();i < m_szNewFileName.size();i++)
	{
		USES_CONVERSION;
		std::string szTranInFileName(W2A(m_szNewFileName[i]));
		bool isTranscodeSucc =isTranscodeFun(szTranInFileName.c_str());
		if (isTranscodeSucc)
		{
			m_szInFileName.push_back(m_szNewFileName[i]);
		}

		CListContainerElementUI *pListElement = new CListContainerElementUI;
		pListElement->SetAttribute(_T("height"),_T("30"));
		CHorizontalLayoutUI *pHorizontalLayout = new CHorizontalLayoutUI;
		CLabelUI *pLabel = NULL;
		CDuiString strPos;
		CDuiString strText;
		CListHeaderItemUI *pListHeader= static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("srcFiles")));
		if (pListHeader)
		{
			strText = m_szNewFileName[i];
			int iWidth = 280;
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("left"));
			pLabel->SetToolTip(strText);
			pLabel->SetText(strText);
			pHorizontalLayout->Add(pLabel);
		}
		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("isTranc")));
		if (pListHeader)
		{
			strText.Format(_T("%s"),m_szIsTrancodeSucc[i]);
			int iWidth = 110;
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("center"));
			pLabel->SetText(strText);
			pHorizontalLayout->Add(pLabel);
		}
		pListHeader = static_cast<CListHeaderItemUI *>(m_PaintManager.FindControl(_T("highSet")));
		if (pListHeader)
		{
			strText = _T("无");
			int iWidth = 110;
			strPos.Format(_T("%d"),iWidth);
			pLabel = new CLabelUI;
			pLabel->SetAttribute(_T("width"),strPos);
			pLabel->SetAttribute(_T("align"),_T("center"));
			pLabel->SetText(strText);
			pHorizontalLayout->Add(pLabel);
		}
		if (pListElement!=NULL)
		{
			pListElement->Add(pHorizontalLayout);
			pListElement->SetTag(i);
			pList->Add(pListElement);
		}
		Sleep(1);
	}
	m_szTempNewFileName = m_szNewFileName;
	return ;
}

unsigned __stdcall CNewTranscodeWnd::isTranscodeSuccThread( LPVOID lpParam )
{

	return 0;
}

bool CNewTranscodeWnd::isTranscodeFun(const char *szInFileName)
{
	FILE *fp = fopen(szInFileName, ("rb+"));
//	fseek(fp, 0, SEEK_END);
	if(fp !=NULL)
	{
		fclose(fp);
	}
	int rv = CAVIReader_OpenFile(&m_phReader, szInFileName);
	if (rv != 0)
	{
		m_szIsTrancodeSucc.push_back(_T("否"));
		if (m_phReader != NULL)
		{
			CAVIReader_Close(m_phReader);
			m_phReader = NULL;
		}
		return false;
	}
	rv = CAVIReader_GetStream(m_phReader, &m_phStreamAR, CAVIREADER_streamtypeAUDIO, 0);
	int ra = CAVIReader_GetStream(m_phReader, &m_phStreamVR, CAVIREADER_streamtypeVIDEO, 0);
	if (rv == 0 || ra == 0)
	{
		if (ra == 0)
		{
			TCAVIReaderStreamHeader sinfoV;
			ra = CAVIReader_GetStreamInfo(m_phStreamVR, &sinfoV);
			int siz = sizeof(sinfoV);
			if (sinfoV.fccHandler == 0x34363248)
			{
				m_szIsTrancodeSucc.push_back(_T("否"));
				if (m_phReader != NULL)
				{
					CAVIReader_Close(m_phReader);
					m_phReader = NULL;
					m_phStreamVR = NULL;
					m_phStreamAR = NULL;
				}
				return false;
			}
			m_szIsTrancodeSucc.push_back(_T("能"));
			if (m_phReader != NULL)
			{
				CAVIReader_Close(m_phReader);
				m_phReader = NULL;
				m_phStreamVR = NULL;
				m_phStreamAR = NULL;
			}
		}
		else
		{
			TCAVIReaderStreamHeader sinfoA;
			rv = CAVIReader_GetStreamInfo(m_phStreamAR, &sinfoA);
			int ss = sizeof(sinfoA);
			if (sinfoA.dwScale == 1 && sinfoA.dwRate == 8000 && sinfoA.fccHandler == 1)
			{
				m_szIsTrancodeSucc.push_back(_T("否"));
				if (m_phReader != NULL)
				{
					CAVIReader_Close(m_phReader);
					m_phReader = NULL;
					m_phStreamAR = NULL;
					m_phStreamVR = NULL;
				}
				return false;
			}
			m_szIsTrancodeSucc.push_back(_T("能"));
			if (m_phReader != NULL)
			{
				CAVIReader_Close(m_phReader);
				m_phReader = NULL;
				m_phStreamAR = NULL;
				m_phStreamVR = NULL;
			}
		}	
	}
	else
	{
		m_szIsTrancodeSucc.push_back(_T("否"));
		if (m_phReader != NULL)
		{
			CAVIReader_Close(m_phReader);
			m_phReader = NULL;
			m_phStreamAR = NULL;
			m_phStreamVR = NULL;
		}
		return false;
	}
	return true;
}

