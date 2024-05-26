#include <netcdf>
#include <vtkActor2D.h>
#include <vtkNamedColors.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkProperty2D.h>
#include <vtkRenderer.h>
#include <vtkVertexGlyphFilter.h>
#include <memory>

#include "layers/BackgroundImage.h"
#include "layers/EColLayer.h"
#include "layers/EGlyphLayer.h"
#include "layers/LGlyphLayer.h"
#include "Program.h"
#include "advection/UVGrid.h"
#include "advection/kernel/RK4AdvectionKernel.h"
#include "advection/kernel/SnapBoundaryConditionKernel.h"

#include <QApplication>
#include <QVTKOpenGLNativeWidget.h>
#include <QDockWidget>
#include <QGridLayout>
#include <vtkGenericOpenGLRenderWindow.h>
#include <QLabel>
#include <QMainWindow>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>
#include "QT/MainWindow.h"

using namespace std;

#define DT 60 * 60 // 60 sec/min * 60 mins

int main(int argc, char* argv[]) {
  QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());

  QApplication app(argc, argv);

  // Main window.
  MainWindow mainWindow;
  mainWindow.resize(1200, 900);

  mainWindow.show();
  return app.exec();
}

