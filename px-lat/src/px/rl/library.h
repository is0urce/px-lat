// name: library.h
// type: c++ header
// desc: class declaration
// auth: is0urce

#ifndef PX_RL_LIBRARY_H
#define PX_RL_LIBRARY_H

#include <px/es/unit.h>

#include <px/shell/sprite_manager.hpp>
#include <px/shell/sprite_sheet.hpp>

namespace px
{
	namespace rl
	{
		class library
		{
		private:
			shell::sprite_manager *m_sprites;
			shell::sprite_sheet *m_sheet;

		public:
			library(shell::sprite_manager *image_manager, shell::sprite_sheet* sheet)
				: m_sprites(image_manager)
				, m_sheet(sheet)
			{
			}
			virtual ~library() {}

		public:
			std::shared_ptr<shell::sprite_manager::element> make_image(unsigned int u_plus)
			{
				auto s = m_sprites->make_shared();
				s->alternative_glyph = u_plus;
				return s;
			}
			auto image(const std::string &name) -> decltype(m_sheet->at(name))
			{
				return m_sheet->at(name);
			}
		};
	}
}

#endif