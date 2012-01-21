#include <Aurora/Code/aurora.h>
#include <VisualEssence/Code/context/context.h>
#include <VisualEssence/Code/shape/rectangle.h>
#include "physical_object.h"

namespace pong {

class System;

struct Ball : public aurora::EngineComponent, public Circle, public PhysicalObject {
  System* system;
  float object_time;

  Ball() : PhysicalObject() {
    radius = 1.0f;
    mass = 1.0f;
  }

  void PrintInfo();
  void Initialize(System* system);
  void Reset();
  void Update(float dt);
};

}