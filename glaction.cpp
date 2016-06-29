/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"
const int sRobotTM::MAX_ROBOTS_TM   = 2;
const int sRobotFly::MAX_ROBOTS_FLY = 2;







xUser::xUser(void) {
	snd_uron  = NULL;
	snd_fire  = NULL;
	gun       = NULL;
	tex_gun   = 0u;
	tex_lazer = 0u;
	pos       = 0.0f;
	size      = 0.0f;
	ifire     = FALSE;
	imove     = FALSE;
	life      = 0;
}



xUser::xUser(LPDIRECTSOUND sound) {
	snd_uron  = NULL;
	snd_fire  = NULL;
	snd_step  = NULL;
	gun       = NULL;
	tex_gun   = 0u;
	tex_lazer = 0u;
	pos       = 0.0f;
	size      = 0.0f;
	ifire     = FALSE;
	imove     = FALSE;
	life      = 0;
	this->Create(sound);
}



xUser::~xUser() {
	this->Destroy();
}






void  xUser::Create(LPDIRECTSOUND sound) {
	gun = new xFigure();
	gun->CreateCylinder(0.5, 0.5, 7.0, 18, 18);

	//загружаем картинки лазера
	tex_gun   = xTextura::OpenImage(_T("image\\gun.jpg"));
	tex_lazer = xTextura::OpenImage(_T("image\\lazer.jpg"));

	//загружаем звуки выстрелов, лазера и шагов
	snd_uron = new xSound(sound, _T("sound\\user_uron.wav")); 
	snd_fire = new xSound(sound, _T("sound\\user_lazer.wav")); 
	snd_step = new xSound(sound, _T("sound\\user_step.wav")); 
}






//инициализируем юзера
void  xUser::Initialize(void) {
	pos    = GLVECTOR3(0.5f, -1.9f, -1.9f);
	size   = GLVECTOR3(9.5f, 5.0f, 9.5f);
	ifire  = FALSE;
	imove  = FALSE;
	life   = 30;
}






// вывод пользователя только один бластер на этом хватит
void  xUser::Display(GLfloat felapsed, GLfloat ftime) {

	static GLfloat angle = 0.0f;
	static GLfloat fstep = 0.0f;

	xTextura::Set(tex_gun);

	glLoadIdentity();
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(4.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(-40.0f, 0.0f, 0.0f, 1.0f);
	gun->Display();

	if(ifire) { // обработка стрельбы
		angle += fNUM(felapsed, 8.0f);
		pos.z  = -1.9f + 1.5f * sinf(angle);
		if(angle > GL_PI) {
			angle = 0.0f;
			pos.z = -1.9f;
			ifire = FALSE;
		}
	}


	if(imove) { // обработка ходьбы
		pos.y = -1.9f - 0.3f * sinf(fstep);
		fstep += fNUM(felapsed, 4.0f);
	} else
		fstep  = 0.0f;



}





// вывод плазменных пуль
void  xUser::DisplayLazer(GLfloat felapsed, GLfloat ftime, GLfloat angleY) {
	
	xTextura::Set(tex_lazer);

	GLfloat num = fNUM(felapsed, 150.0f);
	for(std::list<sBullet>::iterator iter = bullets.begin(); iter != bullets.end(); ++iter) {
		xModel::DrawRect(&iter->pos, 1.0f, angleY);
		iter->pos -= iter->vec * num;
	}
}






// вывод цели - крестика
void  xUser::DisplayCross(const LPSIZE pscreen) {
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, 0.0f);

	const GLint x = (GLint)pscreen->cx / 2L; 
	const GLint y = (GLint)pscreen->cy / 2L;

	xTextura::Enabled(GL_FALSE);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex2i(x - 7, y);
		glVertex2i(x + 7, y);
		glVertex2i(x, y - 7);
		glVertex2i(x, y + 7);
	glEnd();
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	xTextura::Enabled(GL_TRUE);
}




// метод выпускает плазменный пучок из бластера	
void  xUser::OnFire(WPARAM wParam, const LPGLVECTOR3& hpos, GLfloat felapsed, GLfloat yaw, GLfloat pitch) {
	if(wParam & MK_LBUTTON) {

		ifire = TRUE;

		sBullet  bullet;
		bullet.pos.x =  hpos->x;
		bullet.pos.y =  hpos->y - 1.2f;
		bullet.pos.z = -hpos->z;
		VectorAngle(&bullet.vec, -yaw, pitch);	
		bullets.push_back(bullet);

		snd_fire->Play();
	}
}






