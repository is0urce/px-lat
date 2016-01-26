// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "renderer.h"

#include <px/shell/opengl.h>
#include <px/shell/font.h>

#include <memory>
#include <stdexcept>
#include <vector>
#include <cmath>

namespace
{
	const unsigned int quad = 4; // four vertices for quad
	const unsigned int strip = 6; // six indices for 2-triangles
}
namespace px
{
	namespace shell
	{
		renderer::renderer(opengl *opengl)
			: m_opengl(opengl)
			, m_aspect(1)
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(opengl* opengl) - opengl is null");

			m_ui.font = std::make_unique<font>("PragmataPro.ttf", 16);
			m_ui.bg.vao = vao({ 2, 4 });
			m_ui.bg.shader = program("shaders/bg", [aspect = glGetUniformLocation(m_ui.bg.shader.id(), "size"), this]()
				{
					glUniform1f(aspect, (GLfloat)this->m_aspect);
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

			// ui
			int ui_w = canvas.width();
			int ui_h = canvas.height();
			if (ui_w != m_ui.width || ui_h != m_ui.height)
			{
				m_ui.width = ui_w;
				m_ui.height = ui_h;
				int size = ui_w * ui_h;

				// 2-d vertices
				m_ui.bg.vertices.resize(size * 2 * quad);
				m_ui.text.vertices.resize(size * 2 * quad);

				// rgba colors
				m_ui.bg.colors.resize(size * 4 * quad);
				m_ui.text.colors.resize(size * 4 * quad);

				// texture coordinates for text
				m_ui.text.texture.resize(size * 2 * quad);

				// 2-dvertices
				unsigned int offset = 0;
				for (int j = 0; j < ui_h; ++j)
				{

					for (int i = 0; i < ui_w; ++i)
					{
						int x = i * ui_cell_width;
						int dx = (i + 1) * ui_cell_width;
						int y = j * ui_cell_height;
						int dy = (j + 1) * ui_cell_height;

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

				// indices
				m_ui.indices.resize(size * strip);
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

				// fill constant vao parts
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
						m_ui.bg.colors[offset] = 0.5f;
						++offset;
					}
				}
			}


			m_ui.bg.vao.fill_attributes(ui_w * ui_h * quad, 1, &m_ui.bg.colors[0]);
			m_ui.bg.shader.use();
			m_ui.bg.vao.draw();

			m_opengl->swap();
		}
	}
}