
// Copyright [2024] tammirav, brainyki

#include "v2Model.h"

namespace s21 {

void s21::v2Model::updateModel(struct view_data data) {
  model_color.setX(data.model_color.redF());
  model_color.setY(data.model_color.greenF());
  model_color.setZ(data.model_color.blueF());
  model_color.setW(data.model_color.alphaF());

  vertex_color.setX(data.vertex_color.redF());
  vertex_color.setY(data.vertex_color.greenF());
  vertex_color.setZ(data.vertex_color.blueF());
  vertex_color.setW(data.vertex_color.alphaF());

  fill_face = data.isFilled;

  background_color.setX(data.background_color.redF());
  background_color.setY(data.background_color.greenF());
  background_color.setZ(data.background_color.blueF());
  background_color.setW(data.background_color.alphaF());

  line_width = data.thickness;

  show_vertex = data.vertex_show;

  vertex_size = data.vertex_size;

  vertex_shape = data.vertex_shape;

  move_vector = data.move_vector;
  rotate_vector = data.rotate_vector;
  scale_factor = data.scale_factor;

  mat_model = data.mat_model;
  mat_view = data.mat_view;
  mat_projection = data.mat_projection;

  vertices = data.vertices;
  modelFileName = data.modelFileName;
  new_model = data.new_model;
  projection = data.projection;
  isDashed = data.isDashed;
}

struct view_data s21::v2Model::getData() {
  struct view_data temp;
  temp.model_color.setRedF(model_color.x());
  temp.model_color.setGreenF(model_color.y());
  temp.model_color.setBlueF(model_color.z());
  temp.model_color.setAlphaF(model_color.w());

  temp.vertex_color.setRedF(vertex_color.x());
  temp.vertex_color.setGreenF(vertex_color.y());
  temp.vertex_color.setBlueF(vertex_color.z());
  temp.vertex_color.setAlphaF(vertex_color.w());

  temp.isFilled = fill_face;

  temp.background_color.setRedF(background_color.x());
  temp.background_color.setGreenF(background_color.y());
  temp.background_color.setBlueF(background_color.z());
  temp.background_color.setAlphaF(background_color.w());

  temp.thickness = line_width;

  temp.vertex_show = show_vertex;

  temp.vertex_size = vertex_size;

  temp.vertex_shape = vertex_shape;

  temp.move_vector = move_vector;
  temp.rotate_vector = rotate_vector;
  temp.scale_factor = scale_factor;

  temp.modelFileName = modelFileName;

  temp.vertices = vertices;

  temp.mat_model = mat_model;
  temp.mat_view = mat_view;
  temp.mat_projection = mat_projection;

  temp.new_model = new_model;
  temp.projection = projection;
  temp.isDashed = isDashed;

  return temp;
}

}  // namespace s21
