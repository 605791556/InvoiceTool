#ifndef CGLOBLE_H
#define CGLOBLE_H

#include <vector>
#include <string.h>
#include <shlwapi.h>
#include <atlconv.h>
#include <atlimage.h>
#include <winspool.h>
#include <vector>

#pragma comment(lib,"Shlwapi.lib")
using std::string;
using std::vector;

#define  WM_UPDATEWND WM_USER+100

struct ST_YS
{
	int id;
	CString str;
	CString str2;
	CString strValue;
	int x;
	int y;
	int width;
	int height;
	ST_YS()
	{
		x = y = width = height = 0;
	}
};

class CGloble
{
public:
	CGloble();
	~CGloble();

public:
	//��ʼ��
	bool InitGloble();
	CString GetEXEPath();
	bool Print(CImage& img);
	bool Print(vector<ST_YS>& vt);

	bool Print2(vector<ST_YS>& vt);
	//����ת����
	void PixToMm(int pix,int piy,int& mm_x,int& mm_y);
	//��ȡƱֽ���ش�С
	void GetPix(int& width, int& height);
	//��תͼƬ180��
	void SpinImg(CImage& dstImg, const CImage& srcImg);

public:
	CString   m_strIniFilePath;
	CString   m_PrinterName;
	int       m_fontSize;
	int       m_bold;
	int       m_nShow;
};
extern CGloble g_Globle;
#endif // CGLOBLE_H
