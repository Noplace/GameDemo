#include "display_window.h"

namespace my_app {

class ClientApp : public core::windows::Application {
 public:
   ClientApp(HINSTANCE instance , LPSTR command_line, int show_command) : 
       core::windows::Application(instance,command_line,show_command) {
  
  }
  int Run() {
    //if (RanBefore("My Super App") == true) {
    //  MessageBox(NULL,"App Already Running!","Error",MB_ICONWARNING);
    //  return -1;
    //}
    
    MSG msg;
    display_window.Initialize();

    do {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      } else {
        display_window.Step();
      }
    } while(msg.message!=WM_QUIT);
 
    //Return the exit code to the system. 
    return static_cast<int>(msg.wParam);
  }
 private:
   DisplayWindow display_window;
};

}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
  my_app::ClientApp app(hInstance,lpCmdLine,nShowCmd);
  return app.Run();
}