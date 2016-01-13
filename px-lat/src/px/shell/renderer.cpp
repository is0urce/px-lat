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
			, m_ui_width(-1), m_ui_height(-1)
			, m_bg({2, 4}), m_shader("shaders/bg")
			, m_ui_font(std::make_unique<font>("PragmataPro.ttf", 16))
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(opengl* opengl) - opengl is null");

			m_shader.prepare([aspect = glGetUniformLocation(m_shader.id(), "size"), this]()
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

		void renderer::render(const ui::canvas& canvas)
		{
			m_opengl->update(m_width, m_height);

			// ui
			int w = canvas.width();
			int h = canvas.height();
			if (w != m_ui_width && h != m_ui_height)
			{
				m_ui_width = w;
				m_ui_height = h;
			}
			std::vector<GLfloat> vertice{ 0.0f,0.0f, 0.0f,1.0f, 1.0f,1.0f, 1.0f,0.0f };
			std::vector<GLfloat> color{ 1.0f,0.0f,0.0f,1.0f, 0.0f,1.0f,0.0f,1.0f, 0.0f,0.0f,1.0f,1.0f, 1.0f,1.0f,1.0f,1.0f };
			m_bg.fill(6, { &vertice, &color }, { 0,2,1, 0,3,2 });
			m_shader.use();
			m_bg.draw();

			m_opengl->swap();
		}
	}
}