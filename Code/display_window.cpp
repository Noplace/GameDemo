/******************************************************************************
* Filename    : display_window.cpp
* Description : 
* 
*
* 
* 
* 
*******************************************************************************/
#include "display_window.h"


namespace my_app {

//const double M_PI = 3.141592653589793238460;

DisplayWindow::DisplayWindow() : core::windows::Window() {
  
}

DisplayWindow::~DisplayWindow() {

}


void DisplayWindow::Initialize() {
  this->Create("Test");
  SetClientSize(640,480);
  Center();
  Show();

  game.Initialize(this);
}

void DisplayWindow::Step() {
  
  //gfx_context_.Render();
  game.Loop();
}

int DisplayWindow::OnCreate(WPARAM wParam,LPARAM lParam) {
  return 0;
}

int DisplayWindow::OnDestroy(WPARAM wParam,LPARAM lParam) {
  PostQuitMessage(0);
  return 0;
}

int DisplayWindow::OnCommand(WPARAM wParam,LPARAM lParam) {
  return 0;
}

}