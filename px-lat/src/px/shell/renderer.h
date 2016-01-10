// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#ifndef PX_SHELL_RENDERER_H
#define PX_SHELL_RENDERER_H

#include <px/shell/avatar.hpp>
#include <px/shell/avatar_handle.hpp>
#include <px/shell/font.h>
#include <px/shell/font_texture.h>
#include <px/shell/opengl.h>
#include <px/shell/vao.h>
#include <px/point.hpp>
#include <px/vector.hpp>

#include <memory>

namespace px
{
	namespace shell
	{
		class opengl;
		class renderer
		{
		public:
			typedef avatar<unsigned int> avatar_t;

		private:
			opengl* m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_scale;
			font_texture m_ui;

		public:
			renderer(opengl *opengl);
			virtual ~renderer();

		public:
			void add(avatar_handle<avatar_t>);
			void remove(const void*);

			void draw();
		};
	}
}

#endif