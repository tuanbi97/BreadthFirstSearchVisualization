
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "DemoBFS.h"
#include "ChildView.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}

void CChildView::Init()
{
	Background.LoadBitmap(IDB_BACKGROUND);
	OnCreate = false;
	OnFind = false;
	MyMenu.Init();
	Play.Init(IDB_PLAYNOR, IDB_PLAYON);
	Edges.clear();
	Vertices.clear();
	InitQueue();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
//	ON_WM_TIMER()
ON_WM_ERASEBKGND()
ON_WM_LBUTTONDOWN()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
//ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	Init();
	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC srcDC, dstDC;
	srcDC.CreateCompatibleDC(&dc);
	dstDC.CreateCompatibleDC(&dc);
	Board.CreateCompatibleBitmap(&dc, 1000, 700);
	CBitmap *OldSrc = srcDC.SelectObject(&Background);
	CBitmap *OldDst = dstDC.SelectObject(&Board);
	//srcDC.FillSolidRect(0, 0, 1000, 700, COLORREF(RGB(0,0,0)));
	dstDC.BitBlt(0, 0, 1000, 700, &srcDC, 0, 0, SRCCOPY);
	for (int i = 0;i < m;i++) {
		Edges[i]->Draw(&dstDC, Vertices, false);
	}
	for (int i = 0;i < EdgesW.size();i++) {
		bool Stop = EdgesW[i]->Update(G, Vertices, EdgesW, trace, Queue, Target, Ql, Qr);
		EdgesW[i]->Draw(&dstDC, Vertices, true);
		if (Stop) {
			isPlayFind = false;
			if (trace[Target] != -1) {
				int finish = Target;
				while (finish != Source) {
					int size = EdgesW.size();
					EdgesW[trace[finish]]->Color = COLORREF(RGB(0, 255, 0));
					if (EdgesW[trace[finish]]->u == finish) finish = EdgesW[trace[finish]]->v;
					else finish = EdgesW[trace[finish]]->u;
				}
			}
			Play.isPlayOn = false;
			isPlayFind = false;
		}
	}
	for (int i = 0;i < n;i++) {
		Vertices[i]->Collide(Vertices);
		Vertices[i]->Draw(&dstDC);
	}
	Play.Update();
	Play.Draw(&dstDC);
	if (Play.NeedTextbox) {
		RECT rect1 = { Play.xs + 11, Play.ys + 67, Play.xs + 65, Play.ys + 87 };
		RECT rect2 = { Play.xs + 218, Play.ys + 67, Play.xs + 269, Play.ys + 87 };
		SourceBox.Create(WS_CHILD | WS_VISIBLE | ES_CENTER, rect1, this, 1000);
		TargetBox.Create(WS_CHILD | WS_VISIBLE | ES_CENTER, rect2, this, 1001);
		Play.NeedTextbox = false;
	}
	MyMenu.Draw(&dstDC);
	dc.BitBlt(0, 0, 1000, 700, &dstDC, 0, 0, SRCCOPY);
	srcDC.SelectObject(OldSrc);
	dstDC.SelectObject(OldDst);
	Board.DeleteObject();
	
	// TODO: Add your message handler code here
	
	// Do not call CWnd::OnPaint() for painting messages
}


void CChildView::Delete()
{
	Background.DeleteObject();
	MyMenu.Delete();
	Play.Delete();
	for (int i = 0;i < m;i++) delete Edges[i];
	Edges.clear();
	for (int i = 0;i < n;i++) {
		Vertices[i]->Delete();
		delete Vertices[i];
		trace[i] = 0;
	}
	Vertices.clear();
	::DestroyWindow(SourceBox);
	::DestroyWindow(TargetBox);
}

