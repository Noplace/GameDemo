#include <Aurora/Code/aurora.h>
#include "physical_object.h"

namespace pong {


class System : public aurora::EngineComponent {
 public:
  void Initialize();
  void Deinitialize();
  void Update(float dt);
  Ball ball;
  Box table;
  Paddle paddle1;
  Paddle paddle2;
};

}