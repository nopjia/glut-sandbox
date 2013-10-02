#include "Ball.h"

Ball::Ball(float x, float y, float z) {
  radius = 1.0f;
  mass = 1.0f;
  moi = 2.0f/5.0f * mass * radius;  // momoent of intertia

  p = vec3(x, y, z);
  q = normalize( quat(1.0f, 1.0f, 1.0f, 0.0f) );
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

bool Ball::intersectBounds() {

  vec3 nor;  // unit vec ball center to wall, for angular

  // linear
  if (p.x >= B_H_WIDTH-radius) {
    p.x = B_H_WIDTH-radius-EPS;
    v.x = -C_BOUNCE*v.x;
    nor = vec3(1.0f, 0.0f, 0.0f);    
  }
  else if (p.x <= -B_H_WIDTH+radius) {
    p.x = -B_H_WIDTH+radius+EPS;
    v.x = -C_BOUNCE*v.x;
    nor = vec3(-1.0f, 0.0f, 0.0f);
  }
  if (p.z >= B_H_HEIGHT-radius) {
    p.z = B_H_HEIGHT-radius-EPS;
    v.z = -C_BOUNCE*v.z;
    nor = vec3(0.0f, 0.0f, 1.0f);
  }
  else if (p.z <= -B_H_HEIGHT+radius) {
    p.z = -B_H_HEIGHT+radius+EPS;
    v.z = -C_BOUNCE*v.z;
    nor = vec3(0.0f, 0.0f, -1.0f);
  }

  // angular
  vec3 vp = cross(w,nor) + v;  // perimeter velocity
  if (!equalsZero(nor) && !equalsZero(vp)) {
    float deltaVn = abs(dot(v, nor))*(1.0f+C_BOUNCE);  // change in normal velocity
    
    // fake, no forces/torques, use C_WALL_MU to control
    v += -C_WALL_MU*mass*deltaVn*normalize(vp);
    v.y = 0.0f;  // lock y
    w += cross(nor, -C_WALL_MU*mass*deltaVn*normalize(vp)); // TODO: must not REVERSE!
    
    // fake angular velocity (along normal) canceling
    vec3 tan = nor.z != 0.0f ? vec3(1.0f, 0.0f, 0.0f) : vec3(0.0f, 0.0f, 1.0f);
    w += -C_WALL_W_CANCEL*dot(w,tan)*tan;

    return true;
  }

  return false;
}

// NOTE: also modifies other ball
bool Ball::intersectBall(Ball& other) {

  if (distance(p, other.p) > radius+other.radius)
    return false;
    
  vec3 nor = normalize(p-other.p);
  vec3 tan = cross(nor, vec3(0.0f, 1.0f, 0.0f));

  // linear
  vec3 newV = dot(v,tan)*tan + dot(other.v,nor)*nor;
  vec3 newOtherV = dot(other.v,tan)*tan + dot(v,nor)*nor;

  // fake angular velocity (along normal) canceling
  w += -C_BALL_W_CANCEL*dot(w,tan)*tan;
  other.w += -C_BALL_W_CANCEL*dot(other.w,tan)*tan;

  // angular
  float deltaVn = abs(dot(newV-v,nor));  // change in v in normal dir
  if (!equalsZero(other.w))
    w += cross(nor, -C_BALL_MU*mass*deltaVn*normalize(cross(other.w,nor)));
  if (!equalsZero(w))
    other.w += cross(nor, -C_BALL_MU*mass*deltaVn*normalize(cross(w,nor)));
    
  // set velocities
  v = newV;
  other.v = newOtherV;

  return true;
}

void Ball::simStep(const float deltaT) {

  // 1. COMPUTE FORCES

  const vec3 downR = vec3(0.0f, -radius, 0.0f);  // center to ground

  // sliding friction  
  vec3 vp = cross(w,downR) + v;  // perimeter velocity
  if (!equalsZero(vp)) {
    force += -C_SLIDE_MU*mass*C_G*normalize(vp);
    torque += cross(downR,-C_SLIDE_MU*mass*C_G*radius*normalize(vp));
  }
  
  // rolling friction
  if (!equalsZero(v))
    force += -C_ROLL_MU*mass*C_G*normalize(v);
  if (!equalsZero(w))
    torque += -C_ROLL_MU*mass*C_G*normalize(w);

  // 2. DERIVATIVES
  
  dp = v;
  dv = force / mass;
  dw = torque / moi;

  // 3. UPDATE STATES

  p += deltaT * dp;
  v += deltaT * dv;
  w += deltaT * dw;

  { // special case quaternions
    float wlen = length(w);
    if (wlen > 0.0f) {
      quat dq1 = 0.5f*quat(0,w*deltaT)*q;
      dq1.w = 1.0f;
      dq1 = normalize(dq1);
      //q = normalize(dq * q);
      dq = angleAxis(degrees(wlen) * deltaT, w/wlen);
      dq = normalize(dq);
      q = normalize(dq * q);
    }
  }

  // 4. CLEAR FORCES

  force = vec3(0.0f);
  torque = vec3(0.0f);

  // stop ball when speeds below threshold
  if (equalsZero(v,C_STOP_TH) &&
      equalsZero(w,C_STOP_TH) &&
      equalsZero(v,C_STOP_TH)) {
      v = w = vec3(0.0f);
  }
}