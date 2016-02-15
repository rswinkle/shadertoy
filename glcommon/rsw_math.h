#pragma once
#ifndef RMATH_H
#define RMATH_H

#include <iostream>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <stdint.h>


#define RM_PI (3.14159265358979323846)
#define RM_2PI (2.0 * RM_PI)
#define PI_DIV_180 (0.017453292519943296)
#define INV_PI_DIV_180 (57.2957795130823229)

#define DEG_TO_RAD(x)	((x)*PI_DIV_180)
#define RAD_TO_DEG(x)	((x)*INV_PI_DIV_180)

// Hour angles
#define HR_TO_DEG(x)	((x) * (1.0 / 15.0))
#define HR_TO_RAD(x)	DEG_TO_RAD(HR_TO_DEG(x))

#define DEG_TO_HR(x)	((x) * 15.0))
#define RAD_TO_HR(x)	DEG_TO_HR(RAD_TO_DEG(x))


#define MAX(a, b)  ((a) > (b)) ? (a) : (b)
#define MIN(a, b)  ((a) < (b)) ? (a) : (b)

typedef unsigned char byte;
typedef uint32_t dword;


namespace rsw
{

inline float rand_float(float min, float max)
{
	return (float(rand())/float(RAND_MAX-1))*(max-min) + min;
}

/*
 * vectors
 *
 *
 *  vector operators:
 * 		A * B = dot product
 * 		A - B = vector from B to A
 * 		A + B = add vectors together
 *  float operators
 * 		A * b = A scaled by b
 * 		A / b = A scaled by b (made smaller I know I could just use A * (1/b))
 */

struct vec2
{
	float x;
	float y;


	vec2(float x=0, float y=0) : x(x), y(y) {}
	float len() { return sqrt((*this)*(*this)); }
	vec2 norm() { float l = this->len(); return vec2(x/l, y/l); }
	void normalize() { (*this)/=this->len(); }

	float operator*(vec2 b) {	return x*b.x + y*b.y; }

	vec2& operator -=(vec2 a) { x -= a.x; y -= a.y; return *this; }
	vec2& operator +=(vec2 a) { x += a.x; y += a.y; return *this; }
	vec2& operator *=(float a) { x *= a; y *= a; return *this; }
	vec2& operator /=(float a) { x /= a; y /= a; return *this; }
};


inline vec2 operator+(vec2 a, vec2 b) {	return vec2(a.x+b.x, a.y+b.y); }
inline vec2 operator-(vec2 a, vec2 b) {	return vec2(a.x-b.x, a.y-b.y); }
inline vec2 operator-(vec2 a) {			return vec2(-a.x, -a.y); }

inline vec2 operator*(vec2 a, float b) { return a*=b; }
inline vec2 operator*(float b, vec2 a) { return a*=b; }
inline vec2 operator/(vec2 a, float b) { return a/=b; }


inline std::ostream& operator<<(std::ostream& stream, const vec2& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<")";
}

inline bool operator==(const vec2& a, const vec2& b)
{
	return ((a.x==b.x) && (a.y==b.y));
}

inline bool eql_epsilon(vec2 a, vec2 b, float epsilon)
{
	return ((fabs(a.x-b.x)<epsilon) && (fabs(a.y-b.y)<epsilon));
}




/**********************************************************/

struct vec3
{
	union {
		struct {
			float x,y,z;
		};
		float pts[3];
	};

	vec3(float x=0, float y=0, float z=0) : x(x), y(y), z(z) {}
	float len() { return sqrt((*this)*(*this)); }
	vec3 norm() { float l = this->len(); return vec3(x/l, y/l, z/l); }
	void normalize() { (*this)/=this->len(); }

	float operator*(const vec3 b) const {	return x*b.x + y*b.y + z*b.z; }

