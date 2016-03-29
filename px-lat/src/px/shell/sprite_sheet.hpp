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
		namespace
		{
			const char default_glyph = '?';
			const color default_color(1, 1, 1);
		}
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
				std::ifstream file(file_path, std::ifstream::in);

				if (!file) throw std::runtime_error(std::string("px::shell::sprite_sheet - can not read file ") + std::string(file_path));

				json js(file);

				unsigned int atlas = m_index;
				++m_index;

				auto frames = js["frames"];
				for (auto it = frames.begin(), last = frames.end(); it != last; ++it)
				{
					const auto &frame = it->at("frame");

					auto i = m_map.emplace(it.key(), image());

					const std::string &name = i.first->first;
					element &img = i.first->second;
					
					// calculate image data
					double x = frame["x"];
					double y = frame["y"];
					double w = frame["w"];
					double h = frame["h"];

					// fill image data
					img.atlas = atlas;
					img.name = name.c_str();
					img.alternative_glyph = default_glyph;

					img.top = float(y / height);
					img.left = float(x / width);
					img.bottom = float((y + h) / height);
					img.right = float((x + w) / width);
					img.width = float(w / width);
					img.height = float(h / height);

					img.tint = default_color;
					img.transparency = 0;
				}

				return atlas;
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