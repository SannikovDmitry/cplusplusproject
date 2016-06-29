/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#if ! defined(_HEADER_VILLAGE_)
#define _HEADER_VILLAGE_
#if _MSC_VER > 1000
#pragma once
#endif
#ifdef DIRECTSOUND_VERSION
#if DIRECTSOUND_VERSION < 0x0800
#define DIRECTSOUND_VERSION  0x0700
#endif
#endif

#ifndef STRICT
#define STRICT
#endif

#ifndef GL_PI
#define GL_PI 3.1457893f
#endif

#define  fRADIAN(degree)           (degree * (GL_PI / 180.0f))
#define  fDEGREE(radian)           ((radian * 180.0f) / GL_PI)
#define  fNUM(elapsed, per)        (per / 1000.0f * elapsed)
#define  fRAND(a, b)               (a + ((GLfloat)rand() * (b - a) / RAND_MAX))
#define  DELETE_HEAP(obj)          { if(obj != NULL) {  delete obj, obj = NULL; } }
#define  DELETE_HEAP_ARRAY(arr)    { if(arr != NULL) {  delete[] arr, arr = NULL; } }

#ifndef  ABS
#define  ABS(a)  ((a < 0) ? (-a) : a)
#endif

#include <tchar.h>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <olectl.h>
#include <vector>
#include <list>
#include "resource.h"
#include "glscene.h"
#include "glmath.h"
#include "sound.h"
#include "globjects.h"
#include "glmodel.h"
#include "glaction.h"
#include "game.h"









#endif