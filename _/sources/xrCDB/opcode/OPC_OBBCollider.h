///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for an OBB collider.
 *	\file		OPC_OBBCollider.h
 *	\author		Pierre Terdiman
 *	\date		January, 1st, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_OBBCOLLIDER_H__
#define __OPC_OBBCOLLIDER_H__

#include "OPC_Types.h"

struct OBBCache : VolumeCache
{
	OBBCache()
	{
	}
};

class OBBCollider : public VolumeCollider
{
public:
	// Constructor / Destructor
	OBBCollider();
	virtual ~OBBCollider();
	// Generic collision query

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Generic collision query for generic OPCODE models. After the call, access the results:
	 *	- with GetContactStatus()
	 *	- with GetNbTouchedFaces()
	 *	- with GetTouchedFaces()
	 *
	 *	\param		cache			[in/out] a box cache
	 *	\param		box				[in] collision OBB in local space
	 *	\param		model			[in] Opcode model to collide with
	 *	\param		worldb			[in] OBB's world matrix, or null
	 *	\param		worldm			[in] model's world matrix, or null
	 *	\return		true if success
	 *	\warning	SCALE NOT SUPPORTED. The matrices must contain rotation & translation parts only.
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Collide(OBBCache& cache, const OBB& box, OPCODE_Model* model, const Matrix4x4* worldb = null,
	             const Matrix4x4* worldm = null);

	// Collision queries
	bool Collide(OBBCache& cache, const OBB& box, const AABBCollisionTree* tree, const Matrix4x4* worldb = null,
	             const Matrix4x4* worldm = null);
	bool Collide(OBBCache& cache, const OBB& box, const AABBNoLeafTree* tree, const Matrix4x4* worldb = null,
	             const Matrix4x4* worldm = null);
	bool Collide(OBBCache& cache, const OBB& box, const AABBQuantizedTree* tree, const Matrix4x4* worldb = null,
	             const Matrix4x4* worldm = null);
	bool Collide(OBBCache& cache, const OBB& box, const AABBQuantizedNoLeafTree* tree, const Matrix4x4* worldb = null,
	             const Matrix4x4* worldm = null);
	// Settings

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Settings: select between full box-box tests or "SAT-lite" tests (where Class III axes are discarded)
	 *	\param		flag		[in] true for full tests, false for coarse tests
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline void SetFullBoxBoxTest(bool flag) { mFullBoxBoxTest = flag; }

	// Settings

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Validates current settings. You should call this method after all the settings and callbacks have been defined for a collider.
	 *	\return		null if everything is ok, else a string describing the problem
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const char* ValidateSettings();

