#include "stdafx.h"
#include "MiniThunderDlg.h"
#include "afxdialogex.h"
#include <windows.h>

class CryHelp : public CDialogEx
{
public:
	CryHelp();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CryHelp::CryHelp() : CDialogEx(CryHelp::IDD)
{
}

void CryHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CryHelp, CDialogEx)
END_MESSAGE_MAP()


// CMiniThunderDlg �Ի���



CMiniThunderDlg::CMiniThunderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMiniThunderDlg::IDD, pParent), m_DownEngine(L"./xldl.dll"), m_DownCancel(false), m_Downloading(false), m_Event(FALSE, TRUE)
{
	m_uTimerIdForQuery = 1;
	m_uElapseForQuery = 60;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMiniThunderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_RECV_SPEED, m_Recv_Speed);
	DDX_Control(pDX, IDC_STATIC_RECV_PRECENT, m_Recv_Precent);
	DDX_Control(pDX, IDC_STATIC_FILE_TOTALSIZE, m_File_TotalSize);
	DDX_Control(pDX, IDC_STATIC_RECV_TOTALDOWNLOAD, m_Recv_TotalDownload);
	DDX_Control(pDX, IDC_STATIC_SHARE_SPEED, m_Share_Speed);
	DDX_Control(pDX, IDC_STATIC_SHARE_COUNT, m_Share_Count);
	DDX_Control(pDX, IDC_DOWNLOAD_PROGRESS, m_Download_Progress);
}

BEGIN_MESSAGE_MAP(CMiniThunderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_STAR_DOWN, &CMiniThunderDlg::OnBnClickedButtonStarDown)
	ON_BN_CLICKED(IDC_BUTTON_STOP_DOWN, &CMiniThunderDlg::OnBnClickedButtonStopDown)
END_MESSAGE_MAP()


// CMiniThunderDlg ��Ϣ�������

BOOL CMiniThunderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	if (!m_DownEngine.InitEngine())
	{
		AfxMessageBox("���������ʼ��ʧ��\n��ǰ���ٷ���վ�������°汾");
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CMiniThunderDlg::OnClose()
{
	PostMessage(WM_QUIT);
	CDialog::OnClose();
}
void CMiniThunderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CryHelp dlg;
		dlg.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMiniThunderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
}

