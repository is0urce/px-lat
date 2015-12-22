// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "renderer.h"

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
			: m_aspect(1)
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(renderer::opengl_handle opengl) opengl is null");
			m_ui = { std::make_unique<font>("PragmataPro.ttf", 16) };
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
	}
}