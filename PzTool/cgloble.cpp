#include "stdafx.h"
#include "cgloble.h"

CGloble g_Globle;


CGloble::CGloble()
{
	InitGloble();
}

CGloble::~CGloble()
{
	
}

bool CGloble::InitGloble()
{
	CString rootPath = GetEXEPath();
	m_strIniFilePath = rootPath + L"\\config\\config.ini";
	if (!PathFileExists(m_strIniFilePath))
	{
		::MessageBox(NULL, L"找不到配置文件", L"错误", NULL);
		return false;
	}
	else
	{
		{
			WCHAR  name[MAX_PATH];
			::GetPrivateProfileString(L"PRINTER", L"name", L"", name, MAX_PATH, m_strIniFilePath);
			m_PrinterName = name;
		}
		{
			WCHAR  st[MAX_PATH];
			::GetPrivateProfileString(L"FONT", L"size", L"", st, MAX_PATH, m_strIniFilePath);
			m_fontSize = _ttoi(st);
		}
		{
			WCHAR  st[MAX_PATH];
			::GetPrivateProfileString(L"FONT", L"bold", L"", st, MAX_PATH, m_strIniFilePath);
			m_bold = _ttoi(st);
		}
	}
	return true;
}




CString CGloble::GetEXEPath()
{
	CString strPath;
	GetModuleFileName(NULL, strPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);

	strPath.ReleaseBuffer();
	int pos = strPath.ReverseFind('\\');
	strPath = strPath.Left(pos);
	return strPath;
}

BOOL GetJobs(HANDLE hPrinter,        /* Handle to the printer. */

	JOB_INFO_2 **ppJobInfo, /* Pointer to be filled.  */
	int *pcJobs,            /* Count of jobs filled.  */
	DWORD *pStatus)         /* Print Queue status.    */

{

	DWORD               cByteNeeded,
		nReturned,
		cByteUsed;
	JOB_INFO_2          *pJobStorage = NULL;
	PRINTER_INFO_2       *pPrinterInfo = NULL;

	/* Get the buffer size needed. */
	if (!GetPrinter(hPrinter, 2, NULL, 0, &cByteNeeded))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
			return FALSE;
	}

	pPrinterInfo = (PRINTER_INFO_2 *)malloc(cByteNeeded);
	::memset(pPrinterInfo, 0, cByteNeeded);
	if (!(pPrinterInfo))
		/* Failure to allocate memory. */
		return FALSE;

	/* Get the printer information. */
	if (!GetPrinter(hPrinter,
		2,
		(LPBYTE)pPrinterInfo,
		cByteNeeded,
		&cByteUsed))
	{
		/* Failure to access the printer. */
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return FALSE;
	}

	/* Get job storage space. */
	if (!EnumJobs(hPrinter,
		0,
		pPrinterInfo->cJobs,
		2,
		NULL,
		0,
		(LPDWORD)&cByteNeeded,
		(LPDWORD)&nReturned))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			free(pPrinterInfo);
			pPrinterInfo = NULL;
			return FALSE;
		}
	}

	pJobStorage = (JOB_INFO_2 *)malloc(cByteNeeded);
	if (!pJobStorage)
	{
		/* Failure to allocate Job storage space. */
		free(pPrinterInfo);
		pPrinterInfo = NULL;
		return FALSE;
	}

	ZeroMemory(pJobStorage, cByteNeeded);

	/* Get the list of jobs. */
	if (!EnumJobs(hPrinter,
		0,
		pPrinterInfo->cJobs,
		2,
		(LPBYTE)pJobStorage,
		cByteNeeded,
		(LPDWORD)&cByteUsed,
		(LPDWORD)&nReturned))
	{
		free(pPrinterInfo);
		free(pJobStorage);
		pJobStorage = NULL;
		pPrinterInfo = NULL;
		return FALSE;
	}

	/*
	*  Return the information.
	*/
	*pcJobs = nReturned;
	*pStatus = pPrinterInfo->Status;
	*ppJobInfo = pJobStorage;
	free(pPrinterInfo);

	return TRUE;

}

