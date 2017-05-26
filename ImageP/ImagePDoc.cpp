
// ImagePDoc.cpp : CImagePDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "ImageP.h"
#endif

#include "ImagePDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImagePDoc

IMPLEMENT_DYNCREATE(CImagePDoc, CDocument)

BEGIN_MESSAGE_MAP(CImagePDoc, CDocument)
END_MESSAGE_MAP()


// CImagePDoc 构造/析构

CImagePDoc::CImagePDoc()
{
	// TODO:  在此添加一次性构造代码

}

CImagePDoc::~CImagePDoc()
{
}

BOOL CImagePDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO:  在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImagePDoc 序列化

void CImagePDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO:  在此添加存储代码
	}
	else
	{
		// TODO:  在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CImagePDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CImagePDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CImagePDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImagePDoc 诊断

#ifdef _DEBUG
void CImagePDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImagePDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImagePDoc 命令

/************************************************************************
 *	检索DIB对象
 ************************************************************************/
CDib* CImagePDoc::getDib()
{
	return &m_dib;
}

/************************************************************************
*	重新加载图像
*  说明：
*		加载成功返回TRUE
*		加载失败返回FALSE
************************************************************************/
BOOL CImagePDoc::AfreshLoad()
{
	// 分离并删除原图像
	m_dib.Destroy();
	// 加载图像
	m_dib.Load(m_dib.m_filename);
	// 是否加载成功
	if (!m_dib.IsNull())
		return TRUE;
	return FALSE;
}