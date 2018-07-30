/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
//  transform.h
//  DCPP
//
//  Created by Jorge López on 27/12/12.
//
//

#pragma once

#include "math/matrix.h"

#include "component.h"

namespace dc
{
	// ===========================================================
	// External Enums / Typedefs for global usage
	// ===========================================================
	
	class CTransform;
	
	using TTransformList = std::vector<CTransform*>;
	using TTransformIterator = TTransformList::iterator;
	
	/**
	 * \class
	 * \brief
	 * \author Jorge Lopez Gonzalez
	 *
	 * Transform component.
	 */
	class CTransform : public CComponent
	{
		// ===========================================================
		// Constant / Enums / Typedefs internal usage
		// ===========================================================
		RTTI_DECLARATIONS(CTransform, CComponent)
		
		// ===========================================================
		// Static fields / methods
		// ===========================================================
		
		// ===========================================================
		// Inner and Anonymous Classes
		// ===========================================================
		
		// ===========================================================
		// Getter & Setter
		// ===========================================================
	public:
		void						LocalMatrix(const math::Matrix4x4f& matrix);
		const math::Matrix4x4f&		LocalMatrix() const { return m_localMatrix; }
		const math::Matrix4x4f&		WorldMatrix() const { return m_globalMatrix; }
		
		const bool				HasChild(CTransform* transform) const;
		const bool				HasChildren() const	{ return !m_children.empty(); }
		const unsigned int		ChildCount() const	{ return m_children.size(); }
		const TTransformList	Children() const	{ return m_children; }
		
		TTransformIterator		Begin()	{ return m_children.begin(); }
		TTransformIterator		End() { return m_children.end(); }
		
		CTransform*				Root() const { return mp_root; }
		
		const bool				HasParent() const { return mp_parent != 0; }
		CTransform*				Parent() const { return mp_parent; }
		void					Parent(CTransform* parent);
		
		math::Vector3f			Position() { return m_globalMatrix.Position(); }

		math::Vector3f			LocalPosition() const { return m_localMatrix.Position(); }
		math::Quaternionf		LocalRotation() const { return m_localMatrix.Rotation(); }
		math::Vector3f			LocalScale()	const { return m_localMatrix.Scale(); }

		void					LocalPosition(const math::Vector3f& position);
		void					LocalRotation(const math::Quaternionf& rotation);
		void					LocalScale(const math::Vector3f& scale);
		
	private:
		void					Root(CTransform* root);
		
		// ===========================================================
		// Constructors
		// ===========================================================
	public:
		CTransform()
		{
			Reset();
		}
		
		~CTransform() {}

		CTransform(const CTransform& copy) = delete;
		void operator= (const CTransform& copy) = delete;

		// ===========================================================
		// Methods for/from SuperClass/Interfaces
		// ===========================================================
		
		// ===========================================================
		// Methods
		// ===========================================================
	public:
		void Reset();
		
		void Add(CTransform* child);
		void Remove(CTransform* child);
		
		CTransform* FindChild(const char* name);
		
		// Transforms position from local space to world space.
		math::Vector3f TransformPosition(const math::Vector3f& point);
		
	private:
		void CalculateLocalTransform();
		void CalculateWorldTransform();
		
		void CalculateTransforms();
		
		// ===========================================================
		// Fields
		// ===========================================================
	private:
		CTransform*		mp_root;		// Topmost transform in the hierarchy
		CTransform*		mp_parent;		// Transform parent

		math::Matrix4x4f		m_localMatrix;
		math::Matrix4x4f		m_globalMatrix;
		
		math::Vector3f		m_position;		// Local position
		math::Vector3f		m_scale;		// Local scale
		math::Quaternionf	m_rotation;		// Local rotaion
		TTransformList	m_children;
	};
	// ===========================================================
	// Class typedefs
	// ===========================================================
	
	// ===========================================================
	// Template/Inline implementation
	// ===========================================================
	
	void PrintLocalMatrix(const CTransform* transform);
	void PrintMat4(const math::Matrix4x4f& matrix);
}