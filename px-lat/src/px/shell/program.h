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

namespace px
{
	namespace shell
	{
		struct program
		{
		private:
			GLuint m_id;
			bool m_init;

		public:
			program() : m_init(false)
			{
			}
			program(const std::string& path)
			{
				m_id = glsl::program(path);
				m_init = true;
			}
			program(const program&) = delete;
			~program()
			{
				if (m_init)
				{
					glDeleteProgram(m_id);
				}
			}

			GLuint id() const
			{
				return m_id;
			}
			void use() const
			{
				glUseProgram(m_id);
			}
		};
	}
}

#endif