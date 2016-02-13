// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#ifndef PX_SHELL_RENDERER_H
#define PX_SHELL_RENDERER_H

#include <px/shell/font.h>
#include <px/shell/font_texture.h>
#include <px/shell/vao.h>
#include <px/shell/program.h>
#include <px/shell/sprite_manager.hpp>

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

		public:
			const static unsigned int ui_cell_width = 16;
			const static unsigned int ui_cell_height = 20;

		private:
			opengl* m_opengl;
			int m_width, m_height;
			double m_aspect;
			float m_scale;

			// ui rendering
			// buffers cashed due constant canvas size
			struct ui_draw
			{
			public:
				int width;
				int height;
				struct
				{
					vao vao;
					program shader;
					std::vector<GLfloat> vertices;
					std::vector<GLfloat> colors;
				} bg;
				struct
				{
					vao vao;
					program shader;
					std::vector<GLfloat> vertices;
					std::vector<GLfloat> colors;
					std::vector<GLfloat> texture;
					font_texture font;
				} text;
				std::vector<GLuint> indices; // indices are shared
				float scale_x, scale_y, offset_x, offset_y; // uniform values
			} m_ui;
			struct sprite_draw
			{
			public:
				sprite_manager manager;
				vao vao;
				program shader;
				GLfloat *vertices, *colors, *texture;
				GLuint* index;
			} m_sprite;

		public:
			renderer(opengl *opengl);
			virtual ~renderer();

		public:
			void render(const ui::canvas& gui, time_t time);
			sprite_manager* sprite_manager() { return &m_sprite.manager; }
			void scale(float delta);

		private:
			void draw_canvas(const ui::canvas& gui);
		};
	}
}

#endif