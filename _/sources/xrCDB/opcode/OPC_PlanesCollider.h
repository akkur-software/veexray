///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for a planes collider.
 *	\file		OPC_PlanesCollider.h
 *	\author		Pierre Terdiman
 *	\date		January, 1st, 2002
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_PLANESCOLLIDER_H__
#define __OPC_PLANESCOLLIDER_H__

struct PlanesCache : VolumeCache
{
	PlanesCache()
	{
	}
};

class PlanesCollider : public VolumeCollider
{
public:
	// Constructor / Destructor
	PlanesCollider();
	virtual ~PlanesCollider();
	// Generic collision query

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Generic collision query for generic OPCODE models. After the call, access the results:
	 *	- with GetContactStatus()
	 *	- with GetNbTouchedFaces()
	 *	- with GetTouchedFaces()
	 *
	 *	\param		cache			[in/out] a planes cache
	 *	\param		planes			[in] list of planes in world space
	 *	\param		nb_planes		[in] number of planes
	 *	\param		model			[in] Opcode model to collide with
	 *	\param		worldm			[in] model's world matrix, or null
	 *	\return		true if success
	 *	\warning	SCALE NOT SUPPORTED. The matrices must contain rotation & translation parts only.
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool Collide(PlanesCache& cache, const Plane* planes, udword nb_planes, OPCODE_Model* model,
	             const Matrix4x4* worldm = null);

	// Collision queries
	bool Collide(PlanesCache& cache, const Plane* planes, udword nb_planes, const AABBCollisionTree* tree,
	             const Matrix4x4* worldm = null);
	bool Collide(PlanesCache& cache, const Plane* planes, udword nb_planes, const AABBNoLeafTree* tree,
	             const Matrix4x4* worldm = null);
	bool Collide(PlanesCache& cache, const Plane* planes, udword nb_planes, const AABBQuantizedTree* tree,
	             const Matrix4x4* worldm = null);
	bool Collide(PlanesCache& cache, const Plane* planes, udword nb_planes, const AABBQuantizedNoLeafTree* tree,
	             const Matrix4x4* worldm = null);

	// Mutant box-with-planes collision queries
	inline bool Collide(PlanesCache& cache, const OBB& box, OPCODE_Model* model, const Matrix4x4* worldb = null,
	                     const Matrix4x4* worldm = null)
	{
		Plane PL[6];

		if (worldb)
		{
			// Create a _new_ OBB in world space
			OBB WorldBox;
			box.Rotate(*worldb, WorldBox);
			// Compute planes from the sides of the box
			WorldBox.ComputePlanes(PL);
		}
		else
		{
			// Compute planes from the sides of the box
			box.ComputePlanes(PL);
		}

		// Collide with box planes
		return Collide(cache, PL, 6, model, worldm);
	}

	inline bool PlanesAABBOverlap(const icePoint& center, const icePoint& extents, udword& out_clip_mask, udword in_clip_mask)
	{
		// Stats
		mNbVolumeBVTests++;

		const Plane* p = mPlanes;

		// Evaluate through all active frustum planes. We determine the relation 
		// between the AABB and a plane by using the concept of "near" and "far"
		// vertices originally described by Zhang (and later by Möller). Our
		// variant here uses 3 fabs ops, 6 muls, 7 adds and two floating point
		// comparisons per plane. The routine early-exits if the AABB is found
		// to be outside any of the planes. The loop also constructs a _new_ output
		// clip mask. Most FPUs have a native single-cycle _abs() operation.

		udword Mask = 1; // current mask index (1,2,4,8,..)
		udword TmpOutClipMask = 0; // initialize output clip mask into empty. 

		while (Mask <= in_clip_mask) // keep looping while we have active planes left...
		{
			if (in_clip_mask & Mask) // if clip plane is active, process it..
			{
				float NP = extents.x * _abs(p->n.x) + extents.y * _abs(p->n.y) + extents.z * _abs(p->n.z);
				// ### _abs could be precomputed
				float MP = center.x * p->n.x + center.y * p->n.y + center.z * p->n.z + p->d;

				if (NP < MP) // near vertex behind the clip plane... 
					return false; // .. so there is no intersection..
				if ((-NP) < MP) // near and far vertices on different sides of plane..
					TmpOutClipMask |= Mask; // .. so update the clip mask...
			}
			Mask += Mask; // mk = (1<<plane)
			p++; // advance to next plane
		}

		out_clip_mask = TmpOutClipMask; // copy output value (temp used to resolve aliasing!)
		return true; // indicate that AABB intersects frustum
	}

	inline bool PlanesTriOverlap(udword in_clip_mask)
	{
		// Stats
		mNbVolumePrimTests++;

		const Plane* p = mPlanes;
		udword Mask = 1;

		while (Mask <= in_clip_mask)
		{
			if (in_clip_mask & Mask)
			{
				float d0 = p->Distance(*mVP.Vertex[0]);
				float d1 = p->Distance(*mVP.Vertex[1]);
				float d2 = p->Distance(*mVP.Vertex[2]);
				if (d0 > 0.0f && d1 > 0.0f && d2 > 0.0f) return false;
			}
			Mask += Mask;
			p++;
		}
		return true;
	}

	// Settings

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/**
	 *	Validates current settings. You should call this method after all the settings and callbacks have been defined for a collider.
	 *	\return		null if everything is ok, else a string describing the problem
	 */
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual const char* ValidateSettings();

protected:
	// Planes in model space
	udword mNbPlanes;
	Plane* mPlanes;
	// Leaf description
	VertexPointers mVP;
	// Internal methods
	void _Collide(const AABBCollisionNode* node, udword clipmask);
	void _Collide(const AABBNoLeafNode* node, udword clipmask);
	void _Collide(const AABBQuantizedNode* node, udword clipmask);
	void _Collide(const AABBQuantizedNoLeafNode* node, udword clipmask);
	// Init methods
	bool InitQuery(PlanesCache& cache, const Plane* planes, udword nb_planes, const Matrix4x4* worldm = null);
};

#endif // __OPC_PLANESCOLLIDER_H__
