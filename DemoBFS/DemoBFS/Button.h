#pragma once
class Button
{
public:
	CBitmap Normal, Onclick;
	double xs, ys;
	double xt, yt;
	double vx;
	double ax;
	bool isOnclick = false;
public:
	Button();
	~Button();
	bool isClickon(int x, int y);
	void Init(int normal, int onclick, double XS, double YS, double XT, double YT, double VX, double AX);
	void Update();
	void Draw(CDC* pDC);
	void Delete();
};