	vec3& operator -=(vec3 a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
	vec3& operator +=(vec3 a) { x += a.x; y += a.y; z += a.z; return *this; }
	vec3& operator *=(float a) { x *= a; y *= a; z *= a; return *this; }
	vec3& operator /=(float a) { x /= a; y /= a; z /= a; return *this; }

	//swizzles
	vec2 xx() { return vec2(x,x); }
	vec2 xy() { return vec2(x,y); }
	vec2 xz() { return vec2(x,z); }
	vec2 yx() { return vec2(y,x); }
	vec2 yy() { return vec2(y,y); }
	vec2 yz() { return vec2(y,z); }
	vec2 zx() { return vec2(z,x); }
	vec2 zy() { return vec2(z,y); }
	vec2 zz() { return vec2(z,z); }

	vec3 xxx() { return vec3(x,x,x); }
	vec3 xxy() { return vec3(x,x,y); }
	vec3 xxz() { return vec3(x,x,z); }
	vec3 xyx() { return vec3(x,y,x); }
	vec3 xyy() { return vec3(x,y,y); }
	vec3 xyz() { return *this; }
	vec3 xzx() { return vec3(x,z,x); }
	vec3 xzy() { return vec3(x,z,y); }
	vec3 xzz() { return vec3(x,z,z); }

	vec3 yxx() { return vec3(y,x,x); }
	vec3 yxy() { return vec3(y,x,y); }
	vec3 yxz() { return vec3(y,x,z); }
	vec3 yyx() { return vec3(y,y,x); }
	vec3 yyy() { return vec3(y,y,y); }
	vec3 yyz() { return vec3(y,y,z); }
	vec3 yzx() { return vec3(y,z,x); }
	vec3 yzy() { return vec3(y,z,y); }
	vec3 yzz() { return vec3(y,z,z); }

	vec3 zxx() { return vec3(z,x,x); }
	vec3 zxy() { return vec3(z,x,y); }
	vec3 zxz() { return vec3(z,x,z); }
	vec3 zyx() { return vec3(z,y,x); }
	vec3 zyy() { return vec3(z,y,y); }
	vec3 zyz() { return vec3(z,y,z); }
	vec3 zzx() { return vec3(z,z,x); }
	vec3 zzy() { return vec3(z,z,y); }
	vec3 zzz() { return vec3(z,z,z); }
};



inline vec3 operator+(vec3 a, vec3 b) {	return vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline vec3 operator-(vec3 a, vec3 b) {	return vec3(a.x-b.x, a.y-b.y, a.z-b.z); }
//inline float operator*(vec3 a, vec3 b) {	return a.x*b.x + a.y*b.y + a.z*b.z; }
inline vec3 operator-(vec3 a) {	return vec3(-a.x, -a.y, -a.z); }

inline vec3 operator*(vec3 a, float b) { return a*=b; }
inline vec3 operator*(float b, vec3 a) { return a*=b; }
inline vec3 operator/(vec3 a, float b) { return a/=b; }


inline std::ostream& operator<<(std::ostream& stream, const vec3& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<")";
}

inline std::istream& operator>>(std::istream& stream, vec3& a)
{
	//no error checking for now
	//ignore junk ie whitespace till open paren, 1000 is arbitrary
	//but probably adequate
	stream.ignore(1000, '(');
	stream >> a.x;
	stream.get();
	stream >> a.y;
	stream.get();
	stream >> a.z;
	stream.get();
	return stream;
}

inline bool operator==(const vec3& a, const vec3& b)
{
	return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z));
}

inline bool operator!=(const vec3& a, const vec3& b)
{
	return ((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z));
}


inline bool eql_epsilon(vec3 a, vec3 b, float epsilon)
{
	return ((fabs(a.x-b.x)<epsilon) && (fabs(a.y-b.y)<epsilon) && (fabs(a.z-b.z)<epsilon));
}

inline vec3 cross_product(const vec3 u, const vec3 v)
{
	vec3 result;
	result.x = u.y*v.z - v.y*u.z;
	result.y = -u.x*v.z + v.x*u.z;
	result.z = u.x*v.y - v.x*u.y;
	return result;
}

inline float angle_between_vec3(const vec3 u, const vec3 v)
{
	return acos(u * v);
}



/**********************************************************/
struct vec4
{
	union {
		struct {
			float x,y,z,w;
		};
		float pts[4];
	};

	vec4(float x=0, float y=0, float z=0, float w=1) : x(x), y(y), z(z), w(w) {}
	vec4(vec3 a) : x(a.x), y(a.y), z(a.z), w(1) {}
	vec3 tovec3() { return vec3(x, y, z); }
	vec2 tovec2() { return vec2(x, y); }
	vec3 vec3h() { return vec3(x/w, y/w, z/w); }
	vec2 vec2h() { return vec2(x/w, y/w); }
//
//	vec3& norm();
//	float len();
//
//	vec3& operator-=(vec3);
//	vec3& operator+=(vec3);
//	vec3& operator*=(float);
//	vec3& operator/=(float);\

	//swizzles
	vec2 xx() { return vec2(x,x); }
	vec2 xy() { return vec2(x,y); }
	vec2 xz() { return vec2(x,z); }
	vec2 yx() { return vec2(y,x); }
	vec2 yy() { return vec2(y,y); }
	vec2 yz() { return vec2(y,z); }
	vec2 zx() { return vec2(z,x); }
	vec2 zy() { return vec2(z,y); }
	vec2 zz() { return vec2(z,z); }

	vec3 xxx() { return vec3(x,x,x); }
	vec3 xxy() { return vec3(x,x,y); }
	vec3 xxz() { return vec3(x,x,z); }
	vec3 xyx() { return vec3(x,y,x); }
	vec3 xyy() { return vec3(x,y,y); }
	vec3 xyz() { return vec3(x,y,z); }
	vec3 xzx() { return vec3(x,z,x); }
	vec3 xzy() { return vec3(x,z,y); }
	vec3 xzz() { return vec3(x,z,z); }

