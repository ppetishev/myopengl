#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>

template <class T> struct Vec2{
    union{
        struct{T u, v;};
    	struct{T x, y;};
		T raw[2];
    };
    
    Vec2():u(0), v(0){}
    Vec2(T _u, T _v):u(_u), v(_v) {}
    inline Vec2<T> operator+(const Vec2<T> &V){return Vec2<T>(u+V.u, v+V.v);}
    inline Vec2<T> operator-(const Vec2<T> &V){return Vec2<T>(u-V.u, v-V.v);}
    inline Vec2<T> operator*(float f){return Vec2<T>(u*f, v*f);}
	template <class > friend Vec2<T> operator*(float f, const Vec2<T> &v);
    template <class > friend std::ostream& operator << (std::ostream& s, Vec2<T> &v);
    inline const float vec_len(){return std::sqrt(v*v + u*u);}
    Vec2<T> normalize(float l=1){
        return (*this) * (l/vec_len());
    }
};

template <class t> struct Vec3{
    union{
        struct{t x, y, z;};
        struct{t ivert, iuv, inorm;};
        t raw[3];
    };
    Vec3(): x(0), y(0), z(0){}
    Vec3(t _x, t _y, t _z): x(_x), y(_y), z(_z){}
    inline Vec3<t> operator+(const Vec3<t> &v){return Vec3<t>(x+v.x, y+v.y, z+v.z);}
    inline Vec3<t> operator-(const Vec3<t> &v){return Vec3<t>(x-v.x, y-v.y, z-v.z);}
    inline t operator*(const Vec3<t> &v){return x*v.x + y*v.y + z*v.z;}
    inline Vec3<t> operator*(float f){return Vec3<t>(x*f, y*f, z*f);}
    inline const float vec_len(){return std::sqrt(x*x+y*y+z*z);}
    Vec3<t> normalize(t l=1){
        return (*this) * (l / vec_len());
    }
    template <class > friend std::ostream& operator << (std::ostream& s, Vec3<t> &v);
    template <class > friend Vec2<t> operator *(float f, Vec3<t> &v);
};



typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec3<int> Vec3i;
typedef Vec3<float> Vec3f;

template <class T> std::ostream& operator << (std::ostream& s, Vec2<T> &v){
    s << "(" << v.x << ", " << v.y << ")\n";
    return s;
}
template <class t> std::ostream& operator << (std::ostream& s, Vec3<t> &v){
    s << "(" <<v.x<< ", " << v.y << ", " << v.z << ")\n";
    return s;
}

template <class T> Vec2<T> operator*(float f, const Vec2<T> &v){
	return Vec2<T>(v.u*f, v.v*f);
}
template <class T> Vec3<T> operator*(float f, const Vec3<T> &v){
	return Vec3<T>(v.x*f, v.y*f, v.z*f);
}


#endif //__GEOMETRY_H__
