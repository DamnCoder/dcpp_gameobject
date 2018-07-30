/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

//
//  scene.h
//  DCPP
//
//  Created by Jorge López on 20/1/17.
//
//

#pragma once

#include <vector>

#include "gameobject.h"

namespace dc
{
	// ===========================================================
	// External Enums / Typedefs for global usage
	// ===========================================================

	/**
	 * \class CScene
	 * \brief
	 * \author Jorge López González
	 *
	 * Holds all the game objects that participate in a scene.
	 * When a game object is added, the life cycle of its components
	 * is as follows:
	 * - When you add it, it calls Awake for all components.  It's not added yet to the scene
	 * - In PrepareUpdate is added to the scene and calls Start for all components.
	 *	That way the components are initalized before the first call to Update.
	 */
	class CScene
	{
		// ===========================================================
		// Static fields / methods
		// ===========================================================
		
		// ===========================================================
		// Constant / Enums / Typedefs
		// ===========================================================
		
		// ===========================================================
		// Inner and Anonymous Classes
		// ===========================================================
		
		// ===========================================================
		// Getter & Setter
		// ===========================================================
	public:
		const char*			Name()			const { return mp_name; }
		const unsigned int	RootCount()		const { return m_goList.size(); }
		const TGOList&		GameObjects()	const { return m_goList; }
		
		const bool			Exists(const CGameObject* gameObject);
		
		template<typename CT>
		std::vector<CT*>	GetSceneComponents();
		
		// ===========================================================
		// Constructors
		// ===========================================================
	public:
		CScene(const char* name) : mp_name(name) {}
		~CScene();
		
		CScene(const CScene& copy) = delete;
		void operator= (const CScene& copy) = delete;

		// ===========================================================
		// Methods for/from SuperClass/Interfaces
		// ===========================================================
		
		// ===========================================================
		// Methods
		// ===========================================================
	public:
		void Update();
		
		void Add(CGameObject* gameObject);
		void Remove(CGameObject* gameObject);
		
	private:
		void PrepareUpdate();
		void FinishUpdate();

		void AddToScene(CGameObject* gameObject);
		void RemoveFromScene(CGameObject* gameObject);
		
		void AddComponents(const char* name, const TComponentList& componentList);
		void RemoveComponents(const char* name, const TComponentList& componentList);
		
		// ===========================================================
		// Fields
		// ===========================================================
	private:
		const char*			mp_name;
		
		TGOList				m_goList;
		TGOList				m_newGOList;
		TGOList				m_oldGOList;
		
		TComponentListTable	m_componentsMap;
	};
	
	// ===========================================================
	// Class typedefs
	// ===========================================================
	
	// ===========================================================
	// Template/Inline implementation
	// ===========================================================
	template<typename CT>
	std::vector<CT*> CScene::GetSceneComponents()
	{
		const auto& componentsEntryIt = m_componentsMap.find(CT::TypeName());
		assert(componentsEntryIt != m_componentsMap.end() && "[CScene::GetSceneComponents] You shouldn't be asking for Components that doesn't exist");
		
		const TComponentList& componentList = componentsEntryIt->second;
		std::vector<CT*> castedComponentList(componentList.size());
		
		for(CComponent* component : componentList)
		{
			CT* castedComponent = component->DirectCast<CT>();
			castedComponentList.push_back(castedComponent);
		}
		
		return castedComponentList;
	}
}