	//! A dedicated version when one box is constant
	inline bool BoxBoxOverlap(const icePoint& extents, const icePoint& center)
	{
		// Stats
		mNbVolumeBVTests++;

		float t, t2;

		// Class I : A's basis vectors
		float Tx = mTBoxToModel.x - center.x;
		t = extents.x + mBBx1;
		if (GREATER(Tx, t)) return false;
		float Ty = mTBoxToModel.y - center.y;
		t = extents.y + mBBy1;
		if (GREATER(Ty, t)) return false;
		float Tz = mTBoxToModel.z - center.z;
		t = extents.z + mBBz1;
		if (GREATER(Tz, t)) return false;

		// Class II : B's basis vectors
		t = Tx * mRBoxToModel.m[0][0] + Ty * mRBoxToModel.m[0][1] + Tz * mRBoxToModel.m[0][2];
		t2 = extents.x * mAR.m[0][0] + extents.y * mAR.m[0][1] + extents.z * mAR.m[0][2] + mBoxExtents.x;
		if (GREATER(t, t2)) return false;

		t = Tx * mRBoxToModel.m[1][0] + Ty * mRBoxToModel.m[1][1] + Tz * mRBoxToModel.m[1][2];
		t2 = extents.x * mAR.m[1][0] + extents.y * mAR.m[1][1] + extents.z * mAR.m[1][2] + mBoxExtents.y;
		if (GREATER(t, t2)) return false;

		t = Tx * mRBoxToModel.m[2][0] + Ty * mRBoxToModel.m[2][1] + Tz * mRBoxToModel.m[2][2];
		t2 = extents.x * mAR.m[2][0] + extents.y * mAR.m[2][1] + extents.z * mAR.m[2][2] + mBoxExtents.z;
		if (GREATER(t, t2)) return false;

		// Class III : 9 cross products
		// Cool trick: always perform the full test for first level, regardless of settings.
		// That way pathological cases (such as the pencils scene) are quickly rejected anyway !
		if (mFullBoxBoxTest || mNbVolumeBVTests == 1)
		{
			t = Tz * mRBoxToModel.m[0][1] - Ty * mRBoxToModel.m[0][2];
			t2 = extents.y * mAR.m[0][2] + extents.z * mAR.m[0][1] + mBB_1;
			if (GREATER(t, t2)) return false; // L = A0 x B0
			t = Tz * mRBoxToModel.m[1][1] - Ty * mRBoxToModel.m[1][2];
			t2 = extents.y * mAR.m[1][2] + extents.z * mAR.m[1][1] + mBB_2;
			if (GREATER(t, t2)) return false; // L = A0 x B1
			t = Tz * mRBoxToModel.m[2][1] - Ty * mRBoxToModel.m[2][2];
			t2 = extents.y * mAR.m[2][2] + extents.z * mAR.m[2][1] + mBB_3;
			if (GREATER(t, t2)) return false; // L = A0 x B2
			t = Tx * mRBoxToModel.m[0][2] - Tz * mRBoxToModel.m[0][0];
			t2 = extents.x * mAR.m[0][2] + extents.z * mAR.m[0][0] + mBB_4;
			if (GREATER(t, t2)) return false; // L = A1 x B0
			t = Tx * mRBoxToModel.m[1][2] - Tz * mRBoxToModel.m[1][0];
			t2 = extents.x * mAR.m[1][2] + extents.z * mAR.m[1][0] + mBB_5;
			if (GREATER(t, t2)) return false; // L = A1 x B1
			t = Tx * mRBoxToModel.m[2][2] - Tz * mRBoxToModel.m[2][0];
			t2 = extents.x * mAR.m[2][2] + extents.z * mAR.m[2][0] + mBB_6;
			if (GREATER(t, t2)) return false; // L = A1 x B2
			t = Ty * mRBoxToModel.m[0][0] - Tx * mRBoxToModel.m[0][1];
			t2 = extents.x * mAR.m[0][1] + extents.y * mAR.m[0][0] + mBB_7;
			if (GREATER(t, t2)) return false; // L = A2 x B0
			t = Ty * mRBoxToModel.m[1][0] - Tx * mRBoxToModel.m[1][1];
			t2 = extents.x * mAR.m[1][1] + extents.y * mAR.m[1][0] + mBB_8;
			if (GREATER(t, t2)) return false; // L = A2 x B1
			t = Ty * mRBoxToModel.m[2][0] - Tx * mRBoxToModel.m[2][1];
			t2 = extents.x * mAR.m[2][1] + extents.y * mAR.m[2][0] + mBB_9;
			if (GREATER(t, t2)) return false; // L = A2 x B2
		}
		return true;
	}

protected:
	// Precomputed data
	Matrix3x3 mAR; //!< Absolute rotation matrix
	Matrix3x3 mRModelToBox; //!< Rotation from model space to obb space
	Matrix3x3 mRBoxToModel; //!< Rotation from obb space to model space
	icePoint mTModelToBox; //!< Translation from model space to obb space
	icePoint mTBoxToModel; //!< Translation from obb space to model space

	icePoint mBoxExtents;
	icePoint mB0; //!< - mTModelToBox + mBoxExtents
	icePoint mB1; //!< - mTModelToBox - mBoxExtents

	float mBBx1;
	float mBBy1;
	float mBBz1;

	float mBB_1;
	float mBB_2;
	float mBB_3;
	float mBB_4;
	float mBB_5;
	float mBB_6;
	float mBB_7;
	float mBB_8;
	float mBB_9;

	// Leaf description
	icePoint mLeafVerts[3]; //!< Triangle vertices
	// Settings
	bool mFullBoxBoxTest; //!< Perform full BV-BV tests (true) or SAT-lite tests (false)
	// Internal methods
	void _Collide(const AABBCollisionNode* node);
	void _Collide(const AABBNoLeafNode* node);
	void _Collide(const AABBQuantizedNode* node);
	void _Collide(const AABBQuantizedNoLeafNode* node);
	// Overlap tests
	inline bool OBBContainsBox(const icePoint& bc, const icePoint& be);
	inline bool TriBoxOverlap();
	// Init methods
	bool InitQuery(OBBCache& cache, const OBB& box, const Matrix4x4* worldb = null, const Matrix4x4* worldm = null);
};

#endif // __OPC_OBBCOLLIDER_H__
