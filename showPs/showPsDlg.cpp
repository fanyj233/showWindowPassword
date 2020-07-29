
// showPsDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "showPs.h"
#include "showPsDlg.h"
#include "afxdialogex.h"
#include <map>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int num;
std::map<int, HWND>hwndMap;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CshowPsDlg 对话框



CshowPsDlg::CshowPsDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOWPS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CshowPsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list1);
}

BEGIN_MESSAGE_MAP(CshowPsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CshowPsDlg::OnBnClickedButton1)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEACTIVATE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CshowPsDlg 消息处理程序

BOOL CshowPsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_list1.InsertColumn(0, _T("hwnd"), LVCFMT_LEFT, 50);
	m_list1.InsertColumn(1, _T("x"), LVCFMT_LEFT, 50);
	m_list1.InsertColumn(2, _T("y"), LVCFMT_LEFT, 50);
	m_list1.InsertColumn(3, _T("text"), LVCFMT_LEFT, 550);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CshowPsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CshowPsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CshowPsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CshowPsDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetCapture();
}


BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam)
{
	hwndMap[num] = hWndChild;
	num++;
	return TRUE;
}

void CshowPsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetCursorPos(&point);
	ScreenToClient(&point);
	CRect   rect;
	AfxGetMainWnd()->GetWindowRect(&rect);
	ScreenToClient(&rect);
	ReleaseCapture();
	if (rect.PtInRect(point)) //判断是否在本应用程序窗口内
	{
		CDialogEx::OnLButtonDown(nFlags, point);
		return;
	}
	GetCursorPos(&point);
	CWnd* targetHwnd = WindowFromPoint(point);//获取句柄

	{
		num = 0;
		m_list1.DeleteAllItems();
		CString str;
		hwndMap.clear();
		EnumChildWindows(targetHwnd->GetSafeHwnd(), EnumChildProc, NULL);
		CWnd* pChildWnd;
		CRect rect;
		TCHAR  tch[255];
		for (num = 0; hwndMap[num]; num++)
		{
			str.Format(_T("%x"), (DWORD)hwndMap[num]);
			m_list1.InsertItem(num, str);
			pChildWnd = CWnd::FromHandle(hwndMap[num]);
			pChildWnd->GetWindowRect(&rect);
			targetHwnd->ScreenToClient(&rect);
			str.Format(_T("%d"), rect.left);
			m_list1.SetItemText(num, 1, str);

			str.Format(_T("%d"), rect.top);
			m_list1.SetItemText(num, 2, str);
			LONG dwStyle = GetWindowLong(hwndMap[num], GWL_STYLE);
			if (dwStyle & ES_PASSWORD)
			{
				::SetWindowWord(hwndMap[num], GWL_STYLE, 0);
				::SetWindowLong(hwndMap[num], GWL_STYLE, dwStyle & ~ES_PASSWORD);
			}
			::SendMessage(hwndMap[num], WM_GETTEXT, (WPARAM)255, (LPARAM)tch);
			str = tch;

			m_list1.SetItemText(num, 3, str);
		}
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
