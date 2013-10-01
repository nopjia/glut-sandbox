#ifndef BALL_H
#define BALL_H

#include "Utils.h"

using namespace glm;

#define C_G             9.8f
#define C_BOUNCE        0.99f
#define C_SLIDE_MU      5.0f
#define C_ROLL_V_MU     0.7f
#define C_ROLL_W_MU     0.7f
#define C_WALL_MU       0.2f  // fake angular v collision control
#define C_BALL_MU       0.2f
#define C_WALL_W_CANCEL 0.9f  // canceling out normal angular v
#define C_BALL_W_CANCEL 0.6f
#define C_STOP_TH       0.75f // stopping ball threshold

#define B_H_WIDTH     30.0f
#define B_H_HEIGHT    20.0f

class Ball {
public:
   Ball(float x, float y, float z);
   ~Ball();
   mat4 modelMatrix();
   void draw();
   void testUpdate();

   // simulation
   void simStep(const float deltaT);
   bool intersectBounds();
   bool intersectBall(Ball& other);
   
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