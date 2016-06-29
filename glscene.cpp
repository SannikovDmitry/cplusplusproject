/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"
xDlgScene*  ptr_scene = NULL;







xDlgScene::xDlgScene(HINSTANCE hInstance) {
	instance   = hInstance;
	hwnd       = NULL;
	cgl        = NULL;
	gltime     = 0.0f;
	glelapsed  = 0.0f;
	glfps      = 0.0f;
	fullscreen = false;
	ptr_scene  = this;
	cdc        = NULL;
}



xDlgScene::~xDlgScene() {
	this->Destroy();
}





// Метод создаёт окно и сцену OpenGL
bool  xDlgScene::Create(const TCHAR* caption, int width, int height, bool pfullscreen) {
	PIXELFORMATDESCRIPTOR  fmt;
	WNDCLASS cls;
	DEVMODE mode;
	int pixel;

	ZeroMemory(&cls,  sizeof(WNDCLASS));
	cls.style         = CS_OWNDC;
	cls.lpfnWndProc   = (WNDPROC) xDlgScene::WndProc;
	cls.hInstance     = instance;
	cls.hCursor       = LoadCursor(NULL, IDC_ARROW);
	cls.hIcon         = LoadIcon(instance, MAKEINTRESOURCE(IDI_GAMEICON));
	cls.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	cls.lpszClassName = CLASS_NAME;
    
	if(! RegisterClass(&cls))
		return  false;

	if(pfullscreen) 
		hwnd = CreateWindow(CLASS_NAME, caption, WS_POPUPWINDOW, 0, 0, width, height, NULL, NULL, instance, NULL);
	else {
		hwnd = CreateWindow(CLASS_NAME, caption, WS_POPUPWINDOW,
				GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2,
				width, height, NULL, NULL, instance, NULL);
	}

	if(! hwnd) {
		UnregisterClass(CLASS_NAME, instance);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOREPOSITION | SWP_NOMOVE | SWP_NOSIZE);

	cdc = GetDC(hwnd);

	ZeroMemory(&fmt, sizeof(PIXELFORMATDESCRIPTOR));
	fmt.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
	fmt.nVersion   = 1u;
	fmt.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	fmt.iPixelType = PFD_TYPE_RGBA;
	fmt.cColorBits = 32u;
	fmt.cDepthBits = 32u;
	fmt.iLayerType = PFD_MAIN_PLANE;

	if((pixel  = ChoosePixelFormat(cdc, &fmt)) == 0)  // выбор формата пикселов
		return false;

	if(SetPixelFormat(cdc, pixel, &fmt) == 0) // установка формата пикселов для сцены
		return false;

	if(DescribePixelFormat(cdc, GetPixelFormat(cdc), sizeof(PIXELFORMATDESCRIPTOR), &fmt) == 0)
		return false;

	if((cgl = wglCreateContext(cdc)) == 0) 
		return false;

	screen.cx = width;
	screen.cy = height;

	wglMakeCurrent(cdc,  cgl);

//	glEnable(GL_DITHER);
	glShadeModel(GL_SMOOTH);

	glClearDepth(1.0f); 

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glDepthRange(0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0f, GLfloat(width / height), 1.0f, 1400.0f);

	gluLookAt(0.0f, 0.0f, 8.0f,  // положение камеры
		      0.0f, 0.0f, 0.0f,  // центр сцены
			  0.0f, 1.0f, 0.0f); // направление по-y

	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);

	if(pfullscreen) { // Полноэкранный режим
		ZeroMemory(&mode, sizeof(DEVMODE));
		mode.dmSize             = sizeof(DEVMODE);
		mode.dmPelsWidth        = screen.cx;
		mode.dmPelsHeight       = screen.cy;
		mode.dmBitsPerPel       = 32u;
		mode.dmDisplayFrequency = 75u;
		mode.dmFields           = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
		ChangeDisplaySettings(&mode, CDS_FULLSCREEN);
		ShowCursor(FALSE);
	}

	fullscreen = pfullscreen;

	InvalidateRect(hwnd, NULL, TRUE);
	this->this_loaden();

#ifdef _MSC_VER  // для среды Microsoft Visual C++

		if(DirectSoundCreate(NULL, &sound, NULL) == DS_OK)
			sound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

#else // здесь для Borland C++, Dev, GCC...

		HRESULT res = CoCreateInstance(CLSID_DirectSound, NULL, CLSCTX_INPROC_SERVER, IID_IDirectSound, (LPVOID*)&sound);
		if(FAILED(res))
			return NULL;
		sound->Initialize(NULL);
		sound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);

