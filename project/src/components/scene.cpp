/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "scene.h"

#include <cassert>

#include "transform.h"

#include "help/deletehelp.h"
#include "help/vectorhelp.h"

namespace dc
{
	const bool CScene::Exists(const CGameObject* gameObject)
	{
		assert(gameObject && "[CScene::Exists] game object can't be NULL");
		TGOList::iterator end = m_goList.end();
		TGOList::iterator it = std::find(m_goList.begin(), end, gameObject);
		return it != end;
	}

	CScene::~CScene()
	{
		SafeDelete(m_goList);
		SafeDelete(m_newGOList);
		SafeDelete(m_oldGOList);
	}
	
	void CScene::PrepareUpdate()
	{
		if(m_newGOList.size() == 0)
			return;
		
		for(CGameObject* gameObject : m_newGOList)
		{
			AddToScene(gameObject);
		}
		m_newGOList.clear();
	}
	
	void CScene::Update()
	{
		PrepareUpdate();

		for(auto& componentListEntry : m_componentsMap)
		{
			for(auto* component : componentListEntry.second)
			{
				component->Update();
			}
		}

		FinishUpdate();
	}

	void CScene::FinishUpdate()
	{
		if(m_oldGOList.size() == 0)
			return;
		
		for(CGameObject* gameObject : m_oldGOList)
		{
			RemoveFromScene(gameObject);
		}
		m_oldGOList.clear();
	}

	
	void CScene::Add(CGameObject* gameObject)
	{
		assert(gameObject && "[CScene::Exists] game object can't be NULL");

		//printf("[CScene::AddToScene] Adding %s\n", gameObject->Name().c_str());
		
		// Check if the same Game Object is already in the scene, we can't have two instances of the same game object
		assert(!Exists(gameObject) && "[CScene::AddToScene] You can't add more than one instance of a GameObject");
		
		// We add it to a list to include it in the scene in a deferred way
		m_newGOList.push_back(gameObject);
		
		// As the Game Object is a valid one, we initialize its components
		const TComponentListTable& goComponentsMap = gameObject->ComponentsTable();
		for(auto& componentListEntry : goComponentsMap)
		{
			const TComponentList& componentList = componentListEntry.second;
			for(CComponent* component : componentList)
			{
				component->Awake();
			}
		}
		
		// And now the children components
		const TTransformList& childrenList = gameObject->Transform()->Children();
		
		for(auto* children : childrenList)
		{
			Add(children->GameObject());
		}
	}
	
	void CScene::Remove(CGameObject* gameObject)
	{
		// We add it to a list to remove it from the scene in a deferred way
		m_oldGOList.push_back(gameObject);
		
		// To prepare the Game Object for removal we call Sleep on its components
		const TComponentListTable& goComponentsMap = gameObject->ComponentsTable();
		for(auto& componentListEntry : goComponentsMap)
		{
			const TComponentList& componentList = componentListEntry.second;
			for(CComponent* component : componentList)
			{
				component->Sleep();
			}
		}
		
		// And now the children components
		const TTransformList& childrenList = gameObject->Transform()->Children();
		
		for(auto* children : childrenList)
		{
			Remove(children->GameObject());
		}
	}

	void CScene::AddToScene(CGameObject* gameObject)
	{
		m_goList.push_back(gameObject);
		
		const TComponentListTable& goComponentsMap = gameObject->ComponentsTable();
		for(auto& componentListEntry : goComponentsMap)
		{
			AddComponents(componentListEntry.first, componentListEntry.second);
		}
	}
	
	void CScene::RemoveFromScene(CGameObject* gameObject)
	{
		dc::Remove(m_goList, gameObject);
		
		const TComponentListTable& goComponentsMap = gameObject->ComponentsTable();
		for(auto& componentListEntry : goComponentsMap)
		{
			RemoveComponents(componentListEntry.first, componentListEntry.second);
		}
	}
	
	void CScene::AddComponents(const char* name, const TComponentList& newComponentList)
	{
		assert(newComponentList.size() && "[CScene::AddToScene] No components being added");
		
		// The Game Object is finally added into the scene, we call Awake
		TComponentList& componentList = m_componentsMap[name];
		
		for(CComponent* component : newComponentList)
		{
			componentList.push_back(component);
			component->Start();
		}
	}
	
	void CScene::RemoveComponents(const char* name, const TComponentList& oldComponentList)
	{
		assert(oldComponentList.size() && "[CScene::Remove] No components being removed");
		
		TComponentList& componentList = m_componentsMap[name];
		
		TComponentList::iterator oldBegin = std::find(componentList.begin(), componentList.end(), *oldComponentList.begin());
		TComponentList::iterator oldEnd = oldBegin + oldComponentList.size();
		
		componentList.erase(oldBegin, oldEnd);
		
		for(CComponent* component : oldComponentList)
		{
			component->Finish();
		}
	}
}

