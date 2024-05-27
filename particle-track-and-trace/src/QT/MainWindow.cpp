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
#include "../layers/LColLayer.h"
#include "../Program.h"
#include "../advection/UVGrid.h"
#include "../advection/kernel/RK4AdvectionKernel.h"
#include "../advection/kernel/SnapBoundaryConditionKernel.h"
#include "../layers/enums.h"

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

  auto kernelRK4_2 = make_unique<RK4AdvectionKernel>(uvGrid);
  auto kernelRK4BoundaryChecked_2 = make_unique<SnapBoundaryConditionKernel>(std::move(kernelRK4_2), uvGrid);
  auto lCol = new LColLayer(uvGrid, std::move(kernelRK4BoundaryChecked_2));
  lGlyph->setDt(3600);
  lCol->setDt(3600);

  technique1->addLayer(lGlyph);
  technique2->addLayer(lCol); 

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
    ui->program->setActiveTechnique(COLGLYPH);
  }
}


void MainWindow::on_SecondButton_clicked(bool checked) {
  if (checked) {
    ui->program->setActiveTechnique(GLYPHCOL);
  }
}


void MainWindow::on_ComplementaryButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setColorMode(COMPLEMENTARY);
    }
  }
}


void MainWindow::on_ContrastingButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setColorMode(CONTRASTING);
    }
  }
}


void MainWindow::on_MonochromaticButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setColorMode(MONOCHROMATIC);
    }
  }
}


void MainWindow::on_SaturateButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setSaturationMode(SATURATED);
    }
  }
}


void MainWindow::on_DesaturateButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setSaturationMode(DESATURATED);
    }
  }
}


void MainWindow::on_CircleButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setGlyphStyle(CIRCLE);
    }
  }
}


void MainWindow::on_TriangleButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setGlyphStyle(TRIANGLE);
    }
  }
}


void MainWindow::on_SquareButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setGlyphStyle(SQUARE);
    }
  }
}


void MainWindow::on_HexagonButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setGlyphStyle(HEXAGON);
    }
  }
}


void MainWindow::on_FullySampledButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setSamplingMode(FULLYSAMPLED);
    }
  }
}


void MainWindow::on_RegularlySubsampledButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setSamplingMode(REGULARLYSUBSAMPLED);
    }
  }
}


void MainWindow::on_IregularlySubsampledButton_clicked(bool checked) {
  if (checked) {
    for (Technique *t : ui->program->getTechniques()) {
      t->setSamplingMode(IRREGULARLYSUBSAMPLED);
    }
  }
}