#endif

	this->OnCreate(cdc, hwnd);

	SetTimer(hwnd, 777u, 1u, NULL);
	return true;
}






// Цикл сообщений, почему рендеринг не здесь происходит, с PeekMessage у меня с ней долгая история
// иногда бывает такая зараза касаемой интерполяции
int xDlgScene::Run(void) {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(GetMessage(&msg, NULL, 0u, 0u)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);	
	}
	return 0;
}






void  xDlgScene::Begin2D(void) {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, screen.cx, screen.cy, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}



void  xDlgScene::End2D(void) {
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
}





void  xDlgScene::Destroy(void) {
	if(sound != NULL)
		sound->Release();
	sound = NULL;

	if(cgl != NULL) {
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(cgl);
	}
	cgl = NULL;

	if(cdc != NULL)
		ReleaseDC(hwnd, cdc);
	cdc = NULL;

	if(fullscreen)
		ChangeDisplaySettings(NULL, 0);

	if(hwnd != NULL) {
		DestroyWindow(hwnd);
		UnregisterClass(CLASS_NAME, instance);
	}
	hwnd = NULL;
}






void  xDlgScene::this_render(void) {
	static GLfloat fstart = (GLfloat)timeGetTime();
	static GLfloat ftick  = fstart;
	static GLint   cnt    = 0;
	GLfloat tick;

	tick   = (GLfloat) timeGetTime();
	gltime = tick;

	if((tick - ftick) > 0.0f) {
		glelapsed = tick - ftick; // временной промежуток DELTA между кадрами
		ftick     = tick;
	}

	cnt++;
	if((tick - fstart) >= 1.0f) {
		if(cnt == 0)
			cnt = 1;
		glfps  = glelapsed / GLfloat(cnt);
		fstart = tick;
		cnt    = 0;
	}


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	this->OnRender();
	glFlush();
	SwapBuffers(cdc);
}






LRESULT CALLBACK  xDlgScene::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
	case WM_TIMER:
		if(GetFocus() != hWnd)
			break;
		ptr_scene->this_render();
		break;
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		ptr_scene->OnMouseDown(wParam, (int)LOWORD(lParam), (int)HIWORD(lParam));
		break;
	case WM_DESTROY:
		KillTimer(hWnd, 777u);
		ptr_scene->OnDestroy();
		PostQuitMessage(0u);
		break;
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
   }
   return 0;
}




	
// загрузка игры
void  xDlgScene::this_loaden(void) {
	const TCHAR* str = _T("Подождите, игра загружается...");
	const TCHAR* ptr = _T("Игра работает на: OpenGL, DirectSound");
	int   mode = 0;
	SIZE  size = { 0L, 0L };
	RECT  rect;
	HGDIOBJ old, old1;

	HBRUSH brush = CreateSolidBrush(RGB(0x0, 0x48, 0x9A));
	HPEN   pen   = CreatePen(PS_SOLID, 2, RGB(0xFF, 0xA9, 0));
	
	HFONT font   = CreateFont(-24, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_OUTLINE_PRECIS,
									  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, VARIABLE_PITCH, _T("Arial"));
	if(font == NULL)
		font = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	mode = SaveDC(cdc);
	
	old = SelectObject(cdc, font);
	SetBkMode(cdc, TRANSPARENT);
	SetTextColor(cdc, RGB(0xFF, 0xA9, 0));
	GetTextExtentPoint32(cdc, str, _tcslen(str), &size);

	SetRect(&rect, 200, 200, screen.cx - 200, screen.cy - 200);
	FillRect(cdc, &rect, brush);

	InflateRect(&rect, 80, 80);
	FrameRect(cdc, &rect, brush);

	old1 = SelectObject(cdc, pen);
	MoveToEx(cdc, screen.cx / 2 - size.cx / 2, screen.cy / 2 + 10, NULL);
	LineTo(cdc, screen.cx / 2 + size.cx / 2, screen.cy / 2 + 10);

	TextOut(cdc, screen.cx / 2 - size.cx / 2, screen.cy / 2 - size.cy, str, _tcslen(str));

	SetRect(&rect, 0, screen.cy - 50, screen.cx, screen.cy);
	Rectangle(cdc, rect.left, rect.top, rect.right, rect.bottom);
	SetTextColor(cdc, RGB(0x77, 0x77, 0x77));
	TextOut(cdc, 10, screen.cy - 36, ptr, _tcslen(ptr));

	RestoreDC(cdc, mode);
	SelectObject(cdc, old);
	SelectObject(cdc, old1);
	DeleteObject(pen);
	DeleteObject(font);
	DeleteObject(brush);
}
