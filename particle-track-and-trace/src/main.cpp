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

  // Control area.
  // QDockWidget controlDock;
  // mainWindow.addDockWidget(Qt::LeftDockWidgetArea, &controlDock);
  //
  // QLabel controlDockTitle("Control Dock");
  // controlDockTitle.setMargin(20);
  // controlDock.setTitleBarWidget(&controlDockTitle);
  //
  // QPointer<QVBoxLayout> dockLayout = new QVBoxLayout();
  // QWidget layoutContainer;
  // layoutContainer.setLayout(dockLayout);
  // controlDock.setWidget(&layoutContainer);
  //
  // QPushButton randomizeButton;
  // randomizeButton.setText("Randomize");
  // dockLayout->addWidget(&randomizeButton);

  // Render area.
  // QPointer<QVTKOpenGLNativeWidget> vtkRenderWidget =
  //     new QVTKOpenGLNativeWidget();
  // mainWindow.setCentralWidget(vtkRenderWidget);

  // VTK part.
  // vtkNew<vtkGenericOpenGLRenderWindow> window;
  // vtkRenderWidget->setRenderWindow(window.Get());

  cout << "Reading data..." << endl;
  string dataPath = "../../../../data";
  shared_ptr<UVGrid> uvGrid = make_shared<UVGrid>(dataPath);
  auto kernelRK4 = make_unique<RK4AdvectionKernel>(uvGrid);
  auto kernelRK4BoundaryChecked = make_unique<SnapBoundaryConditionKernel>(std::move(kernelRK4), uvGrid);
  cout << "Starting vtk..." << endl;

  auto l = new LGlyphLayer(uvGrid, std::move(kernelRK4BoundaryChecked));
  // l->spoofPoints();
  l->setDt(DT);
  // TODO: implement feature to call this function on widget
  // l->cycleGlyphStyle();

  unique_ptr<Program> program = make_unique<Program>();
  program->addLayer(new BackgroundImage(dataPath + "/map_qgis_1035.png"));
  // TODO: implement feature to cycle between layers thru QT
  program->addLayer(new EGlyphLayer(uvGrid));
  program->addLayer(new EColLayer(uvGrid));
  program->addLayer(l);

  program->setupInteractions();

  // program->render();

  // Setup initial status.
  // std::mt19937 randEng(0);
  // ::Randomize(sphere, mapper, window, randEng);

  // connect the buttons
  // QObject::connect(&randomizeButton, &QPushButton::released,
                   // [&]() { ::Randomize(sphere, mapper, window, randEng); });

  mainWindow.show();
  return app.exec();
}

