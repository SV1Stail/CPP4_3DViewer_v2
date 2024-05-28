// Copyright [2024] tammirav, brainyki

#ifndef SRC_S21_3DVIEWER2_MODEL_V2MODEL_H_
#define SRC_S21_3DVIEWER2_MODEL_V2MODEL_H_

#include <QMatrix4x4>
#include <QWidget>
#include <vector>

#include "../viewData.h"

namespace s21 {

class v2Model {
 public:
  QString modelFileName;  // путь к загруженному файлу obj
  bool new_model;  // признак для обновления буфера GPU, обновляем только один
                   // раз при загрузке новой модели obj
  std::vector<float> vertices;  // массив вершин (x,y,z)
  QMatrix4x4 mat_model;  // матрица модели - локальное координатиное
                         // пространство (0,0,0 - центр объекта)
  QMatrix4x4 mat_view;  // матрица вида - мировое координатное пространство.
                        // Храним тут результаты трансформаций.
  QMatrix4x4
      mat_projection;  // матрица проекции, для учета перспективы проекции
  QVector4D background_color;  // цвет фона
  bool fill_face;  // заливать полигон сплошным цветом (true) или только контур
                   // (false)
  int line_width;  // толщина линий
  QVector4D model_color;  // цвет модели
  bool show_vertex;  // отображать вершины (true) или нет (false)
  int vertex_shape;  // форма вершины (0 - квадрат, 1 - снежинка, 2 - звездочка,
                     // 3 - спираль, 4 - пустой круг, 5 - полный круг)
  QVector4D vertex_color;   // цвет вершины
  float vertex_size;        // размер вершины
  QVector3D move_vector;    // вектор перемещения
  QVector3D rotate_vector;  // вектор вращения
  float scale_factor;  // коэффициент масштабирования
  int projection;      // проекция 0 - ortho, 1 - perspective
  bool isDashed;  // false - обычные линии, true - пунктир

  void updateModel(struct view_data data);
  struct view_data getData();
};

}  // namespace s21

#endif  // SRC_S21_3DVIEWER2_MODEL_V2MODEL_H_
