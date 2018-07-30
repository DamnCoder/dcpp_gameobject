/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
//  component.h
//  DCPP
//
//  Created by Jorge López on 26/12/12.
//
//

#pragma once

#include "types/rtti.h"

#include <map>
#include <vector>

namespace dc
{
	// ===========================================================
	// External Enums / Typedefs for global usage
	// ===========================================================
	class CGameObject;
	
	/**
	 * \class CComponent
	 * \brief
	 * \author Jorge López González
	 *
	 * Component base class and interface
	 */
	class CComponent
	{
		// ===========================================================
		// Constant / Enums / Typedefs internal usage
		// ===========================================================
		RTTI_BASE_DECLARATIONS(CComponent)
		
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
		// Getters / Setters
		CGameObject*	GameObject() const					{ return mp_gameObject; }
		void			GameObject(CGameObject* gameObject)	{ mp_gameObject = gameObject; }

		template<typename ComponentType>
		ComponentType*	GetComponent() const
		{
			return mp_gameObject->GetComponent<ComponentType>();
		}

		// ===========================================================
		// Constructors
		// ===========================================================
	public:
		CComponent():
			mp_gameObject(0)
		{}
		
		virtual ~CComponent() {}
		
		CComponent(const CComponent& copy) = delete;
		void operator= (const CComponent& copy) = delete;
		
		// ===========================================================
		// Methods for/from SuperClass/Interfaces
		// ===========================================================
	
		// ===========================================================
		// Methods for/from SuperClass/Interfaces
		// ===========================================================
	public:
		virtual void Awake() {}
		virtual void Start() {}
		
		virtual void Update() {}

		virtual void Finish() {}
		virtual void Sleep() {}
		
		// ===========================================================
		// Methods
		// ===========================================================
		
		// ===========================================================
		// Fields
		// ===========================================================
	private:
		CGameObject*	mp_gameObject;
	};
	
	// ===========================================================
	// Class typedefs
	// ===========================================================
	
	using TComponentList		= std::vector<CComponent*>;
	using TComponentListTable	= std::map<const char*, TComponentList>;
}