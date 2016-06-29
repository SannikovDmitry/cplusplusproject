/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#if ! defined(_GLOBJECTS_VILLAGE_)
#define _GLOBJECTS_VILLAGE_
#if _MSC_VER > 1000
#pragma once
#endif




// Класс со статическими функциями для установки света, материала, альфа-смешиваний и т.д.
class xEffect {
public:

	static void SetLight(const LPGLCOLOR& mcolor, const LPGLCOLOR& ldiffuse, const LPGLCOLOR& lambient, GLfloat shinnes = (128.0f * 0.7f)) {
		const GLfloat specular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        glLightfv(GL_LIGHT0, GL_DIFFUSE,  (const GLfloat*)ldiffuse);
        glLightfv(GL_LIGHT0, GL_AMBIENT,  (const GLfloat*)lambient);
	//	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	//	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); 

		glEnable(GL_COLOR_MATERIAL);
	//	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
	//	glMaterialf(GL_FRONT, GL_SHININESS, shinnes);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		glColor4fv((const GLfloat*)mcolor);
    }

	static void  LightPos(const LPGLVECTOR4& hpos) {
		glLightfv(GL_LIGHT0, GL_POSITION, (const GLfloat*)hpos);
	}

	// Активация освещения
	static void  BeginLight(void) {
        glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	static void  EndLight(void) {
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

      
	// Включить режим альфа-смешивания
	// GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA | GLenum src = GL_SRC_ALPHA, GLenum dst = GL_ONE
    static void ModeAlpha(const LPGLCOLOR& color, GLenum src = GL_SRC_ALPHA, GLenum dst = GL_ONE) {
		glEnable(GL_BLEND);
        glBlendFunc(src, dst); 
		glColor4fv((const GLfloat*)color);
    }
	static void EndAlpha(void) {
		glDisable(GL_BLEND);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}


	// Установка прозрачности текстуры и вершины TGA-файл
	static void  ModeClip(GLfloat glass = 0.0f) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, glass); 
	}
	static void  EndClip(void) {
		glDisable(GL_ALPHA_TEST);
	}


	// Включить режим трафарета
	static void ModeStencil(void) {
		glClearStencil(0.0f);
		glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT);
		glDepthMask(GL_FALSE);// Выключим запись в буфер глубины
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_STENCIL_TEST); // Включим тест буфера трафарета

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Не рисовать в буфер цвета
		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE); 
	}

	static void EndStencil(const LPGLCOLOR& color) {
		// Включить визуализацию цветового буфера для всех компонент
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

		// Нарисовать теневой прямоугольник на весь экран
		glColor4fv((const GLfloat*)color);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glStencilFunc(GL_NOTEQUAL, 0, 0xFFFFFFFFL);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-4.1f, 4.1f,-0.10f);
			glVertex3f(-4.1f,-4.1f,-0.10f);
			glVertex3f( 4.1f, 4.1f,-0.10f);
			glVertex3f( 4.1f,-4.1f,-0.10f);
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_STENCIL_TEST); // ВЫключим тест буфера трафарета
		glPopAttrib();
		glColor4f(0.8f, 0.8f, 0.8f, 1.0f);
		glDepthMask(GL_TRUE); 
	}

	// отсечение CULLING по часовой стрелке GL_CW, против часовой стрелки GL_CCW
	static void ModeCull(GLenum cull = GL_CCW) {
		glEnable(GL_CULL_FACE);
		glFrontFace(cull);
	}
	static void EndCull(void) {
		glDisable(GL_CULL_FACE);
	}
};











#pragma pack(push, 1)

// структура заголовкa TGA-файла
typedef struct _TGAFILEHEADER {  
	BYTE id_length;  
	BYTE map_type;   
	BYTE img_type;
	WORD index;
	WORD length;
	BYTE co_size;
	WORD x_org;
	WORD y_org;
	WORD width;
	WORD height;
	BYTE pixel_size;
	BYTE bits;
} TGAFILEHEADER;

#pragma pack(pop)



