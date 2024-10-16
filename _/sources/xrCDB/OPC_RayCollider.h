///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for a ray collider.
 *	\file		OPC_RayCollider.h
 *	\author		Pierre Terdiman
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_RAYCOLLIDER_H__
#define __OPC_RAYCOLLIDER_H__

class RayCollider : public Collider
{
public:
	// Constructor / Destructor
	RayCollider();
	virtual ~RayCollider();
	// Generic collision query

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Generic stabbing query for generic OPCODE models. After the call, access the results:
	 *	- with GetContactStatus()
	 *	- in the user-provided destination array
	 *
	 *	\param		world_ray		[in] stabbing ray in world space
	 *	\param		model			[in] Opcode model to collide with
	 *	\param		world			[in] model's world matrix, or null
	 *	\param		cache			[in] a possibly cached face index, or null
	 *	\return		true if success
	 *	\warning	SCALE NOT SUPPORTED. The matrices must contain rotation & translation parts only.
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Collide(const Ray& world_ray, OPCODE_Model* model, const Matrix4x4* world = null, udword* cache = null);

	// Collision queries
	bool Collide(const Ray& world_ray, const AABBCollisionTree* tree, const Matrix4x4* world = null,
	             udword* cache = null);
	bool Collide(const Ray& world_ray, const AABBNoLeafTree* tree, const Matrix4x4* world = null, udword* cache = null);
	bool Collide(const Ray& world_ray, const AABBQuantizedTree* tree, const Matrix4x4* world = null,
	             udword* cache = null);
	bool Collide(const Ray& world_ray, const AABBQuantizedNoLeafTree* tree, const Matrix4x4* world = null,
	             udword* cache = null);
	bool Collide(const Ray& world_ray, const AABBTree* tree, Container& box_indices);
	// Settings

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Settings: enable or disable "closest hit" mode.
	 *	\param		flag		[in] true to report closest hit only
	 *	\see		SetCulling(bool flag)
	 *	\see		SetMaxDist(float maxdist)
	 *	\see		SetDestination(StabbedFaces* sf)
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void SetClosestHit(bool flag) { mClosestHit = flag; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Settings: enable or disable backface culling.
	 *	\param		flag		[in] true to enable backface culling
	 *	\see		SetClosestHit(bool flag)
	 *	\see		SetMaxDist(float maxdist)
	 *	\see		SetDestination(StabbedFaces* sf)
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void SetCulling(bool flag) { mCulling = flag; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Settings: sets the higher distance bound.
	 *	\param		maxdist		[in] higher distance bound. Default = maximal value, for ray queries (else segment)
	 *	\see		SetClosestHit(bool flag)
	 *	\see		SetCulling(bool flag)
	 *	\see		SetDestination(StabbedFaces* sf)
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void SetMaxDist(float maxdist = flt_max) { mMaxDist = maxdist; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Settings: sets the destination array for stabbed faces.
	 *	\param		cf			[in] destination array, filled during queries
	 *	\see		SetClosestHit(bool flag)
	 *	\see		SetCulling(bool flag)
	 *	\see		SetMaxDist(float maxdist)
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void SetDestination(CollisionFaces* cf) { mStabbedFaces = cf; }

	// Stats
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Stats: gets the number of Ray-BV overlap tests after a collision query.
	 *	\see		GetNbRayPrimTests()
	 *	\see		GetNbIntersections()
	 *	\return		the number of Ray-BV tests performed during last query
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline udword GetNbRayBVTests() const { return mNbRayBVTests; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Stats: gets the number of Ray-Triangle overlap tests after a collision query.
	 *	\see		GetNbRayBVTests()
	 *	\see		GetNbIntersections()
	 *	\return		the number of Ray-Triangle tests performed during last query
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline udword GetNbRayPrimTests() const { return mNbRayPrimTests; }

	// In-out test
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Stats: gets the number of intersection found after a collision query. Can be used for in/out tests.
	 *	\see		GetNbRayBVTests()
	 *	\see		GetNbRayPrimTests()
	 *	\return		the number of valid intersections during last query
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline udword GetNbIntersections() const { return mNbIntersections; }

#ifdef OPC_USE_CALLBACKS
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Callback control: a method to setup object callback. Must provide triangle-vertices for a given triangle index.
	 *	\param		callback	[in] user-defined callback
	 *	\param		data		[in] user-defined data
	 */
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		inline				void			SetCallback(OPC_CALLBACK callback, udword data)			{ mObjCallback = callback;	mUserData = data;	}
#else
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Pointers control: a method to setup object pointers. Must provide access to faces and vertices for a given object.
	 *	\param		faces	[in] pointer to faces
	 *	\param		verts	[in] pointer to vertices
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void SetPointers(const IndexedTriangle* faces, const icePoint* verts)
	{
		mFaces = faces;
		mVerts = verts;
	}
#endif
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Validates current settings. You should call this method after all the settings and callbacks have been defined for a collider.
	 *	\return		null if everything is ok, else a string describing the problem
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const char* ValidateSettings();

	inline bool RayTriOverlap(const icePoint& vert0, const icePoint& vert1, const icePoint& vert2)
	{
		// Stats
		mNbRayPrimTests++;

		// Find vectors for two edges sharing vert0
		icePoint edge1 = vert1 - vert0;
		icePoint edge2 = vert2 - vert0;

		// Begin calculating determinant - also used to calculate U parameter
		icePoint pvec = mDir ^ edge2;

		// If determinant is near zero, ray lies in plane of triangle
		float det = edge1 | pvec;

		if (mCulling)
		{
			if (det < LOCAL_EPSILON) return false;
			// From here, det is > 0. So we can use integer cmp.

			// Calculate distance from vert0 to ray origin
			icePoint tvec = mOrigin - vert0;

			// Calculate U parameter and test bounds
			mStabbedFace.mU = tvec | pvec;
			//		if(IR(u)&0x80000000 || u>det)					return false;
			if (IR(mStabbedFace.mU) & 0x80000000 || IR(mStabbedFace.mU) > IR(det)) return false;

			// Prepare to test V parameter
			icePoint qvec = tvec ^ edge1;

			// Calculate V parameter and test bounds
			mStabbedFace.mV = mDir | qvec;
			if (IR(mStabbedFace.mV) & 0x80000000 || mStabbedFace.mU + mStabbedFace.mV > det) return false;

			// Calculate t, scale parameters, ray intersects triangle
			mStabbedFace.mDistance = edge2 | qvec;
			float inv_det = 1.0f / det;
			mStabbedFace.mDistance *= inv_det;
			mStabbedFace.mU *= inv_det;
			mStabbedFace.mV *= inv_det;
		}
		else
		{
			// the non-culling branch
			if (det > -LOCAL_EPSILON && det < LOCAL_EPSILON) return false;
			float inv_det = 1.0f / det;

			// Calculate distance from vert0 to ray origin
			icePoint tvec = mOrigin - vert0;

			// Calculate U parameter and test bounds
			mStabbedFace.mU = (tvec | pvec) * inv_det;
			//		if(IR(u)&0x80000000 || u>1.0f)					return false;
			if (IR(mStabbedFace.mU) & 0x80000000 || IR(mStabbedFace.mU) > IEEE_1_0) return false;

			// prepare to test V parameter
			icePoint qvec = tvec ^ edge1;

			// Calculate V parameter and test bounds
			mStabbedFace.mV = (mDir | qvec) * inv_det;
			if (IR(mStabbedFace.mV) & 0x80000000 || mStabbedFace.mU + mStabbedFace.mV > 1.0f) return false;

			// Calculate t, ray intersects triangle
			mStabbedFace.mDistance = (edge2 | qvec) * inv_det;
		}
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Computes a segment-AABB overlap test using the separating axis theorem. Segment is cached within the class.
 *	\param		center	[in] AABB center
 *	\param		extents	[in] AABB extents
 *	\return		true on overlap
 */
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool SegmentAABBOverlap(const icePoint& center, const icePoint& extents)
	{
		// Stats
		mNbRayBVTests++;

		float Dx = mData2.x - center.x;
		if (_abs(Dx) > extents.x + mFDir.x) return false;
		float Dy = mData2.y - center.y;
		if (_abs(Dy) > extents.y + mFDir.y) return false;
		float Dz = mData2.z - center.z;
		if (_abs(Dz) > extents.z + mFDir.z) return false;

		float f;
		f = mData.y * Dz - mData.z * Dy;
		if (_abs(f) > extents.y * mFDir.z + extents.z * mFDir.y) return false;
		f = mData.z * Dx - mData.x * Dz;
		if (_abs(f) > extents.x * mFDir.z + extents.z * mFDir.x) return false;
		f = mData.x * Dy - mData.y * Dx;
		if (_abs(f) > extents.x * mFDir.y + extents.y * mFDir.x) return false;

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Computes a ray-AABB overlap test using the separating axis theorem. Ray is cached within the class.
	 *	\param		center	[in] AABB center
	 *	\param		extents	[in] AABB extents
	 *	\return		true on overlap
	 */
	 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool RayAABBOverlap(const icePoint& center, const icePoint& extents)
	{
		// Stats
		mNbRayBVTests++;

		//	float Dx = mOrigin.x - center.x;	if(_abs(Dx) > extents.x && Dx*mDir.x>=0.0f)	return false;
		//	float Dy = mOrigin.y - center.y;	if(_abs(Dy) > extents.y && Dy*mDir.y>=0.0f)	return false;
		//	float Dz = mOrigin.z - center.z;	if(_abs(Dz) > extents.z && Dz*mDir.z>=0.0f)	return false;

		float Dx = mOrigin.x - center.x;
		if (GREATER(Dx, extents.x) && Dx * mDir.x >= 0.0f) return false;
		float Dy = mOrigin.y - center.y;
		if (GREATER(Dy, extents.y) && Dy * mDir.y >= 0.0f) return false;
		float Dz = mOrigin.z - center.z;
		if (GREATER(Dz, extents.z) && Dz * mDir.z >= 0.0f) return false;

		//	float Dx = mOrigin.x - center.x;	if(GREATER(Dx, extents.x) && ((SIR(Dx)-1)^SIR(mDir.x))>=0.0f)	return false;
		//	float Dy = mOrigin.y - center.y;	if(GREATER(Dy, extents.y) && ((SIR(Dy)-1)^SIR(mDir.y))>=0.0f)	return false;
		//	float Dz = mOrigin.z - center.z;	if(GREATER(Dz, extents.z) && ((SIR(Dz)-1)^SIR(mDir.z))>=0.0f)	return false;

		float f;
		f = mDir.y * Dz - mDir.z * Dy;
		if (_abs(f) > extents.y * mFDir.z + extents.z * mFDir.y) return false;
		f = mDir.z * Dx - mDir.x * Dz;
		if (_abs(f) > extents.x * mFDir.z + extents.z * mFDir.x) return false;
		f = mDir.x * Dy - mDir.y * Dx;
		if (_abs(f) > extents.x * mFDir.y + extents.y * mFDir.x) return false;

		return true;
	}

protected:
	// Ray in local space
	icePoint mOrigin; //!< Ray origin
	icePoint mDir; //!< Ray direction (normalized)
	icePoint mFDir; //!< fabsf(mDir)
	icePoint mData, mData2;
	// Stabbed faces
	CollisionFace mStabbedFace; //!< Current stabbed face
	CollisionFaces* mStabbedFaces; //!< List of stabbed faces
#ifdef OPC_USE_CALLBACKS
		// User callback
							udword			mUserData;			//!< User-defined data sent to callback
							OPC_CALLBACK	mObjCallback;		//!< Object callback
#else
	// User pointers
	const IndexedTriangle* mFaces; //!< User-defined faces
	const icePoint* mVerts; //!< User-defined vertices
#endif
	// Stats
	udword mNbRayBVTests; //!< Number of Ray-BV tests
	udword mNbRayPrimTests; //!< Number of Ray-Primitive tests
	// In-out test
	udword mNbIntersections; //!< Number of valid intersections
	// Dequantization coeffs
	icePoint mCenterCoeff;
	icePoint mExtentsCoeff;
	// Settings
	float mMaxDist; //!< Valid segment on the ray
	bool mClosestHit; //!< Report closest hit only
	bool mCulling; //!< Stab culled faces or not
	// Internal methods
	void _Stab(const AABBCollisionNode* node);
	void _Stab(const AABBNoLeafNode* node);
	void _Stab(const AABBQuantizedNode* node);
	void _Stab(const AABBQuantizedNoLeafNode* node);
	void _Stab(const AABBTreeNode* node, Container& box_indices);
	void _UnboundedStab(const AABBCollisionNode* node);
	void _UnboundedStab(const AABBNoLeafNode* node);
	void _UnboundedStab(const AABBQuantizedNode* node);
	void _UnboundedStab(const AABBQuantizedNoLeafNode* node);
	void _UnboundedStab(const AABBTreeNode* node, Container& box_indices);
	// Overlap tests
	// Init methods
	bool InitQuery(const Ray& world_ray, const Matrix4x4* world = null, udword* faceid = null);
};

#endif // __OPC_RAYCOLLIDER_H__
