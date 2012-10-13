// SnortTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SnortTest.h"
#include "SnortTestDlg.h"
#include "SnortSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSnortTestDlg 对话框




CSnortTestDlg::CSnortTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSnortTestDlg::IDD, pParent)
	, m_dwIP(0)
	, m_dwPort(0)
	, m_strData(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSnortTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_dwIP);
	DDX_Text(pDX, IDC_EDIT1, m_dwPort);
	DDV_MinMaxUInt(pDX, m_dwPort, 1, 65535);
	DDX_Text(pDX, IDC_EDIT2, m_strData);
	DDX_Control(pDX, IDC_COMBO1, m_comProto);
}

BEGIN_MESSAGE_MAP(CSnortTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CSnortTestDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSnortTestDlg 消息处理程序

BOOL CSnortTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_comProto.AddString("TCP");
	m_comProto.AddString("UDP");

	m_comProto.SetCurSel(0);

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSnortTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSnortTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CSnortTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSnortTestDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();
	UpdateData(TRUE);

	m_strData.Trim();

	int nProto = m_comProto.GetCurSel();	

	string strIp = Util::Int2Ip(htonl(m_dwIP));

	CSnortSocket sock;

	bool bRet = false;

	char szBuffer[MAX_PATTERN_SIZE] = {0};
	u_int nSize = 0;
	if( !Util::ConvertHex((char*)(LPCTSTR)m_strData, szBuffer, nSize) || nSize == 0 )
	{
		MessageBox("转换数据内容出错！");
		return;
	}

	switch(nProto)
	{
	case 0://TCP
		bRet = sock.SendTcp(strIp.c_str(), m_dwPort, szBuffer, nSize);
		break;
	case 1://UDP
		bRet = sock.SendUdp(strIp.c_str(), m_dwPort, szBuffer, nSize);
	}

	if( bRet )
	{
		MessageBox("发送成功！");
	}
	else
	{
		MessageBox("发送失败！");
	}


}
