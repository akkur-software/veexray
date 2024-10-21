///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for a sphere collider.
 *	\file		OPC_SphereCollider.h
 *	\author		Pierre Terdiman
 *	\date		June, 2, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_SPHERECOLLIDER_H__
#define __OPC_SPHERECOLLIDER_H__

struct SphereCache : VolumeCache
{
	SphereCache() : Center(0.0f, 0.0f, 0.0f), FatRadius2(0.0f), FatCoeff(1.1f)
	{
	}

	// Cached faces signature
	icePoint Center; //!< Sphere used when performing the query resulting in cached faces
	float FatRadius2; //!< Sphere used when performing the query resulting in cached faces
	// User settings
	float FatCoeff; //!< mRadius2 multiplier used to create a fat sphere
};

class SphereCollider : public VolumeCollider
{
public:
	// Constructor / Destructor
	SphereCollider();
	virtual ~SphereCollider();
	// Generic collision query

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Generic collision query for generic OPCODE models. After the call, access the results:
	 *	- with GetContactStatus()
	 *	- in the user-provided destination array
	 *
	 *	\param		cache			[in/out] a sphere cache
	 *	\param		sphere			[in] collision sphere in local space
	 *	\param		model			[in] Opcode model to collide with
	 *	\param		worlds			[in] sphere's world matrix, or null
	 *	\param		worldm			[in] model's world matrix, or null
	 *	\return		true if success
	 *	\warning	SCALE NOT SUPPORTED. The matrices must contain rotation & translation parts only.
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Collide(SphereCache& cache, const Sphere& sphere, OPCODE_Model* model, const Matrix4x4* worlds = null,
	             const Matrix4x4* worldm = null);

	// Collision queries
	bool Collide(SphereCache& cache, const Sphere& sphere, const AABBCollisionTree* tree,
	             const Matrix4x4* worlds = null, const Matrix4x4* worldm = null);
	bool Collide(SphereCache& cache, const Sphere& sphere, const AABBNoLeafTree* tree, const Matrix4x4* worlds = null,
	             const Matrix4x4* worldm = null);
	bool Collide(SphereCache& cache, const Sphere& sphere, const AABBQuantizedTree* tree,
	             const Matrix4x4* worlds = null, const Matrix4x4* worldm = null);
	bool Collide(SphereCache& cache, const Sphere& sphere, const AABBQuantizedNoLeafTree* tree,
	             const Matrix4x4* worlds = null, const Matrix4x4* worldm = null);
	bool Collide(SphereCache& cache, const Sphere& sphere, const AABBTree* tree);
	// Settings

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Validates current settings. You should call this method after all the settings and callbacks have been defined for a collider.
	 *	\return		null if everything is ok, else a string describing the problem
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const char* ValidateSettings();

	inline bool SphereAABBOverlap(const icePoint& center, const icePoint& extents)
	{
		// Stats
		mNbVolumeBVTests++;

		float d = 0.0f;

		//find the square of the distance
		//from the sphere to the box
#ifdef OLDIES
		for (udword i = 0; i < 3; i++)
		{
			float tmp = mCenter[i] - center[i];
			float s = tmp + extents[i];

			if (s < 0.0f)	d += s * s;
			else
			{
				s = tmp - extents[i];
				if (s > 0.0f)	d += s * s;
			}
		}
#endif
		float tmp, s;

		tmp = mCenter.x - center.x;
		s = tmp + extents.x;

		if (s < 0.0f)
		{
			d += s * s;
			if (d > mRadius2) return false;
		}
		else
		{
			s = tmp - extents.x;
			if (s > 0.0f)
			{
				d += s * s;
				if (d > mRadius2) return false;
			}
		}

		tmp = mCenter.y - center.y;
		s = tmp + extents.y;

		if (s < 0.0f)
		{
			d += s * s;
			if (d > mRadius2) return false;
		}
		else
		{
			s = tmp - extents.y;
			if (s > 0.0f)
			{
				d += s * s;
				if (d > mRadius2) return false;
			}
		}

		tmp = mCenter.z - center.z;
		s = tmp + extents.z;

		if (s < 0.0f)
		{
			d += s * s;
			if (d > mRadius2) return false;
		}
		else
		{
			s = tmp - extents.z;
			if (s > 0.0f)
			{
				d += s * s;
				if (d > mRadius2) return false;
			}
		}
		//#endif

#ifdef OLDIES
	//	icePoint Min = center - extents;
//	icePoint Max = center + extents;

		float d = 0.0f;

		//find the square of the distance
		//from the sphere to the box
		for (udword i = 0; i < 3; i++)
		{
			float Min = center[i] - extents[i];

			//		if(mCenter[i]<Min[i])
			if (mCenter[i] < Min)
			{
				//			float s = mCenter[i] - Min[i];
				float s = mCenter[i] - Min;
				d += s * s;
			}
			else
			{
				float Max = center[i] + extents[i];

				//			if(mCenter[i]>Max[i])
				if (mCenter[i] > Max)
				{
					float s = mCenter[i] - Max;
					d += s * s;
				}
			}
		}
#endif
		return d <= mRadius2;
	}

