#ifndef GAME_H
#define GAME_H

#include <WinCore/types.h>
#include <WinCore/timer/timer2.h>
#include <WinCore/windows/windows.h>
#include <GameEngine/game_engine.h>
#include "scene1.h"
#include "scene2.h"

namespace my_app {




class Game {
 public:
   Game();
   ~Game();
   void Initialize(core::windows::Window* window);
   void Loop();
 private:
  game_engine::Engine engine;
  utilities::Timer main_timer_;

  core::windows::Window* window_;



};

}

#endif

