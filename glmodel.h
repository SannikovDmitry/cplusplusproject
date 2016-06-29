/*
  Автор(с): Санников, Кулебякин, Ступак и Пархоменко
  Название команды: AGRAGE
  e-mail:   sdk96@mail.ru
*/
#if ! defined(_GLMODEL_VILLAGE_)
#define _GLMODEL_VILLAGE_
#pragma once








// класс по выводу моделей непосредственно
class  xModel {
public:

	static  void  DrawHouse(const LPGLVECTOR3& pos, const LPGLVECTOR3& size, GLuint thigh, GLuint tlow) {
		GLfloat h = size->y + size->y;
	
		const GLfloat tarr_a[4][2] = {	{0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f} };
		const GLfloat tarr_b[4][2] = {  {0.0f, 0.0f}, {0.0f, 2.0f}, {2.0f, 0.0f}, {2.0f, 2.0f} };

		xTextura::Set(thigh);

		glBegin(GL_TRIANGLE_STRIP);	// крыша 

			glTexCoord2f(0.0f, 0.0f);// передния сторона
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_a[1]);
			glVertex3f(pos->x, pos->y + h, pos->z + size->z);

			glTexCoord2fv(tarr_a[2]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[0]);// левая сторона
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z); 

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x, pos->y + h, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x, pos->y + h, pos->z - size->z);

			glTexCoord2f(0.0f, 0.0f);// задняя сторона
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(pos->x , pos->y + h, pos->z - size->z); 

			glTexCoord2fv(tarr_a[1]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_a[2]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[0]);// правая сторона
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z); 

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x, pos->y + h, pos->z - size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x, pos->y + h, pos->z + size->z);

		glEnd();

		
		xTextura::Set(tlow);

		glBegin(GL_TRIANGLE_STRIP); // дом

			glTexCoord2fv(tarr_b[0]); // передния сторона
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);  

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[0]);// левая сторона
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z + size->z);  

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z);
			
			glTexCoord2fv(tarr_b[0]); // задняя сторона
			glNormal3f(0.0f, 0.0f, -1.0f);
			glVertex3f(pos->x - size->x, pos->y + size->y, pos->z - size->z); 

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x - size->x, pos->y - size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[0]);// правая сторона
			glNormal3f(1.0f, 0.0f, 0.0f);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z - size->z);  

			glTexCoord2fv(tarr_b[1]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z - size->z);

			glTexCoord2fv(tarr_b[2]);
			glVertex3f(pos->x + size->x, pos->y - size->y, pos->z + size->z);

			glTexCoord2fv(tarr_b[3]);
			glVertex3f(pos->x + size->x, pos->y + size->y, pos->z + size->z);

		glEnd();
	}



	/////////////////////////////////////////////////////////////////////////////////////


	static  void  ShadowHouse(const LPGLVECTOR3& pos, const LPGLVECTOR3& size, GLfloat* mat) {
			
		GLfloat h  = size->y + size->y;
	
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glMultMatrixf(mat);
	
		glBegin(GL_TRIANGLE_STRIP);	// крыша 
			glVertex3f(size->x, size->y, size->z);
			glVertex3f( 0.0f, h, size->z);
			glVertex3f(-size->x, size->y, size->z);
			glVertex3f(-size->x, size->y, size->z); 
			glVertex3f(-size->x, size->y, -size->z);
			glVertex3f( 0.0f, h, size->z);
			glVertex3f( 0.0f, h, -size->z);
			glVertex3f( 0.0f, h, -size->z); 
			glVertex3f(-size->x, size->y, -size->z);
			glVertex3f( size->x, size->y, -size->z);
			glVertex3f( size->x, size->y, -size->z); 
			glVertex3f(0.0f, h, -size->z);
			glVertex3f( size->x, size->y, size->z);
			glVertex3f( 0.0f, h, size->z);
		glEnd();
		glBegin(GL_TRIANGLE_STRIP); // дом
			glVertex3f(size->x, size->y, size->z);  
			glVertex3f(-size->x, size->y, size->z);
			glVertex3f(size->x, -size->y, size->z);
			glVertex3f(-size->x, -size->y, size->z);
			glVertex3f(-size->x, -size->y, size->z);  
			glVertex3f(-size->x,  size->y, size->z);
			glVertex3f(-size->x, -size->y, -size->z);
			glVertex3f(-size->x,  size->y, -size->z);
			glVertex3f(-size->x, size->y, -size->z); 
			glVertex3f(size->x,  size->y, -size->z);
			glVertex3f(-size->x, -size->y, -size->z);
			glVertex3f(size->x,  -size->y, -size->z);
			glVertex3f(size->x, -size->y, -size->z);  
			glVertex3f(size->x,  size->y, -size->z);
			glVertex3f(size->x, -size->y, size->z);
			glVertex3f(size->x,  size->y, size->z);
		glEnd();
		glPopMatrix();
	}


	// Функция рисование прямоугольника
	static void  DrawRect(const LPGLVECTOR3& pos, GLfloat size, GLfloat angleY) {
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP); 
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(size, size,   0.0f);  
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-size, size,  0.0f);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(size, -size,  0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-size, -size, 0.0f);
		glEnd();
		glPopMatrix();
	}



	// Метод анимирует спрайт
	static void  DrawAnimationSprite(const LPGLVECTOR3& pos, const LPGLVECTOR3& size, GLfloat rotY, const LPGLVECTOR2& cadr_pos, const LPGLVECTOR2& cadr_size) {
		GLfloat tarr[4][2] = {  
			{ cadr_pos->x, cadr_pos->y                               },
			{ cadr_pos->x, cadr_pos->y + cadr_size->y                },
			{ cadr_pos->x + cadr_size->x, cadr_pos->y                },
			{ cadr_pos->x + cadr_size->x, cadr_pos->y + cadr_size->y }
		};
	
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(rotY, 0.0f, 1.0f, 0.0f);
		glBegin(GL_TRIANGLE_STRIP); 
			glTexCoord2fv(tarr[0]);
			glVertex3f( size->x,   size->y, size->z);
			glTexCoord2fv(tarr[1]);
			glVertex3f(-size->x,   size->y, size->z);
			glTexCoord2fv(tarr[2]);
			glVertex3f( size->x,  -size->y, size->z);
			glTexCoord2fv(tarr[3]);
			glVertex3f(-size->x,  -size->y, size->z);
		glEnd();
		glPopMatrix();
	}

};





