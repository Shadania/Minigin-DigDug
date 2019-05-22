#pragma once

namespace dae
{
	class Texture2D;
	class Font;
	class ResourceManager
	{
		std::string mDataPath;
	public:
		ResourceManager() = default;

		void Init(std::string&& data, const std::string& defaultFontPath = "", size_t defaultFontSize = 0);

		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size);
		const std::shared_ptr<Font>& GetDefaultFont() const { return m_spDefaultFont; }
	private:
		std::shared_ptr<Font> m_spDefaultFont;
	};

}
