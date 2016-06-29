/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#if! defined(_GLSCENE_VILLAGE_)
#define _GLSCENE_VILLAGE_
#if _MSC_VER > 1000
#pragma once
#endif
#define  CLASS_NAME  _T("VillageGame")





// Базовый класс сцены OpenGL(накидал по простому)
class  xDlgScene {
private:
	HINSTANCE     instance;
	LPDIRECTSOUND sound;
	HWND          hwnd;
	HGLRC         cgl;
	bool          fullscreen;
	SIZE          screen;
	GLfloat       glelapsed;
	GLfloat       gltime;
	GLfloat       glfps;
	HDC           cdc;
public:
	xDlgScene(HINSTANCE hInstance);
	~xDlgScene();
public:
	bool  Create(const TCHAR* caption, int width, int height, bool _fulllscreen = false);
	int   Run(void);	
	void  Destroy(void);
	void  Begin2D(void);
	void  End2D(void);

	HWND  Handle(void) {
		return hwnd;
	}

	HINSTANCE Module(void) {
		return instance;
	}

	SIZE&  Screen(void) {
		return screen;
	}


	GLfloat fTime(void) {
		return gltime;
	}


	GLfloat fElapsed(void) {
		return glelapsed;
	}

	GLfloat  fFPS(void) {
		return glfps;
	}


	LPDIRECTSOUND  Sound(void) {
		return sound;
	}

protected:
	virtual  void  OnRender(void) {}
	virtual  void  OnCreate(HDC hDC, HWND  hwnd) {}
	virtual  void  OnMouseDown(WPARAM wParam, int x, int y) {}
	virtual  void  OnDestroy(void) {}
private:
	static LRESULT CALLBACK  WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void  this_render(void);
	void  this_loaden(void);
};





#endif



