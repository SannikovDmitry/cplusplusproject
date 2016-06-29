/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#if ! defined(_GLACTON_VILLAGE_)
#define _GLACTON_VILLAGE_
#if _MSC_VER > 1000
#pragma once
#endif
#define  MAX_HOUSES  8





// структура для плазменной пули
struct sBullet {
	GLVECTOR3  pos;
	GLVECTOR3  vec;
};



// Пользователь от первого лица
class  xUser {
private:
	xSound*    snd_uron;
	xSound*    snd_fire;
	xSound*    snd_step;
	xFigure*   gun;
	GLuint     tex_gun;
	GLuint     tex_lazer;
	GLVECTOR3  pos;
	GLVECTOR3  size;
	BOOL       ifire;
	BOOL       imove;
	int        life;

	std::list<sBullet>  bullets;
public:
	xUser(void);
	xUser(LPDIRECTSOUND sound);
	~xUser();
public:
	void  Create(LPDIRECTSOUND sound);
	void  Initialize(void);
	void  Display(GLfloat felapsed, GLfloat ftime);
	void  DisplayLazer(GLfloat felapsed, GLfloat ftime, GLfloat angleY);
	void  DisplayCross(const LPSIZE pscreen);
	void  OnFire(WPARAM wParam, const LPGLVECTOR3& hpos, GLfloat felapsed, GLfloat yaw, GLfloat pitch);
	void  SetMove(BOOL move);
	void  Destroy(void);
	void  DecLife(void);

	std::list<sBullet>*  Bullets(void) {
		return &bullets;
	}

	int  GetLife(void) const {
		return life;
	}

	GLVECTOR3& Size(void) {
		return size;
	}
};




/////////////////////////////////////////////////////////////////////////////////////////////////////







// структура для робота "марки - TM"
struct  sRobotTM {
	GLVECTOR3 pos;
	GLVECTOR3 vec;
	GLVECTOR3 rot;
	GLfloat   angle;
	int       life;
	BOOL      uron;
	BOOL      crush;
	GLVECTOR2 cadrs; 
	GLfloat   time_crush;
	GLfloat   fsmoke;
	GLfloat   length;
	GLfloat   position;
	GLVECTOR3 tpos[2];
	int       primary_task;
	xSound*   snd_uron;
	xSound*   snd_crush;
	xSound*   snd_move;
	GLfloat   time_sound;

	static const int MAX_ROBOTS_TM;

	sRobotTM(void){}
	sRobotTM(const LPGLVECTOR3& user_pos, GLfloat x, GLfloat z) {
		this->Initialize(user_pos, x, z);
	}

	// инициализация
	void  Initialize(const LPGLVECTOR3& user_pos, GLfloat x, GLfloat z) {
		pos.x        = x;
		pos.z        = z;
		pos.y        = 0.0f;
		rot          = 0.0f;
		vec          = 0.0f;
		life         = 10;
		angle        = 0.0f;
		crush        = FALSE;
		uron         = FALSE;
		fsmoke       = 0.0f;
		primary_task = rand() % 2;
		position     = 0.0f;
		length       = 0.0f;
		time_sound   = 0.0f;

		if(primary_task != 0) {
			VectorSubtract(&vec, &pos, user_pos);
			rot.y     = fDEGREE(atan2f(vec.x, vec.z));
			VectorNormalize(&vec);
			vec.y     = 0.0f;
			vec      *= 0.9f;
			tpos[0]   = pos;
			tpos[1]   = *user_pos;
			tpos[0].y = tpos[1].y = 0.0f;
			length    = VectorLength(&(tpos[0] - tpos[1]));
		}

		snd_move->Play(DSBPLAY_LOOPING);
		snd_move->SetVolume(DSBVOLUME_MIN);
	}



	// всякие полезные действия по анимации-движения
	void  UpdateMove(const LPGLVECTOR3& user_pos, GLfloat cube, GLfloat felapsed, GLfloat ftime) {
	
		if(crush)
			return;

		if(! primary_task) {
			VectorSubtract(&vec, &pos, user_pos);
			rot.y  = fDEGREE(atan2f(vec.x, vec.z));
			VectorNormalize(&vec);
			vec.y  = 0.0f;
			vec   *= 0.9f;
			pos   -= vec * fNUM(felapsed, 21.0f);
		} else {

			GLfloat tmp = pos.y;
			LineP2(&pos, position / length, &tpos[0], &tpos[1]);
			position  += fNUM(felapsed, 20.0f);
			pos.y      = tmp;

			if(position >= length) {
				primary_task = 0;
				position     = length = 0.0f;
				return;
			}
			
			// если в радиусе пользователь окажется взять направление на него
			if(IsCollisionSphere(&pos, 25.0f, user_pos, 25.0f))
				primary_task = 0;
		}
		angle -= fNUM(felapsed, 290.0f);

		// через каждую секунду регулировка грмоксоти звка на расстояние между пользователем
		if((ftime - time_sound) > 1000.0f) {
			time_sound = ftime;
			this->volume_ctrl(user_pos);
		}

	}



