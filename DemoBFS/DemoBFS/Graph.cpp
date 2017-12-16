#include "stdafx.h"
#include "Graph.h"

Edge::Edge()
{
}

Edge::~Edge()
{
}

void Edge::Init(int U, int V, int XS, int YS, int XT, int YT, double XC, double YC, double Velocity)
{
	u = U; v = V;
	xs = XS; ys = YS; xt = XT; yt = YT; xc = XC; yc = YC; vel = Velocity;
}

bool Edge::Update(int G[20][20], vector<Vertex*>& Vertices, vector<Edge*> &EdgesW, int trace[], int Queue[], int Target, int &Ql, int&Qr)
{
	if (xc != xt || yc != yt) {
		double ang = atan2(1.0*(yt - ys), 1.0*(xt - xs));
		xc += vel*cos(ang);
		yc += vel*sin(ang);
		if ((xt - xc)*(xt - xc) + (yt - yc)*(yt - yc) < 7) {
			xc = xt; yc = yt;
			Vertices[v]->isSelected = true;
			if (v == Target) {
				return true;
			}
			else {
				for (int i = 1;i <= G[u][0] ;i++) {
					int vNext = G[u][i];
					if (Vertices[vNext]->isSelected == false) {
						Edge *tmp;
						tmp = new Edge;
						tmp->Init(u, vNext, Vertices[u]->x, Vertices[u]->y, Vertices[vNext]->x, Vertices[vNext]->y, Vertices[u]->x, Vertices[u]->y, 5);
						tmp->Color = COLORREF(RGB(255, 255, 255));
						EdgesW.push_back(tmp);
						int mw = EdgesW.size();
						trace[vNext] = mw - 1;
						Qr++;
						Queue[Qr] = vNext;
						return false;
					}
				}
				Ql++;
				while (Ql <= Qr) {
					int uNext = Queue[Ql];
					for (int i = 1;i <= G[uNext][0];i++) {
						int vNext = G[uNext][i];
						if (Vertices[vNext]->isSelected == false) {
							Edge *tmp;
							tmp = new Edge;
							tmp->Init(uNext, vNext, Vertices[uNext]->x, Vertices[uNext]->y, Vertices[vNext]->x, Vertices[vNext]->y, Vertices[uNext]->x, Vertices[uNext]->y, 5);
							tmp->Color = COLORREF(RGB(255, 255, 255));
							EdgesW.push_back(tmp);
							int mw = EdgesW.size();
							trace[vNext] = mw - 1;
							Qr++;
							Queue[Qr] = vNext;
							return false;
						}
					}
					Ql++;
				}
				return true;
			}
		}
	}
	return false;
}

void Edge::Draw(CDC * pDC, vector<Vertex*> &Vertices, bool OnAnimation)
{
	CPen aPen; 
	aPen.CreatePen(PS_SOLID, 8, Color);
	CPen *OldPen = pDC->SelectObject(&aPen);
	if (u != -1) {
		xs = Vertices[u]->x;
		ys = Vertices[u]->y;
	}
	if (v != -1) {
		xt = Vertices[v]->x;
		yt = Vertices[v]->y;
	}
	if (!OnAnimation) xc = xt, yc = yt;
	pDC->MoveTo(xs, ys);
	pDC->LineTo(xc, yc);
	pDC->SelectObject(OldPen);
	aPen.DeleteObject();
}

Vertex::Vertex()
{
}

Vertex::~Vertex()
{
}

void Vertex::Init(int ID, int X, int Y)
{
	id = ID; x = X; y = Y;
	xt = X; yt = Y;
	Normal.LoadBitmap(IDB_BITMAP1 + ID);
	Selected.LoadBitmap(IDB_BITMAP16 + ID);
}

void Vertex::Collide(vector<Vertex*>& Vertices)
{
	int n = Vertices.size();
	for (int i = 0;i < n;i++){
		if (i != id) {
			double dist, mindist;
			dist = sqrt((Vertices[i]->x - x)*(Vertices[i]->x - x) + (Vertices[i]->y - y)*(Vertices[i]->y - y));
			mindist = 70;
			if (dist < mindist) {
				double ang = atan2(Vertices[i]->y - y, Vertices[i]->x - x);
				Vertices[i]->x += 3 * cos(ang);
				Vertices[i]->y += 3 * sin(ang);
			}
		}
	}
}

void Vertex::Draw(CDC * pDC)
{
	CDC srcDC, maskDC;
	CBitmap Mask;
	srcDC.CreateCompatibleDC(pDC);
	maskDC.CreateCompatibleDC(pDC);
	if (isSelected) {
		PrepareMask(&Selected, &Mask, 0, 0, 0);
		CBitmap *Oldmsk = maskDC.SelectObject(&Mask);
		CBitmap *Oldsrc = srcDC.SelectObject(&Selected);
		pDC->BitBlt(x - 35, y - 35, 70, 70, &maskDC, 0, 0, SRCAND);
		pDC->BitBlt(x - 35, y - 35, 70, 70, &srcDC, 0, 0, SRCPAINT);
		srcDC.SelectObject(Oldsrc);
		maskDC.SelectObject(Oldmsk);
	}
	else {
		PrepareMask(&Normal, &Mask, 0, 0, 0);
		CBitmap *Oldmsk = maskDC.SelectObject(&Mask);
		CBitmap *Oldsrc = srcDC.SelectObject(&Normal);
		pDC->BitBlt(x - 35, y - 35, 70, 70, &maskDC, 0, 0, SRCAND);
		pDC->BitBlt(x - 35, y - 35, 70, 70, &srcDC, 0, 0, SRCPAINT);
		srcDC.SelectObject(Oldsrc);
		maskDC.SelectObject(Oldmsk);
	}
	srcDC.DeleteDC();
	maskDC.DeleteDC();
}

void Vertex::Delete()
{
	Normal.DeleteObject();
	Selected.DeleteObject();
}

void PrepareMask(CBitmap * pBmpSource, CBitmap * pBmpMask, COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY)
{
	BITMAP bm;
	pBmpSource->GetObject(sizeof(BITMAP), &bm);
	pBmpMask->DeleteObject();
	pBmpMask->CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);
	CDC hdcSrc, hdcDst;
	hdcSrc.CreateCompatibleDC(NULL);
	hdcDst.CreateCompatibleDC(NULL);
	CBitmap* hbmSrcT = (CBitmap*)hdcSrc.SelectObject(pBmpSource);
	CBitmap* hbmDstT = (CBitmap*)hdcDst.SelectObject(pBmpMask);
	COLORREF clrTrans;
	if (clrpTransColor == NULL)
	{
		clrTrans = hdcSrc.GetPixel(iTransPixelX, iTransPixelY);
	}
	else
	{
		clrTrans = clrpTransColor;
	}
	COLORREF clrSaveBk = hdcSrc.SetBkColor(clrTrans);
	hdcDst.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &hdcSrc, 0, 0, SRCCOPY);

	COLORREF clrSaveDstText = hdcSrc.SetTextColor(COLORREF(RGB(255, 255, 255)));
	hdcSrc.SetBkColor(COLORREF(RGB(0, 0, 0)));

	hdcSrc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &hdcDst, 0, 0, SRCAND);

	hdcDst.SetTextColor(clrSaveDstText);

	hdcSrc.SetBkColor(clrSaveBk);
	hdcSrc.SelectObject(hbmSrcT);
	hdcDst.SelectObject(hbmDstT);

	hdcSrc.DeleteDC();
	hdcDst.DeleteDC();
}