// сигнал о перемещение пользователя
void  xUser::SetMove(BOOL move) {
	if(move)
		snd_step->Play(DSBPLAY_LOOPING);
	else
		snd_step->Stop();
	
	imove = move;
	if(! move)
		pos.y = -1.9f;
}




//деструктор юзера
void  xUser::Destroy(void) {
	DELETE_HEAP(snd_step);
	DELETE_HEAP(snd_uron);
	DELETE_HEAP(snd_fire);
	DELETE_HEAP(gun);
	xTextura::Free(tex_gun);
	xTextura::Free(tex_lazer);
	tex_gun = tex_lazer = 0u;

	if(bullets.size() > 0u)
		bullets.clear();
}



// убавить жизнь
void  xUser::DecLife(void) {
	life--;
	snd_uron->Play();
}





////////////////////////////////////////////////////////////////////////////////////////////////////




//класс land-полигон
xTerrain::xTerrain(void) {
	fmap       = NULL;
	vertices   = NULL;
	normals    = NULL;
	texs       = NULL;
	vsize      = 0;
	vstep      = 0;
	vstep      = 0;
	map_size   = 0;
	half_size  = 0;
	tex_crush  = 0u;
	tex_smoke  = 0u;
	tex        = 0u;
	top_floor  = 0.6f;
	vidlist    = 0u;
	size_house = 0;
	map_rect   = 0;
	snd_crush  = NULL;
	snd_uron   = NULL;
	ZeroMemory(tex_house, sizeof(tex_house));
	ZeroMemory(snd_move, sizeof(snd_move));
}



//деструтор land-полигона
xTerrain::~xTerrain() {
	this->Destroy();
}






	
// Метод создаёт land-полигон 
bool  xTerrain::Create(LPDIRECTSOUND sound, GLint field_size, GLint step) {

	const GLint vcount = (field_size / step) * (field_size / step);
	const GLfloat offx = GLfloat(-field_size) / 2.0f;
	const GLfloat offz = GLfloat(-field_size) / 2.0f;

	vstep     = step;
	map_size  = field_size;
	half_size = field_size / 2;
	vsize     = 12 * vcount;
	tsize     =  8 * vcount;

	vertices  = new GLfloat[vsize];
	if(vertices == NULL)
		return false;

	texs = new GLfloat[tsize];
	if(texs == NULL) 
		return false;

	normals = new GLfloat[vsize];
	if(normals == NULL) 
		return false;

	GLfloat*  vptr = vertices;
	GLfloat*  tptr = texs;
	GLfloat*  nptr = normals;

	for(GLint x = 0; x < (field_size - step); x += step) {
		for(GLint z = 0; z < (field_size - step); z += step) {

			///////////////////// ВЕРШИНЫ///////////////////////////////////
			*vptr++ = offx + GLfloat(x);
			*vptr++ = 0.0f;
			*vptr++ = offz + GLfloat(z);  

			*vptr++ = offx + GLfloat(x);
			*vptr++ = 0.0f;
			*vptr++ = offz + GLfloat(z + step);

			*vptr++ = offx + GLfloat(x + step);
			*vptr++ = 0.0f;
			*vptr++ = offz + GLfloat(z + step);

			*vptr++ = offx + GLfloat(x + step);
			*vptr++ = 0.0f;
			*vptr++ = offz + GLfloat(z);

			////////////////////////////НОРМАЛИ/////////////////////////////
			for(GLint k = 0; k < 4; k++) {
				*nptr++ = 0.0f;
				*nptr++ = 1.0f;
				*nptr++ = 0.0f;
			}
		}
	}

	vsize /= 3;

	trees.resize(880u);  // чтобы не париться с реаллокациями выделить заранее память из кучи

	vidlist = glGenLists(4);
	this->compile_tree(vidlist, 0,      5.4f, 11.4f);
	this->compile_tree(vidlist + 1, 1,  6.4f, 15.4f);
	this->compile_tree(vidlist + 2, 2,  4.8f, 12.4f);
	this->compile_tree(vidlist + 3, 3,  2.0f, 4.1f);

	house_size = GLVECTOR3(6.5f, 5.5f, 7.5f);

	robot_tm.Create(_T("image\\skull.jpg"), _T("image\\circle.jpg"));
	robot_fly.Create(_T("image\\skull2.jpg"));
	tex_smoke  = xTextura::OpenImage(_T("image\\uron.jpg"));
	tex_crush  = xTextura::OpenImageTGA(_T("image\\crush.tga"));

	snd_uron    = new xSound(sound, _T("sound\\robot_uron.wav")); 
	snd_crush   = new xSound(sound, _T("sound\\robot_crush.wav"));
	snd_move[0] = new xSound(sound, _T("sound\\robot_move.wav"));
	snd_move[1] = new xSound(sound, snd_move[0]);

	robots_tm[0].snd_uron  = snd_uron;
	robots_tm[1].snd_crush = snd_crush;
	robots_tm[0].snd_move  = snd_move[0];
	robots_tm[1].snd_move  = snd_move[1];

	return true;
}





