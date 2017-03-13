#ifndef __CDIB_H
#define __CDIB_H

class CDib :public CObject
{
public:
	CString m_fileName;						// 图像文件名
	BITMAPFILEHEADER m_bitmapFileHeader;	// 位图文件头
	BITMAPINFO* m_pBitmapInfo;				// 位图信息
	BITMAPINFOHEADER* m_pBitmapInfoHeader;	// 位图信息头
	RGBQUAD* m_pRGB;						// 颜色表首地址

	BYTE* m_pData;							// 图像像素数据首地址
	BYTE* m_pDib;							// 位图首地址(去掉位图文件头后图像的首地址)
	BOOL m_valid;							// IsValid函数的返回值
public:
	CDib();
	~CDib();
	//void LoadFile(const char* dibFileName);// 加载文件
	void ReadFile(CString  dibFileName);// 读取文件
	BOOL IsValid();		// 检验是否加载了bmp格式图片. 是, 返回TRUE, 否则返回FALSE
	CString GetFileName();  // 获取文件名
	
	BITMAPINFO* GetInfo();// 获取图像信息头
	DWORD GetSize();// 获取图像大小
	UINT GetWidth();// 获取图像宽度
	UINT GetHeight();// 获取图像高度

	RGBQUAD* GetRGB();// 获取颜色表
	UINT GetNumberOfColors();// 获取颜色数目
	WORD GetPaletteSize(LPBYTE lpDIB);// 调色板大小

	BYTE* GetData();// 获取图像的像素数据

	WORD DIBNumColors(LPBYTE lpDIB);
	//void SaveFile(const char* filename);
public:
	
};

#endif // !__CDIB_H
