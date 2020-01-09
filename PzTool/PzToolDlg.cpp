
// PzToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PzTool.h"
#include "PzToolDlg.h"
#include "afxdialogex.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CPzToolDlg 对话框



CPzToolDlg::CPzToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PZTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_fontID = NULL;
	m_bInit = NULL;
}

void CPzToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_E_ID, m_sta_id);
}

BEGIN_MESSAGE_MAP(CPzToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_CONFIG, &CPzToolDlg::OnBnClickedBtnConfig)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CPzToolDlg::OnBnClickedBtnSave)
	ON_MESSAGE(WM_UPDATEWND, OnUpdateWnd)
	ON_BN_CLICKED(IDOK, &CPzToolDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_UP, &CPzToolDlg::OnBnClickedBtnUp)
	ON_BN_CLICKED(IDC_BTN_DOWN, &CPzToolDlg::OnBnClickedBtnDown)
	ON_BN_CLICKED(IDC_BTN_LEFT, &CPzToolDlg::OnBnClickedBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, &CPzToolDlg::OnBnClickedBtnRight)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPzToolDlg 消息处理程序

BOOL CPzToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	AddMoveCtrl();
	SetFont();
	SetCtlValue();
	MoveCtl();
	NoMoveCtlLayOut();
	m_bInit = true;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPzToolDlg::AddMoveCtrl()
{
	m_movectrl.Initialize(m_hWnd);

	ST_YS st;
	st.id = IDC_E_KF; st.str = "KF"; st.str2 = "kf";
	m_vt.push_back(st);
	st.id = IDC_E_YEAR; st.str = "YEAR"; st.str2 = "year";
	m_vt.push_back(st);
	st.id = IDC_E_MONTH; st.str = "MONTH"; st.str2 = "month";
	m_vt.push_back(st);
	st.id = IDC_E_DAY; st.str = "DAY"; st.str2 = "day";
	m_vt.push_back(st);
	st.id = IDC_E_Z1; st.str = "Z1"; st.str2 = "z1";
	m_vt.push_back(st);
	st.id = IDC_E_H1; st.str = "H1"; st.str2 = "h1";
	m_vt.push_back(st);
	st.id = IDC_E_Z2; st.str = "Z2"; st.str2 = "z2";
	m_vt.push_back(st);
	st.id = IDC_E_H2; st.str = "H2"; st.str2 = "h2";
	m_vt.push_back(st);
	st.id = IDC_E_ID; st.str = "ID"; st.str2 = "id";
	m_vt.push_back(st);
	st.id = IDC_E_NAME; st.str = "NAME"; st.str2 = "name";
	m_vt.push_back(st);
	st.id = IDC_E_CBS; st.str = "CBS"; st.str2 = "cbs";
	m_vt.push_back(st);
	st.id = IDC_E_BC; st.str = "BC"; st.str2 = "bc";
	m_vt.push_back(st);
	st.id = IDC_E_KB; st.str = "KB"; st.str2 = "kb";
	m_vt.push_back(st);
	st.id = IDC_E_DJ; st.str = "DJ"; st.str2 = "dj";
	m_vt.push_back(st);
	st.id = IDC_E_BCJS; st.str = "BCJS"; st.str2 = "bcjs";
	m_vt.push_back(st);
	st.id = IDC_E_BK; st.str = "BK"; st.str2 = "bk";
	m_vt.push_back(st);
	st.id = IDC_E_LT; st.str = "LT"; st.str2 = "lt";
	m_vt.push_back(st);
	st.id = IDC_E_GJ; st.str = "GJ"; st.str2 = "gj";
	m_vt.push_back(st);
	st.id = IDC_E_MY; st.str = "MY"; st.str2 = "my";
	m_vt.push_back(st);
	st.id = IDC_E_ZK; st.str = "ZK"; st.str2 = "zk";
	m_vt.push_back(st);
	st.id = IDC_E_SY; st.str = "SY"; st.str2 = "sy";
	m_vt.push_back(st);
	st.id = IDC_E_KDR; st.str = "KDR"; st.str2 = "kdr";
	m_vt.push_back(st);
	st.id = IDC_E_SSR; st.str = "SSR"; st.str2 = "ssr";
	m_vt.push_back(st);

	int n = m_vt.size();
	for (int i=0;i<n;i++)
		m_movectrl.AddCtrl(*GetDlgItem(m_vt[i].id));
}

void CPzToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPzToolDlg::OnPaint()
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
HCURSOR CPzToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CPzToolDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	m_movectrl.HookMessage(pMsg);
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CPzToolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT id = pWnd->GetDlgCtrlID();

	int nCount = m_vt.size();
	for (int i=0;i<nCount;i++)
	{
		if (id == m_vt[i].id)
		{
			pDC->SetBkMode(TRANSPARENT);      //属性设置为透明
			return (HBRUSH)::GetStockObject(NULL_BRUSH); //不返回画刷
		}
	}
	return hbr;
}

LRESULT  CPzToolDlg::OnUpdateWnd(WPARAM wParam, LPARAM lParam)
{
	Invalidate();
	return 0;
}

void CPzToolDlg::SetCtlValue()
{
	int n = m_vt.size();
	for (int i = 0;i<n;i++)
	{
		WCHAR st[MAX_PATH];
		::GetPrivateProfileString(m_vt[i].str, m_vt[i].str2, L"", st, MAX_PATH, g_Globle.m_strIniFilePath);
		SetDlgItemText(m_vt[i].id, st);
	}
}

void CPzToolDlg::MoveCtl()
{
	int x=0, y=0;
	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		x = ::GetPrivateProfileInt(m_vt[i].str, L"x", 0, g_Globle.m_strIniFilePath);
		y = ::GetPrivateProfileInt(m_vt[i].str, L"y", 0, g_Globle.m_strIniFilePath);
		if (x!=0 && y!=0)
		{
			CRect rect;
			GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
			ScreenToClient(&rect);//获得相对于主窗体的坐标
			rect.MoveToXY(x, y);
			GetDlgItem(m_vt[i].id)->MoveWindow(rect);
		}	
	}
	int nSHow = ::GetPrivateProfileInt(L"PIAO", L"show", 1, g_Globle.m_strIniFilePath);
	if (nSHow)
		GetDlgItem(IDC_PIC)->ShowWindow(SW_SHOW);
	else
		GetDlgItem(IDC_PIC)->ShowWindow(SW_HIDE);

	int pic_w, pic_h;
	g_Globle.GetPix(pic_w, pic_h);
	CRect rt_pic;//pic
	GetDlgItem(IDC_PIC)->GetWindowRect(&rt_pic);
	GetDlgItem(IDC_PIC)->MoveWindow(0, 0, pic_w, pic_h, TRUE);
}

