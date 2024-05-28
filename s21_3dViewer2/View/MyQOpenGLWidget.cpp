// Copyright [2024] tammirav, brainyki

#include "MyQOpenGLWidget.h"

#include <QDebug>

//  https://www.khronos.org/opengl/wiki/Vertex_Specification#Vertex_Buffer_Object
unsigned int VBO, VAO;

// Шейдерная программа (сгенерируется после компиляции и линковки шейдеров)
unsigned int shaderProgram;

s21::MyQOpenGLWidget::MyQOpenGLWidget(QWidget* parent) : QOpenGLWidget{parent} {
  mainView = qobject_cast<v2MainView*>(parent);
}

s21::MyQOpenGLWidget::~MyQOpenGLWidget() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}

void s21::MyQOpenGLWidget::initializeGL() {
  // получаем указатели на openGL функции
  qDebug() << initializeOpenGLFunctions();

  /*
  Современные графические  приложения практически всегда shader-based.
  Т.е. содержат отдельную программу, написанную на special-purpose shader
  language, которая запускается непосредственно на GPU (видеокарте).

  On the hardware side, OpenGL provides a multi-stage graphics pipeline that is
  partially programmable using a language called GLSL (OpenGL Shading Language).

  При этом процесс преобразования 3D-сцены в 2D-картинку происходит в несколько
  этапов (OpenGL graphics pipeline). Некоторые этапы этого процесса (vertex,
  tessellation, geometry, and fragment) можно запрограммировать на GLSL.
  Происходит это так:
    - основное приложение берет "GLSL shader code"
    - создаются "OpenGL shader objects" в которые загружается этот код
    - при помощи OpenGL команд этот код компилируется, линкуется и
  устанавливается на GPU.

  В минимальном варианте, требуется GLSL code для двух этапов - the vertex and
  fragment stages. Все загруженные вершины (vertices) прогоняются поочередно
  через шейдер. (They do so one by one; that is, the shader is executed once per
  vertex)
  */

  // подгрузим код шейдеров из файлов
  this->shader_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                               ":/View/Shaders/source.vert");
  this->shader_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                               ":/View/Shaders/source.frag");

  // проверим на ошибки
  bool success = this->shader_program.link();
  if (!success) {
    qDebug() << "ERROR: " << this->shader_program.log();
  } else {
    qDebug() << "Shaiders successfully loaded from qrc";
  }

  /* ===================== VAO and VBO =====================
     when sets of data are prepared for sending down the pipeline, they are
     organized into buffers. Those buffers are in turn organized into Vertex
     Array Objects (VAOs).
  */

  glGenVertexArrays(1, &VAO);  // создаем один айди для VAO
  glGenBuffers(1, &VBO);       // создаем один айди для VBO

  // биндим VAO и VBO
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);  // GL_ARRAY_BUFFER - это тип буфера
  /*
    Создаем новое хранилище для объекта buffer,
    (область хранения на графическом процессоре, в которое отправляем данные
    вершин) Если data not NULL, хранилище инициализируется данными из этого
    указателя https://registry.khronos.org/OpenGL-Refpages/es3.0/

  */
  glBufferData(GL_ARRAY_BUFFER, mainView->data.vertices.size() * sizeof(float),
               mainView->data.vertices.data(),
               GL_STATIC_DRAW);  // потом заменим на динамик

  this->shader_program.bind();
  GLint aPosLocation = this->shader_program.attributeLocation(
      "aPos");  // 0 в VAO (оно же layout (location = 0))

  /*
    сообщаем GPU как анализировать значения атрибутов в буфере
  */
  glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        reinterpret_cast<void*>(0));
  glEnableVertexAttribArray(aPosLocation);  // Enable VAO

  // отвязываем VAO, VBO и EBO
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  /*
     Оптимизация - создает два буфера.
     Позволяет не отображать еще полностью не сформированную картинку.
     Для отрисовки используется отдельныый буфер, после завершения отрисовки
     буферы меняются местами.
  */
  glEnable(GL_DOUBLEBUFFER);

  glEnable(
      GL_DEPTH_TEST);  // включаем буфер глубины для отображения Z-координаты
}

