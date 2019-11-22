
// Lewis_Lam_Chat_AppDlg.h : header file
//

#pragma once


// CLewisLamChatAppDlg dialog
class CLewisLamChatAppDlg : public CDialogEx
{
// Construction
public:
	CLewisLamChatAppDlg(CWnd* pParent = nullptr);	// standard constructor
	static UINT __cdecl Listening(LPVOID pParam);
	UINT __cdecl ListeningProc();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LEWIS_LAM_CHAT_APP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSend();
	afx_msg void OnEnChangeChatBox();
	afx_msg void OnEnChangeInputBox();
	afx_msg void OnEnKillfocusChatBox();
};
