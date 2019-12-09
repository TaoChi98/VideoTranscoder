#pragma once

class CParamSetWnd:
	public CXMLBaseWnd
{
public:
	CParamSetWnd(LPCTSTR pszXMLPath);
	~CParamSetWnd(void);

	void GetParaentWndHwnd(HWND pParentHwnd){m_pHwnd = pParentHwnd;}
	HWND SetCurrentWndHwnd(){return this->m_hWnd;}

protected:
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);	
	virtual void InitWindow();
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//virtual void OnFinalMessage(HWND hWnd);

private:
	HWND m_pHwnd;
	CString m_szOutPath;
	CWndShadow m_WndShadow;
	int m_szComTaskNum;
};
