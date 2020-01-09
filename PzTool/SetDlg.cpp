// SetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PzTool.h"
#include "SetDlg.h"
#include "afxdialogex.h"

// CSetDlg �Ի���

IMPLEMENT_DYNAMIC(CSetDlg, CDialogEx)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CONFIG, pParent)
{
	m_pFont = NULL;
}

CSetDlg::~CSetDlg()
{
	if (m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetDlg ��Ϣ�������
void CSetDlg::SetDlgValue()
{
	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		WCHAR st[MAX_PATH];
		::GetPrivateProfileString(m_vt[i].str, m_vt[i].str2, L"", st, MAX_PATH, g_Globle.m_strIniFilePath);
		SetDlgItemText(m_vt[i].id, st);
	}

	//��ӡ��
	{
		WCHAR  name[MAX_PATH];
		::GetPrivateProfileString(L"PRINTER", L"name", L"", name, MAX_PATH, g_Globle.m_strIniFilePath);
		vector<CString> vt = GetPrintList();
		int ndex = -1;
		for (int i = 0; i < vt.size(); i++)
		{
			if (name == vt[i])
				ndex = i;
			((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(vt[i]);
		}
		if (ndex >= 0)
			((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(ndex);
	}
	//����
	{
		{
			int ndex = -1;
			CString str;
			WCHAR  st[MAX_PATH];
			::GetPrivateProfileString(L"FONT", L"size", L"", st, MAX_PATH, g_Globle.m_strIniFilePath);
			for (int i = 0; i < 50; i++)
			{
				int size = _ttoi(st);
				if (size == i + 1)
					ndex = i;

				str.Format(L"%d", i + 1);
				((CComboBox*)GetDlgItem(IDC_COMBO2))->AddString(str);
			}
			if (ndex >= 0)
				((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(ndex);
		}
		{
			WCHAR  st[MAX_PATH];
			::GetPrivateProfileString(L"FONT", L"bold", L"", st, MAX_PATH, g_Globle.m_strIniFilePath);
			int bold = _ttoi(st);
			((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(bold);
		}
	}
	//��ֽ
	int nOut = ::GetPrivateProfileInt(L"OUT", L"out", 1, g_Globle.m_strIniFilePath);
	if (nOut)
		((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);

	//Ʊֽ
	int nWidth = ::GetPrivateProfileInt(L"PIAO", L"width", 1, g_Globle.m_strIniFilePath);
	int nHeight = ::GetPrivateProfileInt(L"PIAO", L"height", 1, g_Globle.m_strIniFilePath);
	int nSHow = ::GetPrivateProfileInt(L"PIAO", L"show", 1, g_Globle.m_strIniFilePath);
	SetDlgItemInt(IDC_PZ_W, nWidth);
	SetDlgItemInt(IDC_PZ_H, nHeight);
	((CButton*)GetDlgItem(IDC_CK_SHOW))->SetCheck(nSHow);

}

BOOL CSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_pFont = new CFont;
	m_pFont->CreateFont(g_Globle.m_fontSize, // nHeight 
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
		_T("����"));
	SetFont();
	AddMoveCtrl();
	SetDlgValue();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CSetDlg::SetFont()
{
	if (m_pFont)
	{
		int n = m_vt.size();
		for (int i = 0; i < n; i++)
			GetDlgItem(m_vt[i].id)->SetFont(m_pFont);
	}
}

//����
void CSetDlg::OnBnClickedOk()
{
	if (!PathFileExists(g_Globle.m_strIniFilePath))
	{
		MessageBox(L"�Ҳ��������ļ�", L"����", NULL);
		return;
	}

	CString str;
	int n = m_vt.size();
	for (int i = 0; i < n; i++)
	{
		GetDlgItemText(m_vt[i].id, str);
		::WritePrivateProfileString(m_vt[i].str, m_vt[i].str2, str, g_Globle.m_strIniFilePath);
	}

	//��ӡ��
	CString name;
	int ndex = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();
	if (ndex >=0)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO1))->GetLBText(ndex,name);
	}
	g_Globle.m_PrinterName = name;
	::WritePrivateProfileString(L"PRINTER", L"name", name, g_Globle.m_strIniFilePath);

	//����
	{
		CString str;
		int ndex = ((CComboBox*)GetDlgItem(IDC_COMBO2))->GetCurSel();
		if (ndex >= 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO2))->GetLBText(ndex, str);
		}
		g_Globle.m_fontSize = _ttoi(str);
		::WritePrivateProfileString(L"FONT", L"size", str, g_Globle.m_strIniFilePath);
	}
	{
		CString str;
		int bold = ((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck();
		str.Format(L"%d", bold);
		g_Globle.m_bold = bold;
		::WritePrivateProfileString(L"FONT", L"bold",str, g_Globle.m_strIniFilePath);
	}
	//��ֽ
	{
		int nCheck = ((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck();
		if (nCheck)
			::WritePrivateProfileString(L"OUT", L"out", L"1", g_Globle.m_strIniFilePath);
		else
			::WritePrivateProfileString(L"OUT", L"out", L"0", g_Globle.m_strIniFilePath);
	}
	//Ʊֽ
	{
		GetDlgItemText(IDC_PZ_W,str);
		::WritePrivateProfileString(L"PIAO", L"width", str, g_Globle.m_strIniFilePath);
		GetDlgItemText(IDC_PZ_H, str);
		::WritePrivateProfileString(L"PIAO", L"height", str, g_Globle.m_strIniFilePath);

		CString str;
		int nShow = ((CButton*)GetDlgItem(IDC_CK_SHOW))->GetCheck();
		str.Format(L"%d", nShow);
		g_Globle.m_nShow = nShow;
		::WritePrivateProfileString(L"PIAO", L"show", str, g_Globle.m_strIniFilePath);
	}
	MessageBox( L"����ɹ�", L"��ʾ", NULL);
	CDialogEx::OnOK();
}

vector<CString> CSetDlg::GetPrintList()
{
	vector<CString> vt;
	DWORD Flags = PRINTER_ENUM_FAVORITE | PRINTER_ENUM_LOCAL;
	DWORD cbBuf = 0;
	DWORD pcReturned = 0;
	DWORD Level = 2;
	char Name[512] = { 0 };
	LPPRINTER_INFO_2A pPrinterEnum = NULL;
	::EnumPrintersA(Flags, Name, Level, NULL, 0, &cbBuf, &pcReturned);
	if (0 == cbBuf)
	{
		//v["printer"].append("û���ҵ���ӡ������");
		return  vt;
	}

	pPrinterEnum = (LPPRINTER_INFO_2A)new char[cbBuf];
	EnumPrintersA(
		Flags,    //   DWORD   Flags,   printer   object   types    
		Name,    //   LPTSTR   Name,   name   of   printer   object    
		Level,    //   DWORD   Level,   information   level    
		(LPBYTE)pPrinterEnum,    //   LPBYTE   pPrinterEnum,   printer   information   buffer    
		cbBuf,    //   DWORD   cbBuf,   size   of   printer   information   buffer  
		&cbBuf, //   LPDWORD   pcbNeeded,   bytes   received   or   required    
		&pcReturned);    //   LPDWORD   pcReturned   number   of   printers   enumerated    

	LPPRINTER_INFO_2A p = pPrinterEnum;
	for (int i = 0; i < pcReturned; i++)
	{
		CString str = (CString)p->pPrinterName;
		vt.push_back(str);
		p++;
	}
	if (pPrinterEnum)delete ((char*)pPrinterEnum);
	return vt;
}

void CSetDlg::AddMoveCtrl()
{
	ST_YS st;
	st.id = IDC_KF; st.str = "KF"; st.str2 = "kf";
	m_vt.push_back(st);
	st.id = IDC_YEAR; st.str = "YEAR"; st.str2 = "year";
	m_vt.push_back(st);
	st.id = IDC_MONTH; st.str = "MONTH"; st.str2 = "month";
	m_vt.push_back(st);
	st.id = IDC_DAY; st.str = "DAY"; st.str2 = "day";
	m_vt.push_back(st);
	st.id = IDC_Z1; st.str = "Z1"; st.str2 = "z1";
	m_vt.push_back(st);
	st.id = IDC_H1; st.str = "H1"; st.str2 = "h1";
	m_vt.push_back(st);
	st.id = IDC_Z2; st.str = "Z2"; st.str2 = "z2";
	m_vt.push_back(st);
	st.id = IDC_H2; st.str = "H2"; st.str2 = "h2";
	m_vt.push_back(st);
	st.id = IDC_ID; st.str = "ID"; st.str2 = "id";
	m_vt.push_back(st);
	st.id = IDC_NAME; st.str = "NAME"; st.str2 = "name";
	m_vt.push_back(st);
	st.id = IDC_CBS; st.str = "CBS"; st.str2 = "cbs";
	m_vt.push_back(st);
	st.id = IDC_BC; st.str = "BC"; st.str2 = "bc";
	m_vt.push_back(st);
	st.id = IDC_KB; st.str = "KB"; st.str2 = "kb";
	m_vt.push_back(st);
	st.id = IDC_DJ; st.str = "DJ"; st.str2 = "dj";
	m_vt.push_back(st);
	st.id = IDC_BCJS; st.str = "BCJS"; st.str2 = "bcjs";
	m_vt.push_back(st);
	st.id = IDC_BK; st.str = "BK"; st.str2 = "bk";
	m_vt.push_back(st);
	st.id = IDC_LT; st.str = "LT"; st.str2 = "lt";
	m_vt.push_back(st);
	st.id = IDC_GJ; st.str = "GJ"; st.str2 = "gj";
	m_vt.push_back(st);
	st.id = IDC_MY; st.str = "MY"; st.str2 = "my";
	m_vt.push_back(st);
	st.id = IDC_ZK; st.str = "ZK"; st.str2 = "zk";
	m_vt.push_back(st);
	st.id = IDC_SY; st.str = "SY"; st.str2 = "sy";
	m_vt.push_back(st);
	st.id = IDC_KDR; st.str = "KDR"; st.str2 = "kdr";
	m_vt.push_back(st);
	st.id = IDC_SSR; st.str = "SSR"; st.str2 = "ssr";
	m_vt.push_back(st);
}
