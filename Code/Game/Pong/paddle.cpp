#include <Windows.h>
#include <xnamath.h>
#include <stdio.h>
#include "paddle.h"

namespace pong {

void Paddle::Initialize(float width, float height) {
  physics.mass = 2;
  shape.Initialize(&engine_->gfx_context());
  shape.SetParams(30,180);
  shape.SetColor(XMCOLOR(0xff00ff00));
  shape.Construct();
  position.x = 0;
  position.y = 0;
}

void Paddle::Update(float dt) {
  shape.SetTopLeft(position.x,position.y);
  shape.BuildTransform();
}

void Paddle::Draw() {
  shape.Draw();
}

Box Paddle::box() {
  Box b = {position.x,position.y,position.x+shape.width(),position.y+shape.height()};
  return b;
}

}