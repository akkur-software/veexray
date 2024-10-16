///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains common classes & defs used in OPCODE.
 *	\file		OPC_Common.h
 *	\author		Pierre Terdiman
 *	\date		March, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_COMMON_H__
#define __OPC_COMMON_H__

// [GOTTFRIED]: Just a small change for readability.
#ifdef OPC_CPU_COMPARE
#define GREATER(x, y)	AIR(x) > IR(y)
#else
	#define GREATER(x, y)	fabsf(x) > (y)
#endif

struct VertexPointers
{
	const icePoint* Vertex[3];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	User-callback, called by OPCODE to request vertices from the app.
 *	\param		triangle_index	[in] face index for which the system is requesting the vertices
 *	\param		triangle		[out] triangle's vertices (must be provided by the user)
 *	\param		user_data		[in] user-defined data from SetCallback()
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (*OPC_CALLBACK)(udword triangle_index, VertexPointers& triangle, udword user_data);

class CollisionAABB
{
public:
	//! Constructor
	inline CollisionAABB()
	{
	}

	//! Constructor
	inline CollisionAABB(const AABB& b)
	{
		b.GetCenter(mCenter);
		b.GetExtents(mExtents);
	}

	//! Destructor
	inline ~CollisionAABB()
	{
	}

	//! Get component of the box's min point along a given axis
	inline float GetMin(udword axis) const { return ((const float*)mCenter)[axis] - ((const float*)mExtents)[axis]; }
	//! Get component of the box's max point along a given axis
	inline float GetMax(udword axis) const { return ((const float*)mCenter)[axis] + ((const float*)mExtents)[axis]; }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Checks a box is inside another box.
	 *	\param		box		[in] the other box
	 *	\return		true if current box is inside input box
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline bool IsInside(const CollisionAABB& box) const
	{
		if (box.GetMin(0) > GetMin(0)) return false;
		if (box.GetMin(1) > GetMin(1)) return false;
		if (box.GetMin(2) > GetMin(2)) return false;
		if (box.GetMax(0) < GetMax(0)) return false;
		if (box.GetMax(1) < GetMax(1)) return false;
		if (box.GetMax(2) < GetMax(2)) return false;
		return true;
	}

	icePoint mCenter; //!< Box center
	icePoint mExtents; //!< Box extents
};

class QuantizedAABB
{
public:
	//! Constructor
	inline QuantizedAABB()
	{
	}

	//! Destructor
	inline ~QuantizedAABB()
	{
	}

	sword mCenter[3]; //!< Quantized center
	uword mExtents[3]; //!< Quantized extents
};

class CollisionFace
{
public:
	//! Constructor
	inline CollisionFace()
	{
	}

	//! Destructor
	inline ~CollisionFace()
	{
	}

	udword mFaceID; //!< Index of touched face
	float mDistance; //!< Distance from collider to hitpoint
	float mU, mV; //!< Impact barycentric coordinates
};

class CollisionFaces : private Container
{
public:
	//! Constructor
	inline CollisionFaces()
	{
	}

	//! Destructor
	inline ~CollisionFaces()
	{
	}

	inline udword GetNbFaces() const { return GetNbEntries() >> 2; }
	inline const CollisionFace* GetFaces() const { return (const CollisionFace*)GetEntries(); }

	inline void Reset() { Container::Reset(); }

	inline void AddFace(const CollisionFace& face) { Add(face.mFaceID).Add(face.mDistance).Add(face.mU).Add(face.mV); }
};

//! Quickly rotates & translates a vector3
inline void TransformPoint(icePoint& dest, const icePoint& source, const Matrix3x3& rot, const icePoint& trans)
{
	dest.x = trans.x + source.x * rot.m[0][0] + source.y * rot.m[1][0] + source.z * rot.m[2][0];
	dest.y = trans.y + source.x * rot.m[0][1] + source.y * rot.m[1][1] + source.z * rot.m[2][1];
	dest.z = trans.z + source.x * rot.m[0][2] + source.y * rot.m[1][2] + source.z * rot.m[2][2];
}

#endif //__OPC_COMMON_H__
