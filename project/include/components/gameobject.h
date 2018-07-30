/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
//  gameobject.h
//  DCPP
//
//  Created by Jorge López on 26/12/12.
//
//

#pragma once

#include "component.h"

namespace dc
{
	// ===========================================================
	// External Enums / Typedefs for global usage
	// ===========================================================

	class CTransform;

	/**
	 * \class CGameObject
	 * \brief
	 * \author Jorge López González
	 *
	 * Implementation of GameObject component container
	 */
	class CGameObject
	{
		// ===========================================================
		// Constant / Enums / Typedefs internal usage
		// ===========================================================

		// ===========================================================
		// Static fields / methods
		// ===========================================================
	public:
		static CGameObject* Instantiate()
		{
			CGameObject* gameObject = new CGameObject("GameObject");
			return gameObject;
		}
		
		// ===========================================================
		// Inner and Anonymous Classes
		// ===========================================================
		
		// ===========================================================
		// Getter & Setter
		// ===========================================================
	public:
		const char*					Name() const					{ return mp_name; }
		void						Name(const char* name)			{ mp_name = name; }
		
		CTransform*					Transform() const				{ return mp_transform; }
		
		const bool					HasChild(const char* name) const;
		
		const unsigned int			ComponentsNum(const char* compId) const;

		const TComponentListTable&	ComponentsTable() const			{ return m_componentTable; }
		
		/**
		 * Returns the first component of the specified type
		 */
		template<typename ComponentType>
		ComponentType*				GetComponent() const;
		
		/**
		 * Returns all the components of the specified type in an array
		 */
		template<typename ComponentType>
		std::vector<ComponentType*>	GetComponents() const;

	private:
		const TComponentList&		GetComponents(const char* compId) const;
		
		// ===========================================================
		// Constructors
		// ===========================================================
	public:
		CGameObject();
		CGameObject(const char* name);
		
		~CGameObject();
		
		CGameObject(const CGameObject& copy) = delete;
		
		// ===========================================================
		// Methods for/from SuperClass/Interfaces
		// ===========================================================
	public:
		void operator= (const CGameObject& copy) = delete;
		
		// ===========================================================
		// Methods
		// ===========================================================
	public:
		/**
		 * Creates a component and adds it to the GameObject
		 */
		CComponent* AddComponent(CComponent* component);

		template<typename ComponentType, typename ...Args>
		ComponentType* AddComponent(Args... args);
		
		/**
		 * Removes a component of the specified type from the GameObject and destroys it
		 */
		void RemoveComponent(const char* name);

		template<typename ComponentType>
		void RemoveComponent();
		
		CGameObject* FindChild(const char* name) const;

		// ===========================================================
		// Fields
		// ===========================================================
	private:
		const char*			mp_name;
		CTransform*			mp_transform;
		TComponentListTable	m_componentTable;
	};
	
	// ===========================================================
	// Class typedefs
	// ===========================================================
	using TGOList = std::vector<CGameObject*>;
	
	// ===========================================================
	// Template/Inline implementation
	// ===========================================================

	template<typename ComponentType>
	ComponentType* CGameObject::GetComponent() const
	{
		const TComponentList& componentList = GetComponents(ComponentType::TypeName());
		return componentList.front()->DirectCast<ComponentType>();
	}
	
	template<typename ComponentType>
	std::vector<ComponentType*> CGameObject::GetComponents() const
	{
		const TComponentList& componentList = GetComponents(ComponentType::TypeName());
		
		std::vector<ComponentType*> castedComponentList;
		castedComponentList.reserve(componentList.size());
		
		for(CComponent* component : componentList)
		{
			ComponentType* castedComponent = component->DirectCast<ComponentType>();
			castedComponentList.push_back(castedComponent);
		}
		
		return castedComponentList;
	}
	
	template<typename ComponentType, typename ...Args>
	ComponentType* CGameObject::AddComponent(Args... args)
	{
		ComponentType* component = new ComponentType(std::forward<Args>(args)...);
		AddComponent(component);
		return component;
	}
	
	template<typename ComponentType>
	void CGameObject::RemoveComponent()
	{
		CComponent* component = RemoveComponent(ComponentType::TypeName());
		if(component)
		{
			delete component;
		}
	}
}
