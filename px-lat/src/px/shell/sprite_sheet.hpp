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
			typedef image element;

		private:
			unsigned int m_index;
			std::map<name, element> m_map;

		public:
			sprite_sheet() : m_index(0)
			{
			}
			~sprite_sheet()
			{
			}
			sprite_sheet(const sprite_sheet&) = delete;

		public:
			unsigned int init(const char* file_path, unsigned int width, unsigned int height)
			{
				std::ifstream file = std::ifstream(file_path, std::ifstream::in);

				if (!file) throw std::runtime_error(std::string("px::shell::sprite_sheet - can not read file ") + std::string(file_path));

				float horisontal = 1.0f / width;
				float vertical = 1.0f / height;

				json f(file);
				auto j = f["frames"];
				for (auto it = j.begin(), last = j.end(); it != last; ++it)
				{
					const auto &frame = it->at("frame");

					auto i = m_map.emplace(it.key(), image());

					const std::string &name = i.first->first;
					element &img = i.first->second;
					
					// calculate image data
					float x = frame["x"];
					float y = frame["y"];
					float w = frame["w"];
					float h = frame["h"];

					// fill image data
					img.top = y * vertical;
					img.left = x * horisontal;
					img.bottom = (y + h) * vertical;
					img.right = (x + w) * horisontal;
					img.width = w * horisontal;
					img.height = h * vertical;

					img.tint = color(1, 1, 1);
					img.transparency = 0;

					img.alternative_glyph = '?';
					img.name = name.c_str();
					img.atlas = m_index;
				}

				return m_index++;
			}
		public:
			const image& operator[](const std::string &name) const
			{
				return m_map.at(name);
			}
			const image& at(const std::string &name) const
			{
				try
				{ 
					return m_map.at(name);
				}
				catch (const std::exception& e)
				{
					throw std::runtime_error(name + e.what());
				}
			}
		};
	}
}

#endif