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
  this->setupTechniques();
}

MainWindow::~MainWindow() { 
  delete ui; 
}


void MainWindow::setupTechniques() {
  cout << "Reading data..." << endl;
  string dataPath = "../../../../data";
  shared_ptr<UVGrid> uvGrid = make_shared<UVGrid>(dataPath);

  // initialize techniques
  Program *program = this->ui->program;
  auto technique1 = new Technique(program->getCamera());
  auto technique2 = new Technique(program->getCamera());
  
  // add bg layer
  auto bg = new BackgroundImage(dataPath + "/map_qgis_1035.png");
  technique1->addLayer(bg);
  technique2->addLayer(bg);

  // add Euler layers
  technique1->addLayer(new EColLayer(uvGrid));
  technique2->addLayer(new EGlyphLayer(uvGrid));

  // setup LGlyphLayer
  auto kernelRK4 = make_unique<RK4AdvectionKernel>(uvGrid);
  auto kernelRK4BoundaryChecked = make_unique<SnapBoundaryConditionKernel>(std::move(kernelRK4), uvGrid);
  auto lGlyph = new LGlyphLayer(uvGrid, std::move(kernelRK4BoundaryChecked));
  lGlyph->setDt(3600);

  technique1->addLayer(lGlyph);
  // technique2->addLayer(new LColLayer(uvGrid)); // TODO: add LColLayer 
  technique2->addLayer(lGlyph);

  cout << technique1->numberOfLayers() << endl;

  program->addTechnique(technique1);
  program->addTechnique(technique2);

  program->setActiveTechnique(0);

  // TODO: implement feature to call this function on widget
  // l->spoofPoints();
  // l->cycleGlyphStyle();
  

}


/* --------------------------------------------------------------------
 * +                    QTWidget callbacks                            +
 * --------------------------------------------------------------------*/


void MainWindow::on_FirstButton_clicked(bool checked) {
  if (checked) {
    cout << "clicked button 1!" << endl;
  }
}


void MainWindow::on_SecondButton_clicked(bool checked) {
  if (checked) {
    cout << "clicked button 2!" << endl;

  }
}


void MainWindow::on_ComplementaryButton_clicked(bool checked) {
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


void MainWindow::on_IregularlySubsampledButton_clicked(bool checked) {
  if (checked) {

  }}


