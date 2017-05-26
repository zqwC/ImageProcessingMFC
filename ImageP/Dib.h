#pragma once
#ifndef __CDIB_H_
#define __CDIB_H_

#include "atlimage.h"
class CDib :
	public CImage
{
public:
	CDib();
	~CDib();
// 特性
public:
	CString m_filename;
	BOOL m_IsUseOBJ;	// 指示图像是否可使用
	BOOL m_IsUseTEM;	// 指示模板是否可用
	BYTE m_pixelValue;	// 保存像素点的灰度值
	DWORD  lWidth;
	DWORD lHeight;
// 操作
public:
	BOOL IsGray();				// 返回 TRUE 是灰度图像，FALSE不是灰度图像
	void Template3x3(int arr[], double coef);// 3x3常规模版
	void Template5x5(int arr[], double coef);// 5x5常规模版
	void Histeq();				// 直方图均衡化
	void Gauss3x3();			// 高斯平均3x3
	void Gauss5x5();			// 高斯平均5x5
	void Laplace4();			// 拉普拉斯算子（4）
	void Laplace8();			// 拉普拉斯算子（8）
};

 #endif // !__CDIB_H_
