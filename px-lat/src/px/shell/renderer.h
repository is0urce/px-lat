// name: renderer.h
// type: c++ header file
// desc: opengl rendering
// auth: is0urce

// extends ogl with drawing functions

#ifndef PX_SHELL_RENDERER_H
#define PX_SHELL_RENDERER_H

#include <px/shell/opengl.h>
#include <px/shell/vao.h>
#include <px/point.hpp>
#include <px/vector.hpp>

#include <memory>

namespace px
{
	class unit;
	class tile;

	namespace shell
	{
		class font;
		class opengl;
		class avatar;

		class renderer
		{
		private:
			opengl* m_opengl;
			int m_width, m_height;
			double m_aspect;
			double m_scale;
			vector m_center;
			double m_move_phase;

			GLuint m_sampler;
			struct s_font
			{
				std::unique_ptr<font> font;
				GLuint texture;
			}
				m_ui,
				m_popup,
				m_glyph;

			struct drawcall
			{
			public:
				vao vao;
				GLuint program;

			public:
				drawcall(const std::vector<unsigned int> &depths, GLuint program_id) : vao(depths), program(program_id) {}
			}
				m_background,
				m_tiles,
				m_units,
				m_notification,
				m_projectiles,
				m_light, m_lightmap,
				m_lightmap_prev,
				m_scene,
				m_lightdraw,
				m_uiback,
				m_uitext;

			GLuint m_scene_frame, m_light_frame, m_lightmap_frame; // framebuffers
			GLuint m_scene_texture, m_light_texture; // framebuffer textures for g-geometry and lights
			GLuint m_lightmap_texture, m_lightmap_prev_texture; // static lights textures
			unsigned int m_scene_size;
			unsigned int m_lightmap_size;

		private:
			void update_textures();
			void setup_scene();

		public:
			renderer(opengl *opengl);
			virtual ~renderer();

		public:
			void add(std::unique_ptr<avatar>&);
			void remove(void*);
		};
	}
}

#endif