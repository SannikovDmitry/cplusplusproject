/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#if ! defined(_GLMATH_VILLAGE_)
#define _GLMATH_VILLAGE_
#if _MSC_VER > 1000
#pragma once
#endif





// цвет RGBA
typedef struct _GLCOLOR {
	GLfloat r, g, b, a;

	_GLCOLOR(void){}
	_GLCOLOR(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
	_GLCOLOR(const _GLCOLOR& color) {
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

} GLCOLOR, *LPGLCOLOR;





// структура 4D-вектор
typedef struct _GLVECTOR4 {

	GLfloat x, y, z, w;

	_GLVECTOR4(void){}

	_GLVECTOR4(GLfloat n) {
		x = y = z = w = n;
	}

	_GLVECTOR4(const _GLVECTOR4& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	_GLVECTOR4(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

} GLVECTOR4, *LPGLVECTOR4;





// структура 2D-вектор
typedef struct _GLVECTOR2 {

	GLfloat x, y;

	_GLVECTOR2(void){}

	_GLVECTOR2(GLfloat n) {
		x = y  = n;
	}

	_GLVECTOR2(const _GLVECTOR4& v) {
		x = v.x;
		y = v.y;
	}

	_GLVECTOR2(GLfloat x, GLfloat y) {
		this->x = x;
		this->y = y;
	}

} GLVECTOR2, *LPGLVECTOR2;





// структура 3D-вектор
typedef struct _GLVECTOR3 {

	GLfloat x, y, z;

	_GLVECTOR3(void){}

	_GLVECTOR3(GLfloat n) {
		x = y = z = n;
	}

	_GLVECTOR3(const _GLVECTOR3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
	}

	_GLVECTOR3(GLfloat x, GLfloat y, GLfloat z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline _GLVECTOR3 operator - (void) {
		_GLVECTOR3 vec;
		vec.x = -x;
		vec.y = -y;
		vec.z = -z;
		return vec;
	}

	inline _GLVECTOR3& operator += (const _GLVECTOR3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline _GLVECTOR3& operator *= (const _GLVECTOR3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	inline _GLVECTOR3& operator -= (const _GLVECTOR3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline _GLVECTOR3& operator /= (const _GLVECTOR3& v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}

	inline _GLVECTOR3& operator += (GLfloat n) {
		x += n;
		y += n;
		z += n;
		return *this;
	}

	inline _GLVECTOR3& operator *= (GLfloat n) {
		x *= n;
		y *= n;
		z *= n;
		return *this;
	}

	inline _GLVECTOR3& operator -= (GLfloat n) {
		x -= n;
		y -= n;
		z -= n;
		return *this;
	}

	inline _GLVECTOR3& operator /= (GLfloat n) {
		x /= n;
		y /= n;
		z /= n;
		return *this;
	}

	inline _GLVECTOR3 operator + (const _GLVECTOR3& v) {
		GLVECTOR3 dst;
		dst.x = x + v.x;
		dst.y = y + v.y;
		dst.z = z + v.z;
		return dst;
	}
		
	inline _GLVECTOR3 operator * (const _GLVECTOR3& v)  {
		GLVECTOR3 dst;
		dst.x = x * v.x;
		dst.y = y * v.y;
		dst.z = z * v.z;
		return dst;
	}

	inline _GLVECTOR3 operator - (const _GLVECTOR3& v)  {
		GLVECTOR3 dst;
		dst.x = x - v.x;
		dst.y = y - v.y;
		dst.z = z - v.z;
		return dst;
	}

	inline _GLVECTOR3 operator / (const _GLVECTOR3& v)  {
		GLVECTOR3 dst;
		dst.x = x / v.x;
		dst.y = y / v.y;
		dst.z = z / v.z;
		return dst;
	}

	inline _GLVECTOR3 operator + (GLfloat n) {
		GLVECTOR3 dst;
		dst.x = x + n;
		dst.y = y + n;
		dst.z = z + n;
		return dst;
	}
		
	inline _GLVECTOR3 operator * (GLfloat n)  {
		GLVECTOR3 dst;
		dst.x = x * n;
		dst.y = y * n;
		dst.z = z * n;
		return dst;
	}
		
	inline _GLVECTOR3 operator - (GLfloat n)  {
		GLVECTOR3 dst;
		dst.x = x - n;
		dst.y = y - n;
		dst.z = z - n;
		return dst;
	}
		
	inline _GLVECTOR3 operator / (GLfloat n)  {
		GLVECTOR3 dst;
		dst.x = x / n;
		dst.y = y / n;
		dst.z = z / n;
		return dst;
	}

	inline friend _GLVECTOR3 operator / (const _GLVECTOR3& va, const _GLVECTOR3& vb) {
		_GLVECTOR3 v;
		v.x = va.x / vb.x;
		v.y = va.y / vb.y;
		v.z = va.z / vb.z;
		return v;
	}

	inline friend _GLVECTOR3 operator * (const _GLVECTOR3& va, const _GLVECTOR3& vb) {
		_GLVECTOR3 v;
		v.x = va.x * vb.x;
		v.y = va.y * vb.y;
		v.z = va.z * vb.z;
		return v;
	}

	inline friend _GLVECTOR3 operator + (const _GLVECTOR3& va, const _GLVECTOR3& vb) {
		_GLVECTOR3 v;
		v.x = va.x + vb.x;
		v.y = va.y + vb.y;
		v.z = va.z + vb.z;
		return v;
	}

	inline friend _GLVECTOR3 operator - (const _GLVECTOR3& va, const _GLVECTOR3& vb) {
		_GLVECTOR3 v;
		v.x = va.x - vb.x;
		v.y = va.y - vb.y;
		v.z = va.z - vb.z;
		return v;
	}


	inline friend _GLVECTOR3 operator / (const _GLVECTOR3& v, GLfloat n) {
		GLVECTOR3 nv;
		nv.x = v.x / n;
		nv.y = v.y / n;
		nv.z = v.z / n;
		return nv;
	}

	inline friend _GLVECTOR3 operator * (const _GLVECTOR3& v, GLfloat n) {
		GLVECTOR3 nv;
		nv.x = v.x * n;
		nv.y = v.y * n;
		nv.z = v.z * n;
		return nv;
	}

	inline friend _GLVECTOR3 operator / (GLfloat n, const _GLVECTOR3& v) {
		GLVECTOR3 nv;
		nv.x = n / v.x;
		nv.y = n / v.y;
		nv.z = n / v.z;
		return nv;
	}

	inline friend _GLVECTOR3 operator * (GLfloat n, const _GLVECTOR3& v) {
		GLVECTOR3 nv;
		nv.x = n * v.x;
		nv.y = n * v.y;
		nv.z = n * v.z;
		return nv;
	}

	inline bool operator == (GLfloat n) const {
		return ((x == n) && (y == n) && (z == n));
	}

	inline bool operator == (const _GLVECTOR3& v) const {
		return ((x == v.x) && (y == v.y) && (z == v.z));
	}

	inline bool operator != (GLfloat n) const {
		return ((x != n) && (y != n) && (z != n));
	}

	inline bool operator != (const _GLVECTOR3& v) const {
		return ((x != v.x) && (y != v.y) && (z != v.z));
	}

} GLVECTOR3, *LPGLVECTOR3;






// Не представляю свою жизнь без кватернионов
typedef  struct _GLQUATERNION {
	GLfloat x, y, z, w;

	_GLQUATERNION(void){}
	_GLQUATERNION(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	_GLQUATERNION(const _GLQUATERNION& q) {
		x = q.x;
		y = q.y;
		z = q.z;
		w = q.w;
	}
	_GLQUATERNION(GLfloat n) {
		x = y = z = w = n;
	}

	inline _GLQUATERNION operator - (void) {
		return _GLQUATERNION(-x, -y, -z, w);
	}


	inline _GLQUATERNION& operator *= (GLfloat n) {
		x *= n;
		y *= n;
		z *= n;
		w *= n;
		return *this;
	}


	inline _GLQUATERNION& operator += (GLfloat n) {
		x += n;
		y += n;
		z += n;
		w += n;
		return *this;
	}


	inline _GLQUATERNION operator * (const _GLQUATERNION& q) {
		_GLQUATERNION quat;
		quat.x = w * q.x + x * q.w + y * q.z - z * q.y;
		quat.y = w * q.y + y * q.w + z * q.x - x * q.z;
		quat.z = w * q.z + z * q.w + x * q.y - y * q.x;
		quat.w = w * q.w - x * q.x - y * q.y - z * q.z;
		return quat;
	}

} GLQUATERNION, *LPGLQUATERNION;







// Структура плоскость
typedef struct _GLPLANE {
	GLfloat a, b, c, d;

	_GLPLANE(void){}
	_GLPLANE(GLfloat n) {
		a = b = c = d = n;
	}

	_GLPLANE(const _GLPLANE& p) {
		a = p.a;
		b = p.b;
		c = p.c;
		d = p.d;
	}

	_GLPLANE(GLfloat a, GLfloat b, GLfloat c, GLfloat d) {
		this->a = a;
		this->b = b;
		this->c = c;
		this->d = d;
	}

} GLPLANE, *LPGLPLANE;





extern inline void VectorSubtract(LPGLVECTOR3 vout, const LPGLVECTOR3& va, const LPGLVECTOR3& vb);
extern inline void VectorCross(LPGLVECTOR3 vout, const LPGLVECTOR3& va, const LPGLVECTOR3& vb);
extern inline GLfloat VectorLength(const LPGLVECTOR3& vec);
extern inline GLfloat VectorDot(const LPGLVECTOR3& va, const LPGLVECTOR3& vb);
extern inline void VectorNormalize(LPGLVECTOR3 vout);
extern inline void VectorNormal(LPGLVECTOR3 pn, const LPGLVECTOR3& va, const LPGLVECTOR3& vb, const LPGLVECTOR3& vc);
extern inline void VectorAngle(LPGLVECTOR3 hvec, GLfloat angle_x, GLfloat angle_y);
extern inline LPGLVECTOR4 VectorMatrixMultiply(LPGLVECTOR4 v, const GLfloat* mat);

extern inline GLfloat VectorPlaneDot(const LPGLVECTOR3& v, const LPGLPLANE& p);
extern inline GLfloat VectorPlaneDot(const LPGLVECTOR4& v, const LPGLPLANE& p);
extern inline void PlaneEquation(LPGLPLANE plane, const LPGLVECTOR3 varr);

extern inline void QuaterionToMatrix(const LPGLQUATERNION& q, GLfloat* mat);
extern inline void QuaternionFromAxisAngle(LPGLQUATERNION q, const LPGLVECTOR3& v, GLfloat d);
extern inline void QuaternionNormalize(LPGLQUATERNION q);
extern inline GLfloat QuaternionMagnitudeSeq(const LPGLQUATERNION& q);
extern inline void QuaternionFromVector(LPGLVECTOR3 dst, const LPGLQUATERNION& q, const LPGLVECTOR3& v);
extern inline void QuaternionPitchYawRoll(LPGLQUATERNION q, GLfloat pitch, GLfloat yaw, GLfloat roll);

extern inline void MatrixShadow(GLfloat* mat, const LPGLPLANE& p, const LPGLVECTOR4& v);
extern inline void MatrixIdentity(GLfloat* mat);
extern inline void MatrixRotateY(GLfloat* mat, GLfloat degree);

//__forceinline
// столкновения-пересечения intersect
extern inline bool IsCollisionSphere(const LPGLVECTOR3& va, GLfloat ra, const LPGLVECTOR3& vb, GLfloat rb); 
extern inline bool IsCollisionPointRect(const LPGLVECTOR3& pt, const LPGLVECTOR3& vb, const LPGLVECTOR3& size);
extern inline bool IsRectIntersect(LPGLVECTOR3 va, const LPGLVECTOR3& s1, const LPGLVECTOR3& vb, const LPGLVECTOR3& s2);


extern inline  void  LineP2(LPGLVECTOR3 vout, GLfloat t, const LPGLVECTOR3& first, const LPGLVECTOR3& last);

#endif