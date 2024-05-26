#include "MainWindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <vtkDataSetReader.h>

#include "../layers/BackgroundImage.h"
#include "../layers/EColLayer.h"
#include "../layers/EGlyphLayer.h"
#include "../layers/LGlyphLayer.h"
#include "../Program.h"
#include "../advection/UVGrid.h"
#include "../advection/kernel/RK4AdvectionKernel.h"
#include "../advection/kernel/SnapBoundaryConditionKernel.h"
using namespace std;

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

  cout << "Reading data..." << endl;
  string dataPath = "../../../../data";
  shared_ptr<UVGrid> uvGrid = make_shared<UVGrid>(dataPath);
  auto kernelRK4 = make_unique<RK4AdvectionKernel>(uvGrid);
  auto kernelRK4BoundaryChecked = make_unique<SnapBoundaryConditionKernel>(std::move(kernelRK4), uvGrid);
  cout << "Starting vtk..." << endl;

  auto l = new LGlyphLayer(uvGrid, std::move(kernelRK4BoundaryChecked));
  // l->spoofPoints();
  l->setDt(3600);
  // TODO: implement feature to call this function on widget
  // l->cycleGlyphStyle();

  Program *program = this->ui->getProgram();
  program->addLayer(new BackgroundImage(dataPath + "/map_qgis_1035.png"));
  // TODO: implement feature to cycle between layers thru QT
  program->addLayer(new EGlyphLayer(uvGrid));
  program->addLayer(new EColLayer(uvGrid));
  program->addLayer(l);

  program->setupInteractions();
}

MainWindow::~MainWindow() { 
  delete ui; 
}


void MainWindow::on_FirstButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_SecondButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_ComplentaryButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_ContrastingButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_MonochromaticButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_SaturateButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_DesaturateButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_CircleButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_TriangleButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_SquareButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_HexagonButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_FullySampledButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_RegularlySubsampledButton_clicked(bool checked) {
  if (checked) {

  }
}


void MainWindow::on_IregularlySampledButton_clicked(bool checked) {
  if (checked) {

  }}


