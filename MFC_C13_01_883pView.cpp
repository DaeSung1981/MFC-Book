
// MFC_C13_01_883pView.cpp: CMFCC1301883pView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_C13_01_883p.h"
#endif

#include "MFC_C13_01_883pDoc.h"
#include "MFC_C13_01_883pView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCC1301883pView

IMPLEMENT_DYNCREATE(CMFCC1301883pView, CView)

BEGIN_MESSAGE_MAP(CMFCC1301883pView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCC1301883pView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_DRAW_BEZIER, &CMFCC1301883pView::OnDrawBezier)
	ON_COMMAND(ID_VIEW_POLYGON, &CMFCC1301883pView::OnViewPolygon)
	ON_UPDATE_COMMAND_UI(ID_VIEW_POLYGON, &CMFCC1301883pView::OnUpdateViewPolygon)
	ON_COMMAND(ID_VIEW_CURVE, &CMFCC1301883pView::OnViewCurve)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CURVE, &CMFCC1301883pView::OnUpdateViewCurve)
END_MESSAGE_MAP()

// CMFCC1301883pView 생성/소멸

CMFCC1301883pView::CMFCC1301883pView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_nCtrlPointNum = 0;
	m_colorPolygon = RGB(200, 200, 200);		//컨트롤 폴리곤의 색상- 밝은 회색
	m_colorCurve = RGB(255, 0, 0);				//베이지 곡선의 색상 설정 - 빨강색
	m_colorPoint = RGB(0, 0, 255);				//턴트롤 포인트의 색상 - 파란색

	m_bStart = FALSE;
	m_nMode = NONE_MODE;						//프로그램 모드를 초기 상태로 설정.

	m_bVisiblePolygon = TRUE;					//컨트롤 폴리곤이 보이도록 설정.
	m_bVisibleCurve = TRUE;						//베지어 곡선이 보이도록 설정.

	for (int i = 0; i < 50; i++)
	{
		m_ptControl[i] = 0;
	}
}

CMFCC1301883pView::~CMFCC1301883pView()
{
}

BOOL CMFCC1301883pView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CMFCC1301883pView 그리기

void CMFCC1301883pView::OnDraw(CDC* pDC)
{
	CMFCC1301883pDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	DrawControlPolygon(pDC, m_bVisiblePolygon);
	DrawBezierCurve(pDC, m_bVisibleCurve);
}


// CMFCC1301883pView 인쇄


void CMFCC1301883pView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCC1301883pView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CMFCC1301883pView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CMFCC1301883pView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CMFCC1301883pView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCC1301883pView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCC1301883pView 진단

