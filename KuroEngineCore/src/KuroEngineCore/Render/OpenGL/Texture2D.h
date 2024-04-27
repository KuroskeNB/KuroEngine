#pragma once

namespace KuroEngine
{
	class Texture2D
	{
	public:

		Texture2D(const char* path);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D& operator=(const Texture2D&) = delete;
		Texture2D& operator=(Texture2D&& texture) noexcept;
		Texture2D(Texture2D&& texture) noexcept;

		unsigned int Get_texture_id() { return texture_id; };

		void bind();
	private:
		unsigned int texture_id = 0;
		unsigned int texture_width = 0;
		unsigned int texture_height = 0;
	};
}