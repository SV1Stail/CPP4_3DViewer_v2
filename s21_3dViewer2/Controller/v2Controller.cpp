// "Copyright 2024 tammirav"  [legal/copyright]

#include "v2Controller.h"

#include <cmath>
#include <vector>

#include "../../objparser/objparser.h"

namespace s21 {

void s21::v2Controller::recieveViewWasChanged(struct view_data data) {
  model->updateModel(data);
  emit sendUpdateView(model->getData());
}

float getMaxAbs(float a, float b) {
  return fabsf(a) > fabsf(b) ? fabsf(a) : fabsf(b);
}

void s21::v2Controller::recieveReadModelFromFile(QString modelFileName) {
  // обновляем модель  из obj файла, обновляем модель
  model->modelFileName = modelFileName;
  model->new_model = true;
  std::vector<p_face> vec;
  s21::getVertices(modelFileName.toStdString(), vec);
  // пока не будем использовать ни тексутры, ни свет, поэтому подгрузим в модель
  // только координаты вершин. загружаем вершины в модель, заодно нормализуем
  // координаты
  float maxCoord = vec[0].vertex[0].vecPos.x;
  model->vertices.clear();
  for (auto &item : vec) {
    // для отрисовки будем использовать glDrawArrays, поэтому добавляем подряд
    // координаты каждой поверхности заодно определим max значения координат для
    // последующей нормализации
    model->vertices.push_back(item.vertex[0].vecPos.x);
    maxCoord = getMaxAbs(maxCoord, item.vertex[0].vecPos.x);
    model->vertices.push_back(item.vertex[0].vecPos.y);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[0].vecPos.y);
    model->vertices.push_back(item.vertex[0].vecPos.z);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[0].vecPos.z);
    model->vertices.push_back(item.vertex[1].vecPos.x);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[1].vecPos.x);
    model->vertices.push_back(item.vertex[1].vecPos.y);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[1].vecPos.y);
    model->vertices.push_back(item.vertex[1].vecPos.z);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[1].vecPos.z);
    model->vertices.push_back(item.vertex[2].vecPos.x);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[2].vecPos.x);
    model->vertices.push_back(item.vertex[2].vecPos.y);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[2].vecPos.y);
    model->vertices.push_back(item.vertex[2].vecPos.z);
    maxCoord = maxCoord = getMaxAbs(maxCoord, item.vertex[2].vecPos.z);
  }

  // нормализуем координаты
  for (auto &item : model->vertices) item = item / maxCoord;

  emit sendUpdateView(model->getData());
}

}  // namespace s21