void s21::MyQOpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void s21::MyQOpenGLWidget::paintGL() {
  // Цвет фона (цвет очистки буфера изображения)
  glClearColor(mainView->data.background_color.redF(),
               mainView->data.background_color.greenF(),
               mainView->data.background_color.blueF(),
               mainView->data.background_color.alphaF());
  glEnable(GL_DEPTH_TEST);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // режим рисования примитовов - заливка треугольников или только линии
  if (mainView->data.isFilled)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // биндим VAO
  glBindVertexArray(VAO);
  this->shader_program.bind();
  // проверяем не изменилась ли модель, если изменилась - загрузим новые
  // координаты и индексы в GPU
  if (mainView->data.new_model) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 mainView->data.vertices.size() * sizeof(float),
                 mainView->data.vertices.data(), GL_STATIC_DRAW);
    mainView->data.new_model = false;
  }
  mainView->data.mat_projection.setToIdentity();
  // проверяем проекцию (0 - ortho, 1 - perspective)
  if (mainView->data.projection) {
    mainView->data.mat_projection.perspective(
        45, static_cast<float>(width()) / static_cast<float>(height()), 0.1f,
        100.0f);
  } else {
    mainView->data.mat_projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -20.0f,
                                        20.0f);
  }

  mainView->data.mat_model.setToIdentity();
  mainView->data.mat_view.setToIdentity();
  mainView->data.mat_view.translate(
      0.0f, 0.0f, -3.0f);  // отступаем по оси z на 3 для начала последующих
                           // расчетов загружаем проекцию в шейдер
  this->shader_program.setUniformValue("mat_projection",
                                       mainView->data.mat_projection);
  // прокидываем матрицы
  this->shader_program.setUniformValue("mat_view", mainView->data.mat_view);
  mainView->data.mat_model.translate(
      mainView->data.move_vector);  // перемещаем в новое положение
  mainView->data.mat_model.rotate(
      mainView->data.rotate_vector.x(), 1.0f, 0.0f,
      0.0f);  // поворачиваем на заданный угол вокруг заданной оси
  mainView->data.mat_model.rotate(mainView->data.rotate_vector.y(), 0.0f, 1.0f,
                                  0.0f);
  mainView->data.mat_model.rotate(mainView->data.rotate_vector.z(), 0.0f, 0.0f,
                                  1.0f);
  mainView->data.mat_model.scale(mainView->data.scale_factor);  //  масштабируем
  this->shader_program.setUniformValue("mat_model", mainView->data.mat_model);
  // прокидываем цвет
  this->shader_program.setUniformValue(
      "ourColor", mainView->data.model_color.redF(),
      mainView->data.model_color.greenF(), mainView->data.model_color.blueF(),
      mainView->data.model_color.alphaF());
  this->shader_program.setUniformValue("isVertex", false);

  // рисуем поверхности
  glEnable(GL_LINE_SMOOTH);
  // Line width > 1.0 is deprecated and not further supported in a core profile
  // https://www.reddit.com/r/opengl/comments/at1az3/gllinewidth_not_working/
  glLineWidth(mainView->data.thickness / 10.0f);  // толщина линий
  this->shader_program.setUniformValue("isDashed", mainView->data.isDashed);
  if (mainView->data.isFilled || !mainView->data.isDashed) {
    glDrawArrays(GL_TRIANGLES, 0, mainView->data.vertices.size());
  } else {
    this->shader_program.setUniformValue("u_resolution",
                                         static_cast<float>(width()),
                                         static_cast<float>(height()));
    this->shader_program.setUniformValue("u_dashSize", 10.0f);
    this->shader_program.setUniformValue("u_gapSize", 10.0f);
    glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, mainView->data.vertices.size());
  }
  // рисуем вершины
  if (mainView->data.vertex_show) {
    glPointSize(
        mainView->data.vertex_size);  // размер точки, по идее надо менять при
                                      // масштабировании вдоль оси z
    this->shader_program.setUniformValue("ourColor",
                                         mainView->data.vertex_color.redF(),
                                         mainView->data.vertex_color.greenF(),
                                         mainView->data.vertex_color.blueF(),
                                         mainView->data.vertex_color.alphaF());
    this->shader_program.setUniformValue("isVertex", true);
    this->shader_program.setUniformValue("shape", mainView->data.vertex_shape);
    glDrawArrays(GL_POINTS, 0, mainView->data.vertices.size());
  }
}

void s21::MyQOpenGLWidget::mousePressEvent(QMouseEvent* event) {
  Qt::MouseButton button = event->button();
  if (button == Qt::LeftButton) {
    this->leftButtonPress = true;
    this->leftButtonPoint = event->pos();
  } else if (button == Qt::RightButton) {
    this->rightButtonPress = true;
    this->rightButtonPoint = event->pos();
  }
}

void s21::MyQOpenGLWidget::mouseReleaseEvent(QMouseEvent* event) {
  Qt::MouseButton button = event->button();
  if (button == Qt::LeftButton)
    this->leftButtonPress = false;
  else if (button == Qt::RightButton)
    this->rightButtonPress = false;
}

void s21::MyQOpenGLWidget::mouseMoveEvent(QMouseEvent* event) {
  if (this->leftButtonPress) {
    emit leftButtonMove(event->pos() - this->leftButtonPoint);
    this->leftButtonPoint = event->pos();
  }

  if (this->rightButtonPress) {
    emit rightButtonMove(event->pos() - this->rightButtonPoint);
    this->rightButtonPoint = event->pos();
  }
}

void s21::MyQOpenGLWidget::wheelEvent(QWheelEvent* event) {
  emit this->mouseScroll(event->angleDelta().y());
}

void s21::MyQOpenGLWidget::saveBmpImage(const QString& filename) {
  this->grabFramebuffer().save(filename, "bmp");
}

void s21::MyQOpenGLWidget::saveJpegImage(const QString& filename) {
  this->grabFramebuffer().save(filename, "jpeg");
}