#ifdef _DEBUG
void CMFCC1301883pView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCC1301883pView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCC1301883pDoc* CMFCC1301883pView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCC1301883pDoc)));
	return (CMFCC1301883pDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCC1301883pView 메시지 처리기


void CMFCC1301883pView::DrawControlPolygon(CDC* pDC, bool bDrawLine)
{
	// TODO: 여기에 구현 코드 추가.
	CRect rectPoint;
	CPen CtrlPointPen, PolygonPen, * OldPen;

	CtrlPointPen.CreatePen(PS_SOLID, 3, m_colorPoint);
	PolygonPen.CreatePen(PS_SOLID, 1, m_colorPolygon);
	OldPen = pDC->SelectObject(&CtrlPointPen);

	if (bDrawLine)
	{
		for (int i = 0; i < m_nCtrlPointNum; i++)
		{
			rectPoint = CRect(m_ptControl[i].x - 4, m_ptControl[i].y - 4, m_ptControl[i].x + 4, m_ptControl[i].y + 4);
			pDC->SelectObject(&CtrlPointPen);
			pDC->Ellipse(&rectPoint);
		}
		
		pDC->SelectObject(&PolygonPen);
		pDC->Polyline(m_ptControl, m_nCtrlPointNum);

	}

	pDC->SelectObject(OldPen);
	DeleteObject(CtrlPointPen);
	DeleteObject(PolygonPen);
}


CPoint CMFCC1301883pView::ComputeCurvePoint(int nDegree, CPoint AryCtrlPoint[], float fDelta)
{
	// TODO: 여기에 구현 코드 추가.
	CPoint fAryCtrlPoint[30];

	float fDelta_T;
	fDelta_T = 1.0f - fDelta;

	for (int i = 0; i <= nDegree; i++)
	{
		fAryCtrlPoint[i] = AryCtrlPoint[i];
	}

	for (int r = 1; r <= nDegree; r++)
		for (int i = 0; i <= nDegree; i++)
		{
			fAryCtrlPoint[i].x = (int)(fDelta_T * (float)fAryCtrlPoint[i].x + fDelta * (float)fAryCtrlPoint[i + 1].x);
			fAryCtrlPoint[i].y = (int)(fDelta_T * (float)fAryCtrlPoint[i].y + fDelta * (float)fAryCtrlPoint[i + 1].y);
 		}

	return fAryCtrlPoint[0];
}


void CMFCC1301883pView::CreateCurvePoint(int nDegree, int nPointNum, CPoint ptControl[], CPoint ptCurve[])
{
	// TODO: 여기에 구현 코드 추가.
	float fDelta, t;

	fDelta = 1.0f / (float)(nPointNum);
	t = 0.0f;

	for (int i = 0; i <= nPointNum; i++)
	{
		ptCurve[i] = ComputeCurvePoint(nDegree, ptControl, t);
		t = t + fDelta;
	}
}


void CMFCC1301883pView::DrawBezierCurve(CDC* pDC, bool bLine)
{
	// TODO: 여기에 구현 코드 추가.
	CPen CurvePen, * OldPen;
	CurvePen.CreatePen(PS_SOLID, 1, m_colorCurve);
	OldPen = pDC->SelectObject(&CurvePen);

	if (m_nCtrlPointNum > 1)
	{
		CreateCurvePoint(m_nCtrlPointNum - 1, 20, m_ptControl, m_ptCurve);

		if (bLine)
			pDC->Polyline(m_ptCurve, 21);
	}

	pDC->SelectObject(OldPen);
	DeleteObject(CurvePen);
}


void CMFCC1301883pView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nMode == DRAW_MODE)
	{
		//다시그리기 또는 처음 그리는 경우 : 초기화
		if (m_bStart == FALSE)
		{
			m_nCtrlPointNum = 0;
			for (int i = 0; i < 50; i++)
				m_ptControl[i] = 0;

			Invalidate();
		}

		CRect rectPoint(point.x - 4, point.y - 4, point.x + 4, point.y + 4);

		//선택한 지점에 컨트롤 포인트를 그린다.
		CClientDC dc(this);
		CPen NewPen, * OldPen;

		NewPen.CreatePen(PS_SOLID, 3, m_colorPoint);
		OldPen = dc.SelectObject(&NewPen);
		dc.Ellipse(&rectPoint);
		dc.SelectObject(OldPen);
		DeleteObject(NewPen);

		m_ptOrign = m_ptPrev = point;				//러버밴드 구현을 위한 임시 저장.
		m_ptControl[m_nCtrlPointNum++] = point;		// 현재 좌표 값을 배열에 저장.

		//마우스 캡쳐
		SetCapture();
		RECT rectClient;
		GetClientRect(&rectClient);
		ClientToScreen(&rectClient);
		// 마우스 이동 범위를 클라이언트 영역으로 제한.
		::ClipCursor(&rectClient);
		m_bStart = TRUE;							// 선을 그리기 시작했음을 저장.
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMFCC1301883pView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClientDC dc(this);

	if (m_nMode == DRAW_MODE)		//현재 그리기 모드이면
	{
		//러버 밴드 동작이 시작되었으면
		if (m_bStart)
		{
			dc.SetROP2(R2_NOT);
			dc.MoveTo(m_ptOrign);
			dc.LineTo(m_ptPrev);

			dc.MoveTo(m_ptOrign);
			dc.LineTo(point);

			m_ptPrev = point;
		}
	}

	CView::OnMouseMove(nFlags, point);
}


void CMFCC1301883pView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_nMode == DRAW_MODE)		// 현재 선 그리기 모드이면
	{
		// 현재 저장된 점들의 수가 PolyBezier() 함수가 요구하는 점의 개수와 동일하면
		if (m_nCtrlPointNum > 1)
		{
			if (m_bStart)			// 선 그리기가 시작되었다면.....
			{
				// 오른쪽을 눌러서 마지막 점을 저장
				m_ptControl[m_nCtrlPointNum++] = point;

				// 마지막 점의 컨트롤 포인트를 그린다.
				CRect rectPoint(point.x - 4, point.y - 4, point.x + 4, point.y + 4);
				CClientDC dc(this);
				CPen NewPen, * OldPen;
				NewPen.CreatePen(PS_SOLID, 3, m_colorPoint);
				OldPen = dc.SelectObject(&NewPen);
				dc.Ellipse(&rectPoint);
				dc.SelectObject(OldPen);
				DeleteObject(NewPen);

				//모든 컨트롤로 폴리곤을 그린다.
				DrawControlPolygon(&dc, m_bVisiblePolygon);

				//베지어 곡선을 완성한다.
				DrawBezierCurve(&dc, m_bVisibleCurve);
				//dc.PolyBezier(m_PointArray, m_nPointNum);

				//그리기 작업의 끝을 저장한다.
				m_bStart = FALSE;
				//마우스 캡쳐와 제한을 해제한다.
				::ReleaseCapture();
				::ClipCursor(NULL);
			}
		}
		else
		{
			AfxMessageBox(_T("적어도 2개 이상의 컨트롤 포인트가 있어야 합니다."));
		}
	}
	CView::OnRButtonDown(nFlags, point);
}


void CMFCC1301883pView::OnDrawBezier()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_nCtrlPointNum = 0;
	m_bStart = FALSE;

	for (int i = 0; i < 50; i++)
		m_ptControl[i] = 0;

	m_nMode = DRAW_MODE;
	Invalidate();
}


void CMFCC1301883pView::OnViewPolygon()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bVisiblePolygon = !m_bVisiblePolygon;
	Invalidate();
}


void CMFCC1301883pView::OnUpdateViewPolygon(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bVisiblePolygon);
}


void CMFCC1301883pView::OnViewCurve()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_bVisibleCurve = !m_bVisibleCurve;
	Invalidate();
}


void CMFCC1301883pView::OnUpdateViewCurve(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_bVisibleCurve);
}
