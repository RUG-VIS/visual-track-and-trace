#include <QApplication>
#include <QVTKOpenGLNativeWidget.h>
#include "QT/MainWindow.h"

// TODO: add a widget to act as legend. This is fourfold: particle-age, LCol-density, Lcol-age, ECol-direction ECol-strength?
// TODO: add text widget showing simulation time (in days/weeks/months from 0).
// TODO: make Lagrangian Layers share one vtkPoints for seemless technique switching
// TODO: make LColLayer use a modified spawnpointCallback to spawn multiple particles per interaction
// TODO: add text gui explaining the above?
// TODO: yoink Robin's isNearestNeighbourZero function to improve beaching
// TODO: add a button to reset the simulation - set time=0 and reset points array of particles.
// FIXME: go over each function and add const where appropriate.

int main(int argc, char* argv[]) {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  QApplication app(argc, argv);

  MainWindow w;
  w.resize(1200, 900);

  w.show();
  return app.exec();
}