	vec3 yxx() { return vec3(y,x,x); }
	vec3 yxy() { return vec3(y,x,y); }
	vec3 yxz() { return vec3(y,x,z); }
	vec3 yyx() { return vec3(y,y,x); }
	vec3 yyy() { return vec3(y,y,y); }
	vec3 yyz() { return vec3(y,y,z); }
	vec3 yzx() { return vec3(y,z,x); }
	vec3 yzy() { return vec3(y,z,y); }
	vec3 yzz() { return vec3(y,z,z); }

	vec3 zxx() { return vec3(z,x,x); }
	vec3 zxy() { return vec3(z,x,y); }
	vec3 zxz() { return vec3(z,x,z); }
	vec3 zyx() { return vec3(z,y,x); }
	vec3 zyy() { return vec3(z,y,y); }
	vec3 zyz() { return vec3(z,y,z); }
	vec3 zzx() { return vec3(z,z,x); }
	vec3 zzy() { return vec3(z,z,y); }
	vec3 zzz() { return vec3(z,z,z); }

};


//I think this is probably what I'll most want, just ignoring the homogeneous w
inline vec4 operator+(vec4 a, vec4 b) {	return vec4(a.x+b.x, a.y+b.y, a.z+b.z); }
inline vec4 operator-(vec4 a, vec4 b) {	return vec4(a.x-b.x, a.y-b.y, a.z-b.z); }
inline vec4 operator+(vec4 a, vec3 b) {	return vec4(a.x+b.x, a.y+b.y, a.z+b.z); }
inline vec4 operator-(vec4 a, vec3 b) {	return vec4(a.x-b.x, a.y-b.y, a.z-b.z); }

//inline float operator*(vec4 a, vec4 b) {	return a.x*b.x + a.y*b.y + a.z*b.z; }
//inline vec4 operator-(vec4 a) {	return vec4(-a.x, -a.y, -a.z); }

//inline vec4 operator*(vec4 a, float b) { return a*=b; }
//inline vec4 operator*(float b, vec4 a) { return a*=b; }
//inline vec4 operator/(vec4 a, float b) { return a/=b; }

inline std::ostream& operator<<(std::ostream& stream, const vec4& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<", "<<a.w<<")";
}

inline float operator*(vec4 a, vec4 b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

inline bool operator==(const vec4& a, const vec4& b)
{
	return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w));
}

inline bool operator!=(const vec4& a, const vec4& b)
{
	return ((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w));
}


inline bool eql_epsilon(vec4 a, vec4 b, float epsilon)
{
	return ((fabs(a.x-b.x)<epsilon) && (fabs(a.y-b.y)<epsilon) && (fabs(a.z-b.z)<epsilon) && (fabs(a.w-b.w)<epsilon));
}



/*
 * TODO
 * cross product functions
 * get angle between vectors
 * normalize
 */

/*******************************************/

struct dvec2
{
	double x;
	double y;


	dvec2(double x=0, double y=0) : x(x), y(y) {}
	double len() { return sqrt((*this)*(*this)); }
	dvec2 norm() { double l = this->len(); return dvec2(x/l, y/l); }
	void normalize() { (*this)/=this->len(); }

	double operator*(dvec2 b) {	return x*b.x + y*b.y; }

	dvec2& operator -=(dvec2 a) { x -= a.x; y -= a.y; return *this; }
	dvec2& operator +=(dvec2 a) { x += a.x; y += a.y; return *this; }
	dvec2& operator *=(double a) { x *= a; y *= a; return *this; }
	dvec2& operator /=(double a) { x /= a; y /= a; return *this; }
};


inline dvec2 operator+(dvec2 a, dvec2 b) {	return dvec2(a.x+b.x, a.y+b.y); }
inline dvec2 operator-(dvec2 a, dvec2 b) {	return dvec2(a.x-b.x, a.y-b.y); }
inline dvec2 operator-(dvec2 a) {			return dvec2(-a.x, -a.y); }

inline dvec2 operator*(dvec2 a, double b) { return a*=b; }
inline dvec2 operator*(double b, dvec2 a) { return a*=b; }
inline dvec2 operator/(dvec2 a, double b) { return a/=b; }


inline std::ostream& operator<<(std::ostream& stream, const dvec2& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<")";
}

inline bool operator==(const dvec2& a, const dvec2& b)
{
	return ((a.x==b.x) && (a.y==b.y));
}

inline bool eql_epsilon(dvec2 a, dvec2 b, double epsilon)
{
	return ((fabs(a.x-b.x)<epsilon) && (fabs(a.y-b.y)<epsilon));
}




/**********************************************************/
struct dvec4;

struct dvec3
{
	union {
		struct {
			double x,y,z;
		};
		double pts[3];
	};

	dvec3(double x=0, double y=0, double z=0) : x(x), y(y), z(z) {}
	double len() { return sqrt((*this)*(*this)); }
	dvec3 norm() { double l = this->len(); return dvec3(x/l, y/l, z/l); }
	void normalize() { (*this)/=this->len(); }

	double operator*(const dvec3 b) const {	return x*b.x + y*b.y + z*b.z; }