// Метод инициализирует ландшафт
void   xTerrain::Initialize(const LPGLVECTOR3& user_pos) {
	robots_fly[0].Initialize(user_pos, 0.0f, -CubeSize() - 10.0f);
	robots_fly[1].Initialize(user_pos, 0.0f,  CubeSize() + 10.0f);

	robots_tm[0].Initialize(user_pos, CubeSize() - 10.0f, 0.0f);
	robots_tm[1].Initialize(user_pos, -CubeSize() + 10.0f, 0.0f);
}







// Метод для вычисления падающей тени на плоскость
void  xTerrain::SetLightPos(const LPGLVECTOR4& light) {
	GLfloat y = max(houses[0].y, max(houses[1].y, houses[2].y)) - house_size.y * 2.2f;
	GLVECTOR3 dots[3] = {
		GLVECTOR3(CubeSize(),  y, -CubeSize()), 
		GLVECTOR3(-CubeSize(), y, -CubeSize()),
		GLVECTOR3(-CubeSize(), y,  CubeSize())
	};
	GLPLANE plane;
	PlaneEquation(&plane, dots);
	MatrixShadow(house_shadow, &plane, light);
}






// Метод для вычисления падающей тени на плоскость для сторонних объектов игры
GLfloat  xTerrain::GetPlaneShadow(GLfloat* mat, LPGLVECTOR3 pos, const LPGLVECTOR4& light) {
	GLfloat h = this->GetOffsetTop(pos->x, pos->z);
	
	GLVECTOR3 dots[3] = {
		GLVECTOR3(CubeSize(),  h, -CubeSize()), 
		GLVECTOR3(-CubeSize(), h, -CubeSize()),
		GLVECTOR3(-CubeSize(), h,  CubeSize())
	};

	GLPLANE plane;
	PlaneEquation(&plane, dots);
	MatrixShadow(mat, &plane, light);
	return h;
}








// Компиляция списка(дерево) запишем в видеопамять
void  xTerrain::compile_tree(GLuint lid, GLuint tid, GLfloat x, GLfloat y) {
	glNewList(lid, GL_COMPILE);
		xTextura::Set(tex_tree[tid]);
		glBegin(GL_TRIANGLE_STRIP); 
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-x,  y, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-x, 0.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(x,  y,  0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(x, 0.0f, 0.0f);
		glEnd();
	glEndList();
}





// Метод выводит ландшафт
void  xTerrain::Display(void) {

	xTextura::Set(tex);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, texs);
	glNormalPointer(GL_FLOAT, 0, normals);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawArrays(GL_QUADS, 0, vsize);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}




// 0  1  2   3    11 12 13 14
// 4  5  6   7    21 22 23 24
// 8  9  10  11	  31 32 33 34
// 12 13 14  15   41 42 43 44
// Вывод растительности
void  xTerrain::DisplayVegetations(const LPGLVECTOR3& user_pos, GLfloat angleY, GLfloat ftime, GLfloat felapsed) {
	GLfloat mat[16];

	MatrixIdentity(mat);
	MatrixRotateY(mat, angleY); // преобразование поворота в область просмотра камеры выполняется один раз для всех биллбордов-деревьев
	
	for(std::vector<sTree>::const_iterator citer = trees.begin(); citer != trees.end(); ++citer) {
		
		// трансляция для каждого дерева
		mat[12] = citer->x;
		mat[13] = citer->y;
		mat[14] = citer->z;	

		glPushMatrix();
		glMultMatrixf(mat);
		glCallList(citer->type);
		glPopMatrix();
	}


	// Вывод взрыва летающего робота
	for(sRobotFly* pfly = robots_fly; pfly != robots_fly + sRobotFly::MAX_ROBOTS_FLY; *pfly++) {
		if(pfly->IsCrush())
			pfly->DisplayEffect(user_pos, tex_crush, angleY, felapsed, ftime);
	}


	// Вывод взрыва TM-робота
	for(sRobotTM* ptm = robots_tm; ptm != robots_tm + sRobotTM::MAX_ROBOTS_TM; *ptm++) {
		if(ptm->IsCrush())
			ptm->DisplayEffect(user_pos, tex_crush, angleY, felapsed, ftime);
	}
}
	




