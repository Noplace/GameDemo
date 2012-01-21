#include <Windows.h>
#include <xnamath.h>
#include <stdio.h>
#include <time.h>
#include "pong.h"

namespace pong {

void Ball::PrintInfo() {
  printf("Ball Values:\n",radius);
  PhysicalObject::PrintInfo();
  printf("radius\t\t: %0.4f\n",radius);
}

void Ball::Initialize(System* system) {
  this->system = system;
  Reset();
}

void Ball::Reset() {

  FLOAT angle;
  //set random angle for initial impulse
  srand((unsigned int)time(NULL));
  angle = ((FLOAT)rand() / RAND_MAX ) * 360;
  if (angle > 45 && angle < 135) {
    angle -= 90;
  } else if (angle > 225 && angle < 315) {
    angle -= 90;
  }
  angle *= (XM_2PI / 360);
  
  //initialize position to screen center ( bounding box center)
  XMFLOAT2 center;
  center.x = (system->table.left + system->table.right)/2.0f;
  center.y = (system->table.top + system->table.bottom)/2.0f;
  center.x -= radius;
  center.y -= radius;
  impulse = XMVectorSet(cos(angle),sin(angle),0,0);
  impulse = XMVectorScale(impulse,37000);
  position = XMVectorSet(center.x,center.y,0,0);
  object_time = 0;
}

void Ball::Update(float dt) {
  //d = v*t
  //v = a*t;
  //d = v*t*t+c
  //dp = v*dt
  //pos = pos + a

  XMVECTOR force_vec = XMVectorAdd(force,impulse);
  XMVECTOR mass_vec = XMVectorReplicate(mass);
  XMVECTOR time_vec = XMVectorReplicate(dt*0.001f);
  
  impulse = XMVectorZero();

  acceleration = XMVectorDivide(force_vec,mass_vec);
  velocity = XMVectorMultiplyAdd(acceleration,time_vec,velocity);
  position = XMVectorMultiplyAdd(velocity,time_vec,position);

 // auto coll = IntersectCircleAxisAlignedBox(this,&system->table);

  //if (coll == 0) {
    //velocity = XMVectorNegate(velocity);
  //}

  object_time += dt;
}

}