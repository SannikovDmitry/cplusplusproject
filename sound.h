/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#if ! defined(_SOUND_VILLAGE_)
#define _SOUND_VILLAGE_
#ifdef _MSC_VER
#if _MSC_VER > 1000
#pragma once
#endif
#endif




// объект звуковые эффекты
class  xSound {
private:
	LPDIRECTSOUNDBUFFER    buf;
public:
	xSound(void);
	xSound(LPDIRECTSOUND sound, xSound* obj);
	xSound(LPDIRECTSOUND sound, const TCHAR* filename);
	~xSound();
public:
	BOOL  Create(LPDIRECTSOUND sound, const TCHAR* filename);
	BOOL  Create(LPDIRECTSOUND sound, xSound* obj);
	void  Play(DWORD  flags = 0uL /*DSBPLAY_LOOPING*/);
	void  Stop(void);
	void  SetPosition(DWORD pos);
	void  SetVolume(LONG vol);
	LONG  GetVolume(void);
	void  Destroy(void);
	LPDIRECTSOUNDBUFFER  GetBuffer(void);
	LPDIRECTSOUNDBUFFER  Duplicate(LPDIRECTSOUND sound);
};




#endif