void CPzToolDlg::NoMoveCtlLayOut()
{
	//布局非元素控件
	CRect clientRect;
	CRect rt_main;
	GetClientRect(clientRect);
	GetWindowRect(&rt_main);
	int noClient_Y = rt_main.Height() - clientRect.Height();
	int w = rt_main.Width();
	int h = rt_main.Height();
	int pic_w, pic_h;
	{
		CRect rt_gp;//group
		GetDlgItem(IDC_GROUP)->GetWindowRect(&rt_gp);
		GetDlgItem(IDC_GROUP)->MoveWindow(w - 20 - rt_gp.Width(), 10, rt_gp.Width(), h- noClient_Y - 20, TRUE);

		CRect rt_up;//上
		GetDlgItem(IDC_BTN_UP)->GetWindowRect(&rt_up);
		GetDlgItem(IDC_BTN_UP)->MoveWindow(w - 10 - rt_gp.Width()/2- rt_up.Width() / 2, 120, rt_up.Width(), rt_up.Height(), TRUE);
		CRect rt_l;//左
		GetDlgItem(IDC_BTN_LEFT)->GetWindowRect(&rt_l);
		GetDlgItem(IDC_BTN_LEFT)->MoveWindow(w - 10 - rt_gp.Width() / 2 - rt_up.Width() / 2 - rt_l.Width()-1, 120 +rt_up.Height()+1, rt_up.Width(), rt_up.Height(), TRUE);
		CRect rt_r;//右
		GetDlgItem(IDC_BTN_RIGHT)->GetWindowRect(&rt_r);
		GetDlgItem(IDC_BTN_RIGHT)->MoveWindow(w - 10 - rt_gp.Width() / 2 + rt_up.Width() / 2+1, 120 + rt_up.Height() + 1, rt_up.Width(), rt_up.Height(), TRUE);
		CRect rt_d;//下
		GetDlgItem(IDC_BTN_DOWN)->GetWindowRect(&rt_d);
		GetDlgItem(IDC_BTN_DOWN)->MoveWindow(w - 10 - rt_gp.Width() / 2 - rt_up.Width() / 2, 120 +rt_up.Height()*2+2, rt_up.Width(), rt_up.Height(), TRUE);

		CRect rt;//提示
		GetDlgItem(IDC_STA_TS)->GetWindowRect(&rt);
		GetDlgItem(IDC_STA_TS)->MoveWindow(5, h - noClient_Y-rt.Height() - 5, rt.Width(), rt.Height(), TRUE);

		CRect rt_dy;//打印
		GetDlgItem(IDOK)->GetWindowRect(&rt_dy);
		GetDlgItem(IDOK)->MoveWindow(w - 10 - rt_gp.Width() / 2 - rt_dy.Width() / 2, h - noClient_Y - rt_dy.Height() - 20, rt_dy.Width(), rt_dy.Height(), TRUE);

		CRect rt_bj;//布局
		GetDlgItem(IDC_BTN_SAVE)->GetWindowRect(&rt_bj);
		GetDlgItem(IDC_BTN_SAVE)->MoveWindow(w - 10 - rt_gp.Width() / 2 - rt_dy.Width() / 2, h - noClient_Y - rt_bj.Height() * 3 - 20, rt_bj.Width(), rt_bj.Height(), TRUE);

		CRect rt_cf;//配置
		GetDlgItem(IDC_BTN_CONFIG)->GetWindowRect(&rt_cf);
		GetDlgItem(IDC_BTN_CONFIG)->MoveWindow(w - 10 - rt_gp.Width() / 2 - rt_dy.Width() / 2, h - noClient_Y - rt_bj.Height() * 5 - 20 , rt_cf.Width(), rt_cf.Height(), TRUE);
	
	}
}

void CPzToolDlg::SetFont()
{
	if (m_fontID)
	{
		delete m_fontID;
		m_fontID = NULL;
	}

	m_fontID = new CFont;
	m_fontID->CreateFont(g_Globle.m_fontSize, // nHeight 
		0, // nWidth 
		0, // nEscapement 
		0, // nOrientation 
		g_Globle.m_bold > 0 ? FW_BOLD : 0, // nWeight 
		FALSE, // bItalic 
		FALSE, // bUnderline 
		0, // cStrikeOut 
		ANSI_CHARSET, // nCharSet 
		OUT_DEFAULT_PRECIS, // nOutPrecision 
		CLIP_DEFAULT_PRECIS, // nClipPrecision 
		DEFAULT_QUALITY, // nQuality 
		DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
		_T("宋体"));

	int n = m_vt.size();
	for (int i=0;i<n;i++)
		GetDlgItem(m_vt[i].id)->SetFont(m_fontID);

	Invalidate();
}

//配置页面
void CPzToolDlg::OnBnClickedBtnConfig()
{
	CSetDlg dlg;
	if (IDOK == dlg.DoModal())
	{
		SetFont();
		MoveCtl();
		SetCtlValue();
	}
}

//保存布局
void CPzToolDlg::OnBnClickedBtnSave()
{
	CRect rt;
	CString str;
	
	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		GetDlgItem(m_vt[i].id)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		str.Format(L"%d", rt.left);
		::WritePrivateProfileString(m_vt[i].str, L"x", str, g_Globle.m_strIniFilePath);
		str.Format(L"%d", rt.top);
		::WritePrivateProfileString(m_vt[i].str, L"y", str, g_Globle.m_strIniFilePath);
	}
	MessageBox(L"保存成功",L"");
}

