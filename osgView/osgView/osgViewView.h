
// osgViewView.h : interface of the CosgViewView class
//

#pragma once

#include "MFC_OSG.h"
class CosgViewView : public CView
{
protected: // create from serialization only
	CosgViewView();
	DECLARE_DYNCREATE(CosgViewView)

// Attributes
public:
	CosgViewDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	 cOSG* mOSG;
	 bool stereo;
    //HANDLE mThreadHandle;
    CRenderingThread* mThreadHandle;
// Implementation
public:
	virtual ~CosgViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in osgViewView.cpp
inline CosgViewDoc* CosgViewView::GetDocument() const
   { return reinterpret_cast<CosgViewDoc*>(m_pDocument); }
#endif

