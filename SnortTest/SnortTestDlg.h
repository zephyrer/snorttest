// SnortTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSnortTestDlg �Ի���
class CSnortTestDlg : public CDialog
{
// ����
public:
	CSnortTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SNORTTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_dwIP;
public:
	DWORD m_dwPort;
public:
	CString m_strData;
public:
	afx_msg void OnBnClickedOk();
public:
	CComboBox m_comProto;
};
