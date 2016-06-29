/*
	Автор(с): Санников, Кулебякин, Ступак и Пархоменко
	Название команды: AGRAGE
	e-mail:   sdk96@mail.ru
*/
#include "header.h"





// Вычитание векторов
inline void VectorSubtract(LPGLVECTOR3 vout, const LPGLVECTOR3& va, const LPGLVECTOR3& vb) {
	vout->x = va->x - vb->x;
	vout->y = va->y - vb->y;
	vout->z = va->z - vb->z;
}




// Векторное произведение
inline void VectorCross(LPGLVECTOR3 vout, const LPGLVECTOR3& va, const LPGLVECTOR3& vb) {
	vout->x = va->y * vb->z - va->z * vb->y;
	vout->y = va->z * vb->x - va->x * vb->z;
	vout->z = va->x * vb->y - va->y * vb->x;
}




// Функция возвращает длинну вектора
inline GLfloat VectorLength(const LPGLVECTOR3& vec) {
	return sqrtf(vec->x * vec->x + vec->y * vec->y + vec->z * vec->z);
}




// Скалярное произведение
inline GLfloat VectorDot(const LPGLVECTOR3& va, const LPGLVECTOR3& vb) {
	return va->x * vb->x + va->y * vb->y + va->z * vb->z;
}




// Функция нормализации вектора
inline void VectorNormalize(LPGLVECTOR3 vout) {
	GLfloat len = VectorLength(vout);
	if(len != 0.0f) {
		vout->x /= len;
		vout->y /= len;
		vout->z /= len;
	}
}



// Функция для расчёта нормали
inline void VectorNormal(LPGLVECTOR3 pn, const LPGLVECTOR3& va, const LPGLVECTOR3& vb, const LPGLVECTOR3& vc) {
	GLVECTOR3 vn1;
	GLVECTOR3 vn2;

	VectorSubtract(&vn1, vb, va);
	VectorSubtract(&vn2, vc, va);
	VectorCross(pn, &vn1, &vn2);
	VectorNormalize(pn);
}




// Функция расчитывает вектор направления(например выстрел производится в сторону куда смотрит камера)
inline void VectorAngle(LPGLVECTOR3 hvec, GLfloat angle_x, GLfloat angle_y) {
	GLfloat radx = fRADIAN(angle_x);
	GLfloat rady = fRADIAN(angle_y);
	GLfloat icos = cosf(rady);
	hvec->x = sinf(radx) * icos;
	hvec->y = sinf(rady);
	hvec->z = cosf(radx) * icos;
}





// Функция перемножения 4-векторa на 4x4 матрицу
inline LPGLVECTOR4 VectorMatrixMultiply(LPGLVECTOR4 v, const GLfloat* mat) {
	GLVECTOR4 vres;
	vres.x = mat[0] * v->x + mat[4] * v->y + mat[8]  * v->z + mat[12] * v->w;
	vres.y = mat[1] * v->x + mat[5] * v->y + mat[9]  * v->z + mat[13] * v->w;
	vres.z = mat[2] * v->x + mat[6] * v->y + mat[10] * v->z + mat[14] * v->w;
	vres.w = mat[3] * v->x + mat[7] * v->y + mat[11] * v->z + mat[15] * v->w;
	*v     = vres;
	return v;
}




// Скалярное произведение вектора на плоскость
inline GLfloat VectorPlaneDot(const LPGLVECTOR3& v, const LPGLPLANE& p) {
	return p->a * v->x + p->b * v->y + p->c * v->z + p->d;
}
inline GLfloat VectorPlaneDot(const LPGLVECTOR4& v, const LPGLPLANE& p) {
	return p->a * v->x + p->b * v->y + p->c * v->z + p->d;
}





// Перевод кватерниона в матрицу 4x4
inline void QuaterionToMatrix(const LPGLQUATERNION& q, GLfloat* mat) {
	*mat++ = 1.0f - 2.0f * (q->y * q->y + q->z * q->z);
	*mat++ = 2.0f * (q->x * q->y + q->z * q->w);
	*mat++ = 2.0f * (q->x * q->z - q->y * q->w);
	*mat++ = 0.0f;

	*mat++ = 2.0f * (q->x * q->y - q->z * q->w);
	*mat++ = 1.0f - 2.0f * (q->x * q->x + q->z * q->z);
	*mat++ = 2.0f * (q->z * q->y + q->x * q->w);
	*mat++ = 0.0f;

	*mat++ = 2.0f * (q->x * q->z + q->y * q->w);
	*mat++ = 2.0f * (q->y * q->z - q->x * q->w);
	*mat++ = 1.0f - 2.0f * (q->x * q->x + q->y * q->y);
	*mat++ = 0.0f;

	*mat++ = 0;
	*mat++ = 0;
	*mat++ = 0;
	*mat++ = 1.0f;
}