	dvec3& operator -=(dvec3 a) { x -= a.x; y -= a.y; z -= a.z; return *this; }
	dvec3& operator +=(dvec3 a) { x += a.x; y += a.y; z += a.z; return *this; }
	dvec3& operator *=(double a) { x *= a; y *= a; z *= a; return *this; }
	dvec3& operator /=(double a) { x /= a; y /= a; z /= a; return *this; }

	//swizzles
	dvec2 xy() { return dvec2(x,y); }
	dvec2 xz() { return dvec2(x,z); }
	dvec2 yx() { return dvec2(y,x); }
	dvec2 yy() { return dvec2(y,y); }
	dvec2 yz() { return dvec2(y,z); }
	dvec2 zx() { return dvec2(z,x); }
	dvec2 zy() { return dvec2(z,y); }
	dvec2 zz() { return dvec2(z,z); }

	dvec3 xxx() { return dvec3(x,x,x); }
	dvec3 xxy() { return dvec3(x,x,y); }
	dvec3 xxz() { return dvec3(x,x,z); }
	dvec3 xyx() { return dvec3(x,y,x); }
	dvec3 xyy() { return dvec3(x,y,y); }
	dvec3 xyz() { return *this; }
	dvec3 xzx() { return dvec3(x,z,x); }
	dvec3 xzy() { return dvec3(x,z,y); }
	dvec3 xzz() { return dvec3(x,z,z); }

	dvec3 yxx() { return dvec3(y,x,x); }
	dvec3 yxy() { return dvec3(y,x,y); }
	dvec3 yxz() { return dvec3(y,x,z); }
	dvec3 yyx() { return dvec3(y,y,x); }
	dvec3 yyy() { return dvec3(y,y,y); }
	dvec3 yyz() { return dvec3(y,y,z); }
	dvec3 yzx() { return dvec3(y,z,x); }
	dvec3 yzy() { return dvec3(y,z,y); }
	dvec3 yzz() { return dvec3(y,z,z); }

	dvec3 zxx() { return dvec3(z,x,x); }
	dvec3 zxy() { return dvec3(z,x,y); }
	dvec3 zxz() { return dvec3(z,x,z); }
	dvec3 zyx() { return dvec3(z,y,x); }
	dvec3 zyy() { return dvec3(z,y,y); }
	dvec3 zyz() { return dvec3(z,y,z); }
	dvec3 zzx() { return dvec3(z,z,x); }
	dvec3 zzy() { return dvec3(z,z,y); }
	dvec3 zzz() { return dvec3(z,z,z); }
};



inline dvec3 operator+(dvec3 a, dvec3 b) {	return dvec3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline dvec3 operator-(dvec3 a, dvec3 b) {	return dvec3(a.x-b.x, a.y-b.y, a.z-b.z); }
//inline double operator*(dvec3 a, dvec3 b) {	return a.x*b.x + a.y*b.y + a.z*b.z; }
inline dvec3 operator-(dvec3 a) {	return dvec3(-a.x, -a.y, -a.z); }

inline dvec3 operator*(dvec3 a, double b) { return a*=b; }
inline dvec3 operator*(double b, dvec3 a) { return a*=b; }
inline dvec3 operator/(dvec3 a, double b) { return a/=b; }


inline std::ostream& operator<<(std::ostream& stream, const dvec3& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<")";
}

inline std::istream& operator>>(std::istream& stream, dvec3& a)
{
	//no error checking for now
	//ignore junk ie whitespace till open paren, 1000 is arbitrary
	//but probably adequate
	stream.ignore(1000, '(');
	stream >> a.x;
	stream.get();
	stream >> a.y;
	stream.get();
	stream >> a.z;
	stream.get();
	return stream;
}

inline bool operator==(const dvec3& a, const dvec3& b)
{
	return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z));
}

inline bool operator!=(const dvec3& a, const dvec3& b)
{
	return ((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z));
}


inline bool eql_epsilon(dvec3 a, dvec3 b, double epsilon)
{
	return ((fabs(a.x-b.x)<epsilon) && (fabs(a.y-b.y)<epsilon) && (fabs(a.z-b.z)<epsilon));
}

inline dvec3 cross_product(const dvec3 u, const dvec3 v)
{
	dvec3 result;
	result.x = u.y*v.z - v.y*u.z;
	result.y = -u.x*v.z + v.x*u.z;
	result.z = u.x*v.y - v.x*u.y;
	return result;
}

inline double angle_between_dvec3(const dvec3 u, const dvec3 v)
{
	return acos(u * v);
}



/**********************************************************/
struct dvec4
{
	union {
		struct {
			double x,y,z,w;
		};
		double pts[4];
	};

