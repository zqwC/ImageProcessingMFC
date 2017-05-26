<style>
   h1{color:#ff0000;text-align: center;}
   h2{text-align: center;}
</style>
<section id="downloads" class="clearfix">
<a href="https://zhangqunwei.github.io/ImageProcessing/Release/Release.zip" title="测试环境为Visual Studio 2013" style="float:right;" id="view-on-github" class="button"><span>下载exe</span></a>    
<a href="https://zhangqunwei.github.io/ImageProcessing/sourceZIP/ImageP.zip" title="测试环境为Visual Studio 2013" style="float:right;" id="view-on-github" class="button"><span>下载项目源码</span></a>      
<hr>
</section> 


# 基于MFC的图像处理软件
   根据《图像处理》课程内容实现基于MFC的图像处理软件，目前仅实现直方图均衡化、
   线性平滑、线性锐化滤波三部分。

## GUI界面
  
  ![UI界面](https://zhangqunwei.github.io/ImageProcessing/gui.bmp)

## MFC做图像处理的若干缺点
1.MFC对图像处理的支持基本为零，需要自己写DIB图像格式的底层接口。
2.写出来的软件只能处理BMP一种图像格式，其它格式的不行。
3.在图像的操作中还存在一个字节对齐的问题。
4.图像是二维的，在MFC中获取图像的像素点时，如果调用库函数效率会特别低，如果用指针通过寻址的方式在一维中找到与二维对应的点。效率虽然高但很容易出错。
5.MFC具有很强的数据类型检查，编程中不仅要考虑图像处理的算法还得考虑数据的类型，所有很容易出错。
6.MFC中需要自己编写矩阵运算和复数运算，没有Matlab中那么方便。

总之，对于图像处理MFC并不适合，如果是研究图像处理的算法最好还是用Matlab，因为Matlab也叫矩阵实验室，支持各种各样的图像操作，而且使用还很方便。当然也可以用OpenCV，本人未用过也就不介绍了。MFC更适合做普通的Windows软件。
 
 
部分源码：
 ```C++
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

 ```