// вывод домиков
void  xTerrain::DisplayObjects(void) {
	for(LPGLVECTOR3 pvec = houses; pvec != houses + size_house; *pvec++)
		xModel::DrawHouse(pvec, &house_size,  tex_house[1], tex_house[0]);
}

// вывод теней от домиков
void  xTerrain::DisplayShadow(void) {
	for(LPGLVECTOR3 pvec = houses; pvec != houses + size_house; *pvec++)
		xModel::ShadowHouse(pvec, &house_size, house_shadow);	
}







/////**********************************************************************************************
// Метод выводит в сцену роботов


void  xTerrain::DisplayRobots(const LPGLVECTOR3& user_pos, GLfloat felapsed, GLfloat ftime) {
	GLVECTOR3 scube = house_size;
	GLfloat h;

	scube   += 0.1f;
	scube.y += 3.0f;

	// вывод летающих роботов
	for(sRobotFly* pfly = robots_fly; pfly != robots_fly + sRobotFly::MAX_ROBOTS_FLY; *pfly++) {
		pfly->UpdateMove(user_pos, CubeSize(), felapsed, ftime);

		if(pfly->IsTaskPush()) { // если задание настигнуть пользователя нужно чтобы робот не провалился в ландшафт
			h = this->GetOffsetTop(pfly->pos.x, pfly->pos.z);
			if(h < (pfly->pos.y + 8.0f)) {
				pfly->pos.y *= 0.9f;
				pfly->pos.y += (1.0f - 0.9f) * h + 1.0f;
			}

			// столкновения с домиками
			for(LPGLVECTOR3 hptr = houses; hptr != houses + MAX_HOUSES; ++hptr) 
				IsRectIntersect(&pfly->pos, &GLVECTOR3(9.0f), hptr, &scube);
		}

		if(! pfly->IsCrush())
			robot_fly.Display(&pfly->pos, pfly->rotY, pfly->fangle);
	}


	// вывод TM-роботов
	for(sRobotTM* ptm = robots_tm; ptm != robots_tm + sRobotTM::MAX_ROBOTS_TM; *ptm++) {
		ptm->UpdateMove(user_pos, CubeSize(), felapsed, ftime);

		h = this->GetOffsetTop(ptm->pos.x, ptm->pos.z);
		ptm->pos.y *= 0.9f;
		ptm->pos.y += (1.0f - 0.9f) * h;
		
		// столкновения с домиками
		for(LPGLVECTOR3 hptr = houses; hptr != houses + MAX_HOUSES; ++hptr) {
			if(IsRectIntersect(&ptm->pos, &GLVECTOR3(9.0f), hptr, &scube))
				ptm->ResetTask();
		}
	
		if(! ptm->IsCrush())
			robot_tm.Display(&ptm->pos, &ptm->rot, ptm->angle);
	}
}



		

// Метод выводит падающие тени от роботoв(не стал расчитывать каждую вершину и смежные вершины, нормаль)
// корявые тени
void  xTerrain::DisplayShadowRobots(const LPGLVECTOR4& light_pos) {
	GLfloat mat[16];
	GLVECTOR3 pos;

	// вывод теней летающих роботов
	for(sRobotFly* pfly = robots_fly; pfly != robots_fly + sRobotFly::MAX_ROBOTS_FLY; *pfly++) {
		if(pfly->IsTaskPush() && (! pfly->IsCrush())) {
			pos   = pfly->pos;
			pos.y = this->GetPlaneShadow(mat, &pos, light_pos) + 5.0f;
			robot_fly.DisplayShadow(&pos, pfly->rotY, pfly->fangle, mat);
		}
	}

	// вывод теней TM-роботов
	for(sRobotTM* ptm = robots_tm; ptm != robots_tm + sRobotTM::MAX_ROBOTS_TM; *ptm++) {
		if(! ptm->IsCrush()) {
			pos   = ptm->pos;
			pos.y = this->GetPlaneShadow(mat, &pos, light_pos) + 5.0f;
			robot_tm.DisplayShadow(&pos, &ptm->rot, mat);
		}
	}
}





