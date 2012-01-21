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

//const double M_PI = 3.14159265358979323846;
/*
  <summary>Constructor</summary>
  <para>DisplayWindow Constructor</para>
*/
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

int DisplayWindow::OnKeyDown(WPARAM wParam,LPARAM lParam) {
  return 0;
}

int DisplayWindow::OnKeyUp(WPARAM wParam,LPARAM lParam) {
  return 0;
}

int DisplayWindow::OnMouseMove(WPARAM wParam,LPARAM lParam) {
  return 0;
}

int DisplayWindow::PreProcessMessages(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  game.HandleWindowMessages(hwnd,uMsg,wParam,lParam);
  return -1; //to enable normal message processing
}

}