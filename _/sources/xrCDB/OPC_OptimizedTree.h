///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 *	OPCODE - Optimized Collision Detection
 *	Copyright (C) 2001 Pierre Terdiman
 *	Homepage: http://www.codercorner.com/Opcode.htm
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for optimized trees.
 *	\file		OPC_OptimizedTree.h
 *	\author		Pierre Terdiman
 *	\date		March, 20, 2001
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef __OPC_OPTIMIZEDTREE_H__
#define __OPC_OPTIMIZEDTREE_H__

//! Common interface for a node of an implicit tree
#define IMPLEMENT_IMPLICIT_NODE(baseclass, volume)													\
		public:																							\
		/* Constructor / Destructor */																	\
		inline								baseclass() : mData(0)	{}									\
		inline								~baseclass()			{}									\
		/* Leaf test */																					\
		inline			bool				IsLeaf()		const	{ return (bool)(mData&1);		}	\
		/* Data access */																				\
		inline			const baseclass*	GetPos()		const	{ return (baseclass*)mData;		}	\
		inline			const baseclass*	GetNeg()		const	{ return ((baseclass*)mData)+1;	}	\
		inline			udword				GetPrimitive()	const	{ return udword(mData>>1);		}	\
		/* Stats */																						\
		inline			size_t				GetNodeSize()	const	{ return SIZEOFOBJECT;			}	\
																										\
						volume				mAABB;														\
						uintptr_t			mData;

//! Common interface for a node of a no-leaf tree
#define IMPLEMENT_NOLEAF_NODE(baseclass, volume)													\
		public:																							\
		/* Constructor / Destructor */																	\
		inline								baseclass() : mData(0), mData2(0)	{}						\
		inline								~baseclass()						{}						\
		/* Leaf tests */																				\
		inline			bool				HasLeaf()		const	{ return (bool)(mData&1);		}	\
		inline			bool				HasLeaf2()		const	{ return (bool)(mData2&1);		}	\
		/* Data access */																				\
		inline			const baseclass*	GetPos()		const	{ return (baseclass*)mData;		}	\
		inline			const baseclass*	GetNeg()		const	{ return (baseclass*)mData2;	}	\
		inline			udword				GetPrimitive()	const	{ return udword(mData>>1);		}	\
		inline			udword				GetPrimitive2()	const	{ return udword(mData2>>1);		}	\
		/* Stats */																						\
		inline			size_t				GetNodeSize()	const	{ return SIZEOFOBJECT;			}	\
																										\
						volume				mAABB;														\
						uintptr_t			mData;														\
						uintptr_t			mData2;

class AABBCollisionNode
{
IMPLEMENT_IMPLICIT_NODE(AABBCollisionNode, CollisionAABB)

	inline float GetVolume() const { return mAABB.mExtents.x * mAABB.mExtents.y * mAABB.mExtents.z; }
	inline float GetSize() const { return mAABB.mExtents.SquareMagnitude(); }
	inline udword GetRadius() const
	{
		udword* Bits = (udword*)&mAABB.mExtents.x;
		udword Max = Bits[0];
		if (Bits[1] > Max) Max = Bits[1];
		if (Bits[2] > Max) Max = Bits[2];
		return Max;
	}

	// NB: using the square-magnitude or the true volume of the box, seems to yield better results
	// (assuming UNC-like informed traversal methods). I borrowed this idea from PQP. The usual "size"
	// otherwise, is the largest box extent. In SOLID that extent is computed on-the-fly each time it's
	// needed (the best approach IMHO). In RAPID the rotation matrix is permuted so that Extent[0] is
	// always the greatest, which saves looking for it at runtime. On the other hand, it yields matrices
	// whose determinant is not 1, i.e. you can't encode them anymore as unit quaternions. Not a very
	// good strategy.
};

class AABBQuantizedNode
{
IMPLEMENT_IMPLICIT_NODE(AABBQuantizedNode, QuantizedAABB)

	inline uword GetSize() const
	{
		const uword* Bits = mAABB.mExtents;
		uword Max = Bits[0];
		if (Bits[1] > Max) Max = Bits[1];
		if (Bits[2] > Max) Max = Bits[2];
		return Max;
	}

	// NB: for quantized nodes I don't feel like computing a square-magnitude with integers all
	// over the place.......!
};

