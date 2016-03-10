// name: sprite_sheet.hpp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_SHELL_SPRITE_SHEET_H
#define PX_SHELL_SPRITE_SHEET_H

#include <px/shell/image.h>

#include <map>
#include <string>
#include <fstream>

#include <json.hpp>
using json = nlohmann::json;

namespace px
{
	namespace shell
	{

		class sprite_sheet
		{
		public:
			typedef std::string name;
		private:
			unsigned int m_index;
			std::map<name, image> m_map;

		public:
			sprite_sheet()
			{
			}
			sprite_sheet(const char* file_path, unsigned int index)
			{
				init(file_path, index);
			}
			~sprite_sheet()
			{
			}
			sprite_sheet(const sprite_sheet&) = delete;

		public:
			void init(const char* file_path, unsigned int index)
			{
				m_index = index;
				std::ifstream file = std::ifstream(file_path, std::ifstream::in);

				if (!file) throw std::runtime_error(std::string("px::shell::sprite_sheet - can not read file ") + std::string(file_path));

				float horisontal = 1.0f / 128;
				float vertical = 1.0f / 64;

				json j(file);
				for (auto it = j.begin(), last = j.end(); it != last; ++it)
				{
					auto &img = m_map[it.key()];
					const auto &frame = it->at("frame");

					float x = frame["x"];
					float y = frame["y"];
					float w = frame["width"];
					float h = frame["height"];

					img.top = y * vertical;
					img.left = x * horisontal;
					img.bottom = (y + h - 1) * vertical;
					img.right = (x + w - 1) * horisontal;
					img.alternative_ascii = '?';
					img.tint = color(1, 1, 1);
				}
			}
		public:
			const image& operator[](const std::string &name) const
			{
				return m_map.at(name);
			}
		};
	}
}

#endif