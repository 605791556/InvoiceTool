#pragma once
#include <vector>
#include "cgloble.h"

using std::vector;

// CSetDlg 对话框

class CSetDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONFIG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CFont* m_pFont;
	vector<ST_YS> m_vt;

public:
	void AddMoveCtrl();
	void SetFont();
	void SetDlgValue();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	vector<CString> GetPrintList();
};