class AABBNoLeafNode
{
IMPLEMENT_NOLEAF_NODE(AABBNoLeafNode, CollisionAABB)
};

class AABBQuantizedNoLeafNode
{
IMPLEMENT_NOLEAF_NODE(AABBQuantizedNoLeafNode, QuantizedAABB)
};

//! Common interface for a collision tree
#define IMPLEMENT_COLLISION_TREE(baseclass, volume)														\
		public:																								\
		/* Constructor / Destructor */																		\
											baseclass();													\
		virtual								~baseclass();													\
		/* Build from a standard tree */																	\
		virtual			bool				Build(AABBTree* tree);											\
		/* Data access */																					\
		inline			const volume*		GetNodes()		const	{ return mNodes;					}	\
    void* GetData() const override { return static_cast<void*>(mNodes); }\
    void SetData(void* ptr, udword nbNodes) override { mNodes = static_cast<volume*>(ptr); mNbNodes = nbNodes; }\
		/* Stats */																							\
		virtual			udword				GetUsedBytes()	const	{ return mNbNodes*sizeof(volume);	}	\
		private:																							\
						volume*				mNodes;

class AABBOptimizedTree
{
public:
	// Constructor / Destructor
	AABBOptimizedTree() : mNbNodes(0)
	{
	}

	virtual ~AABBOptimizedTree()
	{
	}

	// Data access
	inline udword GetNbNodes() const { return mNbNodes; }

	virtual void* GetData() const = 0;
	virtual void SetData(void* ptr, udword nbNodes) = 0;

	virtual udword GetUsedBytes() const = 0;
	virtual bool Build(AABBTree* tree) = 0;
protected:
	udword mNbNodes;
};

class AABBCollisionTree : public AABBOptimizedTree
{
IMPLEMENT_COLLISION_TREE(AABBCollisionTree, AABBCollisionNode)
};

class AABBNoLeafTree : public AABBOptimizedTree
{
public: 
	AABBNoLeafTree(); 
	virtual ~AABBNoLeafTree(); 
	virtual bool Build(AABBTree* tree); 
	inline const AABBNoLeafNode* GetNodes() const
	{
		return mNodes;
	} 
	
	void* GetData() const override
	{
		return static_cast<void*>(mNodes);
	} 

	void SetData(void* ptr, udword nbNodes) override
	{
		mNodes = static_cast<AABBNoLeafNode*>(ptr); mNbNodes = nbNodes;
	} 

	virtual udword GetUsedBytes() const
	{
		return mNbNodes * sizeof(AABBNoLeafNode);
	} 

private: 
	AABBNoLeafNode* mNodes;
};

class AABBQuantizedTree : public AABBOptimizedTree
{
public: 
	AABBQuantizedTree(); 
	virtual ~AABBQuantizedTree(); 
	virtual bool Build(AABBTree* tree); 

	inline const AABBQuantizedNode* GetNodes() const
	{
		return mNodes;
	} 
	
	void* GetData() const override
	{
		return static_cast<void*>(mNodes);
	} 

	void SetData(void* ptr, udword nbNodes) override
	{
		mNodes = static_cast<AABBQuantizedNode*>(ptr); mNbNodes = nbNodes;
	} 

	virtual udword GetUsedBytes() const
	{
		return mNbNodes * sizeof(AABBQuantizedNode);
	} 

private: 
	AABBQuantizedNode* mNodes;

public:
	icePoint mCenterCoeff;
	icePoint mExtentsCoeff;
};

class AABBQuantizedNoLeafTree : public AABBOptimizedTree
{
public: 
	AABBQuantizedNoLeafTree(); 

	virtual ~AABBQuantizedNoLeafTree(); 
	virtual bool Build(AABBTree* tree); 
	inline const AABBQuantizedNoLeafNode* GetNodes() const
{
	return mNodes;
} void* GetData() const override
{
	return static_cast<void*>(mNodes);
} void SetData(void* ptr, udword nbNodes) override
{
	mNodes = static_cast<AABBQuantizedNoLeafNode*>(ptr); mNbNodes = nbNodes;
} virtual udword GetUsedBytes() const
{
	return mNbNodes * sizeof(AABBQuantizedNoLeafNode);
} private: AABBQuantizedNoLeafNode* mNodes;

public:
	icePoint mCenterCoeff;
	icePoint mExtentsCoeff;
};

#endif // __OPC_OPTIMIZEDTREE_H__