	dvec4(double x=0, double y=0, double z=0, double w=1) : x(x), y(y), z(z), w(w) {}
	dvec4(dvec3 a) : x(a.x), y(a.y), z(a.z), w(1) {}
	dvec3 todvec3() { return dvec3(x, y, z); }
	dvec2 todvec2() { return dvec2(x, y); }
	dvec3 dvec3h() { return dvec3(x/w, y/w, z/w); }
	dvec2 dvec2h() { return dvec2(x/w, y/w); }
//
//	dvec3& norm();
//	double len();
//
//	dvec3& operator-=(dvec3);
//	dvec3& operator+=(dvec3);
//	dvec3& operator*=(double);
//	dvec3& operator/=(double);\

	//swizzles
	dvec2 xx() { return dvec2(x,x); }
	dvec2 xy() { return dvec2(x,y); }
	dvec2 xz() { return dvec2(x,z); }
	dvec2 yx() { return dvec2(y,x); }
	dvec2 yy() { return dvec2(y,y); }
	dvec2 yz() { return dvec2(y,z); }
	dvec2 zx() { return dvec2(z,x); }
	dvec2 zy() { return dvec2(z,y); }
	dvec2 zz() { return dvec2(z,z); }

	dvec3 xxx() { return dvec3(x,x,x); }
	dvec3 xxy() { return dvec3(x,x,y); }
	dvec3 xxz() { return dvec3(x,x,z); }
	dvec3 xyx() { return dvec3(x,y,x); }
	dvec3 xyy() { return dvec3(x,y,y); }
	dvec3 xyz() { return dvec3(x,y,z); }
	dvec3 xzx() { return dvec3(x,z,x); }
	dvec3 xzy() { return dvec3(x,z,y); }
	dvec3 xzz() { return dvec3(x,z,z); }

	dvec3 yxx() { return dvec3(y,x,x); }
	dvec3 yxy() { return dvec3(y,x,y); }
	dvec3 yxz() { return dvec3(y,x,z); }
	dvec3 yyx() { return dvec3(y,y,x); }
	dvec3 yyy() { return dvec3(y,y,y); }
	dvec3 yyz() { return dvec3(y,y,z); }
	dvec3 yzx() { return dvec3(y,z,x); }
	dvec3 yzy() { return dvec3(y,z,y); }
	dvec3 yzz() { return dvec3(y,z,z); }

	dvec3 zxx() { return dvec3(z,x,x); }
	dvec3 zxy() { return dvec3(z,x,y); }
	dvec3 zxz() { return dvec3(z,x,z); }
	dvec3 zyx() { return dvec3(z,y,x); }
	dvec3 zyy() { return dvec3(z,y,y); }
	dvec3 zyz() { return dvec3(z,y,z); }
	dvec3 zzx() { return dvec3(z,z,x); }
	dvec3 zzy() { return dvec3(z,z,y); }
	dvec3 zzz() { return dvec3(z,z,z); }

};

//I can define these functions later but I can effectively do the same thing
//by just calling vec3(a.x, a.y, a.z) when a is dvec4

//float operator*(dvec4 a, vec3 b);

inline std::ostream& operator<<(std::ostream& stream, const dvec4& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<", "<<a.w<<")";
}

inline float operator*(dvec4 a, dvec4 b) { return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

inline bool operator==(const dvec4& a, const dvec4& b)
{
	return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w));
}

inline bool operator!=(const dvec4& a, const dvec4& b)
{
	return ((a.x!=b.x) || (a.y!=b.y) || (a.z!=b.z) || (a.w!=b.w));
}


inline bool eql_epsilon(dvec4 a, dvec4 b, float epsilon)
{
	return ((fabs(a.x-b.x)<epsilon) && (fabs(a.y-b.y)<epsilon) && (fabs(a.z-b.z)<epsilon) && (fabs(a.w-b.w)<epsilon));
}












struct ivec3;


/**********************************************************/
struct ivec2
{
	public:
	union {
		struct {
			int x,y;
		};
		int pts[2];
	};

	ivec2(int x=0, int y=0) : x(x), y(y) {}


	ivec2& operator+=(ivec2 a) { x += a.x; y += a.y; return *this; }
	ivec2& operator*=(int a) { x *= a; y *= a; return *this; }
	int operator[](int i) { return pts[i]; }

};

inline ivec2 operator+(ivec2 a, ivec2 b) { return ivec2(a.x+b.x, a.y+b.y); }
inline ivec2 operator-(ivec2 a) { return ivec2(-a.x, -a.y); }

inline std::ostream& operator<<(std::ostream& stream, const ivec2 a)
{
	return stream <<"("<<a.x<<", "<<a.y<<")";
}

inline bool operator==(const ivec2& a, const ivec2& b) { return ((a.x==b.x) && (a.y==b.y)); }



/**********************************************************/
struct ivec3
{
	public:
	union {
		struct {
			int x,y,z;
		};
		int pts[3];
	};

	ivec3(int x=0, int y=0, int z=0) : x(x), y(y), z(z) {}


	ivec3& operator+=(ivec3 a) { x += a.x; y += a.y; z += a.z; return *this; }
	ivec3& operator*=(int a) { x *= a; y *= a; z *= a; return *this; }
	int operator[](int i) { return pts[i]; }
};

