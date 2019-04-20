#pragma once
#include "BaseComponent.h"
#include "TextureComponents.h"

namespace dae
{
	/*
	 * A "sprite" is a single sequence of images.
	 * They are combined in the SpriteComponent.
	 */
	class Sequence
	{
	public:
		Sequence(std::shared_ptr<Texture2D> tex, const std::string& name, size_t amtFrames);

		// for conform timing
		void SetSecPerFrame(float amt);
		// for non conform timing on frames
		void SetSecPerFrame(const std::vector<float> amts);
		// check name
		bool IsName(int nameHash);
		
		void Render(const std::shared_ptr<GameObject>& go) const;
		void Update();

	private:
		std::shared_ptr<Texture2D> m_spTex;
		std::string m_Name;
		int m_NameHash;
		size_t m_AmtFrames;
		std::vector<float> m_SecPerFrame;

		float m_AccuSec;
		int m_CurrFrameIdx;
	};




	class SpriteComponent final : public BaseComponent
	{
	public:
		SpriteComponent();
		virtual void Update() override;
		virtual void Render() const override;

		void AddSprite(std::shared_ptr<Sequence> sprite);
		void RemoveSprite(const std::string& name);
		void SetActiveSprite(const std::string& name);

	private:
		std::vector<std::shared_ptr<Sequence>> m_Sprites;
		std::shared_ptr<Sequence> m_ActiveSprite;

	};
}