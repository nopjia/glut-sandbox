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

#define EPS 0.0001f

using namespace glm;

#define EQUALS_ZERO(X) ((X)<EPS && (X)>-EPS)

inline bool equalsZero(float x) {
  return x < EPS && x > -EPS;
}

inline bool equalsZero(vec3 v) {
  return 
    EQUALS_ZERO(v.x) &&
    EQUALS_ZERO(v.y) &&
    EQUALS_ZERO(v.z);
}

inline bool equalsZero(quat q) {
  return 
    EQUALS_ZERO(q.w) &&
    EQUALS_ZERO(q.x) &&
    EQUALS_ZERO(q.y) &&
    EQUALS_ZERO(q.z);
}

#endif