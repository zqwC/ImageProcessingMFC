
// ImagePView.cpp : CImagePView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageP.h"
#endif

#include "ImagePDoc.h"
#include "ImagePView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImagePView

IMPLEMENT_DYNCREATE(CImagePView, CView)

BEGIN_MESSAGE_MAP(CImagePView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CImagePView::OnFileOpen)
	ON_COMMAND(ID_32771, &CImagePView::OnHisteq)
	ON_COMMAND(ID_32772, &CImagePView::OnGauss5x5)
	ON_COMMAND(ID_32774, &CImagePView::OnGauss3x3)
	ON_COMMAND(ID_32777, &CImagePView::OnLaplace4)
	ON_COMMAND(ID_32778, &CImagePView::OnLaplace8)
END_MESSAGE_MAP()

// CImagePView 构造/析构

CImagePView::CImagePView()
{
	// TODO:  在此处添加构造代码
	m_font.CreateFontW(-20,			// 逻辑高度
		0,							// 平均宽度，取默认值
		0,							// 行角度为0，水平
		0,							// 字符角度0，正立
		FW_NORMAL,					// 正常体
		FALSE,						// 不倾斜
		FALSE,						// 不加下划线
		FALSE,						// 不加删除线
		GB2312_CHARSET,				// 中文字符集
		OUT_STROKE_PRECIS,
		CLIP_STROKE_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN,
		L"楷体_GB2312"); 

}

CImagePView::~CImagePView()
{
}

BOOL CImagePView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style &= ~WS_BORDER;
	return CView::PreCreateWindow(cs);
}

// CImagePView 绘制

void CImagePView::OnDraw(CDC* pDC)
{
	CImagePDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) 
		return;
	m_pDoc = pDoc;
	// TODO:  在此处为本机数据添加绘制代码
	CRect rect;
	
	// 获取客户区的大小
	GetClientRect(rect);
	// 设置为灰色背景
	//pDC->FillSolidRect(rect, RGB(230, 230, 230));     
	// 左上角始终显示原始图像
	if (pDoc->m_dib.m_IsUseOBJ)
	{
		AutoShow(L"原始图像");
	}
}


// CImagePView 打印

BOOL CImagePView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImagePView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CImagePView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CImagePView 诊断

#ifdef _DEBUG
void CImagePView::AssertValid() const
{
	CView::AssertValid();
}

void CImagePView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImagePDoc* CImagePView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagePDoc)));
	return (CImagePDoc*)m_pDocument;
}
#endif //_DEBUG


// CImagePView 消息处理程序

/************************************************************************
 *	默认初始化
 ************************************************************************/
void CImagePView::DefaultInitial()
{
	m_imageHeight = GetDocument()->m_dib.GetHeight();
	m_imageWidth = GetDocument()->m_dib.GetWidth();
	m_AutoShowNum = 0;
	
}

/************************************************************************
 *	自动显示
 ************************************************************************/
void CImagePView::AutoShow(LPCTSTR title)
{
	int x, y;
	if (m_AutoShowNum < 3)
	{
		x = (m_imageWidth+20)*m_AutoShowNum;
		y = 0;
	}
	else
	{
		x = (m_imageWidth+20)*(m_AutoShowNum - 3);
		y = m_imageHeight + 30;
	}
	GetDC()->SelectObject(m_font);
	GetDC()->SetBkMode(TRANSPARENT);	// 字体背景色透明
	GetDC()->TextOut(x, y, title);
	GetDocument()->m_dib.Draw(GetDC()->m_hDC, x, y+20);
	m_AutoShowNum++;
}

/************************************************************************
 *	打开文件
 ************************************************************************/