// Метод выводит эффекты для роботов
void    xTerrain::DisplayEffectRobots(const LPGLVECTOR3& user_pos, GLfloat angleY, GLfloat felapsed, GLfloat ftime) {
	// вывод эффектов летающих роботов
	for(sRobotFly* pfly = robots_fly; pfly != robots_fly + sRobotFly::MAX_ROBOTS_FLY; *pfly++) {
		if(pfly->IsUron())
			pfly->DisplayEffect(user_pos, tex_smoke, angleY, felapsed, ftime);
	}

	// вывод эффектов роботов-TM
	for(sRobotTM* ptm = robots_tm; ptm  != robots_tm + sRobotTM::MAX_ROBOTS_TM; *ptm++) {
		if(ptm->IsUron())
			ptm->DisplayEffect(user_pos, tex_smoke, angleY, felapsed, ftime);
	}
}


/////**********************************************************************************************





// Установка положения ландшафта
void  xTerrain::BeginMatrix(const LPGLVECTOR3& hpos) {
	glPushMatrix();
	glTranslatef(hpos->x, hpos->y, hpos->z);
}




void  xTerrain::EndMatrix(void) {
	glPopMatrix();
}





// Получения ячейки для координаты Y по указанной строке и столбцу X-Z
inline  GLfloat xTerrain::Offset(GLint x, GLint y) {
	x = min(x, map_size - 1);
	y = min(y, map_size - 1);
	
	GLint index = (y * map_size) + x;

	// обезопасить выход индекса за массив, это страшная ошибка
	if((index >= map_rect) || (index < 0)) 
		return 0.0f;
	return fmap[index];
}
	




// Тоже самое как и предыдущий метод
inline  GLfloat xTerrain::GetOffsetTop(GLfloat x, GLfloat z) {
	GLint ix = GLint(x);
	GLint iz = GLint(z);
	return this->Offset(ABS(half_size + ix), ABS(half_size + iz));
}


	




// Метод расчитывает нормаль для ландшафта(карты высот)
void  xTerrain::Normalize(void) {
	
	GLfloat*  vptr = vertices;
	GLfloat*  nptr = normals;

	if((nptr == NULL) || (nptr == NULL))
		return;

	GLVECTOR3 vec[3];
	GLVECTOR3 nor;

	for(GLint x = 0; x < (map_size - vstep); x += vstep) {
		for(GLint z = 0; z < (map_size - vstep); z += vstep) {

			vec[0].x = *vptr++;
			vec[0].y = *vptr++;
			vec[0].z = *vptr++;

			vec[1].x = *vptr++;
			vec[1].y = *vptr++;
			vec[1].z = *vptr++;

			vec[2].x = *vptr++;
			vec[2].y = *vptr++;
			vec[2].z = *vptr++;

			VectorNormal(&nor, &vec[0], &vec[1], &vec[2]);
			*nptr++ = nor.x;
			*nptr++ = nor.y;
			*nptr++ = nor.z;

			*nptr++ = nor.x;
			*nptr++ = nor.y;
			*nptr++ = nor.z;
		
			vec[0].x = *vptr++;
			vec[0].y = *vptr++;
			vec[0].z = *vptr++;

			VectorNormal(&nor, &vec[0], &vec[1], &vec[2]);
			*nptr++ = nor.x;
			*nptr++ = nor.y;
			*nptr++ = nor.z;
	
			*nptr++ = nor.x;
			*nptr++ = nor.y;
			*nptr++ = nor.z;
		}
	}

}





// Загрузка текстуры для карты высот
void   xTerrain::LoadTextura(const TCHAR* filename) {
	ZeroMemory(tex_tree, sizeof(tex_tree));

	tex          = xTextura::OpenImage(filename);

	tex_tree[0]  = xTextura::OpenImageTGA(_T("image\\tree1.tga"));
	tex_tree[1]  = xTextura::OpenImageTGA(_T("image\\tree2.tga"));
	tex_tree[2]  = xTextura::OpenImageTGA(_T("image\\tree3.tga"));
	tex_tree[3]  = xTextura::OpenImageTGA(_T("image\\bush.tga"));

	tex_house[0] = xTextura::OpenImage(_T("image\\house1.jpg"), GL_TRUE);
	tex_house[1] = xTextura::OpenImage(_T("image\\house2.jpg"), GL_TRUE);
}





