#include <QApplication>
#include <QVTKOpenGLNativeWidget.h>
#include "QT/MainWindow.h"

int main(int argc, char* argv[]) {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  QApplication app(argc, argv);

  MainWindow w;
  w.resize(1200, 900);

  w.show();
  return app.exec();
}

