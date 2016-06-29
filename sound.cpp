/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"





// звуковые эффекты
xSound::xSound(void) : buf(NULL){}
	


xSound::xSound(LPDIRECTSOUND sound, xSound* obj) {
	buf   = NULL;
	this->Create(sound, obj);	
}


xSound::xSound(LPDIRECTSOUND sound, const TCHAR* filename) {
	buf   = NULL;
	this->Create(sound, filename);
}


xSound::~xSound() {
	this->Destroy();
}




// Метод создаёт первичный буфер загружая данные из WAV(RIFF) файла формата PCM
BOOL   xSound::Create(LPDIRECTSOUND sound, const TCHAR* filename) {
	CHAR*  data;
	WAVEFORMATEX wave;

	HANDLE  fp = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(fp == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD  riff = 0uL, dr = 0uL;

	ReadFile(fp, &riff, 4uL, &dr, NULL);
	if(riff != MAKEFOURCC('R', 'I', 'F', 'F')) {
		CloseHandle(fp);
		return FALSE;
	}
	SetFilePointer(fp, 4L, NULL, FILE_CURRENT);

	ReadFile(fp, &riff, 4uL, &dr, NULL);
	if(riff != MAKEFOURCC('W', 'A', 'V', 'E')) {
		CloseHandle(fp);
		return FALSE;
	}

	ReadFile(fp, &riff, 4uL, &dr, NULL);
	if(riff != MAKEFOURCC('f', 'm', 't', ' ')) {
		CloseHandle(fp);
		return FALSE;
	}

	LONG fmt = 0L;

	ReadFile(fp, &fmt, 4uL, &dr, NULL);
	if(fmt == 0L) {
		CloseHandle(fp);
		return FALSE;
	}

	ZeroMemory(&wave, sizeof(WAVEFORMATEX));
	ReadFile(fp, &wave, sizeof(WAVEFORMATEX), &dr, NULL);
	SetFilePointer(fp, fmt - sizeof(WAVEFORMATEX), NULL, FILE_CURRENT);

	if(wave.wFormatTag !=  1uL) {
		CloseHandle(fp);
		return FALSE;
	}

	ReadFile(fp, &riff, 4uL, &dr, NULL);
	if(riff == MAKEFOURCC('f', 'a', 'c', 't')) {
		ReadFile(fp, &fmt, 4uL, &dr, NULL);
		SetFilePointer(fp, fmt, NULL, FILE_CURRENT);
		ReadFile(fp, &riff, 4uL, &dr, NULL);
	}

	if(riff != MAKEFOURCC('d', 'a', 't', 'a')) {
		CloseHandle(fp);
		return FALSE;
	}

	DWORD   size_data = 0u;
	ReadFile(fp, &size_data, 4uL, &dr, NULL);
 
	data = new  CHAR[size_data];
	if(data == NULL) {
		CloseHandle(fp);
		return FALSE;
	}

	ReadFile(fp, data, size_data, &dr, NULL);
	CloseHandle(fp);

	if(dr != size_data) {
		delete[] data;
		return  FALSE;
	}

	DSBUFFERDESC   desc;
	ZeroMemory(&desc, sizeof(DSBUFFERDESC));
	desc.dwSize          = sizeof(DSBUFFERDESC);
	desc.dwBufferBytes   = size_data;
	desc.dwFlags         = DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC;
	desc.lpwfxFormat     = &wave;

	if(FAILED(sound->CreateSoundBuffer(&desc, &buf, 0))) {
		delete[] data;
		return FALSE;
	}

	size_data   = 0uL;
	LPVOID  dst = NULL; // получим адрес буфера для записи в него данных
	if(FAILED(buf->Lock(0, 0, &dst,  &size_data, 0, 0,  DSBLOCK_ENTIREBUFFER))) {
		delete[] data;
		return FALSE;
	}
	CopyMemory(dst, data, size_data);   // копируем внего
	buf->Unlock(dst, size_data, 0, 0);   
	delete[] data;
	return TRUE;
}





// Метод создаёт дупликат с оригиналa
BOOL xSound::Create(LPDIRECTSOUND sound, xSound* obj) {
	if(sound->DuplicateSoundBuffer(obj->GetBuffer(), &buf) == DS_OK) 
		return TRUE;
	return FALSE;
}




 // Метод воспроизводит звук
void  xSound::Play(DWORD  flags /*DSBPLAY_LOOPING*/) {
	DWORD state = 0uL;
	if(buf != NULL) {
		if(buf->GetStatus(&state) == DS_OK) {
			if(! (state & DSBSTATUS_LOOPING) && !(state & DSBSTATUS_PLAYING))
				buf->Play(0uL, 0uL, flags);
		}
	}
}





// Метод останавливает воспроизведение звука
void  xSound::Stop(void) { 
	DWORD state = 0uL;
	if(buf != NULL) {
		if(buf->GetStatus(&state) == DS_OK) {
			if((state & DSBSTATUS_LOOPING) || (state & DSBSTATUS_PLAYING))
				buf->Stop();
		}
	}
}



// Метод по установки позиции
void  xSound::SetPosition(DWORD  pos) {
	if(buf != NULL)
		buf->SetCurrentPosition(pos);
}



// 0 ...  -10000
void  xSound::SetVolume(LONG  vol) { 
	if(buf != NULL)
		buf->SetVolume(vol);
}




LONG  xSound::GetVolume(void) {
	LONG  vol = 0L;
	buf->GetVolume(&vol);
	return vol;
}




LPDIRECTSOUNDBUFFER  xSound::GetBuffer(void) {
	return buf;
}



// Метод уничтожает буфер
void  xSound::Destroy(void) {
	if(buf != NULL) {
		buf->Stop();
		buf->Release();
	}
	buf = NULL;
}


