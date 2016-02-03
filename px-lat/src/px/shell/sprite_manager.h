// name: component.hp
// type: c++ header
// desc: class definition
// auth: is0urce

#ifndef PX_SHELL_SPRITE_MANAGER_H
#define PX_SHELL_SPRITE_MANAGER_H

#include <px/rl/component.hpp>
#include <px/point.hpp>
#include <px/color.hpp>

namespace px
{
	namespace shell
	{
		namespace
		{
			template<typename _T>
			const _T* to_pointer(const std::vector<_T>& a)
			{
				return (a.size() == 0) ? nullptr : &a[0];
			}
			template<typename _T>
			_T* to_pointer(std::vector<_T>& a)
			{
				return (a.size() == 0) ? nullptr : &a[0];
			}
		}
		struct sprite
		{
			unsigned int atlas;
			float left, right, bottom, top;
			float width, height;
			color tint;
			double transparency;
		};
		struct location
		{
			point position;
		};

		class sprite_component;
		class location_component;

		class sprite_component
			: public sprite
			, public rl::component_link<location_component>
			, public rl::component<rl::component_manager<sprite_component, 100>>
		{
		public:
			sprite_component()
			{
			}
			virtual ~sprite_component()
			{
			}
		};

		class location_component
			: public location
			, public rl::component<rl::component_manager<location_component, 100>>
		{
		public:
			location_component()
			{
			}
			virtual ~location_component()
			{
			}
		};

		class sprite_manager : public rl::component_manager<sprite_component, 100>
		{
		private:
			unsigned int m_length;
			std::vector<float> m_vertice;
			std::vector<float> m_color;
			std::vector<float> m_texture;
			std::vector<unsigned int> m_index;

		public:
			void draw(float*& vertex, float*& color, float*& texture, unsigned int*& index)
			{
				vertex = to_pointer(m_vertice);
				color = to_pointer(m_color);
				texture = to_pointer(m_texture);
				index = to_pointer(m_index);
			}
			void construct()
			{
				unsigned int len = count();
				m_vertice.resize(len * 4 * 4);
				m_color.resize(len * 4 * 4);
				m_texture.resize(len * 4 * 2);
				m_index.resize(len * 2 * 3);

				m_length = 0;
				unsigned int vertex_offset = 0;
				unsigned int color_offset = 0;
				unsigned int texture_offset = 0;
				update([&](sprite_component &sprite)
				{
					// vertex coordinates
					auto* location = (location_component*)sprite;
					auto horisontal = location->position.X * sprite.width * 0.5f;
					auto vertical = location->position.Y * sprite.height * 0.5f;

					m_vertice[vertex_offset + 0] = horisontal - 0.5f;
					m_vertice[vertex_offset + 1] = vertical - 0.5f;
					m_vertice[vertex_offset + 2] = 0;
					m_vertice[vertex_offset + 3] = 1;

					m_vertice[vertex_offset + 4] = horisontal - 0.5f;
					m_vertice[vertex_offset + 5] = vertical + 0.5f;
					m_vertice[vertex_offset + 6] = 0;
					m_vertice[vertex_offset + 7] = 1;

					m_vertice[vertex_offset + 8] = horisontal + 0.5f;
					m_vertice[vertex_offset + 9] = vertical + 0.5f;
					m_vertice[vertex_offset + 10] = 0;
					m_vertice[vertex_offset + 11] = 1;

					m_vertice[vertex_offset + 12] = horisontal + 0.5f;
					m_vertice[vertex_offset + 13] = vertical - 0.5f;
					m_vertice[vertex_offset + 14] = 0;
					m_vertice[vertex_offset + 15] = 1;
					
					// color tint
					sprite.tint.write(&m_color[color_offset], 4);

					// texture coordinates
					m_texture[texture_offset + 0] = sprite.left;
					m_texture[texture_offset + 1] = sprite.bottom;
					m_texture[texture_offset + 2] = sprite.left;
					m_texture[texture_offset + 3] = sprite.top;
					m_texture[texture_offset + 4] = sprite.right;
					m_texture[texture_offset + 5] = sprite.top;
					m_texture[texture_offset + 6] = sprite.right;
					m_texture[texture_offset + 7] = sprite.bottom;

					vertex_offset += 4 * 4;
					color_offset += 4 * 4;
					texture_offset += 4 * 2;

					++m_length;
				});

				// indices
				unsigned int index_offset;
				for (unsigned int i = 0; i < m_length; ++i)
				{
					m_index[index_offset + 0] = i + 0;
					m_index[index_offset + 1] = i + 2;
					m_index[index_offset + 2] = i + 1;
					m_index[index_offset + 3] = i + 0;
					m_index[index_offset + 4] = i + 3;
					m_index[index_offset + 5] = i + 2;

					++index_offset;
				}
			}
		};
	}
}

#endif