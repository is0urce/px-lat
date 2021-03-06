// name: renderer.cpp
// type: c++ class implementation
// desc: opengl rendering
// auth: is0urce

#include "renderer.h"

#include <px/shell/opengl.h>
#include <px/shell/font.h>

#include <px/color.hpp>

#include <memory>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <functional>

#include <lodepng.h>

namespace px
{
	namespace shell
	{
		namespace
		{
			const unsigned int quad = 4; // four vertices for quad
			const unsigned int strip = 6; // six indices for 2-triangles
			const unsigned int texture_depth = 2;
			const unsigned int vertex_depth = 4;
			const unsigned int color_depth = 4;

			const double zoom_exp = 1.0 / 1000;
			const double zoom_min = 0.01;
			const double zoom_max = 10000;

			const unsigned int perception_reach = 17;
			const unsigned int perception_width = perception_reach * 2 + 1;
			const unsigned int perception_height = perception_width;
			const unsigned int perception_size = perception_width * perception_height;
			const point perception_range(perception_width, perception_height);
			const point perception_half(perception_width / 2, perception_height / 2);

			const float move_speed = 5.0f;

			const char* font_name = "PragmataPro.ttf";
			const unsigned int font_size = renderer::ui_cell_height;

			const char* texture_path = "textures/compiled.png";
			const char* texture_desc = "textures/compiled.json";