// Метод по загрузке карты высот формата файла RAW
bool  xTerrain::LoadTerrain(const TCHAR* raw_filename, GLfloat scale) {

	HANDLE fp = CreateFile(raw_filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fp == INVALID_HANDLE_VALUE)
		return false;

	DELETE_HEAP_ARRAY(fmap);

	map_rect = map_size * map_size; 
	fmap     = new GLfloat[map_rect];
	if(fmap == NULL) {
		CloseHandle(fp);
		return false;
	}

	
	GLfloat* ptr = fmap;
	DWORD  dread = 0uL;
	BYTE   ph    = 0u;
	BOOL   ieof;
	for(GLint row = 0; row < map_size; row++) {
		for(GLint col = 0; col < map_size; *ptr++ = GLfloat(ph) * scale, col++) {
			ieof = ReadFile(fp, (LPVOID)&ph, 1uL, &dread, NULL);
			if(ieof && dread == 0uL)
				break;
		}
	}
	CloseHandle(fp);

	GLfloat* tptr = texs;
	GLfloat* vptr = vertices;
	for(GLint x = 0; x < (map_size - vstep); x += vstep) {
		for(GLint z = 0; z < (map_size - vstep); z += vstep) {
			*vptr++;
			*vptr++ = Offset(x, z);
			*vptr++;

			*vptr++;
			*vptr++ = Offset(x, z + vstep);
			*vptr++;

			*vptr++;
			*vptr++ = Offset(x + vstep, z + vstep);
			*vptr++;

			*vptr++;
			*vptr++ = Offset(x + vstep, z);
			*vptr++;
		}
	}
	this->Normalize();
	this->ObjectsToField();
	return true;
}






//    0 ... 1024
// -512  |  512
// Метод вычисляет положение камеры и устанавливает камеру по оси-Y на позицию поверхности карты высот
void   xTerrain::OffsetCameraY(LPGLVECTOR3 pos) {

	GLint x = GLint(pos->x);
	GLint z = GLint(-pos->z);

	x = ABS(half_size + x);
	z = ABS(half_size + z);

	pos->y *= 0.9f;
	pos->y += Offset(x, z) * (1.0f - 0.9f) + top_floor;
	
}



	


void   xTerrain::RectOffset(GLint x, GLint z, GLfloat* arr) {
	arr[0] = Offset(x, z);
	arr[1] = Offset(x, z + vstep);
	arr[2] = Offset(x + vstep, z + vstep);
	arr[3] = Offset(x + vstep, z);
}







void   xTerrain::ObjectsToField(void) {
	const GLfloat tcoord[] = {  // текстурные координаты для натягивания на карту высот
		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.5f, 0.5f,  
		0.5f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 1.0f, 0.5f,
		0.5f, 0.5f, 1.0f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.5f, 1.0f
	};

	std::vector<sTree>::iterator iter = trees.begin();

	GLfloat off[4];
	GLfloat tmp  = 0.0f;
	GLfloat pos  = 0.0f;
	GLfloat hmax = 0.0f;
	ZeroMemory(off, sizeof(off));

	GLfloat* tptr = texs;
	GLfloat* vptr = vertices;
	for(GLint x = 0; x < (map_size - vstep); x += vstep) {
		for(GLint z = 0; z < (map_size - vstep); z += vstep) {

			this->RectOffset(x, z, off);
			
			// натяжка текстуры
			if(off[0] > 8) 
				CopyMemory(tptr, tcoord, sizeof(GLfloat) * 8);
			else if(off[0] > 3) 
				CopyMemory(tptr, tcoord + 8 * (1 + (rand() % 2)) , sizeof(GLfloat) * 8);
			else 
				CopyMemory(tptr, tcoord + 24, sizeof(GLfloat) * 8);
			tptr += 8;


			// расставляем деревья только на холмы
			if((off[0] > 49) && (fabsf(vptr[2] - tmp) > 30.0f)) {  // 
				GLuint id = GLuint(rand() % 3);
				if(iter != trees.end()) 
					*iter++ = sTree(vidlist + id, vptr[0], off[0], vptr[2]);
				tmp = vptr[2];
			} 


			// раставляем домики
			if((off[0] < 0.5f) && (size_house < MAX_HOUSES) && (fabsf(vptr[0] - pos) > 60.0f && x > 50)) {

				hmax = max(hmax, max(off[3], max(off[0], max(off[1], off[2]))) + 5.8f);
				houses[size_house++] = GLVECTOR3(vptr[0], hmax, vptr[2]);
				pos  = vptr[0];

			} else if(off[0] < 1.0f){ // раставляем кусты
				
				if(iter != trees.end()) 
					*iter++ = sTree(vidlist + 3, vptr[0] - fRAND(2, 10), off[0] + 0.7f, vptr[2] + fRAND(2, 10));
			}

			vptr += 12;
		}
	}	
}





