///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for rays.
 *	\file		IceRay.h
 *	\author		Pierre Terdiman
 *	\date		April, 4, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICERAY_H__
#define __ICERAY_H__

class Ray
{
public:
	//! Constructor
	inline Ray()
	{
	}

	//! Constructor
	inline Ray(const icePoint& orig, const icePoint& dir) : mOrig(orig), mDir(dir)
	{
	}

	//! Copy constructor
	inline Ray(const Ray& ray) : mOrig(ray.mOrig), mDir(ray.mDir)
	{
	}

	//! Destructor
	inline ~Ray()
	{
	}

	icePoint mOrig; //!< Ray origin
	icePoint mDir; //!< Normalized direction
};

class Segment
{
public:
	//! Constructor
	inline Segment()
	{
	}

	//! Constructor
	inline Segment(const icePoint& p0, const icePoint& p1) : mP0(p0), mP1(p1)
	{
	}

	//! Copy constructor
	inline Segment(const Segment& seg) : mP0(seg.mP0), mP1(seg.mP1)
	{
	}

	//! Destructor
	inline ~Segment()
	{
	}

	inline const icePoint& GetOrigin() const { return mP0; }
	inline icePoint ComputeDirection() const { return mP1 - mP0; }

	inline void SetOriginDirection(const icePoint& origin, const icePoint& direction)
	{
		mP0 = mP1 = origin;
		mP1 += direction;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Computes a point on the segment
	 *	\param		pt	[out] point on segment
	 *	\param		t	[in] point's parameter [t=0 => pt = mP0, t=1 => pt = mP1]
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void ComputePoint(icePoint& pt, float t) const { pt = mP0 + t * (mP1 - mP0); }

	icePoint mP0; //!< Start of segment
	icePoint mP1; //!< End of segment
};

#endif // __ICERAY_H__