int GetPrintStatus(CString docName, const char* printerName)//return 0成功 1打印中 -1失败
{
	int ret = -1;//出错
	HANDLE hPrinter;
	if (!OpenPrinterA((LPSTR)printerName, &hPrinter, 0))
	{
		return ret;
	}

	JOB_INFO_2  *pJobs;
	int         cJobs, i;
	DWORD       dwPrinterStatus;

	/*
	*  Get the state information for the Printer Queue and
	*  the jobs in the Printer Queue.
	*/
	if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
	{
		ClosePrinter(hPrinter);
		return ret;
	}


	/*
	*  If the Printer reports an error, believe it.
	*/
	if (dwPrinterStatus &
		(PRINTER_STATUS_ERROR |
			PRINTER_STATUS_PAPER_JAM |
			PRINTER_STATUS_PAPER_OUT |
			PRINTER_STATUS_PAPER_PROBLEM |
			PRINTER_STATUS_OUTPUT_BIN_FULL |
			PRINTER_STATUS_NOT_AVAILABLE |
			PRINTER_STATUS_NO_TONER |
			PRINTER_STATUS_OUT_OF_MEMORY |
			PRINTER_STATUS_OFFLINE |
			PRINTER_STATUS_DOOR_OPEN))
	{
		ClosePrinter(hPrinter);
		free(pJobs);
		return ret;
	}

	/*
	*  Find the Job in the Queue that is printing.
	*/
	ret = 0;
	for (i = 0; i < cJobs; i++)
	{
		if (docName == pJobs[i].pDocument)
		{
			if (JOB_STATUS_COMPLETE&pJobs[i].Status)
			{
				ret = 0;
				break;
			}
			if ((pJobs[i].Status & JOB_STATUS_PRINTING) || 0 == pJobs[i].Status)ret = 1;
			else
			{
				::SetJobA(hPrinter, pJobs[i].JobId, 2, (LPBYTE)&pJobs[i], JOB_CONTROL_DELETE);
				ret = -1;
			}
			break;
		}
	}

	/*
	*  No error condition.
	*/
	free(pJobs);
	ClosePrinter(hPrinter);
	return ret;
}

BOOL DeleteJob(CString docName, const char* printerName)
{
	BOOL ret = FALSE;//出错
	HANDLE hPrinter;
	if (!OpenPrinterA((LPSTR)printerName, &hPrinter, 0))
	{
		return ret;
	}

	JOB_INFO_2  *pJobs;
	int         cJobs, i;
	DWORD       dwPrinterStatus;

	/*
	*  Get the state information for the Printer Queue and
	*  the jobs in the Printer Queue.
	*/
	if (!GetJobs(hPrinter, &pJobs, &cJobs, &dwPrinterStatus))
	{
		ClosePrinter(hPrinter);
		return ret;
	}


	/*
	*  If the Printer reports an error, believe it.
	*/
	if (dwPrinterStatus &
		(PRINTER_STATUS_ERROR |
			PRINTER_STATUS_PAPER_JAM |
			PRINTER_STATUS_PAPER_OUT |
			PRINTER_STATUS_PAPER_PROBLEM |
			PRINTER_STATUS_OUTPUT_BIN_FULL |
			PRINTER_STATUS_NOT_AVAILABLE |
			PRINTER_STATUS_NO_TONER |
			PRINTER_STATUS_OUT_OF_MEMORY |
			PRINTER_STATUS_OFFLINE |
			PRINTER_STATUS_DOOR_OPEN))
	{
		ClosePrinter(hPrinter);
		free(pJobs);
		return ret;
	}

	/*
	*  Find the Job in the Queue that is printing.
	*/
	ret = TRUE;
	for (i = 0; i < cJobs; i++)
	{
		if (docName == pJobs[i].pDocument)
		{
			::SetJobA(hPrinter, pJobs[i].JobId, 2, (LPBYTE)&pJobs[i], JOB_CONTROL_DELETE);
			break;
		}
	}

	/*
	*  No error condition.
	*/
	free(pJobs);
	ClosePrinter(hPrinter);
	return ret;
}

void CGloble::SpinImg(CImage& dstImg, const CImage& srcImg)
{
	int w = srcImg.GetWidth();
	int h = srcImg.GetHeight();

	int srcBitsCount = srcImg.GetBPP();
	int srcPitch = srcImg.GetPitch();
	BYTE* srcPtr = (BYTE*)srcImg.GetBits();

	dstImg.Create(w, h, srcBitsCount, 0);
	if (srcBitsCount <= 8 && srcImg.IsIndexed())
	{
		RGBQUAD pal[256];
		int nColors = srcImg.GetMaxColorTableEntries();
		if (nColors > 0)
		{
			srcImg.GetColorTable(0, nColors, pal);
			dstImg.SetColorTable(0, nColors, pal);
		}
	}

	//目标图像参数  
	BYTE *destPtr = (BYTE*)dstImg.GetBits();
	int destPitch = dstImg.GetPitch();

	//复制图像数据
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int n = srcBitsCount / 8;
			for (int k = 0; k < n; k++)
			{
				*(destPtr + (h - 1 - i)*destPitch + n*( w - 1 - j)+k) = *(srcPtr + i*srcPitch + n*j + k);
			}
		}
	}
}