////////////////////////////////////////////////////////////////////////////////////////////////////




// Робот передвигающий по земле
class  xRobotTM {
private:
	xFigure circle;
	GLuint  id_cube;
	GLuint  tex_circle;
	GLuint  tex_head;
public:

	// создание
	void  Create(const TCHAR* texture_head, const TCHAR*  texture_circle) {
		id_cube  = xFigure::CreateCube(2.2f, 3.0f, 2.0f, GLVECTOR3(0.0f, 4.4f, 0.0f));
		circle.CreateSphere(1.4, 10, 10);
		tex_circle = xTextura::OpenImage(texture_circle);
		tex_head   = xTextura::OpenImage(texture_head, GL_TRUE);
	}

	// вывод на дисплей
	void  Display(const LPGLVECTOR3& pos, const LPGLVECTOR3& rot, GLfloat rot_circle) {
		glPushMatrix();
		glTranslatef(pos->x, pos->y + 2.0f, pos->z);
		glRotatef(rot->y, 0.0f, 1.0f, 0.0f); 
		glRotatef(rot->z, 0.0f, 0.0f, 1.0f);   

		xTextura::Set(tex_head);
		glCallList(id_cube);
		triangle();

		glRotatef(rot_circle, 1.0f, 0.0f, 0.0f);  // вращение колесом
		xTextura::Set(tex_circle);
		circle.Display();

		glPopMatrix();
	}


	// вывод на дисплей
	void  DisplayShadow(const LPGLVECTOR3& pos, const LPGLVECTOR3& rot, GLfloat* mat) {
		glPushMatrix();
		glMultMatrixf(mat);
		glTranslatef(pos->x, pos->y + 2.0f, pos->z);
		glRotatef(rot->y, 0.0f, 1.0f, 0.0f); 
		glCallList(id_cube);
		triangle(true);
		glPopMatrix();
	}


	// уничтожения
	void  Destroy(void) {
		xFigure::DestroyCube(id_cube);
		circle.Destroy();
		xTextura::Free(tex_head);
		xTextura::Free(tex_circle);
	}


	GLuint GetTex(void) {
		return tex_head;
	}

private:

