#pragma once
#include "BaseComponent.h"
#include "TextureComponents.h"


namespace dae
{
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(const std::shared_ptr<Font> font, const Float4& color);
		virtual void Update() override;
		virtual void Render() const override;
		virtual void SetGameObj(std::shared_ptr<GameObject> newObj) override;

		void SetColor(const Float4& newColor);
		void SetFont(const std::shared_ptr<Font>& newFont);

		virtual void Initialize() override;

	private:
		std::shared_ptr<TextComponent> m_spText;
		const std::shared_ptr<Font> m_spFont;
		Float4 m_Color;
	};
}