bool CGloble::Print(vector<ST_YS>& vt)
{
	return Print2(vt);
	
	int w, h;
	GetPix(w, h);
	CImage img;
	img.Create(w, h, 24);
	//img.Create(1200, 600, 24);
	HDC hdc = img.GetDC();
	CDC* pDC = CDC::FromHandle(hdc);
	pDC->FillSolidRect(0, 0, img.GetWidth(), img.GetHeight(), RGB(255, 255, 255));

	int all_h = 0;
	int count = vt.size();
	for (int i = 0; i < count;i++)
	{
		ST_YS* st = &vt[i];
		if (st->strValue.IsEmpty())
		{
			continue;
		}
		CFont* font = new CFont;
		font->CreateFont(g_Globle.m_fontSize,0,0,0,g_Globle.m_bold > 0 ? FW_BOLD : 0, FALSE, FALSE, 0, ANSI_CHARSET,OUT_DEFAULT_PRECIS, // nOutPrecision 
			CLIP_DEFAULT_PRECIS, // nClipPrecision 
			DEFAULT_QUALITY, // nQuality 
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 
			_T("宋体"));

		pDC->SelectObject(font);
		if (st->width)
		{
			pDC->TextOutW(st->x, st->y, st->strValue);
			/*if (st->height == 0)
				st->height = g_Globle.m_fontSize;

			pDC->DrawText(st->strValue, &CRect(st->x, st->y, st->x + st->width,
				st->y + g_Globle.m_fontSize), DT_WORDBREAK | DT_EDITCONTROL);*/

			//pDC->DrawText(st->strValue, &CRect(st->x, st->y - st->height, st->x + st->width,
			//	st->y + g_Globle.m_fontSize), DT_SINGLELINE | DT_BOTTOM | DT_WORDBREAK | DT_EDITCONTROL);
		}
		else
			pDC->TextOutW(st->x, st->y, st->strValue);
	}
	img.ReleaseDC();

	CImage print_img;
	int nOut = ::GetPrivateProfileInt(L"OUT",L"out",1,m_strIniFilePath);//出纸顺序
	if (nOut)
		print_img = img;
	else
		SpinImg(print_img, img);

	print_img.Save(L"D:/img.jpg");
	return Print(print_img);
}

bool CGloble::Print2(vector<ST_YS>& vt)
{
	USES_CONVERSION;
	string strPrint = W2A(m_PrinterName);
	CString docName;
	{
		char tmp[1024] = { 0 };
		sprintf_s(tmp, "1905doc_%x", ::GetTickCount());
		docName = tmp;
	}

	do
	{
		HDC hdcprint = 0; // 定义一个设备环境句柄
		{
			//PRINTDLG printInfo;
			//ZeroMemory(&printInfo, sizeof(printInfo));  //清空该结构
			//printInfo.lStructSize = sizeof(printInfo);
			//printInfo.hwndOwner = 0;
			//printInfo.hDevMode = 0;
			//printInfo.hDevNames = 0;
			////这个是关键，PD_RETURNDC 如果不设这个标志，就拿不到hDC了
			////PD_RETURNDEFAULT 这个就是得到默认打印机，不需要弹出设置对话框
			//printInfo.Flags = PD_RETURNDC | PD_RETURNDEFAULT | PD_ALLPAGES;

			//PrintDlg(&printInfo);//调用API拿出默认打印机
			//DWORD rst = CommDlgExtendedError();//看看出错没有
			//if (rst != 0)
			//{//出错了，清空标志再次调用API，此时就会弹出打印设置对话框供用户选择了
			//	printInfo.Flags = 0;
			//	PrintDlg(&printInfo);
			//}

			//hdcprint = printInfo.hDC; //得到打印DC，输出到打印
		}

		DOCINFO di = { sizeof(DOCINFO),docName,NULL };//定义一个打印作业
		if ((hdcprint = CreateDCA("WINSPOOL", strPrint.c_str(), 0, NULL)) == 0)
			break;

		CDC MemDc;
		MemDc.Attach(hdcprint);

		//DOCINFO di;//打印文件结构体
		//memset(&di, 0, sizeof(di));//初始化操作
		//di.cbSize = sizeof(di);//设置打印文件大小

		if (GetPrintStatus(docName, strPrint.c_str()) < 0)
		{
			break;
		}
		if (MemDc.StartDoc(&di) <= 0)
		{
			DeleteDC(hdcprint);
			break;
		}
		if (MemDc.StartPage() <= 0)
		{
			DeleteDC(hdcprint);
			break;
		};
		CFont TitleFont;
		TitleFont.CreateFont(-MulDiv(g_Globle.m_fontSize, MemDc.GetDeviceCaps(LOGPIXELSY), 72),
			0,
			0,
			0, 
			g_Globle.m_bold > 0 ? FW_BOLD : FW_NORMAL,
			0, 0, 0, GB2312_CHARSET,
			OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS, DRAFT_QUALITY,
			VARIABLE_PITCH | FF_SWISS, _T("宋体"));

		CFont* oldfont = MemDc.SelectObject(&TitleFont);

		int xP = GetDeviceCaps(MemDc.m_hDC, LOGPIXELSX);    //x方向每英寸像素点数   
		int yP = GetDeviceCaps(MemDc.m_hDC, LOGPIXELSY);    //y方向每英寸像素点数   

		DOUBLE xPix = (DOUBLE)xP * 10 / 254;    //每 mm 宽度的像素   
		DOUBLE yPix = (DOUBLE)yP * 10 / 254;

		int count = vt.size();
		for (int i = 0; i < count; i++)
		{
			ST_YS* st = &vt[i];
			if (st->strValue.IsEmpty())
			{
				continue;
			}

			if (st->width)
			{
				int mm_x, mm_y;
				PixToMm(st->x, st->y, mm_x, mm_y);
				MemDc.TextOutW(mm_x*xPix, mm_y*yPix, st->strValue);
				//MemDc.DrawText(st->strValue, &CRect(st->x, st->y, st->x + st->width,
				//	st->y + g_Globle.m_fontSize), DT_WORDBREAK | DT_EDITCONTROL);
			}
			else
				MemDc.TextOutW(st->x, st->y, st->strValue);
		}
		MemDc.EndPage();
		MemDc.EndDoc();
		DeleteDC(MemDc.Detach());
	} while (0);
	return true;
}

