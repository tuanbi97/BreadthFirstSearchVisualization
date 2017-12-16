#pragma once
#include "resource.h"
#include <vector>
using namespace std;

class Vertex {
public:
	int id;
	int x = 0, y = 0;
	bool isSelected = false;
	CBitmap Normal, Selected;
	double vx = 0, vy = 0;
	int xt, yt;
public:
	Vertex();
	~Vertex();
	void Init(int ID, int X, int Y);
	void Collide(vector<Vertex*> &Vertices);
	void Draw(CDC *pDC);
	void Delete();
};
class Edge {
public:
	int u = -1, v = -1;
	int xs = 0, ys = 0, xt = 0, yt = 0;
	double vel;
	double xc, yc;
	COLORREF Color{ RGB(169,225,250) };
public:
	Edge();
	~Edge();
	void Init(int U, int V, int XS, int YS, int XT, int YT, double XC, double YC, double Velocity);
	bool Update(int G[20][20], vector<Vertex*>& Vertices, vector<Edge*> &EdgesW, int trace[], int Queue[], int Target, int &Ql, int&Qr);
	void Draw(CDC *pDC, vector<Vertex*> &Vertices, bool OnAnimation);
};
void PrepareMask(CBitmap * pBmpSource, CBitmap * pBmpMask, COLORREF clrpTransColor, int iTransPixelX, int iTransPixelY);