// Метод устанавливает высоту камеры над землёй
void  xTerrain::SetTopFloor(GLfloat top) {
	top_floor = top;
}





// Метод удаляет карту высот из памяти и вершины
void  xTerrain::Destroy(void) {

	for(int k = 0; k < 2; k++) {
		if(snd_move[k] != NULL)
			delete snd_move[k];
		snd_move[k] = NULL;
	}

	DELETE_HEAP(snd_crush);
	DELETE_HEAP(snd_uron);
	robot_fly.Destroy();
	robot_tm.Destroy();
	xTextura::Free(tex_crush);
	xTextura::Free(tex_smoke);
	xTextura::Free(tex);
	DELETE_HEAP_ARRAY(vertices);
	DELETE_HEAP_ARRAY(normals);
	DELETE_HEAP_ARRAY(texs);
	DELETE_HEAP_ARRAY(fmap);
	map_size  = 0;
	vsize     = 0;
	tsize     = 0;
	vstep     = 0;
	tex       = 0u;

	for(int i = 0; i < (sizeof(tex_tree)/sizeof(tex_tree[0])); i++) 
		xTextura::Free(tex_tree[i]);
	ZeroMemory(tex_tree, sizeof(tex_tree));

	if(! trees.empty())
		trees.clear();

	xTextura::Free(tex_house[0]);
	xTextura::Free(tex_house[1]);
	glDeleteLists(vidlist, 4);
	vidlist = 0u;
}






// Метод про проверке пуль на столкновения с ландшафтными объектами
void  xTerrain::CollisionBullet(const LPGLVECTOR3& user_pos, std::list<sBullet>* plist) {
	int ret;
	GLVECTOR3 scube = house_size;
	GLfloat  cube   = this->CubeSize();
	GLVECTOR3 tmp;

	scube   += 0.1f;
	scube.y += 3.0f;

	// цикл по всем пулям
	for(std::list<sBullet>::iterator iter = plist->begin(); iter != plist->end(); ++iter) {

		// предыдущия позиция пули
		tmp = iter->pos + iter->vec;

		// столкновения пуль с домиками
		for(LPGLVECTOR3 hptr = houses; hptr != houses + MAX_HOUSES; ++hptr) {
			if(IsCollisionPointRect(&tmp, hptr, &scube)) {
				iter = plist->erase(iter);
			//	--iter;
				break;
			}
		}
		
		// проверка итератора на валидность и выхода за узел
		if(iter == plist->end())
			break;
		tmp = iter->pos + iter->vec;


		// столкновения пуль c летающимu роботам
		for(sRobotFly* pfly = robots_fly; pfly != robots_fly + sRobotFly::MAX_ROBOTS_FLY; ++pfly) {
			if((ret = pfly->IsCollision(&tmp, 1.0f)) != 0) {
				iter = plist->erase(iter);
				//--iter;
				this->play_sound(&pfly->pos, user_pos, ret);
				break;
			}
		}
		if(iter == plist->end())
			break;
		tmp = iter->pos + iter->vec;


		// столкновения пуль c TM-роботам
		for(sRobotTM* ptm = robots_tm; ptm != robots_tm + sRobotTM::MAX_ROBOTS_TM; ++ptm) {
			if((ret = ptm->IsCollision(&tmp, 1.0f)) != 0) {
				iter = plist->erase(iter);
				//--iter;
				this->play_sound(&ptm->pos, user_pos, ret);
				break;
			}
		}
		if(iter == plist->end())
			break;
		tmp = iter->pos + iter->vec;


		// удаление пуль за вылет ландшафта
		/*if((iter->pos.y < this->GetOffsetTop(iter->pos.x, iter->pos.z)) || (iter->pos.x > cube) || (iter->pos.x < -cube) || (iter->pos.z > cube) || (iter->pos.z < -cube)) {
			if (iter != plist->end()){
				iter = plist->erase(iter);
				--iter;
			}
		}
		*/
		
	}

}





	
// Метод по проверке столкновения с пользователем(камерой) домики, враги-роботы
bool  xTerrain::CollisionUser(xCamera* camera, const LPGLVECTOR3& user_size) {
	GLVECTOR3 cpos  = camera->GetPos();
	GLVECTOR3 scube = house_size + 0.8f;

	cpos.z  = -cpos.z;

	// столкновения пользователя с домиками
	for(LPGLVECTOR3 hptr = houses; hptr != houses + MAX_HOUSES; ++hptr) {
		if(IsRectIntersect(&cpos, user_size, hptr, &scube)) {
			cpos.z           = -cpos.z;
			camera->GetPos() = cpos;
			break;
		}
	}


	// столкновения пользователя c летающимu роботам
	for(sRobotFly* pfly = robots_fly; pfly != robots_fly + sRobotFly::MAX_ROBOTS_FLY; ++pfly) {
		if(pfly->IsCollision(&cpos, user_size->x)) {
			camera->StartUron(&pfly->vec, 1);
			return true;
		}
	}


	// столкновения пользователя c TM-роботам
	for(sRobotTM* ptm = robots_tm; ptm != robots_tm + sRobotTM::MAX_ROBOTS_TM; ++ptm) {
		if(ptm->IsCollision(&cpos, user_size->x)) {
			camera->StartUron(&ptm->vec, 2);
			return true;
		}
	}

	return  false;
}



