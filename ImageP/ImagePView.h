
// ImagePView.h : CImagePView 类的接口
//

#pragma once

class CImagePView : public CView
{
protected: // 仅从序列化创建
	CImagePView();
	DECLARE_DYNCREATE(CImagePView)

// 特性
public:
	CImagePDoc* GetDocument() const;
	CImagePDoc* m_pDoc;
	CFile m_memImageFile;
	CFont m_font;
	int m_imageHeight;
	int m_imageWidth;
	UINT8 m_AutoShowNum;
// 操作
public:
	void DefaultInitial();
	void AutoShow(LPCTSTR title);
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImagePView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
	afx_msg void OnHisteq();

	afx_msg void OnGauss5x5();
	afx_msg void OnGauss3x3();
	afx_msg void OnLaplace4();
	afx_msg void OnLaplace8();
};

#ifndef _DEBUG  // ImagePView.cpp 中的调试版本
inline CImagePDoc* CImagePView::GetDocument() const
   { return reinterpret_cast<CImagePDoc*>(m_pDocument); }
#endif

