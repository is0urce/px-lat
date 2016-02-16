// name: perception.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "perception.h"

#include <numeric>

namespace px
{
	namespace shell
	{

		perception::perception(point range) : perception(range, {})
		{
		}

		perception::perception(point range, point start)
			: m_version(0)
			, m_start(start)
			, m_black(0x000000)
		{
			m_ground = std::make_unique<map<perception::ground_t>>(range);
			m_ground_fade = std::make_unique<map<perception::ground_t>>(range);

			m_color = std::make_unique<map<color>>(range);
			m_color_fade = std::make_unique<map<color>>(range);

			m_hide = std::make_unique<map<bool>>(range);
			m_hide_fade = std::make_unique<map<bool>>(range);
		}

		perception::~perception()
		{
		}

		int perception::width() const
		{
			return m_ground->width();
		}

		int perception::height() const
		{
			return m_ground->height();
		}

		point perception::range() const
		{
			return m_ground->range();
		}

		bool perception::contains(const point &point) const
		{
			return m_ground->contains(point);
		}

		unsigned int perception::version() const
		{
			return m_version;
		}

		const color& perception::light(const point &position) const
		{
			return m_color->at(position);
		}

		const color& perception::light_previous(const point &position) const
		{
			point prev = position + m_move;
			return m_color_fade->contains(prev) ? m_color_fade->at(prev) : m_black; // _color->getelement(position); // to fade in
		}

		const perception::ground_t& perception::ground(const point &position) const
		{
			return m_ground->at(position);
		}

		const perception::ground_t& perception::ground_previous(const point &position) const
		{
			point prev = position + m_move;
			return m_ground_fade->contains(prev) ? m_ground_fade->at(prev) : m_default; // m_ground->at(position);
		}

		bool perception::hide(const point &position) const
		{
			return m_hide->at(position, true);
		}

		bool perception::hide_previous(const point &position) const
		{
			return m_hide_fade->at(position + m_move, true);
		}

		void perception::light(const point &position, const color& light_value)
		{
			m_color->at(position) = light_value;
			++m_version;
		}

		void perception::ground(const point &position, const perception::ground_t &ground_value)
		{
			m_ground->at(position) = ground_value;
			++m_version;
		}

		void perception::hide(const point &position, bool shadow)
		{
			m_hide->at(position) = shadow;
		}

		void perception::start(point start)
		{
			m_start = start;
			++m_version;
		}

		point perception::start() const
		{
			return m_start;
		}

		point perception::start_previous() const
		{
			return m_start_previous;
		}

		point perception::movement() const
		{
			return m_move;
		}

		void perception::displace(const point& start)
		{
			std::swap(m_ground, m_ground_fade);
			std::swap(m_color, m_color_fade);

			std::swap(m_hide, m_hide_fade);
			m_hide->fill(false);

			std::swap(m_start, m_start_previous);
			m_start = start;
			m_move = m_start - m_start_previous;

			++m_version;
		}
	}
}