	// Двойные треугольники для того чтобы не переключаться в режим отсечение по часовой стрелки GL_CW
	void  triangle(bool shadow = false) {

		if(! shadow) {
			xTextura::Enabled(GL_FALSE);
			glColor4f(0.5f, 0.5f, 0.5f, 1.0f); 
		}

		glBegin(GL_TRIANGLES);  // треугольники для рычагов
			glNormal3f(-1.0f, 0.0f,  0.0f);
			glVertex3f(-2.1f, 1.7f,  1.0f);
			glVertex3f(-2.1f, 1.7f, -1.0f);
			glVertex3f(-1.2f, 0.0f,  0.0f);
			glVertex3f(-2.1f, 1.7f, -1.0f);
			glVertex3f(-2.1f, 1.7f,  1.0f);
			glVertex3f(-1.2f, 0.0f,  0.0f);
			glNormal3f(1.0f,  0.0f,  0.0f);
			glVertex3f(2.1f,  1.7f,  1.0f);
			glVertex3f(2.1f,  1.7f, -1.0f);
			glVertex3f(1.2f,  0.0f,  0.0f);
			glVertex3f(2.1f,  1.7f, -1.0f);
			glVertex3f(2.1f,  1.7f,  1.0f);
			glVertex3f(1.2f,  0.0f,  0.0f);
		glEnd();
		glBegin(GL_TRIANGLES);  // верхнее крыло
			glColor4f(0.7f, 0.3f, 0.1f, 1.0f); 
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f(0.0f, 7.0f,  2.0f);
			glVertex3f(0.0f, 9.5f, -4.0f);
			glVertex3f(0.0f, 7.0f, -2.0f);
			glNormal3f(1.0f, 0.0f,  0.0f);
			glVertex3f(0.0f, 7.0f,  2.0f);
			glVertex3f(0.0f, 7.0f, -2.0f);
			glVertex3f(0.0f, 9.5f, -4.0f);
		glEnd();

		if(! shadow) {
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			xTextura::Enabled(GL_TRUE);
		}
	}

};




/////////////////////////////////////////////////////////////////////////////////////////////////////





// Летающий робот с крыльями как птица
class  xRobotFly {
private:
	xFigure  circle;
	GLuint   tex_circle;
public:

	// создание
	void  Create(const TCHAR*  texture_circle) {
		circle.CreateSphere(3.0, 14, 14);
		tex_circle = xTextura::OpenImage(texture_circle);
	}


	// вывод на дисплей
	void  Display(const LPGLVECTOR3& pos, GLfloat angleY, GLfloat fangle) {

		GLfloat isin = sinf(fangle);

		xTextura::Set(tex_circle);
		glPushMatrix();
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);

		circle.Display();

		glRotatef(30.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(false);

		glRotatef(-60.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(true);

		glPopMatrix();
	}


	// вывод на дисплей
	void  DisplayShadow(const LPGLVECTOR3& pos, GLfloat angleY, GLfloat fangle, GLfloat* mat) {
		GLfloat isin = sinf(fangle);
		glPushMatrix();
		glMultMatrixf(mat);
		glTranslatef(pos->x, pos->y, pos->z);
		glRotatef(angleY, 0.0f, 1.0f, 0.0f);
		circle.Display();

		glRotatef(30.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(false, true);

		glRotatef(-60.0f * isin, 0.0f, 0.0f, 1.0f);
		triangle(true, true);

		glPopMatrix();
	}


	// уничтожение
	void  Destroy(void) {
		circle.Destroy();
		xTextura::Free(tex_circle);
	}

private:

	// крылья "советов" двойное рисование крыльев чтобы лишний раз не вызывать отсечение обратных граней
	void  triangle(bool left, bool shadow = false) {

		if(! shadow) {
			glColor4f(0.9f, 0.9f, 0.0f, 1.0f);
			xTextura::Enabled(GL_FALSE);
		}

		if(! left) {
			glBegin(GL_TRIANGLE_STRIP);  // правое крыло
				glNormal3f(0.0f, -1.0f,  0.0f);
				glVertex3f(2.8f, 0.5f,  1.5f);
				glVertex3f(2.8f, 0.5f, -1.5f);
				glVertex3f(9.5f, 0.5f,  1.5f);
				glVertex3f(9.5f, 0.5f, -1.5f);
				glNormal3f(0.0f, 1.0f,  0.0f);
				glVertex3f(9.5f, 0.5f, -1.5f);
				glVertex3f(2.8f, 0.5f, -1.5f);
				glVertex3f(9.5f, 0.5f,  1.5f);
				glVertex3f(2.8f, 0.5f,  1.5f);
			glEnd();
		} else {
			glBegin(GL_TRIANGLE_STRIP);   // левое крыло
				glNormal3f(0.0f, 1.0f,  0.0f);
				glVertex3f(-2.8f, 0.5f,  1.5f);
				glVertex3f(-2.8f, 0.5f, -1.5f);
				glVertex3f(-9.5f, 0.5f,  1.5f);
				glVertex3f(-9.5f, 0.5f, -1.5f);
				glNormal3f(0.0f, -1.0f,  0.0f);
				glVertex3f(-9.5f, 0.5f, -1.5f);
				glVertex3f(-2.8f, 0.5f, -1.5f);
				glVertex3f(-9.5f, 0.5f,  1.5f);
				glVertex3f(-2.8f, 0.5f,  1.5f);
			glEnd();
		}

		if(! shadow) {
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			xTextura::Enabled(GL_TRUE);
		}
	}

};







#endif