// name: engine.h
// type: c++ header
// desc: class declaration
// auth: is0urce

// main facade shell to assemble everything together
// general handling of time and input
// holding rendering pipeline and interface

// rendering environment specific ogl hanlding consumed by constructor and should exist as long as engine existing
// input routed either to interface or to game controls

#ifndef PX_CORE_ENGINE_H
#define PX_CORE_ENGINE_H

#include <px/point.hpp>

#include <px/key.h>

#include <memory>

namespace px
{
	class timer;
	namespace ui
	{
		class canvas;
		class stack_panel;
	}
	namespace shell
	{
		class opengl;
		class renderer;
		class fps_counter;
	}
	namespace rl
	{
		class scene;
	}
	namespace core
	{
		class game;
		class library;

		class engine
		{
		private:
			shell::opengl* m_ogl;

			std::shared_ptr<ui::stack_panel> m_ui;
			std::unique_ptr<ui::canvas> m_canvas;

			std::unique_ptr<rl::scene> m_scene;
			std::unique_ptr<shell::renderer> m_renderer;
			std::unique_ptr<game> m_game;
			std::unique_ptr<library> m_lib;

			// environment
			std::unique_ptr<timer> m_timer;
			std::unique_ptr<shell::fps_counter> m_performance;
			bool m_shutdown;

		public:
			engine(shell::opengl *ogl);
			virtual ~engine();

			void frame();

			bool press(key vk);
			bool hover(const point &screen);
			bool click(const point &screen, unsigned int button);
			bool scroll(int delta);

			bool running() const;
			void shutdown();
		};
	}
}

#endif