
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "Menu.h"
#include "Graph.h"
#include <vector>
using namespace std;
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();
	void Init();
	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	CBitmap Board, Background;
	CEdit SourceBox, TargetBox;
	int Source, Target;

	bool OnCreate = false, OnFind = false;
	bool isSetEdge = false; int idEdge = 0;
	bool isSetVertex = false; int idVertex = 0;
	bool isPlayFind = false;
	int n = 0, m = 0;
	vector<Edge*> Edges;
	vector<Vertex*> Vertices;

	int Queue[20], Ql, Qr;
	int trace[20];
	int G[20][20];
	vector<Edge*> EdgesW;

	Menu MyMenu;
	PlayMenu Play;

	void Delete();
	void InitQueue();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

