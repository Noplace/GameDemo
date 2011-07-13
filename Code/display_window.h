#ifndef UISYSTEM_DISPLAY_WINDOW_H
#define UISYSTEM_DISPLAY_WINDOW_H

#include <WinCore/windows/windows.h>
#include "Game/game.h"

namespace my_app {

/*
  Class Name  : DisplayWindow
  Description : this is the application's main window
*/
class DisplayWindow: public core::windows::Window {
  public:
    DisplayWindow();
    ~DisplayWindow();
    void Initialize();
    void Step();
   protected:
    int OnCreate(WPARAM wParam,LPARAM lParam);
    int OnDestroy(WPARAM wParam,LPARAM lParam);
    int OnCommand(WPARAM wParam,LPARAM lParam);
  private:
    Game game;
};

}

#endif