// обработка звуковых данных
void  xTerrain::play_sound(const LPGLVECTOR3& va, const LPGLVECTOR3& vb, int code) {
	GLfloat len = VectorLength(&(*vb - *va));
	len         = ABS(len);

	if(code & 1) { // урон по роботу
		snd_uron->SetVolume(max(DSBVOLUME_MAX - len * 6L, DSBVOLUME_MIN));
		snd_uron->Play();
	} else { // обработка взрыва
		snd_crush->Stop();
		snd_crush->SetVolume(max(DSBVOLUME_MAX - len * 6L, DSBVOLUME_MIN));
		snd_crush->Play();	
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////





xSkyBox::xSkyBox(void) {
	ZeroMemory(texs, sizeof(texs));
	width  = 0.0f;
	height = 0.0f;
	depth  = 0.0f;
	listid = 0u;
}





xSkyBox::~xSkyBox() {
	this->Destroy();
}





// Метод создаёт скайбокс(небесный куб), поместим всё творение сразу в видеопамять и не надо лишние
// метания по видеошине из системной памяти
void  xSkyBox::Create(GLfloat width, GLfloat height, GLfloat depth, GLfloat h) {
	this->width  = width;
	this->height = height;
	this->depth  = depth;

	listid = glGenLists(5);

	glNewList(listid, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);  // рисуем заднию стенку
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(width,  height, -depth);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-width, height, -depth);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(width,  h,  -depth);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-width, h,  -depth);
		glEnd();
	glEndList();

	glNewList(listid + 1u, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);  // рисуем переднию стенку
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-width,  height, depth);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(width,  height, depth);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-width, h, depth);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(width,  h, depth);
		glEnd();
	glEndList();

	glNewList(listid + 2u, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);  // рисуем левую стенку
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-width,  height, -depth);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(-width,  height,  depth);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-width, h, -depth);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(-width, h,  depth);
		glEnd();
	glEndList();

	glNewList(listid + 3u, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);  // рисуем правую стенку
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(width, height,  depth);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(width, height, -depth);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(width, h, depth);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(width, h,-depth);
		glEnd();
	glEndList();

	glNewList(listid + 4u, GL_COMPILE);
		glBegin(GL_TRIANGLE_STRIP);  // рисуем верхнию стенку
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-width,  height, -depth);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(width,  height, -depth);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-width,  height,  depth);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(width,   height,  depth);
		glEnd();
	glEndList();
}





// Метод выводит небесный куб в сцену
void  xSkyBox::Display(void) {
	for(GLuint i = 0u; i < 5; i++) {
		xTextura::Set(texs[i]);
		glCallList(listid + i);
	}
}
	




// Метод загружает текстуры в видеопамять
bool  xSkyBox::Load(int index, const TCHAR* filename) {
	
	if(index >= MAX_TEXS)
		return false;

	texs[index] = xTextura::OpenImage(filename);
	if(texs[index] == 0u)
		return false;

	return true;
}





void  xSkyBox::Destroy(void) {
	glDeleteLists(listid, 5);
	for(GLint i = 0; i < MAX_TEXS; i++)
		xTextura::Free(texs[i]);
	ZeroMemory(texs, sizeof(texs));
}