	// вывод дыма и взрыва
	void  DisplayEffect(const LPGLVECTOR3& user_pos, GLuint tex, GLfloat angleY, GLfloat felapsed, GLfloat ftime) {
		
		GLVECTOR3 vpos;

		if(uron) {  // обработка урона
			xTextura::Set(tex);
			fsmoke -= fNUM(felapsed, 1.4f);
			xModel::DrawRect(&GLVECTOR3(pos.x, pos.y + 5.5f, pos.z), fRAND(5, 9), angleY); 
			if(fsmoke < 0.1f)
				uron = FALSE;
		}


		if(crush) { // обработка взрыва
			const GLfloat cadr = 1.0f / 3.0f;

			if((ftime - time_crush) > 70.0f) {
				time_crush = ftime;
				cadrs.x   += cadr;
				if(cadrs.x >= 1.0f) {
					cadrs.x  = 0.0f;
					cadrs.y += cadr;
					if(cadrs.y >= 1.0f) {
						cadrs = 0.0f;
						crush = FALSE;
						
						GLfloat rex = -500 + 10.0f;
						GLfloat rez = -500 + fRAND(0, (int)500 * 2);
					
						if((rand() % 2) != 0) {
							rex =  500 - 10.0f;
							rez = -255 + fRAND(1, (int)500);
						}

						this->Initialize(user_pos, rex, rez);
						return;
					}
				}
			}
			xTextura::Set(tex);
			xModel::DrawAnimationSprite(&GLVECTOR3(pos.x, pos.y + 5.5f, pos.z), &GLVECTOR3(8.0f, 8.0f, 0.0f), angleY, &cadrs, &GLVECTOR2(cadr)); 
		}

	}


	BOOL  IsCrush(void) const {
		return crush;
	}


	BOOL IsUron(void) const {
		return uron;
	}


	// проверка столкновения с пользовательской пулей
	int  IsCollision(const LPGLVECTOR3& vpos, GLfloat radius) {
		
		if(crush)
			return 0;
		
		if(IsCollisionSphere(&GLVECTOR3(pos.x, pos.y + 4.0f, pos.z), 3.3f, vpos, radius)) {
			primary_task = 0;
			if(--life > 0) {
				uron    = TRUE;
				fsmoke  = 1.0f;
				crush   = FALSE;
				return 1;
			} else {
				uron       = FALSE;
				crush      = TRUE;
				cadrs      = 0.0f;
				time_crush = 0.0f;
				snd_move->Stop();
				return 2;
			}
		}
		return 0;
	}


	void  ResetTask(void) {
		primary_task = 0;
	}

	
	void  volume_ctrl(const LPGLVECTOR3& user_pos) {
		GLfloat len = VectorLength(&(*user_pos - pos));
		len         = ABS(len);
		if(len <= 150.0f)
			snd_move->SetVolume(max(DSBVOLUME_MAX - len * 6L, DSBVOLUME_MIN));
	}
	
};






// структура для летающего робота
struct  sRobotFly {
	GLVECTOR3 pos;
	GLVECTOR3 vec;
	GLfloat   rotY;
	GLfloat   fangle;
	int       life;
	BOOL      uron;
	BOOL      crush;
	GLfloat   fsmoke;
	GLVECTOR2 cadrs; 
	GLfloat   time_crush;

	enum  {
		TASK_PUSH  = 0x1,  // задание скинуть бомбу на пользователя любой ценой
		TASK_MOVE  = 0x2   // цель полёта выбирается во время инициализации
	};
	int       task;

	static const int MAX_ROBOTS_FLY; // кол-во роботов в сцене

	sRobotFly(void): task(TASK_PUSH) {}
	sRobotFly(const LPGLVECTOR3& user_pos, GLfloat x, GLfloat z) {
		this->Initialize(user_pos, x, z);
	}



