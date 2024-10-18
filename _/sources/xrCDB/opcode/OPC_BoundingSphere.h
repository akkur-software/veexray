///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code to compute the minimal bounding sphere.
 *	\file		IceBoundingSphere.h
 *	\author		Pierre Terdiman
 *	\date		January, 29, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __ICEBOUNDINGSPHERE_H__
#define __ICEBOUNDINGSPHERE_H__

class Sphere
{
public:
	//! Constructor
	inline Sphere()
	{
	}

	//! Constructor
	inline Sphere(const icePoint& center, float radius) : mCenter(center), mRadius(radius)
	{
	}

	//! Constructor
	Sphere(const udword n, icePoint* p);
	//! Copy constructor
	inline Sphere(const Sphere& sphere) : mCenter(sphere.mCenter), mRadius(sphere.mRadius)
	{
	}

	//! Destructor
	inline ~Sphere()
	{
	}

	bool Compute(udword nbverts, icePoint* verts);
	bool FastCompute(udword nbverts, icePoint* verts);

	// Access methods
	inline const icePoint& GetCenter() const { return mCenter; }
	inline float GetRadius() const { return mRadius; }

	inline const icePoint& Center() const { return mCenter; }
	inline float Radius() const { return mRadius; }

	inline Sphere& Set(const icePoint& center, float radius)
	{
		mCenter = center;
		mRadius = radius;
		return *this;
	}

	inline Sphere& SetCenter(const icePoint& center)
	{
		mCenter = center;
		return *this;
	}

	inline Sphere& SetRadius(float radius)
	{
		mRadius = radius;
		return *this;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Tests if a point is contained within the sphere.
	 *	\param		p	[in] the point to test
	 *	\return		true if inside the sphere
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Contains(const icePoint& p) const
	{
		return mCenter.SquareDistance(p) < mRadius * mRadius;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Tests if a sphere is contained within the sphere.
	 *	\param		sphere	[in] the sphere to test
	 *	\return		true if inside the sphere
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Contains(const Sphere& sphere) const
	{
		float r = mRadius - sphere.mRadius;
		return mCenter.SquareDistance(sphere.mCenter) < r * r;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Tests if a box is contained within the sphere.
	 *	\param		aabb	[in] the box to test
	 *	\return		true if inside the sphere
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool Contains(const AABB& aabb) const
	{
		// I assume if all 8 box vertices are inside the sphere, so does the whole box.
		// Sounds ok but maybe there's a better way?
		float R2 = mRadius * mRadius;
		icePoint Max;
		aabb.GetMax(Max);
		icePoint Min;
		aabb.GetMin(Min);

		icePoint p;
		p.x = Max.x;
		p.y = Max.y;
		p.z = Max.z;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Min.x;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Max.x;
		p.y = Min.y;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Min.x;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Max.x;
		p.y = Max.y;
		p.z = Min.z;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Min.x;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Max.x;
		p.y = Min.y;
		if (mCenter.SquareDistance(p) >= R2) return false;
		p.x = Min.x;
		if (mCenter.SquareDistance(p) >= R2) return false;

		return true;
	}

public:
	icePoint mCenter; //!< Sphere center
	float mRadius; //!< Sphere radius

//#ifdef OLD
//		private:
//				Sphere			PlanarCircumscribe3							(const icePoint& p0, const icePoint& p1, const icePoint& p2);
//				Sphere			Circumscribe4								(const icePoint& p0, const icePoint& p1, const icePoint& p2, const icePoint& p3);
//				Sphere			MinFix3										(int n, icePoint** perm, icePoint* fixed0, icePoint* fixed1, icePoint* fixed2);
//				Sphere			MinFix2										(int n, icePoint** perm, icePoint* fixed0, icePoint* fixed1);
//				Sphere			MinFix1										(int n, icePoint** perm, icePoint* fixed0);
//
//				int				PointInsideSphere							(const icePoint& p, const Sphere& s);
//				Sphere			MinimalSphere1								(const icePoint& p);
//				Sphere			MinimalSphere2								(const icePoint& p0, const icePoint& p1);
//				Sphere			MinimalSphere3								(const icePoint& p0, const icePoint& p1, const icePoint& p2);
//				Sphere			MinimalSphere4								(const icePoint& p0, const icePoint& p1, const icePoint& p2, const icePoint& p3);
//#endif
};

#endif // __ICEBOUNDINGSPHERE_H__
