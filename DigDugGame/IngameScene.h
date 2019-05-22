#pragma once
#include "Scene.h"

namespace dae
{
	class CharacterDigDug;
	class TextComponent;

	class IngameScene final : public Scene
	{
	public:
		IngameScene();

		virtual void Init() override;
		virtual void Update() override;

	private:
		float m_Scale = 2.0f;

		// Dynamic text
		std::shared_ptr<TextComponent> m_spPlayer1ScoreText;
		std::shared_ptr<TextComponent> m_spPlayer2ScoreText;
		std::shared_ptr<TextComponent> m_spTotalScoreText;
		std::shared_ptr<TextComponent> m_spRoundNR;
	};
}


