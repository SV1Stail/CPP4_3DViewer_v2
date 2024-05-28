// Copyright [2024] tammirav, brainyki

#ifndef SRC_S21_3DVIEWER2_VIEW_V2MAINVIEW_H_
#define SRC_S21_3DVIEWER2_VIEW_V2MAINVIEW_H_

#include <QColor>
#include <QColorDialog>
#include <QFileDialog>
#include <QTimer>
#include <QWidget>

#include "../Controller/v2Controller.h"
#include "../QtGifImage-master/src/gifimage/qgifimage.h"
#include "viewData.h"

namespace s21 {

QT_BEGIN_NAMESPACE
namespace Ui {
class v2MainView;
}
QT_END_NAMESPACE

class v2MainView : public QWidget {
  Q_OBJECT

 public:
  explicit v2MainView(s21::v2Controller *c);
  ~v2MainView();
  QColorDialog *colorModelDialog;
  QColorDialog *colorVertexDialog;
  QColorDialog *colorBackgroundDialog;
  QFileDialog *modelFileDialog;
  QString GIFfileName;
  struct view_data data, demo_data;

 private slots:
  void updateView(struct view_data data);

  void on_pushButtonModelColor_clicked();
  void on_pushButtonVertexColor_clicked();
  void on_pushButtonBackgroundColor_clicked();
  void on_radioButtonFill_clicked();
  void on_radioButtonLinesOnly_clicked();

  void on_spinBoxThickness_valueChanged(int arg1);

  void on_spinBoxVertexSize_valueChanged(int arg1);

  void on_checkBoxVertexShow_stateChanged(int arg1);

  void on_comboBoxVerticesShape_currentIndexChanged(int index);

  void on_spinBoxMoveX_valueChanged(int arg1);

  void on_spinBoxMoveY_valueChanged(int arg1);

  void on_spinBoxMoveZ_valueChanged(int arg1);

  void on_spinBoxRotateX_valueChanged(int arg1);

  void on_spinBoxRotateY_valueChanged(int arg1);

  void on_spinBoxRotateZ_valueChanged(int arg1);

  void on_spinBoxScale_valueChanged(int arg1);

  void on_horizontalSliderScale_valueChanged(int value);

  void on_horizontalSliderRotateZ_valueChanged(int value);

  void on_horizontalSliderRotateY_valueChanged(int value);

  void on_horizontalSliderRotateX_valueChanged(int value);

  void on_horizontalSliderMoveZ_valueChanged(int value);

  void on_horizontalSliderMoveY_valueChanged(int value);

  void on_horizontalSliderMoveX_valueChanged(int value);

  void on_pushButtonLoadModel_clicked();

  void on_pushButtonModelReset_clicked();

  void updateShift(QPoint shift);
  void updateRotate(QPoint shift);
  void updateScale(int y);

  void on_radioButtonParallel_clicked();

  void on_radioButton_Central_clicked();

  void on_checkBoxDashed_stateChanged(int arg1);

  void on_pushButtonSaveAsBMP_clicked();

  void on_pushButtonSaveAsJPEG_clicked();

  void on_pushButtonRecordGIF_clicked();

  void oneGif();

 signals:
  void sendViewWasChanged(
      struct view_data
          data);  // signal to Controller to update Model (and then update View)
  void sendReadModelFromFile(
      QString modelFileName);  // signal to Controller to reload Model from file

 private:
  Ui::v2MainView *ui;
  s21::v2Controller *controller;
  int startTime, tmpTime;
  float timePrint;
  const int gifFps = 10, gifLength = 5;
  QTimer *timer;
  QGifImage *gif;
};

}  // namespace s21

#endif  // SRC_S21_3DVIEWER2_VIEW_V2MAINVIEW_H_
