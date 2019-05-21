#pragma once
#include "BaseComponent.h"
#include "TextureComponents.h"

namespace dae
{
	/*
	 * A "sprite" is a single sequence of images.
	 * They are combined in the SpriteComponent.
	 * Setting secperframe to less than zero freezes the frame
	 */
	class Sequence
	{
	public:
		Sequence(std::shared_ptr<Texture2D> tex, 
			const std::string& name, size_t amtFrames, bool repeat = true,
			bool resetDeltaOnActive = false, float secPerFrame = 0.2f);

		// for conform timing
		void SetSecPerFrame(float amt);
		// for non conform timing on frames
		void SetSecPerFrame(const std::vector<float> amts);
		// check name
		bool IsName(size_t nameHash);
		
		void Render(const std::shared_ptr<GameObject>& go) const;
		void Update(float& accuSec);
		void Freeze();

		void SetFrame(size_t frame);

		bool m_ResetDeltaOnActive;

	private:
		std::shared_ptr<Texture2D> m_spTex;
		std::string m_Name;
		size_t m_NameHash;
		size_t m_AmtFrames;
		std::vector<float> m_SecPerFrame;

		int m_CurrFrameIdx;
		bool m_Repeat;
	};




	class SpriteComponent final : public BaseComponent
	{
	public:
		SpriteComponent();
		virtual void Update() override;
		virtual void Render() const override;

		void AddSequence(std::shared_ptr<Sequence> sprite);
		void RemoveSprite(const std::string& name);
		void SetActiveSprite(const std::string& name);
		bool IsActiveSprite(const std::string& name) const;
		Sequence& GetActiveSprite() { return *m_ActiveSprite; }
		void SetFrame(size_t frame);

		void Freeze(bool resetDelta = false);
		void Unfreeze();

	private:
		std::vector<std::shared_ptr<Sequence>> m_Sprites;
		std::shared_ptr<Sequence> m_ActiveSprite;
		float m_AccuSec;
		bool m_Frozen;
	};
}