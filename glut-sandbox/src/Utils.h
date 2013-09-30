#ifndef UTILS_H
#define UTILS_H

// GL
#include <windows.h>
#include <glut.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// STL
#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include <map>
#include <set>

// GLM libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/gtc/half_float.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/random.hpp>
#include <glm/gtx/quaternion.hpp>

#define EPS 0.001f

using namespace glm;

#define EQUALS_ZERO(X) ((X)<EPS && (X)>-EPS)

inline bool equalsZero(float x) {
  return x < EPS && x > -EPS;
}

inline bool equalsZero(float x, float eps) {
  return x < eps && x > -eps;
}

inline bool equalsZero(vec3 v) {
  return 
    equalsZero(v.x) &&
    equalsZero(v.y) &&
    equalsZero(v.z);
}

inline bool equalsZero(vec3 v, float eps) {
  return 
    equalsZero(v.x, eps) &&
    equalsZero(v.y, eps) &&
    equalsZero(v.z, eps);
}

inline bool equalsZero(quat q) {
  return 
    equalsZero(q.w) &&
    equalsZero(q.x) &&
    equalsZero(q.y) &&
    equalsZero(q.z);
}

#endif