HCURSOR CMiniThunderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CMiniThunderDlg::PrintTaskInfo(DOWN_TASK_STATUS Status, DownTaskInfo info)
{
	switch (Status)
	{
	case NOITEM:
	{
		SetWindowText("δ֪����");
		break;
	}
	case TSC_ERROR:
	{
		PrintTaskError(info.fail_code, info);
		m_DownEngine.TaskDelete(m_hTask);
		DownTaskParam deleteParam;
		StrCpyW(deleteParam.szFilename, info.szFilename);
		StrCpyW(deleteParam.szSavePath, TaskParam.szSavePath);
		m_DownEngine.DelTempFile(deleteParam);
		break;
	}
	case TSC_PAUSE:
	{
		SetWindowText("��ͣ����");
		break;
	}
	case TSC_DOWNLOAD:
	{
		std::wstring szBuffer;
		szBuffer += L"��������";
		szBuffer += L":";
		szBuffer += info.szFilename;
		SetWindowTextW(m_hWnd, szBuffer.c_str());
		break;
	}
	case TSC_COMPLETE:
	{
		SetWindowText("�������");
		break;
	}
	case TSC_STARTPENDING:
	{
		SetWindowText("��������");
		break;
	}
	case TSC_STOPPENDING:
	{
		SetWindowText("��������");
		break;
	}
	}
}
void CMiniThunderDlg::PrintTaskError(TASK_ERROR_TYPE status, DownTaskInfo info)
{
	switch (status)
	{
	case TASK_ERROR_UNKNOWN:
	{
		SetWindowText("δ֪����");
		break;
	}
	case TASK_ERROR_DISK_CREATE:
	{
		SetWindowText("�����ļ�ʧ��");
		break;
	}
	case TASK_ERROR_DISK_WRITE:
	{
		SetWindowText("д�ļ�ʧ��");
		break;
	}
	case TASK_ERROR_DISK_READ:
	{
		SetWindowText("���ļ�ʧ��");
		break;
	}
	case TASK_ERROR_DISK_RENAME:
	{
		SetWindowText("������ʧ��");
		break;
	}
	case TASK_ERROR_DISK_PIECEHASH:
	{
		SetWindowText("�ļ�ƬУ��ʧ��");
		break;
	}
	case TASK_ERROR_DISK_FILEHASH:
	{
		SetWindowText("�ļ�ȫ��У��ʧ��");
		break;
	}
	case TASK_ERROR_DISK_DELETE:
	{
		SetWindowText("ɾ���ļ�ʧ��ʧ��");
		break;
	}
	case TASK_ERROR_DOWN_INVALID:
	{
		SetWindowText("��Ч�ĸ��µ�ַ");
		break;
	}
	case TASK_ERROR_PROXY_AUTH_TYPE_UNKOWN:
	{
		SetWindowText("��������δ֪");
		break;
	}
	case TASK_ERROR_PROXY_AUTH_TYPE_FAILED:
	{
		SetWindowText("������֤ʧ��");
		break;
	}
	case TASK_ERROR_HTTPMGR_NOT_IP:
	{
		SetWindowText("�������޿�����Դ");
		break;
	}
	case TASK_ERROR_TIMEOUT:
	{
		SetWindowText("��������ʱ");
		break;
	}
	case TASK_ERROR_CANCEL:
	{
		SetWindowText("��������ȡ��");
		break;
	}
	case TASK_ERROR_TP_CRASHED:
	{
		SetWindowText("MINITP����");
		break;
	}
	case TASK_ERROR_ID_INVALID:
	{
		SetWindowText("TaskId �Ƿ�");
		break;
	}
	}
}
void CMiniThunderDlg::OnTimer(UINT nIDEvent)
{
	DownTaskInfo info;
	TCHAR szBuffer[32];
	if (m_Downloading)
	{
		if (m_DownEngine.TaskQueryInfoEx(m_hTask, info))
		{
			PrintTaskInfo(info.stat, info);
			_stprintf(szBuffer, "��ǰ�ٶȣ�%0.2fMbps", ((info.nSpeed / 1024.0f) / 1024.0f));
			m_Recv_Speed.SetWindowText(szBuffer);
			_stprintf(szBuffer, "�Ѿ����£�%0.2f%%", info.fPercent * 100.0f);
			m_Recv_Precent.SetWindowText(szBuffer);
			_stprintf(szBuffer, "�ļ���С��%0.2fMB", ((info.nTotalSize / 1024.0f) / 1024.0f));
			m_File_TotalSize.SetWindowText(szBuffer);
			_stprintf(szBuffer, "���½��ȣ�%0.2fMB", ((info.nTotalDownload / 1024.0f) / 1024.0f));
			m_Recv_TotalDownload.SetWindowText(szBuffer);
			_stprintf(szBuffer, "��Դ���٣�%0.2fMB", (((info.nTotalDownload - info.nDonationOrgin) / 1024.0f) / 1024.0f));
			m_Share_Speed.SetWindowText(szBuffer);
			_stprintf(szBuffer, "��Դ������%d/%d", info.nSrcUsing, info.nSrcTotal);
			m_Share_Count.SetWindowText(szBuffer);
			m_Download_Progress.SetPos((UINT)(info.fPercent * 100.0f));
		}
		if (info.stat == TSC_COMPLETE || info.stat == TSC_ERROR || info.fail_code != TASK_ERROR_UNKNOWN)
		{
			m_Downloading = false;
			m_Event.SetEvent();
			KillTimer(m_uTimerIdForQuery);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
bool CMiniThunderDlg::InitDownEngine(const std::wstring url, const std::wstring exe)
{
	m_Download_Progress.SetPos(0);
	m_Downloading = true;
	m_Event.ResetEvent();

	TaskParam = m_DownEngine.CreateDownTaskParam(url.c_str(), exe.c_str(), L"./");
	m_hTask = m_DownEngine.TaskCreate(TaskParam);
	if (m_DownEngine.TaskStart(m_hTask))
	{
		SetTimer(m_uTimerIdForQuery, m_uElapseForQuery, NULL);
		return true;
	}
	return false;
}

bool CMiniThunderDlg::InitDownEngine(const std::string url)
{
	std::wstring UrlBuffer(url.length(), L' ');
	std::copy(url.begin(), url.end(), UrlBuffer.begin());
	return InitDownEngine(UrlBuffer, UrlBuffer.substr(UrlBuffer.find_last_of('/') + 1));
}

void CMiniThunderDlg::OnBnClickedButtonStarDown()
{
	CString szBuffer;
	if (GetDlgItemText(IDC_BUTTON_STAR_DOWN, szBuffer))
	{
		if (szBuffer == _T("��ʼ����"))
		{
			GetDlgItem(IDC_BUTTON_STAR_DOWN)->EnableWindow(FALSE);
			m_DownCancel = false;

			CString lpszBuffer;
			if (GetDlgItemText(IDC_EDIT_URL, lpszBuffer))
			{
				if (InitDownEngine(lpszBuffer.GetBuffer()))
				{
					GetDlgItem(IDC_BUTTON_STOP_DOWN)->EnableWindow(TRUE);
					if (!WaitForHelper.MsgWaitForSingleObject(m_Event.m_hObject))
					{
						AfxMessageBox("Event Init Fail");
					}
				}
			}
			else {
				AfxMessageBox("������URL");
			}
			GetDlgItem(IDC_BUTTON_STAR_DOWN)->EnableWindow(TRUE);
		}
		if (szBuffer == _T("ֹͣ����"))
		{
			if (!m_Downloading && m_hTask)
			{
				if (m_DownEngine.TaskStop(m_hTask))
				{
					m_DownCancel = true;
					m_Downloading = false;
					KillTimer(m_uTimerIdForQuery);
					m_Event.SetEvent();
					m_DownEngine.TaskDelete(m_hTask);
					SetDlgItemText(IDC_BUTTON_STOP_DOWN, "��ͣ����");
					SetDlgItemText(IDC_BUTTON_STAR_DOWN, "��ʼ����");
					GetDlgItem(IDC_BUTTON_STOP_DOWN)->EnableWindow(FALSE);
					m_Download_Progress.SetPos(0);
					m_Recv_Speed.SetWindowText("��ǰ�ٶȣ�");
					m_Recv_Precent.SetWindowText("�Ѿ����أ�");
					m_File_TotalSize.SetWindowText("�ļ���С��");
					m_Recv_TotalDownload.SetWindowText("���½��ȣ�");
					m_Share_Speed.SetWindowText("��Դ���٣�");
					m_Share_Count.SetWindowText("��Դ������");
					SetWindowText("MiniThunder");
				}
			}
		}
	}

}


void CMiniThunderDlg::OnBnClickedButtonStopDown()
{
	CString szBuffer;
	if (GetDlgItemText(IDC_BUTTON_STOP_DOWN, szBuffer))
	{
		if (szBuffer == _T("��ͣ����"))
		{
			if (m_Downloading && m_hTask)
			{
				if (m_DownEngine.TaskForceStop(m_hTask))
				{
					SetDlgItemText(IDC_BUTTON_STOP_DOWN, "��������");
					SetDlgItemText(IDC_BUTTON_STAR_DOWN, "ֹͣ����");
					GetDlgItem(IDC_BUTTON_STAR_DOWN)->EnableWindow(TRUE);
					m_DownCancel = true;
					m_Downloading = false;
				}
			}
		}
		if (szBuffer == _T("��������"))
		{
			if (m_DownCancel && m_hTask)
			{
				if (m_DownEngine.TaskStart(m_hTask))
				{
					//SetTimer(m_uTimerIdForQuery, m_uElapseForQuery, NULL);
					m_DownCancel = false;
					m_Downloading = true;
					SetDlgItemText(IDC_BUTTON_STOP_DOWN, "��ͣ����");
					GetDlgItem(IDC_BUTTON_STAR_DOWN)->EnableWindow(FALSE);
				}
			}
		}
	}
}
