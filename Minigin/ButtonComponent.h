#pragma once
#include "BaseComponent.h"


namespace dae
{
	class Texture2D;
	class TextComponent;
	class Font;
	class Event;
	class Listener;

	class ButtonComponent final : public BaseComponent
	{
	public:
		ButtonComponent(const std::shared_ptr<Texture2D> tex, const Float2& dims);


		void SetDims(const Float2& dims) { m_Dims = dims; }

		void SetTexture(const std::shared_ptr<Texture2D> tex);
		void SetTexture(const std::string& asset);
		void SetUseText(bool set) { m_UseText = set; }
		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);


		virtual void Initialize() override;
		virtual void Render() const override;


		std::shared_ptr<Event> m_spGotClicked;
		
		void MouseClicked();

	private:
		Float2 m_Dims;
		bool m_UseText = false;
		std::shared_ptr<Texture2D> m_spMyTexture;
		std::shared_ptr<TextComponent> m_spText;
	};
}