#pragma once
#include "BaseComponent.h"
#include "TextureComponents.h"

namespace dae
{
	/*
	 * A "sprite" is a single sequence of images.
	 * They are combined in the SpriteComponent.
	 */
	class Sprite
	{
	public:
		Sprite(std::shared_ptr<Texture2D> tex, const std::string& name, size_t amtFrames);

		// for conform timing
		void SetSecPerFrame(float amt);
		// for non conform timing on frames
		void SetSecPerFrame(const std::vector<float> amts);
		// check name
		bool IsName(const std::string& name);
		
		void Render();
		void Update();

	private:
		std::shared_ptr<Texture2D> m_spTex;
		std::string m_Name;
		size_t m_AmtFrames;
		std::vector<float> m_SecPerFrame;
		bool m_UseUniformTiming;

		float m_AccuSec;
		int m_CurrFrameIdx;
	};




	class SpriteComponent final : public BaseComponent
	{
	public:
		SpriteComponent(std::shared_ptr<Texture2D> tex);
		virtual void Update() override;
		virtual void Render() override;

		void AddSprite(std::shared_ptr<Sprite> sprite);
		void RemoveSprite(const std::string& name);
		void SetActiveSprite(const std::string& name);

	private:
		std::vector<std::shared_ptr<Sprite>> m_Sprites;
		std::shared_ptr<Sprite> m_ActiveSprite;

	};
}