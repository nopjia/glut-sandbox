#ifndef BALL_H
#define BALL_H

#include "Utils.h"

using namespace glm;

#define C_G           9.8f
#define C_BOUNCE      0.8f
#define C_SLIDE_MU_S  0.5f
#define C_ROLL_MU_S   0.01f

#define B_H_WIDTH     40.0f
#define B_H_HEIGHT    30.0f

class Ball {
public:
   Ball(float x, float y, float z);
   ~Ball();
   mat4 modelMatrix();
   void draw();
   void testUpdate();

   // simulation
   void simStep(float deltaT);
   void intersectBounds();
   void intersectBalls();
   
   float radius;
   float mass;
   float moi;  // momoent of intertia

   // states
   vec3 p;  // position
   mat3 r;
   quat q;  // rotation
   vec3 v;  // translational velocity
   vec3 w;  // angular velocity
   
   // derivatives
   vec3 dp;
   mat3 dr;
   quat dq;
   vec3 dv;
   vec3 dw;
   
   // computed forces
   vec3 force;
   vec3 torque;
};

#endif