			void fill_vertex(const point &position, GLfloat* dest)
			{
				GLfloat x = (GLfloat)position.X;
				GLfloat y = (GLfloat)position.Y;
				GLfloat dx = (GLfloat)(position.X + 1);
				GLfloat dy = (GLfloat)(position.Y + 1);
				x -= 0.5f;
				y -= 0.5f;
				dx -= 0.5f;
				dy -= 0.5f;

				dest[0] = x;
				dest[1] = y;
				dest[2] = 0;
				dest[3] = 1;
				dest[4] = x;
				dest[5] = dy;
				dest[6] = 0;
				dest[7] = 1;
				dest[8] = dx;
				dest[9] = dy;
				dest[10] = 0;
				dest[11] = 1;
				dest[12] = dx;
				dest[13] = y;
				dest[14] = 0;
				dest[15] = 1;
			}
			void fill_vertex(const vector &position, const vector &quad_range, GLfloat *dest)
			{
				(position + vector(-0.5, -0.5) * quad_range).write2(dest + 0 * vertex_depth);
				(position + vector(-0.5, 0.5) * quad_range).write2(dest + 1 * vertex_depth);
				(position + vector(0.5, 0.5) * quad_range).write2(dest + 2 * vertex_depth);
				(position + vector(0.5, -0.5) * quad_range).write2(dest + 3 * vertex_depth);
			}
			void fill_color(const color &c, GLfloat *dest)
			{
				c.write(dest, quad);
			}
			void fill_texture(GLfloat left, GLfloat bottom, GLfloat right, GLfloat top, GLfloat *dest)
			{
				dest[0] = left;
				dest[1] = bottom;
				dest[2] = left;
				dest[3] = top;
				dest[4] = right;
				dest[5] = top;
				dest[6] = right;
				dest[7] = bottom;
			}
			inline void fill_index(unsigned int num, GLuint *dest)
			{
				unsigned int index_offset = 0;
				for (unsigned int i = 0; i < num; ++i)
				{
					dest[index_offset + 0] = i * quad + 0;
					dest[index_offset + 1] = i * quad + 2;
					dest[index_offset + 2] = i * quad + 1;
					dest[index_offset + 3] = i * quad + 0;
					dest[index_offset + 4] = i * quad + 3;
					dest[index_offset + 5] = i * quad + 2;
					index_offset += strip;
				}
			}
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
		renderer::renderer(opengl *opengl)
			: m_opengl(opengl)
			, m_scale(0.05f), m_pixel_scale(2)
			, m_canvas(1, 1) // any but zero, resised anyway
			, m_perception(perception_range)
		{
			if (!opengl) throw std::runtime_error("renderer::renderer(opengl* opengl) - opengl is null");

			// opengl setup
			glEnable(GL_TEXTURE_2D);
			glClampColor(GL_CLAMP_VERTEX_COLOR, GL_FALSE); // don't clamp hdr
			glClampColor(GL_CLAMP_READ_COLOR, GL_FALSE);
			glClampColor(GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

			// textures
			std::vector<unsigned char> image; //the raw pixels
			unsigned int width, height;
			unsigned int error = lodepng::decode(image, width, height, texture_path);
			if (error) throw std::runtime_error("renderer::renderer - can't read file, error code #" + std::to_string(error));
			m_tile.sheet.init(width, height, 8, &image[0]);
			m_tile.sheet.bind(0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			m_sheet.init(texture_desc, width, height);

			// tiles
			m_tile.vao = vao({ vertex_depth, color_depth, texture_depth });
			m_tile.vertices.resize(perception_size * vertex_depth * quad, 0);
			m_tile.colors.resize(perception_size * color_depth * quad);
			m_tile.textcoords.resize(perception_size * texture_depth * quad);
			m_tile.indices.resize(perception_size * strip);
			m_tile.shader = program("shaders/tile");
			m_tile.shader.activate();
			m_tile.shader.uniform("img", 0);
			m_tile.shader.uniform("offset", (GLfloat)(-perception_half.X), (GLfloat)(-perception_half.Y));
			m_tile.shader.prepare([this
				, scale = m_tile.shader.uniform("scale")
				, offset = m_tile.shader.uniform("offset")]()
			{
				m_tile.sheet.bind(0);
				program::uniform(scale, (GLfloat)m_scale, (GLfloat)(m_scale * m_aspect));
				program::uniform(offset, (GLfloat)(m_camera.X), (GLfloat)(m_camera.Y));
			});

			// unit sprites
			m_sprite.vao = vao({ vertex_depth, color_depth, texture_depth });
			m_sprite.shader = program("shaders/sprite");
			m_sprite.shader.activate();
			m_sprite.shader.uniform("img", 0);
			m_sprite.shader.prepare([this
				, scale = m_sprite.shader.uniform("scale")
				, offset = m_sprite.shader.uniform("offset")]()
			{
				m_tile.sheet.bind(0);
				program::uniform(scale, (GLfloat)m_scale, (GLfloat)(m_scale * m_aspect));
				program::uniform(offset, (GLfloat)(m_camera.X), (GLfloat)(m_camera.Y));
			});

			// ui background
			m_ui.bg.vao = vao({ 2, color_depth });
			m_ui.bg.shader = program("shaders/ui_bg");
			m_ui.bg.shader.prepare([this
				, scale = m_ui.bg.shader.uniform("scale")
				, offset = m_ui.bg.shader.uniform("offset")
			]()
			{
				program::uniform(scale, (GLfloat)m_ui.scale_x, (GLfloat)m_ui.scale_y);
				program::uniform(offset, (GLfloat)m_ui.offset_x, (GLfloat)m_ui.offset_y); 
			});

			// ui font
			m_ui.text.font = std::make_unique<font>(font_name, font_size);
			m_ui.text.vao = vao({ 2, color_depth, texture_depth });
			m_ui.text.shader = program("shaders/ui_text");
			m_ui.text.shader.activate();
			m_ui.text.shader.uniform("font", 0);
			m_ui.text.shader.prepare([this
				, scale = m_ui.text.shader.uniform("scale")
				, offset = m_ui.text.shader.uniform("offset")
			]()
			{
				program::uniform(scale, (GLfloat)m_ui.scale_x, (GLfloat)m_ui.scale_y);
				program::uniform(offset, (GLfloat)m_ui.offset_x, (GLfloat)m_ui.offset_y);
				m_ui.text.font.bind(0);
			});

#if _DEBUG
			GLenum err = GL_NO_ERROR;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				throw std::runtime_error("renderer::renderer() - OpenGL error #" + std::to_string(err));
			}
#endif
		}
		renderer::~renderer()
		{
		}

		void renderer::render(time_t time)
		{
			m_opengl->update(m_width, m_height);
			pixel_clip(32, m_pixel_scale);

			m_aspect = m_width;
			m_aspect /= m_height;

			m_camera = m_perception.movement();
			m_camera *= std::max<time_t>(1 - time * move_speed, 0);
			m_camera -= perception_half;

			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			draw_terrain(time);
			draw_sprites(time);
			draw_canvas(m_canvas);

			m_opengl->swap();

#if _DEBUG
			GLenum err = GL_NO_ERROR;
			while ((err = glGetError()) != GL_NO_ERROR)
			{
				throw std::runtime_error("renderer::render(..) - OpenGL error");
			}
#endif
		}

		void renderer::draw_terrain(time_t time)
		{
			unsigned int vertex_offset = 0;
			unsigned int color_offset = 0;
			unsigned int texture_offset = 0;
			point tile_size(1, 1);
			rectangle({ 0, 0 }, perception_range).enumerate([&](const point& position)
			{
				const auto &sprite = m_perception.ground(position);

				fill_vertex(position, &m_tile.vertices[vertex_offset]);
				fill_color(sprite.tint, &m_tile.colors[color_offset]);
				fill_texture(sprite.left, sprite.bottom, sprite.right, sprite.top, &m_tile.textcoords[texture_offset]);

				vertex_offset += vertex_depth * quad;
				color_offset += color_depth * quad;
				texture_offset += texture_depth * quad;
			});

			fill_index(perception_size, &m_tile.indices[0]);

			// setup states
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// update vao and draw
			m_tile.shader.use();
			m_tile.vao.fill_attributes(perception_size * quad * vertex_depth, 0, to_pointer(m_tile.vertices));
			m_tile.vao.fill_attributes(perception_size * quad * color_depth, 1, to_pointer(m_tile.colors));
			m_tile.vao.fill_attributes(perception_size * quad * texture_depth, 2, to_pointer(m_tile.textcoords));
			m_tile.vao.fill_indices(perception_size * strip, to_pointer(m_tile.indices));
			m_tile.vao.draw();
		}

		void renderer::draw_sprites(time_t time)
		{
			unsigned int count = m_sprite.manager.count();
			unsigned int length = 0; // length of enabled sprites (length <= count)
			m_sprite.vertices.resize(count * quad * vertex_depth);
			m_sprite.colors.resize(count * quad * color_depth);
			m_sprite.texture.resize(count * quad * texture_depth);

			vector start = m_perception.start();

			unsigned int vertex_offset = 0;
			unsigned int color_offset = 0;
			unsigned int texture_offset = 0;
			point sprite_size(1, 1);
			m_sprite.manager.update([&](sprite_component &sprite)
			{
				// vertex coordinates
				auto* location = (rl::location_component*)sprite;
				if (!location) throw std::runtime_error("sprite_manager::update - location link is null");
				vector position = vector(location->prevoius()).lerp(vector(location->position()), std::min<time_t>(1, time * move_speed));

				fill_vertex(position - start, sprite_size, &m_sprite.vertices[vertex_offset]);
				fill_color(sprite.tint, &m_sprite.colors[color_offset]);
				fill_texture(sprite.left, sprite.bottom, sprite.right, sprite.top, &m_sprite.texture[texture_offset]);

				vertex_offset += quad * vertex_depth;
				color_offset += quad * color_depth;
				texture_offset += quad * texture_depth;

				++length;
			});

			// indices
			if (length > m_sprite.max)
			{
				m_sprite.max = length;

				m_sprite.indices.resize(length * strip);
				fill_index(length, to_pointer(m_sprite.indices));
			}

			// setup states
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// update vao and draw
			m_sprite.shader.use();
			m_sprite.vao.fill_attributes(length * quad * vertex_depth, 0, to_pointer(m_sprite.vertices));
			m_sprite.vao.fill_attributes(length * quad * color_depth, 1, to_pointer(m_sprite.colors));
			m_sprite.vao.fill_attributes(length * quad * texture_depth, 2, to_pointer(m_sprite.texture));
			m_sprite.vao.fill_indices(length * strip, to_pointer(m_sprite.indices));
			m_sprite.vao.draw();
		}

		void renderer::draw_canvas(const ui::canvas& canvas)
		{
			// ui
			int ui_w = canvas.width();
			int ui_h = canvas.height();
			if (ui_w <= 0 || ui_h <= 0) return;
			int size = ui_w * ui_h;

			// update cashed buffers if sizes have been changed
			if (ui_w != m_ui.width || ui_h != m_ui.height)
			{
				m_ui.width = ui_w;
				m_ui.height = ui_h;

				// update buffer sizes
				m_ui.indices.resize(size * strip);
				m_ui.bg.vertices.resize(size * 2 * quad);
				m_ui.bg.colors.resize(size * 4 * quad);
				m_ui.text.vertices.resize(size * 2 * quad);
				m_ui.text.colors.resize(size * 4 * quad);
				m_ui.text.texture.resize(size * 2 * quad);

				// cashe vertices
				unsigned int offset = 0;
				for (int j = 0; j < ui_h; ++j)
				{

					for (int i = 0; i < ui_w; ++i)
					{
						// cell coordinates
						int x = i * ui_cell_width;
						int dx = (i + 1) * ui_cell_width;
						int y = (ui_h - j - 1) * ui_cell_height;
						int dy = (ui_h - j) * ui_cell_height;

						m_ui.bg.vertices[offset + 0] = (GLfloat)x;
						m_ui.bg.vertices[offset + 1] = (GLfloat)y;
						m_ui.bg.vertices[offset + 2] = (GLfloat)x;
						m_ui.bg.vertices[offset + 3] = (GLfloat)dy;
						m_ui.bg.vertices[offset + 4] = (GLfloat)dx;
						m_ui.bg.vertices[offset + 5] = (GLfloat)dy;
						m_ui.bg.vertices[offset + 6] = (GLfloat)dx;
						m_ui.bg.vertices[offset + 7] = (GLfloat)y;

						offset += texture_depth * quad;
					}
				}

				fill_index(size, &m_ui.indices[0]);

				// fill cashed parts
				m_ui.bg.vao.fill_attributes(size * quad, 0, &m_ui.bg.vertices[0]);
				m_ui.bg.vao.fill_indices(size * strip, &m_ui.indices[0]);
				m_ui.text.vao.fill_indices(size * strip, &m_ui.indices[0]);
			}

			// fill buffers
			unsigned int color_offset = 0;
			unsigned int vertex_offset = 0;
			unsigned int texture_offset = 0;
			for (int j = 0; j < ui_h; ++j)
			{
				for (int i = 0; i < ui_w; ++i)
				{
					auto &symbol = canvas.at({ i, j });
					auto &g = m_ui.text.font->at(symbol.code);

					// vertices
					int x = i * ui_cell_width + ui_cell_width / 2 - g.pixel_width / 2;
					int xd = x + g.pixel_width;
					int y = (ui_h - j - 1) * ui_cell_height - g.pixel_height + g.pixel_top;
					int yd = y + g.pixel_height;

					m_ui.text.vertices[vertex_offset + 0] = (GLfloat)x;
					m_ui.text.vertices[vertex_offset + 1] = (GLfloat)y;
					m_ui.text.vertices[vertex_offset + 2] = (GLfloat)x;
					m_ui.text.vertices[vertex_offset + 3] = (GLfloat)yd;
					m_ui.text.vertices[vertex_offset + 4] = (GLfloat)xd;
					m_ui.text.vertices[vertex_offset + 5] = (GLfloat)yd;
					m_ui.text.vertices[vertex_offset + 6] = (GLfloat)xd;
					m_ui.text.vertices[vertex_offset + 7] = (GLfloat)y;

					// colors
					fill_color(symbol.back, &m_ui.bg.colors[color_offset]);
					fill_color(symbol.front, &m_ui.text.colors[color_offset]);

					// textures
					fill_texture((GLfloat)g.left, (GLfloat)g.bottom, (GLfloat)g.right, (GLfloat)g.top, &m_ui.text.texture[texture_offset]);

					vertex_offset += 2 * quad;
					color_offset += color_depth * quad;
					texture_offset += texture_depth * quad;
				}
			}

			// setup drawing
			glViewport(0, 0, (GLsizei)m_width, (GLsizei)m_height);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// uniforms preparation source
			m_ui.scale_x = 2.0f / m_width;
			m_ui.scale_y = 2.0f / m_height;
			m_ui.offset_x = (float)(m_width % ui_cell_width / 2);
			m_ui.offset_y = (float)(m_height % ui_cell_height / 2);

			// background
			m_ui.bg.shader.use();
			m_ui.bg.vao.fill_attributes(size * quad, 1, &m_ui.bg.colors[0]);
			m_ui.bg.vao.draw();

			// text
			m_ui.text.shader.use();
			m_ui.text.vao.fill_attributes(size * quad, 0, &m_ui.text.vertices[0]);
			m_ui.text.vao.fill_attributes(size * quad, 1, &m_ui.text.colors[0]);
			m_ui.text.vao.fill_attributes(size * quad, 2, &m_ui.text.texture[0]);
			m_ui.text.vao.draw();
		}

		void renderer::scale(double pan)
		{
			m_scale *= 1 + pan * zoom_exp;
			m_scale = (std::max)(zoom_min, (std::min)(m_scale, zoom_max)); // clamp

			m_pixel_scale = pan > 0 ? m_pixel_scale << 1 : m_pixel_scale >> 1;
			m_pixel_scale = (std::max)(1u, (std::min)(m_pixel_scale, 16u)); // clamp
		}
		void renderer::pixel_clip(unsigned int ppu, unsigned int multiplier)
		{
			m_scale = ppu * multiplier * 2; // 2 is size of opengl screen range (-1,1)
			m_scale /= m_width;
		}

		ui::canvas& renderer::canvas()
		{
			int w, h;
			m_opengl->update(w, h);
			w = (std::max<int>)(1, w / ui_cell_width);
			h = (std::max<int>)(1, h / ui_cell_height);
			m_canvas.resize(w, h);
			return m_canvas;
		}
	}
}