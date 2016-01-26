// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "renderer.h"

#include <px/shell/opengl.h>
#include <px/shell/font.h>
#include <px/color.h>

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
		void fill_color(const px::color &c, GLfloat *dest)
		{
			c.write(dest, quad);
		}
	}
	namespace shell
	{
		renderer::renderer(opengl *opengl)
			: m_opengl(opengl)
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(opengl* opengl) - opengl is null");


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

			m_ui.text.font = std::make_unique<font>("PragmataPro.ttf", 16);
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
				});
		}
		renderer::~renderer()
		{
		}

		void renderer::add(avatar_handle<avatar_t> handle)
		{
		}
		void renderer::remove(const void* ptr)
		{
		}

		void renderer::render(const ui::canvas& canvas, time_t time)
		{
			m_opengl->update(m_width, m_height);

			m_aspect = m_width;
			m_aspect /= m_height;

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
						//point p(i, ui_h - j - 1);

						//int x = i * ui_cell_width;
						//int dx = (i + 1) * ui_cell_width;
						//int y = j * ui_cell_height;
						//int dy = (j + 1) * ui_cell_height;

						//p.moved(0, 0).multiplied(ui_cell_width, ui_cell_height).write2(&m_ui.bg.vertices[offset + 0 * 2]);
						//p.moved(0, 1).multiplied(ui_cell_width, ui_cell_height).write2(&m_ui.bg.vertices[offset + 1 * 2]);
						//p.moved(1, 1).multiplied(ui_cell_width, ui_cell_height).write2(&m_ui.bg.vertices[offset + 2 * 2]);
						//p.moved(1, 0).multiplied(ui_cell_width, ui_cell_height).write2(&m_ui.bg.vertices[offset + 3 * 2]);

						//m_ui.bg.vertices[offset + 0] = (GLfloat)x;
						//m_ui.bg.vertices[offset + 1] = (GLfloat)y;
						//m_ui.bg.vertices[offset + 2] = (GLfloat)x;
						//m_ui.bg.vertices[offset + 3] = (GLfloat)dy;
						//m_ui.bg.vertices[offset + 4] = (GLfloat)dx;
						//m_ui.bg.vertices[offset + 5] = (GLfloat)dy;
						//m_ui.bg.vertices[offset + 6] = (GLfloat)dx;
						//m_ui.bg.vertices[offset + 7] = (GLfloat)y;
						
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
			}

			unsigned int offset = 0;
			for (int j = 0; j < ui_h; ++j)
			{
				for (int i = 0; i < ui_w; ++i)
				{
					for (int k = 0; k < 4; ++k)
					{
						m_ui.bg.colors[offset + 0] = (std::rand() % 10000) / 10000.0f;
						m_ui.bg.colors[offset + 1] = (std::rand() % 10000) / 10000.0f;
						m_ui.bg.colors[offset + 2] = (std::rand() % 10000) / 10000.0f;
						m_ui.bg.colors[offset + 3] = 1;
						offset += 4;
					}
				}
			}

			m_ui.scale_x = 2.0f / m_width;
			m_ui.scale_y = 2.0f / m_height;
			m_ui.offset_x = (float)(m_width % ui_cell_width / 2);
			m_ui.offset_y = (float)(m_height % ui_cell_height / 2);

			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);

			m_ui.bg.shader.use();
			m_ui.bg.vao.fill_attributes(size * quad, 1, &m_ui.bg.colors[0]);
			m_ui.bg.vao.draw();

			//m_ui.text.shader.use();
			//m_ui.text.font.bind(0);
			//m_ui.text.vao.fill_attributes(ui_w * ui_h * quad, 0, &m_ui.text.vertices[0]);
			//m_ui.text.vao.fill_attributes(ui_w * ui_h * quad, 1, &m_ui.text.colors[0]);
			//m_ui.text.vao.fill_attributes(ui_w * ui_h * quad, 2, &m_ui.text.texture[0]);
			//m_ui.text.vao.draw();
		}
	}
}