inline ivec3 operator+(ivec3 a, ivec3 b) { return ivec3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline ivec3 operator-(ivec3 a) { return ivec3(-a.x, -a.y, -a.z); }

inline std::ostream& operator<<(std::ostream& stream, const ivec3& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<")";
}

inline bool operator==(const ivec3& a, const ivec3& b) { return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z)); }

/***********************************************************/
struct ivec4
{
	public:
	union {
		struct {
			int x,y,z,w;
		};
		int pts[4];
	};

	ivec4(int x=0, int y=0, int z=0, int w=1) : x(x), y(y), z(z), w(w) {}


	ivec4& operator+=(ivec4 a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
	ivec4& operator*=(int a) { x *= a; y *= a; z *= a; w *= a; return *this; }
	int operator[](int i) { return pts[i]; }
};

inline ivec4 operator+(ivec4 a, ivec4 b) { return ivec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
inline ivec4 operator-(ivec4 a) { return ivec4(-a.x, -a.y, -a.z, -a.w); }

inline std::ostream& operator<<(std::ostream& stream, const ivec4& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<", "<<a.w<<")";
}

inline bool operator==(const ivec4& a, const ivec4& b) { return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)); }



/**********************************************************/
struct uvec2
{
	public:
	union {
		struct {
			unsigned int x,y;
		};
		unsigned int pts[2];
	};

	uvec2(unsigned int x=0, unsigned int y=0) : x(x), y(y) {}

	uvec2& operator+=(uvec2 a) { x += a.x; y += a.y; return *this; }
	uvec2& operator*=(unsigned int a) { x *= a; y *= a; return *this; }
	unsigned int operator[](int i) { return pts[i]; }
};

inline uvec2 operator+(uvec2 a, uvec2 b) { return uvec2(a.x+b.x, a.y+b.y); }
inline uvec2 operator-(uvec2 a) { return uvec2(-a.x, -a.y); }

inline std::ostream& operator<<(std::ostream& stream, const uvec2& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<")";
}

inline bool operator==(const uvec2& a, const uvec2& b) { return ((a.x==b.x) && (a.y==b.y)); }

/**********************************************************/
struct uvec3
{
	public:
	union {
		struct {
			int x,y,z;
		};
		unsigned int pts[3];
	};

	uvec3(unsigned int x=0, unsigned int y=0, unsigned int z=0) : x(x), y(y), z(z) {}

	uvec3& operator+=(uvec3 a) { x += a.x; y += a.y; z += a.z; return *this; }
	uvec3& operator*=(unsigned int a) { x *= a; y *= a; z *= a; return *this; }
	unsigned int operator[](int i) { return pts[i]; }

};

inline uvec3 operator+(uvec3 a, uvec3 b) { return uvec3(a.x+b.x, a.y+b.y, a.z+b.z); }
inline uvec3 operator-(uvec3 a) { return uvec3(-a.x, -a.y, -a.z); }

inline std::ostream& operator<<(std::ostream& stream, const uvec3& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<")";
}

inline bool operator==(const uvec3& a, const uvec3& b) { return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z)); }



/**********************************************************/
struct uvec4
{
	public:
	union {
		struct {
			unsigned int x,y,z,w;
		};
		unsigned int pts[4];
	};

	uvec4(unsigned int x=0, unsigned int y=0, unsigned int z=0, unsigned int w=1) : x(x), y(y), z(z), w(w) {}

	uvec4& operator+=(uvec4 a) { x += a.x; y += a.y; z += a.z; w += a.w; return *this; }
	uvec4& operator*=(unsigned int a) { x *= a; y *= a; z *= a; w *= a; return *this; }
	unsigned int operator[](int i) { return pts[i]; }

};