	inline bool SphereTriOverlap(const icePoint& vert0, const icePoint& vert1, const icePoint& vert2)
	{
		// Stats
		mNbVolumePrimTests++;

		icePoint TriEdge0 = vert1 - vert0;
		icePoint TriEdge1 = vert2 - vert0;

		icePoint kDiff = vert0 - mCenter;
		float fA00 = TriEdge0.SquareMagnitude();
		float fA01 = TriEdge0 | TriEdge1;
		float fA11 = TriEdge1.SquareMagnitude();
		float fB0 = kDiff | TriEdge0;
		float fB1 = kDiff | TriEdge1;
		float fC = kDiff.SquareMagnitude();
		float fDet = _abs(fA00 * fA11 - fA01 * fA01);
		float u = fA01 * fB1 - fA11 * fB0;
		float v = fA01 * fB0 - fA00 * fB1;
		float SqrDist;

		if (u + v <= fDet)
		{
			if (u < 0.0f)
			{
				if (v < 0.0f) // region 4
				{
					if (fB0 < 0.0f)
					{
						v = 0.0f;
						if (-fB0 >= fA00)
						{
							u = 1.0f;
							SqrDist = fA00 + 2.0f * fB0 + fC;
						}
						else
						{
							u = -fB0 / fA00;
							SqrDist = fB0 * u + fC;
						}
					}
					else
					{
						u = 0.0f;
						if (fB1 >= 0.0f)
						{
							v = 0.0f;
							SqrDist = fC;
						}
						else if (-fB1 >= fA11)
						{
							v = 1.0f;
							SqrDist = fA11 + 2.0f * fB1 + fC;
						}
						else
						{
							v = -fB1 / fA11;
							SqrDist = fB1 * v + fC;
						}
					}
				}
				else // region 3
				{
					u = 0.0f;
					if (fB1 >= 0.0f)
					{
						v = 0.0f;
						SqrDist = fC;
					}
					else if (-fB1 >= fA11)
					{
						v = 1.0f;
						SqrDist = fA11 + 2.0f * fB1 + fC;
					}
					else
					{
						v = -fB1 / fA11;
						SqrDist = fB1 * v + fC;
					}
				}
			}
			else if (v < 0.0f) // region 5
			{
				v = 0.0f;
				if (fB0 >= 0.0f)
				{
					u = 0.0f;
					SqrDist = fC;
				}
				else if (-fB0 >= fA00)
				{
					u = 1.0f;
					SqrDist = fA00 + 2.0f * fB0 + fC;
				}
				else
				{
					u = -fB0 / fA00;
					SqrDist = fB0 * u + fC;
				}
			}
			else // region 0
			{
				// minimum at interior point
				if (fDet == 0.0f)
				{
					u = 0.0f;
					v = 0.0f;
					SqrDist = flt_max;
				}
				else
				{
					float fInvDet = 1.0f / fDet;
					u *= fInvDet;
					v *= fInvDet;
					SqrDist = u * (fA00 * u + fA01 * v + 2.0f * fB0) + v * (fA01 * u + fA11 * v + 2.0f * fB1) + fC;
				}
			}
		}
		else
		{
			float fTmp0, fTmp1, fNumer, fDenom;

			if (u < 0.0f) // region 2
			{
				fTmp0 = fA01 + fB0;
				fTmp1 = fA11 + fB1;
				if (fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00 - 2.0f * fA01 + fA11;
					if (fNumer >= fDenom)
					{
						u = 1.0f;
						v = 0.0f;
						SqrDist = fA00 + 2.0f * fB0 + fC;
					}
					else
					{
						u = fNumer / fDenom;
						v = 1.0f - u;
						SqrDist = u * (fA00 * u + fA01 * v + 2.0f * fB0) + v * (fA01 * u + fA11 * v + 2.0f * fB1) + fC;
					}
				}
				else
				{
					u = 0.0f;
					if (fTmp1 <= 0.0f)
					{
						v = 1.0f;
						SqrDist = fA11 + 2.0f * fB1 + fC;
					}
					else if (fB1 >= 0.0f)
					{
						v = 0.0f;
						SqrDist = fC;
					}
					else
					{
						v = -fB1 / fA11;
						SqrDist = fB1 * v + fC;
					}
				}
			}
			else if (v < 0.0f) // region 6
			{
				fTmp0 = fA01 + fB1;
				fTmp1 = fA00 + fB0;
				if (fTmp1 > fTmp0)
				{
					fNumer = fTmp1 - fTmp0;
					fDenom = fA00 - 2.0f * fA01 + fA11;
					if (fNumer >= fDenom)
					{
						v = 1.0f;
						u = 0.0f;
						SqrDist = fA11 + 2.0f * fB1 + fC;
					}
					else
					{
						v = fNumer / fDenom;
						u = 1.0f - v;
						SqrDist = u * (fA00 * u + fA01 * v + 2.0f * fB0) + v * (fA01 * u + fA11 * v + 2.0f * fB1) + fC;
					}
				}
				else
				{
					v = 0.0f;
					if (fTmp1 <= 0.0f)
					{
						u = 1.0f;
						SqrDist = fA00 + 2.0f * fB0 + fC;
					}
					else if (fB0 >= 0.0f)
					{
						u = 0.0f;
						SqrDist = fC;
					}
					else
					{
						u = -fB0 / fA00;
						SqrDist = fB0 * u + fC;
					}
				}
			}
			else // region 1
			{
				fNumer = fA11 + fB1 - fA01 - fB0;
				if (fNumer <= 0.0f)
				{
					u = 0.0f;
					v = 1.0f;
					SqrDist = fA11 + 2.0f * fB1 + fC;
				}
				else
				{
					fDenom = fA00 - 2.0f * fA01 + fA11;
					if (fNumer >= fDenom)
					{
						u = 1.0f;
						v = 0.0f;
						SqrDist = fA00 + 2.0f * fB0 + fC;
					}
					else
					{
						u = fNumer / fDenom;
						v = 1.0f - u;
						SqrDist = u * (fA00 * u + fA01 * v + 2.0f * fB0) + v * (fA01 * u + fA11 * v + 2.0f * fB1) + fC;
					}
				}
			}
		}

		return _abs(SqrDist) < mRadius2;
	}

protected:
	// Sphere in model space
	icePoint mCenter; //!< Sphere center
	float mRadius2; //!< Sphere radius squared
	// Internal methods
	void _Collide(const AABBCollisionNode* node);
	void _Collide(const AABBNoLeafNode* node);
	void _Collide(const AABBQuantizedNode* node);
	void _Collide(const AABBQuantizedNoLeafNode* node);
	void _Collide(const AABBTreeNode* node);
	// Overlap tests
	inline bool SphereContainsBox(const icePoint& bc, const icePoint& be);
	// Init methods
	bool InitQuery(SphereCache& cache, const Sphere& sphere, const Matrix4x4* worlds = null,
	               const Matrix4x4* worldm = null);
};

#endif // __OPC_SPHERECOLLIDER_H__
