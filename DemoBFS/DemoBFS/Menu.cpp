#include "stdafx.h"
#include "Menu.h"

Menu::Menu()
{
}

Menu::~Menu()
{
}

void Menu::Init()
{
	Create.Init(IDB_CREATENOR, IDB_CREATEON, 1000, 0, 850, 0, -7, 0.16);
	Find.Init(IDB_FINDNOR, IDB_FINDON, 1100, 50, 850, 50, -9, 0.154);
	Clear.Init(IDB_CLEARNOR, IDB_CLEARON, 1200, 100, 850, 100, -11, 0.166);
}

void Menu::Draw(CDC * pDC)
{
	Create.Update();
	Create.Draw(pDC);
	Find.Update();
	Find.Draw(pDC);
	Clear.Update();
	Clear.Draw(pDC);
}

void Menu::Delete()
{
	Create.Delete();
	Find.Delete();
	Clear.Delete();
}

bool PlayMenu::isClickOn(int x, int y)
{
	//Check In
	if (x >= xs + 81 && x <= xs + 202 && y >= ys && y <= ys + 119) return true;
	return false;
}

void PlayMenu::Init(int NorID, int PlayID)
{
	Normal.LoadBitmap(NorID);
	PlayOn.LoadBitmap(PlayID);
}

void PlayMenu::InitVelocity(double XS, double YS, double XT, double YT, double VY, double AY) {
	xs = XS; ys = YS;
	xt = XT; yt = YT;
	vy = VY; ay = AY;
}

void PlayMenu::Update()
{
	if (ys != yt) {
		vy += ay;
		ys += vy;
		double cl = yt - ys;
		cl = cl - 2 * cl*(cl < 0);
		if (cl < 1) {
			ys = yt;
			xs = xt;
			NeedTextbox = true;
		}
	}
}

void PlayMenu::Draw(CDC * pDC)
{
	CBitmap maskBMP;
	CDC maskDC, srcDC;
	maskDC.CreateCompatibleDC(pDC);
	srcDC.CreateCompatibleDC(pDC);
	PrepareMask(&Normal, &maskBMP, COLORREF(RGB(0, 0, 0)), 0, 0);
	CBitmap *OldMask = maskDC.SelectObject(&maskBMP);
	CBitmap *OldSrc = srcDC.SelectObject(&Normal);
	pDC->BitBlt(xs, ys, 283, 119, &maskDC, 0, 0, SRCAND);
	pDC->BitBlt(xs, ys, 283, 119, &srcDC, 0, 0, SRCPAINT);
	
	if (isPlayOn) {
		maskBMP.DeleteObject();
		PrepareMask(&PlayOn, &maskBMP, COLORREF(RGB(0, 0, 0)), 0, 0);
		maskDC.SelectObject(&maskBMP);
		srcDC.SelectObject(&PlayOn);
		pDC->BitBlt(xs + 81, ys, 121, 119, &maskDC, 0, 0, SRCAND);
		pDC->BitBlt(xs + 81, ys, 121, 119, &srcDC, 0, 0, SRCPAINT);
	}

	maskBMP.DeleteObject();
	maskDC.SelectObject(OldMask);
	maskDC.DeleteDC();
	srcDC.SelectObject(OldSrc);
	srcDC.DeleteDC();
}

void PlayMenu::Delete()
{
	Normal.DeleteObject();
	PlayOn.DeleteObject();
}
