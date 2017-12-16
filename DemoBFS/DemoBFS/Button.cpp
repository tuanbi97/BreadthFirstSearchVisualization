#include "stdafx.h"
#include "Button.h"


Button::Button()
{
}

Button::~Button()
{
}

bool Button::isClickon(int x, int y)
{
	if (x >= (int)xs && x <= (int)xs + 140 && y >= (int)ys && y <= (int)ys + 45) return true;
	return false;
}

void Button::Init(int normal, int onclick, double XS, double YS, double XT, double YT, double VX, double AX)
{
	Normal.LoadBitmap(normal);
	Onclick.LoadBitmap(onclick);
	xs = XS; ys = YS; xt = XT; yt = YT; vx = VX; ax = AX;
}

void Button::Update()
{
	if (xs != xt) {
		vx += ax;
		xs += vx;
		double cl = xs - xt;
		if (cl < 0) cl = -cl;
		if (cl < 1) xs = xt;
	}
}

void Button::Draw(CDC* pDC)
{
	CDC srcDC;
	srcDC.CreateCompatibleDC(pDC);
	CBitmap* OldBMP = NULL;
	if (isOnclick == false) OldBMP = srcDC.SelectObject(&Normal);
	else OldBMP = srcDC.SelectObject(&Onclick);
	pDC->BitBlt(xs, ys, 140, 40, &srcDC, 0, 0, SRCCOPY);
	srcDC.SelectObject(OldBMP);
	srcDC.DeleteDC();
}

void Button::Delete()
{
	Normal.DeleteObject();
	Onclick.DeleteObject();
}