// Вращение кватернионом X-Y-Z порядок согласованности в векторе v как по еденичной матрице
inline void QuaternionFromAxisAngle(LPGLQUATERNION q, const LPGLVECTOR3& v, GLfloat d) {
	GLfloat angle = (d / 180.0f) * GL_PI;

	angle *= 0.5f;
	GLfloat isin  = sinf(angle);

	GLVECTOR3 vn(*v);
	VectorNormalize(&vn);

	q->x = vn.x * isin;
	q->y = vn.y * isin;
	q->z = vn.z * isin;
	q->w = cosf(angle);
}



// Функция возвращает скалярное произведение кватерниона(не длинну sqrt)
inline GLfloat QuaternionMagnitudeSeq(const LPGLQUATERNION& q) {
	return q->x * q->x + q->y * q->y + q->x * q->x + q->w * q->w;
}



// Нормализация кватерниона
inline void QuaternionNormalize(LPGLQUATERNION q) {
	const GLfloat range = 0.00001f;
	GLfloat mag;

	mag = QuaternionMagnitudeSeq(q);
	if((ABS(mag - 1.0f) > range) && (ABS(mag) > range)) {  // страхуемся от ошибок деления на нуль
		mag   = sqrtf(mag);
		q->x /= mag;
		q->y /= mag;
		q->z /= mag;
		q->w /= mag;
	}
}





// Перемножения вектора с кватернионом
inline void QuaternionFromVector(LPGLVECTOR3 dst, const LPGLQUATERNION& q, const LPGLVECTOR3& v) {
	GLQUATERNION qa, qb;

	GLVECTOR3 vec(*v);
	VectorNormalize(&vec);
	qa.x = vec.x;
	qa.y = vec.y;
	qa.z = vec.z;
	qa.w = 0.0f;

	qb =  qa   * (-*q);
	qb = (-*q) * qb;

	dst->x = qb.x;
	dst->y = qb.y;
	dst->z = qb.z;
}




// углы Эйлера 
inline void QuaternionPitchYawRoll(LPGLQUATERNION q, GLfloat pitch, GLfloat yaw, GLfloat roll) {
	const GLfloat pi2 = (GL_PI / 180.0f) / 2.0f;

	GLfloat p = pitch * pi2;
	GLfloat y = yaw   * pi2;
	GLfloat r = roll  * pi2;
 
	GLfloat sinp = sinf(p);
	GLfloat siny = sinf(y);
	GLfloat sinr = sinf(r);
	GLfloat cosp = cosf(p);
	GLfloat cosy = cosf(y);
	GLfloat cosr = cosf(r);
 
	q->x = sinr * cosp * cosy - cosr * sinp * siny;  // ось-X
	q->y = cosr * sinp * cosy + sinr * cosp * siny;  // ось-Y
	q->z = cosr * cosp * siny - sinr * sinp * cosy;  // ось-Z
	q->w = cosr * cosp * cosy + sinr * sinp * siny;  
 
	QuaternionNormalize(q); // нормализуем кватернион
}




// Функция для отбрасывания тени на плоскость перемножения вектора направления света с плоскостью
inline void MatrixShadow(GLfloat* mat, const LPGLPLANE& p, const LPGLVECTOR4& v) {
	GLfloat dot = VectorPlaneDot(v, p);

	*mat++ =  dot - v->x * p->a;  // умножениe плоскости на вектор против часовой стрелки GL_CCW
	*mat++ = 0.0f - v->y * p->a;
	*mat++ = 0.0f - v->z * p->a;
	*mat++ = 0.0f - v->w * p->a;

	*mat++ = 0.0f - v->x * p->b;
	*mat++ =  dot - v->y * p->b;
	*mat++ = 0.0f - v->z * p->b;
	*mat++ = 0.0f - v->w * p->b;

	*mat++ = 0.0f - v->x * p->c;
	*mat++ = 0.0f - v->y * p->c;
	*mat++ =  dot - v->z * p->c;
	*mat++ = 0.0f - v->w * p->c;

	*mat++ = 0.0f - v->x * p->d;
	*mat++ = 0.0f - v->y * p->d;
	*mat++ = 0.0f - v->z * p->d;
	*mat++ =  dot - v->w * p->d;
}