void CImagePView::OnFileOpen()
{
	// TODO:  在此添加命令处理程序代码
	// 打开文件对话框
	CFileDialog ImageFile(TRUE, _T("BMP"), _T("*.BMP"),
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("位图文件(*.BMP)|*.BMP|"));
	// 文件是否打开
	if (ImageFile.DoModal() == IDOK)
	{
		// 分离并销毁以前的图像
		m_pDoc->getDib()->Destroy();
		// 加载当前打开的图像
		m_pDoc->getDib()->Load(ImageFile.GetPathName());
		// 是否加载成功
		if (!m_pDoc->getDib()->IsNull())
		{
			//  是灰度图？
			if (!m_pDoc->getDib()->IsGray())
			{
				MessageBox(L"不是灰度图像！");
			}	
			else
			{	// 是灰度图像，设置图像可操作
				m_pDoc->m_dib.m_IsUseOBJ = TRUE;
				// 保存文件名
				m_pDoc->m_dib.m_filename = ImageFile.GetPathName();
				// 对view中的相关特性初始化
				DefaultInitial();
			}	
		}
		else
		{
			AfxMessageBox(L"图片加载失败！");
		}
	}
	Invalidate();
}

/************************************************************************
 *	直方图均衡化
 ************************************************************************/
void CImagePView::OnHisteq()
{
	// TODO:  在此添加命令处理程序代码

	// 重新加载图像
	m_pDoc->AfreshLoad();

	if (m_pDoc->m_dib.m_IsUseOBJ)
	{
		m_pDoc->m_dib.Histeq();
		AutoShow(L"直方图均衡化后:");
	}
	else
	{
		MessageBox(L"请先打开一幅灰度图像(.BMP格式)！");
	}
}

/************************************************************************
 *	高斯平均3x3
 ************************************************************************/
void CImagePView::OnGauss3x3()
{
	// TODO:  在此添加命令处理程序代码

	// 重新加载图像
	m_pDoc->AfreshLoad();

	if (m_pDoc->m_dib.m_IsUseOBJ)
	{
		//clock_t start, finish;
		//start = clock(); //时间测试
		m_pDoc->m_dib.Gauss3x3();
		//finish = clock();
		//double ti =(finish - start) / CLOCKS_PER_SEC;
		//CString str;
		//str.Format(L"%d",ti);
		//MessageBox(str);
		AutoShow(L"高斯平均3x3:");
	}
	else
	{
		MessageBox(L"请先打开一幅灰度图像(.BMP格式)！");
	}
}

/************************************************************************
 *	高斯平均5*5
 ************************************************************************/
void CImagePView::OnGauss5x5()
{
	// TODO:  在此添加命令处理程序代码
	
	// 重新加载图像
	m_pDoc->AfreshLoad();
	
	if (m_pDoc->m_dib.m_IsUseOBJ)
	{
		m_pDoc->m_dib.Gauss5x5();
		AutoShow(L"高斯平均5x5:");
	}
	else
	{
		MessageBox(L"请先打开一幅灰度图像(.BMP格式)！");
	}
}

/************************************************************************
 *	拉普拉斯（4）
 *	4邻域算子
 ************************************************************************/
void CImagePView::OnLaplace4()
{
	// TODO:  在此添加命令处理程序代码

	// 重新加载图像
	m_pDoc->AfreshLoad();

	if (m_pDoc->m_dib.m_IsUseOBJ)
	{
		m_pDoc->m_dib.Laplace4();
		AutoShow(L"拉普拉斯算子4-邻域:");
		//Invalidate();
	}
	else
	{
		MessageBox(L"请先打开一幅灰度图像(.BMP格式)！");
	}
}

/************************************************************************
*	拉普拉斯（8）
*	8邻域算子
************************************************************************/
void CImagePView::OnLaplace8()
{
	// TODO:  在此添加命令处理程序代码

	// 重新加载图像
	m_pDoc->AfreshLoad();

	if (m_pDoc->m_dib.m_IsUseOBJ)
	{
		m_pDoc->m_dib.Laplace8();
		AutoShow(L"拉普拉斯算子8邻域:");
	}
	else
	{
		MessageBox(L"请先打开一幅灰度图像(.BMP格式)！");
	}
}
