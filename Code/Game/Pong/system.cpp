#include <Windows.h>
#include <xnamath.h>
#include <stdio.h>
#include "pong.h"

namespace pong {

BOOL IntersectCircleAxisAlignedBox( const Ball* pVolumeA, const Box* pVolumeB, XMVECTOR& outD )
{
    XMASSERT( pVolumeA );
    XMASSERT( pVolumeB );

    XMVECTOR SphereCenter = pVolumeA->position;//XMLoadFloat2( &pVolumeA->position );
    XMVECTOR SphereRadius = XMVectorReplicatePtr( &pVolumeA->radius );

    XMFLOAT2 center;
    center.x = (pVolumeB->left + pVolumeB->right)/2.0f;
    center.y = (pVolumeB->top + pVolumeB->bottom)/2.0f;

    XMVECTOR BoxCenter = XMLoadFloat2( &center );
    //XMVECTOR BoxExtents = XMLoadFloat3( &pVolumeB->Extents );

    XMVECTOR BoxMin = XMVectorSet(pVolumeB->left,pVolumeB->top,0,0);
    XMVECTOR BoxMax = XMVectorSet(pVolumeB->right,pVolumeB->bottom,0,0);

    // Find the distance to the nearest point on the box.
    // for each i in (x, y, z)
    // if (SphereCenter(i) < BoxMin(i)) d2 += (SphereCenter(i) - BoxMin(i)) ^ 2
    // else if (SphereCenter(i) > BoxMax(i)) d2 += (SphereCenter(i) - BoxMax(i)) ^ 2

    XMVECTOR d = XMVectorZero();

    // Compute d for each dimension.
    XMVECTOR LessThanMin = XMVectorLess( SphereCenter, BoxMin );
    XMVECTOR GreaterThanMax = XMVectorGreater( SphereCenter, BoxMax );

    XMVECTOR MinDelta = SphereCenter - BoxMin;
    XMVECTOR MaxDelta = SphereCenter - BoxMax;

    // Choose value for each dimension based on the comparison.
    d = XMVectorSelect( d, MinDelta, LessThanMin );
    d = XMVectorSelect( d, MaxDelta, GreaterThanMax );

    // Use a dot-product to square them and sum them together.
    XMVECTOR d2 = XMVector2Dot( d, d );
    
    auto result = XMVector4LessOrEqual( d2, XMVectorMultiply( SphereRadius, SphereRadius ) );
    outD = d;
    // Only compute if actually colliding
    if (result)
    {
        XMVECTOR sCenter = pVolumeA->position;//XMLoadFloat2( &ball->position );
        //if (outContactPt)
        //    *outContactPt = XMVectorAdd(sCenter, XMVector3Rotate(d, BoxOrientation));
        //if (outContactNormal)
        //    *outContactNormal = XMVector3Normalize(XMVectorSubtract(*outContactPt, sCenter));
//      if (outPenetration)
//          *outPenetration = XMVectorSubtract(radius2, d2);
    }


    return result;
}

int IntersectCircleBoxInside(const Ball* pVolumeA, const Box* pVolumeB, XMFLOAT2& outD) {

  XMFLOAT2 position;
  XMStoreFloat2(&position,pVolumeA->position);
  outD.y = 0;
  outD.x = 0;
  if (position.x-pVolumeA->radius <= pVolumeB->left) {
    outD.x = -1; 
  }
  if (position.x+pVolumeA->radius >= pVolumeB->right) {
    outD.x = 1;
  }
  if (position.y-pVolumeA->radius <= pVolumeB->top) {
    outD.y = -1;
  }
  if (position.y+pVolumeA->radius >= pVolumeB->bottom) {
    outD.y =1;
  }

  return outD.x != 0 || outD.y != 0;
}

void System::Initialize() {

  FLOAT screen_width = engine_->gfx_context().width();
  FLOAT screen_height = engine_->gfx_context().height();

  
  table.left = 0;
  table.top = 0;
  table.right = screen_width;
  table.bottom = screen_height;


  paddle1.set_engine(engine_);
  paddle1.Initialize(30,180);
  paddle2.position.x = 1;
  paddle2.position.y = 1;

  paddle2.set_engine(engine_);
  paddle2.Initialize(30,480);
  paddle2.position.x = screen_width-31;
  paddle2.position.y = 1;


  ball.set_engine(engine_);
  ball.Initialize(this);
  ball.radius = 16;
  ball.Reset();
}

void System::Deinitialize() {
  paddle1.shape.Deinitialize();
  paddle2.shape.Deinitialize();
}

void System::Update(float dt) {
 
  //input
  if (engine_->input().keyboard_buffer[VK_UP] == true) {
    paddle1.position.y -= dt;
  }
  if (engine_->input().keyboard_buffer[VK_DOWN] == true) {
    paddle1.position.y += dt;
  }

  ball.prev_position = ball.position;
  ball.prev_velocity = ball.velocity;
  ball.Update(dt);
  paddle1.Update(dt);
  paddle2.Update(dt);
  XMVECTOR contact_vec;
  if (IntersectCircleAxisAlignedBox(&ball,&paddle1.box(),contact_vec)==1) {
    int a = 1;
    ball.velocity = XMVectorNegate(ball.velocity);
  }

  if (IntersectCircleAxisAlignedBox(&ball,&paddle2.box(),contact_vec)==1) {

    ball.velocity = XMVectorNegate(ball.velocity);
    FLOAT m1 = ball.mass;
    FLOAT m2 = paddle2.physics.mass;

    XMVECTOR v1 = ((m1-m2)/(m1+m2))*ball.velocity + ((2*m2)/(m1+m2))*paddle2.physics.velocity;
    XMVECTOR v2 = ((m2-m1)/(m1+m2))*paddle2.physics.velocity + ((2*m1)/(m1+m2))*ball.velocity;
  
    //ball.impulse = XMVectorNegate(ball.velocity) * m1;
    int a = 1;
  }
  XMFLOAT2 contact;
  if (IntersectCircleBoxInside(&ball,&table,contact)!=0) {
    int a = 1;
 
    if (contact.y != 0) {
      XMStoreFloat2(&contact,ball.velocity);
      contact.y = -contact.y;
      ball.velocity = XMLoadFloat2(&contact);
    } else if (contact.x != 0) {
      XMStoreFloat2(&contact,ball.velocity);
      contact.x = -contact.x;
      ball.velocity = XMLoadFloat2(&contact);
    } else {
      ball.velocity = XMVectorNegate(ball.velocity);
    }
  }
}

}