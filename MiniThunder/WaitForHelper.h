#pragma once

class CWaitForHelper
{
public:
	/**********************************
	����: ����Windows��Ϣ
	����: 
	bDispatchQuitMsg	: �Ƿ�ת�� WM_QUIT ��Ϣ
	TRUE : ת����Ĭ�ϣ�
	FALSE: ��ת���������� FALSE

	����ֵ:		TRUE  : ������Ϣ
	FALSE : bDispatchQuitMsg ����Ϊ FALSE ���յ� WM_QUIT ��Ϣ		
	**********************************/
	virtual BOOL PeekMessageLoop(BOOL bDispatchQuitMsg = TRUE);

	/**********************************
	����: �ȴ�ָ��ʱ��, ͬʱ����Windows��Ϣ
	����: (�ο�: MsgWaitForMultipleObjectsEx() )
	dwHandles		: ����Ԫ�ظ���
	szHandles		: ����������
	dwMilliseconds	: �ȴ�ʱ�� (����)
	dwWakeMask		: ��Ϣ���˱�ʶ
	dwFlags			: �ȴ�����

	����ֵ: (0 ~ dwHandles - 1): �ȴ��ɹ�
	WAIT_TIMEOUT		: ��ʱ
	WAIT_FAILED			: ִ��ʧ��
	**********************************/
	virtual DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

	/**********************************
	����: �ȴ�ָ��ʱ��, ͬʱ����Windows��Ϣ
	����: (�ο�: MsgWaitForMultipleObjectsEx() )
	hHandle			: ������
	dwMilliseconds	: �ȴ�ʱ�� (����)
	dwWakeMask		: ��Ϣ���˱�ʶ
	dwFlags			: �ȴ�����

	����ֵ: TRUE: �ȴ��ɹ���FALSE: ��ʱ		
	**********************************/
	virtual BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);
};