void CChildView::InitQueue()
{
	Ql = 0;
	Qr = -1;
	for (int i = 0;i < n;i++) {
		for (int j = 0;j < n;j++) G[i][j] = 0;
		trace[i] = -1;
	}
	EdgesW.clear();
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;
	//return CWnd::OnEraseBkgnd(pDC);
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
	if (isPlayFind) return;
	if (isSetEdge){
		if (point.x >= 0 && point.y >= 0 && point.x <= 1000 && point.y <= 700) {
			int ClickOnVertex = -1;
			for (int i = 0;i < n;i++) {
				if ((point.x - Vertices[i]->x)*(point.x - Vertices[i]->x) + (point.y - Vertices[i]->y)*(point.y - Vertices[i]->y) < 35 * 35) {
					ClickOnVertex = i;
					break;
				}
			}
			if (ClickOnVertex != -1 && ClickOnVertex != Edges[idEdge]->u) {
				int u = Edges[idEdge]->u;
				int v = ClickOnVertex;
				Vertices[u]->isSelected = false;
				Vertices[v]->isSelected = false;
				Edges[idEdge]->v = v;
				Edges[idEdge]->Color = RGB(169, 225, 250);
				Edges[idEdge]->xt = Vertices[v]->x;
				Edges[idEdge]->yt = Vertices[v]->y;
				isSetEdge = false;
			}
			else {
				Vertices[Edges[idEdge]->u]->isSelected = false;
				delete Edges[idEdge];
				m--;
				Edges.pop_back();
				isSetEdge = false;
			}
		}
		else {
			Vertices[Edges[idEdge]->u]->isSelected = false;
			delete Edges[idEdge];
			m--;
			Edges.pop_back();
			isSetEdge = false;
		}
	}
	else
	if (isSetVertex) {
		Vertices[idVertex]->isSelected = false;
		Vertices[idVertex]->x = point.x;
		Vertices[idVertex]->y = point.y;
		isSetVertex = false;
	}
	else
	if (!isSetEdge) {
		if (MyMenu.Create.isClickon(point.x, point.y)) {
			MyMenu.Create.isOnclick ^= 1;
			OnCreate ^= 1;
			if (OnCreate) {
				if (OnFind) {
					::DestroyWindow(SourceBox);
					::DestroyWindow(TargetBox);
					Play.InitVelocity(358, 581, 358, 700, 10, 0.4);
					InitQueue();
					for (int i = 0;i < n;i++) Vertices[i]->isSelected = false;
				}
				OnFind = false;
				MyMenu.Find.isOnclick = false;
			}
		}
		else
			if (MyMenu.Find.isClickon(point.x, point.y)) {
				MyMenu.Find.isOnclick ^= 1;
				OnFind ^= 1;
				if (OnFind) {
					OnCreate = false;
					MyMenu.Create.isOnclick = false;
					Play.InitVelocity(358, 700, 358, 581, -10, 0.4);
				}
				else {
					::DestroyWindow(SourceBox);
					::DestroyWindow(TargetBox);
					Play.InitVelocity(358, 581, 358, 700, 10, 0.4);
					InitQueue();
					for (int i = 0;i < n;i++) Vertices[i]->isSelected = false;
				}
			}
			else
				if (MyMenu.Clear.isClickon(point.x, point.y)) {
					for (int i = 0;i < m;i++) delete Edges[i];
					Edges.clear();
					for (int i = 0;i < n;i++) {
						Vertices[i]->Delete();
						delete Vertices[i];
					}
					Vertices.clear();
					m = 0; n = 0;
					OnCreate = false; 
					MyMenu.Create.isOnclick = false;
					if (OnFind) {
						::DestroyWindow(SourceBox);
						::DestroyWindow(TargetBox);
						Play.InitVelocity(358, 581, 358, 700, 10, 0.4);
						InitQueue();
						for (int i = 0;i < n;i++) Vertices[i]->isSelected = false;
					}
					OnFind = false;
					MyMenu.Find.isOnclick = false;
					isSetEdge = false; idEdge = 0;
				}
				else
					if (OnCreate) {
						if (point.x >= 0 && point.y >= 0 && point.x <= 1000 && point.y <= 700) {
							int ClickOnVertex = -1;
							for (int i = 0;i < n;i++) {
								if ((point.x - Vertices[i]->x)*(point.x - Vertices[i]->x) + (point.y - Vertices[i]->y)*(point.y - Vertices[i]->y) < 35 * 35) {
									ClickOnVertex = i;
									break;
								}
							}
							if (ClickOnVertex == -1) {
								if (n < 15) {
									n++;
									Vertex *tmp;
									tmp = new Vertex;
									tmp->Init(n - 1, point.x, point.y);
									Vertices.push_back(tmp);
								}
							}
							else {
								isSetVertex = true;
								idVertex = ClickOnVertex;
								Vertices[ClickOnVertex]->isSelected = true;
							}
						}
					}
					else 
						if (OnFind) {
							if (Play.isClickOn(point.x, point.y)) {
								Play.isPlayOn = true;
								isPlayFind = true;
								Source = 0; Target = 0;
								TCHAR tSource[6], tTarget[6];
								::GetWindowText(SourceBox, tSource, 6);
								::GetWindowText(TargetBox, tTarget, 6);
								int len = wcslen(tSource);
								for (int i = 0;i < len;i++) Source = Source * 10 + tSource[i] - '0';
								len = wcslen(tTarget);
								for (int i = 0;i < len;i++) Target = Target * 10 + tTarget[i] - '0';
								Source--; Target--;
								if (Source < n && Target < n && Source >= 0 && Target >= 0) {
									InitQueue();
									for (int i = 0;i < n;i++) Vertices[i]->isSelected = false;
									Vertices[Source]->isSelected = true;
									Queue[0] = Source;
									Ql = 0; Qr = 0;
									for (int i = 0;i < m;i++) {
										int u = Edges[i]->u;
										int v = Edges[i]->v;
										G[u][0]++; G[u][G[u][0]] = v;
										G[v][0]++; G[v][G[v][0]] = u;
									}
									if (Source != Target) {
										for (int i = 1;i <= G[Source][0];i++) {
											Edge *tmp;
											tmp = new Edge;
											int vNext = G[Source][i];
											tmp->Init(Source, vNext, Vertices[Source]->x, Vertices[Source]->y, Vertices[vNext]->x, Vertices[vNext]->y, Vertices[Source]->x, Vertices[Source]->y, 5);
											tmp->Color = COLORREF(RGB(255, 255, 255));
											trace[vNext] = 0;
											EdgesW.push_back(tmp);
											Qr++;
											Queue[Qr] = vNext;
											break;
										}
									}
									else {
										Play.isPlayOn = false;
										isPlayFind = false;
									}
								}
								else {
									Play.isPlayOn = false;
									isPlayFind = false;
								}
							}
						}
	}
}


