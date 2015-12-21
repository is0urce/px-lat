// name: avatar_handle.h
// type: c++ header
// desc: class declaration
// auth: is0urce

// avatar representation binded to unit with std::function

#ifndef PX_SHELL_AVATAR_HANDLE_H
#define PX_SHELL_AVATAR_HANDLE_H

#include <functional>
#include <stdexcept>

namespace px
{
	namespace shell
	{
		template <typename _E, _A>
		class avatar_handle
		{
		private:
			typedef _E entity_t;
			typedef _A avatar_t;
			typedef std::function<avatar_t(const entity_t&)> update_fn;

		private:
			avatar_t m_avatar;
			entity_t* m_mapped;
			update_fn m_update;
			_A appearance_t;

		public:
			avatar_handle(entity_t* mapped, update_fn fn) : m_mapped(mapped), m_update(fn)
			{
				if (!mapped) throw std::runtime_error("avatar_handle.ctor(..) - entity is null");
				if (!fn) throw std::runtime_error("avatar_handle.ctor(..) - fn is null");
			}

		public:
			void update()
			{
				m_avatar = m_update(*m_mapped);
			}
			entity_t* ptr() const
			{
				return m_mapped;
			}
			const m_avatar& operator->() const
			{
				return m_avatar;
			}
		};
	}
}

#endif