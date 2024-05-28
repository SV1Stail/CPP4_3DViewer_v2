// Copyright [2024] tammirav, brainyki

#include <QApplication>
#include <QSurfaceFormat>

#include "View/v2MainView.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // на Маке не  получилось подключить OpenGL 4.5, возможно потому что у меня
  // еще интеловский старый с этими настройками заработал профиль 3.3 core
  QSurfaceFormat format;
  format.setMajorVersion(3);
  format.setMinorVersion(3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  s21::v2Model model;
  s21::v2Controller controller(&model);
  s21::v2MainView mainview(&controller);
  mainview.show();
  return a.exec();
}