void CChildView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (isSetEdge) {
			Edges[idEdge]->xt = point.x;
			Edges[idEdge]->yt = point.y;
			for (int i = 0;i < n;i++) Vertices[i]->isSelected = false;
			Vertices[Edges[idEdge]->u]->isSelected = true;
			for (int i = 0;i < n;i++) {
				if ((point.x - Vertices[i]->x)*(point.x - Vertices[i]->x) + (point.y - Vertices[i]->y)*(point.y - Vertices[i]->y) < 35 * 35) {
					Vertices[i]->isSelected = true;
					break;
				}
			}
	}
	else
		if (isSetVertex) {
				Vertices[idVertex]->x = point.x;
				Vertices[idVertex]->y = point.y;
		}
	CWnd::OnMouseMove(nFlags, point);
}


void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (!isSetEdge) {
		if (OnCreate) {
			if (point.x >= 0 && point.y >= 0 && point.x <= 1000 && point.y <= 700) {
				int ClickOnVertex = -1;
				for (int i = 0;i < n;i++) {
					if ((point.x - Vertices[i]->x)*(point.x - Vertices[i]->x) + (point.y - Vertices[i]->y)*(point.y - Vertices[i]->y) < 35 * 35) {
						ClickOnVertex = i;
						break;
					}
				}
				if (ClickOnVertex != -1) {
					Vertices[ClickOnVertex]->isSelected = true;
					isSetEdge = true; isSetVertex = false;
					m++;
					idEdge = m - 1;
					Edge *tmp;
					tmp = new Edge;
					tmp->Color = RGB(255, 255, 255);
					tmp->u = ClickOnVertex; tmp->v = -1;
					tmp->xs = Vertices[ClickOnVertex]->x; tmp->ys = Vertices[ClickOnVertex]->y;
					tmp->xt = Vertices[ClickOnVertex]->x; tmp->yt = Vertices[ClickOnVertex]->y;
					Edges.push_back(tmp);
				}
			}
		}
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}
