// Copyright [2024] tammirav, brainyki

#ifndef SRC_S21_3DVIEWER2_CONTROLLER_V2CONTROLLER_H_
#define SRC_S21_3DVIEWER2_CONTROLLER_V2CONTROLLER_H_

#include <QWidget>

#include "../Model/v2Model.h"
#include "../viewData.h"

namespace s21 {

class v2Controller : public QObject {
  Q_OBJECT

 public:
  explicit v2Controller(s21::v2Model *m) : model(m) {}

 public slots:
  void recieveViewWasChanged(struct view_data data);
  void recieveReadModelFromFile(QString modelFileName);

 signals:
  void sendUpdateView(struct view_data data);  // signal to update view

 private:
  s21::v2Model *model;
};

}  // namespace s21

#endif  // SRC_S21_3DVIEWER2_CONTROLLER_V2CONTROLLER_H_