bool CGloble::Print(CImage& img)
{
	USES_CONVERSION;
	string strPrint = W2A(m_PrinterName);
	if (m_strIniFilePath.IsEmpty())
	{
		return false;
	}
	CString docName;
	{
		char tmp[1024] = { 0 };
		sprintf_s(tmp, "1905doc_%x", ::GetTickCount());
		docName = tmp;
	}

	do
	{
		HDC hdcprint = 0; // 定义一个设备环境句柄
		DOCINFO di = { sizeof(DOCINFO),docName,NULL };//定义一个打印作业
		if ((hdcprint = CreateDCA("WINSPOOL", strPrint.c_str(), 0, NULL)) == 0)
		{
			break;
		}
		
		if (GetPrintStatus(docName, strPrint.c_str()) < 0)
		{
			break;
		}
		if (StartDoc(hdcprint, &di) <= 0)
		{
			DeleteDC(hdcprint);
			break;
		}
		if (StartPage(hdcprint) <= 0)
		{
			DeleteDC(hdcprint);
			break;
		};
		//打印机走纸,开始打印
		img.BitBlt(hdcprint, 0, 0);
		EndPage(hdcprint);
		EndDoc(hdcprint);
		DeleteDC(hdcprint);

		string buffer;
		int time = 150;

		if (buffer.empty())
			time = 70;
		else
			sscanf_s(buffer.c_str(), "%d", &time);
		for (int i = 0; i < time; i++)
		{
			const int ret = GetPrintStatus(docName, strPrint.c_str());
			if (0 == ret)
				return true;
			else if (ret < 0)
				return false;
			::Sleep(100);
		}
		DeleteJob(docName, strPrint.c_str());
		return false;

	} while (0);
	return false;
}

//像素转毫米
void CGloble::PixToMm(int pix, int piy, int& mm_x, int& mm_y)
{
	HDC hdc = CreateDC(_T("display"), NULL, NULL, NULL);
	// 得到显示设备的水平单位英寸像素数
	int iLOGPIXELSX = GetDeviceCaps(hdc, LOGPIXELSX);
	// 得到显示设备的垂直单位英寸像素数
	int iLOGPIXELSY = GetDeviceCaps(hdc, LOGPIXELSY);

	DOUBLE xPix = (DOUBLE)iLOGPIXELSX * 10 / 254; //得到水平每mm的像素个数   
	DOUBLE yPix = (DOUBLE)iLOGPIXELSY * 10 / 254;//得到垂直每mm的像素个数   

	mm_x = pix / xPix;
	mm_y = piy / yPix;
}

//获取票纸像素大小
void CGloble::GetPix(int& width, int& height)
{
	int w = ::GetPrivateProfileInt(L"PIAO", L"width", 0, m_strIniFilePath);
	int h = ::GetPrivateProfileInt(L"PIAO", L"height", 0, m_strIniFilePath);
	if (w!=0 && h!=0)
	{
		//一英寸等于25.4mm 
		HDC hdc = CreateDC(_T("display"), NULL, NULL, NULL);
		// 得到当前显示设备的水平单位英寸像素数
		int iLOGPIXELSX = GetDeviceCaps(hdc, LOGPIXELSX);
		// 得到当前显示设备的垂直单位英寸像素数
		int iLOGPIXELSY = GetDeviceCaps(hdc, LOGPIXELSY);
		width = (iLOGPIXELSX * w) / 25.4;
		height = (iLOGPIXELSY * h) / 25.4;
	}
}