	// инициализация
	void  Initialize(const LPGLVECTOR3& user_pos, GLfloat x, GLfloat z) {
		pos.x  = x;
		pos.y  = 90.0f + fRAND(1, 21);
		pos.z  = z;
		life   = 7;
		fangle = 0.0f;
		vec    = 0.0f;
		task   = 1 + (rand() % 2);
		uron   = FALSE;
		crush  = FALSE;

		if(task & TASK_MOVE) {
			VectorSubtract(&vec, &pos, user_pos);
			rotY   = atan2f(vec.x, vec.z) * 180.0f / GL_PI;
			VectorNormalize(&vec);
			vec   *= 0.9f;
			vec.y  = 0.0f;
		}
	}


	// всякие полезные действия
	void  UpdateMove(const LPGLVECTOR3& user_pos, GLfloat cube, GLfloat felapsed, GLfloat ftime) {
		
		if(crush)
			return;

		if(task & TASK_PUSH) { // полёт с тараном

			VectorSubtract(&vec, &pos, user_pos);
			rotY   = fDEGREE(atan2f(vec.x, vec.z));
			VectorNormalize(&vec);
			vec   *= 0.9f;
			pos   -= vec * fNUM(felapsed, 24.0f);

		} else if(task & TASK_MOVE) {  // простой полёт

			pos   -= vec * fNUM(felapsed, 24.0f);
			if((pos.x > cube) || (pos.x < -cube) || (pos.z > cube) || (pos.z < -cube)) { // всё улутели за край мира(skybox) с генерировать новую позицию-задачу
				
				GLfloat rex = -cube + 10.0f;
				GLfloat rez = -cube + fRAND(0, (int)cube * 2);
			
				if((rand() % 2) != 0) {
					rex = cube - 10.0f;
					rez = cube + fRAND(1, (int)cube);
				}

				this->Initialize(user_pos, rex, rez);
			}
		}
		fangle += fNUM(felapsed, 8.0f);
	}



	// вывод дыма и взрыва
	void  DisplayEffect(const LPGLVECTOR3& user_pos, GLuint tex, GLfloat angleY, GLfloat felapsed, GLfloat ftime) {
		
		GLVECTOR3 vpos;

		if(uron) {  // обработка урона
			xTextura::Set(tex);
			fsmoke -= fNUM(felapsed, 1.4f);
			xModel::DrawRect(&pos, fRAND(5, 9), angleY); 
			if(fsmoke < 0.1f)
				uron = FALSE;
		}


		if(crush) { // обработка взрыва
			const GLfloat cadr = 1.0f / 3.0f;

			if((ftime - time_crush) > 70.0f) {
				time_crush = ftime;
				cadrs.x   += cadr;
				if(cadrs.x >= 1.0f) {
					cadrs.x  = 0.0f;
					cadrs.y += cadr;
					if(cadrs.y >= 1.0f) {
						cadrs = 0.0f;
						crush = FALSE;
						
						GLfloat rex = -500 + 10.0f;
						GLfloat rez = -500 + fRAND(0, (int)500 * 2);
					
						if((rand() % 2) != 0) {
							rex =  500 - 10.0f;
							rez = -255 + fRAND(1, (int)500);
						}

						this->Initialize(user_pos, rex, rez);
						return;
					}
				}
			}
			xTextura::Set(tex);
			xModel::DrawAnimationSprite(&pos, &GLVECTOR3(5.0f, 5.0f, 0.0f), angleY, &cadrs, &GLVECTOR2(cadr)); 
		}

	}


	// проверка текущей операции
	int  IsTaskPush(void) const {
		return (task & TASK_PUSH);
	}


	BOOL  IsCrush(void) const {
		return crush;
	}


	BOOL IsUron(void) const {
		return uron;
	}


	// проверка столкновения с пользовательской пулей
	int  IsCollision(const LPGLVECTOR3& vpos, GLfloat radius) {
		
		if(crush)
			return 0;
		
		if(IsCollisionSphere(&pos, 6.0f, vpos, radius)) {
			if(--life > 0) {
				fsmoke  = 1.0f;
				uron    = TRUE;
				return 1;
			} else {
				uron       = FALSE;
				crush      = TRUE;
				cadrs      = 0.0f;
				time_crush = 0.0f;
				return 2;
			}
		}
		return 0;
	}
};









// Класс-карта высот, загружает карту из файла .RAW
class  xTerrain {

