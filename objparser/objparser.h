// Copyright [2024] tammirav, brainyki

#ifndef SRC_OBJPARSER_OBJPARSER_H_
#define SRC_OBJPARSER_OBJPARSER_H_

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "parsertypes.h"

namespace s21 {

// Splits a given string at the characters = splitter
std::vector<std::string> split(std::string, char16_t splitter);

// Parses v, vt, and vn
p_vec3 parseVertexData(std::string line);

// Parses faces and return a vector of strings, each string represents one face
std::vector<p_vertex> parseFace(std::string line,
                                const std::vector<p_vec3> &vecPos,
                                const std::vector<p_vec3> &vecTex,
                                const std::vector<p_vec3> &vecNormal);

// Main function in the library, handles the entire process of parsing and
// calling the above functions
void getVertices(std::string objPath, std::vector<p_face> &vertices);  // NOLINT

}  // namespace s21

#endif  // SRC_OBJPARSER_OBJPARSER_H_
