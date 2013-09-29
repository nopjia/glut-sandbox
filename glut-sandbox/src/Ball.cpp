#include "Ball.h"

Ball::Ball(float x, float y, float z) {
  radius = 1.0f;
  mass = 1.0f;
  moi = 2.0f/5.0f * mass * radius;  // momoent of intertia

  p = vec3(x, y, z);
  q = quat(1.0f, 0.0f, 0.0f, 0.0f);
  v = vec3(0.0f);
  w = vec3(0.0f);
   
  force = vec3(0.0f);
  torque = vec3(0.0f);
}

Ball::~Ball() {
   
}

mat4 Ball::modelMatrix() {
  return translate(mat4(1.0f), p) * toMat4(q);
}

void Ball::draw() {  
  GLUquadric* sphere;
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_SILHOUETTE);  
  glPushMatrix();
  glMultMatrixf(value_ptr(modelMatrix()));
  gluSphere(sphere, radius, 8, 4);
  glPopMatrix();
}

void Ball::testUpdate() {
   vec3 axis(0.0f, 1.0f, 0.0f);
   float angle = glm::angle(q);
   angle += 0.1f;
   q = angleAxis(angle, axis);
}

void Ball::intersectBounds() {
  if (p.x >= B_H_WIDTH)
    v.x = -C_BOUNCE*v.x;
  else if (p.x <= -B_H_WIDTH)
    v.x = -C_BOUNCE*v.x;
  if (p.z >= B_H_HEIGHT)
    v.z = -C_BOUNCE*v.z;
  else if (p.z <= -B_H_HEIGHT)
    v.z = -C_BOUNCE*v.z;
}

void Ball::simStep(float deltaT) {

  // 1. COMPUTE FORCES
  
  const vec3 downR = vec3(0.0f, -radius, 0.0f);  // center to ground

  // rolling friction
  if (!equalsZero(v))
    force += -C_ROLL_MU_S*mass*C_G*normalize(v);

  // sliding friction  
  vec3 vp = cross(w,downR) + v;  // perimeter velocity
  if (!equalsZero(vp)) {
    force += -C_SLIDE_MU_S*mass*C_G*normalize(vp);
    torque += cross(downR,-C_SLIDE_MU_S*mass*C_G*radius*normalize(vp));
  }

  // 2. DERIVATIVES
  
  dp = v;
  dr = toMat3(angleAxis(3.15f/3.0f, 0.0f,1.0f,0.0f));
  dv = force / mass;
  dw = torque / moi;

  // 3. UPDATE STATES

  p += deltaT * dp;  
  v += deltaT * dv;
  w += deltaT * dw;

  { // special case quaternions
    float wlen = length(w);
    if (wlen > 0.0f) {
      dq = angleAxis(wlen, w/wlen);
    }
    q = normalize(dq * q);
  }

  // 4. CLEAR FORCES

  force = vec3(0.0f);
  torque = vec3(0.0f);

  intersectBounds();
}