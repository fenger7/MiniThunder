
// MiniThunderDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DownWrapper.h"
#include "WaitForHelper.h"
#include <string>

class CMiniThunderDlg : public CDialogEx
{
// ����
public:
	CMiniThunderDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MINITHUNDER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

protected:
	void PrintTaskInfo(DOWN_TASK_STATUS Status, DownTaskInfo info);
	void PrintTaskError(TASK_ERROR_TYPE status, DownTaskInfo info);
	bool InitDownEngine(const std::wstring url, const std::wstring exe);
	bool InitDownEngine(const std::string url);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	UINT_PTR  m_uTimerIdForQuery;
	UINT      m_uElapseForQuery;
	afx_msg void OnTimer(UINT nIDEvent);
private:
	CStatic m_Recv_Speed;
	CStatic m_Recv_Precent;
	CStatic m_File_TotalSize;
	CStatic m_Recv_TotalDownload;
	CStatic m_Share_Speed;
	CStatic m_Share_Count;
	CProgressCtrl m_Download_Progress;
private:
	HANDLE m_hTask;
	DownEngine::CDownWrapper m_DownEngine;
	DownTaskParam TaskParam;
	bool    m_Downloading;
	bool	m_DownCancel;
	CEvent m_Event;
	CWaitForHelper WaitForHelper;
public:
	
	afx_msg void OnBnClickedButtonStarDown();
	afx_msg void OnBnClickedButtonStopDown();
};
