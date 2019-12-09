#pragma once
class DllManager;

typedef struct newTranscodeInfo
{
	newTranscodeInfo()
	{
	   szNewFilePath = _T("");
	   szNewTranscodeType=_T("");
	  szNewFileNameVec.clear();
	}
	vector<CString> szNewFileNameVec;
	CString szNewFilePath;
	CString szNewTranscodeType;
	CString szTaskNum;
}NewTranscodeInfo,*pNewTranscodeInfo;

typedef struct newTaskAndFilePath
{
	newTaskAndFilePath()
	{
		szOut = _T("");
		nNum = _T("");
	}
	CString szOut;
	CString nNum;
}NewTaskAndFilePath,*pNewTaskAndFilePath;

class CNewTranscodeWnd: 
	public CXMLBaseWnd,public IListCallbackUI
{
public:
	CNewTranscodeWnd(LPCTSTR pszXMLPath);
	~CNewTranscodeWnd(void);

	HWND GetCurrentwndHwnd(){return this->m_hWnd;}
	void SetParentHwnd(HWND hwnd){m_hParentHwnd = hwnd;}

protected:
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//virtual void OnFinalMessage(HWND hWnd);

	double GetDiskSize(CString szFileName);
	void AddFileNameToList();

	static unsigned __stdcall isTranscodeSuccThread( LPVOID lpParam );
	bool isTranscodeFun(const char  *szInFileName);

private:
	vector<CString> m_szNewFileName;
	vector<CString> m_szTempNewFileName;
	vector<CString> m_szIsTrancodeSucc;
	vector<CString> m_szInFileName;

	HWND m_hParentHwnd;
	CString m_szNewOutPath;
	CString m_szOutType;
	CWndShadow m_WndShadow;
	CString m_szTask;

	CAVIREADER_HANDLE m_phReader;
	CAVIREADER_STREAMHANDLE m_phStreamVR;
	CAVIREADER_STREAMHANDLE m_phStreamAR;
	int m_pListFlag ;

};
