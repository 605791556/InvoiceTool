
// PzToolDlg.h : 头文件
//

#pragma once
#include "MoveCtrlHandler.h"
#include <shlwapi.h>
#include <vector>
#include "afxwin.h"
#include "cgloble.h"
#pragma comment(lib,"Shlwapi.lib")

using std::vector;

// CPzToolDlg 对话框
class CPzToolDlg : public CDialogEx
{
// 构造
public:
	CPzToolDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PZTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


public:
	bool m_bInit;
	CMoveCtrlHandler m_movectrl;
	CFont* m_fontID;
	vector<ST_YS> m_vt;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	void MoveCtl();//布局元素控件
	void AddMoveCtrl();//添加元素控件
	void NoMoveCtlLayOut();//布局非元素控件
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnUp();
	afx_msg void OnBnClickedBtnDown();
	afx_msg void OnBnClickedBtnLeft();
	afx_msg void OnBnClickedBtnRight();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
