#include <Aurora/Code/aurora.h>

namespace pong {


class PSM : public aurora::StateManager {
 public:
  int Initialize(aurora::Engine* engine);
  int Deinitialize();
  MainMenu main_menu;
};


int PSM::Initialize(aurora::Engine* engine) {
  StateManager::Initialize(engine);

  this->AddState(&main_menu);
  return S_OK;
}

int PSM::Deinitialize() {
  StateManager::Deinitialize();
  return S_OK;
}



}