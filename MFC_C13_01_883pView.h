
// MFC_C13_01_883pView.h: CMFCC1301883pView 클래스의 인터페이스
//

#pragma once

#define NONE_MODE	0
#define DRAW_MODE	1

class CMFCC1301883pView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCC1301883pView() noexcept;
	DECLARE_DYNCREATE(CMFCC1301883pView)

// 특성입니다.
public:
	CMFCC1301883pDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCC1301883pView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	int m_nMode;
	bool m_bStart;
	CPoint m_ptOrign;
	CPoint m_ptPrev;
	COLORREF m_colorPoint;
	COLORREF m_colorPolygon;
	COLORREF m_colorCurve;
	bool m_bVisiblePolygon;
	bool m_bVisibleCurve;
	int m_nCtrlPointNum;
	CPoint m_ptControl[50];
	CPoint m_ptCurve[50];
	void DrawControlPolygon(CDC* pDC, bool bDrawLine);
	CPoint ComputeCurvePoint(int nDegree, CPoint AryCtrlPoint[], float fDelta);
	void CreateCurvePoint(int nDegree, int nPointNum, CPoint ptControl[], CPoint ptCurve[]);
	void DrawBezierCurve(CDC* pDC, bool bLine);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDrawBezier();
	afx_msg void OnViewPolygon();
	afx_msg void OnUpdateViewPolygon(CCmdUI* pCmdUI);
	afx_msg void OnViewCurve();
	afx_msg void OnUpdateViewCurve(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // MFC_C13_01_883pView.cpp의 디버그 버전
inline CMFCC1301883pDoc* CMFCC1301883pView::GetDocument() const
   { return reinterpret_cast<CMFCC1301883pDoc*>(m_pDocument); }
#endif

