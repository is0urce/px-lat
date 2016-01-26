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
#include <px/shell/vao.h>
#include <px/shell/program.h>

#include <px/ui/canvas.h>

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
			typedef double time_t;
			typedef avatar<unsigned int> avatar_t;
		public:
			const static unsigned int ui_cell_width = 12;
			const static unsigned int ui_cell_height = 16;

		private:
			opengl* m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_scale;

			// ui rendering
			// buffers cashed due constant canvas size
			struct ui_draw
			{
			public:
				int width = -1;
				int height = -1;
				font_texture font;
				struct
				{
					vao vao;
					program shader;
					std::vector<GLfloat> vertices;
					std::vector<GLfloat> colors;
				} bg;
				struct
				{
					program shader;
					std::vector<GLfloat> vertices;
					std::vector<GLfloat> colors;
					std::vector<GLfloat> texture;
				} text;
				std::vector<GLuint> indices; // indices are shared
			} m_ui;

		public:
			renderer(opengl *opengl);
			virtual ~renderer();

		public:
			void add(avatar_handle<avatar_t>);
			void remove(const void*);

			void render(const ui::canvas& gui, time_t time);
		};
	}
}

#endif