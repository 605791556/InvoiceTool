
// PzToolDlg.h : ͷ�ļ�
//

#pragma once
#include "MoveCtrlHandler.h"
#include <shlwapi.h>
#include <vector>
#include "afxwin.h"
#include "cgloble.h"
#pragma comment(lib,"Shlwapi.lib")

using std::vector;

// CPzToolDlg �Ի���
class CPzToolDlg : public CDialogEx
{
// ����
public:
	CPzToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PZTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


public:
	bool m_bInit;
	CMoveCtrlHandler m_movectrl;
	CFont* m_fontID;
	vector<ST_YS> m_vt;
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT  OnUpdateWnd(WPARAM wParam,LPARAM lParam);
	CStatic m_sta_id;
	afx_msg void OnBnClickedBtnConfig();
	void SetFont();
	void SetCtlValue();
	void MoveCtl();//����Ԫ�ؿؼ�
	void AddMoveCtrl();//���Ԫ�ؿؼ�
	void NoMoveCtlLayOut();//���ַ�Ԫ�ؿؼ�
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
