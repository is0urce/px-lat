// name: core.cpp
// type: c++
// desc: class implementation
// auth: is0urce

#include "engine.h"

#include <px/core/game.h>
#include <px/shell/renderer.h>
#include <px/rl/scene.h>

#include <px/ui/canvas.h>
#include <px/ui/stack_panel.h>
#include <px/ui/board_panel.h>

#include <px/shell/timer.h>
#include <px/shell/fps_counter.h>
#include <px/shell/opengl.h>

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
			m_timer = std::make_unique<timer>();
			m_performance = std::make_unique<shell::fps_counter>();
			std::srand((unsigned int)m_timer->counter());

			// create rendering pipeline
			m_renderer = std::make_unique<shell::renderer>(ogl);

			// interface
			m_canvas = std::make_unique<ui::canvas>(1, 1);
			m_ui = std::make_shared<ui::stack_panel>();
			m_ui->add(std::make_shared<ui::board_panel>(color(0, 0, 0.5)), ui::alignment({ 0.0f, 0.0f }, { 1, 1 }, { -2, -2 }, { 1.0f, 1.0f }));

			// game
			m_scene = std::make_unique<rl::scene>();
			m_scene->on_add
				([this](rl::scene::unit_ptr unit)
			{
				m_renderer->add({ unit.get(), [unit]()
				{
					shell::renderer::avatar_t result;
					result.position = { 0, 0 };
					result.size = 1;
					result.img = '@';
					return result;
				} });
			});
			m_scene->on_remove([&](rl::scene::unit_ptr unit)
			{
				m_renderer->remove(unit.get());
			});

			m_game = std::make_unique<game>(m_scene.get(), m_ui.get());
		}
		engine::~engine() {}

		void engine::frame()
		{
			m_performance->frame_processed();

			int w, h;
			m_ogl->update(w, h);
			w = (std::max<int>)(1, w / shell::renderer::ui_cell_width);
			h = (std::max<int>)(1, h / shell::renderer::ui_cell_height);
			m_canvas->resize(w, h);
			m_ui->layout({ { 0, 0 }, { w, h } });
			m_ui->draw(*m_canvas);
			m_canvas->write({ 0, 0 }, "fps:");
			m_canvas->write({ 5, 0 }, std::to_string(m_performance->fps()));

			m_renderer->render(*m_canvas, m_timer->measure());
		}

		bool engine::press(key vk)
		{
			bool result = false;
			switch (vk)
			{
			case key::command_cancel:
				shutdown();
				result = true;
				break;
			case key::move_none:
				result = m_game->step({ 0, 0 });
				break;
			case key::move_north:
				result = m_game->step({ 0, 1 });
				break;
			case key::move_south:
				result = m_game->step({ 0, -1 });
				break;
			case key::move_east:
				result = m_game->step({ 1, 0 });
				break;
			case key::move_west:
				result = m_game->step({ -1, 0 });
				break;
			case key::move_southwest:
				result = m_game->step({ -1, -1 });
				break;
			case key::move_southeast:
				result = m_game->step({ 1, -1 });
				break;
			case key::move_northwest:
				result = m_game->step({ -1, 1 });
				break;
			case key::move_northeast:
				result = m_game->step({ 1, 1 });
				break;
			case key::action_use:
				result = m_game->use();
				break;
			case key::action0:
				result = m_game->cast(0);
				break;
			case key::action1:
				result = m_game->cast(1);
				break;
			case key::action2:
				result = m_game->cast(2);
				break;
			case key::action3:
				result = m_game->cast(3);
				break;
			case key::action4:
				result = m_game->cast(4);
				break;
			case key::action5:
				result = m_game->cast(5);
				break;
			default:
				result = false;
				break;
			}
			return result;
		}
		bool engine::hover(const point &screen)
		{
			return false;
		}
		bool engine::click(const point &screen, unsigned int button)
		{
			return false;
		}
		bool engine::scroll(int delta)
		{
			return false;
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