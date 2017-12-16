#pragma once
#include "Button.h"
#include "resource.h"
#include "Graph.h"
class Menu
{
public:
	Button Create;
	Button Find;
	Button Clear;
public:
	Menu();
	~Menu();
	void Init();
	void Draw(CDC *pDC);
	void Delete();
};

class PlayMenu {
public:
	CBitmap Normal;
	CBitmap PlayOn;
	bool isPlayOn = false;
	bool NeedTextbox = false;
	double xs, ys, xt, yt;
	double vy, ay;
public:
	bool isClickOn(int x, int y);
	void Init(int NorID, int PlayID);
	void InitVelocity(double XS, double YS, double XT, double YT, double VY, double AY);
	void Update();
	void Draw(CDC *pDC);
	void Delete();
};