// Класс текстура для загрузки растровых файлов(PNG - не поддерживается)
class  xTextura {
public:
	static GLuint  OpenImage(const TCHAR* filename, GLboolean mapping = GL_FALSE);
	static GLuint  OpenImageTGA(const TCHAR* filename);
	static IPicture*  PictureLoad(const TCHAR* filename);
	static void  Set(GLuint tid);
	static void  Enabled(GLboolean enabled);
	static void  Free(GLuint tid);
};






// Класс для создания фигур из стандартной библиотеки glu32.dll(сфера, диск, конус)
class xFigure {
private:
	GLUquadricObj* obj;
	GLuint         id;
public:
	xFigure(void);
	~xFigure();
public:
	void  CreateSphere(GLdouble radius, GLint slices, GLint stacks);
	void  CreateDisk(GLdouble radius1, GLdouble radius2, GLint slices, GLint loops);
	void  CreateCylinder(GLdouble radius1, GLdouble radius2, GLdouble height, GLint slices, GLint stacks);
	void  Display(void);
	void  Display(const LPGLVECTOR3& hpos);
	void  Display(const LPGLVECTOR3& hpos, const LPGLVECTOR3& hrot);
	void  Display(const LPGLVECTOR3& hpos, const LPGLVECTOR3 hrot, const LPGLVECTOR3& hscale);
	void  Destroy(void);

	static GLuint CreateCube(GLfloat width, GLfloat height, GLfloat depth, const GLVECTOR3& pos);
	static void   DestroyCube(GLuint id);
};







// Класс-объект камера, вращения исполняется при помощи кватернионов а направлениe движения
// с вычислением углов Эйлера. 
class  xCamera {
private:
	GLVECTOR3 camera_pos;
	GLVECTOR3 look;
	GLVECTOR3 streif;
	GLfloat   deg_pitch;
	GLfloat   deg_yaw;
	GLfloat   deg_roll;  
	GLfloat   lup;
	GLfloat   ldown;
	int       c_width;
	int       c_height;
	BOOL      uron;
	GLVECTOR3 vec_uron;
	BOOL      imenu;
public:
	void  Initialize(const GLVECTOR3& pos, GLfloat lock_up = -70.0f, GLfloat lock_down = 50.0f);
	void  UpdateMove(void);
	void  MouseMove(HWND hwnd, GLfloat felapsed);
	bool  KeyMove(GLfloat felapsed, GLfloat lock_cube);
	void  UpdateMove(GLfloat felapsed, GLfloat ftime);
	void  SetY(GLfloat h);
	void  StartUron(const LPGLVECTOR3& pvec, int type);

	void  CinematicMenu(BOOL enabled) { //автоматический камеро-обзор
		imenu      = enabled;
		camera_pos = GLVECTOR3(0.0f, 150.0f, 0.0f);
		deg_pitch  = 20.0f;
		deg_yaw    = 0.0f;
	}

	GLVECTOR3&  GetPos(void) {
		return camera_pos;
	}

	GLfloat  GetPitch(void) const {
		return deg_pitch;
	}

	GLfloat  GetYaw(void) const {
		return deg_yaw;
	}

	GLfloat  GetRoll(void) const {
		return deg_roll;
	}


	BOOL  IsUron(void) const {
		return uron;
	}


private:
	void  move_left_right(GLfloat velocity);
	void  move_back_front(GLfloat velocity);
};







///////////////////////////////////////////////////////////////////////////////////////////////////////


// класс шрифт
class  xFont {
public:


