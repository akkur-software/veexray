///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for 3D vectors.
 *	\file		IcePoint.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#pragma once

enum PointComponent
{
	_X = 0,
	_Y = 1,
	_Z = 2,
	_W = 3,
	_FORCE_DWORD = 0x7fffffff
};

// Forward declarations
class HPoint;
class Plane;
class Matrix3x3;
class Matrix4x4;

#define CROSS2D(a, b)	(a.x*b.y - b.x*a.y)

#define EPSILON2 1.0e-20f;

class icePoint
{
public:

	//! Empty constructor
	inline icePoint()
	{
	}

	//! Constructor from a single float
	inline icePoint(float val) : x(val), y(val), z(val)
	{
	}

	//! Constructor from floats
	inline icePoint(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{
	}

	//! Constructor from array
	inline icePoint(float f[3]) : x(f[_X]), y(f[_Y]), z(f[_Z])
	{
	}

	//! Copy constructor
	inline icePoint(const icePoint& p) : x(p.x), y(p.y), z(p.z)
	{
	}

	//! Destructor
	inline ~icePoint()
	{
	}

	//! Clears the vector3
	inline icePoint& Zero()
	{
		x = y = z = 0.0f;
		return *this;
	}

	//! + infinity
	inline icePoint& SetPlusInfinity()
	{
		x = y = z = flt_max;
		return *this;
	}

	//! - infinity
	inline icePoint& SetMinusInfinity()
	{
		x = y = z = flt_min;
		return *this;
	}

	//! Sets positive unit random vector3
	icePoint& PositiveUnitRandomVector();
	//! Sets unit random vector3
	icePoint& UnitRandomVector();

	//! Assignment from values
	inline icePoint& Set(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
		return *this;
	}

	//! Assignment from array
	inline icePoint& Set(float f[3])
	{
		x = f[_X];
		y = f[_Y];
		z = f[_Z];
		return *this;
	}

	//! Assignment from another point
	inline icePoint& Set(const icePoint& src)
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}

