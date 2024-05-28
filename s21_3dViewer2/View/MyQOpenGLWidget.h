// Copyright [2024] tammirav, brainyki

#ifndef SRC_S21_3DVIEWER2_VIEW_MYQOPENGLWIDGET_H_
#define SRC_S21_3DVIEWER2_VIEW_MYQOPENGLWIDGET_H_

#include <QMouseEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QWheelEvent>

#include "v2MainView.h"

namespace s21 {

class MyQOpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_3_3_Core {
  Q_OBJECT

 public:
  explicit MyQOpenGLWidget(QWidget* parent = nullptr);
  ~MyQOpenGLWidget();
  void initializeGL() override;
  void paintGL() override;
  void resizeGL(int w, int h) override;

  void mousePressEvent(QMouseEvent* event) override;
  void mouseReleaseEvent(QMouseEvent* event) override;
  void mouseMoveEvent(QMouseEvent* event) override;
  void wheelEvent(QWheelEvent* event) override;

  void saveBmpImage(const QString& filename);
  void saveJpegImage(const QString& filename);

 protected:
  bool leftButtonPress = false;
  bool rightButtonPress = false;

  QPoint leftButtonPoint;
  QPoint rightButtonPoint;

 private:
  QOpenGLShaderProgram shader_program;
  v2MainView* mainView;

 signals:
  void leftButtonMove(QPoint shift);
  void rightButtonMove(QPoint shift);
  void mouseScroll(int y);
};

}  // namespace s21

#endif  //  SRC_S21_3DVIEWER2_VIEW_MYQOPENGLWIDGET_H_
