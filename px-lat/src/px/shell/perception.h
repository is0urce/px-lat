// name: perception.h
// type: c++ header
// desc: class declaration
// auth: is0urce

// "wiew" of game board

#ifndef PX_SHELL_PERCEPTION_H
#define PX_SHELL_PERCEPTION_H

#include <px/map.hpp>
#include <px/point.hpp>
#include <px/color.hpp>

#include <px/shell/image.h>

#include <list>
#include <string>
#include <memory>
#include <functional>
#include <map>

namespace px
{
	namespace shell
	{
		class perception
		{
		public:
			typedef image ground_t;

		private:
			unsigned int m_version = 0;
			point m_start;
			point m_start_previous;
			point m_move; // current-prev start

			ground_t m_default;
			color m_black;

			// ground
			std::unique_ptr<map<ground_t>> m_ground;
			std::unique_ptr<map<ground_t>> m_ground_fade;

			// lighting
			std::unique_ptr<map<color>> m_color;
			std::unique_ptr<map<color>> m_color_fade;

			// tile hiding ('shadows' for glyph mode)
			std::unique_ptr<map<bool>> m_hide;
			std::unique_ptr<map<bool>> m_hide_fade;


		public:
			perception(point range);
			perception(point range, point start);
			virtual ~perception();

		private:
			perception(const perception&) = delete; // disable copy

		public:
			int width() const;
			int height() const;
			bool contains(const point &position) const;
			point range() const;
			void outer(ground_t outer);

			unsigned int version() const;
			void displace(const point &start);
			void start(point point);
			point start() const;
			point start_previous() const;
			point movement() const;

			const color& light(const point &position) const;
			const color& light_previous(const point &position) const;
			const ground_t& ground(const point &position) const;
			const ground_t& ground_previous(const point &position) const;
			bool hide(const point &position) const;
			bool hide_previous(const point &position) const;

			void light(const point &position, const color &color_value);
			void light_previous(const point &position, const color &color_value);
			void ground(const point &position, const ground_t &ground_value);
			void ground_previous(const point &position, const ground_t &ground_value);
			void hide(const point &position, bool shadow);
		};
	}
}

#endif