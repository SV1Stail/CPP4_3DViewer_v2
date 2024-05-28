// Copyright [2024] tammirav, brainyki

#ifndef SRC_OBJPARSER_PARSERTYPES_H_
#define SRC_OBJPARSER_PARSERTYPES_H_

namespace s21 {

struct p_vec3 {
  float x;
  float y;
  float z;
  p_vec3(float xx = 0.0f, float yy = 0.0f, float zz = 0.0f) {  // NOLINT
    x = xx;
    y = yy;
    z = zz;
  }
};

// For vertex data
struct p_vertex {
  p_vec3 vecPos;
  p_vec3 vecTex;
  p_vec3 vecNormal;
};

// For face data (vertex[0], vertex[1], vertex[2])
struct p_face {
  p_vertex vertex[3];
};

}  // namespace s21

#endif  // SRC_OBJPARSER_PARSERTYPES_H_
