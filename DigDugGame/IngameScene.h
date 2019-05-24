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


		void RespawnPlayer();
		
		size_t GetClosestPlayerTo(size_t idx);

	private:
		float m_Scale = 2.0f;

		// Dynamic text
		std::shared_ptr<TextComponent> m_spPlayer1ScoreText;
		std::shared_ptr<TextComponent> m_spPlayer2ScoreText;
		std::shared_ptr<TextComponent> m_spTotalScoreText;
		std::shared_ptr<TextComponent> m_spRoundNR;

		// Game stats
		size_t m_Player1Score = 0;
		size_t m_Player2Score = 0;
		size_t m_RoundNr = 1;

		// Game info
		std::shared_ptr<CharacterDigDug> m_spPlayer1;
		std::shared_ptr<CharacterDigDug> m_spPlayer2;
		size_t m_InitialPlayerPos;
	};
}