inline uvec4 operator+(uvec4 a, uvec4 b) { return uvec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
inline uvec4 operator-(uvec4 a) { return uvec4(-a.x, -a.y, -a.z, -a.w); }

inline std::ostream& operator<<(std::ostream& stream, const uvec4& a)
{
	return stream <<"("<<a.x<<", "<<a.y<<", "<<a.z<<", "<<a.w<<")";
}

inline bool operator==(const uvec4& a, const uvec4& b) { return ((a.x==b.x) && (a.y==b.y) && (a.z==b.z) && (a.w==b.w)); }










/*
 *
 **********************************************************
 * Matrices
 */

struct mat3
{
	float matrix[9];

	mat3()
	{
		memset(matrix, 0, sizeof(float)*9);
		matrix[0] = 1;
		matrix[4] = 1;
		matrix[8] = 1;
	}

	mat3(float array[]) {	memcpy(matrix, array, sizeof(float)*9); }

	void set(float array[]) {	memcpy(matrix, array, sizeof(float)*9); }

	float& operator [](size_t i) { return matrix[i]; }

	//Getters for rows and columns
	vec3 x() const { return vec3(matrix[0], matrix[1], matrix[2]); }
	vec3 y() const { return vec3(matrix[3], matrix[4], matrix[5]); }
	vec3 z() const { return vec3(matrix[6], matrix[7], matrix[8]); }
	vec3 c1() const { return vec3(matrix[0], matrix[3], matrix[6]); }
	vec3 c2() const { return vec3(matrix[1], matrix[4], matrix[7]); }
	vec3 c3() const { return vec3(matrix[2], matrix[5], matrix[8]); }

	//sets 4th row to 0 0 0 1
	void setc1(vec3 v) { matrix[0]=v.x, matrix[3]=v.y, matrix[6]=v.z; }
	void setc2(vec3 v) { matrix[1]=v.x, matrix[4]=v.y, matrix[7]=v.z; }
	void setc3(vec3 v) { matrix[2]=v.x, matrix[5]=v.y, matrix[8]=v.z; }

	//sets 4th column to 0 0 0 1
	void setx(vec3 v) { matrix[0]=v.x, matrix[1]=v.y, matrix[2]=v.z; }
	void sety(vec3 v) { matrix[3]=v.x, matrix[4]=v.y, matrix[5]=v.z; }
	void setz(vec3 v) { matrix[6]=v.x, matrix[7]=v.y, matrix[8]=v.z; }

};


inline vec3 operator*(const mat3& mat, const vec3& vec)
{
	vec3 tmp;
	tmp.x = mat.x()*vec;
	tmp.y = mat.y()*vec;
	tmp.z = mat.z()*vec;
	return tmp;
}

inline std::ostream& operator<<(std::ostream& stream, const mat3& mat)
{
	return stream <<"["<<mat.x()<<"\n"<<mat.y()<<"\n"<<mat.z()<<"]";
}

//implemented inin rmath.cpp
mat3 operator*(const mat3& a, const mat3& b);
void load_rotation_mat3(mat3& mat, vec3 v, float angle);






/**********************************************************/

struct mat4
{
	float matrix[16];

	mat4()
	{
		memset(matrix, 0, sizeof(float)*16);
		matrix[0] = 1;
		matrix[5] = 1;
		matrix[10] = 1;
		matrix[15] = 1;
	}

	mat4(float array[]) { memcpy(matrix, array, sizeof(float)*16); }

	void set(float array[]) { memcpy(matrix, array, sizeof(float)*16); }
//	void set(vec3 x, vec3 y, vec3 z);

	float& operator[](size_t i) { return matrix[i]; }

	vec4 c1() const { return vec4(matrix[0], matrix[4], matrix[8], matrix[12]); }
	vec4 c2() const { return vec4(matrix[1], matrix[5], matrix[9], matrix[13]); }
	vec4 c3() const { return vec4(matrix[2], matrix[6], matrix[10], matrix[14]); }
	vec4 c4() const { return vec4(matrix[3], matrix[7], matrix[11], matrix[15]); }

	vec4 x() const { return vec4(matrix[0], matrix[1], matrix[2], matrix[3]); }
	vec4 y() const { return vec4(matrix[4], matrix[5], matrix[6], matrix[7]); }
	vec4 z() const { return vec4(matrix[8], matrix[9], matrix[10], matrix[11]); }
	vec4 w() const { return vec4(matrix[12], matrix[13], matrix[14], matrix[15]); }
//
//	vec4 x4() const;
//	vec4 y4() const;
//	vec4 z4() const;
//	vec4 w4() const;

	//sets 4th row to 0 0 0 1
	void setc1(vec3 v) { matrix[0]=v.x, matrix[4]=v.y, matrix[8]=v.z, matrix[12]=0; }
	void setc2(vec3 v) { matrix[1]=v.x, matrix[5]=v.y, matrix[9]=v.z, matrix[13]=0; }
	void setc3(vec3 v) { matrix[2]=v.x, matrix[6]=v.y, matrix[10]=v.z, matrix[14]=0; }
	void setc4(vec3 v) { matrix[3]=v.x, matrix[7]=v.y, matrix[11]=v.z, matrix[15]=1; }

	//sets 4th column to 0 0 0 1
	void setx(vec3 v) { matrix[0]=v.x, matrix[1]=v.y, matrix[2]=v.z, matrix[3]=0; }
	void sety(vec3 v) { matrix[4]=v.x, matrix[5]=v.y, matrix[6]=v.z, matrix[7]=0; }
	void setz(vec3 v) { matrix[8]=v.x, matrix[9]=v.y, matrix[10]=v.z, matrix[11]=0; }
	void setw(vec3 v) { matrix[12]=v.x, matrix[13]=v.y, matrix[14]=v.z, matrix[15]=1; }



};

// is a function in a header file automatically inlined?
inline vec3 operator*(const mat4& mat, const vec3& vec)
{
	vec4 tmp4(vec);
	return vec3(mat.x()*tmp4, mat.y()*tmp4, mat.z()*tmp4);
}

inline vec4 operator*(const mat4& mat, const vec4& vec)
{
	//vec4 tmp4(vec);
	return vec4(mat.x()*vec, mat.y()*vec, mat.z()*vec, mat.w()*vec);
}

inline std::ostream& operator<<(std::ostream& stream, const mat4& mat)
{
	return stream <<"["<<mat.x()<<"\n"<<mat.y()<<"\n"<<mat.z()<<"\n"<<mat.w()<<"]";
}


//implemented in rmath.cpp
mat4 operator*(const mat4& a, const mat4& b);
void load_rotation_mat4(mat4& mat, vec3 vec, float angle);


mat4 invert_mat4(const mat4& m);





/**********************************************************/
void make_perspective_matrix(mat4 &the_matrix, float fFov, float fAspect, float near, float far);
void make_pers_matrix(mat4& mat, float z_near, float z_far);

void make_perspective_proj_matrix(mat4 &mat, float left, float right, float bottom, float top, float near, float far);

void make_orthographic_matrix(mat4 &mProjection, float left, float right, float bottom, float top, float near, float far);

void make_viewport_matrix(mat4 &the_matrix, int x, int y, unsigned int width, unsigned int height, int opengl=1);






///////////Matrix transformation functions
inline mat3 scale_mat3(float xScale, float yScale, float zScale)
	{ mat3 m; m.matrix[0] = xScale; m.matrix[4] = yScale; m.matrix[8] = zScale; return m; }

inline mat3 scale_mat3(const vec3 vScale)
	{ mat3 m; m.matrix[0] = vScale.x; m.matrix[4] = vScale.y; m.matrix[8] = vScale.z; return m; }

inline mat4 scale_mat4(float xScale, float yScale, float zScale)
	{ mat4 m; m.matrix[0] = xScale; m.matrix[5] = yScale; m.matrix[10] = zScale; return m; }

inline mat4 scale_mat4(const vec3 vScale)
	{ mat4 m; m.matrix[0] = vScale.x; m.matrix[5] = vScale.y; m.matrix[10] = vScale.z; return m; }

// Create a Translation matrix. Only 4x4 matrices have translation components
inline mat4 translation_mat4(float x, float y, float z)
	{ mat4 m; m.matrix[3] = x; m.matrix[7] = y; m.matrix[11] = z; return m; }

inline mat4 translation_mat4(const vec3 Scale)
	{ mat4 m; m.matrix[3] = Scale.x; m.matrix[7] = Scale.y; m.matrix[11] = Scale.z; return m; }








///////////////////////////////////////////////////////////////////////////////
// Extract a rotation matrix from a 4x4 matrix
// Extracts the rotation matrix (3x3) from a 4x4 matrix
inline mat3 extract_rotation_mat4(mat4 src, bool normalize=false)
{
	mat3 dst;

	if (normalize) {
		dst.setc1(src.c1().tovec3().norm());
		dst.setc2(src.c2().tovec3().norm());
		dst.setc3(src.c3().tovec3().norm());
	} else {
		dst.setc1(src.c1().tovec3());
		dst.setc2(src.c2().tovec3());
		dst.setc3(src.c3().tovec3());
	}
	return dst;
}







/*
 *
 * MISC functions
 */
// Returns the same number if it is a power of
// two. Returns a larger integer if it is not a
// power of two. The larger integer is the next
// highest power of two.
inline unsigned int m3dIsPOW2(unsigned int iValue)
{
	unsigned int nPow2 = 1;

	while(iValue > nPow2)
		nPow2 = (nPow2 << 1);

	return nPow2;
}



struct Color
{

	Color()
	{
		r = g = b = 0;
		a = 255;
	}
	Color(byte red, byte green, byte blue, byte alpha=255) : r(red), g(green), b(blue), a(alpha) {}

	byte r;
	byte g;
	byte b;
	byte a;
};

inline Color vec4_to_Color(vec4 v)
{
	Color c;
	//assume all in the range of [0, 1]
	c.r = v.x * 255;
	c.g = v.y * 255;
	c.b = v.z * 255;
	c.a = v.w * 255;
	return c;
}

inline vec4 Color_to_vec4(Color c)
{
	return vec4((float)c.r/255, (float)c.g/255, (float)c.b/255, (float)c.a/255);
}

struct Line
{
	Line(float x1, float y1, float x2, float y2)
	{
		A = y1 - y2;
		B = x2 - x1;
		C = x1*y2 - x2*y1;
	}

	float func(float x, float y)
	{
		return A*x + B*y + C;
	}

	float findy(float x)
	{
		return -(A*x + C)/B;
	}

	float findx(float y)
	{
		return -(B*y + C)/A;
	}

	float A, B, C;

};


struct Plane
{
	vec3 n;	//normal  points x on plane satisfy n dot x = d
	float d; //d = n dot p

	Plane() {}
	Plane(vec3 a, vec3 b, vec3 c)	//ccw winding
	{
		n = cross_product(b-a, c-a).norm();
		d = n * a;
	}
};


int intersect_segment_plane(vec3 a, vec3 b, Plane p, float& t, vec3& q);


}
#endif /* RMATH_H_ */
