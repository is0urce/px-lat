// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "renderer.h"

#include <px/shell/opengl.h>
#include <px/shell/font.h>

#include <px/color.hpp>

#include <memory>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <functional>

namespace px
{
	namespace
	{
		const unsigned int quad = 4; // four vertices for quad
		const unsigned int strip = 6; // six indices for 2-triangles
		void fill_color(const color &c, GLfloat *dest)
		{
			c.write(dest, quad);
		}
		void fill_texture(GLfloat left, GLfloat bottom, GLfloat right, GLfloat top, GLfloat *dest)
		{
			dest[0] = left;
			dest[1] = bottom;
			dest[2] = left;
			dest[3] = top;
			dest[4] = right;
			dest[5] = top;
			dest[6] = right;
			dest[7] = bottom;
		}
	}
	namespace shell
	{
		renderer::renderer(opengl *opengl)
			: m_opengl(opengl)
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(opengl* opengl) - opengl is null");

			// ui draw setup
			m_ui.bg.vao = vao({ 2, 4 });
			m_ui.bg.shader = program("shaders/ui_bg");
			m_ui.bg.shader.prepare([this
				, scale = m_ui.bg.shader.uniform("scale")
				, offset = m_ui.bg.shader.uniform("offset")
				]()
				{
					glUniform2f(scale, (GLfloat)m_ui.scale_x, (GLfloat)m_ui.scale_y);
					glUniform2f(offset, (GLfloat)m_ui.offset_x, (GLfloat)m_ui.offset_y);
				});

			m_ui.text.font = std::make_unique<font>("PragmataPro.ttf", ui_cell_height);
			m_ui.text.vao = vao({ 2, 4, 2 });
			m_ui.text.shader = program("shaders/ui_text");
			m_ui.text.shader.activate();
			m_ui.text.shader.uniform("font", 0);
			m_ui.text.shader.prepare([this
				, scale = m_ui.text.shader.uniform("scale")
				, offset = m_ui.text.shader.uniform("offset")
				]()
				{
					glUniform2f(scale, (GLfloat)m_ui.scale_x, (GLfloat)m_ui.scale_y);
					glUniform2f(offset, (GLfloat)m_ui.offset_x, (GLfloat)m_ui.offset_y);
					m_ui.text.font.bind(0);
				});

			// sprite draw setup
			auto cc = m_sprite.manager.create();
			auto location = m_sprite.move.create();
			location->position.X = 0;
			location->position.Y = 0;

			auto &g = m_ui.text.font->at('?');
			cc->atlas = 0;
			cc->left = (float)g.left;
			cc->right = (float)g.right;
			cc->bottom = (float)g.bottom;
			cc->top = (float)g.top;
			cc->width = (float)g.width;
			cc->height = (float)g.height;
			cc->tint = color(0xffff00);

			cc->link(location);
			cc->enable();

			m_sprite.vao = vao({ 4, 4, 2 });
			m_sprite.shader = program("shaders/sprite");
			m_sprite.shader.uniform("img", 0);
			m_sprite.shader.prepare([this, scale=m_sprite.shader.uniform("scale")]()
				{
					m_ui.text.font.bind(0);
					m_sprite.shader.uniform(scale, 1.0f, (GLfloat)m_aspect);
				});

			// opengl setup
			glEnable(GL_TEXTURE_2D);

