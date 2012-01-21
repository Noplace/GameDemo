#include <Aurora/Code/aurora.h>
#include <VisualEssence/Code/context/context.h>
#include <VisualEssence/Code/shape/rectangle.h>
#include "physical_object.h"

namespace pong {

class Paddle : public aurora::EngineComponent {
 public:
  PhysicalObject physics;
  graphics::shape::Rectangle shape;
  XMFLOAT2 position;
  void Initialize(float width, float height);
  void Update(float dt);
  void Draw();
  Box box();
};

}