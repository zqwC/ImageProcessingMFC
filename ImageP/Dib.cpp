#include "stdafx.h"
#include "Dib.h"

CDib::CDib()
{
	m_IsUseOBJ = FALSE;
	m_pixelValue = 0;
}


CDib::~CDib()
{
}

/************************************************************************
 *	直方图均衡化
 ************************************************************************/
void CDib::Histeq()
{
	long lWidth, lHeight;	
	// 检索图像的长和宽
	lWidth = GetWidth();
	lHeight = GetHeight();
	
	// 保存图片中各个灰度级的像素的个数
	UINT pixelNum[256] = { 0 };

	// 统计图片中每个灰度级像素的个数
	for (int i = 0; i < lHeight; i++)	// 每行
	for (int j = 0; j < lWidth; j++)	// 每列
	{
		// 检索像素点（j，i）的灰度值
		m_pixelValue = (BYTE)GetPixel(j, i);
		// 将灰度值作为数组的索引，并计数
		pixelNum[m_pixelValue] = pixelNum[m_pixelValue] + 1;
	}

	// **1 原始直方图 （Original histogram ） [ 归一化 即计算各个灰度级的概率 ]
	double Sk[256] = { 0 };
	for (int i = 0; i < 256; i++)
		Sk[i] = pixelNum[i] / (lHeight*lWidth*1.0);

	// **2 累计直方图 （Cumulative histogram）
	double Tk[256] = { 0 };
	for (int i = 0; i < 256; i++)
	if (i == 0)
		Tk[i] = Sk[i];
	else
		Tk[i] = Tk[i - 1] + Sk[i];

	// **3 取整扩展（Integral expansion）[ 公式：pixelCumu = int[(L-1)*pixelCumu+0.5]（L为灰度级）]
	for (int i = 0; i < 256; i++)
		Tk[i] = UINT8((256 - 1)*Tk[i] + 0.5);

	// **4 对灰度值进行映射并写入
	for (int i = 0; i < lHeight; i++)	// 每行
	for (int j = 0; j < lWidth; j++)	// 每列
	{
		m_pixelValue = (BYTE)GetPixel(j, i);
		m_pixelValue = (BYTE)Tk[m_pixelValue];
		SetPixel(j, i, RGB(m_pixelValue, m_pixelValue, m_pixelValue));
	}
}// !void CDib::histeq()

/************************************************************************
 *	使用3x3模版漫游图像
 *  注意：1.处理时对应系数相乘，且已经将计算后的像素值写入图像
 *		  2.模板系数的数据类型为float型
 ************************************************************************/
void CDib::Template3x3(int arr[],double coef)
{
	// 检索图像的长和宽
	lWidth = GetWidth();
	lHeight = GetHeight();

	double sum = 0;
	for (DWORD i = 0; i < lHeight - 3; i++)	// 每行
	for (DWORD j = 0; j < lHeight - 3; j++)	// 每列
	{	// 使用模板进行卷积计算
		sum = arr[0] * (BYTE)GetPixel(j, i + 2) + arr[1] * (BYTE)GetPixel(j + 1, i + 2) + arr[2] * (BYTE)GetPixel(j + 2, i + 2) + sum;
		sum = arr[3] * (BYTE)GetPixel(j, i + 1) + arr[4] * (BYTE)GetPixel(j + 1, i + 1) + arr[5] * (BYTE)GetPixel(j + 2, i + 1) + sum;
		sum = arr[6] * (BYTE)GetPixel(j, i) + arr[7] * (BYTE)GetPixel(j + 1, i) + arr[8] * (BYTE)GetPixel(j + 2, i) + sum;
		sum = (int)(sum*coef);
		//计算绝对值
		sum = fabs(sum);

		//如果小于0，强制赋值为0
		if (sum<0)
			sum = 0;

		//如果大于255，强制赋值为255
		if (sum>255)
			sum = 255;
		SetPixel(j + 1, i + 1, RGB(sum, sum, sum));
	}
}

/************************************************************************
*	使用5x5模版漫游图像
*	注意：1.处理时对应系数相乘，且已经将计算后的像素值写入图像
*		  2.模板系数的数据类型为float型
 ************************************************************************/
