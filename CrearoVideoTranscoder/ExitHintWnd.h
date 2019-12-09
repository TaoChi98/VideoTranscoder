#pragma once

class CExitHintWnd:
	public CXMLBaseWnd
{
public:
	CExitHintWnd(LPCTSTR pszXMLPath);
	~CExitHintWnd(void);
	void GetParaentWndHwnd(HWND pParentHwnd){m_pHwnd = pParentHwnd;}
	HWND SetWndHwnd(){return this->m_hWnd;}

protected:
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);	
	virtual void InitWindow();
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//virtual void OnFinalMessage(HWND hWnd);

private:
	CWndShadow m_WndShadow;
	HWND m_pHwnd;
	std::vector<StreamHandleInfo > m_szTransFileInfoVct;
};
