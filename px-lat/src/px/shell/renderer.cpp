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
			m_ui.vao = vao({ 2, 4 });
			m_ui.shader = program("shaders/bg", [aspect = glGetUniformLocation(m_ui.shader.id(), "size"), this]()
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
			int w = canvas.width();
			int h = canvas.height();
			if (w != m_ui.width || h != m_ui.height)
			{
				m_ui.width = w;
				m_ui.height = h;
				int size = w * h * 4; // quads

				// vertices
				m_ui.vertices.resize(size * 2); // 2-d
				unsigned int offset = 0;
				for (int j = 0; j < h; ++j)
				{

					for (int i = 0; i < w; ++i)
					{
						int x = i * ui_cell_width;
						int dx = (i + 1) * ui_cell_width;
						int y = j * ui_cell_height;
						int dy = (j + 1) * ui_cell_height;

						m_ui.vertices[offset + 0] = (GLfloat)x;
						m_ui.vertices[offset + 1] = (GLfloat)y;
						m_ui.vertices[offset + 2] = (GLfloat)x;
						m_ui.vertices[offset + 3] = (GLfloat)dy;
						m_ui.vertices[offset + 4] = (GLfloat)dx;
						m_ui.vertices[offset + 5] = (GLfloat)dy;
						m_ui.vertices[offset + 6] = (GLfloat)dx;
						m_ui.vertices[offset + 7] = (GLfloat)y;

						offset += 2 * 4;
					}
				}

				// other attributes containers
				m_ui.colors.resize(size * 4); // rgba

				// indices
				m_ui.indices.resize(size * 6); // 2 triangles
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

					attribute_offset += 4; // quad vertices
					indice_offset += 6; // 2 triangle
				}
			}

			unsigned int offset = 0;
			for (int j = 0; j < h; ++j)
			{

				for (int i = 0; i < w; ++i)
				{
					for (int k = 0; k < 4; ++k)
					{
						m_ui.colors[offset] = 0.5f;
						++offset;
					}
				}
			}


			m_ui.vao.fill(w * h * 4, { &m_ui.vertices, &m_ui.colors }, m_ui.indices);

			m_ui.shader.use();
			m_ui.vao.draw();

			m_opengl->swap();
		}
	}
}