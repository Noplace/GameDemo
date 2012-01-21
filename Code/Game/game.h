#ifndef GAME_H
#define GAME_H

#include <Aurora/Code/aurora.h>
#include "Pong/pong.h"


namespace my_app {
class Game;
}
#include "States/main_menu.h"
#include "States/level.h"

namespace my_app {


class Game {
 public:
  Game();
  ~Game();
  void Initialize(core::windows::Window* window);
  void Loop();
  void HandleWindowMessages(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam);
  graphics::font::Writer& font_writer() { return font_writer_; };
  graphics::shader::Shader2DHelper& shader_helper() { return shader_helper_; };
  aurora::StateManager& state_manager() { return state_manager_; };
 private:
  aurora::Engine engine;
  utilities::Timer main_timer_;
  core::windows::Window* window_;
  graphics::font::Writer font_writer_;
  graphics::shader::Shader2DHelper shader_helper_;
  aurora::StateManager state_manager_;
  Level level;
  MainMenu mainmenu;
};

}

#endif

