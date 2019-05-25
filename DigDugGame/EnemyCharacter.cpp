#include "pch.h"
#include "EnemyCharacter.h"

#include "IngameScene.h"
#include "EditableTerrainGridComponent.h"


dae::EnemyCharacter::EnemyCharacter(const std::string& which, IngameScene* pScene, const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startIdx)
	:BaseComponent("EnemyCharacter")
	,m_Subtype{which}
	, m_spTerrain{ spTerrain }
	, m_StartIdx{ startIdx }
	, m_pScene{ pScene }
{

}