	// структура для растительности дерево
	struct  sTree {
		GLfloat x, y, z;
		GLuint  type;

		sTree(void){}
		sTree(GLuint type, GLfloat x, GLfloat y, GLfloat z) {
			this->type = type;
			this->x    = x;
			this->y    = y;
			this->z    = z;
		}
	};

private:
	GLuint    tex_house[2];
	GLuint    tex;
	GLuint    tex_tree[4];
	GLuint    tex_smoke;
	GLuint    tex_crush;
	GLfloat*  vertices;
	GLfloat*  normals;
	GLfloat*  texs;
	GLfloat*  fmap;   
	GLfloat   top_floor;
	GLint     vsize;
	GLint     tsize;
	GLint     vstep;
	GLint     map_size;
	GLint     map_rect;
	GLint     half_size;
	GLuint    vidlist;

	xSound*   snd_crush;  
	xSound*   snd_uron;
	xSound*   snd_move[2];


	std::vector<sTree> trees;  // растительность кусты, деревья.
	GLVECTOR3          houses[MAX_HOUSES];
	GLVECTOR3          house_size;
	int                size_house;
	GLfloat            house_shadow[16];

	xRobotFly          robot_fly;
	xRobotTM           robot_tm;
	sRobotFly          robots_fly[2];  // 2 - летающего робота на этом хватит
	sRobotTM           robots_tm[2];   // 2 - катающих по земле робота
public:
	xTerrain(void);
	~xTerrain();
public:
	bool    Create(LPDIRECTSOUND sound, GLint field_size, GLint step);
	void    Initialize(const LPGLVECTOR3& user_pos);
	void    Display(void);
	void    BeginMatrix(const LPGLVECTOR3& hpos);
	void    EndMatrix(void);
	void    Normalize(void);
	bool    LoadTerrain(const TCHAR* raw_filename, GLfloat scale = 0.08f);
	void    LoadTextura(const TCHAR* filename);
	void    Destroy(void);
	void    DisplayVegetations(const LPGLVECTOR3& user_pos, GLfloat angleY, GLfloat ftime, GLfloat felapsed);
	void    DisplayObjects(void);
	void    DisplayShadow(void);
	void    DisplayRobots(const LPGLVECTOR3& user_pos, GLfloat felapsed, GLfloat ftime);
	void    DisplayShadowRobots(const LPGLVECTOR4& light_pos);
	void    DisplayEffectRobots(const LPGLVECTOR3& user_pos, GLfloat angleY, GLfloat felapsed, GLfloat ftime);
	void    OffsetCameraY(LPGLVECTOR3 pos);
	void    SetTopFloor(GLfloat top);
	void    SetLightPos(const LPGLVECTOR4& light);
	void    RectOffset(GLint x, GLint z, GLfloat* arr);
	void    ObjectsToField(void);
	GLfloat GetPlaneShadow(GLfloat* mat, LPGLVECTOR3 pos, const LPGLVECTOR4& light);
	inline  GLfloat Offset(GLint x, GLint y);
	inline  GLfloat GetOffsetTop(GLfloat x, GLfloat z);
	void    CollisionBullet(const LPGLVECTOR3& user_pos, std::list<sBullet>* plist);
	bool    CollisionUser(xCamera* camera, const LPGLVECTOR3& user_size);

	GLuint  GetVertexSize(void) const {
		return vsize;
	}

	GLuint  tSkull(void) {
		return robot_tm.GetTex();
	}

	GLuint tHouse(int i) {
		return tex_house[i];
	}

	GLfloat  CubeSize(void) const {
		return static_cast<GLfloat>(half_size - 90);
	}

private:
	void  compile_tree(GLuint lid, GLuint tid, GLfloat x, GLfloat y);
	void  play_sound(const LPGLVECTOR3& va, const LPGLVECTOR3& vb, int code);
};



///////////////////////////////////////////////////////////////////////////////////////////////////



// класс небесного куба
class xSkyBox {
private:
	enum { MAX_TEXS = 5 };
	GLuint  listid;
	GLuint  texs[MAX_TEXS];
	GLfloat width;
	GLfloat height;
	GLfloat depth;
public:
	xSkyBox(void);
	~xSkyBox();
public:
	void  Create(GLfloat width, GLfloat height, GLfloat depth, GLfloat h = 0.0f);
	void  Display(void);
	bool  Load(int index, const TCHAR* filename);
	void  Destroy(void);
};













#endif