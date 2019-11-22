
// Lewis_Lam_Chat_AppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lewis_Lam_Chat_App.h"
#include "Lewis_Lam_Chat_AppDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <WinSock2.h>
#include <ws2tcpip.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::thread;

#ifdef _DEBUG

#pragma comment(lib, "Ws2_32.lib")

#define SEND_PORT	3515
#define RECEIVE_PORT    3514
#define BUFFER	256
#define IP      "127.0.0.1"
#define new DEBUG_NEW

#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLewisLamChatAppDlg dialog



CLewisLamChatAppDlg::CLewisLamChatAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LEWIS_LAM_CHAT_APP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLewisLamChatAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLewisLamChatAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CLewisLamChatAppDlg::OnBnClickedSend)
	ON_EN_CHANGE(IDC_CHAT_BOX, &CLewisLamChatAppDlg::OnEnChangeChatBox)
	ON_EN_CHANGE(IDC_INPUT_BOX, &CLewisLamChatAppDlg::OnEnChangeInputBox)
END_MESSAGE_MAP()


// CLewisLamChatAppDlg message handlers

BOOL CLewisLamChatAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	AfxBeginThread(Listening, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLewisLamChatAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLewisLamChatAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLewisLamChatAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLewisLamChatAppDlg::OnBnClickedSend()
{
	CString inputMessage;
	CString history;
	CString empty = CString(_T(""));
	GetDlgItemText(IDC_INPUT_BOX, inputMessage);
	GetDlgItemText(IDC_CHAT_BOX, history);
	SetDlgItemText(IDC_CHAT_BOX, history + _T("Me: ") + inputMessage + _T("\r\n"));
	SetDlgItemText(IDC_INPUT_BOX, empty);

	sockaddr_in destAddr, srcAddr;
	int start;
	SOCKET sockSend;

	WSADATA wsaData;

	char message[BUFFER];
	strcpy_s(message, CStringA(inputMessage).GetString());



	start = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (start != 0) {
		cout << "WSAStartup Failed With Error: " << start << endl;
		exit(1);
	}

	if ((sockSend = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	destAddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &destAddr.sin_addr.s_addr);
	destAddr.sin_port = htons(SEND_PORT);

	srcAddr.sin_family = AF_INET;
	inet_pton(AF_INET, IP, &srcAddr.sin_addr.s_addr);
	srcAddr.sin_port = htons(RECEIVE_PORT);

	if (bind(sockSend, (struct sockaddr*) & srcAddr, sizeof(srcAddr)) < 0) {
		perror("send bind");
		exit(1);
	}

	if (sendto(sockSend, message, strlen(message) + 1, 0, (struct sockaddr*) & destAddr, sizeof(destAddr)) < 0) {
		perror("sendto");
		exit(1);
	}
	start = closesocket(sockSend);
	// TODO: Add your control notification handler code here
}

void CLewisLamChatAppDlg::OnEnChangeInputBox()
{


	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CLewisLamChatAppDlg::OnEnChangeChatBox()
{
	
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

UINT __cdecl CLewisLamChatAppDlg::Listening(LPVOID pParam) {
	ASSERT(pParam != NULL);
	CLewisLamChatAppDlg* pThis = reinterpret_cast<CLewisLamChatAppDlg*>(pParam);
	return pThis->ListeningProc();
}

UINT __cdecl CLewisLamChatAppDlg::ListeningProc() {
	while (true) {
		CString history;
		CString empty = CString(_T(""));

		sockaddr_in destAddr, srcAddr;
		socklen_t srcLen = sizeof(srcAddr);
		int start;
		SOCKET sockReceive;
		char received[BUFFER];
		WSADATA wsaData;
		CString output;

		start = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (start != 0) {
			cout << "WSAStartup Failed With Error: " << start << endl;
			exit(1);
		}

		if ((sockReceive = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("socket");
			exit(1);
		}

		memset(&srcAddr, 0, sizeof(srcAddr));
		srcAddr.sin_family = AF_INET;
		inet_pton(AF_INET, IP, &destAddr.sin_addr.s_addr);
		srcAddr.sin_port = htons(RECEIVE_PORT);

		if (bind(sockReceive, (struct sockaddr*) & srcAddr, sizeof(srcAddr)) < 0) {
			perror("receive bind");
			exit(1);
		}

		if (recvfrom(sockReceive, received, sizeof(received), 0, (struct sockaddr*) & srcAddr, &srcLen) < 0) {
			perror("recvfrom");
			exit(1);
		}

		output = CString(received);
		GetDlgItemText(IDC_CHAT_BOX, history);
		SetDlgItemText(IDC_CHAT_BOX, history + _T(IP": ") + output + _T("\r\n"));


		start = closesocket(sockReceive);
	}
}
