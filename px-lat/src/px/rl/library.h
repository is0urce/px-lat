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
			library(shell::sprite_manager *image_manager, shell::sprite_sheet* sprite_sheet)
				: m_sprites(image_manager)
				, m_sheet(sprite_sheet)
			{
				if (!image_manager) throw std::runtime_error("library::library(..) image_manager is null");
				if (!sprite_sheet) throw std::runtime_error("library::library(..) sprite_sheet is null");
			}
			virtual ~library() {}

		public:
			auto make_image(const std::string &name) -> decltype(m_sprites->make_shared())
			{
				auto s = m_sprites->make_shared();
				shell::image* img = s.get();
				*img = m_sheet->at(name);

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