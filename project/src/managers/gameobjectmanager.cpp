//
//	gameobjectmanager
//	DCPP_COMPONENTS
//
//	Created by Jorge L├│pez Gonz├ílez on 22/07/2018 21:40:02.
//

#include "gameobjectmanager.h"

#include <cassert>
#include <algorithm>

#include "help/vectorhelp.h"

namespace dc
{
	const bool CGameObjectMgr::Exists(const CGameObject * gameObject) const
	{
		assert(gameObject && "[CGameObjectMgr::Exists] game object can't be NULL");
		auto end = m_goList.end();
		auto it = std::find(m_goList.begin(), end, gameObject);
		return it != end;
	}

	CGameObjectMgr::~CGameObjectMgr()
	{
		SafeDelete(m_goList);
		SafeDelete(m_newGOList);
		SafeDelete(m_oldGOList);
	}
}
