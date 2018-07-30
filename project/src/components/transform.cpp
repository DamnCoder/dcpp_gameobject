/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "transform.h"

#include "gameobject.h"

#include <cassert>

namespace dc
{
	void CTransform::LocalMatrix(const math::Matrix4x4f& matrix)
	{
		m_localMatrix = matrix;
		m_position = LocalPosition();
		m_rotation = LocalRotation();
		m_scale = LocalScale();
	}
	
	const bool CTransform::HasChild(CTransform* transform) const
	{
		for (const auto* child : m_children)
		{
			if (child == transform)
				return true;
		}
		return false;
	}
	
	void CTransform::Parent(CTransform* parent)
	{
		assert(parent && "[CTransform::Parent] You're adding a NULL pointer");
		
		// If it's the same we do nothing
		if(mp_parent == parent)
		{
			return;
		}
		
		// If it already has a parent, first we need to remove ourselves from its list of children
		if(mp_parent)
		{
			mp_parent->Remove(this);
		}
		
		mp_parent = parent;

		// If we are not already a child we add ourselves as child
		if(!mp_parent->HasChild(this))
		{
			mp_parent->Add(this);
		}
		
		// If
		if (!mp_parent->Root())
		{
			Root(mp_parent);
		}
		else
		{
			Root(mp_parent->Root());
		}
		
		CalculateTransforms();
	}

	void CTransform::LocalPosition(const math::Vector3f& position)
	{
		m_position = position;
		CalculateTransforms();
	}

	void CTransform::LocalRotation(const math::Quaternionf& rotation)
	{
		m_rotation = rotation;
		CalculateTransforms();
	}

	void CTransform::LocalScale(const math::Vector3f& scale)
	{
		m_scale = scale;
		CalculateTransforms();
	}
	
	void CTransform::Root(CTransform* root)
	{
		mp_root = root;
		
		for(auto* child : m_children)
		{
			child->Root(root);
		}
	}
	
	void CTransform::Reset()
	{
		m_localMatrix.Identify();
		m_globalMatrix.Identify();
		m_scale = math::Vector3f::One();
		m_rotation.Identity();
		mp_root = 0;
		mp_parent = 0;
		m_children.clear();
	}
	
	math::Vector3f CTransform::TransformPosition(const math::Vector3f& point)
	{
		return m_localMatrix.TransformPosition(point);
	}
	
	void CTransform::Add(CTransform* child)
	{
		assert(child && "[CTransform::Add] You're adding a NULL pointer");
		m_children.push_back(child);
		child->Parent(this);
	}
	
	void CTransform::Remove(CTransform* child)
	{
		assert(child && "[CTransform::Remove] You're removing a NULL pointer");
		m_children.erase(std::find(Begin(), End(), child));
	}
	
	CTransform* CTransform::FindChild(const char* name)
	{
		CTransform* found = 0;
		for(auto* child : m_children)
		{
			if(std::strcmp(name, child->GameObject()->Name()) == 0)
				return child;
			
			if(child->HasChildren())
			{
				found = child->FindChild(name);
				if(found)
					return found;
			}
		}
		return found;
	}

	void CTransform::CalculateLocalTransform()
	{
		/*
		glm::mat4 translation = glm::translate(m_position);
		glm::mat4 rotation = glm::toMat4(m_rotation);
		glm::mat4 scale = glm::scale(m_scale);
		m_localMatrix = translation * rotation * scale;
		*/
	}
	
	void CTransform::CalculateWorldTransform()
	{
		if(HasParent())
		{
			mp_parent->CalculateWorldTransform();
			m_globalMatrix = mp_parent->WorldMatrix() * m_localMatrix;
		}
		else
		{
			m_globalMatrix = m_localMatrix;
		}
	}
	
	void CTransform::CalculateTransforms()
	{
		CalculateLocalTransform();
		CalculateWorldTransform();
		
		for(auto* child: m_children)
		{
			child->CalculateTransforms();
		}
	}
	
	void PrintLocalMatrix(const CTransform* transform)
	{
		printf("ROOT\n");
		printf("GO %s\n", transform->GameObject()->Name());
		PrintMat4(transform->LocalMatrix());
		
		printf("CHILDREN\n");
		for(auto* child : transform->Children())
		{
			printf("GO %s\n", child->GameObject()->Name());

			PrintMat4(child->LocalMatrix());
			PrintLocalMatrix(child);
		}
	}

	void PrintMat4(const math::Matrix4x4f& matrix)
	{
		math::PrintMatrix(matrix);
	}
}
