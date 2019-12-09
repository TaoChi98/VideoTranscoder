#pragma once
#include <queue>
#include <deque>
#include <vector>
#include <map>
#include "CR_VA_Stream_Info.h"

class DllManager;
class CExitHintWnd;
class CNewTranscodeWnd;
class CParamSetWnd;

struct NodeInfo:public std::binary_function<Node ,Node ,bool>
{

	bool operator () ( const Node &index1,const Node &index2) const
	{
		return index1.strFileNameOut == index2.strFileNameOut;
	}
};

class CMainFrameWnd:
	public CXMLBaseWnd,public IListCallbackUI
{
public:
	explicit CMainFrameWnd(LPCTSTR pszXMLPath);
	~CMainFrameWnd(void);

protected:
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	virtual LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void AddMainFileNameToList();
	void ChangeProgress();
	LRESULT OnDropFile(HDROP hDropInfo);

	static unsigned __stdcall updateTaskListThread( LPVOID lpParam );
    void updateTaskList();
	bool OnListSize(void *p);

private:
	CString m_szOutPath;
	HWND m_NewTranscodeHwnd;
	vector<CString> m_szInFileName;
	vector<CString> m_szTempFileName;
	vector<CString> m_szInPathFileName;
	vector<CString> m_szCurrentFileName;
	CString m_szTranscodeType;
	vector<CString> m_szOutFileName;
	vector<CString> m_szStatus;
	vector<int> m_szProgress;
	vector<int> m_szUsedTime;
	vector<CString> m_szOutType;
	vector<CString> m_szOutFilePath;
	std::vector<StreamHandleInfo > m_szStreamHandleInfo;

	vector<StreamHandleInfo> m_szTranscodeFileInfoVct;
	vector<StreamHandleInfo> m_szCurrentTaskNumVct;

	BOOL m_szFlag;
	queue<int> m_szQue;

	int m_nIndex;
	CDuiString m_szBKImage1;
	CDuiString m_szBKImage2;
	bool hintWnd;
	int m_szTaskNum;
	CString m_szTempTaskNum;
	CWndShadow m_WndShadow;
	clock_t startTime,endTime;
	/*CParamSetWnd *paramSet;
	CExitHintWnd *pExitHint;
	CNewTranscodeWnd *newTranscode;
	*/
};
