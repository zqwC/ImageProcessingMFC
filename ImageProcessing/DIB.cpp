#include "stdafx.h"
#include "Dib.h"
#include "windowsx.h"

CDib::CDib()
{
}

CDib::~CDib()
{
	GlobalFreePtr(m_pBitmapInfo);
}

void CDib::ReadFile(CString  dibFileName)
{
	m_fileName = dibFileName;
	CFile dibFile((LPCTSTR)m_fileName, CFile::modeRead);// 定义文件对象，只读方式
	dibFile.Read((void *)&m_bitmapFileHeader, sizeof(BITMAPFILEHEADER));// 读取文件头
	if (m_bitmapFileHeader.bfType == 0x4d42) // 文件类型必须为‘BM’B=66=0x42 M=77=0x4d
	{
		/************************去掉文件头************************/
		// 用文件的长度减去文件头的长度即为图像的长度
		DWORD ImageSize = dibFile.GetLength() - sizeof(BITMAPFILEHEADER);
		m_pDib = (BYTE*)GlobalAllocPtr(GMEM_MOVEABLE, ImageSize);	// 从堆中分配特定字节的空间
		dibFile.Read((void*)m_pDib, ImageSize);						// 从位图首地址开始读取
		dibFile.Close();											// 释放文件

		/************************获取图像相关信息*****************/
		m_pBitmapInfo = (BITMAPINFO*)m_pDib;							// 位图信息头和颜色表组成位图信息
		m_pBitmapInfoHeader = (BITMAPINFOHEADER*)m_pDib;				// 位图信息头在前
		m_pRGB = (RGBQUAD*)(m_pDib + m_pBitmapInfoHeader->biSize);		// 颜色表在后
		int m_numberOfColors = GetNumberOfColors();						// 颜色数目
		if (m_pBitmapInfoHeader->biClrUsed == 0)						// 位图实际使用的颜色表中的颜色数
			m_pBitmapInfoHeader->biClrUsed = m_numberOfColors;
		DWORD colorTableSize = m_numberOfColors*sizeof(RGBQUAD);		// 颜色表大小
		// 图像像素数据首地址为位图首地址加偏移量（偏移量=信息头+调色板）
		m_pData = m_pDib + m_pBitmapInfoHeader->biSize + colorTableSize;// 图像数据区
		// 没有调色板
		if (m_pRGB == (RGBQUAD*)m_pData)	 
			m_pRGB = NULL;

		m_pBitmapInfoHeader->biSizeImage = GetSize();					// 图像的大小
		m_valid = TRUE;
	}
	else
	{ // 位图文件加载失败
		m_valid = FALSE;
		AfxMessageBox(L"This isn't a bitmap file!");
	}
}

BOOL CDib::IsValid()
{
	return m_valid;
}

CString CDib::GetFileName()
{
	return m_fileName;
}

// 检索图像的宽度
UINT CDib::GetWidth()
{
	return (UINT)m_pBitmapInfoHeader->biWidth;
}

// 检索图像的高度
UINT CDib::GetHeight()
{
	return (UINT)m_pBitmapInfoHeader->biHeight;
}

// 获取图像的大小
DWORD CDib::GetSize()
{
	if (m_pBitmapInfoHeader->biSizeImage != 0)
		return m_pBitmapInfoHeader->biSizeImage;
	else
	{
		DWORD height = (DWORD)GetHeight();
		DWORD width = (DWORD)GetWidth();
		return height*width;
	}
}

// 返回位图的颜色数目
UINT CDib::GetNumberOfColors()
{
	int numberOfColors;
	// 图像中使用的颜色数为0，且像素的位数小于9
	if ((m_pBitmapInfoHeader->biClrUsed == 0) &&
		(m_pBitmapInfoHeader->biBitCount < 9))
	{
		switch (m_pBitmapInfoHeader->biBitCount)
		{
		case 1:
			numberOfColors = 2; break;
		case 4:
			numberOfColors = 16; break;
		case 8:
			numberOfColors = 256;
		}
	}
	else
		numberOfColors = (int)m_pBitmapInfoHeader->biClrUsed;
	return numberOfColors;
}

// 返回信息的起始地址
BITMAPINFO* CDib::GetInfo()
{
	return m_pBitmapInfo;
}

// 返回每个像素的位数
WORD CDib::DIBNumColors(LPBYTE lpDIB)
{
	WORD wBitCount; // DIB bit count
	wBitCount = ((LPBITMAPCOREHEADER)lpDIB)->bcBitCount;
	switch (wBitCount)
	{
	case 1:
		return 2;
	case 4:
		return 16;
	case 8:
		return 256;
	default:
		return 0;
	}
}

// 返回调色板的起始地址
RGBQUAD* CDib::GetRGB()
{
	return m_pRGB;
}

// 返回调色板的大小
WORD CDib::GetPaletteSize(LPBYTE lpDIB)
{
	return (DIBNumColors(lpDIB)*sizeof(RGBTRIPLE));
}

// 返回像素数据的起始地址
BYTE* CDib::GetData()
{
	return m_pData;
}





//void CDib::SaveFile(const char* filename)
//{// 此函数只能保存经处理后宽度、高度均没有改变大小的图像
//	strcpy_s(m_fileName, filename);
//	CFile dibFile((LPCTSTR)m_fileName, CFile::modeCreate | CFile::modeWrite);
//	dibFile.Write((void*)&bitmapFileHeader, sizeof(BITMAPFILEHEADER));
//	dibFile.Write((void*)pDib, size);
//	dibFile.Close();
//}





