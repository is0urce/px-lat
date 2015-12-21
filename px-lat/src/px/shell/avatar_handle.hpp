// name: avatar_handle.hpp
// type: c++ header
// desc: class declaration
// auth: is0urce

// avatar representation binded to unit with std::function
// to function requires mapped unit to live while live instance of this class

#ifndef PX_SHELL_AVATAR_HANDLE_H
#define PX_SHELL_AVATAR_HANDLE_H

#include <functional>
#include <stdexcept>

namespace px
{
	namespace shell
	{
		template <typename _A>
		class avatar_handle
		{
		private:
			typedef _A avatar_t;
			typedef std::function<avatar_t()> update_fn;

		private:
			avatar_t m_avatar;
			update_fn m_update;
			const void* m_mapped;

		public:
			avatar_handle(const void* mapped, update_fn fn) : m_mapped(mapped), m_update(fn)
			{
				if (!mapped) throw std::runtime_error("avatar_handle.ctor(..) - entity is null");
				if (!fn) throw std::runtime_error("avatar_handle.ctor(..) - fn is null");
			}

		public:
			void update()
			{
				m_avatar = m_update();
			}
			const void* ptr() const
			{
				return m_mapped;
			}
			const avatar_t& operator->() const
			{
				return m_avatar;
			}
		};
	}
}

#endif