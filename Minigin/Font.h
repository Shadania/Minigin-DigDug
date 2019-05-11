#pragma once

typedef struct _TTF_Font TTF_Font;

namespace dae
{
	class Font
	{
	public:
		TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, unsigned int size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;
	private:
		TTF_Font* mFont;
		unsigned int mSize;
	};

}
