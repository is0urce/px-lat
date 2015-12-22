// name: "font_texture.h"
// type: c++
// desc: class header
// auth: is0urce

// opengl texture with font

#ifndef PX_SHELL_FONT_TEXTURE_H
#define PX_SHELL_FONT_TEXTURE_H

#include <gl/glew.h>

#include <memory>

namespace px
{
	namespace shell
	{
		class font;
		class font_texture final
		{
			typedef GLuint texture_id;

		private:
			texture_id m_texture;
			std::shared_ptr<font> m_font;
			unsigned int m_version;

		public:
			~font_texture();
			font_texture();
			font_texture(std::shared_ptr<font>);
			font_texture(const font_texture&) = delete;

		public:
			void swap();
			void update();
			void update(bool force);
			texture_id last_texture() const;
			texture_id texture();
		};
	}
}

#endif