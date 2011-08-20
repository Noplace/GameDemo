#include "game.h"

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>


namespace my_app {

class MainScene3 : public game_engine::GameView {
 public:
  MainScene3() { }
  ~MainScene3() {}
  int Initialize(game_engine::Engine* engine) {
    mainscene1.Initialize(engine);
    mainscene2.Initialize(engine);
    return S_OK;
  }
  int Deinitialize() {
      mainscene2.Deinitialize();
    mainscene1.Deinitialize();
    return S_OK;
  }

  void Update(float delta_time) {
    mainscene2.Update(delta_time);
  }

  void Draw() {
    //mainscene1.Draw();
    mainscene2.Draw();
  } 
private:
    MainScene1 mainscene1;
  MainScene2 mainscene2;
};
MainScene3 ms3;

Game::Game() {
  
}

Game::~Game() {

  engine.Deinitialize();
}


void Game::Initialize(core::windows::Window* window)  {
 

  window_ = window;
  main_timer_.Calibrate();
  engine.set_timer(&main_timer_);
  engine.set_window(window_);
  engine.Initialize();
  ms3.Initialize(&engine);
  engine.current_scene = &ms3;


}

void Game::Loop() {
  engine.Loop();
}

}

