#include <QApplication>
#include <QVTKOpenGLNativeWidget.h>
#include "QT/MainWindow.h"

// TODO: make Lagrangian Layers share one vtkPoints for seemless technique swiching
// TODO: yoink Robin's isNearestNeighbourZero function to improve beaching
// TODO: add a button to reset the simulation - set time=0 and reset points array of particles.
// FIXME: go over each function and add const where appropriate.
// FIXME: rotate ECol monochromatic layer

// COULDHAVE: the Legends are just statically rendered images; ideally these would be created along with the luts and then displayed accordingly.

int main(int argc, char* argv[]) {
  srand (static_cast <unsigned> (time(0)));

  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  QApplication app(argc, argv);

  MainWindow w;
  w.resize(1200, 900);

  w.show();
  return app.exec();
}

