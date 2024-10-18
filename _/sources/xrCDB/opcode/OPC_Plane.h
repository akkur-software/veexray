///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for planes.
 *	\file		IcePlane.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEPLANE_H__
#define __ICEPLANE_H__

#define PLANE_EPSILON		(1.0e-7f)

class Plane
{
public:
	//! Constructor
	inline Plane()
	{
	}

	//! Constructor
	inline Plane(float nx, float ny, float nz, float d) { Set(nx, ny, nz, d); }
	//! Constructor
	inline Plane(const icePoint& p, const icePoint& n) { Set(p, n); }
	//! Constructor
	inline Plane(const icePoint& p0, const icePoint& p1, const icePoint& p2) { Set(p0, p1, p2); }
	//! Constructor
	inline Plane(const icePoint& n, float d)
	{
		this->n = n;
		this->d = d;
	}

	//! Copy constructor
	inline Plane(const Plane& plane) : n(plane.n), d(plane.d)
	{
	}

	//! Destructor
	inline ~Plane()
	{
	}

	inline Plane& Zero()
	{
		n.Zero();
		d = 0.0f;
		return *this;
	}

	inline Plane& Set(float nx, float ny, float nz, float d)
	{
		n.Set(nx, ny, nz);
		this->d = d;
		return *this;
	}

	inline Plane& Set(const icePoint& p, const icePoint& n)
	{
		this->n = n;
		d = - p | n;
		return *this;
	}

	Plane& Set(const icePoint& p0, const icePoint& p1, const icePoint& p2);

	inline float Distance(const icePoint& p) const { return (p | n) + d; }
	inline bool Belongs(const icePoint& p) const { return _abs(Distance(p)) < PLANE_EPSILON; }

	inline void Normalize()
	{
		float Denom = 1.0f / n.Magnitude();
		n.x *= Denom;
		n.y *= Denom;
		n.z *= Denom;
		d *= Denom;
	}

public:
	// Members
	icePoint n; //!< The normal to the plane
	float d; //!< The distance from the origin

	// Cast operators
	inline operator icePoint() const { return n; }
	/*		inline			operator HPoint()					const	{ return HPoint(n, d);								}
	
			// Arithmetic operators
			inline	Plane	operator*(const Matrix4x4& m)		const
							{
								// Old code from Irion. Kept for reference.
								Plane Ret(*this);
								return Ret *= m;
							}
	
			inline	Plane&	operator*=(const Matrix4x4& m)
							{
								// Old code from Irion. Kept for reference.
								icePoint n2 = HPoint(n, 0.0f) * m;
								d = -((icePoint) (HPoint( -d*n, 1.0f ) * m) | n2);
								n = n2;
								return *this;
							}
	*/
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms a plane by a 4x4 matrix. Same as Plane * Matrix4x4 operator, but faster.
 *	\param		transformed	[out] transformed plane
 *	\param		plane		[in] source plane
 *	\param		transform	[in] transform matrix
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TransformPlane(Plane& transformed, const Plane& plane, const Matrix4x4& transform)
{
	// Catch the rotation part of the 4x4 matrix
	Matrix3x3 Rot = transform;

	// Rotate the normal
	transformed.n = plane.n * Rot;

	// Compute _new_ d
	icePoint Trans;
	transform.GetTrans(Trans);
	transformed.d = (plane.d * transformed.n - Trans) | transformed.n;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Transforms a plane by a 4x4 matrix. Same as Plane * Matrix4x4 operator, but faster.
 *	\param		plane		[in/out] source plane (transformed on return)
 *	\param		transform	[in] transform matrix
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void TransformPlane(Plane& plane, const Matrix4x4& transform)
{
	// Catch the rotation part of the 4x4 matrix
	Matrix3x3 Rot = transform;

	// Rotate the normal
	plane.n *= Rot;

	// Compute _new_ d
	icePoint Trans;
	transform.GetTrans(Trans);
	plane.d = (plane.d * plane.n - Trans) | plane.n;
}

#endif // __ICEPLANE_H__
