/*
  Автор(с): Санников, Кулебякин, Ступак и Пархоменко
  Название команды: AGRAGE
  e-mail:   sdk96@mail.ru
*/
#if! defined(_GAME_VILLAGE_)
#define _GAME_VILLAGE_
#if _MSC_VER > 1000
#pragma once
#endif
#define  MAX_BUTTONS  2



// менюшная кнопка
struct sButton {
	TCHAR    caption[32];
	GLint    x;
	GLint    y;
	GLint    width;
	GLint    height;
	COLORREF color;
	COLORREF color_state;
	COLORREF color_active;
	int      id;

	sButton(void){}
	sButton(int pid, const TCHAR* pcaption, GLint px, GLint py, GLint pwidth, GLint pheight, COLORREF pcolor_state, COLORREF pcolor_active) {
		_tcscpy(caption, pcaption);
		x            = px;
		y            = py;
		width        = pwidth;
		height       = pheight;
		color_state  = pcolor_state;
		color_active = pcolor_active;
		color        = color_state;
		id           = pid;
	}


	bool  MouseMove(LPPOINT pt) {
		RECT rect = { x, y, x + width, y + height };
		if(PtInRect(&rect, *pt)) {
			color = color_active;
			return true;
		} else
			color = color_state;
		return false;
	}


	int MouseDown(const LPPOINT pt) {
		if(this->MouseMove(pt))
			return id;
		return 0;
	}
};







// класс-игры
class xGame : public xDlgScene {
private:
	GLuint        tex_cursor;
	GLuint        tex_digits;
	GLuint        tex_alpha;
	xTerrain*     terrain;
	xUser*        user;
	xSkyBox       skybox;
	xCamera       camera;
	GLuint        tex_life;
	GLVECTOR4     light_pos;

	enum {
		GAME_MENU  = 1,
		GAME_START = 2,
		GAME_OVER  = 8,
		GAME_QUIT  = 16
	};
	int       state_game;
	sButton   buttons[MAX_BUTTONS];

public:
	xGame(HINSTANCE hInstance);
	~xGame();
private:
	virtual  void  OnRender(void);
	virtual  void  OnCreate(HDC hDC, HWND  hwnd);
	virtual  void  OnDestroy(void);
	virtual  void  OnMouseDown(WPARAM wParam, int x, int y);
private:
	void  Initialize(void);
	void  DrawMenu(void);
	void  DrawGameOver(void);
};











#endif










