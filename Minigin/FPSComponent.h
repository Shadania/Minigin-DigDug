#pragma once
#include "BaseComponent.h"
#include "TextureComponents.h"


namespace dae
{
	class FPSComponent final : public BaseComponent
	{
	public:
		FPSComponent(const std::shared_ptr<Font> font, const Float4& color);

		virtual void Initialize() override;
		virtual void Update() override;

		void SetColor(const Float4& newColor);
		void SetFont(const std::shared_ptr<Font>& newFont);


	private:
		std::shared_ptr<TextComponent> m_spText;
		const std::shared_ptr<Font> m_spFont;
		Float4 m_Color;
	};
}