void CDib::Template5x5(int arr[],double coef)
{
	// 检索图像的长和宽
	lWidth = GetWidth();
	lHeight = GetHeight();

	double sum = 0;
	for (DWORD i = 0; i < lHeight - 4; i++)		// 每行
	for (DWORD j = 0; j < lWidth - 4; j++)		// 每列
	{	// 使用模板进行卷积计算
		sum = arr[0] * (BYTE)GetPixel(j, i + 4) + arr[1] * (BYTE)GetPixel(j + 1, i + 4) + arr[2] * (BYTE)GetPixel(j + 2, i + 4) +
			  arr[3] * (BYTE)GetPixel(j + 3, i + 4) + arr[4] * (BYTE)GetPixel(j + 4, i + 4) + sum;
		sum = arr[5] * (BYTE)GetPixel(j, i + 3) + arr[6] * (BYTE)GetPixel(j + 1, i + 3) + arr[7] * (BYTE)GetPixel(j + 2, i + 3) +
			  arr[8] * (BYTE)GetPixel(j + 3, i + 3) + arr[9] * (BYTE)GetPixel(j + 4, i + 3) + sum;
		sum = arr[10] * (BYTE)GetPixel(j, i + 2) + arr[11] * (BYTE)GetPixel(j + 1, i + 2) + arr[12] * (BYTE)GetPixel(j + 2, i + 2) +
			  arr[13] * (BYTE)GetPixel(j + 3, i + 2) + arr[14] * (BYTE)GetPixel(j + 4, i + 2) + sum;
		sum = arr[15] * (BYTE)GetPixel(j, i + 1) + arr[16] * (BYTE)GetPixel(j + 1, i + 1) + arr[17] * (BYTE)GetPixel(j + 2, i + 1) +
			  arr[18] * (BYTE)GetPixel(j + 3, i + 1) + arr[19] * (BYTE)GetPixel(j + 4, i + 1) + sum;
		sum = arr[20] * (BYTE)GetPixel(j, i) + arr[21] * (BYTE)GetPixel(j + 1, i) + arr[22] * (BYTE)GetPixel(j + 2, i) +
			  arr[23] * (BYTE)GetPixel(j + 3, i) + arr[24] * (BYTE)GetPixel(j + 4, i) + sum;

		//sum = (int)(1 / 273 * sum);	// 显示全黑
		//sum = (int)1 / 273 * sum;		// 显示全黑
		sum = (int)(sum*coef);
		//计算绝对值
		sum = fabs(sum);

		//如果小于0，强制赋值为0
		if (sum<0)
			sum = 0;

		//如果大于255，强制赋值为255
		if (sum>255)
			sum = 255;
		SetPixel(j+2, i+2, RGB(sum, sum, sum));
	}
}

/************************************************************************
 *	高斯平均3x3
 ************************************************************************/
void CDib::Gauss3x3()
{
	// 设定模板系数
	int arr[9] = { 1, 2, 1,
				   2, 4, 2,
				   1, 2, 1 };
	// 使用3x3常规模版
	//Template3x3(arr,1/16);
	Template3x3(arr, (double)1 / 16);
}

/************************************************************************
 *	高斯平均5x5
 ************************************************************************/
void CDib::Gauss5x5()
{
	//	设定模板系数：
	int arr[25] = { 1,  4,  7,  4, 1,
					4, 16, 26, 16, 4,
					7, 26, 41, 26, 7,
					4, 16, 26, 16, 4,
					1,  4,  7,  4,  1 };
	// 使用5x5常规模版
	//Template5x5(arr,1/273);
	//Template3x3(arr,float(1 / 273));
	Template5x5(arr, (double)1 / 273);
}

/************************************************************************
 *	拉普拉斯算子（4）
 *	说明：4 指中心系数为4的算子
 ************************************************************************/
void CDib::Laplace4()
{
	// 设定算子系数
	int arr[9] = { 0, -1, 0,
				   -1, 4, -1,
				   0, -1, 0  };

	// 使用3x3常规模版
	Template3x3(arr,1);
}

/************************************************************************
*	拉普拉斯算子（8）
*	说明：8 指中心系数为8的算子
************************************************************************/
void CDib::Laplace8()
{
	// 设定算子系数
	int arr[9] = { -1, -1, -1,
				   -1,  12, -1,
				   -1, -1, -1 };

	// 使用3x3常规模版
	Template3x3(arr,1);
}

/************************************************************************
 *	判断是否是灰度图像
 *	是		返回TRUE
 *	不是	返回FALSE
 ************************************************************************/		
BOOL CDib::IsGray()
{
	COLORREF col;
	// 检索像素（2，2）的颜色值，因图像尺寸限制，值不宜过大
	col = GetPixel(2, 2);
	// 解析各个颜色分量
	BYTE r, g, b;
	r = GetRValue(col);
	g = GetGValue(col);
	b = GetBValue(col);
	// 每个像素的位数为8位且取出的各个颜色分量相等则为灰度图
	//if ((8 == GetBPP()) && (r == g == b)) // 错误
	if ((8 != GetBPP()) || (r != g))
		return FALSE;
	else
		return TRUE;
}//!BOOL CDib::isGray()