	// Вывод цифр треугольными полосками(TRIANGLE_STRIP) так как они самые быстрые
	static void  DisplayFontInt(GLuint tex, int num, GLint x, GLint y, GLint width, GLint height, const LPGLVECTOR2& size) {
		TCHAR  buf[32];
		int  dig, row, col;

		const GLfloat cw  = 1.0f / size->x;  // размер цифры по x
		const GLfloat ch  = 1.0f / size->y;	// размер цифры по y

		TCHAR* ptr  = buf;
		int len     = (int)wsprintf(buf, _T("%d"), num);
		GLfloat tx  = 0.0f;
		GLfloat ty  = 0.0f;

		xTextura::Set(tex);
		while(len-- > 0) {
		
			if((*ptr < _T('0')) && (*ptr > _T('9'))) {
				*ptr++;
				continue;
			}

			dig = int(*ptr - _T('0'));
			row = dig / 4;
			col = dig - (row * 4);
			tx  = GLfloat(col) * cw;
			ty  = GLfloat(2 - row) * ch;

			glBegin(GL_TRIANGLE_STRIP); 
				glTexCoord2f(tx + cw, ty + ch);
				glVertex2i(x + width, y); 
				glTexCoord2f(tx, ty + ch);
				glVertex2i(x, y);
				glTexCoord2f(tx + cw, ty);
				glVertex2i(x + width, y + height);
				glTexCoord2f(tx, ty);				
				glVertex2i(x, y + height);
			glEnd();

			x += width;
			*ptr++;
		}
	}


	// Вывод текста треугольными полосками(TRIANGLE_STRIP) так как они самые быстрые
	static void  DisplayFontText(GLuint tex, const TCHAR* str, GLint x, GLint y, GLint width, GLint height, const LPGLVECTOR2& size) {
		int  dig, row, col;

		const GLfloat cw  = 1.0f / size->x;  // размер буквы по x
		const GLfloat ch  = 1.0f / size->y;	// размер буквы по y

		GLfloat tx  = 0.0f;
		GLfloat ty  = 0.0f;

		xTextura::Set(tex);
		while(*str != _T('\0')) {
		
			if((*str < _T('А')) && (*str > _T('Я')) && (*str != _T(' '))) {
				*str++;
				continue;
			}

			dig = (*str != _T(' ')) ? int(*str - _T('А')) : 33;
			row = dig / 7;
			col = dig - (row * 7);
			tx  = GLfloat(col) * cw;
			ty  = GLfloat(4 - row) * ch;

			glBegin(GL_TRIANGLE_STRIP); 
				glTexCoord2f(tx + cw, ty + ch);
				glVertex2i(x + width, y); 
				glTexCoord2f(tx, ty + ch);
				glVertex2i(x, y);
				glTexCoord2f(tx + cw, ty);
				glVertex2i(x + width, y + height);
				glTexCoord2f(tx, ty);				
				glVertex2i(x, y + height);
			glEnd();

			x += width;
			*str++;
		}
	}
};




///////////////////////////////////////////////////////////////////////////////////////////////////


// класс по выводу 2D-примитивов
class xRect2D {
public:

	// Функция рисование 2D-прямоугольника c текстурой	
	static void  DrawRectTex(GLuint tex, GLint x, GLint y, GLint width, GLint height, GLfloat texel = 1.0f) {
		xTextura::Set(tex);
		glBegin(GL_TRIANGLE_STRIP); 
			glTexCoord2f(texel, 0.0f);
			glVertex2i(x + width, y); 
			glTexCoord2f(0.0f, 0.0f);
			glVertex2i(x, y);
			glTexCoord2f(texel, texel);
			glVertex2i(x + width, y + height);
			glTexCoord2f(0.0f, texel);			
			glVertex2i(x, y + height);
		glEnd();
	}


	// Функция рисование 2D-прямоугольник цветовую
	static void  DrawRect(const LPGLCOLOR& color, GLint x, GLint y, GLint width, GLint height) {
		glColor4fv((const GLfloat*)color);
		glBegin(GL_TRIANGLE_STRIP); 
			glVertex2i(x + width, y); 
			glVertex2i(x, y);
			glVertex2i(x + width, y + height);			
			glVertex2i(x, y + height);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}


	// Функция рисование 2D-прямоугольник цветовую
	static void  DrawRect(COLORREF color, GLint x, GLint y, GLint width, GLint height) {
		glColor4f(GLfloat(GetRValue(color)) / 255, GLfloat(GetGValue(color)) / 255, GLfloat(GetBValue(color)) / 255, 0.5f);
		glBegin(GL_TRIANGLE_STRIP); 
			glVertex2i(x + width, y); 
			glVertex2i(x, y);
			glVertex2i(x + width, y + height);			
			glVertex2i(x, y + height);
		glEnd();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
};




#endif