	//! Adds a vector3
	inline icePoint& Add(const icePoint& p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	//! Adds a vector3
	inline icePoint& Add(float _x, float _y, float _z)
	{
		x += _x;
		y += _y;
		z += _z;
		return *this;
	}

	//! Adds a vector3
	inline icePoint& Add(float f[3])
	{
		x += f[_X];
		y += f[_Y];
		z += f[_Z];
		return *this;
	}

	//! Adds vectors
	inline icePoint& Add(const icePoint& p, const icePoint& q)
	{
		x = p.x + q.x;
		y = p.y + q.y;
		z = p.z + q.z;
		return *this;
	}

	//! Subtracts a vector3
	inline icePoint& Sub(const icePoint& p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

	//! Subtracts a vector3
	inline icePoint& Sub(float _x, float _y, float _z)
	{
		x -= _x;
		y -= _y;
		z -= _z;
		return *this;
	}

	//! Subtracts a vector3
	inline icePoint& Sub(float f[3])
	{
		x -= f[_X];
		y -= f[_Y];
		z -= f[_Z];
		return *this;
	}

	//! Subtracts vectors
	inline icePoint& Sub(const icePoint& p, const icePoint& q)
	{
		x = p.x - q.x;
		y = p.y - q.y;
		z = p.z - q.z;
		return *this;
	}

	//! this = -this
	inline icePoint& Neg()
	{
		x = -x;
		y = -y;
		z = -z;
		return *this;
	}

	//! this = -a
	inline icePoint& Neg(const icePoint& a)
	{
		x = -a.x;
		y = -a.y;
		z = -a.z;
		return *this;
	}

	//! Multiplies by a scalar
	inline icePoint& Mult(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	//! this = a * scalar
	inline icePoint& Mult(const icePoint& a, float scalar)
	{
		x = a.x * scalar;
		y = a.y * scalar;
		z = a.z * scalar;
		return *this;
	}

	//! this = a + b * scalar
	inline icePoint& Mac(const icePoint& a, const icePoint& b, float scalar)
	{
		x = a.x + b.x * scalar;
		y = a.y + b.y * scalar;
		z = a.z + b.z * scalar;
		return *this;
	}

	//! this = this + a * scalar
	inline icePoint& Mac(const icePoint& a, float scalar)
	{
		x += a.x * scalar;
		y += a.y * scalar;
		z += a.z * scalar;
		return *this;
	}

	//! this = a - b * scalar
	inline icePoint& Msc(const icePoint& a, const icePoint& b, float scalar)
	{
		x = a.x - b.x * scalar;
		y = a.y - b.y * scalar;
		z = a.z - b.z * scalar;
		return *this;
	}

	//! this = this - a * scalar
	inline icePoint& Msc(const icePoint& a, float scalar)
	{
		x -= a.x * scalar;
		y -= a.y * scalar;
		z -= a.z * scalar;
		return *this;
	}

	//! this = a + b * scalarb + c * scalarc
	inline icePoint& Mac2(const icePoint& a, const icePoint& b, float scalarb, const icePoint& c, float scalarc)
	{
		x = a.x + b.x * scalarb + c.x * scalarc;
		y = a.y + b.y * scalarb + c.y * scalarc;
		z = a.z + b.z * scalarb + c.z * scalarc;
		return *this;
	}

	//! this = a - b * scalarb - c * scalarc
	inline icePoint& Msc2(const icePoint& a, const icePoint& b, float scalarb, const icePoint& c, float scalarc)
	{
		x = a.x - b.x * scalarb - c.x * scalarc;
		y = a.y - b.y * scalarb - c.y * scalarc;
		z = a.z - b.z * scalarb - c.z * scalarc;
		return *this;
	}

	//! this = mat * a
	inline icePoint& Mult(const Matrix3x3& mat, const icePoint& a);

	//! this = mat1 * a1 + mat2 * a2
	inline icePoint& Mult2(const Matrix3x3& mat1, const icePoint& a1, const Matrix3x3& mat2, const icePoint& a2);

	//! this = this + mat * a
	inline icePoint& Mac(const Matrix3x3& mat, const icePoint& a);

	//! this = transpose(mat) * a
	inline icePoint& TransMult(const Matrix3x3& mat, const icePoint& a);

	//! Linear interpolate between two vectors: this = a + t * (b - a)
	inline icePoint& Lerp(const icePoint& a, const icePoint& b, float t)
	{
		x = a.x + t * (b.x - a.x);
		y = a.y + t * (b.y - a.y);
		z = a.z + t * (b.z - a.z);
		return *this;
	}

	//! Hermite interpolate between p1 and p2. p0 and p3 are used for finding gradient at p1 and p2.
	//! this =	p0 * (2t^2 - t^3 - t)/2
	//!			+ p1 * (3t^3 - 5t^2 + 2)/2
	//!			+ p2 * (4t^2 - 3t^3 + t)/2
	//!			+ p3 * (t^3 - t^2)/2
	inline icePoint& Herp(const icePoint& p0, const icePoint& p1, const icePoint& p2, const icePoint& p3, float t)
	{
		float t2 = t * t;
		float t3 = t2 * t;
		float kp0 = (2.0f * t2 - t3 - t) * 0.5f;
		float kp1 = (3.0f * t3 - 5.0f * t2 + 2.0f) * 0.5f;
		float kp2 = (4.0f * t2 - 3.0f * t3 + t) * 0.5f;
		float kp3 = (t3 - t2) * 0.5f;
		x = p0.x * kp0 + p1.x * kp1 + p2.x * kp2 + p3.x * kp3;
		y = p0.y * kp0 + p1.y * kp1 + p2.y * kp2 + p3.y * kp3;
		z = p0.z * kp0 + p1.z * kp1 + p2.z * kp2 + p3.z * kp3;
		return *this;
	}

	//! this = rotpos * r + linpos
	inline icePoint& Transform(const icePoint& r, const Matrix3x3& rotpos, const icePoint& linpos);

	//! this = trans(rotpos) * (r - linpos)
	inline icePoint& InvTransform(const icePoint& r, const Matrix3x3& rotpos, const icePoint& linpos);

	//! Returns _min(x, y, z);
	inline float Min() const { return _min(x, _min(y, z)); }
	//! Returns _max(x, y, z);
	inline float Max() const { return _max(x, _max(y, z)); }
	//! Sets each element to be componentwise minimum
	inline icePoint& Min(const icePoint& p)
	{
		x = _min(x, p.x);
		y = _min(y, p.y);
		z = _min(z, p.z);
		return *this;
	}

	//! Sets each element to be componentwise maximum
	inline icePoint& Max(const icePoint& p)
	{
		x = _max(x, p.x);
		y = _max(y, p.y);
		z = _max(z, p.z);
		return *this;
	}

	//! Clamps each element
	inline icePoint& Clamp(float min, float max)
	{
		if (x < min) x = min;
		if (x > max) x = max;
		if (y < min) y = min;
		if (y > max) y = max;
		if (z < min) z = min;
		if (z > max) z = max;
		return *this;
	}

	//! Computes square magnitude
	inline float SquareMagnitude() const { return x * x + y * y + z * z; }
	//! Computes magnitude
	inline float Magnitude() const { return _sqrt(x * x + y * y + z * z); }
	//! Computes volume
	inline float Volume() const { return x * y * z; }

	//! Checks the point is near zero
	bool ApproxZero() const
	{
		return SquareMagnitude() < EPSILON2;
	}

	//! Slighty moves the point
	void Tweak(udword coordmask, udword tweakmask)
	{
		if (coordmask & 1)
		{
			udword Dummy = IR(x);
			Dummy ^= tweakmask;
			x = FR(Dummy);
		}
		if (coordmask & 2)
		{
			udword Dummy = IR(y);
			Dummy ^= tweakmask;
			y = FR(Dummy);
		}
		if (coordmask & 4)
		{
			udword Dummy = IR(z);
			Dummy ^= tweakmask;
			z = FR(Dummy);
		}
	}

#define TWEAKMASK		0x3fffff
#define TWEAKNOTMASK	~TWEAKMASK

	//! Slighty moves the point out
	inline void TweakBigger()
	{
		udword Dummy = (IR(x) & TWEAKNOTMASK);
		if (!IS_NEGATIVE_FLOAT(x)) Dummy += TWEAKMASK + 1;
		x = FR(Dummy);
		Dummy = (IR(y) & TWEAKNOTMASK);
		if (!IS_NEGATIVE_FLOAT(y)) Dummy += TWEAKMASK + 1;
		y = FR(Dummy);
		Dummy = (IR(z) & TWEAKNOTMASK);
		if (!IS_NEGATIVE_FLOAT(z)) Dummy += TWEAKMASK + 1;
		z = FR(Dummy);
	}

	//! Slighty moves the point in
	inline void TweakSmaller()
	{
		udword Dummy = (IR(x) & TWEAKNOTMASK);
		if (IS_NEGATIVE_FLOAT(x)) Dummy += TWEAKMASK + 1;
		x = FR(Dummy);
		Dummy = (IR(y) & TWEAKNOTMASK);
		if (IS_NEGATIVE_FLOAT(y)) Dummy += TWEAKMASK + 1;
		y = FR(Dummy);
		Dummy = (IR(z) & TWEAKNOTMASK);
		if (IS_NEGATIVE_FLOAT(z)) Dummy += TWEAKMASK + 1;
		z = FR(Dummy);
	}

	//! Normalizes the vector3
	inline icePoint& Normalize()
	{
		float M = x * x + y * y + z * z;
		if (M)
		{
			M = 1.0f / _sqrt(M);
			x *= M;
			y *= M;
			z *= M;
		}
		return *this;
	}

	//! Sets vector3 length
	inline icePoint& SetLength(float length)
	{
		float NewLength = length / Magnitude();
		x *= NewLength;
		y *= NewLength;
		z *= NewLength;
		return *this;
	}

	//! Computes distance to another point
	inline float Distance(const icePoint& b) const
	{
		return _sqrt((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y) + (z - b.z) * (z - b.z));
	}

	//! Computes square distance to another point
	inline float SquareDistance(const icePoint& b) const
	{
		return ((x - b.x) * (x - b.x) + (y - b.y) * (y - b.y) + (z - b.z) * (z - b.z));
	}

	//! Dot product dp = this|a
	inline float Dot(const icePoint& p) const { return p.x * x + p.y * y + p.z * z; }

	//! Cross product this = a x b
	inline icePoint& Cross(const icePoint& a, const icePoint& b)
	{
		x = a.y * b.z - a.z * b.y;
		y = a.z * b.x - a.x * b.z;
		z = a.x * b.y - a.y * b.x;
		return *this;
	}

	//! Vector code ( bitmask = sign(z) | sign(y) | sign(x) )
	inline udword VectorCode() const
	{
		return (IR(x) >> 31) | ((IR(y) & SIGN_BITMASK) >> 30) | ((IR(z) & SIGN_BITMASK) >> 29);
	}

	//! Returns largest axis
	inline PointComponent LargestAxis() const
	{
		const float* Vals = &x;
		PointComponent m = _X;
		if (Vals[_Y] > Vals[m]) m = _Y;
		if (Vals[_Z] > Vals[m]) m = _Z;
		return m;
	}

	//! Returns closest axis
	inline PointComponent ClosestAxis() const
	{
		const float* Vals = &x;
		PointComponent m = _X;
		if (AIR(Vals[_Y]) > AIR(Vals[m])) m = _Y;
		if (AIR(Vals[_Z]) > AIR(Vals[m])) m = _Z;
		return m;
	}

	//! Returns smallest axis
	inline PointComponent SmallestAxis() const
	{
		const float* Vals = &x;
		PointComponent m = _X;
		if (Vals[_Y] < Vals[m]) m = _Y;
		if (Vals[_Z] < Vals[m]) m = _Z;
		return m;
	}

	//! Refracts the point
	icePoint& Refract(const icePoint& eye, const icePoint& n, float refractindex, icePoint& refracted);

	//! Projects the point onto a plane
	icePoint& ProjectToPlane(const Plane& p);

	//! Projects the point onto the screen
	void ProjectToScreen(float halfrenderwidth, float halfrenderheight, const Matrix4x4& mat, HPoint& projected) const;

	//! Unfolds the point onto a plane according to edge(a,b)
	icePoint& Unfold(Plane& p, icePoint& a, icePoint& b);

	//! Hash function from Ville Miettinen
	inline udword GetHashValue() const
	{
		const udword* h = (const udword*)(this);
		udword f = (h[0] + h[1] * 11 - (h[2] * 17)) & 0x7fffffff; // avoid problems with +-0
		return (f >> 22) ^ (f >> 12) ^ (f);
	}

	// Arithmetic operators

	//! Unary operator for icePoint Negate = - icePoint
	inline icePoint operator-() const { return icePoint(-x, -y, -z); }

	//! Operator for icePoint Plus = icePoint + icePoint.
	inline icePoint operator+(const icePoint& p) const { return icePoint(x + p.x, y + p.y, z + p.z); }
	//! Operator for icePoint Minus = icePoint - icePoint.
	inline icePoint operator-(const icePoint& p) const { return icePoint(x - p.x, y - p.y, z - p.z); }

	//! Operator for icePoint Mul   = icePoint * icePoint.
	inline icePoint operator*(const icePoint& p) const { return icePoint(x * p.x, y * p.y, z * p.z); }
	//! Operator for icePoint Scale = icePoint * float.
	inline icePoint operator*(float s) const { return icePoint(x * s, y * s, z * s); }
	//! Operator for icePoint Scale = float * icePoint.
	inline friend icePoint operator*(float s, const icePoint& p) { return icePoint(s * p.x, s * p.y, s * p.z); }

	//! Operator for icePoint Div   = icePoint / icePoint.
	inline icePoint operator/(const icePoint& p) const { return icePoint(x / p.x, y / p.y, z / p.z); }
	//! Operator for icePoint Scale = icePoint / float.
	inline icePoint operator/(float s) const
	{
		s = 1.0f / s;
		return icePoint(x * s, y * s, z * s);
	}

	//! Operator for icePoint Scale = float / icePoint.
	inline friend icePoint operator/(float s, const icePoint& p) { return icePoint(s / p.x, s / p.y, s / p.z); }

	//! Operator for float DotProd = icePoint | icePoint.
	inline float operator|(const icePoint& p) const { return x * p.x + y * p.y + z * p.z; }
	//! Operator for icePoint VecProd = icePoint ^ icePoint.
	ICF icePoint operator^(const icePoint& p) const
	{
		return icePoint(
			y * p.z - z * p.y,
			z * p.x - x * p.z,
			x * p.y - y * p.x);
	}

	//! Operator for icePoint += icePoint.
	inline icePoint& operator+=(const icePoint& p)
	{
		x += p.x;
		y += p.y;
		z += p.z;
		return *this;
	}

	//! Operator for icePoint += float.
	inline icePoint& operator+=(float s)
	{
		x += s;
		y += s;
		z += s;
		return *this;
	}

	//! Operator for icePoint -= icePoint.
	inline icePoint& operator-=(const icePoint& p)
	{
		x -= p.x;
		y -= p.y;
		z -= p.z;
		return *this;
	}

	//! Operator for icePoint -= float.
	inline icePoint& operator-=(float s)
	{
		x -= s;
		y -= s;
		z -= s;
		return *this;
	}

	//! Operator for icePoint *= icePoint.
	inline icePoint& operator*=(const icePoint& p)
	{
		x *= p.x;
		y *= p.y;
		z *= p.z;
		return *this;
	}

	//! Operator for icePoint *= float.
	inline icePoint& operator*=(float s)
	{
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	//! Operator for icePoint /= icePoint.
	inline icePoint& operator/=(const icePoint& p)
	{
		x /= p.x;
		y /= p.y;
		z /= p.z;
		return *this;
	}

	//! Operator for icePoint /= float.
	inline icePoint& operator/=(float s)
	{
		s = 1.0f / s;
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	// Logical operators

	//! Operator for "if(icePoint==icePoint)"
	inline bool operator==(const icePoint& p) const
	{
		return ((IR(x) == IR(p.x)) && (IR(y) == IR(p.y)) && (IR(z) == IR(p.z)));
	}

	//! Operator for "if(icePoint!=icePoint)"
	inline bool operator!=(const icePoint& p) const
	{
		return ((IR(x) != IR(p.x)) || (IR(y) != IR(p.y)) || (IR(z) != IR(p.z)));
	}

	// Arithmetic operators

	//! Operator for icePoint Mul = icePoint * Matrix3x3.
	inline icePoint operator*(const Matrix3x3& mat) const
	{
		class ShadowMatrix3x3
		{
		public:
			float m[3][3];
		}; // To allow inlining
		const ShadowMatrix3x3* Mat = (const ShadowMatrix3x3*)&mat;

		return icePoint(
			x * Mat->m[0][0] + y * Mat->m[1][0] + z * Mat->m[2][0],
			x * Mat->m[0][1] + y * Mat->m[1][1] + z * Mat->m[2][1],
			x * Mat->m[0][2] + y * Mat->m[1][2] + z * Mat->m[2][2]);
	}

	//! Operator for icePoint Mul = icePoint * Matrix4x4.
	inline icePoint operator*(const Matrix4x4& mat) const
	{
		class ShadowMatrix4x4
		{
		public:
			float m[4][4];
		}; // To allow inlining
		const ShadowMatrix4x4* Mat = (const ShadowMatrix4x4*)&mat;

		return icePoint(
			x * Mat->m[0][0] + y * Mat->m[1][0] + z * Mat->m[2][0] + Mat->m[3][0],
			x * Mat->m[0][1] + y * Mat->m[1][1] + z * Mat->m[2][1] + Mat->m[3][1],
			x * Mat->m[0][2] + y * Mat->m[1][2] + z * Mat->m[2][2] + Mat->m[3][2]);
	}

	//! Operator for icePoint *= Matrix3x3.
	inline icePoint& operator*=(const Matrix3x3& mat)
	{
		class ShadowMatrix3x3
		{
		public:
			float m[3][3];
		}; // To allow inlining
		const ShadowMatrix3x3* Mat = (const ShadowMatrix3x3*)&mat;

		float xp = x * Mat->m[0][0] + y * Mat->m[1][0] + z * Mat->m[2][0];
		float yp = x * Mat->m[0][1] + y * Mat->m[1][1] + z * Mat->m[2][1];
		float zp = x * Mat->m[0][2] + y * Mat->m[1][2] + z * Mat->m[2][2];

		x = xp;
		y = yp;
		z = zp;

		return *this;
	}

	//! Operator for icePoint *= Matrix4x4.
	inline icePoint& operator*=(const Matrix4x4& mat)
	{
		class ShadowMatrix4x4
		{
		public:
			float m[4][4];
		}; // To allow inlining
		const ShadowMatrix4x4* Mat = (const ShadowMatrix4x4*)&mat;

		float xp = x * Mat->m[0][0] + y * Mat->m[1][0] + z * Mat->m[2][0] + Mat->m[3][0];
		float yp = x * Mat->m[0][1] + y * Mat->m[1][1] + z * Mat->m[2][1] + Mat->m[3][1];
		float zp = x * Mat->m[0][2] + y * Mat->m[1][2] + z * Mat->m[2][2] + Mat->m[3][2];

		x = xp;
		y = yp;
		z = zp;

		return *this;
	}

	// Cast operators

	//! Cast a icePoint to a HPoint. w is set to zero.
	operator HPoint() const;

	inline operator const float*() const { return &x; }
	inline operator float*() { return &x; }

public:
	float x, y, z;
};

FUNCTION void Normalize1(icePoint& a);
FUNCTION void Normalize2(icePoint& a);
