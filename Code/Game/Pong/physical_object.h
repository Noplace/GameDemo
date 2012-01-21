#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H


struct Circle {
  FLOAT radius;
};

struct Box {
  FLOAT left,top,right,bottom;
};

struct PhysicalObject {
  XMVECTOR acceleration;
  XMVECTOR velocity;
  XMVECTOR prev_velocity;
  //XMVECTOR direction;
  XMVECTOR position;
  XMVECTOR prev_position;
  XMVECTOR impulse;
  XMVECTOR force;
  FLOAT mass;

  PhysicalObject() {
    impulse = XMVectorZero();
    position = XMVectorZero();
    velocity = XMVectorZero();
    acceleration = XMVectorZero();
    force = XMVectorZero();
    mass = 0;
    prev_position = XMVectorZero();
    prev_velocity = XMVectorZero();
  }

  XMVECTOR momentum() {
    return XMVectorMultiply(velocity,XMVectorReplicate(mass));
  }

  void PrintInfo() {
    /*XMFLOAT2 a,b,c,d;
    XMStoreFloat2(&a,acceleration);
    XMStoreFloat2(&b,velocity);
    XMStoreFloat2(&c,position);
    XMStoreFloat2(&d,impulse);
    printf("acceleration\t: (%0.4f,%0.4f)\nvelocity\t: (%0.4f,%0.4f)\nposition\t: (%0.4f,%0.4f)\nimpulse\t\t: (%0.4f,%0.4f)\nmass\t\t: %0.4f\n",
    a.x,a.y,b.x,b.y,c.x,c.y,d.x,d.y,mass);*/
  }
};

#endif