			// dont clamp hdr
			glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);
		}
		renderer::~renderer()
		{
		}

		void renderer::render(const ui::canvas& canvas, time_t time)
		{
			m_opengl->update(m_width, m_height);

			m_aspect = m_width;
			m_aspect /= m_height;

			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// sprites draw
			unsigned int size;
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			m_sprite.manager.construct();
			m_sprite.manager.query(size, m_sprite.vertices, m_sprite.colors, m_sprite.texture, m_sprite.index);
			m_sprite.shader.use();
			m_sprite.vao.fill_attributes(size * quad * 4, 0, m_sprite.vertices);
			m_sprite.vao.fill_attributes(size * quad * 4, 1, m_sprite.colors);
			m_sprite.vao.fill_attributes(size * quad * 2, 2, m_sprite.texture);
			m_sprite.vao.fill_indices(size * 2 * 3, m_sprite.index);
			m_sprite.vao.draw();

			draw_canvas(canvas);

			m_opengl->swap();
		}
		void renderer::draw_canvas(const ui::canvas& canvas)
		{
			// ui
			int ui_w = canvas.width();
			int ui_h = canvas.height();
			if (ui_w <= 0 || ui_h <= 0) return;
			int size = ui_w * ui_h;

			if (ui_w != m_ui.width || ui_h != m_ui.height)
			{
				m_ui.width = ui_w;
				m_ui.height = ui_h;

				// update buffer sizes
				m_ui.indices.resize(size * strip);
				m_ui.bg.vertices.resize(size * 2 * quad);
				m_ui.bg.colors.resize(size * 4 * quad);
				m_ui.text.vertices.resize(size * 2 * quad);
				m_ui.text.colors.resize(size * 4 * quad);
				m_ui.text.texture.resize(size * 2 * quad);

				// cashe vertices
				unsigned int offset = 0;
				for (int j = 0; j < ui_h; ++j)
				{

					for (int i = 0; i < ui_w; ++i)
					{
						// cell coordinates
						int x = i * ui_cell_width;
						int dx = (i + 1) * ui_cell_width;
						int y = (ui_h - j - 1) * ui_cell_height;
						int dy = (ui_h - j) * ui_cell_height;

						m_ui.bg.vertices[offset + 0] = (GLfloat)x;
						m_ui.bg.vertices[offset + 1] = (GLfloat)y;
						m_ui.bg.vertices[offset + 2] = (GLfloat)x;
						m_ui.bg.vertices[offset + 3] = (GLfloat)dy;
						m_ui.bg.vertices[offset + 4] = (GLfloat)dx;
						m_ui.bg.vertices[offset + 5] = (GLfloat)dy;
						m_ui.bg.vertices[offset + 6] = (GLfloat)dx;
						m_ui.bg.vertices[offset + 7] = (GLfloat)y;

						offset += 2 * quad;
					}
				}

				// cahse indices
				unsigned int attribute_offset = 0;
				unsigned int indice_offset = 0;
				for (int i = 0; i < size; ++i)
				{
					m_ui.indices[indice_offset + 0] = attribute_offset + 0;
					m_ui.indices[indice_offset + 1] = attribute_offset + 2;
					m_ui.indices[indice_offset + 2] = attribute_offset + 1;
					m_ui.indices[indice_offset + 3] = attribute_offset + 0;
					m_ui.indices[indice_offset + 4] = attribute_offset + 3;
					m_ui.indices[indice_offset + 5] = attribute_offset + 2;

					attribute_offset += quad;
					indice_offset += strip;
				}

				// fill cashed parts
				m_ui.bg.vao.fill_attributes(size * quad, 0, &m_ui.bg.vertices[0]);
				m_ui.bg.vao.fill_indices(size * strip, &m_ui.indices[0]);
				m_ui.text.vao.fill_indices(size * strip, &m_ui.indices[0]);
			}

			unsigned int color_offset = 0;
			unsigned int vertex_offset = 0;
			unsigned int texture_offset = 0;

			for (int j = 0; j < ui_h; ++j)
			{
				for (int i = 0; i < ui_w; ++i)
				{
					auto &symbol = canvas.at({ i, j });
					auto &g = m_ui.text.font->at(symbol.code);

					// vertices
					int x = i * ui_cell_width + ui_cell_width / 2 - g.pixel_width / 2;
					int xd = x + g.pixel_width;
					int y = (ui_h - j - 1) * ui_cell_height - g.pixel_height + g.pixel_top;
					int yd = y + g.pixel_height;

					m_ui.text.vertices[vertex_offset + 0] = (GLfloat)x;
					m_ui.text.vertices[vertex_offset + 1] = (GLfloat)y;
					m_ui.text.vertices[vertex_offset + 2] = (GLfloat)x;
					m_ui.text.vertices[vertex_offset + 3] = (GLfloat)yd;
					m_ui.text.vertices[vertex_offset + 4] = (GLfloat)xd;
					m_ui.text.vertices[vertex_offset + 5] = (GLfloat)yd;
					m_ui.text.vertices[vertex_offset + 6] = (GLfloat)xd;
					m_ui.text.vertices[vertex_offset + 7] = (GLfloat)y;

					// colors
					fill_color(symbol.back, &m_ui.bg.colors[color_offset]);
					fill_color(symbol.front, &m_ui.text.colors[color_offset]);

					// textures
					fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &m_ui.text.texture[texture_offset]);

					color_offset += 4 * quad;
					vertex_offset += 2 * quad;
					texture_offset += 2 * quad;
				}
			}

			// setup drawing
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// uniforms preparation source
			m_ui.scale_x = 2.0f / m_width;
			m_ui.scale_y = 2.0f / m_height;
			m_ui.offset_x = (float)(m_width % ui_cell_width / 2);
			m_ui.offset_y = (float)(m_height % ui_cell_height / 2);

			// background
			m_ui.bg.shader.use();
			m_ui.bg.vao.fill_attributes(size * quad, 1, &m_ui.bg.colors[0]);
			m_ui.bg.vao.draw();

			// text
			m_ui.text.shader.use();
			m_ui.text.vao.fill_attributes(size * quad, 0, &m_ui.text.vertices[0]);
			m_ui.text.vao.fill_attributes(size * quad, 1, &m_ui.text.colors[0]);
			m_ui.text.vao.fill_attributes(size * quad, 2, &m_ui.text.texture[0]);
			m_ui.text.vao.draw();
		}
	}
}