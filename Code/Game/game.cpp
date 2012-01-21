#include "game.h"


namespace my_app {

Game::Game() {
  
}

Game::~Game() {
  state_manager_.Deinitialize();
  font_writer_.Deinitialize();
  shader_helper_.Deinitialize();
  engine.Deinitialize();
}


void Game::Initialize(core::windows::Window* window)  {
 

  window_ = window;
  main_timer_.Calibrate();
  engine.set_timer(&main_timer_);
  engine.set_window(window_);
  engine.Initialize();
  engine.resource_manager.LoadXml("Content\\test.xml");
  //Initialize common core classes
  shader_helper_.Initialize(&engine.gfx_context());

  {
    aurora::resource::Resource* shader_res = engine.resource_manager.GetResourceById<aurora::resource::Resource>(3);
    shader_helper_.CreateFromMemory(shader_res->data_pointer,shader_res->data_length);
    shader_res->Unload();
  }

  {
    font_writer_.Initialize(&engine.gfx_context());
    aurora::resource::FontResource* fr = engine.resource_manager.GetResourceById<aurora::resource::FontResource>(7);
    auto font = fr->font();
    
    font_writer_.set_font(font);
  }
  font_writer_.PrepareWrite(512);

  //Initialize states & state manager
  state_manager_.Initialize(&engine);
  mainmenu.game_ = this;
  level.game_ = this;
  state_manager_.AddState(&mainmenu);
  state_manager_.AddState(&level);
  state_manager_.ChangeState(0);
  engine.current_scene = &state_manager_;

}

void Game::Loop() {
  engine.Loop();
}

void Game::HandleWindowMessages(HWND window_handle, UINT message, WPARAM wparam, LPARAM lparam) {
  engine.HandleWindowMessages(window_handle,message,wparam,lparam);
}

}

