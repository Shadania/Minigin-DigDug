#pragma once
#include "Scene.h"

namespace dae
{
	class CharacterDigDug;
	class TextComponent;
	class Texture2D;

	class IngameScene final : public Scene
	{
	public:
		IngameScene();

		virtual void Init() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
		virtual void Render() const override;


		void RespawnPlayer(size_t idx);
		void AddScoreFor(size_t score, size_t playerIdx);

		
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
		size_t m_InitialPlayer1Pos;
		size_t m_InitialPlayer2Pos;

		// Player lives and respawning variables
		const float m_PlayerRegainLivesTime = 5.0f;
		float m_Player1TimeTillRespawn = 0.0f;
		float m_Player2TimeTillRespawn = 0.0f;
		size_t m_Player1Lives = 3;
		size_t m_Player2Lives = 3;
		bool m_RespawningPlayer1 = false;
		bool m_RespawningPlayer2 = false;

		std::shared_ptr<Texture2D> m_spDigDugImage;
	};
}


