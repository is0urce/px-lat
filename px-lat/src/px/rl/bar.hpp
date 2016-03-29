// name: bar.h
// type: c++ header
// desc: template class declaration & implementation
// auth: is0urce

// resource bar

#ifndef PX_RL_BAR_HPP
#define PX_RL_BAR_HPP

namespace px
{
	namespace rl
	{
		template <typename _T>
		struct bar
		{
		public:
			typedef _T value_t;

		private:
			_T m_current;
			_T m_max;

			// ctor & dtor
		public:
			bar() : m_current{}, m_max{}
			{
			}
			bar(_T max)
			{
				init(max, max);
			}
			bar(_T current, _T max)
			{
				init(max, max);
			}
		private:
			void init(_T current, _T max)
			{
				m_max = max;
				m_current = std::min<_T>(current, m_max);
			}

			// methods
		public:
			void set(_T current_and_max)
			{
				init(current_and_max, current_and_max);
			}
			void set(_T current, _T max)
			{
				init(current, max);
			}
			_T current() const
			{
				return m_current;
			}
			_T maximum() const
			{
				return m_max;
			}
			void modify(_T magnitude)
			{
				m_current = std::min<_T>(_current + magnitude, m_max);
			}
			void restore(_T magnitude)
			{
				if (magnitude < 0) throw std::logical_error("bar<t>::restore(_t magnitude) magnitude < 0");
				m_current = std::min<_T>(_current + magnitude, m_max);
			}
			void damage(_T magnitude)
			{
				if (magnitude < 0) throw std::logical_error("bar<t>::damage(_t magnitude) magnitude < 0");
				m_current -= magnitude;
			}
			void current(_T current)
			{
				m_current = std::min<_T>(current, _max);
			}
			void maximum(_T max)
			{
				_max = max;
				_current = std::min<_T>(_current, _max);
			}
			bool empty() const
			{
				return m_current <= 0;
			}
			bool full() const
			{
				return m_current == m_max;
			}
			bar& operator=(_T c)
			{
				set(c);
				return *this;
			}
			bar& operator+=(_T c)
			{
				modify(c);
				return *this;
			}
			bar& operator-=(_T c)
			{
				modify(-c);
				return *this;
			}
			operator value_t() const
			{
				return m_current;
			}
			operator bool() const
			{
				return m_current > 0;
			}
		};
	}
}

#endif