#pragma once
#include "BaseComponent.h"
#include "Structs.h"
#include "Font.h"
#include "Texture2D.h"

namespace dae
{
	class TextureComponent : public BaseComponent
	{
	public:
		TextureComponent(std::shared_ptr<GameObject> pObj, Float2 pos = { 0, 0 }, float rot = 0.0f, Float2 m_Scale = { 1, 1 });
		
		void SetTexture(const std::string& fileName);
		void RenderTexture();

		virtual void Update() override {}

		TextureComponent(const TextureComponent&) = delete;
		TextureComponent(TextureComponent&&) noexcept = delete;
		TextureComponent& operator=(const TextureComponent&) = delete;
		TextureComponent& operator=(TextureComponent&&) noexcept = delete;



	protected:

		std::shared_ptr<Texture2D> m_spTexture;
		// Local transform
		Float2 m_Pos;
		float m_Rot;
		Float2 m_Scale;
	};

	class TextComponent final : public TextureComponent
	{
	public:
		TextComponent(std::shared_ptr<GameObject> pObj, const std::string& text, const std::shared_ptr<Font> font, 
			Float4 color = { 1, 1, 1, 1 }, Float2 pos = { 0, 0 }, float rot = 0.0f, Float2 m_Scale = { 1, 1 });

		void SetText(const std::string& newText);
		void SetFont(const std::shared_ptr<Font>& newFont);
		void SetColor(const Float4& newColor);

		void GenerateTexture();

		virtual void Update() override;


		TextComponent(const TextComponent&) = delete;
		TextComponent(TextComponent&&) noexcept = delete;
		TextComponent& operator=(const TextComponent&) = delete;
		TextComponent& operator=(TextComponent&&) noexcept = delete;

	private:

		std::shared_ptr<Font> m_spFont;
		std::string m_Text;
		Float4 m_Color;
	
	};
}