
#define SRC_S21_3DVIEWER2_VIEWDATA_H_

#include <QColor>
#include "cereal/types/vector.hpp"

namespace s21 {

struct view_data {  
   bool new_model = false; // признак для обновления буфера GPU
   QColor model_color;
   QColor vertex_color;
   bool isFilled;
   QColor background_color;
   int thickness;
   bool vertex_show;
   int vertex_size;
   int vertex_shape;
   QVector3D move_vector; 
   QVector3D rotate_vector; 
   float scale_factor  = 1.0f; 
   QString modelFileName;
   std::vector<float> vertices;
   std::vector<int> idx;
   QMatrix4x4 mat_model;
   QMatrix4x4 mat_view;
   QMatrix4x4 mat_projection;
   int projection;
   bool isDashed;

   template <class Archive>
   void save(Archive& Output) const {
    Output(
            new_model, isFilled, thickness, vertex_show, vertex_size, vertex_shape, 
            scale_factor, vertices, idx, projection, isDashed
          );
   }

   template <class Archive>
   void load(Archive& Input) {
    Input(new_model, isFilled, thickness, vertex_show, vertex_size, vertex_shape, 
            scale_factor, vertices, idx, projection, isDashed);
   }
};

}  // namespace s21

#endif  // SRC_S21_3DVIEWER2_VIEWDATA_H_