//打印
void CPzToolDlg::OnBnClickedOk()
{
	CRect rt;
	CString str;
	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		GetDlgItem(m_vt[i].id)->GetWindowRect(&rt);
		ScreenToClient(&rt);
		m_vt[i].x = rt.left;
		m_vt[i].y = rt.top;
		m_vt[i].width = rt.Width();
		m_vt[i].height = rt.Height();
		GetDlgItemText(m_vt[i].id, m_vt[i].strValue);
	}

	if(!g_Globle.Print(m_vt))
		MessageBox(L"打印失败", L"");
}


void CPzToolDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_bInit)
	{
		NoMoveCtlLayOut();
	}
}


void CPzToolDlg::OnBnClickedBtnUp()
{
	{
		int n = m_vt.size();
		for (int i = 0; i < n; i++)
		{
			CRect rect;
			GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
			ScreenToClient(&rect);//获得相对于主窗体的坐标
			int n = rect.top ;
			if (n == 0)
				return;
		}
	}

	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		CRect rect;
		GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
		ScreenToClient(&rect);//获得相对于主窗体的坐标
		int n = rect.top - 1;
		if (n > 0)
		{
			rect.MoveToXY(rect.left, n);
			GetDlgItem(m_vt[i].id)->MoveWindow(rect);
		}
		else
			return;
	}
	Invalidate();
}


void CPzToolDlg::OnBnClickedBtnDown()
{
	CRect clientRect;
	CRect rt_main;
	GetClientRect(clientRect);
	GetWindowRect(&rt_main);
	int noClient_Y = rt_main.Height() - clientRect.Height();
	int h = rt_main.Height();

	{
		int n = m_vt.size();
		for (int i = 0; i < n; i++)
		{
			CRect rect;
			GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
			ScreenToClient(&rect);//获得相对于主窗体的坐标
			int n = rect.bottom + rect.Height();
			if (n >= h-noClient_Y)
				return;
		}
	}

	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		CRect rect;
		GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
		ScreenToClient(&rect);//获得相对于主窗体的坐标
		int n = rect.top + 1;
		if (n + rect.Height() < h - noClient_Y)
		{
			rect.MoveToXY(rect.left, n);
			GetDlgItem(m_vt[i].id)->MoveWindow(rect);
		}
		else
			return;
	}
	Invalidate();
}


void CPzToolDlg::OnBnClickedBtnLeft()
{
	{
		int n = m_vt.size();
		for (int i = 0; i < n; i++)
		{
			CRect rect;
			GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
			ScreenToClient(&rect);//获得相对于主窗体的坐标
			int n = rect.left;
			if (n == 0)
				return;
		}
	}

	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		CRect rect;
		GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
		ScreenToClient(&rect);//获得相对于主窗体的坐标
		int n = rect.left - 1;
		if (n > 0)
		{
			rect.MoveToXY(n, rect.top);
			GetDlgItem(m_vt[i].id)->MoveWindow(rect);
		}
		else
			return;
	}
	Invalidate();
}


void CPzToolDlg::OnBnClickedBtnRight()
{
	CRect clientRect;
	CRect rt_main;
	GetClientRect(clientRect);
	GetWindowRect(&rt_main);
	int noClient_Y = rt_main.Height() - clientRect.Height();
	int w = rt_main.Width();

	{
		int n = m_vt.size();
		for (int i = 0; i < n; i++)
		{
			CRect rect;
			GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
			ScreenToClient(&rect);//获得相对于主窗体的坐标
			int n = rect.right;
			if (n >= w)
				return;
		}
	}

	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		CRect rect;
		GetDlgItem(m_vt[i].id)->GetWindowRect(&rect);//获得空间的绝对坐标
		ScreenToClient(&rect);//获得相对于主窗体的坐标
		int n = rect.left + 1;
		if (n + rect.Width() < w)
		{
			rect.MoveToXY(n, rect.top);
			GetDlgItem(m_vt[i].id)->MoveWindow(rect);
		}
		else
			return;
	}
	Invalidate();
}


void CPzToolDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT id = nFlags;
	if (id == IDC_BTN_UP)
	{
		int n = 9;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}