// Функция делает матрицу еденичной
inline void MatrixIdentity(GLfloat* mat) {
	GLfloat* end   = mat + 16;
	for(GLfloat* p = mat; p != end; *p++ = 0.0f);
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}



// Функция вращения матрицы по оси-Y
inline void MatrixRotateY(GLfloat* mat, GLfloat degree) {
	GLfloat rad = fRADIAN(degree);
	mat[0] =  mat[10] = cosf(rad);
	mat[8] =  mat[2]  = sinf(rad);
	mat[2] = -mat[2];
}




// Функция вычисления плоскости из трёх точек
inline void PlaneEquation(LPGLPLANE plane, const LPGLVECTOR3 varr) {
	plane->a = varr[0].y * (varr[1].z - varr[2].z) + varr[1].y * (varr[2].z - varr[0].z) + varr[2].y * (varr[0].z - varr[1].z);
	plane->b = varr[0].z * (varr[1].x - varr[2].x) + varr[1].z * (varr[2].x - varr[0].x) + varr[2].z * (varr[0].x - varr[1].x);
	plane->c = varr[0].x * (varr[1].y - varr[2].y) + varr[1].x * (varr[2].y - varr[0].y) + varr[2].x * (varr[0].y - varr[1].y);
	plane->d = -(varr[0].x * (varr[1].y * varr[2].z - varr[2].y * varr[1].z) + varr[1].x * (varr[2].y * varr[0].z - varr[0].y * varr[2].z) + varr[2].x * (varr[0].y * varr[1].z - varr[1].y * varr[0].z));
}




// Функция расчитывает столкновения между сферами
inline bool IsCollisionSphere(const LPGLVECTOR3& va, GLfloat ra, const LPGLVECTOR3& vb, GLfloat rb) {
	GLfloat len = (va->x - vb->x) * (va->x - vb->x) + (va->y - vb->y) * (va->y - vb->y) + (va->z - vb->z) * (va->z - vb->z);
	GLfloat rad = (ra + rb) * (ra + rb);
	if(len < rad)
		return true;
	return false;
}





// Функция проверяет столкновения прямоугольника с точкой не для аффиного случая
inline bool IsCollisionPointRect(const LPGLVECTOR3& pt, const LPGLVECTOR3& v, const LPGLVECTOR3& size) {
	GLVECTOR3 res;
	VectorSubtract(&res, pt, v);
	if((res.x > -size->x) && (res.x < size->x)) { 
		if((res.y > -size->y) && (res.y < size->y)) {
			if((res.z > -size->z) && (res.z < size->z)) 
				return true;
		}
	}
	return false;
}





// Функция проверяет столкновения прямоугольника с прямоугольником в случае столкновения
// не даёт пройти его насквозь не для аффиного случая
inline bool IsRectIntersect(LPGLVECTOR3 va, const LPGLVECTOR3& s1, const LPGLVECTOR3& vb, const LPGLVECTOR3& s2) {
	GLfloat x0, z0, x1, z1;
	GLVECTOR3 res;
	VectorSubtract(&res, va, vb);
	if((ABS(res.x) <= s1->x) && (ABS(res.y) <= s1->y) && (ABS(res.z) <= s1->z)) {

		x0 = vb->x - (va->x - s1->x);
		z0 = vb->z - (va->z - s1->z);
		x1 = (va->x + s1->x) - vb->x;
		z1 = (va->z + s1->z) - vb->z;

		if(x1 < x0)
			x0 = -x1;
		if(z1 < z0)
			z0 = -z1;

		if(ABS(x0) < ABS(z0))
			va->x = va->x + x0;
		else if(ABS(x0) > ABS(z0))
			va->z = va->z + z0;
		else {
			va->x = va->x + x0;
			va->z = va->z + z0;
		}
		return true;
	}
	return false;
}





// Функция осуществляющея движение по прямолинейнной траектории принимая скаляр как позиция пути
inline  void  LineP2(LPGLVECTOR3 vout, GLfloat t, const LPGLVECTOR3& first, const LPGLVECTOR3& last) {
	*vout = (*last - *first) * t + *first;
}