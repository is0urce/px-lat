// name: "font_texture.cpp"
// type: c++
// desc: class implementation
// auth: is0urce

#include "font_texture.h"

#include <px/shell/font.h>

namespace
{
	const int mipmaps_total = 8;
}

namespace px
{
	namespace shell
	{
		font_texture::font_texture()
		{
		}
		font_texture::font_texture(std::shared_ptr<font> font_ptr)
		{
			if (!font_ptr) throw std::runtime_error("font_texture.ctor(..) font is null");

			glGenTextures(1, &m_texture);

			m_font = font_ptr;
			update(true);
		}
		font_texture::~font_texture()
		{
			glDeleteTextures(1, &m_texture);
		}

		void font_texture::update()
		{
			update(false);
		}
		void font_texture::update(bool force)
		{

			if (m_font)
			{
				auto version = m_font->version();
				if (force || version != m_version)
				{
					m_version = version;

					const auto &ui_texture = m_font->texture();
					glBindTexture(GL_TEXTURE_2D, m_texture);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmaps_total);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ui_texture.width, ui_texture.height, 0, GL_RED, GL_UNSIGNED_BYTE, ui_texture.data);
					glEnable(GL_TEXTURE_2D);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}
		}

		font_texture::texture_id font_texture::last_texture() const
		{
			return m_texture;
		}
		font_texture::texture_id font_texture::texture()
		{
			update();
			return m_texture;
		}
	}
}

