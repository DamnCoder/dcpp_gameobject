/*
The MIT License (MIT)

Copyright (c) 2018 Jorge López González

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "gameobject.h"

#include <cassert>

#include "help/deletehelp.h"

#include "transform.h"

namespace dc
{
	CGameObject::CGameObject():
		mp_name("GameObject")
	{
		mp_transform = AddComponent<CTransform>();
	}
	
	CGameObject::CGameObject(const char* name):
		mp_name(name)
	{
		mp_transform = AddComponent<CTransform>();
	}
	
	CGameObject::~CGameObject()
	{
		mp_transform = 0;
		SafeDelete(m_componentTable);
	}
	
	const bool CGameObject::HasChild(const char* name) const
	{
		return FindChild(name) != 0;
	}
	
	const unsigned int CGameObject::ComponentsNum(const char* compId) const
	{
		return GetComponents(compId).size();
	}
	
	const TComponentList& CGameObject::GetComponents(const char* compId) const
	{
		const auto& componentsEntryIt = m_componentTable.find(compId);
		assert(componentsEntryIt != m_componentTable.end() && "[CGameObject::GetComponents] You shouldn't be asking for Components that doesn't exist");
		return componentsEntryIt->second;
	}
	
	CComponent* CGameObject::AddComponent(CComponent* component)
	{
		assert(component && "[CGameObject::GetComponents] Component is NULL");
		
		component->GameObject(this);
		TComponentList& componentList = m_componentTable[component->InstanceName()];
		
		componentList.push_back(component);
		return component;
	}
	
	void CGameObject::RemoveComponent(const char* name)
	{
		TComponentListTable::iterator componentsEntryIt = m_componentTable.find(name);
		if(componentsEntryIt != m_componentTable.end())
		{
			TComponentList& componentList = componentsEntryIt->second;
			
			CComponent* component = componentList.front();
			componentList.erase(std::find(componentList.begin(), componentList.end(), component));
			
			if(componentList.empty())
			{
				m_componentTable.erase(componentsEntryIt);
			}
			
			delete component;
		}
	}
	
	CGameObject* CGameObject::FindChild(const char* name) const
	{
		assert(mp_transform && "[CGameObject::FindChild] The game object has no transform component");
		
		TTransformList childrenTransList = mp_transform->Children();
		for(auto* childTrans : childrenTransList)
		{
			auto* childGO = childTrans->GameObject();
			if(childGO->Name() == name)
			{
				return childGO;
			}
			else if(childGO->Transform()->HasChildren())
			{
				CGameObject* go = childGO->FindChild(name);
				if(go)
				{
					return go;
				}
			}
		}
		
		return 0;
	}
}
