
// osgViewView.cpp : implementation of the CosgViewView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "osgView.h"
#endif

#include "osgViewDoc.h"
#include "osgViewView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CosgViewView

IMPLEMENT_DYNCREATE(CosgViewView, CView)

BEGIN_MESSAGE_MAP(CosgViewView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CosgViewView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CosgViewView construction/destruction

CosgViewView::CosgViewView():stereo(false)
{
	// TODO: add construction code here

}

CosgViewView::~CosgViewView()
{
}

BOOL CosgViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CosgViewView drawing

void CosgViewView::OnDraw(CDC* /*pDC*/)
{
	CosgViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CosgViewView printing


void CosgViewView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CosgViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CosgViewView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CosgViewView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CosgViewView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CosgViewView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CosgViewView diagnostics

#ifdef _DEBUG
void CosgViewView::AssertValid() const
{
	CView::AssertValid();
}

void CosgViewView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CosgViewDoc* CosgViewView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CosgViewDoc)));
	return (CosgViewDoc*)m_pDocument;
}
#endif //_DEBUG


// CosgViewView message handlers


void CosgViewView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	// Get Filename from DocumentOpen Dialog
    CString csFileName = GetDocument()->GetFileName();

    // Init the osg class
    mOSG->InitOSG(csFileName.GetString());

    // Start the thread to do OSG Rendering
    //mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG); 
    mThreadHandle = new CRenderingThread(mOSG);
    mThreadHandle->start();
}


int CosgViewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	 mOSG = new cOSG(m_hWnd);

    return 1;
	
}


void CosgViewView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	 delete mThreadHandle;
    if(mOSG != 0) delete mOSG;

    //WaitForSingleObject(mThreadHandle, 1000);

    CView::OnDestroy();
}


BOOL CosgViewView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return true;
}


void CosgViewView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	// Pass Key Presses into OSG
    mOSG->getViewer()->getEventQueue()->keyPress(nChar);

    // Close Window on Escape Key
    if(nChar == VK_ESCAPE)
    {
        GetParent()->SendMessage(WM_CLOSE);
    }
	if(nChar == VK_F1)
    {
		stereo= !stereo;
		mOSG->getViewer()->getCamera()->setDisplaySettings(new osg::DisplaySettings()); 
        mOSG->getViewer()->getCamera()->getDisplaySettings()->setStereo(stereo);
    }
	
}
