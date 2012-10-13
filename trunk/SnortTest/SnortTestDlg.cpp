// SnortTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SnortTest.h"
#include "SnortTestDlg.h"
#include "SnortSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CSnortTestDlg �Ի���




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


// CSnortTestDlg ��Ϣ�������

BOOL CSnortTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_comProto.AddString("TCP");
	m_comProto.AddString("UDP");

	m_comProto.SetCurSel(0);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSnortTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CSnortTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSnortTestDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		MessageBox("ת���������ݳ���");
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
		MessageBox("���ͳɹ���");
	}
	else
	{
		MessageBox("����ʧ�ܣ�");
	}


}
