// Copyright [2024] tammirav, brainyki

#include "v2MainView.h"

#include <QDebug>
#include <fstream>
#include <iostream>
#include <string>

#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "ui_v2MainView.h"

namespace s21 {

s21::v2MainView::v2MainView(s21::v2Controller *c)
    : controller(c), ui(new Ui::v2MainView) {
  ui->setupUi(this);
  colorModelDialog = new QColorDialog();
  colorVertexDialog = new QColorDialog();
  colorBackgroundDialog = new QColorDialog();
  // начальные значения, если файл найден подгрузим из него
#ifdef __APPLE__
std::ifstream file_settings("/Users/Shared/settings.save", std::ios::binary);
#else 
std::ifstream file_settings("settings.save", std::ios::binary);
#endif
  if (file_settings) {
    cereal::BinaryInputArchive archive(file_settings);
    archive(data);
    int red, green, blue;
    archive(red, green, blue);
    data.model_color.setRed(red);
    data.model_color.setGreen(green);
    data.model_color.setBlue(blue);
    archive(red, green, blue);
    data.vertex_color.setRed(red);
    data.vertex_color.setGreen(green);
    data.vertex_color.setBlue(blue);
    archive(red, green, blue);
    data.background_color.setRed(red);
    data.background_color.setGreen(green);
    data.background_color.setBlue(blue);
    float x, y, z;
    archive(x, y, z);
    data.move_vector.setX(x);
    data.move_vector.setY(y);
    data.move_vector.setZ(z);
    archive(x, y, z);
    data.rotate_vector.setX(x);
    data.rotate_vector.setY(y);
    data.rotate_vector.setZ(z);
    std::string str;
    archive(str);
    data.modelFileName = QString::fromStdString(str);
  } else {
    // если файл с настройками не найден, хардкодим по умолчанию
    data.new_model = false;
    data.modelFileName = "Default hardcoded model of cube";
    data.model_color = {0, 255, 0};
    data.vertex_color = {255, 0, 0};
    data.background_color = {51, 76, 76};
    data.isFilled = false;
    data.thickness = 10;
    data.vertex_size = 20;
    data.vertex_show = true;
    data.vertex_shape = 1;
    // координаты вершин куба (модель по умолчанию  для инициализации OpenGL),
    // рисуем треугольниками
    data.vertices = {
        -0.5f, 0.5f,  -0.5f,  // 0
        0.5f,  0.5f,  -0.5f,  // 1
        0.5f,  -0.5f, -0.5f,  // 2

        -0.5f, 0.5f,  -0.5f,  // 0
        0.5f,  -0.5f, -0.5f,  // 2
        -0.5f, -0.5f, -0.5f,  // 3

        0.5f,  -0.5f, -0.5f,  // 2
        0.5f,  0.5f,  -0.5f,  // 1
        0.5f,  0.5f,  0.5f,   // 5

        0.5f,  -0.5f, -0.5f,  // 2
        0.5f,  0.5f,  0.5f,   // 5
        0.5f,  -0.5f, 0.5f,   // 6

        -0.5f, -0.5f, -0.5f,  // 3
        0.5f,  -0.5f, -0.5f,  // 2
        0.5f,  -0.5f, 0.5f,   // 6

        -0.5f, -0.5f, -0.5f,  // 3
        0.5f,  -0.5f, 0.5f,   // 6
        -0.5f, -0.5f, 0.5f,   // 7

        -0.5f, 0.5f,  -0.5f,  // 0
        -0.5f, -0.5f, -0.5f,  // 3
        -0.5f, -0.5f, 0.5f,   // 7

        -0.5f, 0.5f,  -0.5f,  // 0
        -0.5f, -0.5f, 0.5f,   // 7
        -0.5f, 0.5f,  0.5f,   // 4

        0.5f,  0.5f,  -0.5f,  // 1
        -0.5f, 0.5f,  -0.5f,  // 0
        -0.5f, 0.5f,  0.5f,   // 4

        0.5f,  0.5f,  -0.5f,  // 1
        -0.5f, 0.5f,  0.5f,   // 4
        0.5f,  0.5f,  0.5f,   // 5

        0.5f,  -0.5f, 0.5f,  // 6
        0.5f,  0.5f,  0.5f,  // 5
        -0.5f, 0.5f,  0.5f,  // 4

        0.5f,  -0.5f, 0.5f,  // 6
        -0.5f, 0.5f,  0.5f,  // 4
        -0.5f, -0.5f, 0.5f   // 7
    };
    // индексы вершин
    data.idx = {0, 1, 2, 0, 2, 3, 2, 1, 5, 2, 5, 6, 3, 2, 6, 3, 6, 7,
                0, 3, 7, 0, 7, 4, 1, 0, 4, 1, 4, 5, 6, 5, 4, 6, 4, 7};

    data.mat_projection.setToIdentity();
    data.mat_model.setToIdentity();
    data.mat_view.setToIdentity();

    data.projection = 1;
    data.scale_factor = 1.0f;
    data.isDashed = false;
  }

  connect(ui->openGLWidget, SIGNAL(rightButtonMove(QPoint)), this,
          SLOT(updateShift(QPoint)));
  connect(ui->openGLWidget, SIGNAL(leftButtonMove(QPoint)), this,
          SLOT(updateRotate(QPoint)));
  connect(ui->openGLWidget, SIGNAL(mouseScroll(int)), this,
          SLOT(updateScale(int)));

  /*
    Create connections to handler signals from Controller
  */

  connect(controller, &s21::v2Controller::sendUpdateView, this,
          &v2MainView::updateView);  // update view on signal from controller

  /*
    Signals to Controller
  */
  connect(this, &v2MainView::sendViewWasChanged, controller,
          &s21::v2Controller::recieveViewWasChanged);
  connect(this, &v2MainView::sendReadModelFromFile, controller,
          &s21::v2Controller::recieveReadModelFromFile);

  emit sendViewWasChanged(
      data);  // инициализируем начальными значениями все контролы
}

s21::v2MainView::~v2MainView() {
  delete ui;
  // сохраняем данные в файл
  // Serialize
#ifdef __APPLE__
std::ofstream os("/Users/Shared/settings.save", std::ios::binary);
#else 
std::ofstream os("settings.save", std::ios::binary);
#endif
  cereal::BinaryOutputArchive archive(os);
  archive(data);
  // нестандартные типы сохраним по частям (стандартным)
  archive(data.model_color.red(), data.model_color.green(),
          data.model_color.blue());
  archive(data.vertex_color.red(), data.vertex_color.green(),
          data.vertex_color.blue());
  archive(data.background_color.red(), data.background_color.green(),
          data.background_color.blue());
  archive(data.move_vector.x(), data.move_vector.y(), data.move_vector.z());
  archive(data.rotate_vector.x(), data.rotate_vector.y(),
          data.rotate_vector.z());
  archive(data.modelFileName.toStdString());
}

void s21::v2MainView::updateView(struct view_data data) {
  s21::v2MainView::data = data;
  colorModelDialog->setCurrentColor(data.model_color);
  colorVertexDialog->setCurrentColor(data.vertex_color);
  ui->pushButtonModelColor->setStyleSheet("background-color: " +
                                          data.model_color.name());
  ui->pushButtonVertexColor->setStyleSheet("background-color: " +
                                           data.vertex_color.name());
  ui->spinBoxThickness->setDisabled(data.isFilled);
  ui->labelThickness->setDisabled(data.isFilled);
  ui->pushButtonBackgroundColor->setStyleSheet("background-color: " +
                                               data.background_color.name());
  ui->spinBoxThickness->setValue(data.thickness);
  ui->checkBoxVertexShow->setChecked(data.vertex_show);
  ui->comboBoxVerticesShape->setEnabled(data.vertex_show);
  ui->spinBoxVertexSize->setValue(data.vertex_size);
  ui->spinBoxVertexSize->setEnabled(data.vertex_show);
  ui->comboBoxVerticesShape->setCurrentIndex(data.vertex_shape);
  // move
  ui->spinBoxMoveX->setValue(static_cast<int>(data.move_vector.x() * 100.0f));
  ui->spinBoxMoveY->setValue(static_cast<int>(data.move_vector.y() * 100.0f));
  ui->spinBoxMoveZ->setValue(static_cast<int>(data.move_vector.z() * 10.0f));
  ui->horizontalSliderMoveX->setValue(
      static_cast<int>(data.move_vector.x() * 100.0f));
  ui->horizontalSliderMoveY->setValue(
      static_cast<int>(data.move_vector.y() * 100.0f));
  ui->horizontalSliderMoveZ->setValue(
      static_cast<int>(data.move_vector.z() * 10.0f));
  // rotate
  ui->spinBoxRotateX->setValue(data.rotate_vector.x());
  ui->spinBoxRotateY->setValue(data.rotate_vector.y());
  ui->spinBoxRotateZ->setValue(data.rotate_vector.z());
  ui->horizontalSliderRotateX->setValue(data.rotate_vector.x());
  ui->horizontalSliderRotateY->setValue(data.rotate_vector.y());
  ui->horizontalSliderRotateZ->setValue(data.rotate_vector.z());
  //  scale
  ui->spinBoxScale->setValue(static_cast<int>(data.scale_factor * 10.0f));
  ui->horizontalSliderScale->setValue(
      static_cast<int>(data.scale_factor * 10.0f));
  // modelFilename
  ui->lineEditFileInfo->setText(data.modelFileName);
  // количество вершин и треугольников
  ui->labelNumberVerteces->setText("Number of verteces: " +
                                   QString::number(data.vertices.size()));
  ui->labelNumberTriangles->setText("Number of triangles: " +
                                    QString::number(data.vertices.size() / 3));
  // пунктир
  ui->checkBoxDashed->setChecked(data.isDashed);
  // render seen
  // ui->openGLWidget->paintGL();
  this->ui->openGLWidget->update();
}

void s21::v2MainView::on_pushButtonModelColor_clicked() {
  colorModelDialog->exec();
  data.model_color = colorModelDialog->selectedColor();
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_pushButtonVertexColor_clicked() {
  colorVertexDialog->exec();
  data.vertex_color = colorVertexDialog->selectedColor();
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_pushButtonBackgroundColor_clicked() {
  colorBackgroundDialog->exec();
  data.background_color = colorBackgroundDialog->selectedColor();
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_radioButtonFill_clicked() {
  data.isFilled = true;
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_radioButtonLinesOnly_clicked() {
  data.isFilled = false;
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxThickness_valueChanged(int arg1) {
  data.thickness = arg1;
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxVertexSize_valueChanged(int arg1) {
  data.vertex_size = arg1;
  emit sendViewWasChanged(data);
}

void v2MainView::on_checkBoxVertexShow_stateChanged(int arg1) {
  data.vertex_show = arg1;
  emit sendViewWasChanged(data);
}

void v2MainView::on_comboBoxVerticesShape_currentIndexChanged(int index) {
  data.vertex_shape = index;
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxMoveX_valueChanged(int arg1) {
  data.move_vector.setX(static_cast<float>(arg1) / 100.0f);
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxMoveY_valueChanged(int arg1) {
  data.move_vector.setY(static_cast<float>(arg1) / 100.0f);
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxMoveZ_valueChanged(int arg1) {
  data.move_vector.setZ(static_cast<float>(arg1) / 10.0f);
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxRotateX_valueChanged(int arg1) {
  data.rotate_vector.setX(arg1);
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxRotateY_valueChanged(int arg1) {
  data.rotate_vector.setY(arg1);
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxRotateZ_valueChanged(int arg1) {
  data.rotate_vector.setZ(arg1);
  emit sendViewWasChanged(data);
}

void v2MainView::on_spinBoxScale_valueChanged(int arg1) {
  data.scale_factor = static_cast<float>(arg1) / 10.0f;

  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderScale_valueChanged(int value) {
  data.scale_factor = static_cast<float>(value) / 10.0f;

  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderRotateZ_valueChanged(int value) {
  data.rotate_vector.setZ(value);
  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderRotateY_valueChanged(int value) {
  data.rotate_vector.setY(value);
  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderRotateX_valueChanged(int value) {
  data.rotate_vector.setX(value);
  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderMoveZ_valueChanged(int value) {
  data.move_vector.setZ(static_cast<float>(value) / 10.0f);
  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderMoveY_valueChanged(int value) {
  data.move_vector.setY(static_cast<float>(value) / 100.0f);
  emit sendViewWasChanged(data);
}

void v2MainView::on_horizontalSliderMoveX_valueChanged(int value) {
  data.move_vector.setX(static_cast<float>(value) / 100.0f);
  emit sendViewWasChanged(data);
}

void v2MainView::on_pushButtonLoadModel_clicked() {
  QString modelFileName = QFileDialog::getOpenFileName(
      this, "Выбрать файл", nullptr, "Object(*.obj)");
  if (!modelFileName.isNull()) emit sendReadModelFromFile(modelFileName);
}

void v2MainView::on_pushButtonModelReset_clicked() {
  ui->spinBoxMoveX->setValue(0);
  ui->spinBoxMoveY->setValue(0);
  ui->spinBoxMoveZ->setValue(0);
  ui->horizontalSliderMoveX->setValue(0);
  ui->horizontalSliderMoveY->setValue(0);
  ui->horizontalSliderMoveZ->setValue(0);
  // rotate
  ui->spinBoxRotateX->setValue(0);
  ui->spinBoxRotateY->setValue(0);
  ui->spinBoxRotateZ->setValue(0);
  ui->horizontalSliderRotateX->setValue(0);
  ui->horizontalSliderRotateY->setValue(0);
  ui->horizontalSliderRotateZ->setValue(0);
  //  scale
  ui->spinBoxScale->setValue(10);
  ui->horizontalSliderScale->setValue(10);
}

void s21::v2MainView::updateShift(QPoint shift) {
  ui->spinBoxMoveX->setValue(this->ui->spinBoxMoveX->value() + shift.x());
  ui->spinBoxMoveY->setValue(this->ui->spinBoxMoveY->value() - shift.y());
}

void s21::v2MainView::updateRotate(QPoint shift) {
  ui->spinBoxRotateY->setValue(this->ui->spinBoxRotateY->value() + shift.x());
  ui->spinBoxRotateX->setValue(this->ui->spinBoxRotateX->value() + shift.y());
}

void s21::v2MainView::updateScale(int y) {
  ui->spinBoxScale->setValue(this->ui->spinBoxScale->value() - static_cast<int>(y/100.0f));
}

void s21::v2MainView::on_radioButtonParallel_clicked() {
  if (ui->radioButtonParallel->isChecked()) data.projection = 0;
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_radioButton_Central_clicked() {
  if (ui->radioButton_Central->isChecked()) data.projection = 1;
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_checkBoxDashed_stateChanged(int arg1) {
  if (ui->checkBoxDashed->isChecked())
    data.isDashed = true;
  else
    data.isDashed = false;
  emit sendViewWasChanged(data);
}

void s21::v2MainView::on_pushButtonSaveAsBMP_clicked() {
  QString fileName =
      QFileDialog::getSaveFileName(this, "Save file", nullptr, "Image(*.bmp)") +
      ".bmp";
  if (fileName.isNull()) return;
  ui->openGLWidget->saveBmpImage(fileName);
}

void s21::v2MainView::on_pushButtonSaveAsJPEG_clicked() {
  QString fileName = QFileDialog::getSaveFileName(this, "Save file", nullptr,
                                                  "Image(*.jpeg)") +
                     ".jpeg";
  if (fileName.isNull()) return;
  ui->openGLWidget->saveJpegImage(fileName);
}

void s21::v2MainView::on_pushButtonRecordGIF_clicked() {
  GIFfileName =
      QFileDialog::getSaveFileName(this, "Save file", nullptr, "Image(*.gif)") +
      ".gif";
  if (GIFfileName.isNull()) return;

  gif = new QGifImage;
  gif->setDefaultDelay(1000 / gifFps);
  startTime = 0, tmpTime = 1000 / gifFps;
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(oneGif()));
  timer->start(1000 / gifFps);

  ui->pushButtonRecordGIF->setEnabled(false);
  ui->labelRecordGIF->setText("Recording GIF...");
  demo_data = data;
}

void s21::v2MainView::oneGif() {
  if (startTime == tmpTime) {
    QPixmap screenGIF(this->ui->openGLWidget->size());
    ui->openGLWidget->render(&screenGIF);
    QImage image = screenGIF.toImage();
    gif->addFrame(image, 1000 / gifFps);
    timePrint = static_cast<float>(startTime) / 1e3;  // GIF time in seconds
    tmpTime += 1000 / gifFps;
    data.rotate_vector.setX(data.rotate_vector.x() + 2);
    data.rotate_vector.setY(data.rotate_vector.y() + 5);
    data.rotate_vector.setZ(data.rotate_vector.z() + 7);
    data.move_vector.setZ(data.move_vector.z() - 0.3);
    emit sendViewWasChanged(data);
  }
  if (startTime == 1000 * gifLength) {
    gif->save(GIFfileName);
    timer->stop();

    disconnect(timer, SIGNAL(timeout()), this, SLOT(oneGif()));

    delete gif;
    delete timer;

    ui->pushButtonRecordGIF->setEnabled(true);
    ui->labelRecordGIF->setText("");
    data = demo_data;
    emit sendViewWasChanged(data);
  }
  startTime += 1000 / gifFps;
}

}  // namespace s21
