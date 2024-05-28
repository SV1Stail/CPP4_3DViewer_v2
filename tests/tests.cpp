// "Copyright 2024 tammirav"  [legal/copyright]

#include "tests.h"
#include <QDebug>

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Parser, ParseLine) {
  s21::p_vec3 result;
  int status = 0;

  // test 1
  std::string line = "v 11.305434 38.234230 0.000006";
  result = s21::parseVertexData(line);

  EXPECT_NEAR(result.x, 11.305434, 1e-6);
  EXPECT_NEAR(result.y, 38.234230, 1e-6);
  EXPECT_NEAR(result.z, 0.000006, 1e-6);

  // test 2
  line = "v -11.305434 -38.234230 -0.000006";
  result = s21::parseVertexData(line);
  EXPECT_NEAR(result.x, -11.305434, 1e-6);
  EXPECT_NEAR(result.y, -38.234230, 1e-6);
  EXPECT_NEAR(result.z, -0.000006, 1e-6);

}

TEST(Parser, LoadModel) {
  std::vector<s21::p_face> result;
  std::string filename = "../tests/hand.obj";
  s21::getVertices(filename, result);

  // test 1
  EXPECT_EQ(result.size(), 8808);

  // test 2 face coordinates
  EXPECT_NEAR(result[0].vertex[0].vecPos.x, 0.61436, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecPos.y, 0.281365, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecPos.z, -0.675872, 1e-6);

  EXPECT_NEAR(result[0].vertex[1].vecPos.x, 0.595276, 1e-6);
  EXPECT_NEAR(result[0].vertex[1].vecPos.y, 0.250106, 1e-6);
  EXPECT_NEAR(result[0].vertex[1].vecPos.z, -0.624614, 1e-6);

  EXPECT_NEAR(result[0].vertex[2].vecPos.x, 0.557886, 1e-6);
  EXPECT_NEAR(result[0].vertex[2].vecPos.y, 0.213041, 1e-6);
  EXPECT_NEAR(result[0].vertex[2].vecPos.z, -0.661074, 1e-6);

  // test 3 face normals
  EXPECT_NEAR(result[0].vertex[0].vecNormal.x, 0.7525, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecNormal.y, -0.6478, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecNormal.z, -0.1191, 1e-6);

  // test 4 face testures
  EXPECT_NEAR(result[0].vertex[0].vecTex.x, 0.839448, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecTex.y, 0.493413, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecTex.z, 0, 1e-6);

  EXPECT_NEAR(result[0].vertex[1].vecTex.x, 0.832612, 1e-6);
  EXPECT_NEAR(result[0].vertex[1].vecTex.y, 0.493413, 1e-6);
  EXPECT_NEAR(result[0].vertex[1].vecTex.z, 0, 1e-6);

  EXPECT_NEAR(result[0].vertex[2].vecTex.x, 0.832612, 1e-6);
  EXPECT_NEAR(result[0].vertex[2].vecTex.y, 0.49976, 1e-6);
  EXPECT_NEAR(result[0].vertex[2].vecTex.z, 0, 1e-6);

}

TEST(Parser, ExtremeCases) {
  std::vector<s21::p_face> result;
  std::string filename = "../tests/t2.obj";
  s21::getVertices(filename, result);

  // test 1
  EXPECT_EQ(result.size(), 8807);

  // test 2 face coordinates
  EXPECT_NEAR(result[0].vertex[0].vecPos.x, 0.61436, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecPos.y, 0.281365, 1e-6);
  EXPECT_NEAR(result[0].vertex[0].vecPos.z, 0, 1e-6);

}
