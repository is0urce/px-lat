// name: engine.cpp
// type: c++
// desc: class implementation
// auth: is0urce

// facade

#include "engine.h"

#include <px/core/game.h>

#include <px/ui/canvas.h>
#include <px/ui/board_panel.h>
#include <px/ui/text_panel.h>

// stl includes
#include <algorithm>
#include <string>

namespace px
{
	namespace core
	{
		engine::engine(shell::opengl *ogl)
			:
			m_shutdown(false), m_ogl(ogl)
		{
			// start timers and reseed rng
			m_performance = std::make_unique<shell::fps_counter>();
			std::srand((unsigned int)m_timer.counter());

			// create rendering pipeline
			m_renderer = std::make_unique<shell::renderer>(ogl);

			// interface
			m_ui = std::make_shared<ui::stack_panel>();
			m_ui->add(std::make_shared<ui::text_panel>("Hi!", color(1, 1, 1)), ui::alignment({ 0.0f, 0.0f }, { 0, 0 }, { 0, 0 }, { 0.0f, 0.0f }));
			m_ui->add(std::make_shared<ui::board_panel>(color(0, 0, 0.5)), ui::alignment({ 0.0f, 0.0f }, { 1, 1 }, { 15, 5 }, { 0.0f, 0.0f }));

			// game setup
			m_lib = std::make_unique<rl::library>(m_renderer->sprite_manager(), m_renderer->sprite_sheet());
			m_world = std::make_unique<rl::world>(m_lib.get());
			m_scene = std::make_unique<rl::scene>(m_world.get());
			m_game = std::make_unique<game>(m_lib.get(), m_scene.get(), m_renderer->perception());

			m_game->start();
		}
		engine::~engine()
		{
			m_game.reset();
			m_scene.reset();
			m_world.reset();
			m_lib.reset();
		}

		void engine::frame()
		{
			m_performance->frame_processed();

			auto current = m_renderer->perception()->version();
			if (m_version != current)
			{
				m_version = current;
				m_timer.restart();
			}

			auto &cnv = m_renderer->canvas();
			m_ui->output(cnv);
			cnv.write({ 1, 1 }, "fps:");
			cnv.write({ 6, 1 }, std::to_string(m_performance->fps()));

			m_renderer->render(m_timer.measure());
		}

		bool engine::press(key vk)
		{
			if (vk == key::command_cancel)
			{
				shutdown();
			}
			return (m_ui && m_ui->key(vk)) || m_game->key(vk);
		}
		bool engine::hover(const point &screen)
		{
			return (m_ui && m_ui->hover(screen)) || m_game->hover(screen);
		}
		bool engine::click(const point &screen, unsigned int button)
		{
			return (m_ui && m_ui->click(screen, button)) || m_game->click(screen, button);
		}
		bool engine::scroll(int delta)
		{
			bool result = m_ui && m_ui->scroll(delta);
			if (!result)
			{
				m_renderer->scale(delta);
			}
			return result;
		}

		bool engine::running() const
		{
			return !m_shutdown;
		}
		void engine::shutdown()
		{
			m_shutdown = true;
		}
	}
}