// name: program.h
// type: c++ header
// desc: struct declaration
// auth: is0urce

// program scruct for opengl

#ifndef PX_SHELL_PROGRAM_H
#define PX_SHELL_PROGRAM_H

#include <gl\gl.h>
#include <gl\glu.h>

#include <px/shell/glsl.h>

#include <vector>
#include <memory>
#include <functional>

namespace px
{
	namespace shell
	{
		struct program
		{
		private:
			GLuint m_id;
			bool m_init;
			std::function<void()> fn;

		public:
			program() : m_init(false), m_id(0)
			{
			}
			program(const std::string& path, std::function<void()> fn)
			{
				m_id = glsl::link(path);
				m_init = true;
				this->fn = fn;
			}
			program(const std::string& path) : program(path, nullptr)
			{
			}
			program(const program&) = delete;
			~program()
			{
				if (m_init)
				{
					glDeleteProgram(m_id);
				}
			}

			void prepare(std::function<void()> fn)
			{
				this->fn = fn;
			}
			GLuint id() const
			{
				return m_id;
			}
			void use() const
			{
				use(true);
			}
			void use(bool prepare) const
			{
				glUseProgram(m_id);
				if (prepare && fn)
				{
					fn();
				}
			}
		};
	}
}

#endif